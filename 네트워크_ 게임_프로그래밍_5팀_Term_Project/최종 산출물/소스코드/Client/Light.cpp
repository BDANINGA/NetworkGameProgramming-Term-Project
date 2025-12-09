#include "Light.h"

glm::vec3 Light::getColor() { return this->color; }
glm::vec3 Light::getPosition() { return this->position; };
glm::vec3 Light::getViewPos() { return this->viewpos; };
float Light::getAmbientlight() { return this->ambientlight; };