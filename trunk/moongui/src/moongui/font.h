#ifndef __FONT_H
#define __FONT_H

#include "gal.h"

class CFont{
public:
public:
	virtual int DrawText(int x,int y,char *str) = 0;
	virtual int SetClipRect(int left,int top,int right,int bottom) = 0;
	virtual int SetColor(pixel_type fc) = 0;
	virtual int GetCharWidth(char *str) = 0;
	virtual int GetTextSize(char *str,int *cx,int *cy) = 0;
	virtual int GetMinCharWidth() = 0;
	virtual int GetMaxCharHeight() = 0;
    int Init();
    int Destroy();
    int InitBasic();
    CFont();
    virtual ~CFont();
};

#endif
