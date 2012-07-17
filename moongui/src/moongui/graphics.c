#include "graphics.h"

int init_graph()
{
/************************************** jiaojinxing1987@gmail.com ***************************************/
#if _NDS_

	videoSetMode(MODE_FB0);	//not using the main screen
	videoSetModeSub(MODE_5_2D | DISPLAY_BG0_ACTIVE | DISPLAY_BG3_ACTIVE);//sub bg 0 will be used to print text
	vramSetBankC(VRAM_C_SUB_BG); 
	vramSetBankA(VRAM_A_LCD);
	lcdMainOnBottom();
	SUB_BG0_CR = BG_MAP_BASE(2);	
	BG_PALETTE_SUB[255] = RGB(31,0,31);	//by default font will be rendered with color 255	
	consoleInitDefault((pixel_type*)SCREEN_BASE_BLOCK_SUB(2), (pixel_type*)CHAR_BASE_BLOCK_SUB(0), 16);	
	
#elif _SMILEOS_
    /*
     * TODO: SmileOS ÆÁÄ»Ïà¹Ø´úÂë
     */
#else

#endif
/************************************************* end **************************************************/
	return OK;
}
void vline(int x,int y,int h,pixel_type color)
{
	int i;

   	for(i = 0; i < h; i++)
	{
		set_pixel(x,y+i,color);
	}   
}

void hline(int x,int y,int w,pixel_type color)
{
   	int i;
   	pixel_type *p = map_vram(x, y);
	for(i = 0; i < w; i++)
	{
		p[i] = color;
	}      
}   
void vline_xor(int x,int y,int h)
{
	int i;

   	for(i = 0; i < h; i++)
	{
		set_pixel_xor(x, y+i);
	}   
}

void hline_xor(int x,int y,int w)
{
   	int i;

	for(i = 0; i < w; i++)
	{
		set_pixel_xor(x + i, y);
	}      
}   
void fill_rect(int x,int y,int w,int h,pixel_type color)
{
   int i;

   for( i = 0 ;i < h ; i++)
   {
       hline(x,y+i,w,color);
   }   
}   

void draw_rect(int left,int top,int width,int height,pixel_type color)
{
	hline(left,top,width,color);
	hline(left,top+height-1,width,color);
	vline(left,top,height,color);
	vline(left+width-1,top,height,color);
}
void get_line(int x,int y,int w,void *buf)
{
    int i;
	pixel_type *p = (pixel_type*)buf;
	pixel_type *vp = map_vram(x,y);	
	
	for(i=0; i<w; i++)
		p[i] = vp[i];		
}
void put_line(int x,int y,int w,void *buf)
{
    int i;
	pixel_type *p = (pixel_type*)buf;
	pixel_type *vp = map_vram(x,y);	
	
	for(i = 0; i < w; i++)
		vp[i] = p[i];			
}
int get_image(int x,int y,int w,int h,void *buf)
{
	int j;
	pixel_type *p = (pixel_type*)buf;
	
	for(j = 0; j < h; j++)
	{
		get_line(x,j+y,w,p+j*w);
	}
	return OK;
}
int put_image(int x,int y,int w,int h,void *buf)
{
	int j;
	pixel_type *p = (pixel_type*)buf;
	
	for(j = 0; j < h; j++)
	{
		put_line(x,j+y,w,p+j*w);
	}
	
	return OK;
}
int image_size(int w,int h)
{
	return w * h * sizeof(pixel_type);
}

int draw_mono_image(  int rcl,int rct,int rcr,int rcb,
                      int x,int y,int width,int height,char *buf,
                      pixel_type fore_color)
{
	int i,j,pointer;
	int tx,ty,tr,tb;
 	pixel_type *p;
 	
	tx = (rcl > x) ? rcl : x;
	ty = (rct > y) ? rct : y;
	tr = (rcr < (x+width-1)) ? rcr : (x+width-1);
	tb = (rcb < (y+height-1))? rcb : (y+height-1);

	if(tx > tr || ty > tb) return FALSE;
    
	for(j = ty; j <= tb; j++)
 	{
 		p = map_vram(tx,j);
	 	for(i = 0; i <= tr-tx; i++)  	
  		{
			pointer = (j - y) * width + (i-x+tx);
			if((buf[pointer>>3]>>(7 - (pointer&7)))&0x01)
				p[i] = fore_color;
		}
   	}
 	return OK;
}

