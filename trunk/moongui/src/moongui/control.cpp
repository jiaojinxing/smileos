#include "control.h"
#include "globals.h"
#include "colormap.h"

CControl::CControl()
{
    this->InitBasic();
}
CControl::~CControl()
{
    this->Destroy();
}
int CControl::InitBasic()
{
	this->id = 0;
	this->win_id = 0;
	this->rect = NULL;
	this->style = 0;
	this->i_text = NULL;
	this->fore_color = COLOR_BLACK;
	this->back_color = GetSysColor(COLOR_3DFACE);
	this->ctrl_name = CTRL_NAME_UNKNOWN;
	this->i_font = NULL;

    return OK;
}
int CControl::Init()
{
    this->InitBasic();

	NEW(this->rect,CRect);
	this->id = afx.GetUniqueID();
	this->i_font = afx.hz12;

    return OK;
}
int CControl::Destroy()
{
    DEL(this->rect);
    this->InitBasic();
    return OK;
}
int CControl::Copy(CControl *p)
{
    if(this == p)
        return OK;
    return OK;
}
int CControl::Comp(CControl *p)
{
    return 0;
}
int CControl::Print()
{
    return TRUE;
}

int CControl::SetText(char *txt)
{
	this->i_text = txt;
	return OK;
}

int CControl::Redraw()
{
	return this->Redraw(0);
}

int CControl::Draw(CDc *pdc)
{
	return OK;
}

int CControl::PtInCtrl(int x, int y)
{
	return this->rect->PtInRect(x,y);
}
int CControl::HandleMessage(int message, int wparam, int lparam)
{
	return OK;
}

int CControl::Focused()
{
	return this->style & WS_FOCUS;
}

int CControl::SetRect(int x, int y, int w, int h)
{
	return this->rect->Set(x,y,x+w-1,y+h-1);
}

int CControl::SetStyle(u32 style)
{
	this->style = style;
	return OK;
}

int CControl::OnAdded()
{
	return OK;
}

int CControl::SetRect(CRect *r)
{
	ASSERT(r);
	return this->rect->Copy(r);
}

int CControl::SetParent(int win_id)
{
	this->win_id = win_id;
	return OK;
}

int CControl::HasFocus()
{
	return this->style & WS_FOCUS;
}

int CControl::Redraw(int erase_back)
{
	int index = afx.wndmgr->IdToIndex(this->win_id);
	return afx.wndmgr->InvalidateRect(index,this->rect,erase_back);
}
