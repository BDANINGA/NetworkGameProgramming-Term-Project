#include "Object.h"

Object::Object() = default;
glm::vec3 Object::getPosition() { return position; };
float Object::getDistance(const Object &other) {
	return glm::distance(this->position, other.position);
}