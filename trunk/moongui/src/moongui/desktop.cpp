#include "desktop.h"
#include "colormap.h"

CDesktop::CDesktop()
{
    this->InitBasic();
}
CDesktop::~CDesktop()
{
    this->Destroy();
}
int CDesktop::InitBasic()
{
    CWindow::InitBasic();
	this->st_1 = NULL;
    return OK;
}
int CDesktop::Init()
{
    this->InitBasic();
    CWindow::Init();
    //add your code
    return OK;
}
int CDesktop::Destroy()
{
    //add your code
    CWindow::Destroy();
    this->InitBasic();
    return OK;
}

CWindow * CDesktop::CreateDesktop()
{
	CDesktop *desktop;

	NEW(desktop,CDesktop);
	desktop->Init();

	desktop->rect->Set(0,0,GAL_GetWidth(),GAL_GetHeight());
	desktop->dwStyle = WS_VISIBLE;
	desktop->dwExStyle = WS_EX_LOWMOST;
	desktop->bkcolor = COLOR_LIGHTCYAN;

	return desktop;
}

int CDesktop::OnCreate(int wparam, int lparam)
{
	NEW(this->st_1,CStatic);
	this->st_1->Init();
	this->st_1->fore_color = COLOR_YELLOW;
	this->st_1->SetRect(215,150,100,30);
	this->st_1->SetText("³ÂÏòÅó");
	this->AddControl(this->st_1);

	return OK;
}
