#pragma once

#include <vector>
#include <string>
#include <memory>
#include <unordered_map>
#include <queue>

#include <thread>
#include <mutex>

#include "../Entity/Entity.h"
#include "../Entity/DeadBody/DeadBody.h"
#include "../Entity/RemotePlayer/RemotePlayer.h"

class Game
{
private:
	Game();
	~Game();
	Game(const Game& other) = delete;
	Game& operator= (const Game& other) = delete;
	Game(const Game&& other) = delete;
	Game& operator= (const Game&& other) = delete;

private:
	void loadResources();

	void updateEntities();
	void drawDeadBodies();
	void drawEntities();

	void addRemotePlayer(const std::string& clientKey, std::shared_ptr<RemotePlayer> const remotePlayer);

private:
	std::vector<std::shared_ptr<DeadBody>> deadBodies;

	std::vector<std::shared_ptr<Entity>> entities;
	std::vector<std::shared_ptr<Entity>> entitiesForNextFrame;
	std::unordered_map<std::string, std::shared_ptr<RemotePlayer>> remotePlayers;

	const int MAX_NUM_DEAD_BODIES;

	bool isServer;
	bool isInMatch;
	bool hasGameMode;

	std::shared_ptr<std::thread> serverThread;
	std::mutex isServerRunningMutex;
	bool isServerRunning;
	int sizeTeam1 = 0, sizeTeam2 = 0;
	const glm::vec3 colorTeam1 = glm::vec3(0.0f, 0.0f, 1.0f);
	const glm::vec3 colorTeam2 = glm::vec3(1.0f, 0.0f, 0.0f);

public:
	enum class GameStatus
	{
		InGame,
		InMenu
	};

	enum class GameMode : unsigned int
	{
		Survival = 0,
		TeamDeathMatch = 1
	};

private:
	GameStatus gameStatus = GameStatus::InMenu;
	GameMode gameMode = GameMode::TeamDeathMatch;

public:
	static Game& get();

	void run();

	void addEntityForNextFrame(std::shared_ptr<Entity> const entity);
	void addDeadBody(std::shared_ptr<DeadBody> const deadBody);

	inline std::vector<std::shared_ptr<Entity>>& getEntities() { return this->entities; }
	inline std::vector<std::shared_ptr<DeadBody>>& getDeadBodies() { return this->deadBodies; }

	inline void clear() { deadBodies.clear(); entities.clear(); };

	inline GameStatus getGameStatus() const { return gameStatus; }
	inline void setGameStatus(const GameStatus& gameStatus) { this->gameStatus = gameStatus; }

	inline void setGameMode(const GameMode& gameMode) { this->gameMode = gameMode; }
	inline GameMode getGameMode() const { return gameMode; }

	inline std::unordered_map<std::string, std::shared_ptr<RemotePlayer>> getRemotePlayers() const { return remotePlayers; }

	void spawnRemotePlayer(const std::string& clientKey);
	void updateRemotePlayerClientName(const std::string& clientKey, const std::string& name);
	void updateRemotePlayerOutfitColor(const std::string& clientKey, const glm::vec3& color);
	void updateRemotePlayerPosition(const std::string& clientKey, double x, double y);
	void updateRemotePlayerRotateAngle(const std::string& clientKey, double angle);
	void updateRemotePlayerStatuses(const std::string& clientKey, const std::vector<AnimatedEntity::EntityStatus>& statuses);
	void applyRemotePlayerCloseRangeDamage(const std::string& clientKey, double damage, double shortRangeAttackRadius);

	inline bool getIsServer() const { return isServer; }
	void establishConnection();
	void stopConnection();

	// Getters / Setters
	inline bool getIsInMatch() const { return isInMatch; }
	inline void setIsInMatch(bool value) { isInMatch = value; }

	inline bool getHasGameMode() const { return hasGameMode; }
	inline void setHasGameMode(bool value) { hasGameMode = value; }
};

