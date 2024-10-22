#include "ScrollableElement.h"
#include "../../ResourceManager/ResourceManager.h"
#include "../../Renderer/SpriteRenderer.h"
#include "../../WindowManager/WindowManager.h"


ScrollableElement::ScrollableElement(double x, double y, double drawWidth, double drawHeight, double rotateAngle, double speed, const std::string& textureName2D,
	const std::map<std::string, Button>& buttons_,
	const std::map<std::string, std::function<void(Button&)>>& hoverFunctions_,
	const std::map<std::string, std::function<void(Button&)>>& hoverLostFunctions_,
	const std::map<std::string, std::function<void(Button&)>>& clickFunctions_):
		Entity(x, y, drawWidth, drawHeight, rotateAngle, speed),
		TexturableEntity(x, y, drawWidth, drawHeight, rotateAngle, speed, textureName2D),
		ButtonGroup(buttons_, hoverFunctions_, hoverLostFunctions_, clickFunctions_)
{

}

void ScrollableElement::draw()
{
	SpriteRenderer::get().draw(ResourceManager::getShader("sprite"), ResourceManager::getTexture(this->textureName2D), glm::vec2(x, y), glm::vec2(drawWidth, drawHeight), 0);

	float xForButtonGroup = x + WindowManager::get().getWindowWidth() / 2.0 - drawWidth / 2;
	float yForButtonGroup = 0;
	float widthForButtonGroup = 0;
	float heightForButtonGroup = 0;


	ButtonGroup::draw(x, y, drawWidth, drawHeight);
}

