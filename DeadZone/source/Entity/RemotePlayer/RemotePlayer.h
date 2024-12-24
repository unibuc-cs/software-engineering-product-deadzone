#pragma once

#include <glm/glm.hpp>

#include "../Human/Human.h"

class RemotePlayer : public virtual Human
{
public:
	// Constructor
	RemotePlayer(double x, double y, double drawWidth, double drawHeight, double rotateAngle, double speed, double collideWidth, double collideHeight, const std::map<AnimatedEntity::EntityStatus, std::string>& animationsName2D, const std::vector<EntityStatus>& statuses, double runningSpeed, double health = 100.0, double stamina = 100.0, double armor = 0.0);

	// Destructor
	~RemotePlayer();

	virtual void draw() override;

private:
	glm::vec3 outfitColor;
};
