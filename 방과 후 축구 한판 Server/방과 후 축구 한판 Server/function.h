#pragma once
#include "include.h"
#include "network.h"
// ---------------------------------------------------------------------------------------------------------
// 함수들
void windowToOpenGL(int window_x, int window_y, int window_width, int window_height, float& gl_x, float& gl_y);
char* filetobuf(const char* file);
bool checkSegmentCollision(glm::vec3 start, glm::vec3 end, glm::vec3 goalPos, glm::vec3 goalScale);

//timer
void TimerFunction(int value);

// Gameover - GameSessionLoop()함수 안에서 시간을 체크함.
void Gameover(time_t time, SOCKET socket);

// UpdateScore - 11.28일 구현
void UpdateScore();

// ReturntoLogin - 개발일정에 미처 적어두지 못했음. Login 관련이기 떄문에 1차토의 후 일정 검토
void ReturntoLogin();

// randomDir() - 11.27일 구현
glm::vec3 randomDir();