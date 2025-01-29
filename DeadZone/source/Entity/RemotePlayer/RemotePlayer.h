#pragma once

#include <glm/glm.hpp>

#include <memory>

#include "../Human/Human.h"
#include "../DeadBody/DeadBody.h"

class RemotePlayer : public virtual Human
{
private:
public:
	// Constructor
	RemotePlayer(double x, double y, double drawWidth, double drawHeight, double rotateAngle, double speed, double collideWidth, double collideHeight, const std::map<AnimatedEntity::EntityStatus, std::string>& animationsName2D, const std::vector<EntityStatus>& statuses, double runningSpeed, double health = 100.0, double stamina = 100.0, double armor = 0.0);

	// Destructor
	~RemotePlayer();

	virtual void draw() override;

	// Getters
	inline std::string getClientName() const { return clientName; }
	inline glm::vec3 getOutfitColor() const { return outfitColor; }

	// Setters
	inline void setClientName(const std::string& name) { clientName = name; }
	inline void setOutfitColor(const glm::vec3& color) { outfitColor = color; }
	inline int getTeam() { return team; }
	inline void setTeam(int _team) { team = _team; }

private:
	std::string clientName;
	glm::vec3 outfitColor;
	int team;

	std::shared_ptr<DeadBody> deadBody;
};
