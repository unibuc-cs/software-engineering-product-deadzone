#pragma once

#include "../TexturableEntity.h"
#include "../InteractiveEntity.h"

class PickUp : public virtual TexturableEntity, public virtual InteractiveEntity
{
protected:


public:

	PickUp(double x, double y, double drawWidth, double drawHeight, double rotateAngle, double speed, const std::string& textureName2D, double interactionWidth, double interactionHeight);
	virtual ~PickUp();

	std::string convertToJson(int identation = 0, bool showSuper = true);
	void setFieldValue(const std::string field, const std::string value);
	void modifyFromJson(const std::string& str, int startAt = 0);
};