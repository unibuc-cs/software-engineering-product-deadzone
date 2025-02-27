#include "EndScreen.h"
#include "../../ButtonBuilder/ButtonBuilder.h"
#include "../../Renderer/SpriteRenderer.h"
#include "../../ResourceManager/ResourceManager.h"
#include "../../Input/InputHandler.h"
#include "../../Map/Map.h"
#include "../../HUD/HUDManager.h"
#include "../../Entity/Player/Player.h"
#include "../MenuManager.h"
#include "../../WaveManager/WaveManager.h"


EndScreen* EndScreen::instance = NULL;

EndScreen::EndScreen(double x, double y, double drawWidth, double drawHeight, double rotateAngle, double speed, const std::string& textureName2D, const std::string& msg_, const std::string& buttonLabel_, const std::function<void(Button&)>& funcForButton_) :
	Entity(x, y, drawWidth, drawHeight, rotateAngle, speed),
	TexturableEntity(x, y, drawWidth, drawHeight, rotateAngle, speed, textureName2D),
	MenuBase(x, y, drawWidth, drawHeight, rotateAngle, speed, textureName2D, drawWidth * 0.4, drawHeight * 0.1),
	msg(msg_),
	buttons(std::map<std::string, Button>{
		{ "card", Button(getButtonCoordsX(), getButtonCoordsY() + drawHeight * 0.2, drawWidth, drawHeight * 0.6, 0, 0, drawWidth, drawHeight * 0.8, std::map<Button::Status, std::string>{{Button::Status::DEFAULT, "noBackground"}, { Button::Status::HOVERED, "noBackground" }, { Button::Status::CLICKED, "noBackground" }}, msg, 0, 1.0, "Antonio", true) },
		{ "backToMainMenu", Button(getButtonCoordsX() + drawWidth * (0.7 / 10.0), getButtonCoordsY() + drawHeight * 0.8, drawWidth * (1 / 3.0), drawHeight * 0.15, 0, 0, drawWidth * (1 / 3.0),  drawHeight * 0.15, std::map<Button::Status, std::string>{{Button::Status::DEFAULT, "button0Normal"}, {Button::Status::HOVERED, "button0Hovered"}, {Button::Status::CLICKED, "button0Hovered"}}, buttonLabel_, 0, 1.0, "Antonio", true) },
		{ "respawn", Button(getButtonCoordsX() + drawWidth * (3 / 5.0), getButtonCoordsY() + drawHeight * 0.8, drawWidth * (1 / 3.0), drawHeight * 0.15, 0, 0, drawWidth * (1 / 3.0),  drawHeight * 0.15, std::map<Button::Status, std::string>{{Button::Status::DEFAULT, "button0Normal"}, {Button::Status::HOVERED, "button0Hovered"}, {Button::Status::CLICKED, "button0Hovered"}}, "Respawn", 0, 1.0, "Antonio", true)}
})
{
	isInMenu = false;

	buttons.setFunctions(
		std::map<std::string, std::function<void(Button&)>>{{ButtonGroup::getAny(), EndScreen::hoverAnyButton }, { "card", [](Button&) {} }},
		std::map<std::string, std::function<void(Button&)>>{{ButtonGroup::getAny(), EndScreen::hoverLostAnyButton }, { "card", [](Button&) {} }},
		std::map<std::string, std::function<void(Button&)>>{{ButtonGroup::getAny(), [](Button&) {} },
		{
			"backToMainMenu", funcForButton_
		},
		{
			"respawn", EndScreen::RespawnFunction
		}
	}
	);
}

EndScreen& EndScreen::getCenteredEndScreen(const std::string& msg, const std::string& buttonLabel_, const std::function<void(Button&)>& funcForButton_)
{
	double width = WindowManager::get().getWindowWidth() * 0.5;
	double height = WindowManager::get().getWindowWidth() * 0.5;

	double x = 0;
	double y = 0;

	if (instance != NULL)
		delete instance;

	instance = new EndScreen(x, y, width, height, 0, 0, ".0", msg, buttonLabel_, funcForButton_);
	return *instance;
}

void EndScreen::RespawnFunction(Button& button)
{
	const glm::vec3 currentOutfitColor = Player::get().getOutfitColor();
	const int currentTeam = Player::get().getTeam();
	Player::deleteInstance();

	MenuManager::get().clear();

	Player::get().setupPlayerInputComponent();
	Player::get().setOutfitColor(currentOutfitColor);
	Player::get().setTeam(currentTeam);
}

void EndScreen::draw()
{
	SpriteRenderer::get().draw(ResourceManager::getShader("sprite"), ResourceManager::getTexture(this->textureName2D), glm::vec2(x, y), glm::vec2(drawWidth, drawHeight), 0);

	buttons.draw();
}

void EndScreen::setupInputComponent()
{
	buttons.activate();
}


void EndScreen::playMenu()
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
