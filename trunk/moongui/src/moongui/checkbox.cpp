#include "checkbox.h"
#include "gui_misc.h"
#include "globals.h"
#include "sizemap.h"
#include "imagemap.h"

CCheckBox::CCheckBox()
{
    this->InitBasic();
}
CCheckBox::~CCheckBox()
{
    this->Destroy();
}
int CCheckBox::InitBasic()
{
    CControl::InitBasic();
    return OK;
}
int CCheckBox::Init()
{
    this->InitBasic();
    CControl::Init();
    
	this->back_color = COLOR_WHITE;
	this->ctrl_name = CTRL_NAME_CHECK_BOX;

    return OK;
}
int CCheckBox::Destroy()
{
    //add your code
    CControl::Destroy();
    this->InitBasic();
    return OK;
}
int CCheckBox::Copy(CCheckBox *p)
{
    if(this == p)
        return OK;
    CControl::Copy(p);
    return OK;
}
int CCheckBox::Comp(CCheckBox *p)
{
    return 0;
}
int CCheckBox::Print()
{
    CControl::Print();
    return TRUE;
}

int CCheckBox::Draw(CDc *pdc)
{
	CRect rc,rt;
	int box_size = GetSysSize(SM_CHECK_BOX);
	ImageInfo *img;

	ASSERT(pdc);

	this->rect->CenterRect(box_size,box_size,&rc);

	rc.left = this->rect->left;
	rc.right = rc.left + box_size;

	pdc->SetBrushColor(this->back_color);
	pdc->FillRect(&rc);

	if(this->style & WS_CHECKED)
	{
		img = GetStockImage(IMAGE_CHECKED);
		rc.CenterRect(img->width,img->height,&rt);
		pdc->DrawMonoImage(rt.left, rt.top,img,this->fore_color);
	}

	Draw3dThinDownBorder(pdc,&rc);

	if(this->i_text)
	{
		pdc->SetTextColor(this->fore_color);
		rc.Copy(this->rect);		
		rc.left += box_size + 2;
		pdc->DrawText(this->i_text,&rc,DT_VCENTER|DT_LEFT);
	}


	return OK;
}

int CCheckBox::HandleMessage(int message, int wparam, int lparam)
{
	int x,y;

	switch(message)
	{
		case WM_LBUTTONDOWN: 
			x = LOWORD(lparam);	
			y = HIWORD(lparam);

			if(!this->PtInCtrl(x,y))
				return ERROR;

			if(this->style & WS_CHECKED)
				this->style &= ~WS_CHECKED;
			else
				this->style |= WS_CHECKED;
			
			this->Redraw();			
			afx.wndmgr->SendMessage(this->win_id,WM_COMMAND,0,this->id);
			return OK;
		break;
	}
	return ERROR;
}

int CCheckBox::Checked()
{
	return this->style & WS_CHECKED;
}
