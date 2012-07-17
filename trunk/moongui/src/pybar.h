#ifndef __PYBAR_H
#define __PYBAR_H

#include "control.h"	
#include "mem.h"
#include "canvas.h"

class CPyBar:public CControl{
public:
	CMem *mem_input;
	CMem *mem_hz;
	CCanvas *can;
	int start_line,visible_lines;
	int cur_sel;
	CFont *i_font;
	void *call_back_param;
	int (*call_back_hz_item)(void *param,int index,CMem *hz,CRect *rc,int row,int col);
public:
	int IsHzBufEmpty();
	int GetHzByIndex(int index,CFileBase *file);
	int GetHzIndexByPos(int x,int y);
	int IsInputBufEmpty();
	int GetMaxLine();
	int ScrollUp();
	int ScrollDown();
	int GetHzByNum(int num,CFileBase *file);
	int DrawAllHz();
	static int call_back_draw_all_hz_item(void *param,int index,CMem *hz,CRect *rc,int row,int col);
	static int call_back_get_hz_by_num(void *param,int index,CMem *hz,CRect *rc,int row,int col);
	static int call_back_get_hz_by_index(void *param,int index,CMem *hz,CRect *rc,int row,int col);
	static int call_back_get_hz_by_pos(void *param,int index,CMem *hz,CRect *rc,int row,int col);
	static int call_back_get_max_line(void *param,int index,CMem *hz,CRect *rc,int row,int col);
	int GetAllHzItem();
	int Search();
	int SearchPos();
	int ClearInputBuf();
	int AddChar(char ch);
	int GetViewRect(CRect *rc);
	int HandleMessage(int message,int wparam,int lparam);
	int Draw(CDc *pdc);
    CPyBar();
    virtual ~CPyBar();
    int Init();
    int Destroy();
    int Copy(CPyBar *p);
    int Comp(CPyBar *p);
    int Print();
    int InitBasic();
};

#endif

