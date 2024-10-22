#pragma once

#include "../../Entity/TexturableEntity.h"
#include "../../ButtonGroup/ButtonGroup.h"

class ScrollableElement : public virtual TexturableEntity, public virtual ButtonGroup
{

	
public:
	ScrollableElement(double x, double y, double drawWidth, double drawHeight, double rotateAngle, double speed, const std::string& textureName2D,
		const std::map<std::string, Button>& buttons_,
		const std::map<std::string, std::function<void(Button&)>>& hoverFunctions_,
		const std::map<std::string, std::function<void(Button&)>>& hoverLostFunctions_,
		const std::map<std::string, std::function<void(Button&)>>& clickFunctions_);

	void draw() override;
	void updateButtons();


};
