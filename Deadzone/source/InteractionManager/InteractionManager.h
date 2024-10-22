#pragma once

#include <vector>

#include "../Entity/Entity.h"
#include "../Entity/CollidableEntity.h"
#include "../Map/Map.h"
#include "../Entity/Player/Player.h"

class InteractionManager
{
private:
	InteractionManager();
	~InteractionManager();
	InteractionManager(const InteractionManager& other) = delete;
	InteractionManager& operator= (const InteractionManager& other) = delete;
	InteractionManager(const InteractionManager&& other) = delete;
	InteractionManager& operator= (const InteractionManager&& other) = delete;

public:

	static InteractionManager& get();
	void handleInteractions(std::vector<std::shared_ptr<Entity>>& entities);
};

