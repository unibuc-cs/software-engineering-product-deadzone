#pragma once

#include "../MenuBase/MenuBase.h"
#include "../../ButtonGroup/ButtonGroup.h"

class CreateGameMenu : public virtual MenuBase {
private:
	ButtonGroup buttons;
	std::string playerName;
	std::string serverAddress;

	double buttonOffsetX = 100;
	double buttonOffsetY = 100;
	double spaceAfterButton = 20;

	CreateGameMenu(double x, double y, double drawWidth, double drawHeight, double rotateAngle, double speed, const std::string& textureName2D);
	virtual ~CreateGameMenu() = default;
	CreateGameMenu(const CreateGameMenu& other) = delete;
	CreateGameMenu& operator=(const CreateGameMenu& other) = delete;
	CreateGameMenu(const CreateGameMenu&& other) = delete;
	CreateGameMenu& operator=(const CreateGameMenu&& other) = delete;

	std::map<std::string, Button> CreateButtons();

	double getButtonPosX();
	double getButtonPosY(int index = 0);

	void AddLetter(char letter);
	void DeleteLetter();

	void CreateGame(Button& button);

public:
	static CreateGameMenu& get();

	void draw();

	void setupInputComponent() override;
	void playMenu() override;

	void init() override;

};