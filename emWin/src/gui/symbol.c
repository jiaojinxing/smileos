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
** 文件创建日期: 2013 年 02 月 04 日						
**														
** 描        述: emWin 图形库符号表. (此文件由 MakeSymbol 工具自动生成, 请勿修改)			
*********************************************************************************************************/	
#include "kern/func_config.h"                                          
#if CONFIG_MODULE_EN 
#include "module/symbol_tool.h"										
														
#define SYMBOL_TABLE_BEGIN symbol_t gui_symbol_tbl[] = { 						
  														
#define SYMBOL_TABLE_END   {0, 0, 0} };									
														
#define SYMBOL_ITEM_FUNC(pcName)                \
    {   #pcName, (void *)pcName,                \
        SYMBOL_TEXT                             \
    },													
														
#define SYMBOL_ITEM_OBJ(pcName)                 \
    {   #pcName, (void *)&pcName,               \
        SYMBOL_DATA                             \
    },													
														
/*********************************************************************************************************	
** 全局对象声明												
*********************************************************************************************************/	
extern int  WM__UpdateChildPositions(); 
extern int  WM__SendMessageNoPara(); 
extern int  WM__SendMessageIfEnabled(); 
extern int  WM__SendMessage(); 
extern int  WM__Screen2Client(); 
extern int  WM__NotifyVisChanged(); 
extern int  WM__IsEnabled(); 
extern int  WM__IsChild(); 
extern int  WM__IsAncestor(); 
extern int  WM__IsAncestorOrSelf(); 
extern int  WM__GetPrevSibling(); 
extern int  WM__GetOrgX_AA(); 
extern int  WM__GetOrgY_AA(); 
extern int  WM__GetLastSibling(); 
extern int  WM__GetFocussedChild(); 
extern int  WM__GetFirstSibling(); 
extern int  WM__ForEachDesc(); 
extern int  WM_ValidateWindow(); 
extern int  WM_ValidateRect(); 
extern int  WM_GetUserData(); 
extern int  WM_SetUserData(); 
extern int  WM__GetUserDataEx(); 
extern int  WM__SetUserDataEx(); 
extern int  WM_UpdateWindowAndDescs(); 
extern int  WM_HandlePID(); 
extern int  WM__IsInModalArea(); 
extern int  WM__SendPIDMessage(); 
extern int  WM__SendTouchMessage(); 
extern int  WM__SetLastTouched(); 
extern int  WM__SetMotionCallback(); 
extern int  WM__SetToolTipCallback(); 
extern int  WM_TOOLTIP_AddTool(); 
extern int  WM_TOOLTIP_Create(); 
extern int  WM_TOOLTIP_Delete(); 
extern int  WM_TOOLTIP_SetDefaultColor(); 
extern int  WM_TOOLTIP_SetDefaultFont(); 
extern int  WM_TOOLTIP_SetDefaultPeriod(); 
extern int  WM_CreateTimer(); 
extern int  WM_DeleteTimer(); 
extern int  WM_GetTimerId(); 
extern int  WM_RestartTimer(); 
extern int  WM__DeleteAssocTimer(); 
extern int  WM_GetStayOnTop(); 
extern int  WM_SetStayOnTop(); 
extern int  WM_SetScreenSize(); 
extern int  WM_InvalidateWindowAndDescs(); 
extern int  WM_ShowWindow(); 
extern int  WM_SetYSize(); 
extern int  WM_SetXSize(); 
extern int  WM_SetWindowPos(); 
extern int  WM_SetUserClipRect(); 
extern int  WM_SetTransState(); 
extern int  WM_ClrHasTrans(); 
extern int  WM_GetHasTrans(); 
extern int  WM_SetHasTrans(); 
extern int  WM_SetSize(); 
extern int  WM_SetScrollState(); 
extern int  WM_SetScrollPosH(); 
extern int  WM_SetScrollPosV(); 
extern int  WM_SetScrollbarH(); 
extern int  WM_SetScrollbarV(); 
extern int  WM__SetScrollbarH(); 
extern int  WM__SetScrollbarV(); 
extern int  WM_SetpfPollPID(); 
extern int  WM_SetId(); 
extern int  WM_SetFocusOnPrevChild(); 
extern int  WM_SetFocusOnNextChild(); 
extern int  WM_SetFocus(); 
extern int  WM_SetDesktopColor(); 
extern int  WM_SetDesktopColorEx(); 
extern int  WM_SetDesktopColors(); 
extern int  WM_SetCreateFlags(); 
extern int  WM_SetCaptureMove(); 
extern int  WM_ReleaseCapture(); 
extern int  WM_SetCapture(); 
extern int  WM_SetCallback(); 
extern int  WM_SetAnchor(); 
extern int  WM_SendToParent(); 
extern int  WM_SendMessageNoPara(); 
extern int  WM_Screen2hWin(); 
extern int  WM_Screen2hWinEx(); 
extern int  WM__IsInWindow(); 
extern int  WM_ResizeWindow(); 
extern int  WM_PID__GetPrevState(); 
extern int  WM_PID__SetPrevState(); 
extern int  WM_PaintWindowAndDescs(); 
extern int  WM_Paint(); 
extern int  WM_Update(); 
extern int  WM_OnKey(); 
extern int  WM_NotifyParent(); 
extern int  WM_MULTIBUF_Enable(); 
extern int  WM_MoveChildTo(); 
extern int  WM_MoveTo(); 
extern int  WM_MoveWindow(); 
extern int  WM__MoveTo(); 
extern int  WM__MoveWindow(); 
extern int  WM_MOTION_Enable(); 
extern int  WM_MOTION_SetDeceleration(); 
extern int  WM_MOTION_SetDefaultPeriod(); 
extern int  WM_MOTION_SetMotion(); 
extern int  WM_MOTION_SetMoveable(); 
extern int  WM_MOTION_SetMovement(); 
extern int  WM_MOTION_SetSpeed(); 
extern int  WM_DisableMemdev(); 
extern int  WM_EnableMemdev(); 
extern int  WM_MakeModal(); 
extern int  WM_IsWindow(); 
extern int  WM_IsVisible(); 
extern int  WM_IsFocussable(); 
extern int  WM_IsEnabled(); 
extern int  WM_IsCompletelyVisible(); 
extern int  WM_IsCompletelyCovered(); 
extern int  WM_InvalidateArea(); 
extern int  WM_HideWindow(); 
extern int  WM_HasFocus(); 
extern int  WM_HasCaptured(); 
extern int  WM_GetWindowSizeX(); 
extern int  WM_GetWindowSizeY(); 
extern int  WM__GetWindowSizeX(); 
extern int  WM__GetWindowSizeY(); 
extern int  WM_GetWindowRect(); 
extern int  WM_GetWindowRectEx(); 
extern int  WM_GetScrollState(); 
extern int  WM_GetScrollPosH(); 
extern int  WM_GetScrollPosV(); 
extern int  WM_GetScrollPartner(); 
extern int  WM_GetScrollbarH(); 
extern int  WM_GetScrollbarV(); 
extern int  WM_GetPrevSibling(); 
extern int  WM_GetParent(); 
extern int  WM_GetOrgX(); 
extern int  WM_GetOrgY(); 
extern int  WM_GetWindowOrgX(); 
extern int  WM_GetWindowOrgY(); 
extern int  WM_GetNextSibling(); 
extern int  WM_GetInvalidRect(); 
extern int  WM_GetInsideRectExScrollbar(); 
extern int  WM_GetInsideRect(); 
extern int  WM_GetInsideRectEx(); 
extern int  WM_GetId(); 
extern int  WM_GetFocussedWindow(); 
extern int  WM_GetFlags(); 
extern int  WM_GetFirstChild(); 
extern int  WM_GetDialogItem(); 
extern int  WM_GetNumInvalidWindows(); 
extern int  WM_GetNumWindows(); 
extern int  WM_GetDesktopWindowEx(); 
extern int  WM_GetDesktopWindow(); 
extern int  WM_GetClientWindow(); 
extern int  WM_GetClientRect(); 
extern int  WM_GetClientRectEx(); 
extern int  WM__GetClientRectEx(); 
extern int  WM_GetCallback(); 
extern int  WM_GetBkColor(); 
extern int  WM_ForEachDesc(); 
extern int  WM_DisableWindow(); 
extern int  WM_EnableWindow(); 
extern int  WM_SetEnableState(); 
extern int  WM_DIAG_C(); 
extern int  WM__AddCriticalHandle(); 
extern int  WM__RemoveCriticalHandle(); 
extern int  WM_CheckScrollBounds(); 
extern int  WM_CheckScrollPos(); 
extern int  WM_SetScrollValue(); 
extern int  WM_BroadcastMessage(); 
extern int  WM_BringToTop(); 
extern int  WM_BringToBottom(); 
extern int  WM_AttachWindow(); 
extern int  WM_AttachWindowAt(); 
extern int  WM_DetachWindow(); 
extern int  WM_Activate(); 
extern int  WM_CreateWindow(); 
extern int  WM_CreateWindowAsChild(); 
extern int  WM_Deactivate(); 
extern int  WM_DefaultProc(); 
extern int  WM_DeleteWindow(); 
extern int  WM_Exec(); 
extern int  WM_GetActiveWindow(); 
extern int  WM_Init(); 
extern int  WM_InvalidateRect(); 
extern int  WM_InvalidateWindow(); 
extern int  WM_InvalidateWindowAndDescsEx(); 
extern int  WM_SelectWindow(); 
extern int  WM_SendMessage(); 
extern int  WM_SetDefault(); 
extern int  WM__ActivateClipRect(); 
extern int  WM__Client2Screen(); 
extern int  WM__ClipAtParentBorders(); 
extern int  WM__DeleteSecure(); 
extern int  WM__DetachWindow(); 
extern int  WM__GetClientRectWin(); 
extern int  WM__GetNextIVR(); 
extern int  WM__GetTopLevelLayer(); 
extern int  WM__InitIVRSearch(); 
extern int  WM__InsertWindowIntoList(); 
extern int  WM__Invalidate1Abs(); 
extern int  WM__InvalidateDrawAndDescs(); 
extern int  WM__InvalidateParent(); 
extern int  WM__InvalidateRect(); 
extern int  WM__InvalidateRectEx(); 
extern int  WM__IsWindow(); 
extern int  WM__Paint(); 
extern int  WM__Paint1(); 
extern int  WM__RectIsNZ(); 
extern int  WM__RemoveFromLinList(); 
extern int  WM__RemoveWindowFromList(); 
extern int  WM__SelectTopLevelLayer(); 
extern int  WM__SendMsgNoData(); 
extern int  WINDOW_GetDefaultBkColor(); 
extern int  WINDOW_SetDefaultBkColor(); 
extern int  WINDOW_Callback(); 
extern int  WINDOW_CreateEx(); 
extern int  WINDOW_CreateIndirect(); 
extern int  WINDOW_CreateUser(); 
extern int  WINDOW_GetUserData(); 
extern int  WINDOW_SetBkColor(); 
extern int  WINDOW_SetUserData(); 
extern int  WIDGET_SetWidth(); 
extern int  WIDGET_SetEffect(); 
extern int  WIDGET__FillStringInRect(); 
extern int  WIDGET_EFFECT_Simple_GetColor(); 
extern int  WIDGET_EFFECT_Simple_GetNumColors(); 
extern int  WIDGET_EFFECT_Simple_SetColor(); 
extern int  WIDGET_EFFECT_3D2L_GetColor(); 
extern int  WIDGET_EFFECT_3D2L_GetNumColors(); 
extern int  WIDGET_EFFECT_3D2L_SetColor(); 
extern int  WIDGET_EFFECT_3D1L_GetColor(); 
extern int  WIDGET_EFFECT_3D1L_GetNumColors(); 
extern int  WIDGET_EFFECT_3D1L_SetColor(); 
extern int  WIDGET_EFFECT_3D_DrawUp(); 
extern int  WIDGET_EFFECT_3D_GetColor(); 
extern int  WIDGET_EFFECT_3D_GetNumColors(); 
extern int  WIDGET_EFFECT_3D_SetColor(); 
extern int  WIDGET_AndState(); 
extern int  WIDGET_GetDefaultEffect(); 
extern int  WIDGET_GetState(); 
extern int  WIDGET_HandleActive(); 
extern int  WIDGET_OrState(); 
extern int  WIDGET_SetDefaultEffect(); 
extern int  WIDGET_SetState(); 
extern int  WIDGET__DrawFocusRect(); 
extern int  WIDGET__DrawHLine(); 
extern int  WIDGET__DrawTriangle(); 
extern int  WIDGET__DrawVLine(); 
extern int  WIDGET__EFFECT_DrawDown(); 
extern int  WIDGET__EFFECT_DrawDownRect(); 
extern int  WIDGET__EFFECT_DrawUpRect(); 
extern int  WIDGET__FillRectEx(); 
extern int  WIDGET__GetBkColor(); 
extern int  WIDGET__GetClientRect(); 
extern int  WIDGET__GetInsideRect(); 
extern int  WIDGET__GetWindowSizeX(); 
extern int  WIDGET__GetXSize(); 
extern int  WIDGET__GetYSize(); 
extern int  WIDGET__Init(); 
extern int  WIDGET__RotateRect90(); 
extern int  WIDGET__SetScrollState(); 
extern int  TREEVIEW_GetDefaultBkColor(); 
extern int  TREEVIEW_GetDefaultFont(); 
extern int  TREEVIEW_GetDefaultLineColor(); 
extern int  TREEVIEW_GetDefaultTextColor(); 
extern int  TREEVIEW_SetDefaultBkColor(); 
extern int  TREEVIEW_SetDefaultFont(); 
extern int  TREEVIEW_SetDefaultLineColor(); 
extern int  TREEVIEW_SetDefaultTextColor(); 
extern int  TREEVIEW_CreateIndirect(); 
extern int  TREEVIEW_AttachItem(); 
extern int  TREEVIEW_Callback(); 
extern int  TREEVIEW_CreateEx(); 
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
extern int  TEXT_GetNumLines(); 
extern int  TEXT_SetWrapMode(); 
extern int  TEXT_SetTextColor(); 
extern int  TEXT_SetTextAlign(); 
extern int  TEXT_SetText(); 
extern int  TEXT_SetFont(); 
extern int  TEXT_SetBkColor(); 
extern int  TEXT_GetText(); 
extern int  TEXT_GetDefaultFont(); 
extern int  TEXT_SetDefaultFont(); 
extern int  TEXT_SetDefaultTextColor(); 
extern int  TEXT_SetDefaultWrapMode(); 
extern int  TEXT_CreateIndirect(); 
extern int  TEXT_Create(); 
extern int  TEXT_CreateAsChild(); 
extern int  TEXT_Callback(); 
extern int  TEXT_CreateEx(); 
extern int  TEXT_CreateUser(); 
extern int  TEXT_GetUserData(); 
extern int  TEXT_SetUserData(); 
extern int  SPINBOX_DrawSkinFlex(); 
extern int  SPINBOX_GetSkinFlexProps(); 
extern int  SPINBOX_SetSkinFlexProps(); 
extern int  SPINBOX_SetDefaultSkinClassic(); 
extern int  SPINBOX_SetSkinClassic(); 
extern int  SPINBOX_SetDefaultSkin(); 
extern int  SPINBOX_SetSkin(); 
extern int  SPINBOX__ApplyProps(); 
extern int  SPINBOX_GetDefaultButtonSize(); 
extern int  SPINBOX_SetDefaultButtonSize(); 
extern int  SPINBOX_CreateIndirect(); 
extern int  SPINBOX_Callback(); 
extern int  SPINBOX_CreateEx(); 
extern int  SPINBOX_CreateUser(); 
extern int  SPINBOX_EnableBlink(); 
extern int  SPINBOX_GetBkColor(); 
extern int  SPINBOX_GetButtonBkColor(); 
extern int  SPINBOX_GetEditHandle(); 
extern int  SPINBOX_GetUserData(); 
extern int  SPINBOX_GetValue(); 
extern int  SPINBOX_SetBkColor(); 
extern int  SPINBOX_SetButtonBkColor(); 
extern int  SPINBOX_SetButtonSize(); 
extern int  SPINBOX_SetEdge(); 
extern int  SPINBOX_SetFont(); 
extern int  SPINBOX_SetRange(); 
extern int  SPINBOX_SetTextColor(); 
extern int  SPINBOX_SetUserData(); 
extern int  SPINBOX_SetValue(); 
extern int  SPINBOX__DrawBk(); 
extern int  SPINBOX__GetButtonRect(); 
extern int  SPINBOX__GetButtonRectEx(); 
extern int  SPINBOX__GetDefaultMax(); 
extern int  SPINBOX__GetDefaultMin(); 
extern int  SLIDER_DrawSkinFlex(); 
extern int  SLIDER_GetSkinFlexProps(); 
extern int  SLIDER_SetSkinFlexProps(); 
extern int  SLIDER_SetDefaultSkinClassic(); 
extern int  SLIDER_SetSkinClassic(); 
extern int  SLIDER_SetDefaultSkin(); 
extern int  SLIDER_SetSkin(); 
extern int  SLIDER_GetDefaultBarColor(); 
extern int  SLIDER_GetDefaultBkColor(); 
extern int  SLIDER_GetDefaultFocusColor(); 
extern int  SLIDER_GetDefaultTickColor(); 
extern int  SLIDER_SetDefaultBarColor(); 
extern int  SLIDER_SetDefaultBkColor(); 
extern int  SLIDER_SetDefaultFocusColor(); 
extern int  SLIDER_SetDefaultTickColor(); 
extern int  SLIDER_CreateIndirect(); 
extern int  SLIDER_Create(); 
extern int  SLIDER_GetBarColor(); 
extern int  SLIDER_GetBkColor(); 
extern int  SLIDER_GetFocusColor(); 
extern int  SLIDER_GetTickColor(); 
extern int  SLIDER_SetBarColor(); 
extern int  SLIDER_SetBkColor(); 
extern int  SLIDER_SetFocusColor(); 
extern int  SLIDER_SetTickColor(); 
extern int  SLIDER_Callback(); 
extern int  SLIDER_CreateEx(); 
extern int  SLIDER_CreateUser(); 
extern int  SLIDER_Dec(); 
extern int  SLIDER_GetUserData(); 
extern int  SLIDER_GetValue(); 
extern int  SLIDER_Inc(); 
extern int  SLIDER_SetNumTicks(); 
extern int  SLIDER_SetRange(); 
extern int  SLIDER_SetUserData(); 
extern int  SLIDER_SetValue(); 
extern int  SLIDER_SetWidth(); 
extern int  SCROLLBAR_DrawSkinFlex(); 
extern int  SCROLLBAR_GetSkinFlexProps(); 
extern int  SCROLLBAR_SetSkinFlexProps(); 
extern int  SCROLLBAR_SetDefaultSkinClassic(); 
extern int  SCROLLBAR_SetSkinClassic(); 
extern int  SCROLLBAR_SetDefaultSkin(); 
extern int  SCROLLBAR_SetSkin(); 
extern int  SCROLLBAR_SetWidth(); 
extern int  SCROLLBAR_SetColor(); 
extern int  SCROLLBAR_GetValue(); 
extern int  SCROLLBAR_GetPageSize(); 
extern int  SCROLLBAR_GetNumItems(); 
extern int  SCROLLBAR_GetDefaultWidth(); 
extern int  SCROLLBAR_GetThumbSizeMin(); 
extern int  SCROLLBAR_SetDefaultColor(); 
extern int  SCROLLBAR_SetDefaultWidth(); 
extern int  SCROLLBAR_SetThumbSizeMin(); 
extern int  SCROLLBAR_CreateIndirect(); 
extern int  SCROLLBAR_Create(); 
extern int  SCROLLBAR_CreateAttached(); 
extern int  SCROLLBAR_AddValue(); 
extern int  SCROLLBAR_Callback(); 
extern int  SCROLLBAR_CreateEx(); 
extern int  SCROLLBAR_CreateUser(); 
extern int  SCROLLBAR_Dec(); 
extern int  SCROLLBAR_GetUserData(); 
extern int  SCROLLBAR_Inc(); 
extern int  SCROLLBAR_SetNumItems(); 
extern int  SCROLLBAR_SetPageSize(); 
extern int  SCROLLBAR_SetState(); 
extern int  SCROLLBAR_SetUserData(); 
extern int  SCROLLBAR_SetValue(); 
extern int  SCROLLBAR__InvalidatePartner(); 
extern int  SCROLLBAR__Rect2VRect(); 
extern int  RADIO_DrawSkinFlex(); 
extern int  RADIO_GetSkinFlexProps(); 
extern int  RADIO_SetSkinFlexProps(); 
extern int  RADIO_SetDefaultSkinClassic(); 
extern int  RADIO_SetSkinClassic(); 
extern int  RADIO_SetDefaultSkin(); 
extern int  RADIO_SetSkin(); 
extern int  RADIO_SetTextColor(); 
extern int  RADIO_SetText(); 
extern int  RADIO_GetImage(); 
extern int  RADIO_SetImage(); 
extern int  RADIO_SetGroupId(); 
extern int  RADIO_SetFont(); 
extern int  RADIO_SetFocusColor(); 
extern int  RADIO_SetDefaultImage(); 
extern int  RADIO_SetBkColor(); 
extern int  RADIO_GetText(); 
extern int  RADIO_GetDefaultFont(); 
extern int  RADIO_GetDefaultTextColor(); 
extern int  RADIO_SetDefaultFocusColor(); 
extern int  RADIO_SetDefaultFont(); 
extern int  RADIO_SetDefaultTextColor(); 
extern int  RADIO_CreateIndirect(); 
extern int  RADIO_Create(); 
extern int  RADIO_AddValue(); 
extern int  RADIO_Callback(); 
extern int  RADIO_CreateEx(); 
extern int  RADIO_CreateUser(); 
extern int  RADIO_Dec(); 
extern int  RADIO_GetUserData(); 
extern int  RADIO_GetValue(); 
extern int  RADIO_Inc(); 
extern int  RADIO_SetUserData(); 
extern int  RADIO_SetValue(); 
extern int  RADIO__GetButtonSize(); 
extern int  RADIO__SetValue(); 
extern int  PROGBAR_DrawSkinFlex(); 
extern int  PROGBAR_GetSkinFlexProps(); 
extern int  PROGBAR_SetSkinFlexProps(); 
extern int  PROGBAR_SetDefaultSkinClassic(); 
extern int  PROGBAR_SetSkinClassic(); 
extern int  PROGBAR_SetDefaultSkin(); 
extern int  PROGBAR_SetSkin(); 
extern int  PROGBAR_SetTextPos(); 
extern int  PROGBAR_SetTextColor(); 
extern int  PROGBAR_SetTextAlign(); 
extern int  PROGBAR_SetText(); 
extern int  PROGBAR_SetMinMax(); 
extern int  PROGBAR_SetFont(); 
extern int  PROGBAR_SetBarColor(); 
extern int  PROGBAR_CreateIndirect(); 
extern int  PROGBAR_Create(); 
extern int  PROGBAR_CreateAsChild(); 
extern int  PROGBAR_Callback(); 
extern int  PROGBAR_CreateEx(); 
extern int  PROGBAR_CreateUser(); 
extern int  PROGBAR_GetUserData(); 
extern int  PROGBAR_SetUserData(); 
extern int  PROGBAR_SetValue(); 
extern int  PROGBAR__GetTextLocked(); 
extern int  PROGBAR__GetTextRect(); 
extern int  PROGBAR__Value2Pos(); 
extern int  MULTIPAGE_SetRotation(); 
extern int  MULTIPAGE_GetDefaultAlign(); 
extern int  MULTIPAGE_GetDefaultBkColor(); 
extern int  MULTIPAGE_GetDefaultFont(); 
extern int  MULTIPAGE_GetDefaultTextColor(); 
extern int  MULTIPAGE_SetDefaultAlign(); 
extern int  MULTIPAGE_SetDefaultBkColor(); 
extern int  MULTIPAGE_SetDefaultFont(); 
extern int  MULTIPAGE_SetDefaultTextColor(); 
extern int  MULTIPAGE_CreateIndirect(); 
extern int  MULTIPAGE_Create(); 
extern int  MULTIPAGE_AddPage(); 
extern int  MULTIPAGE_Callback(); 
extern int  MULTIPAGE_CreateEx(); 
extern int  MULTIPAGE_CreateUser(); 
extern int  MULTIPAGE_DeletePage(); 
extern int  MULTIPAGE_DisablePage(); 
extern int  MULTIPAGE_EnablePage(); 
extern int  MULTIPAGE_GetEffectColor(); 
extern int  MULTIPAGE_GetNumEffectColors(); 
extern int  MULTIPAGE_GetSelection(); 
extern int  MULTIPAGE_GetUserData(); 
extern int  MULTIPAGE_GetWindow(); 
extern int  MULTIPAGE_IsPageEnabled(); 
extern int  MULTIPAGE_SelectPage(); 
extern int  MULTIPAGE_SetAlign(); 
extern int  MULTIPAGE_SetBkColor(); 
extern int  MULTIPAGE_SetEffectColor(); 
extern int  MULTIPAGE_SetFont(); 
extern int  MULTIPAGE_SetText(); 
extern int  MULTIPAGE_SetTextColor(); 
extern int  MULTIPAGE_SetUserData(); 
extern int  MULTIPAGE__CalcClientRect(); 
extern int  MULTIPAGE__DeleteScrollbar(); 
extern int  MULTIPAGE__DrawTextItemH(); 
extern int  MULTIPAGE__UpdatePositions(); 
extern int  MULTIEDIT_CreateIndirect(); 
extern int  MULTIEDIT_Create(); 
extern int  MULTIEDIT_AddKey(); 
extern int  MULTIEDIT_AddText(); 
extern int  MULTIEDIT_Callback(); 
extern int  MULTIEDIT_CreateEx(); 
extern int  MULTIEDIT_CreateUser(); 
extern int  MULTIEDIT_EnableBlink(); 
extern int  MULTIEDIT_GetCursorCharPos(); 
extern int  MULTIEDIT_GetCursorPixelPos(); 
extern int  MULTIEDIT_GetPrompt(); 
extern int  MULTIEDIT_GetText(); 
extern int  MULTIEDIT_GetTextSize(); 
extern int  MULTIEDIT_GetUserData(); 
extern int  MULTIEDIT_SetAutoScrollH(); 
extern int  MULTIEDIT_SetAutoScrollV(); 
extern int  MULTIEDIT_SetBkColor(); 
extern int  MULTIEDIT_SetBufferSize(); 
extern int  MULTIEDIT_SetCursorOffset(); 
extern int  MULTIEDIT_SetFont(); 
extern int  MULTIEDIT_SetHBorder(); 
extern int  MULTIEDIT_SetInsertMode(); 
extern int  MULTIEDIT_SetMaxNumChars(); 
extern int  MULTIEDIT_SetPasswordMode(); 
extern int  MULTIEDIT_SetPrompt(); 
extern int  MULTIEDIT_SetReadOnly(); 
extern int  MULTIEDIT_SetText(); 
extern int  MULTIEDIT_SetTextAlign(); 
extern int  MULTIEDIT_SetTextColor(); 
extern int  MULTIEDIT_SetUserData(); 
extern int  MULTIEDIT_SetWrapChar(); 
extern int  MULTIEDIT_SetWrapNone(); 
extern int  MULTIEDIT_SetWrapWord(); 
extern int  GUI_MessageBox(); 
extern int  MESSAGEBOX_Callback(); 
extern int  MESSAGEBOX_Create(); 
extern int  MENU__FindItem(); 
extern int  MENU_SetTextColor(); 
extern int  MENU_SetItem(); 
extern int  MENU_SetFont(); 
extern int  MENU_SetBorderSize(); 
extern int  MENU_SetBkColor(); 
extern int  MENU_Popup(); 
extern int  MENU_InsertItem(); 
extern int  MENU_GetNumItems(); 
extern int  MENU_GetItemText(); 
extern int  MENU_GetItem(); 
extern int  MENU_EnableItem(); 
extern int  MENU_DisableItem(); 
extern int  MENU_DeleteItem(); 
extern int  MENU_GetDefaultBkColor(); 
extern int  MENU_GetDefaultBorderSize(); 
extern int  MENU_GetDefaultEffect(); 
extern int  MENU_GetDefaultFont(); 
extern int  MENU_GetDefaultTextColor(); 
extern int  MENU_SetDefaultBkColor(); 
extern int  MENU_SetDefaultBorderSize(); 
extern int  MENU_SetDefaultEffect(); 
extern int  MENU_SetDefaultFont(); 
extern int  MENU_SetDefaultTextColor(); 
extern int  MENU_CreateIndirect(); 
extern int  MENU_Attach(); 
extern int  MENU_AddItem(); 
extern int  MENU_Callback(); 
extern int  MENU_CreateEx(); 
extern int  MENU_CreateUser(); 
extern int  MENU_GetOwner(); 
extern int  MENU_GetUserData(); 
extern int  MENU_SetOwner(); 
extern int  MENU_SetSel(); 
extern int  MENU_SetUserData(); 
extern int  MENU__GetNumItems(); 
extern int  MENU__InvalidateItem(); 
extern int  MENU__RecalcTextWidthOfItems(); 
extern int  MENU__ResizeMenu(); 
extern int  MENU__SendMenuMessage(); 
extern int  MENU__SetItem(); 
extern int  MENU__SetItemFlags(); 
extern int  LISTWHEEL_Create(); 
extern int  LISTWHEEL_CreateAsChild(); 
extern int  LISTWHEEL_CreateIndirect(); 
extern int  LISTWHEEL_AddString(); 
extern int  LISTWHEEL_Callback(); 
extern int  LISTWHEEL_CreateEx(); 
extern int  LISTWHEEL_CreateUser(); 
extern int  LISTWHEEL_GetFont(); 
extern int  LISTWHEEL_GetItemData(); 
extern int  LISTWHEEL_GetItemText(); 
extern int  LISTWHEEL_GetLBorder(); 
extern int  LISTWHEEL_GetLineHeight(); 
extern int  LISTWHEEL_GetNumItems(); 
extern int  LISTWHEEL_GetPos(); 
extern int  LISTWHEEL_GetRBorder(); 
extern int  LISTWHEEL_GetSel(); 
extern int  LISTWHEEL_GetSnapPosition(); 
extern int  LISTWHEEL_GetTextAlign(); 
extern int  LISTWHEEL_GetUserData(); 
extern int  LISTWHEEL_MoveToPos(); 
extern int  LISTWHEEL_OwnerDraw(); 
extern int  LISTWHEEL_SetBkColor(); 
extern int  LISTWHEEL_SetFont(); 
extern int  LISTWHEEL_SetItemData(); 
extern int  LISTWHEEL_SetLBorder(); 
extern int  LISTWHEEL_SetLineHeight(); 
extern int  LISTWHEEL_SetOwnerDraw(); 
extern int  LISTWHEEL_SetPos(); 
extern int  LISTWHEEL_SetRBorder(); 
extern int  LISTWHEEL_SetSel(); 
extern int  LISTWHEEL_SetSnapPosition(); 
extern int  LISTWHEEL_SetText(); 
extern int  LISTWHEEL_SetTextAlign(); 
extern int  LISTWHEEL_SetTextColor(); 
extern int  LISTWHEEL_SetTimerPeriod(); 
extern int  LISTWHEEL_SetUserData(); 
extern int  LISTWHEEL_SetVelocity(); 
extern int  LISTWHEEL__GetpStringLocked(); 
extern int  LISTVIEW_GetUserDataRow(); 
extern int  LISTVIEW_SetUserDataRow(); 
extern int  LISTVIEW_SetWrapMode(); 
extern int  LISTVIEW_SetTextColor(); 
extern int  LISTVIEW_SetTextAlign(); 
extern int  LISTVIEW_CompareDec(); 
extern int  LISTVIEW_CompareText(); 
extern int  LISTVIEW_DisableSort(); 
extern int  LISTVIEW_EnableSort(); 
extern int  LISTVIEW_GetSelUnsorted(); 
extern int  LISTVIEW_SetCompareFunc(); 
extern int  LISTVIEW_SetSelUnsorted(); 
extern int  LISTVIEW_SetSort(); 
extern int  LISTVIEW_SetSel(); 
extern int  LISTVIEW_SetRowHeight(); 
extern int  LISTVIEW_SetRBorder(); 
extern int  LISTVIEW_SetLBorder(); 
extern int  LISTVIEW_SetItemText(); 
extern int  LISTVIEW_SetItemBkColor(); 
extern int  LISTVIEW_SetItemTextColor(); 
extern int  LISTVIEW_SetItemBitmap(); 
extern int  LISTVIEW_SetGridVis(); 
extern int  LISTVIEW_SetFont(); 
extern int  LISTVIEW_SetFixed(); 
extern int  LISTVIEW_SetColumnWidth(); 
extern int  LISTVIEW_SetBkColor(); 
extern int  LISTVIEW_SetAutoScrollH(); 
extern int  LISTVIEW_SetAutoScrollV(); 
extern int  LISTVIEW_InsertRow(); 
extern int  LISTVIEW_GetTextColor(); 
extern int  LISTVIEW_GetSel(); 
extern int  LISTVIEW_GetNumRows(); 
extern int  LISTVIEW_GetNumColumns(); 
extern int  LISTVIEW_GetItemText(); 
extern int  LISTVIEW_GetHeader(); 
extern int  LISTVIEW_GetFont(); 
extern int  LISTVIEW_GetBkColor(); 
extern int  LISTVIEW_DisableRow(); 
extern int  LISTVIEW_EnableRow(); 
extern int  LISTVIEW_DeleteRow(); 
extern int  LISTVIEW__InvalidateRowAndBelow(); 
extern int  LISTVIEW_DeleteColumn(); 
extern int  LISTVIEW_DeleteAllRows(); 
extern int  LISTVIEW_SetDefaultBkColor(); 
extern int  LISTVIEW_SetDefaultFont(); 
extern int  LISTVIEW_SetDefaultGridColor(); 
extern int  LISTVIEW_SetDefaultTextColor(); 
extern int  LISTVIEW_CreateIndirect(); 
extern int  LISTVIEW_Create(); 
extern int  LISTVIEW_CreateAttached(); 
extern int  LISTVIEW_AddColumn(); 
extern int  LISTVIEW_AddRow(); 
extern int  LISTVIEW_Callback(); 
extern int  LISTVIEW_CreateEx(); 
extern int  LISTVIEW_CreateUser(); 
extern int  LISTVIEW_DecSel(); 
extern int  LISTVIEW_GetUserData(); 
extern int  LISTVIEW_IncSel(); 
extern int  LISTVIEW_SetHeaderHeight(); 
extern int  LISTVIEW_SetUserData(); 
extern int  LISTVIEW__GetNumColumns(); 
extern int  LISTVIEW__GetNumRows(); 
extern int  LISTVIEW__GetRowDistY(); 
extern int  LISTVIEW__GetSel(); 
extern int  LISTVIEW__InvalidateInsideArea(); 
extern int  LISTVIEW__InvalidateRow(); 
extern int  LISTVIEW__MoveSel(); 
extern int  LISTVIEW__SetSel(); 
extern int  LISTVIEW__UpdateScrollParas(); 
extern int  LISTVIEW__UpdateScrollPos(); 
extern int  LISTBOX_SetTextColor(); 
extern int  LISTBOX_SetTextAlign(); 
extern int  LISTBOX_SetString(); 
extern int  LISTBOX_SetScrollbarWidth(); 
extern int  LISTBOX_SetScrollbarColor(); 
extern int  LISTBOX_SetOwnerDraw(); 
extern int  LISTBOX_SetOwner(); 
extern int  LISTBOX_SetBkColor(); 
extern int  LISTBOX_SetAutoScrollH(); 
extern int  LISTBOX_SetAutoScrollV(); 
extern int  LISTBOX_GetScrollStepH(); 
extern int  LISTBOX_SetScrollStepH(); 
extern int  LISTBOX_GetItemSel(); 
extern int  LISTBOX_GetMulti(); 
extern int  LISTBOX_SetItemSel(); 
extern int  LISTBOX_SetMulti(); 
extern int  LISTBOX_GetItemSpacing(); 
extern int  LISTBOX_SetItemSpacing(); 
extern int  LISTBOX_GetItemDisabled(); 
extern int  LISTBOX_SetItemDisabled(); 
extern int  LISTBOX_InsertString(); 
extern int  LISTBOX_GetTextAlign(); 
extern int  LISTBOX_GetNumItems(); 
extern int  LISTBOX_GetItemText(); 
extern int  LISTBOX_GetFont(); 
extern int  LISTBOX_SetFont(); 
extern int  LISTBOX_DeleteItem(); 
extern int  LISTBOX_GetDefaultBkColor(); 
extern int  LISTBOX_GetDefaultFont(); 
extern int  LISTBOX_GetDefaultScrollStepH(); 
extern int  LISTBOX_GetDefaultTextAlign(); 
extern int  LISTBOX_GetDefaultTextColor(); 
extern int  LISTBOX_SetDefaultBkColor(); 
extern int  LISTBOX_SetDefaultFont(); 
extern int  LISTBOX_SetDefaultScrollStepH(); 
extern int  LISTBOX_SetDefaultTextAlign(); 
extern int  LISTBOX_SetDefaultTextColor(); 
extern int  LISTBOX_CreateIndirect(); 
extern int  LISTBOX_Create(); 
extern int  LISTBOX_CreateAsChild(); 
extern int  LISTBOX_AddStringH(); 
extern int  LISTBOX_AddKey(); 
extern int  LISTBOX_AddString(); 
extern int  LISTBOX_Callback(); 
extern int  LISTBOX_CreateEx(); 
extern int  LISTBOX_CreateUser(); 
extern int  LISTBOX_DecSel(); 
extern int  LISTBOX_GetSel(); 
extern int  LISTBOX_GetUserData(); 
extern int  LISTBOX_IncSel(); 
extern int  LISTBOX_InvalidateItem(); 
extern int  LISTBOX_OwnerDraw(); 
extern int  LISTBOX_SetSel(); 
extern int  LISTBOX_SetText(); 
extern int  LISTBOX_SetUserData(); 
extern int  LISTBOX_UpdateScrollers(); 
extern int  LISTBOX__AddSize(); 
extern int  LISTBOX__GetNumItems(); 
extern int  LISTBOX__GetpStringLocked(); 
extern int  LISTBOX__InvalidateInsideArea(); 
extern int  LISTBOX__InvalidateItem(); 
extern int  LISTBOX__InvalidateItemAndBelow(); 
extern int  LISTBOX__InvalidateItemSize(); 
extern int  LISTBOX__SetScrollbarColor(); 
extern int  LISTBOX__SetScrollbarWidth(); 
extern int  IMAGE_SetJPEG(); 
extern int  IMAGE_SetJPEGEx(); 
extern int  IMAGE_SetGIF(); 
extern int  IMAGE_SetGIFEx(); 
extern int  IMAGE_SetDTA(); 
extern int  IMAGE_SetDTAEx(); 
extern int  IMAGE_SetBMP(); 
extern int  IMAGE_SetBMPEx(); 
extern int  IMAGE_SetBitmap(); 
extern int  IMAGE_Callback(); 
extern int  IMAGE_CreateEx(); 
extern int  IMAGE_CreateIndirect(); 
extern int  IMAGE_CreateUser(); 
extern int  IMAGE__FreeAttached(); 
extern int  IMAGE__SetWindowSize(); 
extern int  ICONVIEW_SetWrapMode(); 
extern int  ICONVIEW_EnableStreamAuto(); 
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
extern int  HEADER__SetDrawObj(); 
extern int  HEADER_SetStreamedBitmap(); 
extern int  HEADER_SetStreamedBitmapEx(); 
extern int  HEADER_DrawSkinFlex(); 
extern int  HEADER_GetSkinFlexProps(); 
extern int  HEADER_SetSkinFlexProps(); 
extern int  HEADER_SetDefaultSkinClassic(); 
extern int  HEADER_SetSkinClassic(); 
extern int  HEADER_SetDefaultSkin(); 
extern int  HEADER_SetSkin(); 
extern int  HEADER_SetFixed(); 
extern int  HEADER_SetDragLimit(); 
extern int  HEADER_GetSel(); 
extern int  HEADER_GetDefaultArrowColor(); 
extern int  HEADER_GetDefaultBkColor(); 
extern int  HEADER_GetDefaultBorderH(); 
extern int  HEADER_GetDefaultBorderV(); 
extern int  HEADER_GetDefaultCursor(); 
extern int  HEADER_GetDefaultFont(); 
extern int  HEADER_GetDefaultTextColor(); 
extern int  HEADER_SetDefaultArrowColor(); 
extern int  HEADER_SetDefaultBkColor(); 
extern int  HEADER_SetDefaultBorderH(); 
extern int  HEADER_SetDefaultBorderV(); 
extern int  HEADER_SetDefaultCursor(); 
extern int  HEADER_SetDefaultFont(); 
extern int  HEADER_SetDefaultTextColor(); 
extern int  HEADER_CreateIndirect(); 
extern int  HEADER_CreateAttached(); 
extern int  HEADER_GetArrowColor(); 
extern int  HEADER_GetBkColor(); 
extern int  HEADER_GetTextColor(); 
extern int  HEADER_SetArrowColor(); 
extern int  HEADER_SetBkColor(); 
extern int  HEADER_SetTextColor(); 
extern int  HEADER_SetBMP(); 
extern int  HEADER_SetBMPEx(); 
extern int  HEADER_SetBitmap(); 
extern int  HEADER_SetBitmapEx(); 
extern int  HEADER_AddItem(); 
extern int  HEADER_Callback(); 
extern int  HEADER_Create(); 
extern int  HEADER_CreateEx(); 
extern int  HEADER_CreateUser(); 
extern int  HEADER_DeleteItem(); 
extern int  HEADER_GetHeight(); 
extern int  HEADER_GetItemWidth(); 
extern int  HEADER_GetNumItems(); 
extern int  HEADER_GetUserData(); 
extern int  HEADER_SetDirIndicator(); 
extern int  HEADER_SetFont(); 
extern int  HEADER_SetHeight(); 
extern int  HEADER_SetItemText(); 
extern int  HEADER_SetItemWidth(); 
extern int  HEADER_SetScrollPos(); 
extern int  HEADER_SetTextAlign(); 
extern int  HEADER_SetUserData(); 
extern int  GUI_HOOK_Add(); 
extern int  GUI_HOOK_Remove(); 
extern int  GUI_EditString(); 
extern int  GUI_EditHex(); 
extern int  GUI_EditFloat(); 
extern int  GUI_EditDec(); 
extern int  GUI_EditBin(); 
extern int  GUI_DRAW_STREAMED_Create(); 
extern int  GUI_DRAW_SELF_Create(); 
extern int  GUI_DRAW_BMP_Create(); 
extern int  GUI_DRAW_BITMAP_Create(); 
extern int  GUI_DRAW__Draw(); 
extern int  GUI_DRAW__GetXSize(); 
extern int  GUI_DRAW__GetYSize(); 
extern int  GUI_ARRAY_ResizeItemLocked(); 
extern int  GUI_ARRAY_InsertBlankItem(); 
extern int  GUI_ARRAY_InsertItem(); 
extern int  GUI_ARRAY_DeleteItem(); 
extern int  GUI_ARRAY_AddItem(); 
extern int  GUI_ARRAY_Create(); 
extern int  GUI_ARRAY_Delete(); 
extern int  GUI_ARRAY_GetNumItems(); 
extern int  GUI_ARRAY_GethItem(); 
extern int  GUI_ARRAY_GetpItemLocked(); 
extern int  GUI_ARRAY_SetItem(); 
extern int  GUI_ARRAY__GethItem(); 
extern int  GUI_ARRAY__GetpItemLocked(); 
extern int  GUI_ARRAY__SethItem(); 
extern int  GRAPH_SCALE_Create(); 
extern int  GRAPH_SCALE_Delete(); 
extern int  GRAPH_SCALE_SetFactor(); 
extern int  GRAPH_SCALE_SetFont(); 
extern int  GRAPH_SCALE_SetNumDecs(); 
extern int  GRAPH_SCALE_SetOff(); 
extern int  GRAPH_SCALE_SetPos(); 
extern int  GRAPH_SCALE_SetTextColor(); 
extern int  GRAPH_SCALE_SetTickDist(); 
extern int  GRAPH_DATA_YT_AddValue(); 
extern int  GRAPH_DATA_YT_Clear(); 
extern int  GRAPH_DATA_YT_Create(); 
extern int  GRAPH_DATA_YT_Delete(); 
extern int  GRAPH_DATA_YT_MirrorX(); 
extern int  GRAPH_DATA_YT_SetAlign(); 
extern int  GRAPH_DATA_YT_SetOffY(); 
extern int  GRAPH_DATA_XY_AddPoint(); 
extern int  GRAPH_DATA_XY_Clear(); 
extern int  GRAPH_DATA_XY_Create(); 
extern int  GRAPH_DATA_XY_Delete(); 
extern int  GRAPH_DATA_XY_GetLineVis(); 
extern int  GRAPH_DATA_XY_GetPointVis(); 
extern int  GRAPH_DATA_XY_SetLineStyle(); 
extern int  GRAPH_DATA_XY_SetLineVis(); 
extern int  GRAPH_DATA_XY_SetOffX(); 
extern int  GRAPH_DATA_XY_SetOffY(); 
extern int  GRAPH_DATA_XY_SetOwnerDraw(); 
extern int  GRAPH_DATA_XY_SetPenSize(); 
extern int  GRAPH_DATA_XY_SetPointSize(); 
extern int  GRAPH_DATA_XY_SetPointVis(); 
extern int  GRAPH_CreateIndirect(); 
extern int  GRAPH_AttachData(); 
extern int  GRAPH_AttachScale(); 
extern int  GRAPH_Callback(); 
extern int  GRAPH_CreateEx(); 
extern int  GRAPH_CreateUser(); 
extern int  GRAPH_DetachData(); 
extern int  GRAPH_DetachScale(); 
extern int  GRAPH_GetScrollValue(); 
extern int  GRAPH_GetUserData(); 
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
extern int  GRAPH__InvalidateGraph(); 
extern int  FRAMEWIN__UpdateButtons(); 
extern int  FRAMEWIN_DrawSkinFlex(); 
extern int  FRAMEWIN_GetSkinFlexProps(); 
extern int  FRAMEWIN_SetSkinFlexProps(); 
extern int  FRAMEWIN_OwnerDraw(); 
extern int  FRAMEWIN_SetDefaultSkinClassic(); 
extern int  FRAMEWIN_SetSkinClassic(); 
extern int  FRAMEWIN_SetDefaultSkin(); 
extern int  FRAMEWIN_SetSkin(); 
extern int  FRAMEWIN_SetTitleVis(); 
extern int  FRAMEWIN_SetTitleHeight(); 
extern int  FRAMEWIN_SetResizeable(); 
extern int  FRAMEWIN_SetOwnerDraw(); 
extern int  FRAMEWIN_SetFont(); 
extern int  FRAMEWIN_SetBarColor(); 
extern int  FRAMEWIN_SetClientColor(); 
extern int  FRAMEWIN_SetTextColor(); 
extern int  FRAMEWIN_SetTextColorEx(); 
extern int  FRAMEWIN_SetBorderSize(); 
extern int  FRAMEWIN_Maximize(); 
extern int  FRAMEWIN_Minimize(); 
extern int  FRAMEWIN_Restore(); 
extern int  FRAMEWIN_IsMaximized(); 
extern int  FRAMEWIN_IsMinimized(); 
extern int  FRAMEWIN_GetActive(); 
extern int  FRAMEWIN_GetBarColor(); 
extern int  FRAMEWIN_GetBorderSize(); 
extern int  FRAMEWIN_GetBorderSizeEx(); 
extern int  FRAMEWIN_GetFont(); 
extern int  FRAMEWIN_GetText(); 
extern int  FRAMEWIN_GetTextAlign(); 
extern int  FRAMEWIN_GetTitleHeight(); 
extern int  FRAMEWIN_GetDefaultBarColor(); 
extern int  FRAMEWIN_GetDefaultBorderSize(); 
extern int  FRAMEWIN_GetDefaultClientColor(); 
extern int  FRAMEWIN_GetDefaultFont(); 
extern int  FRAMEWIN_GetDefaultTextColor(); 
extern int  FRAMEWIN_GetDefaultTitleHeight(); 
extern int  FRAMEWIN_SetDefaultBarColor(); 
extern int  FRAMEWIN_SetDefaultBorderSize(); 
extern int  FRAMEWIN_SetDefaultClientColor(); 
extern int  FRAMEWIN_SetDefaultFont(); 
extern int  FRAMEWIN_SetDefaultTextAlign(); 
extern int  FRAMEWIN_SetDefaultTextColor(); 
extern int  FRAMEWIN_SetDefaultTitleHeight(); 
extern int  FRAMEWIN_CreateIndirect(); 
extern int  FRAMEWIN_Create(); 
extern int  FRAMEWIN_CreateAsChild(); 
extern int  FRAMEWIN_AddMinButton(); 
extern int  FRAMEWIN_AddMaxButton(); 
extern int  FRAMEWIN_AddCloseButton(); 
extern int  FRAMEWIN_AddButton(); 
extern int  FRAMEWIN_AddMenu(); 
extern int  FRAMEWIN_Callback(); 
extern int  FRAMEWIN_CreateEx(); 
extern int  FRAMEWIN_CreateUser(); 
extern int  FRAMEWIN_GetUserData(); 
extern int  FRAMEWIN_SetActive(); 
extern int  FRAMEWIN_SetMoveable(); 
extern int  FRAMEWIN_SetText(); 
extern int  FRAMEWIN_SetTextAlign(); 
extern int  FRAMEWIN_SetUserData(); 
extern int  FRAMEWIN__CalcPositions(); 
extern int  FRAMEWIN__CalcTitleHeight(); 
extern int  FRAMEWIN__GetBorderSize(); 
extern int  FRAMEWIN__GetTitleLimits(); 
extern int  FRAMEWIN__UpdatePositions(); 
extern int  EDIT_SetTextMode(); 
extern int  EDIT_SetSel(); 
extern int  EDIT_SetpfUpdateBuffer(); 
extern int  EDIT_SetpfAddKeyEx(); 
extern int  EDIT_SetInsertMode(); 
extern int  EDIT_SetFocussable(); 
extern int  EDIT_SetCursorAtChar(); 
extern int  EDIT_GetNumChars(); 
extern int  EDIT_GetCursorCharPos(); 
extern int  EDIT_GetCursorPixelPos(); 
extern int  EDIT_EnableBlink(); 
extern int  EDIT_GetDefaultBkColor(); 
extern int  EDIT_GetDefaultFont(); 
extern int  EDIT_GetDefaultTextAlign(); 
extern int  EDIT_GetDefaultTextColor(); 
extern int  EDIT_SetDefaultBkColor(); 
extern int  EDIT_SetDefaultFont(); 
extern int  EDIT_SetDefaultTextAlign(); 
extern int  EDIT_SetDefaultTextColor(); 
extern int  EDIT_CreateIndirect(); 
extern int  EDIT_Create(); 
extern int  EDIT_CreateAsChild(); 
extern int  EDIT_SetUlongMode(); 
extern int  EDIT_SetHexMode(); 
extern int  EDIT_GetFloatValue(); 
extern int  EDIT_SetFloatMode(); 
extern int  EDIT_SetFloatValue(); 
extern int  EDIT_SetDecMode(); 
extern int  EDIT_SetBinMode(); 
extern int  EDIT_AddKey(); 
extern int  EDIT_Callback(); 
extern int  EDIT_CreateEx(); 
extern int  EDIT_CreateUser(); 
extern int  EDIT_GetBkColor(); 
extern int  EDIT_GetFont(); 
extern int  EDIT_GetText(); 
extern int  EDIT_GetTextColor(); 
extern int  EDIT_GetUserData(); 
extern int  EDIT_GetValue(); 
extern int  EDIT_SetBkColor(); 
extern int  EDIT_SetCursorAtPixel(); 
extern int  EDIT_SetFont(); 
extern int  EDIT_SetMaxLen(); 
extern int  EDIT_SetText(); 
extern int  EDIT_SetTextAlign(); 
extern int  EDIT_SetTextColor(); 
extern int  EDIT_SetUserData(); 
extern int  EDIT_SetValue(); 
extern int  EDIT__GetCurrentChar(); 
extern int  EDIT__SetCursorPos(); 
extern int  EDIT__SetValueUnsigned(); 
extern int  DROPDOWN_DrawSkinFlex(); 
extern int  DROPDOWN_GetSkinFlexProps(); 
extern int  DROPDOWN_SetSkinFlexProps(); 
extern int  DROPDOWN_SetDefaultSkinClassic(); 
extern int  DROPDOWN_SetSkinClassic(); 
extern int  DROPDOWN_SetDefaultSkin(); 
extern int  DROPDOWN_SetSkin(); 
extern int  DROPDOWN_SetTextHeight(); 
extern int  DROPDOWN_SetTextColor(); 
extern int  DROPDOWN_SetTextAlign(); 
extern int  DROPDOWN_SetScrollbarWidth(); 
extern int  DROPDOWN_SetScrollbarColor(); 
extern int  DROPDOWN_SetFont(); 
extern int  DROPDOWN_SetColor(); 
extern int  DROPDOWN_SetBkColor(); 
extern int  DROPDOWN_SetAutoScroll(); 
extern int  DROPDOWN_GetItemSpacing(); 
extern int  DROPDOWN_SetItemSpacing(); 
extern int  DROPDOWN_GetItemDisabled(); 
extern int  DROPDOWN_SetItemDisabled(); 
extern int  DROPDOWN_InsertString(); 
extern int  DROPDOWN_GetNumItems(); 
extern int  DROPDOWN_DecSelExp(); 
extern int  DROPDOWN_GetListbox(); 
extern int  DROPDOWN_GetSelExp(); 
extern int  DROPDOWN_IncSelExp(); 
extern int  DROPDOWN_SetSelExp(); 
extern int  DROPDOWN_DeleteItem(); 
extern int  DROPDOWN_GetDefaultBkColor(); 
extern int  DROPDOWN_GetDefaultColor(); 
extern int  DROPDOWN_GetDefaultFont(); 
extern int  DROPDOWN_GetDefaultScrollbarColor(); 
extern int  DROPDOWN_SetDefaultBkColor(); 
extern int  DROPDOWN_SetDefaultColor(); 
extern int  DROPDOWN_SetDefaultFont(); 
extern int  DROPDOWN_SetDefaultScrollbarColor(); 
extern int  DROPDOWN_CreateIndirect(); 
extern int  DROPDOWN_Create(); 
extern int  DROPDOWN_AddString(); 
extern int  DROPDOWN_AddKey(); 
extern int  DROPDOWN_Callback(); 
extern int  DROPDOWN_Collapse(); 
extern int  DROPDOWN_CreateEx(); 
extern int  DROPDOWN_CreateUser(); 
extern int  DROPDOWN_DecSel(); 
extern int  DROPDOWN_Expand(); 
extern int  DROPDOWN_GetItemText(); 
extern int  DROPDOWN_GetSel(); 
extern int  DROPDOWN_GetUserData(); 
extern int  DROPDOWN_IncSel(); 
extern int  DROPDOWN_SetSel(); 
extern int  DROPDOWN_SetUpMode(); 
extern int  DROPDOWN_SetUserData(); 
extern int  DROPDOWN__AdjustHeight(); 
extern int  DROPDOWN__GetNumItems(); 
extern int  DROPDOWN__GetpItemLocked(); 
extern int  DIALOG_GetBkColor(); 
extern int  DIALOG_SetBkColor(); 
extern int  GUI_CreateDialogBox(); 
extern int  GUI_EndDialog(); 
extern int  GUI_ExecCreatedDialog(); 
extern int  GUI_ExecDialogBox(); 
extern int  GUI_GetDialogStatusPtr(); 
extern int  GUI_SetDialogStatusPtr(); 
extern int  CHOOSEFILE_Callback(); 
extern int  CHOOSEFILE_Create(); 
extern int  CHOOSEFILE_EnableToolTips(); 
extern int  CHOOSEFILE_SetButtonText(); 
extern int  CHOOSEFILE_SetDefaultButtonText(); 
extern int  CHOOSEFILE_SetDelim(); 
extern int  CHOOSEFILE_SetToolTips(); 
extern int  CHOOSEFILE_SetTopMode(); 
extern int  CHOOSECOLOR_Callback(); 
extern int  CHOOSECOLOR_Create(); 
extern int  CHOOSECOLOR_GetSel(); 
extern int  CHOOSECOLOR_SetDefaultBorder(); 
extern int  CHOOSECOLOR_SetDefaultButtonSize(); 
extern int  CHOOSECOLOR_SetDefaultColor(); 
extern int  CHOOSECOLOR_SetDefaultSpace(); 
extern int  CHOOSECOLOR_SetSel(); 
extern int  CHECKBOX_DrawSkinFlex(); 
extern int  CHECKBOX_GetSkinFlexProps(); 
extern int  CHECKBOX_SetSkinFlexProps(); 
extern int  CHECKBOX_SetDefaultSkinClassic(); 
extern int  CHECKBOX_SetSkinClassic(); 
extern int  CHECKBOX_SetDefaultSkin(); 
extern int  CHECKBOX_SetSkin(); 
extern int  CHECKBOX_SetTextColor(); 
extern int  CHECKBOX_SetTextAlign(); 
extern int  CHECKBOX_SetText(); 
extern int  CHECKBOX_SetState(); 
extern int  CHECKBOX_SetSpacing(); 
extern int  CHECKBOX_SetNumStates(); 
extern int  CHECKBOX_SetImage(); 
extern int  CHECKBOX_SetFont(); 
extern int  CHECKBOX_SetFocusColor(); 
extern int  CHECKBOX_SetDefaultImage(); 
extern int  CHECKBOX_SetBoxBkColor(); 
extern int  CHECKBOX_SetBkColor(); 
extern int  CHECKBOX_IsChecked(); 
extern int  CHECKBOX_GetText(); 
extern int  CHECKBOX_GetState(); 
extern int  CHECKBOX_GetDefaultAlign(); 
extern int  CHECKBOX_GetDefaultBkColor(); 
extern int  CHECKBOX_GetDefaultFont(); 
extern int  CHECKBOX_GetDefaultSpacing(); 
extern int  CHECKBOX_GetDefaultTextColor(); 
extern int  CHECKBOX_SetDefaultAlign(); 
extern int  CHECKBOX_SetDefaultBkColor(); 
extern int  CHECKBOX_SetDefaultFocusColor(); 
extern int  CHECKBOX_SetDefaultFont(); 
extern int  CHECKBOX_SetDefaultSpacing(); 
extern int  CHECKBOX_SetDefaultTextColor(); 
extern int  CHECKBOX_CreateIndirect(); 
extern int  CHECKBOX_Create(); 
extern int  CHECKBOX_Callback(); 
extern int  CHECKBOX_CreateEx(); 
extern int  CHECKBOX_CreateUser(); 
extern int  CHECKBOX_GetUserData(); 
extern int  CHECKBOX_SetUserData(); 
extern int  CHECKBOX__GetButtonSize(); 
extern int  BUTTON__SetDrawObj(); 
extern int  BUTTON_SetStreamedBitmap(); 
extern int  BUTTON_SetStreamedBitmapEx(); 
extern int  BUTTON_DrawSkinFlex(); 
extern int  BUTTON_GetSkinFlexProps(); 
extern int  BUTTON_SetSkinFlexProps(); 
extern int  BUTTON_SetDefaultSkinClassic(); 
extern int  BUTTON_SetSkinClassic(); 
extern int  BUTTON_SetDefaultSkin(); 
extern int  BUTTON_SetSkin(); 
extern int  BUTTON_GetTextAlign(); 
extern int  BUTTON_SetTextAlign(); 
extern int  BUTTON_SetFocusColor(); 
extern int  BUTTON_SetSelfDraw(); 
extern int  BUTTON_SetSelfDrawEx(); 
extern int  BUTTON_IsPressed(); 
extern int  BUTTON_GetBitmap(); 
extern int  BUTTON_GetFont(); 
extern int  BUTTON_GetText(); 
extern int  BUTTON_GetDefaultBkColor(); 
extern int  BUTTON_GetDefaultFont(); 
extern int  BUTTON_GetDefaultTextAlign(); 
extern int  BUTTON_GetDefaultTextColor(); 
extern int  BUTTON_SetDefaultBkColor(); 
extern int  BUTTON_SetDefaultFocusColor(); 
extern int  BUTTON_SetDefaultFont(); 
extern int  BUTTON_SetDefaultTextAlign(); 
extern int  BUTTON_SetDefaultTextColor(); 
extern int  BUTTON_CreateIndirect(); 
extern int  BUTTON_Create(); 
extern int  BUTTON_CreateAsChild(); 
extern int  BUTTON_GetBkColor(); 
extern int  BUTTON_GetFrameColor(); 
extern int  BUTTON_GetTextColor(); 
extern int  BUTTON_SetBkColor(); 
extern int  BUTTON_SetFrameColor(); 
extern int  BUTTON_SetTextColor(); 
extern int  BUTTON_SetBMP(); 
extern int  BUTTON_SetBMPEx(); 
extern int  BUTTON_SetBitmap(); 
extern int  BUTTON_SetBitmapEx(); 
extern int  BUTTON_Callback(); 
extern int  BUTTON_CreateEx(); 
extern int  BUTTON_CreateUser(); 
extern int  BUTTON_GetUserData(); 
extern int  BUTTON_SetFocussable(); 
extern int  BUTTON_SetFont(); 
extern int  BUTTON_SetPressed(); 
extern int  BUTTON_SetReactOnLevel(); 
extern int  BUTTON_SetReactOnTouch(); 
extern int  BUTTON_SetState(); 
extern int  BUTTON_SetText(); 
extern int  BUTTON_SetTextOffset(); 
extern int  BUTTON_SetUserData(); 
extern int  GUI_VNC_AttachToLayer(); 
extern int  GUI_VNC_EnableKeyboardInput(); 
extern int  GUI_VNC_GetNumConnections(); 
extern int  GUI_VNC_Process(); 
extern int  GUI_VNC_RingBell(); 
extern int  GUI_VNC_SetAuthentication(); 
extern int  GUI_VNC_SetLockFrame(); 
extern int  GUI_VNC_SetProgName(); 
extern int  GUI_VNC_SetSize(); 
extern int  GUI_VNC_DoDES(); 
extern int  GUI_VNC_SetDESKey(); 
extern int  GUI_VNC_SetPassword(); 
extern int  GUI_MEMDEV__XY2PTR(); 
extern int  GUI_MEMDEV__XY2PTREx(); 
extern int  GUI_MEMDEV__WriteToActiveOpaque(); 
extern int  GUI_MEMDEV__WriteToActiveAlpha(); 
extern int  GUI_MEMDEV_WriteEx(); 
extern int  GUI_MEMDEV_WriteExAt(); 
extern int  GUI_MEMDEV_WriteAlpha(); 
extern int  GUI_MEMDEV_WriteAlphaAt(); 
extern int  GUI_MEMDEV_Write(); 
extern int  GUI_MEMDEV_WriteAt(); 
extern int  GUI_USAGE_BM_Create(); 
extern int  GUI_USAGE_AddRect(); 
extern int  GUI_USAGE_DecUseCnt(); 
extern int  GUI_USAGE_Select(); 
extern int  GUIDEV_StaticDevices_C(); 
extern int  GUI_MEMDEV_CreateStatic(); 
extern int  GUI_MEMDEV_GetStaticDevice(); 
extern int  GUI_MEMDEV_GetWindowDevice(); 
extern int  GUI_MEMDEV_Paint1Static(); 
extern int  GUI_MEMDEV_SetOrg(); 
extern int  GUI_MEMDEV_SetColorConv(); 
extern int  GUI_MEMDEV_SerializeBMP(); 
extern int  GUI_MEMDEV_RotateHQT(); 
extern int  GUI_MEMDEV_Rotate(); 
extern int  GUI_MEMDEV_RotateHQ(); 
extern int  GUI_MEMDEV__Rotate(); 
extern int  GUI_MEMDEV_ReduceYSize(); 
extern int  GUI_MEMDEV__ReadLine(); 
extern int  GUI_MEMDEV_DrawPerspectiveX(); 
extern int  GUI_MEMDEV__DrawSizedAt(); 
extern int  GUIDEV_MoveAndFadeWin_C(); 
extern int  GUI_MEMDEV_FadeInWindow(); 
extern int  GUI_MEMDEV_FadeOutWindow(); 
extern int  GUI_MEMDEV_MoveInWindow(); 
extern int  GUI_MEMDEV_MoveOutWindow(); 
extern int  GUI_MEMDEV_ShiftInWindow(); 
extern int  GUI_MEMDEV_ShiftOutWindow(); 
extern int  GUI_MEMDEV_SwapWindow(); 
extern int  GUI_MEMDEV_FadeDevices(); 
extern int  GUI_MEMDEV_SetAnimationCallback(); 
extern int  GUI_MEMDEV__FadeDevice(); 
extern int  GUI_MEMDEV__FadeDeviceEx(); 
extern int  GUI_MEASDEV_ClearRect(); 
extern int  GUI_MEASDEV_Create(); 
extern int  GUI_MEASDEV_Delete(); 
extern int  GUI_MEASDEV_GetRect(); 
extern int  GUI_MEASDEV_Select(); 
extern int  GUI_MEMDEV_GetYSize(); 
extern int  GUI_MEMDEV_GetXSize(); 
extern int  GUI_MEMDEV_GetXPos(); 
extern int  GUI_MEMDEV_GetYPos(); 
extern int  GUI_MEMDEV_GetDataPtr(); 
extern int  GUI_MEMDEV_CreateFixed(); 
extern int  GUI_MEMDEV_CopyFromLCD(); 
extern int  GUI_MEMDEV_CompareWithLCD(); 
extern int  GUI_MEMDEV_Clear(); 
extern int  GUI_MEMDEV_Draw(); 
extern int  GUI_MEMDEV__SetAlphaCallback(); 
extern int  GUI_MEMDEV_CreateAuto(); 
extern int  GUI_MEMDEV_DeleteAuto(); 
extern int  GUI_MEMDEV_DrawAuto(); 
extern int  GUI_MEMDEV_CopyFromLCDAA(); 
extern int  GUI_MEMDEV_CopyToLCDAA(); 
extern int  GUI_MEMDEV_CopyToLCD(); 
extern int  GUI_MEMDEV_CopyToLCDAt(); 
extern int  GUI_MEMDEV_Create(); 
extern int  GUI_MEMDEV_CreateEx(); 
extern int  GUI_MEMDEV_Delete(); 
extern int  GUI_MEMDEV_GetBitsPerPixel(); 
extern int  GUI_MEMDEV_MarkDirty(); 
extern int  GUI_MEMDEV_Select(); 
extern int  GUI_MEMDEV__CreateFixed(); 
extern int  GUI_MEMDEV__WriteToActiveAt(); 
extern int  LCDWin_c(); 
extern int  GUIDRV_TemplateI_Config(); 
extern int  GUIDRV_TemplateI_SetBus_XXX(); 
extern int  GUIDRV__TemplateI_GetDevFunc(); 
extern int  GUIDRV__TemplateI_GetDevProp(); 
extern int  GUIDRV__TemplateI_GetRect(); 
extern int  GUIDRV__TemplateI_SetOrg(); 
extern int  GUIDRV_SSD1926_Config(); 
extern int  GUIDRV_SSD1926_SetBus16(); 
extern int  GUIDRV__SSD1926_GetDevFunc(); 
extern int  GUIDRV__SSD1926_GetDevProp(); 
extern int  GUIDRV__SSD1926_GetRect(); 
extern int  GUIDRV__SSD1926_SetOrg(); 
extern int  GUIDRV_SPage_SetUC1611(); 
extern int  GUIDRV_SPage_SetST7591(); 
extern int  GUIDRV_SPage_Set1512(); 
extern int  GUIDRV_SPage_SetS1D15(); 
extern int  GUIDRV_SPage_Set1510(); 
extern int  GUIDRV__SPage_AddDirtyRect(); 
extern int  GUIDRV__SPage_ControlCache(); 
extern int  GUIDRV__SPage_FlushCache(); 
extern int  GUIDRV__SPage_ReadCache(); 
extern int  GUIDRV__SPage_WriteCache(); 
extern int  GUIDRV_SPage_Config(); 
extern int  GUIDRV_SPage_SetBus8(); 
extern int  GUIDRV__SPage_GetDevFunc(); 
extern int  GUIDRV__SPage_GetDevProp(); 
extern int  GUIDRV__SPage_GetRect(); 
extern int  GUIDRV__SPage_Init(); 
extern int  GUIDRV__SPage_SetOrg(); 
extern int  GUIDRV_SLin_SetUC1617(); 
extern int  GUIDRV_SLin_SetT6963(); 
extern int  GUIDRV_SLin_SetSSD1848(); 
extern int  GUIDRV_SLin_SetS1D13700(); 
extern int  GUIDRV_SLin_Config(); 
extern int  GUIDRV_SLin_SetBus8(); 
extern int  GUIDRV__SLin_GetDevFunc(); 
extern int  GUIDRV__SLin_GetDevProp(); 
extern int  GUIDRV__SLin_GetRect(); 
extern int  GUIDRV__SLin_SendCache(); 
extern int  GUIDRV__SLin_SetOrg(); 
extern int  GUIDRV__SLin_WriteVMEM(); 
extern int  GUIDRV_S1D15G00_Config(); 
extern int  GUIDRV_S1D15G00_SetBus8(); 
extern int  GUIDRV_S1D13781_Config(); 
extern int  GUIDRV_S1D13781_SetBusSPI(); 
extern int  GUIDRV__S1D13781_GetAddr(); 
extern int  GUIDRV__S1D13781_GetDevFunc(); 
extern int  GUIDRV__S1D13781_GetDevProp(); 
extern int  GUIDRV__S1D13781_GetRect(); 
extern int  GUIDRV__S1D13781_Init(); 
extern int  GUIDRV__S1D13781_InitOnce(); 
extern int  GUIDRV__S1D13781_SetOrg(); 
extern int  GUIDRV_S1D13748_Config(); 
extern int  GUIDRV_S1D13748_SetBus16(); 
extern int  GUIDRV__S1D13748_GetDevFunc(); 
extern int  GUIDRV__S1D13748_GetDevProp(); 
extern int  GUIDRV__S1D13748_GetRect(); 
extern int  GUIDRV__S1D13748_SetOrg(); 
extern int  GUIDRV__NoOpt_DrawBitmap(); 
extern int  GUIDRV__NoOpt_DrawHLine(); 
extern int  GUIDRV__NoOpt_DrawVLine(); 
extern int  GUIDRV__NoOpt_FillRect(); 
extern int  GUIDRV__NoOpt_XorPixel(); 
extern int  GUIDRV_IST3088_SetBus16(); 
extern int  GUIDRV__IST3088_GetDevFunc(); 
extern int  GUIDRV__IST3088_GetDevProp(); 
extern int  GUIDRV__IST3088_GetRect(); 
extern int  GUIDRV__IST3088_SetOrg(); 
extern int  GUIDRV_FlexColor_SetFunc66720(); 
extern int  GUIDRV_FlexColor_SetReadFunc66720_B16(); 
extern int  GUIDRV_FlexColor_SetFunc66719(); 
extern int  GUIDRV_FlexColor_SetFunc66718(); 
extern int  GUIDRV_FlexColor_SetFunc66715(); 
extern int  GUIDRV_FlexColor_SetFunc66714(); 
extern int  GUIDRV_FlexColor_SetFunc66712(); 
extern int  GUIDRV_FlexColor_SetInterface66712_B18(); 
extern int  GUIDRV_FlexColor_SetInterface66712_B9(); 
extern int  GUIDRV_FlexColor_SetReadFunc66712_B16(); 
extern int  GUIDRV_FlexColor_SetReadFunc66712_B9(); 
extern int  GUIDRV_FlexColor__SetFunc66712(); 
extern int  GUIDRV_FlexColor_SetFunc66709(); 
extern int  GUIDRV_FlexColor_SetReadFunc66709_B16(); 
extern int  GUIDRV_FlexColor_SetFunc66708(); 
extern int  GUIDRV_FlexColor_SetFunc66702(); 
extern int  GUIDRV_FlexColor_SetMode18bppC1B9(); 
extern int  GUIDRV_FlexColor_SetMode18bppC1B18(); 
extern int  GUIDRV_FlexColor_SetMode18bppC0B9(); 
extern int  GUIDRV_FlexColor_SetMode18bppC0B18(); 
extern int  GUIDRV_FlexColor_SetMode16bppC1B8(); 
extern int  GUIDRV_FlexColor_SetMode16bppC1B16(); 
extern int  GUIDRV_FlexColor_SetMode16bppC0B8(); 
extern int  GUIDRV_FlexColor_SetMode16bppC0B16(); 
extern int  GUIDRV_FlexColor_Config(); 
extern int  GUIDRV_FlexColor_SetFunc(); 
extern int  GUIDRV_FlexColor__AddCacheRect(); 
extern int  GUIDRV_FlexColor__ControlCache(); 
extern int  GUIDRV_FlexColor__GetPixelIndexCache(); 
extern int  GUIDRV_FlexColor__GetPixelIndexNoCache(); 
extern int  GUIDRV_FlexColor__IncrementCursor(); 
extern int  GUIDRV_FlexColor__InitOnce(); 
extern int  GUIDRV_FlexColor__ManageRect(); 
extern int  GUIDRV_FlexColor__Refresh(); 
extern int  GUIDRV_FlexColor__SetCacheAddr(); 
extern int  GUIDRV_FlexColor__SetCacheRect(); 
extern int  GUIDRV_FlexColor__SetLog2Phys(); 
extern int  GUIDRV_FlexColor__SetPixelIndexCache(); 
extern int  GUIDRV_FlexColor__SetPixelIndexNoCache(); 
extern int  GUIDRV_FlexColor__SetSubRect(); 
extern int  GUIDRV_Dist_AddDriver(); 
extern int  GUIDRV_DCache_SetMode1bpp(); 
extern int  GUIDRV_DCache_AddDriver(); 
extern int  GUIDRV_DCache__AddDirtyRect(); 
extern int  GUIDRV_DCache__ClearDirtyRect(); 
extern int  GUIDRV_DCache__InitOnce(); 
extern int  GUIDRV_BitPlains_Config(); 
extern int  LCD_UpdateColorIndices(); 
extern int  LCD_SetClipRectEx(); 
extern int  LCD_GetPos(); 
extern int  LCD_GetPosEx(); 
extern int  LCD_Refresh(); 
extern int  LCD_RefreshEx(); 
extern int  LCD_SetAlpha(); 
extern int  LCD_SetAlphaEx(); 
extern int  LCD_SetAlphaMode(); 
extern int  LCD_SetAlphaModeEx(); 
extern int  LCD_SetChroma(); 
extern int  LCD_SetChromaEx(); 
extern int  LCD_SetChromaMode(); 
extern int  LCD_SetChromaModeEx(); 
extern int  LCD_SetDevFunc(); 
extern int  LCD_SetLUTEntry(); 
extern int  LCD_SetLUTEntryEx(); 
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
extern int  LCD_SelectLCD(); 
extern int  LCD__RLE8_SetFunc(); 
extern int  LCD__RLE4_SetFunc(); 
extern int  LCD__RLE32_SetFunc(); 
extern int  LCD__RLE16_SetFunc(); 
extern int  GUI_ReadRect(); 
extern int  GUI_ReadRectEx(); 
extern int  LCD_ReadRect(); 
extern int  LCD_ReadRectNoClip(); 
extern int  LCD_GetNextPixelAPI(); 
extern int  LCD_MixColors256(); 
extern int  LCD_GetDevCap(); 
extern int  LCD_GetDevCapEx(); 
extern int  LCD_Index2ColorEx(); 
extern int  LCD_GetPixelColor(); 
extern int  GUI_GetPixelIndex(); 
extern int  LCD_GetPixelIndex(); 
extern int  LCD_GetNumLayers(); 
extern int  GUI_GetBitsPerPixelEx(); 
extern int  GUI_GetpfColor2IndexEx(); 
extern int  GUI_GetpfIndex2ColorEx(); 
extern int  LCD_GetBitsPerPixel(); 
extern int  LCD_GetBitsPerPixelEx(); 
extern int  LCD_GetBitsPerPixelMax(); 
extern int  LCD_GetMirrorX(); 
extern int  LCD_GetMirrorXEx(); 
extern int  LCD_GetMirrorY(); 
extern int  LCD_GetMirrorYEx(); 
extern int  LCD_GetNumColors(); 
extern int  LCD_GetNumColorsEx(); 
extern int  LCD_GetPalette(); 
extern int  LCD_GetPaletteEx(); 
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
extern int  LCD_GetpfColor2IndexEx(); 
extern int  LCD_GetpfIndex2ColorEx(); 
extern int  LCD__GetBPP(); 
extern int  LCD__GetBPPDevice(); 
extern int  LCD_GetBkColorIndex(); 
extern int  LCD_GetColorIndex(); 
extern int  LCD_ControlCache(); 
extern int  LCD_ControlCacheEx(); 
extern int  LCD_GetpPalConvTable(); 
extern int  LCD_GetpPalConvTableUncached(); 
extern int  LCD_InitColors(); 
extern int  LCD_InitLUT(); 
extern int  LCD_SetMaxNumColors(); 
extern int  LCD_AA_MixColors(); 
extern int  LCD_AA_SetAndMask(); 
extern int  LCD_SetPixelAA(); 
extern int  LCD_SetPixelAA_NoTrans(); 
extern int  LCD_SetPixelAA_Xor(); 
extern int  LCD_Color2Index(); 
extern int  LCD_DrawBitmap(); 
extern int  LCD_DrawHLine(); 
extern int  LCD_DrawPixel(); 
extern int  LCD_DrawVLine(); 
extern int  LCD_FillRect(); 
extern int  LCD_Index2Color(); 
extern int  LCD_Init(); 
extern int  LCD_SetBkColor(); 
extern int  LCD_SetBkColorIndex(); 
extern int  LCD_SetClipRectMax(); 
extern int  LCD_SetColor(); 
extern int  LCD_SetColorIndex(); 
extern int  LCD_SetDrawMode(); 
extern int  LCD__DrawBitmap_1bpp(); 
extern int  GUI__WrapGetNumBytesToNextLine(); 
extern int  GUI__WrapGetNumCharsDisp(); 
extern int  GUI__WrapGetNumCharsToNextLine(); 
extern int  GUI__strlen(); 
extern int  GUI__strcmp(); 
extern int  GUI__strcmp_hp(); 
extern int  GUI__sqrt32(); 
extern int  GUI__CosHQ(); 
extern int  GUI__SinHQ(); 
extern int  GUI__SetText(); 
extern int  GUI__MixColors(); 
extern int  GUI__SetPixelAlpha(); 
extern int  GUI__ReduceRect(); 
extern int  GUI__Read16(); 
extern int  GUI__Read32(); 
extern int  GUI__memset16(); 
extern int  GUI__memset(); 
extern int  GUI__memcpy(); 
extern int  GUI__ManageCache(); 
extern int  GUI__ManageCacheEx(); 
extern int  GUI__IntersectRects(); 
extern int  GUI__IntersectRect(); 
extern int  GUI__HandleEOLine(); 
extern int  GUI__GetNumChars(); 
extern int  GUI__GetFontSizeY(); 
extern int  GUI__GetCursorCharacter(); 
extern int  GUI__GetCursorPosChar(); 
extern int  GUI__GetCursorPosX(); 
extern int  GUI__ExpandPixelIndices(); 
extern int  GUI__ExpandPixelIndicesEx(); 
extern int  GUI__DrawTwinArc2(); 
extern int  GUI__DrawTwinArc4(); 
extern int  GUI__FillTrippleArc(); 
extern int  GUI__DrawBitmap16bpp(); 
extern int  GUI__DivideRound32(); 
extern int  GUI__DivideRound(); 
extern int  GUI__CompactPixelIndices(); 
extern int  GUI__CompactPixelIndicesEx(); 
extern int  GUI__CalcTextRect(); 
extern int  GUI__BIDI_GetCharDir(); 
extern int  GUI__BIDI_GetCharWrap(); 
extern int  GUI__BIDI_GetCursorCharacter(); 
extern int  GUI__BIDI_GetCursorPosChar(); 
extern int  GUI__BIDI_GetCursorPosX(); 
extern int  GUI__BIDI_GetLogChar(); 
extern int  GUI__BIDI_GetWordWrap(); 
extern int  GUI__BIDI_IsNSM(); 
extern int  GUI__BIDI_Log2Vis(); 
extern int  GUI__GetPresentationForm(); 
extern int  GUI__IsArabicCharacter(); 
extern int  GUI__AllocAlphaBuffer(); 
extern int  GUI__DoAlphaBlending(); 
extern int  GUI__GetAlphaBuffer(); 
extern int  GUI__SetAlphaBufferSize(); 
extern int  GUI__AddSpaceHex(); 
extern int  GUI_XBF_CreateFont(); 
extern int  GUI_XBF_DeleteFont(); 
extern int  GUI_XBF__ClearLine(); 
extern int  GUI_XBF__GetCharDistX(); 
extern int  GUI_XBF__GetCharInfo(); 
extern int  GUI_XBF__GetFontInfo(); 
extern int  GUI_XBF__GetOff(); 
extern int  GUI_XBF__GetOffAndSize(); 
extern int  GUI_XBF__IsInFont(); 
extern int  GUI_Warn(); 
extern int  GUI_Warn1(); 
extern int  GUI_Warn2(); 
extern int  GUI_Warn3(); 
extern int  GUI_Warn4(); 
extern int  GUI_WaitKey(); 
extern int  GUI_SetSignalEventFunc(); 
extern int  GUI_SetWaitEventFunc(); 
extern int  GUI_SetWaitEventTimedFunc(); 
extern int  GUI_SignalEvent(); 
extern int  GUI_WaitEvent(); 
extern int  GUI_DispFloat(); 
extern int  GUI_DispFloatFix(); 
extern int  GUI_DispFloatMin(); 
extern int  GUI_DispSFloatFix(); 
extern int  GUI_DispSFloatMin(); 
extern int  GUI_DispDec(); 
extern int  GUI_DispDecAt(); 
extern int  GUI_DispDecMin(); 
extern int  GUI_DispDecShift(); 
extern int  GUI_DispDecSpace(); 
extern int  GUI_DispSDec(); 
extern int  GUI_DispSDecShift(); 
extern int  GUI_UC_SetEncodeUTF8(); 
extern int  GUI_UC_SetEncodeNone(); 
extern int  GUI_UC_EnableBIDI(); 
extern int  GUI__BIDI_Log2VisBuffered(); 
extern int  GUI_UC_DispString(); 
extern int  GUI_UC_ConvertUTF82UC(); 
extern int  GUI_UC_ConvertUC2UTF8(); 
extern int  GUI_DB2UC(); 
extern int  GUI_UC2DB(); 
extern int  GUI_UC_Encode(); 
extern int  GUI_UC_GetCharCode(); 
extern int  GUI_UC_GetCharSize(); 
extern int  GUI_UC__CalcSizeOfChar(); 
extern int  GUI_UC__GetCharCodeInc(); 
extern int  GUI_UC__NumBytes2NumChars(); 
extern int  GUI_UC__NumChars2NumBytes(); 
extern int  GUI_TOUCH_GetUnstable(); 
extern int  GUI_TOUCH_StoreUnstable(); 
extern int  GUI_TOUCH_SetLayer(); 
extern int  GUI_TOUCH_StoreState(); 
extern int  GUI_TOUCH_Calibrate(); 
extern int  GUI_TOUCH_Exec(); 
extern int  GUI_TOUCH_GetCalData(); 
extern int  GUI_TOUCH_GetxPhys(); 
extern int  GUI_TOUCH_GetyPhys(); 
extern int  GUI_TOUCH_SetDefaultCalibration(); 
extern int  GUI_TOUCH_SetOrientation(); 
extern int  GUI_TOUCH_SetOrientationPhys(); 
extern int  GUI_TOUCH_GetState(); 
extern int  GUI_TOUCH_StoreStateEx(); 
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
extern int  GUI_Delay(); 
extern int  GUI_GetTime(); 
extern int  GUITASK_CopyContext(); 
extern int  GUITASK_GetpContext(); 
extern int  GUITASK_Init(); 
extern int  GUITASK_SetMaxTask(); 
extern int  GUI_Lock(); 
extern int  GUI_Unlock(); 
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
extern int  GUI_DrawStreamedBitmap(); 
extern int  GUI_DrawStreamedBitmap24Ex(); 
extern int  GUI_DrawStreamedBitmap555Ex(); 
extern int  GUI_DrawStreamedBitmap565Ex(); 
extern int  GUI_DrawStreamedBitmapAuto(); 
extern int  GUI_DrawStreamedBitmapEx(); 
extern int  GUI_DrawStreamedBitmapExAuto(); 
extern int  GUI_DrawStreamedBitmapM555Ex(); 
extern int  GUI_DrawStreamedBitmapM565Ex(); 
extern int  GUI_GetStreamedBitmapInfo(); 
extern int  GUI_GetStreamedBitmapInfoEx(); 
extern int  GUI_SetStreamedBitmapHook(); 
extern int  GUI__CreateBitmapFromStream(); 
extern int  GUI__ReadHeaderFromStream(); 
extern int  GUI_SPRITE_CreateAnim(); 
extern int  GUI_SPRITE_CreateExAnim(); 
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
extern int  GUI_SIF__ClearLine_ExtFrm(); 
extern int  GUI_SIF__GetCharDistX_ExtFrm(); 
extern int  GUI_SIF__GetCharInfo_ExtFrm(); 
extern int  GUI_SIF__GetFontInfo_ExtFrm(); 
extern int  GUI_SIF__IsInFont_ExtFrm(); 
extern int  GUI_SIF_CreateFont(); 
extern int  GUI_SIF_DeleteFont(); 
extern int  GUI_SIF__GetCharDistX(); 
extern int  GUI_SIF__GetFontInfo(); 
extern int  GUI_SIF__GetNumCharAreas(); 
extern int  GUI_SIF__GetpCharInfo(); 
extern int  GUI_SIF__IsInFont(); 
extern int  GUI_SetTextStyle(); 
extern int  GUI_SetTextMode(); 
extern int  GUI_SetTextAlign(); 
extern int  LCD_SetPixelIndex(); 
extern int  GUI_SetOrientation(); 
extern int  GUI_SetOrientationEx(); 
extern int  GUI_SetOrientationExCached(); 
extern int  GUI_SetOrg(); 
extern int  GUI_SetLineStyle(); 
extern int  GUI_SetLBorder(); 
extern int  GUI_SetFont(); 
extern int  GUI_SetDrawMode(); 
extern int  GL_SetDefault(); 
extern int  GUI_SetDefault(); 
extern int  GUI_GetDecChar(); 
extern int  GUI_SetDecChar(); 
extern int  GUI_SetBkColorIndex(); 
extern int  GUI_SetColorIndex(); 
extern int  GUI_SetBkColor(); 
extern int  GUI_SetColor(); 
extern int  GUI_SetClipRect(); 
extern int  GUI_SetAlpha(); 
extern int  GUI__AlphaSuppressMixing(); 
extern int  GUI_SelectLCD(); 
extern int  GUI_AssignCursorLayer(); 
extern int  GUI_GetCursorLayer(); 
extern int  GUI_GetLayerPosEx(); 
extern int  GUI_SelectLayer(); 
extern int  GUI_SetLayerAlphaEx(); 
extern int  GUI_SetLayerPosEx(); 
extern int  GUI_SetLayerSizeEx(); 
extern int  GUI_SetLayerVisEx(); 
extern int  GUI_GetScreenSizeX(); 
extern int  GUI_GetScreenSizeY(); 
extern int  GUI_SetScreenSizeX(); 
extern int  GUI_SetScreenSizeY(); 
extern int  GUI_RestoreContext(); 
extern int  GUI_SaveContext_W(); 
extern int  GUI_RectsIntersect(); 
extern int  GUI_ALLOC_Realloc(); 
extern int  GUI_RotatePolygon(); 
extern int  GUI_MagnifyPolygon(); 
extern int  GUI_EnlargePolygon(); 
extern int  GUI_PID_GetCurrentState(); 
extern int  GUI_PID_GetState(); 
extern int  GUI_PID_IsEmpty(); 
extern int  GUI_PID_IsPressed(); 
extern int  GUI_PID_StoreState(); 
extern int  GUI_GetPenShape(); 
extern int  GUI_GetPenSize(); 
extern int  GUI_SetPenShape(); 
extern int  GUI_SetPenSize(); 
extern int  GUI_ClearKeyBuffer(); 
extern int  GUI_GetKey(); 
extern int  GUI_GetKeyState(); 
extern int  GUI_PollKeyMsg(); 
extern int  GUI_SendKeyMsg(); 
extern int  GUI_StoreKey(); 
extern int  GUI_StoreKeyMsg(); 
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
extern int  GUI_MoveRect(); 
extern int  GUI_MOUSE_DRIVER_PS2_Init(); 
extern int  GUI_MOUSE_DRIVER_PS2_OnRx(); 
extern int  GUI_MOUSE_GetState(); 
extern int  GUI_MOUSE_StoreState(); 
extern int  GUI_MergeRect(); 
extern int  GUI_Log(); 
extern int  GUI_Log1(); 
extern int  GUI_Log2(); 
extern int  GUI_Log3(); 
extern int  GUI_Log4(); 
extern int  GUI_LANG_GetNumItems(); 
extern int  GUI_LANG_GetText(); 
extern int  GUI_LANG_GetTextEx(); 
extern int  GUI_LANG_LoadCSV(); 
extern int  GUI_LANG_LoadCSVEx(); 
extern int  GUI_LANG_LoadText(); 
extern int  GUI_LANG_LoadTextEx(); 
extern int  GUI_LANG_SetLang(); 
extern int  GUI_LANG_SetMaxNumLang(); 
extern int  GUI_LANG_SetSep(); 
extern int  GUI_JPEG_DrawScaled(); 
extern int  GUI_JPEG_DrawScaledEx(); 
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
extern int  GUI_JOYSTICK_StoreState(); 
extern int  GUI_IsInFont(); 
extern int  GUI_InvertRect(); 
extern int  GUI_InitLUT_C(); 
extern int  GUI_Index2Color(); 
extern int  GUI_GotoX(); 
extern int  GUI_GotoXY(); 
extern int  GUI_GotoY(); 
extern int  GUI_GIF_DrawSubScaled(); 
extern int  GUI_GIF_DrawSubScaledEx(); 
extern int  GUI_GIF_Draw(); 
extern int  GUI_GIF_DrawEx(); 
extern int  GUI_GIF_DrawSub(); 
extern int  GUI_GIF_DrawSubEx(); 
extern int  GUI_GIF_GetComment(); 
extern int  GUI_GIF_GetCommentEx(); 
extern int  GUI_GIF_GetImageInfo(); 
extern int  GUI_GIF_GetImageInfoEx(); 
extern int  GUI_GIF_GetInfo(); 
extern int  GUI_GIF_GetInfoEx(); 
extern int  GUI_GIF_GetXSize(); 
extern int  GUI_GIF_GetXSizeEx(); 
extern int  GUI_GIF_GetYSize(); 
extern int  GUI_GIF_GetYSizeEx(); 
extern int  GUI_GIF_SetFillTrans(); 
extern int  GUI_GIF__DrawFromFilePointer(); 
extern int  GUI_GIF__GetData(); 
extern int  GUI_GIF__GetNextByte(); 
extern int  GUI_GIF__InitLZW(); 
extern int  GUI_GIF__ReadData(); 
extern int  GUI_GetYDistOfFont(); 
extern int  GUI_GetYSizeOfFont(); 
extern int  GUI_GetVersionString(); 
extern int  GUI_GetTextMode(); 
extern int  GUI_GetTextExtend(); 
extern int  GUI_GetTextAlign(); 
extern int  GUI_GetStringDistX(); 
extern int  GUI_GetOrg(); 
extern int  GUI__GetOrg(); 
extern int  GUI_GetLineStyle(); 
extern int  GUI_GetFontSizeY(); 
extern int  GUI_GetFontInfo(); 
extern int  GUI_GetFont(); 
extern int  GUI_GetDrawMode(); 
extern int  GUI_GetDispPosX(); 
extern int  GUI_GetDispPosY(); 
extern int  GUI_GetBkColor(); 
extern int  GUI_GetBkColorIndex(); 
extern int  GUI_GetColor(); 
extern int  GUI_GetColorIndex(); 
extern int  GUI_GetClientRect(); 
extern int  GUI_GetLeadingBlankCols(); 
extern int  GUI_GetTrailingBlankCols(); 
extern int  GUI_GetBitmapPixelIndex(); 
extern int  GUI_GetBitmapPixelIndexEx(); 
extern int  GUI_GetBitmapPixelColor(); 
extern int  GUI_FillRoundedFrame(); 
extern int  GUI_FillRoundedRect(); 
extern int  GUI_FillRectEx(); 
extern int  GUI_FillRect(); 
extern int  GL_FillPolygon(); 
extern int  GUI_FillPolygon(); 
extern int  GUI_Exec(); 
extern int  GUI_Exec1(); 
extern int  GUI_ErrorOut(); 
extern int  GUI_ErrorOut1(); 
extern int  GUI_ErrorOut2(); 
extern int  GUI_ErrorOut3(); 
extern int  GUI_ErrorOut4(); 
extern int  GUI_Error(); 
extern int  GUIPROP_EXT__ClearLine(); 
extern int  GUIPROP_EXT__DispLine(); 
extern int  GUIPROP_EXT__SetfpClearLine(); 
extern int  GUI_EnableAlpha(); 
extern int  GUI_RestoreUserAlpha(); 
extern int  GUI_SetUserAlpha(); 
extern int  GUI_DrawVLine(); 
extern int  GUI_DrawRoundedRect(); 
extern int  GUI_DrawRoundedFrame(); 
extern int  GUI_DrawRectEx(); 
extern int  GUI_DrawPolyLine(); 
extern int  GL_DrawPolygon(); 
extern int  GUI_DrawPolygon(); 
extern int  GL_DrawPoint(); 
extern int  GUI_DrawPoint(); 
extern int  GUI_DrawPixel(); 
extern int  GUI_DrawPie(); 
extern int  GL_DrawLine(); 
extern int  GL_DrawLine1(); 
extern int  GL_DrawLine1Ex(); 
extern int  GL_DrawLineEx(); 
extern int  GL_DrawLineTo(); 
extern int  GL_DrawLineToEx(); 
extern int  GUI_DrawLine(); 
extern int  GUI_DrawLineRel(); 
extern int  GUI_DrawLineTo(); 
extern int  GUI_cos(); 
extern int  GUI_sin(); 
extern int  GUI_DrawHLine(); 
extern int  GUI_DrawGraphEx(); 
extern int  GUI_DrawGraph(); 
extern int  GUI_DrawGradientRoundedH(); 
extern int  GUI_DrawGradientRoundedV(); 
extern int  GUI_DrawGradientH(); 
extern int  GUI_DrawGradientV(); 
extern int  GUI_DrawFocusRect(); 
extern int  GUI_DrawBitmapMag(); 
extern int  GUI_DrawBitmapHWAlpha(); 
extern int  GUI_DrawBitmapExp(); 
extern int  GUI_DrawBitmapEx(); 
extern int  GL_DrawBitmap(); 
extern int  GUI_DrawBitmap(); 
extern int  GUI_DispStringLen(); 
extern int  GUI_DispStringInRectWrap(); 
extern int  GUI_WrapGetNumLines(); 
extern int  GUI_DispStringInRectEx(); 
extern int  GUI_DispStringInRect(); 
extern int  GUI_DispStringInRectMax(); 
extern int  GUI__DispStringInRect(); 
extern int  GUI_DispStringHCenterAt(); 
extern int  GUI_DispStringAtCEOL(); 
extern int  GUI_DispStringAt(); 
extern int  GUI_DispString(); 
extern int  GUI_DispHex(); 
extern int  GUI_DispHexAt(); 
extern int  GUI_DispChars(); 
extern int  GUI_DispChar(); 
extern int  GUI_DispCharAt(); 
extern int  GUI_DispCEOL(); 
extern int  GUI_DispBin(); 
extern int  GUI_DispBinAt(); 
extern int  GUI_DIRTYDEVICE_Create(); 
extern int  GUI_DIRTYDEVICE_Delete(); 
extern int  GUI_DIRTYDEVICE_Fetch(); 
extern int  GUI_DEVICE_Create(); 
extern int  GUI_DEVICE_CreateAndLink(); 
extern int  GUI_DEVICE_Delete(); 
extern int  GUI_DEVICE_Link(); 
extern int  GUI_DEVICE_LinkDevices(); 
extern int  GUI_DEVICE_Unlink(); 
extern int  GUI_DEVICE_UnlinkTaskDevices(); 
extern int  GUI_DEVICE__GetpDevice(); 
extern int  GUI_DEVICE__GetpDriver(); 
extern int  GUI_CURSOR_SelectAnim(); 
extern int  GUI_CURSOR_SelectAnimEx(); 
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
extern int  GUI_CURSOR__GetSpriteEx(); 
extern int  GUI_CURSOR__SetSpriteEx(); 
extern int  GUI_Clear(); 
extern int  GUI_ClearRect(); 
extern int  GUI_Exit(); 
extern int  GUI_Init(); 
extern int  GUI_SetDefaultFont(); 
extern int  GUI__Config(); 
extern int  GUI__RegisterExit(); 
extern int  GUI_CopyRect(); 
extern int  GUI_CalcVisColorError(); 
extern int  GUI_Color2VisColor(); 
extern int  GUI_ColorIsAvailable(); 
extern int  GUI_Color2Index(); 
extern int  GUI_ClearRectEx(); 
extern int  GL_DrawCircle(); 
extern int  GL_DrawEllipse(); 
extern int  GL_FillCircle(); 
extern int  GL_FillEllipse(); 
extern int  GUI_DrawCircle(); 
extern int  GUI_DrawEllipse(); 
extern int  GUI_FillCircle(); 
extern int  GUI_FillEllipse(); 
extern int  GUIPROP_FRM_DispChar(); 
extern int  GUIPROP_FRM_GetCharDistX(); 
extern int  GUIPROP_FRM_GetFontInfo(); 
extern int  GUIPROP_FRM_IsInFont(); 
extern int  GUIPROP_EXT_DispChar(); 
extern int  GUIPROP_EXT_GetCharDistX(); 
extern int  GUIPROP_EXT_GetCharInfo(); 
extern int  GUIPROP_EXT_GetFontInfo(); 
extern int  GUIPROP_EXT_IsInFont(); 
extern int  GUIPROP_EXT__FindChar(); 
extern int  GUIPROP_DispChar(); 
extern int  GUIPROP_GetCharDistX(); 
extern int  GUIPROP_GetFontInfo(); 
extern int  GUIPROP_IsInFont(); 
extern int  GUIPROP__FindChar(); 
extern int  GUIMONO_DispChar(); 
extern int  GUIMONO_GetCharDistX(); 
extern int  GUIMONO_GetFontInfo(); 
extern int  GUIMONO_IsInFont(); 
extern int  GUI__DispLine(); 
extern int  GUI__GetLineDistX(); 
extern int  GUI__GetLineNumChars(); 
extern int  GUI__GetOverlap(); 
extern int  GL_DispChar(); 
extern int  GUI_DispNextLine(); 
extern int  GUI_GetCharDistX(); 
extern int  GUI_GetCharDistXEx(); 
extern int  GUI_GetFontDistY(); 
extern int  GUI_GetYAdjust(); 
extern int  GUI_CalcColorDist(); 
extern int  GUI_BMP_Serialize(); 
extern int  GUI_BMP_SerializeEx(); 
extern int  GUI_BMP_SerializeExBpp(); 
extern int  GUI_BMP_EnableAlpha(); 
extern int  GUI_BMP_DrawScaled(); 
extern int  GUI_BMP_DrawScaledEx(); 
extern int  GUI_BMP_DisableAlpha(); 
extern int  GUI_BMP_Draw(); 
extern int  GUI_BMP_DrawEx(); 
extern int  GUI_BMP_GetXSize(); 
extern int  GUI_BMP_GetXSizeEx(); 
extern int  GUI_BMP_GetYSize(); 
extern int  GUI_BMP_GetYSizeEx(); 
extern int  GUI_BMP__GetData(); 
extern int  GUI_BMP__Init(); 
extern int  GUI_BMP__ReadData(); 
extern int  GUI_BMP__ReadPalette(); 
extern int  GL_DrawArc(); 
extern int  GUI_DrawArc(); 
extern int  GUI_ALLOC_AllocZero(); 
extern int  GUI_ALLOC_AllocInit(); 
extern int  GUI_ALLOC_AllocNoInit(); 
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
extern int  GUI_ALLOC_RequestSize(); 
extern int  GUI_ALLOC_SetAvBlockSize(); 
extern int  GUI_ALLOC_SetMaxPercentage(); 
extern int  GUI_ALLOC_Unlock(); 
extern int  GUI_ALLOC_UnlockH(); 
extern int  GUI_ALLOC_h2p(); 
extern int  GUI_ALLOC_p2h(); 
extern int  GUI_AddHex(); 
extern int  GUI_AddDecShift(); 
extern int  GUI_AddSign(); 
extern int  GUI_Long2Len(); 
extern int  GUI_AddDecMin(); 
extern int  GUI_AddDec(); 
extern int  GUI_AddBin(); 
extern int  GL_MoveTo(); 
extern int  GUI_DrawRect(); 
extern int  GUI_MoveRel(); 
extern int  GUI_MoveTo(); 
extern int  LCD_Index2Color_M888(); 
extern int  LCD_Index2Color_M565(); 
extern int  LCD_Index2Color_M555(); 
extern int  LCD_Index2Color_888(); 
extern int  LCD_Index2Color_8888(); 
extern int  LCD_Index2Color_565(); 
extern int  LCD_Index2Color_555(); 
extern int  LCD_SetLUT(); 
extern int  LCD_SetLUTEx(); 
extern int  LCD_Color2Index_8666(); 
extern int  LCD_Index2Color_8666(); 
extern int  GUI_AA_DrawPolyOutline(); 
extern int  GUI_AA_DrawPolyOutlineEx(); 
extern int  GUI_AA_FillPolygon(); 
extern int  GUI_AA_DrawLine(); 
extern int  GUI_AA_DisableHiRes(); 
extern int  GUI_AA_EnableHiRes(); 
extern int  GUI_AA_Exit(); 
extern int  GUI_AA_GetFactor(); 
extern int  GUI_AA_HiRes2Pixel(); 
extern int  GUI_AA_Init(); 
extern int  GUI_AA_Init_HiRes(); 
extern int  GUI_AA_SetDrawMode(); 
extern int  GUI_AA_SetFactor(); 
extern int  GUI_AA_FillRoundedRect(); 
extern int  GUI_AA_DrawRoundedRect(); 
extern int  GL_FillCircleAA_HiRes(); 
extern int  GUI_AA_FillCircle(); 
extern int  GUIPROP_AA4_EXT_DispChar(); 
extern int  GUIPROP_AA4_DispChar(); 
extern int  GUIPROP_AA4_GetCharDistX(); 
extern int  GUIPROP_AA4_GetFontInfo(); 
extern int  GUIPROP_AA4_IsInFont(); 
extern int  GUI_AA__DrawCharAA4(); 
extern int  GUIPROP_AA2_EXT_DispChar(); 
extern int  GUIPROP_AA2_DispChar(); 
extern int  GUIPROP_AA2_GetCharDistX(); 
extern int  GUIPROP_AA2_GetFontInfo(); 
extern int  GUIPROP_AA2_IsInFont(); 
extern int  GUI_AA__DrawCharAA2(); 
extern int  GUIPROPAA_DispChar(); 
extern int  GUIPROPAA_GetCharDistX(); 
extern int  GUIPROPAA_GetFontInfo(); 
extern int  GUIPROPAA_IsInFont(); 
extern int  GUI_AA_DrawArc(); 
extern int  WINDOW__DefaultBkColor; 
extern int  _pEffectDefault; 
extern int  TREEVIEW__DefaultProps; 
extern int  TEXT__DefaultProps; 
extern int  SPINBOX__Skin; 
extern int  SPINBOX__DefaultProps; 
extern int  SPINBOX__pSkinDefault; 
extern int  SLIDER__Skin; 
extern int  SLIDER__DefaultProps; 
extern int  SLIDER__pSkinDefault; 
extern int  SCROLLBAR__Skin; 
extern int  SCROLLBAR__DefaultProps; 
extern int  SCROLLBAR__DefaultWidth; 
extern int  SCROLLBAR__ThumbSizeMin; 
extern int  SCROLLBAR__pSkinDefault; 
extern int  RADIO__Skin; 
extern int  RADIO__DefaultProps; 
extern int  RADIO__pSkinDefault; 
extern int  PROGBAR__Skin; 
extern int  PROGBAR__DefaultProps; 
extern int  PROGBAR__pSkinDefault; 
extern int  MULTIPAGE__DefaultProps; 
extern int  MULTIPAGE__aEffectColor; 
extern int  MENU__DefaultProps; 
extern int  MENU__pDefaultEffect; 
extern int  LISTWHEEL_DefaultProps; 
extern int  LISTVIEW_DefaultProps; 
extern int  LISTBOX_DefaultProps; 
extern int  ICONVIEW__DefaultProps; 
extern int  ICONVIEW__pfDrawStreamedBitmap; 
extern int  HEADER__Skin; 
extern int  HEADER__DefaultBorderH; 
extern int  HEADER__DefaultProps; 
extern int  HEADER__pDefaultCursor; 
extern int  HEADER__pSkinDefault; 
extern int  _Props; 
extern int  GRAPH__DefaultProps; 
extern int  FRAMEWIN__Skin; 
extern int  FRAMEWIN__DefaultProps; 
extern int  FRAMEWIN__pSkinDefault; 
extern int  EDIT__DefaultProps; 
extern int  DROPDOWN__Skin; 
extern int  DROPDOWN__DefaultProps; 
extern int  DROPDOWN__pSkinDefault; 
extern int  CHECKBOX__Skin; 
extern int  CHECKBOX__DefaultProps; 
extern int  CHECKBOX__pSkinDefault; 
extern int  BUTTON__Skin; 
extern int  BUTTON__DefaultProps; 
extern int  BUTTON__pSkinDefault; 
extern int  LCD_APIListCW; 
extern int  LCD_APIListCCW; 
extern int  LCD_APIList180; 
extern int  LCD__pSetColorAPI; 
extern int  GUI__Wrap_pfGetCharWrap; 
extern int  GUI__Wrap_pfGetWordWrap; 
extern int  GUI__GetCursorPos_pfGetCharacter; 
extern int  GUI__GetCursorPos_pfGetPosChar; 
extern int  GUI__GetCursorPos_pfGetPosX; 
extern int  GUI__pFontDefault; 
extern int  GUI_pContext; 
extern int  GUI_CharLine_pfLog2Vis; 
extern int  GUI__pfDrawLine_RGB; 
extern int  IMAGE__DefaultProps; 
extern int  HEADER__DefaultBorderV; 
extern int  GUITASK__EntranceCnt; 
extern int  WIDGET_Effect_Simple; 
extern int  WIDGET_Effect_None; 
extern int  WIDGET_Effect_3D2L; 
extern int  WIDGET_Effect_3D1L; 
extern int  WIDGET_Effect_3D; 
extern int  TREEVIEW__bmClosed; 
extern int  TREEVIEW__bmLeaf; 
extern int  TREEVIEW__bmMinus; 
extern int  TREEVIEW__bmOpen; 
extern int  TREEVIEW__bmPlus; 
extern int  SPINBOX__SkinClassic; 
extern int  SLIDER__SkinClassic; 
extern int  SCROLLBAR__SkinClassic; 
extern int  RADIO__SkinClassic; 
extern int  RADIO__abmRadio; 
extern int  RADIO__bmCheck; 
extern int  PROGBAR__SkinClassic; 
extern int  HEADER__SkinClassic; 
extern int  FRAMEWIN__SkinClassic; 
extern int  DROPDOWN__SkinClassic; 
extern int  CHECKBOX__SkinClassic; 
extern int  CHECKBOX__abmCheck; 
extern int  BUTTON__SkinClassic; 
extern int  _VNC_API; 
extern int  GUI_MEMDEV_DEVICE_8; 
extern int  GUI_MEMDEV_DEVICE_32; 
extern int  GUI_MEMDEV_DEVICE_16; 
extern int  GUI_MEMDEV_DEVICE_1; 
extern int  GUI_FontD80; 
extern int  GUI_FontD64; 
extern int  GUI_FontD60x80; 
extern int  GUI_FontD48x64; 
extern int  GUI_FontD48; 
extern int  GUI_FontD36x48; 
extern int  GUI_FontD32; 
extern int  GUI_FontD24x32; 
extern int  CharInfo24B_ASCII; 
extern int  FontComic24BProp_ASCII; 
extern int  GUI_FontComic24B_ASCII; 
extern int  GUI_FontComic24B_1; 
extern int  CharInfo18B_ASCII; 
extern int  GUI_FontComic18B_ASCII; 
extern int  GUI_FontComic18B_1; 
extern int  GUI_Font8x8ASCII_Prop0; 
extern int  GUI_Font8x8_ASCII; 
extern int  GUI_Font8x8_1; 
extern int  GUI_Font8x8; 
extern int  GUI_Font8x9; 
extern int  GUI_Font8x16ASCII_Prop0; 
extern int  GUI_Font8x16_ASCII; 
extern int  GUI_Font8x16_1; 
extern int  GUI_Font8x16; 
extern int  GUI_Font8x16x1x2; 
extern int  GUI_Font8x16x2x2; 
extern int  GUI_Font8x16x3x3; 
extern int  GUI_Font8x17; 
extern int  GUI_Font8x18; 
extern int  GUI_CharInfoF8x15B_ASCII; 
extern int  GUI_Font8x15B_ASCII; 
extern int  GUI_Font8x15B_1; 
extern int  GUI_CharInfoF8x13_ASCII; 
extern int  GUI_Font8x13_ASCII; 
extern int  GUI_Font8x13_1; 
extern int  GUI_Font8x12_ASCII; 
extern int  GUI_Font8x10_ASCII; 
extern int  GUI_Font6x8ASCII_Prop0; 
extern int  GUI_Font6x8_ASCII; 
extern int  GUI_CharInfoF6x8_1; 
extern int  GUI_Font6x8_1; 
extern int  GUI_Font6x8; 
extern int  GUI_Font6x9; 
extern int  GUI_Font4x6; 
extern int  GUI_Font32_ASCII; 
extern int  GUI_Font32_ASCII_CharInfo; 
extern int  GUI_Font32_1; 
extern int  GUI_Font32B_ASCII; 
extern int  GUI_Font32B_ASCII_CharInfo; 
extern int  GUI_Font32B_1; 
extern int  GUI_Font24_ASCII; 
extern int  GUI_Font24_ASCII_CharInfo; 
extern int  GUI_Font24_1; 
extern int  GUI_Font24B_ASCII; 
extern int  GUI_Font24B_ASCII_CharInfo; 
extern int  GUI_Font24B_1; 
extern int  GUI_Font20_ASCII; 
extern int  GUI_Font20_ASCII_CharInfo; 
extern int  GUI_Font20_ASCII_Prop1; 
extern int  GUI_Font20_1; 
extern int  GUI_Font20F_ASCII; 
extern int  GUI_Font20B_ASCII; 
extern int  GUI_Font20B_ASCII_CharInfo; 
extern int  GUI_Font20B_ASCII_Prop1; 
extern int  GUI_Font20B_1; 
extern int  GUI_Font16_HK; 
extern int  GUI_Font16_HK_CharInfo; 
extern int  GUI_Font16ASCIIProp1; 
extern int  GUI_Font16_ASCII; 
extern int  GUI_Font16_1HK; 
extern int  GUI_Font16_1; 
extern int  GUI_Font16_1_FontProp1; 
extern int  GUI_Font16B_ASCII; 
extern int  GUI_Font16B_ASCII_CharInfo; 
extern int  GUI_Font16B_1; 
extern int  GUI_Font13ASCII_Prop1; 
extern int  GUI_Font13_ASCII; 
extern int  GUI_Font13_1; 
extern int  GUI_Font13H_ASCII; 
extern int  GUI_Font13H_ASCII_CharInfo; 
extern int  GUI_Font13H_1; 
extern int  GUI_Font13HB_ASCII; 
extern int  GUI_Font13HB_ASCII_CharInfo; 
extern int  GUI_Font13HB_1; 
extern int  GUI_Font13B_ASCII; 
extern int  GUI_Font13B_ASCII_Prop1; 
extern int  GUI_Font13B_1; 
extern int  GUI_Font10ASCIIProp1; 
extern int  GUI_Font10_ASCII; 
extern int  GUI_Font10_1; 
extern int  GUI_Font10S_ASCII; 
extern int  GUI_Font10S_ASCII_FontProp1; 
extern int  GUI_Font10S_1; 
extern int  GUI_Font8ASCII_Prop; 
extern int  GUI_Font8_ASCII; 
extern int  GUI_Font8_1; 
extern int  GUIDRV_TEMPLATE_I_16_API; 
extern int  GUIDRV_TEMPLATE_I_OSXY_16_API; 
extern int  GUIDRV_TEMPLATE_I_OSX_16_API; 
extern int  GUIDRV_TEMPLATE_I_OSY_16_API; 
extern int  GUIDRV_TEMPLATE_I_OS_16_API; 
extern int  GUIDRV_TEMPLATE_I_OXY_16_API; 
extern int  GUIDRV_TEMPLATE_I_OX_16_API; 
extern int  GUIDRV_TEMPLATE_I_OY_16_API; 
extern int  GUIDRV_Template_API; 
extern int  GUIDRV_SSD1926_OSXY_8_API; 
extern int  GUIDRV_SSD1926_OSX_8_API; 
extern int  GUIDRV_SSD1926_OSY_8_API; 
extern int  GUIDRV_SSD1926_OS_8_API; 
extern int  GUIDRV_SSD1926_OXY_8_API; 
extern int  GUIDRV_SSD1926_OX_8_API; 
extern int  GUIDRV_SSD1926_OY_8_API; 
extern int  GUIDRV_SSD1926_8_API; 
extern int  GUIDRV_SPage_OSXY_4C1_API; 
extern int  GUIDRV_SPage_OSX_4C1_API; 
extern int  GUIDRV_SPage_OSY_4C1_API; 
extern int  GUIDRV_SPage_OS_4C1_API; 
extern int  GUIDRV_SPage_OXY_4C1_API; 
extern int  GUIDRV_SPage_OX_4C1_API; 
extern int  GUIDRV_SPage_OY_4C1_API; 
extern int  GUIDRV_SPage_OSXY_4C0_API; 
extern int  GUIDRV_SPage_OSX_4C0_API; 
extern int  GUIDRV_SPage_OSY_4C0_API; 
extern int  GUIDRV_SPage_OS_4C0_API; 
extern int  GUIDRV_SPage_OXY_4C0_API; 
extern int  GUIDRV_SPage_OX_4C0_API; 
extern int  GUIDRV_SPage_OY_4C0_API; 
extern int  GUIDRV_SPage_OSXY_2C1_API; 
extern int  GUIDRV_SPage_OSX_2C1_API; 
extern int  GUIDRV_SPage_OSY_2C1_API; 
extern int  GUIDRV_SPage_OS_2C1_API; 
extern int  GUIDRV_SPage_OXY_2C1_API; 
extern int  GUIDRV_SPage_OX_2C1_API; 
extern int  GUIDRV_SPage_OY_2C1_API; 
extern int  GUIDRV_SPage_OSXY_2C0_API; 
extern int  GUIDRV_SPage_OSX_2C0_API; 
extern int  GUIDRV_SPage_OSY_2C0_API; 
extern int  GUIDRV_SPage_OS_2C0_API; 
extern int  GUIDRV_SPage_OXY_2C0_API; 
extern int  GUIDRV_SPage_OX_2C0_API; 
extern int  GUIDRV_SPage_OY_2C0_API; 
extern int  GUIDRV_SPage_OSXY_1C1_API; 
extern int  GUIDRV_SPage_OSX_1C1_API; 
extern int  GUIDRV_SPage_OSY_1C1_API; 
extern int  GUIDRV_SPage_OS_1C1_API; 
extern int  GUIDRV_SPage_OXY_1C1_API; 
extern int  GUIDRV_SPage_OX_1C1_API; 
extern int  GUIDRV_SPage_OY_1C1_API; 
extern int  GUIDRV_SPage_OSXY_1C0_API; 
extern int  GUIDRV_SPage_OSX_1C0_API; 
extern int  GUIDRV_SPage_OSY_1C0_API; 
extern int  GUIDRV_SPage_OS_1C0_API; 
extern int  GUIDRV_SPage_OXY_1C0_API; 
extern int  GUIDRV_SPage_OX_1C0_API; 
extern int  GUIDRV_SPage_OY_1C0_API; 
extern int  GUIDRV_SPage_4C1_API; 
extern int  GUIDRV_SPage_4C0_API; 
extern int  GUIDRV_SPage_2C1_API; 
extern int  GUIDRV_SPage_2C0_API; 
extern int  GUIDRV_SPage_1C1_API; 
extern int  GUIDRV_SPage_1C0_API; 
extern int  GUIDRV_SLin_OSXY_2_API; 
extern int  GUIDRV_SLin_OSX_2_API; 
extern int  GUIDRV_SLin_OSY_2_API; 
extern int  GUIDRV_SLin_OS_2_API; 
extern int  GUIDRV_SLin_OXY_2_API; 
extern int  GUIDRV_SLin_OX_2_API; 
extern int  GUIDRV_SLin_OY_2_API; 
extern int  GUIDRV_SLin_OSXY_1_API; 
extern int  GUIDRV_SLin_OSX_1_API; 
extern int  GUIDRV_SLin_OSY_1_API; 
extern int  GUIDRV_SLin_OS_1_API; 
extern int  GUIDRV_SLin_OXY_1_API; 
extern int  GUIDRV_SLin_OX_1_API; 
extern int  GUIDRV_SLin_OY_1_API; 
extern int  GUIDRV_SLin_2_API; 
extern int  GUIDRV_SLin_1_API; 
extern int  GUIDRV_S1D15G00_API; 
extern int  GUIDRV_S1D13781_8C0_API; 
extern int  GUIDRV_S1D13781_OSX_8C0_API; 
extern int  GUIDRV_S1D13781_OSY_8C0_API; 
extern int  GUIDRV_S1D13781_OXY_8C0_API; 
extern int  GUIDRV_S1D13748_OSXY_16_API; 
extern int  GUIDRV_S1D13748_OSX_16_API; 
extern int  GUIDRV_S1D13748_OSY_16_API; 
extern int  GUIDRV_S1D13748_OS_16_API; 
extern int  GUIDRV_S1D13748_OXY_16_API; 
extern int  GUIDRV_S1D13748_OX_16_API; 
extern int  GUIDRV_S1D13748_OY_16_API; 
extern int  GUIDRV_S1D13748_16_API; 
extern int  GUIDRV_Lin_OY_32_API; 
extern int  GUIDRV_Lin_OY_24_API; 
extern int  GUIDRV_Lin_OY_16_API; 
extern int  GUIDRV_Lin_OX_8_API; 
extern int  GUIDRV_Lin_OX_32_API; 
extern int  GUIDRV_Lin_OX_24_API; 
extern int  GUIDRV_Lin_OX_16_API; 
extern int  GUIDRV_Lin_OXY_8_API; 
extern int  GUIDRV_Lin_OXY_32_API; 
extern int  GUIDRV_Lin_OXY_24_API; 
extern int  GUIDRV_Lin_OXY_16_API; 
extern int  GUIDRV_Lin_OS_32_API; 
extern int  GUIDRV_Lin_OS_24_API; 
extern int  GUIDRV_Lin_OS_16_API; 
extern int  GUIDRV_Lin_OSY_32_API; 
extern int  GUIDRV_Lin_OSY_24_API; 
extern int  GUIDRV_Lin_OSY_16_API; 
extern int  GUIDRV_Lin_OSX_32_API; 
extern int  GUIDRV_Lin_OSX_24_API; 
extern int  GUIDRV_Lin_OSX_16_API; 
extern int  GUIDRV_Lin_8_API; 
extern int  GUIDRV_Lin_4_API; 
extern int  GUIDRV_Lin_32_API; 
extern int  GUIDRV_Lin_24_API; 
extern int  GUIDRV_Lin_2_API; 
extern int  GUIDRV_Lin_16_API; 
extern int  GUIDRV_Lin_1_API; 
extern int  GUIDRV_IST3088_OSXY_4_API; 
extern int  GUIDRV_IST3088_OSX_4_API; 
extern int  GUIDRV_IST3088_OSY_4_API; 
extern int  GUIDRV_IST3088_OS_4_API; 
extern int  GUIDRV_IST3088_OXY_4_API; 
extern int  GUIDRV_IST3088_OX_4_API; 
extern int  GUIDRV_IST3088_OY_4_API; 
extern int  GUIDRV_IST3088_4_API; 
extern int  GUIDRV_FlexColor_API; 
extern int  GUIDRV_Dist_API; 
extern int  GUIDRV_DCache_API; 
extern int  GUIDRV_BitPlains_API; 
extern int  GUI_BitmapMethodsRLEAlpha; 
extern int  GUI_BitmapMethodsRLE8; 
extern int  GUI_BitmapMethodsRLE8Ex; 
extern int  GUI_BitmapMethodsRLE4; 
extern int  GUI_BitmapMethodsRLE4Ex; 
extern int  GUI_BitmapMethodsRLE32; 
extern int  GUI_BitmapMethodsRLE32Ex; 
extern int  GUI_BitmapMethodsRLE16; 
extern int  GUI_BitmapMethodsRLE16Ex; 
extern int  GUI_BitmapMethodsRLEM16; 
extern int  GUI_BitmapMethodsRLEM16Ex; 
extern int  LCD_aMirror; 
extern int  GUI__aConvert_63_255; 
extern int  GUI__aConvert_31_255; 
extern int  GUI__aConvert_255_63; 
extern int  GUI__aConvert_255_31; 
extern int  GUI_XBF_APIList_Prop_Frm; 
extern int  GUI_XBF_APIList_Prop_Ext; 
extern int  GUI_XBF_APIList_Prop; 
extern int  GUI_UC_None; 
extern int  GUI_SIF_APIList_Prop_Frm; 
extern int  GUI_SIF_APIList_Prop_Ext; 
extern int  GUI_SIF_APIList_Prop; 
extern int  GUI_OrientationAPI_C8; 
extern int  GUI_OrientationAPI_C32; 
extern int  GUI_OrientationAPI_C16; 
extern int  GUI_OrientationAPI_C0; 
extern int  GUIDRV_Rotate_API; 
extern int  GUI_GIF__aInterlaceOffset; 
extern int  GUI_GIF__aInterlaceYPos; 
extern int  GUI_ENC_APIList_SJIS; 
extern int  GUI_ENC_APIList_EXT; 
extern int  aTan; 
extern int  GUI_BitmapMethods8888; 
extern int  GUI_BitmapMethods888; 
extern int  GUI_BitmapMethodsM888; 
extern int  GUI_BitmapMethods565; 
extern int  GUI_BitmapMethodsM565; 
extern int  GUI_BitmapMethods555; 
extern int  GUI_BitmapMethodsM555; 
extern int  GUI_BitmapMethods24; 
extern int  _DirtyDevice_API; 
extern int  GUI_CursorPalI; 
extern int  GUI_CursorPal; 
extern int  GUI_PixelsHeaderM; 
extern int  GUI_CursorHeaderMI; 
extern int  GUI_CursorHeaderM; 
extern int  GUI_Pixels_CrossS; 
extern int  GUI_BitmapCrossSI; 
extern int  GUI_CursorCrossSI; 
extern int  GUI_BitmapCrossS; 
extern int  GUI_CursorCrossS; 
extern int  GUI_Pixels_CrossM; 
extern int  GUI_BitmapCrossMI; 
extern int  GUI_CursorCrossMI; 
extern int  GUI_BitmapCrossM; 
extern int  GUI_CursorCrossM; 
extern int  GUI_Pixels_CrossL; 
extern int  GUI_BitmapCrossLI; 
extern int  GUI_CursorCrossLI; 
extern int  GUI_BitmapCrossL; 
extern int  GUI_CursorCrossL; 
extern int  GUI_Pixels_ArrowS; 
extern int  GUI_BitmapArrowSI; 
extern int  GUI_CursorArrowSI; 
extern int  GUI_BitmapArrowS; 
extern int  GUI_CursorArrowS; 
extern int  GUI_Pixels_ArrowM; 
extern int  GUI_BitmapArrowMI; 
extern int  GUI_CursorArrowMI; 
extern int  GUI_BitmapArrowM; 
extern int  GUI_CursorArrowM; 
extern int  GUI_Pixels_ArrowL; 
extern int  GUI_BitmapArrowLI; 
extern int  GUI_CursorArrowLI; 
extern int  GUI_BitmapArrowL; 
extern int  GUI_CursorArrowL; 
extern int  GUI_CursorAnimHourglassM; 
extern int  GUI_Pow10; 
extern int  LCD_API_ColorConv_M8888; 
extern int  LCD_API_ColorConv_M888; 
extern int  LCD_API_ColorConv_1_16; 
extern int  LCD_API_ColorConv_1_2; 
extern int  LCD_API_ColorConv_1_24; 
extern int  LCD_API_ColorConv_1_4; 
extern int  LCD_API_ColorConv_1_5; 
extern int  LCD_API_ColorConv_1_8; 
extern int  LCD_API_ColorConv_8888; 
extern int  LCD_API_ColorConv_888; 
extern int  LCD_API_ColorConv_1; 
extern int  LCD_API_ColorConv_0; 
extern int  LCD_API_ColorConv_5; 
extern int  LCD_API_ColorConv_4; 
extern int  LCD_API_ColorConv_2; 
extern int  LCD_API_ColorConv_M8565; 
extern int  LCD_API_ColorConv_M666_9; 
extern int  LCD_API_ColorConv_M666; 
extern int  LCD_API_ColorConv_M655; 
extern int  LCD_API_ColorConv_M565; 
extern int  LCD_API_ColorConv_M556; 
extern int  LCD_API_ColorConv_M555; 
extern int  LCD_API_ColorConv_M444_16; 
extern int  LCD_API_ColorConv_M444_12; 
extern int  LCD_API_ColorConv_M332; 
extern int  LCD_API_ColorConv_M323; 
extern int  LCD_API_ColorConv_M233; 
extern int  LCD_API_ColorConv_8666_1; 
extern int  LCD_API_ColorConv_8666; 
extern int  LCD_API_ColorConv_84444; 
extern int  LCD_API_ColorConv_822216; 
extern int  LCD_API_ColorConv_666_9; 
extern int  LCD_API_ColorConv_666; 
extern int  LCD_API_ColorConv_655; 
extern int  LCD_API_ColorConv_565; 
extern int  LCD_API_ColorConv_556; 
extern int  LCD_API_ColorConv_555; 
extern int  LCD_API_ColorConv_444_16; 
extern int  LCD_API_ColorConv_444_12_1; 
extern int  LCD_API_ColorConv_444_12; 
extern int  LCD_API_ColorConv_332; 
extern int  LCD_API_ColorConv_323; 
extern int  LCD_API_ColorConv_233; 
extern int  LCD_API_ColorConv_222; 
extern int  LCD_API_ColorConv_M222; 
extern int  LCD_API_ColorConv_16; 
extern int  LCD_API_ColorConv_111; 
extern int  LCD_API_ColorConv_M111; 
extern int  GUI_SIF_APIList_Prop_AA4_EXT; 
extern int  GUI_SIF_APIList_Prop_AA4; 
extern int  GUI_SIF_APIList_Prop_AA2_EXT; 
extern int  GUI_SIF_APIList_Prop_AA2; 
extern int  GUI_XBF_APIList_Prop_AA4_Ext; 
extern int  GUI_XBF_APIList_Prop_AA2_Ext; 
extern int  WM__aCHWinLast; 
extern int  WM__aCHWinModal; 
extern int  WM__aCHWinMouseOver; 
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
extern int  WM__ahCapture; 
extern int  WM__ahDesktopWin; 
extern int  WM__ahWinFocus; 
extern int  WM__hATransWindow; 
extern int  WM__pFirstCriticalHandle; 
extern int  WM_pfPollPID; 
extern int  RADIO__pfHandleSetValue; 
extern int  GUI_MEMDEV__pCbAnimation; 
extern int  GUI_MEMDEV__pVoid; 
extern int  LCD__aConvTable; 
extern int  GUI__BIDI_Enabled; 
extern int  _pfGUI__BIDI_GetCharDir; 
extern int  _pfGUI__BIDI_GetCursorPosChar; 
extern int  _pfGUI__BIDI_GetCursorPosX; 
extern int  _pfGUI__BIDI_GetLogChar; 
extern int  _pfGUI__BIDI_IsNSM; 
extern int  _pfGUI__BIDI_Log2Vis; 
extern int  GUI__CharHasTrans; 
extern int  GUI_RectDispString; 
extern int  GUI_DecChar; 
extern int  GUI_OrgX; 
extern int  GUI_OrgY; 
extern int  GUI__apDevice; 
extern int  GUI_pLCD_APIList; 
extern int  GUI_pUC_API; 
extern int  GUI_pfDispCharStyle; 
extern int  GUI_pfTimerExec; 
extern int  GUI_sError; 
extern int  WM_pfHandlePID; 
extern int  GUI_ALLOC__Context; 
/*********************************************************************************************************	
** 系统静态符号表												
*********************************************************************************************************/	
SYMBOL_TABLE_BEGIN												
    SYMBOL_ITEM_FUNC(WM__UpdateChildPositions) 
    SYMBOL_ITEM_FUNC(WM__SendMessageNoPara) 
    SYMBOL_ITEM_FUNC(WM__SendMessageIfEnabled) 
    SYMBOL_ITEM_FUNC(WM__SendMessage) 
    SYMBOL_ITEM_FUNC(WM__Screen2Client) 
    SYMBOL_ITEM_FUNC(WM__NotifyVisChanged) 
    SYMBOL_ITEM_FUNC(WM__IsEnabled) 
    SYMBOL_ITEM_FUNC(WM__IsChild) 
    SYMBOL_ITEM_FUNC(WM__IsAncestor) 
    SYMBOL_ITEM_FUNC(WM__IsAncestorOrSelf) 
    SYMBOL_ITEM_FUNC(WM__GetPrevSibling) 
    SYMBOL_ITEM_FUNC(WM__GetOrgX_AA) 
    SYMBOL_ITEM_FUNC(WM__GetOrgY_AA) 
    SYMBOL_ITEM_FUNC(WM__GetLastSibling) 
    SYMBOL_ITEM_FUNC(WM__GetFocussedChild) 
    SYMBOL_ITEM_FUNC(WM__GetFirstSibling) 
    SYMBOL_ITEM_FUNC(WM__ForEachDesc) 
    SYMBOL_ITEM_FUNC(WM_ValidateWindow) 
    SYMBOL_ITEM_FUNC(WM_ValidateRect) 
    SYMBOL_ITEM_FUNC(WM_GetUserData) 
    SYMBOL_ITEM_FUNC(WM_SetUserData) 
    SYMBOL_ITEM_FUNC(WM__GetUserDataEx) 
    SYMBOL_ITEM_FUNC(WM__SetUserDataEx) 
    SYMBOL_ITEM_FUNC(WM_UpdateWindowAndDescs) 
    SYMBOL_ITEM_FUNC(WM_HandlePID) 
    SYMBOL_ITEM_FUNC(WM__IsInModalArea) 
    SYMBOL_ITEM_FUNC(WM__SendPIDMessage) 
    SYMBOL_ITEM_FUNC(WM__SendTouchMessage) 
    SYMBOL_ITEM_FUNC(WM__SetLastTouched) 
    SYMBOL_ITEM_FUNC(WM__SetMotionCallback) 
    SYMBOL_ITEM_FUNC(WM__SetToolTipCallback) 
    SYMBOL_ITEM_FUNC(WM_TOOLTIP_AddTool) 
    SYMBOL_ITEM_FUNC(WM_TOOLTIP_Create) 
    SYMBOL_ITEM_FUNC(WM_TOOLTIP_Delete) 
    SYMBOL_ITEM_FUNC(WM_TOOLTIP_SetDefaultColor) 
    SYMBOL_ITEM_FUNC(WM_TOOLTIP_SetDefaultFont) 
    SYMBOL_ITEM_FUNC(WM_TOOLTIP_SetDefaultPeriod) 
    SYMBOL_ITEM_FUNC(WM_CreateTimer) 
    SYMBOL_ITEM_FUNC(WM_DeleteTimer) 
    SYMBOL_ITEM_FUNC(WM_GetTimerId) 
    SYMBOL_ITEM_FUNC(WM_RestartTimer) 
    SYMBOL_ITEM_FUNC(WM__DeleteAssocTimer) 
    SYMBOL_ITEM_FUNC(WM_GetStayOnTop) 
    SYMBOL_ITEM_FUNC(WM_SetStayOnTop) 
    SYMBOL_ITEM_FUNC(WM_SetScreenSize) 
    SYMBOL_ITEM_FUNC(WM_InvalidateWindowAndDescs) 
    SYMBOL_ITEM_FUNC(WM_ShowWindow) 
    SYMBOL_ITEM_FUNC(WM_SetYSize) 
    SYMBOL_ITEM_FUNC(WM_SetXSize) 
    SYMBOL_ITEM_FUNC(WM_SetWindowPos) 
    SYMBOL_ITEM_FUNC(WM_SetUserClipRect) 
    SYMBOL_ITEM_FUNC(WM_SetTransState) 
    SYMBOL_ITEM_FUNC(WM_ClrHasTrans) 
    SYMBOL_ITEM_FUNC(WM_GetHasTrans) 
    SYMBOL_ITEM_FUNC(WM_SetHasTrans) 
    SYMBOL_ITEM_FUNC(WM_SetSize) 
    SYMBOL_ITEM_FUNC(WM_SetScrollState) 
    SYMBOL_ITEM_FUNC(WM_SetScrollPosH) 
    SYMBOL_ITEM_FUNC(WM_SetScrollPosV) 
    SYMBOL_ITEM_FUNC(WM_SetScrollbarH) 
    SYMBOL_ITEM_FUNC(WM_SetScrollbarV) 
    SYMBOL_ITEM_FUNC(WM__SetScrollbarH) 
    SYMBOL_ITEM_FUNC(WM__SetScrollbarV) 
    SYMBOL_ITEM_FUNC(WM_SetpfPollPID) 
    SYMBOL_ITEM_FUNC(WM_SetId) 
    SYMBOL_ITEM_FUNC(WM_SetFocusOnPrevChild) 
    SYMBOL_ITEM_FUNC(WM_SetFocusOnNextChild) 
    SYMBOL_ITEM_FUNC(WM_SetFocus) 
    SYMBOL_ITEM_FUNC(WM_SetDesktopColor) 
    SYMBOL_ITEM_FUNC(WM_SetDesktopColorEx) 
    SYMBOL_ITEM_FUNC(WM_SetDesktopColors) 
    SYMBOL_ITEM_FUNC(WM_SetCreateFlags) 
    SYMBOL_ITEM_FUNC(WM_SetCaptureMove) 
    SYMBOL_ITEM_FUNC(WM_ReleaseCapture) 
    SYMBOL_ITEM_FUNC(WM_SetCapture) 
    SYMBOL_ITEM_FUNC(WM_SetCallback) 
    SYMBOL_ITEM_FUNC(WM_SetAnchor) 
    SYMBOL_ITEM_FUNC(WM_SendToParent) 
    SYMBOL_ITEM_FUNC(WM_SendMessageNoPara) 
    SYMBOL_ITEM_FUNC(WM_Screen2hWin) 
    SYMBOL_ITEM_FUNC(WM_Screen2hWinEx) 
    SYMBOL_ITEM_FUNC(WM__IsInWindow) 
    SYMBOL_ITEM_FUNC(WM_ResizeWindow) 
    SYMBOL_ITEM_FUNC(WM_PID__GetPrevState) 
    SYMBOL_ITEM_FUNC(WM_PID__SetPrevState) 
    SYMBOL_ITEM_FUNC(WM_PaintWindowAndDescs) 
    SYMBOL_ITEM_FUNC(WM_Paint) 
    SYMBOL_ITEM_FUNC(WM_Update) 
    SYMBOL_ITEM_FUNC(WM_OnKey) 
    SYMBOL_ITEM_FUNC(WM_NotifyParent) 
    SYMBOL_ITEM_FUNC(WM_MULTIBUF_Enable) 
    SYMBOL_ITEM_FUNC(WM_MoveChildTo) 
    SYMBOL_ITEM_FUNC(WM_MoveTo) 
    SYMBOL_ITEM_FUNC(WM_MoveWindow) 
    SYMBOL_ITEM_FUNC(WM__MoveTo) 
    SYMBOL_ITEM_FUNC(WM__MoveWindow) 
    SYMBOL_ITEM_FUNC(WM_MOTION_Enable) 
    SYMBOL_ITEM_FUNC(WM_MOTION_SetDeceleration) 
    SYMBOL_ITEM_FUNC(WM_MOTION_SetDefaultPeriod) 
    SYMBOL_ITEM_FUNC(WM_MOTION_SetMotion) 
    SYMBOL_ITEM_FUNC(WM_MOTION_SetMoveable) 
    SYMBOL_ITEM_FUNC(WM_MOTION_SetMovement) 
    SYMBOL_ITEM_FUNC(WM_MOTION_SetSpeed) 
    SYMBOL_ITEM_FUNC(WM_DisableMemdev) 
    SYMBOL_ITEM_FUNC(WM_EnableMemdev) 
    SYMBOL_ITEM_FUNC(WM_MakeModal) 
    SYMBOL_ITEM_FUNC(WM_IsWindow) 
    SYMBOL_ITEM_FUNC(WM_IsVisible) 
    SYMBOL_ITEM_FUNC(WM_IsFocussable) 
    SYMBOL_ITEM_FUNC(WM_IsEnabled) 
    SYMBOL_ITEM_FUNC(WM_IsCompletelyVisible) 
    SYMBOL_ITEM_FUNC(WM_IsCompletelyCovered) 
    SYMBOL_ITEM_FUNC(WM_InvalidateArea) 
    SYMBOL_ITEM_FUNC(WM_HideWindow) 
    SYMBOL_ITEM_FUNC(WM_HasFocus) 
    SYMBOL_ITEM_FUNC(WM_HasCaptured) 
    SYMBOL_ITEM_FUNC(WM_GetWindowSizeX) 
    SYMBOL_ITEM_FUNC(WM_GetWindowSizeY) 
    SYMBOL_ITEM_FUNC(WM__GetWindowSizeX) 
    SYMBOL_ITEM_FUNC(WM__GetWindowSizeY) 
    SYMBOL_ITEM_FUNC(WM_GetWindowRect) 
    SYMBOL_ITEM_FUNC(WM_GetWindowRectEx) 
    SYMBOL_ITEM_FUNC(WM_GetScrollState) 
    SYMBOL_ITEM_FUNC(WM_GetScrollPosH) 
    SYMBOL_ITEM_FUNC(WM_GetScrollPosV) 
    SYMBOL_ITEM_FUNC(WM_GetScrollPartner) 
    SYMBOL_ITEM_FUNC(WM_GetScrollbarH) 
    SYMBOL_ITEM_FUNC(WM_GetScrollbarV) 
    SYMBOL_ITEM_FUNC(WM_GetPrevSibling) 
    SYMBOL_ITEM_FUNC(WM_GetParent) 
    SYMBOL_ITEM_FUNC(WM_GetOrgX) 
    SYMBOL_ITEM_FUNC(WM_GetOrgY) 
    SYMBOL_ITEM_FUNC(WM_GetWindowOrgX) 
    SYMBOL_ITEM_FUNC(WM_GetWindowOrgY) 
    SYMBOL_ITEM_FUNC(WM_GetNextSibling) 
    SYMBOL_ITEM_FUNC(WM_GetInvalidRect) 
    SYMBOL_ITEM_FUNC(WM_GetInsideRectExScrollbar) 
    SYMBOL_ITEM_FUNC(WM_GetInsideRect) 
    SYMBOL_ITEM_FUNC(WM_GetInsideRectEx) 
    SYMBOL_ITEM_FUNC(WM_GetId) 
    SYMBOL_ITEM_FUNC(WM_GetFocussedWindow) 
    SYMBOL_ITEM_FUNC(WM_GetFlags) 
    SYMBOL_ITEM_FUNC(WM_GetFirstChild) 
    SYMBOL_ITEM_FUNC(WM_GetDialogItem) 
    SYMBOL_ITEM_FUNC(WM_GetNumInvalidWindows) 
    SYMBOL_ITEM_FUNC(WM_GetNumWindows) 
    SYMBOL_ITEM_FUNC(WM_GetDesktopWindowEx) 
    SYMBOL_ITEM_FUNC(WM_GetDesktopWindow) 
    SYMBOL_ITEM_FUNC(WM_GetClientWindow) 
    SYMBOL_ITEM_FUNC(WM_GetClientRect) 
    SYMBOL_ITEM_FUNC(WM_GetClientRectEx) 
    SYMBOL_ITEM_FUNC(WM__GetClientRectEx) 
    SYMBOL_ITEM_FUNC(WM_GetCallback) 
    SYMBOL_ITEM_FUNC(WM_GetBkColor) 
    SYMBOL_ITEM_FUNC(WM_ForEachDesc) 
    SYMBOL_ITEM_FUNC(WM_DisableWindow) 
    SYMBOL_ITEM_FUNC(WM_EnableWindow) 
    SYMBOL_ITEM_FUNC(WM_SetEnableState) 
    SYMBOL_ITEM_FUNC(WM_DIAG_C) 
    SYMBOL_ITEM_FUNC(WM__AddCriticalHandle) 
    SYMBOL_ITEM_FUNC(WM__RemoveCriticalHandle) 
    SYMBOL_ITEM_FUNC(WM_CheckScrollBounds) 
    SYMBOL_ITEM_FUNC(WM_CheckScrollPos) 
    SYMBOL_ITEM_FUNC(WM_SetScrollValue) 
    SYMBOL_ITEM_FUNC(WM_BroadcastMessage) 
    SYMBOL_ITEM_FUNC(WM_BringToTop) 
    SYMBOL_ITEM_FUNC(WM_BringToBottom) 
    SYMBOL_ITEM_FUNC(WM_AttachWindow) 
    SYMBOL_ITEM_FUNC(WM_AttachWindowAt) 
    SYMBOL_ITEM_FUNC(WM_DetachWindow) 
    SYMBOL_ITEM_FUNC(WM_Activate) 
    SYMBOL_ITEM_FUNC(WM_CreateWindow) 
    SYMBOL_ITEM_FUNC(WM_CreateWindowAsChild) 
    SYMBOL_ITEM_FUNC(WM_Deactivate) 
    SYMBOL_ITEM_FUNC(WM_DefaultProc) 
    SYMBOL_ITEM_FUNC(WM_DeleteWindow) 
    SYMBOL_ITEM_FUNC(WM_Exec) 
    SYMBOL_ITEM_FUNC(WM_GetActiveWindow) 
    SYMBOL_ITEM_FUNC(WM_Init) 
    SYMBOL_ITEM_FUNC(WM_InvalidateRect) 
    SYMBOL_ITEM_FUNC(WM_InvalidateWindow) 
    SYMBOL_ITEM_FUNC(WM_InvalidateWindowAndDescsEx) 
    SYMBOL_ITEM_FUNC(WM_SelectWindow) 
    SYMBOL_ITEM_FUNC(WM_SendMessage) 
    SYMBOL_ITEM_FUNC(WM_SetDefault) 
    SYMBOL_ITEM_FUNC(WM__ActivateClipRect) 
    SYMBOL_ITEM_FUNC(WM__Client2Screen) 
    SYMBOL_ITEM_FUNC(WM__ClipAtParentBorders) 
    SYMBOL_ITEM_FUNC(WM__DeleteSecure) 
    SYMBOL_ITEM_FUNC(WM__DetachWindow) 
    SYMBOL_ITEM_FUNC(WM__GetClientRectWin) 
    SYMBOL_ITEM_FUNC(WM__GetNextIVR) 
    SYMBOL_ITEM_FUNC(WM__GetTopLevelLayer) 
    SYMBOL_ITEM_FUNC(WM__InitIVRSearch) 
    SYMBOL_ITEM_FUNC(WM__InsertWindowIntoList) 
    SYMBOL_ITEM_FUNC(WM__Invalidate1Abs) 
    SYMBOL_ITEM_FUNC(WM__InvalidateDrawAndDescs) 
    SYMBOL_ITEM_FUNC(WM__InvalidateParent) 
    SYMBOL_ITEM_FUNC(WM__InvalidateRect) 
    SYMBOL_ITEM_FUNC(WM__InvalidateRectEx) 
    SYMBOL_ITEM_FUNC(WM__IsWindow) 
    SYMBOL_ITEM_FUNC(WM__Paint) 
    SYMBOL_ITEM_FUNC(WM__Paint1) 
    SYMBOL_ITEM_FUNC(WM__RectIsNZ) 
    SYMBOL_ITEM_FUNC(WM__RemoveFromLinList) 
    SYMBOL_ITEM_FUNC(WM__RemoveWindowFromList) 
    SYMBOL_ITEM_FUNC(WM__SelectTopLevelLayer) 
    SYMBOL_ITEM_FUNC(WM__SendMsgNoData) 
    SYMBOL_ITEM_FUNC(WINDOW_GetDefaultBkColor) 
    SYMBOL_ITEM_FUNC(WINDOW_SetDefaultBkColor) 
    SYMBOL_ITEM_FUNC(WINDOW_Callback) 
    SYMBOL_ITEM_FUNC(WINDOW_CreateEx) 
    SYMBOL_ITEM_FUNC(WINDOW_CreateIndirect) 
    SYMBOL_ITEM_FUNC(WINDOW_CreateUser) 
    SYMBOL_ITEM_FUNC(WINDOW_GetUserData) 
    SYMBOL_ITEM_FUNC(WINDOW_SetBkColor) 
    SYMBOL_ITEM_FUNC(WINDOW_SetUserData) 
    SYMBOL_ITEM_FUNC(WIDGET_SetWidth) 
    SYMBOL_ITEM_FUNC(WIDGET_SetEffect) 
    SYMBOL_ITEM_FUNC(WIDGET__FillStringInRect) 
    SYMBOL_ITEM_FUNC(WIDGET_EFFECT_Simple_GetColor) 
    SYMBOL_ITEM_FUNC(WIDGET_EFFECT_Simple_GetNumColors) 
    SYMBOL_ITEM_FUNC(WIDGET_EFFECT_Simple_SetColor) 
    SYMBOL_ITEM_FUNC(WIDGET_EFFECT_3D2L_GetColor) 
    SYMBOL_ITEM_FUNC(WIDGET_EFFECT_3D2L_GetNumColors) 
    SYMBOL_ITEM_FUNC(WIDGET_EFFECT_3D2L_SetColor) 
    SYMBOL_ITEM_FUNC(WIDGET_EFFECT_3D1L_GetColor) 
    SYMBOL_ITEM_FUNC(WIDGET_EFFECT_3D1L_GetNumColors) 
    SYMBOL_ITEM_FUNC(WIDGET_EFFECT_3D1L_SetColor) 
    SYMBOL_ITEM_FUNC(WIDGET_EFFECT_3D_DrawUp) 
    SYMBOL_ITEM_FUNC(WIDGET_EFFECT_3D_GetColor) 
    SYMBOL_ITEM_FUNC(WIDGET_EFFECT_3D_GetNumColors) 
    SYMBOL_ITEM_FUNC(WIDGET_EFFECT_3D_SetColor) 
    SYMBOL_ITEM_FUNC(WIDGET_AndState) 
    SYMBOL_ITEM_FUNC(WIDGET_GetDefaultEffect) 
    SYMBOL_ITEM_FUNC(WIDGET_GetState) 
    SYMBOL_ITEM_FUNC(WIDGET_HandleActive) 
    SYMBOL_ITEM_FUNC(WIDGET_OrState) 
    SYMBOL_ITEM_FUNC(WIDGET_SetDefaultEffect) 
    SYMBOL_ITEM_FUNC(WIDGET_SetState) 
    SYMBOL_ITEM_FUNC(WIDGET__DrawFocusRect) 
    SYMBOL_ITEM_FUNC(WIDGET__DrawHLine) 
    SYMBOL_ITEM_FUNC(WIDGET__DrawTriangle) 
    SYMBOL_ITEM_FUNC(WIDGET__DrawVLine) 
    SYMBOL_ITEM_FUNC(WIDGET__EFFECT_DrawDown) 
    SYMBOL_ITEM_FUNC(WIDGET__EFFECT_DrawDownRect) 
    SYMBOL_ITEM_FUNC(WIDGET__EFFECT_DrawUpRect) 
    SYMBOL_ITEM_FUNC(WIDGET__FillRectEx) 
    SYMBOL_ITEM_FUNC(WIDGET__GetBkColor) 
    SYMBOL_ITEM_FUNC(WIDGET__GetClientRect) 
    SYMBOL_ITEM_FUNC(WIDGET__GetInsideRect) 
    SYMBOL_ITEM_FUNC(WIDGET__GetWindowSizeX) 
    SYMBOL_ITEM_FUNC(WIDGET__GetXSize) 
    SYMBOL_ITEM_FUNC(WIDGET__GetYSize) 
    SYMBOL_ITEM_FUNC(WIDGET__Init) 
    SYMBOL_ITEM_FUNC(WIDGET__RotateRect90) 
    SYMBOL_ITEM_FUNC(WIDGET__SetScrollState) 
    SYMBOL_ITEM_FUNC(TREEVIEW_GetDefaultBkColor) 
    SYMBOL_ITEM_FUNC(TREEVIEW_GetDefaultFont) 
    SYMBOL_ITEM_FUNC(TREEVIEW_GetDefaultLineColor) 
    SYMBOL_ITEM_FUNC(TREEVIEW_GetDefaultTextColor) 
    SYMBOL_ITEM_FUNC(TREEVIEW_SetDefaultBkColor) 
    SYMBOL_ITEM_FUNC(TREEVIEW_SetDefaultFont) 
    SYMBOL_ITEM_FUNC(TREEVIEW_SetDefaultLineColor) 
    SYMBOL_ITEM_FUNC(TREEVIEW_SetDefaultTextColor) 
    SYMBOL_ITEM_FUNC(TREEVIEW_CreateIndirect) 
    SYMBOL_ITEM_FUNC(TREEVIEW_AttachItem) 
    SYMBOL_ITEM_FUNC(TREEVIEW_Callback) 
    SYMBOL_ITEM_FUNC(TREEVIEW_CreateEx) 
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
    SYMBOL_ITEM_FUNC(TEXT_GetNumLines) 
    SYMBOL_ITEM_FUNC(TEXT_SetWrapMode) 
    SYMBOL_ITEM_FUNC(TEXT_SetTextColor) 
    SYMBOL_ITEM_FUNC(TEXT_SetTextAlign) 
    SYMBOL_ITEM_FUNC(TEXT_SetText) 
    SYMBOL_ITEM_FUNC(TEXT_SetFont) 
    SYMBOL_ITEM_FUNC(TEXT_SetBkColor) 
    SYMBOL_ITEM_FUNC(TEXT_GetText) 
    SYMBOL_ITEM_FUNC(TEXT_GetDefaultFont) 
    SYMBOL_ITEM_FUNC(TEXT_SetDefaultFont) 
    SYMBOL_ITEM_FUNC(TEXT_SetDefaultTextColor) 
    SYMBOL_ITEM_FUNC(TEXT_SetDefaultWrapMode) 
    SYMBOL_ITEM_FUNC(TEXT_CreateIndirect) 
    SYMBOL_ITEM_FUNC(TEXT_Create) 
    SYMBOL_ITEM_FUNC(TEXT_CreateAsChild) 
    SYMBOL_ITEM_FUNC(TEXT_Callback) 
    SYMBOL_ITEM_FUNC(TEXT_CreateEx) 
    SYMBOL_ITEM_FUNC(TEXT_CreateUser) 
    SYMBOL_ITEM_FUNC(TEXT_GetUserData) 
    SYMBOL_ITEM_FUNC(TEXT_SetUserData) 
    SYMBOL_ITEM_FUNC(SPINBOX_DrawSkinFlex) 
    SYMBOL_ITEM_FUNC(SPINBOX_GetSkinFlexProps) 
    SYMBOL_ITEM_FUNC(SPINBOX_SetSkinFlexProps) 
    SYMBOL_ITEM_FUNC(SPINBOX_SetDefaultSkinClassic) 
    SYMBOL_ITEM_FUNC(SPINBOX_SetSkinClassic) 
    SYMBOL_ITEM_FUNC(SPINBOX_SetDefaultSkin) 
    SYMBOL_ITEM_FUNC(SPINBOX_SetSkin) 
    SYMBOL_ITEM_FUNC(SPINBOX__ApplyProps) 
    SYMBOL_ITEM_FUNC(SPINBOX_GetDefaultButtonSize) 
    SYMBOL_ITEM_FUNC(SPINBOX_SetDefaultButtonSize) 
    SYMBOL_ITEM_FUNC(SPINBOX_CreateIndirect) 
    SYMBOL_ITEM_FUNC(SPINBOX_Callback) 
    SYMBOL_ITEM_FUNC(SPINBOX_CreateEx) 
    SYMBOL_ITEM_FUNC(SPINBOX_CreateUser) 
    SYMBOL_ITEM_FUNC(SPINBOX_EnableBlink) 
    SYMBOL_ITEM_FUNC(SPINBOX_GetBkColor) 
    SYMBOL_ITEM_FUNC(SPINBOX_GetButtonBkColor) 
    SYMBOL_ITEM_FUNC(SPINBOX_GetEditHandle) 
    SYMBOL_ITEM_FUNC(SPINBOX_GetUserData) 
    SYMBOL_ITEM_FUNC(SPINBOX_GetValue) 
    SYMBOL_ITEM_FUNC(SPINBOX_SetBkColor) 
    SYMBOL_ITEM_FUNC(SPINBOX_SetButtonBkColor) 
    SYMBOL_ITEM_FUNC(SPINBOX_SetButtonSize) 
    SYMBOL_ITEM_FUNC(SPINBOX_SetEdge) 
    SYMBOL_ITEM_FUNC(SPINBOX_SetFont) 
    SYMBOL_ITEM_FUNC(SPINBOX_SetRange) 
    SYMBOL_ITEM_FUNC(SPINBOX_SetTextColor) 
    SYMBOL_ITEM_FUNC(SPINBOX_SetUserData) 
    SYMBOL_ITEM_FUNC(SPINBOX_SetValue) 
    SYMBOL_ITEM_FUNC(SPINBOX__DrawBk) 
    SYMBOL_ITEM_FUNC(SPINBOX__GetButtonRect) 
    SYMBOL_ITEM_FUNC(SPINBOX__GetButtonRectEx) 
    SYMBOL_ITEM_FUNC(SPINBOX__GetDefaultMax) 
    SYMBOL_ITEM_FUNC(SPINBOX__GetDefaultMin) 
    SYMBOL_ITEM_FUNC(SLIDER_DrawSkinFlex) 
    SYMBOL_ITEM_FUNC(SLIDER_GetSkinFlexProps) 
    SYMBOL_ITEM_FUNC(SLIDER_SetSkinFlexProps) 
    SYMBOL_ITEM_FUNC(SLIDER_SetDefaultSkinClassic) 
    SYMBOL_ITEM_FUNC(SLIDER_SetSkinClassic) 
    SYMBOL_ITEM_FUNC(SLIDER_SetDefaultSkin) 
    SYMBOL_ITEM_FUNC(SLIDER_SetSkin) 
    SYMBOL_ITEM_FUNC(SLIDER_GetDefaultBarColor) 
    SYMBOL_ITEM_FUNC(SLIDER_GetDefaultBkColor) 
    SYMBOL_ITEM_FUNC(SLIDER_GetDefaultFocusColor) 
    SYMBOL_ITEM_FUNC(SLIDER_GetDefaultTickColor) 
    SYMBOL_ITEM_FUNC(SLIDER_SetDefaultBarColor) 
    SYMBOL_ITEM_FUNC(SLIDER_SetDefaultBkColor) 
    SYMBOL_ITEM_FUNC(SLIDER_SetDefaultFocusColor) 
    SYMBOL_ITEM_FUNC(SLIDER_SetDefaultTickColor) 
    SYMBOL_ITEM_FUNC(SLIDER_CreateIndirect) 
    SYMBOL_ITEM_FUNC(SLIDER_Create) 
    SYMBOL_ITEM_FUNC(SLIDER_GetBarColor) 
    SYMBOL_ITEM_FUNC(SLIDER_GetBkColor) 
    SYMBOL_ITEM_FUNC(SLIDER_GetFocusColor) 
    SYMBOL_ITEM_FUNC(SLIDER_GetTickColor) 
    SYMBOL_ITEM_FUNC(SLIDER_SetBarColor) 
    SYMBOL_ITEM_FUNC(SLIDER_SetBkColor) 
    SYMBOL_ITEM_FUNC(SLIDER_SetFocusColor) 
    SYMBOL_ITEM_FUNC(SLIDER_SetTickColor) 
    SYMBOL_ITEM_FUNC(SLIDER_Callback) 
    SYMBOL_ITEM_FUNC(SLIDER_CreateEx) 
    SYMBOL_ITEM_FUNC(SLIDER_CreateUser) 
    SYMBOL_ITEM_FUNC(SLIDER_Dec) 
    SYMBOL_ITEM_FUNC(SLIDER_GetUserData) 
    SYMBOL_ITEM_FUNC(SLIDER_GetValue) 
    SYMBOL_ITEM_FUNC(SLIDER_Inc) 
    SYMBOL_ITEM_FUNC(SLIDER_SetNumTicks) 
    SYMBOL_ITEM_FUNC(SLIDER_SetRange) 
    SYMBOL_ITEM_FUNC(SLIDER_SetUserData) 
    SYMBOL_ITEM_FUNC(SLIDER_SetValue) 
    SYMBOL_ITEM_FUNC(SLIDER_SetWidth) 
    SYMBOL_ITEM_FUNC(SCROLLBAR_DrawSkinFlex) 
    SYMBOL_ITEM_FUNC(SCROLLBAR_GetSkinFlexProps) 
    SYMBOL_ITEM_FUNC(SCROLLBAR_SetSkinFlexProps) 
    SYMBOL_ITEM_FUNC(SCROLLBAR_SetDefaultSkinClassic) 
    SYMBOL_ITEM_FUNC(SCROLLBAR_SetSkinClassic) 
    SYMBOL_ITEM_FUNC(SCROLLBAR_SetDefaultSkin) 
    SYMBOL_ITEM_FUNC(SCROLLBAR_SetSkin) 
    SYMBOL_ITEM_FUNC(SCROLLBAR_SetWidth) 
    SYMBOL_ITEM_FUNC(SCROLLBAR_SetColor) 
    SYMBOL_ITEM_FUNC(SCROLLBAR_GetValue) 
    SYMBOL_ITEM_FUNC(SCROLLBAR_GetPageSize) 
    SYMBOL_ITEM_FUNC(SCROLLBAR_GetNumItems) 
    SYMBOL_ITEM_FUNC(SCROLLBAR_GetDefaultWidth) 
    SYMBOL_ITEM_FUNC(SCROLLBAR_GetThumbSizeMin) 
    SYMBOL_ITEM_FUNC(SCROLLBAR_SetDefaultColor) 
    SYMBOL_ITEM_FUNC(SCROLLBAR_SetDefaultWidth) 
    SYMBOL_ITEM_FUNC(SCROLLBAR_SetThumbSizeMin) 
    SYMBOL_ITEM_FUNC(SCROLLBAR_CreateIndirect) 
    SYMBOL_ITEM_FUNC(SCROLLBAR_Create) 
    SYMBOL_ITEM_FUNC(SCROLLBAR_CreateAttached) 
    SYMBOL_ITEM_FUNC(SCROLLBAR_AddValue) 
    SYMBOL_ITEM_FUNC(SCROLLBAR_Callback) 
    SYMBOL_ITEM_FUNC(SCROLLBAR_CreateEx) 
    SYMBOL_ITEM_FUNC(SCROLLBAR_CreateUser) 
    SYMBOL_ITEM_FUNC(SCROLLBAR_Dec) 
    SYMBOL_ITEM_FUNC(SCROLLBAR_GetUserData) 
    SYMBOL_ITEM_FUNC(SCROLLBAR_Inc) 
    SYMBOL_ITEM_FUNC(SCROLLBAR_SetNumItems) 
    SYMBOL_ITEM_FUNC(SCROLLBAR_SetPageSize) 
    SYMBOL_ITEM_FUNC(SCROLLBAR_SetState) 
    SYMBOL_ITEM_FUNC(SCROLLBAR_SetUserData) 
    SYMBOL_ITEM_FUNC(SCROLLBAR_SetValue) 
    SYMBOL_ITEM_FUNC(SCROLLBAR__InvalidatePartner) 
    SYMBOL_ITEM_FUNC(SCROLLBAR__Rect2VRect) 
    SYMBOL_ITEM_FUNC(RADIO_DrawSkinFlex) 
    SYMBOL_ITEM_FUNC(RADIO_GetSkinFlexProps) 
    SYMBOL_ITEM_FUNC(RADIO_SetSkinFlexProps) 
    SYMBOL_ITEM_FUNC(RADIO_SetDefaultSkinClassic) 
    SYMBOL_ITEM_FUNC(RADIO_SetSkinClassic) 
    SYMBOL_ITEM_FUNC(RADIO_SetDefaultSkin) 
    SYMBOL_ITEM_FUNC(RADIO_SetSkin) 
    SYMBOL_ITEM_FUNC(RADIO_SetTextColor) 
    SYMBOL_ITEM_FUNC(RADIO_SetText) 
    SYMBOL_ITEM_FUNC(RADIO_GetImage) 
    SYMBOL_ITEM_FUNC(RADIO_SetImage) 
    SYMBOL_ITEM_FUNC(RADIO_SetGroupId) 
    SYMBOL_ITEM_FUNC(RADIO_SetFont) 
    SYMBOL_ITEM_FUNC(RADIO_SetFocusColor) 
    SYMBOL_ITEM_FUNC(RADIO_SetDefaultImage) 
    SYMBOL_ITEM_FUNC(RADIO_SetBkColor) 
    SYMBOL_ITEM_FUNC(RADIO_GetText) 
    SYMBOL_ITEM_FUNC(RADIO_GetDefaultFont) 
    SYMBOL_ITEM_FUNC(RADIO_GetDefaultTextColor) 
    SYMBOL_ITEM_FUNC(RADIO_SetDefaultFocusColor) 
    SYMBOL_ITEM_FUNC(RADIO_SetDefaultFont) 
    SYMBOL_ITEM_FUNC(RADIO_SetDefaultTextColor) 
    SYMBOL_ITEM_FUNC(RADIO_CreateIndirect) 
    SYMBOL_ITEM_FUNC(RADIO_Create) 
    SYMBOL_ITEM_FUNC(RADIO_AddValue) 
    SYMBOL_ITEM_FUNC(RADIO_Callback) 
    SYMBOL_ITEM_FUNC(RADIO_CreateEx) 
    SYMBOL_ITEM_FUNC(RADIO_CreateUser) 
    SYMBOL_ITEM_FUNC(RADIO_Dec) 
    SYMBOL_ITEM_FUNC(RADIO_GetUserData) 
    SYMBOL_ITEM_FUNC(RADIO_GetValue) 
    SYMBOL_ITEM_FUNC(RADIO_Inc) 
    SYMBOL_ITEM_FUNC(RADIO_SetUserData) 
    SYMBOL_ITEM_FUNC(RADIO_SetValue) 
    SYMBOL_ITEM_FUNC(RADIO__GetButtonSize) 
    SYMBOL_ITEM_FUNC(RADIO__SetValue) 
    SYMBOL_ITEM_FUNC(PROGBAR_DrawSkinFlex) 
    SYMBOL_ITEM_FUNC(PROGBAR_GetSkinFlexProps) 
    SYMBOL_ITEM_FUNC(PROGBAR_SetSkinFlexProps) 
    SYMBOL_ITEM_FUNC(PROGBAR_SetDefaultSkinClassic) 
    SYMBOL_ITEM_FUNC(PROGBAR_SetSkinClassic) 
    SYMBOL_ITEM_FUNC(PROGBAR_SetDefaultSkin) 
    SYMBOL_ITEM_FUNC(PROGBAR_SetSkin) 
    SYMBOL_ITEM_FUNC(PROGBAR_SetTextPos) 
    SYMBOL_ITEM_FUNC(PROGBAR_SetTextColor) 
    SYMBOL_ITEM_FUNC(PROGBAR_SetTextAlign) 
    SYMBOL_ITEM_FUNC(PROGBAR_SetText) 
    SYMBOL_ITEM_FUNC(PROGBAR_SetMinMax) 
    SYMBOL_ITEM_FUNC(PROGBAR_SetFont) 
    SYMBOL_ITEM_FUNC(PROGBAR_SetBarColor) 
    SYMBOL_ITEM_FUNC(PROGBAR_CreateIndirect) 
    SYMBOL_ITEM_FUNC(PROGBAR_Create) 
    SYMBOL_ITEM_FUNC(PROGBAR_CreateAsChild) 
    SYMBOL_ITEM_FUNC(PROGBAR_Callback) 
    SYMBOL_ITEM_FUNC(PROGBAR_CreateEx) 
    SYMBOL_ITEM_FUNC(PROGBAR_CreateUser) 
    SYMBOL_ITEM_FUNC(PROGBAR_GetUserData) 
    SYMBOL_ITEM_FUNC(PROGBAR_SetUserData) 
    SYMBOL_ITEM_FUNC(PROGBAR_SetValue) 
    SYMBOL_ITEM_FUNC(PROGBAR__GetTextLocked) 
    SYMBOL_ITEM_FUNC(PROGBAR__GetTextRect) 
    SYMBOL_ITEM_FUNC(PROGBAR__Value2Pos) 
    SYMBOL_ITEM_FUNC(MULTIPAGE_SetRotation) 
    SYMBOL_ITEM_FUNC(MULTIPAGE_GetDefaultAlign) 
    SYMBOL_ITEM_FUNC(MULTIPAGE_GetDefaultBkColor) 
    SYMBOL_ITEM_FUNC(MULTIPAGE_GetDefaultFont) 
    SYMBOL_ITEM_FUNC(MULTIPAGE_GetDefaultTextColor) 
    SYMBOL_ITEM_FUNC(MULTIPAGE_SetDefaultAlign) 
    SYMBOL_ITEM_FUNC(MULTIPAGE_SetDefaultBkColor) 
    SYMBOL_ITEM_FUNC(MULTIPAGE_SetDefaultFont) 
    SYMBOL_ITEM_FUNC(MULTIPAGE_SetDefaultTextColor) 
    SYMBOL_ITEM_FUNC(MULTIPAGE_CreateIndirect) 
    SYMBOL_ITEM_FUNC(MULTIPAGE_Create) 
    SYMBOL_ITEM_FUNC(MULTIPAGE_AddPage) 
    SYMBOL_ITEM_FUNC(MULTIPAGE_Callback) 
    SYMBOL_ITEM_FUNC(MULTIPAGE_CreateEx) 
    SYMBOL_ITEM_FUNC(MULTIPAGE_CreateUser) 
    SYMBOL_ITEM_FUNC(MULTIPAGE_DeletePage) 
    SYMBOL_ITEM_FUNC(MULTIPAGE_DisablePage) 
    SYMBOL_ITEM_FUNC(MULTIPAGE_EnablePage) 
    SYMBOL_ITEM_FUNC(MULTIPAGE_GetEffectColor) 
    SYMBOL_ITEM_FUNC(MULTIPAGE_GetNumEffectColors) 
    SYMBOL_ITEM_FUNC(MULTIPAGE_GetSelection) 
    SYMBOL_ITEM_FUNC(MULTIPAGE_GetUserData) 
    SYMBOL_ITEM_FUNC(MULTIPAGE_GetWindow) 
    SYMBOL_ITEM_FUNC(MULTIPAGE_IsPageEnabled) 
    SYMBOL_ITEM_FUNC(MULTIPAGE_SelectPage) 
    SYMBOL_ITEM_FUNC(MULTIPAGE_SetAlign) 
    SYMBOL_ITEM_FUNC(MULTIPAGE_SetBkColor) 
    SYMBOL_ITEM_FUNC(MULTIPAGE_SetEffectColor) 
    SYMBOL_ITEM_FUNC(MULTIPAGE_SetFont) 
    SYMBOL_ITEM_FUNC(MULTIPAGE_SetText) 
    SYMBOL_ITEM_FUNC(MULTIPAGE_SetTextColor) 
    SYMBOL_ITEM_FUNC(MULTIPAGE_SetUserData) 
    SYMBOL_ITEM_FUNC(MULTIPAGE__CalcClientRect) 
    SYMBOL_ITEM_FUNC(MULTIPAGE__DeleteScrollbar) 
    SYMBOL_ITEM_FUNC(MULTIPAGE__DrawTextItemH) 
    SYMBOL_ITEM_FUNC(MULTIPAGE__UpdatePositions) 
    SYMBOL_ITEM_FUNC(MULTIEDIT_CreateIndirect) 
    SYMBOL_ITEM_FUNC(MULTIEDIT_Create) 
    SYMBOL_ITEM_FUNC(MULTIEDIT_AddKey) 
    SYMBOL_ITEM_FUNC(MULTIEDIT_AddText) 
    SYMBOL_ITEM_FUNC(MULTIEDIT_Callback) 
    SYMBOL_ITEM_FUNC(MULTIEDIT_CreateEx) 
    SYMBOL_ITEM_FUNC(MULTIEDIT_CreateUser) 
    SYMBOL_ITEM_FUNC(MULTIEDIT_EnableBlink) 
    SYMBOL_ITEM_FUNC(MULTIEDIT_GetCursorCharPos) 
    SYMBOL_ITEM_FUNC(MULTIEDIT_GetCursorPixelPos) 
    SYMBOL_ITEM_FUNC(MULTIEDIT_GetPrompt) 
    SYMBOL_ITEM_FUNC(MULTIEDIT_GetText) 
    SYMBOL_ITEM_FUNC(MULTIEDIT_GetTextSize) 
    SYMBOL_ITEM_FUNC(MULTIEDIT_GetUserData) 
    SYMBOL_ITEM_FUNC(MULTIEDIT_SetAutoScrollH) 
    SYMBOL_ITEM_FUNC(MULTIEDIT_SetAutoScrollV) 
    SYMBOL_ITEM_FUNC(MULTIEDIT_SetBkColor) 
    SYMBOL_ITEM_FUNC(MULTIEDIT_SetBufferSize) 
    SYMBOL_ITEM_FUNC(MULTIEDIT_SetCursorOffset) 
    SYMBOL_ITEM_FUNC(MULTIEDIT_SetFont) 
    SYMBOL_ITEM_FUNC(MULTIEDIT_SetHBorder) 
    SYMBOL_ITEM_FUNC(MULTIEDIT_SetInsertMode) 
    SYMBOL_ITEM_FUNC(MULTIEDIT_SetMaxNumChars) 
    SYMBOL_ITEM_FUNC(MULTIEDIT_SetPasswordMode) 
    SYMBOL_ITEM_FUNC(MULTIEDIT_SetPrompt) 
    SYMBOL_ITEM_FUNC(MULTIEDIT_SetReadOnly) 
    SYMBOL_ITEM_FUNC(MULTIEDIT_SetText) 
    SYMBOL_ITEM_FUNC(MULTIEDIT_SetTextAlign) 
    SYMBOL_ITEM_FUNC(MULTIEDIT_SetTextColor) 
    SYMBOL_ITEM_FUNC(MULTIEDIT_SetUserData) 
    SYMBOL_ITEM_FUNC(MULTIEDIT_SetWrapChar) 
    SYMBOL_ITEM_FUNC(MULTIEDIT_SetWrapNone) 
    SYMBOL_ITEM_FUNC(MULTIEDIT_SetWrapWord) 
    SYMBOL_ITEM_FUNC(GUI_MessageBox) 
    SYMBOL_ITEM_FUNC(MESSAGEBOX_Callback) 
    SYMBOL_ITEM_FUNC(MESSAGEBOX_Create) 
    SYMBOL_ITEM_FUNC(MENU__FindItem) 
    SYMBOL_ITEM_FUNC(MENU_SetTextColor) 
    SYMBOL_ITEM_FUNC(MENU_SetItem) 
    SYMBOL_ITEM_FUNC(MENU_SetFont) 
    SYMBOL_ITEM_FUNC(MENU_SetBorderSize) 
    SYMBOL_ITEM_FUNC(MENU_SetBkColor) 
    SYMBOL_ITEM_FUNC(MENU_Popup) 
    SYMBOL_ITEM_FUNC(MENU_InsertItem) 
    SYMBOL_ITEM_FUNC(MENU_GetNumItems) 
    SYMBOL_ITEM_FUNC(MENU_GetItemText) 
    SYMBOL_ITEM_FUNC(MENU_GetItem) 
    SYMBOL_ITEM_FUNC(MENU_EnableItem) 
    SYMBOL_ITEM_FUNC(MENU_DisableItem) 
    SYMBOL_ITEM_FUNC(MENU_DeleteItem) 
    SYMBOL_ITEM_FUNC(MENU_GetDefaultBkColor) 
    SYMBOL_ITEM_FUNC(MENU_GetDefaultBorderSize) 
    SYMBOL_ITEM_FUNC(MENU_GetDefaultEffect) 
    SYMBOL_ITEM_FUNC(MENU_GetDefaultFont) 
    SYMBOL_ITEM_FUNC(MENU_GetDefaultTextColor) 
    SYMBOL_ITEM_FUNC(MENU_SetDefaultBkColor) 
    SYMBOL_ITEM_FUNC(MENU_SetDefaultBorderSize) 
    SYMBOL_ITEM_FUNC(MENU_SetDefaultEffect) 
    SYMBOL_ITEM_FUNC(MENU_SetDefaultFont) 
    SYMBOL_ITEM_FUNC(MENU_SetDefaultTextColor) 
    SYMBOL_ITEM_FUNC(MENU_CreateIndirect) 
    SYMBOL_ITEM_FUNC(MENU_Attach) 
    SYMBOL_ITEM_FUNC(MENU_AddItem) 
    SYMBOL_ITEM_FUNC(MENU_Callback) 
    SYMBOL_ITEM_FUNC(MENU_CreateEx) 
    SYMBOL_ITEM_FUNC(MENU_CreateUser) 
    SYMBOL_ITEM_FUNC(MENU_GetOwner) 
    SYMBOL_ITEM_FUNC(MENU_GetUserData) 
    SYMBOL_ITEM_FUNC(MENU_SetOwner) 
    SYMBOL_ITEM_FUNC(MENU_SetSel) 
    SYMBOL_ITEM_FUNC(MENU_SetUserData) 
    SYMBOL_ITEM_FUNC(MENU__GetNumItems) 
    SYMBOL_ITEM_FUNC(MENU__InvalidateItem) 
    SYMBOL_ITEM_FUNC(MENU__RecalcTextWidthOfItems) 
    SYMBOL_ITEM_FUNC(MENU__ResizeMenu) 
    SYMBOL_ITEM_FUNC(MENU__SendMenuMessage) 
    SYMBOL_ITEM_FUNC(MENU__SetItem) 
    SYMBOL_ITEM_FUNC(MENU__SetItemFlags) 
    SYMBOL_ITEM_FUNC(LISTWHEEL_Create) 
    SYMBOL_ITEM_FUNC(LISTWHEEL_CreateAsChild) 
    SYMBOL_ITEM_FUNC(LISTWHEEL_CreateIndirect) 
    SYMBOL_ITEM_FUNC(LISTWHEEL_AddString) 
    SYMBOL_ITEM_FUNC(LISTWHEEL_Callback) 
    SYMBOL_ITEM_FUNC(LISTWHEEL_CreateEx) 
    SYMBOL_ITEM_FUNC(LISTWHEEL_CreateUser) 
    SYMBOL_ITEM_FUNC(LISTWHEEL_GetFont) 
    SYMBOL_ITEM_FUNC(LISTWHEEL_GetItemData) 
    SYMBOL_ITEM_FUNC(LISTWHEEL_GetItemText) 
    SYMBOL_ITEM_FUNC(LISTWHEEL_GetLBorder) 
    SYMBOL_ITEM_FUNC(LISTWHEEL_GetLineHeight) 
    SYMBOL_ITEM_FUNC(LISTWHEEL_GetNumItems) 
    SYMBOL_ITEM_FUNC(LISTWHEEL_GetPos) 
    SYMBOL_ITEM_FUNC(LISTWHEEL_GetRBorder) 
    SYMBOL_ITEM_FUNC(LISTWHEEL_GetSel) 
    SYMBOL_ITEM_FUNC(LISTWHEEL_GetSnapPosition) 
    SYMBOL_ITEM_FUNC(LISTWHEEL_GetTextAlign) 
    SYMBOL_ITEM_FUNC(LISTWHEEL_GetUserData) 
    SYMBOL_ITEM_FUNC(LISTWHEEL_MoveToPos) 
    SYMBOL_ITEM_FUNC(LISTWHEEL_OwnerDraw) 
    SYMBOL_ITEM_FUNC(LISTWHEEL_SetBkColor) 
    SYMBOL_ITEM_FUNC(LISTWHEEL_SetFont) 
    SYMBOL_ITEM_FUNC(LISTWHEEL_SetItemData) 
    SYMBOL_ITEM_FUNC(LISTWHEEL_SetLBorder) 
    SYMBOL_ITEM_FUNC(LISTWHEEL_SetLineHeight) 
    SYMBOL_ITEM_FUNC(LISTWHEEL_SetOwnerDraw) 
    SYMBOL_ITEM_FUNC(LISTWHEEL_SetPos) 
    SYMBOL_ITEM_FUNC(LISTWHEEL_SetRBorder) 
    SYMBOL_ITEM_FUNC(LISTWHEEL_SetSel) 
    SYMBOL_ITEM_FUNC(LISTWHEEL_SetSnapPosition) 
    SYMBOL_ITEM_FUNC(LISTWHEEL_SetText) 
    SYMBOL_ITEM_FUNC(LISTWHEEL_SetTextAlign) 
    SYMBOL_ITEM_FUNC(LISTWHEEL_SetTextColor) 
    SYMBOL_ITEM_FUNC(LISTWHEEL_SetTimerPeriod) 
    SYMBOL_ITEM_FUNC(LISTWHEEL_SetUserData) 
    SYMBOL_ITEM_FUNC(LISTWHEEL_SetVelocity) 
    SYMBOL_ITEM_FUNC(LISTWHEEL__GetpStringLocked) 
    SYMBOL_ITEM_FUNC(LISTVIEW_GetUserDataRow) 
    SYMBOL_ITEM_FUNC(LISTVIEW_SetUserDataRow) 
    SYMBOL_ITEM_FUNC(LISTVIEW_SetWrapMode) 
    SYMBOL_ITEM_FUNC(LISTVIEW_SetTextColor) 
    SYMBOL_ITEM_FUNC(LISTVIEW_SetTextAlign) 
    SYMBOL_ITEM_FUNC(LISTVIEW_CompareDec) 
    SYMBOL_ITEM_FUNC(LISTVIEW_CompareText) 
    SYMBOL_ITEM_FUNC(LISTVIEW_DisableSort) 
    SYMBOL_ITEM_FUNC(LISTVIEW_EnableSort) 
    SYMBOL_ITEM_FUNC(LISTVIEW_GetSelUnsorted) 
    SYMBOL_ITEM_FUNC(LISTVIEW_SetCompareFunc) 
    SYMBOL_ITEM_FUNC(LISTVIEW_SetSelUnsorted) 
    SYMBOL_ITEM_FUNC(LISTVIEW_SetSort) 
    SYMBOL_ITEM_FUNC(LISTVIEW_SetSel) 
    SYMBOL_ITEM_FUNC(LISTVIEW_SetRowHeight) 
    SYMBOL_ITEM_FUNC(LISTVIEW_SetRBorder) 
    SYMBOL_ITEM_FUNC(LISTVIEW_SetLBorder) 
    SYMBOL_ITEM_FUNC(LISTVIEW_SetItemText) 
    SYMBOL_ITEM_FUNC(LISTVIEW_SetItemBkColor) 
    SYMBOL_ITEM_FUNC(LISTVIEW_SetItemTextColor) 
    SYMBOL_ITEM_FUNC(LISTVIEW_SetItemBitmap) 
    SYMBOL_ITEM_FUNC(LISTVIEW_SetGridVis) 
    SYMBOL_ITEM_FUNC(LISTVIEW_SetFont) 
    SYMBOL_ITEM_FUNC(LISTVIEW_SetFixed) 
    SYMBOL_ITEM_FUNC(LISTVIEW_SetColumnWidth) 
    SYMBOL_ITEM_FUNC(LISTVIEW_SetBkColor) 
    SYMBOL_ITEM_FUNC(LISTVIEW_SetAutoScrollH) 
    SYMBOL_ITEM_FUNC(LISTVIEW_SetAutoScrollV) 
    SYMBOL_ITEM_FUNC(LISTVIEW_InsertRow) 
    SYMBOL_ITEM_FUNC(LISTVIEW_GetTextColor) 
    SYMBOL_ITEM_FUNC(LISTVIEW_GetSel) 
    SYMBOL_ITEM_FUNC(LISTVIEW_GetNumRows) 
    SYMBOL_ITEM_FUNC(LISTVIEW_GetNumColumns) 
    SYMBOL_ITEM_FUNC(LISTVIEW_GetItemText) 
    SYMBOL_ITEM_FUNC(LISTVIEW_GetHeader) 
    SYMBOL_ITEM_FUNC(LISTVIEW_GetFont) 
    SYMBOL_ITEM_FUNC(LISTVIEW_GetBkColor) 
    SYMBOL_ITEM_FUNC(LISTVIEW_DisableRow) 
    SYMBOL_ITEM_FUNC(LISTVIEW_EnableRow) 
    SYMBOL_ITEM_FUNC(LISTVIEW_DeleteRow) 
    SYMBOL_ITEM_FUNC(LISTVIEW__InvalidateRowAndBelow) 
    SYMBOL_ITEM_FUNC(LISTVIEW_DeleteColumn) 
    SYMBOL_ITEM_FUNC(LISTVIEW_DeleteAllRows) 
    SYMBOL_ITEM_FUNC(LISTVIEW_SetDefaultBkColor) 
    SYMBOL_ITEM_FUNC(LISTVIEW_SetDefaultFont) 
    SYMBOL_ITEM_FUNC(LISTVIEW_SetDefaultGridColor) 
    SYMBOL_ITEM_FUNC(LISTVIEW_SetDefaultTextColor) 
    SYMBOL_ITEM_FUNC(LISTVIEW_CreateIndirect) 
    SYMBOL_ITEM_FUNC(LISTVIEW_Create) 
    SYMBOL_ITEM_FUNC(LISTVIEW_CreateAttached) 
    SYMBOL_ITEM_FUNC(LISTVIEW_AddColumn) 
    SYMBOL_ITEM_FUNC(LISTVIEW_AddRow) 
    SYMBOL_ITEM_FUNC(LISTVIEW_Callback) 
    SYMBOL_ITEM_FUNC(LISTVIEW_CreateEx) 
    SYMBOL_ITEM_FUNC(LISTVIEW_CreateUser) 
    SYMBOL_ITEM_FUNC(LISTVIEW_DecSel) 
    SYMBOL_ITEM_FUNC(LISTVIEW_GetUserData) 
    SYMBOL_ITEM_FUNC(LISTVIEW_IncSel) 
    SYMBOL_ITEM_FUNC(LISTVIEW_SetHeaderHeight) 
    SYMBOL_ITEM_FUNC(LISTVIEW_SetUserData) 
    SYMBOL_ITEM_FUNC(LISTVIEW__GetNumColumns) 
    SYMBOL_ITEM_FUNC(LISTVIEW__GetNumRows) 
    SYMBOL_ITEM_FUNC(LISTVIEW__GetRowDistY) 
    SYMBOL_ITEM_FUNC(LISTVIEW__GetSel) 
    SYMBOL_ITEM_FUNC(LISTVIEW__InvalidateInsideArea) 
    SYMBOL_ITEM_FUNC(LISTVIEW__InvalidateRow) 
    SYMBOL_ITEM_FUNC(LISTVIEW__MoveSel) 
    SYMBOL_ITEM_FUNC(LISTVIEW__SetSel) 
    SYMBOL_ITEM_FUNC(LISTVIEW__UpdateScrollParas) 
    SYMBOL_ITEM_FUNC(LISTVIEW__UpdateScrollPos) 
    SYMBOL_ITEM_FUNC(LISTBOX_SetTextColor) 
    SYMBOL_ITEM_FUNC(LISTBOX_SetTextAlign) 
    SYMBOL_ITEM_FUNC(LISTBOX_SetString) 
    SYMBOL_ITEM_FUNC(LISTBOX_SetScrollbarWidth) 
    SYMBOL_ITEM_FUNC(LISTBOX_SetScrollbarColor) 
    SYMBOL_ITEM_FUNC(LISTBOX_SetOwnerDraw) 
    SYMBOL_ITEM_FUNC(LISTBOX_SetOwner) 
    SYMBOL_ITEM_FUNC(LISTBOX_SetBkColor) 
    SYMBOL_ITEM_FUNC(LISTBOX_SetAutoScrollH) 
    SYMBOL_ITEM_FUNC(LISTBOX_SetAutoScrollV) 
    SYMBOL_ITEM_FUNC(LISTBOX_GetScrollStepH) 
    SYMBOL_ITEM_FUNC(LISTBOX_SetScrollStepH) 
    SYMBOL_ITEM_FUNC(LISTBOX_GetItemSel) 
    SYMBOL_ITEM_FUNC(LISTBOX_GetMulti) 
    SYMBOL_ITEM_FUNC(LISTBOX_SetItemSel) 
    SYMBOL_ITEM_FUNC(LISTBOX_SetMulti) 
    SYMBOL_ITEM_FUNC(LISTBOX_GetItemSpacing) 
    SYMBOL_ITEM_FUNC(LISTBOX_SetItemSpacing) 
    SYMBOL_ITEM_FUNC(LISTBOX_GetItemDisabled) 
    SYMBOL_ITEM_FUNC(LISTBOX_SetItemDisabled) 
    SYMBOL_ITEM_FUNC(LISTBOX_InsertString) 
    SYMBOL_ITEM_FUNC(LISTBOX_GetTextAlign) 
    SYMBOL_ITEM_FUNC(LISTBOX_GetNumItems) 
    SYMBOL_ITEM_FUNC(LISTBOX_GetItemText) 
    SYMBOL_ITEM_FUNC(LISTBOX_GetFont) 
    SYMBOL_ITEM_FUNC(LISTBOX_SetFont) 
    SYMBOL_ITEM_FUNC(LISTBOX_DeleteItem) 
    SYMBOL_ITEM_FUNC(LISTBOX_GetDefaultBkColor) 
    SYMBOL_ITEM_FUNC(LISTBOX_GetDefaultFont) 
    SYMBOL_ITEM_FUNC(LISTBOX_GetDefaultScrollStepH) 
    SYMBOL_ITEM_FUNC(LISTBOX_GetDefaultTextAlign) 
    SYMBOL_ITEM_FUNC(LISTBOX_GetDefaultTextColor) 
    SYMBOL_ITEM_FUNC(LISTBOX_SetDefaultBkColor) 
    SYMBOL_ITEM_FUNC(LISTBOX_SetDefaultFont) 
    SYMBOL_ITEM_FUNC(LISTBOX_SetDefaultScrollStepH) 
    SYMBOL_ITEM_FUNC(LISTBOX_SetDefaultTextAlign) 
    SYMBOL_ITEM_FUNC(LISTBOX_SetDefaultTextColor) 
    SYMBOL_ITEM_FUNC(LISTBOX_CreateIndirect) 
    SYMBOL_ITEM_FUNC(LISTBOX_Create) 
    SYMBOL_ITEM_FUNC(LISTBOX_CreateAsChild) 
    SYMBOL_ITEM_FUNC(LISTBOX_AddStringH) 
    SYMBOL_ITEM_FUNC(LISTBOX_AddKey) 
    SYMBOL_ITEM_FUNC(LISTBOX_AddString) 
    SYMBOL_ITEM_FUNC(LISTBOX_Callback) 
    SYMBOL_ITEM_FUNC(LISTBOX_CreateEx) 
    SYMBOL_ITEM_FUNC(LISTBOX_CreateUser) 
    SYMBOL_ITEM_FUNC(LISTBOX_DecSel) 
    SYMBOL_ITEM_FUNC(LISTBOX_GetSel) 
    SYMBOL_ITEM_FUNC(LISTBOX_GetUserData) 
    SYMBOL_ITEM_FUNC(LISTBOX_IncSel) 
    SYMBOL_ITEM_FUNC(LISTBOX_InvalidateItem) 
    SYMBOL_ITEM_FUNC(LISTBOX_OwnerDraw) 
    SYMBOL_ITEM_FUNC(LISTBOX_SetSel) 
    SYMBOL_ITEM_FUNC(LISTBOX_SetText) 
    SYMBOL_ITEM_FUNC(LISTBOX_SetUserData) 
    SYMBOL_ITEM_FUNC(LISTBOX_UpdateScrollers) 
    SYMBOL_ITEM_FUNC(LISTBOX__AddSize) 
    SYMBOL_ITEM_FUNC(LISTBOX__GetNumItems) 
    SYMBOL_ITEM_FUNC(LISTBOX__GetpStringLocked) 
    SYMBOL_ITEM_FUNC(LISTBOX__InvalidateInsideArea) 
    SYMBOL_ITEM_FUNC(LISTBOX__InvalidateItem) 
    SYMBOL_ITEM_FUNC(LISTBOX__InvalidateItemAndBelow) 
    SYMBOL_ITEM_FUNC(LISTBOX__InvalidateItemSize) 
    SYMBOL_ITEM_FUNC(LISTBOX__SetScrollbarColor) 
    SYMBOL_ITEM_FUNC(LISTBOX__SetScrollbarWidth) 
    SYMBOL_ITEM_FUNC(IMAGE_SetJPEG) 
    SYMBOL_ITEM_FUNC(IMAGE_SetJPEGEx) 
    SYMBOL_ITEM_FUNC(IMAGE_SetGIF) 
    SYMBOL_ITEM_FUNC(IMAGE_SetGIFEx) 
    SYMBOL_ITEM_FUNC(IMAGE_SetDTA) 
    SYMBOL_ITEM_FUNC(IMAGE_SetDTAEx) 
    SYMBOL_ITEM_FUNC(IMAGE_SetBMP) 
    SYMBOL_ITEM_FUNC(IMAGE_SetBMPEx) 
    SYMBOL_ITEM_FUNC(IMAGE_SetBitmap) 
    SYMBOL_ITEM_FUNC(IMAGE_Callback) 
    SYMBOL_ITEM_FUNC(IMAGE_CreateEx) 
    SYMBOL_ITEM_FUNC(IMAGE_CreateIndirect) 
    SYMBOL_ITEM_FUNC(IMAGE_CreateUser) 
    SYMBOL_ITEM_FUNC(IMAGE__FreeAttached) 
    SYMBOL_ITEM_FUNC(IMAGE__SetWindowSize) 
    SYMBOL_ITEM_FUNC(ICONVIEW_SetWrapMode) 
    SYMBOL_ITEM_FUNC(ICONVIEW_EnableStreamAuto) 
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
    SYMBOL_ITEM_FUNC(HEADER__SetDrawObj) 
    SYMBOL_ITEM_FUNC(HEADER_SetStreamedBitmap) 
    SYMBOL_ITEM_FUNC(HEADER_SetStreamedBitmapEx) 
    SYMBOL_ITEM_FUNC(HEADER_DrawSkinFlex) 
    SYMBOL_ITEM_FUNC(HEADER_GetSkinFlexProps) 
    SYMBOL_ITEM_FUNC(HEADER_SetSkinFlexProps) 
    SYMBOL_ITEM_FUNC(HEADER_SetDefaultSkinClassic) 
    SYMBOL_ITEM_FUNC(HEADER_SetSkinClassic) 
    SYMBOL_ITEM_FUNC(HEADER_SetDefaultSkin) 
    SYMBOL_ITEM_FUNC(HEADER_SetSkin) 
    SYMBOL_ITEM_FUNC(HEADER_SetFixed) 
    SYMBOL_ITEM_FUNC(HEADER_SetDragLimit) 
    SYMBOL_ITEM_FUNC(HEADER_GetSel) 
    SYMBOL_ITEM_FUNC(HEADER_GetDefaultArrowColor) 
    SYMBOL_ITEM_FUNC(HEADER_GetDefaultBkColor) 
    SYMBOL_ITEM_FUNC(HEADER_GetDefaultBorderH) 
    SYMBOL_ITEM_FUNC(HEADER_GetDefaultBorderV) 
    SYMBOL_ITEM_FUNC(HEADER_GetDefaultCursor) 
    SYMBOL_ITEM_FUNC(HEADER_GetDefaultFont) 
    SYMBOL_ITEM_FUNC(HEADER_GetDefaultTextColor) 
    SYMBOL_ITEM_FUNC(HEADER_SetDefaultArrowColor) 
    SYMBOL_ITEM_FUNC(HEADER_SetDefaultBkColor) 
    SYMBOL_ITEM_FUNC(HEADER_SetDefaultBorderH) 
    SYMBOL_ITEM_FUNC(HEADER_SetDefaultBorderV) 
    SYMBOL_ITEM_FUNC(HEADER_SetDefaultCursor) 
    SYMBOL_ITEM_FUNC(HEADER_SetDefaultFont) 
    SYMBOL_ITEM_FUNC(HEADER_SetDefaultTextColor) 
    SYMBOL_ITEM_FUNC(HEADER_CreateIndirect) 
    SYMBOL_ITEM_FUNC(HEADER_CreateAttached) 
    SYMBOL_ITEM_FUNC(HEADER_GetArrowColor) 
    SYMBOL_ITEM_FUNC(HEADER_GetBkColor) 
    SYMBOL_ITEM_FUNC(HEADER_GetTextColor) 
    SYMBOL_ITEM_FUNC(HEADER_SetArrowColor) 
    SYMBOL_ITEM_FUNC(HEADER_SetBkColor) 
    SYMBOL_ITEM_FUNC(HEADER_SetTextColor) 
    SYMBOL_ITEM_FUNC(HEADER_SetBMP) 
    SYMBOL_ITEM_FUNC(HEADER_SetBMPEx) 
    SYMBOL_ITEM_FUNC(HEADER_SetBitmap) 
    SYMBOL_ITEM_FUNC(HEADER_SetBitmapEx) 
    SYMBOL_ITEM_FUNC(HEADER_AddItem) 
    SYMBOL_ITEM_FUNC(HEADER_Callback) 
    SYMBOL_ITEM_FUNC(HEADER_Create) 
    SYMBOL_ITEM_FUNC(HEADER_CreateEx) 
    SYMBOL_ITEM_FUNC(HEADER_CreateUser) 
    SYMBOL_ITEM_FUNC(HEADER_DeleteItem) 
    SYMBOL_ITEM_FUNC(HEADER_GetHeight) 
    SYMBOL_ITEM_FUNC(HEADER_GetItemWidth) 
    SYMBOL_ITEM_FUNC(HEADER_GetNumItems) 
    SYMBOL_ITEM_FUNC(HEADER_GetUserData) 
    SYMBOL_ITEM_FUNC(HEADER_SetDirIndicator) 
    SYMBOL_ITEM_FUNC(HEADER_SetFont) 
    SYMBOL_ITEM_FUNC(HEADER_SetHeight) 
    SYMBOL_ITEM_FUNC(HEADER_SetItemText) 
    SYMBOL_ITEM_FUNC(HEADER_SetItemWidth) 
    SYMBOL_ITEM_FUNC(HEADER_SetScrollPos) 
    SYMBOL_ITEM_FUNC(HEADER_SetTextAlign) 
    SYMBOL_ITEM_FUNC(HEADER_SetUserData) 
    SYMBOL_ITEM_FUNC(GUI_HOOK_Add) 
    SYMBOL_ITEM_FUNC(GUI_HOOK_Remove) 
    SYMBOL_ITEM_FUNC(GUI_EditString) 
    SYMBOL_ITEM_FUNC(GUI_EditHex) 
    SYMBOL_ITEM_FUNC(GUI_EditFloat) 
    SYMBOL_ITEM_FUNC(GUI_EditDec) 
    SYMBOL_ITEM_FUNC(GUI_EditBin) 
    SYMBOL_ITEM_FUNC(GUI_DRAW_STREAMED_Create) 
    SYMBOL_ITEM_FUNC(GUI_DRAW_SELF_Create) 
    SYMBOL_ITEM_FUNC(GUI_DRAW_BMP_Create) 
    SYMBOL_ITEM_FUNC(GUI_DRAW_BITMAP_Create) 
    SYMBOL_ITEM_FUNC(GUI_DRAW__Draw) 
    SYMBOL_ITEM_FUNC(GUI_DRAW__GetXSize) 
    SYMBOL_ITEM_FUNC(GUI_DRAW__GetYSize) 
    SYMBOL_ITEM_FUNC(GUI_ARRAY_ResizeItemLocked) 
    SYMBOL_ITEM_FUNC(GUI_ARRAY_InsertBlankItem) 
    SYMBOL_ITEM_FUNC(GUI_ARRAY_InsertItem) 
    SYMBOL_ITEM_FUNC(GUI_ARRAY_DeleteItem) 
    SYMBOL_ITEM_FUNC(GUI_ARRAY_AddItem) 
    SYMBOL_ITEM_FUNC(GUI_ARRAY_Create) 
    SYMBOL_ITEM_FUNC(GUI_ARRAY_Delete) 
    SYMBOL_ITEM_FUNC(GUI_ARRAY_GetNumItems) 
    SYMBOL_ITEM_FUNC(GUI_ARRAY_GethItem) 
    SYMBOL_ITEM_FUNC(GUI_ARRAY_GetpItemLocked) 
    SYMBOL_ITEM_FUNC(GUI_ARRAY_SetItem) 
    SYMBOL_ITEM_FUNC(GUI_ARRAY__GethItem) 
    SYMBOL_ITEM_FUNC(GUI_ARRAY__GetpItemLocked) 
    SYMBOL_ITEM_FUNC(GUI_ARRAY__SethItem) 
    SYMBOL_ITEM_FUNC(GRAPH_SCALE_Create) 
    SYMBOL_ITEM_FUNC(GRAPH_SCALE_Delete) 
    SYMBOL_ITEM_FUNC(GRAPH_SCALE_SetFactor) 
    SYMBOL_ITEM_FUNC(GRAPH_SCALE_SetFont) 
    SYMBOL_ITEM_FUNC(GRAPH_SCALE_SetNumDecs) 
    SYMBOL_ITEM_FUNC(GRAPH_SCALE_SetOff) 
    SYMBOL_ITEM_FUNC(GRAPH_SCALE_SetPos) 
    SYMBOL_ITEM_FUNC(GRAPH_SCALE_SetTextColor) 
    SYMBOL_ITEM_FUNC(GRAPH_SCALE_SetTickDist) 
    SYMBOL_ITEM_FUNC(GRAPH_DATA_YT_AddValue) 
    SYMBOL_ITEM_FUNC(GRAPH_DATA_YT_Clear) 
    SYMBOL_ITEM_FUNC(GRAPH_DATA_YT_Create) 
    SYMBOL_ITEM_FUNC(GRAPH_DATA_YT_Delete) 
    SYMBOL_ITEM_FUNC(GRAPH_DATA_YT_MirrorX) 
    SYMBOL_ITEM_FUNC(GRAPH_DATA_YT_SetAlign) 
    SYMBOL_ITEM_FUNC(GRAPH_DATA_YT_SetOffY) 
    SYMBOL_ITEM_FUNC(GRAPH_DATA_XY_AddPoint) 
    SYMBOL_ITEM_FUNC(GRAPH_DATA_XY_Clear) 
    SYMBOL_ITEM_FUNC(GRAPH_DATA_XY_Create) 
    SYMBOL_ITEM_FUNC(GRAPH_DATA_XY_Delete) 
    SYMBOL_ITEM_FUNC(GRAPH_DATA_XY_GetLineVis) 
    SYMBOL_ITEM_FUNC(GRAPH_DATA_XY_GetPointVis) 
    SYMBOL_ITEM_FUNC(GRAPH_DATA_XY_SetLineStyle) 
    SYMBOL_ITEM_FUNC(GRAPH_DATA_XY_SetLineVis) 
    SYMBOL_ITEM_FUNC(GRAPH_DATA_XY_SetOffX) 
    SYMBOL_ITEM_FUNC(GRAPH_DATA_XY_SetOffY) 
    SYMBOL_ITEM_FUNC(GRAPH_DATA_XY_SetOwnerDraw) 
    SYMBOL_ITEM_FUNC(GRAPH_DATA_XY_SetPenSize) 
    SYMBOL_ITEM_FUNC(GRAPH_DATA_XY_SetPointSize) 
    SYMBOL_ITEM_FUNC(GRAPH_DATA_XY_SetPointVis) 
    SYMBOL_ITEM_FUNC(GRAPH_CreateIndirect) 
    SYMBOL_ITEM_FUNC(GRAPH_AttachData) 
    SYMBOL_ITEM_FUNC(GRAPH_AttachScale) 
    SYMBOL_ITEM_FUNC(GRAPH_Callback) 
    SYMBOL_ITEM_FUNC(GRAPH_CreateEx) 
    SYMBOL_ITEM_FUNC(GRAPH_CreateUser) 
    SYMBOL_ITEM_FUNC(GRAPH_DetachData) 
    SYMBOL_ITEM_FUNC(GRAPH_DetachScale) 
    SYMBOL_ITEM_FUNC(GRAPH_GetScrollValue) 
    SYMBOL_ITEM_FUNC(GRAPH_GetUserData) 
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
    SYMBOL_ITEM_FUNC(GRAPH__InvalidateGraph) 
    SYMBOL_ITEM_FUNC(FRAMEWIN__UpdateButtons) 
    SYMBOL_ITEM_FUNC(FRAMEWIN_DrawSkinFlex) 
    SYMBOL_ITEM_FUNC(FRAMEWIN_GetSkinFlexProps) 
    SYMBOL_ITEM_FUNC(FRAMEWIN_SetSkinFlexProps) 
    SYMBOL_ITEM_FUNC(FRAMEWIN_OwnerDraw) 
    SYMBOL_ITEM_FUNC(FRAMEWIN_SetDefaultSkinClassic) 
    SYMBOL_ITEM_FUNC(FRAMEWIN_SetSkinClassic) 
    SYMBOL_ITEM_FUNC(FRAMEWIN_SetDefaultSkin) 
    SYMBOL_ITEM_FUNC(FRAMEWIN_SetSkin) 
    SYMBOL_ITEM_FUNC(FRAMEWIN_SetTitleVis) 
    SYMBOL_ITEM_FUNC(FRAMEWIN_SetTitleHeight) 
    SYMBOL_ITEM_FUNC(FRAMEWIN_SetResizeable) 
    SYMBOL_ITEM_FUNC(FRAMEWIN_SetOwnerDraw) 
    SYMBOL_ITEM_FUNC(FRAMEWIN_SetFont) 
    SYMBOL_ITEM_FUNC(FRAMEWIN_SetBarColor) 
    SYMBOL_ITEM_FUNC(FRAMEWIN_SetClientColor) 
    SYMBOL_ITEM_FUNC(FRAMEWIN_SetTextColor) 
    SYMBOL_ITEM_FUNC(FRAMEWIN_SetTextColorEx) 
    SYMBOL_ITEM_FUNC(FRAMEWIN_SetBorderSize) 
    SYMBOL_ITEM_FUNC(FRAMEWIN_Maximize) 
    SYMBOL_ITEM_FUNC(FRAMEWIN_Minimize) 
    SYMBOL_ITEM_FUNC(FRAMEWIN_Restore) 
    SYMBOL_ITEM_FUNC(FRAMEWIN_IsMaximized) 
    SYMBOL_ITEM_FUNC(FRAMEWIN_IsMinimized) 
    SYMBOL_ITEM_FUNC(FRAMEWIN_GetActive) 
    SYMBOL_ITEM_FUNC(FRAMEWIN_GetBarColor) 
    SYMBOL_ITEM_FUNC(FRAMEWIN_GetBorderSize) 
    SYMBOL_ITEM_FUNC(FRAMEWIN_GetBorderSizeEx) 
    SYMBOL_ITEM_FUNC(FRAMEWIN_GetFont) 
    SYMBOL_ITEM_FUNC(FRAMEWIN_GetText) 
    SYMBOL_ITEM_FUNC(FRAMEWIN_GetTextAlign) 
    SYMBOL_ITEM_FUNC(FRAMEWIN_GetTitleHeight) 
    SYMBOL_ITEM_FUNC(FRAMEWIN_GetDefaultBarColor) 
    SYMBOL_ITEM_FUNC(FRAMEWIN_GetDefaultBorderSize) 
    SYMBOL_ITEM_FUNC(FRAMEWIN_GetDefaultClientColor) 
    SYMBOL_ITEM_FUNC(FRAMEWIN_GetDefaultFont) 
    SYMBOL_ITEM_FUNC(FRAMEWIN_GetDefaultTextColor) 
    SYMBOL_ITEM_FUNC(FRAMEWIN_GetDefaultTitleHeight) 
    SYMBOL_ITEM_FUNC(FRAMEWIN_SetDefaultBarColor) 
    SYMBOL_ITEM_FUNC(FRAMEWIN_SetDefaultBorderSize) 
    SYMBOL_ITEM_FUNC(FRAMEWIN_SetDefaultClientColor) 
    SYMBOL_ITEM_FUNC(FRAMEWIN_SetDefaultFont) 
    SYMBOL_ITEM_FUNC(FRAMEWIN_SetDefaultTextAlign) 
    SYMBOL_ITEM_FUNC(FRAMEWIN_SetDefaultTextColor) 
    SYMBOL_ITEM_FUNC(FRAMEWIN_SetDefaultTitleHeight) 
    SYMBOL_ITEM_FUNC(FRAMEWIN_CreateIndirect) 
    SYMBOL_ITEM_FUNC(FRAMEWIN_Create) 
    SYMBOL_ITEM_FUNC(FRAMEWIN_CreateAsChild) 
    SYMBOL_ITEM_FUNC(FRAMEWIN_AddMinButton) 
    SYMBOL_ITEM_FUNC(FRAMEWIN_AddMaxButton) 
    SYMBOL_ITEM_FUNC(FRAMEWIN_AddCloseButton) 
    SYMBOL_ITEM_FUNC(FRAMEWIN_AddButton) 
    SYMBOL_ITEM_FUNC(FRAMEWIN_AddMenu) 
    SYMBOL_ITEM_FUNC(FRAMEWIN_Callback) 
    SYMBOL_ITEM_FUNC(FRAMEWIN_CreateEx) 
    SYMBOL_ITEM_FUNC(FRAMEWIN_CreateUser) 
    SYMBOL_ITEM_FUNC(FRAMEWIN_GetUserData) 
    SYMBOL_ITEM_FUNC(FRAMEWIN_SetActive) 
    SYMBOL_ITEM_FUNC(FRAMEWIN_SetMoveable) 
    SYMBOL_ITEM_FUNC(FRAMEWIN_SetText) 
    SYMBOL_ITEM_FUNC(FRAMEWIN_SetTextAlign) 
    SYMBOL_ITEM_FUNC(FRAMEWIN_SetUserData) 
    SYMBOL_ITEM_FUNC(FRAMEWIN__CalcPositions) 
    SYMBOL_ITEM_FUNC(FRAMEWIN__CalcTitleHeight) 
    SYMBOL_ITEM_FUNC(FRAMEWIN__GetBorderSize) 
    SYMBOL_ITEM_FUNC(FRAMEWIN__GetTitleLimits) 
    SYMBOL_ITEM_FUNC(FRAMEWIN__UpdatePositions) 
    SYMBOL_ITEM_FUNC(EDIT_SetTextMode) 
    SYMBOL_ITEM_FUNC(EDIT_SetSel) 
    SYMBOL_ITEM_FUNC(EDIT_SetpfUpdateBuffer) 
    SYMBOL_ITEM_FUNC(EDIT_SetpfAddKeyEx) 
    SYMBOL_ITEM_FUNC(EDIT_SetInsertMode) 
    SYMBOL_ITEM_FUNC(EDIT_SetFocussable) 
    SYMBOL_ITEM_FUNC(EDIT_SetCursorAtChar) 
    SYMBOL_ITEM_FUNC(EDIT_GetNumChars) 
    SYMBOL_ITEM_FUNC(EDIT_GetCursorCharPos) 
    SYMBOL_ITEM_FUNC(EDIT_GetCursorPixelPos) 
    SYMBOL_ITEM_FUNC(EDIT_EnableBlink) 
    SYMBOL_ITEM_FUNC(EDIT_GetDefaultBkColor) 
    SYMBOL_ITEM_FUNC(EDIT_GetDefaultFont) 
    SYMBOL_ITEM_FUNC(EDIT_GetDefaultTextAlign) 
    SYMBOL_ITEM_FUNC(EDIT_GetDefaultTextColor) 
    SYMBOL_ITEM_FUNC(EDIT_SetDefaultBkColor) 
    SYMBOL_ITEM_FUNC(EDIT_SetDefaultFont) 
    SYMBOL_ITEM_FUNC(EDIT_SetDefaultTextAlign) 
    SYMBOL_ITEM_FUNC(EDIT_SetDefaultTextColor) 
    SYMBOL_ITEM_FUNC(EDIT_CreateIndirect) 
    SYMBOL_ITEM_FUNC(EDIT_Create) 
    SYMBOL_ITEM_FUNC(EDIT_CreateAsChild) 
    SYMBOL_ITEM_FUNC(EDIT_SetUlongMode) 
    SYMBOL_ITEM_FUNC(EDIT_SetHexMode) 
    SYMBOL_ITEM_FUNC(EDIT_GetFloatValue) 
    SYMBOL_ITEM_FUNC(EDIT_SetFloatMode) 
    SYMBOL_ITEM_FUNC(EDIT_SetFloatValue) 
    SYMBOL_ITEM_FUNC(EDIT_SetDecMode) 
    SYMBOL_ITEM_FUNC(EDIT_SetBinMode) 
    SYMBOL_ITEM_FUNC(EDIT_AddKey) 
    SYMBOL_ITEM_FUNC(EDIT_Callback) 
    SYMBOL_ITEM_FUNC(EDIT_CreateEx) 
    SYMBOL_ITEM_FUNC(EDIT_CreateUser) 
    SYMBOL_ITEM_FUNC(EDIT_GetBkColor) 
    SYMBOL_ITEM_FUNC(EDIT_GetFont) 
    SYMBOL_ITEM_FUNC(EDIT_GetText) 
    SYMBOL_ITEM_FUNC(EDIT_GetTextColor) 
    SYMBOL_ITEM_FUNC(EDIT_GetUserData) 
    SYMBOL_ITEM_FUNC(EDIT_GetValue) 
    SYMBOL_ITEM_FUNC(EDIT_SetBkColor) 
    SYMBOL_ITEM_FUNC(EDIT_SetCursorAtPixel) 
    SYMBOL_ITEM_FUNC(EDIT_SetFont) 
    SYMBOL_ITEM_FUNC(EDIT_SetMaxLen) 
    SYMBOL_ITEM_FUNC(EDIT_SetText) 
    SYMBOL_ITEM_FUNC(EDIT_SetTextAlign) 
    SYMBOL_ITEM_FUNC(EDIT_SetTextColor) 
    SYMBOL_ITEM_FUNC(EDIT_SetUserData) 
    SYMBOL_ITEM_FUNC(EDIT_SetValue) 
    SYMBOL_ITEM_FUNC(EDIT__GetCurrentChar) 
    SYMBOL_ITEM_FUNC(EDIT__SetCursorPos) 
    SYMBOL_ITEM_FUNC(EDIT__SetValueUnsigned) 
    SYMBOL_ITEM_FUNC(DROPDOWN_DrawSkinFlex) 
    SYMBOL_ITEM_FUNC(DROPDOWN_GetSkinFlexProps) 
    SYMBOL_ITEM_FUNC(DROPDOWN_SetSkinFlexProps) 
    SYMBOL_ITEM_FUNC(DROPDOWN_SetDefaultSkinClassic) 
    SYMBOL_ITEM_FUNC(DROPDOWN_SetSkinClassic) 
    SYMBOL_ITEM_FUNC(DROPDOWN_SetDefaultSkin) 
    SYMBOL_ITEM_FUNC(DROPDOWN_SetSkin) 
    SYMBOL_ITEM_FUNC(DROPDOWN_SetTextHeight) 
    SYMBOL_ITEM_FUNC(DROPDOWN_SetTextColor) 
    SYMBOL_ITEM_FUNC(DROPDOWN_SetTextAlign) 
    SYMBOL_ITEM_FUNC(DROPDOWN_SetScrollbarWidth) 
    SYMBOL_ITEM_FUNC(DROPDOWN_SetScrollbarColor) 
    SYMBOL_ITEM_FUNC(DROPDOWN_SetFont) 
    SYMBOL_ITEM_FUNC(DROPDOWN_SetColor) 
    SYMBOL_ITEM_FUNC(DROPDOWN_SetBkColor) 
    SYMBOL_ITEM_FUNC(DROPDOWN_SetAutoScroll) 
    SYMBOL_ITEM_FUNC(DROPDOWN_GetItemSpacing) 
    SYMBOL_ITEM_FUNC(DROPDOWN_SetItemSpacing) 
    SYMBOL_ITEM_FUNC(DROPDOWN_GetItemDisabled) 
    SYMBOL_ITEM_FUNC(DROPDOWN_SetItemDisabled) 
    SYMBOL_ITEM_FUNC(DROPDOWN_InsertString) 
    SYMBOL_ITEM_FUNC(DROPDOWN_GetNumItems) 
    SYMBOL_ITEM_FUNC(DROPDOWN_DecSelExp) 
    SYMBOL_ITEM_FUNC(DROPDOWN_GetListbox) 
    SYMBOL_ITEM_FUNC(DROPDOWN_GetSelExp) 
    SYMBOL_ITEM_FUNC(DROPDOWN_IncSelExp) 
    SYMBOL_ITEM_FUNC(DROPDOWN_SetSelExp) 
    SYMBOL_ITEM_FUNC(DROPDOWN_DeleteItem) 
    SYMBOL_ITEM_FUNC(DROPDOWN_GetDefaultBkColor) 
    SYMBOL_ITEM_FUNC(DROPDOWN_GetDefaultColor) 
    SYMBOL_ITEM_FUNC(DROPDOWN_GetDefaultFont) 
    SYMBOL_ITEM_FUNC(DROPDOWN_GetDefaultScrollbarColor) 
    SYMBOL_ITEM_FUNC(DROPDOWN_SetDefaultBkColor) 
    SYMBOL_ITEM_FUNC(DROPDOWN_SetDefaultColor) 
    SYMBOL_ITEM_FUNC(DROPDOWN_SetDefaultFont) 
    SYMBOL_ITEM_FUNC(DROPDOWN_SetDefaultScrollbarColor) 
    SYMBOL_ITEM_FUNC(DROPDOWN_CreateIndirect) 
    SYMBOL_ITEM_FUNC(DROPDOWN_Create) 
    SYMBOL_ITEM_FUNC(DROPDOWN_AddString) 
    SYMBOL_ITEM_FUNC(DROPDOWN_AddKey) 
    SYMBOL_ITEM_FUNC(DROPDOWN_Callback) 
    SYMBOL_ITEM_FUNC(DROPDOWN_Collapse) 
    SYMBOL_ITEM_FUNC(DROPDOWN_CreateEx) 
    SYMBOL_ITEM_FUNC(DROPDOWN_CreateUser) 
    SYMBOL_ITEM_FUNC(DROPDOWN_DecSel) 
    SYMBOL_ITEM_FUNC(DROPDOWN_Expand) 
    SYMBOL_ITEM_FUNC(DROPDOWN_GetItemText) 
    SYMBOL_ITEM_FUNC(DROPDOWN_GetSel) 
    SYMBOL_ITEM_FUNC(DROPDOWN_GetUserData) 
    SYMBOL_ITEM_FUNC(DROPDOWN_IncSel) 
    SYMBOL_ITEM_FUNC(DROPDOWN_SetSel) 
    SYMBOL_ITEM_FUNC(DROPDOWN_SetUpMode) 
    SYMBOL_ITEM_FUNC(DROPDOWN_SetUserData) 
    SYMBOL_ITEM_FUNC(DROPDOWN__AdjustHeight) 
    SYMBOL_ITEM_FUNC(DROPDOWN__GetNumItems) 
    SYMBOL_ITEM_FUNC(DROPDOWN__GetpItemLocked) 
    SYMBOL_ITEM_FUNC(DIALOG_GetBkColor) 
    SYMBOL_ITEM_FUNC(DIALOG_SetBkColor) 
    SYMBOL_ITEM_FUNC(GUI_CreateDialogBox) 
    SYMBOL_ITEM_FUNC(GUI_EndDialog) 
    SYMBOL_ITEM_FUNC(GUI_ExecCreatedDialog) 
    SYMBOL_ITEM_FUNC(GUI_ExecDialogBox) 
    SYMBOL_ITEM_FUNC(GUI_GetDialogStatusPtr) 
    SYMBOL_ITEM_FUNC(GUI_SetDialogStatusPtr) 
    SYMBOL_ITEM_FUNC(CHOOSEFILE_Callback) 
    SYMBOL_ITEM_FUNC(CHOOSEFILE_Create) 
    SYMBOL_ITEM_FUNC(CHOOSEFILE_EnableToolTips) 
    SYMBOL_ITEM_FUNC(CHOOSEFILE_SetButtonText) 
    SYMBOL_ITEM_FUNC(CHOOSEFILE_SetDefaultButtonText) 
    SYMBOL_ITEM_FUNC(CHOOSEFILE_SetDelim) 
    SYMBOL_ITEM_FUNC(CHOOSEFILE_SetToolTips) 
    SYMBOL_ITEM_FUNC(CHOOSEFILE_SetTopMode) 
    SYMBOL_ITEM_FUNC(CHOOSECOLOR_Callback) 
    SYMBOL_ITEM_FUNC(CHOOSECOLOR_Create) 
    SYMBOL_ITEM_FUNC(CHOOSECOLOR_GetSel) 
    SYMBOL_ITEM_FUNC(CHOOSECOLOR_SetDefaultBorder) 
    SYMBOL_ITEM_FUNC(CHOOSECOLOR_SetDefaultButtonSize) 
    SYMBOL_ITEM_FUNC(CHOOSECOLOR_SetDefaultColor) 
    SYMBOL_ITEM_FUNC(CHOOSECOLOR_SetDefaultSpace) 
    SYMBOL_ITEM_FUNC(CHOOSECOLOR_SetSel) 
    SYMBOL_ITEM_FUNC(CHECKBOX_DrawSkinFlex) 
    SYMBOL_ITEM_FUNC(CHECKBOX_GetSkinFlexProps) 
    SYMBOL_ITEM_FUNC(CHECKBOX_SetSkinFlexProps) 
    SYMBOL_ITEM_FUNC(CHECKBOX_SetDefaultSkinClassic) 
    SYMBOL_ITEM_FUNC(CHECKBOX_SetSkinClassic) 
    SYMBOL_ITEM_FUNC(CHECKBOX_SetDefaultSkin) 
    SYMBOL_ITEM_FUNC(CHECKBOX_SetSkin) 
    SYMBOL_ITEM_FUNC(CHECKBOX_SetTextColor) 
    SYMBOL_ITEM_FUNC(CHECKBOX_SetTextAlign) 
    SYMBOL_ITEM_FUNC(CHECKBOX_SetText) 
    SYMBOL_ITEM_FUNC(CHECKBOX_SetState) 
    SYMBOL_ITEM_FUNC(CHECKBOX_SetSpacing) 
    SYMBOL_ITEM_FUNC(CHECKBOX_SetNumStates) 
    SYMBOL_ITEM_FUNC(CHECKBOX_SetImage) 
    SYMBOL_ITEM_FUNC(CHECKBOX_SetFont) 
    SYMBOL_ITEM_FUNC(CHECKBOX_SetFocusColor) 
    SYMBOL_ITEM_FUNC(CHECKBOX_SetDefaultImage) 
    SYMBOL_ITEM_FUNC(CHECKBOX_SetBoxBkColor) 
    SYMBOL_ITEM_FUNC(CHECKBOX_SetBkColor) 
    SYMBOL_ITEM_FUNC(CHECKBOX_IsChecked) 
    SYMBOL_ITEM_FUNC(CHECKBOX_GetText) 
    SYMBOL_ITEM_FUNC(CHECKBOX_GetState) 
    SYMBOL_ITEM_FUNC(CHECKBOX_GetDefaultAlign) 
    SYMBOL_ITEM_FUNC(CHECKBOX_GetDefaultBkColor) 
    SYMBOL_ITEM_FUNC(CHECKBOX_GetDefaultFont) 
    SYMBOL_ITEM_FUNC(CHECKBOX_GetDefaultSpacing) 
    SYMBOL_ITEM_FUNC(CHECKBOX_GetDefaultTextColor) 
    SYMBOL_ITEM_FUNC(CHECKBOX_SetDefaultAlign) 
    SYMBOL_ITEM_FUNC(CHECKBOX_SetDefaultBkColor) 
    SYMBOL_ITEM_FUNC(CHECKBOX_SetDefaultFocusColor) 
    SYMBOL_ITEM_FUNC(CHECKBOX_SetDefaultFont) 
    SYMBOL_ITEM_FUNC(CHECKBOX_SetDefaultSpacing) 
    SYMBOL_ITEM_FUNC(CHECKBOX_SetDefaultTextColor) 
    SYMBOL_ITEM_FUNC(CHECKBOX_CreateIndirect) 
    SYMBOL_ITEM_FUNC(CHECKBOX_Create) 
    SYMBOL_ITEM_FUNC(CHECKBOX_Callback) 
    SYMBOL_ITEM_FUNC(CHECKBOX_CreateEx) 
    SYMBOL_ITEM_FUNC(CHECKBOX_CreateUser) 
    SYMBOL_ITEM_FUNC(CHECKBOX_GetUserData) 
    SYMBOL_ITEM_FUNC(CHECKBOX_SetUserData) 
    SYMBOL_ITEM_FUNC(CHECKBOX__GetButtonSize) 
    SYMBOL_ITEM_FUNC(BUTTON__SetDrawObj) 
    SYMBOL_ITEM_FUNC(BUTTON_SetStreamedBitmap) 
    SYMBOL_ITEM_FUNC(BUTTON_SetStreamedBitmapEx) 
    SYMBOL_ITEM_FUNC(BUTTON_DrawSkinFlex) 
    SYMBOL_ITEM_FUNC(BUTTON_GetSkinFlexProps) 
    SYMBOL_ITEM_FUNC(BUTTON_SetSkinFlexProps) 
    SYMBOL_ITEM_FUNC(BUTTON_SetDefaultSkinClassic) 
    SYMBOL_ITEM_FUNC(BUTTON_SetSkinClassic) 
    SYMBOL_ITEM_FUNC(BUTTON_SetDefaultSkin) 
    SYMBOL_ITEM_FUNC(BUTTON_SetSkin) 
    SYMBOL_ITEM_FUNC(BUTTON_GetTextAlign) 
    SYMBOL_ITEM_FUNC(BUTTON_SetTextAlign) 
    SYMBOL_ITEM_FUNC(BUTTON_SetFocusColor) 
    SYMBOL_ITEM_FUNC(BUTTON_SetSelfDraw) 
    SYMBOL_ITEM_FUNC(BUTTON_SetSelfDrawEx) 
    SYMBOL_ITEM_FUNC(BUTTON_IsPressed) 
    SYMBOL_ITEM_FUNC(BUTTON_GetBitmap) 
    SYMBOL_ITEM_FUNC(BUTTON_GetFont) 
    SYMBOL_ITEM_FUNC(BUTTON_GetText) 
    SYMBOL_ITEM_FUNC(BUTTON_GetDefaultBkColor) 
    SYMBOL_ITEM_FUNC(BUTTON_GetDefaultFont) 
    SYMBOL_ITEM_FUNC(BUTTON_GetDefaultTextAlign) 
    SYMBOL_ITEM_FUNC(BUTTON_GetDefaultTextColor) 
    SYMBOL_ITEM_FUNC(BUTTON_SetDefaultBkColor) 
    SYMBOL_ITEM_FUNC(BUTTON_SetDefaultFocusColor) 
    SYMBOL_ITEM_FUNC(BUTTON_SetDefaultFont) 
    SYMBOL_ITEM_FUNC(BUTTON_SetDefaultTextAlign) 
    SYMBOL_ITEM_FUNC(BUTTON_SetDefaultTextColor) 
    SYMBOL_ITEM_FUNC(BUTTON_CreateIndirect) 
    SYMBOL_ITEM_FUNC(BUTTON_Create) 
    SYMBOL_ITEM_FUNC(BUTTON_CreateAsChild) 
    SYMBOL_ITEM_FUNC(BUTTON_GetBkColor) 
    SYMBOL_ITEM_FUNC(BUTTON_GetFrameColor) 
    SYMBOL_ITEM_FUNC(BUTTON_GetTextColor) 
    SYMBOL_ITEM_FUNC(BUTTON_SetBkColor) 
    SYMBOL_ITEM_FUNC(BUTTON_SetFrameColor) 
    SYMBOL_ITEM_FUNC(BUTTON_SetTextColor) 
    SYMBOL_ITEM_FUNC(BUTTON_SetBMP) 
    SYMBOL_ITEM_FUNC(BUTTON_SetBMPEx) 
    SYMBOL_ITEM_FUNC(BUTTON_SetBitmap) 
    SYMBOL_ITEM_FUNC(BUTTON_SetBitmapEx) 
    SYMBOL_ITEM_FUNC(BUTTON_Callback) 
    SYMBOL_ITEM_FUNC(BUTTON_CreateEx) 
    SYMBOL_ITEM_FUNC(BUTTON_CreateUser) 
    SYMBOL_ITEM_FUNC(BUTTON_GetUserData) 
    SYMBOL_ITEM_FUNC(BUTTON_SetFocussable) 
    SYMBOL_ITEM_FUNC(BUTTON_SetFont) 
    SYMBOL_ITEM_FUNC(BUTTON_SetPressed) 
    SYMBOL_ITEM_FUNC(BUTTON_SetReactOnLevel) 
    SYMBOL_ITEM_FUNC(BUTTON_SetReactOnTouch) 
    SYMBOL_ITEM_FUNC(BUTTON_SetState) 
    SYMBOL_ITEM_FUNC(BUTTON_SetText) 
    SYMBOL_ITEM_FUNC(BUTTON_SetTextOffset) 
    SYMBOL_ITEM_FUNC(BUTTON_SetUserData) 
    SYMBOL_ITEM_FUNC(GUI_VNC_AttachToLayer) 
    SYMBOL_ITEM_FUNC(GUI_VNC_EnableKeyboardInput) 
    SYMBOL_ITEM_FUNC(GUI_VNC_GetNumConnections) 
    SYMBOL_ITEM_FUNC(GUI_VNC_Process) 
    SYMBOL_ITEM_FUNC(GUI_VNC_RingBell) 
    SYMBOL_ITEM_FUNC(GUI_VNC_SetAuthentication) 
    SYMBOL_ITEM_FUNC(GUI_VNC_SetLockFrame) 
    SYMBOL_ITEM_FUNC(GUI_VNC_SetProgName) 
    SYMBOL_ITEM_FUNC(GUI_VNC_SetSize) 
    SYMBOL_ITEM_FUNC(GUI_VNC_DoDES) 
    SYMBOL_ITEM_FUNC(GUI_VNC_SetDESKey) 
    SYMBOL_ITEM_FUNC(GUI_VNC_SetPassword) 
    SYMBOL_ITEM_FUNC(GUI_MEMDEV__XY2PTR) 
    SYMBOL_ITEM_FUNC(GUI_MEMDEV__XY2PTREx) 
    SYMBOL_ITEM_FUNC(GUI_MEMDEV__WriteToActiveOpaque) 
    SYMBOL_ITEM_FUNC(GUI_MEMDEV__WriteToActiveAlpha) 
    SYMBOL_ITEM_FUNC(GUI_MEMDEV_WriteEx) 
    SYMBOL_ITEM_FUNC(GUI_MEMDEV_WriteExAt) 
    SYMBOL_ITEM_FUNC(GUI_MEMDEV_WriteAlpha) 
    SYMBOL_ITEM_FUNC(GUI_MEMDEV_WriteAlphaAt) 
    SYMBOL_ITEM_FUNC(GUI_MEMDEV_Write) 
    SYMBOL_ITEM_FUNC(GUI_MEMDEV_WriteAt) 
    SYMBOL_ITEM_FUNC(GUI_USAGE_BM_Create) 
    SYMBOL_ITEM_FUNC(GUI_USAGE_AddRect) 
    SYMBOL_ITEM_FUNC(GUI_USAGE_DecUseCnt) 
    SYMBOL_ITEM_FUNC(GUI_USAGE_Select) 
    SYMBOL_ITEM_FUNC(GUIDEV_StaticDevices_C) 
    SYMBOL_ITEM_FUNC(GUI_MEMDEV_CreateStatic) 
    SYMBOL_ITEM_FUNC(GUI_MEMDEV_GetStaticDevice) 
    SYMBOL_ITEM_FUNC(GUI_MEMDEV_GetWindowDevice) 
    SYMBOL_ITEM_FUNC(GUI_MEMDEV_Paint1Static) 
    SYMBOL_ITEM_FUNC(GUI_MEMDEV_SetOrg) 
    SYMBOL_ITEM_FUNC(GUI_MEMDEV_SetColorConv) 
    SYMBOL_ITEM_FUNC(GUI_MEMDEV_SerializeBMP) 
    SYMBOL_ITEM_FUNC(GUI_MEMDEV_RotateHQT) 
    SYMBOL_ITEM_FUNC(GUI_MEMDEV_Rotate) 
    SYMBOL_ITEM_FUNC(GUI_MEMDEV_RotateHQ) 
    SYMBOL_ITEM_FUNC(GUI_MEMDEV__Rotate) 
    SYMBOL_ITEM_FUNC(GUI_MEMDEV_ReduceYSize) 
    SYMBOL_ITEM_FUNC(GUI_MEMDEV__ReadLine) 
    SYMBOL_ITEM_FUNC(GUI_MEMDEV_DrawPerspectiveX) 
    SYMBOL_ITEM_FUNC(GUI_MEMDEV__DrawSizedAt) 
    SYMBOL_ITEM_FUNC(GUIDEV_MoveAndFadeWin_C) 
    SYMBOL_ITEM_FUNC(GUI_MEMDEV_FadeInWindow) 
    SYMBOL_ITEM_FUNC(GUI_MEMDEV_FadeOutWindow) 
    SYMBOL_ITEM_FUNC(GUI_MEMDEV_MoveInWindow) 
    SYMBOL_ITEM_FUNC(GUI_MEMDEV_MoveOutWindow) 
    SYMBOL_ITEM_FUNC(GUI_MEMDEV_ShiftInWindow) 
    SYMBOL_ITEM_FUNC(GUI_MEMDEV_ShiftOutWindow) 
    SYMBOL_ITEM_FUNC(GUI_MEMDEV_SwapWindow) 
    SYMBOL_ITEM_FUNC(GUI_MEMDEV_FadeDevices) 
    SYMBOL_ITEM_FUNC(GUI_MEMDEV_SetAnimationCallback) 
    SYMBOL_ITEM_FUNC(GUI_MEMDEV__FadeDevice) 
    SYMBOL_ITEM_FUNC(GUI_MEMDEV__FadeDeviceEx) 
    SYMBOL_ITEM_FUNC(GUI_MEASDEV_ClearRect) 
    SYMBOL_ITEM_FUNC(GUI_MEASDEV_Create) 
    SYMBOL_ITEM_FUNC(GUI_MEASDEV_Delete) 
    SYMBOL_ITEM_FUNC(GUI_MEASDEV_GetRect) 
    SYMBOL_ITEM_FUNC(GUI_MEASDEV_Select) 
    SYMBOL_ITEM_FUNC(GUI_MEMDEV_GetYSize) 
    SYMBOL_ITEM_FUNC(GUI_MEMDEV_GetXSize) 
    SYMBOL_ITEM_FUNC(GUI_MEMDEV_GetXPos) 
    SYMBOL_ITEM_FUNC(GUI_MEMDEV_GetYPos) 
    SYMBOL_ITEM_FUNC(GUI_MEMDEV_GetDataPtr) 
    SYMBOL_ITEM_FUNC(GUI_MEMDEV_CreateFixed) 
    SYMBOL_ITEM_FUNC(GUI_MEMDEV_CopyFromLCD) 
    SYMBOL_ITEM_FUNC(GUI_MEMDEV_CompareWithLCD) 
    SYMBOL_ITEM_FUNC(GUI_MEMDEV_Clear) 
    SYMBOL_ITEM_FUNC(GUI_MEMDEV_Draw) 
    SYMBOL_ITEM_FUNC(GUI_MEMDEV__SetAlphaCallback) 
    SYMBOL_ITEM_FUNC(GUI_MEMDEV_CreateAuto) 
    SYMBOL_ITEM_FUNC(GUI_MEMDEV_DeleteAuto) 
    SYMBOL_ITEM_FUNC(GUI_MEMDEV_DrawAuto) 
    SYMBOL_ITEM_FUNC(GUI_MEMDEV_CopyFromLCDAA) 
    SYMBOL_ITEM_FUNC(GUI_MEMDEV_CopyToLCDAA) 
    SYMBOL_ITEM_FUNC(GUI_MEMDEV_CopyToLCD) 
    SYMBOL_ITEM_FUNC(GUI_MEMDEV_CopyToLCDAt) 
    SYMBOL_ITEM_FUNC(GUI_MEMDEV_Create) 
    SYMBOL_ITEM_FUNC(GUI_MEMDEV_CreateEx) 
    SYMBOL_ITEM_FUNC(GUI_MEMDEV_Delete) 
    SYMBOL_ITEM_FUNC(GUI_MEMDEV_GetBitsPerPixel) 
    SYMBOL_ITEM_FUNC(GUI_MEMDEV_MarkDirty) 
    SYMBOL_ITEM_FUNC(GUI_MEMDEV_Select) 
    SYMBOL_ITEM_FUNC(GUI_MEMDEV__CreateFixed) 
    SYMBOL_ITEM_FUNC(GUI_MEMDEV__WriteToActiveAt) 
    SYMBOL_ITEM_FUNC(LCDWin_c) 
    SYMBOL_ITEM_FUNC(GUIDRV_TemplateI_Config) 
    SYMBOL_ITEM_FUNC(GUIDRV_TemplateI_SetBus_XXX) 
    SYMBOL_ITEM_FUNC(GUIDRV__TemplateI_GetDevFunc) 
    SYMBOL_ITEM_FUNC(GUIDRV__TemplateI_GetDevProp) 
    SYMBOL_ITEM_FUNC(GUIDRV__TemplateI_GetRect) 
    SYMBOL_ITEM_FUNC(GUIDRV__TemplateI_SetOrg) 
    SYMBOL_ITEM_FUNC(GUIDRV_SSD1926_Config) 
    SYMBOL_ITEM_FUNC(GUIDRV_SSD1926_SetBus16) 
    SYMBOL_ITEM_FUNC(GUIDRV__SSD1926_GetDevFunc) 
    SYMBOL_ITEM_FUNC(GUIDRV__SSD1926_GetDevProp) 
    SYMBOL_ITEM_FUNC(GUIDRV__SSD1926_GetRect) 
    SYMBOL_ITEM_FUNC(GUIDRV__SSD1926_SetOrg) 
    SYMBOL_ITEM_FUNC(GUIDRV_SPage_SetUC1611) 
    SYMBOL_ITEM_FUNC(GUIDRV_SPage_SetST7591) 
    SYMBOL_ITEM_FUNC(GUIDRV_SPage_Set1512) 
    SYMBOL_ITEM_FUNC(GUIDRV_SPage_SetS1D15) 
    SYMBOL_ITEM_FUNC(GUIDRV_SPage_Set1510) 
    SYMBOL_ITEM_FUNC(GUIDRV__SPage_AddDirtyRect) 
    SYMBOL_ITEM_FUNC(GUIDRV__SPage_ControlCache) 
    SYMBOL_ITEM_FUNC(GUIDRV__SPage_FlushCache) 
    SYMBOL_ITEM_FUNC(GUIDRV__SPage_ReadCache) 
    SYMBOL_ITEM_FUNC(GUIDRV__SPage_WriteCache) 
    SYMBOL_ITEM_FUNC(GUIDRV_SPage_Config) 
    SYMBOL_ITEM_FUNC(GUIDRV_SPage_SetBus8) 
    SYMBOL_ITEM_FUNC(GUIDRV__SPage_GetDevFunc) 
    SYMBOL_ITEM_FUNC(GUIDRV__SPage_GetDevProp) 
    SYMBOL_ITEM_FUNC(GUIDRV__SPage_GetRect) 
    SYMBOL_ITEM_FUNC(GUIDRV__SPage_Init) 
    SYMBOL_ITEM_FUNC(GUIDRV__SPage_SetOrg) 
    SYMBOL_ITEM_FUNC(GUIDRV_SLin_SetUC1617) 
    SYMBOL_ITEM_FUNC(GUIDRV_SLin_SetT6963) 
    SYMBOL_ITEM_FUNC(GUIDRV_SLin_SetSSD1848) 
    SYMBOL_ITEM_FUNC(GUIDRV_SLin_SetS1D13700) 
    SYMBOL_ITEM_FUNC(GUIDRV_SLin_Config) 
    SYMBOL_ITEM_FUNC(GUIDRV_SLin_SetBus8) 
    SYMBOL_ITEM_FUNC(GUIDRV__SLin_GetDevFunc) 
    SYMBOL_ITEM_FUNC(GUIDRV__SLin_GetDevProp) 
    SYMBOL_ITEM_FUNC(GUIDRV__SLin_GetRect) 
    SYMBOL_ITEM_FUNC(GUIDRV__SLin_SendCache) 
    SYMBOL_ITEM_FUNC(GUIDRV__SLin_SetOrg) 
    SYMBOL_ITEM_FUNC(GUIDRV__SLin_WriteVMEM) 
    SYMBOL_ITEM_FUNC(GUIDRV_S1D15G00_Config) 
    SYMBOL_ITEM_FUNC(GUIDRV_S1D15G00_SetBus8) 
    SYMBOL_ITEM_FUNC(GUIDRV_S1D13781_Config) 
    SYMBOL_ITEM_FUNC(GUIDRV_S1D13781_SetBusSPI) 
    SYMBOL_ITEM_FUNC(GUIDRV__S1D13781_GetAddr) 
    SYMBOL_ITEM_FUNC(GUIDRV__S1D13781_GetDevFunc) 
    SYMBOL_ITEM_FUNC(GUIDRV__S1D13781_GetDevProp) 
    SYMBOL_ITEM_FUNC(GUIDRV__S1D13781_GetRect) 
    SYMBOL_ITEM_FUNC(GUIDRV__S1D13781_Init) 
    SYMBOL_ITEM_FUNC(GUIDRV__S1D13781_InitOnce) 
    SYMBOL_ITEM_FUNC(GUIDRV__S1D13781_SetOrg) 
    SYMBOL_ITEM_FUNC(GUIDRV_S1D13748_Config) 
    SYMBOL_ITEM_FUNC(GUIDRV_S1D13748_SetBus16) 
    SYMBOL_ITEM_FUNC(GUIDRV__S1D13748_GetDevFunc) 
    SYMBOL_ITEM_FUNC(GUIDRV__S1D13748_GetDevProp) 
    SYMBOL_ITEM_FUNC(GUIDRV__S1D13748_GetRect) 
    SYMBOL_ITEM_FUNC(GUIDRV__S1D13748_SetOrg) 
    SYMBOL_ITEM_FUNC(GUIDRV__NoOpt_DrawBitmap) 
    SYMBOL_ITEM_FUNC(GUIDRV__NoOpt_DrawHLine) 
    SYMBOL_ITEM_FUNC(GUIDRV__NoOpt_DrawVLine) 
    SYMBOL_ITEM_FUNC(GUIDRV__NoOpt_FillRect) 
    SYMBOL_ITEM_FUNC(GUIDRV__NoOpt_XorPixel) 
    SYMBOL_ITEM_FUNC(GUIDRV_IST3088_SetBus16) 
    SYMBOL_ITEM_FUNC(GUIDRV__IST3088_GetDevFunc) 
    SYMBOL_ITEM_FUNC(GUIDRV__IST3088_GetDevProp) 
    SYMBOL_ITEM_FUNC(GUIDRV__IST3088_GetRect) 
    SYMBOL_ITEM_FUNC(GUIDRV__IST3088_SetOrg) 
    SYMBOL_ITEM_FUNC(GUIDRV_FlexColor_SetFunc66720) 
    SYMBOL_ITEM_FUNC(GUIDRV_FlexColor_SetReadFunc66720_B16) 
    SYMBOL_ITEM_FUNC(GUIDRV_FlexColor_SetFunc66719) 
    SYMBOL_ITEM_FUNC(GUIDRV_FlexColor_SetFunc66718) 
    SYMBOL_ITEM_FUNC(GUIDRV_FlexColor_SetFunc66715) 
    SYMBOL_ITEM_FUNC(GUIDRV_FlexColor_SetFunc66714) 
    SYMBOL_ITEM_FUNC(GUIDRV_FlexColor_SetFunc66712) 
    SYMBOL_ITEM_FUNC(GUIDRV_FlexColor_SetInterface66712_B18) 
    SYMBOL_ITEM_FUNC(GUIDRV_FlexColor_SetInterface66712_B9) 
    SYMBOL_ITEM_FUNC(GUIDRV_FlexColor_SetReadFunc66712_B16) 
    SYMBOL_ITEM_FUNC(GUIDRV_FlexColor_SetReadFunc66712_B9) 
    SYMBOL_ITEM_FUNC(GUIDRV_FlexColor__SetFunc66712) 
    SYMBOL_ITEM_FUNC(GUIDRV_FlexColor_SetFunc66709) 
    SYMBOL_ITEM_FUNC(GUIDRV_FlexColor_SetReadFunc66709_B16) 
    SYMBOL_ITEM_FUNC(GUIDRV_FlexColor_SetFunc66708) 
    SYMBOL_ITEM_FUNC(GUIDRV_FlexColor_SetFunc66702) 
    SYMBOL_ITEM_FUNC(GUIDRV_FlexColor_SetMode18bppC1B9) 
    SYMBOL_ITEM_FUNC(GUIDRV_FlexColor_SetMode18bppC1B18) 
    SYMBOL_ITEM_FUNC(GUIDRV_FlexColor_SetMode18bppC0B9) 
    SYMBOL_ITEM_FUNC(GUIDRV_FlexColor_SetMode18bppC0B18) 
    SYMBOL_ITEM_FUNC(GUIDRV_FlexColor_SetMode16bppC1B8) 
    SYMBOL_ITEM_FUNC(GUIDRV_FlexColor_SetMode16bppC1B16) 
    SYMBOL_ITEM_FUNC(GUIDRV_FlexColor_SetMode16bppC0B8) 
    SYMBOL_ITEM_FUNC(GUIDRV_FlexColor_SetMode16bppC0B16) 
    SYMBOL_ITEM_FUNC(GUIDRV_FlexColor_Config) 
    SYMBOL_ITEM_FUNC(GUIDRV_FlexColor_SetFunc) 
    SYMBOL_ITEM_FUNC(GUIDRV_FlexColor__AddCacheRect) 
    SYMBOL_ITEM_FUNC(GUIDRV_FlexColor__ControlCache) 
    SYMBOL_ITEM_FUNC(GUIDRV_FlexColor__GetPixelIndexCache) 
    SYMBOL_ITEM_FUNC(GUIDRV_FlexColor__GetPixelIndexNoCache) 
    SYMBOL_ITEM_FUNC(GUIDRV_FlexColor__IncrementCursor) 
    SYMBOL_ITEM_FUNC(GUIDRV_FlexColor__InitOnce) 
    SYMBOL_ITEM_FUNC(GUIDRV_FlexColor__ManageRect) 
    SYMBOL_ITEM_FUNC(GUIDRV_FlexColor__Refresh) 
    SYMBOL_ITEM_FUNC(GUIDRV_FlexColor__SetCacheAddr) 
    SYMBOL_ITEM_FUNC(GUIDRV_FlexColor__SetCacheRect) 
    SYMBOL_ITEM_FUNC(GUIDRV_FlexColor__SetLog2Phys) 
    SYMBOL_ITEM_FUNC(GUIDRV_FlexColor__SetPixelIndexCache) 
    SYMBOL_ITEM_FUNC(GUIDRV_FlexColor__SetPixelIndexNoCache) 
    SYMBOL_ITEM_FUNC(GUIDRV_FlexColor__SetSubRect) 
    SYMBOL_ITEM_FUNC(GUIDRV_Dist_AddDriver) 
    SYMBOL_ITEM_FUNC(GUIDRV_DCache_SetMode1bpp) 
    SYMBOL_ITEM_FUNC(GUIDRV_DCache_AddDriver) 
    SYMBOL_ITEM_FUNC(GUIDRV_DCache__AddDirtyRect) 
    SYMBOL_ITEM_FUNC(GUIDRV_DCache__ClearDirtyRect) 
    SYMBOL_ITEM_FUNC(GUIDRV_DCache__InitOnce) 
    SYMBOL_ITEM_FUNC(GUIDRV_BitPlains_Config) 
    SYMBOL_ITEM_FUNC(LCD_UpdateColorIndices) 
    SYMBOL_ITEM_FUNC(LCD_SetClipRectEx) 
    SYMBOL_ITEM_FUNC(LCD_GetPos) 
    SYMBOL_ITEM_FUNC(LCD_GetPosEx) 
    SYMBOL_ITEM_FUNC(LCD_Refresh) 
    SYMBOL_ITEM_FUNC(LCD_RefreshEx) 
    SYMBOL_ITEM_FUNC(LCD_SetAlpha) 
    SYMBOL_ITEM_FUNC(LCD_SetAlphaEx) 
    SYMBOL_ITEM_FUNC(LCD_SetAlphaMode) 
    SYMBOL_ITEM_FUNC(LCD_SetAlphaModeEx) 
    SYMBOL_ITEM_FUNC(LCD_SetChroma) 
    SYMBOL_ITEM_FUNC(LCD_SetChromaEx) 
    SYMBOL_ITEM_FUNC(LCD_SetChromaMode) 
    SYMBOL_ITEM_FUNC(LCD_SetChromaModeEx) 
    SYMBOL_ITEM_FUNC(LCD_SetDevFunc) 
    SYMBOL_ITEM_FUNC(LCD_SetLUTEntry) 
    SYMBOL_ITEM_FUNC(LCD_SetLUTEntryEx) 
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
    SYMBOL_ITEM_FUNC(LCD_SelectLCD) 
    SYMBOL_ITEM_FUNC(LCD__RLE8_SetFunc) 
    SYMBOL_ITEM_FUNC(LCD__RLE4_SetFunc) 
    SYMBOL_ITEM_FUNC(LCD__RLE32_SetFunc) 
    SYMBOL_ITEM_FUNC(LCD__RLE16_SetFunc) 
    SYMBOL_ITEM_FUNC(GUI_ReadRect) 
    SYMBOL_ITEM_FUNC(GUI_ReadRectEx) 
    SYMBOL_ITEM_FUNC(LCD_ReadRect) 
    SYMBOL_ITEM_FUNC(LCD_ReadRectNoClip) 
    SYMBOL_ITEM_FUNC(LCD_GetNextPixelAPI) 
    SYMBOL_ITEM_FUNC(LCD_MixColors256) 
    SYMBOL_ITEM_FUNC(LCD_GetDevCap) 
    SYMBOL_ITEM_FUNC(LCD_GetDevCapEx) 
    SYMBOL_ITEM_FUNC(LCD_Index2ColorEx) 
    SYMBOL_ITEM_FUNC(LCD_GetPixelColor) 
    SYMBOL_ITEM_FUNC(GUI_GetPixelIndex) 
    SYMBOL_ITEM_FUNC(LCD_GetPixelIndex) 
    SYMBOL_ITEM_FUNC(LCD_GetNumLayers) 
    SYMBOL_ITEM_FUNC(GUI_GetBitsPerPixelEx) 
    SYMBOL_ITEM_FUNC(GUI_GetpfColor2IndexEx) 
    SYMBOL_ITEM_FUNC(GUI_GetpfIndex2ColorEx) 
    SYMBOL_ITEM_FUNC(LCD_GetBitsPerPixel) 
    SYMBOL_ITEM_FUNC(LCD_GetBitsPerPixelEx) 
    SYMBOL_ITEM_FUNC(LCD_GetBitsPerPixelMax) 
    SYMBOL_ITEM_FUNC(LCD_GetMirrorX) 
    SYMBOL_ITEM_FUNC(LCD_GetMirrorXEx) 
    SYMBOL_ITEM_FUNC(LCD_GetMirrorY) 
    SYMBOL_ITEM_FUNC(LCD_GetMirrorYEx) 
    SYMBOL_ITEM_FUNC(LCD_GetNumColors) 
    SYMBOL_ITEM_FUNC(LCD_GetNumColorsEx) 
    SYMBOL_ITEM_FUNC(LCD_GetPalette) 
    SYMBOL_ITEM_FUNC(LCD_GetPaletteEx) 
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
    SYMBOL_ITEM_FUNC(LCD_GetpfColor2IndexEx) 
    SYMBOL_ITEM_FUNC(LCD_GetpfIndex2ColorEx) 
    SYMBOL_ITEM_FUNC(LCD__GetBPP) 
    SYMBOL_ITEM_FUNC(LCD__GetBPPDevice) 
    SYMBOL_ITEM_FUNC(LCD_GetBkColorIndex) 
    SYMBOL_ITEM_FUNC(LCD_GetColorIndex) 
    SYMBOL_ITEM_FUNC(LCD_ControlCache) 
    SYMBOL_ITEM_FUNC(LCD_ControlCacheEx) 
    SYMBOL_ITEM_FUNC(LCD_GetpPalConvTable) 
    SYMBOL_ITEM_FUNC(LCD_GetpPalConvTableUncached) 
    SYMBOL_ITEM_FUNC(LCD_InitColors) 
    SYMBOL_ITEM_FUNC(LCD_InitLUT) 
    SYMBOL_ITEM_FUNC(LCD_SetMaxNumColors) 
    SYMBOL_ITEM_FUNC(LCD_AA_MixColors) 
    SYMBOL_ITEM_FUNC(LCD_AA_SetAndMask) 
    SYMBOL_ITEM_FUNC(LCD_SetPixelAA) 
    SYMBOL_ITEM_FUNC(LCD_SetPixelAA_NoTrans) 
    SYMBOL_ITEM_FUNC(LCD_SetPixelAA_Xor) 
    SYMBOL_ITEM_FUNC(LCD_Color2Index) 
    SYMBOL_ITEM_FUNC(LCD_DrawBitmap) 
    SYMBOL_ITEM_FUNC(LCD_DrawHLine) 
    SYMBOL_ITEM_FUNC(LCD_DrawPixel) 
    SYMBOL_ITEM_FUNC(LCD_DrawVLine) 
    SYMBOL_ITEM_FUNC(LCD_FillRect) 
    SYMBOL_ITEM_FUNC(LCD_Index2Color) 
    SYMBOL_ITEM_FUNC(LCD_Init) 
    SYMBOL_ITEM_FUNC(LCD_SetBkColor) 
    SYMBOL_ITEM_FUNC(LCD_SetBkColorIndex) 
    SYMBOL_ITEM_FUNC(LCD_SetClipRectMax) 
    SYMBOL_ITEM_FUNC(LCD_SetColor) 
    SYMBOL_ITEM_FUNC(LCD_SetColorIndex) 
    SYMBOL_ITEM_FUNC(LCD_SetDrawMode) 
    SYMBOL_ITEM_FUNC(LCD__DrawBitmap_1bpp) 
    SYMBOL_ITEM_FUNC(GUI__WrapGetNumBytesToNextLine) 
    SYMBOL_ITEM_FUNC(GUI__WrapGetNumCharsDisp) 
    SYMBOL_ITEM_FUNC(GUI__WrapGetNumCharsToNextLine) 
    SYMBOL_ITEM_FUNC(GUI__strlen) 
    SYMBOL_ITEM_FUNC(GUI__strcmp) 
    SYMBOL_ITEM_FUNC(GUI__strcmp_hp) 
    SYMBOL_ITEM_FUNC(GUI__sqrt32) 
    SYMBOL_ITEM_FUNC(GUI__CosHQ) 
    SYMBOL_ITEM_FUNC(GUI__SinHQ) 
    SYMBOL_ITEM_FUNC(GUI__SetText) 
    SYMBOL_ITEM_FUNC(GUI__MixColors) 
    SYMBOL_ITEM_FUNC(GUI__SetPixelAlpha) 
    SYMBOL_ITEM_FUNC(GUI__ReduceRect) 
    SYMBOL_ITEM_FUNC(GUI__Read16) 
    SYMBOL_ITEM_FUNC(GUI__Read32) 
    SYMBOL_ITEM_FUNC(GUI__memset16) 
    SYMBOL_ITEM_FUNC(GUI__memset) 
    SYMBOL_ITEM_FUNC(GUI__memcpy) 
    SYMBOL_ITEM_FUNC(GUI__ManageCache) 
    SYMBOL_ITEM_FUNC(GUI__ManageCacheEx) 
    SYMBOL_ITEM_FUNC(GUI__IntersectRects) 
    SYMBOL_ITEM_FUNC(GUI__IntersectRect) 
    SYMBOL_ITEM_FUNC(GUI__HandleEOLine) 
    SYMBOL_ITEM_FUNC(GUI__GetNumChars) 
    SYMBOL_ITEM_FUNC(GUI__GetFontSizeY) 
    SYMBOL_ITEM_FUNC(GUI__GetCursorCharacter) 
    SYMBOL_ITEM_FUNC(GUI__GetCursorPosChar) 
    SYMBOL_ITEM_FUNC(GUI__GetCursorPosX) 
    SYMBOL_ITEM_FUNC(GUI__ExpandPixelIndices) 
    SYMBOL_ITEM_FUNC(GUI__ExpandPixelIndicesEx) 
    SYMBOL_ITEM_FUNC(GUI__DrawTwinArc2) 
    SYMBOL_ITEM_FUNC(GUI__DrawTwinArc4) 
    SYMBOL_ITEM_FUNC(GUI__FillTrippleArc) 
    SYMBOL_ITEM_FUNC(GUI__DrawBitmap16bpp) 
    SYMBOL_ITEM_FUNC(GUI__DivideRound32) 
    SYMBOL_ITEM_FUNC(GUI__DivideRound) 
    SYMBOL_ITEM_FUNC(GUI__CompactPixelIndices) 
    SYMBOL_ITEM_FUNC(GUI__CompactPixelIndicesEx) 
    SYMBOL_ITEM_FUNC(GUI__CalcTextRect) 
    SYMBOL_ITEM_FUNC(GUI__BIDI_GetCharDir) 
    SYMBOL_ITEM_FUNC(GUI__BIDI_GetCharWrap) 
    SYMBOL_ITEM_FUNC(GUI__BIDI_GetCursorCharacter) 
    SYMBOL_ITEM_FUNC(GUI__BIDI_GetCursorPosChar) 
    SYMBOL_ITEM_FUNC(GUI__BIDI_GetCursorPosX) 
    SYMBOL_ITEM_FUNC(GUI__BIDI_GetLogChar) 
    SYMBOL_ITEM_FUNC(GUI__BIDI_GetWordWrap) 
    SYMBOL_ITEM_FUNC(GUI__BIDI_IsNSM) 
    SYMBOL_ITEM_FUNC(GUI__BIDI_Log2Vis) 
    SYMBOL_ITEM_FUNC(GUI__GetPresentationForm) 
    SYMBOL_ITEM_FUNC(GUI__IsArabicCharacter) 
    SYMBOL_ITEM_FUNC(GUI__AllocAlphaBuffer) 
    SYMBOL_ITEM_FUNC(GUI__DoAlphaBlending) 
    SYMBOL_ITEM_FUNC(GUI__GetAlphaBuffer) 
    SYMBOL_ITEM_FUNC(GUI__SetAlphaBufferSize) 
    SYMBOL_ITEM_FUNC(GUI__AddSpaceHex) 
    SYMBOL_ITEM_FUNC(GUI_XBF_CreateFont) 
    SYMBOL_ITEM_FUNC(GUI_XBF_DeleteFont) 
    SYMBOL_ITEM_FUNC(GUI_XBF__ClearLine) 
    SYMBOL_ITEM_FUNC(GUI_XBF__GetCharDistX) 
    SYMBOL_ITEM_FUNC(GUI_XBF__GetCharInfo) 
    SYMBOL_ITEM_FUNC(GUI_XBF__GetFontInfo) 
    SYMBOL_ITEM_FUNC(GUI_XBF__GetOff) 
    SYMBOL_ITEM_FUNC(GUI_XBF__GetOffAndSize) 
    SYMBOL_ITEM_FUNC(GUI_XBF__IsInFont) 
    SYMBOL_ITEM_FUNC(GUI_Warn) 
    SYMBOL_ITEM_FUNC(GUI_Warn1) 
    SYMBOL_ITEM_FUNC(GUI_Warn2) 
    SYMBOL_ITEM_FUNC(GUI_Warn3) 
    SYMBOL_ITEM_FUNC(GUI_Warn4) 
    SYMBOL_ITEM_FUNC(GUI_WaitKey) 
    SYMBOL_ITEM_FUNC(GUI_SetSignalEventFunc) 
    SYMBOL_ITEM_FUNC(GUI_SetWaitEventFunc) 
    SYMBOL_ITEM_FUNC(GUI_SetWaitEventTimedFunc) 
    SYMBOL_ITEM_FUNC(GUI_SignalEvent) 
    SYMBOL_ITEM_FUNC(GUI_WaitEvent) 
    SYMBOL_ITEM_FUNC(GUI_DispFloat) 
    SYMBOL_ITEM_FUNC(GUI_DispFloatFix) 
    SYMBOL_ITEM_FUNC(GUI_DispFloatMin) 
    SYMBOL_ITEM_FUNC(GUI_DispSFloatFix) 
    SYMBOL_ITEM_FUNC(GUI_DispSFloatMin) 
    SYMBOL_ITEM_FUNC(GUI_DispDec) 
    SYMBOL_ITEM_FUNC(GUI_DispDecAt) 
    SYMBOL_ITEM_FUNC(GUI_DispDecMin) 
    SYMBOL_ITEM_FUNC(GUI_DispDecShift) 
    SYMBOL_ITEM_FUNC(GUI_DispDecSpace) 
    SYMBOL_ITEM_FUNC(GUI_DispSDec) 
    SYMBOL_ITEM_FUNC(GUI_DispSDecShift) 
    SYMBOL_ITEM_FUNC(GUI_UC_SetEncodeUTF8) 
    SYMBOL_ITEM_FUNC(GUI_UC_SetEncodeNone) 
    SYMBOL_ITEM_FUNC(GUI_UC_EnableBIDI) 
    SYMBOL_ITEM_FUNC(GUI__BIDI_Log2VisBuffered) 
    SYMBOL_ITEM_FUNC(GUI_UC_DispString) 
    SYMBOL_ITEM_FUNC(GUI_UC_ConvertUTF82UC) 
    SYMBOL_ITEM_FUNC(GUI_UC_ConvertUC2UTF8) 
    SYMBOL_ITEM_FUNC(GUI_DB2UC) 
    SYMBOL_ITEM_FUNC(GUI_UC2DB) 
    SYMBOL_ITEM_FUNC(GUI_UC_Encode) 
    SYMBOL_ITEM_FUNC(GUI_UC_GetCharCode) 
    SYMBOL_ITEM_FUNC(GUI_UC_GetCharSize) 
    SYMBOL_ITEM_FUNC(GUI_UC__CalcSizeOfChar) 
    SYMBOL_ITEM_FUNC(GUI_UC__GetCharCodeInc) 
    SYMBOL_ITEM_FUNC(GUI_UC__NumBytes2NumChars) 
    SYMBOL_ITEM_FUNC(GUI_UC__NumChars2NumBytes) 
    SYMBOL_ITEM_FUNC(GUI_TOUCH_GetUnstable) 
    SYMBOL_ITEM_FUNC(GUI_TOUCH_StoreUnstable) 
    SYMBOL_ITEM_FUNC(GUI_TOUCH_SetLayer) 
    SYMBOL_ITEM_FUNC(GUI_TOUCH_StoreState) 
    SYMBOL_ITEM_FUNC(GUI_TOUCH_Calibrate) 
    SYMBOL_ITEM_FUNC(GUI_TOUCH_Exec) 
    SYMBOL_ITEM_FUNC(GUI_TOUCH_GetCalData) 
    SYMBOL_ITEM_FUNC(GUI_TOUCH_GetxPhys) 
    SYMBOL_ITEM_FUNC(GUI_TOUCH_GetyPhys) 
    SYMBOL_ITEM_FUNC(GUI_TOUCH_SetDefaultCalibration) 
    SYMBOL_ITEM_FUNC(GUI_TOUCH_SetOrientation) 
    SYMBOL_ITEM_FUNC(GUI_TOUCH_SetOrientationPhys) 
    SYMBOL_ITEM_FUNC(GUI_TOUCH_GetState) 
    SYMBOL_ITEM_FUNC(GUI_TOUCH_StoreStateEx) 
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
    SYMBOL_ITEM_FUNC(GUI_Delay) 
    SYMBOL_ITEM_FUNC(GUI_GetTime) 
    SYMBOL_ITEM_FUNC(GUITASK_CopyContext) 
    SYMBOL_ITEM_FUNC(GUITASK_GetpContext) 
    SYMBOL_ITEM_FUNC(GUITASK_Init) 
    SYMBOL_ITEM_FUNC(GUITASK_SetMaxTask) 
    SYMBOL_ITEM_FUNC(GUI_Lock) 
    SYMBOL_ITEM_FUNC(GUI_Unlock) 
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
    SYMBOL_ITEM_FUNC(GUI_DrawStreamedBitmap) 
    SYMBOL_ITEM_FUNC(GUI_DrawStreamedBitmap24Ex) 
    SYMBOL_ITEM_FUNC(GUI_DrawStreamedBitmap555Ex) 
    SYMBOL_ITEM_FUNC(GUI_DrawStreamedBitmap565Ex) 
    SYMBOL_ITEM_FUNC(GUI_DrawStreamedBitmapAuto) 
    SYMBOL_ITEM_FUNC(GUI_DrawStreamedBitmapEx) 
    SYMBOL_ITEM_FUNC(GUI_DrawStreamedBitmapExAuto) 
    SYMBOL_ITEM_FUNC(GUI_DrawStreamedBitmapM555Ex) 
    SYMBOL_ITEM_FUNC(GUI_DrawStreamedBitmapM565Ex) 
    SYMBOL_ITEM_FUNC(GUI_GetStreamedBitmapInfo) 
    SYMBOL_ITEM_FUNC(GUI_GetStreamedBitmapInfoEx) 
    SYMBOL_ITEM_FUNC(GUI_SetStreamedBitmapHook) 
    SYMBOL_ITEM_FUNC(GUI__CreateBitmapFromStream) 
    SYMBOL_ITEM_FUNC(GUI__ReadHeaderFromStream) 
    SYMBOL_ITEM_FUNC(GUI_SPRITE_CreateAnim) 
    SYMBOL_ITEM_FUNC(GUI_SPRITE_CreateExAnim) 
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
    SYMBOL_ITEM_FUNC(GUI_SIF__ClearLine_ExtFrm) 
    SYMBOL_ITEM_FUNC(GUI_SIF__GetCharDistX_ExtFrm) 
    SYMBOL_ITEM_FUNC(GUI_SIF__GetCharInfo_ExtFrm) 
    SYMBOL_ITEM_FUNC(GUI_SIF__GetFontInfo_ExtFrm) 
    SYMBOL_ITEM_FUNC(GUI_SIF__IsInFont_ExtFrm) 
    SYMBOL_ITEM_FUNC(GUI_SIF_CreateFont) 
    SYMBOL_ITEM_FUNC(GUI_SIF_DeleteFont) 
    SYMBOL_ITEM_FUNC(GUI_SIF__GetCharDistX) 
    SYMBOL_ITEM_FUNC(GUI_SIF__GetFontInfo) 
    SYMBOL_ITEM_FUNC(GUI_SIF__GetNumCharAreas) 
    SYMBOL_ITEM_FUNC(GUI_SIF__GetpCharInfo) 
    SYMBOL_ITEM_FUNC(GUI_SIF__IsInFont) 
    SYMBOL_ITEM_FUNC(GUI_SetTextStyle) 
    SYMBOL_ITEM_FUNC(GUI_SetTextMode) 
    SYMBOL_ITEM_FUNC(GUI_SetTextAlign) 
    SYMBOL_ITEM_FUNC(LCD_SetPixelIndex) 
    SYMBOL_ITEM_FUNC(GUI_SetOrientation) 
    SYMBOL_ITEM_FUNC(GUI_SetOrientationEx) 
    SYMBOL_ITEM_FUNC(GUI_SetOrientationExCached) 
    SYMBOL_ITEM_FUNC(GUI_SetOrg) 
    SYMBOL_ITEM_FUNC(GUI_SetLineStyle) 
    SYMBOL_ITEM_FUNC(GUI_SetLBorder) 
    SYMBOL_ITEM_FUNC(GUI_SetFont) 
    SYMBOL_ITEM_FUNC(GUI_SetDrawMode) 
    SYMBOL_ITEM_FUNC(GL_SetDefault) 
    SYMBOL_ITEM_FUNC(GUI_SetDefault) 
    SYMBOL_ITEM_FUNC(GUI_GetDecChar) 
    SYMBOL_ITEM_FUNC(GUI_SetDecChar) 
    SYMBOL_ITEM_FUNC(GUI_SetBkColorIndex) 
    SYMBOL_ITEM_FUNC(GUI_SetColorIndex) 
    SYMBOL_ITEM_FUNC(GUI_SetBkColor) 
    SYMBOL_ITEM_FUNC(GUI_SetColor) 
    SYMBOL_ITEM_FUNC(GUI_SetClipRect) 
    SYMBOL_ITEM_FUNC(GUI_SetAlpha) 
    SYMBOL_ITEM_FUNC(GUI__AlphaSuppressMixing) 
    SYMBOL_ITEM_FUNC(GUI_SelectLCD) 
    SYMBOL_ITEM_FUNC(GUI_AssignCursorLayer) 
    SYMBOL_ITEM_FUNC(GUI_GetCursorLayer) 
    SYMBOL_ITEM_FUNC(GUI_GetLayerPosEx) 
    SYMBOL_ITEM_FUNC(GUI_SelectLayer) 
    SYMBOL_ITEM_FUNC(GUI_SetLayerAlphaEx) 
    SYMBOL_ITEM_FUNC(GUI_SetLayerPosEx) 
    SYMBOL_ITEM_FUNC(GUI_SetLayerSizeEx) 
    SYMBOL_ITEM_FUNC(GUI_SetLayerVisEx) 
    SYMBOL_ITEM_FUNC(GUI_GetScreenSizeX) 
    SYMBOL_ITEM_FUNC(GUI_GetScreenSizeY) 
    SYMBOL_ITEM_FUNC(GUI_SetScreenSizeX) 
    SYMBOL_ITEM_FUNC(GUI_SetScreenSizeY) 
    SYMBOL_ITEM_FUNC(GUI_RestoreContext) 
    SYMBOL_ITEM_FUNC(GUI_SaveContext_W) 
    SYMBOL_ITEM_FUNC(GUI_RectsIntersect) 
    SYMBOL_ITEM_FUNC(GUI_ALLOC_Realloc) 
    SYMBOL_ITEM_FUNC(GUI_RotatePolygon) 
    SYMBOL_ITEM_FUNC(GUI_MagnifyPolygon) 
    SYMBOL_ITEM_FUNC(GUI_EnlargePolygon) 
    SYMBOL_ITEM_FUNC(GUI_PID_GetCurrentState) 
    SYMBOL_ITEM_FUNC(GUI_PID_GetState) 
    SYMBOL_ITEM_FUNC(GUI_PID_IsEmpty) 
    SYMBOL_ITEM_FUNC(GUI_PID_IsPressed) 
    SYMBOL_ITEM_FUNC(GUI_PID_StoreState) 
    SYMBOL_ITEM_FUNC(GUI_GetPenShape) 
    SYMBOL_ITEM_FUNC(GUI_GetPenSize) 
    SYMBOL_ITEM_FUNC(GUI_SetPenShape) 
    SYMBOL_ITEM_FUNC(GUI_SetPenSize) 
    SYMBOL_ITEM_FUNC(GUI_ClearKeyBuffer) 
    SYMBOL_ITEM_FUNC(GUI_GetKey) 
    SYMBOL_ITEM_FUNC(GUI_GetKeyState) 
    SYMBOL_ITEM_FUNC(GUI_PollKeyMsg) 
    SYMBOL_ITEM_FUNC(GUI_SendKeyMsg) 
    SYMBOL_ITEM_FUNC(GUI_StoreKey) 
    SYMBOL_ITEM_FUNC(GUI_StoreKeyMsg) 
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
    SYMBOL_ITEM_FUNC(GUI_MoveRect) 
    SYMBOL_ITEM_FUNC(GUI_MOUSE_DRIVER_PS2_Init) 
    SYMBOL_ITEM_FUNC(GUI_MOUSE_DRIVER_PS2_OnRx) 
    SYMBOL_ITEM_FUNC(GUI_MOUSE_GetState) 
    SYMBOL_ITEM_FUNC(GUI_MOUSE_StoreState) 
    SYMBOL_ITEM_FUNC(GUI_MergeRect) 
    SYMBOL_ITEM_FUNC(GUI_Log) 
    SYMBOL_ITEM_FUNC(GUI_Log1) 
    SYMBOL_ITEM_FUNC(GUI_Log2) 
    SYMBOL_ITEM_FUNC(GUI_Log3) 
    SYMBOL_ITEM_FUNC(GUI_Log4) 
    SYMBOL_ITEM_FUNC(GUI_LANG_GetNumItems) 
    SYMBOL_ITEM_FUNC(GUI_LANG_GetText) 
    SYMBOL_ITEM_FUNC(GUI_LANG_GetTextEx) 
    SYMBOL_ITEM_FUNC(GUI_LANG_LoadCSV) 
    SYMBOL_ITEM_FUNC(GUI_LANG_LoadCSVEx) 
    SYMBOL_ITEM_FUNC(GUI_LANG_LoadText) 
    SYMBOL_ITEM_FUNC(GUI_LANG_LoadTextEx) 
    SYMBOL_ITEM_FUNC(GUI_LANG_SetLang) 
    SYMBOL_ITEM_FUNC(GUI_LANG_SetMaxNumLang) 
    SYMBOL_ITEM_FUNC(GUI_LANG_SetSep) 
    SYMBOL_ITEM_FUNC(GUI_JPEG_DrawScaled) 
    SYMBOL_ITEM_FUNC(GUI_JPEG_DrawScaledEx) 
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
    SYMBOL_ITEM_FUNC(GUI_JOYSTICK_StoreState) 
    SYMBOL_ITEM_FUNC(GUI_IsInFont) 
    SYMBOL_ITEM_FUNC(GUI_InvertRect) 
    SYMBOL_ITEM_FUNC(GUI_InitLUT_C) 
    SYMBOL_ITEM_FUNC(GUI_Index2Color) 
    SYMBOL_ITEM_FUNC(GUI_GotoX) 
    SYMBOL_ITEM_FUNC(GUI_GotoXY) 
    SYMBOL_ITEM_FUNC(GUI_GotoY) 
    SYMBOL_ITEM_FUNC(GUI_GIF_DrawSubScaled) 
    SYMBOL_ITEM_FUNC(GUI_GIF_DrawSubScaledEx) 
    SYMBOL_ITEM_FUNC(GUI_GIF_Draw) 
    SYMBOL_ITEM_FUNC(GUI_GIF_DrawEx) 
    SYMBOL_ITEM_FUNC(GUI_GIF_DrawSub) 
    SYMBOL_ITEM_FUNC(GUI_GIF_DrawSubEx) 
    SYMBOL_ITEM_FUNC(GUI_GIF_GetComment) 
    SYMBOL_ITEM_FUNC(GUI_GIF_GetCommentEx) 
    SYMBOL_ITEM_FUNC(GUI_GIF_GetImageInfo) 
    SYMBOL_ITEM_FUNC(GUI_GIF_GetImageInfoEx) 
    SYMBOL_ITEM_FUNC(GUI_GIF_GetInfo) 
    SYMBOL_ITEM_FUNC(GUI_GIF_GetInfoEx) 
    SYMBOL_ITEM_FUNC(GUI_GIF_GetXSize) 
    SYMBOL_ITEM_FUNC(GUI_GIF_GetXSizeEx) 
    SYMBOL_ITEM_FUNC(GUI_GIF_GetYSize) 
    SYMBOL_ITEM_FUNC(GUI_GIF_GetYSizeEx) 
    SYMBOL_ITEM_FUNC(GUI_GIF_SetFillTrans) 
    SYMBOL_ITEM_FUNC(GUI_GIF__DrawFromFilePointer) 
    SYMBOL_ITEM_FUNC(GUI_GIF__GetData) 
    SYMBOL_ITEM_FUNC(GUI_GIF__GetNextByte) 
    SYMBOL_ITEM_FUNC(GUI_GIF__InitLZW) 
    SYMBOL_ITEM_FUNC(GUI_GIF__ReadData) 
    SYMBOL_ITEM_FUNC(GUI_GetYDistOfFont) 
    SYMBOL_ITEM_FUNC(GUI_GetYSizeOfFont) 
    SYMBOL_ITEM_FUNC(GUI_GetVersionString) 
    SYMBOL_ITEM_FUNC(GUI_GetTextMode) 
    SYMBOL_ITEM_FUNC(GUI_GetTextExtend) 
    SYMBOL_ITEM_FUNC(GUI_GetTextAlign) 
    SYMBOL_ITEM_FUNC(GUI_GetStringDistX) 
    SYMBOL_ITEM_FUNC(GUI_GetOrg) 
    SYMBOL_ITEM_FUNC(GUI__GetOrg) 
    SYMBOL_ITEM_FUNC(GUI_GetLineStyle) 
    SYMBOL_ITEM_FUNC(GUI_GetFontSizeY) 
    SYMBOL_ITEM_FUNC(GUI_GetFontInfo) 
    SYMBOL_ITEM_FUNC(GUI_GetFont) 
    SYMBOL_ITEM_FUNC(GUI_GetDrawMode) 
    SYMBOL_ITEM_FUNC(GUI_GetDispPosX) 
    SYMBOL_ITEM_FUNC(GUI_GetDispPosY) 
    SYMBOL_ITEM_FUNC(GUI_GetBkColor) 
    SYMBOL_ITEM_FUNC(GUI_GetBkColorIndex) 
    SYMBOL_ITEM_FUNC(GUI_GetColor) 
    SYMBOL_ITEM_FUNC(GUI_GetColorIndex) 
    SYMBOL_ITEM_FUNC(GUI_GetClientRect) 
    SYMBOL_ITEM_FUNC(GUI_GetLeadingBlankCols) 
    SYMBOL_ITEM_FUNC(GUI_GetTrailingBlankCols) 
    SYMBOL_ITEM_FUNC(GUI_GetBitmapPixelIndex) 
    SYMBOL_ITEM_FUNC(GUI_GetBitmapPixelIndexEx) 
    SYMBOL_ITEM_FUNC(GUI_GetBitmapPixelColor) 
    SYMBOL_ITEM_FUNC(GUI_FillRoundedFrame) 
    SYMBOL_ITEM_FUNC(GUI_FillRoundedRect) 
    SYMBOL_ITEM_FUNC(GUI_FillRectEx) 
    SYMBOL_ITEM_FUNC(GUI_FillRect) 
    SYMBOL_ITEM_FUNC(GL_FillPolygon) 
    SYMBOL_ITEM_FUNC(GUI_FillPolygon) 
    SYMBOL_ITEM_FUNC(GUI_Exec) 
    SYMBOL_ITEM_FUNC(GUI_Exec1) 
    SYMBOL_ITEM_FUNC(GUI_ErrorOut) 
    SYMBOL_ITEM_FUNC(GUI_ErrorOut1) 
    SYMBOL_ITEM_FUNC(GUI_ErrorOut2) 
    SYMBOL_ITEM_FUNC(GUI_ErrorOut3) 
    SYMBOL_ITEM_FUNC(GUI_ErrorOut4) 
    SYMBOL_ITEM_FUNC(GUI_Error) 
    SYMBOL_ITEM_FUNC(GUIPROP_EXT__ClearLine) 
    SYMBOL_ITEM_FUNC(GUIPROP_EXT__DispLine) 
    SYMBOL_ITEM_FUNC(GUIPROP_EXT__SetfpClearLine) 
    SYMBOL_ITEM_FUNC(GUI_EnableAlpha) 
    SYMBOL_ITEM_FUNC(GUI_RestoreUserAlpha) 
    SYMBOL_ITEM_FUNC(GUI_SetUserAlpha) 
    SYMBOL_ITEM_FUNC(GUI_DrawVLine) 
    SYMBOL_ITEM_FUNC(GUI_DrawRoundedRect) 
    SYMBOL_ITEM_FUNC(GUI_DrawRoundedFrame) 
    SYMBOL_ITEM_FUNC(GUI_DrawRectEx) 
    SYMBOL_ITEM_FUNC(GUI_DrawPolyLine) 
    SYMBOL_ITEM_FUNC(GL_DrawPolygon) 
    SYMBOL_ITEM_FUNC(GUI_DrawPolygon) 
    SYMBOL_ITEM_FUNC(GL_DrawPoint) 
    SYMBOL_ITEM_FUNC(GUI_DrawPoint) 
    SYMBOL_ITEM_FUNC(GUI_DrawPixel) 
    SYMBOL_ITEM_FUNC(GUI_DrawPie) 
    SYMBOL_ITEM_FUNC(GL_DrawLine) 
    SYMBOL_ITEM_FUNC(GL_DrawLine1) 
    SYMBOL_ITEM_FUNC(GL_DrawLine1Ex) 
    SYMBOL_ITEM_FUNC(GL_DrawLineEx) 
    SYMBOL_ITEM_FUNC(GL_DrawLineTo) 
    SYMBOL_ITEM_FUNC(GL_DrawLineToEx) 
    SYMBOL_ITEM_FUNC(GUI_DrawLine) 
    SYMBOL_ITEM_FUNC(GUI_DrawLineRel) 
    SYMBOL_ITEM_FUNC(GUI_DrawLineTo) 
    SYMBOL_ITEM_FUNC(GUI_cos) 
    SYMBOL_ITEM_FUNC(GUI_sin) 
    SYMBOL_ITEM_FUNC(GUI_DrawHLine) 
    SYMBOL_ITEM_FUNC(GUI_DrawGraphEx) 
    SYMBOL_ITEM_FUNC(GUI_DrawGraph) 
    SYMBOL_ITEM_FUNC(GUI_DrawGradientRoundedH) 
    SYMBOL_ITEM_FUNC(GUI_DrawGradientRoundedV) 
    SYMBOL_ITEM_FUNC(GUI_DrawGradientH) 
    SYMBOL_ITEM_FUNC(GUI_DrawGradientV) 
    SYMBOL_ITEM_FUNC(GUI_DrawFocusRect) 
    SYMBOL_ITEM_FUNC(GUI_DrawBitmapMag) 
    SYMBOL_ITEM_FUNC(GUI_DrawBitmapHWAlpha) 
    SYMBOL_ITEM_FUNC(GUI_DrawBitmapExp) 
    SYMBOL_ITEM_FUNC(GUI_DrawBitmapEx) 
    SYMBOL_ITEM_FUNC(GL_DrawBitmap) 
    SYMBOL_ITEM_FUNC(GUI_DrawBitmap) 
    SYMBOL_ITEM_FUNC(GUI_DispStringLen) 
    SYMBOL_ITEM_FUNC(GUI_DispStringInRectWrap) 
    SYMBOL_ITEM_FUNC(GUI_WrapGetNumLines) 
    SYMBOL_ITEM_FUNC(GUI_DispStringInRectEx) 
    SYMBOL_ITEM_FUNC(GUI_DispStringInRect) 
    SYMBOL_ITEM_FUNC(GUI_DispStringInRectMax) 
    SYMBOL_ITEM_FUNC(GUI__DispStringInRect) 
    SYMBOL_ITEM_FUNC(GUI_DispStringHCenterAt) 
    SYMBOL_ITEM_FUNC(GUI_DispStringAtCEOL) 
    SYMBOL_ITEM_FUNC(GUI_DispStringAt) 
    SYMBOL_ITEM_FUNC(GUI_DispString) 
    SYMBOL_ITEM_FUNC(GUI_DispHex) 
    SYMBOL_ITEM_FUNC(GUI_DispHexAt) 
    SYMBOL_ITEM_FUNC(GUI_DispChars) 
    SYMBOL_ITEM_FUNC(GUI_DispChar) 
    SYMBOL_ITEM_FUNC(GUI_DispCharAt) 
    SYMBOL_ITEM_FUNC(GUI_DispCEOL) 
    SYMBOL_ITEM_FUNC(GUI_DispBin) 
    SYMBOL_ITEM_FUNC(GUI_DispBinAt) 
    SYMBOL_ITEM_FUNC(GUI_DIRTYDEVICE_Create) 
    SYMBOL_ITEM_FUNC(GUI_DIRTYDEVICE_Delete) 
    SYMBOL_ITEM_FUNC(GUI_DIRTYDEVICE_Fetch) 
    SYMBOL_ITEM_FUNC(GUI_DEVICE_Create) 
    SYMBOL_ITEM_FUNC(GUI_DEVICE_CreateAndLink) 
    SYMBOL_ITEM_FUNC(GUI_DEVICE_Delete) 
    SYMBOL_ITEM_FUNC(GUI_DEVICE_Link) 
    SYMBOL_ITEM_FUNC(GUI_DEVICE_LinkDevices) 
    SYMBOL_ITEM_FUNC(GUI_DEVICE_Unlink) 
    SYMBOL_ITEM_FUNC(GUI_DEVICE_UnlinkTaskDevices) 
    SYMBOL_ITEM_FUNC(GUI_DEVICE__GetpDevice) 
    SYMBOL_ITEM_FUNC(GUI_DEVICE__GetpDriver) 
    SYMBOL_ITEM_FUNC(GUI_CURSOR_SelectAnim) 
    SYMBOL_ITEM_FUNC(GUI_CURSOR_SelectAnimEx) 
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
    SYMBOL_ITEM_FUNC(GUI_CURSOR__GetSpriteEx) 
    SYMBOL_ITEM_FUNC(GUI_CURSOR__SetSpriteEx) 
    SYMBOL_ITEM_FUNC(GUI_Clear) 
    SYMBOL_ITEM_FUNC(GUI_ClearRect) 
    SYMBOL_ITEM_FUNC(GUI_Exit) 
    SYMBOL_ITEM_FUNC(GUI_Init) 
    SYMBOL_ITEM_FUNC(GUI_SetDefaultFont) 
    SYMBOL_ITEM_FUNC(GUI__Config) 
    SYMBOL_ITEM_FUNC(GUI__RegisterExit) 
    SYMBOL_ITEM_FUNC(GUI_CopyRect) 
    SYMBOL_ITEM_FUNC(GUI_CalcVisColorError) 
    SYMBOL_ITEM_FUNC(GUI_Color2VisColor) 
    SYMBOL_ITEM_FUNC(GUI_ColorIsAvailable) 
    SYMBOL_ITEM_FUNC(GUI_Color2Index) 
    SYMBOL_ITEM_FUNC(GUI_ClearRectEx) 
    SYMBOL_ITEM_FUNC(GL_DrawCircle) 
    SYMBOL_ITEM_FUNC(GL_DrawEllipse) 
    SYMBOL_ITEM_FUNC(GL_FillCircle) 
    SYMBOL_ITEM_FUNC(GL_FillEllipse) 
    SYMBOL_ITEM_FUNC(GUI_DrawCircle) 
    SYMBOL_ITEM_FUNC(GUI_DrawEllipse) 
    SYMBOL_ITEM_FUNC(GUI_FillCircle) 
    SYMBOL_ITEM_FUNC(GUI_FillEllipse) 
    SYMBOL_ITEM_FUNC(GUIPROP_FRM_DispChar) 
    SYMBOL_ITEM_FUNC(GUIPROP_FRM_GetCharDistX) 
    SYMBOL_ITEM_FUNC(GUIPROP_FRM_GetFontInfo) 
    SYMBOL_ITEM_FUNC(GUIPROP_FRM_IsInFont) 
    SYMBOL_ITEM_FUNC(GUIPROP_EXT_DispChar) 
    SYMBOL_ITEM_FUNC(GUIPROP_EXT_GetCharDistX) 
    SYMBOL_ITEM_FUNC(GUIPROP_EXT_GetCharInfo) 
    SYMBOL_ITEM_FUNC(GUIPROP_EXT_GetFontInfo) 
    SYMBOL_ITEM_FUNC(GUIPROP_EXT_IsInFont) 
    SYMBOL_ITEM_FUNC(GUIPROP_EXT__FindChar) 
    SYMBOL_ITEM_FUNC(GUIPROP_DispChar) 
    SYMBOL_ITEM_FUNC(GUIPROP_GetCharDistX) 
    SYMBOL_ITEM_FUNC(GUIPROP_GetFontInfo) 
    SYMBOL_ITEM_FUNC(GUIPROP_IsInFont) 
    SYMBOL_ITEM_FUNC(GUIPROP__FindChar) 
    SYMBOL_ITEM_FUNC(GUIMONO_DispChar) 
    SYMBOL_ITEM_FUNC(GUIMONO_GetCharDistX) 
    SYMBOL_ITEM_FUNC(GUIMONO_GetFontInfo) 
    SYMBOL_ITEM_FUNC(GUIMONO_IsInFont) 
    SYMBOL_ITEM_FUNC(GUI__DispLine) 
    SYMBOL_ITEM_FUNC(GUI__GetLineDistX) 
    SYMBOL_ITEM_FUNC(GUI__GetLineNumChars) 
    SYMBOL_ITEM_FUNC(GUI__GetOverlap) 
    SYMBOL_ITEM_FUNC(GL_DispChar) 
    SYMBOL_ITEM_FUNC(GUI_DispNextLine) 
    SYMBOL_ITEM_FUNC(GUI_GetCharDistX) 
    SYMBOL_ITEM_FUNC(GUI_GetCharDistXEx) 
    SYMBOL_ITEM_FUNC(GUI_GetFontDistY) 
    SYMBOL_ITEM_FUNC(GUI_GetYAdjust) 
    SYMBOL_ITEM_FUNC(GUI_CalcColorDist) 
    SYMBOL_ITEM_FUNC(GUI_BMP_Serialize) 
    SYMBOL_ITEM_FUNC(GUI_BMP_SerializeEx) 
    SYMBOL_ITEM_FUNC(GUI_BMP_SerializeExBpp) 
    SYMBOL_ITEM_FUNC(GUI_BMP_EnableAlpha) 
    SYMBOL_ITEM_FUNC(GUI_BMP_DrawScaled) 
    SYMBOL_ITEM_FUNC(GUI_BMP_DrawScaledEx) 
    SYMBOL_ITEM_FUNC(GUI_BMP_DisableAlpha) 
    SYMBOL_ITEM_FUNC(GUI_BMP_Draw) 
    SYMBOL_ITEM_FUNC(GUI_BMP_DrawEx) 
    SYMBOL_ITEM_FUNC(GUI_BMP_GetXSize) 
    SYMBOL_ITEM_FUNC(GUI_BMP_GetXSizeEx) 
    SYMBOL_ITEM_FUNC(GUI_BMP_GetYSize) 
    SYMBOL_ITEM_FUNC(GUI_BMP_GetYSizeEx) 
    SYMBOL_ITEM_FUNC(GUI_BMP__GetData) 
    SYMBOL_ITEM_FUNC(GUI_BMP__Init) 
    SYMBOL_ITEM_FUNC(GUI_BMP__ReadData) 
    SYMBOL_ITEM_FUNC(GUI_BMP__ReadPalette) 
    SYMBOL_ITEM_FUNC(GL_DrawArc) 
    SYMBOL_ITEM_FUNC(GUI_DrawArc) 
    SYMBOL_ITEM_FUNC(GUI_ALLOC_AllocZero) 
    SYMBOL_ITEM_FUNC(GUI_ALLOC_AllocInit) 
    SYMBOL_ITEM_FUNC(GUI_ALLOC_AllocNoInit) 
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
    SYMBOL_ITEM_FUNC(GUI_ALLOC_RequestSize) 
    SYMBOL_ITEM_FUNC(GUI_ALLOC_SetAvBlockSize) 
    SYMBOL_ITEM_FUNC(GUI_ALLOC_SetMaxPercentage) 
    SYMBOL_ITEM_FUNC(GUI_ALLOC_Unlock) 
    SYMBOL_ITEM_FUNC(GUI_ALLOC_UnlockH) 
    SYMBOL_ITEM_FUNC(GUI_ALLOC_h2p) 
    SYMBOL_ITEM_FUNC(GUI_ALLOC_p2h) 
    SYMBOL_ITEM_FUNC(GUI_AddHex) 
    SYMBOL_ITEM_FUNC(GUI_AddDecShift) 
    SYMBOL_ITEM_FUNC(GUI_AddSign) 
    SYMBOL_ITEM_FUNC(GUI_Long2Len) 
    SYMBOL_ITEM_FUNC(GUI_AddDecMin) 
    SYMBOL_ITEM_FUNC(GUI_AddDec) 
    SYMBOL_ITEM_FUNC(GUI_AddBin) 
    SYMBOL_ITEM_FUNC(GL_MoveTo) 
    SYMBOL_ITEM_FUNC(GUI_DrawRect) 
    SYMBOL_ITEM_FUNC(GUI_MoveRel) 
    SYMBOL_ITEM_FUNC(GUI_MoveTo) 
    SYMBOL_ITEM_FUNC(LCD_Index2Color_M888) 
    SYMBOL_ITEM_FUNC(LCD_Index2Color_M565) 
    SYMBOL_ITEM_FUNC(LCD_Index2Color_M555) 
    SYMBOL_ITEM_FUNC(LCD_Index2Color_888) 
    SYMBOL_ITEM_FUNC(LCD_Index2Color_8888) 
    SYMBOL_ITEM_FUNC(LCD_Index2Color_565) 
    SYMBOL_ITEM_FUNC(LCD_Index2Color_555) 
    SYMBOL_ITEM_FUNC(LCD_SetLUT) 
    SYMBOL_ITEM_FUNC(LCD_SetLUTEx) 
    SYMBOL_ITEM_FUNC(LCD_Color2Index_8666) 
    SYMBOL_ITEM_FUNC(LCD_Index2Color_8666) 
    SYMBOL_ITEM_FUNC(GUI_AA_DrawPolyOutline) 
    SYMBOL_ITEM_FUNC(GUI_AA_DrawPolyOutlineEx) 
    SYMBOL_ITEM_FUNC(GUI_AA_FillPolygon) 
    SYMBOL_ITEM_FUNC(GUI_AA_DrawLine) 
    SYMBOL_ITEM_FUNC(GUI_AA_DisableHiRes) 
    SYMBOL_ITEM_FUNC(GUI_AA_EnableHiRes) 
    SYMBOL_ITEM_FUNC(GUI_AA_Exit) 
    SYMBOL_ITEM_FUNC(GUI_AA_GetFactor) 
    SYMBOL_ITEM_FUNC(GUI_AA_HiRes2Pixel) 
    SYMBOL_ITEM_FUNC(GUI_AA_Init) 
    SYMBOL_ITEM_FUNC(GUI_AA_Init_HiRes) 
    SYMBOL_ITEM_FUNC(GUI_AA_SetDrawMode) 
    SYMBOL_ITEM_FUNC(GUI_AA_SetFactor) 
    SYMBOL_ITEM_FUNC(GUI_AA_FillRoundedRect) 
    SYMBOL_ITEM_FUNC(GUI_AA_DrawRoundedRect) 
    SYMBOL_ITEM_FUNC(GL_FillCircleAA_HiRes) 
    SYMBOL_ITEM_FUNC(GUI_AA_FillCircle) 
    SYMBOL_ITEM_FUNC(GUIPROP_AA4_EXT_DispChar) 
    SYMBOL_ITEM_FUNC(GUIPROP_AA4_DispChar) 
    SYMBOL_ITEM_FUNC(GUIPROP_AA4_GetCharDistX) 
    SYMBOL_ITEM_FUNC(GUIPROP_AA4_GetFontInfo) 
    SYMBOL_ITEM_FUNC(GUIPROP_AA4_IsInFont) 
    SYMBOL_ITEM_FUNC(GUI_AA__DrawCharAA4) 
    SYMBOL_ITEM_FUNC(GUIPROP_AA2_EXT_DispChar) 
    SYMBOL_ITEM_FUNC(GUIPROP_AA2_DispChar) 
    SYMBOL_ITEM_FUNC(GUIPROP_AA2_GetCharDistX) 
    SYMBOL_ITEM_FUNC(GUIPROP_AA2_GetFontInfo) 
    SYMBOL_ITEM_FUNC(GUIPROP_AA2_IsInFont) 
    SYMBOL_ITEM_FUNC(GUI_AA__DrawCharAA2) 
    SYMBOL_ITEM_FUNC(GUIPROPAA_DispChar) 
    SYMBOL_ITEM_FUNC(GUIPROPAA_GetCharDistX) 
    SYMBOL_ITEM_FUNC(GUIPROPAA_GetFontInfo) 
    SYMBOL_ITEM_FUNC(GUIPROPAA_IsInFont) 
    SYMBOL_ITEM_FUNC(GUI_AA_DrawArc) 
    SYMBOL_ITEM_OBJ(WINDOW__DefaultBkColor) 
    SYMBOL_ITEM_OBJ(_pEffectDefault) 
    SYMBOL_ITEM_OBJ(TREEVIEW__DefaultProps) 
    SYMBOL_ITEM_OBJ(TEXT__DefaultProps) 
    SYMBOL_ITEM_OBJ(SPINBOX__Skin) 
    SYMBOL_ITEM_OBJ(SPINBOX__DefaultProps) 
    SYMBOL_ITEM_OBJ(SPINBOX__pSkinDefault) 
    SYMBOL_ITEM_OBJ(SLIDER__Skin) 
    SYMBOL_ITEM_OBJ(SLIDER__DefaultProps) 
    SYMBOL_ITEM_OBJ(SLIDER__pSkinDefault) 
    SYMBOL_ITEM_OBJ(SCROLLBAR__Skin) 
    SYMBOL_ITEM_OBJ(SCROLLBAR__DefaultProps) 
    SYMBOL_ITEM_OBJ(SCROLLBAR__DefaultWidth) 
    SYMBOL_ITEM_OBJ(SCROLLBAR__ThumbSizeMin) 
    SYMBOL_ITEM_OBJ(SCROLLBAR__pSkinDefault) 
    SYMBOL_ITEM_OBJ(RADIO__Skin) 
    SYMBOL_ITEM_OBJ(RADIO__DefaultProps) 
    SYMBOL_ITEM_OBJ(RADIO__pSkinDefault) 
    SYMBOL_ITEM_OBJ(PROGBAR__Skin) 
    SYMBOL_ITEM_OBJ(PROGBAR__DefaultProps) 
    SYMBOL_ITEM_OBJ(PROGBAR__pSkinDefault) 
    SYMBOL_ITEM_OBJ(MULTIPAGE__DefaultProps) 
    SYMBOL_ITEM_OBJ(MULTIPAGE__aEffectColor) 
    SYMBOL_ITEM_OBJ(MENU__DefaultProps) 
    SYMBOL_ITEM_OBJ(MENU__pDefaultEffect) 
    SYMBOL_ITEM_OBJ(LISTWHEEL_DefaultProps) 
    SYMBOL_ITEM_OBJ(LISTVIEW_DefaultProps) 
    SYMBOL_ITEM_OBJ(LISTBOX_DefaultProps) 
    SYMBOL_ITEM_OBJ(ICONVIEW__DefaultProps) 
    SYMBOL_ITEM_OBJ(ICONVIEW__pfDrawStreamedBitmap) 
    SYMBOL_ITEM_OBJ(HEADER__Skin) 
    SYMBOL_ITEM_OBJ(HEADER__DefaultBorderH) 
    SYMBOL_ITEM_OBJ(HEADER__DefaultProps) 
    SYMBOL_ITEM_OBJ(HEADER__pDefaultCursor) 
    SYMBOL_ITEM_OBJ(HEADER__pSkinDefault) 
    SYMBOL_ITEM_OBJ(_Props) 
    SYMBOL_ITEM_OBJ(GRAPH__DefaultProps) 
    SYMBOL_ITEM_OBJ(FRAMEWIN__Skin) 
    SYMBOL_ITEM_OBJ(FRAMEWIN__DefaultProps) 
    SYMBOL_ITEM_OBJ(FRAMEWIN__pSkinDefault) 
    SYMBOL_ITEM_OBJ(EDIT__DefaultProps) 
    SYMBOL_ITEM_OBJ(DROPDOWN__Skin) 
    SYMBOL_ITEM_OBJ(DROPDOWN__DefaultProps) 
    SYMBOL_ITEM_OBJ(DROPDOWN__pSkinDefault) 
    SYMBOL_ITEM_OBJ(CHECKBOX__Skin) 
    SYMBOL_ITEM_OBJ(CHECKBOX__DefaultProps) 
    SYMBOL_ITEM_OBJ(CHECKBOX__pSkinDefault) 
    SYMBOL_ITEM_OBJ(BUTTON__Skin) 
    SYMBOL_ITEM_OBJ(BUTTON__DefaultProps) 
    SYMBOL_ITEM_OBJ(BUTTON__pSkinDefault) 
    SYMBOL_ITEM_OBJ(LCD_APIListCW) 
    SYMBOL_ITEM_OBJ(LCD_APIListCCW) 
    SYMBOL_ITEM_OBJ(LCD_APIList180) 
    SYMBOL_ITEM_OBJ(LCD__pSetColorAPI) 
    SYMBOL_ITEM_OBJ(GUI__Wrap_pfGetCharWrap) 
    SYMBOL_ITEM_OBJ(GUI__Wrap_pfGetWordWrap) 
    SYMBOL_ITEM_OBJ(GUI__GetCursorPos_pfGetCharacter) 
    SYMBOL_ITEM_OBJ(GUI__GetCursorPos_pfGetPosChar) 
    SYMBOL_ITEM_OBJ(GUI__GetCursorPos_pfGetPosX) 
    SYMBOL_ITEM_OBJ(GUI__pFontDefault) 
    SYMBOL_ITEM_OBJ(GUI_pContext) 
    SYMBOL_ITEM_OBJ(GUI_CharLine_pfLog2Vis) 
    SYMBOL_ITEM_OBJ(GUI__pfDrawLine_RGB) 
    SYMBOL_ITEM_OBJ(IMAGE__DefaultProps) 
    SYMBOL_ITEM_OBJ(HEADER__DefaultBorderV) 
    SYMBOL_ITEM_OBJ(GUITASK__EntranceCnt) 
    SYMBOL_ITEM_OBJ(WIDGET_Effect_Simple) 
    SYMBOL_ITEM_OBJ(WIDGET_Effect_None) 
    SYMBOL_ITEM_OBJ(WIDGET_Effect_3D2L) 
    SYMBOL_ITEM_OBJ(WIDGET_Effect_3D1L) 
    SYMBOL_ITEM_OBJ(WIDGET_Effect_3D) 
    SYMBOL_ITEM_OBJ(TREEVIEW__bmClosed) 
    SYMBOL_ITEM_OBJ(TREEVIEW__bmLeaf) 
    SYMBOL_ITEM_OBJ(TREEVIEW__bmMinus) 
    SYMBOL_ITEM_OBJ(TREEVIEW__bmOpen) 
    SYMBOL_ITEM_OBJ(TREEVIEW__bmPlus) 
    SYMBOL_ITEM_OBJ(SPINBOX__SkinClassic) 
    SYMBOL_ITEM_OBJ(SLIDER__SkinClassic) 
    SYMBOL_ITEM_OBJ(SCROLLBAR__SkinClassic) 
    SYMBOL_ITEM_OBJ(RADIO__SkinClassic) 
    SYMBOL_ITEM_OBJ(RADIO__abmRadio) 
    SYMBOL_ITEM_OBJ(RADIO__bmCheck) 
    SYMBOL_ITEM_OBJ(PROGBAR__SkinClassic) 
    SYMBOL_ITEM_OBJ(HEADER__SkinClassic) 
    SYMBOL_ITEM_OBJ(FRAMEWIN__SkinClassic) 
    SYMBOL_ITEM_OBJ(DROPDOWN__SkinClassic) 
    SYMBOL_ITEM_OBJ(CHECKBOX__SkinClassic) 
    SYMBOL_ITEM_OBJ(CHECKBOX__abmCheck) 
    SYMBOL_ITEM_OBJ(BUTTON__SkinClassic) 
    SYMBOL_ITEM_OBJ(_VNC_API) 
    SYMBOL_ITEM_OBJ(GUI_MEMDEV_DEVICE_8) 
    SYMBOL_ITEM_OBJ(GUI_MEMDEV_DEVICE_32) 
    SYMBOL_ITEM_OBJ(GUI_MEMDEV_DEVICE_16) 
    SYMBOL_ITEM_OBJ(GUI_MEMDEV_DEVICE_1) 
    SYMBOL_ITEM_OBJ(GUI_FontD80) 
    SYMBOL_ITEM_OBJ(GUI_FontD64) 
    SYMBOL_ITEM_OBJ(GUI_FontD60x80) 
    SYMBOL_ITEM_OBJ(GUI_FontD48x64) 
    SYMBOL_ITEM_OBJ(GUI_FontD48) 
    SYMBOL_ITEM_OBJ(GUI_FontD36x48) 
    SYMBOL_ITEM_OBJ(GUI_FontD32) 
    SYMBOL_ITEM_OBJ(GUI_FontD24x32) 
    SYMBOL_ITEM_OBJ(CharInfo24B_ASCII) 
    SYMBOL_ITEM_OBJ(FontComic24BProp_ASCII) 
    SYMBOL_ITEM_OBJ(GUI_FontComic24B_ASCII) 
    SYMBOL_ITEM_OBJ(GUI_FontComic24B_1) 
    SYMBOL_ITEM_OBJ(CharInfo18B_ASCII) 
    SYMBOL_ITEM_OBJ(GUI_FontComic18B_ASCII) 
    SYMBOL_ITEM_OBJ(GUI_FontComic18B_1) 
    SYMBOL_ITEM_OBJ(GUI_Font8x8ASCII_Prop0) 
    SYMBOL_ITEM_OBJ(GUI_Font8x8_ASCII) 
    SYMBOL_ITEM_OBJ(GUI_Font8x8_1) 
    SYMBOL_ITEM_OBJ(GUI_Font8x8) 
    SYMBOL_ITEM_OBJ(GUI_Font8x9) 
    SYMBOL_ITEM_OBJ(GUI_Font8x16ASCII_Prop0) 
    SYMBOL_ITEM_OBJ(GUI_Font8x16_ASCII) 
    SYMBOL_ITEM_OBJ(GUI_Font8x16_1) 
    SYMBOL_ITEM_OBJ(GUI_Font8x16) 
    SYMBOL_ITEM_OBJ(GUI_Font8x16x1x2) 
    SYMBOL_ITEM_OBJ(GUI_Font8x16x2x2) 
    SYMBOL_ITEM_OBJ(GUI_Font8x16x3x3) 
    SYMBOL_ITEM_OBJ(GUI_Font8x17) 
    SYMBOL_ITEM_OBJ(GUI_Font8x18) 
    SYMBOL_ITEM_OBJ(GUI_CharInfoF8x15B_ASCII) 
    SYMBOL_ITEM_OBJ(GUI_Font8x15B_ASCII) 
    SYMBOL_ITEM_OBJ(GUI_Font8x15B_1) 
    SYMBOL_ITEM_OBJ(GUI_CharInfoF8x13_ASCII) 
    SYMBOL_ITEM_OBJ(GUI_Font8x13_ASCII) 
    SYMBOL_ITEM_OBJ(GUI_Font8x13_1) 
    SYMBOL_ITEM_OBJ(GUI_Font8x12_ASCII) 
    SYMBOL_ITEM_OBJ(GUI_Font8x10_ASCII) 
    SYMBOL_ITEM_OBJ(GUI_Font6x8ASCII_Prop0) 
    SYMBOL_ITEM_OBJ(GUI_Font6x8_ASCII) 
    SYMBOL_ITEM_OBJ(GUI_CharInfoF6x8_1) 
    SYMBOL_ITEM_OBJ(GUI_Font6x8_1) 
    SYMBOL_ITEM_OBJ(GUI_Font6x8) 
    SYMBOL_ITEM_OBJ(GUI_Font6x9) 
    SYMBOL_ITEM_OBJ(GUI_Font4x6) 
    SYMBOL_ITEM_OBJ(GUI_Font32_ASCII) 
    SYMBOL_ITEM_OBJ(GUI_Font32_ASCII_CharInfo) 
    SYMBOL_ITEM_OBJ(GUI_Font32_1) 
    SYMBOL_ITEM_OBJ(GUI_Font32B_ASCII) 
    SYMBOL_ITEM_OBJ(GUI_Font32B_ASCII_CharInfo) 
    SYMBOL_ITEM_OBJ(GUI_Font32B_1) 
    SYMBOL_ITEM_OBJ(GUI_Font24_ASCII) 
    SYMBOL_ITEM_OBJ(GUI_Font24_ASCII_CharInfo) 
    SYMBOL_ITEM_OBJ(GUI_Font24_1) 
    SYMBOL_ITEM_OBJ(GUI_Font24B_ASCII) 
    SYMBOL_ITEM_OBJ(GUI_Font24B_ASCII_CharInfo) 
    SYMBOL_ITEM_OBJ(GUI_Font24B_1) 
    SYMBOL_ITEM_OBJ(GUI_Font20_ASCII) 
    SYMBOL_ITEM_OBJ(GUI_Font20_ASCII_CharInfo) 
    SYMBOL_ITEM_OBJ(GUI_Font20_ASCII_Prop1) 
    SYMBOL_ITEM_OBJ(GUI_Font20_1) 
    SYMBOL_ITEM_OBJ(GUI_Font20F_ASCII) 
    SYMBOL_ITEM_OBJ(GUI_Font20B_ASCII) 
    SYMBOL_ITEM_OBJ(GUI_Font20B_ASCII_CharInfo) 
    SYMBOL_ITEM_OBJ(GUI_Font20B_ASCII_Prop1) 
    SYMBOL_ITEM_OBJ(GUI_Font20B_1) 
    SYMBOL_ITEM_OBJ(GUI_Font16_HK) 
    SYMBOL_ITEM_OBJ(GUI_Font16_HK_CharInfo) 
    SYMBOL_ITEM_OBJ(GUI_Font16ASCIIProp1) 
    SYMBOL_ITEM_OBJ(GUI_Font16_ASCII) 
    SYMBOL_ITEM_OBJ(GUI_Font16_1HK) 
    SYMBOL_ITEM_OBJ(GUI_Font16_1) 
    SYMBOL_ITEM_OBJ(GUI_Font16_1_FontProp1) 
    SYMBOL_ITEM_OBJ(GUI_Font16B_ASCII) 
    SYMBOL_ITEM_OBJ(GUI_Font16B_ASCII_CharInfo) 
    SYMBOL_ITEM_OBJ(GUI_Font16B_1) 
    SYMBOL_ITEM_OBJ(GUI_Font13ASCII_Prop1) 
    SYMBOL_ITEM_OBJ(GUI_Font13_ASCII) 
    SYMBOL_ITEM_OBJ(GUI_Font13_1) 
    SYMBOL_ITEM_OBJ(GUI_Font13H_ASCII) 
    SYMBOL_ITEM_OBJ(GUI_Font13H_ASCII_CharInfo) 
    SYMBOL_ITEM_OBJ(GUI_Font13H_1) 
    SYMBOL_ITEM_OBJ(GUI_Font13HB_ASCII) 
    SYMBOL_ITEM_OBJ(GUI_Font13HB_ASCII_CharInfo) 
    SYMBOL_ITEM_OBJ(GUI_Font13HB_1) 
    SYMBOL_ITEM_OBJ(GUI_Font13B_ASCII) 
    SYMBOL_ITEM_OBJ(GUI_Font13B_ASCII_Prop1) 
    SYMBOL_ITEM_OBJ(GUI_Font13B_1) 
    SYMBOL_ITEM_OBJ(GUI_Font10ASCIIProp1) 
    SYMBOL_ITEM_OBJ(GUI_Font10_ASCII) 
    SYMBOL_ITEM_OBJ(GUI_Font10_1) 
    SYMBOL_ITEM_OBJ(GUI_Font10S_ASCII) 
    SYMBOL_ITEM_OBJ(GUI_Font10S_ASCII_FontProp1) 
    SYMBOL_ITEM_OBJ(GUI_Font10S_1) 
    SYMBOL_ITEM_OBJ(GUI_Font8ASCII_Prop) 
    SYMBOL_ITEM_OBJ(GUI_Font8_ASCII) 
    SYMBOL_ITEM_OBJ(GUI_Font8_1) 
    SYMBOL_ITEM_OBJ(GUIDRV_TEMPLATE_I_16_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_TEMPLATE_I_OSXY_16_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_TEMPLATE_I_OSX_16_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_TEMPLATE_I_OSY_16_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_TEMPLATE_I_OS_16_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_TEMPLATE_I_OXY_16_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_TEMPLATE_I_OX_16_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_TEMPLATE_I_OY_16_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_Template_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_SSD1926_OSXY_8_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_SSD1926_OSX_8_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_SSD1926_OSY_8_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_SSD1926_OS_8_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_SSD1926_OXY_8_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_SSD1926_OX_8_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_SSD1926_OY_8_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_SSD1926_8_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_SPage_OSXY_4C1_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_SPage_OSX_4C1_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_SPage_OSY_4C1_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_SPage_OS_4C1_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_SPage_OXY_4C1_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_SPage_OX_4C1_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_SPage_OY_4C1_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_SPage_OSXY_4C0_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_SPage_OSX_4C0_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_SPage_OSY_4C0_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_SPage_OS_4C0_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_SPage_OXY_4C0_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_SPage_OX_4C0_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_SPage_OY_4C0_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_SPage_OSXY_2C1_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_SPage_OSX_2C1_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_SPage_OSY_2C1_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_SPage_OS_2C1_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_SPage_OXY_2C1_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_SPage_OX_2C1_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_SPage_OY_2C1_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_SPage_OSXY_2C0_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_SPage_OSX_2C0_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_SPage_OSY_2C0_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_SPage_OS_2C0_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_SPage_OXY_2C0_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_SPage_OX_2C0_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_SPage_OY_2C0_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_SPage_OSXY_1C1_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_SPage_OSX_1C1_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_SPage_OSY_1C1_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_SPage_OS_1C1_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_SPage_OXY_1C1_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_SPage_OX_1C1_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_SPage_OY_1C1_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_SPage_OSXY_1C0_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_SPage_OSX_1C0_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_SPage_OSY_1C0_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_SPage_OS_1C0_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_SPage_OXY_1C0_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_SPage_OX_1C0_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_SPage_OY_1C0_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_SPage_4C1_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_SPage_4C0_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_SPage_2C1_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_SPage_2C0_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_SPage_1C1_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_SPage_1C0_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_SLin_OSXY_2_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_SLin_OSX_2_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_SLin_OSY_2_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_SLin_OS_2_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_SLin_OXY_2_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_SLin_OX_2_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_SLin_OY_2_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_SLin_OSXY_1_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_SLin_OSX_1_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_SLin_OSY_1_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_SLin_OS_1_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_SLin_OXY_1_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_SLin_OX_1_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_SLin_OY_1_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_SLin_2_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_SLin_1_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_S1D15G00_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_S1D13781_8C0_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_S1D13781_OSX_8C0_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_S1D13781_OSY_8C0_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_S1D13781_OXY_8C0_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_S1D13748_OSXY_16_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_S1D13748_OSX_16_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_S1D13748_OSY_16_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_S1D13748_OS_16_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_S1D13748_OXY_16_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_S1D13748_OX_16_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_S1D13748_OY_16_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_S1D13748_16_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_Lin_OY_32_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_Lin_OY_24_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_Lin_OY_16_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_Lin_OX_8_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_Lin_OX_32_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_Lin_OX_24_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_Lin_OX_16_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_Lin_OXY_8_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_Lin_OXY_32_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_Lin_OXY_24_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_Lin_OXY_16_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_Lin_OS_32_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_Lin_OS_24_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_Lin_OS_16_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_Lin_OSY_32_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_Lin_OSY_24_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_Lin_OSY_16_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_Lin_OSX_32_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_Lin_OSX_24_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_Lin_OSX_16_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_Lin_8_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_Lin_4_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_Lin_32_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_Lin_24_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_Lin_2_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_Lin_16_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_Lin_1_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_IST3088_OSXY_4_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_IST3088_OSX_4_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_IST3088_OSY_4_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_IST3088_OS_4_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_IST3088_OXY_4_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_IST3088_OX_4_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_IST3088_OY_4_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_IST3088_4_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_FlexColor_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_Dist_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_DCache_API) 
    SYMBOL_ITEM_OBJ(GUIDRV_BitPlains_API) 
    SYMBOL_ITEM_OBJ(GUI_BitmapMethodsRLEAlpha) 
    SYMBOL_ITEM_OBJ(GUI_BitmapMethodsRLE8) 
    SYMBOL_ITEM_OBJ(GUI_BitmapMethodsRLE8Ex) 
    SYMBOL_ITEM_OBJ(GUI_BitmapMethodsRLE4) 
    SYMBOL_ITEM_OBJ(GUI_BitmapMethodsRLE4Ex) 
    SYMBOL_ITEM_OBJ(GUI_BitmapMethodsRLE32) 
    SYMBOL_ITEM_OBJ(GUI_BitmapMethodsRLE32Ex) 
    SYMBOL_ITEM_OBJ(GUI_BitmapMethodsRLE16) 
    SYMBOL_ITEM_OBJ(GUI_BitmapMethodsRLE16Ex) 
    SYMBOL_ITEM_OBJ(GUI_BitmapMethodsRLEM16) 
    SYMBOL_ITEM_OBJ(GUI_BitmapMethodsRLEM16Ex) 
    SYMBOL_ITEM_OBJ(LCD_aMirror) 
    SYMBOL_ITEM_OBJ(GUI__aConvert_63_255) 
    SYMBOL_ITEM_OBJ(GUI__aConvert_31_255) 
    SYMBOL_ITEM_OBJ(GUI__aConvert_255_63) 
    SYMBOL_ITEM_OBJ(GUI__aConvert_255_31) 
    SYMBOL_ITEM_OBJ(GUI_XBF_APIList_Prop_Frm) 
    SYMBOL_ITEM_OBJ(GUI_XBF_APIList_Prop_Ext) 
    SYMBOL_ITEM_OBJ(GUI_XBF_APIList_Prop) 
    SYMBOL_ITEM_OBJ(GUI_UC_None) 
    SYMBOL_ITEM_OBJ(GUI_SIF_APIList_Prop_Frm) 
    SYMBOL_ITEM_OBJ(GUI_SIF_APIList_Prop_Ext) 
    SYMBOL_ITEM_OBJ(GUI_SIF_APIList_Prop) 
    SYMBOL_ITEM_OBJ(GUI_OrientationAPI_C8) 
    SYMBOL_ITEM_OBJ(GUI_OrientationAPI_C32) 
    SYMBOL_ITEM_OBJ(GUI_OrientationAPI_C16) 
    SYMBOL_ITEM_OBJ(GUI_OrientationAPI_C0) 
    SYMBOL_ITEM_OBJ(GUIDRV_Rotate_API) 
    SYMBOL_ITEM_OBJ(GUI_GIF__aInterlaceOffset) 
    SYMBOL_ITEM_OBJ(GUI_GIF__aInterlaceYPos) 
    SYMBOL_ITEM_OBJ(GUI_ENC_APIList_SJIS) 
    SYMBOL_ITEM_OBJ(GUI_ENC_APIList_EXT) 
    SYMBOL_ITEM_OBJ(aTan) 
    SYMBOL_ITEM_OBJ(GUI_BitmapMethods8888) 
    SYMBOL_ITEM_OBJ(GUI_BitmapMethods888) 
    SYMBOL_ITEM_OBJ(GUI_BitmapMethodsM888) 
    SYMBOL_ITEM_OBJ(GUI_BitmapMethods565) 
    SYMBOL_ITEM_OBJ(GUI_BitmapMethodsM565) 
    SYMBOL_ITEM_OBJ(GUI_BitmapMethods555) 
    SYMBOL_ITEM_OBJ(GUI_BitmapMethodsM555) 
    SYMBOL_ITEM_OBJ(GUI_BitmapMethods24) 
    SYMBOL_ITEM_OBJ(_DirtyDevice_API) 
    SYMBOL_ITEM_OBJ(GUI_CursorPalI) 
    SYMBOL_ITEM_OBJ(GUI_CursorPal) 
    SYMBOL_ITEM_OBJ(GUI_PixelsHeaderM) 
    SYMBOL_ITEM_OBJ(GUI_CursorHeaderMI) 
    SYMBOL_ITEM_OBJ(GUI_CursorHeaderM) 
    SYMBOL_ITEM_OBJ(GUI_Pixels_CrossS) 
    SYMBOL_ITEM_OBJ(GUI_BitmapCrossSI) 
    SYMBOL_ITEM_OBJ(GUI_CursorCrossSI) 
    SYMBOL_ITEM_OBJ(GUI_BitmapCrossS) 
    SYMBOL_ITEM_OBJ(GUI_CursorCrossS) 
    SYMBOL_ITEM_OBJ(GUI_Pixels_CrossM) 
    SYMBOL_ITEM_OBJ(GUI_BitmapCrossMI) 
    SYMBOL_ITEM_OBJ(GUI_CursorCrossMI) 
    SYMBOL_ITEM_OBJ(GUI_BitmapCrossM) 
    SYMBOL_ITEM_OBJ(GUI_CursorCrossM) 
    SYMBOL_ITEM_OBJ(GUI_Pixels_CrossL) 
    SYMBOL_ITEM_OBJ(GUI_BitmapCrossLI) 
    SYMBOL_ITEM_OBJ(GUI_CursorCrossLI) 
    SYMBOL_ITEM_OBJ(GUI_BitmapCrossL) 
    SYMBOL_ITEM_OBJ(GUI_CursorCrossL) 
    SYMBOL_ITEM_OBJ(GUI_Pixels_ArrowS) 
    SYMBOL_ITEM_OBJ(GUI_BitmapArrowSI) 
    SYMBOL_ITEM_OBJ(GUI_CursorArrowSI) 
    SYMBOL_ITEM_OBJ(GUI_BitmapArrowS) 
    SYMBOL_ITEM_OBJ(GUI_CursorArrowS) 
    SYMBOL_ITEM_OBJ(GUI_Pixels_ArrowM) 
    SYMBOL_ITEM_OBJ(GUI_BitmapArrowMI) 
    SYMBOL_ITEM_OBJ(GUI_CursorArrowMI) 
    SYMBOL_ITEM_OBJ(GUI_BitmapArrowM) 
    SYMBOL_ITEM_OBJ(GUI_CursorArrowM) 
    SYMBOL_ITEM_OBJ(GUI_Pixels_ArrowL) 
    SYMBOL_ITEM_OBJ(GUI_BitmapArrowLI) 
    SYMBOL_ITEM_OBJ(GUI_CursorArrowLI) 
    SYMBOL_ITEM_OBJ(GUI_BitmapArrowL) 
    SYMBOL_ITEM_OBJ(GUI_CursorArrowL) 
    SYMBOL_ITEM_OBJ(GUI_CursorAnimHourglassM) 
    SYMBOL_ITEM_OBJ(GUI_Pow10) 
    SYMBOL_ITEM_OBJ(LCD_API_ColorConv_M8888) 
    SYMBOL_ITEM_OBJ(LCD_API_ColorConv_M888) 
    SYMBOL_ITEM_OBJ(LCD_API_ColorConv_1_16) 
    SYMBOL_ITEM_OBJ(LCD_API_ColorConv_1_2) 
    SYMBOL_ITEM_OBJ(LCD_API_ColorConv_1_24) 
    SYMBOL_ITEM_OBJ(LCD_API_ColorConv_1_4) 
    SYMBOL_ITEM_OBJ(LCD_API_ColorConv_1_5) 
    SYMBOL_ITEM_OBJ(LCD_API_ColorConv_1_8) 
    SYMBOL_ITEM_OBJ(LCD_API_ColorConv_8888) 
    SYMBOL_ITEM_OBJ(LCD_API_ColorConv_888) 
    SYMBOL_ITEM_OBJ(LCD_API_ColorConv_1) 
    SYMBOL_ITEM_OBJ(LCD_API_ColorConv_0) 
    SYMBOL_ITEM_OBJ(LCD_API_ColorConv_5) 
    SYMBOL_ITEM_OBJ(LCD_API_ColorConv_4) 
    SYMBOL_ITEM_OBJ(LCD_API_ColorConv_2) 
    SYMBOL_ITEM_OBJ(LCD_API_ColorConv_M8565) 
    SYMBOL_ITEM_OBJ(LCD_API_ColorConv_M666_9) 
    SYMBOL_ITEM_OBJ(LCD_API_ColorConv_M666) 
    SYMBOL_ITEM_OBJ(LCD_API_ColorConv_M655) 
    SYMBOL_ITEM_OBJ(LCD_API_ColorConv_M565) 
    SYMBOL_ITEM_OBJ(LCD_API_ColorConv_M556) 
    SYMBOL_ITEM_OBJ(LCD_API_ColorConv_M555) 
    SYMBOL_ITEM_OBJ(LCD_API_ColorConv_M444_16) 
    SYMBOL_ITEM_OBJ(LCD_API_ColorConv_M444_12) 
    SYMBOL_ITEM_OBJ(LCD_API_ColorConv_M332) 
    SYMBOL_ITEM_OBJ(LCD_API_ColorConv_M323) 
    SYMBOL_ITEM_OBJ(LCD_API_ColorConv_M233) 
    SYMBOL_ITEM_OBJ(LCD_API_ColorConv_8666_1) 
    SYMBOL_ITEM_OBJ(LCD_API_ColorConv_8666) 
    SYMBOL_ITEM_OBJ(LCD_API_ColorConv_84444) 
    SYMBOL_ITEM_OBJ(LCD_API_ColorConv_822216) 
    SYMBOL_ITEM_OBJ(LCD_API_ColorConv_666_9) 
    SYMBOL_ITEM_OBJ(LCD_API_ColorConv_666) 
    SYMBOL_ITEM_OBJ(LCD_API_ColorConv_655) 
    SYMBOL_ITEM_OBJ(LCD_API_ColorConv_565) 
    SYMBOL_ITEM_OBJ(LCD_API_ColorConv_556) 
    SYMBOL_ITEM_OBJ(LCD_API_ColorConv_555) 
    SYMBOL_ITEM_OBJ(LCD_API_ColorConv_444_16) 
    SYMBOL_ITEM_OBJ(LCD_API_ColorConv_444_12_1) 
    SYMBOL_ITEM_OBJ(LCD_API_ColorConv_444_12) 
    SYMBOL_ITEM_OBJ(LCD_API_ColorConv_332) 
    SYMBOL_ITEM_OBJ(LCD_API_ColorConv_323) 
    SYMBOL_ITEM_OBJ(LCD_API_ColorConv_233) 
    SYMBOL_ITEM_OBJ(LCD_API_ColorConv_222) 
    SYMBOL_ITEM_OBJ(LCD_API_ColorConv_M222) 
    SYMBOL_ITEM_OBJ(LCD_API_ColorConv_16) 
    SYMBOL_ITEM_OBJ(LCD_API_ColorConv_111) 
    SYMBOL_ITEM_OBJ(LCD_API_ColorConv_M111) 
    SYMBOL_ITEM_OBJ(GUI_SIF_APIList_Prop_AA4_EXT) 
    SYMBOL_ITEM_OBJ(GUI_SIF_APIList_Prop_AA4) 
    SYMBOL_ITEM_OBJ(GUI_SIF_APIList_Prop_AA2_EXT) 
    SYMBOL_ITEM_OBJ(GUI_SIF_APIList_Prop_AA2) 
    SYMBOL_ITEM_OBJ(GUI_XBF_APIList_Prop_AA4_Ext) 
    SYMBOL_ITEM_OBJ(GUI_XBF_APIList_Prop_AA2_Ext) 
    SYMBOL_ITEM_OBJ(WM__aCHWinLast) 
    SYMBOL_ITEM_OBJ(WM__aCHWinModal) 
    SYMBOL_ITEM_OBJ(WM__aCHWinMouseOver) 
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
    SYMBOL_ITEM_OBJ(WM__ahCapture) 
    SYMBOL_ITEM_OBJ(WM__ahDesktopWin) 
    SYMBOL_ITEM_OBJ(WM__ahWinFocus) 
    SYMBOL_ITEM_OBJ(WM__hATransWindow) 
    SYMBOL_ITEM_OBJ(WM__pFirstCriticalHandle) 
    SYMBOL_ITEM_OBJ(WM_pfPollPID) 
    SYMBOL_ITEM_OBJ(RADIO__pfHandleSetValue) 
    SYMBOL_ITEM_OBJ(GUI_MEMDEV__pCbAnimation) 
    SYMBOL_ITEM_OBJ(GUI_MEMDEV__pVoid) 
    SYMBOL_ITEM_OBJ(LCD__aConvTable) 
    SYMBOL_ITEM_OBJ(GUI__BIDI_Enabled) 
    SYMBOL_ITEM_OBJ(_pfGUI__BIDI_GetCharDir) 
    SYMBOL_ITEM_OBJ(_pfGUI__BIDI_GetCursorPosChar) 
    SYMBOL_ITEM_OBJ(_pfGUI__BIDI_GetCursorPosX) 
    SYMBOL_ITEM_OBJ(_pfGUI__BIDI_GetLogChar) 
    SYMBOL_ITEM_OBJ(_pfGUI__BIDI_IsNSM) 
    SYMBOL_ITEM_OBJ(_pfGUI__BIDI_Log2Vis) 
    SYMBOL_ITEM_OBJ(GUI__CharHasTrans) 
    SYMBOL_ITEM_OBJ(GUI_RectDispString) 
    SYMBOL_ITEM_OBJ(GUI_DecChar) 
    SYMBOL_ITEM_OBJ(GUI_OrgX) 
    SYMBOL_ITEM_OBJ(GUI_OrgY) 
    SYMBOL_ITEM_OBJ(GUI__apDevice) 
    SYMBOL_ITEM_OBJ(GUI_pLCD_APIList) 
    SYMBOL_ITEM_OBJ(GUI_pUC_API) 
    SYMBOL_ITEM_OBJ(GUI_pfDispCharStyle) 
    SYMBOL_ITEM_OBJ(GUI_pfTimerExec) 
    SYMBOL_ITEM_OBJ(GUI_sError) 
    SYMBOL_ITEM_OBJ(WM_pfHandlePID) 
    SYMBOL_ITEM_OBJ(GUI_ALLOC__Context) 
SYMBOL_TABLE_END												
#endif                                               
/*********************************************************************************************************	
** END FILE													
*********************************************************************************************************/	
