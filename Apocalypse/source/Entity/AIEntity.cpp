#include "AIEntity.h"

const double AIEntity::EPSILON_ANGLE = 7.5;
const double AIEntity::EPSILON_MOVEMENT = 0.3;
const std::vector<std::pair<int, int>> AIEntity::neighbors = { {0, -1}, {0, 1}, {-1, 0}, {1, 0} };
const std::vector<std::pair<int, int>> AIEntity::extendedNeighbors = { {-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1} };

AIEntity::AIEntity(double x, double y, double drawWidth, double drawHeight, double rotateAngle, double speed)
	: Entity(x, y, drawWidth, drawHeight, rotateAngle, speed)
	, nearTargetRadius(0.0)
{

}

AIEntity::~AIEntity()
{

}

