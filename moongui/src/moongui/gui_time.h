#ifndef __GUI_TIME_H
#define __GUI_TIME_H

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

int set_current_timer(int cur);
int start_timer();
int stop_timer();
u16 get_timer_count();
void set_timer_count(u16 tc);
int timer_int_enable();
int timer_int_disable();
void timer_int3_handler();
void install_timer3_int_handler(void);
int get_sys_time();
/************************************** jiaojinxing1987@gmail.com ***************************************/
#if !_SMILEOS_
int sleep(int mSecond);
#endif
/************************************************* end **************************************************/

#ifdef __cplusplus
} //extern "C"
#endif

#endif
