#include "DeadBody.h"

#include "../../Renderer/SpriteRenderer.h"
#include "../../ResourceManager/ResourceManager.h"
#include "../../Camera/Camera.h"

DeadBody::DeadBody(double x, double y, double drawWidth, double drawHeight, double rotateAngle, double speed, const std::map<EntityStatus, std::string>& animationsName2D, const std::vector<EntityStatus>& statuses, const glm::vec3& outfitColor)
	: Entity(x, y, drawWidth, drawHeight, rotateAngle, speed)
	, AnimatedEntity(x, y, drawWidth, drawHeight, rotateAngle, speed, animationsName2D, statuses)
	, outfitColor(outfitColor)
{

}

DeadBody::~DeadBody()
{

}

void DeadBody::draw()
{
	for (int i = 0; i < this->statuses.size(); ++i)
	{
		SpriteRenderer::get().draw(
			outfitColor != glm::vec3(1.0f) ? ResourceManager::getShader("player") : ResourceManager::getShader("sprite"),
			ResourceManager::getFlipbook(this->animationsName2D[this->statuses[i]]).getTextureAtTime(GlobalClock::get().getCurrentTime() - this->timesSinceStatuses[i]),
			Camera::get().screenPosition(this->x, this->y),
			Camera::get().screenSize(this->drawWidth, this->drawHeight),
			this->rotateAngle,
			outfitColor);
	}
}

