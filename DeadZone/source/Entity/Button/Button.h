#pragma once

#include "../CollidableEntity.h"
#include "../TexturableEntity.h"
#include "../../Renderer/TextRenderer.h"

#include <map>
#include <iostream>

class Button : public virtual CollidableEntity, public virtual TexturableEntity
{
public:

	static enum class Status
	{
		DEFAULT = 0,
		HOVERED = 1,
		CLICKED = 2
	};

	static enum class InputStatus
	{
		DEFAULT = 0,
		FOCUSED = 1,
		INVALID_INPUT = 2
	};

protected:

	std::string label;
	std::map<Button::Status, std::string> status_TextureNames;
	Status status = Button::Status::DEFAULT;

	double textOffsetX;
	double textScale;
	std::string font;
	glm::vec3 fontColor;

	void updateTexture();
	void updateTextureNonInteractive();

	glm::vec3 uniformColor;

	bool hasFocus = false;

	bool isInteractive = true;

	std::map<Button::InputStatus, std::string> inputStatus_TextureNames;
	InputStatus inputStatus = Button::InputStatus::DEFAULT;
	InputStatus previousInputStatus = Button::InputStatus::DEFAULT;

public:

	Button(double x, double y, double drawWidth, double drawHeight, double rotateAngle, double speed, double collideWidth, double collideHeight,
			const std::map<Button::Status, std::string>& status_TextureNames_, const std::string& label_ = "", double textOffsetX_ = 50,
			double textScale = 1.0, const std::string& font_ = "Antonio", bool textCenteredX = false, const glm::vec3& fontColor_ = glm::vec3{ 0.0, 0.0, 0.0 },
			const glm::vec3& uniformColor_ = glm::vec3{ -1.0, -1.0, -1.0 }, bool isInteractive_ = true, 
			const std::map<Button::InputStatus, std::string> inputStatus_TextureNames_ = {});
	Button();
	virtual ~Button();

	void onCollide(CollidableEntity& other, glm::vec2 overlap) override;
	void update() override;
	
	std::string getLabel() const { return label; }
	void setLabel(const std::string& label) { this->label = label; }

	double getTextScale()const { return textScale; }
	void setTextScale(double textScale_) { textScale = textScale_; }

	void draw() override;
	void draw(double x_, double y_, double width_, double height_);

	void setX(double x) { this->x = x; }
	void setY(double y) { this->y = y; }

	void setStatus(Button::Status st) { status = st; }
	Button::Status getStatus() const { return status; }

	void setDefault();
	void setHovered();
	void setClicked();

	void setFocused();
	void setUnfocused();

	void setInvalidInputStatusAndPreviousInputStatus();
	void setDefaultInputStatusAndPreviousInputStatus();

	void setFontColor(const glm::vec3& fontColor_);

	inline void setTextureNameForStatus(const Button::Status& status, const std::string& textureName) {	status_TextureNames[status] = textureName; }

	inline bool getHasFocus() const { return this->hasFocus; }
	inline void setHasFocus(bool hasFocus_) { this->hasFocus = hasFocus_; }

	inline Button::InputStatus getInputStatus() const { return inputStatus; }
	inline Button::InputStatus getPreviousInputStatus() const { return previousInputStatus; }

	inline void setInputStatus(Button::InputStatus value) { this->inputStatus = value; }
	inline void setPreviousInputStatus(Button::InputStatus value) { if (value == Button::InputStatus::FOCUSED) return; this->previousInputStatus = value; }
};