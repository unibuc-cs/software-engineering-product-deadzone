#include "Map.h"

#include <iostream>
#include <sstream>

#include "../Renderer/SpriteRenderer.h"
#include "../ResourceManager/ResourceManager.h"

#include "../Entity/Door/Door.h"
#include "../Entity/Floor/Floor.h"
#include "../Entity/Wall/Wall.h"

std::shared_ptr<Map> Map::instance = nullptr;

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
			}
			else if (code[0] == 's') {
				this->map.back().emplace_back(std::make_shared<Shop>((double)this->map.back().size() + 0.5, (double)this->map.size() - 0.5, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, code, 2.0, 2.0));
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

void Map::addDoor(std::shared_ptr<Door> door)
{
	this->doors.emplace_back(door);
}

void Map::addShop(std::shared_ptr<Shop> const shop)
{
	this->shops.emplace_back(shop);
}

void Map::update()
{
	for (int i = 0; i < this->doors.size(); ++i)
	{
		this->doors[i]->update();
	}
}


