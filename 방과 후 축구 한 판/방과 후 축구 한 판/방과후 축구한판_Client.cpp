#define _CRT_SECURE_NO_WARNINGS
//--- 메인 함수
#pragma comment(lib,"glew32.lib") 
#pragma comment(lib,"freeglut.lib") 
#include "방과후 축구한판_Client.h"

//------------------------------------------------------------------------------------------------------
Player player[MAX_PLAYERS];
Ball ball;
Keeper keeper;

Camera camera;
Light light;

PacketInputkey input{};
PacketInputspecialkey s_input{};
PacketRenderData renderData;
PacketChatMessage chatmessage{};
extern SOCKET g_ServerSocket;

// 점수 sprintf
char scoreString[128];
int g_CurrentScores[MAX_PLAYERS]{};
int g_SecondsRemaining{};

extern int g_MyPlayerID;

//------------------------------------------------------------------------
bool start = true;
bool chat = false;
bool left_button = 0;
// 렌더링
GLvoid drawScene() {
	
	if (start) {
		start = false;
		loadObj();
		InitBuffer();
	}

	//--- 변경된 배경색 설정
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(shaderProgramID);
	glEnable(GL_DEPTH_TEST);   // 깊이 테스트 활성화
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// 카메라 설정: 플레이어를 따라가는 카메라 server로부터 받은 player position으로 받아서 설정
	camera.setPosition(player[g_MyPlayerID].getPosition() + glm::vec3(0.0f, 1.0f, 5.0f));  // 플레이어 위치 기준으로 카메라 위치 설정 (위 2, 뒤 5)
	camera.setDirection(player[g_MyPlayerID].getPosition());  // 카메라는 플레이어를 향하도록 설정

	viewTransform();
	projectionTransform();
	make_Light();

	ball.Draw(vao_ball);						// server로부터 position, rotationAngle, rotation을 받으면 된다.
	for (int i = 0; i < MAX_PLAYERS; ++i) {
		player[i].Draw(vao_player);									// server로부터 position, rotation을 받으면 된다.
		keeper.Draw(vao_player);					// server로부터 position을 받으면 된다.
	}
	drawGoal(vao_goalpost);
	drawBackground();

	if (chat) {
		drawRect2D(50.0f, height - 100.0f, 50.0f, 50.0f, 0.0f, 0.0f, 0.0f, 0.5f);
		drawRect2D(50.0f, height - 50.0f, 50.0f, 10.0f, 0.0f, 0.0f, 0.0f, 1.0f);
		drawText(50.0f, height - 50.0f, chatmessage.message);
	}
	// 시간 sprintf
	int minutes = g_SecondsRemaining / 60;
	int seconds = g_SecondsRemaining % 60;
	char timeString[32];
	sprintf(timeString, "TIME %02d:%02d", minutes, seconds);

	
	sprintf(scoreString, "P1: %d  P2: %d  P3: %d",
		g_CurrentScores[0], g_CurrentScores[1], g_CurrentScores[2]);

	// 텍스트 그리기 
	drawText(10, height - 30, timeString); // 좌측 상단
	drawText(width - 250, height - 30, scoreString); // 우측 상단

	
	glutSwapBuffers(); // 화면에 출력하기
}
GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수
{
	glViewport(0, 0, w, h);
}


// 조작
void Keyboard(unsigned char key, int x, int y) {
	std::cout << chat << std::endl;
	if (chat) {
		int len = strlen(chatmessage.message);
		if (key == 8) {
			if (len > 0)
				chatmessage.message[len - 1] = '\0';
		}
		else {
			chatmessage.message[len] = key;
			chatmessage.message[len + 1] = '\0';
		}
		std::cout << chatmessage.message << std::endl;	
	}

	else {
		switch (key) {

		case 'd':
		case 'D':
			PlayerInput('d', input, g_ServerSocket, true);
			break;
		case 'e':
		case 'E':
			PlayerInput('e', input, g_ServerSocket, true);
			break;
		case 'z':
		case 'Z':
			PlayerInput('z', input, g_ServerSocket, true);
			break;
		case 'c':
		case 'C':
			PlayerInput('c', input, g_ServerSocket, true);
			break;
		case 'x':
		case 'X':
			PlayerInput('x', input, g_ServerSocket, true);
			break;
		case '\r':
			chat = true;
			break;
		case 'q':
			// debug: game_quit
			glutLeaveMainLoop();
			break;
		}
	}
	glutPostRedisplay();
}
void KeyboardUp(unsigned char key, int x, int y) {
	switch (key) {
	case 'd':
	case 'D':
		PlayerInput('d', input, g_ServerSocket, false);
		break;
	case 'e':
	case 'E':
		PlayerInput('e', input, g_ServerSocket, false);
		break;
	case 'z':
	case 'Z':
		PlayerInput('z', input, g_ServerSocket, false);
		break;
	case 'c':
	case 'C':
		PlayerInput('c', input, g_ServerSocket, false);
		break;
	case 'x':
	case 'X':
		PlayerInput('x', input, g_ServerSocket, false);
		break;
	}
	glutPostRedisplay();
}
void Mouse(int button, int state, int x, int y)
{
	float gl_x, gl_y;
	windowToOpenGL(x, y, width, height, gl_x, gl_y);
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {

	}
}
void Motion(int x, int y)
{
	float gl_x, gl_y;
	windowToOpenGL(x, y, width, height, gl_x, gl_y);
	if (left_button == true)
	{
	}
	glutPostRedisplay();
}
GLvoid SpecialKeys(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_UP:
		PlayerInput_special(GLUT_KEY_UP, s_input, g_ServerSocket, true);
		break;
	case GLUT_KEY_DOWN:
		PlayerInput_special(GLUT_KEY_DOWN, s_input, g_ServerSocket, true);
		break;
	case GLUT_KEY_LEFT:
		PlayerInput_special(GLUT_KEY_LEFT, s_input, g_ServerSocket, true);
		break;
	case GLUT_KEY_RIGHT:
		PlayerInput_special(GLUT_KEY_RIGHT, s_input, g_ServerSocket, true);
		break;
	}
	glutPostRedisplay();  // 화면 갱신
}
GLvoid SpecialKeysUp(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_UP:
		PlayerInput_special(GLUT_KEY_UP, s_input, g_ServerSocket, false);
		break;
	case GLUT_KEY_DOWN:
		PlayerInput_special(GLUT_KEY_DOWN, s_input, g_ServerSocket, false);
		break;
	case GLUT_KEY_LEFT:
		PlayerInput_special(GLUT_KEY_LEFT, s_input, g_ServerSocket, false);
		break;
	case GLUT_KEY_RIGHT:
		PlayerInput_special(GLUT_KEY_RIGHT, s_input, g_ServerSocket, false);
		break;
	}
	glutPostRedisplay();  // 화면 갱신
}

GLvoid gameoverScene() {
	//--- 변경된 배경색 설정
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(shaderProgramID);
	glEnable(GL_DEPTH_TEST);   // 깊이 테스트 활성화
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	drawGoal(vao_goalpost);
	drawBackground();

	drawText((width / 2) - 25.0f, height - 30, "Score");
	drawText((width / 5) * 2 + 30.0f, height - 80, scoreString);
	drawText((width / 2) - 40.0f, height - 130, "WinPlayer");
	drawText((width / 2) - 30.0f, height - 180, "Player1");

	drawText(100, height - 230, "P1 경기 전적");
	drawText(100, height - 280, "match: x, Win: y, Lose: z, Winrate: xx");
	drawText(100, height - 330, "P2 경기 전적");
	drawText(100, height - 380, "match: x, Win: y, Lose: z, Winrate: xx");
	drawText(100, height - 430, "P3 경기 전적");
	drawText(100, height - 480, "match: x, Win: y, Lose: z, Winrate: xx");


	camera.setPosition(glm::vec3(0.0f, 1.0f, 5.0f)); 
	camera.setDirection(glm::vec3(0.0f, 0.0f, 0.0f));  

	viewTransform();
	projectionTransform();
	make_Light();

	glutSwapBuffers(); // 화면에 출력하기
};
void UI_Update() {
	if (gameover) gameoverScene();
	else drawScene();
}