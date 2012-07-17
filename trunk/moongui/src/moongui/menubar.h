#ifndef __MENUBAR_H
#define __MENUBAR_H

#include "control.h"
#include "canvas.h"
#include "mem.h"
#include "intstk.h"

class CMenuBarItem{
public:
	int id;
	CMem *mem_str;
	CRect *rect;
	int menu_wnd_id;
public:
	int SetName(char *name);
    CMenuBarItem();
    virtual ~CMenuBarItem();
    int Init();
    int Destroy();
    int Copy(CMenuBarItem *p);
    int Comp(CMenuBarItem *p);
    int Print();
    int InitBasic();
};

class CMenuBar:public CControl{
public:
	CCanvas *can;
	CIntStk *stk_items;
	int cur_sel;
public:
	int AddMenuBarItem(char *name,int menu_wnd_id);
	CMenuBarItem * GetSel();
	int SetHilight(int i);
	int PtToIndex(int x,int y);
	int DrawText();
	CMenuBarItem * GetItem(int i);
	int InitAllRect();
	int AddMenuBarItem(CMenuBarItem *item);
	int HandleMessage(int message,int wparam,int lparam);
	int Draw(CDc *pdc);
    CMenuBar();
    virtual ~CMenuBar();
    int Init();
    int Destroy();
    int Copy(CMenuBar *p);
    int Comp(CMenuBar *p);
    int Print();
    int InitBasic();
};

#endif

