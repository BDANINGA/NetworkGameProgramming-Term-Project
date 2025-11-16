#pragma once
#include "Packet.h"

extern GLuint shaderProgramID; //--- 세이더 프로그램 이름
extern GLuint vertexShader; //--- 버텍스 세이더 객체
extern GLuint fragmentShader; //--- 프래그먼트 세이더 객체
extern GLint width, height;

//사운드
extern FMOD::System* ssystem;
extern FMOD::Sound* s_bgm, * s_goal, * s_touch, * s_shoot;
extern FMOD::Channel* c_bgm;
extern FMOD::Channel* c_goal;
extern FMOD::Channel* c_touch;
extern FMOD::Channel* c_shoot;
extern FMOD_RESULT result;
extern void* extradriverdata;

// 바인드 공간
extern std::vector<GLfloat> vertexArray[3];
extern std::vector<GLfloat> colorArray[3];
extern std::vector<GLfloat> normalArray[3];
extern std::vector<GLfloat> textureArray[3];

// 바인드
extern GLuint vao_player, vbo_player[4], vao_ball, vbo_ball[4], vao_goalpost, vbo_goalpost[4];

// 버텍스 길이
extern int PlayerVertexCount, BallVertexCount, GoalPostVertexCount;

// ---------------------------------------------------------------------------------------------------------
// 함수들
void windowToOpenGL(int window_x, int window_y, int window_width, int window_height, float& gl_x, float& gl_y);
char* filetobuf(const char* file);
void deleteTexture(GLuint textureID);
bool checkSegmentCollision(glm::vec3 start, glm::vec3 end, glm::vec3 goalPos, glm::vec3 goalScale);

//timer
void TimerFunction(int value);

// shader
void make_vertexShaders();
void make_fragmentShaders();
GLuint make_shaderProgram();

// obj 데이터 파싱
struct VertexNormal {
	float x, y, z;     // Vertex coordinates
	float nx, ny, nz;  // Normal coordinates

};
struct Vertex {
	float x, y, z;
};
struct TextureCoord {
	float u, v;
};
struct Normal {
	float nx, ny, nz;
};
struct Face {
	std::vector<std::tuple<int, int, int>> vertices; // (vertex_index, tex_coord_index, normal_index)
};
struct ObjData {
	std::vector<Vertex> vertices;
	std::vector<TextureCoord> texCoords;
	std::vector<Normal> normals;
	std::vector<Face> faces;
};

ObjData parseObj(const std::string& filePath);
void convertToGLArrays(const ObjData& objData, std::vector<GLfloat>& vertexArray, std::vector<GLfloat>& colorArray, std::vector<GLfloat>& normalArray, std::vector<GLfloat>& texCoordArray);
void loadObj();

// bind array
void InitBuffer();

// BMP 파일 로드
GLuint loadBMP(const char* filepath);

// 변환
void viewTransform();
void projectionTransform();
void make_Light();

// 조작
void PlayerInput(int key_value, PacketInputkey& key, SOCKET sock, bool toggle);
void PlayerInput_special(int key_value, PacketInputspecialkey& key, SOCKET sock, bool toggle);

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 도형 만들기(사용안함)
void MakeSphere(GLfloat arr[][3], GLfloat normal[][3], GLfloat cx, GLfloat cy, GLfloat cz, GLfloat radius, int first_index);
void MakeShape(GLfloat Shape[][3], GLfloat normal[][3], GLfloat x1, GLfloat y1, GLfloat z1, GLfloat x2, GLfloat y2, GLfloat z2, int first_index, std::string shape);
void MakeColor(GLfloat arr[][3], int first_index, int index_count, GLfloat color[3]);
