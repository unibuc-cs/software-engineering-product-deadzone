#pragma once

#include "Enemy.h"

class EnemyFactory
{
private:

	EnemyFactory() = delete;
	~EnemyFactory() = delete;
	EnemyFactory(const EnemyFactory& other) = delete;
	EnemyFactory& operator= (const EnemyFactory& other) = delete;
	EnemyFactory(const EnemyFactory&& other) = delete;
	EnemyFactory& operator= (const EnemyFactory&& other) = delete;

public:

	static std::shared_ptr<Enemy> getDefaultEnemy(double spawnX, double spawnY);
	static std::shared_ptr<Enemy> getWeakFastEnemy(double spawnX, double spawnY);
	static std::shared_ptr<Enemy> getStrongSlowEnemy(double spawnX, double spawnY);
	static std::shared_ptr<Enemy> getStrongFastEnemy(double spawnX, double spawnY);
};