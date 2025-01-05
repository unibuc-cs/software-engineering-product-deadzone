#include "RemotePlayer.h"

#include "../../Renderer/SpriteRenderer.h"
#include "../../Renderer/TextRenderer.h"
#include "../../ResourceManager/ResourceManager.h"
#include "../../Camera/Camera.h"

RemotePlayer::RemotePlayer(double x, double y, double drawWidth, double drawHeight, double rotateAngle, double speed, double collideWidth, double collideHeight, const std::map<AnimatedEntity::EntityStatus, std::string>& animationsName2D, const std::vector<EntityStatus>& statuses, double runningSpeed, double health, double stamina, double armor)
	: Entity(x, y, drawWidth, drawHeight, rotateAngle, speed)
	, CollidableEntity(x, y, drawWidth, drawHeight, rotateAngle, speed, collideWidth, collideHeight)
	, AnimatedEntity(x, y, drawWidth, drawHeight, rotateAngle, speed, animationsName2D, statuses)
	, Human(x, y, drawWidth, drawHeight, rotateAngle, speed, collideWidth, collideHeight, animationsName2D, statuses, health, armor)
	, clientName(""), outfitColor(glm::vec3(1.0f, 0.0f, 0.0f))
{

}

RemotePlayer::~RemotePlayer()
{
	// default
}

void RemotePlayer::draw()
{
	for (int i = 0; i < this->statuses.size(); ++i)
	{
		SpriteRenderer::get().draw(ResourceManager::getShader("player"),
			ResourceManager::getFlipbook(this->animationsName2D[this->statuses[i]]).getTextureAtTime(GlobalClock::get().getCurrentTime() - this->timesSinceStatuses[i]),
			Camera::get().screenPosition(this->x, this->y),
			Camera::get().screenSize(this->drawWidth, this->drawHeight),
			this->rotateAngle,
			outfitColor
		);
	}

	glm::vec2 clientNameTextScreenPosition = Camera::get().screenPositionText(this->x, this->y);
	clientNameTextScreenPosition.y -= 45.0f;

	// TODO: de pus limita la numarul de caractere pe care il afisam la clientName

	TextRenderer::get().draw(ResourceManager::getShader("text"),
		ResourceManager::getFont("Antonio"),
		clientName,
		clientNameTextScreenPosition.x, clientNameTextScreenPosition.y,
		0.75f,
		outfitColor
	);
}
