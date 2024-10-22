#pragma once

#include <string>

#include <glm/glm.hpp>

#include "../CollidableEntity.h"
#include "../TexturableEntity.h"

class Bullet : public virtual CollidableEntity, public virtual TexturableEntity
{
protected:
	double damage;

public:
	Bullet(double x, double y, double drawWidth, double drawHeight, double rotateAngle, double speed, double collideWidth, double collideHeight, const std::string& textureName2D, double damage);
	virtual ~Bullet() = default;

	virtual void onCollide(CollidableEntity& other, glm::vec2 overlap) override;
	virtual void update() override;

	inline double getDamage() const { return this->damage; }
};

