#ifndef __EDITBOX_H
#define __EDITBOX_H

#include "control.h"	
#include "canvas.h"
#include "scrollbar.h"
#include "filebase.h"
#include "mem.h"
#include "intstk.h"

class CEditBox:public CControl{
public:
	CScrollBar *sb_vert,*sb_horz;
	CFileBase *i_file_txt;
	CCanvas *can;
	CIntStk *stk_lines;
	int eb_style;
	int sel_start,sel_end,ins_pos;
	CRect *rect_txt; //rect on canvas
	int start_line;
	int scroll_width;
	int total_lines;
	int sel_mode,drag_start_char,drag_end_char;
	int *call_back_param;
	int (*call_back_load_file)(int *p,int line);
	int loading;
public:
	int GetLineFromCharPos(int char_pos);
	int ViewYToLine(int view_y);
	int GetYPos(int line);
	int GetLine(int line,CFileBase *file);
	int GetLineOffset(int line_index);
	int ClearSel();
	int RepSel(char *str);
	int RefreshScrollBar();
	int GetSelSize();
	int RepSel(CFileBase *file);
	int HasSelection();
	int ScrollY(int cy);
	int CharVisible(int char_pos);
	int ToPos(int char_pos,int *x,int *y);
	int SetCaret(int char_pos);
	int DrawLineText(int y_pos,int start_char_pos,CMem *str);
	int AdjustCharPos(int char_pos);
	int ToCharPos(int x,int y);
	int SetSel(int start,int end);
	int GetMaxCharsPerLine();
	int ScrollViewY(int cy);
	int DrawText();
	int GetLineHeight();
	int GetVisibleLines();
	int SetTextFile(CFileBase *file);
	int GetTextViewRect(CRect *r);
	int GetViewRect(CRect *r);
	int OnAdded();
	int HasHScroll();
	int HasVScroll();
	int SetRect(int x,int y,int w,int h);
	int ReadLine(CFileBase *file);
	int SetTextRect(int w,int h);
	int HandleMessage(int message,int wparam,int lparam);
	int Draw(CDc *pdc);
    CEditBox();
    virtual ~CEditBox();
    int Init();
    int Destroy();
    int Copy(CEditBox *p);
    int Comp(CEditBox *p);
    int Print();
    int InitBasic();
};

#endif

