#include "Shop.h"

#include "../Player/Player.h"
#include <iostream>

Shop::Shop(double x, double y, double drawWidth, double drawHeight, double rotateAngle, double speed, double collideWidth, double collideHeight, const std::string& textureName2D, double interactionWidth, double interactionHeight)
	: Entity(x, y, drawWidth, drawHeight, rotateAngle, speed)
	, CollidableEntity(x, y, drawWidth, drawHeight, rotateAngle, speed, collideWidth, collideHeight)
	, TexturableEntity(x, y, drawWidth, drawHeight, rotateAngle, speed, textureName2D)
	, InteractiveEntity(x, y, drawWidth, drawHeight, rotateAngle, speed, interactionWidth, interactionHeight)
{

}

void Shop::onCollide(CollidableEntity& other, glm::vec2 overlap)
{
	// Laptop-ul de magazin nu se misca din loc in caz de coliziune.
}

bool Shop::isInInteraction()
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

void Shop::onInteraction()
{
	if (Player::get().getEnterShopUsed())
	{
		Player::get().enterShop();
		Player::get().enterShopButtonReleased();
	}
}

