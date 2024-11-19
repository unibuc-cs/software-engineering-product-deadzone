#include "Entity.h"

Entity::Entity(double x, double y, double drawWidth, double drawHeight, double rotateAngle = 0.0, double speed = 0.0)
	: x(x), y(y), drawWidth(drawWidth), drawHeight(drawHeight), rotateAngle(rotateAngle), speed(speed)
	, deleteEntity(false)
{

}

void Entity::update()
{

}

Entity::~Entity()
{

}

std::string Entity::convertToJson(int indetention)
{
	std::string res, depth;
	while (indetention--) depth += "\t";
	res = depth + "[\n";
	res += depth + "\t\"x\" : " + std::to_string(this->x) + ",\n";
	res += depth + "\t\"y\" : " + std::to_string(this->y) + ",\n";
	res += depth + "\t\"drawWidth\" : " + std::to_string(this->drawWidth) + ",\n";
	res += depth + "\t\"drawHeight\" : " + std::to_string(this->drawHeight) + ",\n";
	res += depth + "\t\"rotateAngle\" : " + std::to_string(this->rotateAngle) + ",\n";
	res += depth + "\t\"speed\" : " + std::to_string(this->speed) + ",\n";
	res += depth + "\t\"deleteEntity\" : " + std::to_string(this->deleteEntity) + "\n";
	res += depth + "]\n";

	return res;
}

void Entity::setFieldValue(const std::string field, const std::string value)
{

	if (field == "x") this->x = std::stod(value);
	if (field == "y") this->y = std::stod(value);
	if (field == "drawWidth") this->drawWidth = std::stod(value);
	if (field == "drawHeight") this->drawHeight = std::stod(value);
	if (field == "rotateAngle") this->rotateAngle = std::stod(value);
	if (field == "speed") this->speed = std::stod(value);
	if (field == "deleteEntity") this->deleteEntity = std::stoi(value);
	

}

//Assumes that it is always an valid format
void Entity::modifyFromJson(const std::string& str, int startAt)
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
			startAt += 3;
			while (startAt < str.length() && str[startAt] != ',' && str[startAt] != ']') {
				value += str[startAt];
				startAt++;
			}

			this->setFieldValue(field, value);
		}
		else startAt++;
	}
}

