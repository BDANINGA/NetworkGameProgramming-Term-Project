#pragma once
#include "Object.h"

class Light : Object {
public:
	glm::vec3 getColor();
	glm::vec3 getPosition();
	glm::vec3 getViewPos();
	float getAmbientlight();
	

private:
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 postion = glm::vec3(30.0f, 10.0f, 50.0f);
	glm::vec3 viewpos = glm::vec3(-30.0f, 0.0f, -50.0f);
	float ambientlight = 0.5f;

};