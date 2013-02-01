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
File        : GUIDRV_SPage.h
Purpose     : Interface definition for GUIDRV_SPage driver
---------------------------END-OF-HEADER------------------------------
*/

#ifndef GUIDRV_SPAGE_H
#define GUIDRV_SPAGE_H

#if defined(__cplusplus)
extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif

/*********************************************************************
*
*       Configuration structure
*/
typedef struct {
  //
  // Driver specific configuration items
  //
  int FirstSEG;
  int FirstCOM;
} CONFIG_SPAGE;

/*********************************************************************
*
*       Display drivers
*/
//
// Addresses
//
extern const GUI_DEVICE_API GUIDRV_SPage_1C0_API;
extern const GUI_DEVICE_API GUIDRV_SPage_OY_1C0_API;
extern const GUI_DEVICE_API GUIDRV_SPage_OX_1C0_API;
extern const GUI_DEVICE_API GUIDRV_SPage_OXY_1C0_API;
extern const GUI_DEVICE_API GUIDRV_SPage_OS_1C0_API;
extern const GUI_DEVICE_API GUIDRV_SPage_OSY_1C0_API;
extern const GUI_DEVICE_API GUIDRV_SPage_OSX_1C0_API;
extern const GUI_DEVICE_API GUIDRV_SPage_OSXY_1C0_API;

extern const GUI_DEVICE_API GUIDRV_SPage_1C1_API;
extern const GUI_DEVICE_API GUIDRV_SPage_OY_1C1_API;
extern const GUI_DEVICE_API GUIDRV_SPage_OX_1C1_API;
extern const GUI_DEVICE_API GUIDRV_SPage_OXY_1C1_API;
extern const GUI_DEVICE_API GUIDRV_SPage_OS_1C1_API;
extern const GUI_DEVICE_API GUIDRV_SPage_OSY_1C1_API;
extern const GUI_DEVICE_API GUIDRV_SPage_OSX_1C1_API;
extern const GUI_DEVICE_API GUIDRV_SPage_OSXY_1C1_API;

extern const GUI_DEVICE_API GUIDRV_SPage_2C0_API;
extern const GUI_DEVICE_API GUIDRV_SPage_OY_2C0_API;
extern const GUI_DEVICE_API GUIDRV_SPage_OX_2C0_API;
extern const GUI_DEVICE_API GUIDRV_SPage_OXY_2C0_API;
extern const GUI_DEVICE_API GUIDRV_SPage_OS_2C0_API;
extern const GUI_DEVICE_API GUIDRV_SPage_OSY_2C0_API;
extern const GUI_DEVICE_API GUIDRV_SPage_OSX_2C0_API;
extern const GUI_DEVICE_API GUIDRV_SPage_OSXY_2C0_API;

extern const GUI_DEVICE_API GUIDRV_SPage_2C1_API;
extern const GUI_DEVICE_API GUIDRV_SPage_OY_2C1_API;
extern const GUI_DEVICE_API GUIDRV_SPage_OX_2C1_API;
extern const GUI_DEVICE_API GUIDRV_SPage_OXY_2C1_API;
extern const GUI_DEVICE_API GUIDRV_SPage_OS_2C1_API;
extern const GUI_DEVICE_API GUIDRV_SPage_OSY_2C1_API;
extern const GUI_DEVICE_API GUIDRV_SPage_OSX_2C1_API;
extern const GUI_DEVICE_API GUIDRV_SPage_OSXY_2C1_API;

extern const GUI_DEVICE_API GUIDRV_SPage_4C0_API;
extern const GUI_DEVICE_API GUIDRV_SPage_OY_4C0_API;
extern const GUI_DEVICE_API GUIDRV_SPage_OX_4C0_API;
extern const GUI_DEVICE_API GUIDRV_SPage_OXY_4C0_API;
extern const GUI_DEVICE_API GUIDRV_SPage_OS_4C0_API;
extern const GUI_DEVICE_API GUIDRV_SPage_OSY_4C0_API;
extern const GUI_DEVICE_API GUIDRV_SPage_OSX_4C0_API;
extern const GUI_DEVICE_API GUIDRV_SPage_OSXY_4C0_API;

extern const GUI_DEVICE_API GUIDRV_SPage_4C1_API;
extern const GUI_DEVICE_API GUIDRV_SPage_OY_4C1_API;
extern const GUI_DEVICE_API GUIDRV_SPage_OX_4C1_API;
extern const GUI_DEVICE_API GUIDRV_SPage_OXY_4C1_API;
extern const GUI_DEVICE_API GUIDRV_SPage_OS_4C1_API;
extern const GUI_DEVICE_API GUIDRV_SPage_OSY_4C1_API;
extern const GUI_DEVICE_API GUIDRV_SPage_OSX_4C1_API;
extern const GUI_DEVICE_API GUIDRV_SPage_OSXY_4C1_API;

//
// Macros to be used in configuration files
//
#if defined(WIN32) && !defined(LCD_SIMCONTROLLER)

  #define GUIDRV_SPAGE_1C0       &GUIDRV_Win_API
  #define GUIDRV_SPAGE_OY_1C0    &GUIDRV_Win_API
  #define GUIDRV_SPAGE_OX_1C0    &GUIDRV_Win_API
  #define GUIDRV_SPAGE_OXY_1C0   &GUIDRV_Win_API
  #define GUIDRV_SPAGE_OS_1C0    &GUIDRV_Win_API
  #define GUIDRV_SPAGE_OSY_1C0   &GUIDRV_Win_API
  #define GUIDRV_SPAGE_OSX_1C0   &GUIDRV_Win_API
  #define GUIDRV_SPAGE_OSXY_1C0  &GUIDRV_Win_API

  #define GUIDRV_SPAGE_1C1       &GUIDRV_Win_API
  #define GUIDRV_SPAGE_OY_1C1    &GUIDRV_Win_API
  #define GUIDRV_SPAGE_OX_1C1    &GUIDRV_Win_API
  #define GUIDRV_SPAGE_OXY_1C1   &GUIDRV_Win_API
  #define GUIDRV_SPAGE_OS_1C1    &GUIDRV_Win_API
  #define GUIDRV_SPAGE_OSY_1C1   &GUIDRV_Win_API
  #define GUIDRV_SPAGE_OSX_1C1   &GUIDRV_Win_API
  #define GUIDRV_SPAGE_OSXY_1C1  &GUIDRV_Win_API

  #define GUIDRV_SPAGE_2C0       &GUIDRV_Win_API
  #define GUIDRV_SPAGE_OY_2C0    &GUIDRV_Win_API
  #define GUIDRV_SPAGE_OX_2C0    &GUIDRV_Win_API
  #define GUIDRV_SPAGE_OXY_2C0   &GUIDRV_Win_API
  #define GUIDRV_SPAGE_OS_2C0    &GUIDRV_Win_API
  #define GUIDRV_SPAGE_OSY_2C0   &GUIDRV_Win_API
  #define GUIDRV_SPAGE_OSX_2C0   &GUIDRV_Win_API
  #define GUIDRV_SPAGE_OSXY_2C0  &GUIDRV_Win_API

  #define GUIDRV_SPAGE_2C1       &GUIDRV_Win_API
  #define GUIDRV_SPAGE_OY_2C1    &GUIDRV_Win_API
  #define GUIDRV_SPAGE_OX_2C1    &GUIDRV_Win_API
  #define GUIDRV_SPAGE_OXY_2C1   &GUIDRV_Win_API
  #define GUIDRV_SPAGE_OS_2C1    &GUIDRV_Win_API
  #define GUIDRV_SPAGE_OSY_2C1   &GUIDRV_Win_API
  #define GUIDRV_SPAGE_OSX_2C1   &GUIDRV_Win_API
  #define GUIDRV_SPAGE_OSXY_2C1  &GUIDRV_Win_API

  #define GUIDRV_SPAGE_4C0       &GUIDRV_Win_API
  #define GUIDRV_SPAGE_OY_4C0    &GUIDRV_Win_API
  #define GUIDRV_SPAGE_OX_4C0    &GUIDRV_Win_API
  #define GUIDRV_SPAGE_OXY_4C0   &GUIDRV_Win_API
  #define GUIDRV_SPAGE_OS_4C0    &GUIDRV_Win_API
  #define GUIDRV_SPAGE_OSY_4C0   &GUIDRV_Win_API
  #define GUIDRV_SPAGE_OSX_4C0   &GUIDRV_Win_API
  #define GUIDRV_SPAGE_OSXY_4C0  &GUIDRV_Win_API

  #define GUIDRV_SPAGE_4C1       &GUIDRV_Win_API
  #define GUIDRV_SPAGE_OY_4C1    &GUIDRV_Win_API
  #define GUIDRV_SPAGE_OX_4C1    &GUIDRV_Win_API
  #define GUIDRV_SPAGE_OXY_4C1   &GUIDRV_Win_API
  #define GUIDRV_SPAGE_OS_4C1    &GUIDRV_Win_API
  #define GUIDRV_SPAGE_OSY_4C1   &GUIDRV_Win_API
  #define GUIDRV_SPAGE_OSX_4C1   &GUIDRV_Win_API
  #define GUIDRV_SPAGE_OSXY_4C1  &GUIDRV_Win_API

#else

  #define GUIDRV_SPAGE_1C0       &GUIDRV_SPage_1C0_API
  #define GUIDRV_SPAGE_OY_1C0    &GUIDRV_SPage_OY_1C0_API
  #define GUIDRV_SPAGE_OX_1C0    &GUIDRV_SPage_OX_1C0_API
  #define GUIDRV_SPAGE_OXY_1C0   &GUIDRV_SPage_OXY_1C0_API
  #define GUIDRV_SPAGE_OS_1C0    &GUIDRV_SPage_OS_1C0_API
  #define GUIDRV_SPAGE_OSY_1C0   &GUIDRV_SPage_OSY_1C0_API
  #define GUIDRV_SPAGE_OSX_1C0   &GUIDRV_SPage_OSX_1C0_API
  #define GUIDRV_SPAGE_OSXY_1C0  &GUIDRV_SPage_OSXY_1C0_API

  #define GUIDRV_SPAGE_1C1       &GUIDRV_SPage_1C1_API
  #define GUIDRV_SPAGE_OY_1C1    &GUIDRV_SPage_OY_1C1_API
  #define GUIDRV_SPAGE_OX_1C1    &GUIDRV_SPage_OX_1C1_API
  #define GUIDRV_SPAGE_OXY_1C1   &GUIDRV_SPage_OXY_1C1_API
  #define GUIDRV_SPAGE_OS_1C1    &GUIDRV_SPage_OS_1C1_API
  #define GUIDRV_SPAGE_OSY_1C1   &GUIDRV_SPage_OSY_1C1_API
  #define GUIDRV_SPAGE_OSX_1C1   &GUIDRV_SPage_OSX_1C1_API
  #define GUIDRV_SPAGE_OSXY_1C1  &GUIDRV_SPage_OSXY_1C1_API

  #define GUIDRV_SPAGE_2C0       &GUIDRV_SPage_2C0_API
  #define GUIDRV_SPAGE_OY_2C0    &GUIDRV_SPage_OY_2C0_API
  #define GUIDRV_SPAGE_OX_2C0    &GUIDRV_SPage_OX_2C0_API
  #define GUIDRV_SPAGE_OXY_2C0   &GUIDRV_SPage_OXY_2C0_API
  #define GUIDRV_SPAGE_OS_2C0    &GUIDRV_SPage_OS_2C0_API
  #define GUIDRV_SPAGE_OSY_2C0   &GUIDRV_SPage_OSY_2C0_API
  #define GUIDRV_SPAGE_OSX_2C0   &GUIDRV_SPage_OSX_2C0_API
  #define GUIDRV_SPAGE_OSXY_2C0  &GUIDRV_SPage_OSXY_2C0_API

  #define GUIDRV_SPAGE_2C1       &GUIDRV_SPage_2C1_API
  #define GUIDRV_SPAGE_OY_2C1    &GUIDRV_SPage_OY_2C1_API
  #define GUIDRV_SPAGE_OX_2C1    &GUIDRV_SPage_OX_2C1_API
  #define GUIDRV_SPAGE_OXY_2C1   &GUIDRV_SPage_OXY_2C1_API
  #define GUIDRV_SPAGE_OS_2C1    &GUIDRV_SPage_OS_2C1_API
  #define GUIDRV_SPAGE_OSY_2C1   &GUIDRV_SPage_OSY_2C1_API
  #define GUIDRV_SPAGE_OSX_2C1   &GUIDRV_SPage_OSX_2C1_API
  #define GUIDRV_SPAGE_OSXY_2C1  &GUIDRV_SPage_OSXY_2C1_API

  #define GUIDRV_SPAGE_4C0       &GUIDRV_SPage_4C0_API
  #define GUIDRV_SPAGE_OY_4C0    &GUIDRV_SPage_OY_4C0_API
  #define GUIDRV_SPAGE_OX_4C0    &GUIDRV_SPage_OX_4C0_API
  #define GUIDRV_SPAGE_OXY_4C0   &GUIDRV_SPage_OXY_4C0_API
  #define GUIDRV_SPAGE_OS_4C0    &GUIDRV_SPage_OS_4C0_API
  #define GUIDRV_SPAGE_OSY_4C0   &GUIDRV_SPage_OSY_4C0_API
  #define GUIDRV_SPAGE_OSX_4C0   &GUIDRV_SPage_OSX_4C0_API
  #define GUIDRV_SPAGE_OSXY_4C0  &GUIDRV_SPage_OSXY_4C0_API

  #define GUIDRV_SPAGE_4C1       &GUIDRV_SPage_4C1_API
  #define GUIDRV_SPAGE_OY_4C1    &GUIDRV_SPage_OY_4C1_API
  #define GUIDRV_SPAGE_OX_4C1    &GUIDRV_SPage_OX_4C1_API
  #define GUIDRV_SPAGE_OXY_4C1   &GUIDRV_SPage_OXY_4C1_API
  #define GUIDRV_SPAGE_OS_4C1    &GUIDRV_SPage_OS_4C1_API
  #define GUIDRV_SPAGE_OSY_4C1   &GUIDRV_SPage_OSY_4C1_API
  #define GUIDRV_SPAGE_OSX_4C1   &GUIDRV_SPage_OSX_4C1_API
  #define GUIDRV_SPAGE_OSXY_4C1  &GUIDRV_SPage_OSXY_4C1_API

#endif

//
// Epson controller definitions
//
#define GUIDRV_SPAGE_EPSON_S1D15E05   0
#define GUIDRV_SPAGE_EPSON_S1D15E06   1
#define GUIDRV_SPAGE_EPSON_S1D15719   2
#define GUIDRV_SPAGE_EPSON_S1D15721   3

/*********************************************************************
*
*       Public routines
*/
#if defined(WIN32) && !defined(LCD_SIMCONTROLLER)
  
  #define GUIDRV_SPage_Config(pDevice, pConfig)
  #define GUIDRV_SPage_SetBus8(pDevice, pHW_API)
  #define GUIDRV_SPage_Set1510(pDevice)
  #define GUIDRV_SPage_Set1512(pDevice)
  #define GUIDRV_SPage_SetST7591(pDevice)
  #define GUIDRV_SPage_SetUC1611(pDevice)

  // Obsolete
  #define GUIDRV_SPage_SetS1D15(pDevice, Controller)

#else

  void GUIDRV_SPage_Config   (GUI_DEVICE * pDevice, CONFIG_SPAGE * pConfig);
  void GUIDRV_SPage_SetBus8  (GUI_DEVICE * pDevice, GUI_PORT_API * pHW_API);
  void GUIDRV_SPage_Set1510  (GUI_DEVICE * pDevice);
  void GUIDRV_SPage_Set1512  (GUI_DEVICE * pDevice);
  void GUIDRV_SPage_SetST7591(GUI_DEVICE * pDevice);
  void GUIDRV_SPage_SetUC1611(GUI_DEVICE * pDevice);

  // Obsolete
  void GUIDRV_SPage_SetS1D15 (GUI_DEVICE * pDevice, int Controller);

#endif

#if defined(__cplusplus)
}
#endif

#endif

/*************************** End of file ****************************/


