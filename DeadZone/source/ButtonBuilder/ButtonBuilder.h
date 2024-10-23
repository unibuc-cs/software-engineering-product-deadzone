#pragma once
#include "../Entity/Button/Button.h"
#include <iostream>
#include <vector>

class ButtonBuilder
{
public:
	
	static std::map<Button::Status, std::string> buttonTextures0();

	static Button backButton(double x = 0, double y = 0);
	static void backButtonClickFunction(Button& button);

	static std::map<std::string, Button> WeaponCard(double x, double y, double width, double height, const std::string& id, int dmg, int firerate, int price, int upgradePrice, const std::string& weaponIconTextureName);

	static std::map<std::string, Button> ShopTabsButtons(double x, double y, double totalWidth, double totalHeight, const std::vector<std::string>& texturesWeapons, const std::vector<std::string>& texturesBullets, const std::vector<std::string>& texturesHealthArmor);

	static std::vector<std::vector<std::string>> tabTexturesWeaponsSelected();
	static std::vector<std::vector<std::string>> tabTexturesBulletsSelected();
	static std::vector<std::vector<std::string>> tabTexturesHealthArmorSelected();

	static std::map<std::string, Button> ShopTitleCard(double x, double y, double totalWidth, double totalHeight);


	static std::map<std::string, Button> HealthArmorCard(double x, double y, double width, double height, const std::string& id, int price, const std::string& weaponIconTextureName, const std::string& description = "");

	static std::map<std::string, Button> BulletsCard(double x, double y, double width, double height, const std::string& id, int price, const std::string& bulletIconTextureName, const std::string& description = "");

	static std::map<std::string, Button> ChangeSkinCard(double x, double y, double width, double height, const std::string& id, const glm::vec3& outfitColor);

};