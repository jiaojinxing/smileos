#ifndef __MESSAGEBOX_H
#define __MESSAGEBOX_H

#include "window.h"
#include "button.h"
#include "static.h"
#include "font.h"

#define WM_MSGBOX_SETTEXT	WM_USER+1
#define WM_MSGBOX_QUIT		WM_USER+2
#define WM_MSGBOX_OK		WM_USER+3
#define WM_MSGBOX_CANCEL	WM_USER+4
#define WM_MSGBOX_YES		WM_USER+5
#define WM_MSGBOX_NO		WM_USER+6

class CWndMsgBox:public CWindow{
public:
	CButton *btn1,*btn2,*btn3;
	CStatic *st_txt;
	int type;
	char *i_txt;
	CFont *i_font;
public:
	static int SetText(int win_id,char *i_txt);
	static int CloseMsgBox(int win_id);
	int AdjustControls();
	int Quit();
	int OnClose(int wparam,int lparam);
	static int Create(int pid,char *title,char *str,int type);
	int GetWndRect(CRect *rc);
    CWndMsgBox();
    virtual ~CWndMsgBox();
    int Init();
    int Destroy();
    int InitBasic();
	int OnCreate(int wparam,int lparam);
	int OnCommand(int wparam,int lparam);
	int OnUnknown(u32 message,int wparam,int lparam);
};

#endif
