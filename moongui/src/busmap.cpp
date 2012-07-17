// BusMap.cpp: implementation of the CBusMap class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "busmap.h"
#include "memfile.h"
#include "event.h"

#define BUS_DIR_ERR		 0
#define BUS_DIR_UP		 1
#define BUS_DIR_DOWN	 2
#define BUS_DIR_CIRCLE	 3
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CBusMap::CBusMap()
{
    this->InitBasic();
}
CBusMap::~CBusMap()
{
    this->Destroy();
}
int CBusMap::InitBasic()
{
	this->stk_all_bus = NULL;
	this->stk_bus_name = NULL;
	this->stk_disp = NULL;
	this->stk_station_name = NULL;
	this->mem_map_name = NULL;
	this->param_call_back = this;
	this->call_back_find = this->call_back_default;
	this->stk_result_path = NULL;
	this->stk_result_times = NULL;
	this->call_back_search = NULL;
	this->param_call_back_search = NULL;
	this->end_flag = 0;

    return OK;
}
int CBusMap::Init()
{
    this->InitBasic();

	NEW(this->mem_map_name,CMem);
	this->mem_map_name->Malloc(128);

    NEW(this->stk_all_bus,CIntStk);
	this->stk_all_bus->Init(2048);

	NEW(this->stk_bus_name,CMemStk);
	this->stk_bus_name->Init(2048);

	NEW(this->stk_disp,CMemStk);
	this->stk_disp->Init(2048);

	NEW(this->stk_station_name,CMemStk);
	this->stk_station_name->Init(2048);
	
	NEW(this->stk_result_times,CIntStk);
	this->stk_result_times->Init(256);

	NEW(this->stk_result_path,CMemStk);
	this->stk_result_path->Init(256);

    return OK;
}
int CBusMap::Destroy()
{
	INTSTK_DEL(stk_all_bus,CIntStk);
   	DEL(this->stk_all_bus);
	DEL(this->stk_bus_name);
	DEL(this->stk_disp);
	DEL(this->stk_station_name);
	DEL(this->mem_map_name);
	DEL(this->stk_result_path);
	DEL(this->stk_result_times);

    this->InitBasic();
    return OK;
}
int CBusMap::Copy(CBusMap *p)
{
    if(this == p)
        return OK;
    return OK;
}
int CBusMap::Comp(CBusMap *p)
{
    return 0;
}
int CBusMap::Print()
{
    return TRUE;
}

int CBusMap::AddBus(CIntStk *stk)
{
	ASSERT(stk);

	this->stk_all_bus->Push((int)stk);
	return OK;
}

int CBusMap::LoadMapFile(CFileBase *file)
{
	ASSERT(file);
	CMem mem,buf;

	LOCAL_MEM(mem);
	LOCAL_MEM(buf);

	file->Seek(0);
	file->ReadLine(&mem);
	mem.SetSplitChars("[] \t");
	mem.Seek(0);
	mem.ReadString(this->mem_map_name);
	
	this->LocateSpLine(file,"[bus_name]");	
	while(file->ReadLine(&mem))
	{
		this->MsgLoop();
		if(mem.p[0] == '[')
		{
			file->ReadLine_Reverse();
			break;
		}
		this->stk_bus_name->Push(&mem);
	}
	this->LocateSpLine(file,"[station_name]");
	while(file->ReadLine(&mem))
	{
		this->MsgLoop();
		if(mem.p[0] == '[')
		{
			file->ReadLine_Reverse();
			break;
		}		
		this->stk_station_name->Push(&mem);
	}

	this->LocateSpLine(file,"[disp_name]");
	while(file->ReadLine(&mem))
	{
		this->MsgLoop();
		if(mem.p[0] == '[')
		{
			file->ReadLine_Reverse();
			break;
		}
		this->stk_disp->Push(&mem);
	}

	this->LocateSpLine(file,"[all_edge]");
	while(file->ReadLine(&mem))
	{
		this->MsgLoop();
		if(mem.p[0] == 0)
			continue;
		this->AddBus(&mem);
	}

	return OK;
}

int CBusMap::LoadMapFile(char *fn)
{
	CMem mf;

	mf.Init();
	ASSERT( mf.LoadFile(fn) );

	return this->LoadMapFile(&mf);
}

int CBusMap::LocateSpLine(CFileBase *file, char *line)
{
	ASSERT(file && line);

	CMem mem;

	LOCAL_MEM(mem);

	while(file->ReadLine(&mem))
	{
		if(mem.StrCmp(line) == 0)
			break;
	}
	
	return OK;
}

int CBusMap::AddBus(CMem *mem)
{
	ASSERT(mem);

	CIntStk *pstk,*ptop;
	CMem buf;
	int i, t = 0;

	LOCAL_MEM(buf);
	
	mem->SetSplitChars(" ,:\t");
	mem->Seek(0);
	
	while(mem->ReadString(&buf))
		t ++;
	
	if(t > 2)
	{
		NEW(pstk,CIntStk);
		pstk->Init(t);

		mem->Seek(0);
		while(mem->ReadString(&buf))
		{
			pstk->Push(atoi(buf.p));
		}

		this->AddBus(pstk);
	}
	else
	{
		//环路车，取上一个路线，然后反向
		ptop = (CIntStk*)this->stk_all_bus->GetTop();
		ASSERT(ptop);
		
		NEW(pstk,CIntStk);
		pstk->Init(ptop->GetLen());
		
		mem->Seek(0);
		mem->ReadString(&buf); 
		pstk->Push(atoi(buf.p));
		mem->ReadString(&buf); 
		pstk->Push(atoi(buf.p));

		for(i = ptop->GetLen() - 1; i >= 2; i--)
		{
			pstk->Push(ptop->GetElem(i));
		}

		this->AddBus(pstk);
	}
	return OK;
}

CIntStk * CBusMap::GetBus(int bus_id)
{
	if(bus_id >= 0 && bus_id < this->stk_all_bus->GetLen()) 
		return (CIntStk*)this->stk_all_bus->GetElem(bus_id);
	return NULL;
}

int CBusMap::PrintBus(int bus_id,CFileBase *file)
{
	CIntStk *pstk;
	CMem *pmem;

	pstk = this->GetBus(bus_id);
	ASSERT(pstk);
	int k,dir;

	dir = this->GetBusDir(bus_id);

	pmem = this->stk_bus_name->GetElem(pstk->GetElem(0));
	ASSERT(pmem);
	file->Puts("【");
	file->Puts(pmem->p);
	if(dir == BUS_DIR_UP)
		file->Puts("(上行)");
	if(dir == BUS_DIR_DOWN)
		file->Puts("(下行)");
	file->Puts("】\r\n");

	pmem = this->stk_disp->GetElem(pstk->GetElem(1));
	ASSERT(pmem);
	file->Puts("〖");
	file->Puts(pmem->p);
	file->Puts("〗\r\n");
	
	for(k = 2; k < pstk->GetLen(); k++)
	{
		pmem = this->stk_station_name->GetElem(pstk->GetElem(k));
		ASSERT(pmem);		
		if(k > 2)
			file->Puts(" - ");
		file->Puts(pmem->p);		
	}

	file->Puts("\r\n");

	return OK;
}

int CBusMap::GetBusCount()
{
	return this->stk_all_bus->GetLen();
}

int CBusMap::IsBusPassStation(int bus_id, int station_id)
{
	int i;
	CIntStk *pstk;

	pstk = this->GetBus(bus_id);
	ASSERT(pstk);

	for(i = 2; i < pstk->GetLen(); i++)
	{
		if(pstk->GetElem(i) == station_id)
			return i;
	}

	return 0;
}
int CBusMap::GetBusByName(CMem *bus_name)
{
	ASSERT(bus_name);
	int i,pos,ff;
	CIntStk *pbus;

	pos = this->stk_bus_name->BSearch_Pos(bus_name,0,&ff);

	for(i = 0; i < this->GetBusCount(); i++)
	{
		pbus = this->GetBus(i);
		ASSERT(pbus);

		if(this->GetBusNameId(i) == pos)
			return i;
	}
	
	return -1;
}
int CBusMap::GetBusByName(char *bus_name)
{
	ASSERT(bus_name);

	CMem mem;

	mem.Init();
	mem.SetP(bus_name,strlen(bus_name)+1);

	return this->GetBusByName(&mem);
}
int CBusMap::GetStationByName(CMem *station_name)
{
	ASSERT(station_name);
	int ff;
	return this->stk_station_name->BSearch_Pos(station_name,0,&ff);
}

int CBusMap::GetStationByName(char *station_name)
{
	ASSERT(station_name);

	CMem mem;

	mem.Init();
	mem.SetP(station_name,strlen(station_name)+1);

	return this->GetStationByName(&mem);
}

int CBusMap::GetAllBusAtStation(int station_id, CIntStk *stk)
{
	ASSERT(stk);
	
	int i;

	stk->Clear();
	for(i = 0; i < this->GetBusCount(); i++)
	{
		if(this->IsBusPassStation(i,station_id))
			stk->Push(i);
	}

	return OK;
}

//start_bus_id用于二次换乘查找中的开始bus_id,因为换乘bus不能和第一个bus相同
int CBusMap::GetAllZeroRelayPath(int start_bus_id1,int start_bus_id2,int start_station_id,int end_station_id,CFileBase *file_log,int station_count)
{
	int i,j,pos,start_size,ret = ERROR;
	int count;
	CIntStk *pbus;

	start_size = file_log->GetSize();

	for(i = 0; i < this->GetBusCount() && !this->end_flag; i++)
	{
		if(start_bus_id1 >= 0)
		{
			if(this->GetBusNameId(i) == this->GetBusNameId(start_bus_id1))
				continue;
		}
		if(start_bus_id2 >= 0)
		{
			if(this->GetBusNameId(i) == this->GetBusNameId(start_bus_id2))
				continue;
		}
		pos = this->IsBusPassStation(i,start_station_id);
		if(pos == 0)
			continue;
		
		pbus = this->GetBus(i);
		ASSERT(pbus);
		file_log->SetSize(start_size);
		if(start_bus_id1 < 0 && start_bus_id2 < 0)
		{
			file_log->Puts("在 [");
			file_log->Puts(this->GetStationName(start_station_id));
			file_log->Puts("] 乘坐 \"");
			file_log->Puts(this->GetBusName(i));
			file_log->Puts("\"");			
		}
		else
		{
			file_log->Puts(" 到 [");
			file_log->Puts(this->GetStationName(start_station_id));
			file_log->Puts("] 然后换乘 \"");
			file_log->Puts(this->GetBusName(i));
			file_log->Puts("\"");					
		}
		count = station_count;
		for(j = pos + 1; j < pbus->GetLen() && !this->end_flag; j++)
		{
			count++;
			//file_log->Printf(" 到 [%s]",this->GetStationName(pbus->GetElem(j)));
			
			if(this->call_back_search)
				this->call_back_search(this->param_call_back_search);

			if(pbus->GetElem(j) == end_station_id)
			{
				//file_log->Printf(" 到 [%s].",GetStationName(pbus->GetElem(j)));
				file_log->Puts(" 到 [");
				file_log->Puts(GetStationName(pbus->GetElem(j)));
				file_log->Puts("].");

				if(this->call_back_find)
					this->call_back_find(this->param_call_back,file_log,count);

				ret = OK;
			}
		}
	}

	file_log->SetSize(start_size);
	return ret;
}

char * CBusMap::GetBusName(int bus_id)
{
	CMem *pmem;

	pmem = this->stk_bus_name->GetElem(this->GetBusNameId(bus_id));
	ASSERT(pmem);

	return pmem->p;
}

char * CBusMap::GetStationName(int station_id)
{
	CMem *pmem;
	
	pmem = this->stk_station_name->GetElem(station_id);
	ASSERT(pmem);

	return pmem->p;
}

int CBusMap::GetAllOneRelayPath(int start_bus_id,int start_station_id,int end_station_id,CFileBase *file_log,int station_count)
{
	int i,j,pos,start_size,ret = ERROR;
	int count;
	CIntStk *pbus;

	start_size = file_log->GetSize();

	for(i = 0; i < this->GetBusCount() && !this->end_flag; i++)
	{
		if(start_bus_id >= 0)
		{
			if(this->GetBusNameId(i) == this->GetBusNameId(start_bus_id))
				continue;
		}

		pos = this->IsBusPassStation(i,start_station_id);
		if(pos == 0)
			continue;
		
		pbus = this->GetBus(i);
		ASSERT(pbus);

		file_log->SetSize(start_size);

		if(start_bus_id < 0)
		{
			file_log->Puts("在 [");
			file_log->Puts(this->GetStationName(start_station_id));
			file_log->Puts("] 乘坐 \"");
			file_log->Puts(this->GetBusName(i));
			file_log->Puts("\"");			
		}
		else
		{
			file_log->Puts(" 到 [");
			file_log->Puts(this->GetStationName(start_station_id));
			file_log->Puts("] 然后换乘 \"");
			file_log->Puts(this->GetBusName(i));
			file_log->Puts("\"");			
		}
		count = station_count;
		for(j = pos + 1; j < pbus->GetLen() && !this->end_flag; j++)
		{
			count++;
			//file_log->Printf(" 到 [%s]",this->GetStationName(pbus->GetElem(j)));
			if(this->GetAllZeroRelayPath(start_bus_id,i,pbus->GetElem(j),end_station_id,file_log,count))
			{
				ret = OK;
			}
		}
	}
	
	file_log->SetSize(start_size);

	return ret;
}
int CBusMap::GetAllTwoRelayPath(int start_station_id,int end_station_id,CFileBase *file_log,int station_count)
{
	int i,j,pos,start_size,ret = ERROR;
	int count;
	CIntStk *pbus;

	start_size = file_log->GetSize();

	for(i = 0; i < this->GetBusCount() && !this->end_flag; i++)
	{
		pos = this->IsBusPassStation(i,start_station_id);
		if(pos == 0)
			continue;
		
		pbus = this->GetBus(i);
		ASSERT(pbus);

		file_log->SetSize(start_size);		

		file_log->Puts("在 [");
		file_log->Puts(this->GetStationName(start_station_id));
		file_log->Puts("] 乘坐 \"");
		file_log->Puts(this->GetBusName(i));
		file_log->Puts("\"");		

		count = station_count;
		for(j = pos + 1; j < pbus->GetLen() && !this->end_flag; j++)
		{
			count ++;
			//file_log->Printf(" 到 [%s]",this->GetStationName(pbus->GetElem(j)));
			if(this->GetAllOneRelayPath(i,pbus->GetElem(j),end_station_id,file_log,count))
			{
				ret = OK;
			}
		}
	}
	
	file_log->SetSize(start_size);

	return ret;
}
int CBusMap::GetBusNameId(int bus_id)
{
	CIntStk *pstk;

	pstk = this->GetBus(bus_id);
	if(pstk == NULL)
		return -1;

	return pstk->GetElem(0);
}

int CBusMap::call_back_default(void *p, CFileBase *file_log,int total_count)
{
	ASSERT(file_log && p);

	CBusMap *self = (CBusMap*)p;

	self->AddResult(total_count,file_log);
	
	return OK;
}

int CBusMap::FindPath(char *station_start, char *station_end)
{
	ASSERT( station_start && station_end);

	CMem mem;
	int s1,s2;
	
	LOCAL_MEM(mem);

	s1 = this->GetStationByName(station_start);
	s2 = this->GetStationByName(station_end);

	ASSERT(s1 >=0 && s2 >= 0);

	if(s1 == s2)
		return OK;

	this->end_flag = 0;

	this->ClearResult();

	if(this->GetAllZeroRelayPath(-1,-1,s1,s2,&mem,0))
		return OK;

	if(this->GetAllOneRelayPath(-1,s1,s2,&mem,0))
		return OK;
	
	if(this->GetAllTwoRelayPath(s1,s2,&mem,0))
		return OK;

	return ERROR;
}

int CBusMap::ClearResult()
{
	this->stk_result_path->Clear();
	this->stk_result_times->Clear();

	return OK;
}

int CBusMap::AddResult(int hc_times, CFileBase *file_path)
{
	ASSERT(file_path);

	CMem *pmem;

	this->stk_result_times->Push(hc_times);
	pmem = this->stk_result_path->PushEmpty();
	
	pmem->Malloc(file_path->GetSize() + 10);
	pmem->WriteFile(file_path);
	pmem->Putc(0);

	return OK;
}

int CBusMap::SortResult()
{
	int i,j,t1,t2;
	CMem *pmem1,*pmem2;

	for(j=this->stk_result_times->GetLen(); j > 0; j--)
    {
        for(i = 0; i < j - 1; i++)
        {
            if(this->stk_result_times->GetElem(i) > this->stk_result_times->GetElem(i+1))
            {
                t1 = this->stk_result_times->GetElem(i);
                t2 = this->stk_result_times->GetElem(i + 1);
                this->stk_result_times->SetElem(i,t2);
				this->stk_result_times->SetElem(i+1,t1);

				pmem1 = this->stk_result_path->GetElem(i);
				pmem2 = this->stk_result_path->GetElem(i+1);
				this->stk_result_path->index[i] = pmem2;
				this->stk_result_path->index[i+1] = pmem1;
            }
        }
    }
	
	return OK;
	
}

int CBusMap::PrintResult(CFileBase *file)
{
	int i;
	CMem *pmem;

	for(i = 0; i < this->stk_result_times->GetLen(); i++)
	{
		file->Puts("------共 "); 
		file->Printf("%d",this->stk_result_times->GetElem(i));
		file->Puts(" 站------\n");
		pmem = this->stk_result_path->GetElem(i);
		ASSERT(pmem);
		file->Puts(pmem->p);
		file->Putc('\n');
	}

	return OK;
}

int CBusMap::MsgLoop()
{
#ifdef _NDS_
	::MsgLoop();
#endif
	return OK;
}

int CBusMap::PrintAllBusAtStation(int station_id, CFileBase *file)
{
	ASSERT(file);
	int i;

	file->Puts("经过\"");
	file->Puts(this->GetStationName(station_id));
	file->Puts("\"的线路有:\n");
	for(i = 0; i < this->GetBusCount(); i++)
	{
		if(this->IsBusPassStation(i,station_id))
		{
			file->Puts(this->GetBusName(i));
			if(this->GetBusDir(i) == BUS_DIR_UP)
				file->Puts("(上行)");
			if(this->GetBusDir(i) == BUS_DIR_DOWN)
				file->Puts("(下行)");
			file->Puts("\n");
		}
	}

	return OK;
}

int CBusMap::GetBusDir(int bus_id)
{
	int pre_id,cur_id,next_id;

	cur_id = this->GetBusNameId(bus_id);
	pre_id = this->GetBusNameId(bus_id - 1);
	next_id = this->GetBusNameId(bus_id + 1);

	if(cur_id == pre_id)
		return BUS_DIR_DOWN;
	if(cur_id == next_id)
		return BUS_DIR_UP;

	return BUS_DIR_ERR;
}

int CBusMap::StopSearchPath()
{
	this->end_flag = true;
	return OK;
}
