#pragma once

#include "Entity.h"

#include <glm/vec2.hpp>

class CollidableEntity : public virtual Entity
{
protected:
	double collideWidth;
	double collideHeight;

	static const double EPS;

	bool collisionActive;

public:

	CollidableEntity(double x, double y, double drawWidth, double drawHeight, double rotateAngle, double speed, double collideWidth, double collideHeight);
	double getCollideWidth() const { return this->collideWidth; }
	double getCollideHeight() const { return this->collideHeight; }
	glm::vec2 isInCollision(const CollidableEntity& other) const;

	virtual void onCollide(CollidableEntity& other, glm::vec2 overlap) = 0; // Functia aceasta rezolva coliziunea doar din perspectiva lui (this), trebuie apelata si din perspectiva lui other.
	virtual ~CollidableEntity();

	inline void setCollisionActive(bool collisionActive) { this->collisionActive = collisionActive; }
	inline bool getCollisionActive() const { return this->collisionActive; }
};

