#ifndef __DC_H
#define __DC_H

#include "cliprgn.h"
#include "gui_types.h"
#include "gal.h"
#include "font.h"
#include "windef.h"
#include "imagemap.h"

class CDc{
public:
	pixel_type bkcolor;
    pixel_type brushcolor;
    pixel_type pencolor;
    pixel_type textcolor;
	CClipRgn *clip_rgn;
	POINT pos;
	CFont *i_font;
public:
	int DrawMonoImage(int x, int y, ImageInfo *img,pixel_type fc,CRect *rclip);
	int DrawRect(CRect *rect);
	int FillRect(CRect *rect);
	int DrawMonoImage(int x,int y,ImageInfo *img,pixel_type fc);
	int PtVisible(int x,int y);
	int GetBound(CRect *r);
	int Intersect(CRect *r);
	int TextOut(int x,int y,char *str,CRect *rc);
	int SubtractClipRect(CRect *r);
	int AddClipRect(CRect *r);
	int SR2LR(CRect *r);
	int LR2SR(CRect *r);
	int DrawText(char *str, CRect *rect, u32 dwDTFormat);
	int SetFont(CFont *font);
	int SetBkColor(pixel_type bc);
	int SetTextColor(pixel_type tc);
	int SetBrushColor(pixel_type bc);
	int SetPenColor(pixel_type pc);
	int FillRect(int x,int y,int w,int h);
	int DrawRect(int x,int y,int w,int h);
	int VLine(int x,int y,int h);
	int HLine(int x,int y,int w);
	int SetPixel(int x,int y,pixel_type color);
	int SP2LP(int *x,int *y);
	int LP2SP(int *x,int *y);
	int SetPos(int x,int y);
    CDc();
    virtual ~CDc();
    int Init();
    int Destroy();
    int Copy(CDc *p);
    int Comp(CDc *p);
    int Print();
    int InitBasic();
};

#endif
