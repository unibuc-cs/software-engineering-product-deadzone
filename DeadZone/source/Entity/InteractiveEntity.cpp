#include "InteractiveEntity.h"

InteractiveEntity::InteractiveEntity(double x, double y, double drawWidth, double drawHeight, double rotateAngle, double speed, double interactionWidth, double interactionHeight)
	: Entity(x, y, drawWidth, drawHeight, rotateAngle, speed)
	, interactionWidth(interactionWidth), interactionHeight(interactionHeight)
{

}

std::string InteractiveEntity::convertToJson(int indetention, bool showSuper)
{
	std::string res, depth;
	while (indetention--) depth += "\t";
	res = depth + "[\n";
	res += depth + "\t\"interactionWidth\" : " + std::to_string(this->interactionWidth) + ",\n";
	res += depth + "\t\"interactionHeight\" : " + std::to_string(this->interactionHeight);
	if (showSuper) {
		Entity* aux = this;
		res += ",\n";
		res += depth + "\t\"Entity\" : " + aux->convertToJson(depth.length() + 1) + "\n";
	}
	else res += "\n";
	res += depth + "]\n";

	return res;
}

void InteractiveEntity::setFieldValue(const std::string field, const std::string value)
{
	if (field == "interactionWidth") this->interactionWidth = stod(value);
	if (field == "interactionHeight") this->interactionWidth = stod(value);
	if (field == "Entity") {
		Entity* aux = this;
		aux->modifyFromJson(value, 0);
	}

}

//Assumes that it is always an valid format
void InteractiveEntity::modifyFromJson(const std::string& str, int startAt)
{
	while (true) {
		if (startAt >= str.length()) throw std::invalid_argument("Bad json format: " + str);
		if (str[startAt] == ']') break;

		if (str[startAt] == '\"') {
			//Assumes that it always has the form "field" : value,
			std::string field;
			startAt++;
			while (startAt < str.length() && str[startAt] != '\"') {
				field += str[startAt];
				startAt++;
			}

			std::string value;
			startAt += 4;
			if (field == "Entity") {
				while (startAt < str.length() && str[startAt] != ']') {
					value += str[startAt];
					startAt++;
				}
				value += "]";
			}
			else {
				while (startAt < str.length() && str[startAt] != ',' && str[startAt] != ']') {
					value += str[startAt];
					startAt++;
				}
			}

			this->setFieldValue(field, value);
		}
		else startAt++;
	}
}