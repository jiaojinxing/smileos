#include "wndbus.h"

#define BUS_DATA_DIR "/moonGUI/bus"

CWndBus::CWndBus()
{
    this->InitBasic();
}
CWndBus::~CWndBus()
{
    this->Destroy();
}
int CWndBus::InitBasic()
{
    CWindow::InitBasic();
	
	this->mf_txt = NULL;
	this->cur_control_group = 0;
	this->bus_map = NULL;
	this->win_id_msgbox = 0;
	this->class_name = WND_CLASS_MSGBOX;
	this->in_loop = FALSE;
	this->mem_caption = NULL;
	this->win_id_process = 0;
	this->mem_process = NULL;

    return OK;
}
int CWndBus::Init()
{
    this->InitBasic();
    CWindow::Init();
	
	NEW(this->mf_txt,CMemFile);
	this->mf_txt->Init(256,1024);
	
	NEW(this->mem_caption,CMem);
	this->mem_caption->Init();
	this->mem_caption->Malloc(100);
	this->mem_caption->Puts("NDS公交查询");
	this->mem_caption->Putc(0);
	
	NEW(this->mem_process,CMem);
	this->mem_process->Init();
	this->mem_process->Malloc(100);
	this->mem_process->Putc(0);

	NEW(this->bus_map,CBusMap);
	this->bus_map->Init();

    return OK;
}
int CWndBus::Destroy()
{
	DEL(this->mem_process);
	DEL(this->mem_caption);
	DEL(this->bus_map);
	DEL(this->mf_txt);
    CWindow::Destroy();
    this->InitBasic();
    return OK;
}
int CWndBus::OnCreate(int wparam,int lparam)
{
	this->SetCaption(this->mem_caption->p);

	NEW(this->lb_tab,CListBox);
	this->lb_tab->Init();
	this->lb_tab->SetRect(3,17,70,150);
	this->lb_tab->SetStyle(WS_BORDER);

	this->lb_tab->AddStr("换乘查询");
	this->lb_tab->AddStr("站点查询");
	this->lb_tab->AddStr("线路查询");
	this->lb_tab->AddStr("系统设置");
	this->lb_tab->SetSel(3);

	this->AddControl(this->lb_tab);
	
	this->CreateGroup4Controls();
	
	return OK;
}
int CWndBus::OnCommand(int wparam,int lparam)
{
	if(this->cur_control_group == 4)
	{
		if(lparam != this->lb_tab->id)
		{
			if(lparam == this->btn_41->id)
				this->LoadData();
			return OK;
		}
	}
	
	if(!this->IsDataLoaded())
	{
		this->ShowMsgBox("错误","还没有加载数据库\n请先加载.\n",MB_OK);
		return ERROR;
	}

	if(this->in_loop)
		return ERROR;
	
	if(lparam == this->lb_tab->id)
	{
		if(wparam == LBN_SELCHANGE)
		{
			if(this->lb_tab->GetSel() == 0)
				this->CreateGroup1Controls();
			if(this->lb_tab->GetSel() == 1)
				this->CreateGroup2Controls();
			if(this->lb_tab->GetSel() == 2)
				this->CreateGroup3Controls();
			if(this->lb_tab->GetSel() == 3)
				this->CreateGroup4Controls();
		}
		return OK;
	}

	if(this->cur_control_group == 1)
	{
		if(this->btn_reset->id == lparam)
		{
			this->cb_es->SetText("");
			this->cb_ss->SetText("");
		}
		else if(this->btn_search->id == lparam)
		{
			this->SearchPath();
		}
		else if(this->cb_ss->id == lparam)
		{
			if(wparam == LBN_ABOUTSHOW)
				this->LoadStations(this->cb_ss);
		}
		else if(this->cb_es->id == lparam)
		{
			if(wparam == LBN_ABOUTSHOW)
				this->LoadStations(this->cb_es);	
		}
	}

	if(this->cur_control_group == 2)
	{
		if(this->cb_21->id == lparam)
		{
			if(wparam == LBN_ABOUTSHOW)
				this->LoadStations(this->cb_21);
		}
		if(this->btn_21->id == lparam)
			this->cb_21->SetText("");
		if(this->btn_22->id == lparam)
			this->SearchStation();
	}

	if(this->cur_control_group == 3)
	{
		if(this->cb_31->id == lparam)
		{
			if(wparam == LBN_ABOUTSHOW)
				this->LoadBus(this->cb_31);
		}
		if(this->btn_31->id == lparam)
			this->cb_31->SetText("");
		if(this->btn_32->id == lparam)
			this->SearchBus();
	}
	return OK;
}

int CWndBus::CreateGroup1Controls()
{
	if( this->cur_control_group == 1 )
		return OK;

	afx.caret->StopBlink();
	this->DelAllControls();

	this->cur_control_group = 1;

	NEW(this->st_1,CStatic);
	this->st_1->Init();
	this->st_1->SetRect(76,17,50,15);
	this->st_1->SetText("起始站:");

	NEW(this->st_2,CStatic);
	this->st_2->Init();
	this->st_2->SetRect(76,34,50,15);
	this->st_2->SetText("终点站:");

	NEW(this->st_3,CStatic);
	this->st_3->Init();
	this->st_3->SetRect(76,53,50,15);
	this->st_3->SetText("结果:");

	NEW(this->cb_ss,CComboBox);
	this->cb_ss->Init();
	this->cb_ss->SetRect(130,17,120,15);

	NEW(this->cb_es,CComboBox);
	this->cb_es->Init();
	this->cb_es->SetRect(130,34,120,15);

	NEW(this->btn_reset,CButton);
	this->btn_reset->Init();
	this->btn_reset->SetText("重填");
	this->btn_reset->SetRect(175,53,35,18);

	NEW(this->btn_search,CButton);
	this->btn_search->Init();
	this->btn_search->SetText("查询");
	this->btn_search->SetRect(215,53,35,18);

	this->mf_txt->SetSize(0);
	NEW(this->eb_result_1,CEditBox);
	this->eb_result_1->Init();
	this->eb_result_1->SetStyle(WS_BORDER|WS_VSCROLL);
	this->eb_result_1->SetRect(76,73,174,94);
	this->eb_result_1->SetTextFile(this->mf_txt);

	this->AddControl(this->st_1);
	this->AddControl(this->st_2);
	this->AddControl(this->st_3);
	this->AddControl(this->cb_ss);
	this->AddControl(this->cb_es);
	this->AddControl(this->btn_reset);
	this->AddControl(this->btn_search);
	this->AddControl(this->eb_result_1);

	return OK;
}

int CWndBus::DelAllControls()
{
	if(this->cur_control_group == 1)
	{
		this->DelControl(this->st_1->id);
		this->DelControl(this->st_2->id);
		this->DelControl(this->st_3->id);
		this->DelControl(this->cb_ss->id);
		this->DelControl(this->cb_es->id);
		this->DelControl(this->btn_reset->id);
		this->DelControl(this->btn_search->id);
		this->DelControl(this->eb_result_1->id);
	}
	
	if(this->cur_control_group == 2)
	{
		this->DelControl(this->st_21->id);
		this->DelControl(this->st_22->id);
		this->DelControl(this->cb_21->id);
		this->DelControl(this->btn_21->id);
		this->DelControl(this->btn_22->id);
		this->DelControl(this->eb_result_21->id);
	}

	if(this->cur_control_group == 3)
	{
		this->DelControl(this->st_31->id);
		this->DelControl(this->st_32->id);
		this->DelControl(this->cb_31->id);
		this->DelControl(this->btn_31->id);
		this->DelControl(this->btn_32->id);
		this->DelControl(this->eb_result_31->id);
	}
	
	if(this->cur_control_group == 4)
	{
		this->DelControl(this->btn_41->id);
		this->DelControl(this->lb_41->id);
		this->DelControl(this->st_41->id);
	}

	this->cur_control_group = 0;

	return OK;
}

int CWndBus::CreateGroup2Controls()
{
	if(this->cur_control_group == 2)
		return ERROR;
	this->DelAllControls();
	this->cur_control_group = 2;

	afx.caret->StopBlink();

	NEW(this->st_21,CStatic);
	this->st_21->Init();
	this->st_21->SetText("站点名称:");
	this->st_21->SetRect(76,17,56,15);

	NEW(this->st_22,CStatic);
	this->st_22->Init();
	this->st_22->SetText("结果:");
	this->st_22->SetRect(76,34,56,18);

	NEW(this->cb_21,CComboBox);
	this->cb_21->Init();
	this->cb_21->SetRect(136,17,115,15);

	NEW(this->btn_21,CButton);
	this->btn_21->Init();
	this->btn_21->SetText("重填");
	this->btn_21->SetRect(176,34,35,18);

	NEW(this->btn_22,CButton);
	this->btn_22->Init();
	this->btn_22->SetText("查询");
	this->btn_22->SetRect(216,34,35,18);

	this->mf_txt->SetSize(0);
	NEW(this->eb_result_21,CEditBox);
	this->eb_result_21->Init();
	this->eb_result_21->SetStyle(WS_BORDER|WS_VSCROLL);
	this->eb_result_21->SetRect(76,53,174,114);
	this->eb_result_21->SetTextFile(this->mf_txt);

	this->AddControl(this->st_21);
	this->AddControl(this->st_22);
	this->AddControl(this->cb_21);
	this->AddControl(this->btn_21);
	this->AddControl(this->btn_22);
	this->AddControl(this->eb_result_21);

	return OK;
}

int CWndBus::CreateGroup3Controls()
{
	if(this->cur_control_group == 3)
		return ERROR;

	this->DelAllControls();
	this->cur_control_group = 3;

	afx.caret->StopBlink();

	NEW(this->st_31,CStatic);
	this->st_31->Init();
	this->st_31->SetText("线路名称:");
	this->st_31->SetRect(76,17,56,15);

	NEW(this->st_32,CStatic);
	this->st_32->Init();
	this->st_32->SetText("结果:");
	this->st_32->SetRect(76,34,56,18);

	NEW(this->cb_31,CComboBox);
	this->cb_31->Init();
	this->cb_31->SetRect(136,17,115,15);

	NEW(this->btn_31,CButton);
	this->btn_31->Init();
	this->btn_31->SetText("重填");
	this->btn_31->SetRect(176,34,35,18);

	NEW(this->btn_32,CButton);
	this->btn_32->Init();
	this->btn_32->SetText("查询");
	this->btn_32->SetRect(216,34,35,18);

	this->mf_txt->SetSize(0);
	NEW(this->eb_result_31,CEditBox);
	this->eb_result_31->Init();
	this->eb_result_31->SetStyle(WS_BORDER|WS_VSCROLL);
	this->eb_result_31->SetRect(76,53,174,114);
	this->eb_result_31->SetTextFile(this->mf_txt);

	this->AddControl(this->st_31);
	this->AddControl(this->st_32);
	this->AddControl(this->cb_31);
	this->AddControl(this->btn_31);
	this->AddControl(this->btn_32);
	this->AddControl(this->eb_result_31);

	return OK;
}

int CWndBus::OnUnknown(u32 message, int wparam, int lparam)
{
	if(lparam && lparam == this->win_id_process)
	{
		if(message == WM_MSGBOX_CANCEL)
			this->bus_map->StopSearchPath();
	}
	return OK;
}

int CWndBus::OnClose(int wparam, int lparam)
{
	if(this->in_loop)
		return ERROR;
	return CWindow::OnClose(wparam,lparam);
}

int CWndBus::ShowMsgBox(char *title, char *str, int type)
{
	afx.wndmgr->SendMessage(this->win_id_msgbox,WM_MSGBOX_QUIT,0,0);
	this->win_id_msgbox = CWndMsgBox::Create(this->id,title,str,type);
	
	return OK;
}

int CWndBus::call_back_search(void *p)
{
	int *pi = (int*)p;
	CWndBus *self;

	self = (CWndBus*)pi[0];

	pi[2] ++;
	if(pi[2] >= 1000)
	{
		pi[2] = 0;

		self->mem_process->SetSize(0);
		self->mem_process->Puts("已查询");
		self->mem_process->Printf(" %d ",1000 * pi[1]++);
		self->mem_process->Puts("条路线.");
		self->mem_process->Putc(0);

		if(!afx.wndmgr->IsWindow(self->win_id_process))
			self->win_id_process = CWndMsgBox::Create(self->id,"正在查询...",self->mem_process->p,MB_CANCEL);
		
		afx.wndmgr->SendMessage(self->win_id_process,WM_MSGBOX_SETTEXT,0,(int)self->mem_process->p);

		MsgLoop();
	}

	return OK;
}

int CWndBus::SearchPath()
{
	int param[3];
	CMem mem1,mem2;

	LOCAL_MEM(mem1);
	LOCAL_MEM(mem2);

	param[0] = (int)this;
	param[1] = 0;
	param[2] = 999; //make call back process message immediately.
	
	if(this->in_loop)
		return ERROR;
	
	this->in_loop = true;

	this->bus_map->param_call_back_search = param;
	this->bus_map->call_back_search = this->call_back_search;
	
	this->cb_ss->GetText(&mem1);
	this->cb_es->GetText(&mem2);

	this->bus_map->FindPath(mem1.p,mem2.p);
	this->bus_map->SortResult();

	this->mf_txt->SetSize(0);
	this->bus_map->PrintResult(this->mf_txt);
	
	this->eb_result_1->SetTextFile(this->mf_txt);	
	afx.wndmgr->CloseWindow(this->win_id_process);
	
	this->in_loop = false;

	return OK;
}

int CWndBus::LoadStations(CComboBox *cb)
{
	ASSERT(cb);

	CMem mem_txt;
	int i,pos;

	LOCAL_MEM(mem_txt);
	
	cb->GetText(&mem_txt);
	cb->ClearList();
	pos = this->bus_map->GetStationByName(&mem_txt);

	if(pos >= 0)
	{
		for(i = pos; i < pos+20; i++)
		{
			if(i >= 0 && i < this->bus_map->stk_station_name->GetLen())
			{
				cb->AddStr(this->bus_map->GetStationName(i));
			}
		}
	}

	return OK;
}

int CWndBus::SearchStation()
{
	CMem mem;
	int pos;

	LOCAL_MEM(mem);

	this->cb_21->GetText(&mem);
	pos = this->bus_map->GetStationByName(&mem);

	this->mf_txt->SetSize(0);
	if(pos >= 0)
	{
		this->bus_map->PrintAllBusAtStation(pos,this->mf_txt);
		this->eb_result_21->SetTextFile(this->mf_txt);
	}
	return OK;
}

int CWndBus::LoadBus(CComboBox *cb)
{
	ASSERT(cb);

	CMem mem_txt,*pmem;
	int i,pos,ff;

	LOCAL_MEM(mem_txt);
	
	cb->GetText(&mem_txt);
	cb->ClearList();
	pos = this->bus_map->stk_bus_name->BSearch_Pos(&mem_txt,0,&ff);

	if(pos >= 0)
	{
		for(i = pos; i < pos+20; i++)
		{
			if(i >= 0 && i < this->bus_map->stk_bus_name->GetLen())
			{
				pmem = this->bus_map->stk_bus_name->GetElem(i);
				ASSERT(pmem);
				cb->AddStr(pmem->p);
			}
		}
	}

	return OK;
}

int CWndBus::SearchBus()
{
	CMem mem;
	int bus_id;

	LOCAL_MEM(mem);

	this->cb_31->GetText(&mem);

	bus_id = this->bus_map->GetBusByName(&mem);

	this->mf_txt->SetSize(0);

	this->bus_map->PrintBus(bus_id,this->mf_txt);
	this->bus_map->PrintBus(bus_id+1,this->mf_txt);
	this->eb_result_31->SetTextFile(this->mf_txt);
	
	return OK;
}

int CWndBus::CreateGroup4Controls()
{
	if( this->cur_control_group == 4 )
		return OK;

	afx.caret->StopBlink();
	this->DelAllControls();

	this->cur_control_group = 4;

	NEW(this->st_41,CStatic);
	this->st_41->Init();
	this->st_41->SetText("请选择数据库:");
	this->st_41->SetRect(76,17,100,16);

	NEW(this->lb_41,CListBox);
	this->lb_41->Init();
	this->lb_41->SetStyle(WS_BORDER|WS_VSCROLL);
	this->lb_41->SetRect(76,40,170,100);

	this->LoadDatabaseList(this->lb_41);

	NEW(this->btn_41,CButton);
	this->btn_41->Init();
	this->btn_41->SetText("加载数据");
	this->btn_41->SetRect(76,144,70,18);

	this->AddControl(this->lb_41);
	this->AddControl(this->st_41);
	this->AddControl(this->btn_41);


	return OK;
}

int CWndBus::IsDataLoaded()
{
	return this->bus_map->stk_bus_name->GetLen() > 0;
}

int CWndBus::LoadDatabaseList(CListBox *lb)
{
	ASSERT(lb);

	CMemFile mf;
	CMem mem,ext;
	CDirMgr dir;		

	LOCAL_MEM(ext);
	LOCAL_MEM(mem);

	mf.Init();
	dir.Init();

	dir.ChDir(BUS_DATA_DIR);

	ASSERT( dir.OpenDir() );
	dir.SearchDir(0,&mf);

	lb->DelAll();
	mf.Seek(0);
	while(mf.ReadLine(&mem))
	{
		if(mem.p[0] == 0 || mem.p[0] == '<')
			continue;
		CDirMgr::GetFileName(mem.p,&ext,FN_EXT);
		if(ext.StrICmp("bus") == 0)
		{
			CFile file;
			file.Init();
			ASSERT( file.OpenFile(mem.p,"rb") );
			file.Seek(0);
			file.ReadLine(&mem);
			lb->AddStr(mem.p);
		}
	}

	return OK;
}

int CWndBus::LoadData()
{
	if(this->in_loop)
		return ERROR;

	CMemFile mf;
	CMem mem,ext,*pmem;
	CDirMgr dir;		
	
	LOCAL_MEM(ext);
	LOCAL_MEM(mem);
	
	pmem = this->lb_41->GetSelText();
	if(pmem == NULL)
		return ERROR;
	
	mf.Init();
	dir.Init();
	
	dir.ChDir(BUS_DATA_DIR);
	ASSERT( dir.OpenDir() ); 
	dir.SearchDir(0,&mf);
	
	mf.Seek(0);
	while(mf.ReadLine(&mem))
	{
		if(mem.p[0] == 0 || mem.p[0] == '<')
			continue;
		CDirMgr::GetFileName(mem.p,&ext,FN_EXT);
		if(ext.StrICmp("bus") == 0)
		{
			CFile file;
			file.Init();
			ASSERT( file.OpenFile(mem.p,"rb") );
			file.Seek(0);			
			file.ReadLine(&ext);
			if(ext.StrICmp(pmem->p) == 0)
			{
				file.Destroy();
				
				this->in_loop = TRUE;
				this->ShowMsgBox("请等待","正在加载数据...",0);
				this->bus_map->Destroy();
				this->bus_map->Init();
				this->bus_map->LoadMapFile(mem.p);
				this->ShowMsgBox("完成","数据加载完成!",MB_OK);
				
				this->mem_caption->SetSize(0);
				this->mem_caption->Puts("NDS公交查询-");
				this->mem_caption->Puts(pmem->p);
				this->mem_caption->Putc(0);
				this->SetCaption(this->mem_caption->p);

				this->in_loop = FALSE;				
				break;
			}
		}
	}
	
	return OK;
}
