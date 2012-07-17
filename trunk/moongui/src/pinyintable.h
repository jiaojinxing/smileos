// PinYinTable.h: interface for the CPinYinTable class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PINYINTABLE_H__B36C712E_2FA9_4B7E_B30C_1E2DB915BF07__INCLUDED_)
#define AFX_PINYINTABLE_H__B36C712E_2FA9_4B7E_B30C_1E2DB915BF07__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "indexfile.h"

class CPinYinTable{
public:
	CIndexFile *index_file;
public:
	int GetMax();
	int Search(char *py);
	int BSearch_Pos(char *py,int*find_flag);
	int GetPinYin(CFileBase *py_line,CFileBase *file);
	int GetItem(int i,CFileBase *file);
	int LoadIndexFile(CFileBase *file);
    CPinYinTable();
    virtual ~CPinYinTable();
    int Init();
    int Destroy();
    int InitBasic();
};

#endif // !defined(AFX_PINYINTABLE_H__B36C712E_2FA9_4B7E_B30C_1E2DB915BF07__INCLUDED_)
