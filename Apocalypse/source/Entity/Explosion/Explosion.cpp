#include "Explosion.h"
#include <memory>

#include "../../GlobalClock/GlobalClock.h"
#include "../Human/Human.h"
#include "../Player/Player.h"

Explosion::Explosion(double x, double y, double drawWidth, double drawHeight, double rotateAngle, double speed, double collideWidth, double collideHeight, const std::map<AnimatedEntity::EntityStatus, std::string>& animationsName2D, const std::vector<EntityStatus>& statuses, double explosionDuration, double explosionDamage)
	: Entity(x, y, drawWidth, drawHeight, rotateAngle, speed)
	, CollidableEntity(x, y, drawWidth, drawHeight, rotateAngle, speed, collideWidth, collideHeight)
	, AnimatedEntity(x, y, drawWidth, drawHeight, rotateAngle, speed, animationsName2D, statuses)
	, explosionStarted(GlobalClock::get().getCurrentTime()), explosionDuration(explosionDuration), explosionDamage(explosionDamage)
{

}

Explosion::~Explosion()
{

}

void Explosion::update()
{
	if (GlobalClock::get().getCurrentTime() - this->explosionStarted > this->explosionDuration)
		this->setDeleteEntity(true);
}

void Explosion::onCollide(CollidableEntity& other, glm::vec2 overlap)
{
	if (dynamic_cast<Player*>(&other))
	{
		double appliedDamage = this->explosionDamage * GlobalClock::get().getDeltaTime();
		if (dynamic_cast<Player*>(&other)->getArmor() >= appliedDamage)
		{
			dynamic_cast<Player*>(&other)->setArmor(dynamic_cast<Player*>(&other)->getArmor() - appliedDamage);
			appliedDamage = 0.0;
		}
		else
		{
			appliedDamage -= dynamic_cast<Player*>(&other)->getArmor();
			dynamic_cast<Player*>(&other)->setArmor(0.0);
		}
		if (dynamic_cast<Player*>(&other)->getHealth() >= appliedDamage)
		{
			dynamic_cast<Player*>(&other)->setHealth(dynamic_cast<Player*>(&other)->getHealth() - appliedDamage);
			appliedDamage = 0.0;
		}
		else
		{
			appliedDamage -= dynamic_cast<Player*>(&other)->getHealth();
			dynamic_cast<Player*>(&other)->setHealth(0.0);
		}
	}
	else if (dynamic_cast<Human*>(&other))
	{
		dynamic_cast<Human*>(&other)->setHealth(std::max(0.0, dynamic_cast<Human*>(&other)->getHealth() - this->explosionDamage * GlobalClock::get().getDeltaTime()));
	}
}