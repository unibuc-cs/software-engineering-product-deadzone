#pragma once

#include "Bullet.h"

#include <string>

class ThrownGrenade : public virtual Bullet
{
protected:

	double timeUntilExplosion;
	double timeThrown;
	double explosionDamage;
	static const double maximumSizeIncreaseAnimation;
	double originalDrawWidth;
	double originalDrawHeight;
	double originalCollideWidth;
	double originalCollideHeight;

	double explosionScale;
	double explosionDuration;

public:
	ThrownGrenade(double x, double y, double drawWidth, double drawHeight, double rotateAngle, double speed, double collideWidth, double collideHeight, const std::string& textureName2D, double damage, double timeUntilExplosion, double explosionDamage, double explosionScale, double explosionDurating);
	virtual ~ThrownGrenade();

	virtual void update() override;

	virtual void onCollide(CollidableEntity& other, glm::vec2 overlap) override;
};