// IndexFile.cpp: implementation of the CIndexFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "indexfile.h"
#include "mem.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIndexFile::CIndexFile()
{
    this->InitBasic();
}
CIndexFile::~CIndexFile()
{
    this->Destroy();
}
int CIndexFile::InitBasic()
{
	this->header_size = 32;
	this->index_size = 0;
	this->call_back_param = (int*)this;
	this->call_back_read_block = this->call_back_read_block_def;

    return OK;
}
int CIndexFile::Init()
{
    this->InitBasic();
    //add your code
    return OK;
}
int CIndexFile::Destroy()
{
    //add your code
    this->InitBasic();
    return OK;
}

//return the size of a block, 0 or minus value will cause create end.
int CIndexFile::call_back_read_block_def(int *param,CFileBase *file)
{
	CIndexFile *self = (CIndexFile*)param;

	ASSERT(self && self->i_file && file);
	CMem mem;

	LOCAL_MEM(mem);

	self->i_file->ReadLine(&mem);

	return file->WriteFile(&mem);
}

int CIndexFile::CreateIndexFile(CFileBase *file)
{
	ASSERT(file && this->call_back_read_block);

	CMemFile buf,mf_index;
	int t,ret ,off = 0;

	buf.Init();
	mf_index.Init();

	mf_index.FillBlock(this->header_size,0);
	file->SetSize(0);
	while(true)
	{
		ret = this->call_back_read_block(this->call_back_param,file);
		if(ret <= 0)
			break;

		mf_index.Write(&off,sizeof(int));
		off += ret;
	}

	this->index_size = mf_index.GetSize() - this->header_size;

	mf_index.Seek(0);
	t = this->header_size; 
	mf_index.Write(&t,sizeof(int));
	t = this->index_size;
	mf_index.Write(&t,sizeof(int));
	
	ASSERT(this->header_size >= sizeof(int) * 3);

	file->Seek(0);
	file->Insert(&mf_index);

	return OK;
}

int CIndexFile::LoadIndexFile(CFileBase *file)
{
	ASSERT(file);

	this->i_file = file;

	this->i_file->Seek(0);
	this->i_file->Read(&this->header_size,sizeof(int));
	this->i_file->Read(&this->index_size,sizeof(int));

	return OK;
}

int CIndexFile::GetTotalBlocks()
{
	return this->index_size / sizeof(int);
}

int CIndexFile::GetBlockOff(int i)
{
	ASSERT(this->i_file);
	
	if(i < 0) i = 0;

	if(i >= this->GetTotalBlocks()) 
	{
		return this->i_file->GetSize();
	}

	int t;

	this->i_file->Seek(this->header_size + i * sizeof(int));
	this->i_file->Read(&t,sizeof(int));

	t += this->header_size + this->index_size;

	return t;
}

int CIndexFile::GetBlock(int i, CFileBase *file)
{
	ASSERT(file && this->i_file);
	
	int s = this->GetBlockOff(i);
	int e = this->GetBlockOff(i+1);

	return this->i_file->WriteToFile(file,s,e-s);
}

int CIndexFile::GetBlockSize(int index)
{
	int s = this->GetBlockOff(index);
	int e = this->GetBlockOff(index+1);

	return e - s;
}

int CIndexFile::GetHeader(CFileBase *file_header)
{
	ASSERT(this->i_file && file_header);
	this->i_file->WriteToFile(file_header,0,this->header_size);
	return OK;
}
