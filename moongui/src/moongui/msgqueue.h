#ifndef __MSGQUEUE_H
#define __MSGQUEUE_H

#include "window.h"

class CMsg{
public:
	int wnd_id;
	u32 message;
	int wparam;
	int lparam;
public:
    CMsg();
    virtual ~CMsg();
    int Init();
    int Destroy();
    int Copy(CMsg *p);
    int Comp(CMsg *p);
    int Print();
    int InitBasic();
};

class CMsgQueue{
public:
	CMsg *data;
    int head, tail;
    int size;
public:
	int PostMessage(int wnd_index,int message,int wparam,int lparam);
	int GetLen();
	int DeQueue(CMsg *node);
	int EnQueue(CMsg *node);
	int Clear();
	int IsFull();
	int IsEmpty();
    CMsgQueue();
    virtual ~CMsgQueue();
    int Init(int size);
    int Destroy();
    int Copy(CMsgQueue *p);
    int Comp(CMsgQueue *p);
    int Print();
    int InitBasic();
};

#endif
