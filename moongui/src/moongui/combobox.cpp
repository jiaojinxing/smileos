#include "combobox.h"
#include "gui_misc.h"
#include "globals.h"

#define FIXED_COMBOBOX_HEIGHT  15

CComboBox::CComboBox()
{
    this->InitBasic();
}
CComboBox::~CComboBox()
{
    this->Destroy();
}
int CComboBox::InitBasic()
{
    CControl::InitBasic();
	this->btn_down = NULL;
	this->leb_txt = NULL;
	this->wnd_drop_down = NULL;

    return OK;
}
int CComboBox::Init()
{
    this->InitBasic();
    CControl::Init();
	this->ctrl_name = CTRL_NAME_COMBO_BOX;
	
	NEW(this->btn_down,CButton);
	this->btn_down->Init();
	this->btn_down->SetStockImage(IMAGE_DOWN_SMALL_ARROW);
	
	NEW(this->leb_txt,CLineEditor);
	this->leb_txt->Init();

	this->wnd_drop_down = CWndDropDown::CreateDropDownWindow(this->win_id);

    return OK;
}
int CComboBox::Destroy()
{
    DEL(this->btn_down);
	DEL(this->leb_txt);
	this->wnd_drop_down->Close();

    CControl::Destroy();
    this->InitBasic();
    return OK;
}
int CComboBox::Copy(CComboBox *p)
{
    if(this == p)
        return OK;
    CControl::Copy(p);
    return OK;
}
int CComboBox::Comp(CComboBox *p)
{
    return 0;
}
int CComboBox::Print()
{
    CControl::Print();
    return TRUE;
}

int CComboBox::Draw(CDc *pdc)
{	
	CRect rc;

	this->btn_down->Draw(pdc);	
	this->leb_txt->Draw(pdc);

	Draw3dThinDownBorder(pdc,this->rect);
	return OK;
}

int CComboBox::HandleMessage(int message, int wparam, int lparam)
{
	int x,y;

	this->btn_down->HandleMessage(message,wparam,lparam);
	this->leb_txt->HandleMessage(message,wparam,lparam);

	switch(message)
	{
		case WM_LBUTTONDOWN: 
			x = (short)LOWORD(lparam);	
			y = (short)HIWORD(lparam);

			if(!this->PtInCtrl(x,y))
				return ERROR;
			
			afx.ctrlmgr->SetFocus(this->id);
		break;

		case WM_CHAR:		
			if(this->HasFocus())
			{
				this->leb_txt->OnChar(wparam,lparam);
			}
		break;

		case WM_COMMAND:
			if(lparam == this->btn_down->id)
			{
				CRect rc;
				CWindow *pwnd;

				pwnd = afx.wndmgr->GetWindowByID(this->win_id);
				ASSERT(pwnd);
				rc.Copy(this->rect);
				pwnd->LR2SR(&rc);
				afx.wndmgr->SendMessage(this->win_id,WM_COMMAND,LBN_ABOUTSHOW,this->id);
				this->wnd_drop_down->ShowAt(rc.left,rc.bottom,rc.GetWidth());
			}

			if(lparam == this->wnd_drop_down->listbox->id)
			{
				CMem *pmem = this->wnd_drop_down->listbox->GetSelText();
				if(pmem == NULL)
					return ERROR;
				this->leb_txt->SetText(pmem->p);
			}
		break;
	}
	return ERROR;
}

int CComboBox::OnAdded()
{
	this->wnd_drop_down->SetParent(this->win_id);
	this->leb_txt->SetParent(this->win_id);
	this->btn_down->SetParent(this->win_id);
	
	this->leb_txt->OnAdded();
	this->btn_down->OnAdded();

	this->rect->bottom = this->rect->top + FIXED_COMBOBOX_HEIGHT;
	
	this->leb_txt->SetRect(this->rect);
	this->leb_txt->rect->right -= FIXED_COMBOBOX_HEIGHT;
	
	this->btn_down->SetRect(this->rect);
	
	this->btn_down->rect->left = this->btn_down->rect->right - FIXED_COMBOBOX_HEIGHT;
	this->btn_down->rect->top ++;	
	this->btn_down->rect->bottom --;
	this->btn_down->rect->right --;

	return OK;
}

int CComboBox::AddStr(char *str)
{
	return this->wnd_drop_down->AddStr(str);
}

int CComboBox::SetText(char *str)
{
	return this->leb_txt->SetText(str);
}

int CComboBox::GetText(CFileBase *file)
{
	return this->leb_txt->GetText(file);
}

int CComboBox::ClearList()
{
	return this->wnd_drop_down->ClearList();
}
