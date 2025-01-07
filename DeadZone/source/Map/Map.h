#pragma once

#include <fstream>
#include <vector>
#include <string>
#include <memory>

#include "../Entity/TexturableEntity.h"
#include "../Entity/AnimatedEntity.h"

#include "../Entity/Door/Door.h"
#include "../Entity/Shop/Shop.h"

#include "../GeneralUtilities/GeneralUtilities.h"

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
	Map();
	Map(const Map& other) = delete;
	Map& operator= (const Map& other) = delete;
	Map(const Map&& other) = delete;
	Map& operator= (const Map&& other) = delete;

	static std::shared_ptr<Map> instance;

private:
	bool mapLoaded = false;

	std::vector<std::shared_ptr<Door>> doors;
	std::vector<std::shared_ptr<Shop>> shops;
	std::vector<std::vector<std::shared_ptr<Entity>>> map;

private:
	static std::vector<std::vector<std::string>> mapString;
	static std::vector<std::vector<bool>> enclosed;
	static int height, width;

public:
	~Map() = default;
	static Map& get();
	void readMapFromFile(const std::string& path);
	void readMapFromBuffer(const std::vector<std::vector<std::string>>& buffer);

	void draw();
	void update();

	void updateDoorStatus(unsigned int id);

	static void deleteInstance();
	
	static void putDoorsInEnclosedAreas();
	static void putShopInGoodArea();
	static std::string generateProceduralMap(const int& width, const int& height);
	static void clearSpawnArea();

	// Getters
	std::vector<std::vector<std::shared_ptr<Entity>>>& getMap() { return this->map; }
	inline std::vector<std::shared_ptr<Door>>& getDoors() { return this->doors; }
	inline std::vector < std::shared_ptr<Shop>>& getShops() { return this->shops; }
	bool getHasBeenLoaded() const { return mapLoaded; }
};

