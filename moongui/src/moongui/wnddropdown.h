#ifndef __WNDDROPDOWN_H
#define __WNDDROPDOWN_H

#include "window.h"
#include "listbox.h"

class CWndDropDown:public CWindow{
public:
	CListBox *listbox;
public:
	int ClearList();
	int ShowAt(int x,int y,int w);
	int AdjustRect();
	int AddStr(char *str);
	static CWndDropDown * CreateDropDownWindow(int pid);
    CWndDropDown();
    virtual ~CWndDropDown();
    int Init();
    int Destroy();
    int InitBasic();
	int OnCreate(int wparam,int lparam);
	int OnCommand(int wparam,int lparam);
};

#endif