#include "HealthArmor.h"

#include "../Player/Player.h"

HealthArmor::HealthArmor(double x, double y, double drawWidth, double drawHeight, double rotateAngle, double speed, const std::string& textureName2D, double interactionWidth, double interactionHeight, double addedArmor)
	: Entity(x, y, drawWidth, drawHeight, rotateAngle, speed)
	, TexturableEntity(x, y, drawWidth, drawHeight, rotateAngle, speed, textureName2D)
	, InteractiveEntity(x, y, drawWidth, drawHeight, rotateAngle, speed, interactionWidth, interactionHeight)
	, PickUp(x, y, drawWidth, drawHeight, rotateAngle, speed, textureName2D, interactionWidth, interactionHeight)
	, addedArmor(addedArmor)
{

}

bool HealthArmor::isInInteraction()
{
	double left1 = this->getX() - this->interactionWidth / 2.0;
	double right1 = this->getX() + this->interactionWidth / 2.0;
	double left2 = Player::get().getX() - Player::get().getCollideWidth() / 2.0;
	double right2 = Player::get().getX() + Player::get().getCollideWidth() / 2.0;

	double down1 = this->getY() - this->interactionHeight / 2.0;
	double up1 = this->getY() + this->interactionHeight / 2.0;
	double down2 = Player::get().getY() - Player::get().getCollideHeight() / 2.0;
	double up2 = Player::get().getY() + Player::get().getCollideHeight() / 2.0;

	return std::max(left1, left2) <= std::min(right1, right2) && std::max(down1, down2) <= std::min(up1, up2);
}

void HealthArmor::onInteraction()
{
	// TODO:
}

HealthArmor::~HealthArmor()
{

}