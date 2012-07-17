#include "dc.h"

CDc::CDc()
{
    this->InitBasic();
}
CDc::~CDc()
{
    this->Destroy();
}
int CDc::InitBasic()
{
	this->i_font = NULL;
	this->clip_rgn = NULL;

	this->bkcolor = RGB(255,255,255);
	this->textcolor = RGB(0,0,0);
	this->pencolor = RGB(0,0,0);
	this->brushcolor = RGB(255,255,255);

	this->pos.x = 0;
	this->pos.y = 0;

    return OK;
}
int CDc::Init()
{
    this->InitBasic();
    
	NEW(this->clip_rgn,CClipRgn);
	this->clip_rgn->Init(32);

    return OK;
}
int  CDc::Destroy()
{
    DEL(this->clip_rgn);

    this->InitBasic();
    return OK;
}
int  CDc::Copy(CDc *p)
{
	ASSERT(p);

    if(this == p)
        return OK;

	this->pencolor = p->pencolor;
	this->bkcolor = p->bkcolor;
	this->brushcolor = p->brushcolor;
	this->textcolor = p->textcolor;
	this->pos.x = p->pos.x;
	this->pos.y = p->pos.y;
	this->clip_rgn->Copy(p->clip_rgn);
	this->i_font = p->i_font;

    return OK;
}
int CDc::Comp(CDc *p)
{
    return 0;
}
int  CDc::Print()
{
    return TRUE;
}

int CDc::SetPos(int x, int y)
{
	this->pos.x = x;
	this->pos.y = y;

	return OK;
}

int CDc::LP2SP(int *x, int *y)
{
	*x += this->pos.x;
	*y += this->pos.y;

	return OK;
}

int CDc::SP2LP(int *x, int *y)
{
	*x -= this->pos.x;
	*y -= this->pos.y;

	return OK;
}

int CDc::LR2SR(CRect *r)
{
	ASSERT(r);

	r->left += this->pos.x;
	r->top += this->pos.y;
	r->right += this->pos.x;
	r->bottom += this->pos.y;

	return OK;
}

int CDc::SR2LR(CRect *r)
{
	ASSERT(r);

	r->left -= this->pos.x;
	r->top -= this->pos.y;
	r->right -= this->pos.x;
	r->bottom -= this->pos.y;

	return OK;
}

int CDc::SetPixel(int x, int y, pixel_type color)
{
	CRect *prect;
	int i;
	
	this->LP2SP(&x,&y);

	for(i = 0; i < this->clip_rgn->GetLen(); i++)
	{
		prect = this->clip_rgn->GetElem(i);
		ASSERT(prect);

		if(prect->PtInRect(x,y))
		{
			GAL_SetPixel(x,y,color);
			break;
		}
	}
	
	return OK;
}

int CDc::HLine(int x, int y, int w)
{
	CRect rt,*prect;	
	int i;
	
	this->LP2SP(&x,&y);

	rt.Set(x,y,x + w - 1, y);

	if( ! rt.IsIntersect(this->clip_rgn->rcBound) )
		return ERROR;

	for(i = 0; i < this->clip_rgn->GetLen(); i++)
	{
		prect = this->clip_rgn->GetElem(i);
		ASSERT(prect);

		if(prect->IsIntersect(&rt))
		{
			GAL_SetClipping(prect->left,prect->top,prect->right,prect->bottom);
			GAL_HLine(x,y,w,this->pencolor);
		}
	}

	return OK;
}
int CDc::VLine(int x, int y, int h)
{
	CRect rt,*prect;	
	int i;
	
	this->LP2SP(&x,&y);

	rt.Set(x,y,x, y + h - 1);

	if( ! rt.IsIntersect(this->clip_rgn->rcBound) )
		return ERROR;

	for(i = 0; i < this->clip_rgn->GetLen(); i++)
	{
		prect = this->clip_rgn->GetElem(i);
		ASSERT(prect);

		if(prect->IsIntersect(&rt))
		{
			GAL_SetClipping(prect->left,prect->top,prect->right,prect->bottom);
			GAL_VLine(x,y,h,this->pencolor);
		}
	}

	return OK;
}

int CDc::DrawRect(int x, int y, int w, int h)
{
	this->HLine(x,y,w);
	this->HLine(x,y+h-1,w);
	this->VLine(x,y,h);
	this->VLine(x+w-1,y,h);
	
	return OK;
}

int CDc::FillRect(int x, int y, int w, int h)
{
	CRect rt,*prect;	
	int i;
	
	this->LP2SP(&x,&y);

	rt.Set(x,y,x + w -1 , y + h - 1);

	if( ! rt.IsIntersect(this->clip_rgn->rcBound) )
		return ERROR;

	for(i = 0; i < this->clip_rgn->GetLen(); i++)
	{
		prect = this->clip_rgn->GetElem(i);
		ASSERT(prect);

		if(prect->IsIntersect(&rt))
		{
			GAL_SetClipping(prect->left,prect->top,prect->right,prect->bottom);
			GAL_FillRect(x,y,w,h,this->brushcolor);
		}
	}
	
	return OK;
}

int CDc::SetPenColor(pixel_type pc)
{
	this->pencolor = pc;
	return OK;
}

int CDc::SetBrushColor(pixel_type bc)
{
	this->brushcolor = bc;
	return OK;
}

int CDc::SetTextColor(pixel_type tc)
{
	this->textcolor = tc;
	return OK;
}

int CDc::SetBkColor(pixel_type bc)
{
	this->bkcolor = bc;
	return OK;
}

int CDc::SetFont(CFont *font)
{
	this->i_font = font;

	return OK;
}

int CDc::DrawText(char *str, CRect *rect, u32 dwDTFormat)
{
	ASSERT(str && rect && this->i_font);
	
	int x,y,w,h,i;
	CRect rc,rt,*prect;

	rc.Copy(rect);
	this->LR2SR(&rc);

	this->i_font->GetTextSize(str,&w,&h);
	
	x = rc.left; y = rc.top;

	if(dwDTFormat & DT_CENTER)
	{
		x = rc.left + ( rc.GetWidth() - w) / 2;
		y = rc.top + ( rc.GetHeight() - h) / 2;
	}

	if(dwDTFormat & DT_VCENTER)
	{
		y = rc.top + ( rc.GetHeight() - h) / 2;
	}

	if(dwDTFormat & DT_RIGHT)
	{
		x = rc.right - w;
	}
	
	if( x < rc.left ) x = rc.left;
	if( y < rc.top ) y = rc.top;

	this->i_font->SetColor(this->textcolor);

	for(i = 0; i < this->clip_rgn->GetLen(); i++)
	{
		prect = this->clip_rgn->GetElem(i);
		ASSERT(prect);

		rt.Intersect(&rc,prect);

		if( ! rt.IsEmpty() )
		{
			GAL_SetClipping(rt.left,rt.top,rt.right,rt.bottom);
			this->i_font->DrawText(x,y,str);			
		}
	}

	return OK;
}


int CDc::AddClipRect(CRect *r)
{
	if(r->IsEmpty())
		return ERROR;

	this->clip_rgn->Subtract(r);
	this->clip_rgn->AddRect(r);

	return OK;
}

int CDc::SubtractClipRect(CRect *r)
{	
	return this->clip_rgn->Subtract(r);	
}

int CDc::TextOut(int x, int y, char *str, CRect *rc)
{
	ASSERT(this->i_font);

	CRect r,rt,*prect;
	int i;

	if(str == NULL)
		return OK;

	this->LP2SP(&x,&y);

	if(rc == NULL)
	{
		r.Set(0,0,GAL_GetWidth(),GAL_GetHeight());
	}
	else
	{
		r.Copy(rc);
		this->LR2SR(&r);
	}

	this->i_font->SetColor(this->textcolor);

	for(i = 0; i < this->clip_rgn->GetLen(); i++)
	{
		prect = this->clip_rgn->GetElem(i);
		ASSERT(prect);

		rt.Intersect(&r,prect);

		if( ! rt.IsEmpty() )
		{
			GAL_SetClipping(rt.left,rt.top,rt.right,rt.bottom);
			this->i_font->DrawText(x,y,str);
		}
	}

	return OK;
}

int CDc::Intersect(CRect *r)
{
	return this->clip_rgn->Intersect(r);
}

int CDc::GetBound(CRect *r)
{
	ASSERT(r);
	r->Copy(this->clip_rgn->rcBound);

	return OK;
}

// x and y are both absolute
int CDc::PtVisible(int x, int y)
{
	CRect *prect;	
	int i;
	
	for(i = 0; i < this->clip_rgn->GetLen(); i++)
	{
		prect = this->clip_rgn->GetElem(i);
		ASSERT(prect);

		if(prect->PtInRect(x,y))
			return TRUE;
	}
	
	return FALSE;
}

int CDc::DrawMonoImage(int x, int y, ImageInfo *img,pixel_type fc)
{
	return this->DrawMonoImage(x,y,img,fc,NULL);
}

int CDc::DrawMonoImage(int x, int y, ImageInfo *img, pixel_type fc, CRect *rclip)
{
	ASSERT(img);

	CRect r,rt,*prect;
	int i;

	this->LP2SP(&x,&y);

	r.Set(x,y,x + img->width - 1, y + img->height - 1);
	
	if(rclip)
	{
		rt.Copy(rclip);
		this->LR2SR(&rt);
		r.Intersect(&r,&rt);
	}

	for(i = 0; i < this->clip_rgn->GetLen(); i++)
	{
		prect = this->clip_rgn->GetElem(i);
		ASSERT(prect);

		rt.Intersect(&r,prect);

		if( ! rt.IsEmpty() )
		{
			GAL_SetClipping(rt.left,rt.top,rt.right,rt.bottom);
			GAL_DrawMonoImage(x,y,img->width,img->height,(char*)img->addr,fc);
		}
	}

	return OK;
}

int CDc::FillRect(CRect *rect)
{
	ASSERT(rect);
	return this->FillRect(rect->left,rect->top,rect->GetWidth(),rect->GetHeight());
}

int CDc::DrawRect(CRect *rect)
{
	ASSERT(rect);
	return this->DrawRect(rect->left,rect->top,rect->GetWidth(),rect->GetHeight());
}

