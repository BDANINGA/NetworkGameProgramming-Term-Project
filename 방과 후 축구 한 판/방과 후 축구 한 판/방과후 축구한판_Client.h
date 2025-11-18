#pragma once
#include "Ball.h"
#include "Keeper.h"
#include "Camera.h"
#include "Light.h"
#include "network.h"

extern bool gameover;

// ∑ª¥ı∏µ
GLvoid drawScene();
GLvoid gameoverScene();
GLvoid Reshape(int w, int h);
void UI_Update();

// ¡∂¿€
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid KeyboardUp(unsigned char key, int x, int y);
void Mouse(int button, int state, int x, int y);
void Motion(int x, int y);
void SpecialKeys(int key, int x, int y);
void SpecialKeysUp(int key, int x, int y);
