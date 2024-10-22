#include "PickUp.h"


PickUp::PickUp(double x, double y, double drawWidth, double drawHeight, double rotateAngle, double speed, const std::string& textureName2D, double interactionWidth, double interactionHeight)
	: TexturableEntity(x, y, drawWidth, drawHeight, rotateAngle, speed, textureName2D)
	, InteractiveEntity(x, y, drawWidth, drawHeight, rotateAngle, speed, interactionWidth, interactionHeight)
{

}

PickUp::~PickUp()
{

}