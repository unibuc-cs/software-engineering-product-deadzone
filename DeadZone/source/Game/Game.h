#pragma once

#include <vector>
#include <memory>

#include "../Entity/Entity.h"
#include "../Entity/DeadBody/DeadBody.h"

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

public:
	enum class PlayerType
	{
		Server,
		Client
	};

private:
	std::vector<std::shared_ptr<DeadBody>> deadBodies;

	std::vector<std::shared_ptr<Entity>> entities;
	std::vector<std::shared_ptr<Entity>> entitiesForNextFrame;

	const int MAX_NUM_DEAD_BODIES;

	PlayerType playerType;

public:
	enum class GameStatus
	{
		InGame,
		InMenu
	};

private:
	GameStatus gameStatus = GameStatus::InMenu;

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

	inline PlayerType getPlayerType() const { return playerType; }
};

