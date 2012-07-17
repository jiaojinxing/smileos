#ifndef __LISTBOX_H
#define __LISTBOX_H

#include "control.h"	
#include "memstk.h"
#include "scrollbar.h"
#include "canvas.h"

class CListBox:public CControl{
public:
	CScrollBar *sb_vert;
	int line_height;
	CMemStk *stk_str;
	CCanvas *can;
	int cur_sel;
public:
	int DelAll();
	CMem * GetSelText();
	int GetSel();
	int PtToIndex(int x,int y);
	int SetSel(int index);
	int ScrollY(int cy);
	int DrawText();
	int GetItemRect(int index,CRect *rc);
	int RefreshScrollBar();
	int GetVisibleLines();
	int AddStr(char *str);
	int HasVScroll();
	int HasHScroll();
	int GetTextViewRect(CRect *r);
	int GetViewRect(CRect *rc);
	int OnAdded();
	int HandleMessage(int message,int wparam,int lparam);
	int Draw(CDc *pdc);
    CListBox();
    virtual ~CListBox();
    int Init();
    int Destroy();
    int Copy(CListBox *p);
    int Comp(CListBox *p);
    int Print();
    int InitBasic();
};

#endif

