#include "Ball.h"

// ---------------------------------------------------------------
// Ball
Ball::Ball() {
	this->rotation = glm::vec3(0.1f, 0.1f, 0.1f);
	this->scale = glm::vec3(0.015f, 0.015f, 0.015f);
};
Ball::Ball(GLfloat x, GLfloat y, GLfloat z) {
	this->position = { x, y, z };
	this->rotation = glm::vec3(0.1f, 0.1f, 0.1f);
	this->scale = glm::vec3(0.015f, 0.015f, 0.015f);
};
Ball::~Ball() = default;

glm::vec3 Ball::getPosition() { return this->position; };
glm::vec3 Ball::getRotation() { return this->rotation; };
glm::vec3 Ball::getVelocity() { return this->velocity; };
glm::vec3 Ball::getDirection() { return this->direction; };
float Ball::getAcceleration() { return this->acceleration; };

void Ball::setPosition(GLfloat x, GLfloat y, GLfloat z) {
	this->position.x = x;
	this->position.y = y;
	this->position.z = z;
};
void Ball::setRotation(glm::vec3 rotation) {
	this->rotation = rotation;
};
void Ball::setRotationAngle(float rotationAngle) {
	this->rotationAngle = rotationAngle;
}
void Ball::setVelocity(glm::vec3 velocity) {
	this->velocity = velocity;
};
void Ball::setDirection(glm::vec3 direction) {
	this->direction = direction;
}
void Ball::setAcceleration(float acceleration) {
	this->acceleration = acceleration;
}

void Ball::Move(glm::vec3 keeperPos, bool keeper_has_ball) {
	// 공의 속도 업데이트
	this->velocity += this->acceleration * this->direction;

	// 골대의 각 부품 좌표 및 크기
	glm::vec3 goalBarPos = glm::vec3(0.0f, 2.0f, -35.0f);
	glm::vec3 goalBarScale = glm::vec3(2.0f, 0.05f, 1.0f);

	glm::vec3 leftPostPos = glm::vec3(-2.0f, 1.0f, -35.0f);
	glm::vec3 leftPostScale = glm::vec3(0.05f, 1.0f, 1.0f);

	glm::vec3 rightPostPos = glm::vec3(2.0f, 1.0f, -35.0f);
	glm::vec3 rightPostScale = glm::vec3(0.05f, 1.0f, 1.0f);

	glm::vec3 bottomBarPos = glm::vec3(0.0f, 1.0f, -36.0f);
	glm::vec3 bottomBarScale = glm::vec3(2.0f, 1.0f, 0.05f);

	glm::vec3 startPos = this->position;
	glm::vec3 endPos = this->position + this->velocity; // 공의 이동 방향

	// **골키퍼와의 충돌 처리**
	glm::vec3 keeperSize = glm::vec3(0.5f, 0.7f, 0.5f);  // 골키퍼 몸통 크기 설정
	//std::cout << keeperPos.x - keeperSize.x << std::endl;
	// 충돌 체크: 공과 골키퍼가 충돌했는지 검사
	if (this->position.x > keeperPos.x - keeperSize.x && this->position.x < keeperPos.x + keeperSize.x &&
		this->position.z > keeperPos.z - keeperSize.z && this->position.z < keeperPos.z + keeperSize.z &&
		this->position.y > keeperPos.y - keeperSize.y && this->position.y < keeperPos.y + keeperSize.y) {
		// 공이 골키퍼 몸통과 충돌했다면
		std::cout << "골키퍼와 충돌!" << std::endl;
		keeper_has_ball = 1;
		this->velocity *= 0.0f;
	}
	else
	{
		keeper_has_ball = 0;
	}
	// 골대와의 충돌 처리
	if (
		(checkSegmentCollision(startPos, endPos, rightPostPos, rightPostScale) && this->position.z <= -32) ||
		(checkSegmentCollision(startPos, endPos, leftPostPos, leftPostScale) && this->position.z <= -32) ||
		(checkSegmentCollision(startPos, endPos, goalBarPos, goalBarScale) && this->position.z <= -35) ||
		(checkSegmentCollision(startPos, endPos, bottomBarPos, bottomBarScale) && this->position.z <= -35)
		)
	{
		// 충돌 처리
		this->velocity = -this->velocity * BALL_BOUNCE_DAMPING;
		std::cout << "충돌 골대" << std::endl;

		// 골대에 들어감
		if (checkSegmentCollision(startPos, endPos, bottomBarPos, bottomBarScale) && this->position.z <= -35) {
			std::cout << "골" << std::endl;
			ssystem->playSound(s_goal, 0, false, &c_goal);

			this->position = glm::vec3(0.0f, 0.0f, 0.0f);
			this->velocity = glm::vec3(0.0f, 0.0f, 0.0f);
		}
	}
	// 중력 적용
	this->velocity.y += GRAVITY;

	// 최대 속도 제한
	float speed = glm::length(this->velocity);
	if (speed > max_speed) {
		this->velocity = glm::normalize(this->velocity) * max_speed;
	}

	// 마찰력 적용
	if (this->position.y == 0.0f) {
		this->velocity *= FRICTION;
	}

	// 공이 멈추도록: 일정 속도 이하로 떨어지면 공을 멈춤
	if (glm::length(this->velocity) < min_speed) {
		this->velocity = glm::vec3(0.0f, 0.0f, 0.0f);
		this->rotationAngle = 0.0f;  // 공이 멈추면 회전도 멈춤
	}
	if (this->curve)
		this->velocity.x -= this->CURVE_TURN_SPEED;
	if (this->strong)
		this->velocity.z += 0.5f;


	// 공의 위치 업데이트
	this->position += this->velocity;

	// 공이 바닥에 닿으면 반사
	if (this->position.y < 0.0f) {
		this->position.y = 0.0f;
		this->velocity.y = -this->velocity.y * BALL_BOUNCE_DAMPING;

		this->curve = false;
		this->strong = false;
		// 일정 이하로 떨어지면 멈춤
		if (this->velocity.y < 0.2f)
			this->velocity.y = 0;
	}
	

	// x와 z 경계에 닿으면 반사 처리
	if (this->position.x < -50.0f) {
		this->position = glm::vec3(0.0f, 0.0f, 0.0f);
		this->velocity = glm::vec3(0.0f, 0.0f, 0.0f);
		this->rotationAngle = 0.0f;  // 공이 멈추면 회전도 멈춤
	}
	else if (this->position.x > 50.0f) {
		this->position = glm::vec3(0.0f, 0.0f, 0.0f);
		this->velocity = glm::vec3(0.0f, 0.0f, 0.0f);
		this->rotationAngle = 0.0f;  // 공이 멈추면 회전도 멈춤
	}

	if (this->position.z < -50.0f) {
		this->position = glm::vec3(0.0f, 0.0f, 0.0f);
		this->velocity = glm::vec3(0.0f, 0.0f, 0.0f);
		this->rotationAngle = 0.0f;  // 공이 멈추면 회전도 멈춤
	}
	else if (this->position.z > 50.0f) {
		this->position = glm::vec3(0.0f, 0.0f, 0.0f);
		this->velocity = glm::vec3(0.0f, 0.0f, 0.0f);
		this->rotationAngle = 0.0f;  // 공이 멈추면 회전도 멈춤
	}

	// 회전 각도 업데이트 (공의 진행 방향에 비례)
	this->rotationAngle += glm::length(this->velocity) * rotationSpeed;

};
void::Ball::Draw(glm::vec3 keeperPos, bool keeper_has_ball, GLuint vao_ball) {
	glBindVertexArray(vao_ball); //--- VAO를 바인드하기

	glm::mat4 T = glm::mat4(1.0f);
	glm::mat4 S = glm::mat4(1.0f);
	glm::mat4 R = glm::mat4(1.0f);  // 회전 행렬
	glm::mat4 Trans = glm::mat4(1.0f);

	// move를 하지않고 서버로부터 recv로 postion, rotationAngle, rotation을 받는다.
	// this->Move(keeperPos, keeper_has_ball);
	
	// 회전 적용 (회전 각도 누적 적용)
	R = glm::rotate(R, this->rotationAngle, this->rotation);  // 방향에 따라 회전

	// 두 번째 객체(공) 이동을 위한 위치 업데이트
	T = glm::translate(T, this->position);  // 두 번째 객체의 위치 적용
	S = glm::scale(S, this->scale);  // 크기 조정
	Trans = T * R * S; // 위치, 회전, 크기 순서로 적용

	unsigned int modelLocation = glGetUniformLocation(shaderProgramID, "modelTransform");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Trans));

	GLuint ballTextures = loadBMP("축구공.bmp");
	glActiveTexture(GL_TEXTURE0);      // 텍스처 생성
	glBindTexture(GL_TEXTURE_2D, ballTextures); // 텍스처 ID 사용

	// 셰이더에 텍스처 유닛 0을 연결
	GLuint texLocation = glGetUniformLocation(shaderProgramID, "Texture");
	glUniform1i(texLocation, 0);  // 유닛 0을 grassTexture에 연결

	// 두 번째 객체(공) 그리기
	glDrawArrays(GL_TRIANGLES, 0, BallVertexCount);

	deleteTexture(ballTextures);
};

void Ball::changeCurve() {
	this->curve = !this->curve;
}
void Ball::changeStrong() {
	this->strong = !this->strong;
}