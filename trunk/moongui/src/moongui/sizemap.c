#include "sizemap.h"

static const int _sys_size[]={
1,1,1,1,1,14,1,1,1,1,
1,15,1,1,2,4,1,10,12,1,
1,1,1,1,1,1,1,1,1,1,
1,1,12,10,13,
};

int GetSysSize(int num)
{
	if(num < 0 || num >= MAX_SYS_METRICS)
		return 0;
	return _sys_size[num];
}

