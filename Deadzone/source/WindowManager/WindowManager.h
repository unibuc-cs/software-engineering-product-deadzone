#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>

class WindowManager
{
private:
	WindowManager();
	~WindowManager();
	WindowManager(const WindowManager& other) = delete;
	WindowManager& operator= (const WindowManager& other) = delete;
	WindowManager(const WindowManager&& other) = delete;
	WindowManager& operator= (const WindowManager&& other) = delete;

private:
	const int WINDOW_WIDTH;
	const int WINDOW_HEIGHT;
	const std::string WINDOW_TITLE;

	GLFWwindow* window;

	// callback functions
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
	static void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);
	static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

public:
	static WindowManager& get();
	inline int getWindowWidth() { return this->WINDOW_WIDTH; }
	inline int getWindowHeight() { return this->WINDOW_HEIGHT; }
	inline GLFWwindow* getWindow() { return this->window; }
};

