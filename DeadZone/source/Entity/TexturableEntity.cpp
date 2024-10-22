#include "TexturableEntity.h"

#include "../Renderer/SpriteRenderer.h"
#include "../ResourceManager/ResourceManager.h"
#include "../Camera/Camera.h"

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