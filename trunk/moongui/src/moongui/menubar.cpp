#include "menubar.h"
#include "gui_misc.h"
#include "globals.h"
#include "mem_tool.h"
#include "menuwnd.h"

CMenuBarItem::CMenuBarItem()
{
    this->InitBasic();
}
CMenuBarItem::~CMenuBarItem()
{
    this->Destroy();
}
int CMenuBarItem::InitBasic()
{
	this->id = 0;
	this->mem_str = NULL;
	this->rect = NULL;
	this->menu_wnd_id = 0;

    return OK;
}
int CMenuBarItem::Init()
{
    this->InitBasic();
    this->id = afx.GetUniqueID();

	NEW(this->mem_str,CMem);
	this->mem_str->Init();

	NEW(this->rect,CRect);
	
    return OK;
}
int CMenuBarItem::Destroy()
{
	DEL(this->rect);
    DEL(this->mem_str);
    this->InitBasic();
    return OK;
}
int CMenuBarItem::Copy(CMenuBarItem *p)
{
    if(this == p)
        return OK;
    return OK;
}
int CMenuBarItem::Comp(CMenuBarItem *p)
{
    return 0;
}
int CMenuBarItem::Print()
{
	LOG("name=%s\n",this->mem_str->p);
    return TRUE;
}
int CMenuBarItem::SetName(char *name)
{
	return this->mem_str->SetStr(name);
}
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
CMenuBar::CMenuBar()
{
    this->InitBasic();
}
CMenuBar::~CMenuBar()
{
    this->Destroy();
}
int CMenuBar::InitBasic()
{
    CControl::InitBasic();
	this->can = NULL;
	this->stk_items = NULL;
	this->cur_sel = -1;
    return OK;
}
int CMenuBar::Init()
{
    this->InitBasic();
    CControl::Init();
    
	this->ctrl_name = CTRL_NAME_MENU_BAR;

	NEW(this->can,CCanvas);
	this->can->Init();

	NEW(this->stk_items,CIntStk);
	this->stk_items->Init(32);

    return OK;
}
int CMenuBar::Destroy()
{
    DEL(this->can);	

	INTSTK_DEL(this->stk_items,CMenuBarItem);
	DEL(this->stk_items);

    CControl::Destroy();
    this->InitBasic();
    return OK;
}
int CMenuBar::Copy(CMenuBar *p)
{
    if(this == p)
        return OK;
    CControl::Copy(p);
    return OK;
}
int CMenuBar::Comp(CMenuBar *p)
{
    return 0;
}
int CMenuBar::Print()
{
    CControl::Print();
    return TRUE;
}

int CMenuBar::Draw(CDc *pdc)
{	
	CRect rc;

	ASSERT(pdc);

	this->can->SetRect(this->rect);
	this->can->SetDC(pdc);
	this->DrawText();
	return OK;
}

int CMenuBar::HandleMessage(int message, int wparam, int lparam)
{
	int x,y;

	switch(message)
	{
		case WM_LBUTTONDOWN: 
			x = (short)LOWORD(lparam);	
			y = (short)HIWORD(lparam);

			if(!this->PtInCtrl(x,y))
				return ERROR;

			this->SetHilight(this->PtToIndex(x,y));
			this->Redraw();
		break;

		case WM_LBUTTONUP: 
			if(this->cur_sel >= 0)
			{
				CMenuBarItem *pbi;
				pbi = this->GetSel();
				ASSERT(pbi);
				afx.wndmgr->SendMessage(this->win_id,WM_COMMAND,0,pbi->id);
				if(pbi->menu_wnd_id > 0)
				{
					CRect rc;
					CWindow *pwnd;
					
					pwnd = afx.wndmgr->GetWindowByID(this->win_id);
					ASSERT(pwnd);

					rc.Copy(pbi->rect);
					this->can->LR2SR(&rc);
					pwnd->LR2SR(&rc);
					
					pwnd = afx.wndmgr->GetWindowByID(pbi->menu_wnd_id);
					ASSERT(pwnd);
					ASSERT( pwnd->IsMenuWnd() );					
					((CMenuWnd*)pwnd)->ShowAt(rc.left,rc.bottom);
				}
				this->SetHilight(-1);
				this->Redraw();
			}
		break;

		case WM_MOUSEMOVE:
		break;

		case WM_MOUSEOUT:
			if(this->cur_sel >= 0)
			{
				this->SetHilight(-1);
				this->Redraw();
			}
		break;
	}
	return ERROR;
}

int CMenuBar::AddMenuBarItem(CMenuBarItem *item)
{
	this->stk_items->Push((int)item);
	this->InitAllRect();
	return OK;
}

int CMenuBar::InitAllRect()
{
	ASSERT(this->i_font);

	int pos = 0,i,cx,cy;
	CMenuBarItem *pbi;

	for(i = 0; i < this->stk_items->GetLen(); i++)
	{
		pbi = this->GetItem(i);
		ASSERT(pbi);
		this->i_font->GetTextSize(pbi->mem_str->p,&cx,&cy);
		pbi->rect->Set(pos,0,pos+cx-1,this->rect->GetHeight());
		pos += (cx+this->i_font->GetMinCharWidth());
	}
	return OK;
}

CMenuBarItem * CMenuBar::GetItem(int i)
{
	return (CMenuBarItem*)this->stk_items->GetElem(i);
}

int CMenuBar::DrawText()
{
	int i;
	CMenuBarItem *pbi;
	
	for(i = 0; i < this->stk_items->GetLen(); i++)
	{
		pbi = this->GetItem(i);
		ASSERT(pbi);
		if(this->cur_sel != i)
		{
			this->can->SetTextColor(this->fore_color);
			this->can->SetBrushColor(this->back_color);
		}
		else
		{
			this->can->SetBrushColor(COLOR_BLUE);			
			this->can->SetTextColor(COLOR_WHITE);
		}
		this->can->FillRect(pbi->rect);
		this->can->TextOut(pbi->rect->left,pbi->rect->top,pbi->mem_str->p,NULL);
	}
	return OK;
}

int CMenuBar::PtToIndex(int x, int y)
{
	int i;
	CMenuBarItem *pbi;
	
	this->can->SP2LP(&x,&y);

	for(i = 0; i < this->stk_items->GetLen(); i++)
	{
		pbi = this->GetItem(i);
		ASSERT(pbi);
		if(pbi->rect->PtInRect(x,y))
			return i;
	}
	return -1;
}

int CMenuBar::SetHilight(int i)
{
	this->cur_sel = i;
	return OK;
}

CMenuBarItem * CMenuBar::GetSel()
{
	return this->GetItem(this->cur_sel);
}

int CMenuBar::AddMenuBarItem(char *name,int menu_wnd_id)
{
	CMenuBarItem *pbi;

	NEW(pbi,CMenuBarItem);

	pbi->Init();
	pbi->SetName(name);
	pbi->menu_wnd_id = menu_wnd_id;

	return this->AddMenuBarItem(pbi);
}
