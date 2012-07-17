#ifndef __GUI_MISC_H
#define __GUI_MISC_H

#include "dc.h"
#include "colormap.h"

int Draw3dThinUpBorder(CDc *pdc,CRect *r);
int Draw3dThinUpFrame(CDc *pdc,CRect *r);
int Draw3dThinDownBorder(CDc *pdc,CRect *r);
int Draw3dThinDownFrame(CDc *pdc,CRect *r);
int Draw3DHLine(CDc *pdc,int x,int y,int w);
int Draw3DVLine(CDc *pdc,int x,int y,int h);
int Draw3DLineRect(CDc *pdc,CRect *r);
int DrawYinBiao(CDc *pdc,int x,int y,char *str_yinbiao,int bf);

#endif

