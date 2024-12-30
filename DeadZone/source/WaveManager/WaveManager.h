#pragma once

#include <vector>
#include <string>
#include <queue>
#include <memory>
#include <unordered_map>

#include "../Entity/Enemy/Enemy.h"

class WaveManager
{
private:

	WaveManager(const double waveCoolDown, bool inWave, double timeWaveEnded, int numEnemiesPerTurn, int numFinishedWaves);
	WaveManager(const WaveManager& other) = delete;
	WaveManager& operator= (const WaveManager& other) = delete;
	WaveManager(const WaveManager&& other) = delete;
	WaveManager& operator= (const WaveManager&& other) = delete;

	const double waveCoolDown;
	bool inWave;
	double timeWaveEnded;
	int numEnemiesPerTurn;
	int numFinishedWaves;

	std::vector<std::vector<bool>> blockedCell;
	std::vector<std::vector<int>> cellDistance;
	std::queue<std::pair<int, int>> q;
	std::vector<std::pair<int, int>> visitedCells;

	void bfsSearch();

	const std::vector<std::pair<int, int>> WaveManager::neighbors = { {0, -1}, {0, 1}, {-1, 0}, {1, 0} };
	const std::vector<std::pair<int, int>> WaveManager::extendedNeighbors = { {-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1} };

	static std::shared_ptr<WaveManager> instance;

	// Multiplayer
	std::unordered_map<std::string, std::shared_ptr<Enemy>> remoteZombies;

public:
	~WaveManager();

	static WaveManager& get();

	int getNumFinishedWaves() const { return this->numFinishedWaves; }
	int getCurrentWaveNumber() const { return this->numFinishedWaves + 1; }

	void update();
	void draw();
	
	static void deleteInstance();

	// Multiplayer
	void spawnRemoteZombie(const std::string& id, double x, double y);
	void updateRemoteZombiePosition(const std::string& id, double x, double y);
	void updateRemoteZombieRotateAngle(const std::string& id, double angle);
	void updateRemoteZombieStatuses(const std::string& id, const std::vector<AnimatedEntity::EntityStatus>& statuses);

	// Getters
	inline bool getInWave() const { return this->inWave; }
	inline std::unordered_map<std::string, std::shared_ptr<Enemy>> getRemoteZombies() const { return remoteZombies; };
};
