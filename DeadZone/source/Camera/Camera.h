#pragma once

// Camera are un boolean care ii spune daca sa isi mute pozitia unde e player-ul sau nu. Daca e pe false atunci camera implicit se intoarce in pozitia (0.0, 0.0)
// Vom folosi acest lucru pentru meniuri, unde camera nu urmareste niciun player.

#include <glm/vec2.hpp>
#include "../WindowManager/WindowManager.h"

class Camera
{
private:
	Camera(); // release zoom 112.0, debug zoom 128.0
	~Camera() = default;
	Camera(const Camera& other) = delete;
	Camera& operator= (const Camera& other) = delete;
	Camera(const Camera&& other) = delete;
	Camera& operator= (const Camera&& other) = delete;

private:
	double x;
	double y;

	double zoom;

	bool followsPlayer;

public:
	static Camera& get();
	double getX() const { return x; };
	double getY() const { return y; };
	bool getFollowsPlayer() const { return this->followsPlayer; }
	void setFollowsPlayer(bool followsPlayer) { this->followsPlayer = followsPlayer; }
	void update();
	glm::vec2 screenPosition(double x, double y) const;
	glm::vec2 screenSize(double width, double height) const;
	glm::vec2 screenPositionText(double x, double y) const;
};

