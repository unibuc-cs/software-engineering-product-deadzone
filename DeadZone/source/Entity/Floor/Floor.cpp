#include "Floor.h"


Floor::Floor(double x, double y, double drawWidth, double drawHeight, double rotateAngle, double speed, const std::string& textureName2D) :
	Entity(x, y, drawWidth, drawHeight, rotateAngle, speed),
	TexturableEntity(x, y, drawWidth, drawHeight, rotateAngle, speed, textureName2D)
{

}


Floor::~Floor()
{

}