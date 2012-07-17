#include "stdafx.h"
#include "mem_tool.h"

static FILE *_fp_leak_detect=NULL;

int Mem_Tool_Init(void)
{
#ifdef _DEBUG_
	_fp_leak_detect = fopen("mem_leak.txt","wb+");
	if(_fp_leak_detect == NULL)
		return ERROR;
#endif
	return OK;
}

int Mem_Tool_Destroy(void)
{
#ifdef _DEBUG_
	if(_fp_leak_detect)
	{
		fclose(_fp_leak_detect);
		_fp_leak_detect = NULL;
	}
#endif	
	return OK;
}

int _leak_printf(const char *szFormat, ...)
{
    char szBuffer [2048] ;

	if(_fp_leak_detect == NULL)
		return ERROR;

    va_list pArgList ;
    va_start (pArgList, szFormat) ;
    vsprintf (szBuffer, szFormat, pArgList) ;
    va_end (pArgList) ;
    
	fputs(szBuffer,_fp_leak_detect);
	fputs("\r\n",_fp_leak_detect);

    return OK;  
}
