#include "PauseMenu.h"
#include "../../WindowManager/WindowManager.h"
#include "../../Renderer/SpriteRenderer.h"
#include "../../ResourceManager/ResourceManager.h"

#include <iostream>
#include "../../Input/InputHandler.h"
#include "../../Map/Map.h"
#include "../../Entity/Player/Player.h"
#include "../../HUD/HUDManager.h"
#include "../MenuManager.h"
#include "../../ButtonBuilder/ButtonBuilder.h"
#include "../../Game/Game.h"
#include "../MainMenu/MainMenu.h"
#include "../../WaveManager/WaveManager.h"
#include "../AlertBox/AlertBox.h"
#include "../ChangeSkinMenu/ChangeSkinMenu.h"



PauseMenu::PauseMenu(double x, double y, double drawWidth, double drawHeight, double rotateAngle, double speed, const std::string& textureName2D) :
	Entity(x, y, drawWidth, drawHeight, rotateAngle, speed),
	TexturableEntity(x, y, drawWidth, drawHeight, rotateAngle, speed, textureName2D),
	MenuBase(x, y, drawWidth, drawHeight, rotateAngle, speed, textureName2D, drawWidth * 0.4, drawHeight * 0.1),
	buttons(std::map<std::string, Button>{
		{ "quit", Button(getButtonPosX(), getButtonPosY(0), buttonWidth, buttonHeight, 0, 0, buttonWidth, buttonHeight, ButtonBuilder::buttonTextures0(), "Quit", 0, 1.0, "Antonio", true) },
		{ "continue", Button(getButtonPosX(), getButtonPosY(1), buttonWidth, buttonHeight, 0, 0, buttonWidth, buttonHeight, ButtonBuilder::buttonTextures0(), "Continue", 0, 1.0, "Antonio", true) },
		{ "save", Button(getButtonPosX(), getButtonPosY(2), buttonWidth, buttonHeight, 0, 0, buttonWidth, buttonHeight, ButtonBuilder::buttonTextures0(), "Save", 0, 1.0, "Antonio", true) },
		{ "back", ButtonBuilder::backButton(getButtonCoordsX(), getButtonCoordsY())}
			//Button(getButtonCoordsX(), getButtonCoordsY(), 20, 20, 0, 0, 20, 20, std::map<Button::Status, std::string>{{Button::Status::DEFAULT, ".0"}, { Button::Status::HOVERED, ".1" }, { Button::Status::CLICKED, ".2" }}, "")}
})
{
	isInMenu = false;

	buttons.setFunctions(
		std::map<std::string, std::function<void(Button&)>>{{ButtonGroup::getAny(), PauseMenu::hoverAnyButton }},
		std::map<std::string, std::function<void(Button&)>>{{ButtonGroup::getAny(), PauseMenu::hoverLostAnyButton }},
		std::map<std::string, std::function<void(Button&)>>{{ButtonGroup::getAny(), [](Button&) {} },
		{
			"continue", [](Button&) {
				PauseMenu::get().isInMenu = false;
				MenuManager::get().pop();
				// InputHandler::setInputComponent(InputHandler::getPlayerInputComponent());
			}
		},
		{
			"back", ButtonBuilder::backButtonClickFunction
		},
		{
			"quit", [](Button&){MenuManager::get().push(AlertBox::getCenteredAlertBox("Are you sure you want to quit?", "Quit", PauseMenu::quit));}
		}
		}
	);
}


PauseMenu& PauseMenu::get()
{
	double dW = WindowManager::get().getWindowWidth() * 0.6;
	double dH = WindowManager::get().getWindowHeight()* 0.5;
	double x = 0;
	double y = 0;


	static PauseMenu instance(x, y, dW, dH, 0, 0, "menuScreen0");
	return instance;
}

void PauseMenu::draw()
{
	SpriteRenderer::get().draw(ResourceManager::getShader("sprite"), ResourceManager::getTexture(this->textureName2D), glm::vec2(x, y), glm::vec2(drawWidth, drawHeight), 0);

	buttons.draw();
}


double PauseMenu::getButtonPosX() {
	return getButtonCoordsX() + buttonOffsetX;
}

double PauseMenu::getButtonPosY(int index) {
	return getButtonCoordsY() + buttonOffsetY + index * (buttonHeight + spaceAfterButton);
}


void PauseMenu::setupInputComponent()
{
	buttons.activate();
}

void PauseMenu::playMenu()
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

void PauseMenu::quit(Button&)
{
	Game::get().clear();

	WaveManager::deleteInstance();
	Player::deleteInstance();
	Map::deleteInstance();

	MenuManager::get().clear();

	MenuManager::get().push(MainMenu::get());
	InputHandler::setInputComponent(InputHandler::getMenuInputComponent());
}