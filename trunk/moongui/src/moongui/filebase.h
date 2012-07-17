// FileBase.h: interface for the CFileBase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILEBASE_H__BD546014_EE30_43B6_90BB_2EE15AD02697__INCLUDED_)
#define AFX_FILEBASE_H__BD546014_EE30_43B6_90BB_2EE15AD02697__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "common.h"
#include "assert.h"

class CFileBase
{
#if _UNICODE_
public:
	WORD *sp_chars_w;
public:
	int ReplaceStrW(WORD *src_str, WORD *des_str, int case_sensive, int word_only,CFileBase *new_file);
	int SearchStrW(WORD *str, int case_sensive,int word_only);
	int ReadStringW_Reverse(CFileBase *file);
	int ReadWordW_Reverse(CFileBase *file);
	int ReadStringW(CFileBase *file);
	int ReadWordW(CFileBase *file);
	int SetSplitCharsW(WORD *sp_str);
	int SkipEmptyCharsW_Reverse();
	int SkipEmptyCharsW();
	int ReverseW(long start, long end);
	int ReadWordWithEmptyCharW_Reverse(CFileBase *file);
	int IsSpCharW(WORD ch);
	int ReadWordWithEmptyCharW(CFileBase *file);
	int ReadLine_ReverseW(CFileBase *file);
	int ReadLine_ReverseW();
	int ReadLineW(CFileBase *file);
	int UnPutcW();
	WORD UnGetcW();
	int SetDefaultSpCharsW();
	int IsEmptyCharW(WORD uc);
	int PrintfW(WORD *szFormat, ...);
	int PutsW(WORD *str);
	int StrCatW(CFileBase *file);
	int StrCatW(WORD *str);
	int StrLwrW();
	int StrCmpW(CFileBase *file);
	int StrICmpW(CFileBase *file);
	int StrCpyW(CFileBase *file);
	int StrCpyW(WORD *str);
	int StrICmpW(WORD *str);
	WORD ToLowerW(WORD uc);
	int StrLenW(WORD *str);
	int StrLenW();
	int SetCharW(int index,WORD uc);
	WORD CharAtW(int index);
	int PutcW(WORD uc);
	WORD GetcW();
#endif

#if _ASC_
public:
	char *file_name;
	char *sp_chars;
	char *except_sp_chars;
public:
	int AdjustGbPos(int char_pos);
	int Puts(CFileBase *file_str);
	int ReadLeftStr(CFileBase *file,int skip_empty);
	int StdStr();
	int StartWith(char *str,int case_sensive,int skip_empty);
	int StartWith(CFileBase *file_str,int case_sensive,int skip_empty);
	int StrCpy(char *str);
	int StrCpy(CFileBase *file);
	int StrICmp(char *str);
	int StrICmp(CFileBase *file);
	int StrCmp(CFileBase *file);
	int StrCat(CFileBase *file);
	int StrCmp(char *str);
	int StrCat(char *str);
	int StrLen();
	int SearchWordsList(char *words_list,int case_sensive);
	static int InStrList(char *str, char *str_list, int case_sensive);
	int ReadQuoteStr(char escape_char,char end_char,CFileBase *file);
	int Getc_GB(char *letter);
	int ReadCStr(CFileBase *file);
	int ReplaceStr(char *src_str,char *des_str,int case_sensive,int word_only,CFileBase *new_file);
	int SearchStr(char *str,int case_sensive,int word_only);
	int SetFileName(char *fn);
	char GetLastChar(int empty_included);
	int ReadString_Reverse(CFileBase *file);
	int Reverse(long start,long end);
	int SetChar(long index,char ch);
	char CharAt(long index);
	int ReadWord_Reverse(CFileBase *file);
	int ReadWordWithEmptyChar_Reverse(CFileBase *file);
	int SkipEmptyChars_Reverse();
	int SetSplitChars(char *sp_str);
	static int IsEmptyChar(char ch);
	char UnGetc();
	int Dump();
	int IsSpChar(char ch);
	int Puts(char *str);
	int ReadLine(CFileBase *file);
	int ReadString(CFileBase *file);
	int ReadWord(CFileBase *file);
	int ReadWordWithEmptyChar(CFileBase *file);
	int SetDefaultSpChars();
	int SkipEmptyChars();
	int UnPutc();
	int ReadLine_Reverse();
	int ReadLine_Reverse(CFileBase *file);
	int Printf(char *szFormat, ...);
#endif
	//common part
	char Getc();
	int Putc(char ch);
	int SearchBin(CFileBase *file_bin);
	long WriteFile(CFileBase *file, long start, long ws,void *buf,int buf_size);
	long WriteToFile(CFileBase *file,long start,long wsize,void *buf,int buf_size);
	int WriteToFile(CFileBase *file);
	int WriteToFile(char *fn);
	int WriteToFile(char *fn,long start,long wsize);
	int SeekEnd();
	int AdjustOffset();
	int FillBlock(long fill_size,char fill_ch);
	int Destroy();
	int Init();
	int ExpandFromOff(long exp_size);
	int LoadFile(char *fn);
	int SeekBack(long back_bytes);
	int Insert(CFileBase *file);
	int Delete(long del_size);
	int IsEnd();
	long WriteFile(CFileBase *file);
	long WriteFile(CFileBase *file, long start, long ws);
	long WriteToFile(CFileBase *file,long wsize);
	long WriteToFile(CFileBase *file,long start,long wsize);
	CFileBase();
	virtual ~CFileBase();
	///////need implement by child class////////
	virtual long GetOffset() = 0;
	virtual long GetSize() = 0;
	virtual long Read(void *buf,long n) = 0;
	virtual long Write(void *buf,long n) = 0;
	virtual int SetSize(long ssize) = 0;
	virtual int Seek(long off) = 0;
	virtual int AddBlock(long bsize) = 0;
	virtual long GetMaxSize() = 0;
};

#endif // !defined(AFX_FILEBASE_H__BD546014_EE30_43B6_90BB_2EE15AD02697__INCLUDED_)
