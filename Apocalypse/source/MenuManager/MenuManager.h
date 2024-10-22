#pragma once

#include <iostream>
#include <vector>
#include <exception>

#include "MenuBase/MenuBase.h"


class noMenuOpened : public std::runtime_error
{
public:
	noMenuOpened(const std::string& msg = "") :runtime_error(msg) {}
};

class MenuManager
{
	std::vector<MenuBase*> menuStack;

public:
	
	static MenuManager& get();

	MenuBase& top() const;

	void pop();

	void push(MenuBase& m);

	void replaceTop(MenuBase& m);

	void clear();

	inline size_t size() const { return menuStack.size(); };

	void draw() const;

	void play();
};

