#ifndef __WNDMGR_H
#define __WNDMGR_H

#include "window.h"

class CWndMgr{
public:
    CWindow **index;
    long top;
    long size;
public:
	int ReposAllAppWindows();
	int CloseAllAppWindows();
	int SetWindowSize(int win_id,int w,int h);
	int CloseWindow(int win_id);
	int HideAllMenuWindows();
	int DelChildWindowsByIndex(int index);
	int ShowWindowByIndex(int index);
	int HideWindowByIndex(int index);
	int GetCaptureWindowIndex();
	int ReleaseCaptureByIndex(int index);
	int SetCaptureByIndex(int index);
	int RedrawTitleBar(int i);
	int MoveWindow(int index,int x,int y);
	CWindow * GetWindowByID(int win_id);
	int MoveWindow(int index,int x,int y,int w,int h);
	int IdToIndex(int win_id);
	int InvalidateRect(int index,CRect *rect,int erase_back);
	int DestroyWindow(int win_id);
	int SetFocusWindow(int i);
	CWindow * RemoveElem(int i);
	int BringWindowToTop(CWindow *pwnd);
	int BringWindowToTop(int i);
	int PtInWhich(int x,int y);
	int GetFocusWindow();
	int GetActiveWindow();
	int SetActiveWindow(CWindow *pwnd);
	int SendMessage(int wnd_id, u32 message, int wparam, int lparam);
	int PostMessage(int wnd_id, u32 message, int wparam, int lparam);
	int DrawAll();
	int RecalcuClipRgn(int i);
	int RecalcuAllRgn();
	int SetActiveWindow(int i);
	int IsWindow(int win_id);
	int IsVisible(int i);
	int IsLowerThan(int i,int j);
	int IsTopThan(int i,int j);
	int WindowToIndex(CWindow *pwnd);
	int RecalcuClipRgn(CWindow *pwnd);
	int AddWindow(CWindow *pwnd);
    int InitBasic();  
    int DelElem(long i);
    CWindow * GetElem(long index);
    CWindow * GetTop();
    CWndMgr();
    int Clear();
    int DelTop();
    int Destroy();
    int Init(long init_size);
    int IsEmpty();
    int IsFull();
    CWindow * Pop();
    int Print();
    int Push(CWindow *node);
    long GetLen();
    ~CWndMgr();
};

#endif
