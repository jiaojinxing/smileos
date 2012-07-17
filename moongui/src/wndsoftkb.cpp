#include "wndsoftkb.h"

/**************
关于按键的映射
0-9 分别表示按键的 0-9
10-35 分别表示按键的 a-z
36 空格
37 shift
38 回车
39 back
40 向上箭头
41 向下箭头
42 向左箭头
43 向右箭头
44 逗号
45 句号
46 反斜杠
47 正斜杠
48 冒号

符号映射
q +
w -
e *
r /
t <
y >
u [
i ]
o {
p }
a =
s _
d /
f \
g |
h \'
j \"
k ?
l ~
z `
x E
c E
v E
b E
n E
m E
***************/

#define MAX_SOFT_BUTTON	 49

#define SOFTKB_IME_ENGLISH  1
#define SOFTKB_IME_PINYIN   2
#define SOFTKB_IME_SYM	    3

static int old_ime_type = SOFTKB_IME_ENGLISH;

CWndSoftKb::CWndSoftKb()
{
    this->InitBasic();
}
CWndSoftKb::~CWndSoftKb()
{
    this->Destroy();
}
int CWndSoftKb::InitBasic()
{
    CWindow::InitBasic();
	this->btn_txt = NULL;
	this->is_capital = 0;
	this->py_bar = NULL;
	this->spin_box = NULL;

    return OK;
}
int CWndSoftKb::Init()
{
    this->InitBasic();
    CWindow::Init();

    return OK;
}
int CWndSoftKb::Destroy()
{
	old_ime_type = this->GetImeType();
	FREE(this->btn);
	FREE(this->btn_txt);
    CWindow::Destroy();
    this->InitBasic();
    return OK;
}
int CWndSoftKb::OnCreate(int wparam,int lparam)
{
	int group;

	group = afx.GetUniqueID();

	NEW(this->rb_eng,CRadioBox);
	this->rb_eng->Init();
	this->rb_eng->SetText("英文");
	this->rb_eng->SetRect(5,57,45,20);
	this->rb_eng->group = group;	
	if(old_ime_type == SOFTKB_IME_ENGLISH)
		this->rb_eng->SetCheck(TRUE);
	
	NEW(this->rb_py,CRadioBox);
	this->rb_py->Init();
	this->rb_py->SetText("拼音");
	this->rb_py->SetRect(105,57,45,20);
	this->rb_py->group = group;
	if(old_ime_type == SOFTKB_IME_PINYIN)
		this->rb_py->SetCheck(TRUE);

	NEW(this->rb_sym,CRadioBox);
	this->rb_sym->Init();
	this->rb_sym->SetText("符号");
	this->rb_sym->SetRect(55,57,45,20);
	this->rb_sym->group = group;
	if(old_ime_type == SOFTKB_IME_SYM)
		this->rb_sym->SetCheck(TRUE);

	this->InitButtons();
	this->RefreshBtnText();

	NEW(this->py_bar,CPyBar);
	this->py_bar->Init();
	this->py_bar->SetRect(3,17,this->rect->GetWidth() - 19,41);
	this->py_bar->i_font = this->privCDC->i_font;
	
	NEW(this->spin_box,CSpinBox);
	this->spin_box->Init();
	this->spin_box->SetRect(this->rect->GetWidth() - 16,31,13,26);

	this->AddControl(this->py_bar);
	this->AddControl(this->rb_eng);
	this->AddControl(this->rb_py);
	this->AddControl(this->rb_sym);
	this->AddControl(this->spin_box);

	return OK;
}
int CWndSoftKb::OnLButtonDown(int wparam,int lparam)
{
	return OK;
}

int CWndSoftKb::OnCommand(int wparam,int lparam)
{
	if(this->btn[37]->id == lparam)
		return this->OnShift();

	if(this->rb_sym->id == lparam)
	{
		this->is_capital = FALSE;
		this->py_bar->ClearInputBuf();
		return this->RefreshBtnText();
	}
	if(this->rb_eng->id == lparam)
	{
		this->py_bar->ClearInputBuf();
		this->is_capital = FALSE;
		return this->RefreshBtnText();
	}
	if(this->rb_py->id == lparam)
	{
		this->py_bar->ClearInputBuf();
		this->is_capital = FALSE;
		return this->RefreshBtnText();
	}
	
	int btn_index = this->IdToBtn(lparam);

	if(btn_index >= 0 && btn_index < MAX_SOFT_BUTTON)
	{
		if(this->GetImeType() == SOFTKB_IME_PINYIN && btn_index >= 0 && btn_index <= 9)
		{
			CMem mem;
			
			LOCAL_MEM(mem);
			
			this->py_bar->GetHzByNum(btn_index - 1,&mem);
			if(mem.p[0])
			{
				this->py_bar->ClearInputBuf();
				this->SendChar(mem.p);			
				this->py_bar->Redraw();
			}
			return OK;
		}
		char ch = this->BtnToChar(btn_index);
		if(ch != 0)
		{
			this->py_bar->AddChar(ch);
			if(this->GetImeType() == SOFTKB_IME_PINYIN)
			{
				this->py_bar->Search();
			}
			this->py_bar->Redraw();
		}

	}
	
	if(lparam == this->spin_box->id)
	{
		if(wparam == 0) //scroll up
			this->py_bar->ScrollUp();
		else if(wparam == 1) //scroll down
			this->py_bar->ScrollDown();		
	}
	////回车的处理
	if(lparam == this->btn[38]->id)
	{
		if(this->GetImeType() == SOFTKB_IME_ENGLISH || this->GetImeType() == SOFTKB_IME_SYM)
		{
			if(this->py_bar->IsInputBufEmpty())
			{
				this->SendChar("\n");
			}
			else
			{
				this->SendChar(this->py_bar->mem_input->p);
				this->py_bar->ClearInputBuf();
				this->py_bar->Redraw();
			}			
		}
	}
	//back 的处理
	if(lparam == this->btn[39]->id)
	{
		if(this->py_bar->IsInputBufEmpty())
			this->SendChar("\b");
	}
	//选中的处理
	if(lparam == this->py_bar->id)
	{
		CMem mem;
		LOCAL_MEM(mem);
		this->py_bar->GetHzByIndex(this->py_bar->cur_sel,&mem);
		this->SendChar(mem.p);
	}

	return OK;
}
int CWndSoftKb::OnUnknown(u32 message,int wparam,int lparam)
{
	return OK;
}
int CWndSoftKb::OnTimer(int wparam,int lparam)
{
	return OK;
}
int CWndSoftKb::OnPaint(int wparam,int lparam)
{
	this->BeginPaint(lparam);
	this->EndPaint();
	return OK;
}

int CWndSoftKb::InitButtons()
{
	int i,w,s,y,sep = 1,kb_y_pos = 77;
	
	MALLOC(this->btn_txt,char,(MAX_SOFT_BUTTON * 2));
	MALLOC(this->btn,CButton *,MAX_SOFT_BUTTON);

	w = 19;s = 3; y = kb_y_pos;
	for(i = 1; i< 10; i++)
	{
		btn_txt[i * 2] = i + '0';
		btn_txt[i * 2 + 1] = 0;

		NEW(this->btn[i],CButton);
		btn[i]->Init();
		btn[i]->SetRect(s + (i - 1)* (w+sep), y, w,w);
		btn[i]->SetText(btn_txt + i * 2);
		this->AddControl(btn[i]);
	}
	for(i = 10; i< 36; i++)
	{
		btn_txt[i * 2] = i + 'a' - 10;
		btn_txt[i * 2 + 1] = 0;
		NEW(btn[i],CButton);
		btn[i]->Init();
		btn[i]->SetText(btn_txt + i * 2);		
	}

	NEW(btn[0],CButton);
	btn[0]->Init();
	btn[0]->SetRect(s + 9 * (w+sep), y, w,w);
	btn_txt[0] = '0';btn_txt[1] = 0;
	btn[0]->SetText(btn_txt);
	this->AddControl(btn[0]);

	s = 3;y = kb_y_pos + 20;
	int line2[] = {26,32,14,27,29,34,30,18,24,25};

	for(i = 0; i < 10; i++)
	{
		btn[line2[i]]->SetRect(s,y,w,w);
		this->AddControl(btn[line2[i]]);
		s += (w + sep);
	}

	s = 3;y = kb_y_pos + 40;
	int line3[] = {10,28,13,15,16,17,19,20,21};
	for(i = 0; i < 9; i++)
	{
		btn[line3[i]]->SetRect(s,y,w,w);
		this->AddControl(btn[line3[i]]);
		s += (w + sep);
	}

	s = 3;y = kb_y_pos + 60;
	int line4[] = {35,33,12,31,11,23,22};
	for(i = 0; i < 7; i++)
	{
		btn[line4[i]]->SetRect(s,y,w,w);
		this->AddControl(btn[line4[i]]);
		s += (w + sep);
	}

	i = 36;
	NEW(btn[i],CButton);
	btn[i]->Init();
	btn[i]->SetRect(43,kb_y_pos + 80,w*5+4,w);
	btn[i]->SetText(" ");
	this->AddControl(btn[i]);

	i = 37;
	NEW(btn[i],CButton);
	btn[i]->Init();
	btn[i]->SetRect(3,kb_y_pos + 80,w*2+sep,w);
	btn[i]->SetText("Shift");	
	this->AddControl(btn[i]);

	i = 38;
	NEW(btn[i],CButton);
	btn[i]->Init();
	btn[i]->SetRect(3+(sep+w)*9,kb_y_pos + 40,w*2+sep,w);
	btn[i]->SetText("Enter");	
	this->AddControl(btn[i]);

	i = 39;
	NEW(btn[i],CButton);
	btn[i]->Init();
	btn[i]->SetRect(3 + 10 * (w+sep) ,kb_y_pos,w,w);
	btn[i]->SetStockImage(IMAGE_BACK_ARROW);
	this->AddControl(btn[i]);

	i = 40;
	NEW(btn[i],CButton);
	btn[i]->Init();
	btn[i]->SetRect(3 + 9 * (w+sep) ,kb_y_pos + 60,w,w);
	btn[i]->SetStockImage(IMAGE_KB_UP_ARROW);
	this->AddControl(btn[i]);

	i = 41;
	NEW(btn[i],CButton);
	btn[i]->Init();
	btn[i]->SetRect(3 + 9 * (w+sep) ,kb_y_pos + 80,w,w);
	btn[i]->SetStockImage(IMAGE_KB_DOWN_ARROW);
	this->AddControl(btn[i]);

	i = 42;
	NEW(btn[i],CButton);
	btn[i]->Init();
	btn[i]->SetRect(3 + 8 * (w+sep) ,kb_y_pos + 80,w,w);
	btn[i]->SetStockImage(IMAGE_KB_LEFT_ARROW);
	this->AddControl(btn[i]);

	i = 43;
	NEW(btn[i],CButton);
	btn[i]->Init();
	btn[i]->SetRect(3 + 10 * (w+sep) ,kb_y_pos + 80,w,w);
	btn[i]->SetStockImage(IMAGE_KB_RIGHT_ARROW);
	this->AddControl(btn[i]);
	
	i = 44;
	NEW(btn[i],CButton);
	btn[i]->Init();
	btn[i]->SetRect(3 + 7 * (w+sep) ,kb_y_pos + 60,w,w);
	btn[i]->SetText(",");
	this->AddControl(btn[i]);

	i = 45;
	NEW(btn[i],CButton);
	btn[i]->Init();
	btn[i]->SetRect(3 + 8 * (w+sep) ,kb_y_pos + 60,w,w);
	btn[i]->SetText(".");
	this->AddControl(btn[i]);

	i = 46;
	NEW(btn[i],CButton);
	btn[i]->Init();
	btn[i]->SetRect(3 + 10 * (w+sep) ,kb_y_pos + 20,w,w);
	btn[i]->SetText("\\");
	this->AddControl(btn[i]);

	i = 47;
	NEW(btn[i],CButton);
	btn[i]->Init();
	btn[i]->SetRect(3 + 10 * (w+sep) ,kb_y_pos + 60,w,w);
	btn[i]->SetText("/");
	this->AddControl(btn[i]);

	i = 48;
	NEW(btn[i],CButton);
	btn[i]->Init();
	btn[i]->SetRect(3 + 7 * (w+sep) ,kb_y_pos + 80,w,w);
	btn[i]->SetText("\"");
	this->AddControl(btn[i]);

	return OK;
}

int CWndSoftKb::GetImeType()
{
	if(this->rb_eng->IsChecked())
		return SOFTKB_IME_ENGLISH;

	if(this->rb_py->IsChecked())
		return SOFTKB_IME_PINYIN;

	if(this->rb_sym->IsChecked())
		return SOFTKB_IME_SYM;

	return 0;
}

int CWndSoftKb::OnShift()
{
	if(this->GetImeType() != SOFTKB_IME_ENGLISH)
		return ERROR;

	this->is_capital = this->is_capital ? FALSE:TRUE;
	this->RefreshBtnText();
	return OK;
}

int CWndSoftKb::RefreshBtnText()
{
	int i;
	char sch;
	char *p;
	
	if(this->GetImeType() == SOFTKB_IME_ENGLISH || this->GetImeType() == SOFTKB_IME_PINYIN)
	{		
		for(i = 0; i< 10; i++)
		{
			btn_txt[i * 2] = i + '0';
			btn_txt[i * 2 + 1] = 0;
		}
		sch = this->is_capital ? 'A' : 'a';
		
		for(i = 10; i< 36; i++)
		{
			btn_txt[i * 2] = i + sch - 10;
			btn_txt[i * 2 + 1] = 0;
		}		
		btn[37]->fore_color = this->is_capital ? COLOR_RED : COLOR_BLACK;
	}
	else if(this->GetImeType() == SOFTKB_IME_SYM)
	{
		for(i = 0; i < 36; i++)
		{
			btn_txt[i*2] = 0;
		}

		p = ")!@#$%^&*(";
		for(i = 0; i< 10; i++)
		{
			btn_txt[i * 2] = p[i];
			btn_txt[i * 2 + 1] = 0;
		}	
		
		p = "=EE/*\\|\']\"?~EE{}+/_<[E-E>`";
		for(i = 0; i < (int)strlen(p); i++)
		{
			btn_txt[(10+i)*2] = p[i];
			if(p[i] == 'E')
				btn_txt[(10+i)*2] = 0;			
			btn_txt[(10+i)*2+1] = 0;
		}

	}
	

	this->InvalidateRect(NULL,1);
	return OK;
}

int CWndSoftKb::IdToBtn(int ctrl_id)
{
	int i;

	for(i = 0; i < MAX_SOFT_BUTTON; i++)
	{
		if(btn[i]->id == ctrl_id)
			return i;
	}

	return -1;
}

char CWndSoftKb::BtnToChar(int btn_index)
{
	if(btn_index >= 0 && btn_index <= 9)
	{
		if(this->GetImeType() == SOFTKB_IME_SYM)
		{ 	
			ASSERT(this->btn[btn_index]->i_text);
			return this->btn[btn_index]->i_text[0];
		}
		return btn_index + '0';
	}

	if(btn_index >= 10 && btn_index <= 35)
	{
		if(this->GetImeType() == SOFTKB_IME_SYM)
		{ 	
			ASSERT(this->btn[btn_index]->i_text);
			return this->btn[btn_index]->i_text[0];
		}
		
		if(this->is_capital)
			return btn_index + 'A' - 10;
		else
			return btn_index + 'a' - 10;
	}

	if(btn_index == 39)
		return 8;

	if(btn_index == 36)
		return ' ';

	return 0;
}

int CWndSoftKb::SendChar(char *str)
{
	int index = afx.wndmgr->GetFocusWindow();
	CWindow *pwnd = afx.wndmgr->GetElem(index);

	if(pwnd)
		pwnd->SendMessage(WM_CHAR,0,(int)str);
	
	return OK;
}
