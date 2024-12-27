#pragma once

#include <vector>
#include <string>
#include <memory>
#include <unordered_map>
#include <queue>

#include "../Entity/Entity.h"
#include "../Entity/DeadBody/DeadBody.h"
#include "../Entity/RemotePlayer/RemotePlayer.h"
#include "../GeneralUtilities/GeneralUtilities.h"

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

public:
	enum class GameStatus
	{
		InGame,
		InMenu
	};

private:
	GameStatus gameStatus = GameStatus::InMenu;
	GeneralUtilities genUtil;

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

	inline std::unordered_map<std::string, std::shared_ptr<RemotePlayer>> getRemotePlayers() const { return remotePlayers; }

	void spawnRemotePlayer(const std::string& clientKey);
	void updateRemotePlayerPosition(const std::string& clientKey, double x, double y);

	void putDoorsInEnclosedAreas(const int& width, const int& height, std::vector<std::vector<std::string>>& M, std::vector<std::vector<bool>>& enclosed);
	void putShopInGoodArea(const int& width, const int& height, std::vector<std::vector<std::string>>& map, const std::vector<std::vector<bool>>& enclosed);

	std::string generateProceduralMap(const int& width, const int& height);
};

