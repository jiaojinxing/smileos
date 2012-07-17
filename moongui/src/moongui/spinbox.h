#ifndef __SPINBOX_H
#define __SPINBOX_H

#include "control.h"	

class CSpinBox:public CControl{
public:
	int sp_style;
public:
	int UnCheckAll();
	int GetRect2(CRect *r);
	int GetRect1(CRect *r);
	int HandleMessage(int message,int wparam,int lparam);
	int Draw(CDc *pdc);
    CSpinBox();
    virtual ~CSpinBox();
    int Init();
    int Destroy();
    int Copy(CSpinBox *p);
    int Comp(CSpinBox *p);
    int Print();
    int InitBasic();
};

#endif

