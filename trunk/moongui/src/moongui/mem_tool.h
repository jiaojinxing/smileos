/*
**************************************
* This is header file Created by
* autocode generator.
* This tools is made by ChenXiangPeng
**************************************
*/

#ifndef __MEM_TOOL_2007_6_27_16_17_29_H
#define __MEM_TOOL_2007_6_27_16_17_29_H

#include "common.h"
#include "assert.h"

int Mem_Tool_Init(void);
int Mem_Tool_Destroy(void);
int _leak_printf(const char *szFormat, ...);

#ifdef _DEBUG_
#define LEAK_DETECT_PRINT _leak_printf
#else
#define LEAK_DETECT_PRINT
#endif

#define NEW(p,obj)  p = new obj;\
{\
ASSERT(p);\
LEAK_DETECT_PRINT("alloc: addr=0x%x file=%s line=%d obj = %s",p,__FILE__,__LINE__,#obj);\
}\

#define NEW_ARRAY(p,obj,items) p = new obj[items];\
{\
ASSERT(p);\
LEAK_DETECT_PRINT("alloc: addr=0x%x file=%s line=%d obj = %s",p,__FILE__,__LINE__,#obj);\
}\

#define DEL(p) if(p)\
{\
delete p;\
LEAK_DETECT_PRINT("free: addr=0x%x file=%s line=%d",p,__FILE__,__LINE__);\
p = NULL;\
}\

#define DEL_ARRAY(p) if(p)\
{\
delete [] p;\
LEAK_DETECT_PRINT("free: addr=0x%x file=%s line=%d",p,__FILE__,__LINE__);\
p = NULL;\
}\

#define MALLOC(p,obj,items) p = (obj*)malloc(sizeof(obj)*items);\
{\
ASSERT(p);\
LEAK_DETECT_PRINT("alloc: addr=0x%x file=%s line=%d obj = %s",p,__FILE__,__LINE__,#obj);\
}\

#define FREE(p) if(p)\
{\
free(p);\
LEAK_DETECT_PRINT("free: addr=0x%x file=%s line=%d",p,__FILE__,__LINE__);\
p = NULL;\
}\

#define REALLOC(p,obj,old_items,items) if(p)\
{\
LEAK_DETECT_PRINT("free: addr=0x%x file=%s line=%d",p,__FILE__,__LINE__);\
p = (obj*)realloc(p,sizeof(obj)*items);\
ASSERT(p);\
LEAK_DETECT_PRINT("alloc: addr=0x%x file=%s line=%d obj = %s",p,__FILE__,__LINE__,#obj);\
}\

#endif
