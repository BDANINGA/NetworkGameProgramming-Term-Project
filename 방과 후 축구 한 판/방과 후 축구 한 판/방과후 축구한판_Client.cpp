#define _CRT_SECURE_NO_WARNINGS
//--- 메인 함수
#pragma comment(lib,"glew32.lib") 
#pragma comment(lib,"freeglut.lib") 
#include "방과후 축구한판_Client.h"

//------------------------------------------------------------------------------------------------------
Player player;
Ball ball;
Keeper keeper(0.0f, 0.0f, -32.0f);

Camera camera;
Light light;

PacketInputkey input{};
PacketInputspecialkey s_input{};

extern SOCKET g_ServerSocket;
extern int g_CurrentScores[3]{ 0,0,0 };
extern int g_SecondsRemaining{ 300 };
//------------------------------------------------------------------------
bool start = true;
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

	// server로부터 데이터를 받아서 사용하려면 인자를 바꿔야 함.
	// ball.Draw(ballPos, ,ballR, ballR_Angle);
	// player.Draw(playerPos, playerR);
	// keeper.Draw(keeperPos);
	ball.Draw(keeper.getPosition(), keeper.ishasBall(), vao_ball);						// server로부터 position, rotationAngle, rotation을 받으면 된다.
	player.Draw(ball, keeper.ishasBall(), vao_player);									// server로부터 position, rotation을 받으면 된다.
	keeper.Draw(ball.getPosition(), player.ishasBall(), vao_player);					// server로부터 position을 받으면 된다.

	drawGoal(vao_goalpost);
	drawBackground();

	// 카메라 설정: 플레이어를 따라가는 카메라 server로부터 받은 player position으로 받아서 설정
	camera.setPosition(player.getPosition() + glm::vec3(0.0f, 1.0f, 5.0f));  // 플레이어 위치 기준으로 카메라 위치 설정 (위 2, 뒤 5)
	camera.setDirection(player.getPosition());  // 카메라는 플레이어를 향하도록 설정

	// 시간 sprintf
	int minutes = g_SecondsRemaining / 60;
	int seconds = g_SecondsRemaining % 60;
	char timeString[32];
	sprintf(timeString, "TIME %02d:%02d", minutes, seconds);

	// 점수 sprintf
	char scoreString[128];
	sprintf(scoreString, "P1: %d  P2: %d  P3: %d",
		g_CurrentScores[0], g_CurrentScores[1], g_CurrentScores[2]);

	// 텍스트 그리기 
	drawText(10, height - 30, timeString); // 좌측 상단
	drawText(width - 250, height - 30, scoreString); // 우측 상단

	viewTransform();
	projectionTransform();
	make_Light();

	glutSwapBuffers(); // 화면에 출력하기
}
GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수
{
	glViewport(0, 0, w, h);
}


// 조작
void Keyboard(unsigned char key, int x, int y) {
	switch (key) {

	case 'd':
	case 'D':
		PlayerInput('d', input, g_ServerSocket, true);
		//player.changeShooting(player.isShooting());
		break;

	case 'r':
	case 'R':
		// debug:ball_reset
		ball.setPosition(player.getPosition().x, player.getPosition().y, player.getPosition().z);
		ball.setVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
		break;
	case 'e':
	case 'E':
		PlayerInput('e', input, g_ServerSocket, true);
		//player.Sprint();
		break;
	case 'z':
	case 'Z':
		PlayerInput('z', input, g_ServerSocket, true);
		//player.changeCurve(player.isCurve());
		break;
	case 'c':
	case 'C':
		PlayerInput('c', input, g_ServerSocket, true);
		//player.changeStrong(player.isStrong());
		break;
	case 'q':
		// debug: game_quit
		glutLeaveMainLoop();
		break;
	}
	glutPostRedisplay();
}
void KeyboardUp(unsigned char key, int x, int y) {
	switch (key) {
	case 'd':
	case 'D':
		PlayerInput('d', input, g_ServerSocket, false);
		//player.Shoot(ball);
		break;
	case 'e':
	case 'E':
		PlayerInput('e', input, g_ServerSocket, false);
		//player.Walk();
		break;
	case 'z':
	case 'Z':
		PlayerInput('z', input, g_ServerSocket, false);
		//player.changeCurve(player.isCurve());
		break;
	case 'c':
	case 'C':
		PlayerInput('c', input, g_ServerSocket, false);
		//player.changeStrong(player.isStrong());
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
		player.keyUp(GLUT_KEY_UP);
		break;
	case GLUT_KEY_DOWN:
		PlayerInput_special(GLUT_KEY_DOWN, s_input, g_ServerSocket, false);
		player.keyUp(GLUT_KEY_DOWN);
		break;
	case GLUT_KEY_LEFT:
		PlayerInput_special(GLUT_KEY_LEFT, s_input, g_ServerSocket, false);
		player.keyUp(GLUT_KEY_LEFT);
		break;
	case GLUT_KEY_RIGHT:
		PlayerInput_special(GLUT_KEY_RIGHT, s_input, g_ServerSocket, false);
		player.keyUp(GLUT_KEY_RIGHT);
		break;
	}
	glutPostRedisplay();  // 화면 갱신
}

GLvoid gameoverScene() {

};
void UI_Update() {
	if (gameover) gameoverScene();
	else drawScene();
}