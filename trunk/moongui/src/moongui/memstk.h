// MemStk.h: interface for the CMemStk class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MEMSTK_H__C52A5CBF_0474_4D45_8034_A015FDCFF513__INCLUDED_)
#define AFX_MEMSTK_H__C52A5CBF_0474_4D45_8034_A015FDCFF513__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "mem.h"

class CMemStk{
public:
    CMem **index;
    long top;
	long size;
	BOOL case_sensitive;
public:
	int GetPath(CFileBase *file);
	CMem * Search(char *str);
	int SetCaseSensetive(BOOL c);
	int CompMem(CMem *mem1,CMem *mem2);
	int Push(char *str);
	long BSearch_Pos(CMem *node,int order,int *find_flag);
	int InsOrdered(CMem *str,int order,int unique);
	int DelElem(long i);
	int InsertElem(long i,CMem *node);
	long BSearch(CMem *node,int order);
	CMem * BSearch_Node(CMem *node,int order);
	int Sort(int order);
	CMem * GetElem(long index);
	CMem * GetTop();
	CMem * Search(CMem *node);
	CMem *PushEmpty();
	CMemStk();
	int Clear();
	int DelTop();
	int Destroy();
	int Init(long init_size);
	int IsEmpty();
	int IsFull();
	int Pop(CMem *node);
	int Print();
	int Push(CMem *node);
	long GetLen();
	~CMemStk();
};


#endif // !defined(AFX_MEMSTK_H__C52A5CBF_0474_4D45_8034_A015FDCFF513__INCLUDED_)
