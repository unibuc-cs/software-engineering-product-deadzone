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

	std::string convertToJson(int identation = 0, bool showSuper = true);
	void setFieldValue(const std::string field, const std::string value);
	void modifyFromJson(const std::string& str, int startAt = 0);
};

