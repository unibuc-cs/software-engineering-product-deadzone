#pragma once

#include "../CollidableEntity.h"
#include "../AnimatedEntity.h"
#include "../InteractiveEntity.h"

class Door : public virtual CollidableEntity, public virtual AnimatedEntity, public virtual InteractiveEntity
{
protected:
	int openCost;

public:
	Door(double x, double y, double drawWidth, double drawHeight, double rotateAngle, double speed, double collideWidth, double collideHeight, const std::map<AnimatedEntity::EntityStatus, std::string> animationsName2D, std::vector<EntityStatus> statuses, double interactionWidth, double interactionHeight, int openCost);
	virtual ~Door() = default;

	void onCollide(CollidableEntity& other, glm::vec2 overlap) override;

	bool isInInteraction() override;
	void onInteraction() override;

	void update() override;
	void draw() override;

	void openDoor();

	// Getters
	inline int getOpenCost() const { return this->openCost; }
	inline unsigned int getID() const { return id; }

private:
	const unsigned int id;

	static unsigned int globalID;
};

