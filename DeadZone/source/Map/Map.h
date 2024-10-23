#pragma once

#include <fstream>
#include <vector>
#include <string>
#include <memory>

#include "../Entity/TexturableEntity.h"
#include "../Entity/AnimatedEntity.h"

#include "../Entity/Door/Door.h"
#include "../Entity/Shop/Shop.h"

/*
* Codificare:
* 
* M0 M1 M2 M3 ... M9 MA MB MC ... = coduri de perete
* .0 .1 .2 .3 ... .9 .a .b .c ... = coduri de podea
* D0 D1 D2 D3 ... D9 DA DB DC ... = coduri de usi
*/

class Map
{
private:
	Map() = default;
	Map(const Map& other) = delete;
	Map& operator= (const Map& other) = delete;
	Map(const Map&& other) = delete;
	Map& operator= (const Map&& other) = delete;

	static std::shared_ptr<Map> instance;

private:
	bool mapLoaded = false;

	std::vector<std::vector<std::shared_ptr<Entity>>> map;
	std::vector<std::shared_ptr<Door>> doors;
	std::vector<std::shared_ptr<Shop>> shops;

public:
	~Map() = default;
	static Map& get();
	void readMap(const std::string& path);
	std::vector<std::vector<std::shared_ptr<Entity>>>& getMap() { return this->map; }
	void draw();
	void addDoor(std::shared_ptr<Door> const door);
	void addShop(std::shared_ptr<Shop> const shop);
	inline std::vector<std::shared_ptr<Door>>& getDoors() { return this->doors; }
	inline std::vector < std::shared_ptr<Shop>>& getShops() { return this->shops; }
	void update();
	static void deleteInstance();

	bool hasBeenLoaded() const { return mapLoaded; }
};

