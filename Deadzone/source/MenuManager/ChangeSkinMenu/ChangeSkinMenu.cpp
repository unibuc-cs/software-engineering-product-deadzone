#include "ChangeSkinMenu.h"

#include "../../ResourceManager/ResourceManager.h"
#include "../../Input/InputHandler.h"
#include "../../Entity/Player/Player.h"
#include "../MenuManager.h"
#include <exception>
#include "../../Map/Map.h"
#include "../../HUD/HUDManager.h"
#include "../../Renderer/SpriteRenderer.h"
#include "../../SoundManager/SoundManager.h"

ChangeSkinMenu::ChangeSkinMenu(double x, double y, double drawWidth, double drawHeight, double rotateAngle, double speed, const std::string& textureName2D) :
	Entity(x, y, drawWidth, drawHeight, rotateAngle, speed),
	TexturableEntity(x, y, drawWidth, drawHeight, rotateAngle, speed, textureName2D),
	MenuBase(x, y, drawWidth, drawHeight, rotateAngle, speed, textureName2D, drawWidth / 4.0, drawHeight / 12.0)
{
	isInMenu = false;

	auto allButtons = loadMenuItems();
	allButtons.insert({ "back", ButtonBuilder::backButton(getButtonCoordsX(), getButtonCoordsY()) });

	buttons.setButtons(allButtons);

	auto clickFunctions = std::map<std::string, std::function<void(Button&)>>{ {ButtonGroup::getAny(), [](Button&) {} },
		{
			"back", ButtonBuilder::backButtonClickFunction
		}
	};

	auto funcs = this->generateBuyFunctions();
	clickFunctions.insert(funcs.begin(), funcs.end());

	buttons.setFunctions(
		std::map<std::string, std::function<void(Button&)>>{{ButtonGroup::getAny(), [](Button&) {} }},
		std::map<std::string, std::function<void(Button&)>>{{ButtonGroup::getAny(), [](Button&) {} }},
		clickFunctions
	);
}

std::map<std::string, Button> ChangeSkinMenu::loadMenuItems()
{
	int id = 1;
	int ordInd = id;
	double offsetSecondColon = 0.0;

	std::map<std::string, Button> rez;

	if (skinColors.size() == 0)
		return rez;

	// first skin is selected
	auto card = ButtonBuilder::ChangeSkinCard(getCardPosX() + offsetSecondColon, getCardPosY(id - 1), buttonWidth, buttonHeight, std::to_string(id), skinColors[0]);
	
	skinSelected = "1_0_card";
	rez.insert(card.begin(), card.end());

	rez[skinSelected].setTextureNameForStatus(Button::Status::DEFAULT, "skinCardSelected");
	rez[skinSelected].setTextureNameForStatus(Button::Status::HOVERED, "skinCardSelected");
	rez[skinSelected].setTextureNameForStatus(Button::Status::CLICKED, "skinCardSelected");
	
	id++;
	ordInd++;


	for(size_t i=1;i<skinColors.size();++i)
	{
		if (getCardPosY(ordInd) >= y + drawHeight)
		{
			ordInd = 1;
			offsetSecondColon = drawWidth * 0.5 + buttonOffsetX;
		}

		auto card = ButtonBuilder::ChangeSkinCard(getCardPosX() + offsetSecondColon, getCardPosY(ordInd - 1), buttonWidth, buttonHeight, std::to_string(id), skinColors[i]);
		
		rez.insert(card.begin(), card.end());

		id++;
		ordInd++;
	}

	return rez;
}

void ChangeSkinMenu::select(int id)
{
	Player::get().setOutfitColor(skinColors[id]);
	try {
		Button& buttonClicked = buttons.getButtonByName(std::to_string(id+1) + "_0_card");
		buttonClicked.setTextureNameForStatus(Button::Status::DEFAULT, "skinCardSelected");
		buttonClicked.setTextureNameForStatus(Button::Status::HOVERED, "skinCardSelected");
		buttonClicked.setTextureNameForStatus(Button::Status::CLICKED, "skinCardSelected");
		if (skinSelected != std::to_string(id + 1) + "_0_card")
		{
			buttons.getButtonByName(skinSelected).setTextureNameForStatus(Button::Status::DEFAULT, "skinCard");
			buttons.getButtonByName(skinSelected).setTextureNameForStatus(Button::Status::HOVERED, "skinCard");
			buttons.getButtonByName(skinSelected).setTextureNameForStatus(Button::Status::CLICKED, "skinCard");
		}
		// else
			// std::cout << "no skin selected\n";
		
		skinSelected = std::to_string(id + 1) + "_0_card";
	}
	catch (noButtonFound& err)
	{
		throw;
	}
}

std::map<std::string, std::function<void(Button&)>> ChangeSkinMenu::generateBuyFunctions()
{
	int id = 1;

	std::map<std::string, std::function<void(Button&)>> rez;

	for (auto i : skinColors)
	{
		// std::cout << std::to_string(id) + "_0_card" << "\n";
		rez[std::to_string(id) + "_0_card"] = [this, id](Button&) {ChangeSkinMenu::select(id - 1);};

		id++;
	}

	return rez;
}

ChangeSkinMenu& ChangeSkinMenu::get()
{
	double dW = WindowManager::get().getWindowWidth() * 0.8;
	double dH = WindowManager::get().getWindowHeight() * 0.9;
	double x = 0;
	double y = (-WindowManager::get().getWindowHeight() + dH) / 2.0;


	static ChangeSkinMenu instance(x, y, dW, dH, 0, 0, "menuScreen0");
	return instance;
}


double ChangeSkinMenu::getCardPosX() {
	return getButtonCoordsX() + buttonOffsetX;
}

double ChangeSkinMenu::getCardPosY(int index) {
	return getButtonCoordsY() + buttonOffsetY + index * (buttonHeight + spaceAfterButton);
}

void ChangeSkinMenu::draw()
{
	SpriteRenderer::get().draw(ResourceManager::getShader("sprite"), ResourceManager::getTexture(this->textureName2D), glm::vec2(x, y), glm::vec2(drawWidth, drawHeight), 0);

	buttons.draw();
}


void ChangeSkinMenu::setupInputComponent()
{
	buttons.activate();
}

void ChangeSkinMenu::playMenu()
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