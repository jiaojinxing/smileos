// Rect.h: interface for the CRect class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RECT_H__CA7AABD2_1D15_40F6_8D72_DB2611734BA3__INCLUDED_)
#define AFX_RECT_H__CA7AABD2_1D15_40F6_8D72_DB2611734BA3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "gal.h"
#include "assert.h"

class CRect{
public:
	int left,top;
	int right,bottom;
public:
	int Set_1(int left,int top,int width,int height);
	static int InitRectPool();
	static int FreeRect(CRect *r);
	static CRect * AllocRect();
	int CenterRect(int w,int h,CRect *rc);
	int Offset(int x,int y);
	int Draw(pixel_type color);
	int GetHeight();
	int GetWidth();
	int PtInRect(int x,int y);
	int Subtract(CRect* psrc1, CRect* rc);
	int GetBound( CRect *psrc1, CRect *psrc2);
	int Set(int l,int t,int r,int b);
	int Union(CRect* psrc1, CRect* psrc2);
	int IsIntersect( CRect* psrc2);
	int Intersect(CRect *psrc1,CRect *psrc2);
	int CoveredBy(CRect *pRect);
	int Normalize();
	int Equal(CRect *p);
	int IsEmpty();
	int Empty();
    CRect();
    virtual ~CRect();
    int Init();
    int Destroy();
	int Comp(CRect *p);
    int Copy(CRect *p);
    int Print();
    int InitBasic();
};

#endif // !defined(AFX_RECT_H__CA7AABD2_1D15_40F6_8D72_DB2611734BA3__INCLUDED_)
