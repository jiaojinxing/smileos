#include "timermgr.h"

CTimerMgr::CTimerMgr()
{
    this->InitBasic();  
}
CTimerMgr::~CTimerMgr()
{
    this->Destroy();
}
int CTimerMgr::InitBasic()
{
    this->index = NULL;
    this->top = 0;;
    this->size = 0;
    return OK;
}
int CTimerMgr::Init(long init_size)
{
    int i;

    this->InitBasic();  
    this->size = init_size;
    MALLOC(this->index,CTimer * ,this->size);
    for(i = 0; i < this->size; i++)
        this->index[i] = NULL;
    return OK;
}
int CTimerMgr::Destroy()
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
    this->InitBasic();    
    return OK;
}
int CTimerMgr::IsEmpty()
{
    if(this->top <= 0)
        return TRUE;
    else
        return FALSE;
}
int CTimerMgr::IsFull()
{
    if(this->top >= this->size)
        return TRUE;
    return FALSE;
}
int CTimerMgr::Push(CTimer *node)
{
	ASSERT(!this->IsFull());
	
	this->index[top] = node;
	this->top ++;

    return OK;
}
CTimer * CTimerMgr::Pop()
{
    if(this->IsEmpty())
        return NULL;

    this->top--;

    return this->index[top];
}
int CTimerMgr::Print()
{
    int i;
    for(i = 0; i <this->top; i++)
    {
        this->index[i]->Print();
        LOG(",");
    }
    LOG("\n");
    LOG("size=%ld\n",this->size);
    LOG("top=%ld\n",this->top);
    LOG("bottom=%ld\n",0);
    return OK;
}
long CTimerMgr::GetLen()
{
    return this->top;
}
CTimer * CTimerMgr::GetTop()
{
    if(this->IsEmpty())
        return NULL;
    return this->index[this->top - 1];
}
int CTimerMgr::DelTop()
{
    if(this->IsEmpty())
        return ERROR;
    this->top--;
    this->index[this->top]->Destroy();
    DEL(this->index[this->top]);
    return OK;
}
int CTimerMgr::Clear()
{
    while(this->DelTop());
    return OK;
}
CTimer * CTimerMgr::GetElem(long index)
{
    if(index < 0 || index >= this->top)
        return NULL;
    return this->index[index];
}
int CTimerMgr::DelElem(long i)
{
    CTimer *p;
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
CTimer * CTimerMgr::RemoveElem(int i)
{
    ASSERT(i >= 0 && i < this->top);
	
	CTimer *p = this->GetElem(i);
	this->index[i] = NULL; //avoid freed
	this->DelElem(i);

	return p;
}
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
int CTimerMgr::TimerToIndex(CTimer *timer)
{
	int i;
	CTimer *pt;

	for(i = 0; i < this->GetLen(); i++)
	{
		pt = this->GetElem(i);
		ASSERT(pt);
		if(pt == timer)
			return i;
	}

	return -1;
}

int CTimerMgr::AddTimer(CTimer *timer)
{
	if(this->TimerToIndex(timer) >= 0)
		return ERROR;
	return this->Push(timer);
}

int CTimerMgr::DelTimers(int win_id)
{
	int i;
	CTimer *pt;

	for(i = 0; i < this->GetLen(); i++)
	{
		pt = this->GetElem(i);
		ASSERT(pt);
		if(pt->win_id == win_id)
		{
			this->DelElem(i);
			i --;
		}
	}
	
	return OK;
}
