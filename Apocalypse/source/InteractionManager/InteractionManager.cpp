#include "InteractionManager.h"

#include <glm/vec2.hpp>

#include "../Entity/InteractiveEntity.h"
#include "../Entity/Door/Door.h"

#include <iostream> // TODO: debug
#include <memory>

InteractionManager::InteractionManager()
{

}

InteractionManager::~InteractionManager()
{

}

InteractionManager& InteractionManager::get()
{
	static InteractionManager instance;

	return instance;
}

void InteractionManager::handleInteractions(std::vector<std::shared_ptr<Entity>>& entities)
{
	// Player vs. Doors
	for (int i = 0; i < Map::get().getDoors().size(); ++i)
	{
		if (Map::get().getDoors()[i]->isInInteraction())
			Map::get().getDoors()[i]->onInteraction();
	}

	// Player vs. Shops
	for (int i = 0; i < Map::get().getShops().size(); ++i)
	{
		if (Map::get().getShops()[i]->isInInteraction())
			Map::get().getShops()[i]->onInteraction();
	}

	// Player vs. Entities
	for (int i = 0; i < entities.size(); ++i)
	{
		if (std::dynamic_pointer_cast<InteractiveEntity>(entities[i]) == nullptr)
			continue;

		if (std::dynamic_pointer_cast<InteractiveEntity>(entities[i])->isInInteraction())
			std::dynamic_pointer_cast<InteractiveEntity>(entities[i])->onInteraction();
	}
}

