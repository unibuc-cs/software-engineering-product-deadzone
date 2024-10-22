#pragma once

#include "../CollidableEntity.h"
#include "../TexturableEntity.h"
#include "../InteractiveEntity.h"

class Shop : public virtual CollidableEntity, public virtual TexturableEntity, public virtual InteractiveEntity
{
protected:

public:
	Shop(double x, double y, double drawWidth, double drawHeight, double rotateAngle, double speed, double collideWidth, double collideHeight, const std::string& textureName2D, double interactionWidth, double interactionHeight);
	void onCollide(CollidableEntity& other, glm::vec2 overlap) override;
	virtual ~Shop() = default;

	bool isInInteraction() override;
	void onInteraction() override;
};

