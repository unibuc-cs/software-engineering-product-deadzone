#include "AlertBox.h"
#include "../../ButtonBuilder/ButtonBuilder.h"
#include "../../Renderer/SpriteRenderer.h"
#include "../../ResourceManager/ResourceManager.h"
#include "../../Input/InputHandler.h"
#include "../../Map/Map.h"
#include "../../HUD/HUDManager.h"
#include "../../Entity/Player/Player.h"
#include "../MenuManager.h"


AlertBox* AlertBox::instance = NULL;

AlertBox::AlertBox(double x, double y, double drawWidth, double drawHeight, double rotateAngle, double speed, const std::string& textureName2D, const std::string& msg_, const std::string& buttonLabel_, const std::function<void(Button&)>& funcForButton_) :
	Entity(x, y, drawWidth, drawHeight, rotateAngle, speed),
	TexturableEntity(x, y, drawWidth, drawHeight, rotateAngle, speed, textureName2D),
	MenuBase(x, y, drawWidth, drawHeight, rotateAngle, speed, textureName2D, drawWidth * 0.4, drawHeight * 0.1),
	msg(msg_),
	buttons(std::map<std::string, Button>{
		{ "back", ButtonBuilder::backButton(getButtonCoordsX(), getButtonCoordsY()) },
		{ "card", Button(getButtonCoordsX(), getButtonCoordsY() + drawHeight * 0.2, drawWidth, drawHeight * 0.6, 0, 0, drawWidth, drawHeight * 0.8, std::map<Button::Status, std::string>{{Button::Status::DEFAULT, "noBackground"}, { Button::Status::HOVERED, "noBackground" }, { Button::Status::CLICKED, "noBackground" }}, msg, 0, 1.0, "Antonio", true) },
		{ "close", Button(getButtonCoordsX() + drawWidth * (2/5.0), getButtonCoordsY() + drawHeight * 0.8, drawWidth * (1/5.0), drawHeight * 0.15, 0, 0, drawWidth * (1/5.0),  drawHeight * 0.2, std::map<Button::Status, std::string>{{Button::Status::DEFAULT, "button0Normal"}, {Button::Status::HOVERED, "button0Hovered"}, {Button::Status::CLICKED, "button0Hovered"}}, buttonLabel_, 0, 1.5, "Antonio", true)}
			//Button(getButtonCoordsX(), getButtonCoordsY(), 20, 20, 0, 0, 20, 20, std::map<Button::Status, std::string>{{Button::Status::DEFAULT, ".0"}, { Button::Status::HOVERED, ".1" }, { Button::Status::CLICKED, ".2" }}, "")}
})
{
	isInMenu = false;

	buttons.setFunctions(
		std::map<std::string, std::function<void(Button&)>>{{ButtonGroup::getAny(), AlertBox::hoverAnyButton }, { "card", [](Button&) {} }},
		std::map<std::string, std::function<void(Button&)>>{{ButtonGroup::getAny(), AlertBox::hoverLostAnyButton }, { "card", [](Button&) {} }},
		std::map<std::string, std::function<void(Button&)>>{{ButtonGroup::getAny(), [](Button&) {} },
		{
			"close", funcForButton_
		},
		{
			"back", ButtonBuilder::backButtonClickFunction
		}
	}
	);
}

AlertBox& AlertBox::getCenteredAlertBox(const std::string& msg, const std::string& buttonLabel_, const std::function<void(Button&)>& funcForButton_)
{
	double width = WindowManager::get().getWindowWidth() * 0.5;
	double height = WindowManager::get().getWindowWidth() * 0.5;
	
	double x = 0;
	double y = 0;

	if (instance != NULL)
		delete instance;

	instance = new AlertBox(x, y, width, height, 0, 0, ".0", msg, buttonLabel_, funcForButton_);
	return *instance;
}

void AlertBox::draw()
{
	SpriteRenderer::get().draw(ResourceManager::getShader("sprite"), ResourceManager::getTexture(this->textureName2D), glm::vec2(x, y), glm::vec2(drawWidth, drawHeight), 0);

	buttons.draw();
}

void AlertBox::setupInputComponent()
{
	buttons.activate();
}


void AlertBox::playMenu()
{
	while (isInMenu == true && !glfwWindowShouldClose(WindowManager::get().getWindow()))
	{

		InputHandler::update();

		// Map
		Map::get().draw();

		// Player
		Player::get().draw();

		// HUD
		HUDManager::get().draw();

		MenuManager::get().draw();



		GlobalClock::get().updateTime();

		// Swap the screen buffers
		glfwSwapBuffers(WindowManager::get().getWindow());

		// Check if any events have been activated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
	}
}
