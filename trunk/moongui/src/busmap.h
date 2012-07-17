// BusMap.h: interface for the CBusMap class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BUSMAP_H__D43FA22F_EB7C_4F34_957A_C0C15A35682E__INCLUDED_)
#define AFX_BUSMAP_H__D43FA22F_EB7C_4F34_957A_C0C15A35682E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "intstk.h"
#include "memstk.h"

class CBusMap{
public:
	CMem *mem_map_name;
	CMemStk *stk_bus_name;
	CMemStk *stk_station_name;
	CMemStk *stk_disp;
	CIntStk *stk_all_bus;  
	void *param_call_back;
	int (*call_back_find)(void *p,CFileBase *file_path,int total_station);
	void *param_call_back_search;
	int (*call_back_search)(void *p);
	CIntStk *stk_result_times;
	CMemStk *stk_result_path;
	int end_flag;
public:
	int StopSearchPath();
	int GetBusDir(int bus_id);
	int PrintAllBusAtStation(int station_id,CFileBase *file);
	int MsgLoop();
	int GetBusByName(char *bus_name);
	int GetBusByName(CMem *bus_name);
	int PrintResult(CFileBase *file);
	int SortResult();
	int AddResult(int hc_times,CFileBase *file_path);
	int ClearResult();
	int FindPath(char *station_start,char *station_end);
	static int call_back_default(void *p,CFileBase *file_log,int total_station);
	int GetAllTwoRelayPath(int start_station_id,int end_station_id,CFileBase *file_log,int station_count);
	int GetBusNameId(int bus_id);
	int GetAllOneRelayPath(int start_bus_id,int start_station_id,int end_station_id,CFileBase *file_log,int station_count);
	char * GetStationName(int station_id);
	char * GetBusName(int bus_id);
	int GetAllZeroRelayPath(int start_bus_id1,int start_bus_id2,int start_station_id,int end_station_id,CFileBase *file_log,int station_count);
	int GetAllBusAtStation(int station_id,CIntStk *stk);
	int GetStationByName(char *station_name);
	int GetStationByName(CMem *station_name);
	int IsBusPassStation(int bus_id,int station_id);
	int GetBusCount();
	int PrintBus(int bus_id,CFileBase *file);
	CIntStk * GetBus(int bus_id);
	int AddBus(CMem *mem);
	int LocateSpLine(CFileBase *file,char *line);
	int LoadMapFile(char *fn);
	int LoadMapFile(CFileBase *file);
	int AddBus(CIntStk *stk);
    CBusMap();
    virtual ~CBusMap();
    int Init();
    int Destroy();
    int Copy(CBusMap *p);
    int Comp(CBusMap *p);
    int Print();
    int InitBasic();
};

#endif // !defined(AFX_BUSMAP_H__D43FA22F_EB7C_4F34_957A_C0C15A35682E__INCLUDED_)
