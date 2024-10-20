#include "Weapon.h"

#include "../../Game/Game.h"
#include "../../Camera/Camera.h"
#include "../../SoundManager/SoundManager.h"
#include "../../Random/Random.h"

#include "../Player/Player.h"
#include "../../GlobalClock/GlobalClock.h"
#include "../Bullet/Bullet.h"
#include "../Enemy/Enemy.h"
#include "../Bullet/ThrownGrenade.h"

#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <random>

const double Weapon::EPSILON_ANGLE = 30.0; // Macar 30 de grade sa se uite catre inamic atunci cand foloseste pumnul sau cutitul pentru a ii da damage.

Weapon::Weapon(double x, double y, double drawWidth, double drawHeight, double rotateAngle, double speed, const std::string& textureName2D, double interactionWidth, double interactionHeight, double fireRate, int maxBullets, double damage, WeaponType weaponType, double shortRangeAttackRadius, const std::string& reloadSound, const std::string& drawSound, const std::string& emptySound, double price)
	: Entity(x, y, drawWidth, drawHeight, rotateAngle, speed)
	, TexturableEntity(x, y, drawWidth, drawHeight, rotateAngle, speed, textureName2D)
	, InteractiveEntity(x, y, drawWidth, drawHeight, rotateAngle, speed, interactionWidth, interactionHeight)
	, PickUp(x, y, drawWidth, drawHeight, rotateAngle, speed, textureName2D, interactionWidth, interactionHeight)
	, fireRate(fireRate), maxBullets(maxBullets), numBullets(maxBullets), damage(damage), weaponType(weaponType)
	, shortRangeAttackRadius(shortRangeAttackRadius), reloadSound(reloadSound), drawSound(drawSound), emptySound(emptySound)
	, price(price), isReloading(false), timeSinceLastShot(GlobalClock::get().getCurrentTime())
{

}

bool Weapon::isInInteraction()
{
	double left1 = this->getX() - this->interactionWidth / 2.0;
	double right1 = this->getX() + this->interactionWidth / 2.0;
	double left2 = Player::get().getX() - Player::get().getCollideWidth() / 2.0;
	double right2 = Player::get().getX() + Player::get().getCollideWidth() / 2.0;

	double down1 = this->getY() - this->interactionHeight / 2.0;
	double up1 = this->getY() + this->interactionHeight / 2.0;
	double down2 = Player::get().getY() - Player::get().getCollideHeight() / 2.0;
	double up2 = Player::get().getY() + Player::get().getCollideHeight() / 2.0;

	return std::max(left1, left2) <= std::min(right1, right2) && std::max(down1, down2) <= std::min(up1, up2);
}

void Weapon::onInteraction()
{
	
}

void Weapon::onClick()
{
	if (this->isReloading)
	{
		return;
	}

	if (numBullets == 0 && weaponType != WeaponType::FIST && weaponType != WeaponType::KNIFE)
	{
		if (!SoundManager::get().isPlaying(emptySound))
		{
			SoundManager::get().play(emptySound, false);
		}
		return;
	}

	if (GlobalClock::get().getCurrentTime() - this->timeSinceLastShot <= this->fireRate)
	{
		return;
	}

	this->timeSinceLastShot = GlobalClock::get().getCurrentTime();

	if (this->weaponType == WeaponType::FIST || this->weaponType == WeaponType::KNIFE)
	{
		switch (weaponType)
		{
		case WeaponType::FIST:
		{
			SoundManager::get().play("fist", false);

			std::vector<std::shared_ptr<Entity>>& entities = Game::get().getEntities();
			for (int i = 0; i < entities.size(); ++i)
			{
				double deltaXEntity = entities[i]->getX() - Player::get().getX();
				double deltaYEntity = entities[i]->getY() - Player::get().getY();
				double normalizedDeltaXEntity = deltaXEntity / glm::sqrt(deltaXEntity * deltaXEntity + deltaYEntity * deltaYEntity);
				double normalizedDeltaYEntity = deltaYEntity / glm::sqrt(deltaXEntity * deltaXEntity + deltaYEntity * deltaYEntity);
				double deltaXPlayerAngle = glm::cos(glm::radians(Player::get().getRotateAngle()));
				double deltaYPlayerAngle = glm::sin(glm::radians(Player::get().getRotateAngle()));

				if (std::dynamic_pointer_cast<Human>(entities[i]) &&
					normalizedDeltaXEntity * deltaXPlayerAngle + normalizedDeltaYEntity * deltaYPlayerAngle > glm::cos(glm::radians(Weapon::EPSILON_ANGLE)) &&
					(entities[i]->getX() - Player::get().getX()) * (entities[i]->getX() - Player::get().getX()) +
					(entities[i]->getY() - Player::get().getY()) * (entities[i]->getY() - Player::get().getY()) < this->shortRangeAttackRadius * this->shortRangeAttackRadius)
				{
					std::dynamic_pointer_cast<Human>(entities[i])->setHealth(std::max(0.0, std::dynamic_pointer_cast<Human>(entities[i])->getHealth() - this->damage));
				}
			}

			break;
		}
		case WeaponType::KNIFE:
		{
			std::vector<std::shared_ptr<Entity>>& entities = Game::get().getEntities();
			for (int i = 0; i < entities.size(); ++i)
			{
				double deltaXEntity = entities[i]->getX() - Player::get().getX();
				double deltaYEntity = entities[i]->getY() - Player::get().getY();
				double normalizedDeltaXEntity = deltaXEntity / glm::sqrt(deltaXEntity * deltaXEntity + deltaYEntity * deltaYEntity);
				double normalizedDeltaYEntity = deltaYEntity / glm::sqrt(deltaXEntity * deltaXEntity + deltaYEntity * deltaYEntity);
				double deltaXPlayerAngle = glm::cos(glm::radians(Player::get().getRotateAngle()));
				double deltaYPlayerAngle = glm::sin(glm::radians(Player::get().getRotateAngle()));

				if (std::dynamic_pointer_cast<Human>(entities[i]) &&
					normalizedDeltaXEntity * deltaXPlayerAngle + normalizedDeltaYEntity * deltaYPlayerAngle > glm::cos(glm::radians(Weapon::EPSILON_ANGLE)) &&
					(entities[i]->getX() - Player::get().getX()) * (entities[i]->getX() - Player::get().getX()) +
					(entities[i]->getY() - Player::get().getY()) * (entities[i]->getY() - Player::get().getY()) < this->shortRangeAttackRadius * this->shortRangeAttackRadius)
				{
					std::dynamic_pointer_cast<Human>(entities[i])->setHealth(std::max(0.0, std::dynamic_pointer_cast<Human>(entities[i])->getHealth() - this->damage));
				}
			}

			int random_number = Random::randomInt(0, 3);
			switch (random_number)
			{
			case 0:
				SoundManager::get().play("knife_01", false);
				break;

			case 1:
				SoundManager::get().play("knife_02", false);
				break;

			case 2:
				SoundManager::get().play("knife_03", false);
				break;

			case 3:
				SoundManager::get().play("knife_04", false);
				break;

			default:
				break;
			}
		}
			break;
		}
	}
	else // spawn bullet
	{
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(Camera::get().screenPosition(Player::get().getX(), Player::get().getY()), 0.0f));
		model = glm::rotate(model, glm::radians(static_cast<float>(Player::get().getRotateAngle())), glm::vec3(0.0f, 0.0f, 1.0f));

		glm::vec4 bulletRelativeLocation = model * glm::vec4(0.5f, 0.05f, 0.0f, 0.0f);
		glm::vec2 bulletLocation = glm::vec2(
			Player::get().getX() + bulletRelativeLocation.x,
			Player::get().getY() + bulletRelativeLocation.y
		);

		--this->numBullets;

		switch (weaponType)
		{
		case WeaponType::REVOLVER:
			SoundManager::get().play("revolver_01", false);
			Game::get().addEntityForNextFrame(std::make_shared<Bullet>(static_cast<double>(bulletLocation.x), static_cast<double>(bulletLocation.y), 0.3, 0.3, Player::get().getRotateAngle(), 10.0, 0.3, 0.3, "bullet0", this->damage));
			break;

		case WeaponType::SHOTGUN:
			SoundManager::get().play("shotgun_01", false);
			Game::get().addEntityForNextFrame(std::make_shared<Bullet>(static_cast<double>(bulletLocation.x), static_cast<double>(bulletLocation.y), 0.3, 0.3, Player::get().getRotateAngle(), 10.0, 0.3, 0.3, "bullet1", this->damage));
			break;

		case WeaponType::AK47:
			SoundManager::get().play("ak47_01", false);
			Game::get().addEntityForNextFrame(std::make_shared<Bullet>(static_cast<double>(bulletLocation.x), static_cast<double>(bulletLocation.y), 0.3, 0.3, Player::get().getRotateAngle(), 20.0, 0.3, 0.3, "bullet3", this->damage));
			break;

		case WeaponType::M4:
		{
			int random_number = std::rand() % 4;
			switch (random_number)
			{
			case 0:
				SoundManager::get().play("m4a1_01", false);
				break;

			case 1:
				SoundManager::get().play("m4a1_02", false);
				break;

			case 2:
				SoundManager::get().play("m4a1_03", false);
				break;

			case 3:
				SoundManager::get().play("m4a1_04", false);
				break;

			default:
				break;
			}

			Game::get().addEntityForNextFrame(std::make_shared<Bullet>(static_cast<double>(bulletLocation.x), static_cast<double>(bulletLocation.y), 0.3, 0.3, Player::get().getRotateAngle(), 20.0, 0.3, 0.3, "bullet3", this->damage));
		}
			break;

		case WeaponType::MINIGUN:
			SoundManager::get().play("minigun_01", false);
			Game::get().addEntityForNextFrame(std::make_shared<Bullet>(static_cast<double>(bulletLocation.x), static_cast<double>(bulletLocation.y), 0.3, 0.3, Player::get().getRotateAngle(), 20.0, 0.3, 0.3, "bullet0", this->damage));
			break;

		case WeaponType::GRENADE:
		{
			int random_number = Random::randomInt(0, 1);
			switch (random_number)
			{
			case 0:
				SoundManager::get().play("grenadeThrow_01", false);
				break;

			case 1:
				SoundManager::get().play("grenadeThrow_02", false);
				break;

			default:
				break;
			}

			Game::get().addEntityForNextFrame(std::make_shared<ThrownGrenade>(static_cast<double>(bulletLocation.x), static_cast<double>(bulletLocation.y), 0.3, 0.3, Player::get().getRotateAngle(), 3.0, 0.3, 0.3, "grenade0", 0.0, 1.0, this->damage, 15.0, 1.0)); // durata aruncare grenada, damage, scale explozie si durata explozie (ultimii 4 parametrii)

			if (this->numBullets == 0)
			{
				if (Player::get().getTotalBulletsCurrentWeapon() >= 1)
				{
					this->numBullets = 1;
					Player::get().modifyBullets(weaponType, -1);
				}
				else
				{
					Player::get().deleteWeaponFromInventory(this->weaponType);
					Player::get().setCurrentWeaponIndex(0);
				}
			}
		}
			break;
		}
	}
}

void Weapon::update()
{
	if (this->isReloading)
	{
		this->isReloading = SoundManager::get().isPlaying(reloadSound);

		if (!this->isReloading)
		{
			int need = this->maxBullets - this->numBullets;
			int available = std::min(need, Player::get().getTotalBulletsCurrentWeapon());

			this->numBullets += available;
			Player::get().modifyBullets(weaponType, -available); // TODO: face presupunerea ca arma ar apartine unui Player + Player-ul nu mai e Singleton acum
		}
	}
}

void Weapon::drawWeapon()
{
	if (this->drawSound.size() != 0)
	{
		SoundManager::get().play(this->drawSound, false);
	}
}

void Weapon::reload()
{
	if (this->weaponType == WeaponType::FIST || this->weaponType == WeaponType::KNIFE)
	{
		return;
	}

	if (this->isReloading)
	{
		return;
	}

	if (this->numBullets == this->maxBullets)
	{
		return;
	}

	if (Player::get().getTotalBulletsCurrentWeapon() == 0)
	{
		return;
	}

	this->isReloading = true;

	if (!this->reloadSound.empty())
	{
		SoundManager::get().play(reloadSound, false);
	}
}

bool Weapon::recentlyShot() const
{
	return GlobalClock::get().getCurrentTime() - this->timeSinceLastShot < this->fireRate;
}

