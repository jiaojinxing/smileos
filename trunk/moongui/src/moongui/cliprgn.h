#ifndef __CLIPRGN_H
#define __CLIPRGN_H

#include "mem_tool.h"
#include "gal.h"
#include "rect.h"

class CClipRgn{
public:
	CRect *rcBound;
    CRect **index;
    long top;
    long size;
public:
	int Set(CRect *pr);
	int Intersect(CClipRgn *prgn1, CClipRgn *prgn2);
	int Draw(pixel_type color);
	int Subtract(CRect *prc);
	int IsIntersect(CRect *pr);
	int Intersect(CRect *prc);
	int AddRect(CRect *pr);
	int DelEmptyRects();
	int EvaluateBound();
	int InitBasic();
	int Copy(CClipRgn *pRgn);
    long BSearch_Pos(CRect *node,int order,int *find_flag);
    int InsOrdered(CRect *str,int order,int unique);
    int DelElem(long i);
    int InsertElem(long i,CRect *node);
    long BSearch(CRect *node,int order);
    CRect * BSearch_CRect(CRect *node,int order);
    int Sort(int order);
    CRect * GetElem(long index);
    CRect * GetTop();
    CRect * Search(CRect *node);
    long Search_Pos(CRect *node);
    CRect *PushEmpty();
    CClipRgn();
    int Clear();
    int DelTop();
    int Destroy();
    int Init(long init_size);
    int IsEmpty();
    int IsFull();
    int Pop(CRect *node);
    int Print();
    int Push(CRect *node);
    long GetLen();
    ~CClipRgn();
};

#endif
