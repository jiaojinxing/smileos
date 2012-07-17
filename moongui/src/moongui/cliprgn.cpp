#include "mem_tool.h"
#include "cliprgn.h"

CClipRgn::CClipRgn()
{
	this->InitBasic();
}
CClipRgn::~CClipRgn()
{
    this->Destroy();
}
int CClipRgn::InitBasic()
{
	this->size = 0;
    this->index = NULL;
    this->top = 0;
	this->rcBound = NULL;

	return OK;
}

int CClipRgn::Init(long init_size)
{
    int i;
    
	this->InitBasic();
	
    this->size = init_size;
	
	this->rcBound = CRect::AllocRect();
	ASSERT(this->rcBound);

    MALLOC(this->index,CRect * ,this->size);
    for(i = 0; i < this->size; i++)
        this->index[i] = NULL;

    return OK;
}
int CClipRgn::Destroy()
{
    int i;

    if(this->index == NULL)
        return ERROR;
    
	for(i = 0; i < this->size; i++)
    {
		CRect::FreeRect(this->index[i]);
    }
    
	FREE(this->index);
	
	CRect::FreeRect(this->rcBound);
	
	this->InitBasic();

    return OK;
}
int CClipRgn::IsEmpty()
{
    if(this->top <= 0)
        return TRUE;
    else
        return FALSE;
}
int CClipRgn::IsFull()
{
    if(this->top >= this->size)
        return TRUE;
    return FALSE;
}
int CClipRgn::Push(CRect *node)
{
    CRect *p;
    p = this->PushEmpty();
    if(p == NULL)
        return ERROR;
    p->Copy(node);
    return OK;
}
int CClipRgn::Pop(CRect *node)
{
    if(this->IsEmpty())
        return ERROR;
    this->top--;
    node->Copy(this->index[this->top]);
    this->index[this->top]->Destroy();
	CRect::FreeRect(this->index[this->top]);
    return OK;
}
int CClipRgn::Print()
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
	LOG("bound = ");
	this->rcBound->Print();

    return OK;
}
long CClipRgn::GetLen()
{
    return this->top;
}
long CClipRgn::Search_Pos(CRect *node)
{
    int i;
    for(i=0;i<this->top;i++)
    {
        if(this->index[i]->Comp(node) == 0)
            return i;
    }
    return -1;
}

CRect * CClipRgn::Search(CRect *node)
{
	long pos = this->Search_Pos(node);
	if(pos >= 0 && pos < this->top)
		return this->index[pos];;
    return NULL;
}
CRect * CClipRgn::GetTop()
{
    if(this->IsEmpty())
        return NULL;
    return this->index[this->top - 1];
}
int CClipRgn::DelTop()
{
    if(this->IsEmpty())
        return ERROR;
    this->top--;
    this->index[this->top]->Destroy();
	CRect::FreeRect(this->index[this->top]);
	this->index[this->top] = NULL;
    return OK;
}
int CClipRgn::Clear()
{
    while(this->DelTop());
	this->top  = 0;	
	this->rcBound->Empty();
    return OK;
}
CRect *CClipRgn::PushEmpty()
{
	ASSERT(!this->IsFull());

    this->index[this->top] = CRect::AllocRect();
	ASSERT(this->index[this->top]);

    this->index[this->top]->Init();
    this->top++;

    return this->index[this->top - 1];
}
CRect * CClipRgn::GetElem(long index)
{
    if(index < 0 || index >= this->top)
        return NULL;
    return this->index[index];
}
int CClipRgn::Sort(int order)
{
    long i,j,c;    
    CRect *pt;
    for(j=this->GetLen(); j > 0; j--)
    {
        for(i = 0; i < j - 1; i++)
        {
            if(order==0)
                c = index[i]->Comp(index[i+1]) > 0;
            else
                c = index[i]->Comp(index[i+1]) < 0;
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
CRect * CClipRgn::BSearch_CRect(CRect *node,int order)
{
    long i;
    i = this->BSearch(node,order);
    return this->GetElem(i);
}
long CClipRgn::BSearch(CRect *node,int order)
{
    int find,pos;
    pos = this->BSearch_Pos(node,order,&find);
    if(find) return pos;
    return -1;
}
int CClipRgn::InsertElem(long i, CRect *node)
{
    //insert a node at pos i
    CRect *new_node;
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
int CClipRgn::DelElem(long i)
{
    CRect *p;
    ASSERT(i >= 0 && i < this->top);
    p = this->GetElem(i);
	
	CRect::FreeRect(p);

    for(int k = i; k < this->top; k++)
    {
        this->index[k] = this->index[k + 1];
    }
    this->top --;
    this->index[top] = NULL;
    return OK;
}
int CClipRgn::InsOrdered(CRect *node, int order,int unique)
{
    int pos,find;
    pos = this->BSearch_Pos(node,order,&find);
    if(find && unique)
        return ERROR;
    return this->InsertElem(pos,node);
}
long CClipRgn::BSearch_Pos(CRect *node, int order, int *find_flag)
{
    long low,high,mid,c;
    long flag = 0,pos = -1;
    ASSERT(order == 0 || order == 1);
    low = 0; high=this->GetLen() - 1;
    while(low<=high)
    {
        mid = (low+high)/2;
        if(node->Comp(this->index[mid]) == 0)
        {
            *find_flag = TRUE;
            return mid;
        }
        if(order == 0)
            c = this->index[mid]->Comp(node) > 0;
        else
            c = this->index[mid]->Comp(node) < 0;
        if(c)
            high = mid-1;
        else
            low = mid+1;
    }
    *find_flag = FALSE;
    return low;
}

int CClipRgn::Copy(CClipRgn *pRgn)
{
	ASSERT(pRgn);

	CRect *pr;
	int i;

	if(pRgn == this)
		return OK;

	this->Clear();
	for(i = 0; i < pRgn->GetLen(); i++)
	{
		pr = pRgn->GetElem(i);
		ASSERT(pr);

		this->Push(pr);
	}
	
	this->rcBound->Copy(pRgn->rcBound);

	return OK;
}


int CClipRgn::EvaluateBound()
{
	int i;
	CRect *pr;

	this->DelEmptyRects();
	this->rcBound->Empty();

	pr = this->GetElem(0);
	if(pr == NULL)
		return OK;

	this->rcBound->Copy(pr);
	for(i = 1; i < this->GetLen(); i++)
	{
		pr = this->GetElem(i);
		ASSERT(pr);

		this->rcBound->GetBound(this->rcBound,pr);
	}

	return OK;
}

int CClipRgn::DelEmptyRects()
{
	int i;
	CRect *pr;

	for(i = 0; i < this->GetLen(); i++)
	{
		pr = this->GetElem(i);
		ASSERT(pr);

		if(pr->IsEmpty())
		{
			this->DelElem(i);
			i --;
		}
	}

	return OK;
}

int CClipRgn::AddRect(CRect *pr)
{
	CRect *p;

	p = this->PushEmpty();
	ASSERT(p);

	p->Copy(pr);
	p->Normalize();

	this->EvaluateBound();

	return OK;
}

int CClipRgn::Intersect(CRect *prc)
{
	ASSERT(prc);

	CRect rc,*p;
	int i;

	rc.Copy(prc);
	rc.Normalize();
	
	for(i = 0; i < this->GetLen(); i++)
	{
		p = this->GetElem(i);
		ASSERT(p);

		p->Intersect(p,&rc);
	}

	this->EvaluateBound();

	return OK;
}

int CClipRgn::IsIntersect(CRect *pr)
{
	ASSERT(pr);

	CRect rc,*p;
	int i;

	if( pr->IsEmpty() )
		return FALSE;

	rc.Copy(pr);
	rc.Normalize();
	
	for(i = 0; i < this->GetLen(); i++)
	{
		p = this->GetElem(i);
		ASSERT(p);

		if(p->IsIntersect(&rc) )
			return TRUE;
	}
	
	return FALSE;
}

int CClipRgn::Subtract(CRect *prc)
{
	ASSERT(prc);

	int i,j,c,len;
	CRect rc,*p,sret[4];

	if(prc->IsEmpty())
		return FALSE;

	rc.Copy(prc);
	rc.Normalize();

	if( ! rc.IsIntersect(this->rcBound) )
		return FALSE;
	
	len = this->GetLen();
	for(i = 0; i < len; i++)
	{
		p = this->GetElem(i);
		ASSERT(p);

		if( ! p->IsIntersect(&rc) )
			continue;
		
		c = p->Subtract(&rc , sret);
		if(c <= 0)
			p->Empty();
		else 
			p->Copy(&sret[0]);

		for( j = 1; j < c; j++)
			this->Push(&sret[j]);
	}

	this->EvaluateBound();

	return OK;
}

int CClipRgn::Draw(pixel_type color)
{
	int i;
	CRect *p;
	
	GAL_SetClipping(0,0,GAL_GetWidth(),GAL_GetHeight());
	for(i = 0; i < this->GetLen(); i++)
	{
		p = this->GetElem(i);
		ASSERT(p);
		p->Draw(color);
	}

	return OK;
}

int CClipRgn::Intersect(CClipRgn *prgn1, CClipRgn *prgn2)
{
	ASSERT( prgn1 && prgn2 );
	int i,j;
	CRect *p1, *p2,rc;
	
	this->Clear();

	for(i = 0; i < prgn1->GetLen(); i++)
	{
		p1 = prgn1->GetElem(i);
		ASSERT(p1);

		for(j = 0; j < prgn2->GetLen(); j++)
		{
			p2 = prgn2->GetElem(i);
			ASSERT(p2);
			
			rc.Intersect(p1,p2);
			this->Push(&rc);
		}
	}

	this->EvaluateBound();

	return OK;
}

int CClipRgn::Set(CRect *pr)
{
	ASSERT(pr);

	CRect rc;

	rc.Copy(pr);
	rc.Normalize();

	this->Clear();
	this->AddRect(&rc);

	return OK;
}
