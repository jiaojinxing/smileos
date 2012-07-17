#include "caret.h"
#include "colormap.h"
#include "globals.h"

CCaret::CCaret()
{
    this->InitBasic();
}
CCaret::~CCaret()
{
    this->Destroy();
}
int CCaret::InitBasic()
{
	this->win_id = -1;
	this->x = 0;
	this->y = 0;
	this->width = 0;
	this->height = 0;
	this->img_buf = NULL;
	this->is_show = FALSE;
	this->global_rect = NULL;
	this->time_out = 1000;
	this->old_time = 0;
	this->is_blink = FALSE;

    return OK;
}
int CCaret::Init()
{
    this->InitBasic();    
	NEW(this->global_rect,CRect);

    return OK;
}
int CCaret::Destroy()
{
	DEL(this->global_rect);
    FREE(this->img_buf);
    this->InitBasic();
    return OK;
}

int CCaret::SetSize(int w, int h)
{
	ASSERT(this->img_buf == NULL);

	this->width = w;
	this->height = h;
	MALLOC(this->img_buf,pixel_type,w * h);

	return OK;
}

int CCaret::Show()
{
	if(this->is_show)
		return ERROR;
	this->is_show = TRUE;

	CWindow *pwnd = afx.wndmgr->GetWindowByID(this->win_id);
	CRect rm;

	if(pwnd == NULL)
		return ERROR;
	
	this->global_rect->Set(x,y,x + width - 1, y + height -1);
	pwnd->LR2SR(this->global_rect);

	afx.mouse->GetRect(&rm);

	if(rm.IsIntersect(this->global_rect))
		afx.mouse->Hide();

	get_image(this->global_rect->left,this->global_rect->top,
	  		  this->global_rect->GetWidth(),this->global_rect->GetHeight(),
			  this->img_buf); 	

	pwnd->privCDC->SetBrushColor(COLOR_RED);
	pwnd->privCDC->FillRect(x,y,width,height);

	afx.mouse->Show();

	return OK;
}

int CCaret::Hide()
{
	if( ! this->is_show )
		return ERROR;
	this->is_show = FALSE;

	CRect cm;

	afx.mouse->GetRect(&cm);
	if(cm.IsIntersect(this->global_rect))
		afx.mouse->Hide();
	
	put_image(this->global_rect->left,this->global_rect->top,
	 		  this->global_rect->GetWidth(),this->global_rect->GetHeight(),
			  this->img_buf); 	

	afx.mouse->Show();

	return OK;
}

int CCaret::SetPos(int x, int y)
{
	this->Hide(); //hide first

	this->x = x;
	this->y = y;

	return OK;
}

int CCaret::SetBlinkTime(int time_out)
{
	this->time_out = time_out;
	return OK;
}

int CCaret::StartBlink()
{
	this->is_blink = TRUE;
	return OK;
}

int CCaret::StopBlink()
{
	this->Hide();
	this->is_blink = FALSE;
	return OK;
}
