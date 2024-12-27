#include "Map.h"

#include <iostream>
#include <sstream>
#include <queue>

#include "../Renderer/SpriteRenderer.h"
#include "../ResourceManager/ResourceManager.h"

#include "../Entity/Door/Door.h"
#include "../Entity/Floor/Floor.h"
#include "../Entity/Wall/Wall.h"

#include "../Random/Random.h"

std::shared_ptr<Map> Map::instance = nullptr;

Map::Map()
{
	this->genUtil = GeneralUtilities::get();
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

void Map::readMap(const std::string& path)
{
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
			else if (code[0] == 's') {
				this->map.back().emplace_back(std::make_shared<Shop>((double)this->map.back().size() + 0.5, (double)this->map.size() - 0.5, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, code, 10.0, 10.0));
				this->shops.emplace_back(std::dynamic_pointer_cast<Shop>(this->map.back().back()));
			}
		}
	}

	in.close();

	this->mapLoaded = true;
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
}

void Map::update()
{
	for (int i = 0; i < this->doors.size(); ++i)
	{
		this->doors[i]->update();
	}
	
	// shops nu are update, deoarece interactiunile cu player-ul au loc in interactionManager
}

void Map::putDoorsInEnclosedAreas(const int& width, const int& height, std::vector<std::vector<std::string>>& map, std::vector<std::vector<bool>>& enclosed) {
	std::vector<std::vector<bool>> visited(height, std::vector<bool>(width, 0));
	const int di[4] = { -1, 0, 1, 0 };
	const int dj[4] = { 0, 1, 0, -1 };

	auto inside = [&](std::pair<int, int> cell) {
		if (cell.first < height && cell.first >= 0 && cell.second < width && cell.second >= 0)
			return 1;
		return 0;
		};

	std::queue<std::pair<int, int>> cellsInQueue;
	std::vector<std::pair<int, int>> candidatesForDoor;
	std::vector<std::pair<int, int>> visitedCells;
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
			if (map[i][j][0] == '.' && !visited[i][j]) {
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
							if (map[new_cell.first][new_cell.second][0] == '.') {
								cellsInQueue.push({ new_cell.first, new_cell.second });
								visited[new_cell.first][new_cell.second] = 1;
								visitedCells.push_back({ new_cell.first, new_cell.second });
							}
							else if (map[new_cell.first][new_cell.second][0] == 'M')
								candidatesForDoor.push_back({ new_cell.first, new_cell.second });
						}
					}
				}
				if (cnt < 100) {
					for (int k = 0; k < visitedCells.size(); k++) {
						std::pair<int, int> cell = visitedCells[k];
						enclosed[cell.first][cell.second] = 1;
					}
					for (int k = 0; k < candidatesForDoor.size(); k++) {
						std::pair<int, int> cell = candidatesForDoor[k];
						if (cell.first > 0 && cell.first < height - 1 && cell.second > 0 && cell.second < width - 1) {
							map[cell.first][cell.second] = "D0";
							break;
						}
					}
				}
			}
}

void Map::putShopInGoodArea(const int& width, const int& height, std::vector<std::vector<std::string>>& map, const std::vector<std::vector<bool>>& enclosed) {
	const std::vector<std::vector<std::string>> pattern = { {".2", ".6", ".1"}, {".8", ".9", ".7"}, {".8", ".9", ".7"}, {".8", ".9", ".7"}, {".4", ".5", ".3"} };

	std::vector<std::pair<int, int>> shopPrefered, shopAnyway;
	for (int i = 0; i < height - 6; i++)
		for (int j = 0; j < width - 3; j++) {
			bool okSP = 1, okSA = 1;
			for (int k = 0; k < 6; k++)
				for (int l = 0; l < 3; l++) {
					if (map[i + k][j + l][0] != '.')
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
		int ind = Random::randomInt(1, static_cast<int>(shopAnyway.size())) - 1;
		positionForShop = shopPrefered[ind];
	}
	else {
		int ind = Random::randomInt(1, static_cast<int>(shopAnyway.size())) - 1;
		positionForShop = shopAnyway[ind];
	}
	std::cout << positionForShop.first << ' ' << positionForShop.second << '\n';
	for (int i = 0; i < 5; i++)
		for (int j = 0; j < 3; j++)
			map[i + positionForShop.first][j + positionForShop.second] = pattern[i][j];
	map[5 + positionForShop.first][1 + positionForShop.second] = "shop0";
}

std::string Map::generateProceduralMap(const int& width, const int& height) {

	std::vector<std::vector<std::string>> map(height, std::vector<std::string>(width, "."));

	// Generate Perlin Noise Map
	genUtil.generatePerlinMap(width, height, 10, 256, map);

	// Update corners
	for (int i = 0; i < height; i++)
		map[i][0] = map[i][width - 1] = "M0";

	for (int j = 0; j < width; j++)
		map[0][j] = map[height - 1][j] = "M0";

	// Put doors where areas are enclosed
	std::vector<std::vector<bool>> enclosed(height, std::vector<bool>(width, 0));
	putDoorsInEnclosedAreas(width, height, map, enclosed);

	// Find position for shop
	putShopInGoodArea(width, height, map, enclosed);

	// Gettime sinch epoch in ms
	long long ms_since_epoch = genUtil.getTimeSinceEpochInMs();

	std::string maps_dir = "maps/";
	std::string new_map_name = "sandbox_" + std::to_string(ms_since_epoch);
	std::string output_dir = maps_dir + new_map_name + ".map";

	// Output generated file
	std::ofstream MAP_OUTPUT(output_dir);
	for (int i = 0; i < height; i++, MAP_OUTPUT << "\n "[i == height])
		for (int j = 0; j < width; j++)
			MAP_OUTPUT << map[i][j] << ' ';

	return output_dir;
}


