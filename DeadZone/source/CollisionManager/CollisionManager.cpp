#include "CollisionManager.h"

#include <glm/vec2.hpp>

#include "../Game/Game.h"
#include "../WaveManager/WaveManager.h"
#include "../Entity/CollidableEntity.h"
#include "../Entity/Bullet/Bullet.h"
#include "../Entity/Enemy/Enemy.h"
#include "../Entity/Bullet/ThrownGrenade.h"
#include "../Entity/Explosion/Explosion.h"
#include "../Client/Client.h"

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

			if (std::dynamic_pointer_cast<Bullet>(entities[i]) || std::dynamic_pointer_cast<Explosion>(entities[i]))
			{
				continue;
			}

			glm::vec2 overlap = Player::get().isInCollision(*std::dynamic_pointer_cast<CollidableEntity>(entities[i]));

			if (overlap.x > 0.0 && overlap.y > 0.0)
			{
				Player::get().onCollide(*std::dynamic_pointer_cast<CollidableEntity>(entities[i]), overlap);
				std::dynamic_pointer_cast<CollidableEntity>(entities[i])->onCollide(Player::get(), overlap);
			}
		}

		// Player vs. Bullets
		for (int i = 0; i < entities.size(); ++i)
		{
			if (std::dynamic_pointer_cast<Bullet>(entities[i]) == nullptr)
			{
				continue;
			}

			if (Game::get().getGameMode() == Game::GameMode::TeamDeathMatch)
			{
				if (std::dynamic_pointer_cast<Bullet>(entities[i])->getOwner() != "player")
				{
					std::string bulletOwner = std::dynamic_pointer_cast<Bullet>(entities[i])->getOwner();
					int bulletTeam = Game::get().getRemotePlayerTeam(bulletOwner);

					glm::vec2 overlap = Player::get().isInCollision(*std::dynamic_pointer_cast<CollidableEntity>(entities[i]));
					if (overlap.x > 0.0 && overlap.y > 0.0)
					{
						if (Player::get().getTeam() != bulletTeam)
						{
							bool notDeadBefore = !Player::get().isDead();

							Player::get().onCollide(*std::dynamic_pointer_cast<CollidableEntity>(entities[i]), overlap);
							std::dynamic_pointer_cast<CollidableEntity>(entities[i])->onCollide(Player::get(), overlap);

							if (Player::get().isDead() && notDeadBefore)
							{
								Client::get().sendConfirmedKill(bulletOwner);
							}
						}
						else
						{
							// Player::get().onCollide(*std::dynamic_pointer_cast<CollidableEntity>(entities[i]), overlap);
							std::dynamic_pointer_cast<CollidableEntity>(entities[i])->onCollide(Player::get(), overlap);
						}
					}
				}
			}
			else
			{
				glm::vec2 overlap = Player::get().isInCollision(*std::dynamic_pointer_cast<CollidableEntity>(entities[i]));
				if (overlap.x > 0.0 && overlap.y > 0.0)
				{
					Player::get().onCollide(*std::dynamic_pointer_cast<CollidableEntity>(entities[i]), overlap);
					std::dynamic_pointer_cast<CollidableEntity>(entities[i])->onCollide(Player::get(), overlap);
				}
			}
		}

		// Player vs. Explosions
		for (int i = 0; i < entities.size(); ++i)
		{
			if (std::dynamic_pointer_cast<Explosion>(entities[i]) == nullptr)
			{
				continue;
			}

			if (Game::get().getGameMode() == Game::GameMode::TeamDeathMatch
				&& std::dynamic_pointer_cast<Explosion>(entities[i])->getOwner() != "player")
			{
				std::string explosionOwner = std::dynamic_pointer_cast<Explosion>(entities[i])->getOwner();
				int explosionTeam = Game::get().getRemotePlayerTeam(explosionOwner);

				glm::vec2 overlap = Player::get().isInCollision(*std::dynamic_pointer_cast<CollidableEntity>(entities[i]));
				if (overlap.x > 0.0 && overlap.y > 0.0)
				{
					if (Player::get().getTeam() != explosionTeam)
					{
						bool notDeadBefore = !Player::get().isDead();

						Player::get().onCollide(*std::dynamic_pointer_cast<CollidableEntity>(entities[i]), overlap);
						std::dynamic_pointer_cast<CollidableEntity>(entities[i])->onCollide(Player::get(), overlap);

						if (Player::get().isDead() && notDeadBefore)
						{
							Client::get().sendConfirmedKill(explosionOwner);
						}
					}
					else
					{
						// Player::get().onCollide(*std::dynamic_pointer_cast<CollidableEntity>(entities[i]), overlap);
						std::dynamic_pointer_cast<CollidableEntity>(entities[i])->onCollide(Player::get(), overlap);
					}
				}
			}
			else
			{
				glm::vec2 overlap = Player::get().isInCollision(*std::dynamic_pointer_cast<CollidableEntity>(entities[i]));
				if (overlap.x > 0.0 && overlap.y > 0.0)
				{
					Player::get().onCollide(*std::dynamic_pointer_cast<CollidableEntity>(entities[i]), overlap);
					std::dynamic_pointer_cast<CollidableEntity>(entities[i])->onCollide(Player::get(), overlap);
				}
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
}

void CollisionManager::handleMultiplayerCollisions(std::vector<std::shared_ptr<Entity>>& entities)
{
	// Functia de onCollide SE APELEAZA DIN AMBELE PERSPECTIVE ALE CELOR 2 OBIECTE IMPLICATE

	// Player vs RemotePlayer
	// TODO

	// RemotePlayer vs Entities
	for (const auto& [clientKey, remotePlayer] : Game::get().getRemotePlayers())
	{
		for (const std::shared_ptr<Entity>& entity : entities)
		{
			if (std::dynamic_pointer_cast<CollidableEntity>(entity) == nullptr)
				continue;

			if (!std::dynamic_pointer_cast<CollidableEntity>(entity)->getCollisionActive())
				continue;

			glm::vec2 overlap = std::dynamic_pointer_cast<CollidableEntity>(remotePlayer)->isInCollision(*std::dynamic_pointer_cast<CollidableEntity>(entity));
			if (overlap.x > 0.0 && overlap.y > 0.0)
			{
				std::dynamic_pointer_cast<CollidableEntity>(remotePlayer)->onCollide(*std::dynamic_pointer_cast<CollidableEntity>(entity), overlap);
				std::dynamic_pointer_cast<CollidableEntity>(entity)->onCollide(*std::dynamic_pointer_cast<CollidableEntity>(remotePlayer), overlap);
			}
		}
	}

	// RemoteZombie vs Entities
	for (const auto& [zombieId, remoteZombie] : WaveManager::get().getRemoteZombies())
	{
		for (const std::shared_ptr<Entity>& entity : entities)
		{
			if (std::dynamic_pointer_cast<CollidableEntity>(entity) == nullptr)
				continue;

			if (!std::dynamic_pointer_cast<CollidableEntity>(entity)->getCollisionActive())
				continue;

			glm::vec2 overlap = std::dynamic_pointer_cast<CollidableEntity>(remoteZombie)->isInCollision(*std::dynamic_pointer_cast<CollidableEntity>(entity));
			if (overlap.x > 0.0 && overlap.y > 0.0)
			{
				std::dynamic_pointer_cast<CollidableEntity>(remoteZombie)->onCollide(*std::dynamic_pointer_cast<CollidableEntity>(entity), overlap);
				std::dynamic_pointer_cast<CollidableEntity>(entity)->onCollide(*std::dynamic_pointer_cast<CollidableEntity>(remoteZombie), overlap);
			}
		}
	}
}

