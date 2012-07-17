#include "spinbox.h"
#include "gui_misc.h"
#include "globals.h"

#define SPINBOX_STYLE_BUTTON1_CHECKED  0x00000001
#define SPINBOX_STYLE_BUTTON2_CHECKED  0x00000002

CSpinBox::CSpinBox()
{
    this->InitBasic();
}
CSpinBox::~CSpinBox()
{
    this->Destroy();
}
int CSpinBox::InitBasic()
{
    CControl::InitBasic();
	this->sp_style = 0;
    return OK;
}
int CSpinBox::Init()
{
    this->InitBasic();
    CControl::Init();
    
	this->ctrl_name = CTRL_NAME_SPIN_BOX;

    return OK;
}
int CSpinBox::Destroy()
{
    //add your code
    CControl::Destroy();
    this->InitBasic();
    return OK;
}
int CSpinBox::Copy(CSpinBox *p)
{
    if(this == p)
        return OK;
    CControl::Copy(p);
    return OK;
}
int CSpinBox::Comp(CSpinBox *p)
{
    return 0;
}
int CSpinBox::Print()
{
    CControl::Print();
    return TRUE;
}

int CSpinBox::Draw(CDc *pdc)
{	
	CRect rc,rt;
	ImageInfo *img;

	ASSERT(pdc);

	this->GetRect1(&rc);
	this->GetRect2(&rt);

	if(this->sp_style & SPINBOX_STYLE_BUTTON1_CHECKED)
		Draw3dThinDownFrame(pdc,&rc);
	else
		Draw3dThinUpFrame(pdc,&rc);

	if(this->sp_style & SPINBOX_STYLE_BUTTON2_CHECKED)
		Draw3dThinDownFrame(pdc,&rt);
	else
		Draw3dThinUpFrame(pdc,&rt);

	img = GetStockImage(IMAGE_UP_SMALL_ARROW);
	rc.CenterRect(img->width,img->height,&rt);
	if(this->sp_style & SPINBOX_STYLE_BUTTON1_CHECKED)
		rt.Offset(1,1);
	pdc->DrawMonoImage(rt.left,rt.top,img,this->fore_color);

	this->GetRect2(&rc);
	img = GetStockImage(IMAGE_DOWN_SMALL_ARROW);
	rc.CenterRect(img->width,img->height,&rt);
	if(this->sp_style & SPINBOX_STYLE_BUTTON2_CHECKED)
		rt.Offset(1,1);
	pdc->DrawMonoImage(rt.left,rt.top,img,this->fore_color);


	return OK;
}

int CSpinBox::HandleMessage(int message, int wparam, int lparam)
{
	int x,y;
	CRect rc,rt;
	CWindow *pwnd;

	switch(message)
	{
		case WM_LBUTTONDOWN: 
			x = (short)LOWORD(lparam);	
			y = (short)HIWORD(lparam);

			if(!this->rect->PtInRect(x,y))
				return ERROR;
			this->GetRect1(&rc);
			this->GetRect2(&rt);
	
			pwnd = afx.wndmgr->GetWindowByID(this->win_id);
			ASSERT(pwnd);
	
			if(rc.PtInRect(x,y))
			{
				if(!(this->sp_style & SPINBOX_STYLE_BUTTON1_CHECKED))
				{
					pwnd->SendMessage(WM_COMMAND,0,this->id);
					this->sp_style |= SPINBOX_STYLE_BUTTON1_CHECKED;
					this->Redraw();
				}
			}
			else if(rt.PtInRect(x,y))
			{
				if(!(this->sp_style & SPINBOX_STYLE_BUTTON2_CHECKED))
				{
					pwnd->SendMessage(WM_COMMAND,1,this->id);
					this->sp_style |= SPINBOX_STYLE_BUTTON2_CHECKED;
					this->Redraw();
				}
			}
		break;

		case WM_LBUTTONUP: 
			this->UnCheckAll();
		break;

		case WM_MOUSEMOVE:		
			x = (short)LOWORD(lparam);	
			y = (short)HIWORD(lparam);
			if(!this->rect->PtInRect(x,y))
				this->UnCheckAll();
		break;

		case WM_MOUSEOUT:
			this->UnCheckAll();
		break;
	}
	return ERROR;
}

int CSpinBox::GetRect1(CRect *r)
{
	ASSERT(r);

	r->Copy(this->rect);
	r->bottom = r->top + rect->GetHeight() / 2 -1;

	return OK;
}

int CSpinBox::GetRect2(CRect *r)
{
	ASSERT(r);

	r->Copy(this->rect);
	r->top = rect->top + rect->GetHeight() / 2 ;

	return OK;
}

int CSpinBox::UnCheckAll()
{
	this->sp_style &= (~SPINBOX_STYLE_BUTTON1_CHECKED);
	this->sp_style &= (~SPINBOX_STYLE_BUTTON2_CHECKED);

	this->Redraw();
	return OK;
}
