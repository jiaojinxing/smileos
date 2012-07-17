#ifndef __MENUITEM_H
#define __MENUITEM_H

#include "mem.h"
#include "font.h"
#include "canvas.h"

#define MS_HILIGHT		0x00000001
#define MS_SEPARATOR	0x00000002

class CMenuItem{
public:
	int id,win_id,child_win_id;
	CMem *mem_str;
	u32 style;
	CFont *i_font;
public:
	int SetHiLight(int flag);
	static int GetUniqueID();
	int SetChildMenu(int child_win_id);
	int HasChild();
	int IsSeparator();
	int IsHilighted();
	int Draw(CCanvas *can,int x,int y);
	int GetHeight();
	int GetWidth();
	int SetName(char *str);
    CMenuItem();
    virtual ~CMenuItem();
    int Init();
    int Destroy();
    int Copy(CMenuItem *p);
    int Comp(CMenuItem *p);
    int Print();
    int InitBasic();
};

#endif

