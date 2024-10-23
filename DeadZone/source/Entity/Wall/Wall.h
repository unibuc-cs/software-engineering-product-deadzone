#pragma once

#include "../CollidableEntity.h"
#include "../TexturableEntity.h"

class Wall : public virtual CollidableEntity, public virtual TexturableEntity
{
protected:

public:

	Wall(double x, double y, double drawWidth, double drawHeight, double rotateAngle, double speed, double collideWidth, double collideHeight, const std::string& textureName2D);
	void onCollide(CollidableEntity& other, glm::vec2 overlap) override;
	virtual ~Wall();
};