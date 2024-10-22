#include "EnemyFactory.h"

std::shared_ptr<Enemy> EnemyFactory::getDefaultEnemy(double spawnX, double spawnY)
{
	return std::shared_ptr<Enemy>(new Enemy(0.5 + spawnX, 0.5 + spawnY, 1.0, 1.0, 90.0, 5.0, 0.5, 0.5, std::map<AnimatedEntity::EntityStatus, std::string>
	{
		{ AnimatedEntity::EntityStatus::ARMS_STAYING_AHEAD, "enemy0ArmsStayingAhead" },
		{ AnimatedEntity::EntityStatus::ARMS_NOT, "enemy0ArmsNot" },
		{ AnimatedEntity::EntityStatus::BODY_IDLE, "enemy0BodyIdle" },
		{ AnimatedEntity::EntityStatus::HEAD_IDLE, "enemy0HeadIdle" },
		{ AnimatedEntity::EntityStatus::LEGS_MOVING_AROUND, "enemy0LegsMovingAround" },
		{ AnimatedEntity::EntityStatus::LEGS_NOT, "enemy0LegsNot" }
	},
	{
		AnimatedEntity::EntityStatus::LEGS_NOT,
		AnimatedEntity::EntityStatus::ARMS_STAYING_AHEAD,
		AnimatedEntity::EntityStatus::BODY_IDLE,
		AnimatedEntity::EntityStatus::HEAD_IDLE
	}, 100.0, 1000.0, 0.01, 0.75));
}

std::shared_ptr<Enemy> EnemyFactory::getWeakFastEnemy(double spawnX, double spawnY)
{
	return std::shared_ptr<Enemy>(new Enemy(0.5 + spawnX, 0.5 + spawnY, 1.0, 1.0, 90.0, 15.0, 0.5, 0.5, std::map<AnimatedEntity::EntityStatus, std::string>
	{
		{ AnimatedEntity::EntityStatus::ARMS_STAYING_AHEAD, "enemy0ArmsStayingAhead" },
		{ AnimatedEntity::EntityStatus::ARMS_NOT, "enemy0ArmsNot" },
		{ AnimatedEntity::EntityStatus::BODY_IDLE, "enemy0BodyIdle" },
		{ AnimatedEntity::EntityStatus::HEAD_IDLE, "enemy0HeadIdle" },
		{ AnimatedEntity::EntityStatus::LEGS_MOVING_AROUND, "enemy0LegsMovingAround" },
		{ AnimatedEntity::EntityStatus::LEGS_NOT, "enemy0LegsNot" }
	},
	{
		AnimatedEntity::EntityStatus::LEGS_NOT,
		AnimatedEntity::EntityStatus::ARMS_STAYING_AHEAD,
		AnimatedEntity::EntityStatus::BODY_IDLE,
		AnimatedEntity::EntityStatus::HEAD_IDLE
	}, 100.0, 1000.0, 0.1, 0.75));
}

std::shared_ptr<Enemy> EnemyFactory::getStrongSlowEnemy(double spawnX, double spawnY)
{
	return std::shared_ptr<Enemy>(new Enemy(0.5 + spawnX, 0.5 + spawnY, 1.0, 1.0, 90.0, 2.5, 0.5, 0.5, std::map<AnimatedEntity::EntityStatus, std::string>
	{
		{ AnimatedEntity::EntityStatus::ARMS_STAYING_AHEAD, "enemy0ArmsStayingAhead" },
		{ AnimatedEntity::EntityStatus::ARMS_NOT, "enemy0ArmsNot" },
		{ AnimatedEntity::EntityStatus::BODY_IDLE, "enemy0BodyIdle" },
		{ AnimatedEntity::EntityStatus::HEAD_IDLE, "enemy0HeadIdle" },
		{ AnimatedEntity::EntityStatus::LEGS_MOVING_AROUND, "enemy0LegsMovingAround" },
		{ AnimatedEntity::EntityStatus::LEGS_NOT, "enemy0LegsNot" }
	},
	{
		AnimatedEntity::EntityStatus::LEGS_NOT,
		AnimatedEntity::EntityStatus::ARMS_STAYING_AHEAD,
		AnimatedEntity::EntityStatus::BODY_IDLE,
		AnimatedEntity::EntityStatus::HEAD_IDLE
	}, 100.0, 1000.0, 0.1, 0.75));
}

std::shared_ptr<Enemy> EnemyFactory::getStrongFastEnemy(double spawnX, double spawnY)
{
	return std::shared_ptr<Enemy>(new Enemy(0.5 + spawnX, 0.5 + spawnY, 1.0, 1.0, 90.0, 10.0, 0.5, 0.5, std::map<AnimatedEntity::EntityStatus, std::string>
	{
		{ AnimatedEntity::EntityStatus::ARMS_STAYING_AHEAD, "enemy0ArmsStayingAhead" },
		{ AnimatedEntity::EntityStatus::ARMS_NOT, "enemy0ArmsNot" },
		{ AnimatedEntity::EntityStatus::BODY_IDLE, "enemy0BodyIdle" },
		{ AnimatedEntity::EntityStatus::HEAD_IDLE, "enemy0HeadIdle" },
		{ AnimatedEntity::EntityStatus::LEGS_MOVING_AROUND, "enemy0LegsMovingAround" },
		{ AnimatedEntity::EntityStatus::LEGS_NOT, "enemy0LegsNot" }
	},
	{
		AnimatedEntity::EntityStatus::LEGS_NOT,
		AnimatedEntity::EntityStatus::ARMS_STAYING_AHEAD,
		AnimatedEntity::EntityStatus::BODY_IDLE,
		AnimatedEntity::EntityStatus::HEAD_IDLE
	}, 100.0, 1000.0, 0.1, 0.75));
}