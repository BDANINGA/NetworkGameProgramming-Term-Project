#include "Keeper.h"

// ---------------------------------------------------------------
// Keeper
Keeper::Keeper() = default;
Keeper::Keeper(GLfloat x, GLfloat y, GLfloat z) {
	this->position = { x, y, z };
};
Keeper::~Keeper() = default;

glm::vec3 Keeper::getPosition() { return this->position; };
glm::vec3 Keeper::getRotation() { return this->rotation; };

void Keeper::Move(glm::vec3 ballPos) {
	// 골키퍼의 이동 범위 설정
	this->targetX = ballPos.x;
	
	// 목표 위치를 범위 내로 제한
	this->targetX = glm::clamp(this->targetX, this->minX, this->maxX);
	this->targetY = glm::clamp(this->targetY, this->minY, this->maxY);

	// 골키퍼 위치를 목표 위치로 부드럽게 이동 (lerp 방식 사용)
	this->position.x = glm::mix(this->position.x, this->targetX, this->velocity);
	if (ballPos.z - this->position.z <= 5)
		this->position.y = glm::mix(this->position.y, this->targetY, this->velocity);
};

void Keeper::hasBall() {
	this->has_ball = true;
};
void Keeper::NothasBall() {
	this->has_ball = false;
};
bool Keeper::ishasBall() { return this->has_ball; };