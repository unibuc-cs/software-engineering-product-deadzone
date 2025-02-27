#include "Map.h"

#include <iostream>
#include <sstream>
#include <queue>
#include <set>

#include "../Renderer/SpriteRenderer.h"
#include "../ResourceManager/ResourceManager.h"

#include "../Entity/Door/Door.h"
#include "../Entity/Floor/Floor.h"
#include "../Entity/Wall/Wall.h"

#include "../Random/Random.h"

std::shared_ptr<Map> Map::instance = nullptr;

std::vector<std::vector<std::string>> Map::mapString;
std::vector<std::vector<bool>> Map::enclosed;
int Map::height, Map::width;

Map::Map()
{

}

Map& Map::get()
{
	if (Map::instance == nullptr)
		Map::instance = std::shared_ptr<Map>(new Map());

	return *Map::instance;
}

void Map::deleteInstance()
{
	Map::instance = nullptr;
}

void Map::readMapFromFile(const std::string& path)
{
	this->mapString.clear();
	this->map.clear();
	this->doors.clear();
	this->shops.clear();
	this->enclosed.clear();

	std::ios_base::sync_with_stdio(false);

	std::ifstream in(path);
	if (in.fail())
	{
		throw std::runtime_error("Cannot open file: " + path);
	}
	in.tie(nullptr);

	while (!in.eof())
	{
		this->map.emplace_back();
		this->mapString.emplace_back();

		std::string line;
		std::getline(in, line);
		std::stringstream ss(line);

		std::string code;
		while (ss >> code)
		{
			if (code[0] == 'M')
			{
				this->map.back().emplace_back(std::make_shared<Wall>((double)this->map.back().size() + 0.5, (double)this->map.size() - 0.5, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, code));
			}
			else if (code[0] == '.')
			{
				this->map.back().emplace_back(std::make_shared<Floor>((double)this->map.back().size() + 0.5, (double)this->map.size() - 0.5, 1.0, 1.0, 0.0, 0.0, code));
			}
			else if (code[0] == 'D')
			{
				std::map<AnimatedEntity::EntityStatus, std::string> m0 = {
					{ AnimatedEntity::EntityStatus::IDLE, "doorStatic0"},
					{ AnimatedEntity::EntityStatus::OPENED, "doorOpening0"}
				};
				std::vector<AnimatedEntity::EntityStatus> v0 = { AnimatedEntity::EntityStatus::IDLE };
				this->map.back().emplace_back(std::make_shared<Door>((double)this->map.back().size() + 0.5, (double)this->map.size() - 0.5, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, m0, v0, 2.0, 2.0, 500));
				this->doors.emplace_back(std::dynamic_pointer_cast<Door>(this->map.back().back()));
			}
			else if (code[0] == 's')
			{
				this->map.back().emplace_back(std::make_shared<Shop>((double)this->map.back().size() + 0.5, (double)this->map.size() - 0.5, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, code, 10.0, 10.0));
				this->shops.emplace_back(std::dynamic_pointer_cast<Shop>(this->map.back().back()));
			}
			this->mapString.back().emplace_back(code);
		}
	}

	in.close();

	this->createEnclosedAreas();

	this->mapLoaded = true;
}

void Map::readMapFromBuffer(const std::vector<std::vector<std::string>>& buffer)
{
	
	this->mapString.clear();
	this->map.clear();
	this->doors.clear();
	this->shops.clear();
	this->enclosed.clear();

	height = buffer.size();
	width = buffer[0].size();

	for (const std::vector<std::string> &line : buffer)
	{
		this->map.emplace_back();
		this->mapString.emplace_back();

		for (const std::string& code : line)
		{
			if (code[0] == 'M')
			{
				this->map.back().emplace_back(std::make_shared<Wall>((double)this->map.back().size() + 0.5, (double)this->map.size() - 0.5, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, code));
			}
			else if (code[0] == '.')
			{
				this->map.back().emplace_back(std::make_shared<Floor>((double)this->map.back().size() + 0.5, (double)this->map.size() - 0.5, 1.0, 1.0, 0.0, 0.0, code));
			}
			else if (code[0] == 'D')
			{
				this->map.back().emplace_back(std::make_shared<Floor>((double)this->map.back().size() + 0.5, (double)this->map.size() - 0.5, 1.0, 1.0, 0.0, 0.0, ".0"));
				std::map<AnimatedEntity::EntityStatus, std::string> m0 = {
					{ AnimatedEntity::EntityStatus::IDLE, "doorStatic0"},
					{ AnimatedEntity::EntityStatus::OPENED, "doorOpening0"}
				};
				std::vector<AnimatedEntity::EntityStatus> v0 = { AnimatedEntity::EntityStatus::IDLE };
				if (code[3] == '0') 
				{
					this->doors.emplace_back(std::make_shared<Door>((double)this->map.back().size() - 0.5, (double)this->map.size() - 0.5, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, m0, v0, 2.0, 2.0, 500));
				}
				else 
				{
					this->doors.emplace_back(std::make_shared<Door>((double)this->map.back().size() - 0.5, (double)this->map.size() - 0.5, 1.0, 1.0, 90.0, 0.0, 1.0, 1.0, m0, v0, 2.0, 2.0, 500));
				}
			}
			else if (code[0] == 'S')
			{
				this->map.back().emplace_back(std::make_shared<Floor>((double)this->map.back().size() + 0.5, (double)this->map.size() - 0.5, 1.0, 1.0, 0.0, 0.0, ".0"));
				this->shops.emplace_back(std::make_shared<Shop>((double)this->map.back().size() - 0.5, (double)this->map.size() - 0.5, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, code, 10.0, 10.0));
			}
			this->mapString.back().emplace_back(code);
		}
	}

	this->createEnclosedAreas();

	this->mapLoaded = true;
}

void Map::createEnclosedAreas() {
	std::vector<std::vector<bool>> visited(height, std::vector<bool>(width, 0));
	std::vector<std::vector<bool>> visited2(height, std::vector<bool>(width, 0));
	const int di[8] = { -1, 0, 1, 0, 1, 1, -1, -1 };
	const int dj[8] = { 0, 1, 0, -1, 1, -1, 1, -1 };

	auto inside = [&](std::pair<int, int> cell) {
		if (cell.first < height && cell.first >= 0 && cell.second < width && cell.second >= 0)
			return 1;
		return 0;
		};

	auto inside2 = [&](std::pair<int, int> cell) {
		if (cell.first < height - 1 && cell.first >= 1 && cell.second < width - 1 && cell.second >= 1)
			return 1;
		return 0;
		};

	std::queue<std::pair<int, int>> cellsInQueue;
	std::vector<std::pair<int, int>> visitedCells;
	enclosed.assign(height, std::vector<bool>(width, 0));
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
			if (mapString[i][j][0] == '.' && !visited[i][j]) {
				visitedCells.clear();
				int cnt = 0;
				cellsInQueue.push({ i, j });
				visited[i][j] = 1;
				visitedCells.push_back({ i, j });
				while (cellsInQueue.size() > 0) {
					cnt++;
					std::pair<int, int> cell = cellsInQueue.front();
					cellsInQueue.pop();
					for (int k = 0; k < 4; k++) {
						std::pair<int, int> new_cell = { cell.first + di[k], cell.second + dj[k] };
						if (inside(new_cell) && !visited[new_cell.first][new_cell.second]) {
							if (mapString[new_cell.first][new_cell.second][0] == '.') {
								cellsInQueue.push({ new_cell.first, new_cell.second });
								visited[new_cell.first][new_cell.second] = 1;
								visitedCells.push_back({ new_cell.first, new_cell.second });
							}
							else if (mapString[new_cell.first][new_cell.second][0] == 'M') {
								std::queue<std::pair<int, int>> queueForDoor;
								queueForDoor.push(new_cell);
								visited2[new_cell.first][new_cell.second] = 1;
								while (queueForDoor.size() > 0) {
									std::pair<int, int> cell_for_door = queueForDoor.front();
									queueForDoor.pop();
									for (int k2 = 0; k2 < 8; k2++) {
										std::pair<int, int> new_cell_for_door = cell_for_door;
										new_cell_for_door.first += di[k2];
										new_cell_for_door.second += dj[k2];
										if (inside2(new_cell_for_door) && !visited2[new_cell_for_door.first][new_cell_for_door.second] && mapString[new_cell_for_door.first][new_cell_for_door.second] == mapString[new_cell.first][new_cell.second]) {
											visited2[new_cell_for_door.first][new_cell_for_door.second] = 1;
											queueForDoor.push(new_cell_for_door);
										}
									}
								}
							}
						}
					}
				}
				if (cnt < 100) {
					for (int k = 0; k < visitedCells.size(); k++) {
						std::pair<int, int> cell = visitedCells[k];
						enclosed[cell.first][cell.second] = 1;
					}
				}
			}
}

void Map::draw()
{
	for (int i = 0; i < this->map.size(); ++i)
	{
		for (int j = 0; j < this->map[i].size(); ++j)
		{
			this->map[i][j]->draw();
		}
	}
	for (int i = 0; i < this->doors.size(); ++i)
	{
		this->doors[i]->draw();
	}
	for (int i = 0; i < this->shops.size(); ++i)
	{
		this->shops[i]->draw();
	}
}

void Map::update()
{
	for (int i = 0; i < this->doors.size(); ++i)
	{
		this->doors[i]->update();
	}
	
	// shops nu are update, deoarece interactiunile cu player-ul au loc in interactionManager
}

void Map::updateDoorStatus(unsigned int id)
{
	for (auto& door : doors)
	{
		if (door.get()->getID() == id)
		{
			door.get()->openDoor();
		}
	}
}

void Map::putDoorsInEnclosedAreas() {
	std::vector<std::vector<bool>> visited(height, std::vector<bool>(width, 0));
	std::vector<std::vector<bool>> visited2(height, std::vector<bool>(width, 0));
	const int di[8] = { -1, 0, 1, 0, 1, 1, -1, -1 };
	const int dj[8] = { 0, 1, 0, -1, 1, -1, 1, -1 };

	auto inside = [&](std::pair<int, int> cell) {
		if (cell.first < height && cell.first >= 0 && cell.second < width && cell.second >= 0)
			return 1;
		return 0;
		};

	auto inside2 = [&](std::pair<int, int> cell) {
		if (cell.first < height - 1 && cell.first >= 1 && cell.second < width - 1 && cell.second >= 1)
			return 1;
		return 0;
		};

	std::queue<std::pair<int, int>> cellsInQueue;
	std::vector<std::pair<int, int>> candidatesForDoor;
	std::vector<std::pair<int, int>> visitedCells;
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
			if (mapString[i][j][0] == '.' && !visited[i][j]) {
				candidatesForDoor.clear();
				visitedCells.clear();
				int cnt = 0;
				cellsInQueue.push({ i, j });
				visited[i][j] = 1;
				visitedCells.push_back({ i, j });
				while (cellsInQueue.size() > 0) {
					cnt++;
					std::pair<int, int> cell = cellsInQueue.front();
					cellsInQueue.pop();
					for (int k = 0; k < 4; k++) {
						std::pair<int, int> new_cell = { cell.first + di[k], cell.second + dj[k] };
						if (inside(new_cell) && !visited[new_cell.first][new_cell.second]) {
							if (mapString[new_cell.first][new_cell.second][0] == '.') {
								cellsInQueue.push({ new_cell.first, new_cell.second });
								visited[new_cell.first][new_cell.second] = 1;
								visitedCells.push_back({ new_cell.first, new_cell.second });
							}
							else if (mapString[new_cell.first][new_cell.second][0] == 'M') {
								std::queue<std::pair<int, int>> queueForDoor;
								queueForDoor.push(new_cell);
								visited2[new_cell.first][new_cell.second] = 1;
								candidatesForDoor.push_back({ new_cell.first, new_cell.second });
								while (queueForDoor.size() > 0) {
									std::pair<int, int> cell_for_door = queueForDoor.front();
									queueForDoor.pop();
									for (int k2 = 0; k2 < 8; k2++) {
										std::pair<int, int> new_cell_for_door = cell_for_door;
										new_cell_for_door.first += di[k2];
										new_cell_for_door.second += dj[k2];
										if (inside2(new_cell_for_door) && !visited2[new_cell_for_door.first][new_cell_for_door.second] && mapString[new_cell_for_door.first][new_cell_for_door.second] == mapString[new_cell.first][new_cell.second]) {
											visited2[new_cell_for_door.first][new_cell_for_door.second] = 1;
											candidatesForDoor.push_back({ new_cell_for_door.first, new_cell_for_door.second });
											queueForDoor.push(new_cell_for_door);
										}
									}
								}
							}
						}
					}
				}
				for (int k = 0; k < candidatesForDoor.size(); k++) {
					std::pair<int, int> cell = candidatesForDoor[k];
					visited2[cell.first][cell.second] = 0;
				}
				if (cnt < 100) {
					for (int k = 0; k < visitedCells.size(); k++) {
						std::pair<int, int> cell = visitedCells[k];
						enclosed[cell.first][cell.second] = 1;
					}
					std::set<std::pair<int, int>> isCandidateForDoor;
					for (int k = 0; k < candidatesForDoor.size(); k++) {
						std::pair<int, int> cell = candidatesForDoor[k];
						if (cell.first > 1 && cell.first < height - 2 && cell.second > 1 && cell.second < width - 2)
							isCandidateForDoor.insert(candidatesForDoor[k]);
					}

					bool findDoor = false;
					for (int k = 0; k < candidatesForDoor.size(); k++) {
						std::pair<int, int> cell = candidatesForDoor[k];
						if (cell.first > 1 && cell.first < height - 2 && cell.second > 1 && cell.second < width - 2) {
							// check to see if I can place it
							if (isCandidateForDoor.count({ cell.first, cell.second - 1 }) && isCandidateForDoor.count({ cell.first, cell.second + 1 })
								&& !isCandidateForDoor.count({ cell.first - 1, cell.second - 1 }) && !isCandidateForDoor.count({ cell.first - 1, cell.second + 1 }) && !isCandidateForDoor.count({ cell.first - 1, cell.second })
								&& !isCandidateForDoor.count({ cell.first + 1, cell.second - 1 }) && !isCandidateForDoor.count({ cell.first + 1, cell.second + 1 }) && !isCandidateForDoor.count({ cell.first + 1, cell.second})) {
								mapString[cell.first][cell.second] = "D0R9";
								findDoor = true;
								break;
							}
							if (isCandidateForDoor.count({ cell.first - 1, cell.second }) && isCandidateForDoor.count({ cell.first + 1, cell.second })
								&& !isCandidateForDoor.count({ cell.first - 1, cell.second - 1 }) && !isCandidateForDoor.count({ cell.first + 1, cell.second - 1 }) && !isCandidateForDoor.count({ cell.first, cell.second - 1 })
								&& !isCandidateForDoor.count({ cell.first - 1, cell.second + 1 }) && !isCandidateForDoor.count({ cell.first + 1, cell.second + 1 }) && !isCandidateForDoor.count({ cell.first, cell.second + 1 })) {
								mapString[cell.first][cell.second] = "D0R0";
								findDoor = true;
								break;
							}
						}
					}
					if (findDoor == false) {
						for (int k = 0; k < candidatesForDoor.size(); k++) {
							std::pair<int, int> cell = candidatesForDoor[k];
							if (cell.first > 1 && cell.first < height - 2 && cell.second > 1 && cell.second < width - 2) {
								if ((!enclosed[cell.first - 1][cell.second] || !enclosed[cell.first + 1][cell.second])
									&& !isCandidateForDoor.count({ cell.first - 1 , cell.second}) && !isCandidateForDoor.count({ cell.first + 1 , cell.second })) {
									mapString[cell.first][cell.second] = ".0";
									break;
								}
								if (!enclosed[cell.first][cell.second + 1] || !enclosed[cell.first][cell.second - 1]
									&& !isCandidateForDoor.count({ cell.first , cell.second + 1 }) && !isCandidateForDoor.count({ cell.first , cell.second - 1 })) {
									mapString[cell.first][cell.second] = ".0";
									break;
								}
							}
						}
					}
				}
			}
}

void Map::putShopInGoodArea() {
	const std::vector<std::vector<std::string>> pattern = { {".2", ".6", ".1"}, {".8", ".9", ".7"}, {".8", ".9", ".7"}, {".8", ".9", ".7"}, {".4", ".5", ".3"} };

	std::vector<std::pair<int, int>> shopPrefered, shopAnyway;
	for (int i = 0; i < height - 6; i++)
		for (int j = 0; j < width - 3; j++) {
			bool okSP = 1, okSA = 1;
			for (int k = 0; k < 6; k++)
				for (int l = 0; l < 3; l++) {
					if (mapString[i + k][j + l][0] != '.')
						okSP = 0, okSA = 0;
					if (enclosed[i + k][j + l] == 0)
						okSP = 0;
				}
			if (okSP)
				shopPrefered.push_back({ i, j });
			if (okSA)
				shopAnyway.push_back({ i, j });
		}
	std::pair<int, int> positionForShop;
	if (shopPrefered.size() > 0) {
		int ind = Random::randomInt(1, static_cast<int>(shopPrefered.size())) - 1;
		positionForShop = shopPrefered[ind];
	}
	else {
		int ind = Random::randomInt(1, static_cast<int>(shopAnyway.size())) - 1;
		positionForShop = shopAnyway[ind];
	}

	for (int i = 0; i < 5; i++)
		for (int j = 0; j < 3; j++)
			mapString[i + positionForShop.first][j + positionForShop.second] = pattern[i][j];
	mapString[5 + positionForShop.first][1 + positionForShop.second] = "S0";
}

void Map::clearSpawnArea() {
	std::pair<int, int> player_spawn_point = { 10, 10 };
	if (!enclosed[player_spawn_point.first][player_spawn_point.second] && mapString[player_spawn_point.first][player_spawn_point.second][0] == '.')
		return;

	const int di[8] = { -1, 0, 1, 0, 1, 1, -1, -1 };
	const int dj[8] = { 0, 1, 0, -1, 1, -1, 1, -1 };

	auto inside = [&](std::pair<int, int> cell) {
		if (cell.first < height && cell.first >= 0 && cell.second < width && cell.second >= 0)
			return 1;
		return 0;
		};

	auto inside2 = [&](std::pair<int, int> cell) {
		if (cell.first < height - 1 && cell.first >= 1 && cell.second < width - 1 && cell.second >= 1)
			return 1;
		return 0;
		};

	std::pair<int, int> now = player_spawn_point;
	if (enclosed[now.first][now.second]) {
		std::queue<std::pair<int, int>> Q;
		std::vector<std::vector<bool>> visited(height, std::vector<bool>(width, 0));
		Q.push(now);
		visited[now.first][now.second] = 1;
		bool canStop = 0;
		while (Q.size() > 0 && canStop == 0) {
			std::pair<int, int> cur = Q.front();
			Q.pop();
			for (int k = 0; k < 4; k++) {
				std::pair<int, int> cur_new = { cur.first + di[k], cur.second + dj[k] };
				if (inside(cur_new) && (mapString[cur_new.first][cur_new.second][0] == mapString[now.first][now.first][0])) {
					Q.push(cur_new);
					visited[cur_new.first][cur_new.second] = 1;
				}
				if (inside2(cur_new) && (mapString[cur_new.first][cur_new.second][0] == 'M')) {
					canStop = 1;
					now = cur_new;
					break;
				}
			}
		}
	}

	std::vector<std::vector<bool>> visited(height, std::vector<bool>(width, 0));
	std::queue<std::pair<int, int>> Q;
	Q.push(now);
	visited[now.first][now.second] = 1;
	while (Q.size() > 0) {
		std::pair<int, int> cur = Q.front();
		mapString[cur.first][cur.second] = ".0";
		Q.pop();
		for (int k = 0; k < 8; k++) {
			std::pair<int, int> cur_new = { cur.first + di[k], cur.second + dj[k] };
			if (inside2(cur_new) && (mapString[cur_new.first][cur_new.second][0] == 'M'
				|| mapString[cur_new.first][cur_new.second][0] == 'D') && !visited[cur_new.first][cur_new.second]) {
				Q.push(cur_new);
				visited[cur_new.first][cur_new.second] = 1;
			}
		}
	}
}

std::pair<int, int> Map::getRandomAccesiblePosition() {
	std::vector<std::pair<int, int>> accesiblePositions;
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
			if (mapString[i][j][0] == '.' && !enclosed[i][j])
				accesiblePositions.push_back({ i, j });
	int ind = Random::randomInt(1, static_cast<int>(accesiblePositions.size())) - 1;
	return accesiblePositions[ind];
}

std::string Map::generateProceduralMap(const int& w, const int& h) {
	
	width = w;
	height = h;

	mapString.assign(height, std::vector<std::string>(width, "."));

	// Generate Perlin Noise Map
	GeneralUtilities::get().generatePerlinMap(width, height, 10, 256, mapString);

	// Update corners
	for (int i = 0; i < height; i++)
		mapString[i][0] = mapString[i][width - 1] = "M0";

	for (int j = 0; j < width; j++)
		mapString[0][j] = mapString[height - 1][j] = "M0";

	// Put doors where areas are enclosed
	enclosed.assign(height, std::vector<bool>(width, 0));
	putDoorsInEnclosedAreas();

	// Find position for shop
	putShopInGoodArea();

	clearSpawnArea();

	// Gettime sinch epoch in ms
	long long ms_since_epoch = GeneralUtilities::get().getTimeSinceEpochInMs();

	std::string maps_dir = "maps/";
	std::string new_map_name = "sandbox_" + std::to_string(ms_since_epoch);
	std::string output_dir = maps_dir + new_map_name + ".map";

	// Output generated file
	std::ofstream MAP_OUTPUT(output_dir);
	for (int i = 0; i < height; i++, MAP_OUTPUT << "\n "[i == height])
		for (int j = 0; j < width; j++)
			MAP_OUTPUT << mapString[i][j] << ' ';

	return output_dir;
}
