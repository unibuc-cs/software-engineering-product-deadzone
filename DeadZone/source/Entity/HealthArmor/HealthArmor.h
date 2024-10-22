#pragma once

#include "../PickUp/PickUp.h"

class HealthArmor : public virtual PickUp
{
protected:
	double addedArmor;

public:

	HealthArmor(double x, double y, double drawWidth, double drawHeight, double rotateAngle, double speed, const std::string& textureName2D, double interactionWidth, double interactionHeight, double addedArmor);

	virtual bool isInInteraction() override;
	virtual void onInteraction() override;

	virtual ~HealthArmor();
};

