#pragma once

#include "../TexturableEntity.h"
#include "../InteractiveEntity.h"

class PickUp : public virtual TexturableEntity, public virtual InteractiveEntity
{
protected:


public:

	PickUp(double x, double y, double drawWidth, double drawHeight, double rotateAngle, double speed, const std::string& textureName2D, double interactionWidth, double interactionHeight);
	virtual ~PickUp();
};