#pragma once

#include "../CollidableEntity.h"
#include "../AnimatedEntity.h"

class Explosion : public virtual CollidableEntity, public virtual AnimatedEntity
{
protected:
	double explosionStarted;
	double explosionDuration;
	double explosionDamage;

public:

	Explosion(double x, double y, double drawWidth, double drawHeight, double rotateAngle, double speed, double collideWidth, double collideHeight, const std::map<AnimatedEntity::EntityStatus, std::string>& animationsName2D, const std::vector<EntityStatus>& statuses, double explosionDuration, double explosionDamage);
	virtual ~Explosion();

	virtual void update() override;
	virtual void onCollide(CollidableEntity& other, glm::vec2 overlap);
};

