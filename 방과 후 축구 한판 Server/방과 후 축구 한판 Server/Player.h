#pragma once
#include "Ball.h"

class Player : Object
{
public:
	Player();
	Player(glm::vec3 pos);

	~Player();

	glm::vec3 getPosition();
	glm::vec3 getRotation();
	float getBallDistance(Ball& ball);

	void setPosition(glm::vec3 pos);

	void Move(Ball& ball, bool keeper_has_ball);

	void changeSprint();
	void Sprint();
	void Walk();
	bool isSprint();

	void TackleCool();
	void changeTackle();
	void DoTackle();
	bool isTackle();

	void ShootInProgress(Ball& ball);
	void Shoot(Ball& ball);
	bool isShooting();
	bool isShoot();

	void changeShooting();
	bool isCurve();
	void changeCurve();
	bool isStrong();
	void changeStrong();

	bool ishasBall();
	void toggleHasBall(bool toggle);

	void keyDown(int keys);
	void keyUp(int keys);
	void keyDown2(int keys);
	void keyUp2(int keys);
	bool isKey(int keys);



private:
	glm::vec3 velocity = glm::vec3(0.0f, 0.0f, 0.0f);  // 속도
	glm::vec3 direction{};  // 이동 방향
	float acceleration = 0.05f;  // 플레이어의 가속도

	float max_speed = 10.0f; // 최대 속도
	const float deceleration = 0.01f; // 감속 (가속도와 반대)
	
	float distance{};	// 공과 플레이어 사이의 거리
	float shootingpower{};  // 슈팅 파워 변수 (0.0 ~ MAX_SHOOTING_POWER 범위)
	const float max_shootingpower = 1.0f;  // 최대 슈팅 파워
	const float shooting_increase = 0.05f;  // 슈팅 파워 증가량

	time_t tacklecool{};	// Tackle의 쿨타임 Session Loop에서 연산.

	bool sprint{};
	bool has_ball= false;	// 첫 렌더링을 위해 true로 바꿈
	bool keystates[256]{};	// 스페셜 키 상태
	bool keystates2[256]{};	// 기본 키 상태
	bool shootingInprogress{};  // 슈팅 진행 중 여부 (d 키가 눌린 상태인지)
	bool curve = false;			// 감아차기
	bool strong = false;		// 파워슛
	bool tackle = false;		// 태클
	bool shooting = false;		// 슛 상태

};

// TackleEvent - 태클로 공 소유권 이전 여부를 서버에서 관리해야한다.
void TackleEvent(Player* player, int count, Ball& ball);