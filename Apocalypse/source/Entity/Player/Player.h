#pragma once

#include "../Human/Human.h"
#include "../Weapon/Weapon.h"

#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <memory>

class Player : public virtual Human // singleton
{
private:

	Player(double x, double y, double drawWidth, double drawHeight, double rotateAngle, double speed, double collideWidth, double collideHeight, const std::map<AnimatedEntity::EntityStatus, std::string>& animationsName2D, const std::vector<EntityStatus>& statuses, double runningSpeed, double health, double stamina, double armor, int numKills);

	Player(const Player& other) = delete;
	Player& operator= (const Player& other) = delete;
	Player(const Player&& other) = delete;
	Player& operator= (const Player&& other) = delete;

	void moveUp();
	void moveDown();
	void moveRight();
	void moveLeft();
	void run();
	void interact();
	void moveUpReleased();
	void moveDownReleased();
	void moveRightReleased();
	void moveLeftReleased();
	void runReleased();
	void interactReleased();

	void look(double xpos, double ypos);

	void shoot();
	void stopShooting();
	void reload();

	void pauseGame();

public:
	void enterShopButton();
	void enterShopButtonReleased();

private:
	void weaponSlot1();
	void weaponSlot2();
	void weaponSlot3();
	void weaponSlot4();
	void weaponSlot5();
	void weaponSlot6();
	void weaponSlot7();

	double runningSpeed;

	double stamina;
	double staminaChangeSpeed;
	double staminaCap;

	int gold;
	const int goldCap;

	bool moveUpUsed;
	bool moveDownUsed;
	bool moveRightUsed;
	bool moveLeftUsed;
	bool runUsed;
	bool interactUsed;
	bool enterShopUsed;

	double walkingOffsetSize;
	double runningOffsetSize;
	double walkingOffsetSpeed;
	double runningOffsetSpeed;

	std::vector<std::shared_ptr<Weapon>> weapons;
	std::map<Weapon::WeaponType, bool> hasWeapon;

	int currentWeaponIndex;

	std::map<Weapon::WeaponType, int> bullets;
	std::map<Weapon::WeaponType, double> bulletPrices;

	bool isTired;
	bool isWalking;
	bool isRunning;
	bool isShooting;

	int numKills;

	glm::vec3 outfitColor;
	static glm::vec3 outfitColor_static;

	static std::shared_ptr<Player> instance;

	bool hasDied = false;

public:

	virtual ~Player();

	static Player& get();

	void update() override;

	void setupPlayerInputComponent();

	inline double getStamina() const { return this->stamina; }
	inline double getStaminaCap() const { return this->staminaCap; }
	inline int getGold() const { return this->gold; }
	inline int getGoldCap() const { return this->goldCap; }

	inline void setGold(int gold) { this->gold = gold; }

	inline bool getInteractUsed() const { return this->interactUsed; }

	inline std::string getCurrentWeaponTexture() const { return this->weapons[this->currentWeaponIndex]->getTextureName2D(); }

	void draw() override;

	void modifyBullets(Weapon::WeaponType weaponType, int amount);
	inline int getTotalBulletsCurrentWeapon() { return bullets[this->weapons[this->currentWeaponIndex]->getWeaponType()]; }
	inline int getBulletsCurrentWeapon() { return this->weapons[this->currentWeaponIndex]->getBullets(); }
	inline std::map<Weapon::WeaponType, int> getBullets() const { return this->bullets; }

	void save();
	void load();

	inline int getNumKills() const { return this->numKills; }
	inline void setNumKills(int numKills) { this->numKills = numKills; }

	inline glm::vec3 getOutfitColor() { return outfitColor; }
	inline void setOutfitColor(const glm::vec3& outfitColor) { outfitColor_static = outfitColor; this->outfitColor = outfitColor; }
	
	inline std::vector<std::shared_ptr<Weapon>> getWeapons() { return this->weapons; }
	inline std::map<Weapon::WeaponType, bool> getHasWeapon() { return this->hasWeapon; }
	inline bool getHasWeapon(Weapon::WeaponType weaponType) { return this->hasWeapon[weaponType]; }
	inline void setHasWeapon(Weapon::WeaponType weaponType) { hasWeapon[weaponType] = true; }

	inline std::map<Weapon::WeaponType, double> getBulletPrices() const { return this->bulletPrices; }
	inline double getBulletPrice(Weapon::WeaponType weaponType) { return this->bulletPrices[weaponType]; }

	static void deleteInstance();

	inline bool getEnterShopUsed() const { return this->enterShopUsed; }

	void enterShop();

	void setCurrentWeaponIndex(int currentWeaponIndex);
	inline void deleteWeaponFromInventory(Weapon::WeaponType weaponType) { this->hasWeapon[weaponType] = false; }
};

