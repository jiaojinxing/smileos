#include "editbox.h"
#include "gui_misc.h"
#include "globals.h"

CEditBox::CEditBox()
{
    this->InitBasic();
}
CEditBox::~CEditBox()
{
    this->Destroy();
}
int CEditBox::InitBasic()
{
    CControl::InitBasic();

	this->can = NULL;
	this->sel_end = -1;
	this->sel_start = -1;	
	this->eb_style = 0;
	this->ins_pos = -1;
	this->i_file_txt = NULL;
	this->rect_txt = NULL;
	this->sb_horz = NULL;
	this->sb_vert = NULL;
	this->i_font = NULL;
	this->scroll_width = 10;
	this->total_lines = 0;
	this->drag_end_char = 0;
	this->drag_start_char = 0;
	this->sel_mode = FALSE;
	this->stk_lines = NULL;
	this->start_line = 0;
	this->call_back_load_file = NULL;
	this->call_back_param = NULL;
	this->loading = 0;

    return OK;
}
int CEditBox::Init()
{
    this->InitBasic();
    CControl::Init();    
	
	this->ctrl_name = CTRL_NAME_EDIT_BOX;
	this->back_color = COLOR_WHITE;
	this->i_font = afx.hz12; //default font

	NEW(this->stk_lines,CIntStk);
	this->stk_lines->Init(16*1024);

	NEW(this->can,CCanvas);
	this->can->Init();

	NEW(this->rect_txt,CRect);
	this->rect_txt->Init();

	NEW(this->sb_horz,CScrollBar);
	this->sb_horz->Init();
	this->sb_horz->SetDir(SBS_HORZ);

	NEW(this->sb_vert,CScrollBar);
	this->sb_vert->Init();
	this->sb_vert->SetDir(SBS_VERT);

    return OK;
}
int CEditBox::Destroy()
{
	DEL(this->stk_lines);
	DEL(this->sb_vert);
	DEL(this->sb_horz);
	DEL(this->rect_txt);
    DEL(this->can);

    CControl::Destroy();
    this->InitBasic();
    return OK;
}
int CEditBox::Copy(CEditBox *p)
{
    if(this == p)
        return OK;
    CControl::Copy(p);
    return OK;
}
int CEditBox::Comp(CEditBox *p)
{
    return 0;
}
int CEditBox::Print()
{
    CControl::Print();
    return TRUE;
}

int CEditBox::Draw(CDc *pdc)
{	
	CRect rc;

	ASSERT(pdc);

	if(this->HasHScroll())
		this->sb_horz->Draw(pdc);

	if(this->HasVScroll())
		this->sb_vert->Draw(pdc);

	this->GetViewRect(&rc);
	
	if(this->style & WS_BORDER)
	{
		pdc->SetPenColor(this->fore_color);
		pdc->DrawRect(&rc);
	}
	
	this->GetTextViewRect(&rc);
	pdc->SetBrushColor(this->back_color);
	pdc->FillRect(&rc);

	this->can->SetRect(&rc);
	this->can->SetDC(pdc);
	
	this->DrawText();

	return OK;
}

int CEditBox::HandleMessage(int message, int wparam, int lparam)
{
	int x,y;
	CRect r_txt;
	int char_pos;
	
	if(this->HasHScroll())
		this->sb_horz->HandleMessage(message,wparam,lparam);
	if(this->HasVScroll())
		this->sb_vert->HandleMessage(message,wparam,lparam);

	x = (short)LOWORD(lparam);	
	y = (short)HIWORD(lparam);

	this->GetTextViewRect(&r_txt);

	switch(message)
	{
		case WM_LBUTTONDOWN: 
			if(!r_txt.PtInRect(x,y))
				return ERROR;
			afx.wndmgr->SetCaptureByIndex(afx.wndmgr->IdToIndex(this->win_id));
			
			if(this->HasSelection())
			{
				this->ClearSel();
				this->Redraw();
			}

			this->can->SP2LP(&x,&y);
			char_pos = this->ToCharPos(x,y);
			this->SetCaret(char_pos);
	
			this->sel_mode = TRUE;
			this->drag_start_char = char_pos;
		break;

		case WM_LBUTTONUP: 	
			afx.wndmgr->ReleaseCaptureByIndex(afx.wndmgr->IdToIndex(this->win_id));
			this->sel_mode = FALSE;
		break;

		case WM_MOUSEMOVE:
			if(this->sel_mode)
			{
				if(y < r_txt.top)
				{
					this->ScrollY(y - r_txt.top);
					y = r_txt.top;
				}
				else if(y > r_txt.bottom)
				{
					this->ScrollY(y - r_txt.bottom);
					y = r_txt.bottom;
				}

				this->can->SP2LP(&x,&y);
				char_pos = this->ToCharPos(x,y);

				if(char_pos != this->drag_end_char)
				{
					this->SetCaret(char_pos);
					this->drag_end_char = char_pos;
					this->SetSel(this->drag_start_char,this->drag_end_char);
					this->Redraw();
				}
			}
		break;

		case WM_COMMAND:
			if(this->sb_vert->id == lparam)
			{
				this->ScrollViewY(this->sb_vert->GetPos()-this->can->view_y);
			}
		break;
	}
	return ERROR;
}

int CEditBox::SetTextRect(int w, int h)
{
	return this->rect_txt->Set_1(0,0,w,h);
}

int CEditBox::ReadLine(CFileBase *file)
{
	ASSERT(this->i_file_txt && file && this->i_font);
	
	char ch[3];	
	int left,ret;
	int cx;

	if(this->i_file_txt->IsEnd())
		return ERROR;

	left = this->rect_txt->GetWidth();

	file->SetSize(0);
	while(!this->i_file_txt->IsEnd())
	{
		ret = this->i_file_txt->Getc_GB(ch);
		if(ch[0] == '\n')break;
		if(ch[0] == '\r')continue;

		cx = this->i_font->GetCharWidth(ch);
		if(left - cx < 0)
		{
			this->i_file_txt->SeekBack(ret);
			break;
		}

		file->Puts(ch);
		left -= cx;
	}
	
	file->Putc(0);

	return OK;
}

int CEditBox::SetRect(int x, int y, int w, int h)
{
	CRect rc;

	CControl::SetRect(x,y,w,h);

	if(this->HasHScroll())
	{
		this->sb_horz->SetRect(x,this->rect->bottom - scroll_width,w - scroll_width,scroll_width);
	}

	if(this->HasVScroll())
	{
		if(this->HasHScroll())
			this->sb_vert->SetRect(this->rect->right - scroll_width,y,scroll_width,h - scroll_width);
		else
			this->sb_vert->SetRect(this->rect->right - scroll_width,y,scroll_width,h);
	}

	this->GetTextViewRect(&rc);
	this->SetTextRect(rc.GetWidth(),rc.GetHeight());

	return OK;
}

int CEditBox::HasVScroll()
{
	return (this->style & WS_VSCROLL);
}

int CEditBox::HasHScroll()
{
	return (this->style & WS_HSCROLL);
}

int CEditBox::OnAdded()
{
	this->sb_horz->win_id = this->win_id;
	this->sb_vert->win_id = this->win_id;
	return OK;
}

int CEditBox::GetViewRect(CRect *r)
{
	ASSERT(r);

	r->Copy(this->rect);

	if(this->HasHScroll())
		r->bottom -= (this->scroll_width+1);
	if(this->HasVScroll())
		r->right -= (this->scroll_width +1);

	return OK;
}

int CEditBox::GetTextViewRect(CRect *r)
{
	ASSERT(r);

	this->GetViewRect(r);

	r->left++;
	r->top++;
	r->right --;
	r->bottom --;

	return OK;
}

int CEditBox::SetTextFile(CFileBase *file)
{
	ASSERT(file);

	CMem mem;

	this->i_file_txt = file;
	LOCAL_MEM(mem);
	
	this->loading = 1;
	this->total_lines = 0;
	this->i_file_txt->Seek(0);
	
	this->stk_lines->Clear();
	this->stk_lines->Push(0); //always start from 0

	while(this->ReadLine(&mem))
	{
		this->stk_lines->Push(this->i_file_txt->GetOffset());
		if(this->call_back_load_file)
			this->call_back_load_file(this->call_back_param,this->total_lines);

		this->total_lines++;
	}
	
	this->start_line = 0;
	this->SetSel(-1,-1);
	this->can->SetViewPos(0,0);
	this->RefreshScrollBar();

	this->loading = 0;
	this->Redraw();

	return OK;
}

int CEditBox::GetVisibleLines()
{
	CRect rc;

	ASSERT(this->i_font);
	ASSERT(this->i_font->GetMaxCharHeight() > 0);

	this->GetTextViewRect(&rc);

	return rc.GetHeight() / this->i_font->GetMaxCharHeight() + 1;
}

int CEditBox::GetLineHeight()
{
	ASSERT(this->i_font);
	return this->i_font->GetMaxCharHeight();
}

int CEditBox::DrawText()
{
	ASSERT(this->i_file_txt && this->i_font);

	if(this->loading)
		return ERROR;

	CMem mem;
	int y_pos,ret,off,i;

	LOCAL_MEM(mem);

	this->can->SetTextColor(this->fore_color);
	y_pos = this->GetYPos(this->start_line);

	for(i = 0; i < this->GetVisibleLines(); i++)
	{
		off = this->GetLineOffset(this->start_line + i);
		if(off < 0)
			break;

		ret = this->GetLine(this->start_line + i,&mem);
		if(ret == ERROR)
			break;
		
		this->DrawLineText(y_pos,off,&mem);

		y_pos += this->GetLineHeight();
	}
	
	return OK;
}

int CEditBox::ScrollViewY(int cy)
{
	this->can->view_y += cy;

	this->start_line = this->ViewYToLine(this->can->view_y);
	this->SetCaret(this->ins_pos);
	this->Redraw();

	return OK;
}


int CEditBox::GetMaxCharsPerLine()
{
	CRect rc;
	ASSERT(this->i_font);

	return this->rect_txt->GetWidth() / this->i_font->GetMinCharWidth();
}

int CEditBox::SetSel(int start, int end)
{
	if(start > end)
	{
		this->sel_start = end;
		this->sel_end = start;
	}
	else
	{
		this->sel_start = start;
		this->sel_end = end;
	}

	return OK;
}

int CEditBox::ToCharPos(int x, int y)
{
	ASSERT(this->i_file_txt && this->i_font);

	CMem mem;
	int wt,off,r,w,t,line;
	char ch[3];

	LOCAL_MEM(mem);
	
	line = this->ViewYToLine(y);
	this->GetLine(line,&mem);
	off = this->GetLineOffset(line);
	
	t = 0; wt = 0;
	mem.Seek(0);
	while(!mem.IsEnd())
	{
		r = mem.Getc_GB(ch);
		w = this->i_font->GetCharWidth(ch);

		if(ch[0] == 0)
			break;
		if(wt <= x && (wt+w) > x)
			break;

		wt += w;
		t += r;
	}

	return off + t;
}

int CEditBox::AdjustCharPos(int char_pos)
{
	ASSERT(this->i_file_txt);
	
	int line,off,i,c = 0;
	BYTE ch;
	CMem mem;

	LOCAL_MEM(mem);

	line = this->GetLineFromCharPos(char_pos);
	off = this->GetLineOffset(line);
	this->GetLine(line,&mem);
	
	return off + mem.AdjustGbPos(char_pos - off);
}
int CEditBox::DrawLineText(int y_pos, int start_char_pos, CMem *str)
{
	ASSERT(this->i_font);

	int cx,cy,pos1,pos2,x_pos=0;
	char ch;
	CRect rc,rt;

	pos1 = this->sel_start - start_char_pos;
	pos2 = this->sel_end - start_char_pos;

	if(pos1 >= pos2)
	{
		this->can->SetTextColor(this->fore_color);
		this->can->TextOut(0,y_pos,str->p,NULL);
		return OK;
	}

	if(pos1 > 0 && pos1 < str->GetSize())
	{
		ch = str->CharAt(pos1);
		str->SetChar(pos1,0);
		this->i_font->GetTextSize(str->p,&cx,&cy);
		str->SetChar(pos1,ch);

		rc.Set(0,y_pos,cx - 1,y_pos + this->GetLineHeight() - 1);
		this->can->TextOut(0,y_pos,str->p,&rc);
		x_pos += cx;
	}
	
	if(pos2 > 0)
	{
		if(pos1 < 0) pos1 = 0;
		if(pos1 < str->GetSize())
		{
			if(pos2 < str->GetSize())
			{
				ch = str->CharAt(pos2);
				str->SetChar(pos2,0);
				this->i_font->GetTextSize(str->p + pos1,&cx,&cy);
				str->SetChar(pos2,ch);
			}
			else
			{	
				this->i_font->GetTextSize(str->p + pos1,&cx,&cy);
			}
			
			rc.Set(x_pos,y_pos,x_pos + cx - 1, y_pos + this->GetLineHeight() - 1);
			
			this->can->SetBrushColor(COLOR_BLUE);
			this->can->FillRect(&rc);
			this->can->SetTextColor(COLOR_WHITE);
			this->can->TextOut(0,y_pos,str->p,&rc);
			x_pos += cx;
		}
	}
	
	rc.Set_1(x_pos,y_pos,x_pos + this->rect_txt->GetWidth(),y_pos + this->GetLineHeight());

	this->can->SetTextColor(this->fore_color);
	this->can->TextOut(0,y_pos,str->p,&rc);

	return OK;
}

int CEditBox::SetCaret(int char_pos)
{
	int x,y;
	CRect rc,rt;

	if(char_pos < 0)
		char_pos = 0;

	this->ins_pos = this->AdjustCharPos(char_pos);
	this->ToPos(this->ins_pos,&x,&y);
	this->can->LP2SP(&x,&y);

	rc.Set(x,y,x+afx.caret->width-1,y+afx.caret->height-1);

	if(!rc.CoveredBy(this->can->rect))
	{
		afx.caret->StopBlink();
		return ERROR;
	}

	afx.caret->win_id = this->win_id;
	afx.caret->SetPos(x,y);
	afx.caret->Show();
	afx.caret->StartBlink();

	return OK;
}
int CEditBox::ToPos(int char_pos, int *x, int *y)
{
	ASSERT(this->i_file_txt && this->i_font);

	CMem mem;
	int line,wt,t,w,r;
	char ch[3];

	LOCAL_MEM(mem);

	line = this->GetLineFromCharPos(char_pos);
	if(line < 0)
		return ERROR;

	this->GetLine(line,&mem);
	
	wt = 0; t = this->GetLineOffset(line);
	mem.Seek(0);		

	while(!mem.IsEnd())
	{
		r = mem.Getc_GB(ch);
		w = this->i_font->GetCharWidth(ch);

		if(ch[0] == 0)
			break;
		if(t <= char_pos && (t + r) > char_pos)
			break;
		wt += w;
		t += r;
	}
	*y = this->GetYPos(line);
	*x = wt;
	return OK;
}

int CEditBox::CharVisible(int char_pos)
{
	int x,y;
	CRect rc;

	this->GetTextViewRect(&rc);

	this->ToPos(char_pos,&x,&y);

	if(y < this->can->view_y)
	{
		this->ScrollY(y - this->can->view_y);
	}

	if(y > this->can->view_y + rc.GetHeight() -  this->GetLineHeight())
	{
		this->ScrollY(y - this->can->view_y - rc.GetHeight() + this->GetLineHeight());
	}

	return OK;
}

int CEditBox::ScrollY(int cy)
{
	if(this->HasVScroll())
		this->sb_vert->Scroll(cy);
	else
		this->ScrollViewY(cy);

	return OK;
}

int CEditBox::HasSelection()
{
	return this->GetSelSize() > 0;
}

int CEditBox::RepSel(CFileBase *file)
{
	ASSERT(file && this->i_file_txt);
	return OK;
}

int CEditBox::GetSelSize()
{
	return this->sel_end - this->sel_start;
}

int CEditBox::RefreshScrollBar()
{
	if(this->HasVScroll())
	{
		int lh = this->GetLineHeight();

		this->sb_vert->SetMinMax(0,(this->total_lines + 1) * lh);
		this->sb_vert->SetPageSize(this->GetVisibleLines() * lh);
		this->sb_vert->SetPageStep(this->GetVisibleLines() * lh);
		this->sb_vert->SetLineStep(lh);
		this->sb_vert->SetPos(0);
	}

	return OK;
}

int CEditBox::RepSel(char *str)
{
	CMem mem;

	mem.Init();
	mem.SetP(str,strlen(str));
	return this->RepSel(&mem);
}

int CEditBox::ClearSel()
{
	return this->SetSel(0,0);
}

int CEditBox::GetLineOffset(int line_index)
{
	if(line_index < 0 || line_index >= this->total_lines)
		return -1;
	return this->stk_lines->GetElem(line_index);
}


int CEditBox::GetLine(int line, CFileBase *file)
{
	ASSERT(file && this->i_file_txt);
	int off;

	file->SetSize(0);
	if(this->loading)
	{
		file->Putc(0);
		return ERROR;
	}

	off = this->GetLineOffset(line);
	if(off < 0)
	{
		file->Putc(0);
		return ERROR;
	}

	this->i_file_txt->Seek(off);	
	return this->ReadLine(file);
}

int CEditBox::GetYPos(int line)
{
	return line * this->GetLineHeight();
}

int CEditBox::ViewYToLine(int view_y)
{
	if(view_y <= 0)
		return 0;
	return view_y / this->GetLineHeight();
}

int CEditBox::GetLineFromCharPos(int char_pos)
{
	int ff,pos;
	return this->stk_lines->BSearch_Pos(char_pos,0,&ff) - 1;
}
