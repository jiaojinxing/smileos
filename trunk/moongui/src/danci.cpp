// GdsFile.cpp: implementation of the CGdsFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "danci.h"

#define WORD_LENGTH    30
#define YINBIAO_LENGTH 30
#define HANYU_LENGTH   41
#define NUMBER_LENGTH  27
#define DANCI_SIZE     128L
#define DATA_OFFSET    290L

CDanci::CDanci()
{
    this->InitBasic();
}
CDanci::~CDanci()
{
    this->Destroy();
}
int CDanci::InitBasic()
{
    this->word = NULL;
	this->yinbiao = NULL;
	this->hanyu = NULL;

    return OK;
}
int CDanci::Init()
{
    this->InitBasic();
    
	NEW(this->word,CMemFile);
	NEW(this->yinbiao,CMemFile);
	NEW(this->hanyu,CMemFile);

	this->word->Init(32,100);
	this->yinbiao->Init(32,100);
	this->hanyu->Init(512,128);

	this->Clear();

    return OK;
}
int CDanci::Destroy()
{
    DEL(this->word);
	DEL(this->yinbiao);
	DEL(this->hanyu);

    this->InitBasic();
    return OK;
}
int CDanci::Copy(CDanci *p)
{
	ASSERT(p);

    if(this == p)
        return OK;
    
	this->word->SetSize(0);
	this->word->WriteFile(p->word);

	this->yinbiao->SetSize(0);
	this->yinbiao->WriteFile(p->yinbiao);

	this->hanyu->SetSize(0);
	this->hanyu->WriteFile(p->hanyu);

    return OK;
}

int CDanci::Comp(CDanci *p)
{
	ASSERT(p);
    return this->word->StrICmp(p->word);
}
int CDanci::Comp(char *str)
{
	ASSERT(str);
    return this->word->StrICmp(str);
}
int CDanci::Print()
{
    LOG("\nword = ");word->Dump();
	LOG("\nyinbiao = ");yinbiao->Dump();
	LOG("\nhanyu = ");hanyu->Dump();	
    return TRUE;
}
int CDanci::Clear()
{
	this->word->SetSize(0);
	this->yinbiao->SetSize(0);
	this->hanyu->SetSize(0);

	return OK;
}

int CDanci::SetWord(CFileBase *file)
{
	ASSERT(file);
	this->word->SetSize(0);
	this->word->StrCat(file);

	return OK;
}
int CDanci::SetWord(char *str)
{
	ASSERT(str);
	this->word->SetSize(0);
	this->word->StrCat(str);
	return OK;
}
int CDanci::SetYinBiao(CFileBase *file)
{
	ASSERT(file);
	this->yinbiao->SetSize(0);
	this->yinbiao->StrCat(file);

	return OK;
}
int CDanci::SetYinBiao(char *str)
{
	ASSERT(str);
	this->yinbiao->SetSize(0);
	this->yinbiao->StrCat(str);
	return OK;
}
int CDanci::SetHanYu(CFileBase *file)
{
	ASSERT(file);
	this->hanyu->SetSize(0);
	this->AddHanyu(file);

	return OK;
}

int CDanci::AddHanyu(CFileBase *file)
{
	ASSERT(file);

	if(this->hanyu->GetSize() > 0)
		this->hanyu->StrCat("\n");
	this->hanyu->StrCat(file);
	
	return OK;
}
int CDanci::AddHanyu(char *str)
{
	ASSERT(str);

	if(this->hanyu->GetSize() > 0)
		this->hanyu->StrCat("\n");
	this->hanyu->StrCat(str);
	
	return OK;
}
int CDanci::IsEmpty()
{
	return this->word->GetSize() <= 0;
}
int CDanci::DumpToFile(CFileBase *file)
{
	ASSERT(file);
	CMem mem;

	LOCAL_MEM(mem);
	
	file->Putc('@'); file->Puts(this->word); file->Putc('\n');
	if(this->yinbiao->GetSize() > 0)
	{
		file->Putc('#'); file->Puts(this->yinbiao); file->Putc('\n');
	}
	
	this->hanyu->Seek(0);
	while(this->hanyu->ReadLine(&mem))
	{
		file->Putc('$');
		file->Puts(mem.p);
		file->Putc('\n');
	}
	
	return OK;
}

//////////////////////////////////////////////////////////////////////
// GDS file
//////////////////////////////////////////////////////////////////////
CGdsFile::CGdsFile()
{
    this->InitBasic();
}
CGdsFile::~CGdsFile()
{
    this->Destroy();
}
int CGdsFile::InitBasic()
{
    this->mf_gds = NULL;
	this->max_danci = 0;

    return OK;
}
int CGdsFile::Init()
{
    this->InitBasic();
    
	NEW(this->mf_gds,CMemFile);
	this->mf_gds->Init();

    return OK;
}
int CGdsFile::Destroy()
{
    DEL(this->mf_gds);
    this->InitBasic();
    return OK;
}
int CGdsFile::Print()
{
    PD(this->max_danci);
    return TRUE;
}

int CGdsFile::GetName(CFileBase *file)
{
	ASSERT(file);

	file->SetSize(0);
	this->mf_gds->WriteToFile(file,12,20);
	this->mf_gds->WriteToFile(file,50,8);
	file->Putc(0);
	
	file->StdStr();

	return OK;
}

int CGdsFile::LoadGdsFile(char *fn)
{
	this->mf_gds->LoadFile(fn);
	this->max_danci = (this->mf_gds->GetSize() - DATA_OFFSET) / DANCI_SIZE;

	return OK;
}

int CGdsFile::GetDanci(int index, CDanci *danci)
{
	ASSERT(danci);

	int pos;

	danci->Clear();
	if(index < 0 || index >= this->max_danci)
	{
		return ERROR;
	}
	
	pos = index*DANCI_SIZE+DATA_OFFSET;
	
	this->mf_gds->WriteToFile(danci->word,pos, WORD_LENGTH);
	pos += WORD_LENGTH;
	this->mf_gds->WriteToFile(danci->yinbiao,pos, YINBIAO_LENGTH);
	pos += YINBIAO_LENGTH;
	this->mf_gds->WriteToFile(danci->hanyu,pos, HANYU_LENGTH);
	
	danci->word->Putc(0);
	danci->yinbiao->Putc(0);
	danci->hanyu->Putc(0);

	danci->word->StdStr();
	danci->yinbiao->StdStr();
	danci->hanyu->StdStr();

	return OK;
}




