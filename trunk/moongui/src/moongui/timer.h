#ifndef __TIMER_H
#define __TIMER_H

#include "syslog.h"
#include "assert.h"
#include "mem_tool.h"

class CTimer{
public:
	int id,win_id;
	int time_out,old_time;
	int	data;
	int enabled;
public:
	int IsEnable();
	int SetTimeOut(int ms);
	int SetAddData(int data);
	int Stop();
	int Start();
    CTimer();
    virtual ~CTimer();
    int Init();
    int Destroy();
    int Copy(CTimer *p);
    int Comp(CTimer *p);
    int Print();
    int InitBasic();
};

#endif
