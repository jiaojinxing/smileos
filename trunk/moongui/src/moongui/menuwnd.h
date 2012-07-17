#ifndef __MENUWND_H
#define __MENUWND_H

#include "window.h"
#include "menuitem.h"
#include "canvas.h"


#define ITEM_AUTO_SCROLL_DIR_UP		1
#define ITEM_AUTO_SCROLL_DIR_DOWN	2

#define ITEM_AUTO_SCROLL_OFFY		15

#define WM_ITEM_AUTO_SCROLL  (WM_USER+100)

#define MWS_UPDOWN_ARROW			0x00000001
#define MWS_UP_ARROW_CHECKED		0x00000002
#define MWS_DOWN_ARROW_CHECKED		0x00000004
#define MWS_ITEM_AUTO_SCROLL_ENABLE	0x00000008
#define MWS_ITEM_AUTO_SCROLL_UP		0x00000010
#define MWS_ITEM_AUTO_SCROLL_DOWN	0x00000020

class CMenuWnd:public CWindow{
public:
	CCanvas *can;
	u32 style;
	int auto_scroll_start_time;
public:
	static CMenuWnd * CreateMenuWnd(int pid);
	int ShowAtMouse();
	int HideAllChildMenu(int win_id);
	int GetChildPos(int index,int *x,int *y);
	int UncheckArrow();
	int OnMouseOut(int wparam,int lparam);
	int StopAuotScroll();
	int StartAutoScroll(int dir);
	int OnUnknown(u32 message, int wparam, int lparam);
	int ScrollY(int cy);
	int OnLButtonUp(int wparam,int lparam);
	int DrawDownArrow(CDc *pdc);
	int DrawUpArrow(CDc *pdc);
	int GetDownArrowRect(CRect *rc);
	int GetUpArrowRect(CRect *rc);
	int HasUpDownArrow();
	int OnLButtonDown(int wparam,int lparam);
	int AddSeparator();
	int GetViewRect(CRect *rc);
	int OnPaint(int wparam,int lparam);
	int GetWndHeight();
	int GetWndWidth();
	int AddMenuItem(CMenuItem *item);
	int GetItemsCount();
	int AdjustRect();
	int ShowAt(int x,int y);
	int OnCreate(int wparam,int lparam);
    CMenuWnd();
    virtual ~CMenuWnd();
    int Init();
    int Destroy();
    int Print();
    int InitBasic();
};

#endif
