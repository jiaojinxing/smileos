#ifndef __BUTTON_H
#define __BUTTON_H

#include "control.h"	

class CButton:public CControl{
public:
	int stock_img;
public:
	int SetStockImage(int stock_img);
	int HandleMessage(int message,int wparam,int lparam);
	int Draw(CDc *pdc);
    CButton();
    virtual ~CButton();
    int Init();
    int Destroy();
    int Copy(CButton *p);
    int Comp(CButton *p);
    int Print();
    int InitBasic();
};

#endif

