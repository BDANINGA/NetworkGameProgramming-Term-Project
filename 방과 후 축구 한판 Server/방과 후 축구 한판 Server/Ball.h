#pragma once
#include "Object.h"

const float maxdistance = 0.75f;  // 공과 플레이어 사이의 최대 거리
const float min_speed = 0.01f; // 최저 속도 한계값

class Ball : Object
{

public:
	Ball();
	Ball(GLfloat x, GLfloat y, GLfloat z);

	~Ball();

	glm::vec3 getPosition();
	glm::vec3 getRotation();
	glm::vec3 getVelocity();
	glm::vec3 getDirection();
	float getRotationAngle();
	float getAcceleration();

	void setPosition(GLfloat x, GLfloat y, GLfloat z);
	void setRotation(glm::vec3 rotation);
	void setVelocity(glm::vec3 velocity);
	void setDirection(glm::vec3 direction);
	void setAcceleration(float acceleration);
	void setMaxspeed(float max_speed);

	void Move(glm::vec3 keeperPos, bool keeper_has_ball);

	void changeCurve();
	void changeStrong();
	void changeFirst();
	bool isFirst();

	void setLastOwner(int playerID);
	int getLastOwner();

private:
	glm::vec3 velocity{};
	glm::vec3 direction{};
	float acceleration{};
	float max_speed = 0.3f; // 공의 최대 속도
	
	// 공의 회전 각도와 회전 속도 변수 추가
	float rotationAngle{};  // 회전 각도 (라디안)
	const float rotationSpeed = 5.0f;  // 공의 회전 속도 (단위: 라디안/초)
	const float CURVE_TURN_SPEED = 0.03f; // 감아차기 회전량

	bool curve{};
	bool strong{};
	bool first = false;

	int lastOwnerID = -1;
};