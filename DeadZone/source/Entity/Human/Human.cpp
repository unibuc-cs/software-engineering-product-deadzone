#include "Human.h"
#include "../Wall/Wall.h"
#include "../Door/Door.h"
#include "../Bullet/Bullet.h"
#include "../Explosion/Explosion.h"
#include "../Bullet/ThrownGrenade.h"

#include <iostream>

Human::Human(double x, double y, double drawWidth, double drawHeight, double rotateAngle, double speed, double collideWidth, double collideHeight, const std::map<AnimatedEntity::EntityStatus, std::string>& animationsName2D, const std::vector<EntityStatus>& statuses, double health, double armor = 0.0) :
	Entity(x, y, drawWidth, drawHeight, rotateAngle, speed),
	CollidableEntity(x, y, drawWidth, drawHeight, rotateAngle, speed, collideWidth, collideHeight),
	AnimatedEntity(x, y, drawWidth, drawHeight, rotateAngle, speed, animationsName2D, statuses),
	health(health), healthCap(100.0), armor(armor), armorCap(100.0)
{

}

void Human::applyDamage(double appliedDamage)
{
	if (this->armor >= appliedDamage)
	{
		this->armor -= appliedDamage;
		appliedDamage = 0.0;
	}
	else
	{
		appliedDamage -= this->armor;
		this->armor = 0.0;
	}

	if (this->health >= appliedDamage)
	{
		this->health -= appliedDamage;
		appliedDamage = 0.0;
	}
	else
	{
		appliedDamage -= this->health;
		this->health = 0.0;
	}
}

void Human::onCollide(CollidableEntity& other, glm::vec2 overlap)
{
	if (dynamic_cast<ThrownGrenade*>(&other) != nullptr) // ignoram grenazile in aer
		return;

	if (dynamic_cast<Wall*>(&other) != nullptr || dynamic_cast<Door*>(&other) != nullptr)
	{
		if (overlap.x < overlap.y)
		{
			if (this->x < other.getX())
				this->x -= (overlap.x + CollidableEntity::EPS);
			else
				this->x += (overlap.x + CollidableEntity::EPS);
		}
		else
		{
			if (this->y < other.getY())
				this->y -= (overlap.y + CollidableEntity::EPS);
			else
				this->y += (overlap.y + CollidableEntity::EPS);
		}
	}
	else if (dynamic_cast<Bullet*>(&other) != nullptr)
	{	
		if (overlap.x < overlap.y)
		{
			if (this->x < other.getX())
				this->x -= (overlap.x + CollidableEntity::EPS) / 2.0;
			else
				this->x += (overlap.x + CollidableEntity::EPS) / 2.0;
		}
		else
		{
			if (this->y < other.getY())
				this->y -= (overlap.y + CollidableEntity::EPS) / 2.0;
			else
				this->y += (overlap.y + CollidableEntity::EPS) / 2.0;
		}

		applyDamage(dynamic_cast<Bullet*>(&other)->getDamage());
	}
	else if (dynamic_cast<Explosion*>(&other) != nullptr)
	{
		applyDamage(dynamic_cast<Explosion*>(&other)->getExplosionDamage() * GlobalClock::get().getDeltaTime());
	}
	else if (dynamic_cast<CollidableEntity*>(&other) != nullptr)
	{
		if (overlap.x < overlap.y)
		{
			if (this->x < other.getX())
				this->x -= (overlap.x + CollidableEntity::EPS) / 2.0;
			else
				this->x += (overlap.x + CollidableEntity::EPS) / 2.0;
		}
		else
		{
			if (this->y < other.getY())
				this->y -= (overlap.y + CollidableEntity::EPS) / 2.0;
			else
				this->y += (overlap.y + CollidableEntity::EPS) / 2.0;
		}
	}
}

Human::~Human()
{

}

