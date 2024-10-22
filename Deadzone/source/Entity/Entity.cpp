#include "Entity.h"

Entity::Entity(double x, double y, double drawWidth, double drawHeight, double rotateAngle = 0.0, double speed = 0.0)
	: x(x), y(y), drawWidth(drawWidth), drawHeight(drawHeight), rotateAngle(rotateAngle), speed(speed)
	, deleteEntity(false)
{

}

void Entity::update()
{

}

Entity::~Entity()
{

}

