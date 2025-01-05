#pragma once

#include <vector>
#include <string>
#include <memory>
#include <unordered_map>

#include "../Entity/Entity.h"
#include "../Entity/CollidableEntity.h"
#include "../Map/Map.h"
#include "../Entity/Player/Player.h"
#include "../Entity/RemotePlayer/RemotePlayer.h"

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
	void handleMultiplayerCollisions(std::vector<std::shared_ptr<Entity>>& entities);
};

