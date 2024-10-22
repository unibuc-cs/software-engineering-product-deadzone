#pragma once

#include <iostream>
#include "../../Entity/Button/Button.h"
#include "../../WindowManager/WindowManager.h"

class MenuBase : public virtual TexturableEntity
{
protected:
	bool isInMenu = false;

	double buttonWidth;
	double buttonHeight;

public:

	MenuBase(double x, double y, double drawWidth, double drawHeight, double rotateAngle, double speed, const std::string& textureName2D, double buttonWidth_ = 0, double buttonHeight_ = 0);

	virtual ~MenuBase() = default;
	virtual void playMenu() = 0;
	virtual void setupInputComponent() = 0;

	double getButtonCoordsX();
	double getButtonCoordsY();

	virtual void setIsInMenu(bool isInMenu_) { isInMenu = isInMenu_; }
	virtual bool getIsInMenu() const { return isInMenu; }

	static std::string toUpper(const std::string& s);
	static std::string toLower(const std::string& s);
	static std::string initCap(const std::string& s);

	static void hoverAnyButton(Button& button);
	static void hoverLostAnyButton(Button& button);
};

