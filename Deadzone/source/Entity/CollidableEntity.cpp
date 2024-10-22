#include "CollidableEntity.h"

const double CollidableEntity::EPS = 0.0000001;

CollidableEntity::CollidableEntity(double x, double y, double drawWidth, double drawHeight, double rotateAngle, double speed, double collideWidth, double collideHeight)
	: Entity(x, y, drawWidth, drawHeight, rotateAngle, speed)
	, collideWidth(collideWidth), collideHeight(collideHeight)
	, collisionActive(true)
{

}

glm::vec2 CollidableEntity::isInCollision(const CollidableEntity& other) const
{
	double left1 = this->getX() - this->collideWidth / 2.0;
	double right1 = this->getX() + this->collideWidth / 2.0;
	double left2 = other.getX() - other.collideWidth / 2.0;
	double right2 = other.getX() + other.collideWidth / 2.0;

	double down1 = this->getY() - this->collideHeight / 2.0;
	double up1 = this->getY() + this->collideHeight / 2.0;
	double down2 = other.getY() - other.collideHeight / 2.0;
	double up2 = other.getY() + other.collideHeight / 2.0;

	return glm::vec2(std::max(0.0, std::min(right1, right2) - std::max(left1, left2)), std::max(0.0, std::min(up1, up2) - std::max(down1, down2)));
}

CollidableEntity::~CollidableEntity()
{

}

