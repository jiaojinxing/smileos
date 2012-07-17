#include "gal.h"

static int gal_clip_x1, gal_clip_y1, gal_clip_x2, gal_clip_y2;

void GAL_InitGraph()
{
	init_graph();
	GAL_SetClipping(0,0,GAL_GetWidth(),GAL_GetHeight());
}

void GAL_CloseGraph()
{
}

int GAL_GetWidth()
{
	return SCREEN_WIDTH;
}

int GAL_GetHeight()
{
	return SCREEN_HEIGHT;
}

void GAL_SetClipping(int x1,int y1,int x2,int y2)
{
	gal_clip_x1 = x1;
	gal_clip_y1 = y1;
	gal_clip_x2 = x2;
	gal_clip_y2 = y2;

	if(gal_clip_x1 < 0) gal_clip_x1 = 0;
	if(gal_clip_y1 < 0) gal_clip_y1 = 0;
	if(gal_clip_x2 < 0) gal_clip_x2 = 0;
	if(gal_clip_y2 < 0) gal_clip_y2 = 0;
	
	if(gal_clip_x2 >= GAL_GetWidth())
		gal_clip_x2 = GAL_GetWidth() - 1;	
	if(gal_clip_y2 >= GAL_GetHeight())
		gal_clip_y2 = GAL_GetHeight() - 1;	
}
int GAL_GetClippingRect(int *dx1,int *dy1,int *dx2,int *dy2)
{
	*dx1 = ((*dx1) > gal_clip_x1) ? (*dx1) : gal_clip_x1;
	*dy1 = ((*dy1) > gal_clip_y1) ? (*dy1) : gal_clip_y1;
	*dx2 = ((*dx2) < gal_clip_x2) ? (*dx2) : gal_clip_x2;
	*dy2 = ((*dy2) < gal_clip_y2) ? (*dy2) : gal_clip_y2;

	if ( *dx1 > *dx2 || *dy1 > *dy2 )
	{
		return FALSE;
	}

	return TRUE;
}

int GAL_HLine(int x,int y,int width,pixel_type color)
{
	int x2 = x+width-1;
	int y2 = y;
	
	if( GAL_GetClippingRect(&x,&y,&x2,&y2) )
		hline(x,y,x2-x+1,color);
	
	return OK;
}

int GAL_HLine_XOR(int x,int y,int width)
{
	int x2 = x+width-1;
	int y2 = y;
	
	if( GAL_GetClippingRect(&x,&y,&x2,&y2) )
		hline_xor(x, y, x2-x+1);
	
	return OK;
}
int GAL_VLine(int x,int y,int h,pixel_type color)
{
	int x2 = x;
	int y2 = y + h - 1;
	
	if(GAL_GetClippingRect(&x,&y,&x2,&y2))
		vline(x,y,y2-y+1,color);

	return OK;
}
int GAL_VLine_XOR(int x,int y,int h)
{
	int x2 = x;
	int y2 = y + h -1;
	
	if(GAL_GetClippingRect(&x,&y,&x2,&y2))
		vline_xor(x,y,y2-y+1);

	return OK;
}

int GAL_DrawRect_XOR(int x,int y,int width,int height)
{
	GAL_HLine_XOR(x, y, width);
	GAL_HLine_XOR(x, y+height-1, width);
	GAL_VLine_XOR(x, y + 1,height - 2);
	GAL_VLine_XOR(x+width-1, y+1, height-2);

	return OK;
}

int GAL_FillRect(int x,int y,int width,int height,pixel_type color)
{
	
	int x1=x, y1=y, x2=x+width-1, y2=y+height-1;
	
	if( GAL_GetClippingRect(&x1,&y1,&x2,&y2) )
		fill_rect(x1,y1,x2-x1+1,y2-y1+1,color);
	
	return OK;
}

int GAL_DrawRect(int left,int top,int width,int height,pixel_type color)
{
	GAL_HLine(left,top,width,color);
	GAL_HLine(left,top+height-1,width,color);
	GAL_VLine(left,top,height,color);
	GAL_VLine(left+width-1,top,height,color);

	return OK;
}

int GAL_DrawMonoImage(int x,int y,int width,int height,char *buf,pixel_type fc)
{
	draw_mono_image(gal_clip_x1,gal_clip_y1,gal_clip_x2, gal_clip_y2,
					x,y,width,height,(char*) buf, fc);
	return OK;
}
