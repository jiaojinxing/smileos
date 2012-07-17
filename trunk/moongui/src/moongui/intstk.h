// IntStk.h: interface for the CIntStk class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INTSTK_H__598F8BDB_F72F_4BAD_A1AC_EB6F522CF902__INCLUDED_)
#define AFX_INTSTK_H__598F8BDB_F72F_4BAD_A1AC_EB6F522CF902__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "common.h"

#define INTSTK_DEL(stk_ptr,elem_type) \
if(stk_ptr){\
int __i;\
for(__i = 0; __i < (stk_ptr)->GetLen(); __i++)\
{\
elem_type *__p;\
__p = (elem_type *)((stk_ptr)->GetElem(__i));\
DEL(__p);\
(stk_ptr)->SetElem(__i,0);\
}}\


class CIntStk{
public:
	int *nums;
	int top;
	int size;
public:
	int SetElem(int index,int val);
	int GetTop();
	int Search_Pos(int n);
	int InsOrdered(int n,int order,int unique);
	int DelElem(int index);
	int InsertElem(int index,int n);
	int BSearch_Pos(int n,int order,int *find_flag);
	int Clear();
	int GetLen();
	int GetElem(int index);
	int Pop();
	int Push(int n);
	int IsEmpty();
	int IsFull();
    CIntStk();
    ~CIntStk();
    int Init(int init_size);
    int Destroy();
    int Print();
    int InitBasic();
};

#endif // !defined(AFX_INTSTK_H__598F8BDB_F72F_4BAD_A1AC_EB6F522CF902__INCLUDED_)
