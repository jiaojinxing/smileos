#ifndef __HZ12_H
#define __HZ12_H

#include "font.h"
#include "mem.h"

class CHz12:public CFont{
public:
	char *i_buf_asc,*i_buf_hzk12;
	pixel_type fore_color;
public:
	int DrawText(int x,int y,char *str);
	int SetClipRect(int left,int top,int right,int bottom);
	int SetColor(pixel_type fc);
	int GetCharWidth(char *str);
	int GetTextSize(char *str,int *cx,int *cy);
	int GetMinCharWidth();
	int GetMaxCharHeight();
    CHz12();
    virtual ~CHz12();
    int Init();
    int Destroy();
    int Copy(CHz12 *p);
    int Comp(CHz12 *p);
    int Print();
    int InitBasic();
};

#endif
