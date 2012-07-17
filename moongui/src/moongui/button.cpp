#include "button.h"
#include "gui_misc.h"
#include "globals.h"

CButton::CButton()
{
    this->InitBasic();
}
CButton::~CButton()
{
    this->Destroy();
}
int CButton::InitBasic()
{
    CControl::InitBasic();
    return OK;
}
int CButton::Init()
{
    this->InitBasic();
    CControl::Init();
    
	this->ctrl_name = CTRL_NAME_BUTTON;
	this->stock_img = -1;

    return OK;
}
int CButton::Destroy()
{
    //add your code
    CControl::Destroy();
    this->InitBasic();
    return OK;
}
int CButton::Copy(CButton *p)
{
    if(this == p)
        return OK;
    CControl::Copy(p);
    return OK;
}
int CButton::Comp(CButton *p)
{
    return 0;
}
int CButton::Print()
{
    CControl::Print();
    return TRUE;
}

int CButton::Draw(CDc *pdc)
{	
	CRect rc,rt;
	ImageInfo *img;

	ASSERT(pdc);

	if(this->style & WS_CHECKED)
		Draw3dThinDownFrame(pdc,this->rect);
	else
		Draw3dThinUpFrame(pdc,this->rect);
	
	rc.Copy(this->rect);
	
	if(this->stock_img >= 0)
	{
		img = GetStockImage(this->stock_img);
		
		rc.CenterRect(img->width,img->height,&rt);
		if(this->i_text)
		{
			rt.left = rc.left + 1;
			rt.right = rt.left + img->width;
			rc.left = rt.right + 1;
		}

		if(this->style & WS_CHECKED)
			rt.Offset(1,1);
		pdc->DrawMonoImage(rt.left,rt.top,img,this->fore_color,this->rect);
		

	}

	if(this->i_text)
	{
		pdc->SetTextColor(this->fore_color);

		if(this->style & WS_CHECKED)
			rc.Offset(1,1);

		pdc->DrawText(this->i_text,&rc,DT_VCENTER|DT_CENTER);
	}


	return OK;
}

int CButton::HandleMessage(int message, int wparam, int lparam)
{
	int x,y;

	switch(message)
	{
		case WM_LBUTTONDOWN: 
			x = (short)LOWORD(lparam);	
			y = (short)HIWORD(lparam);

			if(!this->PtInCtrl(x,y))
				return ERROR;

			if(!(this->style & WS_CHECKED))
			{
				this->style |= WS_CHECKED;
				this->Redraw();
				return OK;
			}
		break;

		case WM_LBUTTONUP: 
			x = (short)LOWORD(lparam);	
			y = (short)HIWORD(lparam);

			if(!this->PtInCtrl(x,y))
				return ERROR;

			if(this->style & WS_CHECKED)
			{
				this->style &= ~WS_CHECKED;
				this->Redraw();
				afx.wndmgr->SendMessage(this->win_id,WM_COMMAND,0,this->id);
				return OK;
			}
		break;

		case WM_MOUSEMOVE:
			x = (short)LOWORD(lparam);	
			y = (short)HIWORD(lparam);

			if(this->PtInCtrl(x,y))
				return ERROR;

			if(this->style & WS_CHECKED)
			{
				this->style &= ~WS_CHECKED;
				this->Redraw();
				return OK;
			}
		break;

		case WM_MOUSEOUT:
			if(this->style & WS_CHECKED)
			{
				this->style &= ~WS_CHECKED;
				this->Redraw();
				return OK;
			}
		break;
	}
	return ERROR;
}

int CButton::SetStockImage(int stock_img)
{
	this->stock_img = stock_img;
	return OK;
}
