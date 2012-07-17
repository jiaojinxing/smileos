#ifndef __CTRLMGR_H
#define __CTRLMGR_H

#include "control.h"

class CCtrlMgr{
public:
    CControl **index;
    long top;
    long size;
public:
	int CtrlToIndex(int ctrl_id);
	int SetFocus(int ctrl_id);
	int AddCtrl(CControl *ctrl);
	int HandleCtrlsMessage(int wind_id,int message,int wparam,int lparam);
	int DrawCtrls(CDc *pdc,int win_id);
	int DelCtrls(int win_id);
    CControl * RemoveElem(int i);
    CControl * GetElem(long index);
    CControl * GetTop();
    CControl * Pop();
    CCtrlMgr();
    int Clear();
    int DelElem(long i);
    int DelTop();
    int Destroy();
    int Init(long init_size);
    int InitBasic();  
    int IsEmpty();
    int IsFull();
    int Print();
    int Push(CControl *node);
    long GetLen();
    ~CCtrlMgr();
};


#endif
