#include "window.h"
#include "mem_tool.h"
#include "sizemap.h"
#include "colormap.h"
#include "gui_misc.h"
#include "imagemap.h"
#include "globals.h"

CWindow::CWindow()
{
    this->InitBasic();
}
CWindow::~CWindow()
{
    this->Destroy();
}
int CWindow::InitBasic()
{
	this->rect = NULL;
	this->rClient = NULL;
	this->dwStyle = 0;
	this->dwExStyle = 0;
	this->i_caption = NULL;
	this->privCDC = NULL;
	this->bkcolor = GetSysColor(COLOR_3DFACE);
	this->id = 0;
	this->pid = 0;
	this->is_complete = FALSE;
	this->class_name = WND_CLASS_DEFAULT;

    return OK;
}
int CWindow::Init()
{
    this->InitBasic();
    
	this->id = afx.GetUniqueID();

	NEW(this->rect,CRect);
	NEW(this->rClient,CRect);

	NEW(this->privCDC,CDc);
	this->privCDC->Init();

	this->privCDC->i_font = afx.hz12; //default font

    return OK;
}
int CWindow::Destroy()
{
	afx.ctrlmgr->DelCtrls(this->id);
	afx.wndmgr->DelChildWindowsByIndex(afx.wndmgr->IdToIndex(this->id));
	afx.timemgr->DelTimers(this->id);
	
	if(afx.caret->win_id == this->id)
		afx.caret->StopBlink();

	DEL(this->rect);
    DEL(this->rClient);
	DEL(this->privCDC);

    this->InitBasic();
    return OK;
}
int CWindow::Copy(CWindow *p)
{
	ASSERT(p);

    if(this == p)
        return OK;
	
	this->dwExStyle = p->dwExStyle;
	this->dwStyle = p->dwStyle;
	this->i_caption = p->i_caption;
	this->privCDC->Copy(p->privCDC);
	this->rClient->Copy(p->rClient);
	this->rect->Copy(p->rect);	
	this->bkcolor = p->bkcolor;

    return OK;
}
int CWindow::Comp(CWindow *p)
{
	LOG("Don't use comp in window class\n");
    return 0;
}
int CWindow::Print()
{
    return TRUE;
}

int CWindow::DrawFrame()
{
	int s,s1;
	CRect r;

	s = GetSysSize(SM_BORDER);
	s1 = GetSysSize(SM_CAPTIONY);

	r.Copy(this->rect);
	this->SR2LR(&r);

	this->privCDC->SetBrushColor(this->bkcolor);
	this->privCDC->FillRect(0,0,this->rect->GetWidth(),this->rect->GetHeight());
	if(this->dwStyle & WS_BORDER)
	{
		Draw3dThinUpBorder(this->privCDC,&r);
	}
	return OK;
}

int CWindow::IsActive()
{
	return this->dwStyle & WS_ACTIVE;
}

int CWindow::GetTitleBarRect(CRect *pr)
{
	ASSERT(pr);
	
	int s,s1;

	if( ! (this->dwStyle & WS_CAPTION) )
	{
		pr->Empty();
		return OK;
	}

	s = GetSysSize(SM_BORDER);  
	s1 = GetSysSize(SM_CAPTIONY);

	pr->left = this->rect->left + s;
	pr->top = this->rect->top + s;
	pr->right = this->rect->right - s;
	pr->bottom = this->rect->top+s+s1;

	return OK;
}

int CWindow::SR2LR(CRect *pr)
{
	ASSERT(pr);
	pr->Offset(0-this->rect->left, 0-this->rect->top);
	return OK;
}

int CWindow::LR2SR(CRect *pr)
{
	ASSERT(pr);
	pr->Offset(this->rect->left, this->rect->top);
	return OK;
}

int CWindow::DrawTitleBar()
{
	int c;
	CRect rt;

	if( ! (this->dwStyle & WS_CAPTION) )
		return ERROR;
	
	this->GetTitleBarRect(&rt);
	this->SR2LR(&rt);	
	
	c = this->IsActive() ? COLOR_ACTIVEBORDER : COLOR_INACTIVEBORDER;

	this->privCDC->SetBrushColor(GetSysColor(c));
	this->privCDC->FillRect(rt.left,rt.top,rt.GetWidth(),rt.GetHeight());

	c = this->IsActive() ? COLOR_CAPTIONTEXT : COLOR_INACTIVECAPTIONTEXT;
	
	rt.left += 1;
	rt.right -= (GetSysSize(SM_CLOSE_BOX) + 2);
	this->privCDC->SetTextColor(GetSysColor(c));	
	this->privCDC->DrawText(this->i_caption,&rt,DT_LEFT | DT_VCENTER);

	return OK;
}

int CWindow::DrawCloseBox()
{
	CRect rc,rt;
	ImageInfo *img;

	if( ! (this->dwStyle & WS_CAPTION ))
		return ERROR;

	this->GetCloseBoxRect(&rc);
	img = GetStockImage(IMAGE_CLOSE);

	this->SR2LR(&rc);

	rc.CenterRect(img->width,img->height,&rt);

	if(this->dwStyle & WS_CLOSEBOXCHECKED)
	{
		Draw3dThinDownFrame(this->privCDC, &rc);
		this->privCDC->DrawMonoImage(rt.left + 1,rt.top+ 1,img, GetSysColor(COLOR_BTNTEXT));
	}
	else
	{
		Draw3dThinUpFrame(this->privCDC, &rc);
		this->privCDC->DrawMonoImage(rt.left,rt.top,img, GetSysColor(COLOR_BTNTEXT));
	}

	return OK;
}

int CWindow::GetCloseBoxRect(CRect *pr)
{
	ASSERT(pr);
	int s = GetSysSize(SM_CLOSE_BOX);
	int s1 = GetSysSize(SM_BORDER);

	pr->Set(this->rect->right - s1 - s, this->rect->top + s1, this->rect->right - s1, this->rect->top + s1 + s);
	pr->Offset(-1, 1);

	return OK;
}

int CWindow::Draw()
{
	if( ! this->IsVisible() )
		return ERROR;

	this->DrawFrame();
	this->DrawTitleBar();
	this->DrawCloseBox();

	return OK;
}

int CWindow::IsVisible()
{
	return this->dwStyle & WS_VISIBLE;
}

int CWindow::HasFocus()
{
	return this->dwStyle & WS_FOCUS;
}

int CWindow::SendMessage(u32 message, int wparam, int lparam)
{
	return this->WndProc(message,wparam,lparam);
}

int CWindow::WndProc(u32 message, int wparam, int lparam)
{
	if(this->is_complete)
		afx.ctrlmgr->HandleCtrlsMessage(this->id,message,wparam,lparam);

	if(message == WM_CREATE && (!this->is_complete))
	{
		 this->OnCreate(wparam,lparam);
		 this->is_complete = TRUE;
		 return OK;
	}

	//make sure the construction is completed.
	if( !this->is_complete )
		return ERROR;

	switch(message)
	{		
		case WM_PAINT: 
			return this->OnPaint(wparam,lparam);
		break;
		
		case WM_LBUTTONDOWN: 
			return this->OnLButtonDown(wparam,lparam);
		break;

		case WM_LBUTTONUP: 
			return this->OnLButtonUp(wparam,lparam);
		break;
		
		case WM_KEYDOWN:
			return this->OnKeyDown(wparam,lparam);
		break;
		
		case WM_KEYUP:
			return this->OnKeyUp(wparam,lparam);
		break;

		case WM_MOUSEMOVE: 
			return this->OnMouseMove(wparam,lparam);
		break;

		case WM_MOUSEOUT: 
			return this->OnMouseOut(wparam,lparam);
		break;

		case WM_CLOSE: 
			return this->OnClose(wparam,lparam);
		break;

		case WM_WINDOWPOSCHANGE: 
			return this->OnPosChanged(wparam,lparam);
		break;
	
		case WM_COMMAND:
			return this->OnCommand(wparam,lparam);
		break;
		
		case WM_TIMER:
			return this->OnTimer(wparam,lparam);
		break;

		default:
			return this->OnUnknown(message,wparam,lparam);
	}

	return ERROR;
}

int CWindow::OnPaint(int wparam, int lparam)
{
	this->BeginPaint(lparam);
	this->EndPaint();

	return OK;
}

int CWindow::OnUnknown(u32 message, int wparam, int lparam)
{
	return OK;
}
int CWindow::OnCreate(int wparam, int lparam)
{
	return OK;
}
int CWindow::OnClose(int wparam, int lparam)
{
	return afx.msgque->PostMessage(-1,WM_SYS_DESTROY,0,this->id);
}
int CWindow::OnLButtonDown(int wparam, int lparam)
{
	return OK;
}

int CWindow::OnLButtonUp(int wparam, int lparam)
{
	return OK;
}

int CWindow::OnMouseMove(int wparam, int lparam)
{
	return OK;
}

int CWindow::OnMouseOut(int wparam, int lparam)
{
	return OK;
}
int CWindow::OnTimer(int wparam, int lparam)
{
	return OK;
}

int CWindow::OnCommand(int wparam, int lparam)
{
	return OK;
}


int CWindow::OnKeyDown(int wparam, int lparam)
{
	return OK;
}

int CWindow::OnKeyUp(int wparam, int lparam)
{
	return OK;
}

CDc * CWindow::BeginPaint(int erase_back)
{
	CRect rt;

	afx.mouse->GetRect(&rt);

	//caret hide must before mouse hide,because caret hide include mouse show function
	if(afx.caret->is_show && this->privCDC->clip_rgn->IsIntersect(afx.caret->global_rect))
		afx.caret->Hide();

	if(this->privCDC->clip_rgn->IsIntersect(&rt))
		afx.mouse->Hide();

	if(erase_back)
		this->Draw();

	return this->privCDC;
}

int CWindow::EndPaint()
{
	afx.ctrlmgr->DrawCtrls(this->privCDC,this->id);
	afx.mouse->Show();
	return OK;
}

int CWindow::PtInCloseBox(int x, int y)
{
	CRect rc;
	
	if( ! this->IsVisible())
		return FALSE;
	if(! (this->dwStyle & WS_CAPTION))
		return FALSE;

	this->GetCloseBoxRect(&rc);

	return rc.PtInRect(x,y);
}

int CWindow::PtVisible(int x, int y)
{
	if( ! this->IsVisible())
		return FALSE;

	return this->privCDC->PtVisible(x,y);
}

int CWindow::PtInTitleBar(int x, int y)
{
	CRect rc;
	
	if( ! this->IsVisible())
		return FALSE;

	if(! (this->dwStyle & WS_CAPTION))
		return FALSE;

	this->GetTitleBarRect(&rc);

	return rc.PtInRect(x,y);
}

int CWindow::OnPosChanged(int wparam, int lparam)
{
	return OK;
}

int CWindow::SP2LP(int *x, int *y)
{
	*x = *x - this->rect->left;
	*y = *y - this->rect->top;
	return OK;
}

int CWindow::LP2SP(int *x, int *y)
{
	*x = *x + this->rect->left;
	*y = *y + this->rect->top;
	return OK;
}
int CWindow::Close()
{
	return this->OnClose(0,0);
}

int CWindow::AddControl(CControl *ctrl)
{
	ASSERT(ctrl);
	int ret;

	ctrl->win_id = this->id;
	ctrl->OnAdded();
	ret = afx.ctrlmgr->AddCtrl(ctrl);
	this->InvalidateRect(ctrl->rect,0);

	return ret;
}

int CWindow::InvalidateRect(CRect *rect, int erase_back)
{
	return afx.wndmgr->InvalidateRect(afx.wndmgr->IdToIndex(this->id),rect,erase_back);
}

int CWindow::IsMenuWnd()
{
	return this->dwStyle & WS_MENUWND;
}

int CWindow::Hide()
{
	return afx.wndmgr->HideWindowByIndex(afx.wndmgr->IdToIndex(this->id));
}

int CWindow::Show()
{
	return afx.wndmgr->ShowWindowByIndex(afx.wndmgr->IdToIndex(this->id));
}

int CWindow::SetParent(int pid)
{
	this->pid = pid;
	return OK;
}

int CWindow::AddTimer(CTimer *timer)
{
	ASSERT(timer);
	timer->win_id = this->id;
	return afx.timemgr->AddTimer(timer);
}

int CWindow::SetRect(int x, int y, int w, int h)
{
	return this->rect->Set(x,y,x+w-1,y+h-1);
}

int CWindow::AutoAdjustRect()
{
	int w = this->rect->GetWidth();
	int h = this->rect->GetHeight();

	if(w > GAL_GetWidth())
		w = GAL_GetWidth() - 1;
	if(h > GAL_GetHeight())
		h = GAL_GetHeight() - 1;

	if(this->rect->left < 0)
	{
		this->rect->left = 0;
		this->rect->right = w - 1;
	}

	if(this->rect->top < 0)
	{
		this->rect->top = 0;
		this->rect->bottom = h ;
	}

	if(this->rect->right >= GAL_GetWidth())
	{
		this->rect->right = GAL_GetWidth() - 1;
		this->rect->left = this->rect->right - w + 1;
	}

	if(this->rect->bottom >= GAL_GetHeight())
	{
		this->rect->bottom = GAL_GetHeight() - 1;
		this->rect->top = this->rect->bottom - h  + 1;
	}

	return OK;
}

int CWindow::DelControl(int ctrl_id)
{
	int i;
	CRect rc;
	CControl *pc;

	i = afx.ctrlmgr->CtrlToIndex(ctrl_id);
	pc = afx.ctrlmgr->GetElem(i);
	ASSERT(pc);
	rc.Copy(pc->rect);

	afx.ctrlmgr->DelElem(i);

	this->InvalidateRect(&rc,1);
	
	return OK;
}

int CWindow::SetCaption(char *str)
{
	this->i_caption = str;
	return afx.wndmgr->RedrawTitleBar(afx.wndmgr->IdToIndex(this->id));
}

int CWindow::IsAppWnd()
{
	return this->dwStyle & WS_APPWND;
}
