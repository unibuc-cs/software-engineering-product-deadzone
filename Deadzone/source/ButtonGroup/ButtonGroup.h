#pragma once
#include <map>
#include <iostream>
#include <functional>
#include "../Entity/Button/Button.h"

class noButtonFound : std::runtime_error
{
public:
	noButtonFound(const std::string& msg = "") : runtime_error(msg) {}
};

class ButtonGroup
{
protected:
	std::map<std::string, Button> buttons;
	std::map<std::string, std::function<void(Button&)>> hoverFunctions;
	std::map<std::string, std::function<void(Button&)>> hoverLostFunctions;
	std::map<std::string, std::function<void(Button&)>> clickFunctions;

	// std::map<std::string, bool> buttonIsHovered;

	static std::string any; // key of a function that can be used with any button 

protected:
	bool hoverBox(double mouseX, double mouseY, double x, double y, double width, double height);
	void callHoverFunction(std::string ButtonName, Button& button);
	void callHoverLostFunction(std::string ButtonName, Button& button);
	void callClickFunction(std::string ButtonName, Button& button);


public:
	ButtonGroup() = default;
	ButtonGroup(const std::map<std::string, Button>& buttons_);
	ButtonGroup(const std::map<std::string, Button>& buttons_,
		const std::map<std::string, std::function<void(Button&)>>& hoverFunctions_,
		const std::map<std::string, std::function<void(Button&)>>& hoverLostFunctions_,
		const std::map<std::string, std::function<void(Button&)>>& clickFunctions_);

	void activate();
	void onHover(double x, double y);
	void onClick();

	static std::string getAny() { return any; }

	void draw();
	void draw(float x, float y, float width, float height);

	void setButtons(const std::map<std::string, Button>& buttons_);
	void setFunctions(const std::map<std::string, std::function<void(Button&)>>& hoverFunctions_,
		              const std::map<std::string, std::function<void(Button&)>>& hoverLostFunctions_,
		              const std::map<std::string, std::function<void(Button&)>>& clickFunctions_);

	Button& getButtonByName(const std::string& buttonName);
};

