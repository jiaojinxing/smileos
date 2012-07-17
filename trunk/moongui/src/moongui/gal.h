#ifndef __GAL_H
#define __GAL_H

#include "graphics.h"

#define GAL_SetPixel  	set_pixel
#define GAL_GetPixel	get_pixel
#define GAL_GetImage	get_image
#define GAL_PutImage	put_image

#ifdef __cplusplus
extern "C" {
#endif
	
void GAL_InitGraph();
void GAL_CloseGraph();
int GAL_GetWidth();
int GAL_GetHeight();
void GAL_SetClipping(int x1,int y1,int x2,int y2);
int GAL_GetClippingRect(int *dx1,int *dy1,int *dx2,int *dy2);
int GAL_HLine(int x,int y,int width,pixel_type color);
int GAL_HLine_XOR(int x,int y,int width);
int GAL_VLine(int x,int y,int h,pixel_type color);
int GAL_VLine_XOR(int x,int y,int h);
int GAL_DrawRect_XOR(int x,int y,int width,int height);
int GAL_FillRect(int x,int y,int width,int height,pixel_type color);
int GAL_DrawRect(int left,int top,int width,int height,pixel_type color);
int GAL_DrawMonoImage(int x,int y,int width,int height,char *buf,pixel_type fc);

#ifdef __cplusplus
} //extern "C"
#endif

#endif
