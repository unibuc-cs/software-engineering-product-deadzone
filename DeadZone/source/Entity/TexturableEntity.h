#pragma once

#include "string"

#include "Entity.h"

class TexturableEntity : public virtual Entity
{
protected:
	std::string textureName2D;

public:

	TexturableEntity(double x, double y, double drawWidth, double drawHeight, double rotateAngle, double speed, const std::string& textureName2D);
	void draw() override;
	virtual ~TexturableEntity();

	std::string getTextureName2D() const { return textureName2D; }
	void setTextureName2D(const std::string& textureName2D) { this->textureName2D = textureName2D; }
};