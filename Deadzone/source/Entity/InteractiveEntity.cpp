#include "InteractiveEntity.h"

InteractiveEntity::InteractiveEntity(double x, double y, double drawWidth, double drawHeight, double rotateAngle, double speed, double interactionWidth, double interactionHeight)
	: Entity(x, y, drawWidth, drawHeight, rotateAngle, speed)
	, interactionWidth(interactionWidth), interactionHeight(interactionHeight)
{

}
