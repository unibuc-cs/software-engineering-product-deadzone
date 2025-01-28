#include "PauseMenu.h"
#include "../../WindowManager/WindowManager.h"
#include "../../Renderer/SpriteRenderer.h"
#include "../../ResourceManager/ResourceManager.h"

#include <iostream>
#include <nlohmann/json.hpp>

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
#include "../../Client/Client.h"

PauseMenu::PauseMenu(double x, double y, double drawWidth, double drawHeight, double rotateAngle, double speed, const std::string& textureName2D) :
	Entity(x, y, drawWidth, drawHeight, rotateAngle, speed),
	TexturableEntity(x, y, drawWidth, drawHeight, rotateAngle, speed, textureName2D),
	MenuBase(x, y, drawWidth, drawHeight, rotateAngle, speed, textureName2D, drawWidth * 0.4, drawHeight * 0.1),
	buttons(std::map<std::string, Button>{
		{ "quit", Button(getButtonPosX(), getButtonPosY(0), buttonWidth, buttonHeight, 0, 0, buttonWidth, buttonHeight, ButtonBuilder::buttonTextures0(), "Quit", 0, 1.0, "Antonio", true) },
		{ "continue", Button(getButtonPosX(), getButtonPosY(1), buttonWidth, buttonHeight, 0, 0, buttonWidth, buttonHeight, ButtonBuilder::buttonTextures0(), "Continue", 0, 1.0, "Antonio", true) },
		{ "save", Button(getButtonPosX(), getButtonPosY(2), buttonWidth, buttonHeight, 0, 0, buttonWidth, buttonHeight, ButtonBuilder::buttonTextures0(), "Save", 0, 1.0, "Antonio", true) },
		{ "back", ButtonBuilder::backButton(getButtonCoordsX(), getButtonCoordsY())},
		{ "gameInfo", Button(getButtonPosX() - buttonOffsetX, getButtonPosY(4), this->drawWidth, buttonHeight, 0, 0, this->drawWidth, buttonHeight, ButtonBuilder::OneTextureForAllStatesThinMargins(), "ServerIp: , ServerPort: , ", 0, 0.7, "Antonio", true) }
	})
{
	isInMenu = false;

	buttons.setFunctions(
		std::map<std::string, std::function<void(Button&)>>{
			{ButtonGroup::getAny(), PauseMenu::hoverAnyButton },
			{ "gameInfo", [](Button&) {} }
		},
		std::map<std::string, std::function<void(Button&)>>{
			{ButtonGroup::getAny(), PauseMenu::hoverLostAnyButton },
			{ "gameInfo", [](Button&) {} }
		},
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


void PauseMenu::setGameInfoInMenu()
{
	std::ifstream saveFile("config/save.json");
	if (!saveFile.is_open())
	{
		throw std::runtime_error("JoinGameMenu::CreateButtons: save.json not found");
	}

	nlohmann::json saveJSON;
	saveFile >> saveJSON;
	saveFile.close();

	bool clientHasServer = saveJSON.contains("clientHasServer");
	std::string PlayerName = saveJSON["clientName"].get<std::string>();
	std::string ServerIP = clientHasServer ? "localhost" : saveJSON["joinServerAddress"].get<std::string>();
	std::string ServerPort = clientHasServer ? saveJSON["createServerPort"].get<std::string>() : saveJSON["joinServerPort"].get<std::string>();
	
	std::string gameInfo = "Player Name: " + PlayerName + ";    Server Ip: " + ServerIP + ";    Server Port: " + ServerPort;
	buttons.getButtonByName("gameInfo").setLabel(gameInfo);
}

PauseMenu& PauseMenu::get()
{
	double dW = WindowManager::get().getWindowWidth() * 0.7;
	double dH = WindowManager::get().getWindowHeight()* 0.7;
	double x = 0;
	double y = 0;


	static PauseMenu instance(x, y, dW, dH, 0, 0, "menuScreen0");

	instance.setGameInfoInMenu();
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
	Game::get().stopConnection();

	Game::get().clear();
	Game::get().setIsInMatch(false);
	Game::get().setHasGameMode(false);

	WaveManager::deleteInstance();
	Player::deleteInstance();
	Map::deleteInstance();

	MenuManager::get().clear();

	MenuManager::get().push(MainMenu::get());
	InputHandler::setInputComponent(InputHandler::getMenuInputComponent());
}