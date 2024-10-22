#pragma once

#include "../../Entity/TexturableEntity.h"
#include "../../Entity/Button/Button.h"
#include "../../ButtonGroup/ButtonGroup.h"
#include "../../ButtonBuilder/ButtonBuilder.h"
#include "../MenuBase/MenuBase.h"
#include "../PauseMenu/PauseMenu.h"

class EndScreen : public virtual MenuBase
{
private:

	std::string msg;

	ButtonGroup buttons;

	static EndScreen* instance;

private:
	EndScreen(double x, double y, double drawWidth, double drawHeight, double rotateAngle, double speed, const std::string& textureName2D, const std::string& msg_ = "", const std::string& buttonLabel_ = "Ok", const std::function<void(Button&)>& funcForButton_ = ButtonBuilder::backButtonClickFunction);
	~EndScreen() = default;
	EndScreen(const EndScreen& other) = default;
	EndScreen& operator=(const EndScreen& other) = default;
	// EndScreen(const EndScreen&& other) = default;
	// EndScreen& operator=(const EndScreen&& other) = default;

protected:


public:
	static EndScreen& getCenteredEndScreen(const std::string& msg = "", const std::string& buttonLabel_ = "Ok", const std::function<void(Button&)>& funcForButton_ = PauseMenu::quit);

	void draw() override;

	void setupInputComponent() override;

	void playMenu() override;

};