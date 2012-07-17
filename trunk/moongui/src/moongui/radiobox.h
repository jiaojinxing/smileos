#ifndef __RADIOBOX_H
#define __RADIOBOX_H

#include "control.h"	

class CRadioBox:public CControl{
public:
	int group;
public:
	int IsChecked();
	int SetCheck(int flag);
	int HandleMessage(int message,int wparam,int lparam);
	int Draw(CDc *pdc);
    CRadioBox();
    virtual ~CRadioBox();
    int Init();
    int Destroy();
    int Copy(CRadioBox *p);
    int Comp(CRadioBox *p);
    int Print();
    int InitBasic();
};

#endif

