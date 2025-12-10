#pragma once
#include "Ball.h"
#include "Keeper.h"
#include "Camera.h"
#include "Light.h"
#include "network.h"

extern bool gameover;

// 렌더링
GLvoid drawScene();
GLvoid gameoverScene();
GLvoid Reshape(int w, int h);
void UI_Update();
void Draw2DBackground();



// 조작
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid KeyboardUp(unsigned char key, int x, int y);
void Mouse(int button, int state, int x, int y);
void Motion(int x, int y);
void SpecialKeys(int key, int x, int y);
void SpecialKeysUp(int key, int x, int y);

enum GameState {
    STATE_LOGIN,    // 로그인 화면 (ID/PW 입력)
    STATE_GAME,     // 인게임 화면
};
