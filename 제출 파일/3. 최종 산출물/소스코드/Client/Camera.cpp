#include "Camera.h"

glm::vec3 Camera::getPosition() { return this->position; };
glm::vec3 Camera::getDirection() { return this->direction; };
glm::vec3 Camera::getUp() { return this->up; };

void Camera::setPosition(glm::vec3 position) {
	this->position = position;
}
void Camera::setDirection(glm::vec3 direction) {
	this->direction = direction;
}
void Camera::setUp(glm::vec3 up) {
	this->up = up;
}