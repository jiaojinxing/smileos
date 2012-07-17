#include "stdafx.h"
#include "syslog.h"

CLog syslog;

CLog::CLog()
{
	this->ptr = NULL;
	this->puts = DefPuts;
	this->log_open_flag = 0;
}
CLog::~CLog()
{
    Destroy();
}
int CLog::Init()
{
	this->puts = DefPuts;
    return OK;
}
int  CLog::Destroy()
{
    return OK;
}
int CLog::DefPuts(char *str)
{
	if(str == NULL)
		return ERROR;
	while(*str)
	{
		putchar(*str);
		str++;
	}
	return OK;
}

int CLog::printf(const char *szFormat, ...)
{
    char szBuffer [2048] ;

	if(this->puts == NULL)
		return ERROR;

    va_list pArgList ;
    va_start (pArgList, szFormat) ;
    vsprintf (szBuffer, szFormat, pArgList) ;
    va_end (pArgList) ;
    
	syslog.puts(szBuffer);

    return OK;  
}

int CLog::Puts(char *str)
{
	if(this->puts == NULL)
		return ERROR;

	this->puts(str);

	return OK;
}

