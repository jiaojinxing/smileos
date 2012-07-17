#include "mouse.h"
#include "mem_tool.h"
#include "graphics.h"

/*************
static char __arrow[]={
00,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
00,00,-1,-1,-1,-1,-1,-1,-1,-1,-1,
00,15,00,-1,-1,-1,-1,-1,-1,-1,-1,
00,15,15,00,-1,-1,-1,-1,-1,-1,-1,
00,15,15,15,00,-1,-1,-1,-1,-1,-1,
00,15,15,15,15,00,-1,-1,-1,-1,-1,
00,15,15,15,15,15,00,-1,-1,-1,-1,
00,15,15,15,15,15,15,00,-1,-1,-1,
00,15,15,15,15,15,15,15,00,-1,-1,
00,15,15,15,15,15,15,15,15,00,-1,
00,15,15,15,15,00,00,00,00,00,00,
00,15,15,00,15,15,00,-1,-1,-1,-1,
00,15,00,00,15,15,00,-1,-1,-1,-1,
00,00,-1,-1,00,15,15,00,-1,-1,-1,
00,-1,-1,-1,00,15,15,00,-1,-1,-1,
-1,-1,-1,-1,-1,00,15,15,00,-1,-1,
-1,-1,-1,-1,-1,00,15,15,00,-1,-1,
-1,-1,-1,-1,-1,-1,00,00,00,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1
};
************/
static char __arrow[]={
00,-1,-1,-1,-1,-1,-1,
00,00,-1,-1,-1,-1,-1,
00,15,00,-1,-1,-1,-1,
00,15,15,00,-1,-1,-1,
00,15,15,15,00,-1,-1,
00,15,15,15,15,00,-1,
00,15,15,15,00,-1,-1,
00,00,00,15,00,-1,-1,
-1,-1,-1,00,-1,-1,-1
};

CMouse::CMouse()
{
    this->InitBasic();
}
CMouse::~CMouse()
{
    this->Destroy();
}
int CMouse::InitBasic()
{
	this->x = 0;
	this->y = 0;
	this->w = 0;
	this->h = 0;
	this->old_x = 0;
	this->old_y = 0;
	this->min_x = 0;
	this->max_x = GAL_GetWidth()-1;;
	this->min_y = 0;
	this->min_y = GAL_GetHeight()-1;;
	this->button = MOUSE_BUTTON_NONE;
	this->old_button = MOUSE_BUTTON_NONE;
	this->back_buf = 0;
	this->hover = 0;
	this->hold = 0;
	this->i_img = NULL;
	this->is_hide = 1;
	this->force_hide = 0;
	this->img_x = 0;
	this->img_y = 0;
	this->old_time = 0;
	this->time_out = 1000;
	this->hold_start = FALSE;

    return OK;
}
int CMouse::Init()
{
    this->InitBasic();

	//by default
	this->SetImg((char*)__arrow,7,9);

    return OK;
}
int CMouse::Destroy()
{
	FREE(this->back_buf);

    this->InitBasic();
    return OK;
}
int CMouse::Copy(CMouse *p)
{
    if(this == p)
        return OK;
    return OK;
}
int CMouse::Comp(CMouse *p)
{
    return 0;
}
int CMouse::Print()
{
    return TRUE;
}

int CMouse::SetImg(char *buf, int width, int height)
{
	this->i_img = buf;

	this->w = width;
	this->h = height;

	MALLOC(this->back_buf,pixel_type,width * height);

	return OK;
}

int CMouse::SetPos(int x, int y)
{
    this->old_x = this->x;
    this->old_y = this->y;

    this->x = x;
    this->y = y;

	return OK;
}

int CMouse::Draw()
{
   	int i,j,k = 0;

	for(j=0;j<this->h;j++)
   	{
   		for(i=0;i<this->w;i++)
      	{
			if(this->i_img[k] != (char)-1)
        	{
            	if(this->i_img[k] == (char)15)
			      set_pixel(this->x+i,this->y+j,RGB(31,31,31));
			   	else
			      set_pixel(this->x+i,this->y+j,RGB(0,0,0));
			}			

			k++;
      	}
   }
   return OK;
}

int CMouse::SaveImg()
{
	if(this->is_hide || this->force_hide)
		return ERROR;
	
	this->img_x = this->x;
	this->img_y = this->y;

	get_image(this->x,this->y,this->w,this->h,this->back_buf);  

	return OK;
}

int CMouse::Show()
{
	if( !this->is_hide || this->force_hide )
		return ERROR;
	this->is_hide = 0;

	this->SaveImg();
	this->Draw();

	return OK;
}

int CMouse::Hide()
{
	if(this->is_hide || this->force_hide)
		return ERROR;

	this->is_hide = 1;

	put_image(img_x,img_y,w,h,this->back_buf);

	return OK;
}

int CMouse::ReadMouse()
{
/************************************** jiaojinxing1987@gmail.com ***************************************/
#if _NDS_
	touchPosition touch;

	scanKeys();

	touch = touchReadXY();
	int held = keysHeld();

	this->old_button = this->button;

	if(held & 0x00001000 )
		this->button = MOUSE_BUTTON_LEFT;
	else
		this->button = MOUSE_BUTTON_NONE;
	
	if(touch.px == 0 && touch.py == 0)
	{
		this->old_x = this->x;
		this->old_y = this->y;
		return ERROR;
	}

	this->SetPos(touch.px,touch.py);
	return OK;
#elif _SMILEOS_
    /*
     * TODO: SmileOS ´¥ÃþÆÁÏà¹Ø´úÂë
     */
    return OK;
#else
    return OK;
#endif
/************************************************* end **************************************************/
}

int CMouse::PosChanged()
{
	return (this->x != this->old_x) || (this->y != this->old_y);
}

int CMouse::ButtonChanged()
{
	return this->old_button != this->button;
}

int CMouse::GetRect(CRect *pr)
{
	ASSERT(pr);
	pr->Set(x,y,x + w -1,y + h -1);
	return OK;
}

int CMouse::Refresh()
{
	if(this->PosChanged())
	{
		this->Hide();
		this->Show();
	}

	return OK;
}

int CMouse::SetForceHide(int flag)
{
	if(flag)
	{
		this->Hide();
		this->force_hide = TRUE;
	}
	else
	{
		this->force_hide = FALSE;
		this->Show();
	}

	return OK;
}

int CMouse::PosChangeMuch()
{
	if(abs(old_x - x) > 3 || abs(old_y - y) > 3)
		return TRUE;
	return FALSE;
}
