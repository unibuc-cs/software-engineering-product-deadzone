#pragma once

#include <cctype>

#include "../../Entity/TexturableEntity.h"
#include "../../Entity/Button/Button.h"
#include "../../ButtonGroup/ButtonGroup.h"
#include "../MenuBase/MenuBase.h"

class PauseMenu : public virtual MenuBase
{
private:

	double buttonOffsetX = 200;
	double buttonOffsetY = 250;
	double spaceAfterButton = 5;

	ButtonGroup buttons;

	double getButtonPosX();
	double getButtonPosY(int index = 0);


private:
	PauseMenu(double x, double y, double drawWidth, double drawHeight, double rotateAngle, double speed, const std::string& textureName2D);
	PauseMenu(const std::string& textureName2D);
	~PauseMenu() = default;
	PauseMenu(const PauseMenu& other) = delete;
	PauseMenu& operator=(const PauseMenu& other) = delete;
	PauseMenu(const PauseMenu&& other) = delete;
	PauseMenu& operator=(const PauseMenu&& other) = delete;

protected:
	

public:
	static PauseMenu& get();
	void draw() override;

	void setupInputComponent() override;

	void playMenu() override;

	static void quit(Button&);

};

