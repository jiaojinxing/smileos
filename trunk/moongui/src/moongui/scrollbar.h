#ifndef __SCROLLBAR_H
#define __SCROLLBAR_H

#include "control.h"	

#define SB_AUTO_SCROLL_DIR_UP		1
#define SB_AUTO_SCROLL_DIR_DOWN		2

#define WM_SB_AUTO_SCROLL  (WM_USER+101)

class CScrollBar:public CControl{
public:
	int min,max,page_size,cur_pos;
	int line_step,page_step;
	int drag_mode,drag_off_x,drag_off_y;
	int sb_style;
	int auto_scroll_start_time;
public:
	int OnLButtonUp(int wparam,int lparam);
	int StopAutoScroll();
	int StartAutoScroll(int dir);
	int SetLineStep(int ls);
	int GetPos();
	int SetPageStep(int ps);
	int SetPageSize(int ps);
	int SetThumbPos(int pixel_pos);
	int Scroll(int cx);
	int GetPage2Rect(CRect *r);
	int GetPage1Rect(CRect *r);
	int SetDir(int dir);
	int GetThumbRect(CRect *r);
	int SetMinMax(int min,int max);
	int SetPos(int pos);
	int DrawArrow2(CDc *pdc);
	int GetArrow2Rect(CRect *r);
	int IsHorz();
	int DrawArrow1(CDc *pdc);
	int GetArrow1Rect(CRect *r);
	int IsVert();
	int HandleMessage(int message,int wparam,int lparam);
	int Draw(CDc *pdc);
    CScrollBar();
    virtual ~CScrollBar();
    int Init();
    int Destroy();
    int Copy(CScrollBar *p);
    int Comp(CScrollBar *p);
    int Print();
    int InitBasic();
};

#endif

