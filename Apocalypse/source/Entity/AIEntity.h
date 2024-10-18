#pragma once

#include <vector>
#include <random>

#include "Entity.h"

class AIEntity : public virtual Entity
{
protected:
	static const double EPSILON_ANGLE;
	static const double EPSILON_MOVEMENT;
	static const std::vector<std::pair<int, int>> neighbors;
	static const std::vector<std::pair<int, int>> extendedNeighbors;
	static const std::vector<double> neighborsAngles;
	static const std::vector<double> extendedNeighborsAngles;

	double nearTargetRadius;

public:

	AIEntity(double x, double y, double drawWidth, double drawHeight, double rotateAngle, double speed);

	virtual void pathFindingTarget() = 0;
	virtual void onTargetReach() = 0;
	virtual bool nearTarget() = 0;

	virtual ~AIEntity();
};