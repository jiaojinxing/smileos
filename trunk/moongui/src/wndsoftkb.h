#ifndef __WNDSOFTKB_H
#define __WNDSOFTKB_H

#include "moongui.h"
#include "pybar.h"

class CWndSoftKb:public CWindow{
public:
	CRadioBox *rb_eng,*rb_py,*rb_sym;
	CButton **btn;
	char *btn_txt;
	int is_capital; //only used by english ime
	CPyBar *py_bar;
	CSpinBox *spin_box;
public:
	int SendChar(char *str);
	char BtnToChar(int btn_index);
	int IdToBtn(int ctrl_id);
	int RefreshBtnText();
	int OnShift();
	int GetImeType();
	int InitButtons();
    CWndSoftKb();
    virtual ~CWndSoftKb();
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
