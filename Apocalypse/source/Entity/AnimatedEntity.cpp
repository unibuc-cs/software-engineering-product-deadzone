#include "AnimatedEntity.h"

#include "../Renderer/SpriteRenderer.h"
#include "../ResourceManager/ResourceManager.h"

#include "../GlobalClock/GlobalClock.h"

#include "../Camera/Camera.h"

#include <iostream>
#include <memory>

AnimatedEntity::AnimatedEntity(double x, double y, double drawWidth, double drawHeight, double rotateAngle, double speed, const std::map<EntityStatus, std::string>& animationsName2D, const std::vector<EntityStatus>& statuses) :
	Entity(x, y, drawWidth, drawHeight, rotateAngle, speed), animationsName2D(animationsName2D), statuses(statuses)
{
	this->timesSinceStatuses.resize(statuses.size());
	for (int i = 0; i < statuses.size(); ++i)
		this->timesSinceStatuses[i] = GlobalClock::get().getCurrentTime();
}

void AnimatedEntity::draw()
{
	for (int i = 0; i < this->statuses.size(); ++i)
	{
		SpriteRenderer::get().draw(ResourceManager::getShader("sprite"),
			ResourceManager::getFlipbook(this->animationsName2D[this->statuses[i]]).getTextureAtTime(GlobalClock::get().getCurrentTime() - this->timesSinceStatuses[i]),
			Camera::get().screenPosition(this->x, this->y),
			Camera::get().screenSize(this->drawWidth, this->drawHeight),
			this->rotateAngle);
	}
}

void AnimatedEntity::updateStatus(EntityStatus newStatus, int index)
{
	if (this->statuses[index] != newStatus)
	{
		this->timesSinceStatuses[index] = GlobalClock::get().getCurrentTime();
	}

	this->statuses[index] = newStatus;
}

AnimatedEntity::~AnimatedEntity()
{

}

