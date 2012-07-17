#ifndef __STATIC_H
#define __STATIC_H

#include "control.h"	

class CStatic:public CControl{
public:
	int st_style;
public:
	int Draw(CDc *pdc);
    CStatic();
    virtual ~CStatic();
    int Init();
    int Destroy();
    int Copy(CStatic *p);
    int Comp(CStatic *p);
    int Print();
    int InitBasic();
};

#endif

