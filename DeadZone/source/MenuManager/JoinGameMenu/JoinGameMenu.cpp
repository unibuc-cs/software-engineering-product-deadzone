#include "JoinGameMenu.h"

#include <sstream>
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



JoinGameMenu::JoinGameMenu(double x, double y, double drawWidth, double drawHeight, double rotateAngle, double speed, const std::string& textureName2D) :
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
			"PlaySurvival", [this](Button& button) {JoinGame(button);}
		},
		{
			"PlayTeamDeathMatch", [this](Button& button) {JoinGame(button);}
		}
	};

	buttons.setFunctions(
		std::map<std::string, std::function<void(Button&)>>{{ButtonGroup::getAny(), [](Button&) {} }},
		std::map<std::string, std::function<void(Button&)>>{{ButtonGroup::getAny(), [](Button&) {} }},
		clickFunctions
	);
}

std::map<std::string, Button> JoinGameMenu::CreateButtons()
{
	std::ifstream saveFile("config/save.json");
	if (!saveFile.is_open())
	{
		throw std::runtime_error("JoinGameMenu::CreateButtons: save.json not found");
	}

	nlohmann::json saveJSON;
	saveFile >> saveJSON;
	saveFile.close();

	std::string PlayerName = saveJSON.contains("clientName") ? saveJSON["clientName"].get<std::string>() : "YourName";
	std::string ServerIP = saveJSON.contains("joinServerAddress") ? saveJSON["joinServerAddress"].get<std::string>() : "localhost";
	std::string ServerPort = saveJSON.contains("joinServerPort") ? saveJSON["joinServerPort"].get<std::string>() : "7777";

	double InputFieldWidth = 600.0;

	double StartGameButtonsWidth = buttonWidth + 100;

	std::map<std::string, Button> rez{
			  { "PlayerName", Button(getButtonPosX(), getButtonPosY(0), buttonWidth, buttonHeight, 0, 0, buttonWidth, buttonHeight, ButtonBuilder::OneTextureForAllStates(), "Player Name:", 0, 1.0, "Antonio", true) }
			, { "PlayerNameInputField", Button(getButtonPosX(), getButtonPosY(1), InputFieldWidth, buttonHeight, 0, 0, InputFieldWidth, buttonHeight, ButtonBuilder::OneTextureForAllStates(), PlayerName, 0, 1.0, "Antonio", true, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0, -1.0, -1.0), false, ButtonBuilder::inputFieldTextures0()) }
			, { "ServerIP", Button(getButtonPosX(), getButtonPosY(2), buttonWidth, buttonHeight, 0, 0, buttonWidth, buttonHeight, ButtonBuilder::OneTextureForAllStates(), "Server IP:", 0, 1.0, "Antonio", true) }
			, { "ServerIPInputField", Button(getButtonPosX(), getButtonPosY(3), InputFieldWidth, buttonHeight, 0, 0, InputFieldWidth, buttonHeight, ButtonBuilder::OneTextureForAllStates(), ServerIP, 0, 1.0, "Antonio", true, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0, -1.0, -1.0), false, ButtonBuilder::inputFieldTextures0()) }
			, { "ServerPort", Button(getButtonPosX(), getButtonPosY(4), buttonWidth, buttonHeight, 0, 0, buttonWidth, buttonHeight, ButtonBuilder::OneTextureForAllStates(), "Server Port:", 0, 1.0, "Antonio", true) }
			, { "ServerPortInputField", Button(getButtonPosX(), getButtonPosY(5), InputFieldWidth, buttonHeight, 0, 0, InputFieldWidth, buttonHeight, ButtonBuilder::OneTextureForAllStates(), ServerPort, 0, 1.0, "Antonio", true, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0, -1.0, -1.0), false, ButtonBuilder::inputFieldTextures0())  }
			, { "PlaySurvival", Button(getButtonPosX(), getButtonPosY(7), StartGameButtonsWidth, buttonHeight, 0, 0, StartGameButtonsWidth, buttonHeight, ButtonBuilder::buttonTextures0(), "Play Survival", 0, 1.0, "Antonio", true) }
			, { "PlayTeamDeathMatch", Button(getButtonPosX() + StartGameButtonsWidth + 50, getButtonPosY(7), StartGameButtonsWidth, buttonHeight, 0, 0, StartGameButtonsWidth, buttonHeight, ButtonBuilder::buttonTextures0(), "Play Team Deathmatch ", 0, 1.0, "Antonio", true) }
	};

	return rez;
}

double JoinGameMenu::getButtonPosX() {
	return getButtonCoordsX() + buttonOffsetX;
}

double JoinGameMenu::getButtonPosY(int index) {
	return getButtonCoordsY() + buttonOffsetY + index * (buttonHeight + spaceAfterButton);
}

JoinGameMenu& JoinGameMenu::get()
{
	double dW = WindowManager::get().getWindowWidth() * 0.8;
	double dH = WindowManager::get().getWindowHeight() * 0.9;
	double x = 0;
	double y = (-WindowManager::get().getWindowHeight() + dH) / 2.0;


	static JoinGameMenu instance(x, y, dW, dH, 0, 0, "menuScreen0");
	return instance;
}

void JoinGameMenu::draw()
{
	SpriteRenderer::get().draw(ResourceManager::getShader("sprite"), ResourceManager::getTexture(this->textureName2D), glm::vec2(x, y), glm::vec2(drawWidth, drawHeight), 0);

	buttons.draw();
}

void JoinGameMenu::AddLetter(char letter)
{
	std::string buttonName = "";

	if(letter >= 'A' && letter <= 'Z')
		letter = letter + ('a' - 'A');

	if (buttons.getButtonByName("PlayerNameInputField").getHasFocus())
		buttonName = "PlayerNameInputField";

	if (buttons.getButtonByName("ServerIPInputField").getHasFocus())
		buttonName = "ServerIPInputField";

	if (buttons.getButtonByName("ServerPortInputField").getHasFocus())
		buttonName = "ServerPortInputField";

	// std::cout << "ButtonName:" << buttonName << "end\n";
	if(buttonName != "")
		buttons.getButtonByName(buttonName).setLabel(buttons.getButtonByName(buttonName).getLabel() + std::string(1, letter));
}

void JoinGameMenu::DeleteLetter()
{
	std::string buttonName = "";

	if (buttons.getButtonByName("PlayerNameInputField").getHasFocus())
		buttonName = "PlayerNameInputField";

	if (buttons.getButtonByName("ServerIPInputField").getHasFocus())
		buttonName = "ServerIPInputField";

	if (buttons.getButtonByName("ServerPortInputField").getHasFocus())
		buttonName = "ServerPortInputField";

	std::string newLabel = buttons.getButtonByName(buttonName).getLabel();

	if(!newLabel.empty())
		newLabel.pop_back();

	if (buttonName != "")
		buttons.getButtonByName(buttonName).setLabel(newLabel);
}

void JoinGameMenu::JoinGame(Button& button)
{
	// input validation
	std::string serverIP = trim(buttons.getButtonByName("ServerIPInputField").getLabel());
	std::string serverPort = trim(buttons.getButtonByName("ServerPortInputField").getLabel());

	bool formIsValid = true;

	if (!validateIP(serverIP))
	{
		std::cout << "IP nevalid\n";
		buttons.getButtonByName("ServerIPInputField").setInvalidInputStatusAndPreviousInputStatus();
		formIsValid = false;
	}
	else
	{
		buttons.getButtonByName("ServerIPInputField").setDefaultInputStatusAndPreviousInputStatus();
	}

	if (!validatePort(serverPort))
	{
		std::cout << "Port nevalid\n";
		buttons.getButtonByName("ServerPortInputField").setInvalidInputStatusAndPreviousInputStatus();

		formIsValid = false;
	}
	else
	{
		buttons.getButtonByName("ServerPortInputField").setDefaultInputStatusAndPreviousInputStatus();
	}

	if (!formIsValid)
	{
		return;
	}


	std::ifstream readFile("config/save.json");
	nlohmann::json saveJSON;
	readFile >> saveJSON;
	readFile.close();

	saveJSON["clientHasServer"] = false;
	saveJSON["clientName"] = buttons.getButtonByName("PlayerNameInputField").getLabel();
	saveJSON["joinServerAddress"] = serverIP;
	saveJSON["joinServerPort"] = serverPort;

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

void JoinGameMenu::setupInputComponent()
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

void JoinGameMenu::playMenu()
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

void JoinGameMenu::init()
{
	std::ifstream saveFile("config/save.json");

	if (!saveFile.is_open())
	{
		throw std::runtime_error("JoinGameMenu::init: save.json not found");
	}

	nlohmann::json saveJSON;
	saveFile >> saveJSON;
	saveFile.close();

	std::string PlayerName = saveJSON["clientName"].get<std::string>();
	std::string ServerIP = saveJSON["joinServerAddress"].get<std::string>();
	std::string ServerPort = saveJSON["joinServerPort"].get<std::string>();

	buttons.getButtonByName("PlayerNameInputField").setLabel(PlayerName);
	buttons.getButtonByName("ServerIPInputField").setLabel(ServerIP);
	buttons.getButtonByName("ServerPortInputField").setLabel(ServerPort);


	buttons.getButtonByName("ServerIPInputField").setDefaultInputStatusAndPreviousInputStatus();
	buttons.getButtonByName("ServerPortInputField").setDefaultInputStatusAndPreviousInputStatus();
}

bool JoinGameMenu::validateIP(const std::string& IP)
{
	if (IP.size() > 15)
		return false;

	std::istringstream stream(IP);
	std::string part;
	int octetCount = 0;

	while (std::getline(stream, part, '.'))
	{
		if (part.empty() || part.size() > 3)
			return false;

		for (char c : part)
		{
			if (!std::isdigit(c))
				return false;
		}

		if (part.size() > 1 && part[0] == '0')
			return false;

		int num = std::stoi(part);
		if (num < 0 || num > 255)
			return false;

		octetCount++;
	}

	return octetCount == 4;
}

bool JoinGameMenu::validatePort(const std::string& port)
{
	if (port.empty()) {
		return false;
	}

	for (char c : port) {
		if (!std::isdigit(c)) {
			return false;
		}
	}

	int portNum = std::stoi(port);

	return portNum >= 1 && portNum <= 65535;
}

std::string JoinGameMenu::trim(const std::string& str) {
	auto start = str.begin();
	while (start != str.end() && std::isspace(*start)) {
		++start;
	}

	auto end = str.end();
	while (end != start && std::isspace(*(end - 1))) {
		--end;
	}

	return std::string(start, end);
}