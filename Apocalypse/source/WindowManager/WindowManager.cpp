#include "WindowManager.h"

#include <iostream>

#include "../Input/InputHandler.h"

WindowManager::WindowManager() 
	: WINDOW_WIDTH(1024), WINDOW_HEIGHT(896), WINDOW_TITLE("DeadZone")
{
	// Init GLFW
	glfwInit();

	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Create a GLFWwindow object that we can use for GLFW's functions
	window = glfwCreateWindow(this->WINDOW_WIDTH, this->WINDOW_HEIGHT, this->WINDOW_TITLE.c_str(), NULL, NULL);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return;
	}

	// Successfully created GLFW Window
	std::cout << "Created GLFW Window" << std::endl;

	// Set GLFW context
	glfwMakeContextCurrent(window);

	// Load OpenGL functions, gladLoadGL returns the loaded version, 0 on error
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return;
	}

	// Successfully loaded GLAD
	std::cout << "Loaded GLAD" << std::endl;

	// Set the required callback functions
	glfwSetKeyCallback(window, keyCallback);
	glfwSetCursorPosCallback(window, cursorPosCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	
	glfwSetScrollCallback(window, mouseScrollCallback);

	// OpenGL configuration
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

WindowManager::~WindowManager()
{
	// Destroy window
	glfwDestroyWindow(this->window);

	// Terminates GLFW, clearing any resources allocated by GLFW
	glfwTerminate();
}

WindowManager& WindowManager::get()
{
	static WindowManager instance;
	return instance;
}

void WindowManager::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	InputHandler::setKey(key, action);
	InputHandler::callbackAction(key, action);
}

void WindowManager::cursorPosCallback(GLFWwindow* window, double xpos, double ypos)
{	
	InputHandler::setMousePosition(xpos, ypos);
	InputHandler::callbackAxis(xpos, ypos);
}

void WindowManager::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	InputHandler::setMouseButtons(button, action);
	InputHandler::callbackAction(button, action);
}

void WindowManager::mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	InputHandler::callbackScroll(xoffset, yoffset);
}

