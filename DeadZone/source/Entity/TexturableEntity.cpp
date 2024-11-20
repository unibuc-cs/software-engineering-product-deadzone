#include "TexturableEntity.h"
#include "../Renderer/SpriteRenderer.h"
#include "../ResourceManager/ResourceManager.h"
#include "../Camera/Camera.h"
#include <iostream>
TexturableEntity::TexturableEntity(double x, double y, double drawWidth, double drawHeight, double rotateAngle, double speed, const std::string& textureName2D)
	: Entity(x, y, drawWidth, drawHeight, rotateAngle, speed), textureName2D(textureName2D)
{

}

void TexturableEntity::draw()
{
	SpriteRenderer::get().draw(ResourceManager::getShader("sprite"),
		ResourceManager::getTexture(this->textureName2D),
		Camera::get().screenPosition(this->x, this->y),
		Camera::get().screenSize(this->drawWidth, this->drawHeight),
		this->rotateAngle);
}

TexturableEntity::~TexturableEntity()
{

}

std::string TexturableEntity::convertToJson(int indetention, bool showSuper)
{
	std::string res, depth;
	while (indetention--) depth += "\t";
	res = depth + "[\n";
	res += depth + "\t\"textureName2D\" : " + this->textureName2D;
	if (showSuper) {
		Entity* aux = this;
		res += ",\n";
		res += depth + "\t\"Entity\" : " + aux->convertToJson(depth.length() + 1) + "\n";
	}
	else res += "\n";
	res += depth + "]\n";

	return res;
}

void TexturableEntity::setFieldValue(const std::string field, const std::string value)
{
	if (field == "textureName2D") this->textureName2D = value;
	if (field == "Entity") {
		Entity* aux = this;
		aux->modifyFromJson(value, 0);
	}
	
}

//Assumes that it is always an valid format
void TexturableEntity::modifyFromJson(const std::string& str, int startAt)
{
	while (true) {
		if (startAt >= str.length()) throw std::invalid_argument("Bad json format: " + str);
		if (str[startAt] == ']') break;

		if (str[startAt] == '\"') {
			//Assumes that it always has the form "field" : value,
			std::string field;
			startAt++;
			while (startAt < str.length() && str[startAt] != '\"') {
				field += str[startAt];
				startAt++;
			}

			std::string value;
			startAt += 4;
			if (field == "Entity") {
				while (startAt < str.length() && str[startAt] != ']') {
					value += str[startAt];
					startAt++;
				}
				value += "]";
			}
			else {
				while (startAt < str.length() && str[startAt] != ',' && str[startAt] != ']') {
					value += str[startAt];
					startAt++;
				}
			}

			this->setFieldValue(field, value);
		}
		else startAt++;
	}
}