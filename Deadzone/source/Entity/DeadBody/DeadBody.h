#pragma once

#include "../AnimatedEntity.h"

#include <glm/glm.hpp>
#include <glm/vec3.hpp>

class DeadBody : public virtual AnimatedEntity
{
protected:

public:
	DeadBody(double x, double y, double drawWidth, double drawHeight, double rotateAngle, double speed, const std::map<EntityStatus, std::string>& animationsName2D, const std::vector<EntityStatus>& statuses, const glm::vec3& outfitColor = glm::vec3(1.0f, 1.0f, 1.0f));
	virtual ~DeadBody();

	virtual void draw() override;

private:
	glm::vec3 outfitColor;
};

