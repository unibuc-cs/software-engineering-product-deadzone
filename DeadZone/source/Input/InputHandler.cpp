#include "InputHandler.h"

// instantiate static variables
std::map<int, int> InputHandler::keys;
std::map<int, int> InputHandler::mouseButtons;
std::pair<double, double> InputHandler::mousePosition;
InputComponent InputHandler::playerInputComponent;
InputComponent InputHandler::meniuInputComponent;
InputComponent* InputHandler::activeInputComponent = nullptr;

void InputHandler::setKey(const int& key, const int& value)
{
	keys[key] = value;
}

void InputHandler::setMouseButtons(const int& key, const int& value)
{
	mouseButtons[key] = value;
}

void InputHandler::setMousePosition(double xpos, double ypos)
{
	mousePosition = std::make_pair(xpos, ypos);
}

void InputHandler::callbackAction(int key, int action)
{
	// TODO: refactor

	if (activeInputComponent)
	{
		activeInputComponent->callbackAction(key, action);
	}
}

void InputHandler::callbackAxis(double xpos, double ypos)
{
	if (activeInputComponent)
	{
		activeInputComponent->callbackAxis(xpos, ypos);
	}
}

void InputHandler::callbackScroll(double xoffset, double yoffset)
{
	if (activeInputComponent)
	{
		activeInputComponent->callbackScroll(xoffset, yoffset);
	}
}

void InputHandler::update()
{
	//for (const auto& key : keys)
	//{
	//	if (key.second == 1 || key.second == 2)
	//	{
	//		activeInputComponent->callbackAction(key.first, key.second);
	//	}
	//}

	//for (const auto& key : mouseButtons)
	//{
	//	if (key.second == 1)
	//	{
	//		activeInputComponent->callbackAction(key.first, key.second);
	//	}
	//}
}

