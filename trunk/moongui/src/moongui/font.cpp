#include "font.h"

CFont::CFont()
{
    this->InitBasic();
}
CFont::~CFont()
{
    this->Destroy();
}
int CFont::InitBasic()
{
    return OK;
}
int CFont::Init()
{
    this->InitBasic();
    //add your code
    return OK;
}
int  CFont::Destroy()
{
    //add your code
    this->InitBasic();
    return OK;
}
