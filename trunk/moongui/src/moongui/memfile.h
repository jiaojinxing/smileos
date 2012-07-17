#ifndef __CMEMFILE_H
#define __CMEMFILE_H

#include "common.h"
#include "syslog.h"
#include "filebase.h"
#include "mem_tool.h"

class CMemFile:public CFileBase{    
public:
	long size;
	long pages;
    long offset;
    int max_page;  
    char **base;
	long pagesize;
	DWORD mod_n;
	DWORD shift_n;
public:
	long GetMaxSize();
	int InitShift();
	long GetSize();
	long GetOffset();
	int SetSize(long s);
	CMemFile();
	~CMemFile();
	int Init(long page_size,long max_page);
	int Init();
	int Destroy();
	int Close();
	int SetMaxPages(long mp);
	int AddPage();
	int Open(long s);
	int AddBlock(long block_size);
	long Read(void *buf,long rsize);
	long Write(void *buf,long wsize);
	int Print();
	int Seek(long off);
};

#endif
