#ifndef __CFILE_H
#define __CFILE_H

#include "common.h"
#include "filebase.h"

//don't use buffer when open a file for write
class CFile:public CFileBase{
public:
    FILE *fp;
	char *file_buf;
	int buf_size,buf_start,init_buf_size;
	int offset,size;
public:
	int InitBasic();
	int BufferBlock(int start_pos);
	int SetBufSize(int size);
	int HasBuffer();
	long GetMaxSize();
	
         CFile();
         ~CFile();
    int Init();
    int Destroy();
    int OpenFile(char *fn,char *mode); 
    int CloseFile();   
	long GetSize();
    long GetSizeReal();
    long GetOffset();
    int Seek(long nOffset);
    long Read(void *buf,long nByteToRead);      
    long Write(void *buf,long nByteToWrite);
    int SetSize(long nSize);
    static BOOL FindFile(char *fn);
	int AddBlock(long bsize);
};

#endif
