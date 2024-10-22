#pragma once

#include "../../ButtonGroup/ButtonGroup.h"
#include "../MenuBase/MenuBase.h"
#include "../../ButtonBuilder/ButtonBuilder.h"
#include "../../Entity/Weapon/Weapon.h"

class ChangeSkinMenu : public virtual MenuBase
{
private:
	double buttonOffsetX = 50;
	double buttonOffsetY = 50;
	double spaceAfterButton = 20;

	ButtonGroup buttons;
	std::string skinSelected;

	const std::vector<glm::vec3> skinColors{ {0.055f, 0.29f, 0.125f}, {0.59, 0.48, 0.02}, {0.03, 0.02, 0.55}, {0.13, 0.11, 0.12},
									   {0.87, 0.80, 0.72}, {0.46, 0.41, 0.35}, {0.21, 0.18, 0.15}, {0.62, 0.08, 0.14}, {0.31, 0.36, 0.34}
										, {0.41, 0.42, 0.25} , {0.44, 0.32, 0.06} , {0.26, 0.17, 0.49} , {0.58, 0.46, 0.38} };

private:
	ChangeSkinMenu(double x, double y, double drawWidth, double drawHeight, double rotateAngle, double speed, const std::string& textureName2D);
	virtual ~ChangeSkinMenu() = default;
	ChangeSkinMenu(const ChangeSkinMenu& other) = delete;
	ChangeSkinMenu& operator=(const ChangeSkinMenu& other) = delete;
	ChangeSkinMenu(const ChangeSkinMenu&& other) = delete;
	ChangeSkinMenu& operator=(const ChangeSkinMenu&& other) = delete;

	virtual std::map<std::string, Button> loadMenuItems();
	std::map<std::string, std::function<void(Button&)>> generateBuyFunctions();
	void select(int id);

public:
	static ChangeSkinMenu& get();

	void draw();

	void setupInputComponent() override;
	void playMenu() override;

	virtual double getCardPosX();
	virtual double getCardPosY(int index);
};