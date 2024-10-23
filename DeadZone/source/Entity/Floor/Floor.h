#pragma once

#include "../TexturableEntity.h"
#include "../CollidableEntity.h"

class Floor : public virtual TexturableEntity
{
protected:

public:

	Floor(double x, double y, double drawWidth, double drawHeight, double rotateAngle, double speed, const std::string& textureName2D);
	virtual ~Floor();
};