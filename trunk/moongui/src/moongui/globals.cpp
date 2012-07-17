#include "globals.h"

CGlobals afx;

CGlobals::CGlobals()
{
    this->InitBasic();
}
CGlobals::~CGlobals()
{
    this->Destroy();
}
int CGlobals::InitBasic()
{
	this->hz12 = NULL;
	this->mem_asc = NULL;
	this->mem_hz12 = NULL;
	this->mem_img = NULL;
	this->wndmgr = NULL;
	this->mouse = NULL;
	this->msgque = NULL;
	this->drag = NULL;
	this->ctrlmgr = NULL;
	this->caret = NULL;
	this->menuitemmgr = NULL;
	this->timemgr = NULL;
	this->py_table = NULL;
	this->file_py = NULL;
	this->kb = NULL;

    return OK;
}
int CGlobals::Init()
{
    this->InitBasic();

	NEW(this->kb,CKeyboard);
	this->kb->Init();

	NEW(this->py_table,CPinYinTable);
	this->py_table->Init();

	NEW(this->timemgr,CTimerMgr);
	this->timemgr->Init(64);

	NEW(this->menuitemmgr,CMenuItemMgr);
	this->menuitemmgr->Init(128);

	NEW(this->caret,CCaret);
	this->caret->Init();
	this->caret->SetSize(2,13);
	
	NEW(this->ctrlmgr,CCtrlMgr);
	this->ctrlmgr->Init(128);

    NEW(this->drag,CDragInfo);
	this->drag->Init();

	NEW(this->hz12,CHz12);
	this->hz12->Init();

	NEW(this->mem_asc,CMem);
	this->mem_asc->Init();

	NEW(this->mem_hz12,CMem);
	this->mem_hz12->Init();

	NEW(this->mem_img,CMem);
	this->mem_img->Init();

	NEW(this->wndmgr,CWndMgr);
	this->wndmgr->Init(64);

	NEW(this->mouse,CMouse);
	this->mouse->Init();

	NEW(this->msgque,CMsgQueue);
	this->msgque->Init(32);

	NEW(this->file_py,CFile);
	this->file_py->Init();

	ASSERT( this->mem_img->LoadFile("/sd0/moonGUI/system/stockimgs.bin") );
	ASSERT( this->mem_asc->LoadFile("/sd0/moonGUI/system/asc.bin") );
	ASSERT( this->mem_hz12->LoadFile("/sd0/moonGUI/system/hzk12.bin") );
	ASSERT( this->file_py->OpenFile("/sd0/moonGUI/system/py.bin","rb") );

	this->hz12->i_buf_asc = this->mem_asc->p;
	this->hz12->i_buf_hzk12 = this->mem_hz12->p;
	this->py_table->LoadIndexFile(this->file_py);

    return OK;
}
int CGlobals::Destroy()
{
	DEL(this->kb);
	DEL(this->file_py);
	DEL(this->py_table);
	DEL(this->timemgr);
	DEL(this->menuitemmgr);
	DEL(this->caret);
	DEL(this->ctrlmgr);
	DEL(this->drag);
	DEL(this->msgque);
	DEL(this->mouse);
	DEL(this->wndmgr);
	DEL(this->mem_img);
	DEL(this->mem_asc);
	DEL(this->mem_hz12);
    DEL(this->hz12);

    this->InitBasic();
    
	return OK;
}
int CGlobals::GetUniqueID()
{
	static int i = 1000;
	return i++;
}
