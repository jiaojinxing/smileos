#include "radiobox.h"
#include "gui_misc.h"
#include "globals.h"
#include "sizemap.h"
#include "imagemap.h"

CRadioBox::CRadioBox()
{
    this->InitBasic();
}
CRadioBox::~CRadioBox()
{
    this->Destroy();
}
int CRadioBox::InitBasic()
{
    CControl::InitBasic();
	this->group = -1;
    return OK;
}
int CRadioBox::Init()
{
    this->InitBasic();
    CControl::Init();
    
	this->back_color = COLOR_WHITE;
	this->ctrl_name = CTRL_NAME_RADIO_BOX;

    return OK;
}
int CRadioBox::Destroy()
{
    //add your code
    CControl::Destroy();
    this->InitBasic();
    return OK;
}
int CRadioBox::Copy(CRadioBox *p)
{
    if(this == p)
        return OK;
    CControl::Copy(p);
    return OK;
}
int CRadioBox::Comp(CRadioBox *p)
{
    return 0;
}
int CRadioBox::Print()
{
    CControl::Print();
    return TRUE;
}

int CRadioBox::Draw(CDc *pdc)
{
	CRect rc;
	int box_size;
	ImageInfo *img;

	ASSERT(pdc);

	img = GetStockImage(IMAGE_RADIO_BACK);
	box_size = img->width;
	
	this->rect->CenterRect(box_size,box_size,&rc);
	rc.left = this->rect->left;
	
	pdc->DrawMonoImage(rc.left,rc.top,img,this->back_color);
	img = GetStockImage(IMAGE_RADIO_CIRCLE);
	pdc->DrawMonoImage(rc.left,rc.top,img,this->fore_color);

	if(this->style & WS_CHECKED)
	{
		img = GetStockImage(IMAGE_RADIO_DOT);
		pdc->DrawMonoImage(rc.left,rc.top,img,this->fore_color);
	}

	if(this->i_text)
	{
		pdc->SetTextColor(this->fore_color);
		rc.Copy(this->rect);		
		rc.left += box_size + 2;
		pdc->DrawText(this->i_text,&rc,DT_VCENTER|DT_LEFT);
	}

	return OK;
}

int CRadioBox::HandleMessage(int message, int wparam, int lparam)
{
	int x,y;

	switch(message)
	{
		case WM_LBUTTONDOWN: 
			x = LOWORD(lparam);	
			y = HIWORD(lparam);
			
			if(!this->PtInCtrl(x,y))
				return ERROR;
			
			this->SetCheck(TRUE);
			
			afx.wndmgr->SendMessage(this->win_id,WM_COMMAND,0,this->id);
			return OK;
		break;
	}
	return ERROR;
}

int CRadioBox::SetCheck(int flag)
{
	CControl *pc;
	CRadioBox *pr;
	int i;

	if(flag)
	{
		if(this->IsChecked())
			return ERROR;

		for(i = 0; i <  afx.ctrlmgr->GetLen(); i++)
		{
			pc = afx.ctrlmgr->GetElem(i);
			ASSERT(pc);
			
			if(pc == this)
				continue;

			if(pc->ctrl_name == CTRL_NAME_RADIO_BOX)
			{
				pr = (CRadioBox *)pc;
				if(pr->group >= 0 && pr->group == this->group && pr->IsChecked())
				{
					pr->style &= (~WS_CHECKED);
					pr->Redraw();
				}
			}
		}

		this->style |= WS_CHECKED;
		this->Redraw();
	} 
	else
	{
		if( ! this->IsChecked() )
			return ERROR;

		this->style |= WS_CHECKED;
		this->Redraw();
	}

	return ERROR;
}

int CRadioBox::IsChecked()
{
	return this->style & WS_CHECKED;
}
