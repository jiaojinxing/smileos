#ifndef __DRAGINFO_H
#define __DRAGINFO_H

#include "gal.h"
#include "window.h"

class CDragInfo{
public:
	int x,y,width,height;
	int old_x,old_y,off_x,off_y;
	int on_drag;
	int win_id;
public:
	int DrawXORectAtOldPos();
	int DrawXORect();
	int SetPos(int x,int y);
	int EndDrag();
	int StartDrag(CWindow *pwnd,int x,int y);
    CDragInfo();
    virtual ~CDragInfo();
    int Init();
    int Destroy();
    int Print();
    int InitBasic();
};

#endif
