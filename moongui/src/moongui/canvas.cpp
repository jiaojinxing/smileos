#include "canvas.h"
#include "colormap.h"

CCanvas::CCanvas()
{
    this->InitBasic();
}
CCanvas::~CCanvas()
{
    this->Destroy();
}
int CCanvas::InitBasic()
{
	this->i_dc = NULL;
	this->rect = NULL;
	this->view_x = 0;
	this->view_y = 0;

    return OK;
}
int CCanvas::Init()
{
    this->InitBasic();
    
	NEW(this->rect,CRect);

    return OK;
}
int CCanvas::Destroy()
{
    DEL(this->rect);
    this->InitBasic();
    return OK;
}

int CCanvas::SetDC(CDc *pdc)
{
	this->i_dc =pdc;
	return OK;
}

int CCanvas::SetRect(int x, int y,int w,int h)
{
	return this->rect->Set(x,y,x+w-1,y+h-1);
}

int CCanvas::SetViewPos(int x, int y)
{
	this->view_x = x;
	this->view_y = y;

	return OK;
}

int CCanvas::HLine(int x, int y, int w)
{
	ASSERT(this->i_dc);

	CRect rl,rc,rt;

	rl.Set(x, y, x + w - 1, y);

	this->LR2SR(&rl);

	rt.Intersect(&rl,this->rect);

	if(rt.IsEmpty())
		return ERROR;

	return this->i_dc->HLine(rt.left,rt.top,rt.GetWidth());
}

int CCanvas::VLine(int x, int y, int h)
{
	ASSERT(this->i_dc);

	CRect rl,rc,rt;

	rl.Set(x, y, x, y + h -1);
	this->LR2SR(&rl);

	rt.Intersect(&rl,this->rect);

	if(rt.IsEmpty())
		return ERROR;

	return this->i_dc->VLine(rt.left,rt.top,rt.GetHeight());
}

int CCanvas::TextOut(int x, int y, char *str, CRect *rect)
{
	CRect rc,rt;

	ASSERT(this->i_dc);

	if(rect)
	{
		rc.Copy(rect);
		this->LR2SR(&rc);
	}
	else
	{
		rc.Copy(this->rect);		
	}

	
	this->LP2SP(&x,&y);
	rt.Intersect(&rc,this->rect);

	if(rt.IsEmpty())
		return ERROR;

	return this->i_dc->TextOut(x,y,str,&rt);
}

int CCanvas::LP2SP(int *x, int *y)
{
	*x += (this->rect->left - this->view_x);
	*y += (this->rect->top - this->view_y);

	return OK;
}

int CCanvas::LR2SR(CRect *lr)
{
	ASSERT(lr);
	return lr->Offset(this->rect->left - this->view_x,this->rect->top - this->view_y);
}

int CCanvas::SetRect(CRect *r)
{
	ASSERT(r);
	return this->rect->Copy(r);
}

int CCanvas::SetPenColor(pixel_type pc)
{
	ASSERT(this->i_dc);
	return this->i_dc->SetPenColor(pc);
}

int CCanvas::SetBrushColor(pixel_type bc)
{
	ASSERT(this->i_dc);
	return this->i_dc->SetBrushColor(bc);
}

int CCanvas::SetTextColor(pixel_type tc)
{
	ASSERT(this->i_dc);
	return this->i_dc->SetTextColor(tc);
}

int CCanvas::FillRect(CRect *r)
{
	CRect rc,rt;

	ASSERT(r);

	rc.Copy(r);
	this->LR2SR(&rc);

	rt.Intersect(&rc,this->rect);

	if(rt.IsEmpty())
		return ERROR;

	return this->i_dc->FillRect(&rt);
}

int CCanvas::FillRect(int x, int y, int w, int h)
{
	CRect r;

	r.Set(x,y,x+w-1,y+h-1);
	return this->FillRect(&r);
}

int CCanvas::SP2LP(int *x, int *y)
{
	*x -= (this->rect->left - this->view_x);
	*y -= (this->rect->top - this->view_y);
	return OK;
}

int CCanvas::SR2LR(CRect *r)
{
	return r->Offset(this->view_x - this->rect->left,this->view_y - this->rect->top);
}

int CCanvas::Draw3DHLine(int x, int y, int w)
{
	this->SetPenColor(GetSysColor(COLOR_3DSHADOW));
	this->HLine(x,y,w);
	this->SetPenColor(GetSysColor(COLOR_3DHILIGHT));
	this->HLine(x,y+1,w);
	return OK;
}

int CCanvas::DrawMonoImage(int x, int y, ImageInfo *img, CRect *rclip)
{
	ASSERT(img);
	CRect rc,rt;

	if(rclip)
	{
		rc.Copy(rclip);	
		this->LR2SR(&rc);
	}
	else
	{
		rc.Copy(this->rect);		
	}

	this->LP2SP(&x,&y);

	rt.Intersect(&rc,this->rect);
	if(rt.IsEmpty())
		return ERROR;

	return this->i_dc->DrawMonoImage(x,y,img,this->i_dc->textcolor,&rt);	
}

int CCanvas::DrawRect(CRect *r)
{
	ASSERT(r);	
	return this->DrawRect(r->left,r->top,r->GetWidth(),r->GetHeight());
}

int CCanvas::DrawRect(int x, int y,int w,int h)
{
	this->HLine(x,y,w);
	this->HLine(x,y+h-1,w);
	this->VLine(x,y,h);
	this->VLine(x+w-1,y,h);
	
	return OK;
}
