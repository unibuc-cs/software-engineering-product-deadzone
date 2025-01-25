#pragma once

#include "../MenuBase/MenuBase.h"
#include "../../ButtonGroup/ButtonGroup.h"

class JoinGameMenu : public virtual MenuBase {
private:
	ButtonGroup buttons;
	std::string playerName;
	std::string serverAddress;

	double buttonOffsetX = 100;
	double buttonOffsetY = 100;
	double spaceAfterButton = 20;

	JoinGameMenu(double x, double y, double drawWidth, double drawHeight, double rotateAngle, double speed, const std::string& textureName2D);
	virtual ~JoinGameMenu() = default;
	JoinGameMenu(const JoinGameMenu& other) = delete;
	JoinGameMenu& operator=(const JoinGameMenu& other) = delete;
	JoinGameMenu(const JoinGameMenu&& other) = delete;
	JoinGameMenu& operator=(const JoinGameMenu&& other) = delete;

	std::map<std::string, Button> CreateButtons();

	double getButtonPosX();
	double getButtonPosY(int index = 0);

	void AddLetter(char letter);
	void DeleteLetter();

	void JoinGame(Button& button);

public:
	static JoinGameMenu& get();

	void draw();

	void setupInputComponent() override;
	void playMenu() override;

	void init() override;

	static bool validateIP(const std::string& IP);
	static bool validatePort(const std::string& port);

	static std::string trim(const std::string& str);
};