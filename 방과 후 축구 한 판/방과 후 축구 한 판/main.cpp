//--- 메인 함수
#include "방과후 축구한판_Client.h"

#define SERVERPORT 9000

// --- 네트워크 통신용 전역 변수 ---
SOCKET g_ServerSocket{};

// --- recv 용도 ---
bool gameover{};

//--- 필요한 변수 선언
GLint width, height;
GLuint shaderProgramID; //--- 세이더 프로그램 이름
GLuint vertexShader; //--- 버텍스 세이더 객체
GLuint fragmentShader; //--- 프래그먼트 세이더 객체

// 바인드 공간
std::vector<GLfloat> vertexArray[3];
std::vector<GLfloat> colorArray[3];
std::vector<GLfloat> normalArray[3];
std::vector<GLfloat> textureArray[3];

// 바인드
GLuint vao_player, vbo_player[4], vao_ball, vbo_ball[4], vao_goalpost, vbo_goalpost[4];

// 버텍스 길이
int PlayerVertexCount{}, BallVertexCount{}, GoalPostVertexCount{};

//사운드
FMOD::System* ssystem;
FMOD::Sound* s_bgm, * s_goal, * s_touch, * s_shoot;
FMOD::Channel* c_bgm = 0;
FMOD::Channel* c_goal = 0;
FMOD::Channel* c_touch = 0;
FMOD::Channel* c_shoot = 0;
FMOD_RESULT result;
void* extradriverdata = 0;

void main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정
{
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		std::cerr << "WSAStartup failed" << std::endl;

	char* serverip = new char[64];
	std::cout << "ServerIP: ";
	std::cin >> serverip;
	srand(time(NULL));
	width = 1200;
	height = 800;
	//--- 윈도우 생성하기
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(width, height);
	glutCreateWindow("Example1");
	//--- GLEW 초기화하기
	glewExperimental = GL_TRUE;
	glewInit();
	//--- 세이더 읽어와서 세이더 프로그램 만들기
	make_vertexShaders(); //--- 버텍스 세이더 만들기
	make_fragmentShaders(); //--- 프래그먼트 세이더 만들기
	shaderProgramID = make_shaderProgram(); //--- 세이더 프로그램 만들기

	if (!ConnectToServer(g_ServerSocket, serverip, SERVERPORT)) {
		std::cout << "faild to ConnectToServer" << std::endl;
		return;
	}



	if (!CreateThread(NULL, 0, ClientNetworkThread, (LPVOID)g_ServerSocket, 0, NULL)) {
		std::cout << "faild to CreateThread: ClientNetworkThread" << std::endl;
		return;
	}
	
	glutDisplayFunc(UI_Update);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutKeyboardUpFunc(KeyboardUp);
	glutSpecialFunc(SpecialKeys);       // 방향키 입력 콜백 함수
	glutSpecialUpFunc(SpecialKeysUp);
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutMainLoop();
}






