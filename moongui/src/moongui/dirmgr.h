// DirMgr.h: interface for the CDirMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIRMGR_H__0939C948_943D_41B6_9FC1_6CE47685F2E3__INCLUDED_)
#define AFX_DIRMGR_H__0939C948_943D_41B6_9FC1_6CE47685F2E3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "common.h"
#include "filebase.h"
#include "mem.h"

#if _WIN32_
#include <time.h>
#endif

/************************************** jiaojinxing1987@gmail.com ***************************************/
#if _LINUX_ || _SMILEOS_
/************************************************* end **************************************************/
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#define MAX_PATH 1024
#endif

#if _NDS_
#include <sys/dir.h>
#include "memstk.h"
#define MAX_PATH 1024
#endif

#define FN_SINGLE	0x00000001
#define FN_EXT		0x00000002
#define FN_PATH		0x00000004

#define FN_FILENAME	(FN_SINGLE|FN_EXT)
#define FN_FULLNAME	(FN_PATH|FN_FILENAME)

class CDirMgr{
public:
	void *_ptr;
	int (*search_dir_call_back)(void *p,char *filename,int isdir);

/************************************** jiaojinxing1987@gmail.com ***************************************/
#if _LINUX_ || _SMILEOS_
/************************************************* end **************************************************/
	DIR * handle;
	struct dirent *entry;
	struct stat statbuf;
#endif
	
#if _WIN32_
	HANDLE handle;
	WIN32_FIND_DATA filestruct;
#endif

#if _NDS_
	CMemStk *stk_path;
	DIR_ITER * handle;
	//Even though sizeof(stat) == 64, 68 bytes get overwritten when you call dirnext!
	//This means if you have allocated stat on the stack, the next variable gets set to zero after you call dirnext.  This can cause all sorts of malfunctions.
	char statbuf[100];  
	CMem *mem_filename;
#endif

public:
	static int GetFileTime(char *fn,long *create_time,long *access_time,long *write_time);	
	static int search_call_back_file(void *p,char *fn,int isdir);
	static int GetFileName(char *line,CFileBase *file,DWORD filter);

#if _WIN32_
	static int GetCurDir(CMem *dir_str);
	static int GetCurDir(char *dir);
	static int ChDir(char *dir_name);
#endif

/************************************** jiaojinxing1987@gmail.com ***************************************/
#if _LINUX_ || _SMILEOS_
/************************************************* end **************************************************/
	static int GetCurDir(CMem *dir_str);
	static int GetCurDir(char *dir);
	static int ChDir(char *dir_name);
#endif

#if _NDS_
	int GetCurDir(CMem *dir_str);
	int GetCurDir(char *dir);
	int ChDir(char *dir_name);
#endif

	int GetCurFileName(CMem *mem);
	int SearchDir(int recursice,CFileBase *file);
	int GetCurFileName(char *fn);
	int IsDir();
	int SearchDir(int recursice);
	int ReadNextDir();
	int CloseDir();
	int OpenDir();
		CDirMgr();
    virtual ~CDirMgr();
    int Init();
    int Destroy();
};

#endif // !defined(AFX_DIRMGR_H__0939C948_943D_41B6_9FC1_6CE47685F2E3__INCLUDED_)