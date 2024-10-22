#pragma once

#include "../CollidableEntity.h"
#include "../AnimatedEntity.h"

class Human : public virtual CollidableEntity, public virtual AnimatedEntity
{
protected:

	double health;
	double healthCap;

	double armor;
	double armorCap;

public:

	double getHealth() const { return this->health; }
	double getHealthCap() const { return this->healthCap; }
	void setHealth(double health) { this->health = health; }

	Human(double x, double y, double drawWidth, double drawHeight, double rotateAngle, double speed, double collideWidth, double collideHeight, const std::map<AnimatedEntity::EntityStatus, std::string>& animationsName2D, const std::vector<EntityStatus>& statuses, double health, double armor);
	virtual ~Human();

	void onCollide(CollidableEntity& other, glm::vec2 overlap) override;

	inline bool isDead() const { return this->health <= 0.0; }

	inline double getArmor() const { return this->armor; }
	inline double getArmorCap() const { return this->armorCap; }

	inline void setArmor(double armor) { this->armor = armor; }
	inline void fillHealth() { health = this->healthCap; }
	inline void fillArmor() { armor = this->armorCap; }

	inline bool hasMaxHealth() const { return health == this->healthCap; }
	inline bool hasMaxArmor() const { return armor == this->armorCap; }

};