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

void Keeper::setPosition(glm::vec3 position) {
	this->position = position;
};
void Keeper::setRotation(glm::vec3 rotation) {
	this->rotation = rotation;
};

void Keeper::Move(glm::vec3 ballPos, bool has_ball) {
	// 골키퍼의 이동 범위 설정
	this->targetX = ballPos.x;
	if (!has_ball)
	{
		this->velocity = 0.05f;
		//targetY = ballPos.y;
	}

	else {
		this->targetY = -0.1f;
		this->minX = -1.0f;
		this->maxX = 1.0f;
	}
	// 목표 위치를 범위 내로 제한
	this->targetX = glm::clamp(this->targetX, this->minX, this->maxX);
	this->targetY = glm::clamp(this->targetY, this->minY, this->maxY);

	// 골키퍼 위치를 목표 위치로 부드럽게 이동 (lerp 방식 사용)
	this->position.x = glm::mix(this->position.x, this->targetX, this->velocity);
	if (ballPos.z - this->position.z <= 5)
		this->position.y = glm::mix(this->position.y, this->targetY, this->velocity);
};
void Keeper::Draw(GLuint vao_player) {
	glBindVertexArray(vao_player); //--- VAO를 바인드하기

	// move를 하지않고 Server로부터 recv로 position을 받는다.
	// this->Move(ballPos, has_ball);

	glm::mat4 Trans = glm::mat4(1.0f);
	// 플레이어 이동을 위한 위치 업데이트
	Trans = glm::translate(Trans, this->position);

	unsigned int modelLocation = glGetUniformLocation(shaderProgramID, "modelTransform");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Trans));

	GLuint keeperTextures = loadBMP("플레이어 색.bmp");
	glActiveTexture(GL_TEXTURE0);      // 텍스처 생성
	glBindTexture(GL_TEXTURE_2D, keeperTextures); // 텍스처 ID 사용

	// 셰이더에 텍스처 유닛 0을 연결
	GLuint texLocation = glGetUniformLocation(shaderProgramID, "Texture");
	glUniform1i(texLocation, 0);  // 유닛 0을 grassTexture에 연결

	// 플레이어 그리기
	glDrawArrays(GL_TRIANGLES, 0, PlayerVertexCount);

	deleteTexture(keeperTextures);
};

void Keeper::hasBall() {
	this->has_ball = true;
};
void Keeper::NothasBall() {
	this->has_ball = false;
};
bool Keeper::ishasBall() { return this->has_ball; };