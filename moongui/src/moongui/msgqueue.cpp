#include "msgqueue.h"
#include "syslog.h"

CMsg::CMsg()
{
    this->InitBasic();
}
CMsg::~CMsg()
{
    this->Destroy();
}
int CMsg::InitBasic()
{
	this->wnd_id = -1;
	this->lparam = 0;
	this->wparam = 0;
	this->message = 0;

    return OK;
}
int CMsg::Init()
{
    this->InitBasic();
    //add your code
    return OK;
}
int CMsg::Destroy()
{
    //add your code
    this->InitBasic();
    return OK;
}
int CMsg::Copy(CMsg *p)
{
    if(this == p)
        return OK;

	this->wnd_id = p->wnd_id;
	this->message = p->message;
	this->wparam = p->wparam;
	this->lparam = p->lparam;

    return OK;
}
int CMsg::Comp(CMsg *p)
{
    return 0;
}
int CMsg::Print()
{
	LOG("wnd_id = %d\n",this->wnd_id);
	LOG("message = %x\n",this->message);
	LOG("wparam = %x,lparam=%x\n",wparam,lparam);
    return TRUE;
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
CMsgQueue::CMsgQueue()
{
    this->InitBasic();
}
CMsgQueue::~CMsgQueue()
{
    this->Destroy();
}
int CMsgQueue::InitBasic()
{
	this->head = 0;
	this->tail = 0;
	this->data = NULL;
	this->size = 0;

    return OK;
}
int CMsgQueue::Init(int size)
{
	int i;

    this->InitBasic();
    
	NEW_ARRAY(this->data,CMsg,size);
	for(i = 0; i < size; i++)
	{
		this->data[i].Init();
	}

	this->size = size;

    return OK;
}
int CMsgQueue::Destroy()
{
	DEL_ARRAY(this->data);
    this->InitBasic();
    return OK;
}
int CMsgQueue::Copy(CMsgQueue *p)
{
    if(this == p)
        return OK;
    return OK;
}
int CMsgQueue::Comp(CMsgQueue *p)
{
    return 0;
}
int CMsgQueue::Print()
{
	int i;
	
	for(i=0;i<this->size;i++)
    {
		this->data[i].Print();
		LOG(",");
	}
    
	LOG("head=%ld\n",this->head);
    LOG("tail=%ld\n",this->tail);
    LOG("size=%ld\n",this->size);
    LOG("length=%ld\n",this->GetLen());

    return TRUE;
}

int CMsgQueue::IsEmpty()
{
    return this->tail == this->head;
}

int CMsgQueue::IsFull()
{
    return (this->tail+1) % this->size == this->head;
}

int CMsgQueue::Clear()
{
	this->head = 0;
	this->tail = 0;
	return OK;
}

int CMsgQueue::EnQueue(CMsg *node)
{
	ASSERT(node);

	if(this->IsFull())
		return ERROR;

	this->data[this->tail].Destroy();
	this->data[this->tail].Init();
	this->data[this->tail].Copy(node);
	
	this->tail=(this->tail+1) % this->size;

	return OK;
}

int CMsgQueue::DeQueue(CMsg *node)
{
	ASSERT(node);

	if(this->IsEmpty())
		return ERROR;
	node->Copy(&this->data[this->head]);
	this->data[this->head].Destroy();

	this->head = (this->head+1) % this->size;
	return OK;
}

int CMsgQueue::GetLen()
{
	return (this->tail - this->head + this->size) % this->size;
}

int CMsgQueue::PostMessage(int wnd_id, int message, int wparam, int lparam)
{
	CMsg msg;

	msg.wnd_id = wnd_id;
	msg.message = message;
	msg.wparam = wparam;
	msg.lparam = lparam;

	return this->EnQueue(&msg);
}
