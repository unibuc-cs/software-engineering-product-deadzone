#pragma once

#include "../../Entity/TexturableEntity.h"
#include "../../Entity/Button/Button.h"
#include "../../ButtonGroup/ButtonGroup.h"
#include "../../ButtonBuilder/ButtonBuilder.h"
#include "../MenuBase/MenuBase.h"

class AlertBox : public virtual MenuBase
{
private:

	std::string msg;

	ButtonGroup buttons;

	static AlertBox* instance;

private:
	AlertBox(double x, double y, double drawWidth, double drawHeight, double rotateAngle, double speed, const std::string& textureName2D, const std::string& msg_ = "", const std::string& buttonLabel_ = "Close", const std::function<void(Button&)>& funcForButton_ = ButtonBuilder::backButtonClickFunction);
	~AlertBox() = default;
	AlertBox(const AlertBox& other) = default;
	AlertBox& operator=(const AlertBox& other) = default;
	// AlertBox(const AlertBox&& other) = default;
	// AlertBox& operator=(const AlertBox&& other) = default;

protected:


public:
	static AlertBox& getCenteredAlertBox(const std::string& msg = "", const std::string& buttonLabel_ = "Close", const std::function<void(Button&)>& funcForButton_ = ButtonBuilder::backButtonClickFunction);

	void draw() override;

	void setupInputComponent() override;

	void playMenu() override;

};