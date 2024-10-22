#pragma once

#include "Entity.h"

class InteractiveEntity : public virtual Entity
{
protected:
	double interactionWidth;
	double interactionHeight;

public:
	InteractiveEntity(double x, double y, double drawWidth, double drawHeight, double rotateAngle, double speed, double interactionWidth, double interactionHeight);
	virtual ~InteractiveEntity() = default;

	virtual bool isInInteraction() = 0;
	virtual void onInteraction() = 0;
};

