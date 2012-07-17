#include "menuitemmgr.h"

CMenuItemMgr::CMenuItemMgr()
{
    this->InitBasic();  
}
CMenuItemMgr::~CMenuItemMgr()
{
    this->Destroy();
}
int CMenuItemMgr::InitBasic()
{
    this->index = NULL;
    this->top = 0;;
    this->size = 0;
    return OK;
}
int CMenuItemMgr::Init(long init_size)
{
    int i;

    this->InitBasic();  
    this->size = init_size;
    MALLOC(this->index,CMenuItem * ,this->size);
    for(i = 0; i < this->size; i++)
        this->index[i] = NULL;
    return OK;
}
int CMenuItemMgr::Destroy()
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
int CMenuItemMgr::IsEmpty()
{
    if(this->top <= 0)
        return TRUE;
    else
        return FALSE;
}
int CMenuItemMgr::IsFull()
{
    if(this->top >= this->size)
        return TRUE;
    return FALSE;
}
int CMenuItemMgr::Push(CMenuItem *node)
{
	ASSERT(!this->IsFull());
	
	this->index[top] = node;
	this->top ++;

    return OK;
}
CMenuItem * CMenuItemMgr::Pop()
{
    if(this->IsEmpty())
        return NULL;

    this->top--;

    return this->index[top];
}
int CMenuItemMgr::Print()
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
long CMenuItemMgr::GetLen()
{
    return this->top;
}
CMenuItem * CMenuItemMgr::GetTop()
{
    if(this->IsEmpty())
        return NULL;
    return this->index[this->top - 1];
}
int CMenuItemMgr::DelTop()
{
    if(this->IsEmpty())
        return ERROR;
    this->top--;
    this->index[this->top]->Destroy();
    DEL(this->index[this->top]);
    return OK;
}
int CMenuItemMgr::Clear()
{
    while(this->DelTop());
    return OK;
}
CMenuItem * CMenuItemMgr::GetElem(long index)
{
    if(index < 0 || index >= this->top)
        return NULL;
    return this->index[index];
}
int CMenuItemMgr::DelElem(long i)
{
    CMenuItem *p;
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
CMenuItem * CMenuItemMgr::RemoveElem(int i)
{
    ASSERT(i >= 0 && i < this->top);
	
	CMenuItem *p = this->GetElem(i);
	this->index[i] = NULL; //avoid freed
	this->DelElem(i);

	return p;
}
///////////////////////////////////////////////////
///////////////////////////////////////////////////
int CMenuItemMgr::DelItems(int win_id)
{
	int i;
	CMenuItem *pi;

	for(i = 0; i < this->GetLen(); i++)
	{
		pi = this->GetElem(i);
		ASSERT(pi);

		if(pi->win_id == win_id)
		{
			this->DelElem(i);
			i --;
		}
	}

	return OK;
}

int CMenuItemMgr::GetItemsCount(int win_id)
{
	int i,t = 0;
	CMenuItem *pi;

	for(i = 0; i < this->GetLen(); i++)
	{
		pi = this->GetElem(i);
		ASSERT(pi);

		if(pi->win_id == win_id)
			t++;
	}

	return t;
}

int CMenuItemMgr::ItemToIndex(CMenuItem *item)
{
	int i;
	CMenuItem *pi;

	for(i = 0; i < this->GetLen(); i++)
	{
		pi = this->GetElem(i);
		ASSERT(pi);

		if(pi == item)
			return i;
	}

	return -1;	
}

int CMenuItemMgr::AddMenuItem(CMenuItem *item)
{
	if(this->ItemToIndex(item) >= 0)
		return ERROR;
	return this->Push(item);
}

int CMenuItemMgr::GetMaxWidth(int win_id)
{
	int i,max = 0;
	CMenuItem *pi;

	for(i = 0; i < this->GetLen(); i++)
	{
		pi = this->GetElem(i);
		ASSERT(pi);

		if(pi->win_id == win_id)
		{
			if(pi->GetWidth() > max)
				max = pi->GetWidth();
		}
	}

	return max;
}

int CMenuItemMgr::GetTotalHeight(int win_id)
{
	int i,t = 0;
	CMenuItem *pi;

	for(i = 0; i < this->GetLen(); i++)
	{
		pi = this->GetElem(i);
		ASSERT(pi);

		if(pi->win_id == win_id)
		{
			t += pi->GetHeight();
		}
	}

	return t;
}

int CMenuItemMgr::DrawItems(CCanvas *can, int win_id)
{
	ASSERT(can);
	
	int i,y_pos = 0;
	CMenuItem *pi;

	for(i = 0; i < this->GetLen(); i++)
	{
		pi = this->GetElem(i);
		ASSERT(pi);

		if(pi->win_id == win_id)
		{
			pi->Draw(can,0,y_pos);
			y_pos += pi->GetHeight();
		}
	}

	return OK;
}

int CMenuItemMgr::PosToIndex(int y,int win_id)
{
	int i,t,y_pos;
	CMenuItem *pi;
	
	y_pos = 0;

	for(i = 0; i < this->GetLen(); i++)
	{
		pi = this->GetElem(i);
		ASSERT(pi);

		if(pi->win_id == win_id)
		{
			
			t =  pi->GetHeight();
			if(y_pos <= y &&  y_pos + t > y)
				return i;

			y_pos += t;
		}
	}

	return -1;
}

int CMenuItemMgr::SetHilight(int index, int win_id)
{
	int i,t;
	CMenuItem *pi;
	
	pi = this->GetElem(index);
	if(pi == NULL)
		return ERROR;

	if(pi->IsHilighted() || pi->IsSeparator())
		return ERROR;

	pi->SetHiLight(TRUE);

	for(i = 0; i < this->GetLen(); i++)
	{
		pi = this->GetElem(i);
		ASSERT(pi);

		if(pi->win_id == win_id)
		{
			if(i != index)
				pi->SetHiLight(FALSE);
		}
	}

	return OK;
}

int CMenuItemMgr::GetHilightItem(int win_id)
{
	int i;
	CMenuItem *pi;

	for(i = 0; i < this->GetLen(); i++)
	{
		pi = this->GetElem(i);
		ASSERT(pi);

		if(pi->win_id == win_id && pi->IsHilighted())
		{
			return i;
		}
	}

	return -1;
}

int CMenuItemMgr::ClearHilight(int win_id)
{
	int i;
	CMenuItem *pi;

	for(i = 0; i < this->GetLen(); i++)
	{
		pi = this->GetElem(i);
		ASSERT(pi);

		if(pi->win_id == win_id && pi->IsHilighted())
		{
			pi->SetHiLight(0);
		}
	}

	return OK;
}

int CMenuItemMgr::IndexToPos(int index,int win_id)
{
	int i,y_pos;
	CMenuItem *pi;
	
	y_pos = 0;

	for(i = 0; i < index; i++)
	{
		pi = this->GetElem(i);
		ASSERT(pi);

		if(pi->win_id == win_id)
		{
			y_pos +=  pi->GetHeight();
		}
	}

	return y_pos;	
}
