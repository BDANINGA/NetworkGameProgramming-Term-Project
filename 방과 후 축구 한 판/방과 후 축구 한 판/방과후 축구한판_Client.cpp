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
PacketUserData userdata;
extern PacketLogin MyLogin;

extern SOCKET g_ServerSocket;

GameState g_GameState = STATE_LOGIN; // 초기 상태는 로그인
GLuint g_TexLoginBack = 0;           // 로그인 배경 텍스처 ID

enum LoginCursor {
	LOGIN_CURSOR_NONE,
	LOGIN_CURSOR_ID,
	LOGIN_CURSOR_PW
};
LoginCursor g_LoginCursor = LOGIN_CURSOR_NONE;
char g_InputID[32] = {};
char g_InputPW[32] = {};

// 점수 sprintf
char scoreString[128];
int g_CurrentScores[MAX_PLAYERS]{};
int g_SecondsRemaining{};

char BroadCastChatMessage[9][256]{};

char userdataString[128];

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
		g_TexLoginBack = loadBMP("FootBall_Login_Scene.bmp"); // [필수] 이미지 로드
	}



	switch(g_GameState){
	case STATE_LOGIN:
		Draw2DBackground();

		// 로그인 창
		drawRect2D(400.0f, 200.0f, 400.0f, 200.0f, 1.0f, 1.0f, 1.0f, 0.8f);	// 창
		drawRect2D(420.0f, 330.0f, 360.0f, 50.0f, 0.0f, 0.0f, 0.0f, 1.0f);	// 아이디 바
		drawRect2D(420.0f, 230.0f, 360.0f, 50.0f, 0.0f, 0.0f, 0.0f, 1.0f);	// 비밀번호 바
		drawText(425.0f, 350.0f, "ID:");
		drawText(470.0f, 350.0f, g_InputID); // 입력된 아이디
		drawText(425.0f, 250.0f, "PW:");
		drawText(470.0f, 250.0f, g_InputPW); // 입력된 비밀번호

		// 회원가입 버튼
		drawRect2D(760.0f, 50.0f, 100.0f, 40.0f, 0.0f, 0.0f, 1.0f, 1.0f);
		drawText(770.0f, 65.0f, "Register");
		break;

	case STATE_GAME:
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

		if (chat) {
			int id_len = strlen(MyLogin.userID);
			drawRect2D(50.0f, 90.0f, 300.0f, 300.0f, 0.0f, 0.0f, 0.0f, 0.5f);
			drawRect2D(50.0f, 90.0f, 300.0f, 30.0f, 0.0f, 0.0f, 0.0f, 1.0f);
			drawText(50.0f, 100.0f, chatmessage.message + id_len + 2, 50.0f, 350.0f, 400.0f, 700.0f);
		}

		// BroadCastChatMessage
		for (int i = 0; i < 9; i++)
			drawText(50.0f, 130.0f + 30.0f * i, BroadCastChatMessage[i], 50.0f, 350.0f, 400.0f, 700.0f);
	}
	glutSwapBuffers(); // 화면에 출력하기
}
GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수
{
	glViewport(0, 0, w, h);
}

// 조작
void Keyboard(unsigned char key, int x, int y) {
	if (g_GameState == STATE_LOGIN)
	{
		switch (g_LoginCursor)
		{
			case LOGIN_CURSOR_ID:
			{
				int len = strlen(g_InputID);
				if (key == 8) // Backspace
				{
					if (len > 0)
						g_InputID[len - 1] = '\0';
				}
				else if (key == '\r') // Enter
				{
					g_LoginCursor = LOGIN_CURSOR_PW;
				}
				else if (len < 31) // 최대 길이 제한
				{
					g_InputID[len] = key;
					g_InputID[len + 1] = '\0';
				}
				break;
			}
			case LOGIN_CURSOR_PW:
			{
				int len = strlen(g_InputPW);
				if (key == 8) // Backspace
				{
					if (len > 0)
						g_InputPW[len - 1] = '\0';
				}
				else if (key == '\r') // Enter
				{
					// 로그인 시도
					std::cout << "Attempting login with ID: " << g_InputID << " PW: " << g_InputPW << std::endl;
					PlayerLogin(MyLogin, g_ServerSocket, g_InputID, g_InputPW, false);
					g_InputID[0] = '\0';
					g_InputPW[0] = '\0';
				}
				else if (len < 31) // 최대 길이 제한
				{
					g_InputPW[len] = key;
					g_InputPW[len + 1] = '\0';
				}
				break;
			}
			default:
				break;
		}
	}

	else if (chat) {
		int len = strlen(chatmessage.message);
		int id_len = strlen(MyLogin.userID);
		if (key == 8) {
			if (len > id_len + 2)
				chatmessage.message[len - 1] = '\0';
		}
		else if (key == '\r') {
			if (chatmessage.message[id_len + 2] != '\0')
				PlayerChat(chatmessage, g_ServerSocket);
			chatmessage.message[0] = '\0';
			chat = false;
		}
		else if (len < 255){
			chatmessage.message[len] = key;
			chatmessage.message[len + 1] = '\0';
		}
	}

	else {
		int id_len = strlen(MyLogin.userID);
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
			strcpy(chatmessage.message, MyLogin.userID);
			chatmessage.message[id_len] = ':';
			chatmessage.message[id_len + 1] = ' ';
			chatmessage.message[id_len + 2] = '\0';
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
		if (g_GameState == STATE_LOGIN)
		{
			// 아이디 입력창 클릭
			if (gl_x >= -0.3f && gl_x <= 0.3f && gl_y >= -0.175f && gl_y <= -0.05f)
			{
				g_LoginCursor = LOGIN_CURSOR_ID;
			}
			// 비밀번호 입력창 클릭
			else if (gl_x >= -0.3f && gl_x <= 0.3f && gl_y >= -0.425f && gl_y <= -0.3f)
			{
				g_LoginCursor = LOGIN_CURSOR_PW;
			}
			// 로그인 버튼 클릭
			else if (gl_x >= -0.25f && gl_x <= 0.25f && gl_y >= -0.9f && gl_y <= -0.7f)
			{
				// 로그인 시도
				std::cout << "Attempting login with ID: " << g_InputID << " PW: " << g_InputPW << std::endl;
				PlayerLogin(MyLogin, g_ServerSocket, g_InputID, g_InputPW, false);
				g_InputID[0] = '\0';
				g_InputPW[0] = '\0';
			}
			// 회원가입 버튼 클릭
			else if (gl_x >= 0.266f && gl_x <= 0.433f && gl_y >= -0.875f && gl_y <= -0.78f)
			{
				if(strlen(g_InputID) == 0 || strlen(g_InputPW) == 0) {
					MessageBox(NULL, L"ID and PW cannot be empty for registration.", L"Registration Error", MB_OK);
				}
				else
				{
					// 회원가입 시도
					std::cout << "Attempting registration with ID: " << g_InputID << " PW: " << g_InputPW << std::endl;
					PlayerLogin(MyLogin, g_ServerSocket, g_InputID, g_InputPW, true);
					g_InputID[0] = '\0';
					g_InputPW[0] = '\0';
				}
			}

			// 클릭 위치(디버그용)
			//std::cout << "gl_x: " << gl_x << ", gl_y: " << gl_y << std::endl;
			//std::cout << "Selected Cursor: " << g_LoginCursor << std::endl;
		}
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
	drawText((width / 2) - 30.0f, height - 180, userdata.winPlayerID);

	sprintf(userdataString, "match: %d, Win: %d, Lose: %d",
		userdata.data[0].totalMatch, userdata.data[0].win, userdata.data[0].totalMatch - userdata.data[0].win);
	drawText(100, height - 230, "P1 경기 전적");
	drawText(100, height - 280, userdataString);

	sprintf(userdataString, "match: %d, Win: %d, Lose: %d",
		userdata.data[1].totalMatch, userdata.data[1].win, userdata.data[1].totalMatch - userdata.data[1].win);
	drawText(100, height - 330, "P2 경기 전적");
	drawText(100, height - 380, userdataString);

	sprintf(userdataString, "match: %d, Win: %d, Lose: %d",
		userdata.data[2].totalMatch, userdata.data[2].win, userdata.data[2].totalMatch - userdata.data[2].win);
	drawText(100, height - 430, "P3 경기 전적");
	drawText(100, height - 480, userdataString);


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

// --- 2D 배경 이미지 그리기 함수 ---
void Draw2DBackground() {
	if (g_TexLoginBack == 0) return;

	// 3D 설정 끄기
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glUseProgram(0); // 쉐이더 끄기

	// 2D 모드 전환
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, width, 0, height); // 화면 크기에 맞춤

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	// 텍스처 활성화 및 그리기
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, g_TexLoginBack);
	glColor3f(1, 1, 1); // 원래 색상 유지

	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex2f(0, 0);           // 좌하
	glTexCoord2f(1, 0); glVertex2f(width, 0);       // 우하
	glTexCoord2f(1, 1); glVertex2f(width, height);  // 우상
	glTexCoord2f(0, 1); glVertex2f(0, height);      // 좌상
	glEnd();

	glDisable(GL_TEXTURE_2D);

	// 3D 설정 복구
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_DEPTH_TEST);
}



