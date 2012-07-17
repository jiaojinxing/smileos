#include "scrollbar.h"
#include "gui_misc.h"
#include "globals.h"
#include "gui_time.h"

CScrollBar::CScrollBar()
{
    this->InitBasic();
}
CScrollBar::~CScrollBar()
{
    this->Destroy();
}
int CScrollBar::InitBasic()
{
    CControl::InitBasic();

	this->min = 0;
	this->max = 0;
	this->page_size = 0;
	this->line_step = 0;
	this->page_step = 0;
	this->cur_pos = 0;
	this->drag_mode = FALSE;
	this->drag_off_x = 0;
	this->drag_off_y = 0;
	this->sb_style = 0;
	this->auto_scroll_start_time = 0;

    return OK;
}
int CScrollBar::Init()
{
    this->InitBasic();
    CControl::Init();
    
	this->ctrl_name = CTRL_NAME_SCROLL_BAR;
	this->SetDir(SBS_HORZ);
	this->back_color = GetSysColor(COLOR_3DSHADOW);

	this->SetMinMax(0,99);
	this->line_step = 1;
	this->page_step  =10;
	this->page_size = 10;

    return OK;
}
int CScrollBar::Destroy()
{
    //add your code
    CControl::Destroy();
    this->InitBasic();
    return OK;
}
int CScrollBar::Copy(CScrollBar *p)
{
    if(this == p)
        return OK;
    CControl::Copy(p);
    return OK;
}
int CScrollBar::Comp(CScrollBar *p)
{
    return 0;
}
int CScrollBar::Print()
{
    CControl::Print();
    return TRUE;
}

int CScrollBar::Draw(CDc *pdc)
{	
	CRect rc;

	ASSERT(pdc);

	pdc->SetBrushColor(this->back_color);
	pdc->FillRect(this->rect);

	this->DrawArrow1(pdc);
	this->DrawArrow2(pdc);

	this->GetThumbRect(&rc);
	Draw3dThinUpFrame(pdc,&rc);

	return OK;
}

int CScrollBar::HandleMessage(int message, int wparam, int lparam)
{
	int x,y;
	CRect rc;
	CWindow *pwnd;

	switch(message)
	{
		case WM_LBUTTONDOWN: 
			x = LOWORD(lparam);	
			y = HIWORD(lparam);
			
			this->GetArrow1Rect(&rc);
			if(rc.PtInRect(x,y))
			{
				afx.wndmgr->SetCaptureByIndex(afx.wndmgr->IdToIndex(this->win_id));
				this->sb_style |= SBS_ARROW1_CHECKED;
				this->StartAutoScroll(SB_AUTO_SCROLL_DIR_UP);
				break;
			}
			
			this->GetArrow2Rect(&rc);
			if(rc.PtInRect(x,y))
			{
				afx.wndmgr->SetCaptureByIndex(afx.wndmgr->IdToIndex(this->win_id));
				this->sb_style |= SBS_ARROW2_CHECKED;
				this->StartAutoScroll(SB_AUTO_SCROLL_DIR_DOWN);
				break;
			}

			this->GetPage1Rect(&rc);
			if(rc.PtInRect(x,y))
			{
				this->Scroll(-this->page_step);
				break;
			}
			
			this->GetPage2Rect(&rc);
			if(rc.PtInRect(x,y))
			{
				this->Scroll(this->page_step);
				break;
			}

			this->GetThumbRect(&rc);
			if(rc.PtInRect(x,y))
			{
				afx.wndmgr->SetCaptureByIndex(afx.wndmgr->IdToIndex(this->win_id));
				this->drag_off_x = x - rc.left;
				this->drag_off_y = y - rc.top;
				this->drag_mode = TRUE;
				break;
			}
		break;

		case WM_LBUTTONUP: 
			this->OnLButtonUp(wparam,lparam);	
		break;

		case WM_MOUSEMOVE:
			x = (short)LOWORD(lparam);	
			y = (short)HIWORD(lparam);

			if(this->drag_mode)
			{
				if(this->IsHorz())
					this->SetThumbPos(x - this->drag_off_x);
				else
					this->SetThumbPos(y - this->drag_off_y);
			}
		break;

		case WM_SB_AUTO_SCROLL:
			if( lparam != this->id) 
				break;
			if( !(this->style & SBS_AUTO_SCROLL_ENABLE) )
				break;
			
			int now_time = get_sys_time();

			if(now_time - this->auto_scroll_start_time > 100)
			{
				this->auto_scroll_start_time = now_time;
				if(this->style & SBS_AUTO_SCROLL_DOWN)
					this->Scroll(this->line_step);
				else if(this->style & SBS_AUTO_SCROLL_UP)
					this->Scroll(-this->line_step);
			}
			afx.wndmgr->PostMessage(this->win_id,WM_SB_AUTO_SCROLL,0,this->id); // make loop
		break;
	}
	return ERROR;
}

int CScrollBar::IsVert()
{
	return !(this->sb_style & SBS_HORZ);
}

int CScrollBar::GetArrow1Rect(CRect *r)
{
	ASSERT(r);

	r->Copy(this->rect);

	if( this->IsHorz() )
		r->right = r->left + r->GetHeight();
	else
		r->bottom = r->top + r->GetWidth();

	return OK;
}

int CScrollBar::DrawArrow1(CDc *pdc)
{
	CRect rc,rt;
	ImageInfo *img;

	if(this->IsHorz())
		img = GetStockImage(IMAGE_LEFT_SMALL_ARROW);
	else
		img = GetStockImage(IMAGE_UP_SMALL_ARROW);

	this->GetArrow1Rect(&rc);
	rc.CenterRect(img->width,img->height,&rt);

	if(this->sb_style & SBS_ARROW1_CHECKED)
	{
		pdc->SetBrushColor(GetSysColor(COLOR_3DFACE));
		pdc->FillRect(&rc);
		pdc->SetPenColor(this->back_color);
		pdc->DrawRect(&rc);
		pdc->DrawMonoImage(rt.left+1,rt.top+1,img,this->fore_color);
	}
	else
	{	
		Draw3dThinUpFrame(pdc,&rc);		
		pdc->DrawMonoImage(rt.left,rt.top,img,this->fore_color);
	}

	return OK;
}

int CScrollBar::IsHorz()
{
	return (!this->IsVert());
}

int CScrollBar::GetArrow2Rect(CRect *r)
{
	ASSERT(r);

	r->Copy(this->rect);

	if( this->IsHorz() )
		r->left = r->right - r->GetHeight();
	else
		r->top = r->bottom - r->GetWidth();

	return OK;
}
int CScrollBar::DrawArrow2(CDc *pdc)
{
	CRect rc,rt;
	ImageInfo *img;

	if(this->IsHorz())
		img = GetStockImage(IMAGE_RIGHT_SMALL_ARROW);
	else
		img = GetStockImage(IMAGE_DOWN_SMALL_ARROW);

	this->GetArrow2Rect(&rc);
	rc.CenterRect(img->width,img->height,&rt);

	if(this->sb_style & SBS_ARROW2_CHECKED)
	{
		pdc->SetBrushColor(GetSysColor(COLOR_3DFACE));
		pdc->FillRect(&rc);
		pdc->SetPenColor(this->back_color);
		pdc->DrawRect(&rc);
		pdc->DrawMonoImage(rt.left+1,rt.top+1,img,this->fore_color);
	}
	else
	{
		Draw3dThinUpFrame(pdc,&rc);
		pdc->DrawMonoImage(rt.left,rt.top,img,this->fore_color);
	}

	return OK;
}

int CScrollBar::SetPos(int pos)
{
	if(pos > this->max - this->page_size)
		pos = this->max - this->page_size;

	if(pos < this->min)
		pos = this->min;

	if(this->cur_pos == pos)
		return ERROR;

	this->cur_pos = pos;

	return OK;
}

int CScrollBar::SetMinMax(int min, int max)
{
	this->max = max;
	this->min = min;

	return OK;
}

int CScrollBar::GetThumbRect(CRect *r)
{
	CRect rc,rt;
	int total_len,thumb_len,draw_pos;
	
	ASSERT(r);
	
	this->GetArrow1Rect(&rc);
	this->GetArrow2Rect(&rt);
	
	if(this->IsHorz())
		total_len = this->rect->GetWidth() - rc.GetWidth() - rt.GetWidth();
	else
		total_len = this->rect->GetHeight() - rc.GetHeight() - rt.GetHeight();
	
	if(this->max <= this->min + 1)
	{
		r->Empty();
		return OK;
	}
	
	thumb_len = (total_len * this->page_size) / (this->max - this->min + 1);	
	if(thumb_len < 8) thumb_len = 8;
	
	draw_pos = (total_len * this->cur_pos) / (this->max - this->min + 1);
	r->Copy(this->rect);
	
	if(this->IsHorz())
	{ 
		r->left += (draw_pos + rc.GetWidth());
		r->right = r->left + thumb_len;
		if(r->left <= rc.right)
		{
			r->left = rc.right + 1;
			r->right = r->left + thumb_len;
			if(r->right >= rt.left)
				r->right = rt.left - 1;
		}
	
		if(r->right >= rt.left)
		{
			r->right = rt.left - 1;
			r->left = r->right - thumb_len;
			if(r->left <= rc.right)
				r->left = rc.right + 1;
		}
	}
	else
	{
		r->top += (draw_pos + rc.GetHeight());
		r->bottom = r->top + thumb_len;

		if(r->top <= rc.bottom)
		{
			r->top = rc.bottom + 1;
			r->bottom = r->top + thumb_len;
			if(r->bottom >= rt.top)
				r->bottom = rt.top - 1;
		}

		if(r->bottom >= rt.top)
		{
			r->bottom = rt.top - 1;
			r->top = r->bottom - thumb_len;
			if(r->top <= rc.bottom)
				r->top = rc.bottom + 1;
		}
	}
	
	return OK;
}

int CScrollBar::SetDir(int dir)
{
	if(dir == SBS_HORZ)
		this->sb_style |= SBS_HORZ;
	else
		this->sb_style &= (~SBS_HORZ);
	return OK;
}

int CScrollBar::GetPage1Rect(CRect *r)
{
	CRect rc,rt;
	ASSERT(r);
	
	r->Copy(this->rect);

	this->GetArrow1Rect(&rc);
	this->GetThumbRect(&rt);

	if(this->IsHorz())
	{
		r->left = rc.right + 1;
		r->right = rt.left - 1;
	}
	else
	{
		r->top = rc.bottom + 1 ;
		r->bottom = rt.top - 1;
	}

	return OK;
}

int CScrollBar::GetPage2Rect(CRect *r)
{
	CRect rc,rt;
	ASSERT(r);
	
	r->Copy(this->rect);

	this->GetArrow2Rect(&rc);
	this->GetThumbRect(&rt);

	if(this->IsHorz())
	{
		r->left = rt.right + 1;
		r->right = rc.left - 1;
	}
	else
	{
		r->top = rt.bottom + 1;
		r->bottom = rc.top - 1;
	}

	return OK;
}

int CScrollBar::Scroll(int cx)
{	
	if( !this->SetPos(this->cur_pos + cx) )
	{
		this->Redraw();
		return ERROR;
	}
	
	this->Redraw();

	CWindow *pwnd = afx.wndmgr->GetWindowByID(this->win_id);
	ASSERT(pwnd);

	pwnd->SendMessage(WM_COMMAND,0,this->id);

	return OK;
}

int CScrollBar::SetThumbPos(int pixel_pos)
{
	CRect rc,rt,rp;
	int pos;

	this->GetArrow1Rect(&rc);
	this->GetArrow2Rect(&rt);

	rp.Copy(this->rect);
	if(this->IsHorz())
	{
		rp.left = rc.right + 1;
		rp.right = rt.left - 1;
		
		if(rp.GetWidth() <= 0)
			return ERROR;

		pos = (pixel_pos - rp.left) * (this->max - this->min + 1) / rp.GetWidth();
	}
	else
	{
		rp.top = rc.bottom + 1;
		rp.bottom = rt.top - 1;
		
		if(rp.GetHeight() <= 0)
			return ERROR;

		pos = (pixel_pos - rp.top) * (this->max - this->min + 1) / rp.GetHeight();
	}
	this->Scroll(pos - this->cur_pos);
	return OK;
}

int CScrollBar::SetPageSize(int ps)
{
	this->page_size = ps;
	return OK;
}

int CScrollBar::SetPageStep(int ps)
{
	this->page_step = ps;
	return OK;
}

int CScrollBar::GetPos()
{
	return this->cur_pos;
}

int CScrollBar::SetLineStep(int ls)
{
	this->line_step = ls;
	return OK;
}

int CScrollBar::StartAutoScroll(int dir)
{
	if(dir == SB_AUTO_SCROLL_DIR_UP)
	{
		this->style |= SBS_AUTO_SCROLL_UP;
		this->style &= (~SBS_AUTO_SCROLL_DOWN);
	}

	if(dir == SB_AUTO_SCROLL_DIR_DOWN)
	{
		this->style |= SBS_AUTO_SCROLL_DOWN;
		this->style &= (~SBS_AUTO_SCROLL_UP);
	}

	this->style |= SBS_AUTO_SCROLL_ENABLE;
	afx.wndmgr->PostMessage(this->win_id,WM_SB_AUTO_SCROLL,0,this->id); // make loop start

	return OK;
}

int CScrollBar::StopAutoScroll()
{
	this->style &= (~SBS_AUTO_SCROLL_ENABLE);
	return OK;
}

int CScrollBar::OnLButtonUp(int wparam, int lparam)
{
	this->StopAutoScroll();
	afx.wndmgr->ReleaseCaptureByIndex(afx.wndmgr->IdToIndex(this->win_id));
	this->drag_mode = FALSE;
	if(this->sb_style & SBS_ARROW1_CHECKED)
	{
		this->sb_style &= ~SBS_ARROW1_CHECKED;
		this->Redraw();
	}

	if(this->sb_style & SBS_ARROW2_CHECKED)
	{
		this->sb_style &= ~SBS_ARROW2_CHECKED;
		this->Redraw();
	}
	
	return OK;
}
