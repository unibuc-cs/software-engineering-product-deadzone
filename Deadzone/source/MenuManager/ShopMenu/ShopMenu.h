#pragma once

#include "../../ButtonGroup/ButtonGroup.h"
#include "../MenuBase/MenuBase.h"
#include "../../ButtonBuilder/ButtonBuilder.h"
#include "../../Entity/Weapon/Weapon.h"

// Abstract shop menu

class ShopMenuAbstract : public virtual MenuBase
{
protected:
	double buttonOffsetX = 50;
	double buttonOffsetY = 50;
	double spaceAfterButton = 20;

	ButtonGroup buttons;

protected:
	ShopMenuAbstract(double x, double y, double drawWidth, double drawHeight, double rotateAngle, double speed, const std::string& textureName2D);
	virtual ~ShopMenuAbstract() = default;
	// ShopMenuAbstract(const ShopMenuAbstract& other) = default;
	// ShopMenuAbstract& operator=(const ShopMenuAbstract& other) = default;
	// ShopMenuAbstract(const ShopMenuAbstract&& other) = default;
	// ShopMenuAbstract& operator=(const ShopMenuAbstract&& other) = default;

	virtual std::map<std::string, Button> loadMenuItems() = 0;

public:
	void draw();

	void setupInputComponent() override;
	void playMenu() override;

	virtual double getCardPosX();
	virtual double getCardPosY(int index);
};





// Weapons tab

class ShopMenuWeapons : public virtual ShopMenuAbstract
{
private:

private:
	ShopMenuWeapons(double x, double y, double drawWidth, double drawHeight, double rotateAngle, double speed, const std::string& textureName2D);
	virtual ~ShopMenuWeapons() = default;
	ShopMenuWeapons(const ShopMenuWeapons& other) = delete;
	ShopMenuWeapons& operator=(const ShopMenuWeapons& other) = delete;
	ShopMenuWeapons(const ShopMenuWeapons&& other) = delete;
	ShopMenuWeapons& operator=(const ShopMenuWeapons&& other) = delete;

	virtual std::map<std::string, Button> loadMenuItems() override;

	static void buy(const Weapon& weapon);

	std::map<std::string, std::function<void(Button&)>> generateBuyFunctions();

public:
	static ShopMenuAbstract& get();
};





// Bullets tab

class ShopMenuBullets : public virtual ShopMenuAbstract
{
private:
	const int amount_bought_once = 100;

	std::map<Weapon::WeaponType, std::string> mappingWeaponType_BulletTexture{
		{Weapon::WeaponType::REVOLVER, "bullet0"},
		{Weapon::WeaponType::SHOTGUN, "bullet1"},
		{Weapon::WeaponType::AK47, "bullet3"},
		{Weapon::WeaponType::M4, "bullet3"},
		{Weapon::WeaponType::MINIGUN, "bullet0"},
		{Weapon::WeaponType::GRENADE, "grenade0"},

	};

	std::map<Weapon::WeaponType, std::string> mappingWeaponType_BulletDescription{
		{Weapon::WeaponType::REVOLVER, "For Revolver"},
		{Weapon::WeaponType::SHOTGUN, "For Shotgun"},
		{Weapon::WeaponType::AK47, "For AK-47"},
		{Weapon::WeaponType::M4, "For M4"},
		{Weapon::WeaponType::MINIGUN, "For Minigun"},
		{Weapon::WeaponType::GRENADE, "Grenade"},

	};

private:
	ShopMenuBullets(double x, double y, double drawWidth, double drawHeight, double rotateAngle, double speed, const std::string& textureName2D);
	virtual ~ShopMenuBullets() = default;
	ShopMenuBullets(const ShopMenuBullets& other) = delete;
	ShopMenuBullets& operator=(const ShopMenuBullets& other) = delete;
	ShopMenuBullets(const ShopMenuBullets&& other) = delete;
	ShopMenuBullets& operator=(const ShopMenuBullets&& other) = delete;

	virtual std::map<std::string, Button> loadMenuItems() override;

	std::map<std::string, std::function<void(Button&)>> generateBuyFunctions();
	static void buy(Weapon::WeaponType weaponType, int amount);

public:
	static ShopMenuAbstract& get();
};





// Health/Armor tab

class ShopMenuHealthArmor : public virtual ShopMenuAbstract
{
private:
	double healthPrice = 50.0;
	double armorPrice = 100.0;

private:
	ShopMenuHealthArmor(double x, double y, double drawWidth, double drawHeight, double rotateAngle, double speed, const std::string& textureName2D);
	virtual ~ShopMenuHealthArmor() = default;
	ShopMenuHealthArmor(const ShopMenuHealthArmor& other) = delete;
	ShopMenuHealthArmor& operator=(const ShopMenuHealthArmor& other) = delete;
	ShopMenuHealthArmor(const ShopMenuHealthArmor&& other) = delete;
	ShopMenuHealthArmor& operator=(const ShopMenuHealthArmor&& other) = delete;

	virtual std::map<std::string, Button> loadMenuItems() override;

public:
	static ShopMenuAbstract& get();
};