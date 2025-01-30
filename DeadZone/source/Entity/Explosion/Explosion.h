#pragma once

#include "../CollidableEntity.h"
#include "../AnimatedEntity.h"

class Explosion : public virtual CollidableEntity, public virtual AnimatedEntity
{
protected:
	double explosionStarted;
	double explosionDuration;
	double explosionDamage;

	std::string owner;

public:
	Explosion(double x, double y, double drawWidth, double drawHeight, double rotateAngle, double speed, double collideWidth, double collideHeight, const std::map<AnimatedEntity::EntityStatus, std::string>& animationsName2D, const std::vector<EntityStatus>& statuses, double explosionDuration, double explosionDamage, const std::string& owner);
	virtual ~Explosion();

	virtual void update() override;
	virtual void onCollide(CollidableEntity& other, glm::vec2 overlap);

	// Getters
	inline double getExplosionStarted() const { return this->explosionStarted; }
	inline double getExplosionDuration() const { return this->explosionDuration; }
	inline double getExplosionDamage() const { return this->explosionDamage; }
	inline std::string getOwner() const { return this->owner; }
};

