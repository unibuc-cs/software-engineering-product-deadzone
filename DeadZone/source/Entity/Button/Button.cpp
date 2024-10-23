#include "Button.h"
#include "../../ResourceManager/ResourceManager.h"
#include "../../Renderer/TextRenderer.h"
#include <iostream>
#include "../../Renderer/SpriteRenderer.h"
#include "../../WindowManager/WindowManager.h"

Button::Button():Entity(0,0,0,0,0,0), CollidableEntity(0,0,0,0,0,0,0,0), TexturableEntity(0, 0, 0, 0, 0, 0, ""), label(""), textOffsetX(50), textScale(1.0){
}

Button::Button(double x, double y, double drawWidth, double drawHeight, double rotateAngle, double speed, double collideWidth, double collideHeight, const std::map<Button::Status, std::string>& status_TextureNames_, const std::string& label_, double textOffsetX_, double textScale_, const std::string& font_, bool textCenteredX, const glm::vec3& fontColor_, const glm::vec3& uniformColor_) :
	Entity(x, y, drawWidth, drawHeight, rotateAngle, speed),
	CollidableEntity(x, y, drawWidth, drawHeight, rotateAngle, speed, collideWidth, collideHeight),
	TexturableEntity(x, y, drawWidth, drawHeight, rotateAngle, speed, ""),
	label(label_),
	status_TextureNames(status_TextureNames_),
	textOffsetX(textOffsetX_),
	textScale(textScale_),
	font(font_),
	fontColor(fontColor_),
	uniformColor(uniformColor_)
{
	auto default_texture = status_TextureNames.find(Button::Status::DEFAULT);
	if (default_texture != status_TextureNames.end())
		TexturableEntity::setTextureName2D(default_texture->second);
	else
		std::cout << "Button constructor: no default texture\n";

	if (textCenteredX)
	{
		textOffsetX = (drawWidth - TextRenderer::get().getTextWidth(ResourceManager::getFont(font), textScale, label)) / 2.0;
	}
}


Button::~Button()
{

}

void Button::onCollide(CollidableEntity& other, glm::vec2 overlap) {}

void Button::update() { }

void Button::draw()
{
	updateTexture();
	// std::cout << TexturableEntity::getTextureName2D() << "\n";
	// TexturableEntity::draw();

	double xForTexture = x - WindowManager::get().getWindowWidth() / 2.0 + drawWidth / 2.0;
	double yForTexture = WindowManager::get().getWindowHeight() / 2.0 - y - drawHeight / 2.0;

	// double textScale = 1.0;

	// double textOffsetX = 50;
	double textOffsetY = (drawHeight - TextRenderer::get().getTextHeight(ResourceManager::getFont(font), textScale)) / 2.0;

	double xForText = x + textOffsetX;
	double yForText = y + textOffsetY;

	if(uniformColor == glm::vec3{ -1.0, -1.0, -1.0 })
		SpriteRenderer::get().draw(ResourceManager::getShader("sprite"), ResourceManager::getTexture(this->textureName2D), glm::vec2(xForTexture, yForTexture), glm::vec2(drawWidth, drawHeight), 0);
	else
	{
		SpriteRenderer::get().draw(ResourceManager::getShader("player"), ResourceManager::getTexture(this->textureName2D), glm::vec2(xForTexture, yForTexture), glm::vec2(drawWidth, drawHeight), 0, uniformColor);
	}

	TextRenderer::get().draw(ResourceManager::getShader("text"), ResourceManager::getFont(font), label, xForText, yForText, textScale, fontColor);
}


void Button::draw(double x_, double y_, double width_, double height_)
{
	double xForTexture = x - WindowManager::get().getWindowWidth() / 2.0 + drawWidth / 2.0;
	double yForTexture = WindowManager::get().getWindowHeight() / 2.0 - y - drawHeight / 2.0;

	double textScale = 1.0;

	double textOffsetX = 50;
	double textOffsetY = (drawHeight - TextRenderer::get().getTextHeight(ResourceManager::getFont("Antonio"), textScale)) / 2.0;

	double xForText = x + textOffsetX;
	double yForText = y + textOffsetY;


	float xTr = x_;
	float yTr = WindowManager::get().getWindowHeight() - y_ - height_;
	float widthTr = width_;
	float heightTr = height_;

	SpriteRenderer::get().draw(ResourceManager::getShader("sprite"), ResourceManager::getTexture(this->textureName2D), glm::vec2(xForTexture, yForTexture), glm::vec2(drawWidth, drawHeight), 0, glm::vec3(1.0f), glm::vec2(xTr, yTr), glm::vec2(widthTr, heightTr));


	// TextRenderer::get().draw(ResourceManager::getShader("text"), ResourceManager::getFont("Antonio"), label, xForText, yForText, textScale, fontColor);
}


void Button::updateTexture()
{
	auto default_texture = status_TextureNames.find(status);
	if (default_texture != status_TextureNames.end())
		TexturableEntity::setTextureName2D(default_texture->second);
	else
		std::cout << "Button setTexture: no texture for specified status\n";
}

void Button::setDefault()
{
	status = Button::Status::DEFAULT;
	updateTexture();
}

void Button::setHovered()
{
	status = Button::Status::HOVERED;
	updateTexture();
}

void Button::setClicked()
{
	status = Button::Status::CLICKED;
	updateTexture();
}

void Button::setFontColor(const glm::vec3& fontColor_)
{
	fontColor = fontColor_;
}

