#pragma once

#include <vector>
#include <string>
#include <map>

#include "Entity.h"

#include "../GlobalClock/GlobalClock.h"

class AnimatedEntity : public virtual Entity
{
private:
	friend class MainMenu;

public:
	static enum class EntityStatus
	{
		IDLE,
		WALKING,
		RUNNING,
		TIRED,
		DYING,
		OPENED,
		HOVERED,
		CLICKED,

		ARMS_HOLDING_GRENADE,
		ARMS_HOLDING_KNIFE,
		ARMS_HOLDING_PISTOL,
		ARMS_HOLDING_SHOTGUN,
		ARMS_HOLDING_AK47,
		ARMS_HOLDING_M4,
		ARMS_MOVING_AHEAD,
		ARMS_MOVING_AROUND_WALKING,
		ARMS_MOVING_AROUND_RUNNING,
		ARMS_NOT,
		ARMS_RELOADING_PISTOL,
		ARMS_RELOADING_SHOTGUN,
		ARMS_RELOADING_AK47,
		ARMS_RELOADING_M4,
		ARMS_USING_GRENADE,
		ARMS_USING_KNIFE,
		ARMS_USING_PISTOL,
		ARMS_USING_SHOTGUN,
		ARMS_USING_FIST,
		ARMS_USING_AK47,
		ARMS_USING_M4,
		ARMS_STAYING_AHEAD,

		BODY_IDLE,

		HEAD_ANGRY,
		HEAD_IDLE,
		HEAD_SATISFIED,
		HEAD_TIRED,

		LEGS_MOVING_AROUND,
		LEGS_NOT,

		DEAD_HUMAN,
	};

protected:
	std::vector<EntityStatus> statuses;
	std::vector<double> timesSinceStatuses;

	std::map<EntityStatus, std::string> animationsName2D;

public:
	AnimatedEntity(double x, double y, double drawWidth, double drawHeight, double rotateAngle, double speed, const std::map<EntityStatus, std::string>& animationsName2D, const std::vector<EntityStatus>& statuses);
	virtual ~AnimatedEntity();

	void draw() override;
	
	void updateStatus(EntityStatus newStatus, int index = 0);

	// Getters
	double getTimeSinceStatus(int index = 0) const { return this->timesSinceStatuses[index]; };
	EntityStatus getStatus(int index = 0) const { return this->statuses[index]; }
	inline std::vector<EntityStatus> getStatuses() const { return statuses; }

	// Setters
	void setTimeSinceStatus(double timeSinceStatus, int index = 0) { this->timesSinceStatuses[index] = timeSinceStatus; }
};

