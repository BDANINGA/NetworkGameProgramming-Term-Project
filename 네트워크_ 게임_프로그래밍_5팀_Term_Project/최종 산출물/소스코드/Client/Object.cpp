#include "Object.h"

Object::Object() = default;
glm::vec3 Object::getPosition() { return position; };

void drawGoal(GLuint vao_goalpost) {
	glBindVertexArray(vao_goalpost); //--- VAO를 바인드하기

	glm::mat4 Trans = glm::mat4(1.0f);
	glm::mat4 Scale = glm::mat4(1.0f);
	glm::mat4 Transform = glm::mat4(1.0f);
	unsigned int modelLocation = glGetUniformLocation(shaderProgramID, "modelTransform");


	Scale = glm::scale(Scale, glm::vec3(2.0f, 0.05f, 1.0f));
	Trans = glm::translate(Trans, glm::vec3(0.0f, 2.0f, -35.0f));
	Transform = Trans * Scale;

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Transform));

	GLuint goalTextures = loadBMP("골대 색.bmp");
	glActiveTexture(GL_TEXTURE0);      // 텍스처 생성
	glBindTexture(GL_TEXTURE_2D, goalTextures); // 텍스처 ID 사용

	// 셰이더에 텍스처 유닛 0을 연결
	GLuint texLocation = glGetUniformLocation(shaderProgramID, "Texture");
	glUniform1i(texLocation, 0);  // 유닛 0을 grassTexture에 연결

	glDrawArrays(GL_TRIANGLES, 0, GoalPostVertexCount);

	Trans = glm::mat4(1.0f);
	Scale = glm::mat4(1.0f);
	Transform = glm::mat4(1.0f);

	Scale = glm::scale(Scale, glm::vec3(0.05f, 1.0f, 1.0f));
	Trans = glm::translate(Trans, glm::vec3(-2.0f, 1.0f, -35.0f));
	Transform = Trans * Scale;

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Transform));
	glDrawArrays(GL_TRIANGLES, 0, GoalPostVertexCount);

	Trans = glm::mat4(1.0f);
	Scale = glm::mat4(1.0f);
	Transform = glm::mat4(1.0f);

	Scale = glm::scale(Scale, glm::vec3(0.05f, 1.0f, 1.0f));
	Trans = glm::translate(Trans, glm::vec3(2.0f, 1.0f, -35.0f));
	Transform = Trans * Scale;

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Transform));
	glDrawArrays(GL_TRIANGLES, 0, GoalPostVertexCount);

	Trans = glm::mat4(1.0f);
	Scale = glm::mat4(1.0f);
	Transform = glm::mat4(1.0f);

	Scale = glm::scale(Scale, glm::vec3(2.0f, 1.0f, 0.05f));
	Trans = glm::translate(Trans, glm::vec3(0.0f, 1.0f, -36.0f));
	Transform = Trans * Scale;

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Transform));
	glDrawArrays(GL_TRIANGLES, 0, GoalPostVertexCount);

	Transform = glm::mat4(1.0f);
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Transform));

	deleteTexture(goalTextures);
}
void drawBackground() {
	// 잔디 그리기
	GLfloat grassVertices[] = {
		// x, y, z
		-20.0f, -0.1f, -40.0f,  // 왼쪽 하단
		20.0f, -0.1f, -40.0f,   // 오른쪽 하단
		20.0f, -0.1f, 40.0f,    // 오른쪽 상단
		-20.0f, -0.1f, 40.0f    // 왼쪽 상단
	};

	GLfloat grassColor[] = {
		1.0f, 1.0f, 1.0f,  // 초록색
		1.0f, 1.0f, 1.0f,  // 초록색
		1.0f, 1.0f, 1.0f,  // 초록색
		1.0f, 1.0f, 1.0f   // 초록색
	};

	GLfloat grassNormal[] = {
		// x, y, z
		0.0f, 0.0f, 0.0f,  // 왼쪽 하단
		0.0f, 0.0f, 0.0f,  // 오른쪽 하단
		0.0f, 0.0f, 0.0f,  // 오른쪽 상단
		0.0f, 0.0f, 0.0f   // 왼쪽 상단
	};

	GLfloat grassTexture[] = {
		// x, y, z
		1.0f, 0.0f,  // 왼쪽 하단
		1.0f, 1.0f,   // 오른쪽 하단
		0.0f, 1.0f,    // 오른쪽 상단
		0.0f, 0.0f    // 왼쪽 상단
	};

	GLuint vao_grass, vbo_grass[4];

	glGenVertexArrays(1, &vao_grass); // VAO 생성
	glBindVertexArray(vao_grass); // VAO 바인드

	glGenBuffers(4, vbo_grass); // VBO 4개 생성

	// 1번째 VBO: Grass vertices (좌표)
	glBindBuffer(GL_ARRAY_BUFFER, vbo_grass[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(grassVertices), grassVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	// 2번째 VBO: Grass color (색상)
	glBindBuffer(GL_ARRAY_BUFFER, vbo_grass[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(grassColor), grassColor, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	// 3번째 VBO: Grass color (색상)
	glBindBuffer(GL_ARRAY_BUFFER, vbo_grass[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(grassNormal), grassNormal, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	// 4번째 VBO: Grass color (색상)
	glBindBuffer(GL_ARRAY_BUFFER, vbo_grass[3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(grassTexture), grassTexture, GL_STATIC_DRAW);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(3);



	GLuint grassTextures = loadBMP("축구장.bmp");
	glActiveTexture(GL_TEXTURE0);      // 텍스처 생성
	glBindTexture(GL_TEXTURE_2D, grassTextures); // 텍스처 ID 사용

	// 셰이더에 텍스처 유닛 0을 연결
	GLuint texLocation = glGetUniformLocation(shaderProgramID, "Texture");
	glUniform1i(texLocation, 0);  // 유닛 0을 grassTexture에 연결

	// glDrawArrays를 이용하여 xz 평면을 그린다
	glDrawArrays(GL_QUADS, 0, 4); // 4개의 정점으로 사각형 그리기

	// VAO, VBO 정리
	glBindVertexArray(0);
	glDeleteBuffers(4, vbo_grass);
	glDeleteVertexArrays(1, &vao_grass);

	deleteTexture(grassTextures);

	// ----------------------------------------------------------------------------------------------------
	// 벽면 그리기
	for (int i = 0; i < 3; ++i) {
		if (i == 0) {
			GLfloat backgroundVertices[] = {
				// x, y, z
				-20.0f, -0.3f, -40.0f,  // 왼쪽 하단
				20.0f, -0.3f, -40.0f,   // 오른쪽 하단
				20.0f, 30.0f, -40.0f,    // 오른쪽 상단
				-20.0f, 30.0f, -40.0f    // 왼쪽 상단
			};

			GLfloat backgroundColor[] = {
				1.0f, 1.0f, 1.0f,  // 초록색
				1.0f, 1.0f, 1.0f,  // 초록색
				1.0f, 1.0f, 1.0f,  // 초록색
				1.0f, 1.0f, 1.0f   // 초록색
			};

			GLfloat backgroundNormal[] = {
				// x, y, z
				0.0f, 1.0f, 0.0f,  // 왼쪽 하단
				0.0f, 1.0f, 0.0f,  // 오른쪽 하단
				0.0f, 1.0f, 0.0f,  // 오른쪽 상단
				0.0f, 1.0f, 0.0f   // 왼쪽 상단
			};

			GLfloat backgroundTexture[] = {
				// x, y, z
				0.0f, 0.0f,  // 왼쪽 하단
				1.0f, 0.0f,   // 오른쪽 하단
				1.0f, 1.0f,    // 오른쪽 상단
				0.0f, 1.0f    // 왼쪽 상단
			};

			GLuint vao_background, vbo_background[4];

			glGenVertexArrays(1, &vao_background); // VAO 생성
			glBindVertexArray(vao_background); // VAO 바인드

			glGenBuffers(4, vbo_background); // VBO 4개 생성

			// 1번째 VBO: Grass vertices (좌표)
			glBindBuffer(GL_ARRAY_BUFFER, vbo_background[0]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(backgroundVertices), backgroundVertices, GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(0);

			// 2번째 VBO: Grass color (색상)
			glBindBuffer(GL_ARRAY_BUFFER, vbo_background[1]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(backgroundColor), backgroundColor, GL_STATIC_DRAW);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(1);

			// 3번째 VBO: Grass color (색상)
			glBindBuffer(GL_ARRAY_BUFFER, vbo_background[2]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(backgroundNormal), backgroundNormal, GL_STATIC_DRAW);
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(2);

			// 4번째 VBO: Grass color (색상)
			glBindBuffer(GL_ARRAY_BUFFER, vbo_background[3]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(backgroundTexture), backgroundTexture, GL_STATIC_DRAW);
			glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(3);



			GLuint backgroundTextures = loadBMP("학교사진.bmp");
			glActiveTexture(GL_TEXTURE0);      // 텍스처 생성
			glBindTexture(GL_TEXTURE_2D, backgroundTextures); // 텍스처 ID 사용

			// 셰이더에 텍스처 유닛 0을 연결
			GLuint texLocation = glGetUniformLocation(shaderProgramID, "Texture");
			glUniform1i(texLocation, 0);  // 유닛 0을 backgroundTexture에 연결

			// glDrawArrays를 이용하여 xz 평면을 그린다
			glDrawArrays(GL_QUADS, 0, 4); // 4개의 정점으로 사각형 그리기

			// VAO, VBO 정리
			glBindVertexArray(0);
			glDeleteBuffers(4, vbo_background);
			glDeleteVertexArrays(1, &vao_background);

			deleteTexture(backgroundTextures);
		}
		else if (i == 1) {
			GLfloat backgroundVertices[] = {
				// x, y, z
				-20.0f, -0.3f, 40.0f,  // 왼쪽 하단
				-20.0f, -0.3f, -40.0f,   // 오른쪽 하단
				-20.0f, 30.0f, -40.0f,    // 오른쪽 상단
				-20.0f, 30.0f, 40.0f    // 왼쪽 상단
			};

			GLfloat backgroundColor[] = {
				1.0f, 1.0f, 1.0f,  // 초록색
				1.0f, 1.0f, 1.0f,  // 초록색
				1.0f, 1.0f, 1.0f,  // 초록색
				1.0f, 1.0f, 1.0f   // 초록색
			};

			GLfloat backgroundNormal[] = {
				// x, y, z
				0.0f, 1.0f, 0.0f,  // 왼쪽 하단
				0.0f, 1.0f, 0.0f,  // 오른쪽 하단
				0.0f, 1.0f, 0.0f,  // 오른쪽 상단
				0.0f, 1.0f, 0.0f   // 왼쪽 상단
			};

			GLfloat backgroundTexture[] = {
				// x, y, z
				0.0f, 0.0f,  // 왼쪽 하단
				1.0f, 0.0f,   // 오른쪽 하단
				1.0f, 1.0f,    // 오른쪽 상단
				0.0f, 1.0f    // 왼쪽 상단
			};

			GLuint vao_background, vbo_background[4];

			glGenVertexArrays(1, &vao_background); // VAO 생성
			glBindVertexArray(vao_background); // VAO 바인드

			glGenBuffers(4, vbo_background); // VBO 4개 생성

			// 1번째 VBO: Grass vertices (좌표)
			glBindBuffer(GL_ARRAY_BUFFER, vbo_background[0]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(backgroundVertices), backgroundVertices, GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(0);

			// 2번째 VBO: Grass color (색상)
			glBindBuffer(GL_ARRAY_BUFFER, vbo_background[1]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(backgroundColor), backgroundColor, GL_STATIC_DRAW);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(1);

			// 3번째 VBO: Grass color (색상)
			glBindBuffer(GL_ARRAY_BUFFER, vbo_background[2]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(backgroundNormal), backgroundNormal, GL_STATIC_DRAW);
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(2);

			// 4번째 VBO: Grass color (색상)
			glBindBuffer(GL_ARRAY_BUFFER, vbo_background[3]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(backgroundTexture), backgroundTexture, GL_STATIC_DRAW);
			glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(3);



			GLuint backgroundTextures = loadBMP("배경1.bmp");
			glActiveTexture(GL_TEXTURE0);      // 텍스처 생성
			glBindTexture(GL_TEXTURE_2D, backgroundTextures); // 텍스처 ID 사용

			// 셰이더에 텍스처 유닛 0을 연결
			GLuint texLocation = glGetUniformLocation(shaderProgramID, "Texture");
			glUniform1i(texLocation, 0);  // 유닛 0을 backgroundTexture에 연결

			// glDrawArrays를 이용하여 xz 평면을 그린다
			glDrawArrays(GL_QUADS, 0, 4); // 4개의 정점으로 사각형 그리기

			// VAO, VBO 정리
			glBindVertexArray(0);
			glDeleteBuffers(4, vbo_background);
			glDeleteVertexArrays(1, &vao_background);

			deleteTexture(backgroundTextures);
		}
		else if (i == 2) {
			GLfloat backgroundVertices[] = {
				// x, y, z
				20.0f, -0.3f, 40.0f,  // 왼쪽 하단
				20.0f, -0.3f, -40.0f,   // 오른쪽 하단
				20.0f, 30.0f, -40.0f,    // 오른쪽 상단
				20.0f, 30.0f, 40.0f    // 왼쪽 상단
			};

			GLfloat backgroundColor[] = {
				1.0f, 1.0f, 1.0f,  // 초록색
				1.0f, 1.0f, 1.0f,  // 초록색
				1.0f, 1.0f, 1.0f,  // 초록색
				1.0f, 1.0f, 1.0f   // 초록색
			};

			GLfloat backgroundNormal[] = {
				// x, y, z
				0.0f, 1.0f, 0.0f,  // 왼쪽 하단
				0.0f, 1.0f, 0.0f,  // 오른쪽 하단
				0.0f, 1.0f, 0.0f,  // 오른쪽 상단
				0.0f, 1.0f, 0.0f   // 왼쪽 상단
			};

			GLfloat backgroundTexture[] = {
				// x, y, z
				0.0f, 0.0f,  // 왼쪽 하단
				1.0f, 0.0f,   // 오른쪽 하단
				1.0f, 1.0f,    // 오른쪽 상단
				0.0f, 1.0f    // 왼쪽 상단
			};

			GLuint vao_background, vbo_background[4];

			glGenVertexArrays(1, &vao_background); // VAO 생성
			glBindVertexArray(vao_background); // VAO 바인드

			glGenBuffers(4, vbo_background); // VBO 4개 생성

			// 1번째 VBO: Grass vertices (좌표)
			glBindBuffer(GL_ARRAY_BUFFER, vbo_background[0]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(backgroundVertices), backgroundVertices, GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(0);

			// 2번째 VBO: Grass color (색상)
			glBindBuffer(GL_ARRAY_BUFFER, vbo_background[1]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(backgroundColor), backgroundColor, GL_STATIC_DRAW);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(1);

			// 3번째 VBO: Grass color (색상)
			glBindBuffer(GL_ARRAY_BUFFER, vbo_background[2]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(backgroundNormal), backgroundNormal, GL_STATIC_DRAW);
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(2);

			// 4번째 VBO: Grass color (색상)
			glBindBuffer(GL_ARRAY_BUFFER, vbo_background[3]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(backgroundTexture), backgroundTexture, GL_STATIC_DRAW);
			glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(3);



			GLuint backgroundTextures = loadBMP("배경1.bmp");
			glActiveTexture(GL_TEXTURE0);      // 텍스처 생성
			glBindTexture(GL_TEXTURE_2D, backgroundTextures); // 텍스처 ID 사용

			// 셰이더에 텍스처 유닛 0을 연결
			GLuint texLocation = glGetUniformLocation(shaderProgramID, "Texture");
			glUniform1i(texLocation, 0);  // 유닛 0을 backgroundTexture에 연결

			// glDrawArrays를 이용하여 xz 평면을 그린다
			glDrawArrays(GL_QUADS, 0, 4); // 4개의 정점으로 사각형 그리기

			// VAO, VBO 정리
			glBindVertexArray(0);
			glDeleteBuffers(4, vbo_background);
			glDeleteVertexArrays(1, &vao_background);

			deleteTexture(backgroundTextures);
		}
	}

}