#include "stdafx.h"
#include "assert.h"
#include "syslog.h"
#include "mem.h"

CMem::CMem()
{
	this->Init();
}
CMem::~CMem()
{
	this->Destroy();
}

int CMem::Destroy()
{
	if(p == NULL)
		return ERROR;

	if(this->self_alloc)
    {
		DEL_ARRAY(this->p);
	}

	this->p = NULL;
	this->self_alloc = FALSE;
    this->size = 0;
	this->max_size = 0;
    this->offset = 0;

	CFileBase::Destroy();

	return OK;
}
long CMem::Malloc(long asize)
{
    if(asize <= 0)
        return 0;

    if(this->p != NULL)
        return 0;

	NEW_ARRAY(this->p,char,asize);

	this->self_alloc = TRUE;
    this->size = 0;
    this->offset = 0;
	this->max_size = asize;

    return asize;
}
long CMem::Read(void *buf,long n)
{
    long  copy_length;
    
    ASSERT(this->p);

    if(this->size - this->offset > n)
        copy_length=n;
    else
        copy_length=this->size - this->offset;
	
	if(copy_length > 0)
	{
		memcpy(buf,(this->p+this->offset),copy_length);
	    this->offset += copy_length; 
		return copy_length;
	}

	return 0;
}
long CMem::Write(void *buf,long n)
{
    long  copy_length;
	
	ASSERT(this->p);
	
	if(n + this->offset > this->size)
	{
		this->AddBlock(n + this->offset - this->size);
	}
	
    if(this->size - this->offset > n)
        copy_length=n;
    else
        copy_length=this->size - this->offset;

	if(copy_length > 0)
	{
		memcpy((this->p+this->offset),buf,copy_length);
		this->offset += copy_length; 
		return copy_length;  
	}
    
	return 0;
}
int CMem::Zero()
{
	ASSERT(this->p);

    memset(this->p,0,this->max_size);

    this->offset = 0;
	this->size = 0;

	return OK;
}
int CMem::Init()
{
	CFileBase::Init();
    this->p = NULL;
    this->size = 0;
    this->offset = 0;
	this->self_alloc = FALSE;
	this->max_size = 0;

	return OK;
}

int CMem::Print()
{
    LOG("\np = 0x%x\n",this->p);
    LOG("size = %ld\n",this->size);
	LOG("max_size = %ld\n",this->max_size);
    LOG("offset = %ld\n",this->offset);

	return OK;
}

int CMem::SetP(char *p,long s)
{
	
	ASSERT(this->p == NULL);

	this->p = p;
	this->size = s;
	this->max_size = s;

	return OK;
}
int CMem::Seek(long off)
{
	if(off < 0 || off >= this->size)
		this->offset = this->size;
	else
		this->offset = off;
	
	return OK;
}
long CMem::GetOffset()
{
	return this->offset;
}

long CMem::GetSize()
{
	return this->size;
}
int CMem::SetSize(long ssize)
{
	if(ssize >= 0 && ssize <= this->max_size)
	{
		this->size = ssize;
		this->AdjustOffset();
		return OK;
	}

	return ERROR;
}

int CMem::AddBlock(long bsize)
{
	//如果没有分配内存，那么分配
	//注意只有一次
	if(this->p == NULL)
	{
		this->Malloc(bsize);
		return OK;
	}

	this->size += bsize;

	if(this->size < 0)
		this->size = 0;

	if(this->size > this->max_size)
		this->size = this->max_size;

	return OK;
}

int CMem::SizeToMax()
{
	this->size = this->GetMaxSize();
	return OK;
}

int CMem::SetStr(char *str)
{
	ASSERT(str);

	CMem mem;
	mem.Init();
	mem.SetP(str,strlen(str) + 1);

	this->Copy(&mem);

	return OK;
}

long CMem::GetMaxSize()
{
	return this->max_size;
}

int CMem::Copy(CMem *mem)
{
	if(this == mem)
		return OK;

	if(this->GetMaxSize() < mem->GetSize())
	{
		this->Destroy();
		this->Init();
		this->Malloc(mem->GetSize());
	}

	this->SetSize(0);
	this->WriteFile(mem);

	return OK;
}

