#include "MainMenu.h"
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
#include "../ChangeSkinMenu/ChangeSkinMenu.h"
#include "../../SoundManager/SoundManager.h"



MainMenu::MainMenu(double x, double y, double drawWidth, double drawHeight, double rotateAngle, double speed, const std::string& textureName2D) :
	Entity(x, y, drawWidth, drawHeight, rotateAngle, speed),
	TexturableEntity(x, y, drawWidth, drawHeight, rotateAngle, speed, textureName2D),
	MenuBase(x, y, drawWidth, drawHeight, rotateAngle, speed, textureName2D, drawWidth * 0.3, drawHeight * 0.1),
	buttons(std::map<std::string, Button>{
		{ "quit", Button(getButtonPosX(), getButtonPosY(2), buttonWidth, buttonHeight, 0, 0, buttonWidth, buttonHeight, ButtonBuilder::buttonTextures0(), "Quit", 0, 1.0, "Antonio", true) },
		{ "play", Button(getButtonPosX(), getButtonPosY(0), buttonWidth, buttonHeight, 0, 0, buttonWidth, buttonHeight, ButtonBuilder::buttonTextures0(), "Play", 0, 1.0, "Antonio", true) },
		{ "Change skin", Button(getButtonPosX(), getButtonPosY(1), buttonWidth, buttonHeight, 0, 0, buttonWidth, buttonHeight, ButtonBuilder::buttonTextures0(), "Change skin", 0, 1.0, "Antonio", true) }
})
{	
	buttons.setFunctions(
		std::map<std::string, std::function<void(Button&)>>{{ButtonGroup::getAny(), MainMenu::hoverAnyButton }},
		std::map<std::string, std::function<void(Button&)>>{{ButtonGroup::getAny(), MainMenu::hoverLostAnyButton }},
		std::map<std::string, std::function<void(Button&)>>{{ButtonGroup::getAny(), [](Button&) {} },
		{
			"quit", [](Button&){glfwSetWindowShouldClose(WindowManager::get().getWindow(), true);}
		},
		{ "play", [](Button&) {
			MainMenu::get().isInMenu = false;
			MenuManager::get().pop();
			InputHandler::setInputComponent(InputHandler::getPlayerInputComponent());

			if (Map::get().hasBeenLoaded() == false)
				try
				{
					std::ifstream gameFile("config/game.json");
					nlohmann::json gameJSON;
					gameFile >> gameJSON;
					gameFile.close();

					std::string file = gameJSON["map"].get<std::string>();

					Map::get().readMap(file);
				}
				catch (const std::runtime_error& err)
				{
					std::cout << "ERROR::MAP: " << err.what() << std::endl;
				}
				catch (...)
				{
					std::cout << "ERROR::MAP: other error" << std::endl;
				}


			Player::get().setupPlayerInputComponent();

		}
		},
		{
			"Change skin", [](Button&) {
				MenuManager::get().push(ChangeSkinMenu::get());
			}
		}
	
	}
	);

}


MainMenu& MainMenu::get()
{
	double dW = WindowManager::get().getWindowWidth(); //  *0.75;
	double dH = WindowManager::get().getWindowHeight();
	double x = 0;
	double y = 0;


	static MainMenu instance(x, y, dW, dH, 0, 0, "menuScreen0");
	return instance;
}

void MainMenu::draw()
{
	SpriteRenderer::get().draw(ResourceManager::getShader("sprite"), ResourceManager::getTexture(this->textureName2D), glm::vec2(x, y), glm::vec2(drawWidth, drawHeight), 0);

	buttons.draw();

}

double MainMenu::getButtonPosX() {
	return getButtonCoordsX() + buttonOffsetX;
	// return x + WindowManager::get().getWindowWidth() / 2.0 - drawWidth / 2 + buttonOffsetX;
	// return  buttonOffsetX;
}

double MainMenu::getButtonPosY(int index) {
	return getButtonCoordsY() + buttonOffsetY + index * (buttonHeight + spaceAfterButton);
	// return -y + WindowManager::get().getWindowHeight() / 2.0 - drawHeight / 2 + buttonOffsetY + index * (buttonHeight + spaceAfterButton);
	// return drawHeight / 2.0 - buttonOffsetY - buttonHeight / 2.0 - index * (buttonHeight + spaceAfterButton);
}


void MainMenu::setupInputComponent()
{
	buttons.activate();
}

void MainMenu::playMenu()
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

