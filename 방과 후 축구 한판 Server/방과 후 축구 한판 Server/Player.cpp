#include "Player.h"

#define CAN_KICK_DISTANCE 0.5f
// ---------------------------------------------------------------
// Player

extern PacketRenderData statePkt;

Player::Player() = default;
Player::Player(glm::vec3 pos) {
	this->position = { pos };
};
Player::~Player() = default;

glm::vec3 Player::getPosition() { return this->position; };
glm::vec3 Player::getRotation() { return this->rotation; };
float Player::getBallDistance(Ball& ball) { return glm::distance(this->position, ball.getPosition()); };

void Player::setPosition(glm::vec3 pos) {
	this->position = pos;
};

void Player::Move(Ball& ball) {
	this->distance = glm::distance(this->position, ball.getPosition());
	if (this->has_ball) {
		glm::vec3 distanceVec = this->position - ball.getPosition();
		distanceVec = glm::normalize(distanceVec);

		this->rotation.y = glm::atan(-distanceVec.x, -distanceVec.z);

		if (this->sprint) {
			this->acceleration = 0.005f * GAMESESSION_TICK;
			this->max_speed = 0.1f;

			if (this->distance <= CAN_KICK_DISTANCE) {
				ball.setAcceleration(0.1f * GAMESESSION_TICK);
				statePkt.SoundKind[1] = true;
			}
			else
				ball.setAcceleration(0);

		}
		else {
			this->acceleration = 0.002f * GAMESESSION_TICK;  // 플레이어의 가속도
			this->max_speed = 0.07f;

			if (this->distance <= CAN_KICK_DISTANCE) {
				ball.setAcceleration(0.01f * GAMESESSION_TICK);
				statePkt.SoundKind[1] = true;
			}
			else
				ball.setAcceleration(0);
		}
		if (this->distance >= CAN_KICK_DISTANCE) {	// 플레이어가 공을 찰 수 있는 위치로 다가가도록 이동
			this->velocity.x -= distanceVec.x * this->acceleration;
			this->velocity.z -= distanceVec.z * this->acceleration;
		}
		else {
			// 방향키에 따른 플레이어 이동 방향 설정
			if (this->keystates[GLUT_KEY_UP]) {
				this->direction.z = -1;  // 뒤쪽으로 이동
				
				ball.setDirection(glm::vec3(ball.getDirection().x, ball.getDirection().y, -1.0f));
				ball.setRotation(glm::vec3(-1.0f, 0.0f, 0.0f));
			}
			else if (this->keystates[GLUT_KEY_DOWN]) {
				this->direction.z = 1;  // 앞쪽으로 이동
				
				ball.setDirection(glm::vec3(ball.getDirection().x, ball.getDirection().y, 1.0f));
				ball.setRotation(glm::vec3(1.0f, 0.0f, 0.0f)); // 반시계방향 회전
			}
			else {
				this->direction.z = 0;
				ball.setDirection(glm::vec3(ball.getDirection().x, ball.getDirection().y, 0.0f));
			}
			if (this->keystates[GLUT_KEY_LEFT]) {
				this->direction.x = -1;  // 왼쪽으로 이동
			
				ball.setDirection(glm::vec3(-1.0f, ball.getDirection().y, ball.getDirection().z));
				ball.setRotation(glm::vec3(0.0f, 0.0f, 1.0f)); // 반시계방향 회전
			}
			else if (this->keystates[GLUT_KEY_RIGHT]) {
				this->direction.x = 1;  // 오른쪽으로 이동

			
				ball.setDirection(glm::vec3(1.0f, ball.getDirection().y, ball.getDirection().z));
				ball.setRotation(glm::vec3(0.0f, 0.0f, -1.0f));  // 180도 회전
			}
			else {
				this->direction.x = 0;

				ball.setDirection(glm::vec3(0.0f, ball.getDirection().y, ball.getDirection().z));
			}
			if (this->keystates[GLUT_KEY_UP] && this->keystates[GLUT_KEY_LEFT]) {
				ball.setDirection(glm::vec3(-0.5f, ball.getDirection().y, -0.5f));
			}
			else if (this->keystates[GLUT_KEY_UP] && this->keystates[GLUT_KEY_RIGHT]) {
				ball.setDirection(glm::vec3(0.5f, ball.getDirection().y, -0.5f));
			}
			else if (this->keystates[GLUT_KEY_DOWN] && this->keystates[GLUT_KEY_LEFT]) {
				ball.setDirection(glm::vec3(-0.5f, ball.getDirection().y, 0.5f));
			}
			else if (this->keystates[GLUT_KEY_DOWN] && this->keystates[GLUT_KEY_RIGHT]) {
				ball.setDirection(glm::vec3(0.5f, ball.getDirection().y, 0.5f));
			}

			// 가속도를 적용하기 전에 이동 방향이 0이 아닌지 확인
			if (glm::length(this->direction) > 0.0f) {
				// 이동 방향을 정규화하여 가속도를 적용
				this->direction = glm::normalize(this->direction);

				// 현재 속도에 가속도를 적용하여 속도 증가
				this->velocity += this->direction * this->acceleration;  // 가속도 적용

				// 최대 속도를 제한
				if (glm::length(this->velocity) > this->max_speed) {
					this->velocity = glm::normalize(this->velocity) * (this->max_speed);  // 최대 속도 제한
				}
			}
			else {
				// 이동하지 않으면 감속을 적용
				if (glm::length(this->velocity) > 0.0f) {
					this->velocity -= glm::normalize(this->velocity) * (this->deceleration);  // 감속
				}

				// 감속 후 속도가 너무 낮아지면 속도를 0으로 설정
				if (glm::length(this->velocity) < min_speed) {
					this->velocity = glm::vec3(0.0f, 0.0f, 0.0f);
				}
			}
		}
	}
	else {
		if (this->sprint) {
			this->acceleration = 0.005f * GAMESESSION_TICK;
			this->max_speed = 0.1f;
		}
		else {
			this->acceleration = 0.002f * GAMESESSION_TICK;  // 플레이어의 가속도
			this->max_speed = 0.07f;
		}

		if (this->keystates[GLUT_KEY_UP]) {
			this->direction.z = -1;  // 뒤쪽으로 이동
			this->rotation.y = glm::radians(180.0f);
		}
		else if (this->keystates[GLUT_KEY_DOWN]) {
			this->direction.z = 1;  // 앞쪽으로 이동
			this->rotation.y = glm::radians(0.0f);
		}
		else {
			this->direction.z = 0;
		}
		if (this->keystates[GLUT_KEY_LEFT]) {
			this->direction.x = -1;  // 왼쪽으로 이동
			this->rotation.y = glm::radians(-90.0f);
		}
		else if (this->keystates[GLUT_KEY_RIGHT]) {
			this->direction.x = 1;  // 오른쪽으로 이동
			this->rotation.y = glm::radians(90.0f);
		}
		else {
			this->direction.x = 0;
		}
		if (this->keystates[GLUT_KEY_UP] && this->keystates[GLUT_KEY_LEFT]) {
			this->rotation.y = glm::radians(225.0f);
		}
		else if (this->keystates[GLUT_KEY_UP] && this->keystates[GLUT_KEY_RIGHT]) {
			this->rotation.y = glm::radians(135.0f);
		}
		else if (this->keystates[GLUT_KEY_DOWN] && this->keystates[GLUT_KEY_LEFT]) {
			this->rotation.y = glm::radians(-45.0f);
		}
		else if (this->keystates[GLUT_KEY_DOWN] && this->keystates[GLUT_KEY_RIGHT]) {
			this->rotation.y = glm::radians(45.0f);
		}

		// 가속도를 적용하기 전에 이동 방향이 0이 아닌지 확인
		if (glm::length(this->direction) > 0.0f) {
			// 이동 방향을 정규화하여 가속도를 적용
			this->direction = glm::normalize(this->direction);

			// 현재 속도에 가속도를 적용하여 속도 증가
			this->velocity += this->direction * this->acceleration;  // 가속도 적용

			// 최대 속도를 제한
			if (glm::length(this->velocity) > this->max_speed) {
				this->velocity = glm::normalize(this->velocity) * (this->max_speed);  // 최대 속도 제한
			}
		}
		else {
			// 이동하지 않으면 감속을 적용
			if (glm::length(this->velocity) > 0.0f) {
				this->velocity -= glm::normalize(this->velocity) * (this->deceleration);  // 감속
			}

			// 감속 후 속도가 너무 낮아지면 속도를 0으로 설정
			if (glm::length(this->velocity) < min_speed) {
				this->velocity = glm::vec3(0.0f, 0.0f, 0.0f);
			}
		}
	}

	// 마찰력 적용
	if (this->position.y == 0.0f) {
		this->velocity *= FRICTION;
	}
	// x와 z 경계에 닿으면 반사 처리
	if (this->position.x < -20.0f) {
		this->velocity = glm::vec3(0.0f, 0.0f, 0.0f);
		this->acceleration = this->acceleration * -1.0f;
		this->position.x = -19.5f;
	}
	else if (this->position.x > 20.0f) {
		this->velocity = glm::vec3(0.0f, 0.0f, 0.0f);
		this->acceleration = this->acceleration * -1.0f;
		this->position.x = 19.5f;
	}

	if (this->position.z < -40.0f) {
		this->velocity = glm::vec3(0.0f, 0.0f, 0.0f);
		this->acceleration = this->acceleration * -1.0f;
		this->position.z = -39.5f;
	}
	else if (this->position.z > 40.0f) {
		this->velocity = glm::vec3(0.0f, 0.0f, 0.0f);
		this->acceleration = this->acceleration * -1.0f;
		this->position.z = 39.5f;
	}

	// 속도를 기준으로 플레이어 위치 업데이트
	this->position += this->velocity;  // 현재 속도를 반영하여 플레이어 위치 이동

	if (!this->has_ball && !ball.isFirst() && this->distance <= CAN_KICK_DISTANCE) {
		this->has_ball = true;
		ball.changeFirst(true);
	}
};

void Player::changeSprint() {
	this->sprint = this->keystates2['e'];
}
void Player::Sprint() {
	this->sprint = true;
};
void Player::Walk() {
	this->sprint = false;
};
bool Player::isSprint() { return this->sprint; };

void Player::changeTackle() {
	this->tackle = false;
}
void Player::TackleCool() {
	if(this->tacklecool < 5) this->tacklecool += 1;
}
void Player::DoTackle() {
	if (this->keystates2['x'] && this->tacklecool >= 5 && !this->has_ball) {
		this->tacklecool = 0.0f;
		this->tackle = true;		// 나머지는 서버에서 연산시킨다.
		std::cout << "태클 준비" << std::endl;
	}
}
bool Player::isTackle() { return this->tackle; };

void Player::ShootInProgress(Ball& ball) {
	if (this->shootingInprogress && distance <= 1.5f) {
		if (this->strong)
			this->shootingpower += this->shooting_increase * 2;
		else
			this->shootingpower += this->shooting_increase;
		if (!this->strong && this->shootingpower > this->max_shootingpower) {
			this->shootingpower = this->max_shootingpower;
		}
		this->shooting = true;
	}
}
void Player::Shoot(Ball& ball) {
	if (!this->shootingInprogress) {
		ball.setVelocity(glm::normalize(ball.getVelocity()) * this->shootingpower);  // 슈팅 파워 적용
		ball.setMaxspeed(3.0f);
		if (this->curve) {
			ball.changeCurve();
			ball.setVelocity(glm::vec3(ball.getVelocity().x + 0.5f, this->shootingpower, ball.getVelocity().z));  // 살짝 위로 튕기게 할 수도 있음
		}
		else if (this->strong) {
			ball.changeStrong();
			ball.setVelocity(glm::vec3(ball.getVelocity().x, this->shootingpower / 2, ball.getVelocity().z));
		}
		else
			ball.setVelocity(glm::vec3(ball.getVelocity().x, this->shootingpower, ball.getVelocity().z));  // 살짝 위로 튕기게 할 수도 있음
		this->shootingpower = 0;
		this->shooting = false;
		this->has_ball = false;
		ball.changeFirst(false);

		statePkt.SoundKind[2] = true;

	}
}
bool Player::isShooting() { return this->shootingInprogress; };
void Player::changeShooting() {
	this->shootingInprogress = this->keystates2['d'];
}
bool Player::isShoot() { return this->shooting; };

bool Player::isCurve() { return this->curve; };
void Player::changeCurve() {
	this->curve = this->keystates2['z'];
}
bool Player::isStrong() { return this->strong; };
void Player::changeStrong() {
	this->strong = this->keystates2['c'];
}

bool Player::ishasBall() { return this->has_ball; };
void Player::toggleHasBall(bool toggle) {
	this->has_ball = toggle;
}

void Player::keyDown(int keys) {
	this->keystates[keys] = true;
};
void Player::keyUp(int keys) {
	this->keystates[keys] = false;
};
void Player::keyDown2(int keys) {
	this->keystates2[keys] = true;
};
void Player::keyUp2(int keys) {
	this->keystates2[keys] = false;
};
bool Player::isKey(int keys) { return this->keystates[keys]; };

// 태클의 조건
// 1. Space 키가 눌렸을 때 (쿨타임 1.5초)
// 2. 자기가 공을 소유하고 있지 않을때
// 3. 주변에 공이 있으며, 그 공을 다른 플레이어가 소유하고 있을 때 
// 공의 위치를 자신에게 맞추며,
// 그 공의 소유권을 자기 것으로 만든다.

// TackleEvent - 태클로 공 소유권 이전 여부를 서버에서 관리해야한다.
void TackleEvent(Player* player, int count, Ball& ball) {
	for (int i = 0; i < count; ++i) {
		if (player[i].isTackle()) {
			for (int j = 0; j < count; ++j) {
				if (player[j].ishasBall()){ 
					if (player[i].getBallDistance(ball) < player[j].getBallDistance(ball)) {
						player[j].toggleHasBall(false);
						player[i].toggleHasBall(true);
					}
				}
			}
			player[i].changeTackle();
		}	
	}
}