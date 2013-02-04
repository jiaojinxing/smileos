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
#define GUI_NUMBYTES  (4 * MB)

//
// Define the average block size
//
#define GUI_BLOCKSIZE 0x128

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
      GUI_X_ErrorOut("emWin failed to alloc private memory, serious error\n");
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
