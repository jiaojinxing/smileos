#ifndef __WNDBUS_H
#define __WNDBUS_H

#include "moongui.h"
#include "busmap.h"

class CWndBus:public CWindow{
public:
	CListBox *lb_tab;
	int cur_control_group;
	CBusMap *bus_map;
	int in_loop;
	int win_id_msgbox;	
	int win_id_process;
	CMem *mem_process;
	CMem *mem_caption;
	//group 1 contorls
	CButton *btn_reset, *btn_search;
	CStatic *st_1,*st_2,*st_3;
	CComboBox *cb_ss,*cb_es;
	CEditBox *eb_result_1;
	CMemFile *mf_txt;
	//group 2 contorls
	CButton *btn_21,*btn_22;
	CComboBox *cb_21;
	CStatic *st_21,*st_22;
	CEditBox *eb_result_21;
	//group 3 controls
	CButton *btn_31,*btn_32;
	CComboBox *cb_31;
	CStatic *st_31,*st_32;
	CEditBox *eb_result_31;
	//group 4 controls
	CStatic *st_41;
	CListBox *lb_41;
	CButton *btn_41;
public:
	int LoadData();
	int LoadDatabaseList(CListBox *lb);
	int IsDataLoaded();
	int CreateGroup4Controls();
	int SearchBus();
	int LoadBus(CComboBox *cb);
	int SearchStation();
	int LoadStations(CComboBox *cb);
	int SearchPath();
	static int call_back_search(void *p);
	int ShowMsgBox(char *title,char *str,int type);
	int OnClose(int wparam,int lparam);
	int OnUnknown(u32 message,int wparam,int lparam);
	int CreateGroup3Controls();
	int CreateGroup2Controls();
	int DelAllControls();
	int CreateGroup1Controls();
    CWndBus();
    virtual ~CWndBus();
    int Init();
    int Destroy();
    int InitBasic();
	int OnCreate(int wparam,int lparam);
	int OnCommand(int wparam,int lparam);
};

#endif
