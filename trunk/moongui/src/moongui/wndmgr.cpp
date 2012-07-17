#include "wndmgr.h"
#include "globals.h"

CWndMgr::CWndMgr()
{
    this->InitBasic();  
}
CWndMgr::~CWndMgr()
{
    this->Destroy();
}
int CWndMgr::InitBasic()
{
    this->index = NULL;
    this->top = 0;;
    this->size = 0;
    return OK;
}
int CWndMgr::Init(long init_size)
{
    int i;

    this->InitBasic();  
    this->size = init_size;
    MALLOC(this->index,CWindow * ,this->size);
    for(i = 0; i < this->size; i++)
        this->index[i] = NULL;
    return OK;
}
int CWndMgr::Destroy()
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
int CWndMgr::IsEmpty()
{
    if(this->top <= 0)
        return TRUE;
    else
        return FALSE;
}
int CWndMgr::IsFull()
{
    if(this->top >= this->size)
        return TRUE;
    return FALSE;
}
int CWndMgr::Push(CWindow *node)
{
	ASSERT(!this->IsFull());
	
	this->index[top] = node;
	this->top ++;

    return OK;
}
CWindow * CWndMgr::Pop()
{
    if(this->IsEmpty())
        return NULL;

    this->top--;

    return this->index[top];
}
int CWndMgr::Print()
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
long CWndMgr::GetLen()
{
    return this->top;
}
CWindow * CWndMgr::GetTop()
{
    if(this->IsEmpty())
        return NULL;
    return this->index[this->top - 1];
}
int CWndMgr::DelTop()
{
    if(this->IsEmpty())
        return ERROR;
    this->top--;
    this->index[this->top]->Destroy();
    DEL(this->index[this->top]);
    return OK;
}
int CWndMgr::Clear()
{
    while(this->DelTop());
    return OK;
}
CWindow * CWndMgr::GetElem(long index)
{
    if(index < 0 || index >= this->top)
        return NULL;
    return this->index[index];
}
int CWndMgr::DelElem(long i)
{
    CWindow *p;
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
CWindow * CWndMgr::RemoveElem(int i)
{
    ASSERT(i >= 0 && i < this->top);
	
	CWindow *p = this->GetElem(i);
	this->index[i] = NULL; //avoid freed
	this->DelElem(i);

	return p;
}

//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
int CWndMgr::AddWindow(CWindow *pwnd)
{
	ASSERT(pwnd);
	
	if(this->WindowToIndex(pwnd) >= 0)
		return ERROR;

	this->Push(pwnd);
	
	pwnd->privCDC->i_font = afx.hz12; //set default font;
	pwnd->dwStyle &= (~WS_ACTIVE); //remove active style
	this->RecalcuAllRgn(); //recalculate all clip region

	pwnd->SendMessage(WM_CREATE,0,0); //send create message	
	this->PostMessage(pwnd->id,WM_PAINT,0,1); //post paint message

	return OK;
}

int CWndMgr::RecalcuClipRgn(CWindow *pwnd)
{
	return this->RecalcuClipRgn(this->WindowToIndex(pwnd));
}

int CWndMgr::WindowToIndex(CWindow *pwnd)
{
	int i;
	CWindow *p;

	for(i = 0; i < this->GetLen(); i++)
	{
		p = this->GetElem(i);
		ASSERT(p);

		if(p == pwnd)
			return i;
	}

	return -1;
}

int CWndMgr::IsTopThan(int i, int j)
{
	CWindow *p1,*p2;
	
	if(i == j)
		return FALSE;

	p1 = this->GetElem(i);
	p2 = this->GetElem(j);

	ASSERT(p1 && p2);

	if(p1->dwExStyle & WS_EX_LOWMOST)
	{
		if(p2->dwExStyle & WS_EX_LOWMOST)
			return i > j;
		else
			return FALSE;
	}
	else
	{
		if(p2->dwExStyle & WS_EX_LOWMOST)
			return TRUE;
	}

	if(p1->dwExStyle & WS_EX_TOPMOST)
	{
		if(p2->dwExStyle & WS_EX_TOPMOST)
			return i > j;
		else
			return TRUE;
	}
	else
	{
		if(p2->dwExStyle & WS_EX_TOPMOST)
			return FALSE;
	}

	return i > j;
}

int CWndMgr::IsLowerThan(int i, int j)
{
	if(i == j)
		return FALSE;

	return (! this->IsTopThan(i,j));
}

int CWndMgr::IsVisible(int i)
{
	CWindow *p;

	p = this->GetElem(i);
	if(p == NULL)
		return FALSE;

	return p->IsVisible();
}

int CWndMgr::IsWindow(int win_id)
{
	return this->IdToIndex(win_id) > 0;
}

int CWndMgr::SetActiveWindow(int i)
{	
	int k;	
	CWindow *p;

	p = this->GetElem(i);
	ASSERT(p);
	
	if(p->IsActive())
		return OK;

	if(p->dwStyle & WS_NOACTIVE)
		return ERROR;

	p->dwStyle |= WS_ACTIVE;
	afx.wndmgr->RedrawTitleBar(i);

	for(k = 0; k < this->GetLen(); k++)
	{
		if(k != i)
		{
			p = this->GetElem(k);
			ASSERT(p);
			if(p->IsActive())
			{
				p->dwStyle &= (~WS_ACTIVE);
				afx.wndmgr->RedrawTitleBar(k);
			}
		}
	}

	return OK;
}

int CWndMgr::RecalcuAllRgn()
{
	int i;

	for(i = 0; i < this->GetLen(); i++)
	{
		this->RecalcuClipRgn(i);
	}

	return OK;
}

int CWndMgr::RecalcuClipRgn(int i)
{
	int k;

	CRect r;
	CWindow *p,*pwnd;
	
	pwnd = this->GetElem(i);
	ASSERT(pwnd);
	pwnd->privCDC->clip_rgn->Set(pwnd->rect);
	pwnd->privCDC->SetPos(pwnd->rect->left,pwnd->rect->top);

	for(k = 0; k < this->GetLen(); k++)
	{
		if(this->IsTopThan(k,i) && this->IsVisible(k))
		{
			p = this->GetElem(k);
			ASSERT(p);			
			pwnd->privCDC->SubtractClipRect(p->rect);
		}
	}

	return OK;
}

int CWndMgr::DrawAll()
{
	int i;
	CWindow *p;

	for(i = 0; i < this->GetLen(); i++)
	{
		p = this->GetElem(i);
		ASSERT(p);
		p->Draw();
	}

	return OK;
}

int CWndMgr::PostMessage(int wnd_id, u32 message, int wparam, int lparam)
{
	return afx.msgque->PostMessage(wnd_id,message,wparam,lparam);
}

int CWndMgr::SendMessage(int wnd_id, u32 message, int wparam, int lparam)
{
	CWindow *pwnd;

	pwnd = this->GetWindowByID(wnd_id);
	if(pwnd == NULL)
		return ERROR;
	return pwnd->SendMessage(message,wparam,lparam);
}

int CWndMgr::SetActiveWindow(CWindow *pwnd)
{
	return this->SetActiveWindow(this->WindowToIndex(pwnd));
}

int CWndMgr::GetActiveWindow()
{
	int i;
	CWindow *p;

	for(i = 0; i < this->GetLen(); i++)
	{
		p = this->GetElem(i);
		ASSERT(p);
		if(p->IsVisible() && p->IsActive())
			return i;
	}

	return -1;
}

int CWndMgr::GetFocusWindow()
{
	int i;
	CWindow *p;

	for(i = 0; i < this->GetLen(); i++)
	{
		p = this->GetElem(i);
		ASSERT(p);
		if(p->IsVisible() && p->HasFocus())
			return i;
	}

	return -1;
}

int CWndMgr::PtInWhich(int x, int y)
{
	int i;
	CWindow *p;

	for(i = 0; i < this->GetLen(); i++)
	{
		p = this->GetElem(i);
		ASSERT(p);

		if(p->PtVisible(x,y))
			return i;
	}
	return -1;
}

int CWndMgr::BringWindowToTop(int i)
{	
	if(i == this->top - 1)
		return ERROR;

	CWindow *p = this->RemoveElem(i);
	ASSERT(p);
	
	this->Push(p); //add to top;	
	
	this->RecalcuAllRgn();
	
	p->SendMessage(WM_PAINT,0,1);

	return OK;
}

int CWndMgr::BringWindowToTop(CWindow *pwnd)
{
	return this->BringWindowToTop(this->WindowToIndex(pwnd));
}

int CWndMgr::SetFocusWindow(int i)
{	
	int k;	
	CWindow *p;

	p = this->GetElem(i);
	ASSERT(p);
	
	if(p->HasFocus())
		return OK;
	
	if(p->dwStyle & WS_NOFOCUS)
		return ERROR;

	p->dwStyle |= WS_FOCUS;

	for(k = 0; k < this->GetLen(); k++)
	{
		if(k != i)
		{
			p = this->GetElem(k);
			ASSERT(p);
			if(p->HasFocus())
			{
				p->dwStyle &= (~WS_FOCUS);
			}
		}
	}

	return OK;
	
}

int CWndMgr::DestroyWindow(int win_id)
{
	CRect rect,rt;
	int i,index;
	CWindow *pwnd;

	index = this->IdToIndex(win_id);
	pwnd = this->GetElem(index);
	if(pwnd == NULL)
		return ERROR;

	rect.Copy(pwnd->rect);
	this->DelElem(index);

	this->RecalcuAllRgn();

	for(i = 0; i < this->GetLen(); i++)
	{
		pwnd = this->GetElem(i);
		ASSERT(pwnd);

		rt.Copy(&rect);
		pwnd->SR2LR(&rt);
		this->InvalidateRect(i,&rt,TRUE);
	}

	return OK;
}

int CWndMgr::InvalidateRect(int index, CRect *rect, int erase_back)
{
	CWindow *pwnd;
	CRect rc;

	pwnd = this->GetElem(index);
	if(pwnd == NULL)
		return ERROR;

	if(!pwnd->IsVisible())
		return ERROR;

	if(rect)
	{
		rc.Copy(rect);
		pwnd->LR2SR(&rc);
		pwnd->privCDC->Intersect(&rc);
	}

	pwnd->SendMessage(WM_PAINT,0,erase_back);	

	if(rect)
	{
		this->RecalcuClipRgn(index);
	}

	return OK;
}

int CWndMgr::IdToIndex(int win_id)
{
	int i;
	CWindow *p;

	for(i = 0; i < this->GetLen(); i++)
	{
		p = this->GetElem(i);
		ASSERT(p);

		if(p->id == win_id)
			return i;
	}

	return -1;
}

int CWndMgr::MoveWindow(int index, int x, int y, int w, int h)
{
	int i;
	CWindow *pwnd;
	CRect old_rc,rt;

	pwnd = this->GetElem(index);
	ASSERT(pwnd);

	old_rc.Copy(pwnd->rect); //save old rc;
	pwnd->rect->Set(x,y,x + w -1, y + h -1); //set new window rect

	if(old_rc.Equal(pwnd->rect))
		return OK;

	pwnd->SendMessage(WM_WINDOWPOSCHANGE,0,0);

	this->RecalcuAllRgn();
		
	for(i = 0; i < this->GetLen(); i++)
	{
		if(this->IsLowerThan(i,index))
		{
			pwnd = this->GetElem(i);
			ASSERT(pwnd);
			rt.Copy(&old_rc);

			pwnd->SR2LR(&rt);
			this->InvalidateRect(i,&rt,1);
		}	
	}

	this->InvalidateRect(index,NULL,1);

	return OK;
}

CWindow * CWndMgr::GetWindowByID(int win_id)
{
	return this->GetElem(this->IdToIndex(win_id));
}	

int CWndMgr::MoveWindow(int index, int x, int y)
{
	CWindow *pwnd;

	pwnd = this->GetElem(index);
	ASSERT(pwnd);

	return this->MoveWindow(index,x,y,pwnd->rect->GetWidth(),pwnd->rect->GetHeight());
}

int CWndMgr::RedrawTitleBar(int i)
{
	CRect rt;

	CWindow *pwnd = this->GetElem(i);
	if(pwnd == NULL)
		return ERROR;

	pwnd->GetTitleBarRect(&rt);
	pwnd->SR2LR(&rt);

	return this->InvalidateRect(i,&rt,1);
}

int CWndMgr::SetCaptureByIndex(int index)
{
	CWindow *pwnd = this->GetElem(index);
	ASSERT(pwnd);
	pwnd->dwStyle |= WS_CAPTURE;
	return OK;
}

int CWndMgr::ReleaseCaptureByIndex(int index)
{
	CWindow *pwnd = this->GetElem(index);
	ASSERT(pwnd);
	pwnd->dwStyle &= ~WS_CAPTURE;
	return OK;
}

int CWndMgr::GetCaptureWindowIndex()
{
	int i;
	CWindow *pwnd;

	for(i = 0; i < this->GetLen(); i++)
	{
		pwnd = this->GetElem(i);
		ASSERT(pwnd);
		if(pwnd->dwStyle & WS_CAPTURE)
			return i;
	}

	return -1;
}

int CWndMgr::HideWindowByIndex(int index)
{
	CRect rect,rt;
	int i;
	CWindow *pwnd;

	pwnd = this->GetElem(index);
	ASSERT(pwnd);
	
	if(!pwnd->IsVisible())
		return ERROR;

	pwnd->dwStyle &= (~WS_VISIBLE);

	rect.Copy(pwnd->rect);
	this->RecalcuAllRgn();

	for(i = 0; i < this->GetLen(); i++)
	{
		pwnd = this->GetElem(i);
		ASSERT(pwnd);

		rt.Copy(&rect);
		pwnd->SR2LR(&rt);
		this->InvalidateRect(i,&rt,TRUE);
	}

	return OK;
}

int CWndMgr::ShowWindowByIndex(int index)
{
	CWindow *pwnd;

	pwnd = this->GetElem(index);
	ASSERT(pwnd);

	if(pwnd->IsVisible())
		return ERROR;

	pwnd->dwStyle |= WS_VISIBLE;
	this->RecalcuAllRgn();
	pwnd->InvalidateRect(NULL,1);

	return OK;
}

int CWndMgr::DelChildWindowsByIndex(int index)
{
	CWindow *pwnd;
	int i,this_id;

	pwnd = this->GetElem(index);
	ASSERT(pwnd);
	this_id = pwnd->id;

	for(i = 0; i < this->GetLen(); i++)
	{
		pwnd = this->GetElem(i);
		ASSERT(pwnd);

		if(pwnd->pid > 0 && pwnd->pid == this_id)
		{
			pwnd->Close();
		}
	}

	return OK;
}

int CWndMgr::HideAllMenuWindows()
{
	CWindow *pwnd;
	int i;

	for(i = 0; i < this->GetLen(); i++)
	{
		pwnd = this->GetElem(i);
		ASSERT(pwnd);

		if(pwnd->IsMenuWnd())
		{
			pwnd->Hide();
		}
	}

	return OK;
}

int CWndMgr::CloseWindow(int win_id)
{
	return afx.wndmgr->SendMessage(win_id,WM_CLOSE,0,0);
}

int CWndMgr::SetWindowSize(int win_id, int w, int h)
{
	int i;
	CRect rc;
	
	i = this->IdToIndex(win_id);
	CWindow *pwnd = this->GetElem(i);
	
	ASSERT(pwnd);
	rc.Copy(pwnd->rect);
	
	rc.right =  rc.left + w - 1;
	rc.bottom = rc.top + h -1;

	if(rc.Equal(pwnd->rect))
		return ERROR;

	this->MoveWindow(i,rc.left,rc.top,w,h);

	return OK;
}

int CWndMgr::CloseAllAppWindows()
{
	CWindow *pwnd;
	int i;

	for(i = 0; i < this->GetLen(); i++)
	{
		pwnd = this->GetElem(i);
		ASSERT(pwnd);

		if(pwnd->IsAppWnd())
		{
			pwnd->Close();
		}
	}

	return OK;
}

int CWndMgr::ReposAllAppWindows()
{
	CWindow *pwnd;
	int i;
	CRect rc,rt;

	rc.Set(0,0,GAL_GetWidth(),GAL_GetHeight() - 20);

	for(i = 0; i < this->GetLen(); i++)
	{
		pwnd = this->GetElem(i);
		ASSERT(pwnd);

		if(pwnd->IsAppWnd())
		{
			rt.Copy(pwnd->rect);
			rc.CenterRect(rt.GetWidth(),rt.GetHeight(),&rt);
			this->MoveWindow(i,rt.left,rt.top);
		}
	}

	return OK;
}
