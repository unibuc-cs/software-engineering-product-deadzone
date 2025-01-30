#include "MenuManager.h"
#include "../Input/InputHandler.h"
#include "../Game/Game.h"


MenuManager& MenuManager::get()
{
	static MenuManager instance;
	return instance;
}

void MenuManager::pop() {
	if (!menuStack.empty())
	{
		if (menuStack.size() > 1)
			menuStack[menuStack.size() - 2]->setIsInMenu(true);
		menuStack[menuStack.size() - 1]->setIsInMenu(false);
	}
	menuStack.pop_back(); 
	if (!menuStack.empty())
	{
		menuStack[menuStack.size() - 1]->setIsInMenu(true);
		menuStack[menuStack.size() - 1]->setupInputComponent();
	}

	if (menuStack.empty())
	{
		Game::get().setGameStatus(Game::GameStatus::InGame);
		InputHandler::setInputComponent(InputHandler::getPlayerInputComponent());
		return;
	}
}

void MenuManager::push(MenuBase& m) { 
	if (!menuStack.empty())
	{
		// m.setIsInMenu(true);
		menuStack[menuStack.size() - 1]->setIsInMenu(false);
	}
	menuStack.push_back(&m); 
	menuStack[menuStack.size() - 1]->setIsInMenu(true);
	menuStack[menuStack.size() - 1]->setupInputComponent();

	menuStack[menuStack.size() - 1]->init();

	Game::get().setGameStatus(Game::GameStatus::InMenu);
}

MenuBase& MenuManager::top() const
{
	// std::cout << this->size() << "\n";
	if (menuStack.empty())
	{
		Game::get().setGameStatus(Game::GameStatus::InGame);
		InputHandler::setInputComponent(InputHandler::getPlayerInputComponent());

		throw noMenuOpened();
	}

	return *menuStack[menuStack.size() - 1];
}

void MenuManager::replaceTop(MenuBase& m)
{
	if (menuStack.empty())
	{
		Game::get().setGameStatus(Game::GameStatus::InGame);
		InputHandler::setInputComponent(InputHandler::getPlayerInputComponent());

		throw noMenuOpened();
	}

	menuStack[menuStack.size() - 1]->setIsInMenu(false);
	menuStack.pop_back();
	menuStack.push_back(&m);
	menuStack[menuStack.size() - 1]->setIsInMenu(true);
	menuStack[menuStack.size() - 1]->setupInputComponent();
}

void MenuManager::clear()
{
	while (!menuStack.empty())
		this->pop();
}

void MenuManager::draw() const
{
	for (auto& i : menuStack)
		i->draw();
}

void MenuManager::play()
{
	//while (MenuManager::size())
	//	if (MenuManager::top().getIsInMenu() == true)
	//		// MenuManager::top().playMenu();
	//		MenuManager::draw();
	//	else
	//		std::cout << "getIsInMenu == false\n";

	if (menuStack.empty())
	{
		Game::get().setGameStatus(Game::GameStatus::InGame);
		InputHandler::setInputComponent(InputHandler::getPlayerInputComponent());

		throw noMenuOpened();
	}

	MenuManager::draw();
}