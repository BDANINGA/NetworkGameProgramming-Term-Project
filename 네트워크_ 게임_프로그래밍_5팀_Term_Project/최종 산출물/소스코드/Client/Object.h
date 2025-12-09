#pragma once
#include "function.h"

const float FRICTION = 0.98f; // 마찰력 (속도 감소 비율)
const float GRAVITY = -0.05f;  // 중력 값, 음수로 설정하여 아래로 떨어지도록
const float BALL_BOUNCE_DAMPING = 0.8f;  // 바운스 감쇠 (0~1 사이로 설정, 1이면 반사 후 속도 그대로)

class Object
{
protected:
	glm::vec3 position{};
	glm::vec3 rotation{};
	glm::vec3 scale{};
	float rotationAngle{};

public:
	Object();
	glm::vec3 getPosition();
};

void drawGoal(GLuint vao_goalpost);
void drawBackground();