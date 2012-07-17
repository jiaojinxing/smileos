#include "draginfo.h"
#include "globals.h"

CDragInfo::CDragInfo()
{
    this->InitBasic();
}
CDragInfo::~CDragInfo()
{
    this->Destroy();
}
int CDragInfo::InitBasic()
{
	this->x = 0;
	this->y = 0;
	this->width = 0;
	this->height = 0;
	this->off_x = 0;
	this->off_y = 0;
	this->old_x = 0;
	this->old_y = 0;
	this->on_drag = FALSE;
	this->win_id = 0;

    return OK;
}
int CDragInfo::Init()
{
    this->InitBasic();
    //add your code
    return OK;
}
int CDragInfo::Destroy()
{
    //add your code
    this->InitBasic();
    return OK;
}
int CDragInfo::Print()
{
    return TRUE;
}

int CDragInfo::StartDrag(CWindow *pwnd, int x, int y)
{
	ASSERT(pwnd);

	afx.caret->StopBlink(); //when start drag, stop caret first

	this->x = x;
	this->y = y;

	this->width = pwnd->rect->GetWidth();
	this->height = pwnd->rect->GetHeight();

	this->old_x = this->x;
	this->old_y = this->y;

	this->off_x = x - pwnd->rect->left;
	this->off_y = y - pwnd->rect->top;

	this->win_id = pwnd->id;

	this->on_drag = TRUE;
	
	return OK;
}

int CDragInfo::EndDrag()
{
	this->on_drag = FALSE;
	return OK;
}

int CDragInfo::SetPos(int x, int y)
{
	this->old_x = this->x;
	this->old_y = this->y;

	this->x = x;
	this->y = y;
	
	return OK;
}


int CDragInfo::DrawXORect()
{
	GAL_SetClipping(0,0,GAL_GetWidth(),GAL_GetHeight());
	GAL_DrawRect_XOR(this->x - this->off_x,this->y - this->off_y,this->width,this->height);
	return OK;
}

int CDragInfo::DrawXORectAtOldPos()
{
	GAL_SetClipping(0,0,GAL_GetWidth(),GAL_GetHeight());
	GAL_DrawRect_XOR(this->old_x - this->off_x,this->old_y - this->off_y,this->width,this->height);
	return OK;
}

