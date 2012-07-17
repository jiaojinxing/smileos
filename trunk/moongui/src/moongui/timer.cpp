#include "timer.h"
#include "globals.h"

CTimer::CTimer()
{
    this->InitBasic();
}
CTimer::~CTimer()
{
    this->Destroy();
}
int CTimer::InitBasic()
{
	this->time_out = 1000;
	this->id = 0;
	this->old_time = 0;
	this->win_id = 0;
	this->data = 0;
	this->enabled = 0;

    return OK;
}
int CTimer::Init()
{
    this->InitBasic();
    this->id = afx.GetUniqueID();
    return OK;
}
int CTimer::Destroy()
{
    //add your code
    this->InitBasic();
    return OK;
}
int CTimer::Copy(CTimer *p)
{
    if(this == p)
        return OK;
	
	this->old_time = p->old_time;
	this->time_out = p->time_out;	
	this->win_id = p->win_id;

    return OK;
}
int CTimer::Comp(CTimer *p)
{
    return 0;
}
int CTimer::Print()
{
	LOG("id = %d\n",id);
	LOG("time_out=%d\n",this->time_out);
    return TRUE;
}
int CTimer::Start()
{
	this->enabled = TRUE;
	return OK;
}

int CTimer::Stop()
{
	this->enabled = FALSE;
	return OK;
}

int CTimer::SetAddData(int data)
{
	this->data = data;
	return OK;
}

int CTimer::SetTimeOut(int ms)
{
	if(ms <= 0)
		return ERROR;
	this->time_out = ms;
	return OK;
}

int CTimer::IsEnable()
{
	return this->enabled;
}
