/*********************************************************************
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2012  SEGGER Microcontroller GmbH & Co. KG       *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

** emWin V5.16 - Graphical user interface for embedded applications **
emWin is protected by international copyright laws.   Knowledge of the
source code may not be used to write a similar product.  This file may
only be used in accordance with a license and should not be re-
distributed in any way. We appreciate your understanding and fairness.
----------------------------------------------------------------------
File        : GUI_X.c
Purpose     : Config / System dependent externals for GUI
---------------------------END-OF-HEADER------------------------------
*/

#include "GUI.h"
#include "kern/kern.h"
#include "kern/ipc.h"

/*********************************************************************
*
*       Global data
*/
static mutex_t gui_lock;

/*********************************************************************
*
*      Timing:
*                 GUI_X_GetTime()
*                 GUI_X_Delay(int)

  Some timing dependent routines require a GetTime
  and delay function. Default time unit (tick), normally is
  1 ms.
*/

int GUI_X_GetTime(void) {
    return getticks() * 1000 / 100;
}

void GUI_X_Delay(int ms) {
    msleep(ms);
}

/*********************************************************************
*
*       GUI_X_Init()
*
* Note:
*     GUI_X_Init() is called from GUI_Init is a possibility to init
*     some hardware which needs to be up and running before the GUI.
*     If not required, leave this routine blank.
*/

void GUI_X_Init(void) {
    extern void TouchTask(void *arg);
    kthread_create("touch", TouchTask, NULL, 16 * KB, 50);
}

/*********************************************************************
*
*       GUI_X_ExecIdle
*
* Note:
*  Called if WM is in idle state
*/

void GUI_X_ExecIdle(void) {
    GUI_X_Delay(1);
}

/*********************************************************************
*
*      Multitasking:
*
*                 GUI_X_InitOS()
*                 GUI_X_GetTaskId()
*                 GUI_X_Lock()
*                 GUI_X_Unlock()
*
* Note:
*   The following routines are required only if emWin is used in a
*   true multi task environment, which means you have more than one
*   thread using the emWin API.
*   In this case the
*                       #define GUI_OS 1
*  needs to be in GUIConf.h
*/

void GUI_X_InitOS(void)    {
    mutex_new(&gui_lock);
}

void GUI_X_Unlock(void)    {
    mutex_unlock(&gui_lock);
}

void GUI_X_Lock(void)      {
    mutex_lock(&gui_lock, 0);
}

U32  GUI_X_GetTaskId(void) {
    return gettid();
}

/*********************************************************************
*
*      Event driving (optional with multitasking)
*
*                 GUI_X_WaitEvent()
*                 GUI_X_WaitEventTimed()
*                 GUI_X_SignalEvent()
*/

void GUI_X_WaitEvent(void)            {

}

void GUI_X_SignalEvent(void)          {

}

void GUI_X_WaitEventTimed(int Period) {

}

/*********************************************************************
*
*      Logging: OS dependent

Note:
  Logging is used in higher debug levels only. The typical target
  build does not use logging and does therefor not require any of
  the logging routines below. For a release build without logging
  the routines below may be eliminated to save some space.
  (If the linker is not function aware and eliminates unreferenced
  functions automatically)

*/

void GUI_X_Log     (const char *s) {
    printk(KERN_INFO "%s", s);
}

void GUI_X_Warn    (const char *s) {
    printk(KERN_WARNING "%s", s);
}

void GUI_X_ErrorOut(const char *s) {
    printk(KERN_ERR "%s", s);
}

/*************************** End of file ****************************/
