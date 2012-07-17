// MemStk.cpp: implementation of the CMemStk class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "memstk.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CMemStk::CMemStk()
{
	this->index = NULL;
	this->top = 0;
	this->case_sensitive = TRUE;
}

CMemStk::~CMemStk()
{
    this->Destroy();
}

int CMemStk::Init(long init_size)
{
    int i;

    this->top = 0;
	this->size = init_size;

	MALLOC(this->index,CMem * ,this->size);
    
	for(i = 0; i < this->size; i++)
        this->index[i] = NULL;
    
	return OK;
}

int CMemStk::Destroy()
{
    int i;
	
	if(this->index == NULL)
		return ERROR;

    for(i = 0; i < this->size; i++)
    {
		if(this->index[i])
		{
			this->index[i]->Destroy();
			DEL(this->index[i]);
		}
	}

    FREE(this->index);

    this->index=NULL;
    this->top=0;
    this->size=0;
    
	return OK;
}

int CMemStk::IsEmpty()
{
    if(this->top <= 0)
        return TRUE;
    else
        return FALSE;
}

int CMemStk::IsFull()
{
	if(this->top >= this->size)
		return TRUE;

	return FALSE;
}

int CMemStk::Push(CMem *node)
{
    CMem *p;

    p = this->PushEmpty();

    if(p == NULL)
		return ERROR;

    p->Copy(node);

    return OK;
}

int CMemStk::Pop(CMem *node)
{
    if(this->IsEmpty())
		return ERROR;

    this->top--;

    node->Copy(this->index[this->top]);
    
	this->index[this->top]->Destroy();

	DEL(this->index[this->top]);
    
	return OK;
}

int CMemStk::Print()
{
    int i;

    for(i = 0; i <this->top; i++)
    {
        LOG("%s\n",this->index[i]->p);
    }

    LOG("\n");
    LOG("size=%ld\n",this->size);
    LOG("top=%ld\n",this->top);
    LOG("bottom=%ld\n",0);

    return OK;
}

long CMemStk::GetLen()
{
    return this->top;
}

CMem * CMemStk::Search(CMem *node)
{
    int i;
    
	for(i=0;i<this->top;i++)
    {
        if(this->CompMem(index[i],node) == 0)
            return this->index[i];
    }

    return NULL;
}

CMem * CMemStk::GetTop()
{
    if(this->IsEmpty())
        return NULL;

    return this->index[this->top - 1];
}

int CMemStk::DelTop()
{
    if(this->IsEmpty())
        return ERROR;

    this->top--;

    this->index[this->top]->Destroy();
	
	DEL(this->index[this->top]);

    return OK;
}

int CMemStk::Clear()
{
    while(this->DelTop());
    return OK;
}
CMem *CMemStk::PushEmpty()
{
	if(this->IsFull())
	{
		REALLOC(this->index,CMem *,this->size,this->size * 2);

		this->size *= 2;

		for(int i = this->top; i < this->size; i++)
		{
			this->index[i] = 0;
		}
	}
	
	NEW(this->index[this->top] , CMem );
	this->index[this->top]->Init();
  
	this->top++;
    
    return this->index[this->top - 1];
}

CMem * CMemStk::GetElem(long index)
{
	if(index < 0 || index >= this->top)
		return NULL;

	return this->index[index];
}

int CMemStk::Sort(int order)
{

    long i,j,c;    

	CMem *pt;

    for(j=this->GetLen(); j > 0; j--)
    {
        for(i = 0; i < j - 1; i++)
        {
            if(order==0)
				c = this->CompMem(index[i],index[i+1]) > 0;
            else
				c = this->CompMem(index[i],index[i+1]) < 0;
            if(c)
            {
                
                pt = index[i];
				index[i] = index[i + 1];
				index[i + 1] = pt;                
            }
        }
    }
  
    return OK;
}

CMem * CMemStk::BSearch_Node(CMem *node,int order)
{
	long i;

	i = this->BSearch(node,order);

	return this->GetElem(i);
}

long CMemStk::BSearch(CMem *node,int order)
{
	int find,pos;

	pos = this->BSearch_Pos(node,order,&find);

	if(find) return pos;

	return -1;
}
int CMemStk::InsertElem(long i, CMem *node)
{
	//insert a node at pos i
	CMem *new_node;

	ASSERT(i >= 0 && i <= this->top);
	
	//Add a new node
	new_node = this->PushEmpty(); 
	ASSERT(new_node);
	//copy value
	new_node->Copy(node);

	for(int k = this->top - 1; k >= i; k--)
	{
		this->index[k] = this->index[k - 1];
	}

	this->index[i] = new_node;
	
	return OK;
}
int CMemStk::DelElem(long i)
{
	CMem *p;
	
	ASSERT(i >= 0 && i < this->top);

	p = this->GetElem(i);

	DEL(p);

	for(int k = i; k < this->top; k++)
	{
		this->index[k] = this->index[k + 1];
	}
	
	this->top --;

	this->index[top] = NULL;

	return OK;
}

int CMemStk::InsOrdered(CMem *node, int order,int unique)
{
	int pos,find;

	pos = this->BSearch_Pos(node,order,&find);

	if(find && unique)
		return ERROR;

	return this->InsertElem(pos,node);
}

long CMemStk::BSearch_Pos(CMem *node, int order, int *find_flag)
{
	long low,high,mid,c;
	long flag = 0,pos = -1;

	ASSERT(order == 0 || order == 1);

    low = 0; high=this->GetLen() - 1;

    while(low<=high)
    {
        mid = (low+high)/2;
        
		if(this->CompMem(node,index[mid]) == 0)
		{
			*find_flag = TRUE;
			return mid;
		}

        if(order == 0)
            c = this->CompMem(index[mid],node) > 0;
        else
            c = this->CompMem(index[mid],node) < 0;
        if(c)
            high = mid-1;
        else
            low = mid+1;
    }
	
	*find_flag = FALSE;

	return low;
}
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
int CMemStk::Push(char *str)
{
	CMem *p;

	p = this->PushEmpty();

	ASSERT(p);

	p->SetStr(str);

	return OK;
}

int CMemStk::CompMem(CMem *mem1, CMem *mem2)
{
	if(this->case_sensitive)
		return mem1->StrCmp(mem2->p);
	else
		return mem1->StrICmp(mem2->p);
}

int CMemStk::SetCaseSensetive(BOOL c)
{
	this->case_sensitive = (c != 0);

	return OK;
}


CMem * CMemStk::Search(char *str)
{
	ASSERT(str);

	CMem mem;

	mem.Init();
	mem.SetP(str,strlen(str) + 1);

	return this->Search(&mem);
}

int CMemStk::GetPath(CFileBase *file)
{
	CMem *pstr;

	ASSERT(file);
	
	file->SetSize(0);

	if(this->IsEmpty())
	{
		file->Putc('/');
		file->Putc(0);
		return OK;
	}

	for(int i = 0 ; i < this->GetLen() ;i++)
	{
		pstr = this->GetElem(i);
		ASSERT(pstr);
		if(pstr->p && pstr->p[0])
		{
			file->Putc('/');
			file->Puts(pstr->p);
		}
	}
	
	file->Putc(0);

	return OK;
}
