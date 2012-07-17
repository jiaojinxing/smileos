// DirMgr.cpp: implementation of the CDirMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "dirmgr.h"
#include "assert.h"
#include "syslog.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////
/////////Win32 Implemetion
#if _WIN32_
#define SEPARATOR '\\'

CDirMgr::CDirMgr()
{
	this->_ptr = NULL;
	this->search_dir_call_back = NULL;

	this->handle = NULL;
	memset((void*)&this->filestruct , 0 , sizeof(WIN32_FIND_DATA));
}

CDirMgr::~CDirMgr()
{
    this->Destroy();
}
int CDirMgr::Init()
{
	this->_ptr = NULL;
	this->search_dir_call_back = NULL;

	this->handle = NULL;
	memset((void*)&this->filestruct , 0 , sizeof(WIN32_FIND_DATA));

    return OK;
}
int  CDirMgr::Destroy()
{
	this->CloseDir();
    return OK;
}

int CDirMgr::OpenDir( )
{

	this->handle = FindFirstFile("*",&this->filestruct); 
	
	if(this->handle == INVALID_HANDLE_VALUE)
		return ERROR;

	return OK;
}

int CDirMgr::CloseDir()
{
	if(this->handle)		
	{
		FindClose(handle);
		this->handle = NULL;
	}
	return OK;
}

int CDirMgr::ReadNextDir()
{
	return FindNextFile(handle,&this->filestruct);	
}

int CDirMgr::ChDir(char *dir_name)
{
	return SetCurrentDirectory(dir_name);
}

int CDirMgr::GetCurDir(CMem *dir_name)
{
	int l;
	
	ASSERT(dir_name);

	dir_name->SetSize(0);

	GetCurrentDirectory(MAX_PATH,dir_name->p);

	l = strlen(dir_name->p);
	if(l <= 0 )  return ERROR;

	dir_name->SetSize(l);

	return OK;
}

int CDirMgr::IsDir()
{
	return GetFileAttributes(filestruct.cFileName) & FILE_ATTRIBUTE_DIRECTORY;
}

int CDirMgr::GetCurFileName(char *fn)
{
	strcpy(fn,this->filestruct.cFileName);

	return OK;
}

int CDirMgr::GetFileTime(char *fn,long *create_time, long *access_time, long *write_time)
{
	FILETIME   createtime,accesstime,writetime;   	
	SYSTEMTIME   screate,saccess,swrite,tloc,tsys;   
	HANDLE   file;
	struct tm local;
	long time_zone;
	
	GetSystemTime(&tsys);
	GetLocalTime(&tloc);

	time_zone = tloc.wHour - tsys.wHour;

	file = CreateFile(fn,GENERIC_READ,0,0,OPEN_EXISTING,0,0);
	if(file == INVALID_HANDLE_VALUE)
		return ERROR;    

	//得到文件的创建   最近访问   和最近修改的时间 
	::GetFileTime(file,&createtime,&accesstime,&writetime);   
	  
	CloseHandle(file);   

	FileTimeToSystemTime(&createtime,&screate);   
	FileTimeToSystemTime(&accesstime,&saccess);   
	FileTimeToSystemTime(&writetime,&swrite);   
    
	local.tm_year = screate.wYear - 1900;
	local.tm_mon = screate.wMonth - 1;
	local.tm_mday = screate.wDay;
	local.tm_hour = screate.wHour + time_zone;
	local.tm_min = screate.wMinute;
	local.tm_sec = screate.wSecond;
	
	if(create_time)
		*create_time =  mktime(&local);

	local.tm_year = saccess.wYear - 1900;
	local.tm_mon = saccess.wMonth - 1;
	local.tm_mday = saccess.wDay;
	local.tm_hour = saccess.wHour + time_zone;
	local.tm_min = saccess.wMinute;
	local.tm_sec = saccess.wSecond;

	if(access_time)
		*access_time =  mktime(&local);

	local.tm_year = swrite.wYear - 1900;
	local.tm_mon = swrite.wMonth - 1;
	local.tm_mday = swrite.wDay;
	local.tm_hour = swrite.wHour + time_zone;
	local.tm_min = swrite.wMinute;
	local.tm_sec = swrite.wSecond;
	
	if(write_time)
		*write_time =  mktime(&local);

	return OK;
}

#endif

/////////////////////////////////////////////////////////////////////////
// linux Implemention
/************************************** jiaojinxing1987@gmail.com ***************************************/
#if _LINUX_ || _SMILEOS_
/************************************************* end **************************************************/
#define SEPARATOR '/'

CDirMgr::CDirMgr()
{
	this->_ptr = NULL;
	this->search_dir_call_back = NULL;
	this->handle = NULL;
	this->entry = NULL;
	memset((void*)(&this->statbuf) , 0 , sizeof(struct stat));
}

CDirMgr::~CDirMgr()
{
	this->Destroy();
}

int CDirMgr::Init()
{
	this->_ptr = NULL;
	this->search_dir_call_back = NULL;
	this->handle = NULL;
	this->entry = NULL;
	memset((void*)(&this->statbuf) , 0 , sizeof(struct stat));

	return OK;
}

int CDirMgr::Destroy()
{
	return this->CloseDir();
}

int CDirMgr::OpenDir()
{
	char path[MAX_PATH];

	this->GetCurDir(path);

	this->handle = opendir(path);

	if(this->handle == NULL)
		return ERROR;
	
	this->entry = readdir(this->handle);
	
	if(this->entry == NULL)
		return ERROR;

/************************************** jiaojinxing1987@gmail.com ***************************************/	
#if _SMILEOS_
    stat(this->entry->d_name, &this->statbuf);
#else
    lstat(this->entry->d_name,&this->statbuf);
#endif
/************************************************* end **************************************************/
	return OK;
}

int CDirMgr::ChDir(char *dir_name)
{
	int ret;

	ret = chdir(dir_name);

	if(ret == 0)
		return OK;
	return ERROR; 
}

int CDirMgr::GetCurDir(CMem *dir_name)
{
	char *ret;
	
	ASSERT(dir_name);

	dir_name->SetSize(0);

	ret = getcwd(dir_name->p,MAX_PATH);

	if(ret == NULL)
		return ERROR;

	int l = strlen(dir_name->p);
	if(l <= 0 )  return ERROR;

	dir_name->SetSize(l);

	return OK;
}

 int CDirMgr::CloseDir()
{
	if(this->handle)
	{
		closedir(this->handle);
		this->handle = NULL;
	}

	return OK;
}

int CDirMgr::GetCurFileName(char *name)
{
	name[0] = 0;
	
	if(this->entry == NULL)
		return ERROR;
	
	strcpy(name,this->entry->d_name);

	return OK;
}

int CDirMgr::IsDir()
{
	return S_ISDIR(this->statbuf.st_mode);
}

int CDirMgr::ReadNextDir()
{
	struct dirent *ret;

	ret = readdir(this->handle);

	if(ret == NULL)
		return ERROR;
	
	this->entry = ret;

/************************************** jiaojinxing1987@gmail.com ***************************************/
#if _SMILEOS_
    stat(this->entry->d_name, &this->statbuf);
#else
    lstat(this->entry->d_name,&this->statbuf);
#endif
/************************************************* end **************************************************/
	return OK;
}
int CDirMgr::GetFileTime(char *fn,long *create_time, long *access_time, long *write_time)
{
	// for future implement
	return OK;
}
#endif
////////////////////////////////////////////////////
///////// NDS Implemetion
#if _NDS_
#define SEPARATOR '/'

CDirMgr::CDirMgr()
{
	this->handle = NULL;
	this->stk_path = NULL;
	this->mem_filename = NULL;
}

CDirMgr::~CDirMgr()
{
	this->Destroy();
}

int CDirMgr::Init()
{
	NEW(this->stk_path,CMemStk);
	this->stk_path->Init(64);

	NEW(this->mem_filename,CMem);
	this->mem_filename->Init();
	this->mem_filename->Malloc(256);
	this->mem_filename->Putc(0);

	this->_ptr = NULL;
	this->search_dir_call_back = NULL;
	this->handle = NULL;

	memset((void*)(&this->statbuf) , 0 , sizeof(struct stat));

	return OK;
}

int CDirMgr::Destroy()
{
	DEL(this->mem_filename);
	DEL(this->stk_path);
	return this->CloseDir();
}

int CDirMgr::OpenDir()
{
	CMem mem;

	LOCAL_MEM(mem);

	this->GetCurDir(&mem);
	
	if(mem.p[0] == 0)
	{
		mem.SetSize(0);
		mem.Putc('/');
		mem.Putc(0);
	}

	this->handle = diropen(mem.p);
	if(this->handle)
		this->ReadNextDir();
	return this->handle != NULL;
}

int CDirMgr::ChDir(char *dir_name)
{
	ASSERT(dir_name);

	CMem mem,buf;

	LOCAL_MEM(buf);
	
	if(dir_name[0] == 0)
		return ERROR;

	mem.Init();
	mem.SetP(dir_name,strlen(dir_name) + 1);
	mem.SetSplitChars("/\\");
	
	if(mem.p[0] == '/')
	{
		this->stk_path->Clear();
	}

	while(mem.ReadString(&buf))
	{
		if(buf.StrICmp("..") == 0)
			this->stk_path->DelTop();
		else		
			this->stk_path->Push(&buf);
	}

	return OK;
}

int CDirMgr::GetCurDir(CMem *dir_name)
{
	ASSERT(dir_name);
	
	int i;
	CMem *pstr;

	dir_name->SetSize(0);
	
	if(this->stk_path->IsEmpty())
		dir_name->Putc('/');

	for(i = 0 ; i < this->stk_path->GetLen() ;i++)
	{
		pstr = this->stk_path->GetElem(i);
		ASSERT(pstr);
		if(pstr->p && pstr->p[0])
		{
			dir_name->Putc('/');
			dir_name->Puts(pstr->p);
		}
	}

	dir_name->Putc(0);
	
	return OK;
}

 int CDirMgr::CloseDir()
{
	if(this->handle)
	{
		dirclose(this->handle);
		this->handle = NULL;
	}

	return OK;
}

int CDirMgr::GetCurFileName(char *name)
{	
	strcpy(name,this->mem_filename->p);
	return OK;
}

int CDirMgr::IsDir()
{
	struct stat *p = (struct stat*)( this->statbuf );
	return S_ISDIR(p->st_mode);
}

int CDirMgr::ReadNextDir()
{
	ASSERT(this->handle);
	
	struct stat *p = (struct stat*)( this->statbuf );
	int ret;
	this->mem_filename->p[0] = 0;
    ret = dirnext(this->handle, this->mem_filename->p, p);	
	return ret == 0;
}
int CDirMgr::GetFileTime(char *fn,long *create_time, long *access_time, long *write_time)
{
	LOG("don't use GetFileTime() in NDS system.\n");
	return OK;
}

#endif
/////////////////////////////////////////////////////////////////////////
// common part
int CDirMgr::search_call_back_file(void *p, char *fn, int isdir)
{
	CFileBase *file;

	file = (CFileBase *) p;

	ASSERT(file);

	if(isdir)
	{
		file->Putc('<');
		file->Puts(fn);
		file->Puts(">\r\n");
	}
	else
	{
		file->Puts(fn);
		file->Puts("\r\n");
	}
	
	return OK;
}

//Must open dir first
int CDirMgr::SearchDir(int recursice,CFileBase *file)
{
	ASSERT(file);

	file->SetSize(0);

	this->_ptr = file;
	this->search_dir_call_back = this->search_call_back_file;

	this->SearchDir(recursice);

	file->Putc(0);

	return OK;
}
//line and file can point to the same memory address
//because temp buffers are used
int CDirMgr::GetFileName(char *line,CFileBase *file, DWORD filter)
{
	int i,j,k,len,dot,slash,dot_pos;
	char ext[MAX_PATH],single[MAX_PATH];
	char path[MAX_PATH];
	char separator = SEPARATOR;

	len = strlen(line);

	file->SetSize(0);

	if(len <= 0 || len > MAX_PATH) 
		goto end;

	dot_pos = len;

	if(filter == FN_FULLNAME)
	{
		file->Puts(line);
		goto end;
	}

	dot = FALSE;
	for(i = len - 1; i >= 0; i--)
	{
		if(line[i] == '.')
		{
			dot = TRUE;
			dot_pos = i;
			break;
		}
		if(line[i] == '/' || line[i] == '\\')
		{
			separator = line[i];
			break;
		}
	}
	
	k = 0;
	if(dot == TRUE)
	{
		for(j = i + 1; j < len; j++)
		{
			ext[k++] = line[j];
		}
	}
	ext[k] = 0;
	////////////////////////////
	slash = FALSE;

	for(i = len - 1; i >= 0; i--)
	{
		if(line[i] == '/' || line[i] == '\\')
		{
			separator = line[i];
			slash = TRUE;
			break;
		}
	}

	k = 0;
	if(slash)
	{
		for(j = 0; j < i; j++)
		{
			path[k++] = line[j];
		}
	}
	path[k] = 0;
	////////////////////////
	k = 0;
	for(j = i + 1; j <dot_pos; j++)
	{
		single[k++] = line[j];
	}
	
	single[k] = 0;
	///////////////////////////////////

	if(filter & FN_PATH)
		file->Puts(path);

	if(filter & FN_SINGLE)
	{
		if(file->GetOffset() > 0)
			file->Putc(separator);
		file->Puts(single);
	}
	if(filter & FN_EXT)
	{
		if(file->GetOffset() > 0 && dot)
		{
			file->Putc('.');			
		}
		file->Puts(ext);
	}
end:
	file->Putc(0);

	return OK;
}

int CDirMgr::SearchDir(int recursice)
{
	static char buf[MAX_PATH + 10];
	static char path[MAX_PATH + 10];
	static char separator = SEPARATOR;
	char __t[2];

	ASSERT(this->_ptr);
	ASSERT(this->search_dir_call_back);

	while(TRUE)
	{
		this->GetCurFileName(buf);		
		if(this->IsDir())
		{		
			if(strcmp(buf,".")!=0 && strcmp(buf,".." ) != 0)
			{				
				this->GetCurDir(path);
			
				__t[0] = separator;
				__t[1] = 0;
				strcat(path,__t);
				strcat(path,buf);
				this->search_dir_call_back(_ptr,path,TRUE);
				///////////////////		
				if(recursice)
				{
					CDirMgr dir_mgr;
					dir_mgr.Init();				

					dir_mgr._ptr = this->_ptr;
					dir_mgr.search_dir_call_back = this->search_dir_call_back;
					if(this->ChDir(buf))
					{	
						dir_mgr.OpenDir();
						dir_mgr.SearchDir(recursice);				
						if(!this->ChDir(".."))
							break;
					}
				}
				////////////////////
			}
		}
		else
		{
			this->GetCurDir(path);
			__t[0] = separator;
			__t[1] = 0;
			strcat(path,__t);
			strcat(path,buf);

			this->search_dir_call_back(this->_ptr,path,FALSE);
		}

		if(this->ReadNextDir() == ERROR)
			break;
	}

	return OK;
}
int CDirMgr::GetCurDir(char *dir)
{
	ASSERT(dir);

	CMem mem;

	mem.Init();
	mem.SetP(dir,MAX_PATH);

	return CDirMgr::GetCurDir(&mem);
}
int CDirMgr::GetCurFileName(CMem *mem)
{
	ASSERT(mem);

	mem->SetSize(0);
	this->GetCurFileName(mem->p);
	mem->SetSize(strlen(mem->p) + 1);

	return OK;
}
