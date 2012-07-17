#ifndef __WNDFILEMGR_H
#define __WNDFILEMGR_H

#include "moongui.h"

#define WM_FILE_OPEN	WM_USER+1000

class CWndFileMgr:public CWindow{
public:
	CDirMgr *dirmgr;
	CListBox *lb_list;
	CLineEditor *leb_fn;
	CButton *btn_open;
	CButton *btn_calcel;
	CButton *btn_up;
	CButton *btn_property;
public:
	static int Create(int pid,char *init_path,char *caption);
	int RefreshList();
    CWndFileMgr();
    virtual ~CWndFileMgr();
    int Init();
    int Destroy();
    int InitBasic();
    int OnCreate(int wparam, int lparam);
    int OnCommand(int wparam, int lparam);
};


#endif
