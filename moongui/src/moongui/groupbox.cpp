#include "groupbox.h"
#include "gui_misc.h"
#include "globals.h"

CGroupBox::CGroupBox()
{
    this->InitBasic();
}
CGroupBox::~CGroupBox()
{
    this->Destroy();
}
int CGroupBox::InitBasic()
{
    CControl::InitBasic();
    return OK;
}
int CGroupBox::Init()
{
    this->InitBasic();
    CControl::Init();
    
	this->ctrl_name = CTRL_NAME_GROUP_BOX;

    return OK;
}
int CGroupBox::Destroy()
{
    //add your code
    CControl::Destroy();
    this->InitBasic();
    return OK;
}
int CGroupBox::Copy(CGroupBox *p)
{
    if(this == p)
        return OK;
    CControl::Copy(p);
    return OK;
}
int CGroupBox::Comp(CGroupBox *p)
{
    return 0;
}
int CGroupBox::Print()
{
    CControl::Print();
    return TRUE;
}

int CGroupBox::Draw(CDc *pdc)
{
	ASSERT(pdc);

	int cx,cy;
	CRect rc,rt;

	rc.Copy(this->rect);

	if(this->i_text)
	{
		rc.top += (pdc->i_font->GetMaxCharHeight() / 2 );
		Draw3DLineRect(pdc,&rc);
		pdc->i_font->GetTextSize(this->i_text,&cx,&cy);
		
		rt.top = this->rect->top;
		rt.left = this->rect->left + 5;
		rt.right = rt.left + cx;
		rt.bottom = rt.top + cy;

		pdc->SetBrushColor(this->back_color);
		pdc->FillRect(&rt);
		pdc->SetTextColor(this->fore_color);
		pdc->TextOut(rt.left,rt.top,this->i_text,&rt);
	}
	else
	{
		Draw3DLineRect(pdc,&rc);
	}
	

	return OK;
}
