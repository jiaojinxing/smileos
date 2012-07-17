#include "event.h"
#include "globals.h"
#include "gui_time.h"

int GetMessage(CMsg *msg)
{
	return afx.msgque->DeQueue(msg);
}

int TranslateMessage(CMsg *msg)
{
	int index,x,y;
	CWindow *pwnd;
	
	if(msg->wnd_id >= 0)
		return ERROR;
	
	if(msg->message == WM_SYS_DESTROY)
	{
		afx.wndmgr->DestroyWindow(msg->lparam);
	}
	else if(msg->message == WM_SYS_LBUTTONDOWN)
	{
		index = afx.wndmgr->GetCaptureWindowIndex();
		if(index < 0)
			index = afx.wndmgr->PtInWhich(msg->wparam,msg->lparam);
		
		pwnd = afx.wndmgr->GetElem(index);
		if(pwnd == NULL) return ERROR;

		if(!pwnd->IsMenuWnd())
			afx.wndmgr->HideAllMenuWindows();

		if(pwnd->PtInCloseBox(msg->wparam,msg->lparam) && !(pwnd->dwStyle & WS_CLOSEBOXCHECKED))
		{
			pwnd->dwStyle |= WS_CLOSEBOXCHECKED;
			afx.wndmgr->RedrawTitleBar(index);
		}
		
		afx.wndmgr->SetActiveWindow(index);
		afx.wndmgr->SetFocusWindow(index);
		afx.wndmgr->BringWindowToTop(index); //may change the windows order

		x = msg->wparam; y = msg->lparam;
		pwnd->SP2LP(&x,&y);
		pwnd->SendMessage(WM_LBUTTONDOWN,0,MAKELONG(x,y));

		if(pwnd->PtInTitleBar(msg->wparam,msg->lparam) && !pwnd->PtInCloseBox(msg->wparam,msg->lparam))
		{
			afx.mouse->SetForceHide(1);
			afx.drag->StartDrag(pwnd,msg->wparam,msg->lparam);
			afx.drag->DrawXORect();
		}
	}
	else if(msg->message == WM_SYS_LBUTTONUP)
	{
		if(afx.drag->on_drag)
		{
			afx.drag->DrawXORect();
			afx.drag->EndDrag();		

			index = afx.wndmgr->IdToIndex(afx.drag->win_id);
			if(index >= 0)
			{
				afx.wndmgr->MoveWindow(index,msg->wparam - afx.drag->off_x, msg->lparam - afx.drag->off_y);
			}

			afx.mouse->SetForceHide(0);		
			
			return OK;
		}
		index = afx.wndmgr->GetCaptureWindowIndex();
		if(index < 0)
			index = afx.wndmgr->PtInWhich(msg->wparam,msg->lparam);
		pwnd = afx.wndmgr->GetElem(index);
		if(pwnd == NULL)
			return ERROR;
		
		if(pwnd->PtInCloseBox(msg->wparam,msg->lparam) && (pwnd->dwStyle & WS_CLOSEBOXCHECKED))
		{
			pwnd->dwStyle &= ~WS_CLOSEBOXCHECKED;
			afx.wndmgr->RedrawTitleBar(index);
			afx.wndmgr->PostMessage(pwnd->id,WM_CLOSE,0,0);
		}	
		else 
		{
			x = msg->wparam; y = msg->lparam;
			pwnd->SP2LP(&x,&y);
			pwnd->SendMessage(WM_LBUTTONUP,0,MAKELONG(x,y));
		}
	}
	else if(msg->message == WM_SYS_MOUSEMOVE)
	{
		if(afx.drag->on_drag)
		{
			afx.drag->SetPos(msg->wparam,msg->lparam);
			afx.drag->DrawXORectAtOldPos();
			afx.drag->DrawXORect();

			return OK;
		}

		index = afx.wndmgr->GetCaptureWindowIndex();
		if(index < 0)
			index = afx.wndmgr->PtInWhich(afx.mouse->old_x,afx.mouse->old_y);
		pwnd = afx.wndmgr->GetElem(index);
		if(pwnd == NULL)
			return ERROR;
		
		if( ! pwnd->PtInCloseBox(afx.mouse->x,afx.mouse->y) )
		{	 
			if(pwnd->dwStyle & WS_CLOSEBOXCHECKED)
			{
				pwnd->dwStyle &= ~WS_CLOSEBOXCHECKED;
				afx.wndmgr->RedrawTitleBar(index);
			}
			else if(afx.wndmgr->PtInWhich(msg->wparam,msg->lparam) != index)
			{
				pwnd->SendMessage(WM_MOUSEOUT,0,0);
			}
			
			x = msg->wparam; y = msg->lparam;
			pwnd->SP2LP(&x,&y);
			pwnd->SendMessage(WM_MOUSEMOVE,0,MAKELONG(x,y));
		}		
	}
	else if(msg->message == WM_SYS_MOUSEHOLD)
	{
		index = afx.wndmgr->GetCaptureWindowIndex();
		if(index < 0)
			index = afx.wndmgr->PtInWhich(afx.mouse->old_x,afx.mouse->old_y);
		pwnd = afx.wndmgr->GetElem(index);
		if(pwnd == NULL)
			return ERROR;
		x = msg->wparam; y = msg->lparam;
		pwnd->SP2LP(&x,&y);
		pwnd->SendMessage(WM_MOUSEHOLD,0,MAKELONG(x,y));
	}
	else if(msg->message == WM_SYS_KEYDOWN)
	{
		index = afx.wndmgr->GetFocusWindow();
		pwnd = afx.wndmgr->GetElem(index);
		if(pwnd == NULL)
			return ERROR;

		pwnd->SendMessage(WM_KEYDOWN,0,msg->lparam);
	}
	else if(msg->message == WM_SYS_KEYUP)
	{
		index = afx.wndmgr->GetFocusWindow();
		pwnd = afx.wndmgr->GetElem(index);
		if(pwnd == NULL)
			return ERROR;

		pwnd->SendMessage(WM_KEYUP,0,msg->lparam);
	}

	return OK;
}

int DispatchMessage(CMsg *msg)
{
	if(msg->wnd_id < 0)
		return ERROR;

	CWindow *pwnd = afx.wndmgr->GetElem(afx.wndmgr->IdToIndex(msg->wnd_id));
	
	if(pwnd == NULL)
		return ERROR;

	return pwnd->WndProc(msg->message,msg->wparam,msg->lparam);
}

int HandleCaret()
{
	if( ! afx.caret->is_blink )
		return ERROR;

	if(get_sys_time() - afx.caret->old_time >= afx.caret->time_out)
	{
		afx.caret->old_time = get_sys_time();
		if(afx.caret->is_show)
			afx.caret->Hide();
		else
			afx.caret->Show();
	}

	return OK;
}

int MsgLoop()
{
	CMsg msg;
	int i,len;

	afx.mouse->ReadMouse();
	afx.mouse->Refresh();

	HandleCaret();
	GetLowLevelMessage();

	len = afx.msgque->GetLen();
	for(i = 0; i < len; i++)
	{
		if(GetMessage(&msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return OK;
}

int GetLowLevelMessage()
{
	GetMouseMessage();
	GetTimerEvent();
	GetKeyboardEvent();

	return OK;
}

int GetMouseMessage()
{
	int now_time = get_sys_time();

	if(afx.mouse->PosChanged())
	{
		afx.msgque->PostMessage(-1,WM_SYS_MOUSEMOVE,afx.mouse->x,afx.mouse->y);
	}

	if(afx.mouse->ButtonChanged())
	{
		if(afx.mouse->button == MOUSE_BUTTON_LEFT && afx.mouse->old_button == MOUSE_BUTTON_NONE)
		{
			afx.msgque->PostMessage(-1,WM_SYS_LBUTTONDOWN,afx.mouse->x,afx.mouse->y);
			afx.mouse->hold_start = TRUE;
			afx.mouse->old_time = now_time;
		}
		else if(afx.mouse->button == MOUSE_BUTTON_NONE && afx.mouse->old_button == MOUSE_BUTTON_LEFT)
		{
			afx.msgque->PostMessage(-1,WM_SYS_LBUTTONUP,afx.mouse->x,afx.mouse->y);
			afx.mouse->hold_start = FALSE;
			afx.mouse->old_time = now_time;
		}
	}
	
	if(afx.mouse->hold_start)
	{
		if(afx.mouse->PosChangeMuch())
		{
			afx.mouse->old_time = now_time;
		}
		else if(afx.mouse->button == MOUSE_BUTTON_LEFT)
		{
			if(now_time - afx.mouse->old_time > afx.mouse->time_out)
			{
				afx.mouse->hold_start = FALSE;
				afx.mouse->old_button = now_time;
				afx.msgque->PostMessage(-1,WM_SYS_MOUSEHOLD,afx.mouse->x,afx.mouse->y);				
			}
		}
		else if(afx.mouse->button != MOUSE_BUTTON_LEFT)
		{
			afx.mouse->hold_start = FALSE;
			afx.mouse->old_time = now_time;
		}
	}

	return OK;
}

int GetTimerEvent()
{
	int i,now_time;
	CTimer *pt;

	now_time = get_sys_time();

	for(i = 0; i < afx.timemgr->GetLen(); i++)
	{
		pt = afx.timemgr->GetElem(i);
		ASSERT(pt);

		if(pt->enabled && (now_time - pt->old_time >= pt->time_out))
		{
			pt->old_time = now_time;
			afx.wndmgr->PostMessage(pt->win_id,WM_TIMER,pt->data,pt->id);
		}
	}
	return OK;
}

int GetKeyboardEvent()
{
	int i,time;

	afx.kb->ReadKey();
	time = get_sys_time();

	for(i = KEY_MIN; i < KEY_MAX; i++)
	{
		if(afx.kb->IsKeyPress(i))
		{
			if(time - afx.kb->key_timer > 300)
			{
				afx.kb->key_timer = time;
				afx.wndmgr->PostMessage(-1,WM_SYS_KEYDOWN,0,i);
			}
		}
		else if(afx.kb->IsOldKeyPress(i))
		{
			afx.wndmgr->PostMessage(-1,WM_SYS_KEYUP,0,i);
		}
	}

	return OK;
}
