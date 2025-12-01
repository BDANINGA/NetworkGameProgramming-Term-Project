#define _CRT_SECURE_NO_WARNINGS
#include "function.h"
#include <random>

#define M_PI 3.14159265358979323846

// 함수들
void windowToOpenGL(int window_x, int window_y, int window_width, int window_height, float& gl_x, float& gl_y) {
    // OpenGL 좌표계의 x값 변환: 윈도우 좌표를 -1.0 ~ 1.0 사이로 정규화
    gl_x = (2.0f * window_x) / window_width - 1.0f;

    // OpenGL 좌표계의 y값 변환: 윈도우 좌표는 위아래가 반대이므로 y축 변환
    gl_y = 1.0f - (2.0f * window_y) / window_height;
}
char* filetobuf(const char* file)
{
    FILE* fptr;
    long length;
    char* buf;
    fptr = fopen(file, "rb"); // Open file for reading
    if (!fptr) // Return NULL on failure
        return NULL;
    fseek(fptr, 0, SEEK_END); // Seek to the end of the file
    length = ftell(fptr); // Find out how many bytes into the file we are
    buf = (char*)malloc(length + 1); // Allocate a buffer for the entire length of the file and a null terminator
    fseek(fptr, 0, SEEK_SET); // Go back to the beginning of the file
    fread(buf, length, 1, fptr); // Read the contents of the file in to the buffer
    fclose(fptr); // Close the file
    buf[length] = 0; // Null terminator
    return buf; // Return the buffer 
}
bool checkSegmentCollision(glm::vec3 start, glm::vec3 end, glm::vec3 goalPos, glm::vec3 goalScale) {
    glm::vec3 boxMin = goalPos - goalScale;
    glm::vec3 boxMax = goalPos + goalScale;

    for (int i = 0; i < 3; ++i) {
        float boxMinCoord = boxMin[i];
        float boxMaxCoord = boxMax[i];

        if (start[i] < boxMinCoord && end[i] < boxMinCoord || start[i] > boxMaxCoord && end[i] > boxMaxCoord)
            return false;

        if (start[i] < boxMinCoord || end[i] > boxMaxCoord) {
            float t = (boxMinCoord - start[i]) / (end[i] - start[i]);
            if (t > 0.0f && t < 1.0f) {
                return true;
            }
        }
    }

    return true;
}

// tiemer
void TimerFunction(int value)
{
    glutTimerFunc(25, TimerFunction, 1);
    glutPostRedisplay();
}

// Gameover - GameSessionLoop()함수 안에서 시간을 체크함.
void Gameover(time_t time, SOCKET socket) {
    if (time >= 300) {    // 5분
        UpdateScore();
        send_gameover(socket);
        ReturntoLogin();
    }
}

// UpdateScore - 11.28일 구현
void UpdateScore() {

}

// ReturntoLogin - 개발일정에 미처 적어두지 못했음. Login 관련이기 떄문에 1차토의 후 일정 검토
void ReturntoLogin() {

}

// randomDir() - 11.27일 구현
glm::vec3 randomDir()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<float> dist(0.0f, 1.0f);

    glm::vec3 v(dist(gen), dist(gen), dist(gen));
    return glm::normalize(v);
}