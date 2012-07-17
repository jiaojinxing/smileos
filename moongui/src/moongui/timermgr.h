#ifndef __TIMERMGR_H
#define __TIMERMGR_H

#include "timer.h"

class CTimerMgr{
public:
    CTimer **index;
    long top;
    long size;
public:
	int DelTimers(int win_id);
	int AddTimer(CTimer *timer);
	int TimerToIndex(CTimer *timer);
    CTimer * RemoveElem(int i);
    CTimer * GetElem(long index);
    CTimer * GetTop();
    CTimer * Pop();
    CTimerMgr();
    int Clear();
    int DelElem(long i);
    int DelTop();
    int Destroy();
    int Init(long init_size);
    int InitBasic();  
    int IsEmpty();
    int IsFull();
    int Print();
    int Push(CTimer *node);
    long GetLen();
    ~CTimerMgr();
};

#endif
