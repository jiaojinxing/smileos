/********************************************************************************************************* 
** 													
**                                    中国软件开源组织 							
**														
**                                   嵌入式实时操作系统							
**														
**                                       SmileOS(TM)							
**														
**                               Copyright  All Rights Reserved						
**														
**--------------文件信息--------------------------------------------------------------------------------	
**														
** 文   件   名: symbol.c											
**														
** 创   建   人: MakeSymbol 工具										
**														
** 文件创建日期: 2013 年 01 月 31 日						
**														
** 描        述: SmileOS 操作系统符号表. (此文件由 MakeSymbol 工具自动生成, 请勿修改)			
*********************************************************************************************************/	
#include "module/symbol_tool.h"										
														
#define SYMBOL_TABLE_BEGIN symbol_t symbol_tbl[] = { 						
  														
#define SYMBOL_TABLE_END   {0, 0, 0} };									
														
#define SYMBOL_ITEM_FUNC(pcName)                \
    {   #pcName, (char *)pcName,                \
        SYMBOL_TEXT                             \
    },													
														
#define SYMBOL_ITEM_OBJ(pcName)                 \
    {   #pcName, (char *)&pcName,               \
        SYMBOL_DATA                             \
    },													
														
/*********************************************************************************************************	
** 全局对象声明												
*********************************************************************************************************/	
extern int  BKDRHash(); 
extern int  BUTTON_Callback(); 
extern int  BUTTON_CreateEx(); 
extern int  BUTTON_CreateIndirect(); 
extern int  BUTTON_CreateUser(); 
extern int  BUTTON_DrawSkinFlex(); 
extern int  BUTTON_GetSkinFlexProps(); 
extern int  BUTTON_GetUserData(); 
extern int  BUTTON_IsPressed(); 
extern int  BUTTON_LockH(); 
extern int  BUTTON_SetDefaultSkin(); 
extern int  BUTTON_SetDefaultSkinClassic(); 
extern int  BUTTON_SetFocussable(); 
extern int  BUTTON_SetFont(); 
extern int  BUTTON_SetPressed(); 
extern int  BUTTON_SetReactOnLevel(); 
extern int  BUTTON_SetReactOnTouch(); 
extern int  BUTTON_SetSkin(); 
extern int  BUTTON_SetSkinClassic(); 
extern int  BUTTON_SetSkinFlexProps(); 
extern int  BUTTON_SetState(); 
extern int  BUTTON_SetText(); 
extern int  BUTTON_SetTextOffset(); 
extern int  BUTTON_SetUserData(); 
extern int  BUTTON__SkinClassic(); 
extern int  CharInfo18B_ASCII(); 
extern int  CharInfo24B_ASCII(); 
extern int  DIALOG_GetBkColor(); 
extern int  DIALOG_SetBkColor(); 
extern int  FRAMEWIN_Callback(); 
extern int  FRAMEWIN_CreateEx(); 
extern int  FRAMEWIN_CreateIndirect(); 
extern int  FRAMEWIN_CreateUser(); 
extern int  FRAMEWIN_DrawSkinFlex(); 
extern int  FRAMEWIN_GetActive(); 
extern int  FRAMEWIN_GetBarColor(); 
extern int  FRAMEWIN_GetBorderSize(); 
extern int  FRAMEWIN_GetBorderSizeEx(); 
extern int  FRAMEWIN_GetDefaultBarColor(); 
extern int  FRAMEWIN_GetDefaultBorderSize(); 
extern int  FRAMEWIN_GetDefaultClientColor(); 
extern int  FRAMEWIN_GetDefaultFont(); 
extern int  FRAMEWIN_GetDefaultTextColor(); 
extern int  FRAMEWIN_GetDefaultTitleHeight(); 
extern int  FRAMEWIN_GetFont(); 
extern int  FRAMEWIN_GetSkinFlexProps(); 
extern int  FRAMEWIN_GetText(); 
extern int  FRAMEWIN_GetTextAlign(); 
extern int  FRAMEWIN_GetTitleHeight(); 
extern int  FRAMEWIN_GetUserData(); 
extern int  FRAMEWIN_LockH(); 
extern int  FRAMEWIN_OwnerDraw(); 
extern int  FRAMEWIN_SetActive(); 
extern int  FRAMEWIN_SetBarColor(); 
extern int  FRAMEWIN_SetClientColor(); 
extern int  FRAMEWIN_SetDefaultBarColor(); 
extern int  FRAMEWIN_SetDefaultBorderSize(); 
extern int  FRAMEWIN_SetDefaultClientColor(); 
extern int  FRAMEWIN_SetDefaultFont(); 
extern int  FRAMEWIN_SetDefaultSkin(); 
extern int  FRAMEWIN_SetDefaultSkinClassic(); 
extern int  FRAMEWIN_SetDefaultTextAlign(); 
extern int  FRAMEWIN_SetDefaultTextColor(); 
extern int  FRAMEWIN_SetDefaultTitleHeight(); 
extern int  FRAMEWIN_SetMoveable(); 
extern int  FRAMEWIN_SetOwnerDraw(); 
extern int  FRAMEWIN_SetSkin(); 
extern int  FRAMEWIN_SetSkinClassic(); 
extern int  FRAMEWIN_SetSkinFlexProps(); 
extern int  FRAMEWIN_SetText(); 
extern int  FRAMEWIN_SetTextAlign(); 
extern int  FRAMEWIN_SetTextColor(); 
extern int  FRAMEWIN_SetTextColorEx(); 
extern int  FRAMEWIN_SetUserData(); 
extern int  FRAMEWIN__CalcPositions(); 
extern int  FRAMEWIN__CalcTitleHeight(); 
extern int  FRAMEWIN__GetBorderSize(); 
extern int  FRAMEWIN__GetTitleLimits(); 
extern int  FRAMEWIN__SkinClassic(); 
extern int  FRAMEWIN__UpdateButtons(); 
extern int  FRAMEWIN__UpdatePositions(); 
extern int  FontComic24BProp_ASCII(); 
extern int  GL_DispChar(); 
extern int  GL_DrawArc(); 
extern int  GL_DrawBitmap(); 
extern int  GL_DrawCircle(); 
extern int  GL_DrawEllipse(); 
extern int  GL_DrawLine(); 
extern int  GL_DrawLine1(); 
extern int  GL_DrawLine1Ex(); 
extern int  GL_DrawLineEx(); 
extern int  GL_DrawLineTo(); 
extern int  GL_DrawLineToEx(); 
extern int  GL_DrawPoint(); 
extern int  GL_FillCircle(); 
extern int  GL_FillCircleAA_HiRes(); 
extern int  GL_FillEllipse(); 
extern int  GL_FillPolygon(); 
extern int  GL_MoveTo(); 
extern int  GL_SetDefault(); 
extern int  GRAPH_AttachData(); 
extern int  GRAPH_AttachScale(); 
extern int  GRAPH_Callback(); 
extern int  GRAPH_CreateEx(); 
extern int  GRAPH_CreateUser(); 
extern int  GRAPH_DATA_YT_AddValue(); 
extern int  GRAPH_DATA_YT_Clear(); 
extern int  GRAPH_DATA_YT_Create(); 
extern int  GRAPH_DATA_YT_Delete(); 
extern int  GRAPH_DATA_YT_MirrorX(); 
extern int  GRAPH_DATA_YT_SetAlign(); 
extern int  GRAPH_DATA_YT_SetOffY(); 
extern int  GRAPH_DetachData(); 
extern int  GRAPH_DetachScale(); 
extern int  GRAPH_GetScrollValue(); 
extern int  GRAPH_GetUserData(); 
extern int  GRAPH_LockH(); 
extern int  GRAPH_SCALE_Create(); 
extern int  GRAPH_SCALE_Delete(); 
extern int  GRAPH_SCALE_SetFactor(); 
extern int  GRAPH_SCALE_SetFont(); 
extern int  GRAPH_SCALE_SetNumDecs(); 
extern int  GRAPH_SCALE_SetOff(); 
extern int  GRAPH_SCALE_SetPos(); 
extern int  GRAPH_SCALE_SetTextColor(); 
extern int  GRAPH_SCALE_SetTickDist(); 
extern int  GRAPH_SetAutoScrollbar(); 
extern int  GRAPH_SetBorder(); 
extern int  GRAPH_SetColor(); 
extern int  GRAPH_SetGridDistX(); 
extern int  GRAPH_SetGridDistY(); 
extern int  GRAPH_SetGridFixedX(); 
extern int  GRAPH_SetGridOffY(); 
extern int  GRAPH_SetGridVis(); 
extern int  GRAPH_SetLineStyle(); 
extern int  GRAPH_SetLineStyleH(); 
extern int  GRAPH_SetLineStyleV(); 
extern int  GRAPH_SetScrollValue(); 
extern int  GRAPH_SetUserData(); 
extern int  GRAPH_SetUserDraw(); 
extern int  GRAPH_SetVSizeX(); 
extern int  GRAPH_SetVSizeY(); 
extern int  GRAPH__AddValue(); 
extern int  GUIDEMO_AddIntToString(); 
extern int  GUIDEMO_AddStringToString(); 
extern int  GUIDEMO_Automotive(); 
extern int  GUIDEMO_BarGraph(); 
extern int  GUIDEMO_Bitmap(); 
extern int  GUIDEMO_CheckCancel(); 
extern int  GUIDEMO_ClearText(); 
extern int  GUIDEMO_ColorBar(); 
extern int  GUIDEMO_Config(); 
extern int  GUIDEMO_Cursor(); 
extern int  GUIDEMO_Delay(); 
extern int  GUIDEMO_DrawBk(); 
extern int  GUIDEMO_Fading(); 
extern int  GUIDEMO_GetConfFlag(); 
extern int  GUIDEMO_GetTime(); 
extern int  GUIDEMO_Graph(); 
extern int  GUIDEMO_HideControlWin(); 
extern int  GUIDEMO_HideInfoWin(); 
extern int  GUIDEMO_IconView(); 
extern int  GUIDEMO_ImageFlow(); 
extern int  GUIDEMO_Intro(); 
extern int  GUIDEMO_Listview(); 
extern int  GUIDEMO_Main(); 
extern int  GUIDEMO_NotifyStartNext(); 
extern int  GUIDEMO_ShowControlWin(); 
extern int  GUIDEMO_ShowInfo(); 
extern int  GUIDEMO_ShowInfoWin(); 
extern int  GUIDEMO_ShowIntro(); 
extern int  GUIDEMO_Skinning(); 
extern int  GUIDEMO_Speed(); 
extern int  GUIDEMO_Speedometer(); 
extern int  GUIDEMO_TransparentDialog(); 
extern int  GUIDEMO_Treeview(); 
extern int  GUIDEMO_UpdateControlText(); 
extern int  GUIDEMO_VScreen(); 
extern int  GUIDEMO_Wait(); 
extern int  GUIDEMO_WashingMachine(); 
extern int  GUIDEMO_ZoomAndRotate(); 
extern int  GUIDEMO_bm4bpp(); 
extern int  GUIDEV_StaticDevices_C(); 
extern int  GUIDRV_Lin_16_API(); 
extern int  GUIMONO_DispChar(); 
extern int  GUIMONO_GetCharDistX(); 
extern int  GUIMONO_GetFontInfo(); 
extern int  GUIMONO_IsInFont(); 
extern int  GUIPROP_AA4_DispChar(); 
extern int  GUIPROP_AA4_EXT_DispChar(); 
extern int  GUIPROP_AA4_GetCharDistX(); 
extern int  GUIPROP_AA4_GetFontInfo(); 
extern int  GUIPROP_AA4_IsInFont(); 
extern int  GUIPROP_DispChar(); 
extern int  GUIPROP_EXT_DispChar(); 
extern int  GUIPROP_EXT_GetCharDistX(); 
extern int  GUIPROP_EXT_GetCharInfo(); 
extern int  GUIPROP_EXT_GetFontInfo(); 
extern int  GUIPROP_EXT_IsInFont(); 
extern int  GUIPROP_EXT__ClearLine(); 
extern int  GUIPROP_EXT__DispLine(); 
extern int  GUIPROP_EXT__FindChar(); 
extern int  GUIPROP_EXT__SetfpClearLine(); 
extern int  GUIPROP_GetCharDistX(); 
extern int  GUIPROP_GetFontInfo(); 
extern int  GUIPROP_IsInFont(); 
extern int  GUIPROP__FindChar(); 
extern int  GUITASK_CopyContext(); 
extern int  GUITASK_GetpContext(); 
extern int  GUITASK_Init(); 
extern int  GUITASK_SetMaxTask(); 
extern int  GUI_AA_DisableHiRes(); 
extern int  GUI_AA_DrawArc(); 
extern int  GUI_AA_DrawLine(); 
extern int  GUI_AA_EnableHiRes(); 
extern int  GUI_AA_Exit(); 
extern int  GUI_AA_FillCircle(); 
extern int  GUI_AA_FillPolygon(); 
extern int  GUI_AA_GetFactor(); 
extern int  GUI_AA_HiRes2Pixel(); 
extern int  GUI_AA_Init(); 
extern int  GUI_AA_Init_HiRes(); 
extern int  GUI_AA_SetFactor(); 
extern int  GUI_AA__DrawCharAA4(); 
extern int  GUI_ALLOC_AllocInit(); 
extern int  GUI_ALLOC_AllocNoInit(); 
extern int  GUI_ALLOC_AllocZero(); 
extern int  GUI_ALLOC_AssignMemory(); 
extern int  GUI_ALLOC_Free(); 
extern int  GUI_ALLOC_FreeFixedBlock(); 
extern int  GUI_ALLOC_FreePtr(); 
extern int  GUI_ALLOC_FreePtrArray(); 
extern int  GUI_ALLOC_GetFixedBlock(); 
extern int  GUI_ALLOC_GetMaxSize(); 
extern int  GUI_ALLOC_GetNumFreeBlocks(); 
extern int  GUI_ALLOC_GetNumFreeBytes(); 
extern int  GUI_ALLOC_GetNumUsedBlocks(); 
extern int  GUI_ALLOC_GetNumUsedBytes(); 
extern int  GUI_ALLOC_GetSize(); 
extern int  GUI_ALLOC_Lock(); 
extern int  GUI_ALLOC_LockH(); 
extern int  GUI_ALLOC_Realloc(); 
extern int  GUI_ALLOC_RequestSize(); 
extern int  GUI_ALLOC_SetAvBlockSize(); 
extern int  GUI_ALLOC_SetMaxPercentage(); 
extern int  GUI_ALLOC_Unlock(); 
extern int  GUI_ALLOC_UnlockH(); 
extern int  GUI_ALLOC_h2p(); 
extern int  GUI_ALLOC_p2h(); 
extern int  GUI_ARRAY_AddItem(); 
extern int  GUI_ARRAY_Create(); 
extern int  GUI_ARRAY_Delete(); 
extern int  GUI_ARRAY_DeleteItem(); 
extern int  GUI_ARRAY_GetNumItems(); 
extern int  GUI_ARRAY_GethItem(); 
extern int  GUI_ARRAY_GetpItemLocked(); 
extern int  GUI_ARRAY_InsertBlankItem(); 
extern int  GUI_ARRAY_InsertItem(); 
extern int  GUI_ARRAY_ResizeItemLocked(); 
extern int  GUI_ARRAY_SetItem(); 
extern int  GUI_ARRAY__GethItem(); 
extern int  GUI_ARRAY__GetpItemLocked(); 
extern int  GUI_ARRAY__SethItem(); 
extern int  GUI_AddDecMin(); 
extern int  GUI_AddDecShift(); 
extern int  GUI_AddHex(); 
extern int  GUI_AddSign(); 
extern int  GUI_AssignCursorLayer(); 
extern int  GUI_BitmapArrowL(); 
extern int  GUI_BitmapArrowLI(); 
extern int  GUI_BitmapArrowM(); 
extern int  GUI_BitmapArrowMI(); 
extern int  GUI_BitmapArrowS(); 
extern int  GUI_BitmapArrowSI(); 
extern int  GUI_BitmapCrossL(); 
extern int  GUI_BitmapCrossLI(); 
extern int  GUI_BitmapCrossM(); 
extern int  GUI_BitmapCrossMI(); 
extern int  GUI_BitmapCrossS(); 
extern int  GUI_BitmapCrossSI(); 
extern int  GUI_BitmapMethods24(); 
extern int  GUI_BitmapMethods555(); 
extern int  GUI_BitmapMethods565(); 
extern int  GUI_BitmapMethods8888(); 
extern int  GUI_BitmapMethodsM555(); 
extern int  GUI_BitmapMethodsM565(); 
extern int  GUI_BitmapMethodsRLE16(); 
extern int  GUI_BitmapMethodsRLE16Ex(); 
extern int  GUI_BitmapMethodsRLE32(); 
extern int  GUI_BitmapMethodsRLE32Ex(); 
extern int  GUI_BitmapMethodsRLE4(); 
extern int  GUI_BitmapMethodsRLE4Ex(); 
extern int  GUI_BitmapMethodsRLE8(); 
extern int  GUI_BitmapMethodsRLE8Ex(); 
extern int  GUI_BitmapMethodsRLEAlpha(); 
extern int  GUI_BitmapMethodsRLEM16(); 
extern int  GUI_BitmapMethodsRLEM16Ex(); 
extern int  GUI_CURSOR_GetState(); 
extern int  GUI_CURSOR_GetStateEx(); 
extern int  GUI_CURSOR_Hide(); 
extern int  GUI_CURSOR_HideEx(); 
extern int  GUI_CURSOR_Select(); 
extern int  GUI_CURSOR_SelectEx(); 
extern int  GUI_CURSOR_SetBitmap(); 
extern int  GUI_CURSOR_SetBitmapEx(); 
extern int  GUI_CURSOR_SetPosition(); 
extern int  GUI_CURSOR_SetPositionEx(); 
extern int  GUI_CURSOR_Show(); 
extern int  GUI_CURSOR_ShowEx(); 
extern int  GUI_CURSOR__SetCallback(); 
extern int  GUI_CalcColorDist(); 
extern int  GUI_Clear(); 
extern int  GUI_ClearKeyBuffer(); 
extern int  GUI_ClearRect(); 
extern int  GUI_ClearRectEx(); 
extern int  GUI_Color2Index(); 
extern int  GUI_CreateBitmapFromStream(); 
extern int  GUI_CreateBitmapFromStream24(); 
extern int  GUI_CreateBitmapFromStream555(); 
extern int  GUI_CreateBitmapFromStream565(); 
extern int  GUI_CreateBitmapFromStreamAlpha(); 
extern int  GUI_CreateBitmapFromStreamIDX(); 
extern int  GUI_CreateBitmapFromStreamM555(); 
extern int  GUI_CreateBitmapFromStreamM565(); 
extern int  GUI_CreateBitmapFromStreamRLE16(); 
extern int  GUI_CreateBitmapFromStreamRLE32(); 
extern int  GUI_CreateBitmapFromStreamRLE4(); 
extern int  GUI_CreateBitmapFromStreamRLE8(); 
extern int  GUI_CreateBitmapFromStreamRLEAlpha(); 
extern int  GUI_CreateBitmapFromStreamRLEM16(); 
extern int  GUI_CreateDialogBox(); 
extern int  GUI_CursorArrowL(); 
extern int  GUI_CursorArrowLI(); 
extern int  GUI_CursorArrowM(); 
extern int  GUI_CursorArrowMI(); 
extern int  GUI_CursorArrowS(); 
extern int  GUI_CursorArrowSI(); 
extern int  GUI_CursorCrossL(); 
extern int  GUI_CursorCrossLI(); 
extern int  GUI_CursorCrossM(); 
extern int  GUI_CursorCrossMI(); 
extern int  GUI_CursorCrossS(); 
extern int  GUI_CursorCrossSI(); 
extern int  GUI_CursorHeaderM(); 
extern int  GUI_CursorPal(); 
extern int  GUI_CursorPalI(); 
extern int  GUI_DEVICE_Create(); 
extern int  GUI_DEVICE_CreateAndLink(); 
extern int  GUI_DEVICE_Delete(); 
extern int  GUI_DEVICE_Link(); 
extern int  GUI_DEVICE_Unlink(); 
extern int  GUI_DEVICE__GetpDevice(); 
extern int  GUI_DEVICE__GetpDriver(); 
extern int  GUI_DRAW__Draw(); 
extern int  GUI_DRAW__GetXSize(); 
extern int  GUI_DRAW__GetYSize(); 
extern int  GUI_Delay(); 
extern int  GUI_DispChar(); 
extern int  GUI_DispCharAt(); 
extern int  GUI_DispDec(); 
extern int  GUI_DispDecAt(); 
extern int  GUI_DispDecMin(); 
extern int  GUI_DispDecShift(); 
extern int  GUI_DispDecSpace(); 
extern int  GUI_DispFloat(); 
extern int  GUI_DispFloatFix(); 
extern int  GUI_DispFloatMin(); 
extern int  GUI_DispNextLine(); 
extern int  GUI_DispSDec(); 
extern int  GUI_DispSDecShift(); 
extern int  GUI_DispSFloatFix(); 
extern int  GUI_DispSFloatMin(); 
extern int  GUI_DispString(); 
extern int  GUI_DispStringAt(); 
extern int  GUI_DispStringHCenterAt(); 
extern int  GUI_DispStringInRect(); 
extern int  GUI_DispStringInRectMax(); 
extern int  GUI_DispStringInRectWrap(); 
extern int  GUI_DrawArc(); 
extern int  GUI_DrawBitmap(); 
extern int  GUI_DrawBitmapMag(); 
extern int  GUI_DrawCircle(); 
extern int  GUI_DrawEllipse(); 
extern int  GUI_DrawFocusRect(); 
extern int  GUI_DrawGradientH(); 
extern int  GUI_DrawGradientV(); 
extern int  GUI_DrawGraphEx(); 
extern int  GUI_DrawHLine(); 
extern int  GUI_DrawLine(); 
extern int  GUI_DrawLineRel(); 
extern int  GUI_DrawLineTo(); 
extern int  GUI_DrawPixel(); 
extern int  GUI_DrawPoint(); 
extern int  GUI_DrawRect(); 
extern int  GUI_DrawRoundedRect(); 
extern int  GUI_DrawStreamedBitmap(); 
extern int  GUI_DrawStreamedBitmap24Ex(); 
extern int  GUI_DrawStreamedBitmap555Ex(); 
extern int  GUI_DrawStreamedBitmap565Ex(); 
extern int  GUI_DrawStreamedBitmapAuto(); 
extern int  GUI_DrawStreamedBitmapEx(); 
extern int  GUI_DrawStreamedBitmapExAuto(); 
extern int  GUI_DrawStreamedBitmapM555Ex(); 
extern int  GUI_DrawStreamedBitmapM565Ex(); 
extern int  GUI_DrawVLine(); 
extern int  GUI_ENC_APIList_EXT(); 
extern int  GUI_EndDialog(); 
extern int  GUI_Error(); 
extern int  GUI_ErrorOut(); 
extern int  GUI_ErrorOut1(); 
extern int  GUI_ErrorOut2(); 
extern int  GUI_ErrorOut3(); 
extern int  GUI_ErrorOut4(); 
extern int  GUI_Exec(); 
extern int  GUI_Exec1(); 
extern int  GUI_ExecCreatedDialog(); 
extern int  GUI_ExecDialogBox(); 
extern int  GUI_Exit(); 
extern int  GUI_FillCircle(); 
extern int  GUI_FillEllipse(); 
extern int  GUI_FillPolygon(); 
extern int  GUI_FillRect(); 
extern int  GUI_FillRectEx(); 
extern int  GUI_FillRoundedFrame(); 
extern int  GUI_FillRoundedRect(); 
extern int  GUI_Font10S_ASCII(); 
extern int  GUI_Font10S_ASCII_FontProp1(); 
extern int  GUI_Font13ASCII_Prop1(); 
extern int  GUI_Font13B_ASCII(); 
extern int  GUI_Font13B_ASCII_Prop1(); 
extern int  GUI_Font13_1(); 
extern int  GUI_Font13_ASCII(); 
extern int  GUI_Font18(); 
extern int  GUI_Font18B_CharInfo(); 
extern int  GUI_Font18_CharInfo(); 
extern int  GUI_Font18_Prop1(); 
extern int  GUI_Font20_ASCII(); 
extern int  GUI_Font20_ASCII_CharInfo(); 
extern int  GUI_Font20_ASCII_Prop1(); 
extern int  GUI_Font24B_ASCII(); 
extern int  GUI_Font24B_ASCII_CharInfo(); 
extern int  GUI_Font60BD(); 
extern int  GUI_Font60BD_CharInfo(); 
extern int  GUI_Font60BD_Prop1(); 
extern int  GUI_Font60min(); 
extern int  GUI_Font60min_CharInfo(); 
extern int  GUI_Font60min_Prop1(); 
extern int  GUI_Font60min_Prop2(); 
extern int  GUI_Font6x8(); 
extern int  GUI_Font6x9(); 
extern int  GUI_Font8ASCII_Prop(); 
extern int  GUI_Font8_1(); 
extern int  GUI_Font8_ASCII(); 
extern int  GUI_FontComic18B_ASCII(); 
extern int  GUI_FontComic24B_ASCII(); 
extern int  GUI_FontD6x8(); 
extern int  GUI_FontD9_AA4(); 
extern int  GUI_FontD9_AA4_CharInfo(); 
extern int  GUI_FontD9_AA4_Prop1(); 
extern int  GUI_FontRounded16(); 
extern int  GUI_FontRounded16_CharInfo(); 
extern int  GUI_FontRounded16_Prop1(); 
extern int  GUI_FontRounded22(); 
extern int  GUI_FontRounded22_CharInfo(); 
extern int  GUI_FontRounded22_Prop1(); 
extern int  GUI_FontSouvenir18(); 
extern int  GUI_FontSouvenir18_CharInfo(); 
extern int  GUI_FontSouvenir18_Prop1(); 
extern int  GUI_GetBitmapPixelIndex(); 
extern int  GUI_GetBitmapPixelIndexEx(); 
extern int  GUI_GetBitsPerPixelEx(); 
extern int  GUI_GetBkColor(); 
extern int  GUI_GetBkColorIndex(); 
extern int  GUI_GetCharDistX(); 
extern int  GUI_GetCharDistXEx(); 
extern int  GUI_GetClientRect(); 
extern int  GUI_GetColor(); 
extern int  GUI_GetColorIndex(); 
extern int  GUI_GetCursorLayer(); 
extern int  GUI_GetDialogStatusPtr(); 
extern int  GUI_GetFontDistY(); 
extern int  GUI_GetFontSizeY(); 
extern int  GUI_GetKey(); 
extern int  GUI_GetKeyState(); 
extern int  GUI_GetLayerPosEx(); 
extern int  GUI_GetLineStyle(); 
extern int  GUI_GetOrg(); 
extern int  GUI_GetPenShape(); 
extern int  GUI_GetPenSize(); 
extern int  GUI_GetPixelIndex(); 
extern int  GUI_GetStreamedBitmapInfo(); 
extern int  GUI_GetStreamedBitmapInfoEx(); 
extern int  GUI_GetStringDistX(); 
extern int  GUI_GetTime(); 
extern int  GUI_GetVersionString(); 
extern int  GUI_GetYAdjust(); 
extern int  GUI_GetYDistOfFont(); 
extern int  GUI_GetYSizeOfFont(); 
extern int  GUI_GetpfColor2IndexEx(); 
extern int  GUI_GetpfIndex2ColorEx(); 
extern int  GUI_GotoX(); 
extern int  GUI_GotoXY(); 
extern int  GUI_GotoY(); 
extern int  GUI_Init(); 
extern int  GUI_JPEG_Draw(); 
extern int  GUI_JPEG_DrawEx(); 
extern int  GUI_JPEG_GetInfo(); 
extern int  GUI_JPEG_GetInfoEx(); 
extern int  GUI_JPEG__AllocBandingCoeffBuffer(); 
extern int  GUI_JPEG__DecodeLine(); 
extern int  GUI_JPEG__DecodeProgressiveBanding(); 
extern int  GUI_JPEG__Free(); 
extern int  GUI_JPEG__GetColorGray(); 
extern int  GUI_JPEG__GetColorRGB(); 
extern int  GUI_JPEG__GetData(); 
extern int  GUI_JPEG__GetInfoEx(); 
extern int  GUI_JPEG__InitDraw(); 
extern int  GUI_JPEG__ReadUntilSOF(); 
extern int  GUI_JPEG__SetNextBand(); 
extern int  GUI_JPEG__SkipLine(); 
extern int  GUI_Lock(); 
extern int  GUI_Log(); 
extern int  GUI_Log1(); 
extern int  GUI_Log2(); 
extern int  GUI_Log3(); 
extern int  GUI_Log4(); 
extern int  GUI_Long2Len(); 
extern int  GUI_MEASDEV_ClearRect(); 
extern int  GUI_MEASDEV_Create(); 
extern int  GUI_MEASDEV_Delete(); 
extern int  GUI_MEASDEV_GetRect(); 
extern int  GUI_MEASDEV_Select(); 
extern int  GUI_MEMDEV_Clear(); 
extern int  GUI_MEMDEV_CopyFromLCD(); 
extern int  GUI_MEMDEV_CopyToLCD(); 
extern int  GUI_MEMDEV_CopyToLCDAt(); 
extern int  GUI_MEMDEV_Create(); 
extern int  GUI_MEMDEV_CreateAuto(); 
extern int  GUI_MEMDEV_CreateEx(); 
extern int  GUI_MEMDEV_CreateFixed(); 
extern int  GUI_MEMDEV_CreateStatic(); 
extern int  GUI_MEMDEV_DEVICE_16(); 
extern int  GUI_MEMDEV_DEVICE_32(); 
extern int  GUI_MEMDEV_Delete(); 
extern int  GUI_MEMDEV_DeleteAuto(); 
extern int  GUI_MEMDEV_Draw(); 
extern int  GUI_MEMDEV_DrawAuto(); 
extern int  GUI_MEMDEV_DrawPerspectiveX(); 
extern int  GUI_MEMDEV_FadeDevices(); 
extern int  GUI_MEMDEV_GetBitsPerPixel(); 
extern int  GUI_MEMDEV_GetDataPtr(); 
extern int  GUI_MEMDEV_GetStaticDevice(); 
extern int  GUI_MEMDEV_GetWindowDevice(); 
extern int  GUI_MEMDEV_GetXPos(); 
extern int  GUI_MEMDEV_GetXSize(); 
extern int  GUI_MEMDEV_GetYPos(); 
extern int  GUI_MEMDEV_GetYSize(); 
extern int  GUI_MEMDEV_MarkDirty(); 
extern int  GUI_MEMDEV_Paint1Static(); 
extern int  GUI_MEMDEV_ReduceYSize(); 
extern int  GUI_MEMDEV_Rotate(); 
extern int  GUI_MEMDEV_RotateHQ(); 
extern int  GUI_MEMDEV_Select(); 
extern int  GUI_MEMDEV_SetAnimationCallback(); 
extern int  GUI_MEMDEV_SetOrg(); 
extern int  GUI_MEMDEV_Write(); 
extern int  GUI_MEMDEV_WriteAt(); 
extern int  GUI_MEMDEV__CreateFixed(); 
extern int  GUI_MEMDEV__DrawSizedAt(); 
extern int  GUI_MEMDEV__FadeDevice(); 
extern int  GUI_MEMDEV__FadeDeviceEx(); 
extern int  GUI_MEMDEV__Rotate(); 
extern int  GUI_MEMDEV__WriteToActiveAlpha(); 
extern int  GUI_MEMDEV__WriteToActiveAt(); 
extern int  GUI_MEMDEV__WriteToActiveOpaque(); 
extern int  GUI_MEMDEV__XY2PTR(); 
extern int  GUI_MEMDEV__XY2PTREx(); 
extern int  GUI_MULTIBUF_Begin(); 
extern int  GUI_MULTIBUF_BeginEx(); 
extern int  GUI_MULTIBUF_Config(); 
extern int  GUI_MULTIBUF_ConfigEx(); 
extern int  GUI_MULTIBUF_Confirm(); 
extern int  GUI_MULTIBUF_ConfirmEx(); 
extern int  GUI_MULTIBUF_End(); 
extern int  GUI_MULTIBUF_EndEx(); 
extern int  GUI_MULTIBUF_GetNumBuffers(); 
extern int  GUI_MULTIBUF_GetNumBuffersEx(); 
extern int  GUI_MULTIBUF_UseSingleBuffer(); 
extern int  GUI_MergeRect(); 
extern int  GUI_MoveRect(); 
extern int  GUI_MoveRel(); 
extern int  GUI_MoveTo(); 
extern int  GUI_PID_GetCurrentState(); 
extern int  GUI_PID_GetState(); 
extern int  GUI_PID_IsEmpty(); 
extern int  GUI_PID_IsPressed(); 
extern int  GUI_PID_StoreState(); 
extern int  GUI_PixelsHeaderM(); 
extern int  GUI_Pixels_ArrowL(); 
extern int  GUI_Pixels_ArrowM(); 
extern int  GUI_Pixels_ArrowS(); 
extern int  GUI_Pixels_CrossL(); 
extern int  GUI_Pixels_CrossM(); 
extern int  GUI_Pixels_CrossS(); 
extern int  GUI_PollKeyMsg(); 
extern int  GUI_Pow10(); 
extern int  GUI_ReadRect(); 
extern int  GUI_ReadRectEx(); 
extern int  GUI_RectsIntersect(); 
extern int  GUI_RestoreContext(); 
extern int  GUI_RotatePolygon(); 
extern int  GUI_SIF_APIList_Prop_Frm(); 
extern int  GUI_SIF_CreateFont(); 
extern int  GUI_SIF_DeleteFont(); 
extern int  GUI_SIF__ClearLine_ExtFrm(); 
extern int  GUI_SIF__GetCharDistX(); 
extern int  GUI_SIF__GetCharDistX_ExtFrm(); 
extern int  GUI_SIF__GetCharInfo_ExtFrm(); 
extern int  GUI_SIF__GetFontInfo(); 
extern int  GUI_SIF__GetFontInfo_ExtFrm(); 
extern int  GUI_SIF__GetNumCharAreas(); 
extern int  GUI_SIF__GetpCharInfo(); 
extern int  GUI_SIF__IsInFont(); 
extern int  GUI_SIF__IsInFont_ExtFrm(); 
extern int  GUI_SPRITE_Create(); 
extern int  GUI_SPRITE_CreateEx(); 
extern int  GUI_SPRITE_Delete(); 
extern int  GUI_SPRITE_GetState(); 
extern int  GUI_SPRITE_Hide(); 
extern int  GUI_SPRITE_SetBitmap(); 
extern int  GUI_SPRITE_SetBitmapAndPosition(); 
extern int  GUI_SPRITE_SetPosition(); 
extern int  GUI_SPRITE_Show(); 
extern int  GUI_SPRITE__CreateEx(); 
extern int  GUI_SPRITE__SetCallback(); 
extern int  GUI_SaveContext_W(); 
extern int  GUI_SelectLCD(); 
extern int  GUI_SelectLayer(); 
extern int  GUI_SendKeyMsg(); 
extern int  GUI_SetAlpha(); 
extern int  GUI_SetBkColor(); 
extern int  GUI_SetBkColorIndex(); 
extern int  GUI_SetClipRect(); 
extern int  GUI_SetColor(); 
extern int  GUI_SetColorIndex(); 
extern int  GUI_SetDefault(); 
extern int  GUI_SetDefaultFont(); 
extern int  GUI_SetDialogStatusPtr(); 
extern int  GUI_SetDrawMode(); 
extern int  GUI_SetFont(); 
extern int  GUI_SetLayerAlphaEx(); 
extern int  GUI_SetLayerPosEx(); 
extern int  GUI_SetLayerSizeEx(); 
extern int  GUI_SetLayerVisEx(); 
extern int  GUI_SetLineStyle(); 
extern int  GUI_SetOrg(); 
extern int  GUI_SetPenShape(); 
extern int  GUI_SetPenSize(); 
extern int  GUI_SetSignalEventFunc(); 
extern int  GUI_SetStreamedBitmapHook(); 
extern int  GUI_SetTextAlign(); 
extern int  GUI_SetTextMode(); 
extern int  GUI_SetWaitEventFunc(); 
extern int  GUI_SetWaitEventTimedFunc(); 
extern int  GUI_SignalEvent(); 
extern int  GUI_StoreKey(); 
extern int  GUI_StoreKeyMsg(); 
extern int  GUI_TIMER_Create(); 
extern int  GUI_TIMER_Delete(); 
extern int  GUI_TIMER_Exec(); 
extern int  GUI_TIMER_GetFlag(); 
extern int  GUI_TIMER_Restart(); 
extern int  GUI_TIMER_SetDelay(); 
extern int  GUI_TIMER_SetPeriod(); 
extern int  GUI_TIMER_SetTime(); 
extern int  GUI_TIMER__GetFirstTimer(); 
extern int  GUI_TIMER__GetNextTimer(); 
extern int  GUI_TIMER__GetNextTimerLin(); 
extern int  GUI_TIMER__GetPeriod(); 
extern int  GUI_TIMER__IsActive(); 
extern int  GUI_UC_Encode(); 
extern int  GUI_UC_GetCharCode(); 
extern int  GUI_UC_GetCharSize(); 
extern int  GUI_UC_None(); 
extern int  GUI_UC_SetEncodeNone(); 
extern int  GUI_UC__CalcSizeOfChar(); 
extern int  GUI_UC__GetCharCodeInc(); 
extern int  GUI_UC__NumBytes2NumChars(); 
extern int  GUI_UC__NumChars2NumBytes(); 
extern int  GUI_USAGE_AddRect(); 
extern int  GUI_USAGE_BM_Create(); 
extern int  GUI_USAGE_DecUseCnt(); 
extern int  GUI_USAGE_Select(); 
extern int  GUI_Unlock(); 
extern int  GUI_WaitEvent(); 
extern int  GUI_Warn(); 
extern int  GUI_Warn1(); 
extern int  GUI_Warn2(); 
extern int  GUI_Warn3(); 
extern int  GUI_Warn4(); 
extern int  GUI_WrapGetNumLines(); 
extern int  GUI_X_Config(); 
extern int  GUI_X_Delay(); 
extern int  GUI_X_ErrorOut(); 
extern int  GUI_X_ExecIdle(); 
extern int  GUI_X_GetTaskId(); 
extern int  GUI_X_GetTime(); 
extern int  GUI_X_Init(); 
extern int  GUI_X_InitOS(); 
extern int  GUI_X_Lock(); 
extern int  GUI_X_Log(); 
extern int  GUI_X_Unlock(); 
extern int  GUI_X_Warn(); 
extern int  GUI__AddSpaceHex(); 
extern int  GUI__AllocAlphaBuffer(); 
extern int  GUI__AlphaSuppressMixing(); 
extern int  GUI__CalcTextRect(); 
extern int  GUI__CompactPixelIndices(); 
extern int  GUI__CompactPixelIndicesEx(); 
extern int  GUI__Config(); 
extern int  GUI__CosHQ(); 
extern int  GUI__CreateBitmapFromStream(); 
extern int  GUI__DispLine(); 
extern int  GUI__DispStringInRect(); 
extern int  GUI__DivideRound32(); 
extern int  GUI__DoAlphaBlending(); 
extern int  GUI__DrawBitmap16bpp(); 
extern int  GUI__DrawTwinArc2(); 
extern int  GUI__DrawTwinArc4(); 
extern int  GUI__ExpandPixelIndices(); 
extern int  GUI__ExpandPixelIndicesEx(); 
extern int  GUI__FillTrippleArc(); 
extern int  GUI__GetAlphaBuffer(); 
extern int  GUI__GetFontSizeY(); 
extern int  GUI__GetLineDistX(); 
extern int  GUI__GetLineNumChars(); 
extern int  GUI__GetNumChars(); 
extern int  GUI__GetOrg(); 
extern int  GUI__GetOverlap(); 
extern int  GUI__HandleEOLine(); 
extern int  GUI__IntersectRect(); 
extern int  GUI__IntersectRects(); 
extern int  GUI__ManageCache(); 
extern int  GUI__ManageCacheEx(); 
extern int  GUI__MixColors(); 
extern int  GUI__Read16(); 
extern int  GUI__Read32(); 
extern int  GUI__ReadHeaderFromStream(); 
extern int  GUI__ReduceRect(); 
extern int  GUI__RegisterExit(); 
extern int  GUI__SetAlphaBufferSize(); 
extern int  GUI__SetPixelAlpha(); 
extern int  GUI__SetText(); 
extern int  GUI__SinHQ(); 
extern int  GUI__WrapGetNumBytesToNextLine(); 
extern int  GUI__WrapGetNumCharsDisp(); 
extern int  GUI__WrapGetNumCharsToNextLine(); 
extern int  GUI__aConvert_255_31(); 
extern int  GUI__aConvert_255_63(); 
extern int  GUI__aConvert_31_255(); 
extern int  GUI__aConvert_63_255(); 
extern int  GUI__memset(); 
extern int  GUI__memset16(); 
extern int  GUI__sqrt32(); 
extern int  GUI__strcmp(); 
extern int  GUI__strcmp_hp(); 
extern int  GUI__strlen(); 
extern int  GUI_cos(); 
extern int  GUI_sin(); 
extern int  HEADER_AddItem(); 
extern int  HEADER_Callback(); 
extern int  HEADER_Create(); 
extern int  HEADER_CreateEx(); 
extern int  HEADER_CreateUser(); 
extern int  HEADER_DeleteItem(); 
extern int  HEADER_DrawSkinFlex(); 
extern int  HEADER_GetHeight(); 
extern int  HEADER_GetItemWidth(); 
extern int  HEADER_GetNumItems(); 
extern int  HEADER_GetSel(); 
extern int  HEADER_GetSkinFlexProps(); 
extern int  HEADER_GetUserData(); 
extern int  HEADER_LockH(); 
extern int  HEADER_SetDefaultSkin(); 
extern int  HEADER_SetDefaultSkinClassic(); 
extern int  HEADER_SetDirIndicator(); 
extern int  HEADER_SetDragLimit(); 
extern int  HEADER_SetFont(); 
extern int  HEADER_SetHeight(); 
extern int  HEADER_SetItemText(); 
extern int  HEADER_SetItemWidth(); 
extern int  HEADER_SetScrollPos(); 
extern int  HEADER_SetSkin(); 
extern int  HEADER_SetSkinClassic(); 
extern int  HEADER_SetSkinFlexProps(); 
extern int  HEADER_SetTextAlign(); 
extern int  HEADER_SetUserData(); 
extern int  HEADER__SkinClassic(); 
extern int  ICONVIEW_AddBitmapItem(); 
extern int  ICONVIEW_AddStreamedBitmapItem(); 
extern int  ICONVIEW_Callback(); 
extern int  ICONVIEW_CreateEx(); 
extern int  ICONVIEW_CreateIndirect(); 
extern int  ICONVIEW_CreateUser(); 
extern int  ICONVIEW_DeleteItem(); 
extern int  ICONVIEW_GetItemText(); 
extern int  ICONVIEW_GetItemUserData(); 
extern int  ICONVIEW_GetNumItems(); 
extern int  ICONVIEW_GetSel(); 
extern int  ICONVIEW_GetUserData(); 
extern int  ICONVIEW_InsertBitmapItem(); 
extern int  ICONVIEW_InsertStreamedBitmapItem(); 
extern int  ICONVIEW_LockH(); 
extern int  ICONVIEW_SetBitmapItem(); 
extern int  ICONVIEW_SetBkColor(); 
extern int  ICONVIEW_SetFont(); 
extern int  ICONVIEW_SetFrame(); 
extern int  ICONVIEW_SetItemText(); 
extern int  ICONVIEW_SetItemUserData(); 
extern int  ICONVIEW_SetSel(); 
extern int  ICONVIEW_SetSpace(); 
extern int  ICONVIEW_SetStreamedBitmapItem(); 
extern int  ICONVIEW_SetTextAlign(); 
extern int  ICONVIEW_SetTextColor(); 
extern int  ICONVIEW_SetUserData(); 
extern int  LCD_AA_MixColors(); 
extern int  LCD_AA_SetAndMask(); 
extern int  LCD_API_ColorConv_0(); 
extern int  LCD_API_ColorConv_565(); 
extern int  LCD_API_ColorConv_888(); 
extern int  LCD_API_ColorConv_8888(); 
extern int  LCD_Color2Index(); 
extern int  LCD_DrawBitmap(); 
extern int  LCD_DrawHLine(); 
extern int  LCD_DrawPixel(); 
extern int  LCD_DrawVLine(); 
extern int  LCD_FillRect(); 
extern int  LCD_GetBitsPerPixel(); 
extern int  LCD_GetBitsPerPixelEx(); 
extern int  LCD_GetBitsPerPixelMax(); 
extern int  LCD_GetBkColorIndex(); 
extern int  LCD_GetColorIndex(); 
extern int  LCD_GetDevCap(); 
extern int  LCD_GetDevCapEx(); 
extern int  LCD_GetMirrorX(); 
extern int  LCD_GetMirrorXEx(); 
extern int  LCD_GetMirrorY(); 
extern int  LCD_GetMirrorYEx(); 
extern int  LCD_GetNextPixelAPI(); 
extern int  LCD_GetNumColors(); 
extern int  LCD_GetNumColorsEx(); 
extern int  LCD_GetNumLayers(); 
extern int  LCD_GetPalette(); 
extern int  LCD_GetPaletteEx(); 
extern int  LCD_GetPixelColor(); 
extern int  LCD_GetPixelIndex(); 
extern int  LCD_GetPos(); 
extern int  LCD_GetPosEx(); 
extern int  LCD_GetSwapXY(); 
extern int  LCD_GetSwapXYEx(); 
extern int  LCD_GetVXSize(); 
extern int  LCD_GetVXSizeEx(); 
extern int  LCD_GetVXSizeMax(); 
extern int  LCD_GetVYSize(); 
extern int  LCD_GetVYSizeEx(); 
extern int  LCD_GetVYSizeMax(); 
extern int  LCD_GetXMag(); 
extern int  LCD_GetXMagEx(); 
extern int  LCD_GetXSize(); 
extern int  LCD_GetXSizeEx(); 
extern int  LCD_GetXSizeMax(); 
extern int  LCD_GetYMag(); 
extern int  LCD_GetYMagEx(); 
extern int  LCD_GetYSize(); 
extern int  LCD_GetYSizeEx(); 
extern int  LCD_GetYSizeMax(); 
extern int  LCD_GetpPalConvTable(); 
extern int  LCD_GetpPalConvTableUncached(); 
extern int  LCD_GetpfColor2IndexEx(); 
extern int  LCD_GetpfIndex2ColorEx(); 
extern int  LCD_Index2Color(); 
extern int  LCD_Index2Color_555(); 
extern int  LCD_Index2Color_565(); 
extern int  LCD_Index2Color_888(); 
extern int  LCD_Index2Color_8888(); 
extern int  LCD_Index2Color_M555(); 
extern int  LCD_Index2Color_M565(); 
extern int  LCD_Init(); 
extern int  LCD_InitColors(); 
extern int  LCD_InitLUT(); 
extern int  LCD_ReadRect(); 
extern int  LCD_ReadRectNoClip(); 
extern int  LCD_Refresh(); 
extern int  LCD_RefreshEx(); 
extern int  LCD_SelectLCD(); 
extern int  LCD_SetAlpha(); 
extern int  LCD_SetAlphaEx(); 
extern int  LCD_SetAlphaMode(); 
extern int  LCD_SetAlphaModeEx(); 
extern int  LCD_SetBkColor(); 
extern int  LCD_SetBkColorIndex(); 
extern int  LCD_SetChroma(); 
extern int  LCD_SetChromaEx(); 
extern int  LCD_SetChromaMode(); 
extern int  LCD_SetChromaModeEx(); 
extern int  LCD_SetClipRectEx(); 
extern int  LCD_SetClipRectMax(); 
extern int  LCD_SetColor(); 
extern int  LCD_SetColorIndex(); 
extern int  LCD_SetDevFunc(); 
extern int  LCD_SetDrawMode(); 
extern int  LCD_SetLUT(); 
extern int  LCD_SetLUTEntry(); 
extern int  LCD_SetLUTEntryEx(); 
extern int  LCD_SetLUTEx(); 
extern int  LCD_SetMaxNumColors(); 
extern int  LCD_SetPixelAA(); 
extern int  LCD_SetPixelAA_NoTrans(); 
extern int  LCD_SetPixelAA_Xor(); 
extern int  LCD_SetPixelIndex(); 
extern int  LCD_SetPos(); 
extern int  LCD_SetPosEx(); 
extern int  LCD_SetSize(); 
extern int  LCD_SetSizeEx(); 
extern int  LCD_SetVRAMAddr(); 
extern int  LCD_SetVRAMAddrEx(); 
extern int  LCD_SetVSize(); 
extern int  LCD_SetVSizeEx(); 
extern int  LCD_SetVis(); 
extern int  LCD_SetVisEx(); 
extern int  LCD_UpdateColorIndices(); 
extern int  LCD_X_Config(); 
extern int  LCD_X_DisplayDriver(); 
extern int  LCD__DrawBitmap_1bpp(); 
extern int  LCD__GetBPP(); 
extern int  LCD__GetBPPDevice(); 
extern int  LCD__RLE16_SetFunc(); 
extern int  LCD__RLE32_SetFunc(); 
extern int  LCD__RLE4_SetFunc(); 
extern int  LCD__RLE8_SetFunc(); 
extern int  LCD_aMirror(); 
extern int  LISTVIEW_AddColumn(); 
extern int  LISTVIEW_AddRow(); 
extern int  LISTVIEW_Callback(); 
extern int  LISTVIEW_CompareDec(); 
extern int  LISTVIEW_CompareText(); 
extern int  LISTVIEW_CreateEx(); 
extern int  LISTVIEW_CreateUser(); 
extern int  LISTVIEW_DecSel(); 
extern int  LISTVIEW_DeleteRow(); 
extern int  LISTVIEW_DisableSort(); 
extern int  LISTVIEW_EnableSort(); 
extern int  LISTVIEW_GetBkColor(); 
extern int  LISTVIEW_GetHeader(); 
extern int  LISTVIEW_GetNumColumns(); 
extern int  LISTVIEW_GetNumRows(); 
extern int  LISTVIEW_GetSelUnsorted(); 
extern int  LISTVIEW_GetTextColor(); 
extern int  LISTVIEW_GetUserData(); 
extern int  LISTVIEW_IncSel(); 
extern int  LISTVIEW_InsertRow(); 
extern int  LISTVIEW_LockH(); 
extern int  LISTVIEW_SetAutoScrollH(); 
extern int  LISTVIEW_SetAutoScrollV(); 
extern int  LISTVIEW_SetCompareFunc(); 
extern int  LISTVIEW_SetGridVis(); 
extern int  LISTVIEW_SetHeaderHeight(); 
extern int  LISTVIEW_SetItemBkColor(); 
extern int  LISTVIEW_SetItemTextColor(); 
extern int  LISTVIEW_SetLBorder(); 
extern int  LISTVIEW_SetRBorder(); 
extern int  LISTVIEW_SetSel(); 
extern int  LISTVIEW_SetSelUnsorted(); 
extern int  LISTVIEW_SetSort(); 
extern int  LISTVIEW_SetUserData(); 
extern int  LISTVIEW__GetNumColumns(); 
extern int  LISTVIEW__GetNumRows(); 
extern int  LISTVIEW__GetRowDistY(); 
extern int  LISTVIEW__GetSel(); 
extern int  LISTVIEW__InvalidateInsideArea(); 
extern int  LISTVIEW__InvalidateRow(); 
extern int  LISTVIEW__InvalidateRowAndBelow(); 
extern int  LISTVIEW__MoveSel(); 
extern int  LISTVIEW__SetSel(); 
extern int  LISTVIEW__UpdateScrollParas(); 
extern int  LISTVIEW__UpdateScrollPos(); 
extern int  MIN(); 
extern int  MainTask(); 
extern int  PROGBAR_Callback(); 
extern int  PROGBAR_CreateEx(); 
extern int  PROGBAR_CreateIndirect(); 
extern int  PROGBAR_CreateUser(); 
extern int  PROGBAR_DrawSkinFlex(); 
extern int  PROGBAR_GetSkinFlexProps(); 
extern int  PROGBAR_GetUserData(); 
extern int  PROGBAR_LockH(); 
extern int  PROGBAR_SetDefaultSkin(); 
extern int  PROGBAR_SetDefaultSkinClassic(); 
extern int  PROGBAR_SetFont(); 
extern int  PROGBAR_SetMinMax(); 
extern int  PROGBAR_SetSkin(); 
extern int  PROGBAR_SetSkinClassic(); 
extern int  PROGBAR_SetSkinFlexProps(); 
extern int  PROGBAR_SetTextAlign(); 
extern int  PROGBAR_SetUserData(); 
extern int  PROGBAR_SetValue(); 
extern int  PROGBAR__GetTextLocked(); 
extern int  PROGBAR__GetTextRect(); 
extern int  PROGBAR__SkinClassic(); 
extern int  PROGBAR__Value2Pos(); 
extern int  SCROLLBAR_AddValue(); 
extern int  SCROLLBAR_Callback(); 
extern int  SCROLLBAR_Create(); 
extern int  SCROLLBAR_CreateAttached(); 
extern int  SCROLLBAR_CreateEx(); 
extern int  SCROLLBAR_CreateUser(); 
extern int  SCROLLBAR_Dec(); 
extern int  SCROLLBAR_DrawSkinFlex(); 
extern int  SCROLLBAR_GetSkinFlexProps(); 
extern int  SCROLLBAR_GetUserData(); 
extern int  SCROLLBAR_GetValue(); 
extern int  SCROLLBAR_Inc(); 
extern int  SCROLLBAR_LockH(); 
extern int  SCROLLBAR_SetDefaultSkin(); 
extern int  SCROLLBAR_SetDefaultSkinClassic(); 
extern int  SCROLLBAR_SetNumItems(); 
extern int  SCROLLBAR_SetPageSize(); 
extern int  SCROLLBAR_SetSkin(); 
extern int  SCROLLBAR_SetSkinClassic(); 
extern int  SCROLLBAR_SetSkinFlexProps(); 
extern int  SCROLLBAR_SetState(); 
extern int  SCROLLBAR_SetUserData(); 
extern int  SCROLLBAR_SetValue(); 
extern int  SCROLLBAR__InvalidatePartner(); 
extern int  SCROLLBAR__Rect2VRect(); 
extern int  SCROLLBAR__SkinClassic(); 
extern int  SLIDER_Callback(); 
extern int  SLIDER_CreateEx(); 
extern int  SLIDER_CreateIndirect(); 
extern int  SLIDER_CreateUser(); 
extern int  SLIDER_Dec(); 
extern int  SLIDER_DrawSkinFlex(); 
extern int  SLIDER_GetSkinFlexProps(); 
extern int  SLIDER_GetUserData(); 
extern int  SLIDER_GetValue(); 
extern int  SLIDER_Inc(); 
extern int  SLIDER_LockH(); 
extern int  SLIDER_SetDefaultSkin(); 
extern int  SLIDER_SetDefaultSkinClassic(); 
extern int  SLIDER_SetNumTicks(); 
extern int  SLIDER_SetRange(); 
extern int  SLIDER_SetSkin(); 
extern int  SLIDER_SetSkinClassic(); 
extern int  SLIDER_SetSkinFlexProps(); 
extern int  SLIDER_SetUserData(); 
extern int  SLIDER_SetValue(); 
extern int  SLIDER_SetWidth(); 
extern int  SLIDER__SkinClassic(); 
extern int  TEXT_Callback(); 
extern int  TEXT_CreateEx(); 
extern int  TEXT_CreateIndirect(); 
extern int  TEXT_CreateUser(); 
extern int  TEXT_GetDefaultFont(); 
extern int  TEXT_GetNumLines(); 
extern int  TEXT_GetUserData(); 
extern int  TEXT_LockH(); 
extern int  TEXT_SetDefaultFont(); 
extern int  TEXT_SetDefaultTextColor(); 
extern int  TEXT_SetDefaultWrapMode(); 
extern int  TEXT_SetFont(); 
extern int  TEXT_SetText(); 
extern int  TEXT_SetTextAlign(); 
extern int  TEXT_SetTextColor(); 
extern int  TEXT_SetUserData(); 
extern int  TEXT_SetWrapMode(); 
extern int  TREEVIEW_AttachItem(); 
extern int  TREEVIEW_Callback(); 
extern int  TREEVIEW_CreateEx(); 
extern int  TREEVIEW_CreateIndirect(); 
extern int  TREEVIEW_CreateUser(); 
extern int  TREEVIEW_DecSel(); 
extern int  TREEVIEW_GetItem(); 
extern int  TREEVIEW_GetSel(); 
extern int  TREEVIEW_GetUserData(); 
extern int  TREEVIEW_ITEM_Collapse(); 
extern int  TREEVIEW_ITEM_CollapseAll(); 
extern int  TREEVIEW_ITEM_Create(); 
extern int  TREEVIEW_ITEM_Delete(); 
extern int  TREEVIEW_ITEM_Detach(); 
extern int  TREEVIEW_ITEM_Expand(); 
extern int  TREEVIEW_ITEM_ExpandAll(); 
extern int  TREEVIEW_ITEM_GetInfo(); 
extern int  TREEVIEW_ITEM_GetText(); 
extern int  TREEVIEW_ITEM_GetUserData(); 
extern int  TREEVIEW_ITEM_SetImage(); 
extern int  TREEVIEW_ITEM_SetText(); 
extern int  TREEVIEW_ITEM_SetUserData(); 
extern int  TREEVIEW_IncSel(); 
extern int  TREEVIEW_InsertItem(); 
extern int  TREEVIEW_LockH(); 
extern int  TREEVIEW_OwnerDraw(); 
extern int  TREEVIEW_SetAutoScrollH(); 
extern int  TREEVIEW_SetAutoScrollV(); 
extern int  TREEVIEW_SetBitmapOffset(); 
extern int  TREEVIEW_SetBkColor(); 
extern int  TREEVIEW_SetFont(); 
extern int  TREEVIEW_SetHasLines(); 
extern int  TREEVIEW_SetImage(); 
extern int  TREEVIEW_SetIndent(); 
extern int  TREEVIEW_SetLineColor(); 
extern int  TREEVIEW_SetOwnerDraw(); 
extern int  TREEVIEW_SetSel(); 
extern int  TREEVIEW_SetSelMode(); 
extern int  TREEVIEW_SetTextColor(); 
extern int  TREEVIEW_SetTextIndent(); 
extern int  TREEVIEW_SetUserData(); 
extern int  TREEVIEW__bmClosed(); 
extern int  TREEVIEW__bmLeaf(); 
extern int  TREEVIEW__bmMinus(); 
extern int  TREEVIEW__bmOpen(); 
extern int  TREEVIEW__bmPlus(); 
extern int  WIDGET_AndState(); 
extern int  WIDGET_EFFECT_3D_DrawUp(); 
extern int  WIDGET_EFFECT_3D_GetColor(); 
extern int  WIDGET_EFFECT_3D_GetNumColors(); 
extern int  WIDGET_EFFECT_3D_SetColor(); 
extern int  WIDGET_EFFECT_Simple_GetColor(); 
extern int  WIDGET_EFFECT_Simple_GetNumColors(); 
extern int  WIDGET_EFFECT_Simple_SetColor(); 
extern int  WIDGET_Effect_3D(); 
extern int  WIDGET_Effect_None(); 
extern int  WIDGET_Effect_Simple(); 
extern int  WIDGET_GetDefaultEffect(); 
extern int  WIDGET_GetState(); 
extern int  WIDGET_HandleActive(); 
extern int  WIDGET_OrState(); 
extern int  WIDGET_SetDefaultEffect(); 
extern int  WIDGET_SetEffect(); 
extern int  WIDGET_SetState(); 
extern int  WIDGET__DrawFocusRect(); 
extern int  WIDGET__DrawHLine(); 
extern int  WIDGET__DrawTriangle(); 
extern int  WIDGET__DrawVLine(); 
extern int  WIDGET__EFFECT_DrawDown(); 
extern int  WIDGET__EFFECT_DrawDownRect(); 
extern int  WIDGET__EFFECT_DrawUpRect(); 
extern int  WIDGET__FillRectEx(); 
extern int  WIDGET__FillStringInRect(); 
extern int  WIDGET__GetBkColor(); 
extern int  WIDGET__GetClientRect(); 
extern int  WIDGET__GetInsideRect(); 
extern int  WIDGET__GetWindowSizeX(); 
extern int  WIDGET__GetXSize(); 
extern int  WIDGET__GetYSize(); 
extern int  WIDGET__Init(); 
extern int  WIDGET__RotateRect90(); 
extern int  WIDGET__SetScrollState(); 
extern int  WM_Activate(); 
extern int  WM_BringToBottom(); 
extern int  WM_BringToTop(); 
extern int  WM_CheckScrollBounds(); 
extern int  WM_CheckScrollPos(); 
extern int  WM_ClrHasTrans(); 
extern int  WM_CreateTimer(); 
extern int  WM_CreateWindow(); 
extern int  WM_CreateWindowAsChild(); 
extern int  WM_Deactivate(); 
extern int  WM_DefaultProc(); 
extern int  WM_DeleteTimer(); 
extern int  WM_DeleteWindow(); 
extern int  WM_DisableMemdev(); 
extern int  WM_EnableMemdev(); 
extern int  WM_Exec(); 
extern int  WM_ForEachDesc(); 
extern int  WM_GetActiveWindow(); 
extern int  WM_GetBkColor(); 
extern int  WM_GetClientRect(); 
extern int  WM_GetClientRectEx(); 
extern int  WM_GetClientWindow(); 
extern int  WM_GetDesktopWindow(); 
extern int  WM_GetDesktopWindowEx(); 
extern int  WM_GetDialogItem(); 
extern int  WM_GetFirstChild(); 
extern int  WM_GetFlags(); 
extern int  WM_GetFocussedWindow(); 
extern int  WM_GetHasTrans(); 
extern int  WM_GetId(); 
extern int  WM_GetInsideRect(); 
extern int  WM_GetInsideRectEx(); 
extern int  WM_GetInsideRectExScrollbar(); 
extern int  WM_GetNextSibling(); 
extern int  WM_GetOrgX(); 
extern int  WM_GetOrgY(); 
extern int  WM_GetParent(); 
extern int  WM_GetScrollPartner(); 
extern int  WM_GetScrollState(); 
extern int  WM_GetScrollbarH(); 
extern int  WM_GetScrollbarV(); 
extern int  WM_GetTimerId(); 
extern int  WM_GetUserData(); 
extern int  WM_GetWindowOrgX(); 
extern int  WM_GetWindowOrgY(); 
extern int  WM_GetWindowRect(); 
extern int  WM_GetWindowRectEx(); 
extern int  WM_GetWindowSizeX(); 
extern int  WM_GetWindowSizeY(); 
extern int  WM_HandlePID(); 
extern int  WM_HasCaptured(); 
extern int  WM_HasFocus(); 
extern int  WM_HideWindow(); 
extern int  WM_Init(); 
extern int  WM_InvalidateRect(); 
extern int  WM_InvalidateWindow(); 
extern int  WM_InvalidateWindowAndDescs(); 
extern int  WM_InvalidateWindowAndDescsEx(); 
extern int  WM_IsFocussable(); 
extern int  WM_IsVisible(); 
extern int  WM_IsWindow(); 
extern int  WM_MakeModal(); 
extern int  WM_MoveChildTo(); 
extern int  WM_MoveTo(); 
extern int  WM_MoveWindow(); 
extern int  WM_NotifyParent(); 
extern int  WM_OnKey(); 
extern int  WM_PID__GetPrevState(); 
extern int  WM_PID__SetPrevState(); 
extern int  WM_ReleaseCapture(); 
extern int  WM_ResizeWindow(); 
extern int  WM_RestartTimer(); 
extern int  WM_Screen2hWin(); 
extern int  WM_Screen2hWinEx(); 
extern int  WM_SelectWindow(); 
extern int  WM_SendMessage(); 
extern int  WM_SendMessageNoPara(); 
extern int  WM_SendToParent(); 
extern int  WM_SetCallback(); 
extern int  WM_SetCapture(); 
extern int  WM_SetCaptureMove(); 
extern int  WM_SetCreateFlags(); 
extern int  WM_SetDefault(); 
extern int  WM_SetFocus(); 
extern int  WM_SetFocusOnNextChild(); 
extern int  WM_SetFocusOnPrevChild(); 
extern int  WM_SetHasTrans(); 
extern int  WM_SetScrollState(); 
extern int  WM_SetScrollValue(); 
extern int  WM_SetScrollbarH(); 
extern int  WM_SetScrollbarV(); 
extern int  WM_SetSize(); 
extern int  WM_SetUserClipRect(); 
extern int  WM_SetUserData(); 
extern int  WM_SetWindowPos(); 
extern int  WM_ShowWindow(); 
extern int  WM_ValidateWindow(); 
extern int  WM__ActivateClipRect(); 
extern int  WM__AddCriticalHandle(); 
extern int  WM__Client2Screen(); 
extern int  WM__ClipAtParentBorders(); 
extern int  WM__DeleteAssocTimer(); 
extern int  WM__DeleteSecure(); 
extern int  WM__DetachWindow(); 
extern int  WM__ForEachDesc(); 
extern int  WM__GetClientRectEx(); 
extern int  WM__GetClientRectWin(); 
extern int  WM__GetFirstSibling(); 
extern int  WM__GetFocussedChild(); 
extern int  WM__GetLastSibling(); 
extern int  WM__GetNextIVR(); 
extern int  WM__GetOrgX_AA(); 
extern int  WM__GetOrgY_AA(); 
extern int  WM__GetPrevSibling(); 
extern int  WM__GetTopLevelLayer(); 
extern int  WM__GetUserDataEx(); 
extern int  WM__GetWindowSizeX(); 
extern int  WM__GetWindowSizeY(); 
extern int  WM__InitIVRSearch(); 
extern int  WM__InsertWindowIntoList(); 
extern int  WM__Invalidate1Abs(); 
extern int  WM__InvalidateDrawAndDescs(); 
extern int  WM__InvalidateParent(); 
extern int  WM__InvalidateRect(); 
extern int  WM__InvalidateRectEx(); 
extern int  WM__IsAncestor(); 
extern int  WM__IsAncestorOrSelf(); 
extern int  WM__IsChild(); 
extern int  WM__IsEnabled(); 
extern int  WM__IsInModalArea(); 
extern int  WM__IsInWindow(); 
extern int  WM__IsWindow(); 
extern int  WM__MoveTo(); 
extern int  WM__MoveWindow(); 
extern int  WM__Paint(); 
extern int  WM__Paint1(); 
extern int  WM__RectIsNZ(); 
extern int  WM__RemoveCriticalHandle(); 
extern int  WM__RemoveFromLinList(); 
extern int  WM__RemoveWindowFromList(); 
extern int  WM__Screen2Client(); 
extern int  WM__SelectTopLevelLayer(); 
extern int  WM__SendMessage(); 
extern int  WM__SendMessageIfEnabled(); 
extern int  WM__SendMessageNoPara(); 
extern int  WM__SendMsgNoData(); 
extern int  WM__SendPIDMessage(); 
extern int  WM__SendTouchMessage(); 
extern int  WM__SetLastTouched(); 
extern int  WM__SetMotionCallback(); 
extern int  WM__SetScrollbarH(); 
extern int  WM__SetScrollbarV(); 
extern int  WM__SetToolTipCallback(); 
extern int  WM__SetUserDataEx(); 
extern int  WM__UpdateChildPositions(); 
extern int  _Balloc(); 
extern int  _Bfree(); 
extern int  _Unwind_Backtrace(); 
extern int  _Unwind_Complete(); 
extern int  _Unwind_DeleteException(); 
extern int  _Unwind_ForcedUnwind(); 
extern int  _Unwind_GetCFA(); 
extern int  _Unwind_GetDataRelBase(); 
extern int  _Unwind_GetLanguageSpecificData(); 
extern int  _Unwind_GetRegionStart(); 
extern int  _Unwind_GetTextRelBase(); 
extern int  _Unwind_RaiseException(); 
extern int  _Unwind_Resume(); 
extern int  _Unwind_Resume_or_Rethrow(); 
extern int  _Unwind_VRS_Get(); 
extern int  _Unwind_VRS_Pop(); 
extern int  _Unwind_VRS_Set(); 
extern int  ___Unwind_Backtrace(); 
extern int  ___Unwind_ForcedUnwind(); 
extern int  ___Unwind_RaiseException(); 
extern int  ___Unwind_Resume(); 
extern int  ___Unwind_Resume_or_Rethrow(); 
extern int  __adddf3(); 
extern int  __addsf3(); 
extern int  __aeabi_cdcmpeq(); 
extern int  __aeabi_cdcmple(); 
extern int  __aeabi_cdrcmple(); 
extern int  __aeabi_cfcmpeq(); 
extern int  __aeabi_cfcmple(); 
extern int  __aeabi_cfrcmple(); 
extern int  __aeabi_d2f(); 
extern int  __aeabi_d2iz(); 
extern int  __aeabi_d2uiz(); 
extern int  __aeabi_dadd(); 
extern int  __aeabi_dcmpeq(); 
extern int  __aeabi_dcmpge(); 
extern int  __aeabi_dcmpgt(); 
extern int  __aeabi_dcmple(); 
extern int  __aeabi_dcmplt(); 
extern int  __aeabi_ddiv(); 
extern int  __aeabi_dmul(); 
extern int  __aeabi_drsub(); 
extern int  __aeabi_dsub(); 
extern int  __aeabi_f2d(); 
extern int  __aeabi_f2iz(); 
extern int  __aeabi_f2uiz(); 
extern int  __aeabi_fadd(); 
extern int  __aeabi_fcmpeq(); 
extern int  __aeabi_fcmpge(); 
extern int  __aeabi_fcmpgt(); 
extern int  __aeabi_fcmple(); 
extern int  __aeabi_fcmplt(); 
extern int  __aeabi_fdiv(); 
extern int  __aeabi_fmul(); 
extern int  __aeabi_frsub(); 
extern int  __aeabi_fsub(); 
extern int  __aeabi_i2d(); 
extern int  __aeabi_i2f(); 
extern int  __aeabi_idiv(); 
extern int  __aeabi_idivmod(); 
extern int  __aeabi_l2d(); 
extern int  __aeabi_l2f(); 
extern int  __aeabi_ui2d(); 
extern int  __aeabi_ui2f(); 
extern int  __aeabi_uidiv(); 
extern int  __aeabi_uidivmod(); 
extern int  __aeabi_ul2d(); 
extern int  __aeabi_ul2f(); 
extern int  __aeabi_uldivmod(); 
extern int  __aeabi_unwind_cpp_pr0(); 
extern int  __any_on(); 
extern int  __ascii_mbtowc(); 
extern int  __ascii_wctomb(); 
extern int  __assert(); 
extern int  __assert_func(); 
extern int  __b2d(); 
extern int  __clzsi2(); 
extern int  __cmpdf2(); 
extern int  __cmpsf2(); 
extern int  __copybits(); 
extern int  __d2b(); 
extern int  __divdf3(); 
extern int  __divdi3(); 
extern int  __divsf3(); 
extern int  __divsi3(); 
extern int  __env_lock(); 
extern int  __env_unlock(); 
extern int  __eqdf2(); 
extern int  __eqsf2(); 
extern int  __errno(); 
extern int  __extendsfdf2(); 
extern int  __ffssi2(); 
extern int  __fixdfsi(); 
extern int  __fixsfsi(); 
extern int  __fixunsdfsi(); 
extern int  __fixunssfsi(); 
extern int  __floatdidf(); 
extern int  __floatdisf(); 
extern int  __floatsidf(); 
extern int  __floatsisf(); 
extern int  __floatundidf(); 
extern int  __floatundisf(); 
extern int  __floatunsidf(); 
extern int  __floatunsisf(); 
extern int  __fp_lock_all(); 
extern int  __fp_unlock_all(); 
extern int  __fpclassifyd(); 
extern int  __gedf2(); 
extern int  __gesf2(); 
extern int  __gethex(); 
extern int  __gettzinfo(); 
extern int  __gnu_Unwind_Backtrace(); 
extern int  __gnu_Unwind_ForcedUnwind(); 
extern int  __gnu_Unwind_RaiseException(); 
extern int  __gnu_Unwind_Restore_VFP(); 
extern int  __gnu_Unwind_Restore_VFP_D(); 
extern int  __gnu_Unwind_Restore_VFP_D_16_to_31(); 
extern int  __gnu_Unwind_Restore_WMMXC(); 
extern int  __gnu_Unwind_Restore_WMMXD(); 
extern int  __gnu_Unwind_Resume(); 
extern int  __gnu_Unwind_Resume_or_Rethrow(); 
extern int  __gnu_Unwind_Save_VFP(); 
extern int  __gnu_Unwind_Save_VFP_D(); 
extern int  __gnu_Unwind_Save_VFP_D_16_to_31(); 
extern int  __gnu_Unwind_Save_WMMXC(); 
extern int  __gnu_Unwind_Save_WMMXD(); 
extern int  __gnu_ldivmod_helper(); 
extern int  __gnu_uldivmod_helper(); 
extern int  __gnu_unwind_execute(); 
extern int  __gnu_unwind_frame(); 
extern int  __gtdf2(); 
extern int  __gtsf2(); 
extern int  __hexdig_init(); 
extern int  __hexnan(); 
extern int  __hi0bits(); 
extern int  __i2b(); 
extern int  __ieee754_sqrt(); 
extern int  __kfree(); 
extern int  __kmalloc(); 
extern int  __ledf2(); 
extern int  __lesf2(); 
extern int  __lo0bits(); 
extern int  __locale_charset(); 
extern int  __locale_cjk_lang(); 
extern int  __locale_mb_cur_max(); 
extern int  __locale_msgcharset(); 
extern int  __lshift(); 
extern int  __ltdf2(); 
extern int  __ltsf2(); 
extern int  __mcmp(); 
extern int  __mdiff(); 
extern int  __mprec_bigtens(); 
extern int  __mprec_tens(); 
extern int  __mprec_tinytens(); 
extern int  __muldf3(); 
extern int  __mulsf3(); 
extern int  __multadd(); 
extern int  __multiply(); 
extern int  __nedf2(); 
extern int  __nesf2(); 
extern int  __pow5mult(); 
extern int  __ratio(); 
extern int  __restore_core_regs(); 
extern int  __s2b(); 
extern int  __sccl(); 
extern int  __sclose(); 
extern int  __seofread(); 
extern int  __sflags(); 
extern int  __sfmoreglue(); 
extern int  __sfp(); 
extern int  __sfp_lock_acquire(); 
extern int  __sfp_lock_release(); 
extern int  __sfvwrite_r(); 
extern int  __sinit(); 
extern int  __sinit_lock_acquire(); 
extern int  __sinit_lock_release(); 
extern int  __smakebuf_r(); 
extern int  __sprint_r(); 
extern int  __sread(); 
extern int  __sseek(); 
extern int  __ssprint_r(); 
extern int  __ssrefill_r(); 
extern int  __ssvfiscanf_r(); 
extern int  __ssvfscanf_r(); 
extern int  __subdf3(); 
extern int  __submore(); 
extern int  __subsf3(); 
extern int  __swbuf(); 
extern int  __swbuf_r(); 
extern int  __swrite(); 
extern int  __swsetup_r(); 
extern int  __truncdfsf2(); 
extern int  __tz_lock(); 
extern int  __tz_unlock(); 
extern int  __tzcalc_limits(); 
extern int  __udivdi3(); 
extern int  __udivsi3(); 
extern int  __ulp(); 
extern int  _bmBrowser(); 
extern int  _bmCheckMark(); 
extern int  _bmClock(); 
extern int  _bmDate(); 
extern int  _bmEmail(); 
extern int  _bmPassword(); 
extern int  _bmRead(); 
extern int  _bmRemote(); 
extern int  _bmSystem(); 
extern int  _bmToucan(); 
extern int  _bmWrite(); 
extern int  _calloc_r(); 
extern int  _cleanup(); 
extern int  _cleanup_r(); 
extern int  _close_r(); 
extern int  _ctype_(); 
extern int  _dtoa_r(); 
extern int  _execve_r(); 
extern int  _exit(); 
extern int  _fclose_r(); 
extern int  _fdopen_r(); 
extern int  _fflush_r(); 
extern int  _findenv_r(); 
extern int  _fini(); 
extern int  _fiprintf_r(); 
extern int  _fork_r(); 
extern int  _fprintf_r(); 
extern int  _fputwc_r(); 
extern int  _free_r(); 
extern int  _fstat_r(); 
extern int  _fwalk(); 
extern int  _fwalk_reent(); 
extern int  _getenv_r(); 
extern int  _getpid_r(); 
extern int  _gettimeofday_r(); 
extern int  _global_impure_ptr(); 
extern int  _init(); 
extern int  _isatty_r(); 
extern int  _kill_r(); 
extern int  _link_r(); 
extern int  _localeconv_r(); 
extern int  _lseek_r(); 
extern int  _malloc_r(); 
extern int  _mbrtowc_r(); 
extern int  _mbtowc_r(); 
extern int  _mkdir_r(); 
extern int  _mktm_r(); 
extern int  _mprec_log10(); 
extern int  _open_r(); 
extern int  _printf_r(); 
extern int  _putenv_r(); 
extern int  _read_r(); 
extern int  _realloc_r(); 
extern int  _rename_r(); 
extern int  _rmdir_r(); 
extern int  _sbrk_r(); 
extern int  _setenv_r(); 
extern int  _setlocale_r(); 
extern int  _sfread_r(); 
extern int  _snprintf_r(); 
extern int  _sprintf_r(); 
extern int  _sscanf_r(); 
extern int  _start(); 
extern int  _stat_r(); 
extern int  _strdup_r(); 
extern int  _strndup_r(); 
extern int  _strtod_r(); 
extern int  _strtol_r(); 
extern int  _strtoul_r(); 
extern int  _sungetc_r(); 
extern int  _svfiprintf_r(); 
extern int  _svfprintf_r(); 
extern int  _times_r(); 
extern int  _tzset_r(); 
extern int  _ungetc_r(); 
extern int  _unlink_r(); 
extern int  _unsetenv_r(); 
extern int  _vfiprintf_r(); 
extern int  _vfprintf_r(); 
extern int  _vsnprintf_r(); 
extern int  _wait_r(); 
extern int  _wcrtomb_r(); 
extern int  _wctomb_r(); 
extern int  _write_r(); 
extern int  aTan(); 
extern int  abort(); 
extern int  acGUI_Font18B_0020(); 
extern int  acGUI_Font18B_0021(); 
extern int  acGUI_Font18B_0022(); 
extern int  acGUI_Font18B_0023(); 
extern int  acGUI_Font18B_0024(); 
extern int  acGUI_Font18B_0025(); 
extern int  acGUI_Font18B_0026(); 
extern int  acGUI_Font18B_0027(); 
extern int  acGUI_Font18B_0028(); 
extern int  acGUI_Font18B_0029(); 
extern int  acGUI_Font18B_002A(); 
extern int  acGUI_Font18B_002B(); 
extern int  acGUI_Font18B_002C(); 
extern int  acGUI_Font18B_002D(); 
extern int  acGUI_Font18B_002E(); 
extern int  acGUI_Font18B_002F(); 
extern int  acGUI_Font18B_0030(); 
extern int  acGUI_Font18B_0031(); 
extern int  acGUI_Font18B_0032(); 
extern int  acGUI_Font18B_0033(); 
extern int  acGUI_Font18B_0034(); 
extern int  acGUI_Font18B_0035(); 
extern int  acGUI_Font18B_0036(); 
extern int  acGUI_Font18B_0037(); 
extern int  acGUI_Font18B_0038(); 
extern int  acGUI_Font18B_0039(); 
extern int  acGUI_Font18B_003A(); 
extern int  acGUI_Font18B_003B(); 
extern int  acGUI_Font18B_003C(); 
extern int  acGUI_Font18B_003D(); 
extern int  acGUI_Font18B_003E(); 
extern int  acGUI_Font18B_003F(); 
extern int  acGUI_Font18B_0040(); 
extern int  acGUI_Font18B_0041(); 
extern int  acGUI_Font18B_0042(); 
extern int  acGUI_Font18B_0043(); 
extern int  acGUI_Font18B_0044(); 
extern int  acGUI_Font18B_0045(); 
extern int  acGUI_Font18B_0046(); 
extern int  acGUI_Font18B_0047(); 
extern int  acGUI_Font18B_0048(); 
extern int  acGUI_Font18B_0049(); 
extern int  acGUI_Font18B_004A(); 
extern int  acGUI_Font18B_004B(); 
extern int  acGUI_Font18B_004C(); 
extern int  acGUI_Font18B_004D(); 
extern int  acGUI_Font18B_004E(); 
extern int  acGUI_Font18B_004F(); 
extern int  acGUI_Font18B_0050(); 
extern int  acGUI_Font18B_0051(); 
extern int  acGUI_Font18B_0052(); 
extern int  acGUI_Font18B_0053(); 
extern int  acGUI_Font18B_0054(); 
extern int  acGUI_Font18B_0055(); 
extern int  acGUI_Font18B_0056(); 
extern int  acGUI_Font18B_0057(); 
extern int  acGUI_Font18B_0058(); 
extern int  acGUI_Font18B_0059(); 
extern int  acGUI_Font18B_005A(); 
extern int  acGUI_Font18B_005B(); 
extern int  acGUI_Font18B_005C(); 
extern int  acGUI_Font18B_005D(); 
extern int  acGUI_Font18B_005E(); 
extern int  acGUI_Font18B_005F(); 
extern int  acGUI_Font18B_0060(); 
extern int  acGUI_Font18B_0061(); 
extern int  acGUI_Font18B_0062(); 
extern int  acGUI_Font18B_0063(); 
extern int  acGUI_Font18B_0064(); 
extern int  acGUI_Font18B_0065(); 
extern int  acGUI_Font18B_0066(); 
extern int  acGUI_Font18B_0067(); 
extern int  acGUI_Font18B_0068(); 
extern int  acGUI_Font18B_0069(); 
extern int  acGUI_Font18B_006A(); 
extern int  acGUI_Font18B_006B(); 
extern int  acGUI_Font18B_006C(); 
extern int  acGUI_Font18B_006D(); 
extern int  acGUI_Font18B_006E(); 
extern int  acGUI_Font18B_006F(); 
extern int  acGUI_Font18B_0070(); 
extern int  acGUI_Font18B_0071(); 
extern int  acGUI_Font18B_0072(); 
extern int  acGUI_Font18B_0073(); 
extern int  acGUI_Font18B_0074(); 
extern int  acGUI_Font18B_0075(); 
extern int  acGUI_Font18B_0076(); 
extern int  acGUI_Font18B_0077(); 
extern int  acGUI_Font18B_0078(); 
extern int  acGUI_Font18B_0079(); 
extern int  acGUI_Font18B_007A(); 
extern int  acGUI_Font18B_007B(); 
extern int  acGUI_Font18B_007C(); 
extern int  acGUI_Font18B_007D(); 
extern int  acGUI_Font18B_007E(); 
extern int  acGUI_Font18_0020(); 
extern int  acGUI_Font18_0021(); 
extern int  acGUI_Font18_0022(); 
extern int  acGUI_Font18_0023(); 
extern int  acGUI_Font18_0024(); 
extern int  acGUI_Font18_0025(); 
extern int  acGUI_Font18_0026(); 
extern int  acGUI_Font18_0027(); 
extern int  acGUI_Font18_0028(); 
extern int  acGUI_Font18_0029(); 
extern int  acGUI_Font18_002A(); 
extern int  acGUI_Font18_002B(); 
extern int  acGUI_Font18_002C(); 
extern int  acGUI_Font18_002D(); 
extern int  acGUI_Font18_002E(); 
extern int  acGUI_Font18_002F(); 
extern int  acGUI_Font18_0030(); 
extern int  acGUI_Font18_0031(); 
extern int  acGUI_Font18_0032(); 
extern int  acGUI_Font18_0033(); 
extern int  acGUI_Font18_0034(); 
extern int  acGUI_Font18_0035(); 
extern int  acGUI_Font18_0036(); 
extern int  acGUI_Font18_0037(); 
extern int  acGUI_Font18_0038(); 
extern int  acGUI_Font18_0039(); 
extern int  acGUI_Font18_003A(); 
extern int  acGUI_Font18_003B(); 
extern int  acGUI_Font18_003C(); 
extern int  acGUI_Font18_003D(); 
extern int  acGUI_Font18_003E(); 
extern int  acGUI_Font18_003F(); 
extern int  acGUI_Font18_0040(); 
extern int  acGUI_Font18_0041(); 
extern int  acGUI_Font18_0042(); 
extern int  acGUI_Font18_0043(); 
extern int  acGUI_Font18_0044(); 
extern int  acGUI_Font18_0045(); 
extern int  acGUI_Font18_0046(); 
extern int  acGUI_Font18_0047(); 
extern int  acGUI_Font18_0048(); 
extern int  acGUI_Font18_0049(); 
extern int  acGUI_Font18_004A(); 
extern int  acGUI_Font18_004B(); 
extern int  acGUI_Font18_004C(); 
extern int  acGUI_Font18_004D(); 
extern int  acGUI_Font18_004E(); 
extern int  acGUI_Font18_004F(); 
extern int  acGUI_Font18_0050(); 
extern int  acGUI_Font18_0051(); 
extern int  acGUI_Font18_0052(); 
extern int  acGUI_Font18_0053(); 
extern int  acGUI_Font18_0054(); 
extern int  acGUI_Font18_0055(); 
extern int  acGUI_Font18_0056(); 
extern int  acGUI_Font18_0057(); 
extern int  acGUI_Font18_0058(); 
extern int  acGUI_Font18_0059(); 
extern int  acGUI_Font18_005A(); 
extern int  acGUI_Font18_005B(); 
extern int  acGUI_Font18_005C(); 
extern int  acGUI_Font18_005D(); 
extern int  acGUI_Font18_005E(); 
extern int  acGUI_Font18_005F(); 
extern int  acGUI_Font18_0060(); 
extern int  acGUI_Font18_0061(); 
extern int  acGUI_Font18_0062(); 
extern int  acGUI_Font18_0063(); 
extern int  acGUI_Font18_0064(); 
extern int  acGUI_Font18_0065(); 
extern int  acGUI_Font18_0066(); 
extern int  acGUI_Font18_0067(); 
extern int  acGUI_Font18_0068(); 
extern int  acGUI_Font18_0069(); 
extern int  acGUI_Font18_006A(); 
extern int  acGUI_Font18_006B(); 
extern int  acGUI_Font18_006C(); 
extern int  acGUI_Font18_006D(); 
extern int  acGUI_Font18_006E(); 
extern int  acGUI_Font18_006F(); 
extern int  acGUI_Font18_0070(); 
extern int  acGUI_Font18_0071(); 
extern int  acGUI_Font18_0072(); 
extern int  acGUI_Font18_0073(); 
extern int  acGUI_Font18_0074(); 
extern int  acGUI_Font18_0075(); 
extern int  acGUI_Font18_0076(); 
extern int  acGUI_Font18_0077(); 
extern int  acGUI_Font18_0078(); 
extern int  acGUI_Font18_0079(); 
extern int  acGUI_Font18_007A(); 
extern int  acGUI_Font18_007B(); 
extern int  acGUI_Font18_007C(); 
extern int  acGUI_Font18_007D(); 
extern int  acGUI_Font18_007E(); 
extern int  acGUI_Font60BD_0030(); 
extern int  acGUI_Font60BD_0031(); 
extern int  acGUI_Font60BD_0032(); 
extern int  acGUI_Font60BD_0033(); 
extern int  acGUI_Font60BD_0034(); 
extern int  acGUI_Font60BD_0035(); 
extern int  acGUI_Font60BD_0036(); 
extern int  acGUI_Font60BD_0037(); 
extern int  acGUI_Font60BD_0038(); 
extern int  acGUI_Font60BD_0039(); 
extern int  acGUI_Font60BD_003A(); 
extern int  acGUI_Font60min_0069(); 
extern int  acGUI_Font60min_006D(); 
extern int  acGUI_Font60min_006E(); 
extern int  acGUI_FontD9_AA4_0030(); 
extern int  acGUI_FontD9_AA4_0031(); 
extern int  acGUI_FontD9_AA4_0032(); 
extern int  acGUI_FontD9_AA4_0033(); 
extern int  acGUI_FontD9_AA4_0034(); 
extern int  acGUI_FontD9_AA4_0035(); 
extern int  acGUI_FontD9_AA4_0036(); 
extern int  acGUI_FontD9_AA4_0037(); 
extern int  acGUI_FontD9_AA4_0038(); 
extern int  acGUI_FontD9_AA4_0039(); 
extern int  acGUI_FontRounded16_0020(); 
extern int  acGUI_FontRounded16_0021(); 
extern int  acGUI_FontRounded16_0022(); 
extern int  acGUI_FontRounded16_0023(); 
extern int  acGUI_FontRounded16_0024(); 
extern int  acGUI_FontRounded16_0025(); 
extern int  acGUI_FontRounded16_0026(); 
extern int  acGUI_FontRounded16_0027(); 
extern int  acGUI_FontRounded16_0028(); 
extern int  acGUI_FontRounded16_0029(); 
extern int  acGUI_FontRounded16_002A(); 
extern int  acGUI_FontRounded16_002B(); 
extern int  acGUI_FontRounded16_002C(); 
extern int  acGUI_FontRounded16_002D(); 
extern int  acGUI_FontRounded16_002E(); 
extern int  acGUI_FontRounded16_002F(); 
extern int  acGUI_FontRounded16_0030(); 
extern int  acGUI_FontRounded16_0031(); 
extern int  acGUI_FontRounded16_0032(); 
extern int  acGUI_FontRounded16_0033(); 
extern int  acGUI_FontRounded16_0034(); 
extern int  acGUI_FontRounded16_0035(); 
extern int  acGUI_FontRounded16_0036(); 
extern int  acGUI_FontRounded16_0037(); 
extern int  acGUI_FontRounded16_0038(); 
extern int  acGUI_FontRounded16_0039(); 
extern int  acGUI_FontRounded16_003A(); 
extern int  acGUI_FontRounded16_003B(); 
extern int  acGUI_FontRounded16_003C(); 
extern int  acGUI_FontRounded16_003D(); 
extern int  acGUI_FontRounded16_003E(); 
extern int  acGUI_FontRounded16_003F(); 
extern int  acGUI_FontRounded16_0040(); 
extern int  acGUI_FontRounded16_0041(); 
extern int  acGUI_FontRounded16_0042(); 
extern int  acGUI_FontRounded16_0043(); 
extern int  acGUI_FontRounded16_0044(); 
extern int  acGUI_FontRounded16_0045(); 
extern int  acGUI_FontRounded16_0046(); 
extern int  acGUI_FontRounded16_0047(); 
extern int  acGUI_FontRounded16_0048(); 
extern int  acGUI_FontRounded16_0049(); 
extern int  acGUI_FontRounded16_004A(); 
extern int  acGUI_FontRounded16_004B(); 
extern int  acGUI_FontRounded16_004C(); 
extern int  acGUI_FontRounded16_004D(); 
extern int  acGUI_FontRounded16_004E(); 
extern int  acGUI_FontRounded16_004F(); 
extern int  acGUI_FontRounded16_0050(); 
extern int  acGUI_FontRounded16_0051(); 
extern int  acGUI_FontRounded16_0052(); 
extern int  acGUI_FontRounded16_0053(); 
extern int  acGUI_FontRounded16_0054(); 
extern int  acGUI_FontRounded16_0055(); 
extern int  acGUI_FontRounded16_0056(); 
extern int  acGUI_FontRounded16_0057(); 
extern int  acGUI_FontRounded16_0058(); 
extern int  acGUI_FontRounded16_0059(); 
extern int  acGUI_FontRounded16_005A(); 
extern int  acGUI_FontRounded16_005B(); 
extern int  acGUI_FontRounded16_005C(); 
extern int  acGUI_FontRounded16_005D(); 
extern int  acGUI_FontRounded16_005E(); 
extern int  acGUI_FontRounded16_005F(); 
extern int  acGUI_FontRounded16_0060(); 
extern int  acGUI_FontRounded16_0061(); 
extern int  acGUI_FontRounded16_0062(); 
extern int  acGUI_FontRounded16_0063(); 
extern int  acGUI_FontRounded16_0064(); 
extern int  acGUI_FontRounded16_0065(); 
extern int  acGUI_FontRounded16_0066(); 
extern int  acGUI_FontRounded16_0067(); 
extern int  acGUI_FontRounded16_0068(); 
extern int  acGUI_FontRounded16_0069(); 
extern int  acGUI_FontRounded16_006A(); 
extern int  acGUI_FontRounded16_006B(); 
extern int  acGUI_FontRounded16_006C(); 
extern int  acGUI_FontRounded16_006D(); 
extern int  acGUI_FontRounded16_006E(); 
extern int  acGUI_FontRounded16_006F(); 
extern int  acGUI_FontRounded16_0070(); 
extern int  acGUI_FontRounded16_0071(); 
extern int  acGUI_FontRounded16_0072(); 
extern int  acGUI_FontRounded16_0073(); 
extern int  acGUI_FontRounded16_0074(); 
extern int  acGUI_FontRounded16_0075(); 
extern int  acGUI_FontRounded16_0076(); 
extern int  acGUI_FontRounded16_0077(); 
extern int  acGUI_FontRounded16_0078(); 
extern int  acGUI_FontRounded16_0079(); 
extern int  acGUI_FontRounded16_007A(); 
extern int  acGUI_FontRounded16_007B(); 
extern int  acGUI_FontRounded16_007C(); 
extern int  acGUI_FontRounded16_007D(); 
extern int  acGUI_FontRounded16_007E(); 
extern int  acGUI_FontRounded22_0020(); 
extern int  acGUI_FontRounded22_0021(); 
extern int  acGUI_FontRounded22_0022(); 
extern int  acGUI_FontRounded22_0023(); 
extern int  acGUI_FontRounded22_0024(); 
extern int  acGUI_FontRounded22_0025(); 
extern int  acGUI_FontRounded22_0026(); 
extern int  acGUI_FontRounded22_0027(); 
extern int  acGUI_FontRounded22_0028(); 
extern int  acGUI_FontRounded22_0029(); 
extern int  acGUI_FontRounded22_002A(); 
extern int  acGUI_FontRounded22_002B(); 
extern int  acGUI_FontRounded22_002C(); 
extern int  acGUI_FontRounded22_002D(); 
extern int  acGUI_FontRounded22_002E(); 
extern int  acGUI_FontRounded22_002F(); 
extern int  acGUI_FontRounded22_0030(); 
extern int  acGUI_FontRounded22_0031(); 
extern int  acGUI_FontRounded22_0032(); 
extern int  acGUI_FontRounded22_0033(); 
extern int  acGUI_FontRounded22_0034(); 
extern int  acGUI_FontRounded22_0035(); 
extern int  acGUI_FontRounded22_0036(); 
extern int  acGUI_FontRounded22_0037(); 
extern int  acGUI_FontRounded22_0038(); 
extern int  acGUI_FontRounded22_0039(); 
extern int  acGUI_FontRounded22_003A(); 
extern int  acGUI_FontRounded22_003B(); 
extern int  acGUI_FontRounded22_003C(); 
extern int  acGUI_FontRounded22_003D(); 
extern int  acGUI_FontRounded22_003E(); 
extern int  acGUI_FontRounded22_003F(); 
extern int  acGUI_FontRounded22_0040(); 
extern int  acGUI_FontRounded22_0041(); 
extern int  acGUI_FontRounded22_0042(); 
extern int  acGUI_FontRounded22_0043(); 
extern int  acGUI_FontRounded22_0044(); 
extern int  acGUI_FontRounded22_0045(); 
extern int  acGUI_FontRounded22_0046(); 
extern int  acGUI_FontRounded22_0047(); 
extern int  acGUI_FontRounded22_0048(); 
extern int  acGUI_FontRounded22_0049(); 
extern int  acGUI_FontRounded22_004A(); 
extern int  acGUI_FontRounded22_004B(); 
extern int  acGUI_FontRounded22_004C(); 
extern int  acGUI_FontRounded22_004D(); 
extern int  acGUI_FontRounded22_004E(); 
extern int  acGUI_FontRounded22_004F(); 
extern int  acGUI_FontRounded22_0050(); 
extern int  acGUI_FontRounded22_0051(); 
extern int  acGUI_FontRounded22_0052(); 
extern int  acGUI_FontRounded22_0053(); 
extern int  acGUI_FontRounded22_0054(); 
extern int  acGUI_FontRounded22_0055(); 
extern int  acGUI_FontRounded22_0056(); 
extern int  acGUI_FontRounded22_0057(); 
extern int  acGUI_FontRounded22_0058(); 
extern int  acGUI_FontRounded22_0059(); 
extern int  acGUI_FontRounded22_005A(); 
extern int  acGUI_FontRounded22_005B(); 
extern int  acGUI_FontRounded22_005C(); 
extern int  acGUI_FontRounded22_005D(); 
extern int  acGUI_FontRounded22_005E(); 
extern int  acGUI_FontRounded22_005F(); 
extern int  acGUI_FontRounded22_0060(); 
extern int  acGUI_FontRounded22_0061(); 
extern int  acGUI_FontRounded22_0062(); 
extern int  acGUI_FontRounded22_0063(); 
extern int  acGUI_FontRounded22_0064(); 
extern int  acGUI_FontRounded22_0065(); 
extern int  acGUI_FontRounded22_0066(); 
extern int  acGUI_FontRounded22_0067(); 
extern int  acGUI_FontRounded22_0068(); 
extern int  acGUI_FontRounded22_0069(); 
extern int  acGUI_FontRounded22_006A(); 
extern int  acGUI_FontRounded22_006B(); 
extern int  acGUI_FontRounded22_006C(); 
extern int  acGUI_FontRounded22_006D(); 
extern int  acGUI_FontRounded22_006E(); 
extern int  acGUI_FontRounded22_006F(); 
extern int  acGUI_FontRounded22_0070(); 
extern int  acGUI_FontRounded22_0071(); 
extern int  acGUI_FontRounded22_0072(); 
extern int  acGUI_FontRounded22_0073(); 
extern int  acGUI_FontRounded22_0074(); 
extern int  acGUI_FontRounded22_0075(); 
extern int  acGUI_FontRounded22_0076(); 
extern int  acGUI_FontRounded22_0077(); 
extern int  acGUI_FontRounded22_0078(); 
extern int  acGUI_FontRounded22_0079(); 
extern int  acGUI_FontRounded22_007A(); 
extern int  acGUI_FontRounded22_007B(); 
extern int  acGUI_FontRounded22_007C(); 
extern int  acGUI_FontRounded22_007D(); 
extern int  acGUI_FontRounded22_007E(); 
extern int  acGUI_FontSouvenir18_0020(); 
extern int  acGUI_FontSouvenir18_0021(); 
extern int  acGUI_FontSouvenir18_0022(); 
extern int  acGUI_FontSouvenir18_0023(); 
extern int  acGUI_FontSouvenir18_0024(); 
extern int  acGUI_FontSouvenir18_0025(); 
extern int  acGUI_FontSouvenir18_0026(); 
extern int  acGUI_FontSouvenir18_0027(); 
extern int  acGUI_FontSouvenir18_0028(); 
extern int  acGUI_FontSouvenir18_0029(); 
extern int  acGUI_FontSouvenir18_002A(); 
extern int  acGUI_FontSouvenir18_002B(); 
extern int  acGUI_FontSouvenir18_002C(); 
extern int  acGUI_FontSouvenir18_002D(); 
extern int  acGUI_FontSouvenir18_002E(); 
extern int  acGUI_FontSouvenir18_002F(); 
extern int  acGUI_FontSouvenir18_0030(); 
extern int  acGUI_FontSouvenir18_0031(); 
extern int  acGUI_FontSouvenir18_0032(); 
extern int  acGUI_FontSouvenir18_0033(); 
extern int  acGUI_FontSouvenir18_0034(); 
extern int  acGUI_FontSouvenir18_0035(); 
extern int  acGUI_FontSouvenir18_0036(); 
extern int  acGUI_FontSouvenir18_0037(); 
extern int  acGUI_FontSouvenir18_0038(); 
extern int  acGUI_FontSouvenir18_0039(); 
extern int  acGUI_FontSouvenir18_003A(); 
extern int  acGUI_FontSouvenir18_003B(); 
extern int  acGUI_FontSouvenir18_003C(); 
extern int  acGUI_FontSouvenir18_003D(); 
extern int  acGUI_FontSouvenir18_003E(); 
extern int  acGUI_FontSouvenir18_003F(); 
extern int  acGUI_FontSouvenir18_0040(); 
extern int  acGUI_FontSouvenir18_0041(); 
extern int  acGUI_FontSouvenir18_0042(); 
extern int  acGUI_FontSouvenir18_0043(); 
extern int  acGUI_FontSouvenir18_0044(); 
extern int  acGUI_FontSouvenir18_0045(); 
extern int  acGUI_FontSouvenir18_0046(); 
extern int  acGUI_FontSouvenir18_0047(); 
extern int  acGUI_FontSouvenir18_0048(); 
extern int  acGUI_FontSouvenir18_0049(); 
extern int  acGUI_FontSouvenir18_004A(); 
extern int  acGUI_FontSouvenir18_004B(); 
extern int  acGUI_FontSouvenir18_004C(); 
extern int  acGUI_FontSouvenir18_004D(); 
extern int  acGUI_FontSouvenir18_004E(); 
extern int  acGUI_FontSouvenir18_004F(); 
extern int  acGUI_FontSouvenir18_0050(); 
extern int  acGUI_FontSouvenir18_0051(); 
extern int  acGUI_FontSouvenir18_0052(); 
extern int  acGUI_FontSouvenir18_0053(); 
extern int  acGUI_FontSouvenir18_0054(); 
extern int  acGUI_FontSouvenir18_0055(); 
extern int  acGUI_FontSouvenir18_0056(); 
extern int  acGUI_FontSouvenir18_0057(); 
extern int  acGUI_FontSouvenir18_0058(); 
extern int  acGUI_FontSouvenir18_0059(); 
extern int  acGUI_FontSouvenir18_005A(); 
extern int  acGUI_FontSouvenir18_005B(); 
extern int  acGUI_FontSouvenir18_005C(); 
extern int  acGUI_FontSouvenir18_005D(); 
extern int  acGUI_FontSouvenir18_005E(); 
extern int  acGUI_FontSouvenir18_005F(); 
extern int  acGUI_FontSouvenir18_0060(); 
extern int  acGUI_FontSouvenir18_0061(); 
extern int  acGUI_FontSouvenir18_0062(); 
extern int  acGUI_FontSouvenir18_0063(); 
extern int  acGUI_FontSouvenir18_0064(); 
extern int  acGUI_FontSouvenir18_0065(); 
extern int  acGUI_FontSouvenir18_0066(); 
extern int  acGUI_FontSouvenir18_0067(); 
extern int  acGUI_FontSouvenir18_0068(); 
extern int  acGUI_FontSouvenir18_0069(); 
extern int  acGUI_FontSouvenir18_006A(); 
extern int  acGUI_FontSouvenir18_006B(); 
extern int  acGUI_FontSouvenir18_006C(); 
extern int  acGUI_FontSouvenir18_006D(); 
extern int  acGUI_FontSouvenir18_006E(); 
extern int  acGUI_FontSouvenir18_006F(); 
extern int  acGUI_FontSouvenir18_0070(); 
extern int  acGUI_FontSouvenir18_0071(); 
extern int  acGUI_FontSouvenir18_0072(); 
extern int  acGUI_FontSouvenir18_0073(); 
extern int  acGUI_FontSouvenir18_0074(); 
extern int  acGUI_FontSouvenir18_0075(); 
extern int  acGUI_FontSouvenir18_0076(); 
extern int  acGUI_FontSouvenir18_0077(); 
extern int  acGUI_FontSouvenir18_0078(); 
extern int  acGUI_FontSouvenir18_0079(); 
extern int  acGUI_FontSouvenir18_007A(); 
extern int  acGUI_FontSouvenir18_007B(); 
extern int  acGUI_FontSouvenir18_007C(); 
extern int  acGUI_FontSouvenir18_007D(); 
extern int  acGUI_FontSouvenir18_007E(); 
extern int  accept(); 
extern int  access(); 
extern int  add_mtd_device(); 
extern int  alarm(); 
extern int  alarmd_create(); 
extern int  atomic_add(); 
extern int  atomic_dec(); 
extern int  atomic_dec_and_test(); 
extern int  atomic_dec_return(); 
extern int  atomic_inc(); 
extern int  atomic_inc_and_test(); 
extern int  atomic_inc_return(); 
extern int  atomic_read(); 
extern int  atomic_set(); 
extern int  atomic_sub(); 
extern int  atomic_sub_and_test(); 
extern int  atomic_sub_return(); 
extern int  audio_init(); 
extern int  bcopy(); 
extern int  bind(); 
extern int  bmSeggerLogo(); 
extern int  bmSeggerLogo70x35(); 
extern int  board_nand_init(); 
extern int  bsp_devices_create(); 
extern int  bsp_drivers_install(); 
extern int  bsp_mem_map(); 
extern int  bsp_resv_space(); 
extern int  ceil(); 
extern int  chdir(); 
extern int  clock_init(); 
extern int  close(); 
extern int  closedir(); 
extern int  clust2sect(); 
extern int  connect(); 
extern int  cpu_kernel_init(); 
extern int  cpu_mem_map(); 
extern int  cpu_reset_init(); 
extern int  cpu_resv_space(); 
extern int  creat(); 
extern int  dabt_c_handler(); 
extern int  del_mtd_device(); 
extern int  device_create(); 
extern int  device_get(); 
extern int  device_lookup(); 
extern int  device_manager_init(); 
extern int  device_remove(); 
extern int  disk_initialize(); 
extern int  disk_ioctl(); 
extern int  disk_read(); 
extern int  disk_status(); 
extern int  disk_write(); 
extern int  do_bind(); 
extern int  do_close(); 
extern int  do_connect(); 
extern int  do_delconn(); 
extern int  do_disconnect(); 
extern int  do_getaddr(); 
extern int  do_listen(); 
extern int  do_netifapi_netif_add(); 
extern int  do_netifapi_netif_common(); 
extern int  do_netifapi_netif_set_addr(); 
extern int  do_newconn(); 
extern int  do_recv(); 
extern int  do_send(); 
extern int  do_write(); 
extern int  driver_install(); 
extern int  driver_lookup(); 
extern int  driver_manager_init(); 
extern int  driver_ref_by_name(); 
extern int  driver_remove(); 
extern int  dup(); 
extern int  dup2(); 
extern int  etharp_add_static_entry(); 
extern int  etharp_cleanup_netif(); 
extern int  etharp_find_addr(); 
extern int  etharp_output(); 
extern int  etharp_query(); 
extern int  etharp_remove_static_entry(); 
extern int  etharp_request(); 
extern int  etharp_tmr(); 
extern int  ethbroadcast(); 
extern int  ethernet_input(); 
extern int  ethernetif_init(); 
extern int  ethzero(); 
extern int  f_chmod(); 
extern int  f_close(); 
extern int  f_getfree(); 
extern int  f_lseek(); 
extern int  f_mkdir(); 
extern int  f_mkfs(); 
extern int  f_mount(); 
extern int  f_open(); 
extern int  f_opendir(); 
extern int  f_read(); 
extern int  f_readdir(); 
extern int  f_rename(); 
extern int  f_stat(); 
extern int  f_sync(); 
extern int  f_truncate(); 
extern int  f_unlink(); 
extern int  f_utime(); 
extern int  f_write(); 
extern int  fatfs_file_sem_lock(); 
extern int  fb_create(); 
extern int  fb_init(); 
extern int  fclose(); 
extern int  fcntl(); 
extern int  fdopen(); 
extern int  ff_cre_syncobj(); 
extern int  ff_del_syncobj(); 
extern int  ff_memalloc(); 
extern int  ff_memfree(); 
extern int  ff_rel_grant(); 
extern int  ff_req_grant(); 
extern int  fflush(); 
extern int  fifo_create(); 
extern int  fifo_init(); 
extern int  file_system_install(); 
extern int  file_system_lookup(); 
extern int  file_system_manager_init(); 
extern int  file_system_remove(); 
extern int  fiprintf(); 
extern int  fiq_c_handler(); 
extern int  floor(); 
extern int  fprintf(); 
extern int  fputwc(); 
extern int  free(); 
extern int  fstat(); 
extern int  get_fat(); 
extern int  get_fattime(); 
extern int  get_mtd_device(); 
extern int  get_mtd_device_nm(); 
extern int  getcwd(); 
extern int  getsockopt(); 
extern int  getticks(); 
extern int  gettid(); 
extern int  heap_alloc(); 
extern int  heap_free(); 
extern int  heap_init(); 
extern int  icmp_dest_unreach(); 
extern int  icmp_input(); 
extern int  icmp_time_exceeded(); 
extern int  in_interrupt(); 
extern int  in_kernel(); 
extern int  index(); 
extern int  inet_chksum(); 
extern int  inet_chksum_pbuf(); 
extern int  inet_chksum_pseudo(); 
extern int  inet_chksum_pseudo_partial(); 
extern int  interrupt_disable(); 
extern int  interrupt_enter(); 
extern int  interrupt_exit(); 
extern int  interrupt_init(); 
extern int  interrupt_install(); 
extern int  interrupt_mask(); 
extern int  interrupt_resume(); 
extern int  interrupt_unmask(); 
extern int  ioctl(); 
extern int  ip4_addr_isbroadcast(); 
extern int  ip4_addr_netmask_valid(); 
extern int  ip_addr_any(); 
extern int  ip_addr_broadcast(); 
extern int  ip_frag(); 
extern int  ip_input(); 
extern int  ip_output(); 
extern int  ip_output_hinted(); 
extern int  ip_output_if(); 
extern int  ip_reass(); 
extern int  ip_reass_tmr(); 
extern int  ip_route(); 
extern int  ipaddr_addr(); 
extern int  ipaddr_aton(); 
extern int  ipaddr_ntoa(); 
extern int  ipaddr_ntoa_r(); 
extern int  ipc_task_cleanup(); 
extern int  ipc_task_init(); 
extern int  irq_c_handler(); 
extern int  isatty(); 
extern int  isr_invaild(); 
extern int  iswspace(); 
extern int  kcomplain(); 
extern int  kernel_enter(); 
extern int  kernel_init(); 
extern int  kernel_start(); 
extern int  kernel_timer(); 
extern int  kheap_create(); 
extern int  kidle_create(); 
extern int  kill(); 
extern int  klogd_create(); 
extern int  kputc(); 
extern int  kthread_create(); 
extern int  lcd_init(); 
extern int  listen(); 
extern int  localeconv(); 
extern int  lwip_accept(); 
extern int  lwip_bind(); 
extern int  lwip_chksum_copy(); 
extern int  lwip_close(); 
extern int  lwip_connect(); 
extern int  lwip_fcntl(); 
extern int  lwip_getpeername(); 
extern int  lwip_getsockname(); 
extern int  lwip_getsockopt(); 
extern int  lwip_htonl(); 
extern int  lwip_htons(); 
extern int  lwip_init(); 
extern int  lwip_ioctl(); 
extern int  lwip_listen(); 
extern int  lwip_ntohl(); 
extern int  lwip_ntohs(); 
extern int  lwip_read(); 
extern int  lwip_recv(); 
extern int  lwip_recvfrom(); 
extern int  lwip_select(); 
extern int  lwip_send(); 
extern int  lwip_sendto(); 
extern int  lwip_setsockopt(); 
extern int  lwip_shutdown(); 
extern int  lwip_socket(); 
extern int  lwip_socket_init(); 
extern int  lwip_socket_set_ctx(); 
extern int  lwip_strerr(); 
extern int  lwip_write(); 
extern int  main(); 
extern int  malloc(); 
extern int  matherr(); 
extern int  mbrtowc(); 
extern int  mem_calloc(); 
extern int  mem_free(); 
extern int  mem_free_callback(); 
extern int  mem_init(); 
extern int  mem_malloc(); 
extern int  mem_trim(); 
extern int  memchr(); 
extern int  memcmp(); 
extern int  memcpy(); 
extern int  memmove(); 
extern int  memp_free(); 
extern int  memp_init(); 
extern int  memp_malloc(); 
extern int  memrchr(); 
extern int  memset(); 
extern int  mkdir(); 
extern int  mmu_clean_dcache_index(); 
extern int  mmu_clean_invalidate_dcache_index(); 
extern int  mmu_clean_invalidate_dcache_mva(); 
extern int  mmu_disable(); 
extern int  mmu_disable_align_fault_check(); 
extern int  mmu_disable_dcache(); 
extern int  mmu_disable_icache(); 
extern int  mmu_drain_write_buffer(); 
extern int  mmu_enable(); 
extern int  mmu_enable_align_fault_check(); 
extern int  mmu_enable_dcache(); 
extern int  mmu_enable_icache(); 
extern int  mmu_get_cache_type(); 
extern int  mmu_get_cpu_id(); 
extern int  mmu_get_data_fault_status(); 
extern int  mmu_get_fault_address(); 
extern int  mmu_get_prefetch_fault_status(); 
extern int  mmu_init(); 
extern int  mmu_invalidate_dcache(); 
extern int  mmu_invalidate_dtlb(); 
extern int  mmu_invalidate_dtlb_mva(); 
extern int  mmu_invalidate_icache(); 
extern int  mmu_invalidate_icache_dcache(); 
extern int  mmu_invalidate_icache_mva(); 
extern int  mmu_invalidate_itlb(); 
extern int  mmu_invalidate_itlb_dtlb(); 
extern int  mmu_invalidate_itlb_mva(); 
extern int  mmu_map_page(); 
extern int  mmu_map_region(); 
extern int  mmu_map_section(); 
extern int  mmu_map_section_as_page(); 
extern int  mmu_set_domain(); 
extern int  mmu_set_sys_rom_protect_bit(); 
extern int  mmu_set_ttb(); 
extern int  mmu_set_vector_addr(); 
extern int  mmu_unmap_section(); 
extern int  mmu_wait_for_interrupt(); 
extern int  module_init(); 
extern int  module_load(); 
extern int  module_lookup(); 
extern int  module_ref_by_addr(); 
extern int  module_unload(); 
extern int  module_unref(); 
extern int  mount_point_get(); 
extern int  mount_point_install(); 
extern int  mount_point_lookup(); 
extern int  mount_point_manager_init(); 
extern int  mount_point_remove(); 
extern int  mqueue_abort(); 
extern int  mqueue_abort_fetch(); 
extern int  mqueue_abort_post(); 
extern int  mqueue_fetch(); 
extern int  mqueue_flush(); 
extern int  mqueue_free(); 
extern int  mqueue_msg_nr(); 
extern int  mqueue_new(); 
extern int  mqueue_post(); 
extern int  mqueue_set_valid(); 
extern int  mqueue_tryfetch(); 
extern int  mqueue_trypost(); 
extern int  mqueue_valid(); 
extern int  msleep(); 
extern int  mtdblock_create(); 
extern int  mtdblock_init(); 
extern int  mutex_abort(); 
extern int  mutex_free(); 
extern int  mutex_lock(); 
extern int  mutex_new(); 
extern int  mutex_set_valid(); 
extern int  mutex_trylock(); 
extern int  mutex_unlock(); 
extern int  mutex_valid(); 
extern int  mva_to_va(); 
extern int  nand_calculate_ecc(); 
extern int  nand_correct_data(); 
extern int  nand_default_bbt(); 
extern int  nand_erase_nand(); 
extern int  nand_init(); 
extern int  nand_isbad_bbt(); 
extern int  nand_release(); 
extern int  nand_scan(); 
extern int  nand_scan_bbt(); 
extern int  nand_scan_ident(); 
extern int  nand_scan_tail(); 
extern int  nand_update_bbt(); 
extern int  nand_wait_ready(); 
extern int  nandmtd1_MarkNANDBlockBad(); 
extern int  nandmtd1_QueryNANDBlock(); 
extern int  nandmtd1_ReadChunkWithTagsFromNAND(); 
extern int  nandmtd1_WriteChunkWithTagsToNAND(); 
extern int  nandmtd2_MarkNANDBlockBad(); 
extern int  nandmtd2_QueryNANDBlock(); 
extern int  nandmtd2_ReadChunkWithTagsFromNAND(); 
extern int  nandmtd2_WriteChunkWithTagsToNAND(); 
extern int  nandmtd_EraseBlockInNAND(); 
extern int  nandmtd_InitialiseNAND(); 
extern int  nanf(); 
extern int  netbuf_alloc(); 
extern int  netbuf_chain(); 
extern int  netbuf_data(); 
extern int  netbuf_delete(); 
extern int  netbuf_first(); 
extern int  netbuf_free(); 
extern int  netbuf_new(); 
extern int  netbuf_next(); 
extern int  netbuf_ref(); 
extern int  netconn_accept(); 
extern int  netconn_alloc(); 
extern int  netconn_bind(); 
extern int  netconn_close(); 
extern int  netconn_connect(); 
extern int  netconn_delete(); 
extern int  netconn_disconnect(); 
extern int  netconn_free(); 
extern int  netconn_getaddr(); 
extern int  netconn_listen_with_backlog(); 
extern int  netconn_new_with_proto_and_callback(); 
extern int  netconn_recv(); 
extern int  netconn_recv_tcp_pbuf(); 
extern int  netconn_recved(); 
extern int  netconn_send(); 
extern int  netconn_sendto(); 
extern int  netconn_shutdown(); 
extern int  netconn_write_partly(); 
extern int  netif_add(); 
extern int  netif_find(); 
extern int  netif_init(); 
extern int  netif_loop_output(); 
extern int  netif_poll(); 
extern int  netif_remove(); 
extern int  netif_set_addr(); 
extern int  netif_set_default(); 
extern int  netif_set_down(); 
extern int  netif_set_gw(); 
extern int  netif_set_ipaddr(); 
extern int  netif_set_link_callback(); 
extern int  netif_set_link_down(); 
extern int  netif_set_link_up(); 
extern int  netif_set_netmask(); 
extern int  netif_set_remove_callback(); 
extern int  netif_set_status_callback(); 
extern int  netif_set_up(); 
extern int  netifapi_netif_add(); 
extern int  netifapi_netif_common(); 
extern int  netifapi_netif_set_addr(); 
extern int  netjob_add(); 
extern int  netjob_create(); 
extern int  null_init(); 
extern int  open(); 
extern int  opendir(); 
extern int  pabt_c_handler(); 
extern int  pause(); 
extern int  pbuf_alloc(); 
extern int  pbuf_alloced_custom(); 
extern int  pbuf_cat(); 
extern int  pbuf_chain(); 
extern int  pbuf_clen(); 
extern int  pbuf_coalesce(); 
extern int  pbuf_copy(); 
extern int  pbuf_copy_partial(); 
extern int  pbuf_dechain(); 
extern int  pbuf_fill_chksum(); 
extern int  pbuf_free(); 
extern int  pbuf_free_callback(); 
extern int  pbuf_get_at(); 
extern int  pbuf_header(); 
extern int  pbuf_memcmp(); 
extern int  pbuf_memfind(); 
extern int  pbuf_realloc(); 
extern int  pbuf_ref(); 
extern int  pbuf_strstr(); 
extern int  pbuf_take(); 
extern int  poll(); 
extern int  printf(); 
extern int  printk(); 
extern int  process0_enter(); 
extern int  process_create(); 
extern int  put_fat(); 
extern int  put_mtd_device(); 
extern int  putenv(); 
extern int  raise(); 
extern int  ramdisk_create(); 
extern int  ramdisk_init(); 
extern int  rand(); 
extern int  raw_bind(); 
extern int  raw_connect(); 
extern int  raw_input(); 
extern int  raw_new(); 
extern int  raw_recv(); 
extern int  raw_remove(); 
extern int  raw_send(); 
extern int  raw_sendto(); 
extern int  read(); 
extern int  readdir(); 
extern int  recv(); 
extern int  recvfrom(); 
extern int  restore_core_regs(); 
extern int  rewinddir(); 
extern int  rmdir(); 
extern int  schedule(); 
extern int  seekdir(); 
extern int  select(); 
extern int  select_helper(); 
extern int  select_init(); 
extern int  select_report(); 
extern int  select_select(); 
extern int  select_unselect(); 
extern int  sem_abort(); 
extern int  sem_free(); 
extern int  sem_new(); 
extern int  sem_set_valid(); 
extern int  sem_signal(); 
extern int  sem_sync(); 
extern int  sem_trywait(); 
extern int  sem_valid(); 
extern int  sem_wait(); 
extern int  send(); 
extern int  sendto(); 
extern int  serial0_init(); 
extern int  setlocale(); 
extern int  setpinfo(); 
extern int  setsockopt(); 
extern int  sharemem_create(); 
extern int  sharemem_init(); 
extern int  shutdown(); 
extern int  signal(); 
extern int  signal_action(); 
extern int  signal_handler_exit(); 
extern int  signal_pending(); 
extern int  signal_procmask(); 
extern int  signal_queue(); 
extern int  signal_schedule(); 
extern int  signal_signal(); 
extern int  signal_suspend(); 
extern int  signal_switch(); 
extern int  signal_task_cleanup(); 
extern int  signal_task_init(); 
extern int  sigprocmask(); 
extern int  sigsuspend(); 
extern int  sleep(); 
extern int  smileos_socket_report(); 
extern int  snprintf(); 
extern int  soc_devices_create(); 
extern int  soc_drivers_install(); 
extern int  socket(); 
extern int  socket_attach(); 
extern int  socket_init(); 
extern int  socket_priv_fd(); 
extern int  socket_stat(); 
extern int  sprintf(); 
extern int  sqrt(); 
extern int  srand(); 
extern int  sscanf(); 
extern int  stat(); 
extern int  stats_display(); 
extern int  stats_display_mem(); 
extern int  stats_display_memp(); 
extern int  stats_display_proto(); 
extern int  stats_display_sys(); 
extern int  stats_init(); 
extern int  strcat(); 
extern int  strchr(); 
extern int  strchrnul(); 
extern int  strcmp(); 
extern int  strcpy(); 
extern int  strdup(); 
extern int  strlcpy(); 
extern int  strlen(); 
extern int  strncmp(); 
extern int  strncpy(); 
extern int  strndup(); 
extern int  strnlen(); 
extern int  strtod(); 
extern int  strtof(); 
extern int  strtol(); 
extern int  strtoul(); 
extern int  symbol_init(); 
extern int  symbol_lookup(); 
extern int  sys_arch_mbox_fetch(); 
extern int  sys_arch_mbox_tryfetch(); 
extern int  sys_arch_sem_wait(); 
extern int  sys_init(); 
extern int  sys_jiffies(); 
extern int  sys_mbox_free(); 
extern int  sys_mbox_new(); 
extern int  sys_mbox_post(); 
extern int  sys_mbox_set_invalid(); 
extern int  sys_mbox_trypost(); 
extern int  sys_mbox_valid(); 
extern int  sys_msleep(); 
extern int  sys_mutex_free(); 
extern int  sys_mutex_lock(); 
extern int  sys_mutex_new(); 
extern int  sys_mutex_set_invalid(); 
extern int  sys_mutex_unlock(); 
extern int  sys_mutex_valid(); 
extern int  sys_now(); 
extern int  sys_resv_space(); 
extern int  sys_sem_free(); 
extern int  sys_sem_new(); 
extern int  sys_sem_set_invalid(); 
extern int  sys_sem_signal(); 
extern int  sys_sem_valid(); 
extern int  sys_thread_new(); 
extern int  sys_timeout(); 
extern int  sys_timeouts_init(); 
extern int  sys_timeouts_mbox_fetch(); 
extern int  sys_untimeout(); 
extern int  task_alarm(); 
extern int  task_cleanup(); 
extern int  task_getpid(); 
extern int  task_pause(); 
extern int  task_schedule(); 
extern int  task_setpinfo(); 
extern int  task_sleep(); 
extern int  task_switch(); 
extern int  task_switch_hook(); 
extern int  task_switch_to(); 
extern int  tcp_abandon(); 
extern int  tcp_abort(); 
extern int  tcp_accept(); 
extern int  tcp_alloc(); 
extern int  tcp_arg(); 
extern int  tcp_backoff(); 
extern int  tcp_bind(); 
extern int  tcp_close(); 
extern int  tcp_connect(); 
extern int  tcp_debug_state_str(); 
extern int  tcp_eff_send_mss(); 
extern int  tcp_enqueue_flags(); 
extern int  tcp_err(); 
extern int  tcp_fasttmr(); 
extern int  tcp_init(); 
extern int  tcp_input(); 
extern int  tcp_keepalive(); 
extern int  tcp_listen_with_backlog(); 
extern int  tcp_new(); 
extern int  tcp_next_iss(); 
extern int  tcp_output(); 
extern int  tcp_pcb_lists(); 
extern int  tcp_pcb_purge(); 
extern int  tcp_pcb_remove(); 
extern int  tcp_persist_backoff(); 
extern int  tcp_poll(); 
extern int  tcp_process_refused_data(); 
extern int  tcp_recv(); 
extern int  tcp_recv_null(); 
extern int  tcp_recved(); 
extern int  tcp_rexmit(); 
extern int  tcp_rexmit_fast(); 
extern int  tcp_rexmit_rto(); 
extern int  tcp_rst(); 
extern int  tcp_seg_copy(); 
extern int  tcp_seg_free(); 
extern int  tcp_segs_free(); 
extern int  tcp_send_empty_ack(); 
extern int  tcp_send_fin(); 
extern int  tcp_sent(); 
extern int  tcp_setprio(); 
extern int  tcp_shutdown(); 
extern int  tcp_slowtmr(); 
extern int  tcp_state_str(); 
extern int  tcp_timer_needed(); 
extern int  tcp_tmr(); 
extern int  tcp_update_rcv_ann_wnd(); 
extern int  tcp_write(); 
extern int  tcp_zero_window_probe(); 
extern int  tcpip_apimsg(); 
extern int  tcpip_apimsg_lock(); 
extern int  tcpip_callback_with_block(); 
extern int  tcpip_callbackmsg_delete(); 
extern int  tcpip_callbackmsg_new(); 
extern int  tcpip_init(); 
extern int  tcpip_input(); 
extern int  tcpip_netifapi_lock(); 
extern int  tcpip_timeout(); 
extern int  tcpip_trycallback(); 
extern int  tcpip_untimeout(); 
extern int  telldir(); 
extern int  timer_init(); 
extern int  tzset(); 
extern int  uart_init(); 
extern int  udp_bind(); 
extern int  udp_connect(); 
extern int  udp_disconnect(); 
extern int  udp_init(); 
extern int  udp_input(); 
extern int  udp_new(); 
extern int  udp_recv(); 
extern int  udp_remove(); 
extern int  udp_send(); 
extern int  udp_send_chksum(); 
extern int  udp_sendto(); 
extern int  udp_sendto_chksum(); 
extern int  udp_sendto_if(); 
extern int  udp_sendto_if_chksum(); 
extern int  undf_c_handler(); 
extern int  ungetc(); 
extern int  usleep(); 
extern int  va_to_mva(); 
extern int  vfiprintf(); 
extern int  vfprintf(); 
extern int  vfs_access(); 
extern int  vfs_chdir(); 
extern int  vfs_close(); 
extern int  vfs_closedir(); 
extern int  vfs_dup(); 
extern int  vfs_dup2(); 
extern int  vfs_fcntl(); 
extern int  vfs_fdatasync(); 
extern int  vfs_fstat(); 
extern int  vfs_fsync(); 
extern int  vfs_ftruncate(); 
extern int  vfs_get_file(); 
extern int  vfs_getcwd(); 
extern int  vfs_init(); 
extern int  vfs_ioctl(); 
extern int  vfs_isatty(); 
extern int  vfs_link(); 
extern int  vfs_lseek(); 
extern int  vfs_mkdir(); 
extern int  vfs_mkfs(); 
extern int  vfs_mount(); 
extern int  vfs_mount_point_create(); 
extern int  vfs_open(); 
extern int  vfs_opendir(); 
extern int  vfs_path_add_mount_point(); 
extern int  vfs_path_normalization(); 
extern int  vfs_read(); 
extern int  vfs_readdir(); 
extern int  vfs_rename(); 
extern int  vfs_rewinddir(); 
extern int  vfs_rmdir(); 
extern int  vfs_seekdir(); 
extern int  vfs_select(); 
extern int  vfs_stat(); 
extern int  vfs_sync(); 
extern int  vfs_task_cleanup(); 
extern int  vfs_task_init(); 
extern int  vfs_telldir(); 
extern int  vfs_truncate(); 
extern int  vfs_unlink(); 
extern int  vfs_unmount(); 
extern int  vfs_write(); 
extern int  vmm_init(); 
extern int  vmm_page_map(); 
extern int  vmm_process_cleanup(); 
extern int  vmm_process_init(); 
extern int  vsnprintf(); 
extern int  vspace_usable(); 
extern int  warn(); 
extern int  wcrtomb(); 
extern int  xmalloc(); 
extern int  xstrdup(); 
extern int  xstrndup(); 
extern int  xxx_init(); 
extern int  xzalloc(); 
extern int  yaffs_CalcECC(); 
extern int  yaffs_CalcTagsECC(); 
extern int  yaffs_CheckECCOnTags(); 
extern int  yaffs_CheckFF(); 
extern int  yaffs_CheckpointClose(); 
extern int  yaffs_CheckpointInvalidateStream(); 
extern int  yaffs_CheckpointOpen(); 
extern int  yaffs_CheckpointRead(); 
extern int  yaffs_CheckpointRestore(); 
extern int  yaffs_CheckpointSave(); 
extern int  yaffs_CheckpointWrite(); 
extern int  yaffs_CountBits(); 
extern int  yaffs_Deinitialise(); 
extern int  yaffs_DeleteChunk(); 
extern int  yaffs_DeleteFile(); 
extern int  yaffs_DeleteObject(); 
extern int  yaffs_DoWriteDataToFile(); 
extern int  yaffs_Dofsync(); 
extern int  yaffs_DumpDevStruct(); 
extern int  yaffs_ECCCalculate(); 
extern int  yaffs_ECCCalculateOther(); 
extern int  yaffs_ECCCorrect(); 
extern int  yaffs_ECCCorrectOther(); 
extern int  yaffs_EraseBlockInNAND(); 
extern int  yaffs_FindObjectByName(); 
extern int  yaffs_FindObjectByNumber(); 
extern int  yaffs_FlushEntireDeviceCache(); 
extern int  yaffs_FlushFile(); 
extern int  yaffs_GetAttributes(); 
extern int  yaffs_GetCheckpointSum(); 
extern int  yaffs_GetEquivalentObject(); 
extern int  yaffs_GetFileSize(); 
extern int  yaffs_GetNumberOfFreeChunks(); 
extern int  yaffs_GetObjectFileLength(); 
extern int  yaffs_GetObjectInode(); 
extern int  yaffs_GetObjectLinkCount(); 
extern int  yaffs_GetObjectName(); 
extern int  yaffs_GetObjectType(); 
extern int  yaffs_GetSymlinkAlias(); 
extern int  yaffs_GetTempBuffer(); 
extern int  yaffs_GutsInitialise(); 
extern int  yaffs_HandleChunkError(); 
extern int  yaffs_HandleDeferedFree(); 
extern int  yaffs_InitialiseNAND(); 
extern int  yaffs_InitialiseTags(); 
extern int  yaffs_IsManagedTempBuffer(); 
extern int  yaffs_Link(); 
extern int  yaffs_LoadLevel0Tnode(); 
extern int  yaffs_LostNFound(); 
extern int  yaffs_MarkBlockBad(); 
extern int  yaffs_MknodDirectory(); 
extern int  yaffs_MknodFile(); 
extern int  yaffs_MknodSpecial(); 
extern int  yaffs_MknodSymLink(); 
extern int  yaffs_PackTags1(); 
extern int  yaffs_PackTags2(); 
extern int  yaffs_PackTags2TagsPart(); 
extern int  yaffs_QueryInitialBlockState(); 
extern int  yaffs_ReadChunkWithTagsFromNAND(); 
extern int  yaffs_ReadDataFromFile(); 
extern int  yaffs_ReleaseTempBuffer(); 
extern int  yaffs_RenameObject(); 
extern int  yaffs_ResizeFile(); 
extern int  yaffs_Root(); 
extern int  yaffs_SetAttributes(); 
extern int  yaffs_TagsCompatabilityMarkNANDBlockBad(); 
extern int  yaffs_TagsCompatabilityQueryNANDBlock(); 
extern int  yaffs_TagsCompatabilityReadChunkWithTagsFromNAND(); 
extern int  yaffs_TagsCompatabilityWriteChunkWithTagsToNAND(); 
extern int  yaffs_Unlink(); 
extern int  yaffs_UnpackTags1(); 
extern int  yaffs_UnpackTags2(); 
extern int  yaffs_UnpackTags2TagsPart(); 
extern int  yaffs_UpdateDirtyDirectories(); 
extern int  yaffs_ValidateTags(); 
extern int  yaffs_WriteChunkWithTagsToNAND(); 
extern int  yaffs_WriteDataToFile(); 
extern int  yaffs_access(); 
extern int  yaffs_chmod(); 
extern int  yaffs_close(); 
extern int  yaffs_closedir(); 
extern int  yaffs_dup(); 
extern int  yaffs_fchmod(); 
extern int  yaffs_fdatasync(); 
extern int  yaffs_flush(); 
extern int  yaffs_free(); 
extern int  yaffs_freespace(); 
extern int  yaffs_fstat(); 
extern int  yaffs_fsync(); 
extern int  yaffs_ftruncate(); 
extern int  yaffs_initialise(); 
extern int  yaffs_inodecount(); 
extern int  yaffs_link(); 
extern int  yaffs_lseek(); 
extern int  yaffs_lstat(); 
extern int  yaffs_malloc(); 
extern int  yaffs_mkdir(); 
extern int  yaffs_mknod(); 
extern int  yaffs_mount(); 
extern int  yaffs_mount2(); 
extern int  yaffs_open(); 
extern int  yaffs_opendir(); 
extern int  yaffs_pread(); 
extern int  yaffs_pwrite(); 
extern int  yaffs_qsort(); 
extern int  yaffs_read(); 
extern int  yaffs_readdir(); 
extern int  yaffs_readlink(); 
extern int  yaffs_remount(); 
extern int  yaffs_rename(); 
extern int  yaffs_rewinddir(); 
extern int  yaffs_rmdir(); 
extern int  yaffs_stat(); 
extern int  yaffs_symlink(); 
extern int  yaffs_sync(); 
extern int  yaffs_totalspace(); 
extern int  yaffs_truncate(); 
extern int  yaffs_unlink(); 
extern int  yaffs_unmount(); 
extern int  yaffs_unmount2(); 
extern int  yaffs_write(); 
extern int  yaffsfs_CurrentTime(); 
extern int  yaffsfs_DoUnlink(); 
extern int  yaffsfs_FindDevice(); 
extern int  yaffsfs_GetHandleObject(); 
extern int  yaffsfs_GetHandlePointer(); 
extern int  yaffsfs_GetInodePointer(); 
extern int  yaffsfs_Init(); 
extern int  yaffsfs_InitHandles(); 
extern int  yaffsfs_IsPathDivider(); 
extern int  yaffsfs_Lock(); 
extern int  yaffsfs_Match(); 
extern int  yaffsfs_Mount(); 
extern int  yaffsfs_RemoveObjectCallback(); 
extern int  yaffsfs_SetError(); 
extern int  yaffsfs_Unlock(); 
extern int  yaffsfs_Unmount(); 
extern int  yaffsfs_do_read(); 
extern int  yaffsfs_do_write(); 
extern int  zero_init(); 
extern int  __gnu_thumb1_case_sqi(); 
extern int  __gnu_thumb1_case_uqi(); 
extern int  __gnu_thumb1_case_shi(); 
extern int  __gnu_thumb1_case_uhi(); 
extern int  __gnu_thumb1_case_si(); 
extern int  __aeabi_uidiv(); 
extern int  __aeabi_uidivmod(); 
extern int  __udivsi3(); 
extern int  __aeabi_idiv(); 
extern int  __aeabi_idivmod(); 
extern int  __divsi3(); 
extern int  __umodsi3(); 
extern int  __modsi3(); 
extern int  _call_via_fp(); 
extern int  _call_via_ip(); 
extern int  _call_via_lr(); 
extern int  _call_via_r0(); 
extern int  _call_via_r1(); 
extern int  _call_via_r2(); 
extern int  _call_via_r3(); 
extern int  _call_via_r4(); 
extern int  _call_via_r5(); 
extern int  _call_via_r6(); 
extern int  _call_via_r7(); 
extern int  _call_via_r8(); 
extern int  _call_via_r9(); 
extern int  _call_via_sl(); 
extern int  _call_via_sp(); 
extern int  _arm_return(); 
extern int  _arm_return_r11(); 
extern int  _arm_return_r7(); 
extern int  _interwork_call_via_fp(); 
extern int  _interwork_call_via_ip(); 
extern int  _interwork_call_via_lr(); 
extern int  _interwork_call_via_r0(); 
extern int  _interwork_call_via_r1(); 
extern int  _interwork_call_via_r2(); 
extern int  _interwork_call_via_r3(); 
extern int  _interwork_call_via_r4(); 
extern int  _interwork_call_via_r5(); 
extern int  _interwork_call_via_r6(); 
extern int  _interwork_call_via_r7(); 
extern int  _interwork_call_via_r8(); 
extern int  _interwork_call_via_r9(); 
extern int  _interwork_call_via_sl(); 
extern int  _interwork_call_via_sp(); 
extern int  _interwork_r11_call_via_fp(); 
extern int  _interwork_r11_call_via_ip(); 
extern int  _interwork_r11_call_via_r0(); 
extern int  _interwork_r11_call_via_r1(); 
extern int  _interwork_r11_call_via_r2(); 
extern int  _interwork_r11_call_via_r3(); 
extern int  _interwork_r11_call_via_r4(); 
extern int  _interwork_r11_call_via_r5(); 
extern int  _interwork_r11_call_via_r6(); 
extern int  _interwork_r11_call_via_r7(); 
extern int  _interwork_r11_call_via_r8(); 
extern int  _interwork_r11_call_via_r9(); 
extern int  _interwork_r11_call_via_sl(); 
extern int  _interwork_r11_call_via_sp(); 
extern int  _interwork_r7_call_via_fp(); 
extern int  _interwork_r7_call_via_ip(); 
extern int  _interwork_r7_call_via_r0(); 
extern int  _interwork_r7_call_via_r1(); 
extern int  _interwork_r7_call_via_r2(); 
extern int  _interwork_r7_call_via_r3(); 
extern int  _interwork_r7_call_via_r4(); 
extern int  _interwork_r7_call_via_r5(); 
extern int  _interwork_r7_call_via_r6(); 
extern int  _interwork_r7_call_via_r7(); 
extern int  _interwork_r7_call_via_r8(); 
extern int  _interwork_r7_call_via_r9(); 
extern int  _interwork_r7_call_via_sl(); 
extern int  _interwork_r7_call_via_sp(); 
extern int  __aeabi_llsr(); 
extern int  __lshrdi3(); 
extern int  __aeabi_lasr(); 
extern int  __ashrdi3(); 
extern int  __aeabi_llsl(); 
extern int  __ashldi3(); 
extern int  __aeabi_dneg(); 
extern int  __negdf2(); 
extern int  __adddf3(); 
extern int  __aeabi_dadd(); 
extern int  __aeabi_drsub(); 
extern int  __aeabi_dsub(); 
extern int  __aeabi_f2d(); 
extern int  __aeabi_i2d(); 
extern int  __aeabi_l2d(); 
extern int  __aeabi_ui2d(); 
extern int  __aeabi_ul2d(); 
extern int  __extendsfdf2(); 
extern int  __floatdidf(); 
extern int  __floatsidf(); 
extern int  __floatundidf(); 
extern int  __floatunsidf(); 
extern int  __subdf3(); 
extern int  __aeabi_ddiv(); 
extern int  __aeabi_dmul(); 
extern int  __divdf3(); 
extern int  __muldf3(); 
extern int  __aeabi_cdcmpeq(); 
extern int  __aeabi_cdcmple(); 
extern int  __aeabi_cdrcmple(); 
extern int  __aeabi_dcmpeq(); 
extern int  __aeabi_dcmpge(); 
extern int  __aeabi_dcmpgt(); 
extern int  __aeabi_dcmple(); 
extern int  __aeabi_dcmplt(); 
extern int  __cmpdf2(); 
extern int  __eqdf2(); 
extern int  __gedf2(); 
extern int  __gtdf2(); 
extern int  __ledf2(); 
extern int  __ltdf2(); 
extern int  __nedf2(); 
extern int  __aeabi_dcmpun(); 
extern int  __unorddf2(); 
extern int  __aeabi_d2iz(); 
extern int  __fixdfsi(); 
extern int  __aeabi_d2uiz(); 
extern int  __fixunsdfsi(); 
extern int  __aeabi_d2f(); 
extern int  __truncdfsf2(); 
extern int  __aeabi_fneg(); 
extern int  __negsf2(); 
extern int  __addsf3(); 
extern int  __aeabi_fadd(); 
extern int  __aeabi_frsub(); 
extern int  __aeabi_fsub(); 
extern int  __aeabi_i2f(); 
extern int  __aeabi_l2f(); 
extern int  __aeabi_ui2f(); 
extern int  __aeabi_ul2f(); 
extern int  __floatdisf(); 
extern int  __floatsisf(); 
extern int  __floatundisf(); 
extern int  __floatunsisf(); 
extern int  __subsf3(); 
extern int  __aeabi_fdiv(); 
extern int  __aeabi_fmul(); 
extern int  __divsf3(); 
extern int  __mulsf3(); 
extern int  __aeabi_cfcmpeq(); 
extern int  __aeabi_cfcmple(); 
extern int  __aeabi_cfrcmple(); 
extern int  __aeabi_fcmpeq(); 
extern int  __aeabi_fcmpge(); 
extern int  __aeabi_fcmpgt(); 
extern int  __aeabi_fcmple(); 
extern int  __aeabi_fcmplt(); 
extern int  __cmpsf2(); 
extern int  __eqsf2(); 
extern int  __gesf2(); 
extern int  __gtsf2(); 
extern int  __lesf2(); 
extern int  __ltsf2(); 
extern int  __nesf2(); 
extern int  __aeabi_fcmpun(); 
extern int  __unordsf2(); 
extern int  __aeabi_f2iz(); 
extern int  __fixsfsi(); 
extern int  __aeabi_f2uiz(); 
extern int  __fixunssfsi(); 
extern int  __clzsi2(); 
extern int  __clzdi2(); 
extern int  __aeabi_lcmp(); 
extern int  __aeabi_ulcmp(); 
extern int  __aeabi_ldivmod(); 
extern int  __aeabi_uldivmod(); 
extern int  __aeabi_lmul(); 
extern int  __muldi3(); 
extern int  __negdi2(); 
extern int  __cmpdi2(); 
extern int  __ucmpdi2(); 
extern int  __clear_cache(); 
extern int  __enable_execute_stack(); 
extern int  __absvsi2(); 
extern int  __absvdi2(); 
extern int  __addvsi3(); 
extern int  __addvdi3(); 
extern int  __subvsi3(); 
extern int  __subvdi3(); 
extern int  __mulvsi3(); 
extern int  __mulvdi3(); 
extern int  __negvsi2(); 
extern int  __negvdi2(); 
extern int  __ffssi2(); 
extern int  __ffsdi2(); 
extern int  __ctzsi2(); 
extern int  __ctzdi2(); 
extern int  __popcountsi2(); 
extern int  __popcountdi2(); 
extern int  __paritysi2(); 
extern int  __paritydi2(); 
extern int  __powisf2(); 
extern int  __powidf2(); 
extern int  __mulsc3(); 
extern int  __muldc3(); 
extern int  __divsc3(); 
extern int  __divdc3(); 
extern int  __bswapsi2(); 
extern int  __bswapdi2(); 
extern int  __aeabi_f2lz(); 
extern int  __fixsfdi(); 
extern int  __aeabi_d2lz(); 
extern int  __fixdfdi(); 
extern int  __aeabi_f2ulz(); 
extern int  __fixunssfdi(); 
extern int  __aeabi_d2ulz(); 
extern int  __fixunsdfdi(); 
extern int  __eprintf(); 
extern int  __gcc_bcmp(); 
extern int  __divdi3(); 
extern int  __moddi3(); 
extern int  __udivdi3(); 
extern int  __umoddi3(); 
extern int  __udiv_w_sdiv(); 
extern int  __udivmoddi4(); 
extern int  __gnu_ldivmod_helper(); 
extern int  __gnu_uldivmod_helper(); 
extern int  __aeabi_uread4(); 
extern int  __aeabi_uread8(); 
extern int  __aeabi_uwrite4(); 
extern int  __aeabi_uwrite8(); 
extern int  __gnu_f2h_alternative(); 
extern int  __gnu_f2h_ieee(); 
extern int  __gnu_h2f_alternative(); 
extern int  __gnu_h2f_ieee(); 
extern int  __gnu_h2f_internal(); 
extern int  _Unwind_Complete(); 
extern int  _Unwind_DeleteException(); 
extern int  _Unwind_GetCFA(); 
extern int  _Unwind_VRS_Get(); 
extern int  _Unwind_VRS_Pop(); 
extern int  _Unwind_VRS_Set(); 
extern int  __aeabi_unwind_cpp_pr0(); 
extern int  __gnu_Unwind_Backtrace(); 
extern int  __gnu_Unwind_ForcedUnwind(); 
extern int  __gnu_Unwind_RaiseException(); 
extern int  __gnu_Unwind_Resume(); 
extern int  __gnu_Unwind_Resume_or_Rethrow(); 
extern int  _Unwind_Backtrace(); 
extern int  _Unwind_ForcedUnwind(); 
extern int  _Unwind_RaiseException(); 
extern int  _Unwind_Resume(); 
extern int  _Unwind_Resume_or_Rethrow(); 
extern int  ___Unwind_Backtrace(); 
extern int  ___Unwind_ForcedUnwind(); 
extern int  ___Unwind_RaiseException(); 
extern int  ___Unwind_Resume(); 
extern int  ___Unwind_Resume_or_Rethrow(); 
extern int  __gnu_Unwind_Restore_VFP(); 
extern int  __gnu_Unwind_Restore_VFP_D(); 
extern int  __gnu_Unwind_Restore_VFP_D_16_to_31(); 
extern int  __gnu_Unwind_Restore_WMMXC(); 
extern int  __gnu_Unwind_Restore_WMMXD(); 
extern int  __gnu_Unwind_Save_VFP(); 
extern int  __gnu_Unwind_Save_VFP_D(); 
extern int  __gnu_Unwind_Save_VFP_D_16_to_31(); 
extern int  __gnu_Unwind_Save_WMMXC(); 
extern int  __gnu_Unwind_Save_WMMXD(); 
extern int  __restore_core_regs(); 
extern int  restore_core_regs(); 
extern int  _Unwind_GetDataRelBase(); 
extern int  _Unwind_GetLanguageSpecificData(); 
extern int  _Unwind_GetRegionStart(); 
extern int  _Unwind_GetTextRelBase(); 
extern int  __gnu_unwind_execute(); 
extern int  __gnu_unwind_frame(); 
extern int  __gcc_personality_v0(); 
extern int  __emutls_get_address(); 
extern int  __emutls_register_common(); 
extern int  acoshl(); 
extern int  acosl(); 
extern int  asinhl(); 
extern int  asinl(); 
extern int  atan2l(); 
extern int  atanhl(); 
extern int  atanl(); 
extern int  cabs(); 
extern int  cabsf(); 
extern int  cacos(); 
extern int  cacosf(); 
extern int  cacosh(); 
extern int  cacoshf(); 
extern int  carg(); 
extern int  cargf(); 
extern int  casin(); 
extern int  casinf(); 
extern int  casinh(); 
extern int  casinhf(); 
extern int  catan(); 
extern int  catanf(); 
extern int  catanh(); 
extern int  catanhf(); 
extern int  cbrtl(); 
extern int  ccos(); 
extern int  ccosf(); 
extern int  ccosh(); 
extern int  ccoshf(); 
extern int  ceill(); 
extern int  _cchsh(); 
extern int  _ctans(); 
extern int  _redupi(); 
extern int  _cchshf(); 
extern int  _ctansf(); 
extern int  _redupif(); 
extern int  cexp(); 
extern int  cexpf(); 
extern int  cimag(); 
extern int  cimagf(); 
extern int  clog(); 
extern int  clogf(); 
extern int  conj(); 
extern int  conjf(); 
extern int  copysignl(); 
extern int  coshl(); 
extern int  cosl(); 
extern int  cpow(); 
extern int  cpowf(); 
extern int  cproj(); 
extern int  cprojf(); 
extern int  creal(); 
extern int  crealf(); 
extern int  csin(); 
extern int  csinf(); 
extern int  csinh(); 
extern int  csinhf(); 
extern int  csqrt(); 
extern int  csqrtf(); 
extern int  ctan(); 
extern int  ctanf(); 
extern int  ctanh(); 
extern int  ctanhf(); 
extern int  __ieee754_acos(); 
extern int  __ieee754_acosh(); 
extern int  __ieee754_asin(); 
extern int  __ieee754_atan2(); 
extern int  __ieee754_atanh(); 
extern int  __ieee754_cosh(); 
extern int  __ieee754_exp(); 
extern int  __ieee754_fmod(); 
extern int  __ieee754_hypot(); 
extern int  __ieee754_j0(); 
extern int  __ieee754_y0(); 
extern int  __ieee754_j1(); 
extern int  __ieee754_y1(); 
extern int  __ieee754_jn(); 
extern int  __ieee754_yn(); 
extern int  __ieee754_log(); 
extern int  __ieee754_log10(); 
extern int  __ieee754_pow(); 
extern int  __ieee754_rem_pio2(); 
extern int  __ieee754_remainder(); 
extern int  __ieee754_scalb(); 
extern int  __ieee754_sinh(); 
extern int  __ieee754_sqrt(); 
extern int  __ieee754_acosf(); 
extern int  __ieee754_acoshf(); 
extern int  __ieee754_asinf(); 
extern int  __ieee754_atan2f(); 
extern int  __ieee754_atanhf(); 
extern int  __ieee754_coshf(); 
extern int  __ieee754_expf(); 
extern int  __ieee754_fmodf(); 
extern int  __ieee754_hypotf(); 
extern int  __ieee754_j0f(); 
extern int  __ieee754_y0f(); 
extern int  __ieee754_j1f(); 
extern int  __ieee754_y1f(); 
extern int  __ieee754_jnf(); 
extern int  __ieee754_ynf(); 
extern int  __ieee754_logf(); 
extern int  __ieee754_log10f(); 
extern int  __ieee754_powf(); 
extern int  __ieee754_rem_pio2f(); 
extern int  __ieee754_remainderf(); 
extern int  __ieee754_scalbf(); 
extern int  __ieee754_sinhf(); 
extern int  __ieee754_sqrtf(); 
extern int  __ieee754_gamma_r(); 
extern int  __ieee754_lgamma_r(); 
extern int  __ieee754_gammaf_r(); 
extern int  __ieee754_lgammaf_r(); 
extern int  erfcl(); 
extern int  erfl(); 
extern int  exp2l(); 
extern int  expl(); 
extern int  expm1l(); 
extern int  fabsl(); 
extern int  fdiml(); 
extern int  floorl(); 
extern int  fmal(); 
extern int  fmaxl(); 
extern int  fminl(); 
extern int  fmodl(); 
extern int  frexpl(); 
extern int  hypotl(); 
extern int  ilogbl(); 
extern int  __kernel_cos(); 
extern int  __kernel_rem_pio2(); 
extern int  __kernel_sin(); 
extern int  __kernel_standard(); 
extern int  __kernel_tan(); 
extern int  __kernel_cosf(); 
extern int  __kernel_rem_pio2f(); 
extern int  __kernel_sinf(); 
extern int  __kernel_tanf(); 
extern int  ldexpl(); 
extern int  lgammal(); 
extern int  llrintl(); 
extern int  llroundl(); 
extern int  log10l(); 
extern int  log1pl(); 
extern int  logl(); 
extern int  lrintl(); 
extern int  lroundl(); 
extern int  modfl(); 
extern int  nanl(); 
extern int  nearbyintl(); 
extern int  nextafterl(); 
extern int  powl(); 
extern int  remainderl(); 
extern int  remquol(); 
extern int  rintl(); 
extern int  roundl(); 
extern int  asinh(); 
extern int  atan(); 
extern int  cbrt(); 
extern int  ceil(); 
extern int  copysign(); 
extern int  cos(); 
extern int  erf(); 
extern int  erfc(); 
extern int  exp10(); 
extern int  expm1(); 
extern int  fabs(); 
extern int  fdim(); 
extern int  finite(); 
extern int  floor(); 
extern int  fma(); 
extern int  fmax(); 
extern int  fmin(); 
extern int  __fpclassifyd(); 
extern int  frexp(); 
extern int  ilogb(); 
extern int  infinity(); 
extern int  isinf(); 
extern int  __isinfd(); 
extern int  isnan(); 
extern int  __isnand(); 
extern int  ldexp(); 
extern int  llrint(); 
extern int  llround(); 
extern int  log1p(); 
extern int  log2(); 
extern int  logb(); 
extern int  lrint(); 
extern int  lround(); 
extern int  matherr(); 
extern int  modf(); 
extern int  nan(); 
extern int  nearbyint(); 
extern int  nextafter(); 
extern int  pow10(); 
extern int  remquo(); 
extern int  rint(); 
extern int  round(); 
extern int  scalbln(); 
extern int  scalbn(); 
extern int  __signbitd(); 
extern int  __signbitf(); 
extern int  significand(); 
extern int  sin(); 
extern int  tan(); 
extern int  tanh(); 
extern int  trunc(); 
extern int  scalblnl(); 
extern int  scalbnl(); 
extern int  asinhf(); 
extern int  atanf(); 
extern int  cbrtf(); 
extern int  ceilf(); 
extern int  copysignf(); 
extern int  cosf(); 
extern int  erfcf(); 
extern int  erff(); 
extern int  exp10f(); 
extern int  expm1f(); 
extern int  fabsf(); 
extern int  fdimf(); 
extern int  finitef(); 
extern int  floorf(); 
extern int  fmaf(); 
extern int  fmaxf(); 
extern int  fminf(); 
extern int  __fpclassifyf(); 
extern int  frexpf(); 
extern int  ilogbf(); 
extern int  infinityf(); 
extern int  isinff(); 
extern int  __isinff(); 
extern int  isnanf(); 
extern int  __isnanf(); 
extern int  ldexpf(); 
extern int  llrintf(); 
extern int  llroundf(); 
extern int  log1pf(); 
extern int  log2f(); 
extern int  logbf(); 
extern int  lrintf(); 
extern int  lroundf(); 
extern int  modff(); 
extern int  nanf(); 
extern int  nearbyintf(); 
extern int  nextafterf(); 
extern int  pow10f(); 
extern int  remquof(); 
extern int  rintf(); 
extern int  roundf(); 
extern int  scalblnf(); 
extern int  scalbnf(); 
extern int  significandf(); 
extern int  sinf(); 
extern int  tanf(); 
extern int  tanhf(); 
extern int  truncf(); 
extern int  sinhl(); 
extern int  sinl(); 
extern int  sqrtl(); 
extern int  tanhl(); 
extern int  tanl(); 
extern int  tgammal(); 
extern int  truncl(); 
extern int  acos(); 
extern int  acosh(); 
extern int  asin(); 
extern int  atan2(); 
extern int  atanh(); 
extern int  cosh(); 
extern int  drem(); 
extern int  exp(); 
extern int  exp2(); 
extern int  fmod(); 
extern int  gamma(); 
extern int  hypot(); 
extern int  j0(); 
extern int  y0(); 
extern int  j1(); 
extern int  y1(); 
extern int  jn(); 
extern int  yn(); 
extern int  lgamma(); 
extern int  log(); 
extern int  log10(); 
extern int  pow(); 
extern int  remainder(); 
extern int  scalb(); 
extern int  sincos(); 
extern int  sinh(); 
extern int  sqrt(); 
extern int  tgamma(); 
extern int  acosf(); 
extern int  acoshf(); 
extern int  asinf(); 
extern int  atan2f(); 
extern int  atanhf(); 
extern int  coshf(); 
extern int  dremf(); 
extern int  expf(); 
extern int  exp2f(); 
extern int  fmodf(); 
extern int  gammaf(); 
extern int  hypotf(); 
extern int  j0f(); 
extern int  y0f(); 
extern int  j1f(); 
extern int  y1f(); 
extern int  jnf(); 
extern int  ynf(); 
extern int  lgammaf(); 
extern int  logf(); 
extern int  log10f(); 
extern int  powf(); 
extern int  remainderf(); 
extern int  scalbf(); 
extern int  sincosf(); 
extern int  sinhf(); 
extern int  sqrtf(); 
extern int  tgammaf(); 
extern int  gamma_r(); 
extern int  lgamma_r(); 
extern int  gammaf_r(); 
extern int  lgammaf_r(); 
extern int  __adjust(); 
extern int  __register_exitproc(); 
extern int  __call_exitprocs(); 
extern int  __dprintf(); 
extern int  __exp10(); 
extern int  __ten_mul(); 
extern int  _Exit(); 
extern int  a64l(); 
extern int  abort(); 
extern int  abs(); 
extern int  access(); 
extern int  __aeabi_atexit(); 
extern int  argz_add(); 
extern int  argz_add_sep(); 
extern int  argz_append(); 
extern int  argz_count(); 
extern int  argz_create(); 
extern int  argz_create_sep(); 
extern int  argz_delete(); 
extern int  argz_extract(); 
extern int  argz_insert(); 
extern int  argz_next(); 
extern int  argz_replace(); 
extern int  argz_stringify(); 
extern int  asctime(); 
extern int  asctime_r(); 
extern int  _asiprintf_r(); 
extern int  asiprintf(); 
extern int  _asniprintf_r(); 
extern int  asniprintf(); 
extern int  _asnprintf_r(); 
extern int  asnprintf(); 
extern int  _asprintf_r(); 
extern int  asprintf(); 
extern int  __assert(); 
extern int  __assert_func(); 
extern int  atexit(); 
extern int  atof(); 
extern int  atoff(); 
extern int  _atoi_r(); 
extern int  atoi(); 
extern int  _atol_r(); 
extern int  atol(); 
extern int  _atoll_r(); 
extern int  atoll(); 
extern int  bcmp(); 
extern int  bcopy(); 
extern int  bsearch(); 
extern int  btowc(); 
extern int  _buf_findstr(); 
extern int  bzero(); 
extern int  calloc(); 
extern int  _calloc_r(); 
extern int  cfree(); 
extern int  clearerr(); 
extern int  clock(); 
extern int  _close_r(); 
extern int  ctime(); 
extern int  ctime_r(); 
extern int  __cxa_atexit(); 
extern int  __cxa_finalize(); 
extern int  difftime(); 
extern int  _diprintf_r(); 
extern int  diprintf(); 
extern int  div(); 
extern int  _dprintf_r(); 
extern int  dprintf(); 
extern int  _drand48_r(); 
extern int  drand48(); 
extern int  _dtoa_r(); 
extern int  __dtoa(); 
extern int  _dcvt(); 
extern int  _gcvt(); 
extern int  ecvtbuf(); 
extern int  fcvtbuf(); 
extern int  ecvt(); 
extern int  ecvtf(); 
extern int  fcvt(); 
extern int  fcvtf(); 
extern int  gcvt(); 
extern int  gcvtf(); 
extern int  __env_lock(); 
extern int  __env_unlock(); 
extern int  envz_add(); 
extern int  envz_entry(); 
extern int  envz_get(); 
extern int  envz_merge(); 
extern int  envz_remove(); 
extern int  envz_strip(); 
extern int  __eprintf(); 
extern int  _erand48_r(); 
extern int  erand48(); 
extern int  __errno(); 
extern int  _execve_r(); 
extern int  _fork_r(); 
extern int  _wait_r(); 
extern int  exit(); 
extern int  _fclose_r(); 
extern int  fclose(); 
extern int  _fcloseall_r(); 
extern int  fcloseall(); 
extern int  _fdopen_r(); 
extern int  fdopen(); 
extern int  feof(); 
extern int  ferror(); 
extern int  _fflush_r(); 
extern int  fflush(); 
extern int  ffs(); 
extern int  _fgetc_r(); 
extern int  fgetc(); 
extern int  _fgetpos_r(); 
extern int  fgetpos(); 
extern int  _fgets_r(); 
extern int  fgets(); 
extern int  _fgetwc_r(); 
extern int  fgetwc(); 
extern int  _fgetws_r(); 
extern int  fgetws(); 
extern int  fileno(); 
extern int  __fp_lock_all(); 
extern int  __fp_unlock_all(); 
extern int  __sfmoreglue(); 
extern int  __sfp(); 
extern int  __sfp_lock_acquire(); 
extern int  __sfp_lock_release(); 
extern int  __sinit(); 
extern int  __sinit_lock_acquire(); 
extern int  __sinit_lock_release(); 
extern int  _cleanup(); 
extern int  _cleanup_r(); 
extern int  __libc_fini_array(); 
extern int  _fiprintf_r(); 
extern int  fiprintf(); 
extern int  _fiscanf_r(); 
extern int  fiscanf(); 
extern int  __fix_locale_grouping_str(); 
extern int  __sflags(); 
extern int  _fmemopen_r(); 
extern int  fmemopen(); 
extern int  _fopen_r(); 
extern int  fopen(); 
extern int  _fopencookie_r(); 
extern int  fopencookie(); 
extern int  _fprintf_r(); 
extern int  fprintf(); 
extern int  _fpurge_r(); 
extern int  fpurge(); 
extern int  _fputc_r(); 
extern int  fputc(); 
extern int  _fputs_r(); 
extern int  fputs(); 
extern int  _fputwc_r(); 
extern int  fputwc(); 
extern int  _fputws_r(); 
extern int  fputws(); 
extern int  _fread_r(); 
extern int  fread(); 
extern int  _freopen_r(); 
extern int  freopen(); 
extern int  _fscanf_r(); 
extern int  fscanf(); 
extern int  _fseek_r(); 
extern int  fseek(); 
extern int  _fseeko_r(); 
extern int  fseeko(); 
extern int  _fsetpos_r(); 
extern int  fsetpos(); 
extern int  _fstat_r(); 
extern int  _ftell_r(); 
extern int  ftell(); 
extern int  _ftello_r(); 
extern int  ftello(); 
extern int  _funopen_r(); 
extern int  funopen(); 
extern int  __sfvwrite_r(); 
extern int  _fwalk(); 
extern int  _fwalk_reent(); 
extern int  _fwide_r(); 
extern int  fwide(); 
extern int  _fwprintf_r(); 
extern int  fwprintf(); 
extern int  _fwrite_r(); 
extern int  fwrite(); 
extern int  _fwscanf_r(); 
extern int  fwscanf(); 
extern int  __gethex(); 
extern int  __hexdig_init(); 
extern int  __hexnan(); 
extern int  _getc_r(); 
extern int  getc(); 
extern int  _getc_unlocked_r(); 
extern int  getc_unlocked(); 
extern int  _getchar_r(); 
extern int  getchar(); 
extern int  _getchar_unlocked_r(); 
extern int  getchar_unlocked(); 
extern int  __getdelim(); 
extern int  _findenv(); 
extern int  getenv(); 
extern int  _findenv_r(); 
extern int  _getenv_r(); 
extern int  __getline(); 
extern int  __getopt_long_only_r(); 
extern int  __getopt_long_r(); 
extern int  __getopt_r(); 
extern int  getopt(); 
extern int  getopt_long(); 
extern int  getopt_long_only(); 
extern int  __getreent(); 
extern int  _gets_r(); 
extern int  gets(); 
extern int  getsubopt(); 
extern int  _gettimeofday_r(); 
extern int  __gettzinfo(); 
extern int  getw(); 
extern int  _getwc_r(); 
extern int  getwc(); 
extern int  _getwchar_r(); 
extern int  getwchar(); 
extern int  gmtime(); 
extern int  gmtime_r(); 
extern int  __call_hash(); 
extern int  __expand_table(); 
extern int  __hash_open(); 
extern int  __big_delete(); 
extern int  __big_insert(); 
extern int  __big_keydata(); 
extern int  __big_return(); 
extern int  __big_split(); 
extern int  __find_bigpair(); 
extern int  __find_last_page(); 
extern int  __buf_free(); 
extern int  __buf_init(); 
extern int  __get_buf(); 
extern int  __reclaim_buf(); 
extern int  __log2(); 
extern int  __add_ovflpage(); 
extern int  __addel(); 
extern int  __delpair(); 
extern int  __free_ovflpage(); 
extern int  __get_page(); 
extern int  __ibitmap(); 
extern int  __put_page(); 
extern int  __split_page(); 
extern int  hcreate(); 
extern int  hdestroy(); 
extern int  hsearch(); 
extern int  hcreate_r(); 
extern int  hdestroy_r(); 
extern int  hsearch_r(); 
extern int  index(); 
extern int  __libc_init_array(); 
extern int  _iprintf_r(); 
extern int  iprintf(); 
extern int  isalnum(); 
extern int  isalpha(); 
extern int  isascii(); 
extern int  _isatty_r(); 
extern int  isblank(); 
extern int  _iscanf_r(); 
extern int  iscanf(); 
extern int  iscntrl(); 
extern int  isdigit(); 
extern int  islower(); 
extern int  isgraph(); 
extern int  isprint(); 
extern int  ispunct(); 
extern int  isspace(); 
extern int  isupper(); 
extern int  iswalnum(); 
extern int  iswalpha(); 
extern int  iswblank(); 
extern int  iswcntrl(); 
extern int  iswctype(); 
extern int  iswdigit(); 
extern int  iswgraph(); 
extern int  iswlower(); 
extern int  iswprint(); 
extern int  iswpunct(); 
extern int  iswspace(); 
extern int  iswupper(); 
extern int  iswxdigit(); 
extern int  isxdigit(); 
extern int  _jrand48_r(); 
extern int  jrand48(); 
extern int  _l64a_r(); 
extern int  l64a(); 
extern int  labs(); 
extern int  localtime(); 
extern int  localtime_r(); 
extern int  _lcong48_r(); 
extern int  lcong48(); 
extern int  __ctype_load_locale(); 
extern int  __get_current_ctype_locale(); 
extern int  ldiv(); 
extern int  __part_load_locale(); 
extern int  _ldcheck(); 
extern int  _ldtoa_r(); 
extern int  _strtold(); 
extern int  _link_r(); 
extern int  llabs(); 
extern int  lldiv(); 
extern int  __get_current_messages_locale(); 
extern int  __messages_load_locale(); 
extern int  __get_current_monetary_locale(); 
extern int  __monetary_load_locale(); 
extern int  __get_current_numeric_locale(); 
extern int  __numeric_load_locale(); 
extern int  __locale_charset(); 
extern int  __locale_cjk_lang(); 
extern int  __locale_mb_cur_max(); 
extern int  __locale_msgcharset(); 
extern int  _localeconv_r(); 
extern int  _setlocale_r(); 
extern int  localeconv(); 
extern int  setlocale(); 
extern int  _lrand48_r(); 
extern int  lrand48(); 
extern int  _lseek_r(); 
extern int  __smakebuf_r(); 
extern int  memalign(); 
extern int  _memalign_r(); 
extern int  free(); 
extern int  malloc(); 
extern int  mblen(); 
extern int  _mblen_r(); 
extern int  mbrlen(); 
extern int  _mbrtowc_r(); 
extern int  mbrtowc(); 
extern int  mbsinit(); 
extern int  _mbsnrtowcs_r(); 
extern int  mbsnrtowcs(); 
extern int  _mbsrtowcs_r(); 
extern int  mbsrtowcs(); 
extern int  mbstowcs(); 
extern int  _mbstowcs_r(); 
extern int  mbtowc(); 
extern int  __ascii_mbtowc(); 
extern int  _mbtowc_r(); 
extern int  memccpy(); 
extern int  memchr(); 
extern int  memcmp(); 
extern int  memcpy(); 
extern int  memmem(); 
extern int  memmove(); 
extern int  mempcpy(); 
extern int  memset(); 
extern int  _mkdir_r(); 
extern int  _mkdtemp_r(); 
extern int  _mkostemp_r(); 
extern int  _mkostemps_r(); 
extern int  _mkstemp_r(); 
extern int  _mkstemps_r(); 
extern int  _mktemp_r(); 
extern int  mkdtemp(); 
extern int  mkostemp(); 
extern int  mkostemps(); 
extern int  mkstemp(); 
extern int  mkstemps(); 
extern int  mktemp(); 
extern int  mktime(); 
extern int  __tzcalc_limits(); 
extern int  _mktm_r(); 
extern int  __malloc_lock(); 
extern int  __malloc_unlock(); 
extern int  _Balloc(); 
extern int  _Bfree(); 
extern int  __any_on(); 
extern int  __b2d(); 
extern int  __copybits(); 
extern int  __d2b(); 
extern int  __hi0bits(); 
extern int  __i2b(); 
extern int  __lo0bits(); 
extern int  __lshift(); 
extern int  __mcmp(); 
extern int  __mdiff(); 
extern int  __multadd(); 
extern int  __multiply(); 
extern int  __pow5mult(); 
extern int  __ratio(); 
extern int  __s2b(); 
extern int  __ulp(); 
extern int  _mprec_log10(); 
extern int  _mrand48_r(); 
extern int  mrand48(); 
extern int  malloc_usable_size(); 
extern int  _malloc_usable_size_r(); 
extern int  nl_langinfo(); 
extern int  _nrand48_r(); 
extern int  nrand48(); 
extern int  on_exit(); 
extern int  _open_memstream_r(); 
extern int  _open_wmemstream_r(); 
extern int  open_memstream(); 
extern int  open_wmemstream(); 
extern int  _open_r(); 
extern int  _perror_r(); 
extern int  perror(); 
extern int  _printf_r(); 
extern int  printf(); 
extern int  _putc_r(); 
extern int  putc(); 
extern int  _putc_unlocked_r(); 
extern int  putc_unlocked(); 
extern int  _putchar_r(); 
extern int  putchar(); 
extern int  _putchar_unlocked_r(); 
extern int  putchar_unlocked(); 
extern int  putenv(); 
extern int  _putenv_r(); 
extern int  _puts_r(); 
extern int  puts(); 
extern int  putw(); 
extern int  _putwc_r(); 
extern int  putwc(); 
extern int  _putwchar_r(); 
extern int  putwchar(); 
extern int  _pvalloc_r(); 
extern int  qsort(); 
extern int  rand(); 
extern int  srand(); 
extern int  __dorand48(); 
extern int  rand_r(); 
extern int  _read_r(); 
extern int  realloc(); 
extern int  _reallocf_r(); 
extern int  reallocf(); 
extern int  _realloc_r(); 
extern int  _reclaim_reent(); 
extern int  _wrapup_reent(); 
extern int  cleanup_glue(); 
extern int  __srefill_r(); 
extern int  _remove_r(); 
extern int  remove(); 
extern int  rename(); 
extern int  _rename_r(); 
extern int  _rewind_r(); 
extern int  rewind(); 
extern int  __srget(); 
extern int  __srget_r(); 
extern int  rindex(); 
extern int  copysign(); 
extern int  finite(); 
extern int  __fpclassifyd(); 
extern int  frexp(); 
extern int  isinf(); 
extern int  __isinfd(); 
extern int  isnan(); 
extern int  __isnand(); 
extern int  ldexp(); 
extern int  modf(); 
extern int  nan(); 
extern int  scalbn(); 
extern int  _sbrk_r(); 
extern int  _scanf_r(); 
extern int  scanf(); 
extern int  __sccl(); 
extern int  _seed48_r(); 
extern int  seed48(); 
extern int  setbuf(); 
extern int  setbuffer(); 
extern int  setenv(); 
extern int  unsetenv(); 
extern int  _setenv_r(); 
extern int  _unsetenv_r(); 
extern int  longjmp(); 
extern int  setjmp(); 
extern int  setlinebuf(); 
extern int  setvbuf(); 
extern int  copysignf(); 
extern int  finitef(); 
extern int  __fpclassifyf(); 
extern int  frexpf(); 
extern int  isinff(); 
extern int  __isinff(); 
extern int  isnanf(); 
extern int  __isnanf(); 
extern int  ldexpf(); 
extern int  modff(); 
extern int  nanf(); 
extern int  scalbnf(); 
extern int  __signgam(); 
extern int  _siprintf_r(); 
extern int  siprintf(); 
extern int  _siscanf_r(); 
extern int  siscanf(); 
extern int  _sniprintf_r(); 
extern int  sniprintf(); 
extern int  _snprintf_r(); 
extern int  snprintf(); 
extern int  _sprintf_r(); 
extern int  sprintf(); 
extern int  _srand48_r(); 
extern int  srand48(); 
extern int  _sscanf_r(); 
extern int  sscanf(); 
extern int  _stat_r(); 
extern int  __sclose(); 
extern int  __seofread(); 
extern int  __sread(); 
extern int  __sseek(); 
extern int  __swrite(); 
extern int  stpcpy(); 
extern int  stpncpy(); 
extern int  strcasecmp(); 
extern int  strcasestr(); 
extern int  strcat(); 
extern int  strchr(); 
extern int  strcmp(); 
extern int  strcoll(); 
extern int  strcpy(); 
extern int  strcspn(); 
extern int  strdup(); 
extern int  _strdup_r(); 
extern int  strerror(); 
extern int  strerror_r(); 
extern int  strftime(); 
extern int  strlcat(); 
extern int  strlcpy(); 
extern int  strlen(); 
extern int  strlwr(); 
extern int  strncasecmp(); 
extern int  strncat(); 
extern int  strncmp(); 
extern int  strncpy(); 
extern int  strndup(); 
extern int  _strndup_r(); 
extern int  strnlen(); 
extern int  strpbrk(); 
extern int  strptime(); 
extern int  strrchr(); 
extern int  strsep(); 
extern int  strsignal(); 
extern int  strspn(); 
extern int  strstr(); 
extern int  _strtod_r(); 
extern int  strtod(); 
extern int  strtof(); 
extern int  strtok(); 
extern int  __strtok_r(); 
extern int  strtok_r(); 
extern int  _strtol_r(); 
extern int  strtol(); 
extern int  strtold(); 
extern int  strtoll(); 
extern int  _strtoll_r(); 
extern int  _strtoul_r(); 
extern int  strtoul(); 
extern int  strtoull(); 
extern int  _strtoull_r(); 
extern int  strupr(); 
extern int  strxfrm(); 
extern int  __ssprint_r(); 
extern int  _svfiprintf_r(); 
extern int  __ssrefill_r(); 
extern int  __ssvfiscanf_r(); 
extern int  _sfread_r(); 
extern int  _sungetc_r(); 
extern int  _svfiwprintf_r(); 
extern int  __ssvfiwscanf_r(); 
extern int  _svfprintf_r(); 
extern int  __ssvfscanf_r(); 
extern int  _svfwprintf_r(); 
extern int  __ssvfwscanf_r(); 
extern int  swab(); 
extern int  _swprintf_r(); 
extern int  swprintf(); 
extern int  _swscanf_r(); 
extern int  swscanf(); 
extern int  close(); 
extern int  execve(); 
extern int  fcntl(); 
extern int  fork(); 
extern int  fstat(); 
extern int  getpid(); 
extern int  gettimeofday(); 
extern int  isatty(); 
extern int  kill(); 
extern int  link(); 
extern int  lseek(); 
extern int  open(); 
extern int  read(); 
extern int  sbrk(); 
extern int  stat(); 
extern int  _system_r(); 
extern int  system(); 
extern int  times(); 
extern int  unlink(); 
extern int  wait(); 
extern int  write(); 
extern int  tdelete(); 
extern int  tdestroy(); 
extern int  tfind(); 
extern int  time(); 
extern int  __get_current_time_locale(); 
extern int  __time_load_locale(); 
extern int  _times_r(); 
extern int  _tmpfile_r(); 
extern int  tmpfile(); 
extern int  _tempnam_r(); 
extern int  _tmpnam_r(); 
extern int  tempnam(); 
extern int  tmpnam(); 
extern int  toascii(); 
extern int  tolower(); 
extern int  toupper(); 
extern int  _towctrans_r(); 
extern int  towctrans(); 
extern int  towlower(); 
extern int  towupper(); 
extern int  tsearch(); 
extern int  twalk(); 
extern int  __tz_lock(); 
extern int  __tz_unlock(); 
extern int  tzset(); 
extern int  _tzset_r(); 
extern int  _user_strerror(); 
extern int  __submore(); 
extern int  _ungetc_r(); 
extern int  ungetc(); 
extern int  _ungetwc_r(); 
extern int  ungetwc(); 
extern int  _unlink_r(); 
extern int  pvalloc(); 
extern int  valloc(); 
extern int  _valloc_r(); 
extern int  _vasiprintf_r(); 
extern int  vasiprintf(); 
extern int  _vasniprintf_r(); 
extern int  vasniprintf(); 
extern int  _vasnprintf_r(); 
extern int  vasnprintf(); 
extern int  _vasprintf_r(); 
extern int  vasprintf(); 
extern int  _vdiprintf_r(); 
extern int  vdiprintf(); 
extern int  _vdprintf_r(); 
extern int  vdprintf(); 
extern int  __sprint_r(); 
extern int  _vfiprintf_r(); 
extern int  vfiprintf(); 
extern int  __svfiscanf(); 
extern int  __svfiscanf_r(); 
extern int  _vfiscanf_r(); 
extern int  vfiscanf(); 
extern int  _vfiwprintf_r(); 
extern int  vfiwprintf(); 
extern int  __svfiwscanf(); 
extern int  __svfiwscanf_r(); 
extern int  _vfiwscanf_r(); 
extern int  vfiwscanf(); 
extern int  _vfprintf_r(); 
extern int  vfprintf(); 
extern int  __svfscanf(); 
extern int  __svfscanf_r(); 
extern int  _vfscanf_r(); 
extern int  vfscanf(); 
extern int  _vfwprintf_r(); 
extern int  vfwprintf(); 
extern int  __svfwscanf(); 
extern int  __svfwscanf_r(); 
extern int  _vfwscanf_r(); 
extern int  vfwscanf(); 
extern int  _viprintf_r(); 
extern int  viprintf(); 
extern int  _viscanf_r(); 
extern int  viscanf(); 
extern int  _vprintf_r(); 
extern int  vprintf(); 
extern int  _vscanf_r(); 
extern int  vscanf(); 
extern int  _vsiprintf_r(); 
extern int  vsiprintf(); 
extern int  _vsiscanf_r(); 
extern int  vsiscanf(); 
extern int  _vsniprintf_r(); 
extern int  vsniprintf(); 
extern int  _vsnprintf_r(); 
extern int  vsnprintf(); 
extern int  _vsprintf_r(); 
extern int  vsprintf(); 
extern int  _vsscanf_r(); 
extern int  vsscanf(); 
extern int  _vswprintf_r(); 
extern int  vswprintf(); 
extern int  _vswscanf_r(); 
extern int  vswscanf(); 
extern int  _vwprintf_r(); 
extern int  vwprintf(); 
extern int  _vwscanf_r(); 
extern int  vwscanf(); 
extern int  __swbuf(); 
extern int  __swbuf_r(); 
extern int  wcpcpy(); 
extern int  wcpncpy(); 
extern int  _wcrtomb_r(); 
extern int  wcrtomb(); 
extern int  wcscasecmp(); 
extern int  wcscat(); 
extern int  wcschr(); 
extern int  wcscmp(); 
extern int  wcscoll(); 
extern int  wcscpy(); 
extern int  wcscspn(); 
extern int  _wcsdup_r(); 
extern int  wcsdup(); 
extern int  __ctloc(); 
extern int  wcsftime(); 
extern int  wcslcat(); 
extern int  wcslcpy(); 
extern int  wcslen(); 
extern int  wcsncasecmp(); 
extern int  wcsncat(); 
extern int  wcsncmp(); 
extern int  wcsncpy(); 
extern int  wcsnlen(); 
extern int  _wcsnrtombs_r(); 
extern int  wcsnrtombs(); 
extern int  wcspbrk(); 
extern int  wcsrchr(); 
extern int  _wcsrtombs_r(); 
extern int  wcsrtombs(); 
extern int  wcsspn(); 
extern int  wcsstr(); 
extern int  _wcstod_r(); 
extern int  _wcstof_r(); 
extern int  wcstod(); 
extern int  wcstof(); 
extern int  wcstok(); 
extern int  _wcstol_r(); 
extern int  wcstol(); 
extern int  wcstold(); 
extern int  wcstoll(); 
extern int  _wcstoll_r(); 
extern int  wcstombs(); 
extern int  _wcstombs_r(); 
extern int  _wcstoul_r(); 
extern int  wcstoul(); 
extern int  wcstoull(); 
extern int  _wcstoull_r(); 
extern int  wcswidth(); 
extern int  wcsxfrm(); 
extern int  wctob(); 
extern int  wctomb(); 
extern int  __ascii_wctomb(); 
extern int  _wctomb_r(); 
extern int  _wctrans_r(); 
extern int  wctrans(); 
extern int  _wctype_r(); 
extern int  wctype(); 
extern int  __wcwidth(); 
extern int  wcwidth(); 
extern int  wmemchr(); 
extern int  wmemcmp(); 
extern int  wmemcpy(); 
extern int  wmemmove(); 
extern int  wmemset(); 
extern int  _wprintf_r(); 
extern int  wprintf(); 
extern int  _write_r(); 
extern int  _wscanf_r(); 
extern int  wscanf(); 
extern int  __swsetup_r(); 
extern int  BUTTON__DefaultProps; 
extern int  BUTTON__Skin; 
extern int  BUTTON__pSkinDefault; 
extern int  FRAMEWIN__DefaultProps; 
extern int  FRAMEWIN__Skin; 
extern int  FRAMEWIN__pSkinDefault; 
extern int  GRAPH__DefaultProps; 
extern int  GUI_CharLine_pfLog2Vis; 
extern int  GUI__Wrap_pfGetCharWrap; 
extern int  GUI__Wrap_pfGetWordWrap; 
extern int  GUI__pFontDefault; 
extern int  HEADER__DefaultBorderH; 
extern int  HEADER__DefaultProps; 
extern int  HEADER__Skin; 
extern int  HEADER__pDefaultCursor; 
extern int  HEADER__pSkinDefault; 
extern int  ICONVIEW__DefaultProps; 
extern int  ICONVIEW__pfDrawStreamedBitmap; 
extern int  LCD__pSetColorAPI; 
extern int  LCD_pAColorIndex; 
extern int  LCD_pBkColorIndex; 
extern int  LCD_pColorIndex; 
extern int  LISTVIEW_DefaultProps; 
extern int  PROGBAR__DefaultProps; 
extern int  PROGBAR__Skin; 
extern int  PROGBAR__pSkinDefault; 
extern int  SCROLLBAR__DefaultProps; 
extern int  SCROLLBAR__DefaultWidth; 
extern int  SCROLLBAR__Skin; 
extern int  SCROLLBAR__ThumbSizeMin; 
extern int  SCROLLBAR__pSkinDefault; 
extern int  SLIDER__DefaultProps; 
extern int  SLIDER__Skin; 
extern int  SLIDER__pSkinDefault; 
extern int  TEXT__DefaultProps; 
extern int  TREEVIEW__DefaultProps; 
extern int  _Props; 
extern int  __ctype_ptr__; 
extern int  __fdlib_version; 
extern int  __mb_cur_max; 
extern int  __mbtowc; 
extern int  __wctomb; 
extern int  _aWave; 
extern int  _data; 
extern int  _impure_ptr; 
extern int  _pEffectDefault; 
extern int  _pfDraw; 
extern int  _tzname; 
extern int  devfs; 
extern int  environ; 
extern int  fatfs; 
extern int  fb_drv; 
extern int  nand_curr_device; 
extern int  nand_flash_ids; 
extern int  nand_manuf_ids; 
extern int  rootfs; 
extern int  socket_drv; 
extern int  sys_do_table; 
extern int  yaffs1; 
extern int  yaffs2; 
extern int  yaffs_checkptrw_c_version; 
extern int  yaffs_ecc_c_version; 
extern int  yaffs_guts_c_version; 
extern int  yaffs_mtdif1_c_version; 
extern int  yaffs_mtdif2_c_version; 
extern int  yaffs_mtdif_c_version; 
extern int  yaffs_nand_c_version; 
extern int  yaffs_traceMask; 
extern int  yaffsfs_c_version; 
extern int  GUITASK__EntranceCnt; 
extern int  GUI_ALLOC__Context; 
extern int  GUI_Context; 
extern int  GUI_DecChar; 
extern int  GUI_MEMDEV__pCbAnimation; 
extern int  GUI_MEMDEV__pVoid; 
extern int  GUI_OrgX; 
extern int  GUI_OrgY; 
extern int  GUI__CharHasTrans; 
extern int  GUI_pLCD_APIList; 
extern int  GUI_pUC_API; 
extern int  GUI_pfDispCharStyle; 
extern int  GUI_pfTimerExec; 
extern int  GUI_sError; 
extern int  HEADER__DefaultBorderV; 
extern int  LCD__aConvTable; 
extern int  WM_MULTIBUF__pAPI; 
extern int  WM__CaptureReleaseAuto; 
extern int  WM__CreateFlags; 
extern int  WM__FirstWin; 
extern int  WM__NumInvalidWindows; 
extern int  WM__NumWindows; 
extern int  WM__PaintCallbackCnt; 
extern int  WM__TouchedLayer; 
extern int  WM__TransWindowCnt; 
extern int  WM__aBkColor; 
extern int  WM__aCHWinLast; 
extern int  WM__aCHWinModal; 
extern int  WM__aCHWinMouseOver; 
extern int  WM__ahCapture; 
extern int  WM__ahDesktopWin; 
extern int  WM__ahWinFocus; 
extern int  WM__hATransWindow; 
extern int  WM__pFirstCriticalHandle; 
extern int  WM_pfHandlePID; 
extern int  WM_pfPollPID; 
extern int  _PathLocale; 
extern int  __bss_start; 
extern int  __bss_start__; 
extern int  __hexdig; 
extern int  __mlocale_changed; 
extern int  __nlocale_changed; 
extern int  _daylight; 
extern int  _hScaleH; 
extern int  _hScaleV; 
extern int  _timezone; 
extern int  current; 
extern int  current_header; 
extern int  current_iphdr_dest; 
extern int  current_iphdr_src; 
extern int  current_netif; 
extern int  dev_mgr_lock; 
extern int  fs_mgr_lock; 
extern int  interrupt_nest; 
extern int  lock_tcpip_core; 
extern int  lwip_stats; 
extern int  mtd_table; 
extern int  nand_info; 
extern int  netif_default; 
extern int  netif_list; 
extern int  pbuf_free_ooseq_pending; 
extern int  point_mgr_lock; 
extern int  ram_heap; 
extern int  reents; 
extern int  rootfs_point; 
extern int  running; 
extern int  tasks; 
extern int  tcp_active_pcbs; 
extern int  tcp_active_pcbs_changed; 
extern int  tcp_bound_pcbs; 
extern int  tcp_input_pcb; 
extern int  tcp_listen_pcbs; 
extern int  tcp_ticks; 
extern int  tcp_tmp_pcb; 
extern int  tcp_tw_pcbs; 
extern int  ticks; 
extern int  udp_pcbs; 
extern int  yaffs_wr_attempts; 
extern int  __aeabi_idiv0; 
extern int  __aeabi_ldiv0; 
extern int  __aeabi_unwind_cpp_pr1; 
extern int  __aeabi_unwind_cpp_pr2; 
extern int  __CTOR_LIST__; 
extern int  __DTOR_LIST__; 
extern int  __clz_tab; 
extern int  __popcount_tab; 
extern int  __aeabi_idiv0; 
extern int  __aeabi_ldiv0; 
extern int  __aeabi_unwind_cpp_pr1; 
extern int  __aeabi_unwind_cpp_pr2; 
extern int  __fdlib_version; 
extern int  __infinity; 
extern int  __infinityf; 
extern int  __infinityld; 
extern int  __ctype_ptr__; 
extern int  environ; 
extern int  opterr; 
extern int  optopt; 
extern int  __default_hash; 
extern int  _impure_ptr; 
extern int  __mb_cur_max; 
extern int  __mbtowc; 
extern int  _tzname; 
extern int  __wctomb; 
extern int  optarg; 
extern int  optind; 
extern int  _PathLocale; 
extern int  __mlocale_changed; 
extern int  __nlocale_changed; 
extern int  _daylight; 
extern int  _timezone; 
extern int  __infinity; 
extern int  __infinityf; 
extern int  __infinityld; 
extern int  tm_year_base; 
extern int  __unctrl; 
extern int  __unctrllen; 
extern int  __hexdig; 
extern int  suboptarg; 
extern int  errno; 
/*********************************************************************************************************	
** 系统静态符号表												
*********************************************************************************************************/	
SYMBOL_TABLE_BEGIN												
    SYMBOL_ITEM_FUNC(BKDRHash) 
    SYMBOL_ITEM_FUNC(BUTTON_Callback) 
    SYMBOL_ITEM_FUNC(BUTTON_CreateEx) 
    SYMBOL_ITEM_FUNC(BUTTON_CreateIndirect) 
    SYMBOL_ITEM_FUNC(BUTTON_CreateUser) 
    SYMBOL_ITEM_FUNC(BUTTON_DrawSkinFlex) 
    SYMBOL_ITEM_FUNC(BUTTON_GetSkinFlexProps) 
    SYMBOL_ITEM_FUNC(BUTTON_GetUserData) 
    SYMBOL_ITEM_FUNC(BUTTON_IsPressed) 
    SYMBOL_ITEM_FUNC(BUTTON_LockH) 
    SYMBOL_ITEM_FUNC(BUTTON_SetDefaultSkin) 
    SYMBOL_ITEM_FUNC(BUTTON_SetDefaultSkinClassic) 
    SYMBOL_ITEM_FUNC(BUTTON_SetFocussable) 
    SYMBOL_ITEM_FUNC(BUTTON_SetFont) 
    SYMBOL_ITEM_FUNC(BUTTON_SetPressed) 
    SYMBOL_ITEM_FUNC(BUTTON_SetReactOnLevel) 
    SYMBOL_ITEM_FUNC(BUTTON_SetReactOnTouch) 
    SYMBOL_ITEM_FUNC(BUTTON_SetSkin) 
    SYMBOL_ITEM_FUNC(BUTTON_SetSkinClassic) 
    SYMBOL_ITEM_FUNC(BUTTON_SetSkinFlexProps) 
    SYMBOL_ITEM_FUNC(BUTTON_SetState) 
    SYMBOL_ITEM_FUNC(BUTTON_SetText) 
    SYMBOL_ITEM_FUNC(BUTTON_SetTextOffset) 
    SYMBOL_ITEM_FUNC(BUTTON_SetUserData) 
    SYMBOL_ITEM_FUNC(BUTTON__SkinClassic) 
    SYMBOL_ITEM_FUNC(CharInfo18B_ASCII) 
    SYMBOL_ITEM_FUNC(CharInfo24B_ASCII) 
    SYMBOL_ITEM_FUNC(DIALOG_GetBkColor) 
    SYMBOL_ITEM_FUNC(DIALOG_SetBkColor) 
    SYMBOL_ITEM_FUNC(FRAMEWIN_Callback) 
    SYMBOL_ITEM_FUNC(FRAMEWIN_CreateEx) 
    SYMBOL_ITEM_FUNC(FRAMEWIN_CreateIndirect) 
    SYMBOL_ITEM_FUNC(FRAMEWIN_CreateUser) 
    SYMBOL_ITEM_FUNC(FRAMEWIN_DrawSkinFlex) 
    SYMBOL_ITEM_FUNC(FRAMEWIN_GetActive) 
    SYMBOL_ITEM_FUNC(FRAMEWIN_GetBarColor) 
    SYMBOL_ITEM_FUNC(FRAMEWIN_GetBorderSize) 
    SYMBOL_ITEM_FUNC(FRAMEWIN_GetBorderSizeEx) 
    SYMBOL_ITEM_FUNC(FRAMEWIN_GetDefaultBarColor) 
    SYMBOL_ITEM_FUNC(FRAMEWIN_GetDefaultBorderSize) 
    SYMBOL_ITEM_FUNC(FRAMEWIN_GetDefaultClientColor) 
    SYMBOL_ITEM_FUNC(FRAMEWIN_GetDefaultFont) 
    SYMBOL_ITEM_FUNC(FRAMEWIN_GetDefaultTextColor) 
    SYMBOL_ITEM_FUNC(FRAMEWIN_GetDefaultTitleHeight) 
    SYMBOL_ITEM_FUNC(FRAMEWIN_GetFont) 
    SYMBOL_ITEM_FUNC(FRAMEWIN_GetSkinFlexProps) 
    SYMBOL_ITEM_FUNC(FRAMEWIN_GetText) 
    SYMBOL_ITEM_FUNC(FRAMEWIN_GetTextAlign) 
    SYMBOL_ITEM_FUNC(FRAMEWIN_GetTitleHeight) 
    SYMBOL_ITEM_FUNC(FRAMEWIN_GetUserData) 
    SYMBOL_ITEM_FUNC(FRAMEWIN_LockH) 
    SYMBOL_ITEM_FUNC(FRAMEWIN_OwnerDraw) 
    SYMBOL_ITEM_FUNC(FRAMEWIN_SetActive) 
    SYMBOL_ITEM_FUNC(FRAMEWIN_SetBarColor) 
    SYMBOL_ITEM_FUNC(FRAMEWIN_SetClientColor) 
    SYMBOL_ITEM_FUNC(FRAMEWIN_SetDefaultBarColor) 
    SYMBOL_ITEM_FUNC(FRAMEWIN_SetDefaultBorderSize) 
    SYMBOL_ITEM_FUNC(FRAMEWIN_SetDefaultClientColor) 
    SYMBOL_ITEM_FUNC(FRAMEWIN_SetDefaultFont) 
    SYMBOL_ITEM_FUNC(FRAMEWIN_SetDefaultSkin) 
    SYMBOL_ITEM_FUNC(FRAMEWIN_SetDefaultSkinClassic) 
    SYMBOL_ITEM_FUNC(FRAMEWIN_SetDefaultTextAlign) 
    SYMBOL_ITEM_FUNC(FRAMEWIN_SetDefaultTextColor) 
    SYMBOL_ITEM_FUNC(FRAMEWIN_SetDefaultTitleHeight) 
    SYMBOL_ITEM_FUNC(FRAMEWIN_SetMoveable) 
    SYMBOL_ITEM_FUNC(FRAMEWIN_SetOwnerDraw) 
    SYMBOL_ITEM_FUNC(FRAMEWIN_SetSkin) 
    SYMBOL_ITEM_FUNC(FRAMEWIN_SetSkinClassic) 
    SYMBOL_ITEM_FUNC(FRAMEWIN_SetSkinFlexProps) 
    SYMBOL_ITEM_FUNC(FRAMEWIN_SetText) 
    SYMBOL_ITEM_FUNC(FRAMEWIN_SetTextAlign) 
    SYMBOL_ITEM_FUNC(FRAMEWIN_SetTextColor) 
    SYMBOL_ITEM_FUNC(FRAMEWIN_SetTextColorEx) 
    SYMBOL_ITEM_FUNC(FRAMEWIN_SetUserData) 
    SYMBOL_ITEM_FUNC(FRAMEWIN__CalcPositions) 
    SYMBOL_ITEM_FUNC(FRAMEWIN__CalcTitleHeight) 
    SYMBOL_ITEM_FUNC(FRAMEWIN__GetBorderSize) 
    SYMBOL_ITEM_FUNC(FRAMEWIN__GetTitleLimits) 
    SYMBOL_ITEM_FUNC(FRAMEWIN__SkinClassic) 
    SYMBOL_ITEM_FUNC(FRAMEWIN__UpdateButtons) 
    SYMBOL_ITEM_FUNC(FRAMEWIN__UpdatePositions) 
    SYMBOL_ITEM_FUNC(FontComic24BProp_ASCII) 
    SYMBOL_ITEM_FUNC(GL_DispChar) 
    SYMBOL_ITEM_FUNC(GL_DrawArc) 
    SYMBOL_ITEM_FUNC(GL_DrawBitmap) 
    SYMBOL_ITEM_FUNC(GL_DrawCircle) 
    SYMBOL_ITEM_FUNC(GL_DrawEllipse) 
    SYMBOL_ITEM_FUNC(GL_DrawLine) 
    SYMBOL_ITEM_FUNC(GL_DrawLine1) 
    SYMBOL_ITEM_FUNC(GL_DrawLine1Ex) 
    SYMBOL_ITEM_FUNC(GL_DrawLineEx) 
    SYMBOL_ITEM_FUNC(GL_DrawLineTo) 
    SYMBOL_ITEM_FUNC(GL_DrawLineToEx) 
    SYMBOL_ITEM_FUNC(GL_DrawPoint) 
    SYMBOL_ITEM_FUNC(GL_FillCircle) 
    SYMBOL_ITEM_FUNC(GL_FillCircleAA_HiRes) 
    SYMBOL_ITEM_FUNC(GL_FillEllipse) 
    SYMBOL_ITEM_FUNC(GL_FillPolygon) 
    SYMBOL_ITEM_FUNC(GL_MoveTo) 
    SYMBOL_ITEM_FUNC(GL_SetDefault) 
    SYMBOL_ITEM_FUNC(GRAPH_AttachData) 
    SYMBOL_ITEM_FUNC(GRAPH_AttachScale) 
    SYMBOL_ITEM_FUNC(GRAPH_Callback) 
    SYMBOL_ITEM_FUNC(GRAPH_CreateEx) 
    SYMBOL_ITEM_FUNC(GRAPH_CreateUser) 
    SYMBOL_ITEM_FUNC(GRAPH_DATA_YT_AddValue) 
    SYMBOL_ITEM_FUNC(GRAPH_DATA_YT_Clear) 
    SYMBOL_ITEM_FUNC(GRAPH_DATA_YT_Create) 
    SYMBOL_ITEM_FUNC(GRAPH_DATA_YT_Delete) 
    SYMBOL_ITEM_FUNC(GRAPH_DATA_YT_MirrorX) 
    SYMBOL_ITEM_FUNC(GRAPH_DATA_YT_SetAlign) 
    SYMBOL_ITEM_FUNC(GRAPH_DATA_YT_SetOffY) 
    SYMBOL_ITEM_FUNC(GRAPH_DetachData) 
    SYMBOL_ITEM_FUNC(GRAPH_DetachScale) 
    SYMBOL_ITEM_FUNC(GRAPH_GetScrollValue) 
    SYMBOL_ITEM_FUNC(GRAPH_GetUserData) 
    SYMBOL_ITEM_FUNC(GRAPH_LockH) 
    SYMBOL_ITEM_FUNC(GRAPH_SCALE_Create) 
    SYMBOL_ITEM_FUNC(GRAPH_SCALE_Delete) 
    SYMBOL_ITEM_FUNC(GRAPH_SCALE_SetFactor) 
    SYMBOL_ITEM_FUNC(GRAPH_SCALE_SetFont) 
    SYMBOL_ITEM_FUNC(GRAPH_SCALE_SetNumDecs) 
    SYMBOL_ITEM_FUNC(GRAPH_SCALE_SetOff) 
    SYMBOL_ITEM_FUNC(GRAPH_SCALE_SetPos) 
    SYMBOL_ITEM_FUNC(GRAPH_SCALE_SetTextColor) 
    SYMBOL_ITEM_FUNC(GRAPH_SCALE_SetTickDist) 
    SYMBOL_ITEM_FUNC(GRAPH_SetAutoScrollbar) 
    SYMBOL_ITEM_FUNC(GRAPH_SetBorder) 
    SYMBOL_ITEM_FUNC(GRAPH_SetColor) 
    SYMBOL_ITEM_FUNC(GRAPH_SetGridDistX) 
    SYMBOL_ITEM_FUNC(GRAPH_SetGridDistY) 
    SYMBOL_ITEM_FUNC(GRAPH_SetGridFixedX) 
    SYMBOL_ITEM_FUNC(GRAPH_SetGridOffY) 
    SYMBOL_ITEM_FUNC(GRAPH_SetGridVis) 
    SYMBOL_ITEM_FUNC(GRAPH_SetLineStyle) 
    SYMBOL_ITEM_FUNC(GRAPH_SetLineStyleH) 
    SYMBOL_ITEM_FUNC(GRAPH_SetLineStyleV) 
    SYMBOL_ITEM_FUNC(GRAPH_SetScrollValue) 
    SYMBOL_ITEM_FUNC(GRAPH_SetUserData) 
    SYMBOL_ITEM_FUNC(GRAPH_SetUserDraw) 
    SYMBOL_ITEM_FUNC(GRAPH_SetVSizeX) 
    SYMBOL_ITEM_FUNC(GRAPH_SetVSizeY) 
    SYMBOL_ITEM_FUNC(GRAPH__AddValue) 
    SYMBOL_ITEM_FUNC(GUIDEMO_AddIntToString) 
    SYMBOL_ITEM_FUNC(GUIDEMO_AddStringToString) 
    SYMBOL_ITEM_FUNC(GUIDEMO_Automotive) 
    SYMBOL_ITEM_FUNC(GUIDEMO_BarGraph) 
    SYMBOL_ITEM_FUNC(GUIDEMO_Bitmap) 
    SYMBOL_ITEM_FUNC(GUIDEMO_CheckCancel) 
    SYMBOL_ITEM_FUNC(GUIDEMO_ClearText) 
    SYMBOL_ITEM_FUNC(GUIDEMO_ColorBar) 
    SYMBOL_ITEM_FUNC(GUIDEMO_Config) 
    SYMBOL_ITEM_FUNC(GUIDEMO_Cursor) 
    SYMBOL_ITEM_FUNC(GUIDEMO_Delay) 
    SYMBOL_ITEM_FUNC(GUIDEMO_DrawBk) 
    SYMBOL_ITEM_FUNC(GUIDEMO_Fading) 
    SYMBOL_ITEM_FUNC(GUIDEMO_GetConfFlag) 
    SYMBOL_ITEM_FUNC(GUIDEMO_GetTime) 
    SYMBOL_ITEM_FUNC(GUIDEMO_Graph) 
    SYMBOL_ITEM_FUNC(GUIDEMO_HideControlWin) 
    SYMBOL_ITEM_FUNC(GUIDEMO_HideInfoWin) 
    SYMBOL_ITEM_FUNC(GUIDEMO_IconView) 
    SYMBOL_ITEM_FUNC(GUIDEMO_ImageFlow) 
    SYMBOL_ITEM_FUNC(GUIDEMO_Intro) 
    SYMBOL_ITEM_FUNC(GUIDEMO_Listview) 
    SYMBOL_ITEM_FUNC(GUIDEMO_Main) 
    SYMBOL_ITEM_FUNC(GUIDEMO_NotifyStartNext) 
    SYMBOL_ITEM_FUNC(GUIDEMO_ShowControlWin) 
    SYMBOL_ITEM_FUNC(GUIDEMO_ShowInfo) 
    SYMBOL_ITEM_FUNC(GUIDEMO_ShowInfoWin) 
    SYMBOL_ITEM_FUNC(GUIDEMO_ShowIntro) 
    SYMBOL_ITEM_FUNC(GUIDEMO_Skinning) 
    SYMBOL_ITEM_FUNC(GUIDEMO_Speed) 
    SYMBOL_ITEM_FUNC(GUIDEMO_Speedometer) 
    SYMBOL_ITEM_FUNC(GUIDEMO_TransparentDialog) 
    SYMBOL_ITEM_FUNC(GUIDEMO_Treeview) 
    SYMBOL_ITEM_FUNC(GUIDEMO_UpdateControlText) 
    SYMBOL_ITEM_FUNC(GUIDEMO_VScreen) 
    SYMBOL_ITEM_FUNC(GUIDEMO_Wait) 
    SYMBOL_ITEM_FUNC(GUIDEMO_WashingMachine) 
    SYMBOL_ITEM_FUNC(GUIDEMO_ZoomAndRotate) 
    SYMBOL_ITEM_FUNC(GUIDEMO_bm4bpp) 
    SYMBOL_ITEM_FUNC(GUIDEV_StaticDevices_C) 
    SYMBOL_ITEM_FUNC(GUIDRV_Lin_16_API) 
    SYMBOL_ITEM_FUNC(GUIMONO_DispChar) 
    SYMBOL_ITEM_FUNC(GUIMONO_GetCharDistX) 
    SYMBOL_ITEM_FUNC(GUIMONO_GetFontInfo) 
    SYMBOL_ITEM_FUNC(GUIMONO_IsInFont) 
    SYMBOL_ITEM_FUNC(GUIPROP_AA4_DispChar) 
    SYMBOL_ITEM_FUNC(GUIPROP_AA4_EXT_DispChar) 
    SYMBOL_ITEM_FUNC(GUIPROP_AA4_GetCharDistX) 
    SYMBOL_ITEM_FUNC(GUIPROP_AA4_GetFontInfo) 
    SYMBOL_ITEM_FUNC(GUIPROP_AA4_IsInFont) 
    SYMBOL_ITEM_FUNC(GUIPROP_DispChar) 
    SYMBOL_ITEM_FUNC(GUIPROP_EXT_DispChar) 
    SYMBOL_ITEM_FUNC(GUIPROP_EXT_GetCharDistX) 
    SYMBOL_ITEM_FUNC(GUIPROP_EXT_GetCharInfo) 
    SYMBOL_ITEM_FUNC(GUIPROP_EXT_GetFontInfo) 
    SYMBOL_ITEM_FUNC(GUIPROP_EXT_IsInFont) 
    SYMBOL_ITEM_FUNC(GUIPROP_EXT__ClearLine) 
    SYMBOL_ITEM_FUNC(GUIPROP_EXT__DispLine) 
    SYMBOL_ITEM_FUNC(GUIPROP_EXT__FindChar) 
    SYMBOL_ITEM_FUNC(GUIPROP_EXT__SetfpClearLine) 
    SYMBOL_ITEM_FUNC(GUIPROP_GetCharDistX) 
    SYMBOL_ITEM_FUNC(GUIPROP_GetFontInfo) 
    SYMBOL_ITEM_FUNC(GUIPROP_IsInFont) 
    SYMBOL_ITEM_FUNC(GUIPROP__FindChar) 
    SYMBOL_ITEM_FUNC(GUITASK_CopyContext) 
    SYMBOL_ITEM_FUNC(GUITASK_GetpContext) 
    SYMBOL_ITEM_FUNC(GUITASK_Init) 
    SYMBOL_ITEM_FUNC(GUITASK_SetMaxTask) 
    SYMBOL_ITEM_FUNC(GUI_AA_DisableHiRes) 
    SYMBOL_ITEM_FUNC(GUI_AA_DrawArc) 
    SYMBOL_ITEM_FUNC(GUI_AA_DrawLine) 
    SYMBOL_ITEM_FUNC(GUI_AA_EnableHiRes) 
    SYMBOL_ITEM_FUNC(GUI_AA_Exit) 
    SYMBOL_ITEM_FUNC(GUI_AA_FillCircle) 
    SYMBOL_ITEM_FUNC(GUI_AA_FillPolygon) 
    SYMBOL_ITEM_FUNC(GUI_AA_GetFactor) 
    SYMBOL_ITEM_FUNC(GUI_AA_HiRes2Pixel) 
    SYMBOL_ITEM_FUNC(GUI_AA_Init) 
    SYMBOL_ITEM_FUNC(GUI_AA_Init_HiRes) 
    SYMBOL_ITEM_FUNC(GUI_AA_SetFactor) 
    SYMBOL_ITEM_FUNC(GUI_AA__DrawCharAA4) 
    SYMBOL_ITEM_FUNC(GUI_ALLOC_AllocInit) 
    SYMBOL_ITEM_FUNC(GUI_ALLOC_AllocNoInit) 
    SYMBOL_ITEM_FUNC(GUI_ALLOC_AllocZero) 
    SYMBOL_ITEM_FUNC(GUI_ALLOC_AssignMemory) 
    SYMBOL_ITEM_FUNC(GUI_ALLOC_Free) 
    SYMBOL_ITEM_FUNC(GUI_ALLOC_FreeFixedBlock) 
    SYMBOL_ITEM_FUNC(GUI_ALLOC_FreePtr) 
    SYMBOL_ITEM_FUNC(GUI_ALLOC_FreePtrArray) 
    SYMBOL_ITEM_FUNC(GUI_ALLOC_GetFixedBlock) 
    SYMBOL_ITEM_FUNC(GUI_ALLOC_GetMaxSize) 
    SYMBOL_ITEM_FUNC(GUI_ALLOC_GetNumFreeBlocks) 
    SYMBOL_ITEM_FUNC(GUI_ALLOC_GetNumFreeBytes) 
    SYMBOL_ITEM_FUNC(GUI_ALLOC_GetNumUsedBlocks) 
    SYMBOL_ITEM_FUNC(GUI_ALLOC_GetNumUsedBytes) 
    SYMBOL_ITEM_FUNC(GUI_ALLOC_GetSize) 
    SYMBOL_ITEM_FUNC(GUI_ALLOC_Lock) 
    SYMBOL_ITEM_FUNC(GUI_ALLOC_LockH) 
    SYMBOL_ITEM_FUNC(GUI_ALLOC_Realloc) 
    SYMBOL_ITEM_FUNC(GUI_ALLOC_RequestSize) 
    SYMBOL_ITEM_FUNC(GUI_ALLOC_SetAvBlockSize) 
    SYMBOL_ITEM_FUNC(GUI_ALLOC_SetMaxPercentage) 
    SYMBOL_ITEM_FUNC(GUI_ALLOC_Unlock) 
    SYMBOL_ITEM_FUNC(GUI_ALLOC_UnlockH) 
    SYMBOL_ITEM_FUNC(GUI_ALLOC_h2p) 
    SYMBOL_ITEM_FUNC(GUI_ALLOC_p2h) 
    SYMBOL_ITEM_FUNC(GUI_ARRAY_AddItem) 
    SYMBOL_ITEM_FUNC(GUI_ARRAY_Create) 
    SYMBOL_ITEM_FUNC(GUI_ARRAY_Delete) 
    SYMBOL_ITEM_FUNC(GUI_ARRAY_DeleteItem) 
    SYMBOL_ITEM_FUNC(GUI_ARRAY_GetNumItems) 
    SYMBOL_ITEM_FUNC(GUI_ARRAY_GethItem) 
    SYMBOL_ITEM_FUNC(GUI_ARRAY_GetpItemLocked) 
    SYMBOL_ITEM_FUNC(GUI_ARRAY_InsertBlankItem) 
    SYMBOL_ITEM_FUNC(GUI_ARRAY_InsertItem) 
    SYMBOL_ITEM_FUNC(GUI_ARRAY_ResizeItemLocked) 
    SYMBOL_ITEM_FUNC(GUI_ARRAY_SetItem) 
    SYMBOL_ITEM_FUNC(GUI_ARRAY__GethItem) 
    SYMBOL_ITEM_FUNC(GUI_ARRAY__GetpItemLocked) 
    SYMBOL_ITEM_FUNC(GUI_ARRAY__SethItem) 
    SYMBOL_ITEM_FUNC(GUI_AddDecMin) 
    SYMBOL_ITEM_FUNC(GUI_AddDecShift) 
    SYMBOL_ITEM_FUNC(GUI_AddHex) 
    SYMBOL_ITEM_FUNC(GUI_AddSign) 
    SYMBOL_ITEM_FUNC(GUI_AssignCursorLayer) 
    SYMBOL_ITEM_FUNC(GUI_BitmapArrowL) 
    SYMBOL_ITEM_FUNC(GUI_BitmapArrowLI) 
    SYMBOL_ITEM_FUNC(GUI_BitmapArrowM) 
    SYMBOL_ITEM_FUNC(GUI_BitmapArrowMI) 
    SYMBOL_ITEM_FUNC(GUI_BitmapArrowS) 
    SYMBOL_ITEM_FUNC(GUI_BitmapArrowSI) 
    SYMBOL_ITEM_FUNC(GUI_BitmapCrossL) 
    SYMBOL_ITEM_FUNC(GUI_BitmapCrossLI) 
    SYMBOL_ITEM_FUNC(GUI_BitmapCrossM) 
    SYMBOL_ITEM_FUNC(GUI_BitmapCrossMI) 
    SYMBOL_ITEM_FUNC(GUI_BitmapCrossS) 
    SYMBOL_ITEM_FUNC(GUI_BitmapCrossSI) 
    SYMBOL_ITEM_FUNC(GUI_BitmapMethods24) 
    SYMBOL_ITEM_FUNC(GUI_BitmapMethods555) 
    SYMBOL_ITEM_FUNC(GUI_BitmapMethods565) 
    SYMBOL_ITEM_FUNC(GUI_BitmapMethods8888) 
    SYMBOL_ITEM_FUNC(GUI_BitmapMethodsM555) 
    SYMBOL_ITEM_FUNC(GUI_BitmapMethodsM565) 
    SYMBOL_ITEM_FUNC(GUI_BitmapMethodsRLE16) 
    SYMBOL_ITEM_FUNC(GUI_BitmapMethodsRLE16Ex) 
    SYMBOL_ITEM_FUNC(GUI_BitmapMethodsRLE32) 
    SYMBOL_ITEM_FUNC(GUI_BitmapMethodsRLE32Ex) 
    SYMBOL_ITEM_FUNC(GUI_BitmapMethodsRLE4) 
    SYMBOL_ITEM_FUNC(GUI_BitmapMethodsRLE4Ex) 
    SYMBOL_ITEM_FUNC(GUI_BitmapMethodsRLE8) 
    SYMBOL_ITEM_FUNC(GUI_BitmapMethodsRLE8Ex) 
    SYMBOL_ITEM_FUNC(GUI_BitmapMethodsRLEAlpha) 
    SYMBOL_ITEM_FUNC(GUI_BitmapMethodsRLEM16) 
    SYMBOL_ITEM_FUNC(GUI_BitmapMethodsRLEM16Ex) 
    SYMBOL_ITEM_FUNC(GUI_CURSOR_GetState) 
    SYMBOL_ITEM_FUNC(GUI_CURSOR_GetStateEx) 
    SYMBOL_ITEM_FUNC(GUI_CURSOR_Hide) 
    SYMBOL_ITEM_FUNC(GUI_CURSOR_HideEx) 
    SYMBOL_ITEM_FUNC(GUI_CURSOR_Select) 
    SYMBOL_ITEM_FUNC(GUI_CURSOR_SelectEx) 
    SYMBOL_ITEM_FUNC(GUI_CURSOR_SetBitmap) 
    SYMBOL_ITEM_FUNC(GUI_CURSOR_SetBitmapEx) 
    SYMBOL_ITEM_FUNC(GUI_CURSOR_SetPosition) 
    SYMBOL_ITEM_FUNC(GUI_CURSOR_SetPositionEx) 
    SYMBOL_ITEM_FUNC(GUI_CURSOR_Show) 
    SYMBOL_ITEM_FUNC(GUI_CURSOR_ShowEx) 
    SYMBOL_ITEM_FUNC(GUI_CURSOR__SetCallback) 
    SYMBOL_ITEM_FUNC(GUI_CalcColorDist) 
    SYMBOL_ITEM_FUNC(GUI_Clear) 
    SYMBOL_ITEM_FUNC(GUI_ClearKeyBuffer) 
    SYMBOL_ITEM_FUNC(GUI_ClearRect) 
    SYMBOL_ITEM_FUNC(GUI_ClearRectEx) 
    SYMBOL_ITEM_FUNC(GUI_Color2Index) 
    SYMBOL_ITEM_FUNC(GUI_CreateBitmapFromStream) 
    SYMBOL_ITEM_FUNC(GUI_CreateBitmapFromStream24) 
    SYMBOL_ITEM_FUNC(GUI_CreateBitmapFromStream555) 
    SYMBOL_ITEM_FUNC(GUI_CreateBitmapFromStream565) 
    SYMBOL_ITEM_FUNC(GUI_CreateBitmapFromStreamAlpha) 
    SYMBOL_ITEM_FUNC(GUI_CreateBitmapFromStreamIDX) 
    SYMBOL_ITEM_FUNC(GUI_CreateBitmapFromStreamM555) 
    SYMBOL_ITEM_FUNC(GUI_CreateBitmapFromStreamM565) 
    SYMBOL_ITEM_FUNC(GUI_CreateBitmapFromStreamRLE16) 
    SYMBOL_ITEM_FUNC(GUI_CreateBitmapFromStreamRLE32) 
    SYMBOL_ITEM_FUNC(GUI_CreateBitmapFromStreamRLE4) 
    SYMBOL_ITEM_FUNC(GUI_CreateBitmapFromStreamRLE8) 
    SYMBOL_ITEM_FUNC(GUI_CreateBitmapFromStreamRLEAlpha) 
    SYMBOL_ITEM_FUNC(GUI_CreateBitmapFromStreamRLEM16) 
    SYMBOL_ITEM_FUNC(GUI_CreateDialogBox) 
    SYMBOL_ITEM_FUNC(GUI_CursorArrowL) 
    SYMBOL_ITEM_FUNC(GUI_CursorArrowLI) 
    SYMBOL_ITEM_FUNC(GUI_CursorArrowM) 
    SYMBOL_ITEM_FUNC(GUI_CursorArrowMI) 
    SYMBOL_ITEM_FUNC(GUI_CursorArrowS) 
    SYMBOL_ITEM_FUNC(GUI_CursorArrowSI) 
    SYMBOL_ITEM_FUNC(GUI_CursorCrossL) 
    SYMBOL_ITEM_FUNC(GUI_CursorCrossLI) 
    SYMBOL_ITEM_FUNC(GUI_CursorCrossM) 
    SYMBOL_ITEM_FUNC(GUI_CursorCrossMI) 
    SYMBOL_ITEM_FUNC(GUI_CursorCrossS) 
    SYMBOL_ITEM_FUNC(GUI_CursorCrossSI) 
    SYMBOL_ITEM_FUNC(GUI_CursorHeaderM) 
    SYMBOL_ITEM_FUNC(GUI_CursorPal) 
    SYMBOL_ITEM_FUNC(GUI_CursorPalI) 
    SYMBOL_ITEM_FUNC(GUI_DEVICE_Create) 
    SYMBOL_ITEM_FUNC(GUI_DEVICE_CreateAndLink) 
    SYMBOL_ITEM_FUNC(GUI_DEVICE_Delete) 
    SYMBOL_ITEM_FUNC(GUI_DEVICE_Link) 
    SYMBOL_ITEM_FUNC(GUI_DEVICE_Unlink) 
    SYMBOL_ITEM_FUNC(GUI_DEVICE__GetpDevice) 
    SYMBOL_ITEM_FUNC(GUI_DEVICE__GetpDriver) 
    SYMBOL_ITEM_FUNC(GUI_DRAW__Draw) 
    SYMBOL_ITEM_FUNC(GUI_DRAW__GetXSize) 
    SYMBOL_ITEM_FUNC(GUI_DRAW__GetYSize) 
    SYMBOL_ITEM_FUNC(GUI_Delay) 
    SYMBOL_ITEM_FUNC(GUI_DispChar) 
    SYMBOL_ITEM_FUNC(GUI_DispCharAt) 
    SYMBOL_ITEM_FUNC(GUI_DispDec) 
    SYMBOL_ITEM_FUNC(GUI_DispDecAt) 
    SYMBOL_ITEM_FUNC(GUI_DispDecMin) 
    SYMBOL_ITEM_FUNC(GUI_DispDecShift) 
    SYMBOL_ITEM_FUNC(GUI_DispDecSpace) 
    SYMBOL_ITEM_FUNC(GUI_DispFloat) 
    SYMBOL_ITEM_FUNC(GUI_DispFloatFix) 
    SYMBOL_ITEM_FUNC(GUI_DispFloatMin) 
    SYMBOL_ITEM_FUNC(GUI_DispNextLine) 
    SYMBOL_ITEM_FUNC(GUI_DispSDec) 
    SYMBOL_ITEM_FUNC(GUI_DispSDecShift) 
    SYMBOL_ITEM_FUNC(GUI_DispSFloatFix) 
    SYMBOL_ITEM_FUNC(GUI_DispSFloatMin) 
    SYMBOL_ITEM_FUNC(GUI_DispString) 
    SYMBOL_ITEM_FUNC(GUI_DispStringAt) 
    SYMBOL_ITEM_FUNC(GUI_DispStringHCenterAt) 
    SYMBOL_ITEM_FUNC(GUI_DispStringInRect) 
    SYMBOL_ITEM_FUNC(GUI_DispStringInRectMax) 
    SYMBOL_ITEM_FUNC(GUI_DispStringInRectWrap) 
    SYMBOL_ITEM_FUNC(GUI_DrawArc) 
    SYMBOL_ITEM_FUNC(GUI_DrawBitmap) 
    SYMBOL_ITEM_FUNC(GUI_DrawBitmapMag) 
    SYMBOL_ITEM_FUNC(GUI_DrawCircle) 
    SYMBOL_ITEM_FUNC(GUI_DrawEllipse) 
    SYMBOL_ITEM_FUNC(GUI_DrawFocusRect) 
    SYMBOL_ITEM_FUNC(GUI_DrawGradientH) 
    SYMBOL_ITEM_FUNC(GUI_DrawGradientV) 
    SYMBOL_ITEM_FUNC(GUI_DrawGraphEx) 
    SYMBOL_ITEM_FUNC(GUI_DrawHLine) 
    SYMBOL_ITEM_FUNC(GUI_DrawLine) 
    SYMBOL_ITEM_FUNC(GUI_DrawLineRel) 
    SYMBOL_ITEM_FUNC(GUI_DrawLineTo) 
    SYMBOL_ITEM_FUNC(GUI_DrawPixel) 
    SYMBOL_ITEM_FUNC(GUI_DrawPoint) 
    SYMBOL_ITEM_FUNC(GUI_DrawRect) 
    SYMBOL_ITEM_FUNC(GUI_DrawRoundedRect) 
    SYMBOL_ITEM_FUNC(GUI_DrawStreamedBitmap) 
    SYMBOL_ITEM_FUNC(GUI_DrawStreamedBitmap24Ex) 
    SYMBOL_ITEM_FUNC(GUI_DrawStreamedBitmap555Ex) 
    SYMBOL_ITEM_FUNC(GUI_DrawStreamedBitmap565Ex) 
    SYMBOL_ITEM_FUNC(GUI_DrawStreamedBitmapAuto) 
    SYMBOL_ITEM_FUNC(GUI_DrawStreamedBitmapEx) 
    SYMBOL_ITEM_FUNC(GUI_DrawStreamedBitmapExAuto) 
    SYMBOL_ITEM_FUNC(GUI_DrawStreamedBitmapM555Ex) 
    SYMBOL_ITEM_FUNC(GUI_DrawStreamedBitmapM565Ex) 
    SYMBOL_ITEM_FUNC(GUI_DrawVLine) 
    SYMBOL_ITEM_FUNC(GUI_ENC_APIList_EXT) 
    SYMBOL_ITEM_FUNC(GUI_EndDialog) 
    SYMBOL_ITEM_FUNC(GUI_Error) 
    SYMBOL_ITEM_FUNC(GUI_ErrorOut) 
    SYMBOL_ITEM_FUNC(GUI_ErrorOut1) 
    SYMBOL_ITEM_FUNC(GUI_ErrorOut2) 
    SYMBOL_ITEM_FUNC(GUI_ErrorOut3) 
    SYMBOL_ITEM_FUNC(GUI_ErrorOut4) 
    SYMBOL_ITEM_FUNC(GUI_Exec) 
    SYMBOL_ITEM_FUNC(GUI_Exec1) 
    SYMBOL_ITEM_FUNC(GUI_ExecCreatedDialog) 
    SYMBOL_ITEM_FUNC(GUI_ExecDialogBox) 
    SYMBOL_ITEM_FUNC(GUI_Exit) 
    SYMBOL_ITEM_FUNC(GUI_FillCircle) 
    SYMBOL_ITEM_FUNC(GUI_FillEllipse) 
    SYMBOL_ITEM_FUNC(GUI_FillPolygon) 
    SYMBOL_ITEM_FUNC(GUI_FillRect) 
    SYMBOL_ITEM_FUNC(GUI_FillRectEx) 
    SYMBOL_ITEM_FUNC(GUI_FillRoundedFrame) 
    SYMBOL_ITEM_FUNC(GUI_FillRoundedRect) 
    SYMBOL_ITEM_FUNC(GUI_Font10S_ASCII) 
    SYMBOL_ITEM_FUNC(GUI_Font10S_ASCII_FontProp1) 
    SYMBOL_ITEM_FUNC(GUI_Font13ASCII_Prop1) 
    SYMBOL_ITEM_FUNC(GUI_Font13B_ASCII) 
    SYMBOL_ITEM_FUNC(GUI_Font13B_ASCII_Prop1) 
    SYMBOL_ITEM_FUNC(GUI_Font13_1) 
    SYMBOL_ITEM_FUNC(GUI_Font13_ASCII) 
    SYMBOL_ITEM_FUNC(GUI_Font18) 
    SYMBOL_ITEM_FUNC(GUI_Font18B_CharInfo) 
    SYMBOL_ITEM_FUNC(GUI_Font18_CharInfo) 
    SYMBOL_ITEM_FUNC(GUI_Font18_Prop1) 
    SYMBOL_ITEM_FUNC(GUI_Font20_ASCII) 
    SYMBOL_ITEM_FUNC(GUI_Font20_ASCII_CharInfo) 
    SYMBOL_ITEM_FUNC(GUI_Font20_ASCII_Prop1) 
    SYMBOL_ITEM_FUNC(GUI_Font24B_ASCII) 
    SYMBOL_ITEM_FUNC(GUI_Font24B_ASCII_CharInfo) 
    SYMBOL_ITEM_FUNC(GUI_Font60BD) 
    SYMBOL_ITEM_FUNC(GUI_Font60BD_CharInfo) 
    SYMBOL_ITEM_FUNC(GUI_Font60BD_Prop1) 
    SYMBOL_ITEM_FUNC(GUI_Font60min) 
    SYMBOL_ITEM_FUNC(GUI_Font60min_CharInfo) 
    SYMBOL_ITEM_FUNC(GUI_Font60min_Prop1) 
    SYMBOL_ITEM_FUNC(GUI_Font60min_Prop2) 
    SYMBOL_ITEM_FUNC(GUI_Font6x8) 
    SYMBOL_ITEM_FUNC(GUI_Font6x9) 
    SYMBOL_ITEM_FUNC(GUI_Font8ASCII_Prop) 
    SYMBOL_ITEM_FUNC(GUI_Font8_1) 
    SYMBOL_ITEM_FUNC(GUI_Font8_ASCII) 
    SYMBOL_ITEM_FUNC(GUI_FontComic18B_ASCII) 
    SYMBOL_ITEM_FUNC(GUI_FontComic24B_ASCII) 
    SYMBOL_ITEM_FUNC(GUI_FontD6x8) 
    SYMBOL_ITEM_FUNC(GUI_FontD9_AA4) 
    SYMBOL_ITEM_FUNC(GUI_FontD9_AA4_CharInfo) 
    SYMBOL_ITEM_FUNC(GUI_FontD9_AA4_Prop1) 
    SYMBOL_ITEM_FUNC(GUI_FontRounded16) 
    SYMBOL_ITEM_FUNC(GUI_FontRounded16_CharInfo) 
    SYMBOL_ITEM_FUNC(GUI_FontRounded16_Prop1) 
    SYMBOL_ITEM_FUNC(GUI_FontRounded22) 
    SYMBOL_ITEM_FUNC(GUI_FontRounded22_CharInfo) 
    SYMBOL_ITEM_FUNC(GUI_FontRounded22_Prop1) 
    SYMBOL_ITEM_FUNC(GUI_FontSouvenir18) 
    SYMBOL_ITEM_FUNC(GUI_FontSouvenir18_CharInfo) 
    SYMBOL_ITEM_FUNC(GUI_FontSouvenir18_Prop1) 
    SYMBOL_ITEM_FUNC(GUI_GetBitmapPixelIndex) 
    SYMBOL_ITEM_FUNC(GUI_GetBitmapPixelIndexEx) 
    SYMBOL_ITEM_FUNC(GUI_GetBitsPerPixelEx) 
    SYMBOL_ITEM_FUNC(GUI_GetBkColor) 
    SYMBOL_ITEM_FUNC(GUI_GetBkColorIndex) 
    SYMBOL_ITEM_FUNC(GUI_GetCharDistX) 
    SYMBOL_ITEM_FUNC(GUI_GetCharDistXEx) 
    SYMBOL_ITEM_FUNC(GUI_GetClientRect) 
    SYMBOL_ITEM_FUNC(GUI_GetColor) 
    SYMBOL_ITEM_FUNC(GUI_GetColorIndex) 
    SYMBOL_ITEM_FUNC(GUI_GetCursorLayer) 
    SYMBOL_ITEM_FUNC(GUI_GetDialogStatusPtr) 
    SYMBOL_ITEM_FUNC(GUI_GetFontDistY) 
    SYMBOL_ITEM_FUNC(GUI_GetFontSizeY) 
    SYMBOL_ITEM_FUNC(GUI_GetKey) 
    SYMBOL_ITEM_FUNC(GUI_GetKeyState) 
    SYMBOL_ITEM_FUNC(GUI_GetLayerPosEx) 
    SYMBOL_ITEM_FUNC(GUI_GetLineStyle) 
    SYMBOL_ITEM_FUNC(GUI_GetOrg) 
    SYMBOL_ITEM_FUNC(GUI_GetPenShape) 
    SYMBOL_ITEM_FUNC(GUI_GetPenSize) 
    SYMBOL_ITEM_FUNC(GUI_GetPixelIndex) 
    SYMBOL_ITEM_FUNC(GUI_GetStreamedBitmapInfo) 
    SYMBOL_ITEM_FUNC(GUI_GetStreamedBitmapInfoEx) 
    SYMBOL_ITEM_FUNC(GUI_GetStringDistX) 
    SYMBOL_ITEM_FUNC(GUI_GetTime) 
    SYMBOL_ITEM_FUNC(GUI_GetVersionString) 
    SYMBOL_ITEM_FUNC(GUI_GetYAdjust) 
    SYMBOL_ITEM_FUNC(GUI_GetYDistOfFont) 
    SYMBOL_ITEM_FUNC(GUI_GetYSizeOfFont) 
    SYMBOL_ITEM_FUNC(GUI_GetpfColor2IndexEx) 
    SYMBOL_ITEM_FUNC(GUI_GetpfIndex2ColorEx) 
    SYMBOL_ITEM_FUNC(GUI_GotoX) 
    SYMBOL_ITEM_FUNC(GUI_GotoXY) 
    SYMBOL_ITEM_FUNC(GUI_GotoY) 
    SYMBOL_ITEM_FUNC(GUI_Init) 
    SYMBOL_ITEM_FUNC(GUI_JPEG_Draw) 
    SYMBOL_ITEM_FUNC(GUI_JPEG_DrawEx) 
    SYMBOL_ITEM_FUNC(GUI_JPEG_GetInfo) 
    SYMBOL_ITEM_FUNC(GUI_JPEG_GetInfoEx) 
    SYMBOL_ITEM_FUNC(GUI_JPEG__AllocBandingCoeffBuffer) 
    SYMBOL_ITEM_FUNC(GUI_JPEG__DecodeLine) 
    SYMBOL_ITEM_FUNC(GUI_JPEG__DecodeProgressiveBanding) 
    SYMBOL_ITEM_FUNC(GUI_JPEG__Free) 
    SYMBOL_ITEM_FUNC(GUI_JPEG__GetColorGray) 
    SYMBOL_ITEM_FUNC(GUI_JPEG__GetColorRGB) 
    SYMBOL_ITEM_FUNC(GUI_JPEG__GetData) 
    SYMBOL_ITEM_FUNC(GUI_JPEG__GetInfoEx) 
    SYMBOL_ITEM_FUNC(GUI_JPEG__InitDraw) 
    SYMBOL_ITEM_FUNC(GUI_JPEG__ReadUntilSOF) 
    SYMBOL_ITEM_FUNC(GUI_JPEG__SetNextBand) 
    SYMBOL_ITEM_FUNC(GUI_JPEG__SkipLine) 
    SYMBOL_ITEM_FUNC(GUI_Lock) 
    SYMBOL_ITEM_FUNC(GUI_Log) 
    SYMBOL_ITEM_FUNC(GUI_Log1) 
    SYMBOL_ITEM_FUNC(GUI_Log2) 
    SYMBOL_ITEM_FUNC(GUI_Log3) 
    SYMBOL_ITEM_FUNC(GUI_Log4) 
    SYMBOL_ITEM_FUNC(GUI_Long2Len) 
    SYMBOL_ITEM_FUNC(GUI_MEASDEV_ClearRect) 
    SYMBOL_ITEM_FUNC(GUI_MEASDEV_Create) 
    SYMBOL_ITEM_FUNC(GUI_MEASDEV_Delete) 
    SYMBOL_ITEM_FUNC(GUI_MEASDEV_GetRect) 
    SYMBOL_ITEM_FUNC(GUI_MEASDEV_Select) 
    SYMBOL_ITEM_FUNC(GUI_MEMDEV_Clear) 
    SYMBOL_ITEM_FUNC(GUI_MEMDEV_CopyFromLCD) 
    SYMBOL_ITEM_FUNC(GUI_MEMDEV_CopyToLCD) 
    SYMBOL_ITEM_FUNC(GUI_MEMDEV_CopyToLCDAt) 
    SYMBOL_ITEM_FUNC(GUI_MEMDEV_Create) 
    SYMBOL_ITEM_FUNC(GUI_MEMDEV_CreateAuto) 
    SYMBOL_ITEM_FUNC(GUI_MEMDEV_CreateEx) 
    SYMBOL_ITEM_FUNC(GUI_MEMDEV_CreateFixed) 
    SYMBOL_ITEM_FUNC(GUI_MEMDEV_CreateStatic) 
    SYMBOL_ITEM_FUNC(GUI_MEMDEV_DEVICE_16) 
    SYMBOL_ITEM_FUNC(GUI_MEMDEV_DEVICE_32) 
    SYMBOL_ITEM_FUNC(GUI_MEMDEV_Delete) 
    SYMBOL_ITEM_FUNC(GUI_MEMDEV_DeleteAuto) 
    SYMBOL_ITEM_FUNC(GUI_MEMDEV_Draw) 
    SYMBOL_ITEM_FUNC(GUI_MEMDEV_DrawAuto) 
    SYMBOL_ITEM_FUNC(GUI_MEMDEV_DrawPerspectiveX) 
    SYMBOL_ITEM_FUNC(GUI_MEMDEV_FadeDevices) 
    SYMBOL_ITEM_FUNC(GUI_MEMDEV_GetBitsPerPixel) 
    SYMBOL_ITEM_FUNC(GUI_MEMDEV_GetDataPtr) 
    SYMBOL_ITEM_FUNC(GUI_MEMDEV_GetStaticDevice) 
    SYMBOL_ITEM_FUNC(GUI_MEMDEV_GetWindowDevice) 
    SYMBOL_ITEM_FUNC(GUI_MEMDEV_GetXPos) 
    SYMBOL_ITEM_FUNC(GUI_MEMDEV_GetXSize) 
    SYMBOL_ITEM_FUNC(GUI_MEMDEV_GetYPos) 
    SYMBOL_ITEM_FUNC(GUI_MEMDEV_GetYSize) 
    SYMBOL_ITEM_FUNC(GUI_MEMDEV_MarkDirty) 
    SYMBOL_ITEM_FUNC(GUI_MEMDEV_Paint1Static) 
    SYMBOL_ITEM_FUNC(GUI_MEMDEV_ReduceYSize) 
    SYMBOL_ITEM_FUNC(GUI_MEMDEV_Rotate) 
    SYMBOL_ITEM_FUNC(GUI_MEMDEV_RotateHQ) 
    SYMBOL_ITEM_FUNC(GUI_MEMDEV_Select) 
    SYMBOL_ITEM_FUNC(GUI_MEMDEV_SetAnimationCallback) 
    SYMBOL_ITEM_FUNC(GUI_MEMDEV_SetOrg) 
    SYMBOL_ITEM_FUNC(GUI_MEMDEV_Write) 
    SYMBOL_ITEM_FUNC(GUI_MEMDEV_WriteAt) 
    SYMBOL_ITEM_FUNC(GUI_MEMDEV__CreateFixed) 
    SYMBOL_ITEM_FUNC(GUI_MEMDEV__DrawSizedAt) 
    SYMBOL_ITEM_FUNC(GUI_MEMDEV__FadeDevice) 
    SYMBOL_ITEM_FUNC(GUI_MEMDEV__FadeDeviceEx) 
    SYMBOL_ITEM_FUNC(GUI_MEMDEV__Rotate) 
    SYMBOL_ITEM_FUNC(GUI_MEMDEV__WriteToActiveAlpha) 
    SYMBOL_ITEM_FUNC(GUI_MEMDEV__WriteToActiveAt) 
    SYMBOL_ITEM_FUNC(GUI_MEMDEV__WriteToActiveOpaque) 
    SYMBOL_ITEM_FUNC(GUI_MEMDEV__XY2PTR) 
    SYMBOL_ITEM_FUNC(GUI_MEMDEV__XY2PTREx) 
    SYMBOL_ITEM_FUNC(GUI_MULTIBUF_Begin) 
    SYMBOL_ITEM_FUNC(GUI_MULTIBUF_BeginEx) 
    SYMBOL_ITEM_FUNC(GUI_MULTIBUF_Config) 
    SYMBOL_ITEM_FUNC(GUI_MULTIBUF_ConfigEx) 
    SYMBOL_ITEM_FUNC(GUI_MULTIBUF_Confirm) 
    SYMBOL_ITEM_FUNC(GUI_MULTIBUF_ConfirmEx) 
    SYMBOL_ITEM_FUNC(GUI_MULTIBUF_End) 
    SYMBOL_ITEM_FUNC(GUI_MULTIBUF_EndEx) 
    SYMBOL_ITEM_FUNC(GUI_MULTIBUF_GetNumBuffers) 
    SYMBOL_ITEM_FUNC(GUI_MULTIBUF_GetNumBuffersEx) 
    SYMBOL_ITEM_FUNC(GUI_MULTIBUF_UseSingleBuffer) 
    SYMBOL_ITEM_FUNC(GUI_MergeRect) 
    SYMBOL_ITEM_FUNC(GUI_MoveRect) 
    SYMBOL_ITEM_FUNC(GUI_MoveRel) 
    SYMBOL_ITEM_FUNC(GUI_MoveTo) 
    SYMBOL_ITEM_FUNC(GUI_PID_GetCurrentState) 
    SYMBOL_ITEM_FUNC(GUI_PID_GetState) 
    SYMBOL_ITEM_FUNC(GUI_PID_IsEmpty) 
    SYMBOL_ITEM_FUNC(GUI_PID_IsPressed) 
    SYMBOL_ITEM_FUNC(GUI_PID_StoreState) 
    SYMBOL_ITEM_FUNC(GUI_PixelsHeaderM) 
    SYMBOL_ITEM_FUNC(GUI_Pixels_ArrowL) 
    SYMBOL_ITEM_FUNC(GUI_Pixels_ArrowM) 
    SYMBOL_ITEM_FUNC(GUI_Pixels_ArrowS) 
    SYMBOL_ITEM_FUNC(GUI_Pixels_CrossL) 
    SYMBOL_ITEM_FUNC(GUI_Pixels_CrossM) 
    SYMBOL_ITEM_FUNC(GUI_Pixels_CrossS) 
    SYMBOL_ITEM_FUNC(GUI_PollKeyMsg) 
    SYMBOL_ITEM_FUNC(GUI_Pow10) 
    SYMBOL_ITEM_FUNC(GUI_ReadRect) 
    SYMBOL_ITEM_FUNC(GUI_ReadRectEx) 
    SYMBOL_ITEM_FUNC(GUI_RectsIntersect) 
    SYMBOL_ITEM_FUNC(GUI_RestoreContext) 
    SYMBOL_ITEM_FUNC(GUI_RotatePolygon) 
    SYMBOL_ITEM_FUNC(GUI_SIF_APIList_Prop_Frm) 
    SYMBOL_ITEM_FUNC(GUI_SIF_CreateFont) 
    SYMBOL_ITEM_FUNC(GUI_SIF_DeleteFont) 
    SYMBOL_ITEM_FUNC(GUI_SIF__ClearLine_ExtFrm) 
    SYMBOL_ITEM_FUNC(GUI_SIF__GetCharDistX) 
    SYMBOL_ITEM_FUNC(GUI_SIF__GetCharDistX_ExtFrm) 
    SYMBOL_ITEM_FUNC(GUI_SIF__GetCharInfo_ExtFrm) 
    SYMBOL_ITEM_FUNC(GUI_SIF__GetFontInfo) 
    SYMBOL_ITEM_FUNC(GUI_SIF__GetFontInfo_ExtFrm) 
    SYMBOL_ITEM_FUNC(GUI_SIF__GetNumCharAreas) 
    SYMBOL_ITEM_FUNC(GUI_SIF__GetpCharInfo) 
    SYMBOL_ITEM_FUNC(GUI_SIF__IsInFont) 
    SYMBOL_ITEM_FUNC(GUI_SIF__IsInFont_ExtFrm) 
    SYMBOL_ITEM_FUNC(GUI_SPRITE_Create) 
    SYMBOL_ITEM_FUNC(GUI_SPRITE_CreateEx) 
    SYMBOL_ITEM_FUNC(GUI_SPRITE_Delete) 
    SYMBOL_ITEM_FUNC(GUI_SPRITE_GetState) 
    SYMBOL_ITEM_FUNC(GUI_SPRITE_Hide) 
    SYMBOL_ITEM_FUNC(GUI_SPRITE_SetBitmap) 
    SYMBOL_ITEM_FUNC(GUI_SPRITE_SetBitmapAndPosition) 
    SYMBOL_ITEM_FUNC(GUI_SPRITE_SetPosition) 
    SYMBOL_ITEM_FUNC(GUI_SPRITE_Show) 
    SYMBOL_ITEM_FUNC(GUI_SPRITE__CreateEx) 
    SYMBOL_ITEM_FUNC(GUI_SPRITE__SetCallback) 
    SYMBOL_ITEM_FUNC(GUI_SaveContext_W) 
    SYMBOL_ITEM_FUNC(GUI_SelectLCD) 
    SYMBOL_ITEM_FUNC(GUI_SelectLayer) 
    SYMBOL_ITEM_FUNC(GUI_SendKeyMsg) 
    SYMBOL_ITEM_FUNC(GUI_SetAlpha) 
    SYMBOL_ITEM_FUNC(GUI_SetBkColor) 
    SYMBOL_ITEM_FUNC(GUI_SetBkColorIndex) 
    SYMBOL_ITEM_FUNC(GUI_SetClipRect) 
    SYMBOL_ITEM_FUNC(GUI_SetColor) 
    SYMBOL_ITEM_FUNC(GUI_SetColorIndex) 
    SYMBOL_ITEM_FUNC(GUI_SetDefault) 
    SYMBOL_ITEM_FUNC(GUI_SetDefaultFont) 
    SYMBOL_ITEM_FUNC(GUI_SetDialogStatusPtr) 
    SYMBOL_ITEM_FUNC(GUI_SetDrawMode) 
    SYMBOL_ITEM_FUNC(GUI_SetFont) 
    SYMBOL_ITEM_FUNC(GUI_SetLayerAlphaEx) 
    SYMBOL_ITEM_FUNC(GUI_SetLayerPosEx) 
    SYMBOL_ITEM_FUNC(GUI_SetLayerSizeEx) 
    SYMBOL_ITEM_FUNC(GUI_SetLayerVisEx) 
    SYMBOL_ITEM_FUNC(GUI_SetLineStyle) 
    SYMBOL_ITEM_FUNC(GUI_SetOrg) 
    SYMBOL_ITEM_FUNC(GUI_SetPenShape) 
    SYMBOL_ITEM_FUNC(GUI_SetPenSize) 
    SYMBOL_ITEM_FUNC(GUI_SetSignalEventFunc) 
    SYMBOL_ITEM_FUNC(GUI_SetStreamedBitmapHook) 
    SYMBOL_ITEM_FUNC(GUI_SetTextAlign) 
    SYMBOL_ITEM_FUNC(GUI_SetTextMode) 
    SYMBOL_ITEM_FUNC(GUI_SetWaitEventFunc) 
    SYMBOL_ITEM_FUNC(GUI_SetWaitEventTimedFunc) 
    SYMBOL_ITEM_FUNC(GUI_SignalEvent) 
    SYMBOL_ITEM_FUNC(GUI_StoreKey) 
    SYMBOL_ITEM_FUNC(GUI_StoreKeyMsg) 
    SYMBOL_ITEM_FUNC(GUI_TIMER_Create) 
    SYMBOL_ITEM_FUNC(GUI_TIMER_Delete) 
    SYMBOL_ITEM_FUNC(GUI_TIMER_Exec) 
    SYMBOL_ITEM_FUNC(GUI_TIMER_GetFlag) 
    SYMBOL_ITEM_FUNC(GUI_TIMER_Restart) 
    SYMBOL_ITEM_FUNC(GUI_TIMER_SetDelay) 
    SYMBOL_ITEM_FUNC(GUI_TIMER_SetPeriod) 
    SYMBOL_ITEM_FUNC(GUI_TIMER_SetTime) 
    SYMBOL_ITEM_FUNC(GUI_TIMER__GetFirstTimer) 
    SYMBOL_ITEM_FUNC(GUI_TIMER__GetNextTimer) 
    SYMBOL_ITEM_FUNC(GUI_TIMER__GetNextTimerLin) 
    SYMBOL_ITEM_FUNC(GUI_TIMER__GetPeriod) 
    SYMBOL_ITEM_FUNC(GUI_TIMER__IsActive) 
    SYMBOL_ITEM_FUNC(GUI_UC_Encode) 
    SYMBOL_ITEM_FUNC(GUI_UC_GetCharCode) 
    SYMBOL_ITEM_FUNC(GUI_UC_GetCharSize) 
    SYMBOL_ITEM_FUNC(GUI_UC_None) 
    SYMBOL_ITEM_FUNC(GUI_UC_SetEncodeNone) 
    SYMBOL_ITEM_FUNC(GUI_UC__CalcSizeOfChar) 
    SYMBOL_ITEM_FUNC(GUI_UC__GetCharCodeInc) 
    SYMBOL_ITEM_FUNC(GUI_UC__NumBytes2NumChars) 
    SYMBOL_ITEM_FUNC(GUI_UC__NumChars2NumBytes) 
    SYMBOL_ITEM_FUNC(GUI_USAGE_AddRect) 
    SYMBOL_ITEM_FUNC(GUI_USAGE_BM_Create) 
    SYMBOL_ITEM_FUNC(GUI_USAGE_DecUseCnt) 
    SYMBOL_ITEM_FUNC(GUI_USAGE_Select) 
    SYMBOL_ITEM_FUNC(GUI_Unlock) 
    SYMBOL_ITEM_FUNC(GUI_WaitEvent) 
    SYMBOL_ITEM_FUNC(GUI_Warn) 
    SYMBOL_ITEM_FUNC(GUI_Warn1) 
    SYMBOL_ITEM_FUNC(GUI_Warn2) 
    SYMBOL_ITEM_FUNC(GUI_Warn3) 
    SYMBOL_ITEM_FUNC(GUI_Warn4) 
    SYMBOL_ITEM_FUNC(GUI_WrapGetNumLines) 
    SYMBOL_ITEM_FUNC(GUI_X_Config) 
    SYMBOL_ITEM_FUNC(GUI_X_Delay) 
    SYMBOL_ITEM_FUNC(GUI_X_ErrorOut) 
    SYMBOL_ITEM_FUNC(GUI_X_ExecIdle) 
    SYMBOL_ITEM_FUNC(GUI_X_GetTaskId) 
    SYMBOL_ITEM_FUNC(GUI_X_GetTime) 
    SYMBOL_ITEM_FUNC(GUI_X_Init) 
    SYMBOL_ITEM_FUNC(GUI_X_InitOS) 
    SYMBOL_ITEM_FUNC(GUI_X_Lock) 
    SYMBOL_ITEM_FUNC(GUI_X_Log) 
    SYMBOL_ITEM_FUNC(GUI_X_Unlock) 
    SYMBOL_ITEM_FUNC(GUI_X_Warn) 
    SYMBOL_ITEM_FUNC(GUI__AddSpaceHex) 
    SYMBOL_ITEM_FUNC(GUI__AllocAlphaBuffer) 
    SYMBOL_ITEM_FUNC(GUI__AlphaSuppressMixing) 
    SYMBOL_ITEM_FUNC(GUI__CalcTextRect) 
    SYMBOL_ITEM_FUNC(GUI__CompactPixelIndices) 
    SYMBOL_ITEM_FUNC(GUI__CompactPixelIndicesEx) 
    SYMBOL_ITEM_FUNC(GUI__Config) 
    SYMBOL_ITEM_FUNC(GUI__CosHQ) 
    SYMBOL_ITEM_FUNC(GUI__CreateBitmapFromStream) 
    SYMBOL_ITEM_FUNC(GUI__DispLine) 
    SYMBOL_ITEM_FUNC(GUI__DispStringInRect) 
    SYMBOL_ITEM_FUNC(GUI__DivideRound32) 
    SYMBOL_ITEM_FUNC(GUI__DoAlphaBlending) 
    SYMBOL_ITEM_FUNC(GUI__DrawBitmap16bpp) 
    SYMBOL_ITEM_FUNC(GUI__DrawTwinArc2) 
    SYMBOL_ITEM_FUNC(GUI__DrawTwinArc4) 
    SYMBOL_ITEM_FUNC(GUI__ExpandPixelIndices) 
    SYMBOL_ITEM_FUNC(GUI__ExpandPixelIndicesEx) 
    SYMBOL_ITEM_FUNC(GUI__FillTrippleArc) 
    SYMBOL_ITEM_FUNC(GUI__GetAlphaBuffer) 
    SYMBOL_ITEM_FUNC(GUI__GetFontSizeY) 
    SYMBOL_ITEM_FUNC(GUI__GetLineDistX) 
    SYMBOL_ITEM_FUNC(GUI__GetLineNumChars) 
    SYMBOL_ITEM_FUNC(GUI__GetNumChars) 
    SYMBOL_ITEM_FUNC(GUI__GetOrg) 
    SYMBOL_ITEM_FUNC(GUI__GetOverlap) 
    SYMBOL_ITEM_FUNC(GUI__HandleEOLine) 
    SYMBOL_ITEM_FUNC(GUI__IntersectRect) 
    SYMBOL_ITEM_FUNC(GUI__IntersectRects) 
    SYMBOL_ITEM_FUNC(GUI__ManageCache) 
    SYMBOL_ITEM_FUNC(GUI__ManageCacheEx) 
    SYMBOL_ITEM_FUNC(GUI__MixColors) 
    SYMBOL_ITEM_FUNC(GUI__Read16) 
    SYMBOL_ITEM_FUNC(GUI__Read32) 
    SYMBOL_ITEM_FUNC(GUI__ReadHeaderFromStream) 
    SYMBOL_ITEM_FUNC(GUI__ReduceRect) 
    SYMBOL_ITEM_FUNC(GUI__RegisterExit) 
    SYMBOL_ITEM_FUNC(GUI__SetAlphaBufferSize) 
    SYMBOL_ITEM_FUNC(GUI__SetPixelAlpha) 
    SYMBOL_ITEM_FUNC(GUI__SetText) 
    SYMBOL_ITEM_FUNC(GUI__SinHQ) 
    SYMBOL_ITEM_FUNC(GUI__WrapGetNumBytesToNextLine) 
    SYMBOL_ITEM_FUNC(GUI__WrapGetNumCharsDisp) 
    SYMBOL_ITEM_FUNC(GUI__WrapGetNumCharsToNextLine) 
    SYMBOL_ITEM_FUNC(GUI__aConvert_255_31) 
    SYMBOL_ITEM_FUNC(GUI__aConvert_255_63) 
    SYMBOL_ITEM_FUNC(GUI__aConvert_31_255) 
    SYMBOL_ITEM_FUNC(GUI__aConvert_63_255) 
    SYMBOL_ITEM_FUNC(GUI__memset) 
    SYMBOL_ITEM_FUNC(GUI__memset16) 
    SYMBOL_ITEM_FUNC(GUI__sqrt32) 
    SYMBOL_ITEM_FUNC(GUI__strcmp) 
    SYMBOL_ITEM_FUNC(GUI__strcmp_hp) 
    SYMBOL_ITEM_FUNC(GUI__strlen) 
    SYMBOL_ITEM_FUNC(GUI_cos) 
    SYMBOL_ITEM_FUNC(GUI_sin) 
    SYMBOL_ITEM_FUNC(HEADER_AddItem) 
    SYMBOL_ITEM_FUNC(HEADER_Callback) 
    SYMBOL_ITEM_FUNC(HEADER_Create) 
    SYMBOL_ITEM_FUNC(HEADER_CreateEx) 
    SYMBOL_ITEM_FUNC(HEADER_CreateUser) 
    SYMBOL_ITEM_FUNC(HEADER_DeleteItem) 
    SYMBOL_ITEM_FUNC(HEADER_DrawSkinFlex) 
    SYMBOL_ITEM_FUNC(HEADER_GetHeight) 
    SYMBOL_ITEM_FUNC(HEADER_GetItemWidth) 
    SYMBOL_ITEM_FUNC(HEADER_GetNumItems) 
    SYMBOL_ITEM_FUNC(HEADER_GetSel) 
    SYMBOL_ITEM_FUNC(HEADER_GetSkinFlexProps) 
    SYMBOL_ITEM_FUNC(HEADER_GetUserData) 
    SYMBOL_ITEM_FUNC(HEADER_LockH) 
    SYMBOL_ITEM_FUNC(HEADER_SetDefaultSkin) 
    SYMBOL_ITEM_FUNC(HEADER_SetDefaultSkinClassic) 
    SYMBOL_ITEM_FUNC(HEADER_SetDirIndicator) 
    SYMBOL_ITEM_FUNC(HEADER_SetDragLimit) 
    SYMBOL_ITEM_FUNC(HEADER_SetFont) 
    SYMBOL_ITEM_FUNC(HEADER_SetHeight) 
    SYMBOL_ITEM_FUNC(HEADER_SetItemText) 
    SYMBOL_ITEM_FUNC(HEADER_SetItemWidth) 
    SYMBOL_ITEM_FUNC(HEADER_SetScrollPos) 
    SYMBOL_ITEM_FUNC(HEADER_SetSkin) 
    SYMBOL_ITEM_FUNC(HEADER_SetSkinClassic) 
    SYMBOL_ITEM_FUNC(HEADER_SetSkinFlexProps) 
    SYMBOL_ITEM_FUNC(HEADER_SetTextAlign) 
    SYMBOL_ITEM_FUNC(HEADER_SetUserData) 
    SYMBOL_ITEM_FUNC(HEADER__SkinClassic) 
    SYMBOL_ITEM_FUNC(ICONVIEW_AddBitmapItem) 
    SYMBOL_ITEM_FUNC(ICONVIEW_AddStreamedBitmapItem) 
    SYMBOL_ITEM_FUNC(ICONVIEW_Callback) 
    SYMBOL_ITEM_FUNC(ICONVIEW_CreateEx) 
    SYMBOL_ITEM_FUNC(ICONVIEW_CreateIndirect) 
    SYMBOL_ITEM_FUNC(ICONVIEW_CreateUser) 
    SYMBOL_ITEM_FUNC(ICONVIEW_DeleteItem) 
    SYMBOL_ITEM_FUNC(ICONVIEW_GetItemText) 
    SYMBOL_ITEM_FUNC(ICONVIEW_GetItemUserData) 
    SYMBOL_ITEM_FUNC(ICONVIEW_GetNumItems) 
    SYMBOL_ITEM_FUNC(ICONVIEW_GetSel) 
    SYMBOL_ITEM_FUNC(ICONVIEW_GetUserData) 
    SYMBOL_ITEM_FUNC(ICONVIEW_InsertBitmapItem) 
    SYMBOL_ITEM_FUNC(ICONVIEW_InsertStreamedBitmapItem) 
    SYMBOL_ITEM_FUNC(ICONVIEW_LockH) 
    SYMBOL_ITEM_FUNC(ICONVIEW_SetBitmapItem) 
    SYMBOL_ITEM_FUNC(ICONVIEW_SetBkColor) 
    SYMBOL_ITEM_FUNC(ICONVIEW_SetFont) 
    SYMBOL_ITEM_FUNC(ICONVIEW_SetFrame) 
    SYMBOL_ITEM_FUNC(ICONVIEW_SetItemText) 
    SYMBOL_ITEM_FUNC(ICONVIEW_SetItemUserData) 
    SYMBOL_ITEM_FUNC(ICONVIEW_SetSel) 
    SYMBOL_ITEM_FUNC(ICONVIEW_SetSpace) 
    SYMBOL_ITEM_FUNC(ICONVIEW_SetStreamedBitmapItem) 
    SYMBOL_ITEM_FUNC(ICONVIEW_SetTextAlign) 
    SYMBOL_ITEM_FUNC(ICONVIEW_SetTextColor) 
    SYMBOL_ITEM_FUNC(ICONVIEW_SetUserData) 
    SYMBOL_ITEM_FUNC(LCD_AA_MixColors) 
    SYMBOL_ITEM_FUNC(LCD_AA_SetAndMask) 
    SYMBOL_ITEM_FUNC(LCD_API_ColorConv_0) 
    SYMBOL_ITEM_FUNC(LCD_API_ColorConv_565) 
    SYMBOL_ITEM_FUNC(LCD_API_ColorConv_888) 
    SYMBOL_ITEM_FUNC(LCD_API_ColorConv_8888) 
    SYMBOL_ITEM_FUNC(LCD_Color2Index) 
    SYMBOL_ITEM_FUNC(LCD_DrawBitmap) 
    SYMBOL_ITEM_FUNC(LCD_DrawHLine) 
    SYMBOL_ITEM_FUNC(LCD_DrawPixel) 
    SYMBOL_ITEM_FUNC(LCD_DrawVLine) 
    SYMBOL_ITEM_FUNC(LCD_FillRect) 
    SYMBOL_ITEM_FUNC(LCD_GetBitsPerPixel) 
    SYMBOL_ITEM_FUNC(LCD_GetBitsPerPixelEx) 
    SYMBOL_ITEM_FUNC(LCD_GetBitsPerPixelMax) 
    SYMBOL_ITEM_FUNC(LCD_GetBkColorIndex) 
    SYMBOL_ITEM_FUNC(LCD_GetColorIndex) 
    SYMBOL_ITEM_FUNC(LCD_GetDevCap) 
    SYMBOL_ITEM_FUNC(LCD_GetDevCapEx) 
    SYMBOL_ITEM_FUNC(LCD_GetMirrorX) 
    SYMBOL_ITEM_FUNC(LCD_GetMirrorXEx) 
    SYMBOL_ITEM_FUNC(LCD_GetMirrorY) 
    SYMBOL_ITEM_FUNC(LCD_GetMirrorYEx) 
    SYMBOL_ITEM_FUNC(LCD_GetNextPixelAPI) 
    SYMBOL_ITEM_FUNC(LCD_GetNumColors) 
    SYMBOL_ITEM_FUNC(LCD_GetNumColorsEx) 
    SYMBOL_ITEM_FUNC(LCD_GetNumLayers) 
    SYMBOL_ITEM_FUNC(LCD_GetPalette) 
    SYMBOL_ITEM_FUNC(LCD_GetPaletteEx) 
    SYMBOL_ITEM_FUNC(LCD_GetPixelColor) 
    SYMBOL_ITEM_FUNC(LCD_GetPixelIndex) 
    SYMBOL_ITEM_FUNC(LCD_GetPos) 
    SYMBOL_ITEM_FUNC(LCD_GetPosEx) 
    SYMBOL_ITEM_FUNC(LCD_GetSwapXY) 
    SYMBOL_ITEM_FUNC(LCD_GetSwapXYEx) 
    SYMBOL_ITEM_FUNC(LCD_GetVXSize) 
    SYMBOL_ITEM_FUNC(LCD_GetVXSizeEx) 
    SYMBOL_ITEM_FUNC(LCD_GetVXSizeMax) 
    SYMBOL_ITEM_FUNC(LCD_GetVYSize) 
    SYMBOL_ITEM_FUNC(LCD_GetVYSizeEx) 
    SYMBOL_ITEM_FUNC(LCD_GetVYSizeMax) 
    SYMBOL_ITEM_FUNC(LCD_GetXMag) 
    SYMBOL_ITEM_FUNC(LCD_GetXMagEx) 
    SYMBOL_ITEM_FUNC(LCD_GetXSize) 
    SYMBOL_ITEM_FUNC(LCD_GetXSizeEx) 
    SYMBOL_ITEM_FUNC(LCD_GetXSizeMax) 
    SYMBOL_ITEM_FUNC(LCD_GetYMag) 
    SYMBOL_ITEM_FUNC(LCD_GetYMagEx) 
    SYMBOL_ITEM_FUNC(LCD_GetYSize) 
    SYMBOL_ITEM_FUNC(LCD_GetYSizeEx) 
    SYMBOL_ITEM_FUNC(LCD_GetYSizeMax) 
    SYMBOL_ITEM_FUNC(LCD_GetpPalConvTable) 
    SYMBOL_ITEM_FUNC(LCD_GetpPalConvTableUncached) 
    SYMBOL_ITEM_FUNC(LCD_GetpfColor2IndexEx) 
    SYMBOL_ITEM_FUNC(LCD_GetpfIndex2ColorEx) 
    SYMBOL_ITEM_FUNC(LCD_Index2Color) 
    SYMBOL_ITEM_FUNC(LCD_Index2Color_555) 
    SYMBOL_ITEM_FUNC(LCD_Index2Color_565) 
    SYMBOL_ITEM_FUNC(LCD_Index2Color_888) 
    SYMBOL_ITEM_FUNC(LCD_Index2Color_8888) 
    SYMBOL_ITEM_FUNC(LCD_Index2Color_M555) 
    SYMBOL_ITEM_FUNC(LCD_Index2Color_M565) 
    SYMBOL_ITEM_FUNC(LCD_Init) 
    SYMBOL_ITEM_FUNC(LCD_InitColors) 
    SYMBOL_ITEM_FUNC(LCD_InitLUT) 
    SYMBOL_ITEM_FUNC(LCD_ReadRect) 
    SYMBOL_ITEM_FUNC(LCD_ReadRectNoClip) 
    SYMBOL_ITEM_FUNC(LCD_Refresh) 
    SYMBOL_ITEM_FUNC(LCD_RefreshEx) 
    SYMBOL_ITEM_FUNC(LCD_SelectLCD) 
    SYMBOL_ITEM_FUNC(LCD_SetAlpha) 
    SYMBOL_ITEM_FUNC(LCD_SetAlphaEx) 
    SYMBOL_ITEM_FUNC(LCD_SetAlphaMode) 
    SYMBOL_ITEM_FUNC(LCD_SetAlphaModeEx) 
    SYMBOL_ITEM_FUNC(LCD_SetBkColor) 
    SYMBOL_ITEM_FUNC(LCD_SetBkColorIndex) 
    SYMBOL_ITEM_FUNC(LCD_SetChroma) 
    SYMBOL_ITEM_FUNC(LCD_SetChromaEx) 
    SYMBOL_ITEM_FUNC(LCD_SetChromaMode) 
    SYMBOL_ITEM_FUNC(LCD_SetChromaModeEx) 
    SYMBOL_ITEM_FUNC(LCD_SetClipRectEx) 
    SYMBOL_ITEM_FUNC(LCD_SetClipRectMax) 
    SYMBOL_ITEM_FUNC(LCD_SetColor) 
    SYMBOL_ITEM_FUNC(LCD_SetColorIndex) 
    SYMBOL_ITEM_FUNC(LCD_SetDevFunc) 
    SYMBOL_ITEM_FUNC(LCD_SetDrawMode) 
    SYMBOL_ITEM_FUNC(LCD_SetLUT) 
    SYMBOL_ITEM_FUNC(LCD_SetLUTEntry) 
    SYMBOL_ITEM_FUNC(LCD_SetLUTEntryEx) 
    SYMBOL_ITEM_FUNC(LCD_SetLUTEx) 
    SYMBOL_ITEM_FUNC(LCD_SetMaxNumColors) 
    SYMBOL_ITEM_FUNC(LCD_SetPixelAA) 
    SYMBOL_ITEM_FUNC(LCD_SetPixelAA_NoTrans) 
    SYMBOL_ITEM_FUNC(LCD_SetPixelAA_Xor) 
    SYMBOL_ITEM_FUNC(LCD_SetPixelIndex) 
    SYMBOL_ITEM_FUNC(LCD_SetPos) 
    SYMBOL_ITEM_FUNC(LCD_SetPosEx) 
    SYMBOL_ITEM_FUNC(LCD_SetSize) 
    SYMBOL_ITEM_FUNC(LCD_SetSizeEx) 
    SYMBOL_ITEM_FUNC(LCD_SetVRAMAddr) 
    SYMBOL_ITEM_FUNC(LCD_SetVRAMAddrEx) 
    SYMBOL_ITEM_FUNC(LCD_SetVSize) 
    SYMBOL_ITEM_FUNC(LCD_SetVSizeEx) 
    SYMBOL_ITEM_FUNC(LCD_SetVis) 
    SYMBOL_ITEM_FUNC(LCD_SetVisEx) 
    SYMBOL_ITEM_FUNC(LCD_UpdateColorIndices) 
    SYMBOL_ITEM_FUNC(LCD_X_Config) 
    SYMBOL_ITEM_FUNC(LCD_X_DisplayDriver) 
    SYMBOL_ITEM_FUNC(LCD__DrawBitmap_1bpp) 
    SYMBOL_ITEM_FUNC(LCD__GetBPP) 
    SYMBOL_ITEM_FUNC(LCD__GetBPPDevice) 
    SYMBOL_ITEM_FUNC(LCD__RLE16_SetFunc) 
    SYMBOL_ITEM_FUNC(LCD__RLE32_SetFunc) 
    SYMBOL_ITEM_FUNC(LCD__RLE4_SetFunc) 
    SYMBOL_ITEM_FUNC(LCD__RLE8_SetFunc) 
    SYMBOL_ITEM_FUNC(LCD_aMirror) 
    SYMBOL_ITEM_FUNC(LISTVIEW_AddColumn) 
    SYMBOL_ITEM_FUNC(LISTVIEW_AddRow) 
    SYMBOL_ITEM_FUNC(LISTVIEW_Callback) 
    SYMBOL_ITEM_FUNC(LISTVIEW_CompareDec) 
    SYMBOL_ITEM_FUNC(LISTVIEW_CompareText) 
    SYMBOL_ITEM_FUNC(LISTVIEW_CreateEx) 
    SYMBOL_ITEM_FUNC(LISTVIEW_CreateUser) 
    SYMBOL_ITEM_FUNC(LISTVIEW_DecSel) 
    SYMBOL_ITEM_FUNC(LISTVIEW_DeleteRow) 
    SYMBOL_ITEM_FUNC(LISTVIEW_DisableSort) 
    SYMBOL_ITEM_FUNC(LISTVIEW_EnableSort) 
    SYMBOL_ITEM_FUNC(LISTVIEW_GetBkColor) 
    SYMBOL_ITEM_FUNC(LISTVIEW_GetHeader) 
    SYMBOL_ITEM_FUNC(LISTVIEW_GetNumColumns) 
    SYMBOL_ITEM_FUNC(LISTVIEW_GetNumRows) 
    SYMBOL_ITEM_FUNC(LISTVIEW_GetSelUnsorted) 
    SYMBOL_ITEM_FUNC(LISTVIEW_GetTextColor) 
    SYMBOL_ITEM_FUNC(LISTVIEW_GetUserData) 
    SYMBOL_ITEM_FUNC(LISTVIEW_IncSel) 
    SYMBOL_ITEM_FUNC(LISTVIEW_InsertRow) 
    SYMBOL_ITEM_FUNC(LISTVIEW_LockH) 
    SYMBOL_ITEM_FUNC(LISTVIEW_SetAutoScrollH) 
    SYMBOL_ITEM_FUNC(LISTVIEW_SetAutoScrollV) 
    SYMBOL_ITEM_FUNC(LISTVIEW_SetCompareFunc) 
    SYMBOL_ITEM_FUNC(LISTVIEW_SetGridVis) 
    SYMBOL_ITEM_FUNC(LISTVIEW_SetHeaderHeight) 
    SYMBOL_ITEM_FUNC(LISTVIEW_SetItemBkColor) 
    SYMBOL_ITEM_FUNC(LISTVIEW_SetItemTextColor) 
    SYMBOL_ITEM_FUNC(LISTVIEW_SetLBorder) 
    SYMBOL_ITEM_FUNC(LISTVIEW_SetRBorder) 
    SYMBOL_ITEM_FUNC(LISTVIEW_SetSel) 
    SYMBOL_ITEM_FUNC(LISTVIEW_SetSelUnsorted) 
    SYMBOL_ITEM_FUNC(LISTVIEW_SetSort) 
    SYMBOL_ITEM_FUNC(LISTVIEW_SetUserData) 
    SYMBOL_ITEM_FUNC(LISTVIEW__GetNumColumns) 
    SYMBOL_ITEM_FUNC(LISTVIEW__GetNumRows) 
    SYMBOL_ITEM_FUNC(LISTVIEW__GetRowDistY) 
    SYMBOL_ITEM_FUNC(LISTVIEW__GetSel) 
    SYMBOL_ITEM_FUNC(LISTVIEW__InvalidateInsideArea) 
    SYMBOL_ITEM_FUNC(LISTVIEW__InvalidateRow) 
    SYMBOL_ITEM_FUNC(LISTVIEW__InvalidateRowAndBelow) 
    SYMBOL_ITEM_FUNC(LISTVIEW__MoveSel) 
    SYMBOL_ITEM_FUNC(LISTVIEW__SetSel) 
    SYMBOL_ITEM_FUNC(LISTVIEW__UpdateScrollParas) 
    SYMBOL_ITEM_FUNC(LISTVIEW__UpdateScrollPos) 
    SYMBOL_ITEM_FUNC(MIN) 
    SYMBOL_ITEM_FUNC(MainTask) 
    SYMBOL_ITEM_FUNC(PROGBAR_Callback) 
    SYMBOL_ITEM_FUNC(PROGBAR_CreateEx) 
    SYMBOL_ITEM_FUNC(PROGBAR_CreateIndirect) 
    SYMBOL_ITEM_FUNC(PROGBAR_CreateUser) 
    SYMBOL_ITEM_FUNC(PROGBAR_DrawSkinFlex) 
    SYMBOL_ITEM_FUNC(PROGBAR_GetSkinFlexProps) 
    SYMBOL_ITEM_FUNC(PROGBAR_GetUserData) 
    SYMBOL_ITEM_FUNC(PROGBAR_LockH) 
    SYMBOL_ITEM_FUNC(PROGBAR_SetDefaultSkin) 
    SYMBOL_ITEM_FUNC(PROGBAR_SetDefaultSkinClassic) 
    SYMBOL_ITEM_FUNC(PROGBAR_SetFont) 
    SYMBOL_ITEM_FUNC(PROGBAR_SetMinMax) 
    SYMBOL_ITEM_FUNC(PROGBAR_SetSkin) 
    SYMBOL_ITEM_FUNC(PROGBAR_SetSkinClassic) 
    SYMBOL_ITEM_FUNC(PROGBAR_SetSkinFlexProps) 
    SYMBOL_ITEM_FUNC(PROGBAR_SetTextAlign) 
    SYMBOL_ITEM_FUNC(PROGBAR_SetUserData) 
    SYMBOL_ITEM_FUNC(PROGBAR_SetValue) 
    SYMBOL_ITEM_FUNC(PROGBAR__GetTextLocked) 
    SYMBOL_ITEM_FUNC(PROGBAR__GetTextRect) 
    SYMBOL_ITEM_FUNC(PROGBAR__SkinClassic) 
    SYMBOL_ITEM_FUNC(PROGBAR__Value2Pos) 
    SYMBOL_ITEM_FUNC(SCROLLBAR_AddValue) 
    SYMBOL_ITEM_FUNC(SCROLLBAR_Callback) 
    SYMBOL_ITEM_FUNC(SCROLLBAR_Create) 
    SYMBOL_ITEM_FUNC(SCROLLBAR_CreateAttached) 
    SYMBOL_ITEM_FUNC(SCROLLBAR_CreateEx) 
    SYMBOL_ITEM_FUNC(SCROLLBAR_CreateUser) 
    SYMBOL_ITEM_FUNC(SCROLLBAR_Dec) 
    SYMBOL_ITEM_FUNC(SCROLLBAR_DrawSkinFlex) 
    SYMBOL_ITEM_FUNC(SCROLLBAR_GetSkinFlexProps) 
    SYMBOL_ITEM_FUNC(SCROLLBAR_GetUserData) 
    SYMBOL_ITEM_FUNC(SCROLLBAR_GetValue) 
    SYMBOL_ITEM_FUNC(SCROLLBAR_Inc) 
    SYMBOL_ITEM_FUNC(SCROLLBAR_LockH) 
    SYMBOL_ITEM_FUNC(SCROLLBAR_SetDefaultSkin) 
    SYMBOL_ITEM_FUNC(SCROLLBAR_SetDefaultSkinClassic) 
    SYMBOL_ITEM_FUNC(SCROLLBAR_SetNumItems) 
    SYMBOL_ITEM_FUNC(SCROLLBAR_SetPageSize) 
    SYMBOL_ITEM_FUNC(SCROLLBAR_SetSkin) 
    SYMBOL_ITEM_FUNC(SCROLLBAR_SetSkinClassic) 
    SYMBOL_ITEM_FUNC(SCROLLBAR_SetSkinFlexProps) 
    SYMBOL_ITEM_FUNC(SCROLLBAR_SetState) 
    SYMBOL_ITEM_FUNC(SCROLLBAR_SetUserData) 
    SYMBOL_ITEM_FUNC(SCROLLBAR_SetValue) 
    SYMBOL_ITEM_FUNC(SCROLLBAR__InvalidatePartner) 
    SYMBOL_ITEM_FUNC(SCROLLBAR__Rect2VRect) 
    SYMBOL_ITEM_FUNC(SCROLLBAR__SkinClassic) 
    SYMBOL_ITEM_FUNC(SLIDER_Callback) 
    SYMBOL_ITEM_FUNC(SLIDER_CreateEx) 
    SYMBOL_ITEM_FUNC(SLIDER_CreateIndirect) 
    SYMBOL_ITEM_FUNC(SLIDER_CreateUser) 
    SYMBOL_ITEM_FUNC(SLIDER_Dec) 
    SYMBOL_ITEM_FUNC(SLIDER_DrawSkinFlex) 
    SYMBOL_ITEM_FUNC(SLIDER_GetSkinFlexProps) 
    SYMBOL_ITEM_FUNC(SLIDER_GetUserData) 
    SYMBOL_ITEM_FUNC(SLIDER_GetValue) 
    SYMBOL_ITEM_FUNC(SLIDER_Inc) 
    SYMBOL_ITEM_FUNC(SLIDER_LockH) 
    SYMBOL_ITEM_FUNC(SLIDER_SetDefaultSkin) 
    SYMBOL_ITEM_FUNC(SLIDER_SetDefaultSkinClassic) 
    SYMBOL_ITEM_FUNC(SLIDER_SetNumTicks) 
    SYMBOL_ITEM_FUNC(SLIDER_SetRange) 
    SYMBOL_ITEM_FUNC(SLIDER_SetSkin) 
    SYMBOL_ITEM_FUNC(SLIDER_SetSkinClassic) 
    SYMBOL_ITEM_FUNC(SLIDER_SetSkinFlexProps) 
    SYMBOL_ITEM_FUNC(SLIDER_SetUserData) 
    SYMBOL_ITEM_FUNC(SLIDER_SetValue) 
    SYMBOL_ITEM_FUNC(SLIDER_SetWidth) 
    SYMBOL_ITEM_FUNC(SLIDER__SkinClassic) 
    SYMBOL_ITEM_FUNC(TEXT_Callback) 
    SYMBOL_ITEM_FUNC(TEXT_CreateEx) 
    SYMBOL_ITEM_FUNC(TEXT_CreateIndirect) 
    SYMBOL_ITEM_FUNC(TEXT_CreateUser) 
    SYMBOL_ITEM_FUNC(TEXT_GetDefaultFont) 
    SYMBOL_ITEM_FUNC(TEXT_GetNumLines) 
    SYMBOL_ITEM_FUNC(TEXT_GetUserData) 
    SYMBOL_ITEM_FUNC(TEXT_LockH) 
    SYMBOL_ITEM_FUNC(TEXT_SetDefaultFont) 
    SYMBOL_ITEM_FUNC(TEXT_SetDefaultTextColor) 
    SYMBOL_ITEM_FUNC(TEXT_SetDefaultWrapMode) 
    SYMBOL_ITEM_FUNC(TEXT_SetFont) 
    SYMBOL_ITEM_FUNC(TEXT_SetText) 
    SYMBOL_ITEM_FUNC(TEXT_SetTextAlign) 
    SYMBOL_ITEM_FUNC(TEXT_SetTextColor) 
    SYMBOL_ITEM_FUNC(TEXT_SetUserData) 
    SYMBOL_ITEM_FUNC(TEXT_SetWrapMode) 
    SYMBOL_ITEM_FUNC(TREEVIEW_AttachItem) 
    SYMBOL_ITEM_FUNC(TREEVIEW_Callback) 
    SYMBOL_ITEM_FUNC(TREEVIEW_CreateEx) 
    SYMBOL_ITEM_FUNC(TREEVIEW_CreateIndirect) 
    SYMBOL_ITEM_FUNC(TREEVIEW_CreateUser) 
    SYMBOL_ITEM_FUNC(TREEVIEW_DecSel) 
    SYMBOL_ITEM_FUNC(TREEVIEW_GetItem) 
    SYMBOL_ITEM_FUNC(TREEVIEW_GetSel) 
    SYMBOL_ITEM_FUNC(TREEVIEW_GetUserData) 
    SYMBOL_ITEM_FUNC(TREEVIEW_ITEM_Collapse) 
    SYMBOL_ITEM_FUNC(TREEVIEW_ITEM_CollapseAll) 
    SYMBOL_ITEM_FUNC(TREEVIEW_ITEM_Create) 
    SYMBOL_ITEM_FUNC(TREEVIEW_ITEM_Delete) 
    SYMBOL_ITEM_FUNC(TREEVIEW_ITEM_Detach) 
    SYMBOL_ITEM_FUNC(TREEVIEW_ITEM_Expand) 
    SYMBOL_ITEM_FUNC(TREEVIEW_ITEM_ExpandAll) 
    SYMBOL_ITEM_FUNC(TREEVIEW_ITEM_GetInfo) 
    SYMBOL_ITEM_FUNC(TREEVIEW_ITEM_GetText) 
    SYMBOL_ITEM_FUNC(TREEVIEW_ITEM_GetUserData) 
    SYMBOL_ITEM_FUNC(TREEVIEW_ITEM_SetImage) 
    SYMBOL_ITEM_FUNC(TREEVIEW_ITEM_SetText) 
    SYMBOL_ITEM_FUNC(TREEVIEW_ITEM_SetUserData) 
    SYMBOL_ITEM_FUNC(TREEVIEW_IncSel) 
    SYMBOL_ITEM_FUNC(TREEVIEW_InsertItem) 
    SYMBOL_ITEM_FUNC(TREEVIEW_LockH) 
    SYMBOL_ITEM_FUNC(TREEVIEW_OwnerDraw) 
    SYMBOL_ITEM_FUNC(TREEVIEW_SetAutoScrollH) 
    SYMBOL_ITEM_FUNC(TREEVIEW_SetAutoScrollV) 
    SYMBOL_ITEM_FUNC(TREEVIEW_SetBitmapOffset) 
    SYMBOL_ITEM_FUNC(TREEVIEW_SetBkColor) 
    SYMBOL_ITEM_FUNC(TREEVIEW_SetFont) 
    SYMBOL_ITEM_FUNC(TREEVIEW_SetHasLines) 
    SYMBOL_ITEM_FUNC(TREEVIEW_SetImage) 
    SYMBOL_ITEM_FUNC(TREEVIEW_SetIndent) 
    SYMBOL_ITEM_FUNC(TREEVIEW_SetLineColor) 
    SYMBOL_ITEM_FUNC(TREEVIEW_SetOwnerDraw) 
    SYMBOL_ITEM_FUNC(TREEVIEW_SetSel) 
    SYMBOL_ITEM_FUNC(TREEVIEW_SetSelMode) 
    SYMBOL_ITEM_FUNC(TREEVIEW_SetTextColor) 
    SYMBOL_ITEM_FUNC(TREEVIEW_SetTextIndent) 
    SYMBOL_ITEM_FUNC(TREEVIEW_SetUserData) 
    SYMBOL_ITEM_FUNC(TREEVIEW__bmClosed) 
    SYMBOL_ITEM_FUNC(TREEVIEW__bmLeaf) 
    SYMBOL_ITEM_FUNC(TREEVIEW__bmMinus) 
    SYMBOL_ITEM_FUNC(TREEVIEW__bmOpen) 
    SYMBOL_ITEM_FUNC(TREEVIEW__bmPlus) 
    SYMBOL_ITEM_FUNC(WIDGET_AndState) 
    SYMBOL_ITEM_FUNC(WIDGET_EFFECT_3D_DrawUp) 
    SYMBOL_ITEM_FUNC(WIDGET_EFFECT_3D_GetColor) 
    SYMBOL_ITEM_FUNC(WIDGET_EFFECT_3D_GetNumColors) 
    SYMBOL_ITEM_FUNC(WIDGET_EFFECT_3D_SetColor) 
    SYMBOL_ITEM_FUNC(WIDGET_EFFECT_Simple_GetColor) 
    SYMBOL_ITEM_FUNC(WIDGET_EFFECT_Simple_GetNumColors) 
    SYMBOL_ITEM_FUNC(WIDGET_EFFECT_Simple_SetColor) 
    SYMBOL_ITEM_FUNC(WIDGET_Effect_3D) 
    SYMBOL_ITEM_FUNC(WIDGET_Effect_None) 
    SYMBOL_ITEM_FUNC(WIDGET_Effect_Simple) 
    SYMBOL_ITEM_FUNC(WIDGET_GetDefaultEffect) 
    SYMBOL_ITEM_FUNC(WIDGET_GetState) 
    SYMBOL_ITEM_FUNC(WIDGET_HandleActive) 
    SYMBOL_ITEM_FUNC(WIDGET_OrState) 
    SYMBOL_ITEM_FUNC(WIDGET_SetDefaultEffect) 
    SYMBOL_ITEM_FUNC(WIDGET_SetEffect) 
    SYMBOL_ITEM_FUNC(WIDGET_SetState) 
    SYMBOL_ITEM_FUNC(WIDGET__DrawFocusRect) 
    SYMBOL_ITEM_FUNC(WIDGET__DrawHLine) 
    SYMBOL_ITEM_FUNC(WIDGET__DrawTriangle) 
    SYMBOL_ITEM_FUNC(WIDGET__DrawVLine) 
    SYMBOL_ITEM_FUNC(WIDGET__EFFECT_DrawDown) 
    SYMBOL_ITEM_FUNC(WIDGET__EFFECT_DrawDownRect) 
    SYMBOL_ITEM_FUNC(WIDGET__EFFECT_DrawUpRect) 
    SYMBOL_ITEM_FUNC(WIDGET__FillRectEx) 
    SYMBOL_ITEM_FUNC(WIDGET__FillStringInRect) 
    SYMBOL_ITEM_FUNC(WIDGET__GetBkColor) 
    SYMBOL_ITEM_FUNC(WIDGET__GetClientRect) 
    SYMBOL_ITEM_FUNC(WIDGET__GetInsideRect) 
    SYMBOL_ITEM_FUNC(WIDGET__GetWindowSizeX) 
    SYMBOL_ITEM_FUNC(WIDGET__GetXSize) 
    SYMBOL_ITEM_FUNC(WIDGET__GetYSize) 
    SYMBOL_ITEM_FUNC(WIDGET__Init) 
    SYMBOL_ITEM_FUNC(WIDGET__RotateRect90) 
    SYMBOL_ITEM_FUNC(WIDGET__SetScrollState) 
    SYMBOL_ITEM_FUNC(WM_Activate) 
    SYMBOL_ITEM_FUNC(WM_BringToBottom) 
    SYMBOL_ITEM_FUNC(WM_BringToTop) 
    SYMBOL_ITEM_FUNC(WM_CheckScrollBounds) 
    SYMBOL_ITEM_FUNC(WM_CheckScrollPos) 
    SYMBOL_ITEM_FUNC(WM_ClrHasTrans) 
    SYMBOL_ITEM_FUNC(WM_CreateTimer) 
    SYMBOL_ITEM_FUNC(WM_CreateWindow) 
    SYMBOL_ITEM_FUNC(WM_CreateWindowAsChild) 
    SYMBOL_ITEM_FUNC(WM_Deactivate) 
    SYMBOL_ITEM_FUNC(WM_DefaultProc) 
    SYMBOL_ITEM_FUNC(WM_DeleteTimer) 
    SYMBOL_ITEM_FUNC(WM_DeleteWindow) 
    SYMBOL_ITEM_FUNC(WM_DisableMemdev) 
    SYMBOL_ITEM_FUNC(WM_EnableMemdev) 
    SYMBOL_ITEM_FUNC(WM_Exec) 
    SYMBOL_ITEM_FUNC(WM_ForEachDesc) 
    SYMBOL_ITEM_FUNC(WM_GetActiveWindow) 
    SYMBOL_ITEM_FUNC(WM_GetBkColor) 
    SYMBOL_ITEM_FUNC(WM_GetClientRect) 
    SYMBOL_ITEM_FUNC(WM_GetClientRectEx) 
    SYMBOL_ITEM_FUNC(WM_GetClientWindow) 
    SYMBOL_ITEM_FUNC(WM_GetDesktopWindow) 
    SYMBOL_ITEM_FUNC(WM_GetDesktopWindowEx) 
    SYMBOL_ITEM_FUNC(WM_GetDialogItem) 
    SYMBOL_ITEM_FUNC(WM_GetFirstChild) 
    SYMBOL_ITEM_FUNC(WM_GetFlags) 
    SYMBOL_ITEM_FUNC(WM_GetFocussedWindow) 
    SYMBOL_ITEM_FUNC(WM_GetHasTrans) 
    SYMBOL_ITEM_FUNC(WM_GetId) 
    SYMBOL_ITEM_FUNC(WM_GetInsideRect) 
    SYMBOL_ITEM_FUNC(WM_GetInsideRectEx) 
    SYMBOL_ITEM_FUNC(WM_GetInsideRectExScrollbar) 
    SYMBOL_ITEM_FUNC(WM_GetNextSibling) 
    SYMBOL_ITEM_FUNC(WM_GetOrgX) 
    SYMBOL_ITEM_FUNC(WM_GetOrgY) 
    SYMBOL_ITEM_FUNC(WM_GetParent) 
    SYMBOL_ITEM_FUNC(WM_GetScrollPartner) 
    SYMBOL_ITEM_FUNC(WM_GetScrollState) 
    SYMBOL_ITEM_FUNC(WM_GetScrollbarH) 
    SYMBOL_ITEM_FUNC(WM_GetScrollbarV) 
    SYMBOL_ITEM_FUNC(WM_GetTimerId) 
    SYMBOL_ITEM_FUNC(WM_GetUserData) 
    SYMBOL_ITEM_FUNC(WM_GetWindowOrgX) 
    SYMBOL_ITEM_FUNC(WM_GetWindowOrgY) 
    SYMBOL_ITEM_FUNC(WM_GetWindowRect) 
    SYMBOL_ITEM_FUNC(WM_GetWindowRectEx) 
    SYMBOL_ITEM_FUNC(WM_GetWindowSizeX) 
    SYMBOL_ITEM_FUNC(WM_GetWindowSizeY) 
    SYMBOL_ITEM_FUNC(WM_HandlePID) 
    SYMBOL_ITEM_FUNC(WM_HasCaptured) 
    SYMBOL_ITEM_FUNC(WM_HasFocus) 
    SYMBOL_ITEM_FUNC(WM_HideWindow) 
    SYMBOL_ITEM_FUNC(WM_Init) 
    SYMBOL_ITEM_FUNC(WM_InvalidateRect) 
    SYMBOL_ITEM_FUNC(WM_InvalidateWindow) 
    SYMBOL_ITEM_FUNC(WM_InvalidateWindowAndDescs) 
    SYMBOL_ITEM_FUNC(WM_InvalidateWindowAndDescsEx) 
    SYMBOL_ITEM_FUNC(WM_IsFocussable) 
    SYMBOL_ITEM_FUNC(WM_IsVisible) 
    SYMBOL_ITEM_FUNC(WM_IsWindow) 
    SYMBOL_ITEM_FUNC(WM_MakeModal) 
    SYMBOL_ITEM_FUNC(WM_MoveChildTo) 
    SYMBOL_ITEM_FUNC(WM_MoveTo) 
    SYMBOL_ITEM_FUNC(WM_MoveWindow) 
    SYMBOL_ITEM_FUNC(WM_NotifyParent) 
    SYMBOL_ITEM_FUNC(WM_OnKey) 
    SYMBOL_ITEM_FUNC(WM_PID__GetPrevState) 
    SYMBOL_ITEM_FUNC(WM_PID__SetPrevState) 
    SYMBOL_ITEM_FUNC(WM_ReleaseCapture) 
    SYMBOL_ITEM_FUNC(WM_ResizeWindow) 
    SYMBOL_ITEM_FUNC(WM_RestartTimer) 
    SYMBOL_ITEM_FUNC(WM_Screen2hWin) 
    SYMBOL_ITEM_FUNC(WM_Screen2hWinEx) 
    SYMBOL_ITEM_FUNC(WM_SelectWindow) 
    SYMBOL_ITEM_FUNC(WM_SendMessage) 
    SYMBOL_ITEM_FUNC(WM_SendMessageNoPara) 
    SYMBOL_ITEM_FUNC(WM_SendToParent) 
    SYMBOL_ITEM_FUNC(WM_SetCallback) 
    SYMBOL_ITEM_FUNC(WM_SetCapture) 
    SYMBOL_ITEM_FUNC(WM_SetCaptureMove) 
    SYMBOL_ITEM_FUNC(WM_SetCreateFlags) 
    SYMBOL_ITEM_FUNC(WM_SetDefault) 
    SYMBOL_ITEM_FUNC(WM_SetFocus) 
    SYMBOL_ITEM_FUNC(WM_SetFocusOnNextChild) 
    SYMBOL_ITEM_FUNC(WM_SetFocusOnPrevChild) 
    SYMBOL_ITEM_FUNC(WM_SetHasTrans) 
    SYMBOL_ITEM_FUNC(WM_SetScrollState) 
    SYMBOL_ITEM_FUNC(WM_SetScrollValue) 
    SYMBOL_ITEM_FUNC(WM_SetScrollbarH) 
    SYMBOL_ITEM_FUNC(WM_SetScrollbarV) 
    SYMBOL_ITEM_FUNC(WM_SetSize) 
    SYMBOL_ITEM_FUNC(WM_SetUserClipRect) 
    SYMBOL_ITEM_FUNC(WM_SetUserData) 
    SYMBOL_ITEM_FUNC(WM_SetWindowPos) 
    SYMBOL_ITEM_FUNC(WM_ShowWindow) 
    SYMBOL_ITEM_FUNC(WM_ValidateWindow) 
    SYMBOL_ITEM_FUNC(WM__ActivateClipRect) 
    SYMBOL_ITEM_FUNC(WM__AddCriticalHandle) 
    SYMBOL_ITEM_FUNC(WM__Client2Screen) 
    SYMBOL_ITEM_FUNC(WM__ClipAtParentBorders) 
    SYMBOL_ITEM_FUNC(WM__DeleteAssocTimer) 
    SYMBOL_ITEM_FUNC(WM__DeleteSecure) 
    SYMBOL_ITEM_FUNC(WM__DetachWindow) 
    SYMBOL_ITEM_FUNC(WM__ForEachDesc) 
    SYMBOL_ITEM_FUNC(WM__GetClientRectEx) 
    SYMBOL_ITEM_FUNC(WM__GetClientRectWin) 
    SYMBOL_ITEM_FUNC(WM__GetFirstSibling) 
    SYMBOL_ITEM_FUNC(WM__GetFocussedChild) 
    SYMBOL_ITEM_FUNC(WM__GetLastSibling) 
    SYMBOL_ITEM_FUNC(WM__GetNextIVR) 
    SYMBOL_ITEM_FUNC(WM__GetOrgX_AA) 
    SYMBOL_ITEM_FUNC(WM__GetOrgY_AA) 
    SYMBOL_ITEM_FUNC(WM__GetPrevSibling) 
    SYMBOL_ITEM_FUNC(WM__GetTopLevelLayer) 
    SYMBOL_ITEM_FUNC(WM__GetUserDataEx) 
    SYMBOL_ITEM_FUNC(WM__GetWindowSizeX) 
    SYMBOL_ITEM_FUNC(WM__GetWindowSizeY) 
    SYMBOL_ITEM_FUNC(WM__InitIVRSearch) 
    SYMBOL_ITEM_FUNC(WM__InsertWindowIntoList) 
    SYMBOL_ITEM_FUNC(WM__Invalidate1Abs) 
    SYMBOL_ITEM_FUNC(WM__InvalidateDrawAndDescs) 
    SYMBOL_ITEM_FUNC(WM__InvalidateParent) 
    SYMBOL_ITEM_FUNC(WM__InvalidateRect) 
    SYMBOL_ITEM_FUNC(WM__InvalidateRectEx) 
    SYMBOL_ITEM_FUNC(WM__IsAncestor) 
    SYMBOL_ITEM_FUNC(WM__IsAncestorOrSelf) 
    SYMBOL_ITEM_FUNC(WM__IsChild) 
    SYMBOL_ITEM_FUNC(WM__IsEnabled) 
    SYMBOL_ITEM_FUNC(WM__IsInModalArea) 
    SYMBOL_ITEM_FUNC(WM__IsInWindow) 
    SYMBOL_ITEM_FUNC(WM__IsWindow) 
    SYMBOL_ITEM_FUNC(WM__MoveTo) 
    SYMBOL_ITEM_FUNC(WM__MoveWindow) 
    SYMBOL_ITEM_FUNC(WM__Paint) 
    SYMBOL_ITEM_FUNC(WM__Paint1) 
    SYMBOL_ITEM_FUNC(WM__RectIsNZ) 
    SYMBOL_ITEM_FUNC(WM__RemoveCriticalHandle) 
    SYMBOL_ITEM_FUNC(WM__RemoveFromLinList) 
    SYMBOL_ITEM_FUNC(WM__RemoveWindowFromList) 
    SYMBOL_ITEM_FUNC(WM__Screen2Client) 
    SYMBOL_ITEM_FUNC(WM__SelectTopLevelLayer) 
    SYMBOL_ITEM_FUNC(WM__SendMessage) 
    SYMBOL_ITEM_FUNC(WM__SendMessageIfEnabled) 
    SYMBOL_ITEM_FUNC(WM__SendMessageNoPara) 
    SYMBOL_ITEM_FUNC(WM__SendMsgNoData) 
    SYMBOL_ITEM_FUNC(WM__SendPIDMessage) 
    SYMBOL_ITEM_FUNC(WM__SendTouchMessage) 
    SYMBOL_ITEM_FUNC(WM__SetLastTouched) 
    SYMBOL_ITEM_FUNC(WM__SetMotionCallback) 
    SYMBOL_ITEM_FUNC(WM__SetScrollbarH) 
    SYMBOL_ITEM_FUNC(WM__SetScrollbarV) 
    SYMBOL_ITEM_FUNC(WM__SetToolTipCallback) 
    SYMBOL_ITEM_FUNC(WM__SetUserDataEx) 
    SYMBOL_ITEM_FUNC(WM__UpdateChildPositions) 
    SYMBOL_ITEM_FUNC(_Balloc) 
    SYMBOL_ITEM_FUNC(_Bfree) 
    SYMBOL_ITEM_FUNC(_Unwind_Backtrace) 
    SYMBOL_ITEM_FUNC(_Unwind_Complete) 
    SYMBOL_ITEM_FUNC(_Unwind_DeleteException) 
    SYMBOL_ITEM_FUNC(_Unwind_ForcedUnwind) 
    SYMBOL_ITEM_FUNC(_Unwind_GetCFA) 
    SYMBOL_ITEM_FUNC(_Unwind_GetDataRelBase) 
    SYMBOL_ITEM_FUNC(_Unwind_GetLanguageSpecificData) 
    SYMBOL_ITEM_FUNC(_Unwind_GetRegionStart) 
    SYMBOL_ITEM_FUNC(_Unwind_GetTextRelBase) 
    SYMBOL_ITEM_FUNC(_Unwind_RaiseException) 
    SYMBOL_ITEM_FUNC(_Unwind_Resume) 
    SYMBOL_ITEM_FUNC(_Unwind_Resume_or_Rethrow) 
    SYMBOL_ITEM_FUNC(_Unwind_VRS_Get) 
    SYMBOL_ITEM_FUNC(_Unwind_VRS_Pop) 
    SYMBOL_ITEM_FUNC(_Unwind_VRS_Set) 
    SYMBOL_ITEM_FUNC(___Unwind_Backtrace) 
    SYMBOL_ITEM_FUNC(___Unwind_ForcedUnwind) 
    SYMBOL_ITEM_FUNC(___Unwind_RaiseException) 
    SYMBOL_ITEM_FUNC(___Unwind_Resume) 
    SYMBOL_ITEM_FUNC(___Unwind_Resume_or_Rethrow) 
    SYMBOL_ITEM_FUNC(__adddf3) 
    SYMBOL_ITEM_FUNC(__addsf3) 
    SYMBOL_ITEM_FUNC(__aeabi_cdcmpeq) 
    SYMBOL_ITEM_FUNC(__aeabi_cdcmple) 
    SYMBOL_ITEM_FUNC(__aeabi_cdrcmple) 
    SYMBOL_ITEM_FUNC(__aeabi_cfcmpeq) 
    SYMBOL_ITEM_FUNC(__aeabi_cfcmple) 
    SYMBOL_ITEM_FUNC(__aeabi_cfrcmple) 
    SYMBOL_ITEM_FUNC(__aeabi_d2f) 
    SYMBOL_ITEM_FUNC(__aeabi_d2iz) 
    SYMBOL_ITEM_FUNC(__aeabi_d2uiz) 
    SYMBOL_ITEM_FUNC(__aeabi_dadd) 
    SYMBOL_ITEM_FUNC(__aeabi_dcmpeq) 
    SYMBOL_ITEM_FUNC(__aeabi_dcmpge) 
    SYMBOL_ITEM_FUNC(__aeabi_dcmpgt) 
    SYMBOL_ITEM_FUNC(__aeabi_dcmple) 
    SYMBOL_ITEM_FUNC(__aeabi_dcmplt) 
    SYMBOL_ITEM_FUNC(__aeabi_ddiv) 
    SYMBOL_ITEM_FUNC(__aeabi_dmul) 
    SYMBOL_ITEM_FUNC(__aeabi_drsub) 
    SYMBOL_ITEM_FUNC(__aeabi_dsub) 
    SYMBOL_ITEM_FUNC(__aeabi_f2d) 
    SYMBOL_ITEM_FUNC(__aeabi_f2iz) 
    SYMBOL_ITEM_FUNC(__aeabi_f2uiz) 
    SYMBOL_ITEM_FUNC(__aeabi_fadd) 
    SYMBOL_ITEM_FUNC(__aeabi_fcmpeq) 
    SYMBOL_ITEM_FUNC(__aeabi_fcmpge) 
    SYMBOL_ITEM_FUNC(__aeabi_fcmpgt) 
    SYMBOL_ITEM_FUNC(__aeabi_fcmple) 
    SYMBOL_ITEM_FUNC(__aeabi_fcmplt) 
    SYMBOL_ITEM_FUNC(__aeabi_fdiv) 
    SYMBOL_ITEM_FUNC(__aeabi_fmul) 
    SYMBOL_ITEM_FUNC(__aeabi_frsub) 
    SYMBOL_ITEM_FUNC(__aeabi_fsub) 
    SYMBOL_ITEM_FUNC(__aeabi_i2d) 
    SYMBOL_ITEM_FUNC(__aeabi_i2f) 
    SYMBOL_ITEM_FUNC(__aeabi_idiv) 
    SYMBOL_ITEM_FUNC(__aeabi_idivmod) 
    SYMBOL_ITEM_FUNC(__aeabi_l2d) 
    SYMBOL_ITEM_FUNC(__aeabi_l2f) 
    SYMBOL_ITEM_FUNC(__aeabi_ui2d) 
    SYMBOL_ITEM_FUNC(__aeabi_ui2f) 
    SYMBOL_ITEM_FUNC(__aeabi_uidiv) 
    SYMBOL_ITEM_FUNC(__aeabi_uidivmod) 
    SYMBOL_ITEM_FUNC(__aeabi_ul2d) 
    SYMBOL_ITEM_FUNC(__aeabi_ul2f) 
    SYMBOL_ITEM_FUNC(__aeabi_uldivmod) 
    SYMBOL_ITEM_FUNC(__aeabi_unwind_cpp_pr0) 
    SYMBOL_ITEM_FUNC(__any_on) 
    SYMBOL_ITEM_FUNC(__ascii_mbtowc) 
    SYMBOL_ITEM_FUNC(__ascii_wctomb) 
    SYMBOL_ITEM_FUNC(__assert) 
    SYMBOL_ITEM_FUNC(__assert_func) 
    SYMBOL_ITEM_FUNC(__b2d) 
    SYMBOL_ITEM_FUNC(__clzsi2) 
    SYMBOL_ITEM_FUNC(__cmpdf2) 
    SYMBOL_ITEM_FUNC(__cmpsf2) 
    SYMBOL_ITEM_FUNC(__copybits) 
    SYMBOL_ITEM_FUNC(__d2b) 
    SYMBOL_ITEM_FUNC(__divdf3) 
    SYMBOL_ITEM_FUNC(__divdi3) 
    SYMBOL_ITEM_FUNC(__divsf3) 
    SYMBOL_ITEM_FUNC(__divsi3) 
    SYMBOL_ITEM_FUNC(__env_lock) 
    SYMBOL_ITEM_FUNC(__env_unlock) 
    SYMBOL_ITEM_FUNC(__eqdf2) 
    SYMBOL_ITEM_FUNC(__eqsf2) 
    SYMBOL_ITEM_FUNC(__errno) 
    SYMBOL_ITEM_FUNC(__extendsfdf2) 
    SYMBOL_ITEM_FUNC(__ffssi2) 
    SYMBOL_ITEM_FUNC(__fixdfsi) 
    SYMBOL_ITEM_FUNC(__fixsfsi) 
    SYMBOL_ITEM_FUNC(__fixunsdfsi) 
    SYMBOL_ITEM_FUNC(__fixunssfsi) 
    SYMBOL_ITEM_FUNC(__floatdidf) 
    SYMBOL_ITEM_FUNC(__floatdisf) 
    SYMBOL_ITEM_FUNC(__floatsidf) 
    SYMBOL_ITEM_FUNC(__floatsisf) 
    SYMBOL_ITEM_FUNC(__floatundidf) 
    SYMBOL_ITEM_FUNC(__floatundisf) 
    SYMBOL_ITEM_FUNC(__floatunsidf) 
    SYMBOL_ITEM_FUNC(__floatunsisf) 
    SYMBOL_ITEM_FUNC(__fp_lock_all) 
    SYMBOL_ITEM_FUNC(__fp_unlock_all) 
    SYMBOL_ITEM_FUNC(__fpclassifyd) 
    SYMBOL_ITEM_FUNC(__gedf2) 
    SYMBOL_ITEM_FUNC(__gesf2) 
    SYMBOL_ITEM_FUNC(__gethex) 
    SYMBOL_ITEM_FUNC(__gettzinfo) 
    SYMBOL_ITEM_FUNC(__gnu_Unwind_Backtrace) 
    SYMBOL_ITEM_FUNC(__gnu_Unwind_ForcedUnwind) 
    SYMBOL_ITEM_FUNC(__gnu_Unwind_RaiseException) 
    SYMBOL_ITEM_FUNC(__gnu_Unwind_Restore_VFP) 
    SYMBOL_ITEM_FUNC(__gnu_Unwind_Restore_VFP_D) 
    SYMBOL_ITEM_FUNC(__gnu_Unwind_Restore_VFP_D_16_to_31) 
    SYMBOL_ITEM_FUNC(__gnu_Unwind_Restore_WMMXC) 
    SYMBOL_ITEM_FUNC(__gnu_Unwind_Restore_WMMXD) 
    SYMBOL_ITEM_FUNC(__gnu_Unwind_Resume) 
    SYMBOL_ITEM_FUNC(__gnu_Unwind_Resume_or_Rethrow) 
    SYMBOL_ITEM_FUNC(__gnu_Unwind_Save_VFP) 
    SYMBOL_ITEM_FUNC(__gnu_Unwind_Save_VFP_D) 
    SYMBOL_ITEM_FUNC(__gnu_Unwind_Save_VFP_D_16_to_31) 
    SYMBOL_ITEM_FUNC(__gnu_Unwind_Save_WMMXC) 
    SYMBOL_ITEM_FUNC(__gnu_Unwind_Save_WMMXD) 
    SYMBOL_ITEM_FUNC(__gnu_ldivmod_helper) 
    SYMBOL_ITEM_FUNC(__gnu_uldivmod_helper) 
    SYMBOL_ITEM_FUNC(__gnu_unwind_execute) 
    SYMBOL_ITEM_FUNC(__gnu_unwind_frame) 
    SYMBOL_ITEM_FUNC(__gtdf2) 
    SYMBOL_ITEM_FUNC(__gtsf2) 
    SYMBOL_ITEM_FUNC(__hexdig_init) 
    SYMBOL_ITEM_FUNC(__hexnan) 
    SYMBOL_ITEM_FUNC(__hi0bits) 
    SYMBOL_ITEM_FUNC(__i2b) 
    SYMBOL_ITEM_FUNC(__ieee754_sqrt) 
    SYMBOL_ITEM_FUNC(__kfree) 
    SYMBOL_ITEM_FUNC(__kmalloc) 
    SYMBOL_ITEM_FUNC(__ledf2) 
    SYMBOL_ITEM_FUNC(__lesf2) 
    SYMBOL_ITEM_FUNC(__lo0bits) 
    SYMBOL_ITEM_FUNC(__locale_charset) 
    SYMBOL_ITEM_FUNC(__locale_cjk_lang) 
    SYMBOL_ITEM_FUNC(__locale_mb_cur_max) 
    SYMBOL_ITEM_FUNC(__locale_msgcharset) 
    SYMBOL_ITEM_FUNC(__lshift) 
    SYMBOL_ITEM_FUNC(__ltdf2) 
    SYMBOL_ITEM_FUNC(__ltsf2) 
    SYMBOL_ITEM_FUNC(__mcmp) 
    SYMBOL_ITEM_FUNC(__mdiff) 
    SYMBOL_ITEM_FUNC(__mprec_bigtens) 
    SYMBOL_ITEM_FUNC(__mprec_tens) 
    SYMBOL_ITEM_FUNC(__mprec_tinytens) 
    SYMBOL_ITEM_FUNC(__muldf3) 
    SYMBOL_ITEM_FUNC(__mulsf3) 
    SYMBOL_ITEM_FUNC(__multadd) 
    SYMBOL_ITEM_FUNC(__multiply) 
    SYMBOL_ITEM_FUNC(__nedf2) 
    SYMBOL_ITEM_FUNC(__nesf2) 
    SYMBOL_ITEM_FUNC(__pow5mult) 
    SYMBOL_ITEM_FUNC(__ratio) 
    SYMBOL_ITEM_FUNC(__restore_core_regs) 
    SYMBOL_ITEM_FUNC(__s2b) 
    SYMBOL_ITEM_FUNC(__sccl) 
    SYMBOL_ITEM_FUNC(__sclose) 
    SYMBOL_ITEM_FUNC(__seofread) 
    SYMBOL_ITEM_FUNC(__sflags) 
    SYMBOL_ITEM_FUNC(__sfmoreglue) 
    SYMBOL_ITEM_FUNC(__sfp) 
    SYMBOL_ITEM_FUNC(__sfp_lock_acquire) 
    SYMBOL_ITEM_FUNC(__sfp_lock_release) 
    SYMBOL_ITEM_FUNC(__sfvwrite_r) 
    SYMBOL_ITEM_FUNC(__sinit) 
    SYMBOL_ITEM_FUNC(__sinit_lock_acquire) 
    SYMBOL_ITEM_FUNC(__sinit_lock_release) 
    SYMBOL_ITEM_FUNC(__smakebuf_r) 
    SYMBOL_ITEM_FUNC(__sprint_r) 
    SYMBOL_ITEM_FUNC(__sread) 
    SYMBOL_ITEM_FUNC(__sseek) 
    SYMBOL_ITEM_FUNC(__ssprint_r) 
    SYMBOL_ITEM_FUNC(__ssrefill_r) 
    SYMBOL_ITEM_FUNC(__ssvfiscanf_r) 
    SYMBOL_ITEM_FUNC(__ssvfscanf_r) 
    SYMBOL_ITEM_FUNC(__subdf3) 
    SYMBOL_ITEM_FUNC(__submore) 
    SYMBOL_ITEM_FUNC(__subsf3) 
    SYMBOL_ITEM_FUNC(__swbuf) 
    SYMBOL_ITEM_FUNC(__swbuf_r) 
    SYMBOL_ITEM_FUNC(__swrite) 
    SYMBOL_ITEM_FUNC(__swsetup_r) 
    SYMBOL_ITEM_FUNC(__truncdfsf2) 
    SYMBOL_ITEM_FUNC(__tz_lock) 
    SYMBOL_ITEM_FUNC(__tz_unlock) 
    SYMBOL_ITEM_FUNC(__tzcalc_limits) 
    SYMBOL_ITEM_FUNC(__udivdi3) 
    SYMBOL_ITEM_FUNC(__udivsi3) 
    SYMBOL_ITEM_FUNC(__ulp) 
    SYMBOL_ITEM_FUNC(_bmBrowser) 
    SYMBOL_ITEM_FUNC(_bmCheckMark) 
    SYMBOL_ITEM_FUNC(_bmClock) 
    SYMBOL_ITEM_FUNC(_bmDate) 
    SYMBOL_ITEM_FUNC(_bmEmail) 
    SYMBOL_ITEM_FUNC(_bmPassword) 
    SYMBOL_ITEM_FUNC(_bmRead) 
    SYMBOL_ITEM_FUNC(_bmRemote) 
    SYMBOL_ITEM_FUNC(_bmSystem) 
    SYMBOL_ITEM_FUNC(_bmToucan) 
    SYMBOL_ITEM_FUNC(_bmWrite) 
    SYMBOL_ITEM_FUNC(_calloc_r) 
    SYMBOL_ITEM_FUNC(_cleanup) 
    SYMBOL_ITEM_FUNC(_cleanup_r) 
    SYMBOL_ITEM_FUNC(_close_r) 
    SYMBOL_ITEM_FUNC(_ctype_) 
    SYMBOL_ITEM_FUNC(_dtoa_r) 
    SYMBOL_ITEM_FUNC(_execve_r) 
    SYMBOL_ITEM_FUNC(_exit) 
    SYMBOL_ITEM_FUNC(_fclose_r) 
    SYMBOL_ITEM_FUNC(_fdopen_r) 
    SYMBOL_ITEM_FUNC(_fflush_r) 
    SYMBOL_ITEM_FUNC(_findenv_r) 
    SYMBOL_ITEM_FUNC(_fini) 
    SYMBOL_ITEM_FUNC(_fiprintf_r) 
    SYMBOL_ITEM_FUNC(_fork_r) 
    SYMBOL_ITEM_FUNC(_fprintf_r) 
    SYMBOL_ITEM_FUNC(_fputwc_r) 
    SYMBOL_ITEM_FUNC(_free_r) 
    SYMBOL_ITEM_FUNC(_fstat_r) 
    SYMBOL_ITEM_FUNC(_fwalk) 
    SYMBOL_ITEM_FUNC(_fwalk_reent) 
    SYMBOL_ITEM_FUNC(_getenv_r) 
    SYMBOL_ITEM_FUNC(_getpid_r) 
    SYMBOL_ITEM_FUNC(_gettimeofday_r) 
    SYMBOL_ITEM_FUNC(_global_impure_ptr) 
    SYMBOL_ITEM_FUNC(_init) 
    SYMBOL_ITEM_FUNC(_isatty_r) 
    SYMBOL_ITEM_FUNC(_kill_r) 
    SYMBOL_ITEM_FUNC(_link_r) 
    SYMBOL_ITEM_FUNC(_localeconv_r) 
    SYMBOL_ITEM_FUNC(_lseek_r) 
    SYMBOL_ITEM_FUNC(_malloc_r) 
    SYMBOL_ITEM_FUNC(_mbrtowc_r) 
    SYMBOL_ITEM_FUNC(_mbtowc_r) 
    SYMBOL_ITEM_FUNC(_mkdir_r) 
    SYMBOL_ITEM_FUNC(_mktm_r) 
    SYMBOL_ITEM_FUNC(_mprec_log10) 
    SYMBOL_ITEM_FUNC(_open_r) 
    SYMBOL_ITEM_FUNC(_printf_r) 
    SYMBOL_ITEM_FUNC(_putenv_r) 
    SYMBOL_ITEM_FUNC(_read_r) 
    SYMBOL_ITEM_FUNC(_realloc_r) 
    SYMBOL_ITEM_FUNC(_rename_r) 
    SYMBOL_ITEM_FUNC(_rmdir_r) 
    SYMBOL_ITEM_FUNC(_sbrk_r) 
    SYMBOL_ITEM_FUNC(_setenv_r) 
    SYMBOL_ITEM_FUNC(_setlocale_r) 
    SYMBOL_ITEM_FUNC(_sfread_r) 
    SYMBOL_ITEM_FUNC(_snprintf_r) 
    SYMBOL_ITEM_FUNC(_sprintf_r) 
    SYMBOL_ITEM_FUNC(_sscanf_r) 
    SYMBOL_ITEM_FUNC(_start) 
    SYMBOL_ITEM_FUNC(_stat_r) 
    SYMBOL_ITEM_FUNC(_strdup_r) 
    SYMBOL_ITEM_FUNC(_strndup_r) 
    SYMBOL_ITEM_FUNC(_strtod_r) 
    SYMBOL_ITEM_FUNC(_strtol_r) 
    SYMBOL_ITEM_FUNC(_strtoul_r) 
    SYMBOL_ITEM_FUNC(_sungetc_r) 
    SYMBOL_ITEM_FUNC(_svfiprintf_r) 
    SYMBOL_ITEM_FUNC(_svfprintf_r) 
    SYMBOL_ITEM_FUNC(_times_r) 
    SYMBOL_ITEM_FUNC(_tzset_r) 
    SYMBOL_ITEM_FUNC(_ungetc_r) 
    SYMBOL_ITEM_FUNC(_unlink_r) 
    SYMBOL_ITEM_FUNC(_unsetenv_r) 
    SYMBOL_ITEM_FUNC(_vfiprintf_r) 
    SYMBOL_ITEM_FUNC(_vfprintf_r) 
    SYMBOL_ITEM_FUNC(_vsnprintf_r) 
    SYMBOL_ITEM_FUNC(_wait_r) 
    SYMBOL_ITEM_FUNC(_wcrtomb_r) 
    SYMBOL_ITEM_FUNC(_wctomb_r) 
    SYMBOL_ITEM_FUNC(_write_r) 
    SYMBOL_ITEM_FUNC(aTan) 
    SYMBOL_ITEM_FUNC(abort) 
    SYMBOL_ITEM_FUNC(acGUI_Font18B_0020) 
    SYMBOL_ITEM_FUNC(acGUI_Font18B_0021) 
    SYMBOL_ITEM_FUNC(acGUI_Font18B_0022) 
    SYMBOL_ITEM_FUNC(acGUI_Font18B_0023) 
    SYMBOL_ITEM_FUNC(acGUI_Font18B_0024) 
    SYMBOL_ITEM_FUNC(acGUI_Font18B_0025) 
    SYMBOL_ITEM_FUNC(acGUI_Font18B_0026) 
    SYMBOL_ITEM_FUNC(acGUI_Font18B_0027) 
    SYMBOL_ITEM_FUNC(acGUI_Font18B_0028) 
    SYMBOL_ITEM_FUNC(acGUI_Font18B_0029) 
    SYMBOL_ITEM_FUNC(acGUI_Font18B_002A) 
    SYMBOL_ITEM_FUNC(acGUI_Font18B_002B) 
    SYMBOL_ITEM_FUNC(acGUI_Font18B_002C) 
    SYMBOL_ITEM_FUNC(acGUI_Font18B_002D) 
    SYMBOL_ITEM_FUNC(acGUI_Font18B_002E) 
    SYMBOL_ITEM_FUNC(acGUI_Font18B_002F) 
    SYMBOL_ITEM_FUNC(acGUI_Font18B_0030) 
    SYMBOL_ITEM_FUNC(acGUI_Font18B_0031) 
    SYMBOL_ITEM_FUNC(acGUI_Font18B_0032) 
    SYMBOL_ITEM_FUNC(acGUI_Font18B_0033) 
    SYMBOL_ITEM_FUNC(acGUI_Font18B_0034) 
    SYMBOL_ITEM_FUNC(acGUI_Font18B_0035) 
    SYMBOL_ITEM_FUNC(acGUI_Font18B_0036) 
    SYMBOL_ITEM_FUNC(acGUI_Font18B_0037) 
    SYMBOL_ITEM_FUNC(acGUI_Font18B_0038) 
    SYMBOL_ITEM_FUNC(acGUI_Font18B_0039) 
    SYMBOL_ITEM_FUNC(acGUI_Font18B_003A) 
    SYMBOL_ITEM_FUNC(acGUI_Font18B_003B) 
    SYMBOL_ITEM_FUNC(acGUI_Font18B_003C) 
    SYMBOL_ITEM_FUNC(acGUI_Font18B_003D) 
    SYMBOL_ITEM_FUNC(acGUI_Font18B_003E) 
    SYMBOL_ITEM_FUNC(acGUI_Font18B_003F) 
    SYMBOL_ITEM_FUNC(acGUI_Font18B_0040) 
    SYMBOL_ITEM_FUNC(acGUI_Font18B_0041) 
    SYMBOL_ITEM_FUNC(acGUI_Font18B_0042) 
    SYMBOL_ITEM_FUNC(acGUI_Font18B_0043) 
    SYMBOL_ITEM_FUNC(acGUI_Font18B_0044) 
    SYMBOL_ITEM_FUNC(acGUI_Font18B_0045) 
    SYMBOL_ITEM_FUNC(acGUI_Font18B_0046) 
    SYMBOL_ITEM_FUNC(acGUI_Font18B_0047) 
    SYMBOL_ITEM_FUNC(acGUI_Font18B_0048) 
    SYMBOL_ITEM_FUNC(acGUI_Font18B_0049) 
    SYMBOL_ITEM_FUNC(acGUI_Font18B_004A) 
    SYMBOL_ITEM_FUNC(acGUI_Font18B_004B) 
    SYMBOL_ITEM_FUNC(acGUI_Font18B_004C) 
    SYMBOL_ITEM_FUNC(acGUI_Font18B_004D) 
    SYMBOL_ITEM_FUNC(acGUI_Font18B_004E) 
    SYMBOL_ITEM_FUNC(acGUI_Font18B_004F) 
    SYMBOL_ITEM_FUNC(acGUI_Font18B_0050) 
    SYMBOL_ITEM_FUNC(acGUI_Font18B_0051) 
    SYMBOL_ITEM_FUNC(acGUI_Font18B_0052) 
    SYMBOL_ITEM_FUNC(acGUI_Font18B_0053) 
    SYMBOL_ITEM_FUNC(acGUI_Font18B_0054) 
    SYMBOL_ITEM_FUNC(acGUI_Font18B_0055) 
    SYMBOL_ITEM_FUNC(acGUI_Font18B_0056) 
    SYMBOL_ITEM_FUNC(acGUI_Font18B_0057) 
    SYMBOL_ITEM_FUNC(acGUI_Font18B_0058) 
    SYMBOL_ITEM_FUNC(acGUI_Font18B_0059) 
    SYMBOL_ITEM_FUNC(acGUI_Font18B_005A) 
    SYMBOL_ITEM_FUNC(acGUI_Font18B_005B) 
    SYMBOL_ITEM_FUNC(acGUI_Font18B_005C) 
    SYMBOL_ITEM_FUNC(acGUI_Font18B_005D) 
    SYMBOL_ITEM_FUNC(acGUI_Font18B_005E) 
    SYMBOL_ITEM_FUNC(acGUI_Font18B_005F) 
    SYMBOL_ITEM_FUNC(acGUI_Font18B_0060) 
    SYMBOL_ITEM_FUNC(acGUI_Font18B_0061) 
    SYMBOL_ITEM_FUNC(acGUI_Font18B_0062) 
    SYMBOL_ITEM_FUNC(acGUI_Font18B_0063) 
    SYMBOL_ITEM_FUNC(acGUI_Font18B_0064) 
    SYMBOL_ITEM_FUNC(acGUI_Font18B_0065) 
    SYMBOL_ITEM_FUNC(acGUI_Font18B_0066) 
    SYMBOL_ITEM_FUNC(acGUI_Font18B_0067) 
    SYMBOL_ITEM_FUNC(acGUI_Font18B_0068) 
    SYMBOL_ITEM_FUNC(acGUI_Font18B_0069) 
    SYMBOL_ITEM_FUNC(acGUI_Font18B_006A) 
    SYMBOL_ITEM_FUNC(acGUI_Font18B_006B) 
    SYMBOL_ITEM_FUNC(acGUI_Font18B_006C) 
    SYMBOL_ITEM_FUNC(acGUI_Font18B_006D) 
    SYMBOL_ITEM_FUNC(acGUI_Font18B_006E) 
    SYMBOL_ITEM_FUNC(acGUI_Font18B_006F) 
    SYMBOL_ITEM_FUNC(acGUI_Font18B_0070) 
    SYMBOL_ITEM_FUNC(acGUI_Font18B_0071) 
    SYMBOL_ITEM_FUNC(acGUI_Font18B_0072) 
    SYMBOL_ITEM_FUNC(acGUI_Font18B_0073) 
    SYMBOL_ITEM_FUNC(acGUI_Font18B_0074) 
    SYMBOL_ITEM_FUNC(acGUI_Font18B_0075) 
    SYMBOL_ITEM_FUNC(acGUI_Font18B_0076) 
    SYMBOL_ITEM_FUNC(acGUI_Font18B_0077) 
    SYMBOL_ITEM_FUNC(acGUI_Font18B_0078) 
    SYMBOL_ITEM_FUNC(acGUI_Font18B_0079) 
    SYMBOL_ITEM_FUNC(acGUI_Font18B_007A) 
    SYMBOL_ITEM_FUNC(acGUI_Font18B_007B) 
    SYMBOL_ITEM_FUNC(acGUI_Font18B_007C) 
    SYMBOL_ITEM_FUNC(acGUI_Font18B_007D) 
    SYMBOL_ITEM_FUNC(acGUI_Font18B_007E) 
    SYMBOL_ITEM_FUNC(acGUI_Font18_0020) 
    SYMBOL_ITEM_FUNC(acGUI_Font18_0021) 
    SYMBOL_ITEM_FUNC(acGUI_Font18_0022) 
    SYMBOL_ITEM_FUNC(acGUI_Font18_0023) 
    SYMBOL_ITEM_FUNC(acGUI_Font18_0024) 
    SYMBOL_ITEM_FUNC(acGUI_Font18_0025) 
    SYMBOL_ITEM_FUNC(acGUI_Font18_0026) 
    SYMBOL_ITEM_FUNC(acGUI_Font18_0027) 
    SYMBOL_ITEM_FUNC(acGUI_Font18_0028) 
    SYMBOL_ITEM_FUNC(acGUI_Font18_0029) 
    SYMBOL_ITEM_FUNC(acGUI_Font18_002A) 
    SYMBOL_ITEM_FUNC(acGUI_Font18_002B) 
    SYMBOL_ITEM_FUNC(acGUI_Font18_002C) 
    SYMBOL_ITEM_FUNC(acGUI_Font18_002D) 
    SYMBOL_ITEM_FUNC(acGUI_Font18_002E) 
    SYMBOL_ITEM_FUNC(acGUI_Font18_002F) 
    SYMBOL_ITEM_FUNC(acGUI_Font18_0030) 
    SYMBOL_ITEM_FUNC(acGUI_Font18_0031) 
    SYMBOL_ITEM_FUNC(acGUI_Font18_0032) 
    SYMBOL_ITEM_FUNC(acGUI_Font18_0033) 
    SYMBOL_ITEM_FUNC(acGUI_Font18_0034) 
    SYMBOL_ITEM_FUNC(acGUI_Font18_0035) 
    SYMBOL_ITEM_FUNC(acGUI_Font18_0036) 
    SYMBOL_ITEM_FUNC(acGUI_Font18_0037) 
    SYMBOL_ITEM_FUNC(acGUI_Font18_0038) 
    SYMBOL_ITEM_FUNC(acGUI_Font18_0039) 
    SYMBOL_ITEM_FUNC(acGUI_Font18_003A) 
    SYMBOL_ITEM_FUNC(acGUI_Font18_003B) 
    SYMBOL_ITEM_FUNC(acGUI_Font18_003C) 
    SYMBOL_ITEM_FUNC(acGUI_Font18_003D) 
    SYMBOL_ITEM_FUNC(acGUI_Font18_003E) 
    SYMBOL_ITEM_FUNC(acGUI_Font18_003F) 
    SYMBOL_ITEM_FUNC(acGUI_Font18_0040) 
    SYMBOL_ITEM_FUNC(acGUI_Font18_0041) 
    SYMBOL_ITEM_FUNC(acGUI_Font18_0042) 
    SYMBOL_ITEM_FUNC(acGUI_Font18_0043) 
    SYMBOL_ITEM_FUNC(acGUI_Font18_0044) 
    SYMBOL_ITEM_FUNC(acGUI_Font18_0045) 
    SYMBOL_ITEM_FUNC(acGUI_Font18_0046) 
    SYMBOL_ITEM_FUNC(acGUI_Font18_0047) 
    SYMBOL_ITEM_FUNC(acGUI_Font18_0048) 
    SYMBOL_ITEM_FUNC(acGUI_Font18_0049) 
    SYMBOL_ITEM_FUNC(acGUI_Font18_004A) 
    SYMBOL_ITEM_FUNC(acGUI_Font18_004B) 
    SYMBOL_ITEM_FUNC(acGUI_Font18_004C) 
    SYMBOL_ITEM_FUNC(acGUI_Font18_004D) 
    SYMBOL_ITEM_FUNC(acGUI_Font18_004E) 
    SYMBOL_ITEM_FUNC(acGUI_Font18_004F) 
    SYMBOL_ITEM_FUNC(acGUI_Font18_0050) 
    SYMBOL_ITEM_FUNC(acGUI_Font18_0051) 
    SYMBOL_ITEM_FUNC(acGUI_Font18_0052) 
    SYMBOL_ITEM_FUNC(acGUI_Font18_0053) 
    SYMBOL_ITEM_FUNC(acGUI_Font18_0054) 
    SYMBOL_ITEM_FUNC(acGUI_Font18_0055) 
    SYMBOL_ITEM_FUNC(acGUI_Font18_0056) 
    SYMBOL_ITEM_FUNC(acGUI_Font18_0057) 
    SYMBOL_ITEM_FUNC(acGUI_Font18_0058) 
    SYMBOL_ITEM_FUNC(acGUI_Font18_0059) 
    SYMBOL_ITEM_FUNC(acGUI_Font18_005A) 
    SYMBOL_ITEM_FUNC(acGUI_Font18_005B) 
    SYMBOL_ITEM_FUNC(acGUI_Font18_005C) 
    SYMBOL_ITEM_FUNC(acGUI_Font18_005D) 
    SYMBOL_ITEM_FUNC(acGUI_Font18_005E) 
    SYMBOL_ITEM_FUNC(acGUI_Font18_005F) 
    SYMBOL_ITEM_FUNC(acGUI_Font18_0060) 
    SYMBOL_ITEM_FUNC(acGUI_Font18_0061) 
    SYMBOL_ITEM_FUNC(acGUI_Font18_0062) 
    SYMBOL_ITEM_FUNC(acGUI_Font18_0063) 
    SYMBOL_ITEM_FUNC(acGUI_Font18_0064) 
    SYMBOL_ITEM_FUNC(acGUI_Font18_0065) 
    SYMBOL_ITEM_FUNC(acGUI_Font18_0066) 
    SYMBOL_ITEM_FUNC(acGUI_Font18_0067) 
    SYMBOL_ITEM_FUNC(acGUI_Font18_0068) 
    SYMBOL_ITEM_FUNC(acGUI_Font18_0069) 
    SYMBOL_ITEM_FUNC(acGUI_Font18_006A) 
    SYMBOL_ITEM_FUNC(acGUI_Font18_006B) 
    SYMBOL_ITEM_FUNC(acGUI_Font18_006C) 
    SYMBOL_ITEM_FUNC(acGUI_Font18_006D) 
    SYMBOL_ITEM_FUNC(acGUI_Font18_006E) 
    SYMBOL_ITEM_FUNC(acGUI_Font18_006F) 
    SYMBOL_ITEM_FUNC(acGUI_Font18_0070) 
    SYMBOL_ITEM_FUNC(acGUI_Font18_0071) 
    SYMBOL_ITEM_FUNC(acGUI_Font18_0072) 
    SYMBOL_ITEM_FUNC(acGUI_Font18_0073) 
    SYMBOL_ITEM_FUNC(acGUI_Font18_0074) 
    SYMBOL_ITEM_FUNC(acGUI_Font18_0075) 
    SYMBOL_ITEM_FUNC(acGUI_Font18_0076) 
    SYMBOL_ITEM_FUNC(acGUI_Font18_0077) 
    SYMBOL_ITEM_FUNC(acGUI_Font18_0078) 
    SYMBOL_ITEM_FUNC(acGUI_Font18_0079) 
    SYMBOL_ITEM_FUNC(acGUI_Font18_007A) 
    SYMBOL_ITEM_FUNC(acGUI_Font18_007B) 
    SYMBOL_ITEM_FUNC(acGUI_Font18_007C) 
    SYMBOL_ITEM_FUNC(acGUI_Font18_007D) 
    SYMBOL_ITEM_FUNC(acGUI_Font18_007E) 
    SYMBOL_ITEM_FUNC(acGUI_Font60BD_0030) 
    SYMBOL_ITEM_FUNC(acGUI_Font60BD_0031) 
    SYMBOL_ITEM_FUNC(acGUI_Font60BD_0032) 
    SYMBOL_ITEM_FUNC(acGUI_Font60BD_0033) 
    SYMBOL_ITEM_FUNC(acGUI_Font60BD_0034) 
    SYMBOL_ITEM_FUNC(acGUI_Font60BD_0035) 
    SYMBOL_ITEM_FUNC(acGUI_Font60BD_0036) 
    SYMBOL_ITEM_FUNC(acGUI_Font60BD_0037) 
    SYMBOL_ITEM_FUNC(acGUI_Font60BD_0038) 
    SYMBOL_ITEM_FUNC(acGUI_Font60BD_0039) 
    SYMBOL_ITEM_FUNC(acGUI_Font60BD_003A) 
    SYMBOL_ITEM_FUNC(acGUI_Font60min_0069) 
    SYMBOL_ITEM_FUNC(acGUI_Font60min_006D) 
    SYMBOL_ITEM_FUNC(acGUI_Font60min_006E) 
    SYMBOL_ITEM_FUNC(acGUI_FontD9_AA4_0030) 
    SYMBOL_ITEM_FUNC(acGUI_FontD9_AA4_0031) 
    SYMBOL_ITEM_FUNC(acGUI_FontD9_AA4_0032) 
    SYMBOL_ITEM_FUNC(acGUI_FontD9_AA4_0033) 
    SYMBOL_ITEM_FUNC(acGUI_FontD9_AA4_0034) 
    SYMBOL_ITEM_FUNC(acGUI_FontD9_AA4_0035) 
    SYMBOL_ITEM_FUNC(acGUI_FontD9_AA4_0036) 
    SYMBOL_ITEM_FUNC(acGUI_FontD9_AA4_0037) 
    SYMBOL_ITEM_FUNC(acGUI_FontD9_AA4_0038) 
    SYMBOL_ITEM_FUNC(acGUI_FontD9_AA4_0039) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded16_0020) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded16_0021) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded16_0022) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded16_0023) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded16_0024) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded16_0025) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded16_0026) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded16_0027) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded16_0028) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded16_0029) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded16_002A) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded16_002B) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded16_002C) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded16_002D) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded16_002E) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded16_002F) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded16_0030) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded16_0031) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded16_0032) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded16_0033) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded16_0034) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded16_0035) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded16_0036) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded16_0037) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded16_0038) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded16_0039) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded16_003A) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded16_003B) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded16_003C) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded16_003D) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded16_003E) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded16_003F) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded16_0040) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded16_0041) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded16_0042) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded16_0043) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded16_0044) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded16_0045) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded16_0046) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded16_0047) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded16_0048) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded16_0049) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded16_004A) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded16_004B) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded16_004C) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded16_004D) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded16_004E) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded16_004F) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded16_0050) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded16_0051) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded16_0052) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded16_0053) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded16_0054) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded16_0055) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded16_0056) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded16_0057) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded16_0058) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded16_0059) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded16_005A) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded16_005B) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded16_005C) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded16_005D) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded16_005E) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded16_005F) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded16_0060) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded16_0061) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded16_0062) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded16_0063) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded16_0064) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded16_0065) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded16_0066) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded16_0067) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded16_0068) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded16_0069) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded16_006A) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded16_006B) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded16_006C) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded16_006D) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded16_006E) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded16_006F) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded16_0070) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded16_0071) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded16_0072) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded16_0073) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded16_0074) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded16_0075) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded16_0076) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded16_0077) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded16_0078) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded16_0079) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded16_007A) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded16_007B) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded16_007C) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded16_007D) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded16_007E) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded22_0020) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded22_0021) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded22_0022) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded22_0023) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded22_0024) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded22_0025) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded22_0026) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded22_0027) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded22_0028) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded22_0029) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded22_002A) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded22_002B) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded22_002C) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded22_002D) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded22_002E) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded22_002F) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded22_0030) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded22_0031) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded22_0032) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded22_0033) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded22_0034) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded22_0035) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded22_0036) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded22_0037) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded22_0038) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded22_0039) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded22_003A) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded22_003B) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded22_003C) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded22_003D) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded22_003E) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded22_003F) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded22_0040) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded22_0041) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded22_0042) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded22_0043) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded22_0044) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded22_0045) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded22_0046) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded22_0047) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded22_0048) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded22_0049) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded22_004A) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded22_004B) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded22_004C) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded22_004D) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded22_004E) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded22_004F) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded22_0050) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded22_0051) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded22_0052) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded22_0053) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded22_0054) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded22_0055) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded22_0056) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded22_0057) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded22_0058) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded22_0059) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded22_005A) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded22_005B) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded22_005C) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded22_005D) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded22_005E) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded22_005F) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded22_0060) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded22_0061) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded22_0062) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded22_0063) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded22_0064) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded22_0065) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded22_0066) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded22_0067) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded22_0068) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded22_0069) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded22_006A) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded22_006B) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded22_006C) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded22_006D) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded22_006E) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded22_006F) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded22_0070) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded22_0071) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded22_0072) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded22_0073) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded22_0074) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded22_0075) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded22_0076) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded22_0077) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded22_0078) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded22_0079) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded22_007A) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded22_007B) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded22_007C) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded22_007D) 
    SYMBOL_ITEM_FUNC(acGUI_FontRounded22_007E) 
    SYMBOL_ITEM_FUNC(acGUI_FontSouvenir18_0020) 
    SYMBOL_ITEM_FUNC(acGUI_FontSouvenir18_0021) 
    SYMBOL_ITEM_FUNC(acGUI_FontSouvenir18_0022) 
    SYMBOL_ITEM_FUNC(acGUI_FontSouvenir18_0023) 
    SYMBOL_ITEM_FUNC(acGUI_FontSouvenir18_0024) 
    SYMBOL_ITEM_FUNC(acGUI_FontSouvenir18_0025) 
    SYMBOL_ITEM_FUNC(acGUI_FontSouvenir18_0026) 
    SYMBOL_ITEM_FUNC(acGUI_FontSouvenir18_0027) 
    SYMBOL_ITEM_FUNC(acGUI_FontSouvenir18_0028) 
    SYMBOL_ITEM_FUNC(acGUI_FontSouvenir18_0029) 
    SYMBOL_ITEM_FUNC(acGUI_FontSouvenir18_002A) 
    SYMBOL_ITEM_FUNC(acGUI_FontSouvenir18_002B) 
    SYMBOL_ITEM_FUNC(acGUI_FontSouvenir18_002C) 
    SYMBOL_ITEM_FUNC(acGUI_FontSouvenir18_002D) 
    SYMBOL_ITEM_FUNC(acGUI_FontSouvenir18_002E) 
    SYMBOL_ITEM_FUNC(acGUI_FontSouvenir18_002F) 
    SYMBOL_ITEM_FUNC(acGUI_FontSouvenir18_0030) 
    SYMBOL_ITEM_FUNC(acGUI_FontSouvenir18_0031) 
    SYMBOL_ITEM_FUNC(acGUI_FontSouvenir18_0032) 
    SYMBOL_ITEM_FUNC(acGUI_FontSouvenir18_0033) 
    SYMBOL_ITEM_FUNC(acGUI_FontSouvenir18_0034) 
    SYMBOL_ITEM_FUNC(acGUI_FontSouvenir18_0035) 
    SYMBOL_ITEM_FUNC(acGUI_FontSouvenir18_0036) 
    SYMBOL_ITEM_FUNC(acGUI_FontSouvenir18_0037) 
    SYMBOL_ITEM_FUNC(acGUI_FontSouvenir18_0038) 
    SYMBOL_ITEM_FUNC(acGUI_FontSouvenir18_0039) 
    SYMBOL_ITEM_FUNC(acGUI_FontSouvenir18_003A) 
    SYMBOL_ITEM_FUNC(acGUI_FontSouvenir18_003B) 
    SYMBOL_ITEM_FUNC(acGUI_FontSouvenir18_003C) 
    SYMBOL_ITEM_FUNC(acGUI_FontSouvenir18_003D) 
    SYMBOL_ITEM_FUNC(acGUI_FontSouvenir18_003E) 
    SYMBOL_ITEM_FUNC(acGUI_FontSouvenir18_003F) 
    SYMBOL_ITEM_FUNC(acGUI_FontSouvenir18_0040) 
    SYMBOL_ITEM_FUNC(acGUI_FontSouvenir18_0041) 
    SYMBOL_ITEM_FUNC(acGUI_FontSouvenir18_0042) 
    SYMBOL_ITEM_FUNC(acGUI_FontSouvenir18_0043) 
    SYMBOL_ITEM_FUNC(acGUI_FontSouvenir18_0044) 
    SYMBOL_ITEM_FUNC(acGUI_FontSouvenir18_0045) 
    SYMBOL_ITEM_FUNC(acGUI_FontSouvenir18_0046) 
    SYMBOL_ITEM_FUNC(acGUI_FontSouvenir18_0047) 
    SYMBOL_ITEM_FUNC(acGUI_FontSouvenir18_0048) 
    SYMBOL_ITEM_FUNC(acGUI_FontSouvenir18_0049) 
    SYMBOL_ITEM_FUNC(acGUI_FontSouvenir18_004A) 
    SYMBOL_ITEM_FUNC(acGUI_FontSouvenir18_004B) 
    SYMBOL_ITEM_FUNC(acGUI_FontSouvenir18_004C) 
    SYMBOL_ITEM_FUNC(acGUI_FontSouvenir18_004D) 
    SYMBOL_ITEM_FUNC(acGUI_FontSouvenir18_004E) 
    SYMBOL_ITEM_FUNC(acGUI_FontSouvenir18_004F) 
    SYMBOL_ITEM_FUNC(acGUI_FontSouvenir18_0050) 
    SYMBOL_ITEM_FUNC(acGUI_FontSouvenir18_0051) 
    SYMBOL_ITEM_FUNC(acGUI_FontSouvenir18_0052) 
    SYMBOL_ITEM_FUNC(acGUI_FontSouvenir18_0053) 
    SYMBOL_ITEM_FUNC(acGUI_FontSouvenir18_0054) 
    SYMBOL_ITEM_FUNC(acGUI_FontSouvenir18_0055) 
    SYMBOL_ITEM_FUNC(acGUI_FontSouvenir18_0056) 
    SYMBOL_ITEM_FUNC(acGUI_FontSouvenir18_0057) 
    SYMBOL_ITEM_FUNC(acGUI_FontSouvenir18_0058) 
    SYMBOL_ITEM_FUNC(acGUI_FontSouvenir18_0059) 
    SYMBOL_ITEM_FUNC(acGUI_FontSouvenir18_005A) 
    SYMBOL_ITEM_FUNC(acGUI_FontSouvenir18_005B) 
    SYMBOL_ITEM_FUNC(acGUI_FontSouvenir18_005C) 
    SYMBOL_ITEM_FUNC(acGUI_FontSouvenir18_005D) 
    SYMBOL_ITEM_FUNC(acGUI_FontSouvenir18_005E) 
    SYMBOL_ITEM_FUNC(acGUI_FontSouvenir18_005F) 
    SYMBOL_ITEM_FUNC(acGUI_FontSouvenir18_0060) 
    SYMBOL_ITEM_FUNC(acGUI_FontSouvenir18_0061) 
    SYMBOL_ITEM_FUNC(acGUI_FontSouvenir18_0062) 
    SYMBOL_ITEM_FUNC(acGUI_FontSouvenir18_0063) 
    SYMBOL_ITEM_FUNC(acGUI_FontSouvenir18_0064) 
    SYMBOL_ITEM_FUNC(acGUI_FontSouvenir18_0065) 
    SYMBOL_ITEM_FUNC(acGUI_FontSouvenir18_0066) 
    SYMBOL_ITEM_FUNC(acGUI_FontSouvenir18_0067) 
    SYMBOL_ITEM_FUNC(acGUI_FontSouvenir18_0068) 
    SYMBOL_ITEM_FUNC(acGUI_FontSouvenir18_0069) 
    SYMBOL_ITEM_FUNC(acGUI_FontSouvenir18_006A) 
    SYMBOL_ITEM_FUNC(acGUI_FontSouvenir18_006B) 
    SYMBOL_ITEM_FUNC(acGUI_FontSouvenir18_006C) 
    SYMBOL_ITEM_FUNC(acGUI_FontSouvenir18_006D) 
    SYMBOL_ITEM_FUNC(acGUI_FontSouvenir18_006E) 
    SYMBOL_ITEM_FUNC(acGUI_FontSouvenir18_006F) 
    SYMBOL_ITEM_FUNC(acGUI_FontSouvenir18_0070) 
    SYMBOL_ITEM_FUNC(acGUI_FontSouvenir18_0071) 
    SYMBOL_ITEM_FUNC(acGUI_FontSouvenir18_0072) 
    SYMBOL_ITEM_FUNC(acGUI_FontSouvenir18_0073) 
    SYMBOL_ITEM_FUNC(acGUI_FontSouvenir18_0074) 
    SYMBOL_ITEM_FUNC(acGUI_FontSouvenir18_0075) 
    SYMBOL_ITEM_FUNC(acGUI_FontSouvenir18_0076) 
    SYMBOL_ITEM_FUNC(acGUI_FontSouvenir18_0077) 
    SYMBOL_ITEM_FUNC(acGUI_FontSouvenir18_0078) 
    SYMBOL_ITEM_FUNC(acGUI_FontSouvenir18_0079) 
    SYMBOL_ITEM_FUNC(acGUI_FontSouvenir18_007A) 
    SYMBOL_ITEM_FUNC(acGUI_FontSouvenir18_007B) 
    SYMBOL_ITEM_FUNC(acGUI_FontSouvenir18_007C) 
    SYMBOL_ITEM_FUNC(acGUI_FontSouvenir18_007D) 
    SYMBOL_ITEM_FUNC(acGUI_FontSouvenir18_007E) 
    SYMBOL_ITEM_FUNC(accept) 
    SYMBOL_ITEM_FUNC(access) 
    SYMBOL_ITEM_FUNC(add_mtd_device) 
    SYMBOL_ITEM_FUNC(alarm) 
    SYMBOL_ITEM_FUNC(alarmd_create) 
    SYMBOL_ITEM_FUNC(atomic_add) 
    SYMBOL_ITEM_FUNC(atomic_dec) 
    SYMBOL_ITEM_FUNC(atomic_dec_and_test) 
    SYMBOL_ITEM_FUNC(atomic_dec_return) 
    SYMBOL_ITEM_FUNC(atomic_inc) 
    SYMBOL_ITEM_FUNC(atomic_inc_and_test) 
    SYMBOL_ITEM_FUNC(atomic_inc_return) 
    SYMBOL_ITEM_FUNC(atomic_read) 
    SYMBOL_ITEM_FUNC(atomic_set) 
    SYMBOL_ITEM_FUNC(atomic_sub) 
    SYMBOL_ITEM_FUNC(atomic_sub_and_test) 
    SYMBOL_ITEM_FUNC(atomic_sub_return) 
    SYMBOL_ITEM_FUNC(audio_init) 
    SYMBOL_ITEM_FUNC(bcopy) 
    SYMBOL_ITEM_FUNC(bind) 
    SYMBOL_ITEM_FUNC(bmSeggerLogo) 
    SYMBOL_ITEM_FUNC(bmSeggerLogo70x35) 
    SYMBOL_ITEM_FUNC(board_nand_init) 
    SYMBOL_ITEM_FUNC(bsp_devices_create) 
    SYMBOL_ITEM_FUNC(bsp_drivers_install) 
    SYMBOL_ITEM_FUNC(bsp_mem_map) 
    SYMBOL_ITEM_FUNC(bsp_resv_space) 
    SYMBOL_ITEM_FUNC(ceil) 
    SYMBOL_ITEM_FUNC(chdir) 
    SYMBOL_ITEM_FUNC(clock_init) 
    SYMBOL_ITEM_FUNC(close) 
    SYMBOL_ITEM_FUNC(closedir) 
    SYMBOL_ITEM_FUNC(clust2sect) 
    SYMBOL_ITEM_FUNC(connect) 
    SYMBOL_ITEM_FUNC(cpu_kernel_init) 
    SYMBOL_ITEM_FUNC(cpu_mem_map) 
    SYMBOL_ITEM_FUNC(cpu_reset_init) 
    SYMBOL_ITEM_FUNC(cpu_resv_space) 
    SYMBOL_ITEM_FUNC(creat) 
    SYMBOL_ITEM_FUNC(dabt_c_handler) 
    SYMBOL_ITEM_FUNC(del_mtd_device) 
    SYMBOL_ITEM_FUNC(device_create) 
    SYMBOL_ITEM_FUNC(device_get) 
    SYMBOL_ITEM_FUNC(device_lookup) 
    SYMBOL_ITEM_FUNC(device_manager_init) 
    SYMBOL_ITEM_FUNC(device_remove) 
    SYMBOL_ITEM_FUNC(disk_initialize) 
    SYMBOL_ITEM_FUNC(disk_ioctl) 
    SYMBOL_ITEM_FUNC(disk_read) 
    SYMBOL_ITEM_FUNC(disk_status) 
    SYMBOL_ITEM_FUNC(disk_write) 
    SYMBOL_ITEM_FUNC(do_bind) 
    SYMBOL_ITEM_FUNC(do_close) 
    SYMBOL_ITEM_FUNC(do_connect) 
    SYMBOL_ITEM_FUNC(do_delconn) 
    SYMBOL_ITEM_FUNC(do_disconnect) 
    SYMBOL_ITEM_FUNC(do_getaddr) 
    SYMBOL_ITEM_FUNC(do_listen) 
    SYMBOL_ITEM_FUNC(do_netifapi_netif_add) 
    SYMBOL_ITEM_FUNC(do_netifapi_netif_common) 
    SYMBOL_ITEM_FUNC(do_netifapi_netif_set_addr) 
    SYMBOL_ITEM_FUNC(do_newconn) 
    SYMBOL_ITEM_FUNC(do_recv) 
    SYMBOL_ITEM_FUNC(do_send) 
    SYMBOL_ITEM_FUNC(do_write) 
    SYMBOL_ITEM_FUNC(driver_install) 
    SYMBOL_ITEM_FUNC(driver_lookup) 
    SYMBOL_ITEM_FUNC(driver_manager_init) 
    SYMBOL_ITEM_FUNC(driver_ref_by_name) 
    SYMBOL_ITEM_FUNC(driver_remove) 
    SYMBOL_ITEM_FUNC(dup) 
    SYMBOL_ITEM_FUNC(dup2) 
    SYMBOL_ITEM_FUNC(etharp_add_static_entry) 
    SYMBOL_ITEM_FUNC(etharp_cleanup_netif) 
    SYMBOL_ITEM_FUNC(etharp_find_addr) 
    SYMBOL_ITEM_FUNC(etharp_output) 
    SYMBOL_ITEM_FUNC(etharp_query) 
    SYMBOL_ITEM_FUNC(etharp_remove_static_entry) 
    SYMBOL_ITEM_FUNC(etharp_request) 
    SYMBOL_ITEM_FUNC(etharp_tmr) 
    SYMBOL_ITEM_FUNC(ethbroadcast) 
    SYMBOL_ITEM_FUNC(ethernet_input) 
    SYMBOL_ITEM_FUNC(ethernetif_init) 
    SYMBOL_ITEM_FUNC(ethzero) 
    SYMBOL_ITEM_FUNC(f_chmod) 
    SYMBOL_ITEM_FUNC(f_close) 
    SYMBOL_ITEM_FUNC(f_getfree) 
    SYMBOL_ITEM_FUNC(f_lseek) 
    SYMBOL_ITEM_FUNC(f_mkdir) 
    SYMBOL_ITEM_FUNC(f_mkfs) 
    SYMBOL_ITEM_FUNC(f_mount) 
    SYMBOL_ITEM_FUNC(f_open) 
    SYMBOL_ITEM_FUNC(f_opendir) 
    SYMBOL_ITEM_FUNC(f_read) 
    SYMBOL_ITEM_FUNC(f_readdir) 
    SYMBOL_ITEM_FUNC(f_rename) 
    SYMBOL_ITEM_FUNC(f_stat) 
    SYMBOL_ITEM_FUNC(f_sync) 
    SYMBOL_ITEM_FUNC(f_truncate) 
    SYMBOL_ITEM_FUNC(f_unlink) 
    SYMBOL_ITEM_FUNC(f_utime) 
    SYMBOL_ITEM_FUNC(f_write) 
    SYMBOL_ITEM_FUNC(fatfs_file_sem_lock) 
    SYMBOL_ITEM_FUNC(fb_create) 
    SYMBOL_ITEM_FUNC(fb_init) 
    SYMBOL_ITEM_FUNC(fclose) 
    SYMBOL_ITEM_FUNC(fcntl) 
    SYMBOL_ITEM_FUNC(fdopen) 
    SYMBOL_ITEM_FUNC(ff_cre_syncobj) 
    SYMBOL_ITEM_FUNC(ff_del_syncobj) 
    SYMBOL_ITEM_FUNC(ff_memalloc) 
    SYMBOL_ITEM_FUNC(ff_memfree) 
    SYMBOL_ITEM_FUNC(ff_rel_grant) 
    SYMBOL_ITEM_FUNC(ff_req_grant) 
    SYMBOL_ITEM_FUNC(fflush) 
    SYMBOL_ITEM_FUNC(fifo_create) 
    SYMBOL_ITEM_FUNC(fifo_init) 
    SYMBOL_ITEM_FUNC(file_system_install) 
    SYMBOL_ITEM_FUNC(file_system_lookup) 
    SYMBOL_ITEM_FUNC(file_system_manager_init) 
    SYMBOL_ITEM_FUNC(file_system_remove) 
    SYMBOL_ITEM_FUNC(fiprintf) 
    SYMBOL_ITEM_FUNC(fiq_c_handler) 
    SYMBOL_ITEM_FUNC(floor) 
    SYMBOL_ITEM_FUNC(fprintf) 
    SYMBOL_ITEM_FUNC(fputwc) 
    SYMBOL_ITEM_FUNC(free) 
    SYMBOL_ITEM_FUNC(fstat) 
    SYMBOL_ITEM_FUNC(get_fat) 
    SYMBOL_ITEM_FUNC(get_fattime) 
    SYMBOL_ITEM_FUNC(get_mtd_device) 
    SYMBOL_ITEM_FUNC(get_mtd_device_nm) 
    SYMBOL_ITEM_FUNC(getcwd) 
    SYMBOL_ITEM_FUNC(getsockopt) 
    SYMBOL_ITEM_FUNC(getticks) 
    SYMBOL_ITEM_FUNC(gettid) 
    SYMBOL_ITEM_FUNC(heap_alloc) 
    SYMBOL_ITEM_FUNC(heap_free) 
    SYMBOL_ITEM_FUNC(heap_init) 
    SYMBOL_ITEM_FUNC(icmp_dest_unreach) 
    SYMBOL_ITEM_FUNC(icmp_input) 
    SYMBOL_ITEM_FUNC(icmp_time_exceeded) 
    SYMBOL_ITEM_FUNC(in_interrupt) 
    SYMBOL_ITEM_FUNC(in_kernel) 
    SYMBOL_ITEM_FUNC(index) 
    SYMBOL_ITEM_FUNC(inet_chksum) 
    SYMBOL_ITEM_FUNC(inet_chksum_pbuf) 
    SYMBOL_ITEM_FUNC(inet_chksum_pseudo) 
    SYMBOL_ITEM_FUNC(inet_chksum_pseudo_partial) 
    SYMBOL_ITEM_FUNC(interrupt_disable) 
    SYMBOL_ITEM_FUNC(interrupt_enter) 
    SYMBOL_ITEM_FUNC(interrupt_exit) 
    SYMBOL_ITEM_FUNC(interrupt_init) 
    SYMBOL_ITEM_FUNC(interrupt_install) 
    SYMBOL_ITEM_FUNC(interrupt_mask) 
    SYMBOL_ITEM_FUNC(interrupt_resume) 
    SYMBOL_ITEM_FUNC(interrupt_unmask) 
    SYMBOL_ITEM_FUNC(ioctl) 
    SYMBOL_ITEM_FUNC(ip4_addr_isbroadcast) 
    SYMBOL_ITEM_FUNC(ip4_addr_netmask_valid) 
    SYMBOL_ITEM_FUNC(ip_addr_any) 
    SYMBOL_ITEM_FUNC(ip_addr_broadcast) 
    SYMBOL_ITEM_FUNC(ip_frag) 
    SYMBOL_ITEM_FUNC(ip_input) 
    SYMBOL_ITEM_FUNC(ip_output) 
    SYMBOL_ITEM_FUNC(ip_output_hinted) 
    SYMBOL_ITEM_FUNC(ip_output_if) 
    SYMBOL_ITEM_FUNC(ip_reass) 
    SYMBOL_ITEM_FUNC(ip_reass_tmr) 
    SYMBOL_ITEM_FUNC(ip_route) 
    SYMBOL_ITEM_FUNC(ipaddr_addr) 
    SYMBOL_ITEM_FUNC(ipaddr_aton) 
    SYMBOL_ITEM_FUNC(ipaddr_ntoa) 
    SYMBOL_ITEM_FUNC(ipaddr_ntoa_r) 
    SYMBOL_ITEM_FUNC(ipc_task_cleanup) 
    SYMBOL_ITEM_FUNC(ipc_task_init) 
    SYMBOL_ITEM_FUNC(irq_c_handler) 
    SYMBOL_ITEM_FUNC(isatty) 
    SYMBOL_ITEM_FUNC(isr_invaild) 
    SYMBOL_ITEM_FUNC(iswspace) 
    SYMBOL_ITEM_FUNC(kcomplain) 
    SYMBOL_ITEM_FUNC(kernel_enter) 
    SYMBOL_ITEM_FUNC(kernel_init) 
    SYMBOL_ITEM_FUNC(kernel_start) 
    SYMBOL_ITEM_FUNC(kernel_timer) 
    SYMBOL_ITEM_FUNC(kheap_create) 
    SYMBOL_ITEM_FUNC(kidle_create) 
    SYMBOL_ITEM_FUNC(kill) 
    SYMBOL_ITEM_FUNC(klogd_create) 
    SYMBOL_ITEM_FUNC(kputc) 
    SYMBOL_ITEM_FUNC(kthread_create) 
    SYMBOL_ITEM_FUNC(lcd_init) 
    SYMBOL_ITEM_FUNC(listen) 
    SYMBOL_ITEM_FUNC(localeconv) 
    SYMBOL_ITEM_FUNC(lwip_accept) 
    SYMBOL_ITEM_FUNC(lwip_bind) 
    SYMBOL_ITEM_FUNC(lwip_chksum_copy) 
    SYMBOL_ITEM_FUNC(lwip_close) 
    SYMBOL_ITEM_FUNC(lwip_connect) 
    SYMBOL_ITEM_FUNC(lwip_fcntl) 
    SYMBOL_ITEM_FUNC(lwip_getpeername) 
    SYMBOL_ITEM_FUNC(lwip_getsockname) 
    SYMBOL_ITEM_FUNC(lwip_getsockopt) 
    SYMBOL_ITEM_FUNC(lwip_htonl) 
    SYMBOL_ITEM_FUNC(lwip_htons) 
    SYMBOL_ITEM_FUNC(lwip_init) 
    SYMBOL_ITEM_FUNC(lwip_ioctl) 
    SYMBOL_ITEM_FUNC(lwip_listen) 
    SYMBOL_ITEM_FUNC(lwip_ntohl) 
    SYMBOL_ITEM_FUNC(lwip_ntohs) 
    SYMBOL_ITEM_FUNC(lwip_read) 
    SYMBOL_ITEM_FUNC(lwip_recv) 
    SYMBOL_ITEM_FUNC(lwip_recvfrom) 
    SYMBOL_ITEM_FUNC(lwip_select) 
    SYMBOL_ITEM_FUNC(lwip_send) 
    SYMBOL_ITEM_FUNC(lwip_sendto) 
    SYMBOL_ITEM_FUNC(lwip_setsockopt) 
    SYMBOL_ITEM_FUNC(lwip_shutdown) 
    SYMBOL_ITEM_FUNC(lwip_socket) 
    SYMBOL_ITEM_FUNC(lwip_socket_init) 
    SYMBOL_ITEM_FUNC(lwip_socket_set_ctx) 
    SYMBOL_ITEM_FUNC(lwip_strerr) 
    SYMBOL_ITEM_FUNC(lwip_write) 
    SYMBOL_ITEM_FUNC(main) 
    SYMBOL_ITEM_FUNC(malloc) 
    SYMBOL_ITEM_FUNC(matherr) 
    SYMBOL_ITEM_FUNC(mbrtowc) 
    SYMBOL_ITEM_FUNC(mem_calloc) 
    SYMBOL_ITEM_FUNC(mem_free) 
    SYMBOL_ITEM_FUNC(mem_free_callback) 
    SYMBOL_ITEM_FUNC(mem_init) 
    SYMBOL_ITEM_FUNC(mem_malloc) 
    SYMBOL_ITEM_FUNC(mem_trim) 
    SYMBOL_ITEM_FUNC(memchr) 
    SYMBOL_ITEM_FUNC(memcmp) 
    SYMBOL_ITEM_FUNC(memcpy) 
    SYMBOL_ITEM_FUNC(memmove) 
    SYMBOL_ITEM_FUNC(memp_free) 
    SYMBOL_ITEM_FUNC(memp_init) 
    SYMBOL_ITEM_FUNC(memp_malloc) 
    SYMBOL_ITEM_FUNC(memrchr) 
    SYMBOL_ITEM_FUNC(memset) 
    SYMBOL_ITEM_FUNC(mkdir) 
    SYMBOL_ITEM_FUNC(mmu_clean_dcache_index) 
    SYMBOL_ITEM_FUNC(mmu_clean_invalidate_dcache_index) 
    SYMBOL_ITEM_FUNC(mmu_clean_invalidate_dcache_mva) 
    SYMBOL_ITEM_FUNC(mmu_disable) 
    SYMBOL_ITEM_FUNC(mmu_disable_align_fault_check) 
    SYMBOL_ITEM_FUNC(mmu_disable_dcache) 
    SYMBOL_ITEM_FUNC(mmu_disable_icache) 
    SYMBOL_ITEM_FUNC(mmu_drain_write_buffer) 
    SYMBOL_ITEM_FUNC(mmu_enable) 
    SYMBOL_ITEM_FUNC(mmu_enable_align_fault_check) 
    SYMBOL_ITEM_FUNC(mmu_enable_dcache) 
    SYMBOL_ITEM_FUNC(mmu_enable_icache) 
    SYMBOL_ITEM_FUNC(mmu_get_cache_type) 
    SYMBOL_ITEM_FUNC(mmu_get_cpu_id) 
    SYMBOL_ITEM_FUNC(mmu_get_data_fault_status) 
    SYMBOL_ITEM_FUNC(mmu_get_fault_address) 
    SYMBOL_ITEM_FUNC(mmu_get_prefetch_fault_status) 
    SYMBOL_ITEM_FUNC(mmu_init) 
    SYMBOL_ITEM_FUNC(mmu_invalidate_dcache) 
    SYMBOL_ITEM_FUNC(mmu_invalidate_dtlb) 
    SYMBOL_ITEM_FUNC(mmu_invalidate_dtlb_mva) 
    SYMBOL_ITEM_FUNC(mmu_invalidate_icache) 
    SYMBOL_ITEM_FUNC(mmu_invalidate_icache_dcache) 
    SYMBOL_ITEM_FUNC(mmu_invalidate_icache_mva) 
    SYMBOL_ITEM_FUNC(mmu_invalidate_itlb) 
    SYMBOL_ITEM_FUNC(mmu_invalidate_itlb_dtlb) 
    SYMBOL_ITEM_FUNC(mmu_invalidate_itlb_mva) 
    SYMBOL_ITEM_FUNC(mmu_map_page) 
    SYMBOL_ITEM_FUNC(mmu_map_region) 
    SYMBOL_ITEM_FUNC(mmu_map_section) 
    SYMBOL_ITEM_FUNC(mmu_map_section_as_page) 
    SYMBOL_ITEM_FUNC(mmu_set_domain) 
    SYMBOL_ITEM_FUNC(mmu_set_sys_rom_protect_bit) 
    SYMBOL_ITEM_FUNC(mmu_set_ttb) 
    SYMBOL_ITEM_FUNC(mmu_set_vector_addr) 
    SYMBOL_ITEM_FUNC(mmu_unmap_section) 
    SYMBOL_ITEM_FUNC(mmu_wait_for_interrupt) 
    SYMBOL_ITEM_FUNC(module_init) 
    SYMBOL_ITEM_FUNC(module_load) 
    SYMBOL_ITEM_FUNC(module_lookup) 
    SYMBOL_ITEM_FUNC(module_ref_by_addr) 
    SYMBOL_ITEM_FUNC(module_unload) 
    SYMBOL_ITEM_FUNC(module_unref) 
    SYMBOL_ITEM_FUNC(mount_point_get) 
    SYMBOL_ITEM_FUNC(mount_point_install) 
    SYMBOL_ITEM_FUNC(mount_point_lookup) 
    SYMBOL_ITEM_FUNC(mount_point_manager_init) 
    SYMBOL_ITEM_FUNC(mount_point_remove) 
    SYMBOL_ITEM_FUNC(mqueue_abort) 
    SYMBOL_ITEM_FUNC(mqueue_abort_fetch) 
    SYMBOL_ITEM_FUNC(mqueue_abort_post) 
    SYMBOL_ITEM_FUNC(mqueue_fetch) 
    SYMBOL_ITEM_FUNC(mqueue_flush) 
    SYMBOL_ITEM_FUNC(mqueue_free) 
    SYMBOL_ITEM_FUNC(mqueue_msg_nr) 
    SYMBOL_ITEM_FUNC(mqueue_new) 
    SYMBOL_ITEM_FUNC(mqueue_post) 
    SYMBOL_ITEM_FUNC(mqueue_set_valid) 
    SYMBOL_ITEM_FUNC(mqueue_tryfetch) 
    SYMBOL_ITEM_FUNC(mqueue_trypost) 
    SYMBOL_ITEM_FUNC(mqueue_valid) 
    SYMBOL_ITEM_FUNC(msleep) 
    SYMBOL_ITEM_FUNC(mtdblock_create) 
    SYMBOL_ITEM_FUNC(mtdblock_init) 
    SYMBOL_ITEM_FUNC(mutex_abort) 
    SYMBOL_ITEM_FUNC(mutex_free) 
    SYMBOL_ITEM_FUNC(mutex_lock) 
    SYMBOL_ITEM_FUNC(mutex_new) 
    SYMBOL_ITEM_FUNC(mutex_set_valid) 
    SYMBOL_ITEM_FUNC(mutex_trylock) 
    SYMBOL_ITEM_FUNC(mutex_unlock) 
    SYMBOL_ITEM_FUNC(mutex_valid) 
    SYMBOL_ITEM_FUNC(mva_to_va) 
    SYMBOL_ITEM_FUNC(nand_calculate_ecc) 
    SYMBOL_ITEM_FUNC(nand_correct_data) 
    SYMBOL_ITEM_FUNC(nand_default_bbt) 
    SYMBOL_ITEM_FUNC(nand_erase_nand) 
    SYMBOL_ITEM_FUNC(nand_init) 
    SYMBOL_ITEM_FUNC(nand_isbad_bbt) 
    SYMBOL_ITEM_FUNC(nand_release) 
    SYMBOL_ITEM_FUNC(nand_scan) 
    SYMBOL_ITEM_FUNC(nand_scan_bbt) 
    SYMBOL_ITEM_FUNC(nand_scan_ident) 
    SYMBOL_ITEM_FUNC(nand_scan_tail) 
    SYMBOL_ITEM_FUNC(nand_update_bbt) 
    SYMBOL_ITEM_FUNC(nand_wait_ready) 
    SYMBOL_ITEM_FUNC(nandmtd1_MarkNANDBlockBad) 
    SYMBOL_ITEM_FUNC(nandmtd1_QueryNANDBlock) 
    SYMBOL_ITEM_FUNC(nandmtd1_ReadChunkWithTagsFromNAND) 
    SYMBOL_ITEM_FUNC(nandmtd1_WriteChunkWithTagsToNAND) 
    SYMBOL_ITEM_FUNC(nandmtd2_MarkNANDBlockBad) 
    SYMBOL_ITEM_FUNC(nandmtd2_QueryNANDBlock) 
    SYMBOL_ITEM_FUNC(nandmtd2_ReadChunkWithTagsFromNAND) 
    SYMBOL_ITEM_FUNC(nandmtd2_WriteChunkWithTagsToNAND) 
    SYMBOL_ITEM_FUNC(nandmtd_EraseBlockInNAND) 
    SYMBOL_ITEM_FUNC(nandmtd_InitialiseNAND) 
    SYMBOL_ITEM_FUNC(nanf) 
    SYMBOL_ITEM_FUNC(netbuf_alloc) 
    SYMBOL_ITEM_FUNC(netbuf_chain) 
    SYMBOL_ITEM_FUNC(netbuf_data) 
    SYMBOL_ITEM_FUNC(netbuf_delete) 
    SYMBOL_ITEM_FUNC(netbuf_first) 
    SYMBOL_ITEM_FUNC(netbuf_free) 
    SYMBOL_ITEM_FUNC(netbuf_new) 
    SYMBOL_ITEM_FUNC(netbuf_next) 
    SYMBOL_ITEM_FUNC(netbuf_ref) 
    SYMBOL_ITEM_FUNC(netconn_accept) 
    SYMBOL_ITEM_FUNC(netconn_alloc) 
    SYMBOL_ITEM_FUNC(netconn_bind) 
    SYMBOL_ITEM_FUNC(netconn_close) 
    SYMBOL_ITEM_FUNC(netconn_connect) 
    SYMBOL_ITEM_FUNC(netconn_delete) 
    SYMBOL_ITEM_FUNC(netconn_disconnect) 
    SYMBOL_ITEM_FUNC(netconn_free) 
    SYMBOL_ITEM_FUNC(netconn_getaddr) 
    SYMBOL_ITEM_FUNC(netconn_listen_with_backlog) 
    SYMBOL_ITEM_FUNC(netconn_new_with_proto_and_callback) 
    SYMBOL_ITEM_FUNC(netconn_recv) 
    SYMBOL_ITEM_FUNC(netconn_recv_tcp_pbuf) 
    SYMBOL_ITEM_FUNC(netconn_recved) 
    SYMBOL_ITEM_FUNC(netconn_send) 
    SYMBOL_ITEM_FUNC(netconn_sendto) 
    SYMBOL_ITEM_FUNC(netconn_shutdown) 
    SYMBOL_ITEM_FUNC(netconn_write_partly) 
    SYMBOL_ITEM_FUNC(netif_add) 
    SYMBOL_ITEM_FUNC(netif_find) 
    SYMBOL_ITEM_FUNC(netif_init) 
    SYMBOL_ITEM_FUNC(netif_loop_output) 
    SYMBOL_ITEM_FUNC(netif_poll) 
    SYMBOL_ITEM_FUNC(netif_remove) 
    SYMBOL_ITEM_FUNC(netif_set_addr) 
    SYMBOL_ITEM_FUNC(netif_set_default) 
    SYMBOL_ITEM_FUNC(netif_set_down) 
    SYMBOL_ITEM_FUNC(netif_set_gw) 
    SYMBOL_ITEM_FUNC(netif_set_ipaddr) 
    SYMBOL_ITEM_FUNC(netif_set_link_callback) 
    SYMBOL_ITEM_FUNC(netif_set_link_down) 
    SYMBOL_ITEM_FUNC(netif_set_link_up) 
    SYMBOL_ITEM_FUNC(netif_set_netmask) 
    SYMBOL_ITEM_FUNC(netif_set_remove_callback) 
    SYMBOL_ITEM_FUNC(netif_set_status_callback) 
    SYMBOL_ITEM_FUNC(netif_set_up) 
    SYMBOL_ITEM_FUNC(netifapi_netif_add) 
    SYMBOL_ITEM_FUNC(netifapi_netif_common) 
    SYMBOL_ITEM_FUNC(netifapi_netif_set_addr) 
    SYMBOL_ITEM_FUNC(netjob_add) 
    SYMBOL_ITEM_FUNC(netjob_create) 
    SYMBOL_ITEM_FUNC(null_init) 
    SYMBOL_ITEM_FUNC(open) 
    SYMBOL_ITEM_FUNC(opendir) 
    SYMBOL_ITEM_FUNC(pabt_c_handler) 
    SYMBOL_ITEM_FUNC(pause) 
    SYMBOL_ITEM_FUNC(pbuf_alloc) 
    SYMBOL_ITEM_FUNC(pbuf_alloced_custom) 
    SYMBOL_ITEM_FUNC(pbuf_cat) 
    SYMBOL_ITEM_FUNC(pbuf_chain) 
    SYMBOL_ITEM_FUNC(pbuf_clen) 
    SYMBOL_ITEM_FUNC(pbuf_coalesce) 
    SYMBOL_ITEM_FUNC(pbuf_copy) 
    SYMBOL_ITEM_FUNC(pbuf_copy_partial) 
    SYMBOL_ITEM_FUNC(pbuf_dechain) 
    SYMBOL_ITEM_FUNC(pbuf_fill_chksum) 
    SYMBOL_ITEM_FUNC(pbuf_free) 
    SYMBOL_ITEM_FUNC(pbuf_free_callback) 
    SYMBOL_ITEM_FUNC(pbuf_get_at) 
    SYMBOL_ITEM_FUNC(pbuf_header) 
    SYMBOL_ITEM_FUNC(pbuf_memcmp) 
    SYMBOL_ITEM_FUNC(pbuf_memfind) 
    SYMBOL_ITEM_FUNC(pbuf_realloc) 
    SYMBOL_ITEM_FUNC(pbuf_ref) 
    SYMBOL_ITEM_FUNC(pbuf_strstr) 
    SYMBOL_ITEM_FUNC(pbuf_take) 
    SYMBOL_ITEM_FUNC(poll) 
    SYMBOL_ITEM_FUNC(printf) 
    SYMBOL_ITEM_FUNC(printk) 
    SYMBOL_ITEM_FUNC(process0_enter) 
    SYMBOL_ITEM_FUNC(process_create) 
    SYMBOL_ITEM_FUNC(put_fat) 
    SYMBOL_ITEM_FUNC(put_mtd_device) 
    SYMBOL_ITEM_FUNC(putenv) 
    SYMBOL_ITEM_FUNC(raise) 
    SYMBOL_ITEM_FUNC(ramdisk_create) 
    SYMBOL_ITEM_FUNC(ramdisk_init) 
    SYMBOL_ITEM_FUNC(rand) 
    SYMBOL_ITEM_FUNC(raw_bind) 
    SYMBOL_ITEM_FUNC(raw_connect) 
    SYMBOL_ITEM_FUNC(raw_input) 
    SYMBOL_ITEM_FUNC(raw_new) 
    SYMBOL_ITEM_FUNC(raw_recv) 
    SYMBOL_ITEM_FUNC(raw_remove) 
    SYMBOL_ITEM_FUNC(raw_send) 
    SYMBOL_ITEM_FUNC(raw_sendto) 
    SYMBOL_ITEM_FUNC(read) 
    SYMBOL_ITEM_FUNC(readdir) 
    SYMBOL_ITEM_FUNC(recv) 
    SYMBOL_ITEM_FUNC(recvfrom) 
    SYMBOL_ITEM_FUNC(restore_core_regs) 
    SYMBOL_ITEM_FUNC(rewinddir) 
    SYMBOL_ITEM_FUNC(rmdir) 
    SYMBOL_ITEM_FUNC(schedule) 
    SYMBOL_ITEM_FUNC(seekdir) 
    SYMBOL_ITEM_FUNC(select) 
    SYMBOL_ITEM_FUNC(select_helper) 
    SYMBOL_ITEM_FUNC(select_init) 
    SYMBOL_ITEM_FUNC(select_report) 
    SYMBOL_ITEM_FUNC(select_select) 
    SYMBOL_ITEM_FUNC(select_unselect) 
    SYMBOL_ITEM_FUNC(sem_abort) 
    SYMBOL_ITEM_FUNC(sem_free) 
    SYMBOL_ITEM_FUNC(sem_new) 
    SYMBOL_ITEM_FUNC(sem_set_valid) 
    SYMBOL_ITEM_FUNC(sem_signal) 
    SYMBOL_ITEM_FUNC(sem_sync) 
    SYMBOL_ITEM_FUNC(sem_trywait) 
    SYMBOL_ITEM_FUNC(sem_valid) 
    SYMBOL_ITEM_FUNC(sem_wait) 
    SYMBOL_ITEM_FUNC(send) 
    SYMBOL_ITEM_FUNC(sendto) 
    SYMBOL_ITEM_FUNC(serial0_init) 
    SYMBOL_ITEM_FUNC(setlocale) 
    SYMBOL_ITEM_FUNC(setpinfo) 
    SYMBOL_ITEM_FUNC(setsockopt) 
    SYMBOL_ITEM_FUNC(sharemem_create) 
    SYMBOL_ITEM_FUNC(sharemem_init) 
    SYMBOL_ITEM_FUNC(shutdown) 
    SYMBOL_ITEM_FUNC(signal) 
    SYMBOL_ITEM_FUNC(signal_action) 
    SYMBOL_ITEM_FUNC(signal_handler_exit) 
    SYMBOL_ITEM_FUNC(signal_pending) 
    SYMBOL_ITEM_FUNC(signal_procmask) 
    SYMBOL_ITEM_FUNC(signal_queue) 
    SYMBOL_ITEM_FUNC(signal_schedule) 
    SYMBOL_ITEM_FUNC(signal_signal) 
    SYMBOL_ITEM_FUNC(signal_suspend) 
    SYMBOL_ITEM_FUNC(signal_switch) 
    SYMBOL_ITEM_FUNC(signal_task_cleanup) 
    SYMBOL_ITEM_FUNC(signal_task_init) 
    SYMBOL_ITEM_FUNC(sigprocmask) 
    SYMBOL_ITEM_FUNC(sigsuspend) 
    SYMBOL_ITEM_FUNC(sleep) 
    SYMBOL_ITEM_FUNC(smileos_socket_report) 
    SYMBOL_ITEM_FUNC(snprintf) 
    SYMBOL_ITEM_FUNC(soc_devices_create) 
    SYMBOL_ITEM_FUNC(soc_drivers_install) 
    SYMBOL_ITEM_FUNC(socket) 
    SYMBOL_ITEM_FUNC(socket_attach) 
    SYMBOL_ITEM_FUNC(socket_init) 
    SYMBOL_ITEM_FUNC(socket_priv_fd) 
    SYMBOL_ITEM_FUNC(socket_stat) 
    SYMBOL_ITEM_FUNC(sprintf) 
    SYMBOL_ITEM_FUNC(sqrt) 
    SYMBOL_ITEM_FUNC(srand) 
    SYMBOL_ITEM_FUNC(sscanf) 
    SYMBOL_ITEM_FUNC(stat) 
    SYMBOL_ITEM_FUNC(stats_display) 
    SYMBOL_ITEM_FUNC(stats_display_mem) 
    SYMBOL_ITEM_FUNC(stats_display_memp) 
    SYMBOL_ITEM_FUNC(stats_display_proto) 
    SYMBOL_ITEM_FUNC(stats_display_sys) 
    SYMBOL_ITEM_FUNC(stats_init) 
    SYMBOL_ITEM_FUNC(strcat) 
    SYMBOL_ITEM_FUNC(strchr) 
    SYMBOL_ITEM_FUNC(strchrnul) 
    SYMBOL_ITEM_FUNC(strcmp) 
    SYMBOL_ITEM_FUNC(strcpy) 
    SYMBOL_ITEM_FUNC(strdup) 
    SYMBOL_ITEM_FUNC(strlcpy) 
    SYMBOL_ITEM_FUNC(strlen) 
    SYMBOL_ITEM_FUNC(strncmp) 
    SYMBOL_ITEM_FUNC(strncpy) 
    SYMBOL_ITEM_FUNC(strndup) 
    SYMBOL_ITEM_FUNC(strnlen) 
    SYMBOL_ITEM_FUNC(strtod) 
    SYMBOL_ITEM_FUNC(strtof) 
    SYMBOL_ITEM_FUNC(strtol) 
    SYMBOL_ITEM_FUNC(strtoul) 
    SYMBOL_ITEM_FUNC(symbol_init) 
    SYMBOL_ITEM_FUNC(symbol_lookup) 
    SYMBOL_ITEM_FUNC(sys_arch_mbox_fetch) 
    SYMBOL_ITEM_FUNC(sys_arch_mbox_tryfetch) 
    SYMBOL_ITEM_FUNC(sys_arch_sem_wait) 
    SYMBOL_ITEM_FUNC(sys_init) 
    SYMBOL_ITEM_FUNC(sys_jiffies) 
    SYMBOL_ITEM_FUNC(sys_mbox_free) 
    SYMBOL_ITEM_FUNC(sys_mbox_new) 
    SYMBOL_ITEM_FUNC(sys_mbox_post) 
    SYMBOL_ITEM_FUNC(sys_mbox_set_invalid) 
    SYMBOL_ITEM_FUNC(sys_mbox_trypost) 
    SYMBOL_ITEM_FUNC(sys_mbox_valid) 
    SYMBOL_ITEM_FUNC(sys_msleep) 
    SYMBOL_ITEM_FUNC(sys_mutex_free) 
    SYMBOL_ITEM_FUNC(sys_mutex_lock) 
    SYMBOL_ITEM_FUNC(sys_mutex_new) 
    SYMBOL_ITEM_FUNC(sys_mutex_set_invalid) 
    SYMBOL_ITEM_FUNC(sys_mutex_unlock) 
    SYMBOL_ITEM_FUNC(sys_mutex_valid) 
    SYMBOL_ITEM_FUNC(sys_now) 
    SYMBOL_ITEM_FUNC(sys_resv_space) 
    SYMBOL_ITEM_FUNC(sys_sem_free) 
    SYMBOL_ITEM_FUNC(sys_sem_new) 
    SYMBOL_ITEM_FUNC(sys_sem_set_invalid) 
    SYMBOL_ITEM_FUNC(sys_sem_signal) 
    SYMBOL_ITEM_FUNC(sys_sem_valid) 
    SYMBOL_ITEM_FUNC(sys_thread_new) 
    SYMBOL_ITEM_FUNC(sys_timeout) 
    SYMBOL_ITEM_FUNC(sys_timeouts_init) 
    SYMBOL_ITEM_FUNC(sys_timeouts_mbox_fetch) 
    SYMBOL_ITEM_FUNC(sys_untimeout) 
    SYMBOL_ITEM_FUNC(task_alarm) 
    SYMBOL_ITEM_FUNC(task_cleanup) 
    SYMBOL_ITEM_FUNC(task_getpid) 
    SYMBOL_ITEM_FUNC(task_pause) 
    SYMBOL_ITEM_FUNC(task_schedule) 
    SYMBOL_ITEM_FUNC(task_setpinfo) 
    SYMBOL_ITEM_FUNC(task_sleep) 
    SYMBOL_ITEM_FUNC(task_switch) 
    SYMBOL_ITEM_FUNC(task_switch_hook) 
    SYMBOL_ITEM_FUNC(task_switch_to) 
    SYMBOL_ITEM_FUNC(tcp_abandon) 
    SYMBOL_ITEM_FUNC(tcp_abort) 
    SYMBOL_ITEM_FUNC(tcp_accept) 
    SYMBOL_ITEM_FUNC(tcp_alloc) 
    SYMBOL_ITEM_FUNC(tcp_arg) 
    SYMBOL_ITEM_FUNC(tcp_backoff) 
    SYMBOL_ITEM_FUNC(tcp_bind) 
    SYMBOL_ITEM_FUNC(tcp_close) 
    SYMBOL_ITEM_FUNC(tcp_connect) 
    SYMBOL_ITEM_FUNC(tcp_debug_state_str) 
    SYMBOL_ITEM_FUNC(tcp_eff_send_mss) 
    SYMBOL_ITEM_FUNC(tcp_enqueue_flags) 
    SYMBOL_ITEM_FUNC(tcp_err) 
    SYMBOL_ITEM_FUNC(tcp_fasttmr) 
    SYMBOL_ITEM_FUNC(tcp_init) 
    SYMBOL_ITEM_FUNC(tcp_input) 
    SYMBOL_ITEM_FUNC(tcp_keepalive) 
    SYMBOL_ITEM_FUNC(tcp_listen_with_backlog) 
    SYMBOL_ITEM_FUNC(tcp_new) 
    SYMBOL_ITEM_FUNC(tcp_next_iss) 
    SYMBOL_ITEM_FUNC(tcp_output) 
    SYMBOL_ITEM_FUNC(tcp_pcb_lists) 
    SYMBOL_ITEM_FUNC(tcp_pcb_purge) 
    SYMBOL_ITEM_FUNC(tcp_pcb_remove) 
    SYMBOL_ITEM_FUNC(tcp_persist_backoff) 
    SYMBOL_ITEM_FUNC(tcp_poll) 
    SYMBOL_ITEM_FUNC(tcp_process_refused_data) 
    SYMBOL_ITEM_FUNC(tcp_recv) 
    SYMBOL_ITEM_FUNC(tcp_recv_null) 
    SYMBOL_ITEM_FUNC(tcp_recved) 
    SYMBOL_ITEM_FUNC(tcp_rexmit) 
    SYMBOL_ITEM_FUNC(tcp_rexmit_fast) 
    SYMBOL_ITEM_FUNC(tcp_rexmit_rto) 
    SYMBOL_ITEM_FUNC(tcp_rst) 
    SYMBOL_ITEM_FUNC(tcp_seg_copy) 
    SYMBOL_ITEM_FUNC(tcp_seg_free) 
    SYMBOL_ITEM_FUNC(tcp_segs_free) 
    SYMBOL_ITEM_FUNC(tcp_send_empty_ack) 
    SYMBOL_ITEM_FUNC(tcp_send_fin) 
    SYMBOL_ITEM_FUNC(tcp_sent) 
    SYMBOL_ITEM_FUNC(tcp_setprio) 
    SYMBOL_ITEM_FUNC(tcp_shutdown) 
    SYMBOL_ITEM_FUNC(tcp_slowtmr) 
    SYMBOL_ITEM_FUNC(tcp_state_str) 
    SYMBOL_ITEM_FUNC(tcp_timer_needed) 
    SYMBOL_ITEM_FUNC(tcp_tmr) 
    SYMBOL_ITEM_FUNC(tcp_update_rcv_ann_wnd) 
    SYMBOL_ITEM_FUNC(tcp_write) 
    SYMBOL_ITEM_FUNC(tcp_zero_window_probe) 
    SYMBOL_ITEM_FUNC(tcpip_apimsg) 
    SYMBOL_ITEM_FUNC(tcpip_apimsg_lock) 
    SYMBOL_ITEM_FUNC(tcpip_callback_with_block) 
    SYMBOL_ITEM_FUNC(tcpip_callbackmsg_delete) 
    SYMBOL_ITEM_FUNC(tcpip_callbackmsg_new) 
    SYMBOL_ITEM_FUNC(tcpip_init) 
    SYMBOL_ITEM_FUNC(tcpip_input) 
    SYMBOL_ITEM_FUNC(tcpip_netifapi_lock) 
    SYMBOL_ITEM_FUNC(tcpip_timeout) 
    SYMBOL_ITEM_FUNC(tcpip_trycallback) 
    SYMBOL_ITEM_FUNC(tcpip_untimeout) 
    SYMBOL_ITEM_FUNC(telldir) 
    SYMBOL_ITEM_FUNC(timer_init) 
    SYMBOL_ITEM_FUNC(tzset) 
    SYMBOL_ITEM_FUNC(uart_init) 
    SYMBOL_ITEM_FUNC(udp_bind) 
    SYMBOL_ITEM_FUNC(udp_connect) 
    SYMBOL_ITEM_FUNC(udp_disconnect) 
    SYMBOL_ITEM_FUNC(udp_init) 
    SYMBOL_ITEM_FUNC(udp_input) 
    SYMBOL_ITEM_FUNC(udp_new) 
    SYMBOL_ITEM_FUNC(udp_recv) 
    SYMBOL_ITEM_FUNC(udp_remove) 
    SYMBOL_ITEM_FUNC(udp_send) 
    SYMBOL_ITEM_FUNC(udp_send_chksum) 
    SYMBOL_ITEM_FUNC(udp_sendto) 
    SYMBOL_ITEM_FUNC(udp_sendto_chksum) 
    SYMBOL_ITEM_FUNC(udp_sendto_if) 
    SYMBOL_ITEM_FUNC(udp_sendto_if_chksum) 
    SYMBOL_ITEM_FUNC(undf_c_handler) 
    SYMBOL_ITEM_FUNC(ungetc) 
    SYMBOL_ITEM_FUNC(usleep) 
    SYMBOL_ITEM_FUNC(va_to_mva) 
    SYMBOL_ITEM_FUNC(vfiprintf) 
    SYMBOL_ITEM_FUNC(vfprintf) 
    SYMBOL_ITEM_FUNC(vfs_access) 
    SYMBOL_ITEM_FUNC(vfs_chdir) 
    SYMBOL_ITEM_FUNC(vfs_close) 
    SYMBOL_ITEM_FUNC(vfs_closedir) 
    SYMBOL_ITEM_FUNC(vfs_dup) 
    SYMBOL_ITEM_FUNC(vfs_dup2) 
    SYMBOL_ITEM_FUNC(vfs_fcntl) 
    SYMBOL_ITEM_FUNC(vfs_fdatasync) 
    SYMBOL_ITEM_FUNC(vfs_fstat) 
    SYMBOL_ITEM_FUNC(vfs_fsync) 
    SYMBOL_ITEM_FUNC(vfs_ftruncate) 
    SYMBOL_ITEM_FUNC(vfs_get_file) 
    SYMBOL_ITEM_FUNC(vfs_getcwd) 
    SYMBOL_ITEM_FUNC(vfs_init) 
    SYMBOL_ITEM_FUNC(vfs_ioctl) 
    SYMBOL_ITEM_FUNC(vfs_isatty) 
    SYMBOL_ITEM_FUNC(vfs_link) 
    SYMBOL_ITEM_FUNC(vfs_lseek) 
    SYMBOL_ITEM_FUNC(vfs_mkdir) 
    SYMBOL_ITEM_FUNC(vfs_mkfs) 
    SYMBOL_ITEM_FUNC(vfs_mount) 
    SYMBOL_ITEM_FUNC(vfs_mount_point_create) 
    SYMBOL_ITEM_FUNC(vfs_open) 
    SYMBOL_ITEM_FUNC(vfs_opendir) 
    SYMBOL_ITEM_FUNC(vfs_path_add_mount_point) 
    SYMBOL_ITEM_FUNC(vfs_path_normalization) 
    SYMBOL_ITEM_FUNC(vfs_read) 
    SYMBOL_ITEM_FUNC(vfs_readdir) 
    SYMBOL_ITEM_FUNC(vfs_rename) 
    SYMBOL_ITEM_FUNC(vfs_rewinddir) 
    SYMBOL_ITEM_FUNC(vfs_rmdir) 
    SYMBOL_ITEM_FUNC(vfs_seekdir) 
    SYMBOL_ITEM_FUNC(vfs_select) 
    SYMBOL_ITEM_FUNC(vfs_stat) 
    SYMBOL_ITEM_FUNC(vfs_sync) 
    SYMBOL_ITEM_FUNC(vfs_task_cleanup) 
    SYMBOL_ITEM_FUNC(vfs_task_init) 
    SYMBOL_ITEM_FUNC(vfs_telldir) 
    SYMBOL_ITEM_FUNC(vfs_truncate) 
    SYMBOL_ITEM_FUNC(vfs_unlink) 
    SYMBOL_ITEM_FUNC(vfs_unmount) 
    SYMBOL_ITEM_FUNC(vfs_write) 
    SYMBOL_ITEM_FUNC(vmm_init) 
    SYMBOL_ITEM_FUNC(vmm_page_map) 
    SYMBOL_ITEM_FUNC(vmm_process_cleanup) 
    SYMBOL_ITEM_FUNC(vmm_process_init) 
    SYMBOL_ITEM_FUNC(vsnprintf) 
    SYMBOL_ITEM_FUNC(vspace_usable) 
    SYMBOL_ITEM_FUNC(warn) 
    SYMBOL_ITEM_FUNC(wcrtomb) 
    SYMBOL_ITEM_FUNC(xmalloc) 
    SYMBOL_ITEM_FUNC(xstrdup) 
    SYMBOL_ITEM_FUNC(xstrndup) 
    SYMBOL_ITEM_FUNC(xxx_init) 
    SYMBOL_ITEM_FUNC(xzalloc) 
    SYMBOL_ITEM_FUNC(yaffs_CalcECC) 
    SYMBOL_ITEM_FUNC(yaffs_CalcTagsECC) 
    SYMBOL_ITEM_FUNC(yaffs_CheckECCOnTags) 
    SYMBOL_ITEM_FUNC(yaffs_CheckFF) 
    SYMBOL_ITEM_FUNC(yaffs_CheckpointClose) 
    SYMBOL_ITEM_FUNC(yaffs_CheckpointInvalidateStream) 
    SYMBOL_ITEM_FUNC(yaffs_CheckpointOpen) 
    SYMBOL_ITEM_FUNC(yaffs_CheckpointRead) 
    SYMBOL_ITEM_FUNC(yaffs_CheckpointRestore) 
    SYMBOL_ITEM_FUNC(yaffs_CheckpointSave) 
    SYMBOL_ITEM_FUNC(yaffs_CheckpointWrite) 
    SYMBOL_ITEM_FUNC(yaffs_CountBits) 
    SYMBOL_ITEM_FUNC(yaffs_Deinitialise) 
    SYMBOL_ITEM_FUNC(yaffs_DeleteChunk) 
    SYMBOL_ITEM_FUNC(yaffs_DeleteFile) 
    SYMBOL_ITEM_FUNC(yaffs_DeleteObject) 
    SYMBOL_ITEM_FUNC(yaffs_DoWriteDataToFile) 
    SYMBOL_ITEM_FUNC(yaffs_Dofsync) 
    SYMBOL_ITEM_FUNC(yaffs_DumpDevStruct) 
    SYMBOL_ITEM_FUNC(yaffs_ECCCalculate) 
    SYMBOL_ITEM_FUNC(yaffs_ECCCalculateOther) 
    SYMBOL_ITEM_FUNC(yaffs_ECCCorrect) 
    SYMBOL_ITEM_FUNC(yaffs_ECCCorrectOther) 
    SYMBOL_ITEM_FUNC(yaffs_EraseBlockInNAND) 
    SYMBOL_ITEM_FUNC(yaffs_FindObjectByName) 
    SYMBOL_ITEM_FUNC(yaffs_FindObjectByNumber) 
    SYMBOL_ITEM_FUNC(yaffs_FlushEntireDeviceCache) 
    SYMBOL_ITEM_FUNC(yaffs_FlushFile) 
    SYMBOL_ITEM_FUNC(yaffs_GetAttributes) 
    SYMBOL_ITEM_FUNC(yaffs_GetCheckpointSum) 
    SYMBOL_ITEM_FUNC(yaffs_GetEquivalentObject) 
    SYMBOL_ITEM_FUNC(yaffs_GetFileSize) 
    SYMBOL_ITEM_FUNC(yaffs_GetNumberOfFreeChunks) 
    SYMBOL_ITEM_FUNC(yaffs_GetObjectFileLength) 
    SYMBOL_ITEM_FUNC(yaffs_GetObjectInode) 
    SYMBOL_ITEM_FUNC(yaffs_GetObjectLinkCount) 
    SYMBOL_ITEM_FUNC(yaffs_GetObjectName) 
    SYMBOL_ITEM_FUNC(yaffs_GetObjectType) 
    SYMBOL_ITEM_FUNC(yaffs_GetSymlinkAlias) 
    SYMBOL_ITEM_FUNC(yaffs_GetTempBuffer) 
    SYMBOL_ITEM_FUNC(yaffs_GutsInitialise) 
    SYMBOL_ITEM_FUNC(yaffs_HandleChunkError) 
    SYMBOL_ITEM_FUNC(yaffs_HandleDeferedFree) 
    SYMBOL_ITEM_FUNC(yaffs_InitialiseNAND) 
    SYMBOL_ITEM_FUNC(yaffs_InitialiseTags) 
    SYMBOL_ITEM_FUNC(yaffs_IsManagedTempBuffer) 
    SYMBOL_ITEM_FUNC(yaffs_Link) 
    SYMBOL_ITEM_FUNC(yaffs_LoadLevel0Tnode) 
    SYMBOL_ITEM_FUNC(yaffs_LostNFound) 
    SYMBOL_ITEM_FUNC(yaffs_MarkBlockBad) 
    SYMBOL_ITEM_FUNC(yaffs_MknodDirectory) 
    SYMBOL_ITEM_FUNC(yaffs_MknodFile) 
    SYMBOL_ITEM_FUNC(yaffs_MknodSpecial) 
    SYMBOL_ITEM_FUNC(yaffs_MknodSymLink) 
    SYMBOL_ITEM_FUNC(yaffs_PackTags1) 
    SYMBOL_ITEM_FUNC(yaffs_PackTags2) 
    SYMBOL_ITEM_FUNC(yaffs_PackTags2TagsPart) 
    SYMBOL_ITEM_FUNC(yaffs_QueryInitialBlockState) 
    SYMBOL_ITEM_FUNC(yaffs_ReadChunkWithTagsFromNAND) 
    SYMBOL_ITEM_FUNC(yaffs_ReadDataFromFile) 
    SYMBOL_ITEM_FUNC(yaffs_ReleaseTempBuffer) 
    SYMBOL_ITEM_FUNC(yaffs_RenameObject) 
    SYMBOL_ITEM_FUNC(yaffs_ResizeFile) 
    SYMBOL_ITEM_FUNC(yaffs_Root) 
    SYMBOL_ITEM_FUNC(yaffs_SetAttributes) 
    SYMBOL_ITEM_FUNC(yaffs_TagsCompatabilityMarkNANDBlockBad) 
    SYMBOL_ITEM_FUNC(yaffs_TagsCompatabilityQueryNANDBlock) 
    SYMBOL_ITEM_FUNC(yaffs_TagsCompatabilityReadChunkWithTagsFromNAND) 
    SYMBOL_ITEM_FUNC(yaffs_TagsCompatabilityWriteChunkWithTagsToNAND) 
    SYMBOL_ITEM_FUNC(yaffs_Unlink) 
    SYMBOL_ITEM_FUNC(yaffs_UnpackTags1) 
    SYMBOL_ITEM_FUNC(yaffs_UnpackTags2) 
    SYMBOL_ITEM_FUNC(yaffs_UnpackTags2TagsPart) 
    SYMBOL_ITEM_FUNC(yaffs_UpdateDirtyDirectories) 
    SYMBOL_ITEM_FUNC(yaffs_ValidateTags) 
    SYMBOL_ITEM_FUNC(yaffs_WriteChunkWithTagsToNAND) 
    SYMBOL_ITEM_FUNC(yaffs_WriteDataToFile) 
    SYMBOL_ITEM_FUNC(yaffs_access) 
    SYMBOL_ITEM_FUNC(yaffs_chmod) 
    SYMBOL_ITEM_FUNC(yaffs_close) 
    SYMBOL_ITEM_FUNC(yaffs_closedir) 
    SYMBOL_ITEM_FUNC(yaffs_dup) 
    SYMBOL_ITEM_FUNC(yaffs_fchmod) 
    SYMBOL_ITEM_FUNC(yaffs_fdatasync) 
    SYMBOL_ITEM_FUNC(yaffs_flush) 
    SYMBOL_ITEM_FUNC(yaffs_free) 
    SYMBOL_ITEM_FUNC(yaffs_freespace) 
    SYMBOL_ITEM_FUNC(yaffs_fstat) 
    SYMBOL_ITEM_FUNC(yaffs_fsync) 
    SYMBOL_ITEM_FUNC(yaffs_ftruncate) 
    SYMBOL_ITEM_FUNC(yaffs_initialise) 
    SYMBOL_ITEM_FUNC(yaffs_inodecount) 
    SYMBOL_ITEM_FUNC(yaffs_link) 
    SYMBOL_ITEM_FUNC(yaffs_lseek) 
    SYMBOL_ITEM_FUNC(yaffs_lstat) 
    SYMBOL_ITEM_FUNC(yaffs_malloc) 
    SYMBOL_ITEM_FUNC(yaffs_mkdir) 
    SYMBOL_ITEM_FUNC(yaffs_mknod) 
    SYMBOL_ITEM_FUNC(yaffs_mount) 
    SYMBOL_ITEM_FUNC(yaffs_mount2) 
    SYMBOL_ITEM_FUNC(yaffs_open) 
    SYMBOL_ITEM_FUNC(yaffs_opendir) 
    SYMBOL_ITEM_FUNC(yaffs_pread) 
    SYMBOL_ITEM_FUNC(yaffs_pwrite) 
    SYMBOL_ITEM_FUNC(yaffs_qsort) 
    SYMBOL_ITEM_FUNC(yaffs_read) 
    SYMBOL_ITEM_FUNC(yaffs_readdir) 
    SYMBOL_ITEM_FUNC(yaffs_readlink) 
    SYMBOL_ITEM_FUNC(yaffs_remount) 
    SYMBOL_ITEM_FUNC(yaffs_rename) 
    SYMBOL_ITEM_FUNC(yaffs_rewinddir) 
    SYMBOL_ITEM_FUNC(yaffs_rmdir) 
    SYMBOL_ITEM_FUNC(yaffs_stat) 
    SYMBOL_ITEM_FUNC(yaffs_symlink) 
    SYMBOL_ITEM_FUNC(yaffs_sync) 
    SYMBOL_ITEM_FUNC(yaffs_totalspace) 
    SYMBOL_ITEM_FUNC(yaffs_truncate) 
    SYMBOL_ITEM_FUNC(yaffs_unlink) 
    SYMBOL_ITEM_FUNC(yaffs_unmount) 
    SYMBOL_ITEM_FUNC(yaffs_unmount2) 
    SYMBOL_ITEM_FUNC(yaffs_write) 
    SYMBOL_ITEM_FUNC(yaffsfs_CurrentTime) 
    SYMBOL_ITEM_FUNC(yaffsfs_DoUnlink) 
    SYMBOL_ITEM_FUNC(yaffsfs_FindDevice) 
    SYMBOL_ITEM_FUNC(yaffsfs_GetHandleObject) 
    SYMBOL_ITEM_FUNC(yaffsfs_GetHandlePointer) 
    SYMBOL_ITEM_FUNC(yaffsfs_GetInodePointer) 
    SYMBOL_ITEM_FUNC(yaffsfs_Init) 
    SYMBOL_ITEM_FUNC(yaffsfs_InitHandles) 
    SYMBOL_ITEM_FUNC(yaffsfs_IsPathDivider) 
    SYMBOL_ITEM_FUNC(yaffsfs_Lock) 
    SYMBOL_ITEM_FUNC(yaffsfs_Match) 
    SYMBOL_ITEM_FUNC(yaffsfs_Mount) 
    SYMBOL_ITEM_FUNC(yaffsfs_RemoveObjectCallback) 
    SYMBOL_ITEM_FUNC(yaffsfs_SetError) 
    SYMBOL_ITEM_FUNC(yaffsfs_Unlock) 
    SYMBOL_ITEM_FUNC(yaffsfs_Unmount) 
    SYMBOL_ITEM_FUNC(yaffsfs_do_read) 
    SYMBOL_ITEM_FUNC(yaffsfs_do_write) 
    SYMBOL_ITEM_FUNC(zero_init) 
    SYMBOL_ITEM_FUNC(__gnu_thumb1_case_sqi) 
    SYMBOL_ITEM_FUNC(__gnu_thumb1_case_uqi) 
    SYMBOL_ITEM_FUNC(__gnu_thumb1_case_shi) 
    SYMBOL_ITEM_FUNC(__gnu_thumb1_case_uhi) 
    SYMBOL_ITEM_FUNC(__gnu_thumb1_case_si) 
    SYMBOL_ITEM_FUNC(__aeabi_uidiv) 
    SYMBOL_ITEM_FUNC(__aeabi_uidivmod) 
    SYMBOL_ITEM_FUNC(__udivsi3) 
    SYMBOL_ITEM_FUNC(__aeabi_idiv) 
    SYMBOL_ITEM_FUNC(__aeabi_idivmod) 
    SYMBOL_ITEM_FUNC(__divsi3) 
    SYMBOL_ITEM_FUNC(__umodsi3) 
    SYMBOL_ITEM_FUNC(__modsi3) 
    SYMBOL_ITEM_FUNC(_call_via_fp) 
    SYMBOL_ITEM_FUNC(_call_via_ip) 
    SYMBOL_ITEM_FUNC(_call_via_lr) 
    SYMBOL_ITEM_FUNC(_call_via_r0) 
    SYMBOL_ITEM_FUNC(_call_via_r1) 
    SYMBOL_ITEM_FUNC(_call_via_r2) 
    SYMBOL_ITEM_FUNC(_call_via_r3) 
    SYMBOL_ITEM_FUNC(_call_via_r4) 
    SYMBOL_ITEM_FUNC(_call_via_r5) 
    SYMBOL_ITEM_FUNC(_call_via_r6) 
    SYMBOL_ITEM_FUNC(_call_via_r7) 
    SYMBOL_ITEM_FUNC(_call_via_r8) 
    SYMBOL_ITEM_FUNC(_call_via_r9) 
    SYMBOL_ITEM_FUNC(_call_via_sl) 
    SYMBOL_ITEM_FUNC(_call_via_sp) 
    SYMBOL_ITEM_FUNC(_arm_return) 
    SYMBOL_ITEM_FUNC(_arm_return_r11) 
    SYMBOL_ITEM_FUNC(_arm_return_r7) 
    SYMBOL_ITEM_FUNC(_interwork_call_via_fp) 
    SYMBOL_ITEM_FUNC(_interwork_call_via_ip) 
    SYMBOL_ITEM_FUNC(_interwork_call_via_lr) 
    SYMBOL_ITEM_FUNC(_interwork_call_via_r0) 
    SYMBOL_ITEM_FUNC(_interwork_call_via_r1) 
    SYMBOL_ITEM_FUNC(_interwork_call_via_r2) 
    SYMBOL_ITEM_FUNC(_interwork_call_via_r3) 
    SYMBOL_ITEM_FUNC(_interwork_call_via_r4) 
    SYMBOL_ITEM_FUNC(_interwork_call_via_r5) 
    SYMBOL_ITEM_FUNC(_interwork_call_via_r6) 
    SYMBOL_ITEM_FUNC(_interwork_call_via_r7) 
    SYMBOL_ITEM_FUNC(_interwork_call_via_r8) 
    SYMBOL_ITEM_FUNC(_interwork_call_via_r9) 
    SYMBOL_ITEM_FUNC(_interwork_call_via_sl) 
    SYMBOL_ITEM_FUNC(_interwork_call_via_sp) 
    SYMBOL_ITEM_FUNC(_interwork_r11_call_via_fp) 
    SYMBOL_ITEM_FUNC(_interwork_r11_call_via_ip) 
    SYMBOL_ITEM_FUNC(_interwork_r11_call_via_r0) 
    SYMBOL_ITEM_FUNC(_interwork_r11_call_via_r1) 
    SYMBOL_ITEM_FUNC(_interwork_r11_call_via_r2) 
    SYMBOL_ITEM_FUNC(_interwork_r11_call_via_r3) 
    SYMBOL_ITEM_FUNC(_interwork_r11_call_via_r4) 
    SYMBOL_ITEM_FUNC(_interwork_r11_call_via_r5) 
    SYMBOL_ITEM_FUNC(_interwork_r11_call_via_r6) 
    SYMBOL_ITEM_FUNC(_interwork_r11_call_via_r7) 
    SYMBOL_ITEM_FUNC(_interwork_r11_call_via_r8) 
    SYMBOL_ITEM_FUNC(_interwork_r11_call_via_r9) 
    SYMBOL_ITEM_FUNC(_interwork_r11_call_via_sl) 
    SYMBOL_ITEM_FUNC(_interwork_r11_call_via_sp) 
    SYMBOL_ITEM_FUNC(_interwork_r7_call_via_fp) 
    SYMBOL_ITEM_FUNC(_interwork_r7_call_via_ip) 
    SYMBOL_ITEM_FUNC(_interwork_r7_call_via_r0) 
    SYMBOL_ITEM_FUNC(_interwork_r7_call_via_r1) 
    SYMBOL_ITEM_FUNC(_interwork_r7_call_via_r2) 
    SYMBOL_ITEM_FUNC(_interwork_r7_call_via_r3) 
    SYMBOL_ITEM_FUNC(_interwork_r7_call_via_r4) 
    SYMBOL_ITEM_FUNC(_interwork_r7_call_via_r5) 
    SYMBOL_ITEM_FUNC(_interwork_r7_call_via_r6) 
    SYMBOL_ITEM_FUNC(_interwork_r7_call_via_r7) 
    SYMBOL_ITEM_FUNC(_interwork_r7_call_via_r8) 
    SYMBOL_ITEM_FUNC(_interwork_r7_call_via_r9) 
    SYMBOL_ITEM_FUNC(_interwork_r7_call_via_sl) 
    SYMBOL_ITEM_FUNC(_interwork_r7_call_via_sp) 
    SYMBOL_ITEM_FUNC(__aeabi_llsr) 
    SYMBOL_ITEM_FUNC(__lshrdi3) 
    SYMBOL_ITEM_FUNC(__aeabi_lasr) 
    SYMBOL_ITEM_FUNC(__ashrdi3) 
    SYMBOL_ITEM_FUNC(__aeabi_llsl) 
    SYMBOL_ITEM_FUNC(__ashldi3) 
    SYMBOL_ITEM_FUNC(__aeabi_dneg) 
    SYMBOL_ITEM_FUNC(__negdf2) 
    SYMBOL_ITEM_FUNC(__adddf3) 
    SYMBOL_ITEM_FUNC(__aeabi_dadd) 
    SYMBOL_ITEM_FUNC(__aeabi_drsub) 
    SYMBOL_ITEM_FUNC(__aeabi_dsub) 
    SYMBOL_ITEM_FUNC(__aeabi_f2d) 
    SYMBOL_ITEM_FUNC(__aeabi_i2d) 
    SYMBOL_ITEM_FUNC(__aeabi_l2d) 
    SYMBOL_ITEM_FUNC(__aeabi_ui2d) 
    SYMBOL_ITEM_FUNC(__aeabi_ul2d) 
    SYMBOL_ITEM_FUNC(__extendsfdf2) 
    SYMBOL_ITEM_FUNC(__floatdidf) 
    SYMBOL_ITEM_FUNC(__floatsidf) 
    SYMBOL_ITEM_FUNC(__floatundidf) 
    SYMBOL_ITEM_FUNC(__floatunsidf) 
    SYMBOL_ITEM_FUNC(__subdf3) 
    SYMBOL_ITEM_FUNC(__aeabi_ddiv) 
    SYMBOL_ITEM_FUNC(__aeabi_dmul) 
    SYMBOL_ITEM_FUNC(__divdf3) 
    SYMBOL_ITEM_FUNC(__muldf3) 
    SYMBOL_ITEM_FUNC(__aeabi_cdcmpeq) 
    SYMBOL_ITEM_FUNC(__aeabi_cdcmple) 
    SYMBOL_ITEM_FUNC(__aeabi_cdrcmple) 
    SYMBOL_ITEM_FUNC(__aeabi_dcmpeq) 
    SYMBOL_ITEM_FUNC(__aeabi_dcmpge) 
    SYMBOL_ITEM_FUNC(__aeabi_dcmpgt) 
    SYMBOL_ITEM_FUNC(__aeabi_dcmple) 
    SYMBOL_ITEM_FUNC(__aeabi_dcmplt) 
    SYMBOL_ITEM_FUNC(__cmpdf2) 
    SYMBOL_ITEM_FUNC(__eqdf2) 
    SYMBOL_ITEM_FUNC(__gedf2) 
    SYMBOL_ITEM_FUNC(__gtdf2) 
    SYMBOL_ITEM_FUNC(__ledf2) 
    SYMBOL_ITEM_FUNC(__ltdf2) 
    SYMBOL_ITEM_FUNC(__nedf2) 
    SYMBOL_ITEM_FUNC(__aeabi_dcmpun) 
    SYMBOL_ITEM_FUNC(__unorddf2) 
    SYMBOL_ITEM_FUNC(__aeabi_d2iz) 
    SYMBOL_ITEM_FUNC(__fixdfsi) 
    SYMBOL_ITEM_FUNC(__aeabi_d2uiz) 
    SYMBOL_ITEM_FUNC(__fixunsdfsi) 
    SYMBOL_ITEM_FUNC(__aeabi_d2f) 
    SYMBOL_ITEM_FUNC(__truncdfsf2) 
    SYMBOL_ITEM_FUNC(__aeabi_fneg) 
    SYMBOL_ITEM_FUNC(__negsf2) 
    SYMBOL_ITEM_FUNC(__addsf3) 
    SYMBOL_ITEM_FUNC(__aeabi_fadd) 
    SYMBOL_ITEM_FUNC(__aeabi_frsub) 
    SYMBOL_ITEM_FUNC(__aeabi_fsub) 
    SYMBOL_ITEM_FUNC(__aeabi_i2f) 
    SYMBOL_ITEM_FUNC(__aeabi_l2f) 
    SYMBOL_ITEM_FUNC(__aeabi_ui2f) 
    SYMBOL_ITEM_FUNC(__aeabi_ul2f) 
    SYMBOL_ITEM_FUNC(__floatdisf) 
    SYMBOL_ITEM_FUNC(__floatsisf) 
    SYMBOL_ITEM_FUNC(__floatundisf) 
    SYMBOL_ITEM_FUNC(__floatunsisf) 
    SYMBOL_ITEM_FUNC(__subsf3) 
    SYMBOL_ITEM_FUNC(__aeabi_fdiv) 
    SYMBOL_ITEM_FUNC(__aeabi_fmul) 
    SYMBOL_ITEM_FUNC(__divsf3) 
    SYMBOL_ITEM_FUNC(__mulsf3) 
    SYMBOL_ITEM_FUNC(__aeabi_cfcmpeq) 
    SYMBOL_ITEM_FUNC(__aeabi_cfcmple) 
    SYMBOL_ITEM_FUNC(__aeabi_cfrcmple) 
    SYMBOL_ITEM_FUNC(__aeabi_fcmpeq) 
    SYMBOL_ITEM_FUNC(__aeabi_fcmpge) 
    SYMBOL_ITEM_FUNC(__aeabi_fcmpgt) 
    SYMBOL_ITEM_FUNC(__aeabi_fcmple) 
    SYMBOL_ITEM_FUNC(__aeabi_fcmplt) 
    SYMBOL_ITEM_FUNC(__cmpsf2) 
    SYMBOL_ITEM_FUNC(__eqsf2) 
    SYMBOL_ITEM_FUNC(__gesf2) 
    SYMBOL_ITEM_FUNC(__gtsf2) 
    SYMBOL_ITEM_FUNC(__lesf2) 
    SYMBOL_ITEM_FUNC(__ltsf2) 
    SYMBOL_ITEM_FUNC(__nesf2) 
    SYMBOL_ITEM_FUNC(__aeabi_fcmpun) 
    SYMBOL_ITEM_FUNC(__unordsf2) 
    SYMBOL_ITEM_FUNC(__aeabi_f2iz) 
    SYMBOL_ITEM_FUNC(__fixsfsi) 
    SYMBOL_ITEM_FUNC(__aeabi_f2uiz) 
    SYMBOL_ITEM_FUNC(__fixunssfsi) 
    SYMBOL_ITEM_FUNC(__clzsi2) 
    SYMBOL_ITEM_FUNC(__clzdi2) 
    SYMBOL_ITEM_FUNC(__aeabi_lcmp) 
    SYMBOL_ITEM_FUNC(__aeabi_ulcmp) 
    SYMBOL_ITEM_FUNC(__aeabi_ldivmod) 
    SYMBOL_ITEM_FUNC(__aeabi_uldivmod) 
    SYMBOL_ITEM_FUNC(__aeabi_lmul) 
    SYMBOL_ITEM_FUNC(__muldi3) 
    SYMBOL_ITEM_FUNC(__negdi2) 
    SYMBOL_ITEM_FUNC(__cmpdi2) 
    SYMBOL_ITEM_FUNC(__ucmpdi2) 
    SYMBOL_ITEM_FUNC(__clear_cache) 
    SYMBOL_ITEM_FUNC(__enable_execute_stack) 
    SYMBOL_ITEM_FUNC(__absvsi2) 
    SYMBOL_ITEM_FUNC(__absvdi2) 
    SYMBOL_ITEM_FUNC(__addvsi3) 
    SYMBOL_ITEM_FUNC(__addvdi3) 
    SYMBOL_ITEM_FUNC(__subvsi3) 
    SYMBOL_ITEM_FUNC(__subvdi3) 
    SYMBOL_ITEM_FUNC(__mulvsi3) 
    SYMBOL_ITEM_FUNC(__mulvdi3) 
    SYMBOL_ITEM_FUNC(__negvsi2) 
    SYMBOL_ITEM_FUNC(__negvdi2) 
    SYMBOL_ITEM_FUNC(__ffssi2) 
    SYMBOL_ITEM_FUNC(__ffsdi2) 
    SYMBOL_ITEM_FUNC(__ctzsi2) 
    SYMBOL_ITEM_FUNC(__ctzdi2) 
    SYMBOL_ITEM_FUNC(__popcountsi2) 
    SYMBOL_ITEM_FUNC(__popcountdi2) 
    SYMBOL_ITEM_FUNC(__paritysi2) 
    SYMBOL_ITEM_FUNC(__paritydi2) 
    SYMBOL_ITEM_FUNC(__powisf2) 
    SYMBOL_ITEM_FUNC(__powidf2) 
    SYMBOL_ITEM_FUNC(__mulsc3) 
    SYMBOL_ITEM_FUNC(__muldc3) 
    SYMBOL_ITEM_FUNC(__divsc3) 
    SYMBOL_ITEM_FUNC(__divdc3) 
    SYMBOL_ITEM_FUNC(__bswapsi2) 
    SYMBOL_ITEM_FUNC(__bswapdi2) 
    SYMBOL_ITEM_FUNC(__aeabi_f2lz) 
    SYMBOL_ITEM_FUNC(__fixsfdi) 
    SYMBOL_ITEM_FUNC(__aeabi_d2lz) 
    SYMBOL_ITEM_FUNC(__fixdfdi) 
    SYMBOL_ITEM_FUNC(__aeabi_f2ulz) 
    SYMBOL_ITEM_FUNC(__fixunssfdi) 
    SYMBOL_ITEM_FUNC(__aeabi_d2ulz) 
    SYMBOL_ITEM_FUNC(__fixunsdfdi) 
    SYMBOL_ITEM_FUNC(__eprintf) 
    SYMBOL_ITEM_FUNC(__gcc_bcmp) 
    SYMBOL_ITEM_FUNC(__divdi3) 
    SYMBOL_ITEM_FUNC(__moddi3) 
    SYMBOL_ITEM_FUNC(__udivdi3) 
    SYMBOL_ITEM_FUNC(__umoddi3) 
    SYMBOL_ITEM_FUNC(__udiv_w_sdiv) 
    SYMBOL_ITEM_FUNC(__udivmoddi4) 
    SYMBOL_ITEM_FUNC(__gnu_ldivmod_helper) 
    SYMBOL_ITEM_FUNC(__gnu_uldivmod_helper) 
    SYMBOL_ITEM_FUNC(__aeabi_uread4) 
    SYMBOL_ITEM_FUNC(__aeabi_uread8) 
    SYMBOL_ITEM_FUNC(__aeabi_uwrite4) 
    SYMBOL_ITEM_FUNC(__aeabi_uwrite8) 
    SYMBOL_ITEM_FUNC(__gnu_f2h_alternative) 
    SYMBOL_ITEM_FUNC(__gnu_f2h_ieee) 
    SYMBOL_ITEM_FUNC(__gnu_h2f_alternative) 
    SYMBOL_ITEM_FUNC(__gnu_h2f_ieee) 
    SYMBOL_ITEM_FUNC(__gnu_h2f_internal) 
    SYMBOL_ITEM_FUNC(_Unwind_Complete) 
    SYMBOL_ITEM_FUNC(_Unwind_DeleteException) 
    SYMBOL_ITEM_FUNC(_Unwind_GetCFA) 
    SYMBOL_ITEM_FUNC(_Unwind_VRS_Get) 
    SYMBOL_ITEM_FUNC(_Unwind_VRS_Pop) 
    SYMBOL_ITEM_FUNC(_Unwind_VRS_Set) 
    SYMBOL_ITEM_FUNC(__aeabi_unwind_cpp_pr0) 
    SYMBOL_ITEM_FUNC(__gnu_Unwind_Backtrace) 
    SYMBOL_ITEM_FUNC(__gnu_Unwind_ForcedUnwind) 
    SYMBOL_ITEM_FUNC(__gnu_Unwind_RaiseException) 
    SYMBOL_ITEM_FUNC(__gnu_Unwind_Resume) 
    SYMBOL_ITEM_FUNC(__gnu_Unwind_Resume_or_Rethrow) 
    SYMBOL_ITEM_FUNC(_Unwind_Backtrace) 
    SYMBOL_ITEM_FUNC(_Unwind_ForcedUnwind) 
    SYMBOL_ITEM_FUNC(_Unwind_RaiseException) 
    SYMBOL_ITEM_FUNC(_Unwind_Resume) 
    SYMBOL_ITEM_FUNC(_Unwind_Resume_or_Rethrow) 
    SYMBOL_ITEM_FUNC(___Unwind_Backtrace) 
    SYMBOL_ITEM_FUNC(___Unwind_ForcedUnwind) 
    SYMBOL_ITEM_FUNC(___Unwind_RaiseException) 
    SYMBOL_ITEM_FUNC(___Unwind_Resume) 
    SYMBOL_ITEM_FUNC(___Unwind_Resume_or_Rethrow) 
    SYMBOL_ITEM_FUNC(__gnu_Unwind_Restore_VFP) 
    SYMBOL_ITEM_FUNC(__gnu_Unwind_Restore_VFP_D) 
    SYMBOL_ITEM_FUNC(__gnu_Unwind_Restore_VFP_D_16_to_31) 
    SYMBOL_ITEM_FUNC(__gnu_Unwind_Restore_WMMXC) 
    SYMBOL_ITEM_FUNC(__gnu_Unwind_Restore_WMMXD) 
    SYMBOL_ITEM_FUNC(__gnu_Unwind_Save_VFP) 
    SYMBOL_ITEM_FUNC(__gnu_Unwind_Save_VFP_D) 
    SYMBOL_ITEM_FUNC(__gnu_Unwind_Save_VFP_D_16_to_31) 
    SYMBOL_ITEM_FUNC(__gnu_Unwind_Save_WMMXC) 
    SYMBOL_ITEM_FUNC(__gnu_Unwind_Save_WMMXD) 
    SYMBOL_ITEM_FUNC(__restore_core_regs) 
    SYMBOL_ITEM_FUNC(restore_core_regs) 
    SYMBOL_ITEM_FUNC(_Unwind_GetDataRelBase) 
    SYMBOL_ITEM_FUNC(_Unwind_GetLanguageSpecificData) 
    SYMBOL_ITEM_FUNC(_Unwind_GetRegionStart) 
    SYMBOL_ITEM_FUNC(_Unwind_GetTextRelBase) 
    SYMBOL_ITEM_FUNC(__gnu_unwind_execute) 
    SYMBOL_ITEM_FUNC(__gnu_unwind_frame) 
    SYMBOL_ITEM_FUNC(__gcc_personality_v0) 
    SYMBOL_ITEM_FUNC(__emutls_get_address) 
    SYMBOL_ITEM_FUNC(__emutls_register_common) 
    SYMBOL_ITEM_FUNC(acoshl) 
    SYMBOL_ITEM_FUNC(acosl) 
    SYMBOL_ITEM_FUNC(asinhl) 
    SYMBOL_ITEM_FUNC(asinl) 
    SYMBOL_ITEM_FUNC(atan2l) 
    SYMBOL_ITEM_FUNC(atanhl) 
    SYMBOL_ITEM_FUNC(atanl) 
    SYMBOL_ITEM_FUNC(cabs) 
    SYMBOL_ITEM_FUNC(cabsf) 
    SYMBOL_ITEM_FUNC(cacos) 
    SYMBOL_ITEM_FUNC(cacosf) 
    SYMBOL_ITEM_FUNC(cacosh) 
    SYMBOL_ITEM_FUNC(cacoshf) 
    SYMBOL_ITEM_FUNC(carg) 
    SYMBOL_ITEM_FUNC(cargf) 
    SYMBOL_ITEM_FUNC(casin) 
    SYMBOL_ITEM_FUNC(casinf) 
    SYMBOL_ITEM_FUNC(casinh) 
    SYMBOL_ITEM_FUNC(casinhf) 
    SYMBOL_ITEM_FUNC(catan) 
    SYMBOL_ITEM_FUNC(catanf) 
    SYMBOL_ITEM_FUNC(catanh) 
    SYMBOL_ITEM_FUNC(catanhf) 
    SYMBOL_ITEM_FUNC(cbrtl) 
    SYMBOL_ITEM_FUNC(ccos) 
    SYMBOL_ITEM_FUNC(ccosf) 
    SYMBOL_ITEM_FUNC(ccosh) 
    SYMBOL_ITEM_FUNC(ccoshf) 
    SYMBOL_ITEM_FUNC(ceill) 
    SYMBOL_ITEM_FUNC(_cchsh) 
    SYMBOL_ITEM_FUNC(_ctans) 
    SYMBOL_ITEM_FUNC(_redupi) 
    SYMBOL_ITEM_FUNC(_cchshf) 
    SYMBOL_ITEM_FUNC(_ctansf) 
    SYMBOL_ITEM_FUNC(_redupif) 
    SYMBOL_ITEM_FUNC(cexp) 
    SYMBOL_ITEM_FUNC(cexpf) 
    SYMBOL_ITEM_FUNC(cimag) 
    SYMBOL_ITEM_FUNC(cimagf) 
    SYMBOL_ITEM_FUNC(clog) 
    SYMBOL_ITEM_FUNC(clogf) 
    SYMBOL_ITEM_FUNC(conj) 
    SYMBOL_ITEM_FUNC(conjf) 
    SYMBOL_ITEM_FUNC(copysignl) 
    SYMBOL_ITEM_FUNC(coshl) 
    SYMBOL_ITEM_FUNC(cosl) 
    SYMBOL_ITEM_FUNC(cpow) 
    SYMBOL_ITEM_FUNC(cpowf) 
    SYMBOL_ITEM_FUNC(cproj) 
    SYMBOL_ITEM_FUNC(cprojf) 
    SYMBOL_ITEM_FUNC(creal) 
    SYMBOL_ITEM_FUNC(crealf) 
    SYMBOL_ITEM_FUNC(csin) 
    SYMBOL_ITEM_FUNC(csinf) 
    SYMBOL_ITEM_FUNC(csinh) 
    SYMBOL_ITEM_FUNC(csinhf) 
    SYMBOL_ITEM_FUNC(csqrt) 
    SYMBOL_ITEM_FUNC(csqrtf) 
    SYMBOL_ITEM_FUNC(ctan) 
    SYMBOL_ITEM_FUNC(ctanf) 
    SYMBOL_ITEM_FUNC(ctanh) 
    SYMBOL_ITEM_FUNC(ctanhf) 
    SYMBOL_ITEM_FUNC(__ieee754_acos) 
    SYMBOL_ITEM_FUNC(__ieee754_acosh) 
    SYMBOL_ITEM_FUNC(__ieee754_asin) 
    SYMBOL_ITEM_FUNC(__ieee754_atan2) 
    SYMBOL_ITEM_FUNC(__ieee754_atanh) 
    SYMBOL_ITEM_FUNC(__ieee754_cosh) 
    SYMBOL_ITEM_FUNC(__ieee754_exp) 
    SYMBOL_ITEM_FUNC(__ieee754_fmod) 
    SYMBOL_ITEM_FUNC(__ieee754_hypot) 
    SYMBOL_ITEM_FUNC(__ieee754_j0) 
    SYMBOL_ITEM_FUNC(__ieee754_y0) 
    SYMBOL_ITEM_FUNC(__ieee754_j1) 
    SYMBOL_ITEM_FUNC(__ieee754_y1) 
    SYMBOL_ITEM_FUNC(__ieee754_jn) 
    SYMBOL_ITEM_FUNC(__ieee754_yn) 
    SYMBOL_ITEM_FUNC(__ieee754_log) 
    SYMBOL_ITEM_FUNC(__ieee754_log10) 
    SYMBOL_ITEM_FUNC(__ieee754_pow) 
    SYMBOL_ITEM_FUNC(__ieee754_rem_pio2) 
    SYMBOL_ITEM_FUNC(__ieee754_remainder) 
    SYMBOL_ITEM_FUNC(__ieee754_scalb) 
    SYMBOL_ITEM_FUNC(__ieee754_sinh) 
    SYMBOL_ITEM_FUNC(__ieee754_sqrt) 
    SYMBOL_ITEM_FUNC(__ieee754_acosf) 
    SYMBOL_ITEM_FUNC(__ieee754_acoshf) 
    SYMBOL_ITEM_FUNC(__ieee754_asinf) 
    SYMBOL_ITEM_FUNC(__ieee754_atan2f) 
    SYMBOL_ITEM_FUNC(__ieee754_atanhf) 
    SYMBOL_ITEM_FUNC(__ieee754_coshf) 
    SYMBOL_ITEM_FUNC(__ieee754_expf) 
    SYMBOL_ITEM_FUNC(__ieee754_fmodf) 
    SYMBOL_ITEM_FUNC(__ieee754_hypotf) 
    SYMBOL_ITEM_FUNC(__ieee754_j0f) 
    SYMBOL_ITEM_FUNC(__ieee754_y0f) 
    SYMBOL_ITEM_FUNC(__ieee754_j1f) 
    SYMBOL_ITEM_FUNC(__ieee754_y1f) 
    SYMBOL_ITEM_FUNC(__ieee754_jnf) 
    SYMBOL_ITEM_FUNC(__ieee754_ynf) 
    SYMBOL_ITEM_FUNC(__ieee754_logf) 
    SYMBOL_ITEM_FUNC(__ieee754_log10f) 
    SYMBOL_ITEM_FUNC(__ieee754_powf) 
    SYMBOL_ITEM_FUNC(__ieee754_rem_pio2f) 
    SYMBOL_ITEM_FUNC(__ieee754_remainderf) 
    SYMBOL_ITEM_FUNC(__ieee754_scalbf) 
    SYMBOL_ITEM_FUNC(__ieee754_sinhf) 
    SYMBOL_ITEM_FUNC(__ieee754_sqrtf) 
    SYMBOL_ITEM_FUNC(__ieee754_gamma_r) 
    SYMBOL_ITEM_FUNC(__ieee754_lgamma_r) 
    SYMBOL_ITEM_FUNC(__ieee754_gammaf_r) 
    SYMBOL_ITEM_FUNC(__ieee754_lgammaf_r) 
    SYMBOL_ITEM_FUNC(erfcl) 
    SYMBOL_ITEM_FUNC(erfl) 
    SYMBOL_ITEM_FUNC(exp2l) 
    SYMBOL_ITEM_FUNC(expl) 
    SYMBOL_ITEM_FUNC(expm1l) 
    SYMBOL_ITEM_FUNC(fabsl) 
    SYMBOL_ITEM_FUNC(fdiml) 
    SYMBOL_ITEM_FUNC(floorl) 
    SYMBOL_ITEM_FUNC(fmal) 
    SYMBOL_ITEM_FUNC(fmaxl) 
    SYMBOL_ITEM_FUNC(fminl) 
    SYMBOL_ITEM_FUNC(fmodl) 
    SYMBOL_ITEM_FUNC(frexpl) 
    SYMBOL_ITEM_FUNC(hypotl) 
    SYMBOL_ITEM_FUNC(ilogbl) 
    SYMBOL_ITEM_FUNC(__kernel_cos) 
    SYMBOL_ITEM_FUNC(__kernel_rem_pio2) 
    SYMBOL_ITEM_FUNC(__kernel_sin) 
    SYMBOL_ITEM_FUNC(__kernel_standard) 
    SYMBOL_ITEM_FUNC(__kernel_tan) 
    SYMBOL_ITEM_FUNC(__kernel_cosf) 
    SYMBOL_ITEM_FUNC(__kernel_rem_pio2f) 
    SYMBOL_ITEM_FUNC(__kernel_sinf) 
    SYMBOL_ITEM_FUNC(__kernel_tanf) 
    SYMBOL_ITEM_FUNC(ldexpl) 
    SYMBOL_ITEM_FUNC(lgammal) 
    SYMBOL_ITEM_FUNC(llrintl) 
    SYMBOL_ITEM_FUNC(llroundl) 
    SYMBOL_ITEM_FUNC(log10l) 
    SYMBOL_ITEM_FUNC(log1pl) 
    SYMBOL_ITEM_FUNC(logl) 
    SYMBOL_ITEM_FUNC(lrintl) 
    SYMBOL_ITEM_FUNC(lroundl) 
    SYMBOL_ITEM_FUNC(modfl) 
    SYMBOL_ITEM_FUNC(nanl) 
    SYMBOL_ITEM_FUNC(nearbyintl) 
    SYMBOL_ITEM_FUNC(nextafterl) 
    SYMBOL_ITEM_FUNC(powl) 
    SYMBOL_ITEM_FUNC(remainderl) 
    SYMBOL_ITEM_FUNC(remquol) 
    SYMBOL_ITEM_FUNC(rintl) 
    SYMBOL_ITEM_FUNC(roundl) 
    SYMBOL_ITEM_FUNC(asinh) 
    SYMBOL_ITEM_FUNC(atan) 
    SYMBOL_ITEM_FUNC(cbrt) 
    SYMBOL_ITEM_FUNC(ceil) 
    SYMBOL_ITEM_FUNC(copysign) 
    SYMBOL_ITEM_FUNC(cos) 
    SYMBOL_ITEM_FUNC(erf) 
    SYMBOL_ITEM_FUNC(erfc) 
    SYMBOL_ITEM_FUNC(exp10) 
    SYMBOL_ITEM_FUNC(expm1) 
    SYMBOL_ITEM_FUNC(fabs) 
    SYMBOL_ITEM_FUNC(fdim) 
    SYMBOL_ITEM_FUNC(finite) 
    SYMBOL_ITEM_FUNC(floor) 
    SYMBOL_ITEM_FUNC(fma) 
    SYMBOL_ITEM_FUNC(fmax) 
    SYMBOL_ITEM_FUNC(fmin) 
    SYMBOL_ITEM_FUNC(__fpclassifyd) 
    SYMBOL_ITEM_FUNC(frexp) 
    SYMBOL_ITEM_FUNC(ilogb) 
    SYMBOL_ITEM_FUNC(infinity) 
    SYMBOL_ITEM_FUNC(isinf) 
    SYMBOL_ITEM_FUNC(__isinfd) 
    SYMBOL_ITEM_FUNC(isnan) 
    SYMBOL_ITEM_FUNC(__isnand) 
    SYMBOL_ITEM_FUNC(ldexp) 
    SYMBOL_ITEM_FUNC(llrint) 
    SYMBOL_ITEM_FUNC(llround) 
    SYMBOL_ITEM_FUNC(log1p) 
    SYMBOL_ITEM_FUNC(log2) 
    SYMBOL_ITEM_FUNC(logb) 
    SYMBOL_ITEM_FUNC(lrint) 
    SYMBOL_ITEM_FUNC(lround) 
    SYMBOL_ITEM_FUNC(matherr) 
    SYMBOL_ITEM_FUNC(modf) 
    SYMBOL_ITEM_FUNC(nan) 
    SYMBOL_ITEM_FUNC(nearbyint) 
    SYMBOL_ITEM_FUNC(nextafter) 
    SYMBOL_ITEM_FUNC(pow10) 
    SYMBOL_ITEM_FUNC(remquo) 
    SYMBOL_ITEM_FUNC(rint) 
    SYMBOL_ITEM_FUNC(round) 
    SYMBOL_ITEM_FUNC(scalbln) 
    SYMBOL_ITEM_FUNC(scalbn) 
    SYMBOL_ITEM_FUNC(__signbitd) 
    SYMBOL_ITEM_FUNC(__signbitf) 
    SYMBOL_ITEM_FUNC(significand) 
    SYMBOL_ITEM_FUNC(sin) 
    SYMBOL_ITEM_FUNC(tan) 
    SYMBOL_ITEM_FUNC(tanh) 
    SYMBOL_ITEM_FUNC(trunc) 
    SYMBOL_ITEM_FUNC(scalblnl) 
    SYMBOL_ITEM_FUNC(scalbnl) 
    SYMBOL_ITEM_FUNC(asinhf) 
    SYMBOL_ITEM_FUNC(atanf) 
    SYMBOL_ITEM_FUNC(cbrtf) 
    SYMBOL_ITEM_FUNC(ceilf) 
    SYMBOL_ITEM_FUNC(copysignf) 
    SYMBOL_ITEM_FUNC(cosf) 
    SYMBOL_ITEM_FUNC(erfcf) 
    SYMBOL_ITEM_FUNC(erff) 
    SYMBOL_ITEM_FUNC(exp10f) 
    SYMBOL_ITEM_FUNC(expm1f) 
    SYMBOL_ITEM_FUNC(fabsf) 
    SYMBOL_ITEM_FUNC(fdimf) 
    SYMBOL_ITEM_FUNC(finitef) 
    SYMBOL_ITEM_FUNC(floorf) 
    SYMBOL_ITEM_FUNC(fmaf) 
    SYMBOL_ITEM_FUNC(fmaxf) 
    SYMBOL_ITEM_FUNC(fminf) 
    SYMBOL_ITEM_FUNC(__fpclassifyf) 
    SYMBOL_ITEM_FUNC(frexpf) 
    SYMBOL_ITEM_FUNC(ilogbf) 
    SYMBOL_ITEM_FUNC(infinityf) 
    SYMBOL_ITEM_FUNC(isinff) 
    SYMBOL_ITEM_FUNC(__isinff) 
    SYMBOL_ITEM_FUNC(isnanf) 
    SYMBOL_ITEM_FUNC(__isnanf) 
    SYMBOL_ITEM_FUNC(ldexpf) 
    SYMBOL_ITEM_FUNC(llrintf) 
    SYMBOL_ITEM_FUNC(llroundf) 
    SYMBOL_ITEM_FUNC(log1pf) 
    SYMBOL_ITEM_FUNC(log2f) 
    SYMBOL_ITEM_FUNC(logbf) 
    SYMBOL_ITEM_FUNC(lrintf) 
    SYMBOL_ITEM_FUNC(lroundf) 
    SYMBOL_ITEM_FUNC(modff) 
    SYMBOL_ITEM_FUNC(nanf) 
    SYMBOL_ITEM_FUNC(nearbyintf) 
    SYMBOL_ITEM_FUNC(nextafterf) 
    SYMBOL_ITEM_FUNC(pow10f) 
    SYMBOL_ITEM_FUNC(remquof) 
    SYMBOL_ITEM_FUNC(rintf) 
    SYMBOL_ITEM_FUNC(roundf) 
    SYMBOL_ITEM_FUNC(scalblnf) 
    SYMBOL_ITEM_FUNC(scalbnf) 
    SYMBOL_ITEM_FUNC(significandf) 
    SYMBOL_ITEM_FUNC(sinf) 
    SYMBOL_ITEM_FUNC(tanf) 
    SYMBOL_ITEM_FUNC(tanhf) 
    SYMBOL_ITEM_FUNC(truncf) 
    SYMBOL_ITEM_FUNC(sinhl) 
    SYMBOL_ITEM_FUNC(sinl) 
    SYMBOL_ITEM_FUNC(sqrtl) 
    SYMBOL_ITEM_FUNC(tanhl) 
    SYMBOL_ITEM_FUNC(tanl) 
    SYMBOL_ITEM_FUNC(tgammal) 
    SYMBOL_ITEM_FUNC(truncl) 
    SYMBOL_ITEM_FUNC(acos) 
    SYMBOL_ITEM_FUNC(acosh) 
    SYMBOL_ITEM_FUNC(asin) 
    SYMBOL_ITEM_FUNC(atan2) 
    SYMBOL_ITEM_FUNC(atanh) 
    SYMBOL_ITEM_FUNC(cosh) 
    SYMBOL_ITEM_FUNC(drem) 
    SYMBOL_ITEM_FUNC(exp) 
    SYMBOL_ITEM_FUNC(exp2) 
    SYMBOL_ITEM_FUNC(fmod) 
    SYMBOL_ITEM_FUNC(gamma) 
    SYMBOL_ITEM_FUNC(hypot) 
    SYMBOL_ITEM_FUNC(j0) 
    SYMBOL_ITEM_FUNC(y0) 
    SYMBOL_ITEM_FUNC(j1) 
    SYMBOL_ITEM_FUNC(y1) 
    SYMBOL_ITEM_FUNC(jn) 
    SYMBOL_ITEM_FUNC(yn) 
    SYMBOL_ITEM_FUNC(lgamma) 
    SYMBOL_ITEM_FUNC(log) 
    SYMBOL_ITEM_FUNC(log10) 
    SYMBOL_ITEM_FUNC(pow) 
    SYMBOL_ITEM_FUNC(remainder) 
    SYMBOL_ITEM_FUNC(scalb) 
    SYMBOL_ITEM_FUNC(sincos) 
    SYMBOL_ITEM_FUNC(sinh) 
    SYMBOL_ITEM_FUNC(sqrt) 
    SYMBOL_ITEM_FUNC(tgamma) 
    SYMBOL_ITEM_FUNC(acosf) 
    SYMBOL_ITEM_FUNC(acoshf) 
    SYMBOL_ITEM_FUNC(asinf) 
    SYMBOL_ITEM_FUNC(atan2f) 
    SYMBOL_ITEM_FUNC(atanhf) 
    SYMBOL_ITEM_FUNC(coshf) 
    SYMBOL_ITEM_FUNC(dremf) 
    SYMBOL_ITEM_FUNC(expf) 
    SYMBOL_ITEM_FUNC(exp2f) 
    SYMBOL_ITEM_FUNC(fmodf) 
    SYMBOL_ITEM_FUNC(gammaf) 
    SYMBOL_ITEM_FUNC(hypotf) 
    SYMBOL_ITEM_FUNC(j0f) 
    SYMBOL_ITEM_FUNC(y0f) 
    SYMBOL_ITEM_FUNC(j1f) 
    SYMBOL_ITEM_FUNC(y1f) 
    SYMBOL_ITEM_FUNC(jnf) 
    SYMBOL_ITEM_FUNC(ynf) 
    SYMBOL_ITEM_FUNC(lgammaf) 
    SYMBOL_ITEM_FUNC(logf) 
    SYMBOL_ITEM_FUNC(log10f) 
    SYMBOL_ITEM_FUNC(powf) 
    SYMBOL_ITEM_FUNC(remainderf) 
    SYMBOL_ITEM_FUNC(scalbf) 
    SYMBOL_ITEM_FUNC(sincosf) 
    SYMBOL_ITEM_FUNC(sinhf) 
    SYMBOL_ITEM_FUNC(sqrtf) 
    SYMBOL_ITEM_FUNC(tgammaf) 
    SYMBOL_ITEM_FUNC(gamma_r) 
    SYMBOL_ITEM_FUNC(lgamma_r) 
    SYMBOL_ITEM_FUNC(gammaf_r) 
    SYMBOL_ITEM_FUNC(lgammaf_r) 
    SYMBOL_ITEM_FUNC(__adjust) 
    SYMBOL_ITEM_FUNC(__register_exitproc) 
    SYMBOL_ITEM_FUNC(__call_exitprocs) 
    SYMBOL_ITEM_FUNC(__dprintf) 
    SYMBOL_ITEM_FUNC(__exp10) 
    SYMBOL_ITEM_FUNC(__ten_mul) 
    SYMBOL_ITEM_FUNC(_Exit) 
    SYMBOL_ITEM_FUNC(a64l) 
    SYMBOL_ITEM_FUNC(abort) 
    SYMBOL_ITEM_FUNC(abs) 
    SYMBOL_ITEM_FUNC(access) 
    SYMBOL_ITEM_FUNC(__aeabi_atexit) 
    SYMBOL_ITEM_FUNC(argz_add) 
    SYMBOL_ITEM_FUNC(argz_add_sep) 
    SYMBOL_ITEM_FUNC(argz_append) 
    SYMBOL_ITEM_FUNC(argz_count) 
    SYMBOL_ITEM_FUNC(argz_create) 
    SYMBOL_ITEM_FUNC(argz_create_sep) 
    SYMBOL_ITEM_FUNC(argz_delete) 
    SYMBOL_ITEM_FUNC(argz_extract) 
    SYMBOL_ITEM_FUNC(argz_insert) 
    SYMBOL_ITEM_FUNC(argz_next) 
    SYMBOL_ITEM_FUNC(argz_replace) 
    SYMBOL_ITEM_FUNC(argz_stringify) 
    SYMBOL_ITEM_FUNC(asctime) 
    SYMBOL_ITEM_FUNC(asctime_r) 
    SYMBOL_ITEM_FUNC(_asiprintf_r) 
    SYMBOL_ITEM_FUNC(asiprintf) 
    SYMBOL_ITEM_FUNC(_asniprintf_r) 
    SYMBOL_ITEM_FUNC(asniprintf) 
    SYMBOL_ITEM_FUNC(_asnprintf_r) 
    SYMBOL_ITEM_FUNC(asnprintf) 
    SYMBOL_ITEM_FUNC(_asprintf_r) 
    SYMBOL_ITEM_FUNC(asprintf) 
    SYMBOL_ITEM_FUNC(__assert) 
    SYMBOL_ITEM_FUNC(__assert_func) 
    SYMBOL_ITEM_FUNC(atexit) 
    SYMBOL_ITEM_FUNC(atof) 
    SYMBOL_ITEM_FUNC(atoff) 
    SYMBOL_ITEM_FUNC(_atoi_r) 
    SYMBOL_ITEM_FUNC(atoi) 
    SYMBOL_ITEM_FUNC(_atol_r) 
    SYMBOL_ITEM_FUNC(atol) 
    SYMBOL_ITEM_FUNC(_atoll_r) 
    SYMBOL_ITEM_FUNC(atoll) 
    SYMBOL_ITEM_FUNC(bcmp) 
    SYMBOL_ITEM_FUNC(bcopy) 
    SYMBOL_ITEM_FUNC(bsearch) 
    SYMBOL_ITEM_FUNC(btowc) 
    SYMBOL_ITEM_FUNC(_buf_findstr) 
    SYMBOL_ITEM_FUNC(bzero) 
    SYMBOL_ITEM_FUNC(calloc) 
    SYMBOL_ITEM_FUNC(_calloc_r) 
    SYMBOL_ITEM_FUNC(cfree) 
    SYMBOL_ITEM_FUNC(clearerr) 
    SYMBOL_ITEM_FUNC(clock) 
    SYMBOL_ITEM_FUNC(_close_r) 
    SYMBOL_ITEM_FUNC(ctime) 
    SYMBOL_ITEM_FUNC(ctime_r) 
    SYMBOL_ITEM_FUNC(__cxa_atexit) 
    SYMBOL_ITEM_FUNC(__cxa_finalize) 
    SYMBOL_ITEM_FUNC(difftime) 
    SYMBOL_ITEM_FUNC(_diprintf_r) 
    SYMBOL_ITEM_FUNC(diprintf) 
    SYMBOL_ITEM_FUNC(div) 
    SYMBOL_ITEM_FUNC(_dprintf_r) 
    SYMBOL_ITEM_FUNC(dprintf) 
    SYMBOL_ITEM_FUNC(_drand48_r) 
    SYMBOL_ITEM_FUNC(drand48) 
    SYMBOL_ITEM_FUNC(_dtoa_r) 
    SYMBOL_ITEM_FUNC(__dtoa) 
    SYMBOL_ITEM_FUNC(_dcvt) 
    SYMBOL_ITEM_FUNC(_gcvt) 
    SYMBOL_ITEM_FUNC(ecvtbuf) 
    SYMBOL_ITEM_FUNC(fcvtbuf) 
    SYMBOL_ITEM_FUNC(ecvt) 
    SYMBOL_ITEM_FUNC(ecvtf) 
    SYMBOL_ITEM_FUNC(fcvt) 
    SYMBOL_ITEM_FUNC(fcvtf) 
    SYMBOL_ITEM_FUNC(gcvt) 
    SYMBOL_ITEM_FUNC(gcvtf) 
    SYMBOL_ITEM_FUNC(__env_lock) 
    SYMBOL_ITEM_FUNC(__env_unlock) 
    SYMBOL_ITEM_FUNC(envz_add) 
    SYMBOL_ITEM_FUNC(envz_entry) 
    SYMBOL_ITEM_FUNC(envz_get) 
    SYMBOL_ITEM_FUNC(envz_merge) 
    SYMBOL_ITEM_FUNC(envz_remove) 
    SYMBOL_ITEM_FUNC(envz_strip) 
    SYMBOL_ITEM_FUNC(__eprintf) 
    SYMBOL_ITEM_FUNC(_erand48_r) 
    SYMBOL_ITEM_FUNC(erand48) 
    SYMBOL_ITEM_FUNC(__errno) 
    SYMBOL_ITEM_FUNC(_execve_r) 
    SYMBOL_ITEM_FUNC(_fork_r) 
    SYMBOL_ITEM_FUNC(_wait_r) 
    SYMBOL_ITEM_FUNC(exit) 
    SYMBOL_ITEM_FUNC(_fclose_r) 
    SYMBOL_ITEM_FUNC(fclose) 
    SYMBOL_ITEM_FUNC(_fcloseall_r) 
    SYMBOL_ITEM_FUNC(fcloseall) 
    SYMBOL_ITEM_FUNC(_fdopen_r) 
    SYMBOL_ITEM_FUNC(fdopen) 
    SYMBOL_ITEM_FUNC(feof) 
    SYMBOL_ITEM_FUNC(ferror) 
    SYMBOL_ITEM_FUNC(_fflush_r) 
    SYMBOL_ITEM_FUNC(fflush) 
    SYMBOL_ITEM_FUNC(ffs) 
    SYMBOL_ITEM_FUNC(_fgetc_r) 
    SYMBOL_ITEM_FUNC(fgetc) 
    SYMBOL_ITEM_FUNC(_fgetpos_r) 
    SYMBOL_ITEM_FUNC(fgetpos) 
    SYMBOL_ITEM_FUNC(_fgets_r) 
    SYMBOL_ITEM_FUNC(fgets) 
    SYMBOL_ITEM_FUNC(_fgetwc_r) 
    SYMBOL_ITEM_FUNC(fgetwc) 
    SYMBOL_ITEM_FUNC(_fgetws_r) 
    SYMBOL_ITEM_FUNC(fgetws) 
    SYMBOL_ITEM_FUNC(fileno) 
    SYMBOL_ITEM_FUNC(__fp_lock_all) 
    SYMBOL_ITEM_FUNC(__fp_unlock_all) 
    SYMBOL_ITEM_FUNC(__sfmoreglue) 
    SYMBOL_ITEM_FUNC(__sfp) 
    SYMBOL_ITEM_FUNC(__sfp_lock_acquire) 
    SYMBOL_ITEM_FUNC(__sfp_lock_release) 
    SYMBOL_ITEM_FUNC(__sinit) 
    SYMBOL_ITEM_FUNC(__sinit_lock_acquire) 
    SYMBOL_ITEM_FUNC(__sinit_lock_release) 
    SYMBOL_ITEM_FUNC(_cleanup) 
    SYMBOL_ITEM_FUNC(_cleanup_r) 
    SYMBOL_ITEM_FUNC(__libc_fini_array) 
    SYMBOL_ITEM_FUNC(_fiprintf_r) 
    SYMBOL_ITEM_FUNC(fiprintf) 
    SYMBOL_ITEM_FUNC(_fiscanf_r) 
    SYMBOL_ITEM_FUNC(fiscanf) 
    SYMBOL_ITEM_FUNC(__fix_locale_grouping_str) 
    SYMBOL_ITEM_FUNC(__sflags) 
    SYMBOL_ITEM_FUNC(_fmemopen_r) 
    SYMBOL_ITEM_FUNC(fmemopen) 
    SYMBOL_ITEM_FUNC(_fopen_r) 
    SYMBOL_ITEM_FUNC(fopen) 
    SYMBOL_ITEM_FUNC(_fopencookie_r) 
    SYMBOL_ITEM_FUNC(fopencookie) 
    SYMBOL_ITEM_FUNC(_fprintf_r) 
    SYMBOL_ITEM_FUNC(fprintf) 
    SYMBOL_ITEM_FUNC(_fpurge_r) 
    SYMBOL_ITEM_FUNC(fpurge) 
    SYMBOL_ITEM_FUNC(_fputc_r) 
    SYMBOL_ITEM_FUNC(fputc) 
    SYMBOL_ITEM_FUNC(_fputs_r) 
    SYMBOL_ITEM_FUNC(fputs) 
    SYMBOL_ITEM_FUNC(_fputwc_r) 
    SYMBOL_ITEM_FUNC(fputwc) 
    SYMBOL_ITEM_FUNC(_fputws_r) 
    SYMBOL_ITEM_FUNC(fputws) 
    SYMBOL_ITEM_FUNC(_fread_r) 
    SYMBOL_ITEM_FUNC(fread) 
    SYMBOL_ITEM_FUNC(_freopen_r) 
    SYMBOL_ITEM_FUNC(freopen) 
    SYMBOL_ITEM_FUNC(_fscanf_r) 
    SYMBOL_ITEM_FUNC(fscanf) 
    SYMBOL_ITEM_FUNC(_fseek_r) 
    SYMBOL_ITEM_FUNC(fseek) 
    SYMBOL_ITEM_FUNC(_fseeko_r) 
    SYMBOL_ITEM_FUNC(fseeko) 
    SYMBOL_ITEM_FUNC(_fsetpos_r) 
    SYMBOL_ITEM_FUNC(fsetpos) 
    SYMBOL_ITEM_FUNC(_fstat_r) 
    SYMBOL_ITEM_FUNC(_ftell_r) 
    SYMBOL_ITEM_FUNC(ftell) 
    SYMBOL_ITEM_FUNC(_ftello_r) 
    SYMBOL_ITEM_FUNC(ftello) 
    SYMBOL_ITEM_FUNC(_funopen_r) 
    SYMBOL_ITEM_FUNC(funopen) 
    SYMBOL_ITEM_FUNC(__sfvwrite_r) 
    SYMBOL_ITEM_FUNC(_fwalk) 
    SYMBOL_ITEM_FUNC(_fwalk_reent) 
    SYMBOL_ITEM_FUNC(_fwide_r) 
    SYMBOL_ITEM_FUNC(fwide) 
    SYMBOL_ITEM_FUNC(_fwprintf_r) 
    SYMBOL_ITEM_FUNC(fwprintf) 
    SYMBOL_ITEM_FUNC(_fwrite_r) 
    SYMBOL_ITEM_FUNC(fwrite) 
    SYMBOL_ITEM_FUNC(_fwscanf_r) 
    SYMBOL_ITEM_FUNC(fwscanf) 
    SYMBOL_ITEM_FUNC(__gethex) 
    SYMBOL_ITEM_FUNC(__hexdig_init) 
    SYMBOL_ITEM_FUNC(__hexnan) 
    SYMBOL_ITEM_FUNC(_getc_r) 
    SYMBOL_ITEM_FUNC(getc) 
    SYMBOL_ITEM_FUNC(_getc_unlocked_r) 
    SYMBOL_ITEM_FUNC(getc_unlocked) 
    SYMBOL_ITEM_FUNC(_getchar_r) 
    SYMBOL_ITEM_FUNC(getchar) 
    SYMBOL_ITEM_FUNC(_getchar_unlocked_r) 
    SYMBOL_ITEM_FUNC(getchar_unlocked) 
    SYMBOL_ITEM_FUNC(__getdelim) 
    SYMBOL_ITEM_FUNC(_findenv) 
    SYMBOL_ITEM_FUNC(getenv) 
    SYMBOL_ITEM_FUNC(_findenv_r) 
    SYMBOL_ITEM_FUNC(_getenv_r) 
    SYMBOL_ITEM_FUNC(__getline) 
    SYMBOL_ITEM_FUNC(__getopt_long_only_r) 
    SYMBOL_ITEM_FUNC(__getopt_long_r) 
    SYMBOL_ITEM_FUNC(__getopt_r) 
    SYMBOL_ITEM_FUNC(getopt) 
    SYMBOL_ITEM_FUNC(getopt_long) 
    SYMBOL_ITEM_FUNC(getopt_long_only) 
    SYMBOL_ITEM_FUNC(__getreent) 
    SYMBOL_ITEM_FUNC(_gets_r) 
    SYMBOL_ITEM_FUNC(gets) 
    SYMBOL_ITEM_FUNC(getsubopt) 
    SYMBOL_ITEM_FUNC(_gettimeofday_r) 
    SYMBOL_ITEM_FUNC(__gettzinfo) 
    SYMBOL_ITEM_FUNC(getw) 
    SYMBOL_ITEM_FUNC(_getwc_r) 
    SYMBOL_ITEM_FUNC(getwc) 
    SYMBOL_ITEM_FUNC(_getwchar_r) 
    SYMBOL_ITEM_FUNC(getwchar) 
    SYMBOL_ITEM_FUNC(gmtime) 
    SYMBOL_ITEM_FUNC(gmtime_r) 
    SYMBOL_ITEM_FUNC(__call_hash) 
    SYMBOL_ITEM_FUNC(__expand_table) 
    SYMBOL_ITEM_FUNC(__hash_open) 
    SYMBOL_ITEM_FUNC(__big_delete) 
    SYMBOL_ITEM_FUNC(__big_insert) 
    SYMBOL_ITEM_FUNC(__big_keydata) 
    SYMBOL_ITEM_FUNC(__big_return) 
    SYMBOL_ITEM_FUNC(__big_split) 
    SYMBOL_ITEM_FUNC(__find_bigpair) 
    SYMBOL_ITEM_FUNC(__find_last_page) 
    SYMBOL_ITEM_FUNC(__buf_free) 
    SYMBOL_ITEM_FUNC(__buf_init) 
    SYMBOL_ITEM_FUNC(__get_buf) 
    SYMBOL_ITEM_FUNC(__reclaim_buf) 
    SYMBOL_ITEM_FUNC(__log2) 
    SYMBOL_ITEM_FUNC(__add_ovflpage) 
    SYMBOL_ITEM_FUNC(__addel) 
    SYMBOL_ITEM_FUNC(__delpair) 
    SYMBOL_ITEM_FUNC(__free_ovflpage) 
    SYMBOL_ITEM_FUNC(__get_page) 
    SYMBOL_ITEM_FUNC(__ibitmap) 
    SYMBOL_ITEM_FUNC(__put_page) 
    SYMBOL_ITEM_FUNC(__split_page) 
    SYMBOL_ITEM_FUNC(hcreate) 
    SYMBOL_ITEM_FUNC(hdestroy) 
    SYMBOL_ITEM_FUNC(hsearch) 
    SYMBOL_ITEM_FUNC(hcreate_r) 
    SYMBOL_ITEM_FUNC(hdestroy_r) 
    SYMBOL_ITEM_FUNC(hsearch_r) 
    SYMBOL_ITEM_FUNC(index) 
    SYMBOL_ITEM_FUNC(__libc_init_array) 
    SYMBOL_ITEM_FUNC(_iprintf_r) 
    SYMBOL_ITEM_FUNC(iprintf) 
    SYMBOL_ITEM_FUNC(isalnum) 
    SYMBOL_ITEM_FUNC(isalpha) 
    SYMBOL_ITEM_FUNC(isascii) 
    SYMBOL_ITEM_FUNC(_isatty_r) 
    SYMBOL_ITEM_FUNC(isblank) 
    SYMBOL_ITEM_FUNC(_iscanf_r) 
    SYMBOL_ITEM_FUNC(iscanf) 
    SYMBOL_ITEM_FUNC(iscntrl) 
    SYMBOL_ITEM_FUNC(isdigit) 
    SYMBOL_ITEM_FUNC(islower) 
    SYMBOL_ITEM_FUNC(isgraph) 
    SYMBOL_ITEM_FUNC(isprint) 
    SYMBOL_ITEM_FUNC(ispunct) 
    SYMBOL_ITEM_FUNC(isspace) 
    SYMBOL_ITEM_FUNC(isupper) 
    SYMBOL_ITEM_FUNC(iswalnum) 
    SYMBOL_ITEM_FUNC(iswalpha) 
    SYMBOL_ITEM_FUNC(iswblank) 
    SYMBOL_ITEM_FUNC(iswcntrl) 
    SYMBOL_ITEM_FUNC(iswctype) 
    SYMBOL_ITEM_FUNC(iswdigit) 
    SYMBOL_ITEM_FUNC(iswgraph) 
    SYMBOL_ITEM_FUNC(iswlower) 
    SYMBOL_ITEM_FUNC(iswprint) 
    SYMBOL_ITEM_FUNC(iswpunct) 
    SYMBOL_ITEM_FUNC(iswspace) 
    SYMBOL_ITEM_FUNC(iswupper) 
    SYMBOL_ITEM_FUNC(iswxdigit) 
    SYMBOL_ITEM_FUNC(isxdigit) 
    SYMBOL_ITEM_FUNC(_jrand48_r) 
    SYMBOL_ITEM_FUNC(jrand48) 
    SYMBOL_ITEM_FUNC(_l64a_r) 
    SYMBOL_ITEM_FUNC(l64a) 
    SYMBOL_ITEM_FUNC(labs) 
    SYMBOL_ITEM_FUNC(localtime) 
    SYMBOL_ITEM_FUNC(localtime_r) 
    SYMBOL_ITEM_FUNC(_lcong48_r) 
    SYMBOL_ITEM_FUNC(lcong48) 
    SYMBOL_ITEM_FUNC(__ctype_load_locale) 
    SYMBOL_ITEM_FUNC(__get_current_ctype_locale) 
    SYMBOL_ITEM_FUNC(ldiv) 
    SYMBOL_ITEM_FUNC(__part_load_locale) 
    SYMBOL_ITEM_FUNC(_ldcheck) 
    SYMBOL_ITEM_FUNC(_ldtoa_r) 
    SYMBOL_ITEM_FUNC(_strtold) 
    SYMBOL_ITEM_FUNC(_link_r) 
    SYMBOL_ITEM_FUNC(llabs) 
    SYMBOL_ITEM_FUNC(lldiv) 
    SYMBOL_ITEM_FUNC(__get_current_messages_locale) 
    SYMBOL_ITEM_FUNC(__messages_load_locale) 
    SYMBOL_ITEM_FUNC(__get_current_monetary_locale) 
    SYMBOL_ITEM_FUNC(__monetary_load_locale) 
    SYMBOL_ITEM_FUNC(__get_current_numeric_locale) 
    SYMBOL_ITEM_FUNC(__numeric_load_locale) 
    SYMBOL_ITEM_FUNC(__locale_charset) 
    SYMBOL_ITEM_FUNC(__locale_cjk_lang) 
    SYMBOL_ITEM_FUNC(__locale_mb_cur_max) 
    SYMBOL_ITEM_FUNC(__locale_msgcharset) 
    SYMBOL_ITEM_FUNC(_localeconv_r) 
    SYMBOL_ITEM_FUNC(_setlocale_r) 
    SYMBOL_ITEM_FUNC(localeconv) 
    SYMBOL_ITEM_FUNC(setlocale) 
    SYMBOL_ITEM_FUNC(_lrand48_r) 
    SYMBOL_ITEM_FUNC(lrand48) 
    SYMBOL_ITEM_FUNC(_lseek_r) 
    SYMBOL_ITEM_FUNC(__smakebuf_r) 
    SYMBOL_ITEM_FUNC(memalign) 
    SYMBOL_ITEM_FUNC(_memalign_r) 
    SYMBOL_ITEM_FUNC(free) 
    SYMBOL_ITEM_FUNC(malloc) 
    SYMBOL_ITEM_FUNC(mblen) 
    SYMBOL_ITEM_FUNC(_mblen_r) 
    SYMBOL_ITEM_FUNC(mbrlen) 
    SYMBOL_ITEM_FUNC(_mbrtowc_r) 
    SYMBOL_ITEM_FUNC(mbrtowc) 
    SYMBOL_ITEM_FUNC(mbsinit) 
    SYMBOL_ITEM_FUNC(_mbsnrtowcs_r) 
    SYMBOL_ITEM_FUNC(mbsnrtowcs) 
    SYMBOL_ITEM_FUNC(_mbsrtowcs_r) 
    SYMBOL_ITEM_FUNC(mbsrtowcs) 
    SYMBOL_ITEM_FUNC(mbstowcs) 
    SYMBOL_ITEM_FUNC(_mbstowcs_r) 
    SYMBOL_ITEM_FUNC(mbtowc) 
    SYMBOL_ITEM_FUNC(__ascii_mbtowc) 
    SYMBOL_ITEM_FUNC(_mbtowc_r) 
    SYMBOL_ITEM_FUNC(memccpy) 
    SYMBOL_ITEM_FUNC(memchr) 
    SYMBOL_ITEM_FUNC(memcmp) 
    SYMBOL_ITEM_FUNC(memcpy) 
    SYMBOL_ITEM_FUNC(memmem) 
    SYMBOL_ITEM_FUNC(memmove) 
    SYMBOL_ITEM_FUNC(mempcpy) 
    SYMBOL_ITEM_FUNC(memset) 
    SYMBOL_ITEM_FUNC(_mkdir_r) 
    SYMBOL_ITEM_FUNC(_mkdtemp_r) 
    SYMBOL_ITEM_FUNC(_mkostemp_r) 
    SYMBOL_ITEM_FUNC(_mkostemps_r) 
    SYMBOL_ITEM_FUNC(_mkstemp_r) 
    SYMBOL_ITEM_FUNC(_mkstemps_r) 
    SYMBOL_ITEM_FUNC(_mktemp_r) 
    SYMBOL_ITEM_FUNC(mkdtemp) 
    SYMBOL_ITEM_FUNC(mkostemp) 
    SYMBOL_ITEM_FUNC(mkostemps) 
    SYMBOL_ITEM_FUNC(mkstemp) 
    SYMBOL_ITEM_FUNC(mkstemps) 
    SYMBOL_ITEM_FUNC(mktemp) 
    SYMBOL_ITEM_FUNC(mktime) 
    SYMBOL_ITEM_FUNC(__tzcalc_limits) 
    SYMBOL_ITEM_FUNC(_mktm_r) 
    SYMBOL_ITEM_FUNC(__malloc_lock) 
    SYMBOL_ITEM_FUNC(__malloc_unlock) 
    SYMBOL_ITEM_FUNC(_Balloc) 
    SYMBOL_ITEM_FUNC(_Bfree) 
    SYMBOL_ITEM_FUNC(__any_on) 
    SYMBOL_ITEM_FUNC(__b2d) 
    SYMBOL_ITEM_FUNC(__copybits) 
    SYMBOL_ITEM_FUNC(__d2b) 
    SYMBOL_ITEM_FUNC(__hi0bits) 
    SYMBOL_ITEM_FUNC(__i2b) 
    SYMBOL_ITEM_FUNC(__lo0bits) 
    SYMBOL_ITEM_FUNC(__lshift) 
    SYMBOL_ITEM_FUNC(__mcmp) 
    SYMBOL_ITEM_FUNC(__mdiff) 
    SYMBOL_ITEM_FUNC(__multadd) 
    SYMBOL_ITEM_FUNC(__multiply) 
    SYMBOL_ITEM_FUNC(__pow5mult) 
    SYMBOL_ITEM_FUNC(__ratio) 
    SYMBOL_ITEM_FUNC(__s2b) 
    SYMBOL_ITEM_FUNC(__ulp) 
    SYMBOL_ITEM_FUNC(_mprec_log10) 
    SYMBOL_ITEM_FUNC(_mrand48_r) 
    SYMBOL_ITEM_FUNC(mrand48) 
    SYMBOL_ITEM_FUNC(malloc_usable_size) 
    SYMBOL_ITEM_FUNC(_malloc_usable_size_r) 
    SYMBOL_ITEM_FUNC(nl_langinfo) 
    SYMBOL_ITEM_FUNC(_nrand48_r) 
    SYMBOL_ITEM_FUNC(nrand48) 
    SYMBOL_ITEM_FUNC(on_exit) 
    SYMBOL_ITEM_FUNC(_open_memstream_r) 
    SYMBOL_ITEM_FUNC(_open_wmemstream_r) 
    SYMBOL_ITEM_FUNC(open_memstream) 
    SYMBOL_ITEM_FUNC(open_wmemstream) 
    SYMBOL_ITEM_FUNC(_open_r) 
    SYMBOL_ITEM_FUNC(_perror_r) 
    SYMBOL_ITEM_FUNC(perror) 
    SYMBOL_ITEM_FUNC(_printf_r) 
    SYMBOL_ITEM_FUNC(printf) 
    SYMBOL_ITEM_FUNC(_putc_r) 
    SYMBOL_ITEM_FUNC(putc) 
    SYMBOL_ITEM_FUNC(_putc_unlocked_r) 
    SYMBOL_ITEM_FUNC(putc_unlocked) 
    SYMBOL_ITEM_FUNC(_putchar_r) 
    SYMBOL_ITEM_FUNC(putchar) 
    SYMBOL_ITEM_FUNC(_putchar_unlocked_r) 
    SYMBOL_ITEM_FUNC(putchar_unlocked) 
    SYMBOL_ITEM_FUNC(putenv) 
    SYMBOL_ITEM_FUNC(_putenv_r) 
    SYMBOL_ITEM_FUNC(_puts_r) 
    SYMBOL_ITEM_FUNC(puts) 
    SYMBOL_ITEM_FUNC(putw) 
    SYMBOL_ITEM_FUNC(_putwc_r) 
    SYMBOL_ITEM_FUNC(putwc) 
    SYMBOL_ITEM_FUNC(_putwchar_r) 
    SYMBOL_ITEM_FUNC(putwchar) 
    SYMBOL_ITEM_FUNC(_pvalloc_r) 
    SYMBOL_ITEM_FUNC(qsort) 
    SYMBOL_ITEM_FUNC(rand) 
    SYMBOL_ITEM_FUNC(srand) 
    SYMBOL_ITEM_FUNC(__dorand48) 
    SYMBOL_ITEM_FUNC(rand_r) 
    SYMBOL_ITEM_FUNC(_read_r) 
    SYMBOL_ITEM_FUNC(realloc) 
    SYMBOL_ITEM_FUNC(_reallocf_r) 
    SYMBOL_ITEM_FUNC(reallocf) 
    SYMBOL_ITEM_FUNC(_realloc_r) 
    SYMBOL_ITEM_FUNC(_reclaim_reent) 
    SYMBOL_ITEM_FUNC(_wrapup_reent) 
    SYMBOL_ITEM_FUNC(cleanup_glue) 
    SYMBOL_ITEM_FUNC(__srefill_r) 
    SYMBOL_ITEM_FUNC(_remove_r) 
    SYMBOL_ITEM_FUNC(remove) 
    SYMBOL_ITEM_FUNC(rename) 
    SYMBOL_ITEM_FUNC(_rename_r) 
    SYMBOL_ITEM_FUNC(_rewind_r) 
    SYMBOL_ITEM_FUNC(rewind) 
    SYMBOL_ITEM_FUNC(__srget) 
    SYMBOL_ITEM_FUNC(__srget_r) 
    SYMBOL_ITEM_FUNC(rindex) 
    SYMBOL_ITEM_FUNC(copysign) 
    SYMBOL_ITEM_FUNC(finite) 
    SYMBOL_ITEM_FUNC(__fpclassifyd) 
    SYMBOL_ITEM_FUNC(frexp) 
    SYMBOL_ITEM_FUNC(isinf) 
    SYMBOL_ITEM_FUNC(__isinfd) 
    SYMBOL_ITEM_FUNC(isnan) 
    SYMBOL_ITEM_FUNC(__isnand) 
    SYMBOL_ITEM_FUNC(ldexp) 
    SYMBOL_ITEM_FUNC(modf) 
    SYMBOL_ITEM_FUNC(nan) 
    SYMBOL_ITEM_FUNC(scalbn) 
    SYMBOL_ITEM_FUNC(_sbrk_r) 
    SYMBOL_ITEM_FUNC(_scanf_r) 
    SYMBOL_ITEM_FUNC(scanf) 
    SYMBOL_ITEM_FUNC(__sccl) 
    SYMBOL_ITEM_FUNC(_seed48_r) 
    SYMBOL_ITEM_FUNC(seed48) 
    SYMBOL_ITEM_FUNC(setbuf) 
    SYMBOL_ITEM_FUNC(setbuffer) 
    SYMBOL_ITEM_FUNC(setenv) 
    SYMBOL_ITEM_FUNC(unsetenv) 
    SYMBOL_ITEM_FUNC(_setenv_r) 
    SYMBOL_ITEM_FUNC(_unsetenv_r) 
    SYMBOL_ITEM_FUNC(longjmp) 
    SYMBOL_ITEM_FUNC(setjmp) 
    SYMBOL_ITEM_FUNC(setlinebuf) 
    SYMBOL_ITEM_FUNC(setvbuf) 
    SYMBOL_ITEM_FUNC(copysignf) 
    SYMBOL_ITEM_FUNC(finitef) 
    SYMBOL_ITEM_FUNC(__fpclassifyf) 
    SYMBOL_ITEM_FUNC(frexpf) 
    SYMBOL_ITEM_FUNC(isinff) 
    SYMBOL_ITEM_FUNC(__isinff) 
    SYMBOL_ITEM_FUNC(isnanf) 
    SYMBOL_ITEM_FUNC(__isnanf) 
    SYMBOL_ITEM_FUNC(ldexpf) 
    SYMBOL_ITEM_FUNC(modff) 
    SYMBOL_ITEM_FUNC(nanf) 
    SYMBOL_ITEM_FUNC(scalbnf) 
    SYMBOL_ITEM_FUNC(__signgam) 
    SYMBOL_ITEM_FUNC(_siprintf_r) 
    SYMBOL_ITEM_FUNC(siprintf) 
    SYMBOL_ITEM_FUNC(_siscanf_r) 
    SYMBOL_ITEM_FUNC(siscanf) 
    SYMBOL_ITEM_FUNC(_sniprintf_r) 
    SYMBOL_ITEM_FUNC(sniprintf) 
    SYMBOL_ITEM_FUNC(_snprintf_r) 
    SYMBOL_ITEM_FUNC(snprintf) 
    SYMBOL_ITEM_FUNC(_sprintf_r) 
    SYMBOL_ITEM_FUNC(sprintf) 
    SYMBOL_ITEM_FUNC(_srand48_r) 
    SYMBOL_ITEM_FUNC(srand48) 
    SYMBOL_ITEM_FUNC(_sscanf_r) 
    SYMBOL_ITEM_FUNC(sscanf) 
    SYMBOL_ITEM_FUNC(_stat_r) 
    SYMBOL_ITEM_FUNC(__sclose) 
    SYMBOL_ITEM_FUNC(__seofread) 
    SYMBOL_ITEM_FUNC(__sread) 
    SYMBOL_ITEM_FUNC(__sseek) 
    SYMBOL_ITEM_FUNC(__swrite) 
    SYMBOL_ITEM_FUNC(stpcpy) 
    SYMBOL_ITEM_FUNC(stpncpy) 
    SYMBOL_ITEM_FUNC(strcasecmp) 
    SYMBOL_ITEM_FUNC(strcasestr) 
    SYMBOL_ITEM_FUNC(strcat) 
    SYMBOL_ITEM_FUNC(strchr) 
    SYMBOL_ITEM_FUNC(strcmp) 
    SYMBOL_ITEM_FUNC(strcoll) 
    SYMBOL_ITEM_FUNC(strcpy) 
    SYMBOL_ITEM_FUNC(strcspn) 
    SYMBOL_ITEM_FUNC(strdup) 
    SYMBOL_ITEM_FUNC(_strdup_r) 
    SYMBOL_ITEM_FUNC(strerror) 
    SYMBOL_ITEM_FUNC(strerror_r) 
    SYMBOL_ITEM_FUNC(strftime) 
    SYMBOL_ITEM_FUNC(strlcat) 
    SYMBOL_ITEM_FUNC(strlcpy) 
    SYMBOL_ITEM_FUNC(strlen) 
    SYMBOL_ITEM_FUNC(strlwr) 
    SYMBOL_ITEM_FUNC(strncasecmp) 
    SYMBOL_ITEM_FUNC(strncat) 
    SYMBOL_ITEM_FUNC(strncmp) 
    SYMBOL_ITEM_FUNC(strncpy) 
    SYMBOL_ITEM_FUNC(strndup) 
    SYMBOL_ITEM_FUNC(_strndup_r) 
    SYMBOL_ITEM_FUNC(strnlen) 
    SYMBOL_ITEM_FUNC(strpbrk) 
    SYMBOL_ITEM_FUNC(strptime) 
    SYMBOL_ITEM_FUNC(strrchr) 
    SYMBOL_ITEM_FUNC(strsep) 
    SYMBOL_ITEM_FUNC(strsignal) 
    SYMBOL_ITEM_FUNC(strspn) 
    SYMBOL_ITEM_FUNC(strstr) 
    SYMBOL_ITEM_FUNC(_strtod_r) 
    SYMBOL_ITEM_FUNC(strtod) 
    SYMBOL_ITEM_FUNC(strtof) 
    SYMBOL_ITEM_FUNC(strtok) 
    SYMBOL_ITEM_FUNC(__strtok_r) 
    SYMBOL_ITEM_FUNC(strtok_r) 
    SYMBOL_ITEM_FUNC(_strtol_r) 
    SYMBOL_ITEM_FUNC(strtol) 
    SYMBOL_ITEM_FUNC(strtold) 
    SYMBOL_ITEM_FUNC(strtoll) 
    SYMBOL_ITEM_FUNC(_strtoll_r) 
    SYMBOL_ITEM_FUNC(_strtoul_r) 
    SYMBOL_ITEM_FUNC(strtoul) 
    SYMBOL_ITEM_FUNC(strtoull) 
    SYMBOL_ITEM_FUNC(_strtoull_r) 
    SYMBOL_ITEM_FUNC(strupr) 
    SYMBOL_ITEM_FUNC(strxfrm) 
    SYMBOL_ITEM_FUNC(__ssprint_r) 
    SYMBOL_ITEM_FUNC(_svfiprintf_r) 
    SYMBOL_ITEM_FUNC(__ssrefill_r) 
    SYMBOL_ITEM_FUNC(__ssvfiscanf_r) 
    SYMBOL_ITEM_FUNC(_sfread_r) 
    SYMBOL_ITEM_FUNC(_sungetc_r) 
    SYMBOL_ITEM_FUNC(_svfiwprintf_r) 
    SYMBOL_ITEM_FUNC(__ssvfiwscanf_r) 
    SYMBOL_ITEM_FUNC(_svfprintf_r) 
    SYMBOL_ITEM_FUNC(__ssvfscanf_r) 
    SYMBOL_ITEM_FUNC(_svfwprintf_r) 
    SYMBOL_ITEM_FUNC(__ssvfwscanf_r) 
    SYMBOL_ITEM_FUNC(swab) 
    SYMBOL_ITEM_FUNC(_swprintf_r) 
    SYMBOL_ITEM_FUNC(swprintf) 
    SYMBOL_ITEM_FUNC(_swscanf_r) 
    SYMBOL_ITEM_FUNC(swscanf) 
    SYMBOL_ITEM_FUNC(close) 
    SYMBOL_ITEM_FUNC(execve) 
    SYMBOL_ITEM_FUNC(fcntl) 
    SYMBOL_ITEM_FUNC(fork) 
    SYMBOL_ITEM_FUNC(fstat) 
    SYMBOL_ITEM_FUNC(getpid) 
    SYMBOL_ITEM_FUNC(gettimeofday) 
    SYMBOL_ITEM_FUNC(isatty) 
    SYMBOL_ITEM_FUNC(kill) 
    SYMBOL_ITEM_FUNC(link) 
    SYMBOL_ITEM_FUNC(lseek) 
    SYMBOL_ITEM_FUNC(open) 
    SYMBOL_ITEM_FUNC(read) 
    SYMBOL_ITEM_FUNC(sbrk) 
    SYMBOL_ITEM_FUNC(stat) 
    SYMBOL_ITEM_FUNC(_system_r) 
    SYMBOL_ITEM_FUNC(system) 
    SYMBOL_ITEM_FUNC(times) 
    SYMBOL_ITEM_FUNC(unlink) 
    SYMBOL_ITEM_FUNC(wait) 
    SYMBOL_ITEM_FUNC(write) 
    SYMBOL_ITEM_FUNC(tdelete) 
    SYMBOL_ITEM_FUNC(tdestroy) 
    SYMBOL_ITEM_FUNC(tfind) 
    SYMBOL_ITEM_FUNC(time) 
    SYMBOL_ITEM_FUNC(__get_current_time_locale) 
    SYMBOL_ITEM_FUNC(__time_load_locale) 
    SYMBOL_ITEM_FUNC(_times_r) 
    SYMBOL_ITEM_FUNC(_tmpfile_r) 
    SYMBOL_ITEM_FUNC(tmpfile) 
    SYMBOL_ITEM_FUNC(_tempnam_r) 
    SYMBOL_ITEM_FUNC(_tmpnam_r) 
    SYMBOL_ITEM_FUNC(tempnam) 
    SYMBOL_ITEM_FUNC(tmpnam) 
    SYMBOL_ITEM_FUNC(toascii) 
    SYMBOL_ITEM_FUNC(tolower) 
    SYMBOL_ITEM_FUNC(toupper) 
    SYMBOL_ITEM_FUNC(_towctrans_r) 
    SYMBOL_ITEM_FUNC(towctrans) 
    SYMBOL_ITEM_FUNC(towlower) 
    SYMBOL_ITEM_FUNC(towupper) 
    SYMBOL_ITEM_FUNC(tsearch) 
    SYMBOL_ITEM_FUNC(twalk) 
    SYMBOL_ITEM_FUNC(__tz_lock) 
    SYMBOL_ITEM_FUNC(__tz_unlock) 
    SYMBOL_ITEM_FUNC(tzset) 
    SYMBOL_ITEM_FUNC(_tzset_r) 
    SYMBOL_ITEM_FUNC(_user_strerror) 
    SYMBOL_ITEM_FUNC(__submore) 
    SYMBOL_ITEM_FUNC(_ungetc_r) 
    SYMBOL_ITEM_FUNC(ungetc) 
    SYMBOL_ITEM_FUNC(_ungetwc_r) 
    SYMBOL_ITEM_FUNC(ungetwc) 
    SYMBOL_ITEM_FUNC(_unlink_r) 
    SYMBOL_ITEM_FUNC(pvalloc) 
    SYMBOL_ITEM_FUNC(valloc) 
    SYMBOL_ITEM_FUNC(_valloc_r) 
    SYMBOL_ITEM_FUNC(_vasiprintf_r) 
    SYMBOL_ITEM_FUNC(vasiprintf) 
    SYMBOL_ITEM_FUNC(_vasniprintf_r) 
    SYMBOL_ITEM_FUNC(vasniprintf) 
    SYMBOL_ITEM_FUNC(_vasnprintf_r) 
    SYMBOL_ITEM_FUNC(vasnprintf) 
    SYMBOL_ITEM_FUNC(_vasprintf_r) 
    SYMBOL_ITEM_FUNC(vasprintf) 
    SYMBOL_ITEM_FUNC(_vdiprintf_r) 
    SYMBOL_ITEM_FUNC(vdiprintf) 
    SYMBOL_ITEM_FUNC(_vdprintf_r) 
    SYMBOL_ITEM_FUNC(vdprintf) 
    SYMBOL_ITEM_FUNC(__sprint_r) 
    SYMBOL_ITEM_FUNC(_vfiprintf_r) 
    SYMBOL_ITEM_FUNC(vfiprintf) 
    SYMBOL_ITEM_FUNC(__svfiscanf) 
    SYMBOL_ITEM_FUNC(__svfiscanf_r) 
    SYMBOL_ITEM_FUNC(_vfiscanf_r) 
    SYMBOL_ITEM_FUNC(vfiscanf) 
    SYMBOL_ITEM_FUNC(_vfiwprintf_r) 
    SYMBOL_ITEM_FUNC(vfiwprintf) 
    SYMBOL_ITEM_FUNC(__svfiwscanf) 
    SYMBOL_ITEM_FUNC(__svfiwscanf_r) 
    SYMBOL_ITEM_FUNC(_vfiwscanf_r) 
    SYMBOL_ITEM_FUNC(vfiwscanf) 
    SYMBOL_ITEM_FUNC(_vfprintf_r) 
    SYMBOL_ITEM_FUNC(vfprintf) 
    SYMBOL_ITEM_FUNC(__svfscanf) 
    SYMBOL_ITEM_FUNC(__svfscanf_r) 
    SYMBOL_ITEM_FUNC(_vfscanf_r) 
    SYMBOL_ITEM_FUNC(vfscanf) 
    SYMBOL_ITEM_FUNC(_vfwprintf_r) 
    SYMBOL_ITEM_FUNC(vfwprintf) 
    SYMBOL_ITEM_FUNC(__svfwscanf) 
    SYMBOL_ITEM_FUNC(__svfwscanf_r) 
    SYMBOL_ITEM_FUNC(_vfwscanf_r) 
    SYMBOL_ITEM_FUNC(vfwscanf) 
    SYMBOL_ITEM_FUNC(_viprintf_r) 
    SYMBOL_ITEM_FUNC(viprintf) 
    SYMBOL_ITEM_FUNC(_viscanf_r) 
    SYMBOL_ITEM_FUNC(viscanf) 
    SYMBOL_ITEM_FUNC(_vprintf_r) 
    SYMBOL_ITEM_FUNC(vprintf) 
    SYMBOL_ITEM_FUNC(_vscanf_r) 
    SYMBOL_ITEM_FUNC(vscanf) 
    SYMBOL_ITEM_FUNC(_vsiprintf_r) 
    SYMBOL_ITEM_FUNC(vsiprintf) 
    SYMBOL_ITEM_FUNC(_vsiscanf_r) 
    SYMBOL_ITEM_FUNC(vsiscanf) 
    SYMBOL_ITEM_FUNC(_vsniprintf_r) 
    SYMBOL_ITEM_FUNC(vsniprintf) 
    SYMBOL_ITEM_FUNC(_vsnprintf_r) 
    SYMBOL_ITEM_FUNC(vsnprintf) 
    SYMBOL_ITEM_FUNC(_vsprintf_r) 
    SYMBOL_ITEM_FUNC(vsprintf) 
    SYMBOL_ITEM_FUNC(_vsscanf_r) 
    SYMBOL_ITEM_FUNC(vsscanf) 
    SYMBOL_ITEM_FUNC(_vswprintf_r) 
    SYMBOL_ITEM_FUNC(vswprintf) 
    SYMBOL_ITEM_FUNC(_vswscanf_r) 
    SYMBOL_ITEM_FUNC(vswscanf) 
    SYMBOL_ITEM_FUNC(_vwprintf_r) 
    SYMBOL_ITEM_FUNC(vwprintf) 
    SYMBOL_ITEM_FUNC(_vwscanf_r) 
    SYMBOL_ITEM_FUNC(vwscanf) 
    SYMBOL_ITEM_FUNC(__swbuf) 
    SYMBOL_ITEM_FUNC(__swbuf_r) 
    SYMBOL_ITEM_FUNC(wcpcpy) 
    SYMBOL_ITEM_FUNC(wcpncpy) 
    SYMBOL_ITEM_FUNC(_wcrtomb_r) 
    SYMBOL_ITEM_FUNC(wcrtomb) 
    SYMBOL_ITEM_FUNC(wcscasecmp) 
    SYMBOL_ITEM_FUNC(wcscat) 
    SYMBOL_ITEM_FUNC(wcschr) 
    SYMBOL_ITEM_FUNC(wcscmp) 
    SYMBOL_ITEM_FUNC(wcscoll) 
    SYMBOL_ITEM_FUNC(wcscpy) 
    SYMBOL_ITEM_FUNC(wcscspn) 
    SYMBOL_ITEM_FUNC(_wcsdup_r) 
    SYMBOL_ITEM_FUNC(wcsdup) 
    SYMBOL_ITEM_FUNC(__ctloc) 
    SYMBOL_ITEM_FUNC(wcsftime) 
    SYMBOL_ITEM_FUNC(wcslcat) 
    SYMBOL_ITEM_FUNC(wcslcpy) 
    SYMBOL_ITEM_FUNC(wcslen) 
    SYMBOL_ITEM_FUNC(wcsncasecmp) 
    SYMBOL_ITEM_FUNC(wcsncat) 
    SYMBOL_ITEM_FUNC(wcsncmp) 
    SYMBOL_ITEM_FUNC(wcsncpy) 
    SYMBOL_ITEM_FUNC(wcsnlen) 
    SYMBOL_ITEM_FUNC(_wcsnrtombs_r) 
    SYMBOL_ITEM_FUNC(wcsnrtombs) 
    SYMBOL_ITEM_FUNC(wcspbrk) 
    SYMBOL_ITEM_FUNC(wcsrchr) 
    SYMBOL_ITEM_FUNC(_wcsrtombs_r) 
    SYMBOL_ITEM_FUNC(wcsrtombs) 
    SYMBOL_ITEM_FUNC(wcsspn) 
    SYMBOL_ITEM_FUNC(wcsstr) 
    SYMBOL_ITEM_FUNC(_wcstod_r) 
    SYMBOL_ITEM_FUNC(_wcstof_r) 
    SYMBOL_ITEM_FUNC(wcstod) 
    SYMBOL_ITEM_FUNC(wcstof) 
    SYMBOL_ITEM_FUNC(wcstok) 
    SYMBOL_ITEM_FUNC(_wcstol_r) 
    SYMBOL_ITEM_FUNC(wcstol) 
    SYMBOL_ITEM_FUNC(wcstold) 
    SYMBOL_ITEM_FUNC(wcstoll) 
    SYMBOL_ITEM_FUNC(_wcstoll_r) 
    SYMBOL_ITEM_FUNC(wcstombs) 
    SYMBOL_ITEM_FUNC(_wcstombs_r) 
    SYMBOL_ITEM_FUNC(_wcstoul_r) 
    SYMBOL_ITEM_FUNC(wcstoul) 
    SYMBOL_ITEM_FUNC(wcstoull) 
    SYMBOL_ITEM_FUNC(_wcstoull_r) 
    SYMBOL_ITEM_FUNC(wcswidth) 
    SYMBOL_ITEM_FUNC(wcsxfrm) 
    SYMBOL_ITEM_FUNC(wctob) 
    SYMBOL_ITEM_FUNC(wctomb) 
    SYMBOL_ITEM_FUNC(__ascii_wctomb) 
    SYMBOL_ITEM_FUNC(_wctomb_r) 
    SYMBOL_ITEM_FUNC(_wctrans_r) 
    SYMBOL_ITEM_FUNC(wctrans) 
    SYMBOL_ITEM_FUNC(_wctype_r) 
    SYMBOL_ITEM_FUNC(wctype) 
    SYMBOL_ITEM_FUNC(__wcwidth) 
    SYMBOL_ITEM_FUNC(wcwidth) 
    SYMBOL_ITEM_FUNC(wmemchr) 
    SYMBOL_ITEM_FUNC(wmemcmp) 
    SYMBOL_ITEM_FUNC(wmemcpy) 
    SYMBOL_ITEM_FUNC(wmemmove) 
    SYMBOL_ITEM_FUNC(wmemset) 
    SYMBOL_ITEM_FUNC(_wprintf_r) 
    SYMBOL_ITEM_FUNC(wprintf) 
    SYMBOL_ITEM_FUNC(_write_r) 
    SYMBOL_ITEM_FUNC(_wscanf_r) 
    SYMBOL_ITEM_FUNC(wscanf) 
    SYMBOL_ITEM_FUNC(__swsetup_r) 
    SYMBOL_ITEM_OBJ(BUTTON__DefaultProps) 
    SYMBOL_ITEM_OBJ(BUTTON__Skin) 
    SYMBOL_ITEM_OBJ(BUTTON__pSkinDefault) 
    SYMBOL_ITEM_OBJ(FRAMEWIN__DefaultProps) 
    SYMBOL_ITEM_OBJ(FRAMEWIN__Skin) 
    SYMBOL_ITEM_OBJ(FRAMEWIN__pSkinDefault) 
    SYMBOL_ITEM_OBJ(GRAPH__DefaultProps) 
    SYMBOL_ITEM_OBJ(GUI_CharLine_pfLog2Vis) 
    SYMBOL_ITEM_OBJ(GUI__Wrap_pfGetCharWrap) 
    SYMBOL_ITEM_OBJ(GUI__Wrap_pfGetWordWrap) 
    SYMBOL_ITEM_OBJ(GUI__pFontDefault) 
    SYMBOL_ITEM_OBJ(HEADER__DefaultBorderH) 
    SYMBOL_ITEM_OBJ(HEADER__DefaultProps) 
    SYMBOL_ITEM_OBJ(HEADER__Skin) 
    SYMBOL_ITEM_OBJ(HEADER__pDefaultCursor) 
    SYMBOL_ITEM_OBJ(HEADER__pSkinDefault) 
    SYMBOL_ITEM_OBJ(ICONVIEW__DefaultProps) 
    SYMBOL_ITEM_OBJ(ICONVIEW__pfDrawStreamedBitmap) 
    SYMBOL_ITEM_OBJ(LCD__pSetColorAPI) 
    SYMBOL_ITEM_OBJ(LCD_pAColorIndex) 
    SYMBOL_ITEM_OBJ(LCD_pBkColorIndex) 
    SYMBOL_ITEM_OBJ(LCD_pColorIndex) 
    SYMBOL_ITEM_OBJ(LISTVIEW_DefaultProps) 
    SYMBOL_ITEM_OBJ(PROGBAR__DefaultProps) 
    SYMBOL_ITEM_OBJ(PROGBAR__Skin) 
    SYMBOL_ITEM_OBJ(PROGBAR__pSkinDefault) 
    SYMBOL_ITEM_OBJ(SCROLLBAR__DefaultProps) 
    SYMBOL_ITEM_OBJ(SCROLLBAR__DefaultWidth) 
    SYMBOL_ITEM_OBJ(SCROLLBAR__Skin) 
    SYMBOL_ITEM_OBJ(SCROLLBAR__ThumbSizeMin) 
    SYMBOL_ITEM_OBJ(SCROLLBAR__pSkinDefault) 
    SYMBOL_ITEM_OBJ(SLIDER__DefaultProps) 
    SYMBOL_ITEM_OBJ(SLIDER__Skin) 
    SYMBOL_ITEM_OBJ(SLIDER__pSkinDefault) 
    SYMBOL_ITEM_OBJ(TEXT__DefaultProps) 
    SYMBOL_ITEM_OBJ(TREEVIEW__DefaultProps) 
    SYMBOL_ITEM_OBJ(_Props) 
    SYMBOL_ITEM_OBJ(__ctype_ptr__) 
    SYMBOL_ITEM_OBJ(__fdlib_version) 
    SYMBOL_ITEM_OBJ(__mb_cur_max) 
    SYMBOL_ITEM_OBJ(__mbtowc) 
    SYMBOL_ITEM_OBJ(__wctomb) 
    SYMBOL_ITEM_OBJ(_aWave) 
    SYMBOL_ITEM_OBJ(_data) 
    SYMBOL_ITEM_OBJ(_impure_ptr) 
    SYMBOL_ITEM_OBJ(_pEffectDefault) 
    SYMBOL_ITEM_OBJ(_pfDraw) 
    SYMBOL_ITEM_OBJ(_tzname) 
    SYMBOL_ITEM_OBJ(devfs) 
    SYMBOL_ITEM_OBJ(environ) 
    SYMBOL_ITEM_OBJ(fatfs) 
    SYMBOL_ITEM_OBJ(fb_drv) 
    SYMBOL_ITEM_OBJ(nand_curr_device) 
    SYMBOL_ITEM_OBJ(nand_flash_ids) 
    SYMBOL_ITEM_OBJ(nand_manuf_ids) 
    SYMBOL_ITEM_OBJ(rootfs) 
    SYMBOL_ITEM_OBJ(socket_drv) 
    SYMBOL_ITEM_OBJ(sys_do_table) 
    SYMBOL_ITEM_OBJ(yaffs1) 
    SYMBOL_ITEM_OBJ(yaffs2) 
    SYMBOL_ITEM_OBJ(yaffs_checkptrw_c_version) 
    SYMBOL_ITEM_OBJ(yaffs_ecc_c_version) 
    SYMBOL_ITEM_OBJ(yaffs_guts_c_version) 
    SYMBOL_ITEM_OBJ(yaffs_mtdif1_c_version) 
    SYMBOL_ITEM_OBJ(yaffs_mtdif2_c_version) 
    SYMBOL_ITEM_OBJ(yaffs_mtdif_c_version) 
    SYMBOL_ITEM_OBJ(yaffs_nand_c_version) 
    SYMBOL_ITEM_OBJ(yaffs_traceMask) 
    SYMBOL_ITEM_OBJ(yaffsfs_c_version) 
    SYMBOL_ITEM_OBJ(GUITASK__EntranceCnt) 
    SYMBOL_ITEM_OBJ(GUI_ALLOC__Context) 
    SYMBOL_ITEM_OBJ(GUI_Context) 
    SYMBOL_ITEM_OBJ(GUI_DecChar) 
    SYMBOL_ITEM_OBJ(GUI_MEMDEV__pCbAnimation) 
    SYMBOL_ITEM_OBJ(GUI_MEMDEV__pVoid) 
    SYMBOL_ITEM_OBJ(GUI_OrgX) 
    SYMBOL_ITEM_OBJ(GUI_OrgY) 
    SYMBOL_ITEM_OBJ(GUI__CharHasTrans) 
    SYMBOL_ITEM_OBJ(GUI_pLCD_APIList) 
    SYMBOL_ITEM_OBJ(GUI_pUC_API) 
    SYMBOL_ITEM_OBJ(GUI_pfDispCharStyle) 
    SYMBOL_ITEM_OBJ(GUI_pfTimerExec) 
    SYMBOL_ITEM_OBJ(GUI_sError) 
    SYMBOL_ITEM_OBJ(HEADER__DefaultBorderV) 
    SYMBOL_ITEM_OBJ(LCD__aConvTable) 
    SYMBOL_ITEM_OBJ(WM_MULTIBUF__pAPI) 
    SYMBOL_ITEM_OBJ(WM__CaptureReleaseAuto) 
    SYMBOL_ITEM_OBJ(WM__CreateFlags) 
    SYMBOL_ITEM_OBJ(WM__FirstWin) 
    SYMBOL_ITEM_OBJ(WM__NumInvalidWindows) 
    SYMBOL_ITEM_OBJ(WM__NumWindows) 
    SYMBOL_ITEM_OBJ(WM__PaintCallbackCnt) 
    SYMBOL_ITEM_OBJ(WM__TouchedLayer) 
    SYMBOL_ITEM_OBJ(WM__TransWindowCnt) 
    SYMBOL_ITEM_OBJ(WM__aBkColor) 
    SYMBOL_ITEM_OBJ(WM__aCHWinLast) 
    SYMBOL_ITEM_OBJ(WM__aCHWinModal) 
    SYMBOL_ITEM_OBJ(WM__aCHWinMouseOver) 
    SYMBOL_ITEM_OBJ(WM__ahCapture) 
    SYMBOL_ITEM_OBJ(WM__ahDesktopWin) 
    SYMBOL_ITEM_OBJ(WM__ahWinFocus) 
    SYMBOL_ITEM_OBJ(WM__hATransWindow) 
    SYMBOL_ITEM_OBJ(WM__pFirstCriticalHandle) 
    SYMBOL_ITEM_OBJ(WM_pfHandlePID) 
    SYMBOL_ITEM_OBJ(WM_pfPollPID) 
    SYMBOL_ITEM_OBJ(_PathLocale) 
    SYMBOL_ITEM_OBJ(__bss_start) 
    SYMBOL_ITEM_OBJ(__bss_start__) 
    SYMBOL_ITEM_OBJ(__hexdig) 
    SYMBOL_ITEM_OBJ(__mlocale_changed) 
    SYMBOL_ITEM_OBJ(__nlocale_changed) 
    SYMBOL_ITEM_OBJ(_daylight) 
    SYMBOL_ITEM_OBJ(_hScaleH) 
    SYMBOL_ITEM_OBJ(_hScaleV) 
    SYMBOL_ITEM_OBJ(_timezone) 
    SYMBOL_ITEM_OBJ(current) 
    SYMBOL_ITEM_OBJ(current_header) 
    SYMBOL_ITEM_OBJ(current_iphdr_dest) 
    SYMBOL_ITEM_OBJ(current_iphdr_src) 
    SYMBOL_ITEM_OBJ(current_netif) 
    SYMBOL_ITEM_OBJ(dev_mgr_lock) 
    SYMBOL_ITEM_OBJ(fs_mgr_lock) 
    SYMBOL_ITEM_OBJ(interrupt_nest) 
    SYMBOL_ITEM_OBJ(lock_tcpip_core) 
    SYMBOL_ITEM_OBJ(lwip_stats) 
    SYMBOL_ITEM_OBJ(mtd_table) 
    SYMBOL_ITEM_OBJ(nand_info) 
    SYMBOL_ITEM_OBJ(netif_default) 
    SYMBOL_ITEM_OBJ(netif_list) 
    SYMBOL_ITEM_OBJ(pbuf_free_ooseq_pending) 
    SYMBOL_ITEM_OBJ(point_mgr_lock) 
    SYMBOL_ITEM_OBJ(ram_heap) 
    SYMBOL_ITEM_OBJ(reents) 
    SYMBOL_ITEM_OBJ(rootfs_point) 
    SYMBOL_ITEM_OBJ(running) 
    SYMBOL_ITEM_OBJ(symbol_tbl) 
    SYMBOL_ITEM_OBJ(tasks) 
    SYMBOL_ITEM_OBJ(tcp_active_pcbs) 
    SYMBOL_ITEM_OBJ(tcp_active_pcbs_changed) 
    SYMBOL_ITEM_OBJ(tcp_bound_pcbs) 
    SYMBOL_ITEM_OBJ(tcp_input_pcb) 
    SYMBOL_ITEM_OBJ(tcp_listen_pcbs) 
    SYMBOL_ITEM_OBJ(tcp_ticks) 
    SYMBOL_ITEM_OBJ(tcp_tmp_pcb) 
    SYMBOL_ITEM_OBJ(tcp_tw_pcbs) 
    SYMBOL_ITEM_OBJ(ticks) 
    SYMBOL_ITEM_OBJ(udp_pcbs) 
    SYMBOL_ITEM_OBJ(yaffs_wr_attempts) 
    SYMBOL_ITEM_OBJ(__aeabi_idiv0) 
    SYMBOL_ITEM_OBJ(__aeabi_ldiv0) 
    SYMBOL_ITEM_OBJ(__aeabi_unwind_cpp_pr1) 
    SYMBOL_ITEM_OBJ(__aeabi_unwind_cpp_pr2) 
    SYMBOL_ITEM_OBJ(__CTOR_LIST__) 
    SYMBOL_ITEM_OBJ(__DTOR_LIST__) 
    SYMBOL_ITEM_OBJ(__clz_tab) 
    SYMBOL_ITEM_OBJ(__popcount_tab) 
    SYMBOL_ITEM_OBJ(__aeabi_idiv0) 
    SYMBOL_ITEM_OBJ(__aeabi_ldiv0) 
    SYMBOL_ITEM_OBJ(__aeabi_unwind_cpp_pr1) 
    SYMBOL_ITEM_OBJ(__aeabi_unwind_cpp_pr2) 
    SYMBOL_ITEM_OBJ(__fdlib_version) 
    SYMBOL_ITEM_OBJ(__infinity) 
    SYMBOL_ITEM_OBJ(__infinityf) 
    SYMBOL_ITEM_OBJ(__infinityld) 
    SYMBOL_ITEM_OBJ(__ctype_ptr__) 
    SYMBOL_ITEM_OBJ(environ) 
    SYMBOL_ITEM_OBJ(opterr) 
    SYMBOL_ITEM_OBJ(optopt) 
    SYMBOL_ITEM_OBJ(__default_hash) 
    SYMBOL_ITEM_OBJ(_impure_ptr) 
    SYMBOL_ITEM_OBJ(__mb_cur_max) 
    SYMBOL_ITEM_OBJ(__mbtowc) 
    SYMBOL_ITEM_OBJ(_tzname) 
    SYMBOL_ITEM_OBJ(__wctomb) 
    SYMBOL_ITEM_OBJ(optarg) 
    SYMBOL_ITEM_OBJ(optind) 
    SYMBOL_ITEM_OBJ(_PathLocale) 
    SYMBOL_ITEM_OBJ(__mlocale_changed) 
    SYMBOL_ITEM_OBJ(__nlocale_changed) 
    SYMBOL_ITEM_OBJ(_daylight) 
    SYMBOL_ITEM_OBJ(_timezone) 
    SYMBOL_ITEM_OBJ(_ctype_) 
    SYMBOL_ITEM_OBJ(_global_impure_ptr) 
    SYMBOL_ITEM_OBJ(__mprec_bigtens) 
    SYMBOL_ITEM_OBJ(__mprec_tens) 
    SYMBOL_ITEM_OBJ(__mprec_tinytens) 
    SYMBOL_ITEM_OBJ(__infinity) 
    SYMBOL_ITEM_OBJ(__infinityf) 
    SYMBOL_ITEM_OBJ(__infinityld) 
    SYMBOL_ITEM_OBJ(tm_year_base) 
    SYMBOL_ITEM_OBJ(__unctrl) 
    SYMBOL_ITEM_OBJ(__unctrllen) 
    SYMBOL_ITEM_OBJ(__hexdig) 
    SYMBOL_ITEM_OBJ(suboptarg) 
    SYMBOL_ITEM_OBJ(errno) 
SYMBOL_TABLE_END												
/*********************************************************************************************************	
** END FILE													
*********************************************************************************************************/	
