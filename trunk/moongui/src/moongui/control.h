#ifndef __CONTROL_H
#define __CONTROL_H

#include "dc.h"

enum {
CTRL_NAME_UNKNOWN,
CTRL_NAME_STATIC,
CTRL_NAME_BUTTON,
CTRL_NAME_CHECK_BOX,
CTRL_NAME_RADIO_BOX,
CTRL_NAME_GROUP_BOX,
CTRL_NAME_LIST_BOX,
CTRL_NAME_COMBO_BOX,
CTRL_NAME_SCROLL_BAR,
CTRL_NAME_LINE_EDITOR,
CTRL_NAME_SPIN_BOX,
CTRL_NAME_EDIT_BOX,
CTRL_NAME_MENU_BAR,
};

class CControl{
public:
	int id, win_id;
	int ctrl_name;
	CRect *rect;
	u32 style;
	char *i_text;
	pixel_type fore_color;
	pixel_type back_color;
	CFont *i_font;
public:
	int Redraw(int erase_back);
	int HasFocus();
	int SetParent(int win_id);
	int SetRect(CRect *r);
	virtual int OnAdded();
	int SetStyle(u32 style);
	int Focused();
	int PtInCtrl(int x,int y);
	virtual int SetRect(int x,int y,int w,int h);
	virtual int HandleMessage(int message, int wparam, int lparam);
	virtual int Draw(CDc *pdc);
	int Redraw();
	int SetText(char *txt);
    CControl();
    virtual ~CControl();
    int Init();
    int Destroy();
    int Copy(CControl *p);
    int Comp(CControl *p);
    int Print();
    int InitBasic();
};

#endif
