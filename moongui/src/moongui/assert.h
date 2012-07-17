#ifndef __ASSERT_H
#define __ASSERT_H

#include "common.h"
#include "syslog.h"

#ifdef _DEBUG_
#define ASSERT(f) \
if(!(f))\
{\
LOG("assertion fail \"%s\" at file=%s line=%d\r\n",#f,__FILE__,__LINE__);\
return 0;\
}
#else
#define ASSERT(f)     
#endif


#endif
