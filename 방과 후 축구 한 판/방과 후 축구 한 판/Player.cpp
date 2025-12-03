#include "Player.h"

// ---------------------------------------------------------------
// Player
Player::Player() = default;
Player::Player(GLfloat x, GLfloat y, GLfloat z) {
	this->position = { x, y, z };
};
Player::~Player() = default;

glm::vec3 Player::getPosition() { return this->position; };
glm::vec3 Player::getRotation() { return this->rotation; };

void Player::setPosition(glm::vec3 position) {
	this->position = position;
};
void Player::setRotation(glm::vec3 rotation) {
	this->rotation = rotation;
};

void Player::Draw(GLuint vao_player) {
	glBindVertexArray(vao_player); //--- VAO를 바인드하기

	glm::mat4 Trans = glm::mat4(1.0f);
	// 플레이어 이동을 위한 위치 업데이트
	Trans = glm::translate(Trans, this->position);
	// 회전 적용
	Trans = glm::rotate(Trans, this->rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));  // 회전 적용

	unsigned int modelLocation = glGetUniformLocation(shaderProgramID, "modelTransform");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Trans));

	GLuint playerTextures = loadBMP("플레이어 색.bmp");
	glActiveTexture(GL_TEXTURE0);      // 텍스처 생성
	glBindTexture(GL_TEXTURE_2D, playerTextures); // 텍스처 ID 사용

	// 셰이더에 텍스처 유닛 0을 연결
	GLuint texLocation = glGetUniformLocation(shaderProgramID, "Texture");
	glUniform1i(texLocation, 0);  // 유닛 0을 grassTexture에 연결

	// 플레이어 그리기
	glDrawArrays(GL_TRIANGLES, 0, PlayerVertexCount);

	deleteTexture(playerTextures);
};

void Player::Sprint() {
	this->sprint = true;
};
void Player::Walk() {
	this->sprint = false;
};
bool Player::isSprint() { return this->sprint; };

void Player::Shoot(Ball& ball) {
	if (this->shootingInprogress && distance <= 1.5f) {
		if (this->strong)
			this->shootingpower = 100.f;
		else
			this->shootingpower += this->shooting_increase;
		if (!this->strong && this->shootingpower > this->max_shootingpower) {
			this->shootingpower = this->max_shootingpower;
		}
	}

	if (this->shootingInprogress) {
		if (ball.getPosition().y == 0.0f) {  // 공이 바닥에 있을 때만 발사
			ball.setVelocity(glm::normalize(ball.getVelocity()) * this->shootingpower);  // 슈팅 파워 적용
			ssystem->playSound(s_shoot, 0, false, &c_shoot);

			if (this->curve) {
				ball.changeCurve();
				ball.setVelocity(glm::vec3(ball.getVelocity().x + 0.5f, this->shootingpower / 2.0f, ball.getVelocity().z));  // 살짝 위로 튕기게 할 수도 있음
			}
			else if (this->strong) {
				ball.changeStrong();
			}
			else	
				ball.setVelocity(glm::vec3(ball.getVelocity().x, this->shootingpower / 200.0f, ball.getVelocity().z));  // 살짝 위로 튕기게 할 수도 있음
			
			
		}
		this->shootingpower = 0.0f;  // 슈팅 파워 초기화
		this->shootingInprogress = false;  // 슈팅 진행 중 상태 초기화
	}
	this->has_ball = false;
}

bool Player::isShooting() { return this->shootingInprogress; };
void Player::changeShooting(bool shootinginprogress) {
	this->shootingInprogress = !this->shootingInprogress;
}
bool Player::isCurve() { return this->curve; };
void Player::changeCurve(bool curve) {
	this->curve = !this->curve;
}
bool Player::isStrong() { return this->strong; };
void Player::changeStrong(bool strong) {
	this->strong = !this->strong;
}

bool Player::ishasBall() { return this->has_ball; };
void Player::changehasBall(bool has_ball) {
	this->has_ball = !this->has_ball;
}

void Player::keyDown(int keys) {
	this->keystates[keys] = true;
};
void Player::keyUp(int keys) {
	this->keystates[keys] = false;
};
bool Player::isKey(int keys) { return this->keystates[keys]; };