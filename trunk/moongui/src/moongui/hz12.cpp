#include "hz12.h"

#define HZ_WIDTH 	13
#define HZ_HEIGHT	12
#define ASC_WIDTH	6

CHz12::CHz12()
{
    this->InitBasic();
}
CHz12::~CHz12()
{
    this->Destroy();
}
int CHz12::InitBasic()
{
    CFont::InitBasic();

	this->i_buf_asc = NULL;
	this->i_buf_hzk12 = NULL;
	this->fore_color = RGB(0,0,0);

    return OK;
}
int CHz12::Init()
{
    this->InitBasic();
    CFont::Init();

    return OK;
}
int  CHz12::Destroy()
{
    CFont::Destroy();
    this->InitBasic();
    return OK;
}
int  CHz12::Copy(CHz12 *p)
{
    if(this == p)
        return OK;

    return OK;
}
int CHz12::Comp(CHz12 *p)
{
    return 0;
}
int  CHz12::Print()
{
    return TRUE;
}

int CHz12::DrawText(int x,int y,char *str)
{
	s32 qm,wm,offset;
	u8 *p;
	int startx;

	startx = x;
	p= (BYTE *) str;	

	while(*p)
	{
		if(*p >= 160 && *(p+1) >= 160)
		{
			qm = *p++ - 161;
			wm = *p++ - 161;
			offset = (wm + qm * 94) * 24L;
			GAL_DrawMonoImage(x, y, 16,12, this->i_buf_hzk12 + offset , this->fore_color);
			x += HZ_WIDTH;
		}
		else if(*p == '\n')
		{
			p ++;
			y += HZ_HEIGHT;
			x = startx;
		}
		else
		{
			GAL_DrawMonoImage( x,y,8,12,this->i_buf_asc + (*p) * 12,this->fore_color);
			x += ASC_WIDTH;
			p ++;
		}
	}
	return OK;
}
int CHz12::SetClipRect(int left,int top,int right,int bottom)
{
	GAL_SetClipping(left,top,right,bottom);
	return OK;
}
int CHz12::SetColor(pixel_type fc)
{
	this->fore_color = fc;
	return OK;
}
int CHz12::GetCharWidth(char *str)
{
	if((u8)(str[0]) >= 160)
		return HZ_WIDTH;
	if(str[0] != 0)
		return ASC_WIDTH;
	return 0;
}
int CHz12::GetTextSize(char *str,int *cx,int *cy)
{
	u8 *p = (u8*)str;
	int x = 0,y = HZ_HEIGHT,max = 0;

	while(*p)
	{
		if(*p>=160)
		{
			x += HZ_WIDTH;
			if(x > max) max = x;
			p++;
			if(*p == 0) break;
		}
		else if(*p == '\n')
		{
			x = 0;
			y += HZ_HEIGHT;
		}
		else 
		{
			x += ASC_WIDTH;
			if(x > max) max = x;
		}
		p++;
	}
	*cx = max;
	*cy = y;	

	return OK;
}
int CHz12::GetMinCharWidth()
{
	return ASC_WIDTH;;
}
int CHz12::GetMaxCharHeight()
{
	return HZ_WIDTH;
}

