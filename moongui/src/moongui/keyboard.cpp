#include "keyboard.h"
	
CKeyboard::CKeyboard()
{
    this->InitBasic();
}
CKeyboard::~CKeyboard()
{
    this->Destroy();
}
int CKeyboard::InitBasic()
{
	this->all_key = 0;
	this->all_key_old = 0;
	this->key_timer = 0;

	return OK;
}
int CKeyboard::Init()
{
	int i;
    this->InitBasic();

    return OK;
}
int CKeyboard::Destroy()
{
    this->InitBasic();
    return OK;
}

int CKeyboard::ReadKey()
{
	this->all_key_old = this->all_key;
/************************************** jiaojinxing1987@gmail.com ***************************************/
#if _NDS_
	this->all_key = keysHeld() & 0x00000fff;
#elif _SMILEOS_
    /*
     * TODO: SmileOS ¼üÅÌÏà¹Ø´úÂë
     */
    this->all_key = 0;
#else
    this->all_key = 0;
#endif
/************************************************* end **************************************************/
	return OK;
}

int CKeyboard::IsKeyPress(int key)
{
	return this->all_key & (1 << key);
}

int CKeyboard::KeyChanged()
{
	return this->all_key != this->all_key_old;
}

int CKeyboard::IsOldKeyPress(int key)
{
	return this->all_key_old & (1 << key);
}
