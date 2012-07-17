#include "gui_misc.h"
#include "globals.h"

int Draw3dThinUpBorder(CDc *pdc,CRect *r)
{
	pixel_type c;
	c=GetSysColor(COLOR_3DHILIGHT);
	pdc->SetPenColor(c);
	pdc->HLine(r->left,r->top,r->GetWidth());
	pdc->VLine(r->left,r->top,r->GetHeight());

	c=GetSysColor(COLOR_3DSHADOW);
	pdc->SetPenColor(c);

	pdc->HLine(r->left,r->bottom,r->GetWidth());
	pdc->VLine(r->right,r->top,r->GetHeight());

	return OK;
}


int Draw3dThinUpFrame(CDc *pdc,CRect *r)
{
	pixel_type c;

	Draw3dThinUpBorder(pdc,r);

	c=GetSysColor(COLOR_3DLIGHT);
	pdc->SetBrushColor(c);

	pdc->FillRect(r->left+1,r->top+1,r->GetWidth()-2,r->GetHeight()-2);
	
	return OK;
}

int Draw3dThinDownBorder(CDc *pdc,CRect *r)
{
	pixel_type c;
	c=GetSysColor(COLOR_3DSHADOW);
	pdc->SetPenColor(c);
	pdc->HLine(r->left,r->top,r->GetWidth());
	pdc->VLine(r->left,r->top,r->GetHeight());

	c=GetSysColor(COLOR_3DHILIGHT);
	pdc->SetPenColor(c);

	pdc->HLine(r->left,r->bottom,r->GetWidth());
	pdc->VLine(r->right,r->top,r->GetHeight());

	return OK;
}

int Draw3dThinDownFrame(CDc *pdc,CRect *r)
{
	pixel_type c;

	Draw3dThinDownBorder(pdc,r);

	c=GetSysColor(COLOR_3DLIGHT);
	pdc->SetBrushColor(c);
	pdc->FillRect(r->left+1,r->top+1,r->GetWidth()-2,r->GetHeight()-2);

	return OK;
}


int Draw3DHLine(CDc *pdc,int x,int y,int w)
{
	pixel_type c;

	c=GetSysColor(COLOR_3DSHADOW);
	pdc->SetPenColor(c);

	pdc->HLine(x,y,w);
	c=GetSysColor(COLOR_3DHILIGHT);
	pdc->SetPenColor(c);
	pdc->HLine(x,y+1,w);

	return OK;
}

int Draw3DVLine(CDc *pdc,int x,int y,int h)
{
	pixel_type c;

	c=GetSysColor(COLOR_3DSHADOW);
	pdc->SetPenColor(c);

	pdc->VLine(x,y,h);
	c=GetSysColor(COLOR_3DHILIGHT);
	pdc->SetPenColor(c);
	pdc->VLine(x+1,y,h);

	return OK;
}

int Draw3DLineRect(CDc *pdc,CRect *r)
{
	pixel_type c,c1;
	int x,y,w,h;

	x = r->left;y = r->top;
	w = r->GetWidth(); h = r->GetHeight();

	c=GetSysColor(COLOR_3DSHADOW);
	c1=GetSysColor(COLOR_3DHILIGHT);
	
	pdc->SetPenColor(c);
	pdc->HLine(x,y,w);
	pdc->VLine(x,y,h);
	pdc->SetPenColor(c1);
	pdc->HLine(x+1,y+1,w-1);
	pdc->VLine(x+1,y+1,h-1);
	pdc->SetPenColor(c);
	pdc->HLine(x+1,y+h-1,w-1);
	pdc->VLine(x+w-1,y+1,h-1);
	pdc->SetPenColor(c1);
	pdc->HLine(x,y+h,w+1);
	pdc->VLine(x+w,y,h);

	return OK;
}

int DrawYinBiao(CDc *pdc,int x,int y,char *str_yinbiao,int bf)
{
	ASSERT(pdc && str_yinbiao);
	
	char ch;
	ImageInfo img;
	int index;
	
	while(ch = *str_yinbiao++)
	{
  		if( (ch >= 'a' && ch <= 'z') || (ch >= 'a' && ch <='z'))
   		{
			img.addr = (int)(afx.mem_asc->p + ch * 12);
			img.width = 8;
			img.height = 12;
			pdc->DrawMonoImage(x,y,&img,pdc->pencolor);
    	}
		else
		{
			switch(ch)
			{
				case '0': index = IMAGE_YINBIAO_0; break;
				case '1': index = IMAGE_YINBIAO_1; break;
				case '2': index = IMAGE_YINBIAO_2; break;
				case '3': index = IMAGE_YINBIAO_3; break;
				case '4': index = IMAGE_YINBIAO_4; break;
				case '5': index = IMAGE_YINBIAO_5; break;
				case '6': index = IMAGE_YINBIAO_6; break;
				case '7': index = IMAGE_YINBIAO_7; break;
				case '8': index = IMAGE_YINBIAO_8; break;
				case '9': index = IMAGE_YINBIAO_9; break;
				case '=': index = IMAGE_YINBIAO_10; break;					
				case ';': index = IMAGE_YINBIAO_12; break;
				case 39: index = IMAGE_YINBIAO_11; break;					
			}
			
			pdc->DrawMonoImage(x,y + 2,GetStockImage(index),pdc->pencolor);			
		}

		x += 6;
	}

	return OK;
}