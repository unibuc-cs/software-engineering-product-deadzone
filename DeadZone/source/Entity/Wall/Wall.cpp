#include "Wall.h"

Wall::Wall(double x, double y, double drawWidth, double drawHeight, double rotateAngle, double speed, double collideWidth, double collideHeight, const std::string& textureName2D)
	: Entity(x, y, drawWidth, drawHeight, rotateAngle, speed)
	, CollidableEntity(x, y, drawWidth, drawHeight, rotateAngle, speed, collideWidth, collideHeight)
	, TexturableEntity(x, y, drawWidth, drawHeight, rotateAngle, speed, textureName2D)
{

}

void Wall::onCollide(CollidableEntity& other, glm::vec2 overlap)
{
	// Un perete nu face nimic daca se afla in coliziune.
}

Wall::~Wall()
{

}