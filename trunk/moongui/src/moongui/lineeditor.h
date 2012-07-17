#ifndef __LINEEDITOR_H
#define __LINEEDITOR_H

#include "control.h"	
#include "canvas.h"
#include "mem.h"
#include "menuwnd.h"

class CLineEditor:public CControl{
public:
	CCanvas *can;
	CMem *mem_txt;
	int sel_start,sel_end;
	int ins_pos;
	int sel_mode;
	int start_char_pos,end_char_pos; //use for drag selection
	CMenuWnd *content_menu;
	CMenuItem *i_copy,*i_paste,*i_selall,*i_cut;
public:
	int GetText(CFileBase *file);
	int OnChar(int wparam,int lparam);
	int GetSelSize();
	int RepSel(char *str);
	int OnAdded();
	int InsPosVisible();
	int InSelection(int char_pos);
	int Scroll(int cx);
	int HasSelection();
	int SetCaret(int char_pos);
	int ToPos(int char_pos);
	int ToCharPos(int x);
	int SetInsPos(int p);
	int SetSel(int start,int end);
	int AdjustCharPos(int pos);
	int DrawText();
	int SetText(char *txt);
	int GetTextRect(CRect *r);
	int HandleMessage(int message,int wparam,int lparam);
	int Draw(CDc *pdc);
    CLineEditor();
    virtual ~CLineEditor();
    int Init();
    int Destroy();
    int Copy(CLineEditor *p);
    int Comp(CLineEditor *p);
    int Print();
    int InitBasic();
};

#endif

