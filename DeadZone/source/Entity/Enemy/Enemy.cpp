#include <queue>
#include <memory>

#include <iostream> // TODO: debug

#include "Enemy.h"
#include "../../Camera/Camera.h"
#include "../Player/Player.h"
#include "../../Map/Map.h"
#include "../../Game/Game.h"
#include "../Wall/Wall.h"
#include "../../ResourceManager/ResourceManager.h"
#include "../../Renderer/SpriteRenderer.h"
#include "../../Random/Random.h"

const float Enemy::INF = 1000000.0f;

Enemy::Enemy(double x, double y, double drawWidth, double drawHeight, double rotateAngle, double speed, double collideWidth, double collideHeight, const std::map<AnimatedEntity::EntityStatus, std::string>& animationsName2D, const std::vector<EntityStatus>& statuses, double health, double rotateSpeed, double attackDamage, double attackRadius, double armor)
	: Entity(x, y, drawWidth, drawHeight, rotateAngle, speed)
	, CollidableEntity(x, y, drawWidth, drawHeight, rotateAngle, speed, collideWidth, collideHeight)
	, AnimatedEntity(x, y, drawWidth, drawHeight, rotateAngle, speed, animationsName2D, statuses)
	, Human(x, y, drawWidth, drawHeight, rotateAngle, speed, collideWidth, collideHeight, animationsName2D, statuses, health, armor)
	, AIEntity(x, y, drawWidth, drawHeight, rotateAngle, speed)
	, rotateSpeed(rotateSpeed), probToChangeDir(1.0 / 250.0)
	, currentTarget(std::make_pair(x, y)), nextTarget(std::make_pair(x, y))
	, movingOffsetSize(0.05), movingOffsetSpeed(15.0), isMoving(false), goldOnKill(100) // TODO:
	, attackDamage(attackDamage), attackRadius(attackRadius)
	, nearestPlayerX(Enemy::INF), nearestPlayerY(Enemy::INF)
{

}


void Enemy::pathFindingTarget()
{
	if (this->nearestPlayerX == Enemy::INF || this->nearestPlayerY == Enemy::INF)
	{
		this->nextTarget = std::make_pair(this->x, this->y);
		return;
	}

	// Clear
	while (!this->pq.empty())
	{
		this->pq.pop();
	}
	this->visitedCells.clear();
	//



	this->blockedCell.resize(Map::get().getMap().size());
	this->cellDistance.resize(Map::get().getMap().size());
	for (int i = 0; i < Map::get().getMap().size(); ++i)
	{
		this->blockedCell[i].resize(Map::get().getMap()[i].size());
		this->cellDistance[i].resize(Map::get().getMap()[i].size());
	}
	for (int i = 0; i < Map::get().getMap().size(); ++i)
	{
		for (int j = 0; j < Map::get().getMap()[i].size(); ++j)
		{
			this->blockedCell[i][j] = false;
			this->cellDistance[i][j] = 0;
		}
	}

	for (int i = 0; i < Map::get().getMap().size(); ++i)
	{
		for (int j = 0; j < Map::get().getMap()[i].size(); ++j)
		{
			if (std::dynamic_pointer_cast<Wall>(Map::get().getMap()[i][j]))
			{
				this->blockedCell[i][j] = true;
			}
		}
	}
	for (int i = 0; i < Map::get().getDoors().size(); ++i)
	{
		if (!Map::get().getDoors()[i]->getCollisionActive())
			continue;

		int xDoor = static_cast<int>(Map::get().getDoors()[i]->getX());
		int yDoor = static_cast<int>(Map::get().getDoors()[i]->getY());
		this->blockedCell[yDoor][xDoor] = true;
	}
	for (int i = 0; i < Map::get().getShops().size(); ++i)
	{
		if (!Map::get().getShops()[i]->getCollisionActive())
			continue;

		int xShop = static_cast<int>(Map::get().getShops()[i]->getX());
		int yShop = static_cast<int>(Map::get().getShops()[i]->getY());
		this->blockedCell[yShop][xShop] = true;
	}

	int xCell = static_cast<int>(this->x);
	int yCell = static_cast<int>(this->y);
	int xTarget = static_cast<int>(this->nearestPlayerX);
	int yTarget = static_cast<int>(this->nearestPlayerY);

	if (xCell == xTarget && yCell == yTarget)
	{
		this->cellDistance[yTarget][xTarget] = 1;
		this->nextTarget = std::make_pair(xTarget + 0.5, yTarget + 0.5);
		return;
	}

	this->cellDistance[yCell][xCell] = 1;
	this->pq.emplace(std::make_pair(std::abs(xCell - xTarget) + std::abs(yCell - yTarget), std::make_pair(xCell, yCell)));
	this->visitedCells.emplace_back(std::make_pair(xCell, yCell));

	bool done = false;
	while (!done && !this->pq.empty())
	{
		int currentX = this->pq.top().second.first;
		int currentY = this->pq.top().second.second;
		this->pq.pop();

		for (int i = 0; i < AIEntity::neighbors.size(); ++i)
		{
			int newX = currentX + AIEntity::neighbors[i].first;
			int newY = currentY + AIEntity::neighbors[i].second;

			if (this->blockedCell[newY][newX] || this->cellDistance[newY][newX] > 0)
				continue;

			this->cellDistance[newY][newX] = this->cellDistance[currentY][currentX] + 1;

			if (newX == xTarget && newY == yTarget)
			{
				done = true;
				break;
			}

			this->pq.emplace(std::make_pair(std::abs(newX - xTarget) + std::abs(newY - yTarget), std::make_pair(newX, newY)));
			this->visitedCells.emplace_back(std::make_pair(newX, newY));
		}
	}

	if (this->cellDistance[yTarget][xTarget] > 0)
	{
		std::pair<int, int> lastCurrentCell = std::make_pair(-1, -1);
		std::pair<int, int> currentCell = std::make_pair(xTarget, yTarget);

		while (currentCell != std::make_pair(xCell, yCell))
		{
			std::vector<std::pair<int, int>> previousCells;

			for (int i = 0; i < AIEntity::neighbors.size(); ++i)
			{
				int newX = currentCell.first + AIEntity::neighbors[i].first;
				int newY = currentCell.second + AIEntity::neighbors[i].second;

				if (this->cellDistance[newY][newX] + 1 == this->cellDistance[currentCell.second][currentCell.first])
				{
					previousCells.emplace_back(std::make_pair(newX, newY));
				}
			}

			int previousCellsIndex = (int)(Random::random01() * ((double)previousCells.size() - Random::EPSILON));
			lastCurrentCell = currentCell;
			currentCell = previousCells[previousCellsIndex];
		}

		this->nextTarget = std::make_pair(lastCurrentCell.first + 0.5, lastCurrentCell.second + 0.5);
	}
	else
	{
		int indexRandom = (int)(Random::random01() * ((double)this->visitedCells.size() - Random::EPSILON));
		this->nextTarget = std::make_pair(this->visitedCells[indexRandom].first + 0.5, this->visitedCells[indexRandom].second + 0.5);
	}
}

void Enemy::onTargetReach()
{
	// TODO: (sau putem lasa asa)
}

bool Enemy::nearTarget()
{
	if (this->nearestPlayerX == Enemy::INF || this->nearestPlayerY == Enemy::INF)
		return false;

	if (this->cellDistance[(int)this->nearestPlayerY][(int)this->nearestPlayerX] == 0)
		return false;
	return this->cellDistance[(int)this->nearestPlayerY][(int)this->nearestPlayerX] - 1 < (int)this->nearTargetRadius;
}

void Enemy::draw()
{
	if (this->isMoving)
	{
		for (int i = 0; i < this->statuses.size(); ++i)
			SpriteRenderer::get().draw(ResourceManager::getShader("sprite"), ResourceManager::getFlipbook(this->animationsName2D[this->statuses[i]]).getTextureAtTime(GlobalClock::get().getCurrentTime() - this->timesSinceStatuses[i]), Camera::get().screenPosition(this->x, this->y), Camera::get().screenSize(this->drawWidth + this->movingOffsetSize * glm::sin(this->movingOffsetSpeed * GlobalClock::get().getCurrentTime()), this->drawHeight + this->movingOffsetSize * glm::sin(this->movingOffsetSpeed * GlobalClock::get().getCurrentTime())), this->rotateAngle);
	}
	else
	{
		for (int i = 0; i < this->statuses.size(); ++i)
			SpriteRenderer::get().draw(ResourceManager::getShader("sprite"), ResourceManager::getFlipbook(this->animationsName2D[this->statuses[i]]).getTextureAtTime(GlobalClock::get().getCurrentTime() - this->timesSinceStatuses[i]), Camera::get().screenPosition(this->x, this->y), Camera::get().screenSize(this->drawWidth, this->drawHeight), this->rotateAngle);
	}
}

void Enemy::update()
{
	if (this->isDead() || getDeleteEntity())
	{
		setDeleteEntity(true);
		return;
	}

	// Gasirea celui mai apropiat jucator
	this->nearestPlayerX = Enemy::INF;
	this->nearestPlayerY = Enemy::INF;

	if (!Player::get().isDead() && (this->x - Player::get().getX()) * (this->x - Player::get().getX()) +
		(this->y - Player::get().getY()) * (this->y - Player::get().getY()) <
		(this->nearestPlayerX - this->x) * (this->nearestPlayerX - this->x) +
		(this->nearestPlayerY - this->y) * (this->nearestPlayerY - this->y))
	{
		this->nearestPlayerX = Player::get().getX();
		this->nearestPlayerY = Player::get().getY();
	}

	for (const auto& remotePlayer : Game::get().getRemotePlayers())
	{
		if (remotePlayer.second->isDead())
			continue;

		if ((this->x - remotePlayer.second->getX()) * (this->x - remotePlayer.second->getX()) +
			(this->y - remotePlayer.second->getY()) * (this->y - remotePlayer.second->getY()) <
			(this->nearestPlayerX - this->x) * (this->nearestPlayerX - this->x) +
			(this->nearestPlayerY - this->y) * (this->nearestPlayerY - this->y))
		{
			this->nearestPlayerX = remotePlayer.second->getX();
			this->nearestPlayerY = remotePlayer.second->getY();
		}
	}

	this->isMoving = false;

	this->pathFindingTarget();

	if (this->nearTarget())
	{
		updateStatus(EntityStatus::LEGS_NOT, 0);

		this->onTargetReach();

		return;
	}

	this->isMoving = true;
	updateStatus(EntityStatus::LEGS_MOVING_AROUND, 0);

	if ((this->x - this->currentTarget.first) * (this->x - this->currentTarget.first) +
		(this->y - this->currentTarget.second) * (this->y - this->currentTarget.second) < AIEntity::EPSILON_MOVEMENT * AIEntity::EPSILON_MOVEMENT)
	{
		this->currentTarget = this->nextTarget;
	}
	else
	{
		double currentTargetAngle = std::atan2f(this->currentTarget.second - this->y, this->currentTarget.first - this->x);
		this->x += this->speed * std::cos(currentTargetAngle) * GlobalClock::get().getDeltaTime();
		this->y += this->speed * std::sin(currentTargetAngle) * GlobalClock::get().getDeltaTime();
	}

	double currentTargetAngle = glm::degrees(std::atan2f(this->currentTarget.second - this->y, this->currentTarget.first - this->x));
	if (currentTargetAngle < 0.0)
		currentTargetAngle += 360.0;

	double angleDist = std::min(std::abs(this->rotateAngle - currentTargetAngle),
		std::abs(360.0 - std::max(this->rotateAngle, currentTargetAngle) +
			std::min(this->rotateAngle, currentTargetAngle)));

	// this->rotateAngle = currentTargetAngle;

	if (angleDist > AIEntity::EPSILON_ANGLE)
	{
		if (std::abs(this->rotateAngle - currentTargetAngle) <
			std::abs(360.0 - std::max(this->rotateAngle, currentTargetAngle) +
				std::min(this->rotateAngle, currentTargetAngle)))
		{
			if (this->rotateAngle < currentTargetAngle) // sens trigonometric
			{
				this->rotateAngle += this->rotateSpeed * GlobalClock::get().getDeltaTime();
				while (this->rotateAngle >= 360.0)
					this->rotateAngle -= 360.0;
			}
			else // sens opus
			{
				this->rotateAngle -= this->rotateSpeed * GlobalClock::get().getDeltaTime();
				while (this->rotateAngle < 0.0)
					this->rotateAngle += 360.0;
			}
		}
		else // sens trigonometric
		{
			this->rotateAngle += this->rotateSpeed * GlobalClock::get().getDeltaTime();
			while (this->rotateAngle >= 360.0)
				this->rotateAngle -= 360.0;
		}
	}
}

void Enemy::updateClient()
{
	// Atac Player
	if ((this->x - Player::get().getX()) * (this->x - Player::get().getX()) +
		(this->y - Player::get().getY()) * (this->y - Player::get().getY())
		<=
		this->attackRadius * this->attackRadius)
	{
		double appliedDamage = this->attackDamage;
		if (Player::get().getArmor() >= appliedDamage)
		{
			Player::get().setArmor(Player::get().getArmor() - appliedDamage);
			appliedDamage = 0.0;
		}
		else
		{
			appliedDamage -= Player::get().getArmor();
			Player::get().setArmor(0.0);
		}
		if (Player::get().getHealth() >= appliedDamage)
		{
			Player::get().setHealth(Player::get().getHealth() - appliedDamage);
			appliedDamage = 0.0;
		}
		else
		{
			appliedDamage -= Player::get().getHealth();
			Player::get().setHealth(0.0);
		}
	}
}

Enemy::~Enemy()
{

}