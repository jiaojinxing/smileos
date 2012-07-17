#include "listbox.h"
#include "gui_misc.h"
#include "globals.h"

CListBox::CListBox()
{
    this->InitBasic();
}
CListBox::~CListBox()
{
    this->Destroy();
}
int CListBox::InitBasic()
{
    CControl::InitBasic();
	this->sb_vert = NULL;
	this->line_height = afx.hz12->GetMaxCharHeight() + 1;
	this->stk_str = NULL;
	this->cur_sel = -1;

    return OK;
}
int CListBox::Init()
{
    this->InitBasic();
    CControl::Init();
    
	this->ctrl_name = CTRL_NAME_LIST_BOX;
	this->back_color = COLOR_WHITE;

	NEW(this->stk_str,CMemStk);
	this->stk_str->Init(1024);

	NEW(this->sb_vert,CScrollBar);
	this->sb_vert->Init();

	NEW(this->can,CCanvas);
	this->can->Init();

    return OK;
}
int CListBox::Destroy()
{
	DEL(this->can);
	DEL(this->stk_str);
    DEL(this->sb_vert);

    CControl::Destroy();
    this->InitBasic();
    return OK;
}
int CListBox::Copy(CListBox *p)
{
    if(this == p)
        return OK;
    CControl::Copy(p);
    return OK;
}
int CListBox::Comp(CListBox *p)
{
    return 0;
}
int CListBox::Print()
{
    CControl::Print();
    return TRUE;
}

int CListBox::Draw(CDc *pdc)
{	
	ASSERT(pdc);

	CRect rc;

	this->GetTextViewRect(&rc);
	pdc->SetBrushColor(this->back_color);
	pdc->FillRect(&rc);

	if(this->style & WS_BORDER)
	{
		this->GetViewRect(&rc);
		pdc->SetPenColor(this->fore_color);
		pdc->DrawRect(&rc);
	}
	
	this->can->SetDC(pdc);
	this->DrawText();

	if(this->HasVScroll())
		this->sb_vert->Draw(pdc);

	return OK;
}

int CListBox::HandleMessage(int message, int wparam, int lparam)
{
	CRect rc;
	int x,y;
	CWindow *pwnd;
	
	if(this->HasVScroll())
		this->sb_vert->HandleMessage(message,wparam,lparam);

	switch(message)
	{
		case WM_LBUTTONDOWN: 
			x = (short)LOWORD(lparam);	
			y = (short)HIWORD(lparam);
			this->GetTextViewRect(&rc);

			if(!rc.PtInRect(x,y))
				return ERROR;
			this->SetSel(this->PtToIndex(x,y));
		break;
		
		case WM_LBUTTONUP:
			x = (short)LOWORD(lparam);	
			y = (short)HIWORD(lparam);
			this->GetTextViewRect(&rc);

			if(!rc.PtInRect(x,y))
				return ERROR;
			afx.wndmgr->SendMessage(this->win_id,WM_COMMAND,LBN_LBUTTONUP,this->id);
		break;

		case WM_COMMAND:
			if(lparam == this->sb_vert->id)
			{
				this->ScrollY(this->sb_vert->GetPos() - this->can->view_y);
			}
		break;

	}
	return ERROR;
}

int CListBox::OnAdded()
{
	CRect rc;

	this->sb_vert->win_id = this->win_id;
	this->sb_vert->SetRect(this->rect->right - 10,this->rect->top,10,this->rect->GetHeight());
	this->sb_vert->SetDir(SBS_VERT);

	this->GetTextViewRect(&rc);
	this->can->SetRect(&rc);
	
	return OK;
}

int CListBox::GetViewRect(CRect *rc)
{
	ASSERT(rc);

	rc->Copy(this->rect);

	if(this->HasVScroll())
		rc->right -= this->sb_vert->rect->GetWidth();

	rc->right --;

	return OK;
}

int CListBox::GetTextViewRect(CRect *r)
{
	ASSERT(r);

	this->GetViewRect(r);
	r->left ++;
	r->top ++;
	r->right -- ;
	r->bottom --;

	return OK;
}

int CListBox::HasVScroll()
{
	return (this->style & WS_VSCROLL);
}

int CListBox::HasHScroll()
{
	return (this->style & WS_HSCROLL);
}

int CListBox::AddStr(char *str)
{
	this->stk_str->Push(str);
	this->RefreshScrollBar();
	this->Redraw();

	return OK;
}

int CListBox::GetVisibleLines()
{
	ASSERT(this->line_height > 0);
	
	CRect rc;
	
	this->GetTextViewRect(&rc);
	return rc.GetHeight() / this->line_height;
}

int CListBox::RefreshScrollBar()
{
	if(this->HasVScroll())
	{
		this->sb_vert->SetMinMax(0,this->stk_str->GetLen() * this->line_height);
		this->sb_vert->SetLineStep(this->line_height);
		this->sb_vert->SetPageStep(this->line_height * this->GetVisibleLines());
		this->sb_vert->SetPageSize(this->line_height * this->GetVisibleLines());
		this->sb_vert->SetPos(0);
	}
	return OK;
}

int CListBox::GetItemRect(int index, CRect *rc)
{
	ASSERT(rc);

	rc->Set(0,index * this->line_height,this->rect->GetWidth(), index * this->line_height + this->line_height - 1);

	return OK;
}

int CListBox::DrawText()
{
	int i;

	CRect rc;
	CMem *pmem;

	for(i = 0; i < this->stk_str->GetLen(); i++)
	{
		pmem = this->stk_str->GetElem(i);
		ASSERT(pmem);
		this->GetItemRect(i,&rc);
		this->can->LR2SR(&rc);

		if(rc.IsIntersect(this->can->rect))
		{
			this->GetItemRect(i,&rc);
			if(i == this->cur_sel)
			{
				this->can->SetBrushColor(RGB(16,16,31));
				this->can->FillRect(&rc);
				this->can->SetPenColor(RGB(0,0,31));
				this->can->DrawRect(&rc);
				this->can->SetTextColor(RGB(31,31,31));
				this->can->TextOut(rc.left,rc.top,pmem->p,NULL);
			}
			else
			{
				this->can->SetTextColor(this->fore_color);
				this->can->TextOut(rc.left,rc.top,pmem->p,NULL);
			}
		}
	}

	return OK;
}

int CListBox::ScrollY(int cy)
{
	this->can->view_y += cy;
	this->Redraw();
	return OK;
}

int CListBox::SetSel(int index)
{
	if(this->cur_sel == index)
		return ERROR;
	this->cur_sel = index;
	this->Redraw();

	afx.wndmgr->SendMessage(this->win_id,WM_COMMAND,LBN_SELCHANGE,this->id);

	return OK;
}

int CListBox::PtToIndex(int x, int y)
{
	ASSERT(this->line_height != 0);
	this->can->SP2LP(&x,&y);
	return y / this->line_height;
}

int CListBox::GetSel()
{
	return this->cur_sel;
}

CMem * CListBox::GetSelText()
{
	return this->stk_str->GetElem(this->GetSel());
}

int CListBox::DelAll()
{
	this->stk_str->Clear();
	this->SetSel(-1);
	this->RefreshScrollBar();
	this->can->SetViewPos(0,0);
	return OK;
}
