#ifndef __GLOBALS_H
#define __GLOBALS_H

#include "hz12.h"
#include "window.h"
#include "wndmgr.h"
#include "mouse.h"
#include "msgqueue.h"
#include "draginfo.h"
#include "ctrlmgr.h"
#include "caret.h"
#include "menuitemmgr.h"
#include "timermgr.h"
#include "pinyintable.h"
#include "file.h"
#include "keyboard.h"

class CGlobals{
public:
	CHz12 *hz12;
	CMem *mem_asc,*mem_hz12;
	CMem *mem_img;
	CWndMgr *wndmgr;
	CMouse *mouse;
	CMsgQueue *msgque;
	CDragInfo *drag;
	CCtrlMgr *ctrlmgr;
	CCaret *caret;
	CMenuItemMgr *menuitemmgr;
	CTimerMgr *timemgr;
	CPinYinTable *py_table;
	CFile *file_py;
	CKeyboard *kb;
public:
    CGlobals();
    virtual ~CGlobals();
    int Init();
    int Destroy();
    int InitBasic();
	int GetUniqueID();
};

extern CGlobals afx;

#endif
