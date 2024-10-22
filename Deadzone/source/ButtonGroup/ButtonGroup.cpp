#include "ButtonGroup.h"
#include "../Input/InputHandler.h"

std::string ButtonGroup::any = "any";

ButtonGroup::ButtonGroup(const std::map<std::string, Button>& buttons_): buttons(buttons_){}

ButtonGroup::ButtonGroup(const std::map<std::string, Button>& buttons_,
	const std::map<std::string, std::function<void(Button&)>>& hoverFunctions_,
	const std::map<std::string, std::function<void(Button&)>>& hoverLostFunctions_,
	const std::map<std::string, std::function<void(Button&)>>& clickFunctions_) : buttons(buttons_),
																		   hoverFunctions(hoverFunctions_),
	                                                                       hoverLostFunctions(hoverLostFunctions_),
	                                                                       clickFunctions(clickFunctions_) {}

void ButtonGroup::activate()
{
	InputHandler::getMenuInputComponent().replaceAxis(std::bind(&ButtonGroup::onHover, this, std::placeholders::_1, std::placeholders::_2));
	InputHandler::getMenuInputComponent().replaceAction("CLICK_BUTTON", InputEvent::IE_Pressed, std::bind(&ButtonGroup::onClick, this));
}

void ButtonGroup::onHover(double x, double y)
{
	for (auto& i : buttons)
	{
		bool buttonIsHovered = hoverBox(x, y, i.second.getX(), i.second.getY(), i.second.getCollideWidth(), i.second.getCollideHeight());
		if (buttonIsHovered)
			i.second.setHovered();
		else
			i.second.setDefault();

		if (buttonIsHovered)
			callHoverFunction(i.first, i.second);
		else
			callHoverLostFunction(i.first, i.second);
	}
}

void ButtonGroup::onClick()
{
	for(auto& i = buttons.rbegin(); i != buttons.rend(); ++i)
		if (i->second.getStatus() == Button::Status::HOVERED)
		{
			// buttonIsHovered[i.first] = false;
			callClickFunction(i->first, i->second);
			break; // TODO: e necesar? (daca butoanele nu au overlap, unul singur va avea hover la un moment dat)
		}
}


bool ButtonGroup::hoverBox(double mouseX, double mouseY, double x, double y, double width, double height)
{
	return mouseX >= x && mouseX <= x + width && mouseY >= y && mouseY <= y + height;
}

void ButtonGroup::callHoverFunction(std::string ButtonName, Button& button)
{
	auto funcToCall = hoverFunctions.find(ButtonName);
	if (funcToCall != hoverFunctions.end())
	{
		funcToCall->second(button);
	}
	else
	{
		funcToCall = hoverFunctions.find(any);

		if (funcToCall != hoverFunctions.end())
			funcToCall->second(button);
		else
			throw std::runtime_error("callHoverFunction() error: no function found");
	}
}

void ButtonGroup::callHoverLostFunction(std::string ButtonName, Button& button)
{
	auto funcToCall = hoverLostFunctions.find(ButtonName);
	if (funcToCall != hoverLostFunctions.end())
	{
		funcToCall->second(button);
	}
	else
	{
		funcToCall = hoverLostFunctions.find(any);

		if (funcToCall != hoverLostFunctions.end())
			funcToCall->second(button);
		else
			throw std::runtime_error("callHoverFunction() error: no function found");
	}

}

void ButtonGroup::callClickFunction(std::string ButtonName, Button& button)
{
	auto funcToCall = clickFunctions.find(ButtonName);
	if (funcToCall != clickFunctions.end())
	{
		funcToCall->second(button);
	}
	else
	{
		funcToCall = clickFunctions.find(any);

		if (funcToCall != clickFunctions.end())
			funcToCall->second(button);
		else
			throw std::runtime_error("callHoverFunction() error: no function found");
	}
}

void ButtonGroup::draw()
{
	for (auto& i : buttons)
		i.second.draw();
}

void ButtonGroup::draw(float x, float y, float width, float height)
{
	for (auto& i : buttons)
		i.second.draw(x, y, width, height);
}


void ButtonGroup::setButtons(const std::map<std::string, Button>& buttons_)
{
	buttons = buttons_;
}

void ButtonGroup::setFunctions(const std::map<std::string, std::function<void(Button&)>>& hoverFunctions_,
	const std::map<std::string, std::function<void(Button&)>>& hoverLostFunctions_,
	const std::map<std::string, std::function<void(Button&)>>& clickFunctions_)
{
	hoverFunctions = hoverFunctions_;
	hoverLostFunctions = hoverLostFunctions_;
	clickFunctions = clickFunctions_;
}


Button& ButtonGroup::getButtonByName(const std::string& buttonName) {
	try {
		return buttons.at(buttonName);
	}
	catch (std::out_of_range& e)
	{
		throw new noButtonFound("no button at specified name");
	}
}