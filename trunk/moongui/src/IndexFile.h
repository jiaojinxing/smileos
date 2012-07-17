// IndexFile.h: interface for the CIndexFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INDEXFILE_H__BD90BEDD_1732_4D63_8A5C_FF6430E6DF8A__INCLUDED_)
#define AFX_INDEXFILE_H__BD90BEDD_1732_4D63_8A5C_FF6430E6DF8A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "memfile.h"

class CIndexFile{
public:
	int header_size;
	int index_size;
	CFileBase *i_file;
	int *call_back_param;
	//每次读一段数据到file_out中，返回的是写入数据的大小
	int (*call_back_read_block)(int *param,CFileBase *file_out);
public:
	int GetHeader(CFileBase *file_header);
	int GetBlockSize(int index);
	int GetBlock(int i,CFileBase *file);
	int GetBlockOff(int i);
	int GetTotalBlocks();
	int LoadIndexFile(CFileBase *file);
	int CreateIndexFile(CFileBase *file);
	static int call_back_read_block_def(int *param,CFileBase *file);
    CIndexFile();
    virtual ~CIndexFile();
    int Init();
    int Destroy();
    int InitBasic();
};

#endif // !defined(AFX_INDEXFILE_H__BD90BEDD_1732_4D63_8A5C_FF6430E6DF8A__INCLUDED_)
