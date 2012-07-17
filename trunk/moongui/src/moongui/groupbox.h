#ifndef __GROUPBOX_H
#define __GROUPBOX_H

#include "control.h"	

class CGroupBox:public CControl{
public:
public:
	int Draw(CDc *pdc);
    CGroupBox();
    virtual ~CGroupBox();
    int Init();
    int Destroy();
    int Copy(CGroupBox *p);
    int Comp(CGroupBox *p);
    int Print();
    int InitBasic();
};

#endif

