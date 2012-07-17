#include "MenuWnd.h"
#include "globals.h"
#include "sizemap.h"
#include "colormap.h"
#include "gui_time.h"

CMenuWnd::CMenuWnd()
{
    this->InitBasic();
}
CMenuWnd::~CMenuWnd()
{
    this->Destroy();
}
int CMenuWnd::InitBasic()
{
    CWindow::InitBasic();
	this->can = NULL;
	this->style = 0;
	auto_scroll_start_time = 0;

    return OK;
}
int CMenuWnd::Init()
{
    this->InitBasic();
    CWindow::Init();

   	this->dwStyle = WS_BORDER|WS_NOACTIVE|WS_MENUWND;
	this->dwExStyle = WS_EX_TOPMOST;
	this->class_name = WND_CLASS_MENU;

    return OK;
}
int CMenuWnd::Destroy()
{
    DEL(this->can);

	//delete all menu items attach with this window
	afx.menuitemmgr->DelItems(this->id);

    CWindow::Destroy();
    this->InitBasic();
    return OK;
}
int CMenuWnd::Print()
{
    CWindow::Print();
    return TRUE;
}

int CMenuWnd::OnCreate(int wparam, int lparam)
{
	CRect rc;

	NEW(this->can,CCanvas);
	this->can->Init();

	return OK;	
}

int CMenuWnd::ShowAt(int x, int y)
{
	this->rect->Set(x,y,x + this->GetWndWidth() - 1,y + this->GetWndHeight() - 1);
	this->AdjustRect();
	if(afx.menuitemmgr->GetTotalHeight(this->id) > GAL_GetHeight())
	{
		style |= MWS_UPDOWN_ARROW;
	}

	this->can->view_y = 0;
	afx.menuitemmgr->ClearHilight(this->id);
	afx.wndmgr->BringWindowToTop(this);

	this->Show();

	return OK;
}

int CMenuWnd::AdjustRect()
{
	return this->AutoAdjustRect();
}
int CMenuWnd::GetItemsCount()
{
	return afx.menuitemmgr->GetItemsCount(this->id);
}

int CMenuWnd::AddMenuItem(CMenuItem *item)
{
	ASSERT(item);
	
	item->win_id = this->id;
	item->i_font = this->privCDC->i_font;

	afx.menuitemmgr->AddMenuItem(item);	

	return OK;
}

int CMenuWnd::GetWndWidth()
{
	int w = afx.menuitemmgr->GetMaxWidth(this->id)+3;

	if( w > GAL_GetWidth())
		w = GAL_GetWidth();

	return w;
}

int CMenuWnd::GetWndHeight()
{
	int h = afx.menuitemmgr->GetTotalHeight(this->id)+3;

	if(h > GAL_GetHeight())
		h = GAL_GetHeight();

	return h;
}

int CMenuWnd::OnPaint(int wparam, int lparam)
{
	CRect rc;

	CDc *pdc = this->BeginPaint(lparam);

	this->DrawUpArrow(pdc);
	this->DrawDownArrow(pdc);

	this->can->SetDC(pdc);
	this->GetViewRect(&rc);
	this->can->SetRect(&rc);

	afx.menuitemmgr->DrawItems(can,this->id);

	this->EndPaint();
	return OK;
}

int CMenuWnd::GetViewRect(CRect *rc)
{
	ASSERT(rc);

	rc->Copy(this->rect);
	
	this->SR2LR(rc);

	if(this->HasUpDownArrow())
	{
		rc->top += GetSysSize(SM_MENU_UPDOWN_ARROW_HEIGHT);
		rc->bottom -= GetSysSize(SM_MENU_UPDOWN_ARROW_HEIGHT);
	}
	else
	{
		rc->top += 2;
		rc->bottom -= 1;
	}

	rc->left += 2;
	rc->right -= 2;

	return OK;
}

int CMenuWnd::AddSeparator()
{
	CMenuItem *p;

	NEW(p,CMenuItem);
	p->Init();
	p->style |= MS_SEPARATOR;
	p->SetName("");

	this->AddMenuItem(p);
	return OK;
}

int CMenuWnd::OnLButtonDown(int wparam, int lparam)
{
	int x,y,index;
	CRect rc,rt;

	x = (short)LOWORD(lparam);	
	y = (short)HIWORD(lparam);

	this->GetUpArrowRect(&rc);
	if(rc.PtInRect(x,y))
	{
		this->HideAllChildMenu(this->id);
		this->style |= MWS_UP_ARROW_CHECKED;
		this->InvalidateRect(&rc,0);
		this->StartAutoScroll(ITEM_AUTO_SCROLL_DIR_UP);
		return OK;
	}
	
	this->GetDownArrowRect(&rc);
	if(rc.PtInRect(x,y))
	{
		this->HideAllChildMenu(this->id);
		this->style |= MWS_DOWN_ARROW_CHECKED;
		this->InvalidateRect(&rc,0);
		this->StartAutoScroll(ITEM_AUTO_SCROLL_DIR_DOWN);
		return OK;
	}

	this->can->SP2LP(&x,&y);
	
	index = afx.menuitemmgr->PosToIndex(y,this->id);
	if(afx.menuitemmgr->SetHilight(index,this->id))
	{
		this->InvalidateRect(this->can->rect,1);
	}

	return OK;
}

int CMenuWnd::OnLButtonUp(int wparam, int lparam)
{
	CWindow *pwnd;
	CMenuItem *pm;
	int i,x,y;
	CRect rc,rt;

	x = (short)LOWORD(lparam);	
	y = (short)HIWORD(lparam);

	this->UncheckArrow();
	this->StopAuotScroll();

	this->GetViewRect(&rc);
	if(!rc.PtInRect(x,y))
		return ERROR;
	
	i = afx.menuitemmgr->GetHilightItem(this->id);
	pm  =afx.menuitemmgr->GetElem(i);
	if(pm == NULL)
		return ERROR;

	if(pm->HasChild())
	{
		pwnd = afx.wndmgr->GetWindowByID(pm->child_win_id);
		ASSERT(pwnd);
		ASSERT(pwnd->IsMenuWnd());
		this->HideAllChildMenu(this->id);
		this->GetChildPos(i,&x,&y);
		((CMenuWnd *)pwnd)->ShowAt(x,y);
		return OK;
	}

	pwnd = afx.wndmgr->GetWindowByID(this->pid);
	ASSERT(pwnd);
	
	pwnd->SendMessage(WM_COMMAND,0,pm->id);
	afx.wndmgr->HideAllMenuWindows();

	return OK;
}

int CMenuWnd::HasUpDownArrow()
{
	return this->style & MWS_UPDOWN_ARROW;
}

int CMenuWnd::GetUpArrowRect(CRect *rc)
{
	ASSERT(rc);
	
	if(!this->HasUpDownArrow())
	{
		rc->Empty();
		return OK;
	}

	this->GetViewRect(rc);

	rc->top -= (GetSysSize(SM_MENU_UPDOWN_ARROW_HEIGHT) - 1);
	rc->bottom = rc->top + GetSysSize(SM_MENU_UPDOWN_ARROW_HEIGHT);
	
	return OK;
}

int CMenuWnd::GetDownArrowRect(CRect *rc)
{
	ASSERT(rc);
	
	if(!this->HasUpDownArrow())
	{
		rc->Empty();
		return OK;
	}

	this->GetViewRect(rc);

	rc->bottom += (GetSysSize(SM_MENU_UPDOWN_ARROW_HEIGHT));
	rc->top = rc->bottom - GetSysSize(SM_MENU_UPDOWN_ARROW_HEIGHT);
	
	return OK;
}

int CMenuWnd::DrawUpArrow(CDc *pdc)
{
	ImageInfo *img;
	CRect rc,rt;
	pixel_type c;
	
	if(!this->HasUpDownArrow())
		return ERROR;

	this->GetUpArrowRect(&rc);

	if(this->style & MWS_UP_ARROW_CHECKED)
	{
		c = COLOR_WHITE;
		pdc->SetBrushColor(COLOR_LIGHTMAGENTA);
	}
	else
	{
		c = COLOR_BLACK;
		pdc->SetBrushColor(RGB(20,23,25));
	}
	pdc->FillRect(&rc);
	img = GetStockImage(IMAGE_UP_ARROW);
	rc.CenterRect(img->width,img->height,&rt);
	pdc->DrawMonoImage(rt.left,rt.top,img,c);

	return OK;
}

int CMenuWnd::DrawDownArrow(CDc *pdc)
{
	ImageInfo *img;
	CRect rc,rt;
	pixel_type c;

	if(!this->HasUpDownArrow())
		return ERROR;

	this->GetDownArrowRect(&rc);

	if(this->style & MWS_DOWN_ARROW_CHECKED)
	{
		c = COLOR_WHITE;
		pdc->SetBrushColor(COLOR_LIGHTMAGENTA);
	}
	else
	{
		c = COLOR_BLACK;
		pdc->SetBrushColor(RGB(20,23,25));
	}
	pdc->FillRect(&rc);
	img = GetStockImage(IMAGE_DOWN_ARROW);
	rc.CenterRect(img->width,img->height,&rt);
	pdc->DrawMonoImage(rt.left,rt.top,img,c);

	return OK;
}

int CMenuWnd::ScrollY(int cy)
{
	int old = this->can->view_y;

	this->can->view_y += cy;
	if(this->can->view_y < 0)
		this->can->view_y = 0;

	int t = afx.menuitemmgr->GetTotalHeight(this->id);
	if(this->can->view_y + this->can->rect->GetHeight() > t )
	{
		this->can->view_y = t - this->can->rect->GetHeight();
	}
	
	if(old != this->can->view_y)
		this->InvalidateRect(this->can->rect,1);
	
	return OK;
}

int CMenuWnd::OnUnknown(u32 message,int wparam,int lparam)
{
	int now_time = get_sys_time();

	if(now_time - this->auto_scroll_start_time < 50)
	{
		afx.wndmgr->PostMessage(this->id,WM_ITEM_AUTO_SCROLL,0,0); //make loop
		return OK;
	}

	this->auto_scroll_start_time = now_time;

	if(message == WM_ITEM_AUTO_SCROLL && (this->style & MWS_ITEM_AUTO_SCROLL_ENABLE))
	{
		if(this->style & MWS_ITEM_AUTO_SCROLL_UP)
			this->ScrollY( - ITEM_AUTO_SCROLL_OFFY);
		else if(this->style & MWS_ITEM_AUTO_SCROLL_DOWN)
			this->ScrollY(ITEM_AUTO_SCROLL_OFFY);

		afx.wndmgr->PostMessage(this->id,WM_ITEM_AUTO_SCROLL,0,0); //make loop
	}

	return OK;
}

int CMenuWnd::StartAutoScroll(int dir)
{
	this->style |= MWS_ITEM_AUTO_SCROLL_ENABLE;

	if(dir == ITEM_AUTO_SCROLL_DIR_UP)
	{
		this->style |= MWS_ITEM_AUTO_SCROLL_UP;
		this->style &= (~MWS_ITEM_AUTO_SCROLL_DOWN);
	}
	else if(dir == ITEM_AUTO_SCROLL_DIR_DOWN)
	{
		this->style |= MWS_ITEM_AUTO_SCROLL_DOWN;
		this->style &= (~MWS_ITEM_AUTO_SCROLL_UP);
	}
	this->auto_scroll_start_time = get_sys_time();
	afx.wndmgr->PostMessage(this->id,WM_ITEM_AUTO_SCROLL,0,0); //start loop

	return OK;
}

int CMenuWnd::StopAuotScroll()
{
	this->style &= (~MWS_ITEM_AUTO_SCROLL_ENABLE);
	return OK;
}

int CMenuWnd::OnMouseOut(int wparam, int lparam)
{
	this->UncheckArrow();
	this->StopAuotScroll();
	return OK;
}

int CMenuWnd::UncheckArrow()
{
	CRect rc;

	if(this->style & MWS_UP_ARROW_CHECKED)
	{
		this->GetUpArrowRect(&rc);
		this->style &= (~MWS_UP_ARROW_CHECKED);
		this->InvalidateRect(&rc,1);
	}

	if(this->style & MWS_DOWN_ARROW_CHECKED)
	{
		this->GetDownArrowRect(&rc);
		this->style &= (~MWS_DOWN_ARROW_CHECKED);
		this->InvalidateRect(&rc,1);
	}

	return OK;
}

int CMenuWnd::GetChildPos(int index, int *x, int *y)
{
	ASSERT(x && y);

	*y = afx.menuitemmgr->IndexToPos(index,this->id);
	this->can->LP2SP(x,y);	
	this->LP2SP(x,y);

	*x = this->rect->right - 3;
	
	return OK;
}

int CMenuWnd::HideAllChildMenu(int win_id)
{
	int i;
	CMenuItem *pm;

	for(i = 0; i < afx.menuitemmgr->GetLen(); i++)
	{
		pm = afx.menuitemmgr->GetElem(i);
		ASSERT(pm);
		if(pm->win_id == win_id && pm->HasChild())
		{
			this->HideAllChildMenu(pm->child_win_id); //recusive call
			afx.wndmgr->HideWindowByIndex(afx.wndmgr->IdToIndex(pm->child_win_id));
		}
	}

	return OK;
}

int CMenuWnd::ShowAtMouse()
{
	return this->ShowAt(afx.mouse->x,afx.mouse->y);
}

CMenuWnd * CMenuWnd::CreateMenuWnd(int pid)
{
	CMenuWnd *pwnd;

	NEW(pwnd,CMenuWnd);
	pwnd->Init();
	pwnd->SetParent(pid);
	afx.wndmgr->AddWindow(pwnd);
	
	return pwnd;
}
