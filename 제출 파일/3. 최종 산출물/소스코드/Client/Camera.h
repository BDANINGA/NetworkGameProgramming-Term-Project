#pragma once
#include "Player.h"

class Camera : Object {
public:
	glm::vec3 getPosition();
	glm::vec3 getDirection();
	glm::vec3 getUp();

	void setPosition(glm::vec3 position);
	void setDirection(glm::vec3 position);
	void setUp(glm::vec3 position);

private:
	glm::vec3 position{};
	glm::vec3 direction{};
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
};