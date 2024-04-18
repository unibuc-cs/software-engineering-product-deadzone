#pragma once

#include "../Human/Human.h"

#include <glm/vec2.hpp>

class Player : public virtual Human // singleton
{
private:

	Player(double x, double y, double drawWidth, double drawHeight, double rotateAngle, double speed, double collideWidth, double collideHeight, const std::map<AnimatedEntity::EntityStatus, std::string>& animationsName2D, double health, double stamina, double armor, double runningSpeed);
	virtual ~Player();

	Player(const Player& other) = delete;
	Player& operator= (const Player& other) = delete;
	Player(const Player&& other) = delete;
	Player& operator= (const Player&& other) = delete;

	void moveUp();
	void moveDown();
	void moveRight();
	void moveLeft();
	void run();
	void moveUpReleased();
	void moveDownReleased();
	void moveRightReleased();
	void moveLeftReleased();
	void runReleased();

	void look(double xpos, double ypos);

	bool moveUpUsed;
	bool moveDownUsed;
	bool moveRightUsed;
	bool moveLeftUsed;
	bool runUsed;

	double runningSpeed;

public:

	static Player& get();

	void onCollide(CollidableEntity& other, glm::vec2 overlap) override;
	void update() override;

	void setupPlayerInputComponent();
};



