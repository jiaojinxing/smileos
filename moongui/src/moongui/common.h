#ifndef __COMMON_H
#define __COMMON_H

/************************************** jiaojinxing1987@gmail.com ***************************************/
#define _LINUX_     0
#define _WIN32_     0
#define _NDS_       0
#define _WINCE_     0
#define _SMILEOS_   1
/************************************************* end **************************************************/

#define _ASC_		1
#define _UNICODE_	0
#define _DEBUG_		1

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define TRUE  		1
#define FALSE		0
#define OK  		1
#define ERROR 		0

#define S_RET(p) if( !(p) ) return ERROR

////////////////////////
// for windows
#if _WIN32_
#include <windows.h>
#include <conio.h>

typedef unsigned __int64 QWORD;

#define LBUF_SIZE			2048
#define FILEBASE_LBUF_SIZE  (32*1024)

#endif

////////////////////////
// for windows CE
#if _WINCE_
#include <windows.h>

typedef unsigned __int64 QWORD;

#define LBUF_SIZE			2048
#define FILEBASE_LBUF_SIZE  (4*1024)

#endif

////////////////////////
// for linux
#if _LINUX_
	
#include <ctype.h>
#include <unistd.h>

typedef unsigned char 		BYTE;
typedef unsigned short int 	WORD;
typedef unsigned long		DWORD;
typedef unsigned char 		BOOL;
typedef unsigned long long	QWORD;

#define stricmp 			strcasecmp
#define _sleep(x)			usleep(1000 *(x))

#define LBUF_SIZE			2048
#define FILEBASE_LBUF_SIZE  (32*1024)

#endif
////////////////////////
// for nintendo NDS
#if _NDS_

#include <nds.h>
#include <ctype.h>

typedef unsigned char 		BYTE;
typedef unsigned short int 	WORD;
typedef unsigned long		DWORD;
typedef unsigned char 		BOOL;
typedef unsigned long long	QWORD;	

#define FILEBASE_LBUF_SIZE  512
#define LBUF_SIZE			1024

#endif

/************************************** jiaojinxing1987@gmail.com ***************************************/
////////////////////////
// for SmileOS
#if _SMILEOS_

#include <ctype.h>
#include <unistd.h>

typedef unsigned char       BYTE;
typedef unsigned short int  WORD;
typedef unsigned long       DWORD;
typedef unsigned char       BOOL;
typedef unsigned long long  QWORD;

typedef unsigned char       u8;
typedef unsigned short      u16;
typedef unsigned int        u32;
typedef          int        s32;

#define stricmp             strcasecmp
#define _sleep(x)           usleep(1000 *(x))

#define LBUF_SIZE           2048
#define FILEBASE_LBUF_SIZE  (32*1024)

#endif
/************************************************* end **************************************************/

#endif
