#include "messagebox.h"
#include "globals.h"

CWndMsgBox::CWndMsgBox()
{
    this->InitBasic();
}
CWndMsgBox::~CWndMsgBox()
{
    this->Destroy();
}
int CWndMsgBox::InitBasic()
{
    CWindow::InitBasic();
	this->btn1 = NULL;
	this->btn2 = NULL;
	this->btn3 = NULL;
	this->i_txt = NULL;
	this->i_font = afx.hz12;
	this->st_txt = NULL;

    return OK;
}
int CWndMsgBox::Init()
{
    this->InitBasic();
    CWindow::Init();

	this->class_name = WND_CLASS_MSGBOX;
    return OK;
}
int CWndMsgBox::Destroy()
{
    CWindow::Destroy();
    this->InitBasic();
    return OK;
}
int CWndMsgBox::OnCreate(int wparam,int lparam)
{
	CRect rc,rt;

	if(this->type == MB_OK || this->type == MB_CANCEL)
	{
		NEW(this->btn1,CButton);
		this->btn1->Init();
		if(this->type == MB_OK)
			this->btn1->SetText("确定");
		if(this->type == MB_CANCEL)
			this->btn1->SetText("取消");		

		this->AddControl(this->btn1);
	}

	NEW(this->st_txt,CStatic);
	this->st_txt->Init();
	this->st_txt->SetText(this->i_txt);
	
	this->AddControl(this->st_txt);

	this->AdjustControls();

	return OK;
}
int CWndMsgBox::OnCommand(int wparam,int lparam)
{
	if(this->type == MB_OK || this->type == MB_CANCEL)
	{
		if(this->btn1->id == lparam)
			this->OnClose(wparam,lparam);
	}
	return OK;
}

int CWndMsgBox::GetWndRect(CRect *rc)
{
	ASSERT(rc && this->i_txt && this->i_font);
	
	int cx,cy;
	CRect rt;

	this->i_font->GetTextSize(this->i_txt,&cx,&cy);

	if(type == MB_OK || type == MB_CANCEL)
		if(cx < 50) cx = 50;

	if(cy < 30) cy = 30;
	
	rt.Set(0,0,GAL_GetWidth(),GAL_GetHeight());
	rt.CenterRect(cx+10,cy+50,rc);
	
	return OK;
}

int CWndMsgBox::Create(int pid, char *title,char *str, int type)
{
	CWndMsgBox *pmsg;
	CRect rc;

	NEW(pmsg,CWndMsgBox);
	pmsg->Init();

	pmsg->SetParent(pid);
	pmsg->dwStyle = WS_CAPTION|WS_BORDER|WS_VISIBLE|WS_APPWND;
	pmsg->dwExStyle = WS_EX_TOPMOST;
	pmsg->type = type;
	pmsg->i_txt = str;
	pmsg->i_caption = title;

	pmsg->GetWndRect(&rc);
	pmsg->SetRect(rc.left,rc.top,rc.GetWidth(),rc.GetHeight());

	afx.wndmgr->AddWindow(pmsg);
	afx.wndmgr->SetActiveWindow(pmsg);

	return pmsg->id;
}

int CWndMsgBox::OnClose(int wparam, int lparam)
{
	if(this->type == 0)
		return ERROR;

	if(this->type == MB_OK)
		afx.wndmgr->SendMessage(this->pid,WM_MSGBOX_OK,0,this->id);
	if(this->type == MB_CANCEL)
		afx.wndmgr->SendMessage(this->pid,WM_MSGBOX_CANCEL,0,this->id);

	CWindow::OnClose(wparam,lparam);

	return OK;
}

int CWndMsgBox::Quit()
{
	return CWindow::OnClose(0,0);
}
int CWndMsgBox::OnUnknown(u32 message,int wparam,int lparam)
{			
	CRect rc,rt;

	switch(message)
	{
		case WM_MSGBOX_SETTEXT:
			this->i_txt = (char*)lparam;
			ASSERT(this->i_txt);

			this->GetWndRect(&rc);
			rt.Copy(this->rect);

			if(rc.GetWidth() != rt.GetWidth() || rc.GetHeight() != rt.GetHeight())
			{
				rt.right = rt.left + rc.GetWidth() - 1;
				rt.bottom = rt.top + rc.GetHeight() - 1;
				afx.wndmgr->MoveWindow(afx.wndmgr->IdToIndex(this->id),rt.left,rt.top,rt.GetWidth(),rt.GetHeight());
				this->AdjustControls();
				this->InvalidateRect(NULL,1);
			}

			this->st_txt->SetText(this->i_txt);
			this->st_txt->Redraw(1);
		break;

		case WM_MSGBOX_QUIT:
			this->Quit();
		break;
	}
	return OK;
}

int CWndMsgBox::AdjustControls()
{
	CRect rc,rt;

	if(this->type == MB_OK || this->type == MB_CANCEL)
	{
		rc.Set(0,0,this->rect->GetWidth() - 1,this->rect->GetHeight() -1);
		
		rc.CenterRect(35,18,&rt);
		
		rt.bottom = rc.bottom - 4;
		rt.top = rt.bottom - 18;
		this->btn1->SetRect(&rt);
	}

	this->st_txt->SetRect(3,17,this->rect->GetWidth() - 4, this->rect->GetHeight() - 37);

	return OK;
}

int CWndMsgBox::CloseMsgBox(int win_id)
{
	CWindow *p = afx.wndmgr->GetWindowByID(win_id);
	ASSERT(p);

	if(p->class_name != WND_CLASS_MSGBOX)
		return ERROR;

	return ((CWndMsgBox*)p)->Quit();
}

int CWndMsgBox::SetText(int win_id, char *i_txt)
{
	ASSERT(i_txt);
	return afx.wndmgr->SendMessage(win_id,WM_MSGBOX_SETTEXT,0,(int)i_txt);
}
