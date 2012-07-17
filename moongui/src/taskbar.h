#ifndef __TASKBAR_H
#define __TASKBAR_H

#include "moongui.h"

class CTaskBar:public CWindow{
public:
	CButton *btn_start;
	CMenuWnd *menu_start,*menu_game,*menu_program,*menu_window;
	CMenuItem *mi_about,*mi_program,*mi_game,*mi_window;
	CMenuItem *mi_mine,*mi_calcu,*mi_reader,*mi_bus;
#if _NDS_
	*mi_doc_reader;
#endif
	CMenuItem *mi_dict;
	CMenuItem *mi_close_all,*mi_repos,*mi_clear_console;
	
	CRect *rect_softkb;
	int win_id_softkb;
	int win_id_bus;
	int win_id_reader;
	int win_id_about;
	int win_id_wine;
#if _NDS_
	int win_id_doc_reader;
#endif
	int win_id_dict;
public:
    CTaskBar();
    virtual ~CTaskBar();
    int Init();
    int Destroy();
    int InitBasic();
	int OnLButtonDown(int wparam,int lparam);
	int OnCreate(int wparam,int lparam);
	int OnCommand(int wparam,int lparam);
	int OnPaint(int wparam,int lparam);
	int OnTimer(int wparam,int lparam);
	int OnUnknown(u32 message,int wparam,int lparam);
};
#endif
