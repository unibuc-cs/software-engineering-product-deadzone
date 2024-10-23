#include "MenuBase.h"


MenuBase::MenuBase(double x, double y, double drawWidth, double drawHeight, double rotateAngle, double speed, const std::string& textureName2D, double buttonWidth_, double buttonHeight_):
	Entity(x, y, drawWidth, drawHeight, rotateAngle, speed),
	TexturableEntity(x, y, drawWidth, drawHeight, rotateAngle, speed, textureName2D),
	buttonWidth(buttonWidth_),
	buttonHeight(buttonHeight_)
{

}

double MenuBase::getButtonCoordsX() { return x + WindowManager::get().getWindowWidth() / 2.0 - drawWidth / 2; }
double MenuBase::getButtonCoordsY() { return -y + WindowManager::get().getWindowHeight() / 2.0 - drawHeight / 2; }

std::string MenuBase::toUpper(const std::string& s)
{
	std::string rez = s;
	for (size_t i = 0; i < rez.size(); ++i)
		rez[i] = toupper(rez[i]);

	return rez;
}

std::string MenuBase::toLower(const std::string& s)
{
	std::string rez = s;
	for (size_t i = 0; i < rez.size(); ++i)
		rez[i] = tolower(rez[i]);

	return rez;
}

std::string MenuBase::initCap(const std::string& s)
{
	std::string rez = toLower(s);
	if (rez.size())
		rez[0] = toupper(rez[0]);

	return rez;
}


void MenuBase::hoverAnyButton(Button& button)
{
	button.setHovered();
	button.setLabel(toUpper(button.getLabel()));
	button.setFontColor(glm::vec3(1.0, 1.0, 0.0));
}

void MenuBase::hoverLostAnyButton(Button& button)
{
	button.setDefault();
	button.setLabel(initCap(button.getLabel()));
	button.setFontColor(glm::vec3(0.0, 0.0, 0.0));
}