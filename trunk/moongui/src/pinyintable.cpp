// PinYinTable.cpp: implementation of the CPinYinTable class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "pinyintable.h"
#include "mem.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CPinYinTable::CPinYinTable()
{
    this->InitBasic();
}
CPinYinTable::~CPinYinTable()
{
    this->Destroy();
}
int CPinYinTable::InitBasic()
{
	this->index_file = NULL;
    return OK;
}
int CPinYinTable::Init()
{
    this->InitBasic();
    NEW(this->index_file,CIndexFile);
	this->index_file->Init();

    return OK;
}
int CPinYinTable::Destroy()
{
    DEL(this->index_file);
    this->InitBasic();
    return OK;
}

int CPinYinTable::LoadIndexFile(CFileBase *file)
{
	ASSERT(file);
	return this->index_file->LoadIndexFile(file);
}

int CPinYinTable::GetItem(int i, CFileBase *file)
{
	ASSERT(file);

	file->SetSize(0);	
	this->index_file->GetBlock(i,file);
	file->Putc(0);

	return OK;
}

int CPinYinTable::GetPinYin(CFileBase *py_line,CFileBase *file)
{
	py_line->Seek(0);
	return py_line->ReadString(file);
}

int CPinYinTable::BSearch_Pos(char *py, int *find_flag)
{
	ASSERT(py && find_flag);

    long low,high,mid,c;
	CMem mem,buf;
	
	LOCAL_MEM(mem);
	LOCAL_MEM(buf);

    low = 0; high=this->index_file->GetTotalBlocks() - 1;
    while(low<=high)
    {
        mid = (low+high)>>1;
		this->GetItem(mid,&mem);
		this->GetPinYin(&mem,&buf);
		
		c = buf.StrICmp(py);
        if(c == 0)
        {
            *find_flag = TRUE;
            return mid;
        }
		
		if(c > 0)
            high = mid-1;
        else
            low = mid+1;
    }
    *find_flag = FALSE;
    return low;
}

int CPinYinTable::Search(char *py)
{
	int ff;
	return this->BSearch_Pos(py,&ff);
}

int CPinYinTable::GetMax()
{
	return this->index_file->GetTotalBlocks();
}
