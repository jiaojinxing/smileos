#include "stdafx.h"
#include "assert.h"
#include "syslog.h"
#include "file.h"
#include "mem_tool.h"

CFile::CFile()
{
	this->InitBasic();
}
CFile::~CFile()
{
    this->Destroy();
}

int CFile::InitBasic()
{
    this->fp = NULL;
	this->buf_size = 0;
	this->buf_start = -1;
	this->file_buf = NULL;
	this->init_buf_size = 0;
	this->offset = 0;
	this->size = 0;

	return OK;
}

int CFile::Init()
{
	this->InitBasic();
	return OK;
}
int CFile::Destroy()
{
	this->CloseFile();

	return OK;
}
int CFile::OpenFile(char *fn , char *mode)
{
	ASSERT(this->fp == NULL);
	
	fp = fopen(fn,mode);
	
	if(fp == NULL) 
		return ERROR;
	
	this->GetSizeReal();
	this->BufferBlock(0);
	
	return OK;
}

int CFile::CloseFile()
{	
	if(this->fp)
	{
		fclose(this->fp);
		this->fp = NULL;
	}

	FREE(this->file_buf);
	
	this->InitBasic();
	
	return OK;
}
long CFile::GetSize()
{
	return this->size;
}
long CFile::GetSizeReal()
{
	long old_off,size;
	
	ASSERT(this->fp);

	old_off = ftell(this->fp);
	fseek(this->fp,0,SEEK_END);
	size = ftell(this->fp);
	fseek(this->fp,old_off,SEEK_SET);

	this->size = size;
	
	return size;	
}

int CFile::Seek(long nOffset)
{
	int ret;
	
	ASSERT(this->fp);

	if(nOffset < 0)
	{
		ret = fseek(this->fp,0,SEEK_END);
		this->offset = ftell(this->fp);
	}
	else
	{
		this->offset = nOffset;
	}

	return this->offset;
}

long CFile::Read(void *buf,long nByteToRead)
{
	long old_off,off;
	
	ASSERT(this->fp);

	old_off = this->GetOffset();    

	if(this->HasBuffer())
	{
		if(old_off >= this->buf_start && (old_off + nByteToRead <= this->buf_start + this->buf_size))
		{
			memcpy(buf,this->file_buf+(old_off-this->buf_start),nByteToRead);
			this->Seek(old_off + nByteToRead);			
		}
		else
		{			
			fseek(this->fp,this->offset,SEEK_SET);
			fread((void*)buf,nByteToRead , 1 ,this->fp);
			this->offset = ftell(this->fp);

			if(old_off < this->buf_start)
				this->BufferBlock(old_off - this->init_buf_size + nByteToRead);
			else
				this->BufferBlock(old_off);
		}    
	}
	else
	{
		fseek(this->fp,this->offset,SEEK_SET);
		fread((void*)buf,nByteToRead , 1 ,this->fp);
		this->offset = ftell(this->fp);
	}

	off = this->GetOffset();
	return off - old_off;
}

long CFile::Write(void *buf,long nByteToWrite)
{
	long old_off;
	
	ASSERT(this->fp);

	old_off = this->GetOffset();
	
	fseek(this->fp,this->offset,SEEK_SET);
    fwrite((void*)buf,nByteToWrite , 1 ,this->fp);
	this->offset = ftell(this->fp);
      
	return this->offset - old_off;
}

int CFile::SetSize(long nSize)
{
	this->AdjustOffset();
	return OK;
}

BOOL CFile::FindFile(char *fn)
{
    FILE *fp;
    fp=fopen(fn,"rb");
    if(fp==NULL)
        return false;
    fclose(fp);
    return true;
}

long CFile::GetOffset()
{
	return this->offset;
}
int CFile::AddBlock(long bsize)
{
	return OK;
}

long CFile::GetMaxSize()
{
	return this->GetSize();
}

int CFile::HasBuffer()
{
	return this->file_buf != NULL;
}

int CFile::SetBufSize(int size)
{
	FREE(this->file_buf);

	MALLOC(this->file_buf,char,size);
	this->buf_size = size;
	this->buf_start = -1;
	this->init_buf_size = size;

	return OK;
}

int CFile::BufferBlock(int start_pos)
{
	ASSERT(this->fp);
	
	if(!this->HasBuffer())
		return ERROR;

	if(start_pos < 0)
		start_pos = 0;

	if(this->buf_start == start_pos)
		return OK;
	
	int off;

	fseek(fp,start_pos,SEEK_SET);
	fread(this->file_buf,this->init_buf_size,1,this->fp);
	off = ftell(fp);

	this->buf_start = start_pos;
	this->buf_size = off - start_pos;
	
	return OK;
}
