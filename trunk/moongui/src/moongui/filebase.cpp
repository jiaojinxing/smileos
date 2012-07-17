// FileBase.cpp: implementation of the CFileBase class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "filebase.h"
#include "assert.h"
#include "syslog.h"
#include "mem_tool.h"
#include "mem.h"

///////////////////////////////////////////////////////////////////
//common part
///////////////////////////////////////////////////////////////////
CFileBase::CFileBase()
{
	this->Init();
}

CFileBase::~CFileBase()
{
	this->Destroy();
}
char CFileBase::Getc()
{
	char ch = 0;
	
	this->Read(&ch,1);

	return ch;
}
int CFileBase::Putc(char ch)
{
	return this->Write(&ch,1);
}
int CFileBase::SearchBin(CFileBase *file_bin)
{
	ASSERT(file_bin);
	int old;
	char ch1,ch2;

	file_bin->Seek(0);

	while(!this->IsEnd())
	{
		old = this->GetOffset();
		file_bin->Seek(0);

		while (!this->IsEnd() && !file_bin->IsEnd()) 
		{
			ch2 = file_bin->Getc();
			ch1 = this->Getc();
			if(ch1 != ch2)
			{
				file_bin->Seek(0);
				break;				
			}
		}

		if(file_bin->IsEnd())
		{
			return this->GetOffset() - file_bin->GetSize();
		}

		this->Seek(old + 1);
	}

	return -1;
}

int CFileBase::WriteToFile(char *fn)
{
	return this->WriteToFile(fn,0,this->GetSize());		
}
int CFileBase::WriteToFile(char *fn,long start,long wsize)
{
	char buf[FILEBASE_LBUF_SIZE];
	long bsize = FILEBASE_LBUF_SIZE;
	long sum = 0,rs,need_read;
	FILE *fp;
	fp = fopen(fn , "wb+");
	if(fp == NULL)
		return sum;

	this->Seek(start);

	while(sum < wsize)
	{
		need_read = bsize;

		if(need_read > wsize - sum)
			need_read = wsize - sum;

		rs = this->Read(buf,need_read);
		if(rs <= 0) break;

		fwrite(buf,rs,1,fp);
		sum += rs;
	}
	
	fclose(fp);
	return sum;
}

int CFileBase::WriteToFile(CFileBase *file)
{
	return this->WriteToFile(file,0,this->GetSize());
}
long CFileBase::WriteToFile(CFileBase *file, long start, long wsize, void *buf, int buf_size)
{
	long bsize = buf_size;
	long sum = 0,ws,rs,need_read;

	ASSERT(file);
	
	this->Seek(start);

	while(sum < wsize)
	{
		need_read = bsize;

		if(need_read > wsize - sum)
			need_read = wsize - sum;
		rs = this->Read(buf,need_read);
		if(rs <= 0) break;

		ws = file->Write(buf,rs);
		if(ws <= 0) break;
		sum += ws;
	}
	
	return sum;
}
int CFileBase::FillBlock(long fill_size, char fill_ch)
{
	for(int i = 0; i<fill_size; i++)
	{
		this->Putc(fill_ch);
	}

	return OK;
}

int CFileBase::AdjustOffset()
{
	if(this->GetOffset() >= this->GetSize())
		this->Seek(this->GetSize());

	if(this->GetOffset() < 0)
		this->Seek(0);

	return OK;
}

int CFileBase::SeekEnd()
{
	return this->Seek(-1);
}

int CFileBase::Destroy()
{
	this->except_sp_chars = NULL;
	if(this->file_name)	
		FREE(this->file_name);

	return OK;
}
int CFileBase::SeekBack(long back_bytes)
{
	long off = this->GetOffset();

	off -= back_bytes;

	if(off < 0) off = 0;

	return this->Seek(off);
}
int CFileBase::LoadFile(char *fn)
{
	FILE *fp;
	char buf[FILEBASE_LBUF_SIZE];
	long fs,rs,buf_size = FILEBASE_LBUF_SIZE;
	
	fp = fopen(fn,"rb");
	if(fp == NULL)
	{
		LOG("Open file %s error!",fn);
		return ERROR;
	}

	fseek(fp,0,SEEK_END);
	fs = ftell(fp);
	fseek(fp,0,SEEK_SET);

	this->AddBlock(fs);
	this->Seek(0);

	do{
		rs = fread(buf,1,buf_size,fp);
		if(rs > 0)
		{
			this->Write(buf,rs);
		}
	}while(rs > 0);

	this->Seek(0);

	fclose(fp);
	
	return OK;
}

int CFileBase::ExpandFromOff(long exp_size)
{
	long b_size;
	
	b_size = this->GetSize();

	this->SetSize(this->GetOffset());
	
	this->AddBlock(exp_size);
	
	if(this->GetSize() < b_size)
		this->SetSize(b_size);

	return OK;
}
int CFileBase::Insert(CFileBase *file)
{
	long bsize = FILEBASE_LBUF_SIZE;
	char temp[FILEBASE_LBUF_SIZE];
	long old_size;
	long old_off,off;
	long sum = 0,need_read,rs,ws;

	ASSERT(file);

	old_size = this->GetSize();
	old_off = this->GetOffset();

	this->AddBlock(file->GetSize());
	sum = old_size - old_off;
	this->Seek(old_size);
	off = old_size;
	while(sum > 0)
	{
		need_read = bsize;
		if(need_read > sum)
			need_read = sum;
		off -= need_read;
		this->Seek(off);
		rs = this->Read(temp,need_read);
		if(rs <= 0) break;
		sum -= rs;
		this->Seek(off + file->GetSize());
		ws = this->Write(temp,rs);

		if(ws <= 0) break;
	}
	this->Seek(old_off);
	this->WriteFile(file);

	this->Seek(old_off + file->GetSize());

	return OK;
}

int CFileBase::Delete(long del_size)
{
	long sum,off,bsize=FILEBASE_LBUF_SIZE;
	char buf[FILEBASE_LBUF_SIZE];
	long need_read,rs;
	
	ASSERT(del_size > 0);

	off = this->GetOffset();
	
	if(this->GetSize() - off < del_size)
		del_size = this->GetSize() - off;
	
	if(del_size <= 0) return ERROR;

	sum = this->GetSize() - off - del_size;
	
	while(sum > 0)
	{
		need_read = bsize;

		if(need_read > sum)
			need_read = sum;		

		this->Seek(off + del_size);
		rs = this->Read(buf,need_read);
		if(rs <= 0) break;		
		this->Seek(off);
		this->Write(buf,rs);
		off += rs;
		sum -= rs;
	}	

	this->SetSize(this->GetSize() - del_size);

	return OK;
}


int CFileBase::IsEnd()
{
	if(this->GetOffset() >= this->GetSize())
		return TRUE;
		
	return FALSE;
}

long CFileBase::WriteFile(CFileBase *file, long start, long ws)
{
	char buf[FILEBASE_LBUF_SIZE];	
	return this->WriteFile(file,start,ws,buf,FILEBASE_LBUF_SIZE);
}

long CFileBase::WriteFile(CFileBase *file, long start, long ws, void *buf, int buf_size)
{
	long block_size = buf_size;
	long sum,rs,b_size,left,write_size;
	
	ASSERT(file);

	sum = 0;
	file->Seek(start);
	left = ws;

	while(left > 0)
	{
		b_size = left;
		if(b_size > block_size)
			b_size = block_size;
		rs = file->Read(buf,b_size);
		if(rs > 0)
		{
			write_size = this->Write(buf,rs);
			if(write_size > 0)
				left -= write_size;
			else
				break;
		}
		else
			break;
	}

	return ws - left;	
}

long CFileBase::WriteFile(CFileBase *file)
{
	ASSERT(file);
	return this->WriteFile(file,0,file->GetSize());
}
long CFileBase::WriteToFile(CFileBase *file,long wsize)
{
	return this->WriteToFile(file,0,wsize);
}

long CFileBase::WriteToFile(CFileBase *file,long start,long wsize)
{
	char buf[FILEBASE_LBUF_SIZE];	
	return this->WriteToFile(file,start,wsize,buf,FILEBASE_LBUF_SIZE);
}

//////////////////////////////////////////////////////////////////////
// ASC version
//////////////////////////////////////////////////////////////////////
#if _ASC_

int CFileBase::Printf(char *szFormat, ...)
{
    char szBuffer [FILEBASE_LBUF_SIZE];

    va_list pArgList ;
    va_start (pArgList, szFormat) ;
    vsprintf (szBuffer, szFormat, pArgList) ;
    va_end (pArgList) ;
    
	return this->Puts(szBuffer);
}

int CFileBase::ReadLine_Reverse()
{
	char ch;
	
	ch = this->UnPutc();
	if(ch != '\n')
		this->Putc(ch);

	while(1)
	{
		ch = this->UnPutc();
		if(this->GetOffset() <= 0)
			return OK;
		if(ch == '\n')
		{
			this->Putc(ch);
			break;
		}
	}

	return OK;
}

int CFileBase::ReadLine_Reverse(CFileBase *file)
{
	long off;

	this->ReadLine_Reverse();	
	off = this->GetOffset();
	this->ReadLine(file);
	this->Seek(off);
	
	return OK;
}

int CFileBase::IsSpChar(char ch)
{
	char *p;
	if(ch == 0)
		return TRUE;
	if(this->except_sp_chars != NULL)
	{
		p = this->except_sp_chars;
		while(*p)
		{
			if(*p == ch)
				return FALSE;
			p++;
		}
	}
	p = this->sp_chars;
	while(*p)
	{
		if(*p == ch)
			return TRUE;
		p++;
	}
	return FALSE;
}

int CFileBase::SetDefaultSpChars()
{
	this->sp_chars = " \t\r\n!@#$%^=&*()|+-\\*/{}[];.\":\',?/<>~";
	return OK;
}
//the return value is the number of bytes it reads.
int CFileBase::ReadWordWithEmptyChar(CFileBase *file)
{
	int k = 0;
	char ch;
	
	ASSERT(file);

	file->SetSize(0);

	if(this->IsEnd())
		goto end;

	ch = this->Getc();
	if(ch == 0)
		goto end;
	
	if(ch == '\\')
	{
		file->Putc('\\');
		file->Putc(this->Getc());
		k = 2;
		goto end;
	}
	if(this->IsSpChar(ch))
	{
		file->Putc(ch);
		k = 1;
		goto end;
	}
	while(!this->IsSpChar(ch))
	{
		file->Putc(ch);
		k++;
		if(this->IsEnd())
			break;
		ch = this->Getc();
	}
	if(this->IsSpChar(ch))
		this->UnGetc();
end:		
	file->Putc(0);
	return k;
}
int CFileBase::ReadString(CFileBase *file)
{
	int k;
	k = this->ReadWord(file);
	while(k)
	{
		file->Seek(0);
		if(!this->IsSpChar(file->Getc()))	
		{
			return OK;
		}
		k = this->ReadWord(file);
	}
	return ERROR;
}
int CFileBase::SkipEmptyChars()
{
	char ch;
	while(!this->IsEnd())
	{
		ch = this->Getc();
		if( ! this->IsEmptyChar(ch) )
		{
			this->UnGetc();
			break;
		}
	}
	return OK;
}
int CFileBase::ReadWord(CFileBase *file)
{
	this->SkipEmptyChars();
	return this->ReadWordWithEmptyChar(file);
}

int CFileBase::Puts(char *str)
{
	if(str == NULL)
		return ERROR;

	return this->Write(str,strlen(str));
}

int CFileBase::UnPutc()
{
	char ch=0;
	long off;

	off = this->GetOffset();

	if(off > 0)
	{
		off--;
		this->Seek(off);
		off = this->GetOffset();
		ch =  this->Getc();
		this->Seek(off);
	}

	return ch;
}
char CFileBase::UnGetc()
{
	return this->UnPutc();
}


int CFileBase::Dump()
{
	char buf[1024];
	long rs,i,save_off;
	
	save_off = this->GetOffset();
	this->Seek(0);

	while(1)
	{
		rs = this->Read(buf,1024);
		if(rs <=0 ) break;
		for(i=0;i<rs;i++)
		{
			if(buf[i] == 0)
				LOG(" ");
			else if(buf[i] != 7)
				LOG("%c",buf[i]);
		}
	}
	
	this->Seek(save_off);

	return OK;
}

int CFileBase::ReadLine(CFileBase *file)
{
	char ch;

	if(file == NULL)
		return ERROR;
	if(this->IsEnd())
		return ERROR;

	file->SetSize(0);

	ch = this->Getc();
	while(ch != '\n')
	{
		if(ch!='\r')
		{
			file->Putc(ch);
		}
		
		if(this->IsEnd())
			break;

		ch=this->Getc();  
	}

	file->Putc(0);

	return OK;
}

int CFileBase::IsEmptyChar(char ch)
{
	if(		ch != ' ' 	&&	ch != '\0'
		&&	ch != '\r'	&&	ch != '\n'
		&&	ch != '\t')
		return FALSE;

	return TRUE;
}


int CFileBase::SetSplitChars(char *sp_str)
{
	if(sp_str)
		this->sp_chars = sp_str;
	return OK;
}


int CFileBase::SkipEmptyChars_Reverse()
{
	char ch;
	while(this->GetOffset() > 0)
	{
		ch = this->UnGetc();
		if( ! this->IsEmptyChar(ch) )
		{
			this->Getc();
			break;
		}
	}
	return OK;
}
//the return value is the number of bytes it reads.
int CFileBase::ReadWordWithEmptyChar_Reverse(CFileBase *file)
{
	int k = 0;
	char ch;
	
	ASSERT(file);

	file->SetSize(0);

	if(this->GetOffset() <= 0 )
		goto end;

	ch = this->UnGetc();
	if(ch == 0)
		goto end;
	
	if(ch == '\\')
	{
		file->Putc('\\');
		file->Putc(this->UnGetc());
		k = 2;
		goto end;
	}
	if(this->IsSpChar(ch))
	{
		file->Putc(ch);
		k = 1;
		goto end;
	}
	while(!this->IsSpChar(ch))
	{
		file->Putc(ch);

		if(this->GetOffset() <= 0)
			break;
		k++;

		ch = this->UnGetc();
	}

	if(this->IsSpChar(ch))
		this->Getc();

end:		
	file->Reverse(0,file->GetOffset() - 1);
	file->Putc(0);

	return k;
}

int CFileBase::ReadWord_Reverse(CFileBase *file)
{
	this->SkipEmptyChars_Reverse();
	return this->ReadWordWithEmptyChar_Reverse(file);
}

char CFileBase::CharAt(long index)
{
	long old;
	char ch = 0;

	old = this->GetOffset();

	this->Seek(index);

	ch = this->Getc();

	this->Seek(old);

	return ch;
}

int CFileBase::SetChar(long index,char ch)
{
	long old;

	old = this->GetOffset();

	this->Seek(index);
	this->Putc(ch);

	this->Seek(old);

	return OK;
}
//start 和 end 都从0开始
//所以长度应该是 end - start  + 1
int CFileBase::Reverse(long start, long end)
{
	long i,t;
	char ch;
	
	t = end - start + 1;
	
	for(i = 0; i < t / 2; i++)
	{
		ch = this->CharAt(i + start);
		this->SetChar(i + start, this->CharAt(start + t - i -1 ));
		this->SetChar(start + t - i - 1, ch);
	}

	return OK;
}

int CFileBase::ReadString_Reverse(CFileBase *file)
{
	int k;
	k = this->ReadWord_Reverse(file);
	while(k)
	{
		file->Seek(0);
		if(!this->IsSpChar(file->Getc()))	
		{
			return OK;
		}
		k = this->ReadWord_Reverse(file);
	}
	return ERROR;
}

int CFileBase::Init()
{	
	this->SetDefaultSpChars();
#if _UNICODE_
	this->SetDefaultSpCharsW();
#endif

	this->except_sp_chars = NULL;
	this->file_name = NULL;

	return OK;
}

char CFileBase::GetLastChar(int empty_included)
{
	long old_off;
	char ch;

	old_off = this->GetOffset();
	this->Seek(this->GetSize() - 1);

	ch =  this->Getc();

	if(!empty_included)
	{
		while(CFileBase::IsEmptyChar(ch))
		{
			ch = this->UnGetc();
			if(this->GetOffset() <= 0)
				break;
		}
	}

	this->Seek(old_off);

	return ch;
}

int CFileBase::SetFileName(char *fn)
{
	if(this->file_name)
	{
		FREE(this->file_name);
	}

	MALLOC(this->file_name,char,strlen(fn) + 1);
	strcpy(this->file_name,fn);
	
	return OK;
}

//if the first char is special char, the word_only is ignored
int CFileBase::SearchStr(char *str, int case_sensive,int word_only)
{
	char *s2,ch1,ch2;
	int old,pos,len;
	int flag = 0;
	
	if(str == NULL)
		return -1;

	len = strlen(str);
	
	while(!this->IsEnd())
	{
		old = this->GetOffset();		
		ch1 = this->Getc();
		s2 = str;
		ch2 = *s2;
		if(!case_sensive)
		{
			ch1 = tolower(ch1);
			ch2 = tolower(ch2);
		}
		while (ch2 && !(ch1 - ch2)) 
		{
			s2 ++;
			ch2 = *s2;
			
			if(this->IsEnd())
			{flag = 1;break;}
			
			ch1=this->Getc();
			if(!case_sensive)
			{
				ch1 = tolower(ch1);
				ch2 = tolower(ch2);
			}
		}
		if(!*s2)
		{
			pos = this->GetOffset() - len - 1;
			if(flag) pos++; //if hit end
			
			if(!word_only)
			{
				return pos;
			}
			else if(IsSpChar(CharAt(pos)))
			{
				return pos;
			}
			else
			{
				if((pos == 0||IsSpChar(CharAt(pos - 1)))&&IsSpChar(CharAt(pos + len)))
					return pos;
			}

		}
		this->Seek(old + 1);
	}
	return -1;
}

int CFileBase::ReplaceStr(char *src_str, char *des_str, int case_sensive, int word_only,CFileBase *new_file)
{
	long pos,old_pos,len1;

	ASSERT(new_file);
	ASSERT(new_file != this);

	len1 = strlen(src_str);

	new_file->SetSize(0);
	
	old_pos = this->GetOffset();
	if(old_pos > 0)
	{
		this->WriteToFile(new_file,0,old_pos);
	}
	this->Seek(old_pos);

	while(!this->IsEnd())
	{
		pos = this->SearchStr(src_str,case_sensive,word_only);
		if(pos < 0)
		{
			this->WriteToFile(new_file,old_pos,this->GetSize() - old_pos);
			break;
		}
		else
		{
			this->WriteToFile(new_file,old_pos,pos - old_pos);
			new_file->Puts(des_str);
			old_pos = pos + len1;
			this->Seek(old_pos);
		}
	}
	return OK;
}

int CFileBase::ReadQuoteStr(char escape_char,char end_char,CFileBase *file)
{
	char ch;

	ASSERT(file);

	file->SetSize(0);

	while(!this->IsEnd())
	{
		ch = this->Getc();
		if(ch == 0) break;
		
		if(ch == escape_char) //escape char
		{
			file->Putc(ch);
			ch = this->Getc();
			file->Putc(ch);
		}
		else if(ch == end_char)
		{
			break;
		}
		else
		{
			file->Putc(ch);
		}
	}

	file->Putc(0);

	return OK;
}

int CFileBase::ReadCStr(CFileBase *file)
{
	return this->ReadQuoteStr('\\','\"',file);
}

int CFileBase::Getc_GB(char *letter)
{
	ASSERT(letter);
	
	char ch;
	int ret = 0;

	ch = this->Getc();

	if((BYTE)ch < 128)
	{
		letter[0] = ch;
		letter[1] = 0;
		ret = 1;
	}
	else
	{
		letter[0] = ch;
		letter[1] = this->Getc();
		ret = 2;
	}

	letter[2] = 0;

	return ret;
}

int CFileBase::InStrList(char *str, char *str_list, int case_sensive)
{
	int c;

	CMem mem,buf;
	
	LOCAL_MEM(buf);

	mem.Init();
	mem.SetP(str_list,strlen(str_list));
	mem.Seek(0);
	mem.SetSplitChars(",");
	while(mem.ReadWordWithEmptyChar(&buf))
	{
		if(buf.p[0] == ',')
			continue;
		if(case_sensive)
			c = buf.StrCmp(str) == 0;
		else
			c = buf.StrICmp(str) == 0;
		if(c)
			return TRUE;
	}
	return FALSE;
}
int CFileBase::SearchWordsList(char *words_list,int case_sensive)
{
	CMem mem,buf,buf1;
	int pos = -1,c,flag = 0;

	LOCAL_MEM(buf);
	LOCAL_MEM(buf1);

	mem.Init();

	mem.SetSplitChars(" \t");
	mem.SetP(words_list,strlen(words_list));

	while(!this->IsEnd())
	{
		mem.Seek(0);
		mem.ReadWord(&buf);
		pos = this->SearchStr(buf.p,case_sensive,TRUE);
		if(pos < 0) return -1;

		this->Seek(pos);
		this->ReadWord(&buf); //skip first word
		flag = 1;
		while(!mem.IsEnd())
		{
			mem.ReadWord(&buf);
			this->ReadWord(&buf1);
		
			c = case_sensive ? buf.StrCmp(buf1.p):buf.StrICmp(buf1.p);
			if(c != 0)
			{
				this->Seek(pos + 1);
				flag = 0;
				break;
			}
		}

		if(flag)
			return pos;
	}

	return -1;
}

int CFileBase::StrLen()
{
	int i,s =  this->GetSize();
	
	for(i = s-1; i >= 0; i--)
	{
		if(this->CharAt(i) ==0)
			s--;
		else
			break;
	}
	
	return s;
}

int CFileBase::StrCat(char *str)
{
	this->Seek(this->StrLen());
	this->Puts(str);
	this->Putc(0);

	return OK;
}

int CFileBase::StrCmp(char *str)
{
	CMem mem;
	mem.Init();
	mem.SetP(str,strlen(str) + 1);

	return this->StrCmp(&mem);
}

int CFileBase::StrCat(CFileBase *file)
{
	int len = this->StrLen();
	this->SetSize(len + 1);
	this->Seek(len);
	this->WriteFile(file);
	this->Putc(0);

	return OK;
}

int CFileBase::StrCmp(CFileBase *file)
{
	int ret = 0;
	unsigned char c1,c2;

	this->Seek(0);
	file->Seek(0);
	while(!file->IsEnd())
	{
		c1 = this->Getc();
		c2 = file->Getc();
		
		ret = c1 - c2;

		if(ret != 0)
			break;		
	}

	return ret;
}

int CFileBase::StrICmp(CFileBase *file)
{
	int ret = 0;
	unsigned char c1,c2;

	this->Seek(0);
	file->Seek(0);
	while(!file->IsEnd())
	{
		c1 = tolower(this->Getc());
		c2 = tolower(file->Getc());
		
		ret = c1 - c2;

		if(ret != 0)
			break;		
	}
	
	return ret;
}

int CFileBase::StrICmp(char *str)
{
	CMem mem;

	mem.Seek(0);
	mem.SetP(str,strlen(str)+1);

	return this->StrICmp(&mem);
}

int CFileBase::StrCpy(CFileBase *file)
{
	this->SetSize(0);
	this->WriteFile(file);
	this->Putc(0);

	return OK;
}

int CFileBase::StrCpy(char *str)
{
	CMem mem;

	mem.Seek(0);
	mem.SetP(str,strlen(str)+1);

	return this->StrCpy(&mem);
}

int CFileBase::StartWith(CFileBase *file_str, int case_sensive, int skip_empty)
{
	ASSERT(file_str);
	
	int i,len;
	unsigned char c1,c2;

	this->Seek(0);
	if(skip_empty)
		this->SkipEmptyChars();
	
	len = file_str->StrLen();

	if(this->GetSize() < len)
		return FALSE;

	file_str->Seek(0);

	for(i = 0; i < len; i++)
	{
		c1 = this->Getc();
		c2 = file_str->Getc();
		if( ! case_sensive)
		{
			c1 = tolower(c1);
			c2 = tolower(c2);
		}

		if(c1 != c2)
			return FALSE;		
	}

	return TRUE;
}

int CFileBase::StartWith(char *str, int case_sensive, int skip_empty)
{
	CMem mem;

	mem.Seek(0);
	mem.SetP(str,strlen(str)+1);
	
	return this->StartWith(&mem,case_sensive,skip_empty);
}
//字符串规范化，清除前后的空字符
int CFileBase::StdStr()
{
	int i,l,n;
	
	l = this->StrLen();

	for(i = l - 1; i >= 0; i--)
	{
		if(!CFileBase::IsEmptyChar(this->CharAt(i)))
			break;
	}
	
	l = i + 1;
	for(i = 0; i < l; i++)
	{
		if(	! CFileBase::IsEmptyChar(this->CharAt(i)) )
			break;
	}
	l -= i;
	n = i;
	for(i = 0; i < l; i++)
	{
		this->SetChar(i,this->CharAt(i+n));
	}

	this->SetChar(l,0);
	this->SetSize(l + 1);

	return OK;
}
//read left to file, from current offset
int CFileBase::ReadLeftStr(CFileBase *file,int skip_empty)
{
	int ret = 0;

	file->SetSize(0);
	if(skip_empty)
		this->SkipEmptyChars();
	ret = this->WriteToFile(file,this->GetOffset(),this->GetSize() - this->GetOffset());
	file->Putc(0);

	return ret;
}

int CFileBase::Puts(CFileBase *file_str)
{
	ASSERT(file_str);
	int len = file_str->StrLen();
	return this->WriteFile(file_str,0,len);
}
int CFileBase::AdjustGbPos(int char_pos)
{
	int c = 0;
	char ch;

	if(char_pos < 0)
		return 0;

	if(char_pos >= this->GetSize())
		char_pos = this->GetSize() - 1;

	this->Seek(char_pos);
	while(this->GetOffset() > 0)
	{
		ch = this->UnGetc();
		if((BYTE)ch <= 160)
			break;

		c++;
	}
	if(c & 0x01)
		return char_pos -1;
	return char_pos;
}

#endif

//////////////////////////////////////////////////////////////////////////
//  Unicode version
//////////////////////////////////////////////////////////////////////////
#if _UNICODE_

WORD CFileBase::GetcW()
{
	WORD w = 0;
	this->Read(&w,2);
	return w;
}

int CFileBase::PutcW(WORD uc)
{
	return this->Write(&uc,2);
}

WORD CFileBase::CharAtW(int index)
{
	WORD u;
	int old_off = this->GetOffset();

	this->Seek(index * 2);
	this->Read(&u,2);

	this->Seek(old_off);

	return u;
}
int CFileBase::SetCharW(int index, WORD uc)
{
	int old_off = this->GetOffset();

	this->Seek(index * 2);
	this->Write(&uc,2);

	this->Seek(old_off);

	return OK;
}

int CFileBase::StrLenW()
{
	int i,s =  this->GetSize() >> 1;
	
	for(i = s-1; i >= 0; i--)
	{
		if(this->CharAtW(i) ==0)
			s--;
		else
			break;
	}
	
	return s;
}
int CFileBase::StrCmpW(CFileBase *file)
{
	int ret = 0;
	WORD c1,c2;

	this->Seek(0);
	file->Seek(0);
	while(!file->IsEnd())
	{
		c1 = this->GetcW();
		c2 = file->GetcW();
		
		ret = c1 - c2;

		if(ret != 0)
			break;		
	}

	return ret;
}
int CFileBase::StrLenW(WORD *str)
{
	int s = 0;
	while(*str++)s++;
	return s;
}

int CFileBase::StrICmpW(CFileBase *file)
{
	int ret = 0;
	WORD c1,c2;

	this->Seek(0);
	file->Seek(0);
	while(!file->IsEnd())
	{
		c1 = CFileBase::ToLowerW(this->GetcW());
		c2 = CFileBase::ToLowerW(file->GetcW());
		
		ret = c1 - c2;

		if(ret != 0)
			break;		
	}
	
	return ret;
}

int CFileBase::StrCpyW(CFileBase *file)
{
	this->SetSize(0);
	this->WriteFile(file);
	this->PutcW(0);

	return OK;
}

int CFileBase::StrCpyW(WORD *str)
{
	CMem mem;

	mem.Seek(0);
	mem.SetP((char*)str,CFileBase::StrLenW(str)+2);

	return this->StrCpyW(&mem);
}

int CFileBase::StrICmpW(WORD *str)
{
	CMem mem;

	mem.Seek(0);
	mem.SetP((char*)str,CFileBase::StrLenW(str)+2);

	return this->StrICmpW(&mem);
}

WORD CFileBase::ToLowerW(WORD uc)
{
	WORD *pu = L"AaZz";

	if(uc >= pu[0] && uc <= pu[2])
	{
		uc += pu[1] - pu[0];
	}

	return uc;
}

int CFileBase::StrLwrW()
{
	this->Seek(0);

	int i,len = this->StrLenW();

	for(i = 0; i < len; i++)
	{
		this->SetCharW(i,this->ToLowerW(this->CharAtW(i)));
	}

	return OK;
}

int CFileBase::StrCatW(CFileBase *file)
{
	int len = this->StrLenW();
	this->SetSize(len + 2);
	this->Seek(len);
	this->WriteFile(file);
	this->PutcW(0);

	return OK;
}

int CFileBase::StrCatW(WORD *str)
{
	this->Seek(this->StrLenW());
	this->PutsW(str);
	this->PutcW(0);

	return OK;
}
int CFileBase::PutsW(WORD *str)
{
	if(str == NULL)
		return ERROR;

	return this->Write(str,this->StrLenW(str) * 2);
}

int CFileBase::PrintfW(WORD *szFormat, ...)
{
    WORD szBuffer [FILEBASE_LBUF_SIZE];

    va_list pArgList ;
    va_start (pArgList, szFormat) ;
    wvsprintfW (szBuffer, szFormat, pArgList) ;
    va_end (pArgList) ;
    
	return this->PutsW(szBuffer);	
}

int CFileBase::IsEmptyCharW(WORD uc)
{
	WORD *str = L" \t\n\r\0";
	
	if(		uc != str[0] &&	uc != str[1]
		&&	uc != str[2] &&	uc != str[3]
		&&	uc != str[4])
		return FALSE;
	
	return TRUE;
}
int CFileBase::SetDefaultSpCharsW()
{
	this->sp_chars_w = L" \t\r\n!@#$%^=&*()|+-\\*/{}[];.\":\',?/<>~";
	return OK;
}

int CFileBase::ReadLineW(CFileBase *file)
{
	WORD ch,*uc=L"\r\n";
	
	if(file == NULL)
		return ERROR;

	if(this->IsEnd()) 
		return ERROR;

	file->SetSize(0);

	ch = this->GetcW();
	while(ch != uc[1])
	{
		if(ch!=uc[0])
		{
			file->PutcW(ch);
		}
		
		if(this->IsEnd())
			break;

		ch = this->GetcW();  
	}

	file->PutcW(0);

	return OK;
}
int CFileBase::ReadLine_ReverseW()
{
	char ch;
	WORD *uc=L"\n";
	
	ch = this->UnPutcW();
	if(ch != uc[0])
		this->PutcW(ch);

	while(1)
	{
		ch = this->UnPutcW();
		if(this->GetOffset() <= 0)
			return OK;
		if(ch == uc[0])
		{
			this->PutcW(ch);
			break;
		}
	}

	return OK;
}
int CFileBase::UnPutcW()
{
	WORD ch = 0;
	long off;

	off = this->GetOffset();

	if(off > 0)
	{
		off -= 2;
		this->Seek(off);
		off = this->GetOffset();
		ch =  this->GetcW();
		this->Seek(off);
	}

	return ch;
}

WORD CFileBase::UnGetcW()
{
	return this->UnPutcW();
}

int CFileBase::ReadLine_ReverseW(CFileBase *file)
{
	long off;

	this->ReadLine_ReverseW();	
	off = this->GetOffset();
	this->ReadLineW(file);
	this->Seek(off);
	
	return OK;
}
//the return value is the number of words it reads.
int CFileBase::ReadWordWithEmptyCharW(CFileBase *file)
{
	int k = 0;
	WORD ch,*uc = L"\\";
	
	ASSERT(file);

	file->SetSize(0);

	if(this->IsEnd())
		goto end;

	ch = this->GetcW();
	if(ch == 0)
		goto end;
	
	if(ch == uc[0])
	{
		file->PutcW(uc[0]);
		file->PutcW(this->GetcW());
		k = 2;
		goto end;
	}
	if(this->IsSpCharW(ch))
	{
		file->PutcW(ch);
		k = 1;
		goto end;
	}
	while(!this->IsSpCharW(ch))
	{
		file->PutcW(ch);
		k++;
		if(this->IsEnd())
			break;
		ch = this->GetcW();
	}
	if(this->IsSpCharW(ch))
		this->UnGetcW();
end:		
	file->PutcW(0);
	return k;
}
int CFileBase::IsSpCharW(WORD ch)
{
	WORD *p;

	if(ch == 0)
		return TRUE;

	p = this->sp_chars_w;
	while(*p)
	{
		if(*p == ch)
			return TRUE;
		p++;
	}
	
	return FALSE;
}

//the return value is the number of bytes it reads.
int CFileBase::ReadWordWithEmptyCharW_Reverse(CFileBase *file)
{
	int k = 0;
	WORD ch,*uc=L"\\";
	
	ASSERT(file);

	file->SetSize(0);

	if(this->GetOffset() <= 0 )
		goto end;

	ch = this->UnGetcW();
	if(ch == 0)
		goto end;
	
	if(ch == uc[0])
	{
		file->PutcW(uc[0]);
		file->PutcW(this->UnGetcW());
		k = 2;
		goto end;
	}
	if(this->IsSpCharW(ch))
	{
		file->PutcW(ch);
		k = 1;
		goto end;
	}
	while(!this->IsSpCharW(ch))
	{
		file->PutcW(ch);

		if(this->GetOffset() <= 0)
			break;
		k++;

		ch = this->UnGetcW();
	}

	if(this->IsSpCharW(ch))
		this->GetcW();
end:		
	file->ReverseW(0,file->GetOffset() - 1);
	file->PutcW(0);

	return k;
}
//start 和 end 都从0开始
//所以长度应该是 end - start  + 1
int CFileBase::ReverseW(long start, long end)
{
	long i,t;
	WORD ch;
	
	t = end - start + 1;
	
	for(i = 0; i < t / 2; i++)
	{
		ch = this->CharAtW(i + start);
		this->SetCharW(i + start, this->CharAtW(start + t - i -1 ));
		this->SetCharW(start + t - i - 1, ch);
	}

	return OK;
}
int CFileBase::SkipEmptyCharsW()
{
	WORD ch;
	while(!this->IsEnd())
	{
		ch = this->GetcW();
		if( ! this->IsEmptyCharW(ch) )
		{
			this->UnGetcW();
			break;
		}
	}
	return OK;
}
int CFileBase::SkipEmptyCharsW_Reverse()
{
	WORD ch;
	while(this->GetOffset() > 0)
	{
		ch = this->UnGetcW();
		if( ! this->IsEmptyCharW(ch) )
		{
			this->GetcW();
			break;
		}
	}
	return OK;
}
int CFileBase::SetSplitCharsW(WORD *sp_str)
{
	if(sp_str)
		this->sp_chars_w = sp_str;
	return OK;
}
int CFileBase::ReadWordW(CFileBase *file)
{
	this->SkipEmptyCharsW();
	return this->ReadWordWithEmptyCharW(file);
}
int CFileBase::ReadStringW(CFileBase *file)
{
	int k;
	k = this->ReadWordW(file);
	while(k)
	{
		file->Seek(0);
		if(!this->IsSpCharW(file->GetcW()))	
		{
			return OK;
		}
		k = this->ReadWordW(file);
	}
	return ERROR;
}
int CFileBase::ReadWordW_Reverse(CFileBase *file)
{
	this->SkipEmptyCharsW_Reverse();
	return this->ReadWordWithEmptyCharW_Reverse(file);
}
int CFileBase::ReadStringW_Reverse(CFileBase *file)
{
	int k;
	k = this->ReadWordW_Reverse(file);
	while(k)
	{
		file->Seek(0);
		if(!this->IsSpCharW(file->GetcW()))	
		{
			return OK;
		}
		k = this->ReadWordW_Reverse(file);
	}
	return ERROR;
}
//if the first char is special char, the word_only is ignored
int CFileBase::SearchStrW(WORD *str, int case_sensive,int word_only)
{
	WORD *s2,ch1,ch2;
	int old,pos,len;
	int flag = 0;
	
	if(str == NULL)
		return -1;

	len = this->StrLenW(str);
	
	while(!this->IsEnd())
	{
		old = this->GetOffset();		
		ch1 = this->GetcW();
		s2 = str;
		ch2 = *s2;
		if(!case_sensive)
		{
			ch1 = this->ToLowerW(ch1);
			ch2 = this->ToLowerW(ch2);
		}
		while (ch2 && !(ch1 - ch2)) 
		{
			s2 ++;
			ch2 = *s2;
			
			if(this->IsEnd())
			{flag = 1;break;}
			
			ch1=this->GetcW();
			if(!case_sensive)
			{
				ch1 = this->ToLowerW(ch1);
				ch2 = this->ToLowerW(ch2);
			}
		}
		if(!*s2)
		{
			pos = (this->GetOffset()>>1) - len - 1;
			if(flag) pos++; //if hit end
			
			if(!word_only)
			{
				return pos;
			}
			else if(IsSpCharW(CharAtW(pos)))
			{
				return pos;
			}
			else
			{
				if((pos == 0||IsSpCharW(CharAtW(pos - 1)))&&IsSpCharW(CharAtW(pos + len)))
					return pos;
			}

		}
		this->Seek(old + 2);
	}
	return -1;
}
int CFileBase::ReplaceStrW(WORD *src_str, WORD *des_str, int case_sensive, int word_only,CFileBase *new_file)
{
	long pos,old_pos,len1;

	ASSERT(new_file);
	ASSERT(new_file != this);

	len1 = this->StrLenW(src_str);

	new_file->SetSize(0);
	
	old_pos = this->GetOffset();
	if(old_pos > 0)
	{
		this->WriteToFile(new_file,0,old_pos);
	}

	this->Seek(old_pos);

	while(!this->IsEnd())
	{
		pos = this->SearchStrW(src_str,case_sensive,word_only);
		if(pos < 0)
		{
			this->WriteToFile(new_file,old_pos,this->GetSize() - old_pos);
			break;
		}
		else
		{
			this->WriteToFile(new_file,old_pos,(pos << 1) - old_pos);
			new_file->PutsW(des_str);
			old_pos = (pos + len1) << 1;

			this->Seek(old_pos);
		}
	}
	return OK;
}

#endif

