#ifndef __WINDOW_H
#define __WINDOW_H

#include "rect.h"
#include "dc.h"
#include "control.h"
#include "menuitem.h"
#include "timer.h"

enum
{
	WND_CLASS_DEFAULT,
	WND_CLASS_MSGBOX,
	WND_CLASS_MENU,
};

class CWindow{
public:
	CRect *rect,*rClient;
	u32 dwStyle,dwExStyle;
	CDc *privCDC;
	char *i_caption;
	int bkcolor;
	int id,pid,class_name;
	int is_complete;
public:	
	int IsAppWnd();
	int SetCaption(char *str);
	int DelControl(int ctrl_id);
	int AutoAdjustRect();
	int SetRect(int x,int y,int w,int h);
	int AddTimer(CTimer *timer);
	int LP2SP(int *x,int *y);
	int SetParent(int pid);
	int Show();
	int Hide();
	int IsMenuWnd();
	int InvalidateRect(CRect *rect,int erase_back);
	int AddControl(CControl *ctrl);
	int Close();
	int SP2LP(int *x,int *y);
	int PtInTitleBar(int x,int y);
	int PtVisible(int x,int y);
	int PtInCloseBox(int x,int y);
	int EndPaint();
	CDc * BeginPaint(int erase_back);
	int SendMessage(u32 message,int wparam,int lparam);
	int HasFocus();
	int IsVisible();
	int Draw();
	int GetCloseBoxRect(CRect *pr);
	int DrawCloseBox();
	int DrawTitleBar();
	int LR2SR(CRect *pr);
	int SR2LR(CRect *pr);
	int GetTitleBarRect(CRect *pr);
	int IsActive();
	int DrawFrame();
    CWindow();
    virtual ~CWindow();
    int Init();
    int Destroy();
    int Copy(CWindow *p);
    int Comp(CWindow *p);
    int Print();
    int InitBasic();
	virtual int WndProc(u32 message,int wparam,int lparam);
	virtual int OnUnknown(u32 message,int wparam, int lparam);
	virtual int OnMouseOut(int wparam,int lparam);
	virtual int OnMouseMove(int wparam,int lparam);
	virtual int OnLButtonUp(int wparam,int lparam);
	virtual int OnLButtonDown(int wparam,int lparam);
	virtual int OnCreate(int wparam,int lparam);
	virtual int OnClose(int wparam,int lparam);
	virtual int OnPaint(int wparam, int lparam);	
	virtual	int OnPosChanged(int wparam,int lparam);
	virtual int OnCommand(int wparam,int lparam);
	virtual int OnTimer(int wparam,int lparam);
	virtual int OnKeyDown(int wparam,int lparam);
	virtual int OnKeyUp(int wparam,int lparam);
};

#endif
