#ifndef __EVENT_H
#define __EVENT_H

#include "msgqueue.h"

int GetMessage(CMsg *msg);
int TranslateMessage(CMsg *msg);
int DispatchMessage(CMsg *msg);
int MsgLoop();
int GetLowLevelMessage();
int GetMouseMessage();
int GetTimerEvent();
int GetKeyboardEvent();

#endif
