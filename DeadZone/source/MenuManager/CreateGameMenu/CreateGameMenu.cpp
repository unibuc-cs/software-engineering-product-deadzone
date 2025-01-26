#include "CreateGameMenu.h"

#include <fstream>
#include <nlohmann/json.hpp>

#include "../../ResourceManager/ResourceManager.h"
#include "../../Renderer/SpriteRenderer.h"
#include "../../SoundManager/SoundManager.h"
#include "../../Input/InputHandler.h"
#include "../../Map/Map.h"
#include "../../Entity/Player/Player.h"
#include "../../HUD/HUDManager.h"
#include "../MenuManager.h"
#include "../../ButtonBuilder/ButtonBuilder.h"
#include "../../Game/Game.h"



CreateGameMenu::CreateGameMenu(double x, double y, double drawWidth, double drawHeight, double rotateAngle, double speed, const std::string& textureName2D) :
	Entity(x, y, drawWidth, drawHeight, rotateAngle, speed),
	TexturableEntity(x, y, drawWidth, drawHeight, rotateAngle, speed, textureName2D),
	MenuBase(x, y, drawWidth, drawHeight, rotateAngle, speed, textureName2D, drawWidth / 4.0, drawHeight / 12.0)
{
	isInMenu = false;

	std::map<std::string, Button> allButtons = CreateButtons();
	allButtons.insert({ "back", ButtonBuilder::backButton(getButtonCoordsX(), getButtonCoordsY()) });

	buttons.setButtons(allButtons);

	auto clickFunctions = std::map<std::string, std::function<void(Button&)>>{ {ButtonGroup::getAny(), [](Button&) {} },
		{
			"back", ButtonBuilder::backButtonClickFunction
		},
		{
			"PlaySurvival", [this](Button& button) {CreateGame(button);}
		},
		{
			"PlayTeamDeathMatch", [this](Button& button) {CreateGame(button);}
		}
	};

	buttons.setFunctions(
		std::map<std::string, std::function<void(Button&)>>{{ButtonGroup::getAny(), [](Button&) {} }},
		std::map<std::string, std::function<void(Button&)>>{{ButtonGroup::getAny(), [](Button&) {} }},
		clickFunctions
	);
}

std::map<std::string, Button> CreateGameMenu::CreateButtons()
{
	std::ifstream saveFile("config/save.json");

	if (!saveFile.is_open())
	{
		throw std::runtime_error("CreateGameMenu::CreateButtons: save.json not found");
	}

	nlohmann::json saveJSON;
	saveFile >> saveJSON;
	saveFile.close();

	std::string PlayerName = saveJSON.contains("clientName") ? saveJSON["clientName"].get<std::string>() : "YourName";
	std::string ServerIP = "localhost";
	std::string ServerPort = saveJSON.contains("createServerPort") ? saveJSON["createServerPort"].get<std::string>() : "7777";

	double InputFieldWidth = 600.0;

	double StartGameButtonsWidth = buttonWidth + 100;

	std::map<std::string, Button> rez{
			  { "PlayerName", Button(getButtonPosX(), getButtonPosY(0), buttonWidth, buttonHeight, 0, 0, buttonWidth, buttonHeight, ButtonBuilder::OneTextureForAllStates(), "Player Name:", 0, 1.0, "Antonio", true) }
			, { "PlayerNameInputField", Button(getButtonPosX(), getButtonPosY(1), InputFieldWidth, buttonHeight, 0, 0, InputFieldWidth, buttonHeight, ButtonBuilder::OneTextureForAllStates(), PlayerName, 0, 1.0, "Antonio", true, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0, -1.0, -1.0), false, ButtonBuilder::inputFieldTextures0()) }
			, { "ServerIP", Button(getButtonPosX(), getButtonPosY(2), buttonWidth, buttonHeight, 0, 0, buttonWidth, buttonHeight, ButtonBuilder::OneTextureForAllStates(), "Server IP:", 0, 1.0, "Antonio", true) }
			, { "ServerIPInputField", Button(getButtonPosX(), getButtonPosY(3), InputFieldWidth, buttonHeight, 0, 0, InputFieldWidth, buttonHeight, ButtonBuilder::OneTextureForAllStates(), ServerIP, 0, 1.0, "Antonio", true) }
			, { "ServerPort", Button(getButtonPosX(), getButtonPosY(4), buttonWidth, buttonHeight, 0, 0, buttonWidth, buttonHeight, ButtonBuilder::OneTextureForAllStates(), "Server Port:", 0, 1.0, "Antonio", true) }
			, { "ServerPortInputField", Button(getButtonPosX(), getButtonPosY(5), InputFieldWidth, buttonHeight, 0, 0, InputFieldWidth, buttonHeight, ButtonBuilder::OneTextureForAllStates(), ServerPort, 0, 1.0, "Antonio", true) }
			, { "PlaySurvival", Button(getButtonPosX(), getButtonPosY(7), StartGameButtonsWidth, buttonHeight, 0, 0, StartGameButtonsWidth, buttonHeight, ButtonBuilder::buttonTextures0(), "Play Survival", 0, 1.0, "Antonio", true) }
			, { "PlayTeamDeathMatch", Button(getButtonPosX() + StartGameButtonsWidth + 50, getButtonPosY(7), StartGameButtonsWidth, buttonHeight, 0, 0, StartGameButtonsWidth, buttonHeight, ButtonBuilder::buttonTextures0(), "Play Team Deathmatch ", 0, 1.0, "Antonio", true) }
	};

	return rez;
}

double CreateGameMenu::getButtonPosX() {
	return getButtonCoordsX() + buttonOffsetX;
}

double CreateGameMenu::getButtonPosY(int index) {
	return getButtonCoordsY() + buttonOffsetY + index * (buttonHeight + spaceAfterButton);
}

void CreateGameMenu::init()
{
	std::ifstream saveFile("config/save.json");

	if (!saveFile.is_open())
	{
		throw std::runtime_error("JoinGameMenu::init: save.json not found");
	}

	nlohmann::json saveJSON;
	saveFile >> saveJSON;
	saveFile.close();

	std::string PlayerName = saveJSON.contains("clientName") ? saveJSON["clientName"].get<std::string>() : "YourName";
	std::string ServerIP = "localhost";
	std::string ServerPort = saveJSON.contains("createServerPort") ? saveJSON["createServerPort"].get<std::string>() : "7777";

	buttons.getButtonByName("PlayerNameInputField").setLabel(PlayerName);
	buttons.getButtonByName("ServerIPInputField").setLabel(ServerIP);
	buttons.getButtonByName("ServerPortInputField").setLabel(ServerPort);
}

CreateGameMenu& CreateGameMenu::get()
{
	double dW = WindowManager::get().getWindowWidth() * 0.8;
	double dH = WindowManager::get().getWindowHeight() * 0.9;
	double x = 0;
	double y = (-WindowManager::get().getWindowHeight() + dH) / 2.0;


	static CreateGameMenu instance(x, y, dW, dH, 0, 0, "menuScreen0");
	return instance;
}

void CreateGameMenu::draw()
{
	SpriteRenderer::get().draw(ResourceManager::getShader("sprite"), ResourceManager::getTexture(this->textureName2D), glm::vec2(x, y), glm::vec2(drawWidth, drawHeight), 0);

	buttons.draw();
}

void CreateGameMenu::AddLetter(char letter)
{
	std::string buttonName = "";

	if (letter >= 'A' && letter <= 'Z')
		letter = letter + ('a' - 'A');

	if (buttons.getButtonByName("PlayerNameInputField").getHasFocus())
		buttonName = "PlayerNameInputField";

	// std::cout << "ButtonName:" << buttonName << "end\n";
	if (buttonName != "")
		buttons.getButtonByName(buttonName).setLabel(buttons.getButtonByName(buttonName).getLabel() + std::string(1, letter));
}

void CreateGameMenu::DeleteLetter()
{
	std::string buttonName = "";

	if (buttons.getButtonByName("PlayerNameInputField").getHasFocus())
		buttonName = "PlayerNameInputField";

	std::string newLabel = "";
	if(buttonName != "")
		newLabel = buttons.getButtonByName(buttonName).getLabel();

	if (!newLabel.empty())
		newLabel.pop_back();

	if (buttonName != "")
		buttons.getButtonByName(buttonName).setLabel(newLabel);
}

void CreateGameMenu::CreateGame(Button& button)
{
	// TODO: validari input -- aici nu ar fi nevoie, ca e doar player name

	std::ifstream readFile("config/save.json");
	nlohmann::json saveJSON;
	readFile >> saveJSON;
	readFile.close();

	saveJSON["clientHasServer"] = true;
	saveJSON["clientName"] = buttons.getButtonByName("PlayerNameInputField").getLabel();
	saveJSON["createServerPort"] = "7777";

	std::ofstream saveFile("config/save.json");
	saveFile << std::setw(4) << saveJSON << std::endl;
	saveFile.close();



	Game::get().establishConnection();
	Game::get().setIsInMatch(true);

	// MainMenu::get().isInMenu = false;
	MenuManager::get().clear();
	InputHandler::setInputComponent(InputHandler::getPlayerInputComponent());

	Player::get().setupPlayerInputComponent();
}

void CreateGameMenu::setupInputComponent()
{
	InputHandler::getMenuInputComponent().clearKeyFunctionCallbacks(); // TODO: e necesar?

	buttons.activate();

	for (int key = GLFW_KEY_A; key <= GLFW_KEY_Z; ++key)
	{
		InputHandler::getMenuInputComponent().bindKey(key, InputEvent::IE_Pressed, [key, this]() {this->AddLetter(key);});
	}

	for (int key = GLFW_KEY_0; key <= GLFW_KEY_9; ++key)
	{
		InputHandler::getMenuInputComponent().bindKey(key, InputEvent::IE_Pressed, [key, this]() {this->AddLetter(key);});
	}

	InputHandler::getMenuInputComponent().bindKey(GLFW_KEY_COMMA, InputEvent::IE_Pressed, [this]() {this->AddLetter(GLFW_KEY_COMMA);});
	InputHandler::getMenuInputComponent().bindKey(GLFW_KEY_PERIOD, InputEvent::IE_Pressed, [this]() {this->AddLetter(GLFW_KEY_PERIOD);});
	InputHandler::getMenuInputComponent().bindKey(GLFW_KEY_SPACE, InputEvent::IE_Pressed, [this]() {this->AddLetter(GLFW_KEY_SPACE);});

	InputHandler::getMenuInputComponent().bindKey(GLFW_KEY_BACKSPACE, InputEvent::IE_Pressed, [this]() {this->DeleteLetter();});
}

void CreateGameMenu::setIsInMenu(bool _isInMenu)
{
	bool temp_isInMenu = isInMenu;
	MenuBase::setIsInMenu(_isInMenu);
	if (temp_isInMenu == false && isInMenu == true)
		SoundManager::get().resume("soundtrack");
	else
		if (temp_isInMenu == true && isInMenu == false)
			SoundManager::get().pause("soundtrack");
}

void CreateGameMenu::playMenu()
{
	SoundManager::get().resume("soundtrack");

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

	SoundManager::get().pause("soundtrack");
}
