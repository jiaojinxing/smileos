#include "moongui.h"
#include "wndsoftkb.h"
#include "taskbar.h"
#include "wndreader.h"
#include "wndfilemgr.h"
#include "upscreen.h"

int main(void) 
{
	init_all();	
	CTaskBar *task_bar;
	
	NEW(task_bar,CTaskBar);
	task_bar->Init();
	task_bar->SetRect(-1,GAL_GetHeight() - 20,GAL_GetWidth()+2,21);
	task_bar->dwStyle = WS_VISIBLE|WS_BORDER|WS_NOACTIVE|WS_NOFOCUS;
	task_bar->dwExStyle = WS_EX_TOPMOST;
	afx.wndmgr->AddWindow(task_bar);
	
	while(MsgLoop());
	
	return OK;
}
