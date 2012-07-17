#ifndef __CHECKBOX_H
#define __CHECKBOX_H

#include "control.h"	

class CCheckBox:public CControl{
public:
public:
	int Checked();
	int HandleMessage(int message,int wparam,int lparam);
	int Draw(CDc *pdc);
    CCheckBox();
    virtual ~CCheckBox();
    int Init();
    int Destroy();
    int Copy(CCheckBox *p);
    int Comp(CCheckBox *p);
    int Print();
    int InitBasic();
};

#endif

