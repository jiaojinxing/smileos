#ifndef __COMBOBOX_H
#define __COMBOBOX_H

#include "control.h"	
#include "lineeditor.h"
#include "button.h"
#include "wnddropdown.h"

class CComboBox:public CControl{
public:
	CButton *btn_down;
	CLineEditor *leb_txt;
	CWndDropDown *wnd_drop_down;
public:
	int ClearList();
	int GetText(CFileBase *file);
	int SetText(char *str);
	int AddStr(char *str);
	int OnAdded();
	int HandleMessage(int message,int wparam,int lparam);
	int Draw(CDc *pdc);
    CComboBox();
    virtual ~CComboBox();
    int Init();
    int Destroy();
    int Copy(CComboBox *p);
    int Comp(CComboBox *p);
    int Print();
    int InitBasic();
};

#endif

