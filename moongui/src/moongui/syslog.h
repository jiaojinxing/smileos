#ifndef __SYSLOG_H
#define __SYSLOG_H

#include "common.h"

class CLog{
public:
	void *ptr;
	int (*puts)(char *str);
	int log_open_flag;
public:
	int Puts(char *str);
    CLog();
    ~CLog();
    int Init();
    int Destroy();
	static int DefPuts(char *str);
	int printf(const char *szFormat, ...);
};

extern CLog syslog;

#define LOG syslog.printf
#define LOGS if(syslog.log_open_flag)LOG

#define PD(p) LOG("%s = %d\n",#p,p)
#define PX(p) LOG("%s = %x\n",#p,p)
#define PS(p) LOG("%s = %s\n",#p,p)
#define PF(p) LOG("%s = %f\n",#p,p)

#define PQ(p) {QWORD *_v_ = (QWORD*)&(p);LOG("%s = ",#p);LOG("%08x%08x\n",*((int*)_v_ + 1),(int)(*_v_));}

#endif

