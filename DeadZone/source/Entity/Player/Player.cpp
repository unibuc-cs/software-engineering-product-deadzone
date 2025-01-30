#include "Player.h"

#include <iostream> // TODO: debug
#include <fstream>
#include <memory>
#include <cmath>

#include <nlohmann/json.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../../WindowManager/WindowManager.h"
#include "../../ResourceManager/ResourceManager.h"
#include "../../Input/InputHandler.h"
#include "../../GlobalClock/GlobalClock.h"
#include "../../Camera/Camera.h"
#include "../../Renderer/SpriteRenderer.h"
#include "../../SoundManager/SoundManager.h"
#include "../../MenuManager/PauseMenu/PauseMenu.h"
#include "../../Game/Game.h"
#include "../Bullet/Bullet.h"
#include "../Wall/Wall.h"
#include "../Door/Door.h"
#include "../../MenuManager/MenuManager.h"
#include "../../MenuManager/ShopMenu/ShopMenu.h"
#include "../../MenuManager/EndScreen/EndScreen.h"
#include "../../Random/Random.h"
#include "../Explosion/Explosion.h"
#include "../../WaveManager/WaveManager.h"
#include "../../Map/Map.h"

std::shared_ptr<Player> Player::instance = nullptr;

const glm::vec3 Player::OUTFIT_COLOR = glm::vec3(0.055f, 0.29f, 0.125f);

const std::map<AnimatedEntity::EntityStatus, std::string> Player::ANIMATIONS_NAME_2D = {
	{ EntityStatus::ARMS_HOLDING_GRENADE, "player0ArmsHoldingGrenade" },
	{ EntityStatus::ARMS_HOLDING_KNIFE, "player0ArmsHoldingKnife" },
	{ EntityStatus::ARMS_HOLDING_PISTOL, "player0ArmsHoldingPistol" },
	{ EntityStatus::ARMS_HOLDING_SHOTGUN, "player0ArmsHoldingShotgun" },
	{ EntityStatus::ARMS_HOLDING_AK47, "player0ArmsHoldingAK47" },
	{ EntityStatus::ARMS_HOLDING_M4, "player0ArmsHoldingM4" },
	{ EntityStatus::ARMS_MOVING_AHEAD, "player0ArmsMovingAhead" },
	{ EntityStatus::ARMS_MOVING_AROUND_WALKING, "player0ArmsMovingAroundWalking" },
	{ EntityStatus::ARMS_MOVING_AROUND_RUNNING, "player0ArmsMovingAroundRunning" },
	{ EntityStatus::ARMS_NOT, "player0ArmsNot" },
	{ EntityStatus::ARMS_RELOADING_PISTOL, "player0ArmsReloadingPistol" },
	{ EntityStatus::ARMS_RELOADING_SHOTGUN, "player0ArmsReloadingShotgun" },
	{ EntityStatus::ARMS_RELOADING_AK47, "player0ArmsReloadingAK47" },
	{ EntityStatus::ARMS_RELOADING_M4, "player0ArmsReloadingM4" },
	{ EntityStatus::ARMS_USING_GRENADE, "player0ArmsUsingGrenade" },
	{ EntityStatus::ARMS_USING_KNIFE, "player0ArmsUsingKnife" },
	{ EntityStatus::ARMS_USING_PISTOL, "player0ArmsUsingPistol" },
	{ EntityStatus::ARMS_USING_SHOTGUN, "player0ArmsUsingShotgun" },
	{ EntityStatus::ARMS_USING_FIST, "player0ArmsUsingFist" },
	{ EntityStatus::ARMS_USING_AK47, "player0ArmsUsingAK47" },
	{ EntityStatus::ARMS_USING_M4, "player0ArmsUsingM4" },
	{ EntityStatus::BODY_IDLE, "player0BodyIdle" },
	{ EntityStatus::HEAD_ANGRY, "player0HeadAngry" },
	{ EntityStatus::HEAD_IDLE, "player0HeadIdle" },
	{ EntityStatus::HEAD_SATISFIED, "player0HeadSatisfied" },
	{ EntityStatus::HEAD_TIRED, "player0HeadTired" },
	{ EntityStatus::LEGS_MOVING_AROUND, "player0LegsMovingAround" },
	{ EntityStatus::LEGS_NOT, "player0LegsNot" }
};

const std::vector<AnimatedEntity::EntityStatus> Player::STATUSES = {
	EntityStatus::LEGS_NOT,
	EntityStatus::ARMS_MOVING_AHEAD,
	EntityStatus::BODY_IDLE,
	EntityStatus::HEAD_IDLE
};

Player::Player(double x, double y, double drawWidth, double drawHeight, double rotateAngle, double speed, double collideWidth, double collideHeight, const std::map<AnimatedEntity::EntityStatus, std::string>& animationsName2D, const std::vector<EntityStatus>& statuses, double runningSpeed, double health = 100.0, double stamina = 100.0, double armor = 0.0, int numKills = 0) :
	Entity(x, y, drawWidth, drawHeight, rotateAngle, speed),
	CollidableEntity(x, y, drawWidth, drawHeight, rotateAngle, speed, collideWidth, collideHeight),
	AnimatedEntity(x, y, drawWidth, drawHeight, rotateAngle, speed, animationsName2D, statuses),
	Human(x, y, drawWidth, drawHeight, rotateAngle, speed, collideWidth, collideHeight, animationsName2D, statuses, health, armor),
	runningSpeed(runningSpeed), stamina(stamina), staminaChangeSpeed(50.0), staminaCap(100.0), gold(1000), goldCap(9999999),
	moveUpUsed(false), moveDownUsed(false), moveRightUsed(false), moveLeftUsed(false), runUsed(false), interactUsed(false), enterShopUsed(false),
	walkingOffsetSize(0.01), runningOffsetSize(0.05),
	walkingOffsetSpeed(10.0), runningOffsetSpeed(15.0),
	weapons({ std::make_shared<Weapon>(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, "fist0", 0.0, 0.0, 0.5, 1, 10.0, Weapon::WeaponType::FIST, 0.75, "", "", "")
		, std::make_shared<Weapon>(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, "knife0", 0.0, 0.0, 0.5, 1, 15.0, Weapon::WeaponType::KNIFE, 0.75, "knifeLook", "knifeDraw", "knifeLook") // knife
		, std::make_shared<Weapon>(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, "pistol0", 0.0, 0.0, 0.5, 20, 25.0, Weapon::WeaponType::REVOLVER, 0.0, "revolverReload", "revolverDraw", "revolverEmpty", 100) // revolver
		, std::make_shared<Weapon>(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, "shotgun0", 0.0, 0.0, 1.0, 4, 50.0, Weapon::WeaponType::SHOTGUN, 0.0, "shotgunReload", "shotgunDraw", "shotgunEmpty", 150) // shotgun
		, std::make_shared<Weapon>(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, "automated1", 0.0, 0.0, 0.3, 25, 35.0, Weapon::WeaponType::AK47, 0.0, "ak47Reload", "ak47Draw", "ak47Empty", 200) // ak47
		, std::make_shared<Weapon>(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, "automated0", 0.0, 0.0, 0.3, 25, 35.0, Weapon::WeaponType::M4, 0.0, "m4a1Reload", "m4a1Draw", "m4a1Empty", 300) // m4
		// , std::make_shared<Weapon>(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, "minigun0", 0.0, 0.0, 0.2, 50, 10.0, Weapon::WeaponType::MINIGUN, 0.0, "minigunReload", "minigunDraw", "minigunEmpty", 500) // minigun
		, std::make_shared<Weapon>(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, "grenade0", 0.0, 0.0, 1.0, 1, 100.0, Weapon::WeaponType::GRENADE, 0.0, "", "grenadeDraw", "grenadeDraw") // grenade
		}),
	hasWeapon({ {Weapon::WeaponType::FIST, true},
		{Weapon::WeaponType::KNIFE, true},
		{Weapon::WeaponType::REVOLVER, true},
		{Weapon::WeaponType::SHOTGUN, false},
		{Weapon::WeaponType::AK47, false},
		{Weapon::WeaponType::M4, false},
		// {Weapon::WeaponType::MINIGUN, false},
		{Weapon::WeaponType::GRENADE, true} }),
	currentWeaponIndex(0),
	isTired(false), isWalking(false), isRunning(false), isShooting(false), numKills(numKills), outfitColor(OUTFIT_COLOR)
{
	bullets[Weapon::WeaponType::REVOLVER] = 60;
	bullets[Weapon::WeaponType::SHOTGUN] = 0;
	bullets[Weapon::WeaponType::AK47] = 0;
	bullets[Weapon::WeaponType::M4] = 0;
	bullets[Weapon::WeaponType::MINIGUN] = 0;
	bullets[Weapon::WeaponType::GRENADE] = 10;

	bulletPrices[Weapon::WeaponType::REVOLVER] = 50;
	bulletPrices[Weapon::WeaponType::SHOTGUN] = 75;
	bulletPrices[Weapon::WeaponType::AK47] = 50;
	bulletPrices[Weapon::WeaponType::M4] = 75;
	bulletPrices[Weapon::WeaponType::MINIGUN] = 50;
	bulletPrices[Weapon::WeaponType::GRENADE] = 200;

	// Load player save.json
	load();

	// TODO: repara
	//if (Map::get().getHasBeenLoaded())
	//{
	//	const std::pair<int, int> pos = Map::getRandomAccesiblePosition();
	//	this->y = pos.first + 0.5f;
	//	this->x = pos.second + 0.5f;
	//}
}

Player& Player::get()
{
	if (Player::instance == nullptr)
	{
		std::pair<double, double> spawn_point = { 10.5, 10.5 };
		Player::instance = std::shared_ptr<Player>(new Player(spawn_point.first, spawn_point.second, 1.0, 1.0, 0.0, 5.0, 0.4, 0.4, ANIMATIONS_NAME_2D, STATUSES, 7.5));
	}

	return *Player::instance;
}

void Player::deleteInstance()
{
	InputHandler::getPlayerInputComponent().clear();

	Player::instance = nullptr;
}

Player::~Player()
{

}

void Player::update()
{
	this->isTired = false;
	this->isWalking = false;
	this->isRunning = false;

	// weapon
	this->weapons[this->currentWeaponIndex]->update();

	if (this->isShooting)
	{
		this->weapons[this->currentWeaponIndex]->onClick();
	}

	// head
	if (this->statuses[3] == EntityStatus::HEAD_TIRED)
	{
		this->isTired = true;

		this->stamina += this->staminaChangeSpeed * GlobalClock::get().getDeltaTime();
		this->stamina = std::min(this->stamina, this->staminaCap);

		if (this->stamina == this->staminaCap)
		{
			this->isTired = false;

			updateStatus(EntityStatus::HEAD_IDLE, 3);
		}
	}

	if (this->isTired)
	{
		updateStatus(EntityStatus::LEGS_NOT, 0);

		if (this->statuses[1] == EntityStatus::ARMS_MOVING_AHEAD ||
			this->statuses[1] == EntityStatus::ARMS_MOVING_AROUND_WALKING ||
			this->statuses[1] == EntityStatus::ARMS_MOVING_AROUND_RUNNING)
			updateStatus(EntityStatus::ARMS_NOT, 1);

		updateStatus(EntityStatus::BODY_IDLE, 2);
	}

	// body (nu avem nimic, o singura animatie de un frame, atat)

	// arms
	if (!this->isTired && (this->moveUpUsed == true || this->moveDownUsed == true
		|| this->moveRightUsed == true || this->moveLeftUsed == true))
	{
		if (this->runUsed == false)
		{
			this->isWalking = true;

			updateStatus(EntityStatus::LEGS_NOT, 0);
			if (this->statuses[1] == EntityStatus::ARMS_MOVING_AHEAD ||
				this->statuses[1] == EntityStatus::ARMS_MOVING_AROUND_WALKING ||
				this->statuses[1] == EntityStatus::ARMS_MOVING_AROUND_RUNNING)
				updateStatus(EntityStatus::ARMS_MOVING_AROUND_WALKING, 1);
			updateStatus(EntityStatus::BODY_IDLE, 2);
		}
		else
		{
			this->isRunning = true;

			updateStatus(EntityStatus::LEGS_MOVING_AROUND, 0);
			if (this->statuses[1] == EntityStatus::ARMS_MOVING_AHEAD ||
				this->statuses[1] == EntityStatus::ARMS_MOVING_AROUND_WALKING ||
				this->statuses[1] == EntityStatus::ARMS_MOVING_AROUND_RUNNING)
				updateStatus(EntityStatus::ARMS_MOVING_AROUND_RUNNING, 1);
			updateStatus(EntityStatus::BODY_IDLE, 2);

			this->stamina -= this->staminaChangeSpeed * GlobalClock::get().getDeltaTime();
			this->stamina = std::max(0.0, this->stamina);

			if (this->stamina == 0.0)
			{
				updateStatus(EntityStatus::HEAD_TIRED, 3);
			}
		}
	}
	else // IDLE
	{
		updateStatus(EntityStatus::LEGS_NOT, 0);
		if (this->statuses[1] == EntityStatus::ARMS_MOVING_AHEAD ||
			this->statuses[1] == EntityStatus::ARMS_MOVING_AROUND_WALKING ||
			this->statuses[1] == EntityStatus::ARMS_MOVING_AROUND_RUNNING)
			updateStatus(EntityStatus::ARMS_MOVING_AHEAD, 1);
		updateStatus(EntityStatus::BODY_IDLE, 2);

		this->stamina += this->staminaChangeSpeed * GlobalClock::get().getDeltaTime();
		this->stamina = std::min(this->stamina, this->staminaCap);
	}

	if (this->weapons[this->currentWeaponIndex]->getWeaponType() == Weapon::WeaponType::FIST)
	{
		// TODO: atac pumn
		if (this->weapons[this->currentWeaponIndex]->recentlyShot())
		{
			updateStatus(EntityStatus::ARMS_USING_FIST, 1);
		}
		else if (this->statuses[1] != EntityStatus::ARMS_MOVING_AHEAD &&
			this->statuses[1] != EntityStatus::ARMS_MOVING_AROUND_WALKING &&
			this->statuses[1] != EntityStatus::ARMS_MOVING_AROUND_RUNNING)
		{
			updateStatus(EntityStatus::ARMS_MOVING_AHEAD, 1);
		}
	}
	else if (this->weapons[this->currentWeaponIndex]->getWeaponType() == Weapon::WeaponType::KNIFE)
	{
		if (this->weapons[this->currentWeaponIndex]->recentlyShot())
		{
			updateStatus(EntityStatus::ARMS_USING_KNIFE, 1);
		}
		else
		{
			updateStatus(EntityStatus::ARMS_HOLDING_KNIFE, 1);
		}
	}
	else if (this->weapons[this->currentWeaponIndex]->getWeaponType() == Weapon::WeaponType::REVOLVER)
	{
		if (!this->weapons[this->currentWeaponIndex]->stillReloading())
		{
			if (this->weapons[this->currentWeaponIndex]->recentlyShot())
			{
				updateStatus(EntityStatus::ARMS_USING_PISTOL, 1);
			}
			else
			{
				updateStatus(EntityStatus::ARMS_HOLDING_PISTOL, 1);
			}
		}
		else
		{
			updateStatus(EntityStatus::ARMS_RELOADING_PISTOL, 1);
		}
	}
	else if (this->weapons[this->currentWeaponIndex]->getWeaponType() == Weapon::WeaponType::SHOTGUN)
	{
		if (!this->weapons[this->currentWeaponIndex]->stillReloading())
		{
			if (this->weapons[this->currentWeaponIndex]->recentlyShot())
			{
				updateStatus(EntityStatus::ARMS_USING_SHOTGUN, 1);
			}
			else
			{
				updateStatus(EntityStatus::ARMS_HOLDING_SHOTGUN, 1);
			}
		}
		else
		{
			updateStatus(EntityStatus::ARMS_RELOADING_SHOTGUN, 1);
		}
	}
	else if (this->weapons[this->currentWeaponIndex]->getWeaponType() == Weapon::WeaponType::AK47)
	{
		if (!this->weapons[this->currentWeaponIndex]->stillReloading())
		{
			if (this->weapons[this->currentWeaponIndex]->recentlyShot())
			{
				updateStatus(EntityStatus::ARMS_USING_AK47, 1);
			}
			else
			{
				updateStatus(EntityStatus::ARMS_HOLDING_AK47, 1);
			}
		}
		else
		{
			updateStatus(EntityStatus::ARMS_RELOADING_AK47, 1);
		}
	}
	else if (this->weapons[this->currentWeaponIndex]->getWeaponType() == Weapon::WeaponType::M4)
	{
		if (!this->weapons[this->currentWeaponIndex]->stillReloading())
		{
			if (this->weapons[this->currentWeaponIndex]->recentlyShot())
			{
				updateStatus(EntityStatus::ARMS_USING_M4, 1);
			}
			else
			{
				updateStatus(EntityStatus::ARMS_HOLDING_M4, 1);
			}
		}
		else
		{
			updateStatus(EntityStatus::ARMS_RELOADING_M4, 1);
		}
	}
	else if (this->weapons[this->currentWeaponIndex]->getWeaponType() == Weapon::WeaponType::GRENADE)
	{
		if (!this->weapons[this->currentWeaponIndex]->stillReloading())
		{
			if (this->weapons[this->currentWeaponIndex]->recentlyShot())
			{
				updateStatus(EntityStatus::ARMS_USING_GRENADE, 1);
			}
			else
			{
				updateStatus(EntityStatus::ARMS_HOLDING_GRENADE, 1);
			}
		}
		else
		{
			//updateStatus(EntityStatus::ARMS_RELOADING_GRENADE, 1);
			//updateStatus(EntityStatus::ARMS_USING_GRENADE, 1);
			updateStatus(EntityStatus::ARMS_HOLDING_GRENADE, 1);
		}
	}
	// else if (this->weapons[this->currentWeaponIndex]->getWeaponType() == Weapon::WeaponType::MINIGUN)	// TODO

	// Sound
	SoundManager::get().pause("walking");
	SoundManager::get().pause("running");
	SoundManager::get().pause("tired");

	if (this->isTired)
	{
		SoundManager::get().resume("tired");
	}
	else if (this->isWalking)
	{
		SoundManager::get().resume("walking");
	}
	else if (this->isRunning)
	{
		SoundManager::get().resume("running");
	}

	if (this->isTired)
	{
		return;
	}

	if (this->statuses[3] != EntityStatus::HEAD_TIRED && !WaveManager::get().getInWave())
	{
		updateStatus(EntityStatus::HEAD_SATISFIED, 3);
	}
	else if (this->statuses[3] != EntityStatus::HEAD_TIRED && this->weapons[this->currentWeaponIndex]->recentlyShot())
	{
		updateStatus(EntityStatus::HEAD_ANGRY, 3);
	}
	else if (this->statuses[3] != EntityStatus::HEAD_TIRED)
	{
		updateStatus(EntityStatus::HEAD_IDLE, 3);
	}

	double currentSpeed = this->speed;

	if (this->isRunning)
		currentSpeed = this->runningSpeed;

	double xOffset = 0.0;
	double yOffset = 0.0;

	if (this->moveRightUsed)
		xOffset += currentSpeed * GlobalClock::get().getDeltaTime();
	if (this->moveLeftUsed)
		xOffset -= currentSpeed * GlobalClock::get().getDeltaTime();
	if (this->moveUpUsed)
		yOffset += currentSpeed * GlobalClock::get().getDeltaTime();
	if (this->moveDownUsed)
		yOffset -= currentSpeed * GlobalClock::get().getDeltaTime();

	if (xOffset != 0.0 && yOffset != 0.0)
	{
		xOffset /= glm::sqrt(2.0);
		yOffset /= glm::sqrt(2.0);
	}

	this->x += xOffset;
	this->y += yOffset;
}

void Player::setupPlayerInputComponent()
{
	InputHandler::getPlayerInputComponent().bindAction("MOVE_UP", InputEvent::IE_Pressed, std::bind(&Player::moveUp, this));
	InputHandler::getPlayerInputComponent().bindAction("MOVE_UP", InputEvent::IE_Released, std::bind(&Player::moveUpReleased, this));

	InputHandler::getPlayerInputComponent().bindAction("MOVE_DOWN", InputEvent::IE_Pressed, std::bind(&Player::moveDown, this));
	InputHandler::getPlayerInputComponent().bindAction("MOVE_DOWN", InputEvent::IE_Released, std::bind(&Player::moveDownReleased, this));

	InputHandler::getPlayerInputComponent().bindAction("MOVE_RIGHT", InputEvent::IE_Pressed, std::bind(&Player::moveRight, this));
	InputHandler::getPlayerInputComponent().bindAction("MOVE_RIGHT", InputEvent::IE_Released, std::bind(&Player::moveRightReleased, this));

	InputHandler::getPlayerInputComponent().bindAction("MOVE_LEFT", InputEvent::IE_Pressed, std::bind(&Player::moveLeft, this));
	InputHandler::getPlayerInputComponent().bindAction("MOVE_LEFT", InputEvent::IE_Released, std::bind(&Player::moveLeftReleased, this));

	InputHandler::getPlayerInputComponent().bindAction("RUN", InputEvent::IE_Pressed, std::bind(&Player::run, this));
	InputHandler::getPlayerInputComponent().bindAction("RUN", InputEvent::IE_Released, std::bind(&Player::runReleased, this));

	InputHandler::getPlayerInputComponent().bindAction("SHOOT", InputEvent::IE_Pressed, std::bind(&Player::shoot, this));
	InputHandler::getPlayerInputComponent().bindAction("SHOOT", InputEvent::IE_Released, std::bind(&Player::stopShooting, this));

	InputHandler::getPlayerInputComponent().bindAction("RELOAD", InputEvent::IE_Pressed, std::bind(&Player::reload, this));

	InputHandler::getPlayerInputComponent().bindAxis(std::bind(&Player::look, this, std::placeholders::_1, std::placeholders::_2));

	InputHandler::getPlayerInputComponent().bindAction("PAUSE", InputEvent::IE_Pressed, std::bind(&Player::pauseGame, this));

	InputHandler::getPlayerInputComponent().bindAction("INTERACT", InputEvent::IE_Pressed, std::bind(&Player::interact, this));
	InputHandler::getPlayerInputComponent().bindAction("INTERACT", InputEvent::IE_Released, std::bind(&Player::interactReleased, this));

	InputHandler::getPlayerInputComponent().bindAction("WEAPON_SLOT_1", InputEvent::IE_Pressed, std::bind(&Player::weaponSlot1, this));
	InputHandler::getPlayerInputComponent().bindAction("WEAPON_SLOT_2", InputEvent::IE_Pressed, std::bind(&Player::weaponSlot2, this));
	InputHandler::getPlayerInputComponent().bindAction("WEAPON_SLOT_3", InputEvent::IE_Pressed, std::bind(&Player::weaponSlot3, this));
	InputHandler::getPlayerInputComponent().bindAction("WEAPON_SLOT_4", InputEvent::IE_Pressed, std::bind(&Player::weaponSlot4, this));
	InputHandler::getPlayerInputComponent().bindAction("WEAPON_SLOT_5", InputEvent::IE_Pressed, std::bind(&Player::weaponSlot5, this));
	InputHandler::getPlayerInputComponent().bindAction("WEAPON_SLOT_6", InputEvent::IE_Pressed, std::bind(&Player::weaponSlot6, this));
	InputHandler::getPlayerInputComponent().bindAction("WEAPON_SLOT_7", InputEvent::IE_Pressed, std::bind(&Player::weaponSlot7, this));

	// shop test
	// InputHandler::getPlayerInputComponent().bindAction("SHOP", InputEvent::IE_Pressed, std::bind(&Player::enterShop, this));
	InputHandler::getPlayerInputComponent().bindAction("SHOP", InputEvent::IE_Pressed, std::bind(&Player::enterShopButton, this));
	InputHandler::getPlayerInputComponent().bindAction("SHOP", InputEvent::IE_Released, std::bind(&Player::enterShopButtonReleased, this));
}

void Player::moveUp()
{
	this->moveUpUsed = true;
}

void Player::moveDown()
{
	this->moveDownUsed = true;
}

void Player::moveRight()
{
	this->moveRightUsed = true;
}

void Player::moveLeft()
{
	this->moveLeftUsed = true;
}

void Player::run()
{
	this->runUsed = true;
}

void Player::interact()
{
	this->interactUsed = true;
}

void Player::moveUpReleased()
{
	this->moveUpUsed = false;
}

void Player::moveDownReleased()
{
	this->moveDownUsed = false;
}

void Player::moveRightReleased()
{
	this->moveRightUsed = false;
}

void Player::moveLeftReleased()
{
	this->moveLeftUsed = false;
}

void Player::runReleased()
{
	this->runUsed = false;
}

void Player::interactReleased()
{
	this->interactUsed = false;
}

void Player::shoot()
{
	this->weapons[this->currentWeaponIndex]->onClick();

	if (4 <= currentWeaponIndex && currentWeaponIndex <= 6)
	{
		this->isShooting = true;
	}
}

void Player::stopShooting()
{
	this->isShooting = false;
}

void Player::reload()
{
	this->weapons[this->currentWeaponIndex]->reload();
}

void Player::look(double xpos, double ypos)
{
	glm::vec2 posCenter = Camera::get().screenPosition(this->x, this->y) + glm::vec2(WindowManager::get().getWindowWidth() / 2.0, WindowManager::get().getWindowHeight() / 2.0);
	double xCenter = posCenter.x;
	double yCenter = posCenter.y;

	double xLungime = std::abs(xpos - xCenter);
	double yLungime = std::abs(ypos - yCenter);

	// cadran raportat la centru
	if (xpos > xCenter && ypos < yCenter) // cadran I
	{
		this->rotateAngle = glm::degrees(glm::atan(yLungime / xLungime));
	}
	else if (xpos < xCenter && ypos < yCenter) // cadran II
	{
		this->rotateAngle = 90.0 + glm::degrees(glm::atan(xLungime / yLungime));
	}
	else if (xpos < xCenter && ypos > yCenter) // cadran III
	{
		this->rotateAngle = 180.0 + glm::degrees(glm::atan(yLungime / xLungime));
	}
	else if (xpos > xCenter && ypos > yCenter) // cadran IV
	{
		this->rotateAngle = 270.0 + glm::degrees(glm::atan(xLungime / yLungime));
	}
}

void Player::pauseGame()
{
	// std::cout << MenuManager::get().size() << "\n";
	MenuManager::get().push(PauseMenu::get());
	// std::cout << MenuManager::get().size() << "\n";
	// PauseMenu::get().setIsInGame(false);
	// PauseMenu::get().setupPauseMenuInputComponent();
	InputHandler::setInputComponent(InputHandler::getMenuInputComponent());
}

void Player::setCurrentWeaponIndex(int currentWeaponIndex)
{
	if (this->hasWeapon[this->weapons[currentWeaponIndex]->getWeaponType()])
	{
		this->isShooting = false;

		if (this->weapons[currentWeaponIndex] != nullptr && this->currentWeaponIndex != currentWeaponIndex)
		{
			weapons[currentWeaponIndex]->drawWeapon();
			this->currentWeaponIndex = currentWeaponIndex;
		}
	}
}

void Player::weaponSlot1()
{
	this->setCurrentWeaponIndex(0);
}

void Player::weaponSlot2()
{
	this->setCurrentWeaponIndex(1);
}

void Player::weaponSlot3()
{
	this->setCurrentWeaponIndex(2);
}

void Player::weaponSlot4()
{
	this->setCurrentWeaponIndex(3);
}

void Player::weaponSlot5()
{
	this->setCurrentWeaponIndex(4);
}

void Player::weaponSlot6()
{
	this->setCurrentWeaponIndex(5);
}

void Player::weaponSlot7()
{
	this->setCurrentWeaponIndex(6);
}

void Player::draw()
{
	if (this->isDead())
	{
		int deadTextureIndex = Random::randomInt(0, 1);
		double deadRotateAngle = (Random::random01() * 360.0 - Random::EPSILON);
		double deadResize = 1.25;

		std::map<AnimatedEntity::EntityStatus, std::string> m0 = {
			{EntityStatus::DEAD_HUMAN, "player" + std::to_string(deadTextureIndex) + "Dead"}
		};
		std::vector<AnimatedEntity::EntityStatus> v0 = { AnimatedEntity::EntityStatus::DEAD_HUMAN };

		// Game::get().addDeadBody(std::make_shared<DeadBody>(this->x, this->y, deadResize * this->drawWidth, deadResize * this->drawHeight, deadRotateAngle, 0.0, m0, v0, outfitColor));

		// this->setDeleteEntity(true);
		
		if (this->hasDied == false)
		{
			MenuManager::get().push(EndScreen::getCenteredEndScreen("You have been killed by zombies"));
			InputHandler::setInputComponent(InputHandler::getMenuInputComponent());
		}

		updateStatus(AnimatedEntity::EntityStatus::DEAD_HUMAN, 0);
		this->hasDied = true;
	}
	else if (this->isWalking)
	{
		for (int i = 0; i < this->statuses.size(); ++i)
			SpriteRenderer::get().draw(ResourceManager::getShader("player"), ResourceManager::getFlipbook(this->animationsName2D[this->statuses[i]]).getTextureAtTime(GlobalClock::get().getCurrentTime() - this->timesSinceStatuses[i]), Camera::get().screenPosition(this->x, this->y), Camera::get().screenSize(this->drawWidth + this->walkingOffsetSize * glm::sin(this->walkingOffsetSpeed * GlobalClock::get().getCurrentTime()), this->drawHeight + this->walkingOffsetSize * glm::sin(this->walkingOffsetSpeed * GlobalClock::get().getCurrentTime())), this->rotateAngle, outfitColor);
	}
	else if (this->isRunning)
	{
		for (int i = 0; i < this->statuses.size(); ++i)
			SpriteRenderer::get().draw(ResourceManager::getShader("player"), ResourceManager::getFlipbook(this->animationsName2D[this->statuses[i]]).getTextureAtTime(GlobalClock::get().getCurrentTime() - this->timesSinceStatuses[i]), Camera::get().screenPosition(this->x, this->y), Camera::get().screenSize(this->drawWidth + this->runningOffsetSize * glm::sin(this->runningOffsetSpeed * GlobalClock::get().getCurrentTime()), this->drawHeight + this->runningOffsetSize * glm::sin(this->runningOffsetSpeed * GlobalClock::get().getCurrentTime())), this->rotateAngle, outfitColor);
	}
	else // IDLE
	{
		for (int i = 0; i < this->statuses.size(); ++i)
		{
			SpriteRenderer::get().draw(ResourceManager::getShader("player"), ResourceManager::getFlipbook(this->animationsName2D[this->statuses[i]]).getTextureAtTime(GlobalClock::get().getCurrentTime() - this->timesSinceStatuses[i]), Camera::get().screenPosition(this->x, this->y), Camera::get().screenSize(this->drawWidth, this->drawHeight), this->rotateAngle, outfitColor);
		}
	}
}

void Player::modifyBullets(Weapon::WeaponType weaponType, int amount)
{
	bullets[weaponType] += amount;
}

void Player::save()
{
	std::ifstream readFile("config/save.json");
	nlohmann::json saveJSON;
	readFile >> saveJSON;
	readFile.close();

	saveJSON["outfitColor"]["r"] = outfitColor.x;
	saveJSON["outfitColor"]["g"] = outfitColor.y;
	saveJSON["outfitColor"]["b"] = outfitColor.z;

	std::ofstream saveFile("config/save.json");
	saveFile << saveJSON.dump(4) << std::endl;
	saveFile.close();
}

void Player::load()
{
	std::ifstream saveFile("config/save.json");

	if (!saveFile.is_open())
	{
		return;
	}

	nlohmann::json saveJSON;
	saveFile >> saveJSON;
	saveFile.close();

	if (saveJSON.contains("outfitColor") && Game::get().getGameMode() != Game::GameMode::TeamDeathMatch)
	{
		setOutfitColor(glm::vec3(
			saveJSON["outfitColor"]["r"].get<double>(),
			saveJSON["outfitColor"]["g"].get<double>(),
			saveJSON["outfitColor"]["b"].get<double>()
		));
	}
}

void Player::enterShopButton()
{
	this->enterShopUsed = true;
}

void Player::enterShopButtonReleased()
{
	this->enterShopUsed = false;
}

void Player::enterShop()
{
	MenuManager::get().push(ShopMenuWeapons::get());
	InputHandler::setInputComponent(InputHandler::getMenuInputComponent());
}

