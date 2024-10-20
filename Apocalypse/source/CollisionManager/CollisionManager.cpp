#include "CollisionManager.h"

#include <glm/vec2.hpp>

#include "../Entity/CollidableEntity.h"
#include "../Entity/Bullet/Bullet.h"
#include "../Entity/Enemy/Enemy.h"
#include "../Entity/Bullet/ThrownGrenade.h"
#include "../Entity/Explosion/Explosion.h"

#include <iostream>
#include <memory>

CollisionManager::CollisionManager()
{

}

CollisionManager::~CollisionManager()
{

}

CollisionManager& CollisionManager::get()
{
	static CollisionManager instance;

	return instance;
}

void CollisionManager::handleCollisions(std::vector<std::shared_ptr<Entity>>& entities)
{
	// Functia de onCollide SE APELEAZA DIN AMBELE PERSPECTIVE ALE CELOR 2 OBIECTE IMPLICATE

	// Player vs. everything
	if (Player::get().getCollisionActive())
	{
		// Player vs. Podea/Pereti
		for (int i = 0; i < Map::get().getMap().size(); ++i)
		{
			for (int j = 0; j < Map::get().getMap()[i].size(); ++j)
			{
				if (std::dynamic_pointer_cast<CollidableEntity>(Map::get().getMap()[i][j]) && std::dynamic_pointer_cast<CollidableEntity>(Map::get().getMap()[i][j])->getCollisionActive())
				{
					glm::vec2 overlap = Player::get().isInCollision(*std::dynamic_pointer_cast<CollidableEntity>(Map::get().getMap()[i][j]));

					if (overlap.x > 0.0 && overlap.y > 0.0)
					{
						Player::get().onCollide(*std::dynamic_pointer_cast<CollidableEntity>(Map::get().getMap()[i][j]), overlap);
						std::dynamic_pointer_cast<CollidableEntity>(Map::get().getMap()[i][j])->onCollide(Player::get(), overlap);
					}
				}
			}
		}

		// Player vs. Doors
		for (int i = 0; i < Map::get().getDoors().size(); ++i)
		{
			if (!Map::get().getDoors()[i]->getCollisionActive())
				continue;

			glm::vec2 overlap = Player::get().isInCollision(*Map::get().getDoors()[i]);

			if (overlap.x > 0.0 && overlap.y > 0.0)
			{
				Player::get().onCollide(*Map::get().getDoors()[i], overlap);
				Map::get().getDoors()[i]->onCollide(Player::get(), overlap);
			}
		}

		// Player vs. Shops
		for (int i = 0; i < Map::get().getShops().size(); ++i)
		{
			if (!Map::get().getShops()[i]->getCollisionActive())
				continue;

			glm::vec2 overlap = Player::get().isInCollision(*Map::get().getShops()[i]);

			if (overlap.x > 0.0 && overlap.y > 0.0)
			{
				Player::get().onCollide(*Map::get().getShops()[i], overlap);
				Map::get().getShops()[i]->onCollide(Player::get(), overlap);
			}
		}

		// Player vs. Entities
		for (int i = 0; i < entities.size(); ++i)
		{
			if (std::dynamic_pointer_cast<CollidableEntity>(entities[i]) == nullptr)
				continue;

			if (!std::dynamic_pointer_cast<CollidableEntity>(entities[i])->getCollisionActive())
				continue;

			glm::vec2 overlap = Player::get().isInCollision(*std::dynamic_pointer_cast<CollidableEntity>(entities[i]));

			if (overlap.x > 0.0 && overlap.y > 0.0)
			{
				Player::get().onCollide(*std::dynamic_pointer_cast<CollidableEntity>(entities[i]), overlap);
				std::dynamic_pointer_cast<CollidableEntity>(entities[i])->onCollide(Player::get(), overlap);
			}
		}
	}

	// Other entities vs. everything
	for (const std::shared_ptr<Entity>& entity : entities)
	{
		if (std::dynamic_pointer_cast<CollidableEntity>(entity) == nullptr)
			continue;

		if (!std::dynamic_pointer_cast<CollidableEntity>(entity)->getCollisionActive())
			continue;

		// Other entities vs. Podea/Pereti
		for (int i = 0; i < Map::get().getMap().size(); ++i)
		{
			for (int j = 0; j < Map::get().getMap()[i].size(); ++j)
			{
				if (std::dynamic_pointer_cast<CollidableEntity>(Map::get().getMap()[i][j]) && std::dynamic_pointer_cast<CollidableEntity>(Map::get().getMap()[i][j])->getCollisionActive())
				{
					glm::vec2 overlap = std::dynamic_pointer_cast<CollidableEntity>(entity)->isInCollision(*std::dynamic_pointer_cast<CollidableEntity>(Map::get().getMap()[i][j]));

					if (overlap.x > 0.0 && overlap.y > 0.0)
					{
						std::dynamic_pointer_cast<CollidableEntity>(entity)->onCollide(*std::dynamic_pointer_cast<CollidableEntity>(Map::get().getMap()[i][j]), overlap);
						std::dynamic_pointer_cast<CollidableEntity>(Map::get().getMap()[i][j])->onCollide(*std::dynamic_pointer_cast<CollidableEntity>(entity), overlap);
					}
				}
			}
		}

		// Other entities vs. Doors
		for (int i = 0; i < Map::get().getDoors().size(); ++i)
		{
			if (!Map::get().getDoors()[i]->getCollisionActive())
				continue;

			glm::vec2 overlap = std::dynamic_pointer_cast<CollidableEntity>(entity)->isInCollision(*Map::get().getDoors()[i]);

			if (overlap.x > 0.0 && overlap.y > 0.0)
			{
				std::dynamic_pointer_cast<CollidableEntity>(entity)->onCollide(*Map::get().getDoors()[i], overlap);
				Map::get().getDoors()[i]->onCollide(*std::dynamic_pointer_cast<CollidableEntity>(entity), overlap);
			}
		}

		// Other entities vs. Shops
		for (int i = 0; i < Map::get().getShops().size(); ++i)
		{
			if (!Map::get().getShops()[i]->getCollisionActive())
				continue;

			glm::vec2 overlap = std::dynamic_pointer_cast<CollidableEntity>(entity)->isInCollision(*Map::get().getShops()[i]);

			if (overlap.x > 0.0 && overlap.y > 0.0)
			{
				std::dynamic_pointer_cast<CollidableEntity>(entity)->onCollide(*Map::get().getShops()[i], overlap);
				Map::get().getShops()[i]->onCollide(*std::dynamic_pointer_cast<CollidableEntity>(entity), overlap);
			}
		}

		// Other entities vs. Other entities
		for (const std::shared_ptr<Entity>& otherEntity : entities)
		{
			if (entity == otherEntity)
				continue;

			if (std::dynamic_pointer_cast<CollidableEntity>(otherEntity) == nullptr)
				continue;

			if (!std::dynamic_pointer_cast<CollidableEntity>(otherEntity)->getCollisionActive())
				continue;

			glm::vec2 overlap = std::dynamic_pointer_cast<CollidableEntity>(entity)->isInCollision(*std::dynamic_pointer_cast<CollidableEntity>(otherEntity));

			if (overlap.x > 0.0 && overlap.y > 0.0)
			{
				std::dynamic_pointer_cast<CollidableEntity>(entity)->onCollide(*std::dynamic_pointer_cast<CollidableEntity>(otherEntity), overlap);
				std::dynamic_pointer_cast<CollidableEntity>(otherEntity)->onCollide(*std::dynamic_pointer_cast<CollidableEntity>(entity), overlap);
			}
		}
	}








	/*

	// Bullets (incluzand thrown grenades) vs. Map
	for (const std::shared_ptr<Entity>& entity : entities)
	{
		if (std::shared_ptr<Bullet> bullet = std::dynamic_pointer_cast<Bullet>(entity))
		{
			for (int i = 0; i < Map::get().getMap().size(); ++i)
			{
				for (int j = 0; j < Map::get().getMap()[i].size(); ++j)
				{
					if (std::dynamic_pointer_cast<CollidableEntity>(Map::get().getMap()[i][j]) && std::dynamic_pointer_cast<CollidableEntity>(Map::get().getMap()[i][j])->getCollisionActive())
					{
						glm::vec2 overlap = bullet->isInCollision(*std::dynamic_pointer_cast<CollidableEntity>(Map::get().getMap()[i][j]));

						if (overlap.x > 0.0 && overlap.y > 0.0)
						{
							bullet->onCollide(*std::dynamic_pointer_cast<CollidableEntity>(Map::get().getMap()[i][j]), overlap);
							std::dynamic_pointer_cast<CollidableEntity>(Map::get().getMap()[i][j])->onCollide(*bullet, overlap);
						}
					}
				}
			}
		}
	}


	// Player vs. Entities
	// TODO: momentan enemy (si bullets, si thrown grenades) // include si exploziile
	for (int i = 0; i < entities.size(); ++i)
	{
		if (std::dynamic_pointer_cast<CollidableEntity>(entities[i]) == nullptr)
			continue;

		if (!std::dynamic_pointer_cast<CollidableEntity>(entities[i])->getCollisionActive())
			continue;

		if (std::dynamic_pointer_cast<ThrownGrenade>(entities[i])) // fara coliziune intre thrown grenade si player SE POATE STERGE
			continue;

		glm::vec2 overlap = std::dynamic_pointer_cast<CollidableEntity>(entities[i])->isInCollision(Player::get());

		if (overlap.x > 0.0 && overlap.y > 0.0)
		{
			Player::get().onCollide(*std::dynamic_pointer_cast<CollidableEntity>(entities[i]), overlap);
			std::dynamic_pointer_cast<CollidableEntity>(entities[i])->onCollide(Player::get(), overlap);
		}
	}

	// Bullets (incluzand thrown grenades) vs. Doors
	for (int i = 0; i < entities.size(); ++i)
	{
		if (std::dynamic_pointer_cast<Bullet>(entities[i]) == nullptr)
			continue;

		for (int j = 0; j < Map::get().getDoors().size(); ++j)
		{
			if (!Map::get().getDoors()[j]->getCollisionActive())
				continue;

			glm::vec2 overlap = Map::get().getDoors()[j]->isInCollision(*std::dynamic_pointer_cast<CollidableEntity>(entities[i]));

			if (overlap.x > 0.0 && overlap.y > 0.0)
			{
				std::dynamic_pointer_cast<CollidableEntity>(entities[i])->onCollide(*Map::get().getDoors()[j], overlap);
				Map::get().getDoors()[j]->onCollide(*std::dynamic_pointer_cast<CollidableEntity>(entities[i]), overlap);
			}
		}
	}

	// Bullets (incluzand thrown grenades) vs Enemies
	for (int i = 0; i < entities.size(); ++i)
	{
		if (std::dynamic_pointer_cast<Bullet>(entities[i]) == nullptr)
			continue;

		if (std::dynamic_pointer_cast<ThrownGrenade>(entities[i])) // fara coliziune intre thrown grenade si player SE POATE STERGE
			continue;

		for (int j = 0; j < entities.size(); ++j)
		{
			if (i == j)
				continue;

			if (std::dynamic_pointer_cast<Enemy>(entities[j]) == nullptr)
				continue;

			if (!std::dynamic_pointer_cast<CollidableEntity>(entities[j])->getCollisionActive())
				continue;

			glm::vec2 overlap = std::dynamic_pointer_cast<CollidableEntity>(entities[i])->isInCollision(*std::dynamic_pointer_cast<CollidableEntity>(entities[j]));

			if (overlap.x > 0.0 && overlap.y > 0.0)
			{
				std::dynamic_pointer_cast<CollidableEntity>(entities[i])->onCollide(*std::dynamic_pointer_cast<CollidableEntity>(entities[j]), overlap);
				std::dynamic_pointer_cast<CollidableEntity>(entities[j])->onCollide(*std::dynamic_pointer_cast<CollidableEntity>(entities[i]), overlap);
			}
		}
	}

	// Explosions vs Enemies
	for (int i = 0; i < entities.size(); ++i)
	{
		if (std::dynamic_pointer_cast<Explosion>(entities[i]) == nullptr)
			continue;

		for (int j = 0; j < entities.size(); ++j)
		{
			if (i == j)
				continue;

			if (std::dynamic_pointer_cast<Enemy>(entities[j]) == nullptr)
				continue;

			if (!std::dynamic_pointer_cast<CollidableEntity>(entities[j])->getCollisionActive())
				continue;

			glm::vec2 overlap = std::dynamic_pointer_cast<CollidableEntity>(entities[i])->isInCollision(*std::dynamic_pointer_cast<CollidableEntity>(entities[j]));

			if (overlap.x > 0.0 && overlap.y > 0.0)
			{
				std::dynamic_pointer_cast<CollidableEntity>(entities[i])->onCollide(*std::dynamic_pointer_cast<CollidableEntity>(entities[j]), overlap);
				std::dynamic_pointer_cast<CollidableEntity>(entities[j])->onCollide(*std::dynamic_pointer_cast<CollidableEntity>(entities[i]), overlap);
			}
		}
	}

	// Enemies vs Enemies
	for (int i = 0; i < entities.size(); ++i)
	{
		if (std::dynamic_pointer_cast<Enemy>(entities[i]) == nullptr)
			continue;

		for (int j = 0; j < entities.size(); ++j)
		{
			if (i == j)
				continue;

			if (std::dynamic_pointer_cast<Enemy>(entities[j]) == nullptr)
				continue;

			glm::vec2 overlap = std::dynamic_pointer_cast<CollidableEntity>(entities[i])->isInCollision(*std::dynamic_pointer_cast<CollidableEntity>(entities[j]));

			if (overlap.x > 0.0 && overlap.y > 0.0)
			{
				std::dynamic_pointer_cast<CollidableEntity>(entities[i])->onCollide(*std::dynamic_pointer_cast<CollidableEntity>(entities[j]), overlap);
				std::dynamic_pointer_cast<CollidableEntity>(entities[j])->onCollide(*std::dynamic_pointer_cast<CollidableEntity>(entities[i]), overlap);
			}
		}
	}

	// Doors vs Enemies
	for (int i = 0; i < Map::get().getDoors().size(); ++i)
	{
		if (!std::dynamic_pointer_cast<CollidableEntity>(Map::get().getDoors()[i])->getCollisionActive())
			continue;

		for (int j = 0; j < entities.size(); ++j)
		{
			if (std::dynamic_pointer_cast<Enemy>(entities[j]) == nullptr)
				continue;

			glm::vec2 overlap = Map::get().getDoors()[i]->isInCollision(*std::dynamic_pointer_cast<CollidableEntity>(entities[j]));

			if (overlap.x > 0.0 && overlap.y > 0.0)
			{
				Map::get().getDoors()[i]->onCollide(*std::dynamic_pointer_cast<CollidableEntity>(entities[j]), overlap);
				std::dynamic_pointer_cast<CollidableEntity>(entities[j])->onCollide(*Map::get().getDoors()[i], overlap);
			}
		}
	}
	
	// Walls vs Enemies
	for (int i = 0; i < Map::get().getMap().size(); ++i)
	{
		for (int j = 0; j < Map::get().getMap()[i].size(); ++j)
		{
			if (std::dynamic_pointer_cast<CollidableEntity>(Map::get().getMap()[i][j]) && std::dynamic_pointer_cast<CollidableEntity>(Map::get().getMap()[i][j])->getCollisionActive())
			{
				for (int k = 0; k < entities.size(); ++k)
				{
					if (std::dynamic_pointer_cast<Enemy>(entities[k]) == nullptr)
						continue;

					glm::vec2 overlap = std::dynamic_pointer_cast<CollidableEntity>(entities[k])->isInCollision(*std::dynamic_pointer_cast<CollidableEntity>(Map::get().getMap()[i][j]));

					if (overlap.x > 0.0 && overlap.y > 0.0)
					{
						std::dynamic_pointer_cast<CollidableEntity>(entities[k])->onCollide(*std::dynamic_pointer_cast<CollidableEntity>(Map::get().getMap()[i][j]), overlap);
						std::dynamic_pointer_cast<CollidableEntity>(Map::get().getMap()[i][j])->onCollide(*std::dynamic_pointer_cast<CollidableEntity>(entities[k]), overlap);
					}
				}
			}
		}
	}

	// Shops vs Collidable (enemies, bullets, explosions, thrown grenades)
	for (int i = 0; i < Map::get().getShops().size(); ++i)
	{
		if (!Map::get().getShops()[i]->getCollisionActive())
			continue;

		for (int j = 0; j < entities.size(); ++j)
		{
			if (std::dynamic_pointer_cast<CollidableEntity>(entities[j]) == nullptr)
				continue;

			if (!std::dynamic_pointer_cast<CollidableEntity>(entities[j])->getCollisionActive())
				continue;

			glm::vec2 overlap = std::dynamic_pointer_cast<CollidableEntity>(entities[j])->isInCollision(*Map::get().getShops()[i]);

			if (overlap.x > 0.0 && overlap.y > 0.0)
			{
				std::dynamic_pointer_cast<CollidableEntity>(entities[j])->onCollide(*Map::get().getShops()[i], overlap);
				Map::get().getShops()[i]->onCollide(*std::dynamic_pointer_cast<CollidableEntity>(entities[j]), overlap);
			}
		}
	}
	*/
}

