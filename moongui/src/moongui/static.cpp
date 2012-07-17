#include "static.h"
#include "gui_misc.h"
#include "globals.h"

CStatic::CStatic()
{
    this->InitBasic();
}
CStatic::~CStatic()
{
    this->Destroy();
}
int CStatic::InitBasic()
{
    CControl::InitBasic();
	this->st_style = 0;
    return OK;
}
int CStatic::Init()
{
    this->InitBasic();
    CControl::Init();
    
	this->ctrl_name = CTRL_NAME_STATIC;

    return OK;
}
int CStatic::Destroy()
{
    //add your code
    CControl::Destroy();
    this->InitBasic();
    return OK;
}
int CStatic::Copy(CStatic *p)
{
    if(this == p)
        return OK;
    CControl::Copy(p);
    return OK;
}
int CStatic::Comp(CStatic *p)
{
    return 0;
}
int CStatic::Print()
{
    CControl::Print();
    return TRUE;
}

int CStatic::Draw(CDc *pdc)
{
	u32 dt = DT_VCENTER | DT_LEFT;
	
	ASSERT(pdc);

	if(this->st_style == SS_3DHLINE)
	{
		Draw3DHLine(pdc,this->rect->left,this->rect->top,this->rect->GetWidth());
		return OK;
	}

	if(this->st_style == SS_3DVLINE)
	{
		Draw3DVLine(pdc,this->rect->left,this->rect->top,this->rect->GetHeight());
		return OK;
	}
	
	if(this->st_style == SS_RIGHT)
	{
		dt &= (~DT_LEFT);
		dt |= DT_RIGHT;
	}

	pdc->SetTextColor(this->fore_color);
	pdc->DrawText(this->i_text,this->rect,dt);

	return OK;
}

