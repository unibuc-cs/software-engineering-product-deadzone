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

protected:

	std::string label;
	std::map<Button::Status, std::string> status_TextureNames;
	Status status = Button::Status::DEFAULT;

	double textOffsetX;
	double textScale;
	std::string font;
	glm::vec3 fontColor;

	void updateTexture();

	glm::vec3 uniformColor;

public:

	Button(double x, double y, double drawWidth, double drawHeight, double rotateAngle, double speed, double collideWidth, double collideHeight, const std::map<Button::Status, std::string>& status_TextureNames_, const std::string& label_ = "", double textOffsetX_ = 50, double textScale = 1.0, const std::string& font_ = "Antonio", bool textCenteredX = false, const glm::vec3& fontColor_ = glm::vec3{0.0, 0.0, 0.0}, const glm::vec3& uniformColor_ = glm::vec3{ -1.0, -1.0, -1.0 } );
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

	void setFontColor(const glm::vec3& fontColor_);

	inline void setTextureNameForStatus(const Button::Status& status, const std::string& textureName) {	status_TextureNames[status] = textureName; }

};