#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#include "common.h"

#define KEY_A		0
#define KEY_B		1
#define KEY_SELECT	2
#define KEY_START	3
#define KEY_RIGHT	4
#define KEY_LEFT	5
#define KEY_UP		6
#define KEY_DOWN	7
#define KEY_R		8
#define KEY_L		9
#define KEY_X		10
#define KEY_Y		11

#define KEY_MIN		0
#define KEY_MAX		12

class CKeyboard{
public:
	int all_key,all_key_old;
	int key_timer;
public:
	int IsOldKeyPress(int key);
	int KeyChanged();
	int IsKeyPress(int key);
	int ReadKey();
    CKeyboard();
    virtual ~CKeyboard();
    int Init();
    int Destroy();
    int InitBasic();
};

#endif
