#pragma once

#include <string>

#include "../../GlobalClock/GlobalClock.h"
#include "../PickUp/PickUp.h"

class Weapon : public virtual PickUp
{
public:
	static enum class WeaponType
	{
		FIST,
		KNIFE,
		REVOLVER,
		SHOTGUN,
		AK47,
		M4,
		MINIGUN,
		GRENADE
	};

	static const double EPSILON_ANGLE;

protected:
	double fireRate;
	int numBullets;
	int maxBullets;
	double damage;
	WeaponType weaponType;
	double shortRangeAttackRadius;
	std::string reloadSound;
	std::string drawSound;
	std::string emptySound;
	
	double price;

	bool isReloading;
	double timeSinceLastShot;

public:
	Weapon(double x, double y, double drawWidth, double drawHeight, double rotateAngle, double speed, const std::string& textureName2D, double interactionWidth, double interactionHeight, double fireRate, int maxBullets, double damage, WeaponType weaponType, double shortRangeAttackRadius, const std::string& reloadSound, const std::string& drawSound, const std::string& emptySound, double price = 0);
	virtual ~Weapon() = default;

	virtual bool isInInteraction() override;
	virtual void onInteraction() override;

	inline WeaponType getWeaponType() const { return this->weaponType; }

	virtual void onClick();

	inline bool stillReloading() const { return this->isReloading; }

	virtual void update() override;

	void drawWeapon();
	void reload();

	inline int getBullets() const { return this->numBullets; }

	bool recentlyShot() const;

	inline double getDamage() const { return this->damage; }
	inline double getFireRate() const { return this->fireRate; }
	inline double getPrice() const { return this->price; }
};

