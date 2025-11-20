#pragma once
#include "Ball.h"

class Player : Object
{
public:
	Player();
	Player(GLfloat x, GLfloat y, GLfloat z);

	~Player();

	glm::vec3 getPosition();
	glm::vec3 getRotation();

	void setPosition(glm::vec3 position);
	void setRotation(glm::vec3 rotation);

	void Move(Ball& ball, bool keeper_has_ball);
	void Draw(GLuint vao_player);

	void Sprint();
	void Walk();
	bool isSprint();

	void Shoot(Ball& ball);

	bool isShooting();
	void changeShooting(bool shootinginprogress);
	bool isCurve();
	void changeCurve(bool curve);
	bool isStrong();
	void changeStrong(bool strong);

	bool ishasBall();
	void changehasBall(bool has_ball);

	void keyDown(int keys);
	void keyUp(int keys);
	bool isKey(int keys);



private:
	glm::vec3 velocity = glm::vec3(0.0f, 0.0f, 0.0f);  // 속도
	glm::vec3 direction{};  // 이동 방향
	float acceleration = 0.05f;  // 플레이어의 가속도

	float max_speed = 10.0f; // 최대 속도
	const float deceleration = 0.01f; // 감속 (가속도와 반대)
	
	float distance{};	// 공과 플레이어 사이의 거리
	float shootingpower{};  // 슈팅 파워 변수 (0.0 ~ MAX_SHOOTING_POWER 범위)
	const float max_shootingpower = 50.0f;  // 최대 슈팅 파워
	const float shooting_increase = 1.0f;  // 슈팅 파워 증가량

	bool sprint{};
	bool has_ball= true;	// 첫 렌더링을 위해 true로 바꿈
	bool keystates[256]{};	// 키보드 상태
	bool shootingInprogress{};  // 슈팅 진행 중 여부 (d 키가 눌린 상태인지)
	bool curve = false;			// 감아차기
	bool strong = false;		// 파워슛

};