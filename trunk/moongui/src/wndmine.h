#ifndef __WNDMINE_H
#define __WNDMINE_H

#include "moongui.h"
#include "mine.h"

class CWndMine:public CWindow{
public:
	CMenuBar *mb_set;
	CCheckBox *ckb_flag;
	CMenuWnd *mwnd_set;
	CMenuItem *mi_junior;
	CMenuItem *mi_middle;
	CMenuItem *mi_senior;
	CMenuItem *mi_exit;
	CMenuItem *mi_newgame;
	CMine *mine;
public:
	int NewGame(int level);
	int OnLButtonDown(int wparam,int lparam);
	int OnPaint(int wparam, int lparam);
    CWndMine();
    virtual ~CWndMine();
    int Init();
    int Destroy();
    int InitBasic();
    int OnCreate(int wparam, int lparam);
    int OnCommand(int wparam, int lparam);
};

#endif
