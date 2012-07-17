#ifndef __GRAPHICS_H
#define __GRAPHICS_H

#include "common.h"

/************************************** jiaojinxing1987@gmail.com ***************************************/
#if _NDS_

#define SCREEN_WIDTH		256
#define SCREEN_HEIGHT		192
#define RGB					RGB15
#define set_pixel(x,y,c) 	VRAM_A[((y)<<8) + (x)] = (c)
#define get_pixel(x,y)		VRAM_A[((y)<<8) + (x)]
#define set_pixel_xor(x,y)	VRAM_A[((y)<<8) + (x)] = RGB(31,31,31) - VRAM_A[((y)<<8) + (x)]
#define map_vram(x,y)		(VRAM_A + ((y)<<8) + (x))

typedef	u16 pixel_type;

#elif _SMILEOS_

extern unsigned int screen_width;
extern unsigned int screen_height;

#define SCREEN_WIDTH		screen_width
#define SCREEN_HEIGHT		screen_height

typedef u16 pixel_type;

#define RGB(r, g, b)        (((r >> 3) << 11) | ((g >> 2 << 5)) | ((b >> 3) << 0))

void set_pixel(unsigned int x, unsigned int y, pixel_type c);
void set_pixel_xor(unsigned int x, unsigned int y);
pixel_type get_pixel(unsigned int x, unsigned int y);
pixel_type *map_vram(unsigned int x, unsigned int y);

#endif
/************************************************* end **************************************************/

#ifdef __cplusplus
extern "C" {
#endif

int init_graph();
void vline(int x,int y,int h,pixel_type color);
void hline(int x,int y,int w,pixel_type color);
void vline_xor(int x,int y,int h);
void hline_xor(int x,int y,int w);
void fill_rect(int x,int y,int w,int h,pixel_type color);
void draw_rect(int left,int top,int width,int height,pixel_type color);
void get_line(int x,int y,int w,void *buf);
void put_line(int x,int y,int w,void *buf);
int get_image(int x,int y,int w,int h,void *buf);
int put_image(int x,int y,int w,int h,void *buf);
int image_size(int w,int h);

int draw_mono_image(  int rcl,int rct,int rcr,int rcb,
                      int x,int y,int height,int width,char *buf,
                      pixel_type fore_color);

#ifdef __cplusplus
} //extern "C"
#endif

#endif

