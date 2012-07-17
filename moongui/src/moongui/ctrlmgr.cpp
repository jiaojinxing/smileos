#include "ctrlmgr.h"

CCtrlMgr::CCtrlMgr()
{
    this->InitBasic();  
}
CCtrlMgr::~CCtrlMgr()
{
    this->Destroy();
}
int CCtrlMgr::InitBasic()
{
    this->index = NULL;
    this->top = 0;;
    this->size = 0;
    return OK;
}
int CCtrlMgr::Init(long init_size)
{
    int i;

    this->InitBasic();  
    this->size = init_size;
    MALLOC(this->index,CControl * ,this->size);
    for(i = 0; i < this->size; i++)
        this->index[i] = NULL;
    return OK;
}
int CCtrlMgr::Destroy()
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
int CCtrlMgr::IsEmpty()
{
    if(this->top <= 0)
        return TRUE;
    else
        return FALSE;
}
int CCtrlMgr::IsFull()
{
    if(this->top >= this->size)
        return TRUE;
    return FALSE;
}
int CCtrlMgr::Push(CControl *node)
{
	ASSERT(!this->IsFull());
	
	this->index[top] = node;
	this->top ++;

    return OK;
}
CControl * CCtrlMgr::Pop()
{
    if(this->IsEmpty())
        return NULL;

    this->top--;

    return this->index[top];
}
int CCtrlMgr::Print()
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
long CCtrlMgr::GetLen()
{
    return this->top;
}
CControl * CCtrlMgr::GetTop()
{
    if(this->IsEmpty())
        return NULL;
    return this->index[this->top - 1];
}
int CCtrlMgr::DelTop()
{
    if(this->IsEmpty())
        return ERROR;
    this->top--;
    this->index[this->top]->Destroy();
    DEL(this->index[this->top]);
    return OK;
}
int CCtrlMgr::Clear()
{
    while(this->DelTop());
    return OK;
}
CControl * CCtrlMgr::GetElem(long index)
{
    if(index < 0 || index >= this->top)
        return NULL;
    return this->index[index];
}
int CCtrlMgr::DelElem(long i)
{
    CControl *p;
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
CControl * CCtrlMgr::RemoveElem(int i)
{
    ASSERT(i >= 0 && i < this->top);
	
	CControl *p = this->GetElem(i);
	this->index[i] = NULL; //avoid freed
	this->DelElem(i);

	return p;
}

int CCtrlMgr::DelCtrls(int win_id)
{
	int i;
	CControl *pctrl;

	for(i = 0; i < this->GetLen(); i++)
	{
		pctrl = this->GetElem(i);
		ASSERT(pctrl);

		if(pctrl->win_id == win_id)
		{
			this->DelElem(i);
			i --;
		}
	}

	return OK;
}

int CCtrlMgr::DrawCtrls(CDc *pdc, int win_id)
{
	int i;
	CControl *pctrl;

	for(i = 0; i < this->GetLen(); i++)
	{
		pctrl = this->GetElem(i);
		ASSERT(pctrl);

		if(pctrl->win_id == win_id)
		{
			pctrl->Draw(pdc);
		}
	}

	return OK;
}

int CCtrlMgr::HandleCtrlsMessage(int win_id, int message, int wparam, int lparam)
{
	int i;
	CControl *pctrl;

	for(i = 0; i < this->GetLen(); i++)
	{
		pctrl = this->GetElem(i);
		ASSERT(pctrl);

		if(pctrl->win_id == win_id)
		{
			pctrl->HandleMessage(message,wparam,lparam);
		}
	}

	return OK;
}

int CCtrlMgr::AddCtrl(CControl *ctrl)
{
	ASSERT(ctrl);
	return this->Push(ctrl);
}

int CCtrlMgr::SetFocus(int ctrl_id)
{
	int i,win_id;
	CControl *pctrl;
	
	pctrl = this->GetElem(this->CtrlToIndex(ctrl_id));
	if(pctrl == NULL)
		return ERROR;

	win_id = pctrl->win_id;
	pctrl->style |= WS_FOCUS;

	for(i = 0; i < this->GetLen(); i++)
	{
		pctrl = this->GetElem(i);
		ASSERT(pctrl);

		if(pctrl->win_id == win_id && pctrl->id != ctrl_id)
		{
			pctrl->style &= (~WS_FOCUS);
		}
	}

	return OK;
}

int CCtrlMgr::CtrlToIndex(int ctrl_id)
{
	int i;
	CControl *pctrl;

	for(i = 0; i < this->GetLen(); i++)
	{
		pctrl = this->GetElem(i);
		ASSERT(pctrl);

		if(pctrl->id == ctrl_id)
		{		
			return i;
		}
	}
	
	return -1;
}

