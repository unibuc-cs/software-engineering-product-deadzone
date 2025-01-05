#include "ThrownGrenade.h"

#include "../../GlobalClock/GlobalClock.h"
#include "../../Game/Game.h"
#include "../Explosion/Explosion.h"
#include "../../Random/Random.h"
#include "../../SoundManager/SoundManager.h"

#include "../Wall/Wall.h"
#include "../Door/Door.h"
#include "../Shop/Shop.h"

#include <vector>
#include <map>

const double ThrownGrenade::maximumSizeIncreaseAnimation = 3.0;

ThrownGrenade::ThrownGrenade(double x, double y, double drawWidth, double drawHeight, double rotateAngle, double speed, double collideWidth, double collideHeight, const std::string& textureName2D, double damage, double timeUntilExplosion, double explosionDamage, double explosionScale, double explosionDuration)
	: Entity(x, y, drawWidth, drawHeight, rotateAngle, speed)
	, CollidableEntity(x, y, drawWidth, drawHeight, rotateAngle, speed, collideWidth, collideHeight)
	, TexturableEntity(x, y, drawWidth, drawHeight, rotateAngle, speed, textureName2D)
	, Bullet(x, y, drawWidth, drawHeight, rotateAngle, speed, collideWidth, collideHeight, textureName2D, damage)
	, timeUntilExplosion(timeUntilExplosion), explosionDamage(explosionDamage), timeThrown(GlobalClock::get().getCurrentTime())
	, originalDrawWidth(drawWidth), originalDrawHeight(drawHeight), originalCollideWidth(collideWidth), originalCollideHeight(collideHeight)
	, explosionScale(explosionScale), explosionDuration(explosionDuration)
{

}

ThrownGrenade::~ThrownGrenade()
{

}

void ThrownGrenade::update()
{
	Bullet::update();

	double scaler = this->maximumSizeIncreaseAnimation - 1;

	double currentScale = ((GlobalClock::get().getCurrentTime() - timeThrown - (timeUntilExplosion / 2.0)) / (timeUntilExplosion / 2.0)) + 1.0;

	if (currentScale < 1.0)
	{
		this->drawWidth = this->originalDrawWidth + this->originalDrawWidth * scaler * currentScale;
		this->drawHeight = this->originalDrawHeight + this->originalDrawHeight * scaler * currentScale;
		this->collideWidth = this->originalCollideWidth + this->originalCollideWidth * scaler * currentScale;
		this->collideHeight = this->originalCollideHeight + this->originalCollideHeight * scaler * currentScale;
	}
	else
	{
		currentScale -= 1.0;
		this->drawWidth = this->originalDrawWidth + this->originalDrawWidth * scaler * (1.0 - currentScale);
		this->drawHeight = this->originalDrawHeight + this->originalDrawHeight * scaler * (1.0 - currentScale);
		this->collideWidth = this->originalCollideWidth + this->originalCollideWidth * scaler * (1.0 - currentScale);
		this->collideHeight = this->originalCollideHeight + this->originalCollideHeight * scaler * (1.0 - currentScale);
	}


	if (GlobalClock::get().getCurrentTime() - timeThrown > timeUntilExplosion)
	{
		// detonate sound effect
		int random_number = Random::randomInt(0, 2);
		switch (random_number)
		{
		case 0:
			SoundManager::get().play("grenadeDetonate_01", false, false);
			break;

		case 1:
			SoundManager::get().play("grenadeDetonate_02", false, false);
			break;

		case 2:
			SoundManager::get().play("grenadeDetonate_03", false, false);
			break;

		default:
			break;
		}

		// apare o explozie cand dispare grenada aruncata
		double randomExplosionAngle = Random::random01() * (360.0 - Random::EPSILON);

		std::map<AnimatedEntity::EntityStatus, std::string> m = { { AnimatedEntity::EntityStatus::IDLE, "grenadeExplosion" } };
		std::vector<AnimatedEntity::EntityStatus> v = { AnimatedEntity::EntityStatus::IDLE };

		Game::get().addEntityForNextFrame(std::make_shared<Explosion>(this->x, this->y,
			this->originalDrawWidth * this->explosionScale, this->originalDrawHeight * this->explosionScale,
			randomExplosionAngle, 0.0, this->originalCollideWidth * this->explosionScale, this->originalCollideHeight * this->explosionScale,
			m, v, this->explosionDuration, this->explosionDamage));

		this->setDeleteEntity(true);
	}
}

void ThrownGrenade::onCollide(CollidableEntity& other, glm::vec2 overlap)
{
	// Facem coliziune doar cu Wall/Door/Shop, atat
	if (dynamic_cast<Wall*>(&other) != nullptr
		&& dynamic_cast<Door*>(&other) != nullptr
		&& dynamic_cast<Shop*>(&other) != nullptr)
		return;

	double normalizedX = glm::cos(glm::radians(this->getRotateAngle()));
	double normalizedY = glm::sin(glm::radians(this->getRotateAngle()));

	if (overlap.x < overlap.y)
	{
		if (normalizedX * (other.getX() - this->x) > 0.0)
			normalizedX = -normalizedX;
	}
	else
	{
		if (normalizedY * (other.getY() - this->y) > 0.0)
			normalizedY = -normalizedY;
	}

	this->rotateAngle = glm::degrees(std::atan2f(normalizedY, normalizedX));
}

