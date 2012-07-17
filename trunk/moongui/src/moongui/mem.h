#ifndef __CMEM_H
#define __CMEM_H

#include "common.h"
#include "syslog.h"
#include "filebase.h"
#include "mem_tool.h"

#define LOCAL_MEM(name) char __##name[LBUF_SIZE];\
name.Init();\
name.SetP(__##name,LBUF_SIZE);\
name.SetSize(0)\

#define LOCAL_MEM_WITH_SIZE(name,size) char __##name[size];\
name.Init();\
name.SetP(__##name,size);\
name.SetSize(0)\

class CMem:public CFileBase{
public:
    char *p;
    long size; //size is changable
    long offset;
	BOOL self_alloc;
	long max_size; 
public:
	int Copy(CMem *mem);
	long GetMaxSize();
	int SetStr(char *str);
	int SizeToMax();
	int AddBlock(long bsize);
	long GetSize();
	long GetOffset();
	CMem();
	~CMem();
	int Destroy();
	long Malloc(long asize);
	long Read(void *buf,long n);
	int Zero();	
	int Init();
	int Print();
	int SetP(char *p,long s);
	int Seek(long off);
	int SetSize(long ssize);
	long Write(void *buf,long n);

};

#endif
