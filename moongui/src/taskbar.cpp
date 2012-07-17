#include "taskbar.h"
#include "wndsoftkb.h"
#include "wndbus.h"
#include "wndreader.h"
#include "wndmine.h"
#if _NDS_
#include "wnddocreader.h"
#endif
#include "upscreen.h"

CTaskBar::CTaskBar()
{
    this->InitBasic();
}
CTaskBar::~CTaskBar()
{
    this->Destroy();
}
int CTaskBar::InitBasic()
{
    CWindow::InitBasic();
	this->btn_start = NULL;
	this->menu_start = NULL;
	this->menu_window = NULL;
	this->mi_about = NULL;
	this->mi_game = NULL;
	this->mi_program = NULL;
	this->mi_window = NULL;
	this->mi_bus = NULL;
	this->mi_calcu = NULL;
	this->mi_mine = NULL;
	this->mi_reader = NULL;
	this->mi_close_all = NULL;
	this->mi_repos = NULL;
	this->rect_softkb = NULL;
	this->win_id_softkb = 0;
	this->mi_dict = NULL;
	this->win_id_bus = 0;
	this->win_id_about = 0;
	this->win_id_reader = 0;
	this->win_id_wine = 0;
#if _NDS_
	this->win_id_doc_reader = 0;
#endif
	this->win_id_dict = 0;

	this->mi_clear_console = NULL;

    return OK;
}
int CTaskBar::Init()
{
    this->InitBasic();
    CWindow::Init();

    return OK;
}
int CTaskBar::Destroy()
{
    CWindow::Destroy();
    this->InitBasic();
    return OK;
}
int CTaskBar::OnCreate(int wparam,int lparam)
{
	ImageInfo *img;

	NEW(this->rect_softkb,CRect);

	img = GetStockImage(IMAGE_SMALL_KB);
	this->rect->CenterRect(img->width,img->height,this->rect_softkb);
	this->SR2LR(this->rect_softkb);

	NEW(this->btn_start,CButton);
	this->btn_start->Init();
	this->btn_start->SetText("开始");
	this->btn_start->SetRect(3,2,36,18);
	this->btn_start->fore_color = COLOR_BLUE;

	this->menu_start = CMenuWnd::CreateMenuWnd(this->id);
	this->menu_game = CMenuWnd::CreateMenuWnd(this->id);
	this->menu_program =  CMenuWnd::CreateMenuWnd(this->id);
	this->menu_window = CMenuWnd::CreateMenuWnd(this->id);

	NEW(this->mi_about,CMenuItem);
	this->mi_about->Init();
	this->mi_about->SetName("关于程序");

	NEW(this->mi_game,CMenuItem);
	this->mi_game->Init();
	this->mi_game->SetName("游戏");

	NEW(this->mi_program,CMenuItem);
	this->mi_program->Init();
	this->mi_program->SetName("程序");

	NEW(this->mi_window,CMenuItem);
	this->mi_window->Init();
	this->mi_window->SetName("窗口管理");

	NEW(this->mi_close_all,CMenuItem);
	this->mi_close_all->Init();
	this->mi_close_all->SetName("关闭全部");
	
	NEW(this->mi_repos,CMenuItem);
	this->mi_repos->Init();
	this->mi_repos->SetName("重新排列");

	NEW(this->mi_clear_console,CMenuItem);
	this->mi_clear_console->Init();
	this->mi_clear_console->SetName("清除LOG信息");

	NEW(this->mi_calcu,CMenuItem);
	this->mi_calcu->Init();
	this->mi_calcu->SetName("计算器");

	NEW(this->mi_bus,CMenuItem);
	this->mi_bus->Init();
	this->mi_bus->SetName("公交车查询");
	
	NEW(this->mi_mine,CMenuItem);
	this->mi_mine->Init();
	this->mi_mine->SetName("扫雷");

	NEW(this->mi_reader,CMenuItem);
	this->mi_reader->Init();
	this->mi_reader->SetName("文本阅读器");

#if _NDS_
	NEW(this->mi_doc_reader,CMenuItem);
	this->mi_doc_reader->Init();
	this->mi_doc_reader->SetName("DOT阅读器");
#endif

	NEW(this->mi_dict,CMenuItem);
	this->mi_dict->Init();
	this->mi_dict->SetName("英汉字典");

	this->menu_start->AddMenuItem(this->mi_game);
	this->menu_start->AddMenuItem(this->mi_program);
	this->menu_start->AddMenuItem(this->mi_window);	
	this->menu_start->AddSeparator();
	this->menu_start->AddMenuItem(this->mi_about);	

	this->menu_game->AddMenuItem(this->mi_mine);
	this->menu_program->AddMenuItem(this->mi_calcu);
	this->menu_program->AddMenuItem(this->mi_bus);
	this->menu_program->AddMenuItem(this->mi_reader);
#if _NDS_
	this->menu_program->AddMenuItem(this->mi_doc_reader);
#endif
	this->menu_program->AddMenuItem(this->mi_dict);

	this->menu_window->AddMenuItem(this->mi_repos);
	this->menu_window->AddMenuItem(this->mi_clear_console);		
	this->menu_window->AddMenuItem(this->mi_close_all);
	
	this->mi_window->SetChildMenu(this->menu_window->id);
	this->mi_game->SetChildMenu(this->menu_game->id);
	this->mi_program->SetChildMenu(this->menu_program->id);

	this->AddControl(this->btn_start);

	return OK;
}
int CTaskBar::OnLButtonDown(int wparam,int lparam)
{
	int	x = (short)LOWORD(lparam);	
	int	y = (short)HIWORD(lparam);
	if(this->rect_softkb->PtInRect(x,y))
	{
		CWindow *pwnd = afx.wndmgr->GetWindowByID(this->win_id_softkb);
		if(pwnd)
		{
			pwnd->Close();
		}
		else
		{
			CWndSoftKb *soft_kb;

			NEW(soft_kb,CWndSoftKb);
			soft_kb->Init();
			soft_kb->SetRect(10,-10,225,180);
			soft_kb->i_caption = "软键盘";
			soft_kb->dwStyle = WS_CAPTION|WS_VISIBLE|WS_BORDER|WS_NOACTIVE|WS_NOFOCUS|WS_APPWND;
			soft_kb->dwExStyle = WS_EX_TOPMOST;
			afx.wndmgr->AddWindow(soft_kb);
			this->win_id_softkb = soft_kb->id;
		}
	}
	return OK;
}
int CTaskBar::OnCommand(int wparam,int lparam)
{
	if(lparam == this->btn_start->id)
	{
		int x = 3;
		int y = GAL_GetHeight() - this->menu_start->GetWndHeight() - this->rect->GetHeight() + 3;
		this->menu_start->ShowAt(x,y);
	}
	
	if(lparam == this->mi_bus->id)
	{
		if(!afx.wndmgr->IsWindow(this->win_id_bus))
		{
			CWndBus *pwnd;
			
			NEW(pwnd,CWndBus);
			pwnd->Init();
			pwnd->SetRect(0,0,255,170);
			pwnd->dwStyle =  WS_CAPTION|WS_VISIBLE|WS_BORDER|WS_APPWND;
			afx.wndmgr->AddWindow(pwnd);
			this->win_id_bus = pwnd->id;
		}
	}

	if(lparam == this->mi_mine->id)
	{
		if(! afx.wndmgr->IsWindow(this->win_id_wine))
		{
			CWndMine *wnd_mine;
			NEW(wnd_mine,CWndMine);
			wnd_mine->Init();
			wnd_mine->SetRect(0,0,118,150);
			wnd_mine->SetCaption("扫雷 - NDS");
			wnd_mine->dwStyle = WS_CAPTION|WS_VISIBLE|WS_BORDER|WS_APPWND;
			wnd_mine->dwExStyle = 0;
			afx.wndmgr->AddWindow(wnd_mine);
			this->win_id_wine = wnd_mine->id;
		}
	}

#if _NDS_
	if(lparam == this->mi_doc_reader->id)
	{
		if(!afx.wndmgr->IsWindow(this->win_id_doc_reader))
		{
			CWndDocReader *wnd_doc_reader;

			NEW(wnd_doc_reader,CWndDocReader);
			wnd_doc_reader->Init();
			wnd_doc_reader->SetRect(0,0,246,170);
			wnd_doc_reader->SetCaption("NDS-文档阅读器");
			wnd_doc_reader->dwStyle = WS_CAPTION|WS_VISIBLE|WS_BORDER|WS_APPWND;
			wnd_doc_reader->dwExStyle = 0;
			afx.wndmgr->AddWindow(wnd_doc_reader);

			this->win_id_doc_reader = wnd_doc_reader->id;
		}
	}
#endif

	if(lparam == this->mi_reader->id)
	{
		if(!afx.wndmgr->IsWindow(this->win_id_reader))
		{
			
			CWndReader *wnd_reader;

			NEW(wnd_reader,CWndReader);
			wnd_reader->Init();
			wnd_reader->SetRect(0,0,255,171);
			wnd_reader->SetCaption("NDS 文本阅读器");
			wnd_reader->dwStyle = WS_CAPTION|WS_VISIBLE|WS_BORDER|WS_APPWND;
			wnd_reader->dwExStyle = 0;
			afx.wndmgr->AddWindow(wnd_reader);
			
			this->win_id_reader = wnd_reader->id;
		}
	}
	
	if(lparam == this->mi_dict->id)
	{
	}
	if(lparam == this->mi_about->id)
	{
		if(!afx.wndmgr->IsWindow(this->win_id_about))
			this->win_id_about = CWndMsgBox::Create(this->id,"关于本软件","陈向朋制作。\nxiangpeng_chen@126.com",MB_OK);
	}

	if(lparam == this->mi_close_all->id)
	{
		afx.wndmgr->CloseAllAppWindows();
	}

	if(lparam == this->mi_repos->id)
	{
		afx.wndmgr->ReposAllAppWindows();
	}

	if(lparam == this->mi_clear_console->id)
	{
	    #if _NDS_
		CUpScreen::ClearConsole();
        #endif
	}

	return OK;
}
int CTaskBar::OnUnknown(u32 message,int wparam,int lparam)
{
	return OK;
}
int CTaskBar::OnTimer(int wparam,int lparam)
{
	return OK;
}
int CTaskBar::OnPaint(int wparam,int lparam)
{
	ImageInfo *img;
	CDc *pdc = this->BeginPaint(lparam);
	
	img = GetStockImage(IMAGE_SMALL_KB);
	pdc->DrawMonoImage(this->rect_softkb->left,this->rect_softkb->top,img,RGB(128,128,255));

	this->EndPaint();
	return OK;
}
