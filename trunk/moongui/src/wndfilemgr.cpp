#include "wndfilemgr.h"
CWndFileMgr::CWndFileMgr()
{
    this->InitBasic();
}
CWndFileMgr::~CWndFileMgr()
{
    this->Destroy();
}
int CWndFileMgr::InitBasic()
{
    CWindow::InitBasic();
	this->dirmgr = NULL;
    return OK;
}
int CWndFileMgr::Init()
{
    this->InitBasic();
    CWindow::Init();
    
	NEW(this->dirmgr,CDirMgr);
	this->dirmgr->Init();
	this->dirmgr->ChDir("/");

    return OK;
}
int  CWndFileMgr::Destroy()
{
    DEL(this->dirmgr);
    CWindow::Destroy();
    this->InitBasic();
    return OK;
}
int CWndFileMgr::OnCreate(int wparam, int lparam)
{
	NEW(this->lb_list,CListBox);
	this->lb_list->Init();
	this->lb_list->SetRect(4,43,217,93);
	this->lb_list->SetStyle(WS_BORDER|WS_VSCROLL);

	NEW(this->leb_fn,CLineEditor);
	this->leb_fn->Init();
	this->leb_fn->SetRect(4,138,116,18);
	this->leb_fn->SetStyle(WS_BORDER);
	this->leb_fn->SetText("");

	NEW(this->btn_open,CButton);
	this->btn_open->Init();
	this->btn_open->SetText("打开");
	this->btn_open->SetRect(124,137,46,18);

	NEW(this->btn_calcel,CButton);
	this->btn_calcel->Init();
	this->btn_calcel->SetText("取消");
	this->btn_calcel->SetRect(174,137,46,18);

	NEW(this->btn_up,CButton);
	this->btn_up->Init();
	this->btn_up->SetRect(4,20,21,21);
	this->btn_up->SetStockImage(IMAGE_FOLDER_UP);

	NEW(this->btn_property,CButton);
	this->btn_property->Init();
	this->btn_property->SetRect(26,20,21,21);
	this->btn_property->SetStockImage(IMAGE_PROPERTY);

	this->AddControl(this->lb_list);
	this->AddControl(this->leb_fn);
	this->AddControl(this->btn_open);
	this->AddControl(this->btn_calcel);
	this->AddControl(this->btn_up);
	this->AddControl(this->btn_property);

	this->RefreshList();

    return OK;
}
int CWndFileMgr::OnCommand(int wparam, int lparam)
{
	if(this->btn_open->id == lparam)
	{
		CMem *pmem = this->lb_list->GetSelText();
		if(pmem == NULL)
			return ERROR;

		if(pmem->p[0] == '<')
		{
			CMem buf;

			LOCAL_MEM(buf);
			buf.WriteFile(pmem,1,pmem->GetSize() - 3);
			buf.Putc(0);
			this->dirmgr->ChDir(buf.p);
			this->RefreshList();
		}
		else
		{
			CMem mem,buf;
			LOCAL_MEM(mem);
			LOCAL_MEM(buf);

			this->leb_fn->GetText(&mem);
			if(mem.p[0])
			{
				this->dirmgr->GetCurDir(&buf);
				buf.SeekEnd();
				buf.SeekBack(1); //over write 0 at end
				buf.Putc('/');
				buf.Puts(mem.p);
				buf.Putc(0);
				afx.wndmgr->SendMessage(this->pid,WM_FILE_OPEN,this->id,(int)buf.p);
				this->Close();
			}
		}
	}

	if(this->lb_list->id == lparam)
	{
		if(wparam == LBN_SELCHANGE)
		{
			CMem *pmem = this->lb_list->GetSelText();
			if(pmem == NULL)
				return ERROR;
			if(pmem->p[0] == '<')
				this->leb_fn->SetText("");
			else
				this->leb_fn->SetText(pmem->p);
		}
	}

	if(this->btn_up->id == lparam)
	{
		this->dirmgr->ChDir("..");
		this->RefreshList();
		this->leb_fn->SetText("");
	}

	if(this->btn_calcel->id == lparam)
	{
		this->Close();
	}
    return OK;
}

int CWndFileMgr::RefreshList()
{
	CMem mem,buf,*pmem;
	CMemStk stk;
	int i;

	LOCAL_MEM(mem);
	LOCAL_MEM(buf);

	stk.Init(256);
	stk.SetCaseSensetive(0);

	this->lb_list->DelAll();
	this->dirmgr->OpenDir();

	while(TRUE)
	{
		this->dirmgr->GetCurFileName(&buf);
		if(this->dirmgr->IsDir())
		{		
			if(buf.StrCmp(".")!=0 && buf.StrCmp(".." ) != 0)
			{
				mem.SetSize(0);
				mem.Putc('<');
				mem.Puts(buf.p);
				mem.Putc('>');
				mem.Putc(0);
				stk.InsOrdered(&mem,0,1);
			}
		}
		else
		{		
			stk.InsOrdered(&buf,0,1);
		}

		if(this->dirmgr->ReadNextDir() == ERROR)
			break;
	}

	for(i = 0; i < stk.GetLen(); i++)
	{
		pmem = stk.GetElem(i);
		ASSERT(pmem);
		this->lb_list->AddStr(pmem->p);
	}
	this->dirmgr->CloseDir();
	return OK;
}

int CWndFileMgr::Create(int pid,char *init_path,char *caption)
{
	CWndFileMgr *wnd_filemgr;
	
	CRect rc,rt;

	rc.Set(0,0,GAL_GetWidth(),GAL_GetHeight());
	rc.CenterRect(225,160,&rt);

	NEW(wnd_filemgr,CWndFileMgr);
	wnd_filemgr->Init();
	wnd_filemgr->SetRect(rt.left,0,rt.GetWidth(),rt.GetHeight());
	wnd_filemgr->SetCaption(caption);
	wnd_filemgr->SetParent(pid);
	wnd_filemgr->dwStyle = WS_CAPTION|WS_VISIBLE|WS_BORDER;
	wnd_filemgr->dwExStyle = WS_EX_TOPMOST;

	wnd_filemgr->dirmgr->ChDir(init_path);

	afx.wndmgr->AddWindow(wnd_filemgr);
	
	return wnd_filemgr->id;
}
