#ifndef __DESKTOP_H
#define __DESKTOP_H

#include "window.h"
#include "static.h"
	
class CDesktop:public CWindow{
public:
	CStatic *st_1;
public:
	int OnCreate(int wparam,int lparam);
	static CWindow * CreateDesktop();
    CDesktop();
    virtual ~CDesktop();
    int Init();
    int Destroy();
    int InitBasic();
};

#endif
