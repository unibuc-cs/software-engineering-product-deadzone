#include "BulletBlast.h"

#include "../../ResourceManager/ResourceManager.h"
#include "../../GlobalClock/GlobalClock.h"

BulletBlast::BulletBlast(double x, double y, double drawWidth, double drawHeight, double rotateAngle, const std::map<AnimatedEntity::EntityStatus, std::string>& animationsName2D, const std::vector<AnimatedEntity::EntityStatus>& statuses)
	: Entity(x, y, drawWidth, drawHeight, rotateAngle, 0.0)
	, AnimatedEntity(x, y, drawWidth, drawHeight, rotateAngle, 0.0, animationsName2D, statuses)
{

}

void BulletBlast::update()
{
	if (ResourceManager::getFlipbook(animationsName2D[this->getStatus()]).getIsFinished(GlobalClock::get().getCurrentTime() - this->getTimeSinceStatus()))
		setDeleteEntity(true);
}

