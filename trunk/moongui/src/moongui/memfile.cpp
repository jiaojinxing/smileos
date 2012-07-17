#include "stdafx.h"
#include "memfile.h"
#include "assert.h"
#include "syslog.h"

CMemFile::CMemFile()
{
	this->size = 0;
	this->base = NULL;
	this->max_page = 0;
	this->offset = 0;
	this->pages = 0;
	this->pagesize = 64*1024;
	this->mod_n = 0xffffffff;
	this->shift_n = 0;
}
CMemFile::~CMemFile()
{
	this->Destroy();
}
int CMemFile::Init()
{   
	CFileBase::Init();

	this->size = 0;
	this->base = NULL;
	this->max_page = 0;
	this->offset = 0;
	this->pages = 0;
	this->pagesize = 64*1024;
	this->InitShift();
	this->SetMaxPages(1024);

	return OK;
}
int CMemFile::Init(long page_size,long max_pages)
{
	if(max_pages < 0)
		return ERROR;
	
	CFileBase::Init();
	
	this->size = 0;
	this->base = NULL;
	this->max_page = 0;
	this->offset = 0;
	this->pages = 0;
	this->pagesize = page_size;
	this->SetMaxPages(max_pages);
	this->InitShift();

	return OK;
}
int CMemFile::InitShift()
{
	int i;

	for(i = 5; i < 32; i++)
	{
		if( (1 << i) == this->pagesize)
		{
			this->shift_n = i;
			this->mod_n = this->pagesize - 1;

			return OK;
		}
	}
	
	LOG("error pagesize, this value must can be gotten by 1<<n\n");
	LOG("and must greater than 32\n");
	
	return ERROR;
}

int CMemFile::Destroy()
{
	this->Close();
	if(this->base)
	{
		FREE(this->base);
		this->base = NULL;
	}
	this->size = 0;
	this->offset = 0;
	this->pages = 0;
	this->max_page = 0;

	return OK;
}
int CMemFile::Close()
{
	long i;
	
	for(i=0;i<this->pages;i++)
	{
		if(this->base[i] != NULL)
		{
			FREE(this->base[i]);
			this->base[i] = NULL;
		}
	}
	this->size = 0;
	this->offset = 0;
	this->pages = 0;
	return OK;
}
int CMemFile::SetMaxPages(long mp)
{
	if(mp <= 0)
		return ERROR;
	
	this->max_page = mp;
	return OK;
}
int CMemFile::AddPage()
{
	long i;

	if(this->base == NULL)
	{
		MALLOC(this->base,char *,this->max_page);
	
		for(i=0;i<this->max_page;i++)
			this->base[i] = NULL;
	}

	if(this->pages < this->max_page - 1)
	{
		MALLOC(this->base[this->pages] ,char, this->pagesize);
		this->pages ++;
	}
	else
	{
		LOG("Too many pages!");
		return ERROR;
	}

	return OK;
}
int CMemFile::Open(long s)
{
	long i,p;
	
	if(this->base == NULL)
		return ERROR;
	if(this->pages > 0)
		return ERROR;
	p = (s >> this->shift_n) +1;
	this->size = s;
	this->offset = 0;
	for(i=0; i < p ;i++)
	{
		this->AddPage();
	}
	return OK;
}
int CMemFile::AddBlock(long block_size)
{
	long i,pn,new_page;
	
	new_page = ((this->size+block_size) >> this->shift_n) + 1; 
	pn=new_page - this->pages;
	if(new_page > this->pages)
	{
		for(i = 0; i < pn; i++)
			this->AddPage();
	}
	this->size += block_size;  
	
	return OK;
}
long CMemFile::Read(void *buf,long rsize)
{
	long left,sum=0,read_size=0;
	long block_end;
	long block=this->offset >> this->shift_n;
	long off=this->offset & this->mod_n;

	if(this->offset + rsize > this->size)
		rsize=this->size - this->offset;

	left = rsize;
	while(sum<rsize)
	{
		block_end = (block << this->shift_n) + this->pagesize;
		if(this->offset + left < block_end)
		{
			read_size=left;
		}
		else
		{
			read_size = block_end - this->offset;
		}
		memcpy((char*)buf+sum , this->base[block] + off, read_size);
		this->offset += read_size;
		sum += read_size;
		left -= read_size;
		block = this->offset >> this->shift_n;
		off = this->offset & this->mod_n;
	}
	return sum;
}
long CMemFile::Write(void *buf,long wsize)
{
	long sum=0, write_size=0;
	long block_end;
	long left=wsize;
	long block= this->offset >> this->shift_n;
	long off=this->offset & this->mod_n;
	
	if(wsize <= 0) return 0;
	
	if(wsize + this->offset > this->size)
	{
		this->AddBlock(wsize + this->offset - this->size);
	}
	
	while(sum<wsize)
	{
		block_end = (block << this->shift_n) + this->pagesize;
		if(this->offset + left < block_end)
		{
			write_size=left;
		}
		else
		{
			write_size=block_end - this->offset;
		}
		memcpy(this->base[block]+off,(char*)buf+sum,write_size);
		this->offset += write_size;
		sum += write_size;
		left -= write_size;
		block = this->offset >> this->shift_n;
		off = this->offset & this->mod_n;
	}
	return sum;
}

int CMemFile::Print()
{
	LOG("\nsize = %ld\n",this->size);
	LOG("pages = %ld\n",this->pages);
	LOG("max_page = %ld\n",this->max_page);
	LOG("offset = %ld\n",this->offset);

	return OK;
}

int CMemFile::Seek(long off)
{
	if(off < 0 || off >= this->size)
		this->offset = this->size;
	else
		this->offset = off;

	return OK;
}

int CMemFile::SetSize(long s)
{
	if(s < 0) return ERROR;
	
	if(s > this->pages * this->pagesize)
		return ERROR;
	
	this->size = s;
	
	this->AdjustOffset();

	return OK;
}

long CMemFile::GetOffset()
{
	return this->offset;
}

long CMemFile::GetSize()
{
	return this->size;
}

long CMemFile::GetMaxSize()
{
	return this->max_page * this->pagesize;
}
