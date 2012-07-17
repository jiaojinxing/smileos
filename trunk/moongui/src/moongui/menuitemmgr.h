#ifndef __MENUITEMMGR_H
#define __MENUITEMMGR_H

#include "syslog.h"
#include "menuitem.h"

class CMenuItemMgr{
public:
    CMenuItem **index;
    long top;
    long size;
public:
	int IndexToPos(int index,int win_id);
	int ClearHilight(int win_id);
	int GetHilightItem(int win_id);
	int SetHilight(int index,int win_id);
	int PosToIndex(int y,int win_id);
	int DrawItems(CCanvas *can,int win_id);
	int GetTotalHeight(int win_id);
	int GetMaxWidth(int win_id);
	int AddMenuItem(CMenuItem *item);
	int ItemToIndex(CMenuItem *item);
	int GetItemsCount(int win_id);
	int DelItems(int win_id);
    CMenuItem * RemoveElem(int i);
    CMenuItem * GetElem(long index);
    CMenuItem * GetTop();
    CMenuItem * Pop();
    CMenuItemMgr();
    int Clear();
    int DelElem(long i);
    int DelTop();
    int Destroy();
    int Init(long init_size);
    int InitBasic();  
    int IsEmpty();
    int IsFull();
    int Print();
    int Push(CMenuItem *node);
    long GetLen();
    ~CMenuItemMgr();
};

#endif