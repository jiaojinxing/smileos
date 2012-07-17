#ifndef __WNDREADER_H
#define __WNDREADER_H

#include "moongui.h"

class CWndReader:public CWindow{
public:
	CMenuBar *mb_main;
	CEditBox *eb_txt;
	CMenuWnd *mwnd_file;
	CMenuItem *mi_open;
	CMenuItem *mi_set_auto;
	CMenuItem *mi_exit;
	CMenuWnd *mwnd_help;
	CMenuItem *mi_help;
	CMenuItem *mi_about;

	CMem *mem_msg;
	CMem *mem_txt;
	CFile *file_txt;
	int win_id_filemgr;
	CTimer *timer_auto;
	int win_id_msg_box;
	int in_loop;
public:
	int StopAutoScroll();
	int StartAutoScroll();
	static int call_back_load_file(int *p,int line);
	int CloseFile();
    CWndReader();
    virtual ~CWndReader();
    int Init();
    int Destroy();
    int InitBasic();
    int OnClose(int wparam, int lparam);
    int OnCreate(int wparam, int lparam);
    int OnCommand(int wparam, int lparam);
    int OnUnknown(u32 message,int wparam, int lparam);
    int OnKeyDown(int wparam, int lparam);
	int OnTimer(int wparam, int lparam);
};

#endif
