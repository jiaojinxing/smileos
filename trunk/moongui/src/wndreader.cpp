#include "wndreader.h"
#include "wndfilemgr.h"

//#define USE_MEM		1

CWndReader::CWndReader()
{
    this->InitBasic();
}
CWndReader::~CWndReader()
{
    this->Destroy();
}
int CWndReader::InitBasic()
{
    CWindow::InitBasic();
	this->win_id_filemgr = 0;
	this->mem_txt = NULL;
	this->timer_auto = NULL;
	this->file_txt = NULL;
	this->win_id_msg_box = 0;
	this->in_loop = 0;
	this->mem_msg = NULL;

    return OK;
}
int CWndReader::Init()
{
    this->InitBasic();
    CWindow::Init();
    
	NEW(this->mem_txt,CMem);
	this->mem_txt->Init();
	
	NEW(this->file_txt,CFile);
	this->file_txt->Init();

	NEW(this->mem_msg,CMem);
	this->mem_msg->Init();
	this->mem_msg->Malloc(100);

    return OK;
}
int  CWndReader::Destroy()
{
	DEL(this->mem_msg);
	DEL(this->mem_txt);
	DEL(this->file_txt);

    CWindow::Destroy();
    this->InitBasic();
    return OK;
}
int CWndReader::OnCreate(int wparam, int lparam)
{
	NEW(this->eb_txt,CEditBox);
	this->eb_txt->Init();	
	this->eb_txt->SetStyle(WS_BORDER|WS_VSCROLL);
	this->eb_txt->SetRect(2,35,252,131);
	this->eb_txt->fore_color = COLOR_BLUE;

	this->eb_txt->SetTextFile(this->mem_txt);

	this->mwnd_file = CMenuWnd::CreateMenuWnd(this->id);
	
	NEW(this->mi_open,CMenuItem);
	this->mi_open->Init();
	this->mi_open->SetName("打开");
	
	NEW(this->mi_set_auto,CMenuItem);
	this->mi_set_auto->Init();
	this->mi_set_auto->SetName("开始自动翻页");
	
	NEW(this->mi_exit,CMenuItem);
	this->mi_exit->Init();
	this->mi_exit->SetName("退出");
	
	this->mwnd_help = CMenuWnd::CreateMenuWnd(this->id);
	
	NEW(this->mi_help,CMenuItem);
	this->mi_help->Init();
	this->mi_help->SetName("帮助");
	
	NEW(this->mi_about,CMenuItem);
	this->mi_about->Init();
	this->mi_about->SetName("关于");
	
	NEW(this->mb_main,CMenuBar);
	this->mb_main->Init();
	this->mb_main->SetRect(3,20,241,16);
	this->mb_main->AddMenuBarItem("文件",this->mwnd_file->id);
	this->mb_main->AddMenuBarItem("帮助",this->mwnd_help->id);

	this->AddControl(this->mb_main);
	this->AddControl(this->eb_txt);
	this->mwnd_file->AddMenuItem(mi_open);
	this->mwnd_file->AddSeparator();
	this->mwnd_file->AddMenuItem(mi_set_auto);
	this->mwnd_file->AddSeparator();
	this->mwnd_file->AddMenuItem(mi_exit);
	this->mwnd_help->AddMenuItem(mi_help);
	this->mwnd_help->AddMenuItem(mi_about);
 	
	NEW(this->timer_auto,CTimer);
	this->timer_auto->Init();
	this->timer_auto->win_id = this->id;
	this->timer_auto->SetTimeOut(2000);

	afx.timemgr->AddTimer(this->timer_auto);

	return OK;
}
int CWndReader::OnClose(int wparam, int lparam)
{
	if(this->in_loop)
		return ERROR;

	return CWindow::OnClose(wparam,lparam);
}

int CWndReader::OnCommand(int wparam, int lparam)
{
	if(this->in_loop)
		return ERROR;

	if(lparam == this->mi_open->id)
	{
		if(!afx.wndmgr->IsWindow(this->win_id_filemgr))
		{
			this->win_id_filemgr = CWndFileMgr::Create(this->id,"/","打开 - 文本阅读器");
		}
	}

	if(lparam == this->mi_set_auto->id)
	{
		if(!this->timer_auto->IsEnable())
		{
			this->StartAutoScroll();
		}
		else
		{
			this->StopAutoScroll();
		}
	}

	if(lparam == this->mi_exit->id)
		this->Close();

    return OK;
}

int CWndReader::OnUnknown(u32 message,int wparam, int lparam)
{
	if(message == WM_FILE_OPEN && wparam == this->win_id_filemgr)
	{
		this->CloseFile();
		ASSERT(lparam);
		this->in_loop = 1;

		this->eb_txt->call_back_param = (int*)this;
		this->eb_txt->call_back_load_file = this->call_back_load_file;

		afx.wndmgr->HideWindowByIndex(afx.wndmgr->IdToIndex(this->win_id_filemgr));
		this->win_id_msg_box = CWndMsgBox::Create(this->id,"请稍后","正在打开文件",0);

#ifdef USE_MEM
		CFile file;

		file.Init();
		
		ASSERT( file.OpenFile((char*)lparam,"rb") );
		
		if( this->mem_txt->Malloc(file.GetSize() + 10) == ERROR)
		{
			CWndMsgBox::Create(this->id,"错误","内存不足!\n",MB_OK);
			return ERROR;
		}

		file.Read(this->mem_txt->p,this->mem_txt->GetMaxSize());
		this->mem_txt->SetSize(file.GetSize());
		this->eb_txt->SetTextFile(this->mem_txt);
#else
		this->file_txt->SetBufSize(10240);
		if( this->file_txt->OpenFile((char*)lparam,"rb"))
		{
			this->eb_txt->SetTextFile(this->file_txt);
		}
		else
		{
			LOG("open file %s error\n",(char*)lparam);
		}
		
#endif	
		CWndMsgBox::CloseMsgBox(this->win_id_msg_box);
		this->in_loop = 0;
	}
	return OK;
}

int CWndReader::CloseFile()
{
#ifdef USE_MEM
	this->mem_txt->Destroy();
	this->mem_txt->Init();
	this->eb_txt->SetTextFile(this->mem_txt);
#else	
	this->file_txt->CloseFile();
	this->eb_txt->SetTextFile(this->mem_txt);
#endif	
	return OK;
}

int CWndReader::OnKeyDown(int wparam, int lparam)
{
	this->StopAutoScroll();
	
	int l = this->eb_txt->GetLineHeight();
	int m = this->eb_txt->GetLineHeight() * this->eb_txt->GetVisibleLines();
	
	if(lparam == KEY_DOWN)
		this->eb_txt->ScrollY(l);
	else if(lparam == KEY_UP)
		this->eb_txt->ScrollY(-l);
	else if(lparam == KEY_LEFT)
		this->eb_txt->ScrollY(-m);
	else if(lparam == KEY_RIGHT)
		this->eb_txt->ScrollY(m);
	
    return OK;
}
int CWndReader::OnTimer(int wparam, int lparam)
{
	if(lparam == this->timer_auto->id)
		this->eb_txt->ScrollY(this->eb_txt->GetLineHeight());		

	return OK;
}

int CWndReader::call_back_load_file(int *p, int line)
{
	CWndReader *self = (CWndReader*)p;
	ASSERT(self);

	if(line % 100 == 0)
	{
		self->mem_msg->SetSize(0);
		self->mem_msg->Puts("已打开");
		self->mem_msg->Printf(" %d ",line);
		self->mem_msg->Puts("行");
		self->mem_msg->Putc(0);

		CWndMsgBox::SetText(self->win_id_msg_box,self->mem_msg->p);
	}
	
	MsgLoop();

	return OK;
}

int CWndReader::StartAutoScroll()
{
	if(!this->timer_auto->IsEnable())
	{
		this->mi_set_auto->SetName("停止自动翻页");
		this->timer_auto->Start();
	}

	return OK;
}

int CWndReader::StopAutoScroll()
{
	if(this->timer_auto->IsEnable())
	{
		this->mi_set_auto->SetName("开始自动翻页");
		this->timer_auto->Stop();
	}
	
	return OK;
}
