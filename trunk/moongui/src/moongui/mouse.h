#ifndef __MOUSE_H
#define __MOUSE_H

#include "gal.h"
#include "rect.h"

#define MOUSE_BUTTON_NONE	0
#define MOUSE_BUTTON_LEFT	1
#define MOUSE_BUTTON_RIGHT	2

class CMouse{
public:
	int x,y,w,h;
    int old_x;
    int old_y;
	int img_x,img_y;
    int min_x,min_y;
    int max_x,max_y;
    int button;
    int old_button;
    pixel_type *back_buf;
    int hover;
    int hold;
	char *i_img;
	int is_hide,force_hide;
	int old_time,time_out,hold_start; //use for hold message
public:
	int PosChangeMuch();
	int SetForceHide(int flag);
	int Refresh();
	int GetRect(CRect *pr);
	int ButtonChanged();
	int PosChanged();
	int ReadMouse();
	int Hide();
	int Show();
	int SaveImg();
	int Draw();
	int SetPos(int x,int y);
	int SetImg(char *buf, int width,int height);
    CMouse();
    virtual ~CMouse();
    int Init();
    int Destroy();
    int Copy(CMouse *p);
    int Comp(CMouse *p);
    int Print();
    int InitBasic();
};

#endif
