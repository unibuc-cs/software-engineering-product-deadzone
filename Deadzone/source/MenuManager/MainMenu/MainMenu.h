#pragma once

#include <cctype>

#include "../../Entity/TexturableEntity.h"
#include "../../Entity/Button/Button.h"
#include "../../ButtonGroup/ButtonGroup.h"
#include "../MenuBase/MenuBase.h"

class MainMenu : public virtual MenuBase
{
private:

	double buttonOffsetX = 350;
	double buttonOffsetY = 400;
	double spaceAfterButton = 5;

	ButtonGroup buttons;

	double getButtonPosX();
	double getButtonPosY(int index = 0);

private:
	MainMenu(double x, double y, double drawWidth, double drawHeight, double rotateAngle, double speed, const std::string& textureName2D);
	MainMenu(const std::string& textureName2D); 
	virtual ~MainMenu() = default;
	MainMenu(const MainMenu& other) = delete;
	MainMenu& operator=(const MainMenu& other) = delete;
	MainMenu(const MainMenu&& other) = delete;
	MainMenu& operator=(const MainMenu&& other) = delete;


protected:


public:
	static MainMenu& get();
	void draw() override;

	void setupInputComponent();


	void playMenu() override;

};

