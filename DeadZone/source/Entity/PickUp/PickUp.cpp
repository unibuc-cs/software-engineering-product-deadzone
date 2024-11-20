#include "PickUp.h"


PickUp::PickUp(double x, double y, double drawWidth, double drawHeight, double rotateAngle, double speed, const std::string& textureName2D, double interactionWidth, double interactionHeight)
	: TexturableEntity(x, y, drawWidth, drawHeight, rotateAngle, speed, textureName2D)
	, InteractiveEntity(x, y, drawWidth, drawHeight, rotateAngle, speed, interactionWidth, interactionHeight)
{

}

PickUp::~PickUp()
{

}

std::string PickUp::convertToJson(int indetention, bool showSuper)
{
	std::string res, depth;
	while (indetention--) depth += "\t";
	res = depth + "[\n";
	if (showSuper) {
		TexturableEntity* auxT = this;
		res += depth + "\t\"TexturableEntity\" : " + auxT->convertToJson(depth.length() + 1, true);
		res[res.size() - 1] = ','; res += "\n";

		InteractiveEntity* auxI = this;
		res += depth + "\t\"InteractiveEntity\" : " + auxI->convertToJson(depth.length() + 1, false) + "\n";
	}
	else res += "\n";
	res += depth + "]\n";

	return res;
}

void PickUp::setFieldValue(const std::string field, const std::string value)
{
	if (field == "TexturableEntity") {
		TexturableEntity* aux = this;
		aux->modifyFromJson(value, 0);
	}

	if (field == "InteractiveEntity") {
		InteractiveEntity* aux = this;
		aux->modifyFromJson(value, 0);
	}

}

//Assumes that it is always an valid format
void PickUp::modifyFromJson(const std::string& str, int startAt)
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
			if (field == "TexturableEntity" || field == "InteractiveEntity") {
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