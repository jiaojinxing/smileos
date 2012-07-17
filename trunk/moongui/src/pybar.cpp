#include "pybar.h"
#include "gui_misc.h"
#include "globals.h"
#include "colormap.h"

CPyBar::CPyBar()
{
    this->InitBasic();
}
CPyBar::~CPyBar()
{
    this->Destroy();
}
int CPyBar::InitBasic()
{
    CControl::InitBasic();
	this->mem_hz = NULL;
	this->mem_input = NULL;
	this->can = NULL;
	this->visible_lines = 2;
	this->i_font = NULL;
	this->call_back_hz_item = NULL;
	this->call_back_param = NULL;
	this->cur_sel = -1;

    return OK;
}
int CPyBar::Init()
{
    this->InitBasic();
    CControl::Init();
    this->back_color = COLOR_WHITE;

	NEW(this->mem_hz,CMem);
	this->mem_hz->Malloc(2048);
	this->mem_hz->Putc(0);

	NEW(this->mem_input,CMem);
	this->mem_input->Malloc(256);
	this->mem_input->Putc(0);

	NEW(this->can,CCanvas);
	this->can->Init();

    return OK;
}
int CPyBar::Destroy()
{
	DEL(this->can);
    DEL(this->mem_hz);
	DEL(this->mem_input);
    CControl::Destroy();
    this->InitBasic();
    return OK;
}
int CPyBar::Copy(CPyBar *p)
{
    if(this == p)
        return OK;
    CControl::Copy(p);
    return OK;
}
int CPyBar::Comp(CPyBar *p)
{
    return 0;
}
int CPyBar::Print()
{
    CControl::Print();
    return TRUE;
}

int CPyBar::Draw(CDc *pdc)
{	
	CRect rc;

	ASSERT(pdc);

	pdc->SetBrushColor(this->back_color);
	pdc->FillRect(this->rect);
	pdc->SetPenColor(this->fore_color);
	pdc->DrawRect(this->rect);

	this->can->SetDC(pdc);
	this->GetViewRect(&rc);
	this->can->SetRect(&rc);

	if(!this->IsInputBufEmpty())
	{
		this->can->SetTextColor(COLOR_RED);
		this->can->TextOut(0,0,this->mem_input->p,NULL);
	}

	if(!this->IsHzBufEmpty())
	{
		this->DrawAllHz();
	}
	return OK;
}

int CPyBar::HandleMessage(int message, int wparam, int lparam)
{
	int x,y;
	CWindow *pwnd;

	switch(message)
	{
		case WM_LBUTTONDOWN: 
			x = (short)LOWORD(lparam);	
			y = (short)HIWORD(lparam);

			if(!this->PtInCtrl(x,y))
				return ERROR;
			
			this->cur_sel = this->GetHzIndexByPos(x,y);
			if(this->cur_sel >= 0)
				this->Redraw();
		break;

		case WM_LBUTTONUP: 
			if(this->cur_sel >= 0)
			{
				afx.wndmgr->SendMessage(this->win_id,WM_COMMAND,0,this->id);
				this->ClearInputBuf();
				this->Redraw();
			}
		break;

		case WM_MOUSEMOVE:
		break;

		case WM_MOUSEOUT:
		break;
	}
	return ERROR;
}


int CPyBar::GetViewRect(CRect *rc)
{
	rc->Copy(this->rect);
	rc->left ++;
	rc->top ++;
	rc->right --;
	rc->bottom --;

	return OK;
}

int CPyBar::AddChar(char ch)
{
	char str[2];

	str[1] = 0;
	str[0] = ch;
	
	if(ch == 8)
	{
		this->mem_input->SetChar(this->mem_input->GetSize() - 2,0);
		this->mem_input->SetSize(this->mem_input->GetSize() - 1);
	}
	else
	{
		this->mem_input->StrCat(str);
	}
	return OK;
}

int CPyBar::ClearInputBuf()
{
	this->mem_input->SetSize(0);
	this->mem_input->Putc(0);
	this->mem_hz->SetSize(0);
	this->mem_hz->Putc(0);
	this->start_line = 0;
	this->cur_sel = -1;

	return OK;
}

int CPyBar::SearchPos()
{
	return afx.py_table->Search(this->mem_input->p);
}

int CPyBar::Search()
{

	int pos = this->SearchPos();
	
	if(this->mem_input->p[0] == 0)
	{
		this->mem_hz->SetSize(0);
		return OK;
	}
	
	if(pos >= 0 && pos < afx.py_table->GetMax())
	{
		afx.py_table->GetItem(pos,this->mem_hz);
		this->start_line = 0;
	}
	else
	{
		this->mem_hz->SetSize(0);
	}
	

	return OK;
}

int CPyBar::GetAllHzItem()
{
	ASSERT(this->i_font);
	
	int row = 0,col = 0,index=0;
	int cx,cy,add_w,left,old_off;
	int pos_x = 0,pos_y;
	CRect rc,rt;
	CMem mem;
	
	LOCAL_MEM(mem);
	
	this->GetViewRect(&rc);
	left = rc.GetWidth();
	
	this->i_font->GetTextSize("1.",&cx,&cy);
	add_w = cx;
	
	pos_y = this->i_font->GetMaxCharHeight();

	this->mem_hz->Seek(0);
	this->mem_hz->ReadString(&mem); //skip first pinyin

	while(!this->mem_hz->IsEnd())
	{
		old_off = this->mem_hz->GetOffset();
		
		this->mem_hz->ReadString(&mem);
		if(mem.p[0] == 0)
			break;

		this->i_font->GetTextSize(mem.p,&cx,&cy);
		cx += add_w;
		if( left - cx <= 0)
		{
			col = 0;
			row ++;
			pos_x = 0;
			pos_y += this->i_font->GetMaxCharHeight();
			left = rc.GetWidth();
		}		

		rt.Set(pos_x,pos_y,pos_x + cx - 1, pos_y + this->i_font->GetMaxCharHeight() - 1);
		
		if(this->call_back_hz_item)
		{
			if( this->call_back_hz_item(this->call_back_param,index,&mem,&rt,row,col) == FALSE )
				break;
		}

		pos_x += cx;		
		index++;col++;
		left -= cx;
	}
	
	return OK;
}

int CPyBar::call_back_draw_all_hz_item(void *param, int index, CMem *hz, CRect *rc, int row, int col)
{
	ASSERT(param);

	CPyBar *self;
	CMem mem;
	CRect rt;

	LOCAL_MEM(mem);
	self = (CPyBar*)param;
	
	if(row < self->start_line || row >= self->start_line + self->visible_lines)
		return TRUE;
	
	row -= self->start_line;

	if(row == 0 && col >= 0 && col < 9)
		mem.Printf("%d.",col+1);
	else
		mem.Printf("  ",col+1);

	mem.Puts(hz->p);
	mem.Putc(0);
	
	rt.Copy(rc);
	rt.top = rc->top - self->i_font->GetMaxCharHeight() * self->start_line;
	rt.bottom = rt.top + self->i_font->GetMaxCharHeight();

	if(index == self->cur_sel)
	{
		self->can->SetBrushColor(COLOR_BLUE);
		self->can->FillRect(&rt);
		self->can->SetTextColor(COLOR_WHITE);
		self->can->TextOut(rt.left,rt.top,mem.p,NULL);
	}
	else
	{
		self->can->SetTextColor(self->fore_color);
		self->can->TextOut(rt.left,rt.top,mem.p,NULL);
	}

	return TRUE;
}

int CPyBar::DrawAllHz()
{
	this->call_back_param = this;
	this->call_back_hz_item = this->call_back_draw_all_hz_item;

	this->GetAllHzItem();

	return OK;
}
int CPyBar::call_back_get_hz_by_num(void *param,int index,CMem *hz,CRect *rc,int row,int col)
{
	int num,*_p;
	CFileBase *file;

	_p = (int*)param;

	CPyBar *self = (CPyBar*)(_p[0]);
	ASSERT(self);

	row -= self->start_line;
	
	num = _p[2];
	file = (CFileBase *)_p[1];
	ASSERT(file);

	if(row == 0 && col == num)
	{
		file->SetSize(0);
		file->Puts(hz->p);
		file->Putc(0);

		return FALSE;
	}

	return TRUE;
}

int CPyBar::call_back_get_max_line(void *param,int index,CMem *hz,CRect *rc,int row,int col)
{
	int *_p = (int*)param;

	ASSERT(param);

	if(row > *_p)
		*_p = row;

	return OK;
}
int CPyBar::GetHzByNum(int num, CFileBase *file)
{
	ASSERT(file);
	
	int param[3];

	param[0] = (int)this;
	param[1] = (int)file;
	param[2] = num;

	file->SetSize(0);
	this->call_back_param = param;
	this->call_back_hz_item = this->call_back_get_hz_by_num;
	this->GetAllHzItem();
	file->Putc(0);

	return OK;
}

int CPyBar::ScrollDown()
{
	int max = this->GetMaxLine();

	this->start_line ++;
	if(this->start_line >= max)
		this->start_line = max - 1;

	this->Redraw();

	return OK;
}

int CPyBar::ScrollUp()
{
	this->start_line --;
	if(this->start_line < 0)
		this->start_line = 0;

	this->Redraw();

	return OK;
}

int CPyBar::GetMaxLine()
{
	int max = 0;
	
	this->call_back_param = &max;
	this->call_back_hz_item = this->call_back_get_max_line;

	this->GetAllHzItem();
	
	return max + 1;
}

int CPyBar::IsInputBufEmpty()
{
	return this->mem_input->p[0] == 0;
}

int CPyBar::call_back_get_hz_by_pos(void *param,int index,CMem *hz,CRect *rc,int row,int col)
{
	int *_p,x,y,*pi;
	
	_p = (int*)param;

	x = _p[0]; y = _p[1];
	pi = (int*)_p[2];

	if(rc->PtInRect(x,y))
	{
		*pi = index;
		return FALSE;
	}
	
	return TRUE;
}
int CPyBar::GetHzIndexByPos(int x, int y)
{
	ASSERT(this->i_font);

	int param[3],index = -1;
	
	y += this->i_font->GetMaxCharHeight() * this->start_line;
	this->can->SP2LP(&x,&y);

	param[0] = x;
	param[1] = y;
	param[2] = (int)(&index);

	this->call_back_param = param;
	this->call_back_hz_item = this->call_back_get_hz_by_pos;

	this->GetAllHzItem();

	return index;
}

int CPyBar::call_back_get_hz_by_index(void *param,int index,CMem *hz,CRect *rc,int row,int col)
{
	int *_p,i;
	
	_p = (int*)param;
	CFileBase *file;

	i = _p[0];
	
	
	if(i == index)
	{
		file = (CFileBase *)_p[1];
		file->WriteFile(hz);
		return FALSE;		
	}

	return TRUE;
}
int CPyBar::GetHzByIndex(int index, CFileBase *file)
{
	ASSERT(file);
	int param[2];

	param[0] = index;
	param[1] = (int)file;

	file->SetSize(0);
	this->call_back_param = param;
	this->call_back_hz_item = this->call_back_get_hz_by_index;
	this->GetAllHzItem();
	file->Putc(0);

	return OK;
}

int CPyBar::IsHzBufEmpty()
{
	return this->mem_hz->p[0] == 0;
}
