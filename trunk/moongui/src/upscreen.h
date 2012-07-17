#ifndef __CUPSCREEN_H
#define __CUPSCREEN_H

#include "moongui.h"

#define PA_RGB(r,g,b) ((1 << 15) + (r) + ((g)<<5) + ((b)<<10))
#define PA_SetBgPalCol(screen, color_number, colorRGB) BG_PALETTE[color_number + ((screen) << 9)] = colorRGB

class CUpScreen{
public:
	static void PA_Default8bitInit(u8 screen, u8 bg_priority);
	static  int ClearScreen_16bit(u16 color);
	static int ClearConsole();
	static u16 * GetFVram_16bit();
	static void PA_Default16bitInit(u8 screen, u8 bg_priority);
	static int SetGraphMode_16bit();
	static int SetGraphMode_8bit();
    CUpScreen();
    virtual ~CUpScreen();
    int Init();
    int Destroy();
    int Print();
    int InitBasic();
};

#endif