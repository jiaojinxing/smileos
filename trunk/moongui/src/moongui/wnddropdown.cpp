#include "wnddropdown.h"
#include "globals.h"

CWndDropDown::CWndDropDown()
{
    this->InitBasic();
}
CWndDropDown::~CWndDropDown()
{
    this->Destroy();
}
int CWndDropDown::InitBasic()
{
    CWindow::InitBasic();
	this->listbox = NULL;

    return OK;
}
int CWndDropDown::Init()
{
    this->InitBasic();
    CWindow::Init();

    return OK;
}
int CWndDropDown::Destroy()
{
    CWindow::Destroy();
    this->InitBasic();
    return OK;
}
int CWndDropDown::OnCreate(int wparam,int lparam)
{
	NEW(this->listbox,CListBox);
	this->listbox->Init();
	this->listbox->SetStyle(WS_VSCROLL);

	this->AddControl(this->listbox);

	return OK;
}
int CWndDropDown::OnCommand(int wparam,int lparam)
{
	if(lparam == this->listbox->id)
	{
		if(wparam == LBN_LBUTTONUP)
			this->Hide();
		if(wparam == LBN_SELCHANGE)
		{
			afx.wndmgr->SendMessage(this->pid,WM_COMMAND,LBN_SELCHANGE,this->listbox->id);
		}
	}
	return OK;
}
CWndDropDown * CWndDropDown::CreateDropDownWindow(int pid)
{
	CWndDropDown *p;

	NEW(p,CWndDropDown);
	p->Init();
	p->SetParent(pid);
	p->dwStyle = WS_BORDER|WS_NOACTIVE|WS_MENUWND;
	p->dwExStyle = WS_EX_TOPMOST;

	afx.wndmgr->AddWindow(p);

	return p;
}

int CWndDropDown::AddStr(char *str)
{
	return this->listbox->AddStr(str);
}

int CWndDropDown::AdjustRect()
{
	this->AutoAdjustRect();	
	this->listbox->SetRect(1,1,this->rect->GetWidth()-2,this->rect->GetHeight()-2);

	this->listbox->OnAdded();
	this->listbox->RefreshScrollBar();

	return OK;
}

int CWndDropDown::ShowAt(int x, int y, int w)
{
	int h = this->listbox->stk_str->GetLen() * this->listbox->line_height + 2;
	if(h < 30) h = 30;

	this->SetRect(x,y,w,h);
	this->AdjustRect();
	afx.wndmgr->BringWindowToTop(afx.wndmgr->IdToIndex(this->id));
	this->Show();	

	return OK;
}

int CWndDropDown::ClearList()
{
	return this->listbox->DelAll();
}
