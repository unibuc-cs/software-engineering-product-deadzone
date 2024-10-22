#pragma once

#include <vector>
#include <string>
#include <map>

#include "../AnimatedEntity.h"

class BulletBlast : public virtual AnimatedEntity
{
public:
	BulletBlast(double x, double y, double drawWidth, double drawHeight, double rotateAngle, const std::map<AnimatedEntity::EntityStatus, std::string>& animationsName2D, const std::vector<AnimatedEntity::EntityStatus>& statuses);
	virtual ~BulletBlast() = default;

	void update() override;
};

