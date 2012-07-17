#ifndef __CANVAS_H
#define __CANVAS_H

#include "dc.h"

class CCanvas{
public:
	CDc *i_dc;
	CRect *rect;
	int view_x,view_y;
public:
	int DrawRect(CRect *r);
	int DrawRect(int x,int y,int w,int h);
	int DrawMonoImage(int x,int y,ImageInfo *img,CRect *rclip);
	int Draw3DHLine(int x,int y,int w);
	int SR2LR(CRect *r);
	int SP2LP(int *x,int *y);
	int FillRect(int x,int y,int w,int h);
	int FillRect(CRect *r);
	int SetTextColor(pixel_type tc);
	int SetBrushColor(pixel_type bc);
	int SetPenColor(pixel_type pc);
	int SetRect(CRect *r);
	int LR2SR(CRect *lr);
	int LP2SP(int *x,int *y);
	int TextOut(int x,int y,char *str,CRect *rect);
	int VLine(int x,int y,int h);
	int HLine(int x,int y,int w);
	int SetViewPos(int x,int y);
	int SetRect(int x,int y,int w,int h);
	int SetDC(CDc *pdc);
    CCanvas();
    virtual ~CCanvas();
    int Init();
    int Destroy();
    int InitBasic();
};

#endif
