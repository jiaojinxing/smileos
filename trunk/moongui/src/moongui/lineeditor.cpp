#include "lineeditor.h"
#include "gui_misc.h"
#include "globals.h"

CLineEditor::CLineEditor()
{
    this->InitBasic();
}
CLineEditor::~CLineEditor()
{
    this->Destroy();
}
int CLineEditor::InitBasic()
{
    CControl::InitBasic();
	
	this->mem_txt = NULL;
	this->can = NULL;
	this->sel_end = -1;
	this->sel_start = -1;
	this->ins_pos = 0;
	this->sel_mode = FALSE;
	this->start_char_pos = -1;
	this->end_char_pos = -1;
	this->content_menu = NULL;

    return OK;
}
int CLineEditor::Init()
{
    this->InitBasic();
    CControl::Init();
    
	this->ctrl_name = CTRL_NAME_LINE_EDITOR;
	this->back_color = COLOR_WHITE;

	this->content_menu = CMenuWnd::CreateMenuWnd(this->id);

	NEW(this->i_copy,CMenuItem);
	this->i_copy->Init();
	this->i_copy->SetName((char*)"¸´ÖÆ");

	NEW(this->i_paste,CMenuItem);
	this->i_paste->Init();
	this->i_paste->SetName((char*)"Õ³Ìù");

	NEW(this->i_cut,CMenuItem);
	this->i_cut->Init();
	this->i_cut->SetName((char*)"¼ôÇÐ");

	NEW(this->i_selall,CMenuItem);
	this->i_selall->Init();
	this->i_selall->SetName((char*)"È«Ñ¡");

	this->content_menu->AddMenuItem(i_copy);
	this->content_menu->AddMenuItem(i_paste);
	this->content_menu->AddMenuItem(i_cut);
	this->content_menu->AddSeparator();
	this->content_menu->AddMenuItem(i_selall);

	NEW(this->can,CCanvas);
	this->can->Init();

	NEW(this->mem_txt,CMem);
	this->mem_txt->Init();
	this->mem_txt->Malloc(1024);
	this->mem_txt->Putc(0);

    return OK;
}
int CLineEditor::Destroy()
{
    DEL(this->can);
	DEL(this->mem_txt);
	this->content_menu->Close();

    CControl::Destroy();
    this->InitBasic();
    return OK;
}
int CLineEditor::Copy(CLineEditor *p)
{
    if(this == p)
        return OK;
    CControl::Copy(p);
    return OK;
}
int CLineEditor::Comp(CLineEditor *p)
{
    return 0;
}
int CLineEditor::Print()
{
    CControl::Print();
    return TRUE;
}

int CLineEditor::Draw(CDc *pdc)
{	
	ASSERT(pdc);
	this->can->SetDC(pdc);
	
	this->DrawText();

	if(this->style & WS_BORDER)
	{
		pdc->SetPenColor(this->fore_color);
		pdc->DrawRect(this->rect);
	}
	else if( this->style & WS_STATICEDGE)
	{
		Draw3dThinDownBorder(pdc,this->rect);
	}

	return OK;
}

int CLineEditor::HandleMessage(int message, int wparam, int lparam)
{
	int x,y,char_pos;

	switch(message)
	{
		case WM_LBUTTONDOWN: 
			x = (short)LOWORD(lparam);	
			y = (short)HIWORD(lparam);

			if(!this->PtInCtrl(x,y))
				return ERROR;
			
			afx.ctrlmgr->SetFocus(this->id);

			this->can->SP2LP(&x,&y);
			char_pos = this->ToCharPos(x);
			
			afx.wndmgr->SetCaptureByIndex(afx.wndmgr->IdToIndex(this->win_id));

			if(!this->InSelection(char_pos))
			{
				this->SetSel(-1,-1);
				this->Redraw();
				this->SetCaret(char_pos);
				this->sel_mode = TRUE;
				this->start_char_pos = char_pos;
			}

		break;

		case WM_LBUTTONUP: 
			afx.wndmgr->ReleaseCaptureByIndex(afx.wndmgr->IdToIndex(this->win_id));

			if(this->sel_mode)
			{
				this->sel_mode = FALSE;
				return OK;
			}
			
			x = LOWORD(lparam);	
			y = HIWORD(lparam);
			
			if(!this->PtInCtrl(x,y))
				return ERROR;
		
			this->can->SP2LP(&x,&y);
			char_pos = this->ToCharPos(x);

			if(this->InSelection(char_pos))
			{
				this->SetSel(-1,-1);
				this->Redraw();
				this->SetCaret(char_pos);
			}

		break;

		case WM_MOUSEMOVE:
			x = (short)LOWORD(lparam);	
			y = (short)HIWORD(lparam);

			if(this->sel_mode)
			{
				this->can->SP2LP(&x,&y);
				char_pos = this->ToCharPos(x);
				this->end_char_pos = char_pos;

				this->SetSel(this->start_char_pos,this->end_char_pos);
				this->Redraw();
				this->SetCaret(char_pos);
				this->InsPosVisible();
			}
		break;

		case WM_MOUSEHOLD:
			x = (short)LOWORD(lparam);	
			y = (short)HIWORD(lparam);

			if(!this->PtInCtrl(x,y))
				return ERROR;
			//afx.wndmgr->ReleaseCapture(afx.wndmgr->IdToIndex(this->win_id));
			//this->content_menu->ShowAtMouse();
		break;

		case WM_CHAR:
			if(this->HasFocus())
				this->OnChar(wparam,lparam);
		break;
	}
	return ERROR;
}

int CLineEditor::GetTextRect(CRect *r)
{
	ASSERT(r);

	r->Copy(this->rect);

	r->left ++; 
	r->top ++;
	r->right --;
	r->bottom --;

	return OK;
}

int CLineEditor::SetText(char *txt)
{
	this->mem_txt->SetSize(0);
	this->mem_txt->Puts(txt);
	this->mem_txt->Putc(0);
	this->SetCaret(0);
	this->SetSel(-1,-1);
	this->can->SetViewPos(0,0);
	this->Redraw();
	return OK;
}

int CLineEditor::DrawText()
{
	CRect r_txt;
	int y_pos,s_pos,e_pos,draw_pos;
	int cx,cy,x,y;
	char ch;

	this->GetTextRect(&r_txt);	
	this->can->SetRect(&r_txt);
	
	y_pos = ( r_txt.GetHeight() - can->i_dc->i_font->GetMaxCharHeight() ) / 2;
	
	this->mem_txt->p[this->mem_txt->GetMaxSize() - 1] = 0; //make sure the last char is 0

	s_pos = this->sel_start;
	e_pos = this->sel_end;

	draw_pos = 0;

	if(this->mem_txt->p[0] == 0)
		goto end;

	ch = this->mem_txt->CharAt(s_pos);
	this->mem_txt->SetChar(s_pos,0);
	this->can->i_dc->i_font->GetTextSize(this->mem_txt->p,&cx,&cy);
	this->can->SetBrushColor(this->back_color);
	this->can->FillRect(draw_pos,0,cx,this->can->rect->GetHeight());
	this->can->SetTextColor(this->fore_color);
	this->can->TextOut(draw_pos,y_pos,this->mem_txt->p,NULL);
	draw_pos += cx;
	this->mem_txt->SetChar(s_pos,ch);
	
	ch = this->mem_txt->CharAt(e_pos);
	this->mem_txt->SetChar(e_pos,0);
	this->can->i_dc->i_font->GetTextSize(this->mem_txt->p + s_pos,&cx,&cy);
	this->can->SetBrushColor(COLOR_BLUE);
	this->can->FillRect(draw_pos,0,cx,this->can->rect->GetHeight());
	this->can->SetTextColor(COLOR_WHITE);
	this->can->TextOut(draw_pos,y_pos,this->mem_txt->p + s_pos,NULL);
	draw_pos += cx;
	this->mem_txt->SetChar(e_pos,ch);
	
	this->can->i_dc->i_font->GetTextSize(this->mem_txt->p + e_pos,&cx,&cy);
	this->can->SetBrushColor(this->back_color);
	this->can->FillRect(draw_pos,0,cx,this->can->rect->GetHeight());
	this->can->SetTextColor(this->fore_color);
	this->can->TextOut(draw_pos,y_pos,this->mem_txt->p + e_pos,NULL);
	draw_pos += cx;

end:	
	x = this->can->rect->right; y = this->can->rect->top; 
	this->can->SP2LP(&x,&y);
	this->can->SetBrushColor(this->back_color);
	this->can->FillRect(draw_pos,0,x - draw_pos + 1,this->can->rect->GetHeight());

	return OK;
}

int CLineEditor::AdjustCharPos(int pos)
{
	return this->mem_txt->AdjustGbPos(pos);
}

int CLineEditor::SetSel(int start, int end)
{
	if(end > start)
	{
		this->sel_start = start;
		this->sel_end = end;
	}
	else
	{
		this->sel_end = start;
		this->sel_start = end;
	}

	this->sel_start = this->AdjustCharPos(this->sel_start);
	this->sel_end = this->AdjustCharPos(this->sel_end);

	return OK;
}

int CLineEditor::SetInsPos(int p)
{
	this->ins_pos = this->AdjustCharPos(p);
	return OK;
}

int CLineEditor::ToCharPos(int x)
{
	char ch[3];
	int t = 0,wt = 0,r,w;

	if(x < 0)
		return  -1;

	this->mem_txt->Seek(0);
	while(!this->mem_txt->IsEnd())
	{
		r = this->mem_txt->Getc_GB(ch);
		if(ch[0] == 0)
			break;
		w = this->can->i_dc->i_font->GetCharWidth(ch);
		if(wt <= x && (wt+w) > x)
			break;
		wt += w;
		t += r;
	}

	return t;
}

int CLineEditor::ToPos(int char_pos)
{
	int w,r,t = 0,wt = 0;
	char ch[3];

	if(char_pos < 0 )
		return -1;
	
	this->mem_txt->Seek(0);
	while(!this->mem_txt->IsEnd())
	{
		r = this->mem_txt->Getc_GB(ch);
		w = this->can->i_dc->i_font->GetCharWidth(ch);

		if(t <= char_pos && (t + r) > char_pos)
			break;
		t += r;
		wt += w;
	}
	return wt;
}

int CLineEditor::SetCaret(int char_pos)
{
	int x,y;
	CRect r_txt,rc;

	if(this->can->i_dc == NULL)
		return ERROR;

	if(char_pos < 0)
		char_pos = 0;

	this->ins_pos = char_pos;
	
	this->GetTextRect(&r_txt);
	x = this->ToPos(char_pos);
	y =  ( r_txt.GetHeight() - can->i_dc->i_font->GetMaxCharHeight() ) / 2;

	this->can->LP2SP(&x,&y);
	rc.Set(x,y,x+afx.caret->width-1,y+afx.caret->height-1);

	if(!rc.IsIntersect(this->can->rect))
		return ERROR;

	afx.caret->win_id = this->win_id;
	afx.caret->SetPos(x,y);
	afx.caret->Show();
	afx.caret->StartBlink();

	return OK;
}

int CLineEditor::HasSelection()
{
	return this->sel_end > this->sel_start;
}

int CLineEditor::Scroll(int cx)
{
	this->can->view_x += cx;
	this->SetCaret(this->ins_pos);
	this->Redraw();
	return OK;
}

int CLineEditor::InSelection(int char_pos)
{
	return (char_pos >= this->sel_start && char_pos <= this->sel_end);
}

int CLineEditor::InsPosVisible()
{
	CRect rc;
	int pos;

	this->GetTextRect(&rc);
	this->can->SR2LR(&rc);

	pos = this->ToPos(this->ins_pos);	
	if(pos < 0)
		return ERROR;

	if(pos < rc.left)
		this->Scroll(pos - rc.left);	
	else if(pos > rc.right)
		this->Scroll(pos - rc.right);

	return OK;

}

int CLineEditor::OnAdded()
{
	this->content_menu->SetParent(this->win_id);
	return OK;
}

int CLineEditor::RepSel(char *str)
{
	ASSERT(str);

	CMem mem;
	int ss;

	mem.Init();
	mem.SetP(str,strlen(str));

	if(!this->HasSelection())
	{
		this->mem_txt->Seek(this->ins_pos);
		this->mem_txt->Insert(&mem);
		this->SetCaret(this->ins_pos+mem.GetSize());
		this->InsPosVisible();
		this->Redraw();
	}
	else
	{
		ss = this->sel_start;
		if(ss < 0) ss = 0;

		this->mem_txt->Seek(ss);
		if(this->GetSelSize() > 0)
			this->mem_txt->Delete(this->GetSelSize());
		this->mem_txt->Seek(ss);
		this->mem_txt->Insert(&mem);
		ss += mem.GetSize();
		this->SetSel(ss,ss);
		this->SetCaret(ss);
		this->InsPosVisible();
		this->Redraw();
	}

	return OK;
}

int CLineEditor::GetSelSize()
{
	int s,e;

	s = this->sel_start;
	e = this->sel_end;

	if(s < 0) s = 0;
	if(e < 0) e = 0;
	return abs(e - s);
}

int CLineEditor::OnChar(int wparam, int lparam)
{
	char *str = (char*)lparam;
	ASSERT(str);

	if(str[0] == '\n')
		return ERROR;

	if(str[0] == '\b')
	{
		if(!this->HasSelection())
		{
			this->SetSel(this->ins_pos - 1,this->ins_pos);
		}
		str="";
	}

	this->RepSel(str);

	return OK;
}

int CLineEditor::GetText(CFileBase *file)
{
	ASSERT(file);

	file->SetSize(0);
	file->WriteFile(this->mem_txt);
	file->Putc(0);

	return OK;
}
