#include "wndmine.h"
#include "gui_misc.h"

CWndMine::CWndMine()
{
    this->InitBasic();
}
CWndMine::~CWndMine()
{
    this->Destroy();
}
int CWndMine::InitBasic()
{
	this->mine = NULL;
    CWindow::InitBasic();
    return OK;
}
int CWndMine::Init()
{
    this->InitBasic();
    CWindow::Init();
    
    return OK;
}
int  CWndMine::Destroy()
{
    DEL(this->mine);
    CWindow::Destroy();
    this->InitBasic();
    return OK;
}
int CWndMine::OnCreate(int wparam, int lparam)
{
	NEW(this->mb_set,CMenuBar);
	this->mb_set->Init();
	this->mb_set->SetRect(5,20,36,16);

	NEW(this->ckb_flag,CCheckBox);
	this->ckb_flag->Init();
	this->ckb_flag->SetText("标记");
	this->ckb_flag->SetRect(55,20,51,16);

	this->mwnd_set = CMenuWnd::CreateMenuWnd(this->id);

	NEW(this->mi_newgame,CMenuItem);
	this->mi_newgame->Init();
	this->mi_newgame->SetName("新游戏");

	NEW(this->mi_junior,CMenuItem);
	this->mi_junior->Init();
	this->mi_junior->SetName("初级");

	NEW(this->mi_middle,CMenuItem);
	this->mi_middle->Init();
	this->mi_middle->SetName("中级");

	NEW(this->mi_senior,CMenuItem);
	this->mi_senior->Init();
	this->mi_senior->SetName("高级");

	NEW(this->mi_exit,CMenuItem);
	this->mi_exit->Init();
	this->mi_exit->SetName("退出");

	this->mb_set->AddMenuBarItem("设置",this->mwnd_set->id);
	this->AddControl(this->mb_set);
	this->AddControl(this->ckb_flag);
	
	this->mwnd_set->AddMenuItem(mi_newgame);
	this->mwnd_set->AddSeparator();
	this->mwnd_set->AddMenuItem(mi_junior);
	this->mwnd_set->AddMenuItem(mi_middle);
	this->mwnd_set->AddMenuItem(mi_senior);
	this->mwnd_set->AddSeparator();
	this->mwnd_set->AddMenuItem(mi_exit);
	
	NEW(this->mine,CMine);
	this->NewGame(1);

    return OK;
}
int CWndMine::OnCommand(int wparam, int lparam)
{
	if(lparam == this->mi_exit->id)
		this->Close();
	if(lparam == this->mi_newgame->id)
	{
		this->NewGame(this->mine->level);
	}
	if(lparam == this->mi_junior->id)
		this->NewGame(1);
	if(lparam == this->mi_middle->id)
		this->NewGame(2);
	if(lparam == this->mi_senior->id)
		this->NewGame(3);

    return OK;
}

int CWndMine::OnPaint(int wparam, int lparam)
{
	CDc *pdc = this->BeginPaint(lparam);

	this->mine->DrawAll(pdc);

	this->EndPaint();

	return OK;
}

int CWndMine::OnLButtonDown(int wparam, int lparam)
{
	this->mine->OnLButtonDown(LOWORD(lparam),HIWORD(lparam),this->ckb_flag->Checked());
	return OK;
}

int CWndMine::NewGame(int level)
{
	if(level == 1)
		afx.wndmgr->SetWindowSize(this->id,118,150);

	if(level == 2)
		afx.wndmgr->SetWindowSize(this->id,192,172);

	if(level == 3)
		afx.wndmgr->SetWindowSize(this->id,251,172);

	this->mine->Destroy();
	this->mine->Init(level);
	this->mine->width = 12;
	this->mine->draw_off_x = 5;
	this->mine->draw_off_y = 37;
	this->mine->win_id = this->id;
	this->InvalidateRect(NULL,1);
	
	return OK;
}
