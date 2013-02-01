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

** emWin V5.18 - Graphical user interface for embedded applications **
All  Intellectual Property rights  in the Software belongs to  SEGGER.
emWin is protected by  international copyright laws.  Knowledge of the
source code may not be used to write a similar product.  This file may
only be used in accordance with the following terms:

The software has been licensed to  NXP Semiconductors USA, Inc.  whose
registered  office  is  situated  at  1109 McKay Dr, M/S 76, San Jose, 
CA 95131, USA  solely for  the  purposes  of  creating  libraries  for 
NXPs M0, M3/M4 and  ARM7/9 processor-based  devices,  sublicensed  and
distributed under the terms and conditions of the NXP End User License
Agreement.
Full source code is available at: www.segger.com

We appreciate your understanding and fairness.
----------------------------------------------------------------------
File        : GUIConf.c
Purpose     : Display controller initialization
---------------------------END-OF-HEADER------------------------------
*/

#include "GUI.h"
#include "kern/kern.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
//
// Define the available number of bytes available for the GUI
//
#define GUI_NUMBYTES  (5 * MB)

//
// Define the average block size
//
#define GUI_BLOCKSIZE 0x256

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       GUI_X_Config
*
* Purpose:
*   Called during the initialization process in order to set up the
*   available memory for the GUI.
*/
void GUI_X_Config(void) {
  //
  // Assign memory to emWin
  //
  void *ptr = kmalloc(GUI_NUMBYTES, GFP_KERNEL);
  if (ptr == NULL) {
      GUI_X_ErrorOut("failed to alloc emWin memory, serious error\n");
      /*
       * TODO: ³ö´í´¦Àí
       */
  }

  GUI_ALLOC_AssignMemory(ptr, GUI_NUMBYTES);

  GUI_ALLOC_SetAvBlockSize(GUI_BLOCKSIZE);

  //
  // Set default font
  //
  GUI_SetDefaultFont(GUI_FONT_6X8);
}

/*************************** End of file ****************************/
