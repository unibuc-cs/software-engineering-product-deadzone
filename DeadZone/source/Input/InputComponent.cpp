#include "InputComponent.h"

#include <iostream> // TODO: debug
#include <fstream>

#include <nlohmann/json.hpp>

void InputComponent::bindAction(const std::string& actionName, const InputEvent& keyEvent, const std::function<void()>& func)
{
	std::ifstream inputFile("config/input.json");
	nlohmann::json inputJSON;
	inputFile >> inputJSON;
	inputFile.close();

	keyFunctionCallbacks[inputJSON[actionName]][static_cast<int>(keyEvent)].push_back(func);
}

void InputComponent::bindAxis(const std::function<void(double, double)>& func)
{
	mouseFunctionCallbacks.push_back(func);
}

void InputComponent::bindScroll(const std::function<void(double, double)>& func)
{
	scrollFunctionCallbacks.push_back(func);
}

void InputComponent::callbackAction(int key, int action)
{
	for (const auto& f : keyFunctionCallbacks[key][action])
	{
		f();
	}
}

void InputComponent::callbackAxis(double xpos, double ypos)
{
	for (const auto& f : mouseFunctionCallbacks)
	{
		f(xpos, ypos);
	}
}

void InputComponent::callbackScroll(double xoffset, double yoffset)
{
	for (const auto& f : scrollFunctionCallbacks)
	{
		f(xoffset, yoffset);
	}
}

void InputComponent::replaceAction(const std::string& actionName, const InputEvent& keyEvent, const std::function<void()>& func)
{
	std::ifstream inputFile("config/input.json");
	nlohmann::json inputJSON;
	inputFile >> inputJSON;
	inputFile.close();

	keyFunctionCallbacks[inputJSON[actionName]][static_cast<int>(keyEvent)].clear();
	keyFunctionCallbacks[inputJSON[actionName]][static_cast<int>(keyEvent)].push_back(func);
}

void InputComponent::replaceAxis(const std::function<void(double, double)>& func)
{
	mouseFunctionCallbacks.clear();
	mouseFunctionCallbacks.push_back(func);
}

void InputComponent::replaceScroll(const std::function<void(double, double)>& func)
{
	scrollFunctionCallbacks.clear();
	scrollFunctionCallbacks.push_back(func);
}

void InputComponent::clear()
{
	keyFunctionCallbacks.clear();
	mouseFunctionCallbacks.clear();
	scrollFunctionCallbacks.clear();
}