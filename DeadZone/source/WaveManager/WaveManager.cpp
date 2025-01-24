#include "WaveManager.h"

#include "../GlobalClock/GlobalClock.h"
#include "../Map/Map.h"
#include "../Entity/Wall/Wall.h"
#include "../Entity/Player/Player.h"
#include "../Game/Game.h"
#include "../Entity/Enemy/Enemy.h"
#include "../Random/Random.h"
#include "../Entity/Enemy/EnemyFactory.h"
#include "../SoundManager/SoundManager.h"
#include "../Server/Server.h"

std::shared_ptr<WaveManager> WaveManager::instance = nullptr;

WaveManager::WaveManager(const double waveCoolDown, bool inWave, double timeWaveEnded, int numEnemiesPerTurn, int numFinishedWaves) :
	waveCoolDown(waveCoolDown), inWave(inWave), timeWaveEnded(timeWaveEnded), numEnemiesPerTurn(numEnemiesPerTurn), numFinishedWaves(numFinishedWaves)
{

}

WaveManager& WaveManager::get()
{
	if (WaveManager::instance == nullptr)
	{
		WaveManager::instance = std::shared_ptr<WaveManager>(new WaveManager(10.0, false, GlobalClock::get().getCurrentTime(), 7, 0));
	}

	return *WaveManager::instance;
}

void WaveManager::deleteInstance()
{
	WaveManager::instance = nullptr;
}

WaveManager::~WaveManager()
{

}

void WaveManager::bfsSearch()
{
	// Clear
	while (!this->q.empty())
	{
		this->q.pop();
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

	int xSource = static_cast<int>(Player::get().getX());
	int ySource = static_cast<int>(Player::get().getY());

	this->cellDistance[ySource][xSource] = 1;
	this->q.emplace(std::make_pair(xSource, ySource));
	this->visitedCells.emplace_back(std::make_pair(xSource, ySource));


	while (!this->q.empty())
	{
		int currentX = this->q.front().first;
		int currentY = this->q.front().second;
		this->q.pop();

		for (int i = 0; i < this->neighbors.size(); ++i)
		{
			int newX = currentX + this->neighbors[i].first;
			int newY = currentY + this->neighbors[i].second;

			if (this->blockedCell[newY][newX] || this->cellDistance[newY][newX] > 0)
				continue;

			this->cellDistance[newY][newX] = this->cellDistance[currentY][currentX] + 1;

			this->q.emplace(std::make_pair(newX, newY));
			this->visitedCells.emplace_back(std::make_pair(newX, newY));
		}
	}
}

void WaveManager::update()
{
	if (!Map::get().getHasBeenLoaded())
	{
		return;
	}

	// killed zombies
	for (auto it = remoteZombies.begin(); it != remoteZombies.end(); )
	{
		if (it->second->getDeleteEntity())
		{
			int deadTextureIndex = Random::randomInt(0, 1);
			double deadRotateAngle = (Random::random01() * 360.0 - Random::EPSILON);
			double deadResize = 1.25;

			std::map<AnimatedEntity::EntityStatus, std::string> m0 = {
				{ AnimatedEntity::EntityStatus::DEAD_HUMAN, "enemy" + std::to_string(deadTextureIndex) + "Dead" }
			};
			std::vector<AnimatedEntity::EntityStatus> v0 = { AnimatedEntity::EntityStatus::DEAD_HUMAN };

			Game::get().addDeadBody(std::make_shared<DeadBody>(it->second->getX(), it->second->getY(), deadResize * it->second->getDrawWidth(), deadResize * it->second->getDrawHeight(), deadRotateAngle, 0.0, m0, v0));

			// TODO: toti jucatorii primesc gold + kill
			Player::get().setGold(Player::get().getGold() + it->second->getGoldOnKill());
			Player::get().setNumKills(Player::get().getNumKills() + 1);

			it = remoteZombies.erase(it);
		}
		else
		{
			++it;
		}
	}

	// Update specific doar pentru clienti fara server
	for (auto& zombie : remoteZombies)
	{
		zombie.second->updateClient();
	}

	// doar server-ul se ocupa de calcularea tuturor pozitiilor + trimite noile date
	if (Game::get().getIsServer())
	{
		if (this->inWave)
		{
			if (remoteZombies.size() == 0)
			{
				this->inWave = false;
				++this->numFinishedWaves;
				this->timeWaveEnded = GlobalClock::get().getCurrentTime();

				// update other clients
				Server::get().sendNumFinishedWaves(numFinishedWaves);

				//SoundManager::get().play("newWave", false);
			}
		}
		else if (GlobalClock::get().getCurrentTime() - this->timeWaveEnded > this->waveCoolDown)
		{
			this->inWave = true;
			bfsSearch();

			//aici spaunez zombii
			for (int i = 0; i < this->numEnemiesPerTurn && !this->visitedCells.empty(); ++i)
			{
				// Varianta Random
				/*
				int indexRandom = (int)(Random::random01() * ((double)this->visitedCells.size() - Random::EPSILON));
				std::pair<int, int> spawnPos = this->visitedCells[indexRandom];
				std::swap(this->visitedCells[indexRandom], this->visitedCells[(int)this->visitedCells.size() - 1]);
				this->visitedCells.pop_back();
				Game::get().addEntity(EnemyFactory::getDefaultEnemy(spawnPos.first, spawnPos.second));
				*/

				// Varianta unde ii punem cat mai departe de player (Problema: ajung sa se spauneze toti cam in acelasi loc si se incurca intre ei)
				/*
				std::pair<int, int> spawnPos = this->visitedCells[(int)this->visitedCells.size() - 1];
				this->visitedCells.pop_back();
				Game::get().addEntity(EnemyFactory::getDefaultEnemy(spawnPos.first, spawnPos.second));
				*/

				// Varianta unde ii punem cat mai departe de player dar facem sarituri de cate k celule (problema, daca nu avem k celule disponibile? va crashui, TODO:)
				int k = 50;
				std::pair<int, int> spawnPos = this->visitedCells[(int)this->visitedCells.size() - k];
				std::swap(this->visitedCells[(int)this->visitedCells.size() - k], this->visitedCells[(int)this->visitedCells.size() - 1]);
				this->visitedCells.pop_back();

				// spawn remote zombie
				spawnRemoteZombie(std::to_string(i), spawnPos.first, spawnPos.second);

				// sound effect
				SoundManager::get().play("newWave", false);
			}
		}

		// update enemy specific SERVER
		for (auto& zombie : remoteZombies)
		{
			zombie.second->update();
		}

		Server::get().sendZombiesData(remoteZombies);
	}
}

void WaveManager::draw()
{
	for (auto& zombie : remoteZombies)
	{
		zombie.second->draw();
	}
}

void WaveManager::spawnRemoteZombie(const std::string& id, double x, double y)
{
	if (remoteZombies.find(id) != remoteZombies.end())
	{
		// TODO: throw error
		return;
	}

	remoteZombies[id] = EnemyFactory::getDefaultEnemy(x, y);
}

void WaveManager::updateRemoteZombiePosition(const std::string& id, double x, double y)
{
	remoteZombies[id]->setX(x);
	remoteZombies[id]->setY(y);
}

void WaveManager::updateRemoteZombieRotateAngle(const std::string& id, double angle)
{
	remoteZombies[id]->setRotateAngle(angle);
}

void WaveManager::updateRemoteZombieStatuses(const std::string& id, const std::vector<AnimatedEntity::EntityStatus>& statuses)
{
	for (size_t indexStatus = 0; indexStatus < statuses.size(); ++indexStatus)
	{
		remoteZombies[id]->updateStatus(statuses[indexStatus], indexStatus);
	}
}

void WaveManager::updateRemoteZombieDeleteEntity(const std::string& id, bool value)
{
	remoteZombies[id]->setDeleteEntity(value);
}
