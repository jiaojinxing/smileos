#include "menuitem.h"
#include "colormap.h"
#include "sizemap.h"
#include "syslog.h"
#include "globals.h"

CMenuItem::CMenuItem()
{
    this->InitBasic();
}
CMenuItem::~CMenuItem()
{
    this->Destroy();
}
int CMenuItem::InitBasic()
{
	this->mem_str = NULL;
	this->win_id = 0;
	this->i_font = NULL;
	this->style = 0;
	this->child_win_id = 0;

    return OK;
}
int CMenuItem::Init()
{
    this->InitBasic();
    NEW(this->mem_str,CMem);
	this->mem_str->Init();
	this->id = afx.GetUniqueID();

    return OK;
}
int CMenuItem::Destroy()
{
    DEL(this->mem_str);
    this->InitBasic();
    return OK;
}
int CMenuItem::Copy(CMenuItem *p)
{
    if(this == p)
        return OK;

    return OK;
}
int CMenuItem::Comp(CMenuItem *p)
{
    return 0;
}
int CMenuItem::Print()
{
	LOG("name=%s\n",mem_str->p);
	LOG("win_id=%d\n",this->win_id);
    return TRUE;
}

int CMenuItem::SetName(char *str)
{
	ASSERT(str);
	return this->mem_str->SetStr(str);
}

int CMenuItem::GetWidth()
{
	if(this->IsSeparator())
		return 0;

	ASSERT(this->i_font);
	int cx = 0,cy = 0;

	if(this->mem_str->GetSize() > 0)
		this->i_font->GetTextSize(this->mem_str->p,&cx,&cy);

	return cx + 12;
}

int CMenuItem::GetHeight()
{
	if(this->IsSeparator())
		return 5;

	ASSERT(this->i_font);
	return this->i_font->GetMaxCharHeight() + 2;
}

int CMenuItem::Draw(CCanvas *can,int x,int y)
{
	ASSERT(can);
	ImageInfo *img;
	pixel_type c;
	CRect rc,rt;

	if(!this->IsSeparator())
	{
		rc.Set(x,y,x + can->rect->GetWidth() -1 ,y + this->GetHeight() - 1);
		if(this->IsHilighted())
		{
			can->SetBrushColor(COLOR_BLUE);
			can->FillRect(&rc);
		}

		c = this->IsHilighted() ? COLOR_WHITE:COLOR_BLACK;
		can->SetTextColor(c);
		can->TextOut(rc.left + GetSysSize(SM_MENUITEMOFFX),rc.top + 1,this->mem_str->p,NULL);
		if(this->HasChild())
		{
			img = GetStockImage(IMAGE_RIGHT_SMALL_ARROW);
			rc.CenterRect(img->width,img->height,&rt);
			rt.right = rc.right;
			rt.left = rt.right - img->width;
			can->DrawMonoImage(rt.left,rt.top,img,NULL);
		}
	}
	else
	{
		can->Draw3DHLine(x, y + this->GetHeight() / 2 - 1,can->rect->GetWidth());
	}
	return OK;
}

int CMenuItem::IsHilighted()
{
	return this->style & MS_HILIGHT;
}

int CMenuItem::IsSeparator()
{
	return this->style & MS_SEPARATOR;
}

int CMenuItem::HasChild()
{
	return this->child_win_id > 0;
}

int CMenuItem::SetChildMenu(int child_win_id)
{
	this->child_win_id = child_win_id;
	return OK;
}

int CMenuItem::SetHiLight(int flag)
{
	if(flag) 
		this->style |= MS_HILIGHT;
	else
		this->style &= (~MS_HILIGHT);

	return OK;
}

