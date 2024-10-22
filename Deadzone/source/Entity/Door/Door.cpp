#include "Door.h"

#include "../Player/Player.h"

#include "../../Input/InputHandler.h"
#include "../../ResourceManager/ResourceManager.h"
#include "../../Renderer/TextRenderer.h"
#include "../../Camera/Camera.h"
#include "../../SoundManager/SoundManager.h"

Door::Door(double x, double y, double drawWidth, double drawHeight, double rotateAngle, double speed, double collideWidth, double collideHeight, const std::map<AnimatedEntity::EntityStatus, std::string> animationsName2D, std::vector<EntityStatus> statuses, double interactionWidth, double interactionHeight, int openCost) :
	Entity(x, y, drawWidth, drawHeight, rotateAngle, speed),
	CollidableEntity(x, y, drawWidth, drawHeight, rotateAngle, speed, collideWidth, collideHeight),
	AnimatedEntity(x, y, drawWidth, drawHeight, rotateAngle, speed, animationsName2D, statuses),
	InteractiveEntity(x, y, drawWidth, drawHeight, rotateAngle, speed, interactionWidth, interactionHeight),
	openCost(openCost)
{

}

void Door::onCollide(CollidableEntity& other, glm::vec2 overlap)
{
	// O usa nu face nimic daca e in coliziune.
}

bool Door::isInInteraction()
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

void Door::onInteraction()
{
	if (this->getStatus() != EntityStatus::OPENED && Player::get().getInteractUsed() && Player::get().getGold() >= this->openCost)
	{
		this->updateStatus(EntityStatus::OPENED);

		Player::get().setGold(Player::get().getGold() - this->openCost);

		SoundManager::get().play("door", false);
	}
}

void Door::update()
{
	if (this->getStatus() == EntityStatus::OPENED && ResourceManager::getFlipbook(animationsName2D[this->getStatus()]).getIsFinished(GlobalClock::get().getCurrentTime() - this->getTimeSinceStatus()))
		this->setCollisionActive(false);
}

void Door::draw()
{
	AnimatedEntity::draw();

	if (this->getStatus() == EntityStatus::IDLE)
	{
		std::string openCost = std::to_string(this->getOpenCost());
		TextRenderer::get().draw(ResourceManager::getShader("text"),
			ResourceManager::getFont("Antonio"), openCost,
			static_cast<float>(Camera::get().screenPositionText(this->getX(), this->getY()).x),
			static_cast<float>(Camera::get().screenPositionText(this->getX(), this->getY()).y),
			0.75f, glm::vec3(0.0f, 0.5f, 0.5f));
	}
}

