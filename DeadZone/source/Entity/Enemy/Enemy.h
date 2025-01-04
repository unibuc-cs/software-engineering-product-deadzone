#pragma once

#include <vector>
#include <queue>

#include "../Human/Human.h"
#include "../AIEntity.h"

class Enemy : public virtual Human, public virtual AIEntity
{
protected:

	std::vector<std::vector<bool>> blockedCell;
	std::vector<std::vector<int>> cellDistance;
	std::priority_queue < std::pair<int, std::pair<int, int>>, std::vector < std::pair<int, std::pair<int, int>>>, std::greater<std::pair<int, std::pair<int, int>>>> pq;
	std::vector<std::pair<int, int>> visitedCells;

	double rotateSpeed;

	double probToChangeDir;

	std::pair<double, double> currentTarget;
	std::pair<double, double> nextTarget;

	double movingOffsetSize;
	double movingOffsetSpeed;

	bool isMoving;

	int goldOnKill;

	double attackDamage;
	double attackRadius;

	float nearestPlayerX;
	float nearestPlayerY;

	static const float INF;

public:

	Enemy(double x, double y, double drawWidth, double drawHeight, double rotateAngle, double speed, double collideWidth, double collideHeight, const std::map<AnimatedEntity::EntityStatus, std::string>& animationsName2D, const std::vector<EntityStatus>& statuses, double health, double rotateSpeed, double attackDamage, double attackRadius, double armor = 0.0);
	~Enemy();

	virtual void pathFindingTarget() override;
	virtual void onTargetReach() override;
	virtual bool nearTarget() override;

	virtual void update() override;

	virtual void draw() override;

	inline double getAttackDamage() const { return this->attackDamage; }
	inline double getAttackRadius() const { return this->attackRadius; }
	inline int getGoldOnKill() const { return goldOnKill; }
};
