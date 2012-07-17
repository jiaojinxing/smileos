#ifndef __CARET_H
#define __CARET_H

#include "window.h"

class CCaret{
public:
	int win_id;
	int x,y,width,height;
	int is_show,is_blink;
	int time_out,old_time;
	pixel_type *img_buf;
	CRect *global_rect;
public:
	int StopBlink();
	int StartBlink();
	int SetBlinkTime(int time_out);
	int SetPos(int x,int y);
	int Hide();
	int Show();
	int SetSize(int w,int h);
    CCaret();
    virtual ~CCaret();
    int Init();
    int Destroy();
    int InitBasic();
};

#endif
