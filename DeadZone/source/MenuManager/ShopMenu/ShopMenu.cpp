#include "ShopMenu.h"
#include "../../Renderer/SpriteRenderer.h"
#include "../../ResourceManager/ResourceManager.h"
#include "../../Input/InputHandler.h"
#include "../../Map/Map.h"
#include "../../Entity/Player/Player.h"
#include "../../HUD/HUDManager.h"
#include "../MenuManager.h"
#include "../AlertBox/AlertBox.h"
#include <exception>


// Shop menu abstract

ShopMenuAbstract::ShopMenuAbstract(double x, double y, double drawWidth, double drawHeight, double rotateAngle, double speed, const std::string& textureName2D) :
	Entity(x, y, drawWidth, drawHeight, rotateAngle, speed),
	TexturableEntity(x, y, drawWidth, drawHeight, rotateAngle, speed, textureName2D),
	MenuBase(x, y, drawWidth, drawHeight, rotateAngle, speed, textureName2D, drawWidth / 9.0 * 7, drawHeight / 9.0),
	buttons()
{
	
}

double ShopMenuAbstract::getCardPosX() {
	return getButtonCoordsX() + buttonOffsetX;
}

double ShopMenuAbstract::getCardPosY(int index) {
	return getButtonCoordsY() + buttonOffsetY + index * (buttonHeight + spaceAfterButton);
}

void ShopMenuAbstract::draw()
{
	SpriteRenderer::get().draw(ResourceManager::getShader("sprite"), ResourceManager::getTexture(this->textureName2D), glm::vec2(x, y), glm::vec2(drawWidth, drawHeight), 0);

	buttons.draw();
}


void ShopMenuAbstract::setupInputComponent()
{
	buttons.activate();
}

void ShopMenuAbstract::playMenu()
{
	while (isInMenu == true && !glfwWindowShouldClose(WindowManager::get().getWindow()))
	{

		InputHandler::update();

		// Map
		Map::get().draw();

		// Player
		Player::get().draw();

		// HUD
		HUDManager::get().draw();

		MenuManager::get().draw();



		GlobalClock::get().updateTime();

		// Swap the screen buffers
		glfwSwapBuffers(WindowManager::get().getWindow());

		// Check if any events have been activated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
	}
}




// Weapons tab

ShopMenuWeapons::ShopMenuWeapons(double x, double y, double drawWidth, double drawHeight, double rotateAngle, double speed, const std::string& textureName2D) :
	Entity(x, y, drawWidth, drawHeight, rotateAngle, speed),
	TexturableEntity(x, y, drawWidth, drawHeight, rotateAngle, speed, textureName2D),
	MenuBase(x, y, drawWidth, drawHeight, rotateAngle, speed, textureName2D, drawWidth / 9.0 * 7, drawHeight / 9.0),
	ShopMenuAbstract(x, y, drawWidth, drawHeight, rotateAngle, speed, textureName2D)
{
	isInMenu = false;

	// auto allButtons = ButtonBuilder::WeaponCard(getCardPosX(), getCardPosY(0), buttonWidth, buttonHeight, "1", 10, 500, 250, ".0");
	auto allButtons = loadMenuItems();

	allButtons.insert({ "back", ButtonBuilder::backButton(getButtonCoordsX(), getButtonCoordsY()) });

	double tabsWidth = drawWidth;
	double tabsHeight = (WindowManager::get().getWindowHeight() - drawHeight) / 2.0;
	double tabsX = x + (WindowManager::get().getWindowWidth() - drawWidth) / 2.0;
	double tabsY = y + tabsHeight * 2.0;
	auto textures = ButtonBuilder::tabTexturesWeaponsSelected();
	auto tabs = ButtonBuilder::ShopTabsButtons(tabsX, tabsY, tabsWidth, tabsHeight, textures[0], textures[1], textures[2]);

	allButtons.insert(tabs.begin(), tabs.end());

	double titleWidth = drawWidth;
	double titleHeight = (WindowManager::get().getWindowHeight() - drawHeight) / 2.0;
	double titleX = x + (WindowManager::get().getWindowWidth() - drawWidth) / 2.0;
	double titleY = y + tabsHeight;
	auto title = ButtonBuilder::ShopTitleCard(titleX, titleY, titleWidth, titleHeight);

	allButtons.insert(title.begin(), title.end());

	buttons.setButtons(allButtons);

	
	auto clickFunctions = std::map<std::string, std::function<void(Button&)>>{ {ButtonGroup::getAny(), [](Button&) {} },
		{
			"back", ButtonBuilder::backButtonClickFunction
		},
		{
			"Bullets", [](Button&) {
				// ShopMenuWeapons::get().setIsInMenu(false);
				MenuManager::get().replaceTop(ShopMenuBullets::get());
				// MenuManager::get().pop();
				// MenuManager::get().push(ShopMenuBullets::get());
			}
		},
		{
			"Health/Armor", [](Button&) {
			// ShopMenuWeapons::get().setIsInMenu(false);
			MenuManager::get().replaceTop(ShopMenuHealthArmor::get());
			// MenuManager::get().pop();
			// MenuManager::get().push(ShopMenuHealthArmor::get());
}
		}
	};
	
	auto funcs = this->generateBuyFunctions();
	clickFunctions.insert(funcs.begin(), funcs.end());

	buttons.setFunctions(
		std::map<std::string, std::function<void(Button&)>>{{ButtonGroup::getAny(), [](Button&) {} }},
		std::map<std::string, std::function<void(Button&)>>{{ButtonGroup::getAny(), [](Button&) {} }},
		clickFunctions
	);
}

std::map<std::string, Button> ShopMenuWeapons::loadMenuItems()
{
	int id = 1;

	std::map<std::string, Button> rez;

	std::vector<std::shared_ptr<Weapon>> availableWeapons = Player::get().getWeapons();
	std::map<Weapon::WeaponType, bool> hasWeapon = Player::get().getHasWeapon();

	if (availableWeapons.size() != hasWeapon.size())
		throw new std::runtime_error("sizes for availableWeapons and HasWeapon don't match: " + std::to_string(availableWeapons.size()) + " != " + std::to_string(hasWeapon.size()));

	for (size_t i = 2;i < availableWeapons.size();i++) // start from index 2, because fist and knife are always owned by player
	{
		if (availableWeapons[i].get()->getWeaponType() == Weapon::WeaponType::GRENADE)
			continue;

		auto card = ButtonBuilder::WeaponCard(getCardPosX(), getCardPosY(id - 1), buttonWidth, buttonHeight, std::to_string(id), availableWeapons[i].get()->getDamage(), availableWeapons[i].get()->getFireRate(), availableWeapons[i].get()->getPrice(), 250, availableWeapons[i].get()->getTextureName2D());
		rez.insert(card.begin(), card.end());
		
		id++;
	}

	return rez;
}

std::map<std::string, std::function<void(Button&)>> ShopMenuWeapons::generateBuyFunctions()
{
	int id = 1;

	std::map<std::string, std::function<void(Button&)>> rez;

	std::vector<std::shared_ptr<Weapon>> availableWeapons = Player::get().getWeapons();
	std::map<Weapon::WeaponType, bool> hasWeapon = Player::get().getHasWeapon();

	if (availableWeapons.size() != hasWeapon.size())
		throw new std::runtime_error("sizes for availableWeapons and HasWeapon don't match: " + std::to_string(availableWeapons.size()) + " != " + std::to_string(hasWeapon.size()));

	for (size_t i = 2;i < availableWeapons.size();i++) // start from index 2, because fist and knife are always owned by player
	{		
		if (availableWeapons[i].get()->getWeaponType() == Weapon::WeaponType::GRENADE)
			continue;

		Weapon& w = *availableWeapons[i];
		rez[std::to_string(id) + "_1_buy"] = [w](Button&) {ShopMenuWeapons::buy(w);};

		id++;
	}

	return rez;
}

void ShopMenuWeapons::buy(const Weapon& weapon)
{
	int gold = Player::get().getGold();

	if (Player::get().getHasWeapon(weapon.getWeaponType()))
	{
		MenuManager::get().push(AlertBox::getCenteredAlertBox("You already have this weapon."));

		return;
	}

	if (weapon.getPrice() <= gold)
	{
		Player::get().setHasWeapon(weapon.getWeaponType());
		Player::get().setGold(Player::get().getGold() - weapon.getPrice());
	}
	else
	{
		MenuManager::get().push(AlertBox::getCenteredAlertBox("Not enough money"));
	}

}

ShopMenuAbstract& ShopMenuWeapons::get()
{
	double dW = WindowManager::get().getWindowWidth() * 0.8;
	double dH = WindowManager::get().getWindowHeight() * 0.9;
	double x = 0;
	double y = (-WindowManager::get().getWindowHeight() + dH) / 2.0;


	static ShopMenuWeapons instance(x, y, dW, dH, 0, 0, "menuScreen0");
	return instance;
}




// Bullets tab

ShopMenuBullets::ShopMenuBullets(double x, double y, double drawWidth, double drawHeight, double rotateAngle, double speed, const std::string& textureName2D) :
	Entity(x, y, drawWidth, drawHeight, rotateAngle, speed),
	TexturableEntity(x, y, drawWidth, drawHeight, rotateAngle, speed, textureName2D),
	MenuBase(x, y, drawWidth, drawHeight, rotateAngle, speed, textureName2D, drawWidth / 9.0 * 7, drawHeight / 9.0),
	ShopMenuAbstract(x, y, drawWidth, drawHeight, rotateAngle, speed, textureName2D)
{
	isInMenu = false;

	// auto allButtons = ButtonBuilder::WeaponCard(getCardPosX(), getCardPosY(2), buttonWidth, buttonHeight, "1", 10, 500, 250, ".0");
	auto allButtons = loadMenuItems();
	allButtons.insert({ "back", ButtonBuilder::backButton(getButtonCoordsX(), getButtonCoordsY()) });

	double tabsWidth = drawWidth;
	double tabsHeight = (WindowManager::get().getWindowHeight() - drawHeight) / 2.0;
	double tabsX = x + (WindowManager::get().getWindowWidth() - drawWidth) / 2.0;
	double tabsY = y + tabsHeight * 2.0;
	auto textures = ButtonBuilder::tabTexturesBulletsSelected();
	auto tabs = ButtonBuilder::ShopTabsButtons(tabsX, tabsY, tabsWidth, tabsHeight, textures[0], textures[1], textures[2]);

	allButtons.insert(tabs.begin(), tabs.end());

	double titleWidth = drawWidth;
	double titleHeight = (WindowManager::get().getWindowHeight() - drawHeight) / 2.0;
	double titleX = x + (WindowManager::get().getWindowWidth() - drawWidth) / 2.0;
	double titleY = y + tabsHeight;
	auto title = ButtonBuilder::ShopTitleCard(titleX, titleY, titleWidth, titleHeight);

	allButtons.insert(title.begin(), title.end());

	buttons.setButtons(allButtons);

	auto clickFunctions = std::map<std::string, std::function<void(Button&)>>{ {ButtonGroup::getAny(), [](Button&) {} },
		{
			"back", ButtonBuilder::backButtonClickFunction
		},
		{
			"Weapons", [](Button&) {
				// ShopMenuWeapons::get().setIsInMenu(false);
				MenuManager::get().replaceTop(ShopMenuWeapons::get());
				// MenuManager::get().pop();
				// MenuManager::get().push(ShopMenuWeapons::get());
				}
		},
		{
			"Health/Armor", [](Button&) {
			// ShopMenuWeapons::get().setIsInMenu(false);
			MenuManager::get().replaceTop(ShopMenuHealthArmor::get());
			// MenuManager::get().pop();
			// MenuManager::get().push(ShopMenuHealthArmor::get());
			}
		}
	};

	auto funcs = ShopMenuBullets::generateBuyFunctions();
	clickFunctions.insert(funcs.begin(), funcs.end());

	buttons.setFunctions(
		std::map<std::string, std::function<void(Button&)>>{{ButtonGroup::getAny(), [](Button&) {} }},
		std::map<std::string, std::function<void(Button&)>>{{ButtonGroup::getAny(), [](Button&) {} }},
		clickFunctions
	);
}

std::map<std::string, Button> ShopMenuBullets::loadMenuItems()
{
	int id = 1;

	std::map<std::string, Button> rez;

	std::map<Weapon::WeaponType, int> bulletTypes = Player::get().getBullets();
	std::map<Weapon::WeaponType, double> bulletPrices = Player::get().getBulletPrices();

	// if (bulletTypes.size() != bulletPrices.size())
		// throw new std::runtime_error("sizes for availableWeapons and HasWeapon don't match: " + std::to_string(bulletTypes.size()) + " != " + std::to_string(bulletPrices.size()));

	bulletTypes.erase(Weapon::WeaponType::FIST);
	bulletTypes.erase(Weapon::WeaponType::KNIFE);

	for (auto i: bulletTypes)
	{
		std::string icon = mappingWeaponType_BulletTexture[i.first];
		std::string desc = mappingWeaponType_BulletDescription[i.first];
		auto card = ButtonBuilder::BulletsCard(getCardPosX(), getCardPosY(id - 1), buttonWidth, buttonHeight, std::to_string(id), bulletPrices[i.first], icon, desc);
		rez.insert(card.begin(), card.end());

		id++;
	}

	return rez;
}

std::map<std::string, std::function<void(Button&)>> ShopMenuBullets::generateBuyFunctions()
{
	int id = 1;

	std::map<std::string, std::function<void(Button&)>> rez;

	std::map<Weapon::WeaponType, int> bulletTypes = Player::get().getBullets();
	std::map<Weapon::WeaponType, double> bulletPrices = Player::get().getBulletPrices();

	bulletTypes.erase(Weapon::WeaponType::FIST);
	bulletTypes.erase(Weapon::WeaponType::KNIFE);

	// if (bulletTypes.size() != bulletPrices.size())
		// throw new std::runtime_error("sizes for availableWeapons and HasWeapon don't match: " + std::to_string(bulletTypes.size()) + " != " + std::to_string(bulletPrices.size()));

	for (auto i : bulletTypes)
	{
		if(i.first == Weapon::WeaponType::GRENADE)
			rez[std::to_string(id) + "_1_buy"] = [this, i](Button&) {ShopMenuBullets::buy(i.first, 10);};
		else
			rez[std::to_string(id) + "_1_buy"] = [this, i](Button&) {ShopMenuBullets::buy(i.first, amount_bought_once);};

		id++;
	}

	return rez;
}

void ShopMenuBullets::buy(Weapon::WeaponType weaponType, int amount)
{
	int gold = Player::get().getGold();

	double bulletPrice = Player::get().getBulletPrice(weaponType);

	if (bulletPrice <= gold)
	{
		Player::get().modifyBullets(weaponType, amount);
		Player::get().setGold(Player::get().getGold() - bulletPrice);
	}
	else
	{
		MenuManager::get().push(AlertBox::getCenteredAlertBox("Not enough money"));
	}

}

ShopMenuAbstract& ShopMenuBullets::get()
{
	double dW = WindowManager::get().getWindowWidth() * 0.8;
	double dH = WindowManager::get().getWindowHeight() * 0.9;
	double x = 0;
	double y = (-WindowManager::get().getWindowHeight() + dH) / 2.0;


	static ShopMenuBullets instance(x, y, dW, dH, 0, 0, "menuScreen0");
	return instance;
}






// Health/Armor tab

ShopMenuHealthArmor::ShopMenuHealthArmor(double x, double y, double drawWidth, double drawHeight, double rotateAngle, double speed, const std::string& textureName2D) :
	Entity(x, y, drawWidth, drawHeight, rotateAngle, speed),
	TexturableEntity(x, y, drawWidth, drawHeight, rotateAngle, speed, textureName2D),
	MenuBase(x, y, drawWidth, drawHeight, rotateAngle, speed, textureName2D, drawWidth / 9.0 * 7, drawHeight / 9.0),
	ShopMenuAbstract(x, y, drawWidth, drawHeight, rotateAngle, speed, textureName2D)
{
	isInMenu = false;

	// auto allButtons = ButtonBuilder::WeaponCard(getCardPosX(), getCardPosY(4), buttonWidth, buttonHeight, "1", 10, 500, 250, ".0");
	auto allButtons = loadMenuItems();
	allButtons.insert({ "back", ButtonBuilder::backButton(getButtonCoordsX(), getButtonCoordsY()) });

	double tabsWidth = drawWidth;
	double tabsHeight = (WindowManager::get().getWindowHeight() - drawHeight) / 2.0;
	double tabsX = x + (WindowManager::get().getWindowWidth() - drawWidth) / 2.0;
	double tabsY = y + tabsHeight * 2.0;
	auto textures = ButtonBuilder::tabTexturesHealthArmorSelected();
	auto tabs = ButtonBuilder::ShopTabsButtons(tabsX, tabsY, tabsWidth, tabsHeight, textures[0], textures[1], textures[2]);

	allButtons.insert(tabs.begin(), tabs.end());

	double titleWidth = drawWidth;
	double titleHeight = (WindowManager::get().getWindowHeight() - drawHeight) / 2.0;
	double titleX = x + (WindowManager::get().getWindowWidth() - drawWidth) / 2.0;
	double titleY = y + tabsHeight;
	auto title = ButtonBuilder::ShopTitleCard(titleX, titleY, titleWidth, titleHeight);

	allButtons.insert(title.begin(), title.end());

	buttons.setButtons(allButtons);

	buttons.setFunctions(
		std::map<std::string, std::function<void(Button&)>>{{ButtonGroup::getAny(), [](Button&) {} }},
		std::map<std::string, std::function<void(Button&)>>{{ButtonGroup::getAny(), [](Button&) {} }},
		std::map<std::string, std::function<void(Button&)>>{{ButtonGroup::getAny(), [](Button&) {} },
		{
			"1_1_buy", [this](Button&) {
				// std::cout << "Player's health: " << Player::get().getHealth() << "\n";
				if (Player::get().hasMaxHealth())
				{
					MenuManager::get().push(AlertBox::getCenteredAlertBox("You already have max health."));
					return;
				}

				if (this->healthPrice <= Player::get().getGold())
				{
					Player::get().fillHealth();
					Player::get().setGold(Player::get().getGold() - this->healthPrice);
				}
				else
					MenuManager::get().push(AlertBox::getCenteredAlertBox("Not enough money"));
			}
		},
		{
			"2_1_buy", [this](Button&) {
				// std::cout << "Player's armor: " << Player::get().getArmor() << "\n";
				if (Player::get().hasMaxArmor())
				{
					MenuManager::get().push(AlertBox::getCenteredAlertBox("You already have max armor."));
					return;
				}

				if (this->armorPrice <= Player::get().getGold())
				{
					Player::get().fillArmor();
					Player::get().setGold(Player::get().getGold() - this->armorPrice);
				}
				else
					MenuManager::get().push(AlertBox::getCenteredAlertBox("Not enough money"));
			}
		},
		{
			"back", ButtonBuilder::backButtonClickFunction
		},
		{
			"Weapons", [](Button&) {
				// ShopMenuWeapons::get().setIsInMenu(false);
				MenuManager::get().replaceTop(ShopMenuWeapons::get());
				// MenuManager::get().pop();
				// MenuManager::get().push(ShopMenuWeapons::get());
}
		},
		{
			"Bullets", [](Button&) {
				// ShopMenuWeapons::get().setIsInMenu(false);
				MenuManager::get().replaceTop(ShopMenuBullets::get());
				// MenuManager::get().pop();
				// MenuManager::get().push(ShopMenuBullets::get());
}
		}
	}
	);
}

std::map<std::string, Button> ShopMenuHealthArmor::loadMenuItems()
{
	int id = 1;

	std::map<std::string, Button> rez;

	auto card = ButtonBuilder::HealthArmorCard(getCardPosX(), getCardPosY(id-1), buttonWidth, buttonHeight, std::to_string(id), healthPrice, "medicalKit0", "Fully restores Health");
	rez.insert(card.begin(), card.end());

	id++;

	card = ButtonBuilder::HealthArmorCard(getCardPosX(), getCardPosY(id - 1), buttonWidth, buttonHeight, std::to_string(id), healthPrice, "medicalKit0", "Fully restores Armor");
	rez.insert(card.begin(), card.end());

	id++;

	return rez;
}

ShopMenuAbstract& ShopMenuHealthArmor::get()
{
	double dW = WindowManager::get().getWindowWidth() * 0.8;
	double dH = WindowManager::get().getWindowHeight() * 0.9;
	double x = 0;
	double y = (-WindowManager::get().getWindowHeight() + dH) / 2.0;


	static ShopMenuHealthArmor instance(x, y, dW, dH, 0, 0, "menuScreen0");
	return instance;
}

