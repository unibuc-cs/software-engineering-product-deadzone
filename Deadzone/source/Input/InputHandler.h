#pragma once

#include <map>

#include "InputComponent.h"

class InputHandler
{
public:
	static inline bool getKey(const int& key) { return keys[key]; }
	static inline bool getMouseButtons(const int& key) { return mouseButtons[key]; }
	static inline std::pair<double, double> getMousePosition() { return mousePosition; }

	static void setKey(const int& key, const int& value);
	static void setMouseButtons(const int& key, const int& value);
	static void setMousePosition(double xpos, double ypos);

	static inline void setInputComponent(InputComponent& inputComponent) { activeInputComponent = &inputComponent; }

	static inline InputComponent& getPlayerInputComponent() { return playerInputComponent; }
	static inline InputComponent& getMenuInputComponent() { return meniuInputComponent; }
	static inline InputComponent& getActiveInputComponent() { return *activeInputComponent; }

	static void callbackAction(int key, int action);
	static void callbackAxis(double xpos, double ypos);
	static void callbackScroll(double xoffset, double yoffset);

	static void update();

private:
	InputHandler() = default;
	~InputHandler() = default;
	InputHandler(const InputHandler& other) = delete;
	InputHandler& operator= (const InputHandler& other) = delete;
	InputHandler(const InputHandler&& other) = delete;
	InputHandler& operator= (const InputHandler&& other) = delete;

	static std::map<int, int> keys;
	static std::map<int, int> mouseButtons;
	static std::pair<double, double> mousePosition;

	static InputComponent playerInputComponent;
	static InputComponent meniuInputComponent;
	static InputComponent* activeInputComponent;

};

