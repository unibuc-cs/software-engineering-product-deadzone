#pragma once

#include <vector>

#include "../Entity/Entity.h"
#include "../Entity/CollidableEntity.h"
#include "../Map/Map.h"
#include "../Entity/Player/Player.h"

class CollisionManager
{
private:
	CollisionManager();
	~CollisionManager();
	CollisionManager(const CollisionManager& other) = delete;
	CollisionManager& operator= (const CollisionManager& other) = delete;
	CollisionManager(const CollisionManager&& other) = delete;
	CollisionManager& operator= (const CollisionManager&& other) = delete;

public:

	static CollisionManager& get();
	void handleCollisions(std::vector<std::shared_ptr<Entity>>& entities);
};

