// IntStk.cpp: implementation of the CIntStk class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "intstk.h"
#include "syslog.h"
#include "mem_tool.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIntStk::CIntStk()
{
    this->InitBasic();
}
CIntStk::~CIntStk()
{
    this->Destroy();
}
int CIntStk::InitBasic()
{
	this->nums = NULL;
	this->top = 0;
	this->size = 0;

    return OK;
}
int CIntStk::Init(int init_size)
{
	this->InitBasic();

	MALLOC(this->nums,int,init_size);
	this->size = init_size;
    
    return OK;
}
int  CIntStk::Destroy()
{
	FREE(this->nums);
    this->InitBasic();
    return OK;
}
int  CIntStk::Print()
{
	for(int i = 0; i< top; i++)
	{
		LOG("%d,",this->nums[i]);
	}
    return TRUE;
}
int CIntStk::IsEmpty()
{
    if(this->top <= 0)
        return TRUE;
    else
        return FALSE;
}

int CIntStk::IsFull()
{
	if(this->top >= this->size)
		return TRUE;

	return FALSE;
}

int CIntStk::Push(int n)
{
	if(this->IsFull())
	{
		REALLOC(this->nums,int,this->size,this->size * 2);
		this->size *= 2;
	}

	this->nums[top] = n;
	this->top ++;
	
	return OK;
}

int CIntStk::Pop()
{
	ASSERT(!this->IsEmpty());

	this->top --;
	return this->nums[top];
}

int CIntStk::GetElem(int index)
{
	ASSERT(index >= 0 && index < this->top);
	return this->nums[index];
}

int CIntStk::GetLen()
{
	return this->top;
}

int CIntStk::Clear()
{	
	this->top = 0;
	return OK;
}

int CIntStk::BSearch_Pos(int n, int order, int *find_flag)
{
    int low,high,mid,c;
    
	ASSERT(order == 0 || order == 1);

    low = 0; high=this->GetLen() - 1;
    while(low<=high)
    {
        mid = (low+high)>>1;
        if(this->nums[mid] == n)
        {
            *find_flag = TRUE;
            return mid;
        }

        if(order == 0)
            c = this->nums[mid] > n;
        else
            c = this->nums[mid] < n;

        if(c)
            high = mid-1;
        else
            low = mid+1;
    }
    *find_flag = FALSE;
    return low;
}

int CIntStk::InsertElem(int index, int n)
{
	//insert a node at pos i
    ASSERT(index >= 0 && index <= this->top);
        
    for(int k = this->top - 1; k >= index; k--)
    {
        this->nums[k] = this->nums[k - 1];
    }
    
	this->nums[index] = n;
    return OK;
}

int CIntStk::DelElem(int index)
{
    ASSERT(index >= 0 && index < this->top);
    for(int k = index; k < this->top; k++)
    {
        this->nums[k] = this->nums[k + 1];
    }
    this->top --;
    return OK;
}

int CIntStk::InsOrdered(int n, int order, int unique)
{
    int pos,find;
    pos = this->BSearch_Pos(n,order,&find);
    if(find && unique)
        return ERROR;
    return this->InsertElem(pos,n);

}

int CIntStk::Search_Pos(int n)
{
	int i;
	for(i=0;i<this->top;i++)
    {
        if(this->nums[i] == n)
            return i;
    }
 
	return -1;
}

int CIntStk::GetTop()
{
	ASSERT(!this->IsEmpty());

	return this->nums[top - 1];
}

int CIntStk::SetElem(int index, int val)
{
	ASSERT(index >= 0 && index < this->top);
	this->nums[index] = val;
	return OK;
}
