#ifndef __WINDEF_H 
#define __WINDEF_H 

#include "common.h"

#define WM_SYNCMSG         0x0000
#define WM_NULLMSG         0x0000
/* Group 1 from 0x0001 to 0x000F, the mouse messages.*/
#define WM_FIRSTMOUSEMSG   0x0001
#define WM_LBUTTONDOWN     0x0001
#define WM_LBUTTONUP       0x0002
#define WM_LBUTTONDBLCLK   0x0003
#define WM_MOUSEMOVE       0x0004
#define WM_RBUTTONDOWN     0x0005
#define WM_RBUTTONUP       0x0006
#define WM_RBUTTONDBLCLK   0x0007
#define WM_NCMOUSEOFF      0x0007
#define WM_NCLBUTTONDOWN   0x0008
#define WM_NCLBUTTONUP     0x0009
#define WM_NCLBUTTONDBLCLK 0x000A
#define WM_NCMOUSEMOVE     0x000B
#define WM_NCRBUTTONDOWN   0x000C
#define WM_NCRBUTTONUP     0x000D
#define WM_NCRBUTTONDBLCLK 0x000E
#define WM_LASTMOUSEMSG    0x000F
/* Group 2 from 0x0010 to 0x001F, the key messages.*/
#define WM_FIRSTKEYMSG     0x0010
#define WM_KEYDOWN         0x0010
#define WM_CHAR            0x0011
#define WM_KEYUP           0x0012
#define WM_SYSKEYDOWN      0x0013
#define WM_SYSCHAR         0x0014
#define WM_SYSKEYUP        0x0015
#define WM_LASTKEYMSG      0x001F
/* Group 3 from 0x0020 to 0x005F, the post-mousekey messages.*/
#define WM_FIRSTPOSTMSG    0x0020
#define WM_SETCURSOR       0x0020
#define HT_UNKNOWN          0x00
#define HT_OUT              0x01
#define HT_MENUBAR          0x02
#define HT_TRANSPARENT      0x03
#define HT_BORDER_TOP       0x04
#define HT_BORDER_BOTTOM    0x05
#define HT_BORDER_LEFT      0x06
#define HT_BORDER_RIGHT     0x07
#define HT_CORNER_TL        0x08
#define HT_CORNER_TR        0x09
#define HT_CORNER_BL        0x0A
#define HT_CORNER_BR        0x0B
#define HT_CLIENT           0x0C
#define SBPOS_LEFTARROW     0x81
#define SBPOS_RIGHTARROW    0x82
#define SBPOS_LEFTSPACE     0x83
#define SBPOS_RIGHTSPACE    0x84
#define SBPOS_UPARROW       0x85
#define SBPOS_DOWNARROW     0x86
#define SBPOS_UPSPACE       0x87
#define SBPOS_DOWNSPACE     0x88
#define SBPOS_THUMB         0x89
#define SBPOS_UNKNOWN       0x80
#define SBPOS_MASK          0x80
#define HT_BORDER           0x11
#define HT_NCLIENT          0x12
#define HT_CAPTION          0x13
#define HT_ICON             0x14
#define HT_CLOSEBUTTON      0x15
#define HT_MAXBUTTON        0x16
#define HT_MINBUTTON        0x17
#define HT_HSCROLL          0x18
#define HT_VSCROLL          0x19
#define HT_NEEDCAPTURE      0x10
#define WM_NCHITTEST       0x0021      
#define WM_HITTEST         WM_NCHITTEST
#define WM_CHANGESIZE      0x0022
#define WM_QUERYNCRECT     0x0023
#define WM_QUERYCLIENTAREA 0x0024
#define WM_SIZECHANGING    0x0025
#define WM_SIZECHANGED     0x0026
#define WM_SETFOCUS        0x0030
#define WM_KILLFOCUS       0x0031
#define WM_MOUSEACTIVE     0x0032
#define WM_ACTIVE          0x0033
#define WM_ACTIVEMENU      0x0040
#define WM_DEACTIVEMENU    0x0041
/* Scroll bar notifying code*/
#define SB_LINEUP           0x01
#define SB_LINEDOWN         0x02
#define SB_LINELEFT         0x03
#define SB_LINERIGHT        0x04
#define SB_PAGEUP           0x05
#define SB_PAGEDOWN         0x06
#define SB_PAGELEFT         0x07
#define SB_PAGERIGHT        0x08
#define SB_THUMBPOSITION    0x09
#define SB_THUMBTRACK       0x0A
#define SB_ENDSCROLL        0x0B
#define WM_HSCROLL         0x0042
#define WM_VSCROLL         0x0043
#define WM_NCSETCURSOR     0x0044
#define WM_MOUSEMOVEIN     0x0050
#define WM_LASTPOSTMSG     0x005F
/* Group 4 from 0x0060 to 0x009F, the creation messages.*/
#define WM_FIRSTCREATEMSG  0x0060
#define WM_CREATE          0x0060
#define WM_NCCREATE        0x0061
#define WM_INITPANES       0x0062
#define WM_DESTROYPANES    0x0063
#define WM_DESTROY         0x0064
#define WM_NCDESTROY       0x0065
#define WM_CLOSE           0x0066
#define WM_NCCALCSIZE      0x0067
#define WM_MAXIMIZE		0x0068
#define WM_MINIMIZE		0x0069
#define WM_LASTCREATEMSG   0x009F
/* Group 5 from 0x00A0 to 0x00CF, the paint messages.*/
#define WM_FIRSTPAINTMSG   0x00A0
#define WM_SHOWWINDOW      0x00A0 
#define WM_ERASEBKGND      0x00B0      /* this is an async message.*/
#define WM_PAINT           0x00B1
#define WM_NCPAINT         0x00B2
#define WM_NCACTIVATE      0x00B3
#define WM_SYNCPAINT       0x00B4
#define WM_ENABLE          0x00C0
#define WM_LASTPAINTMSG    0x00CF
/* Group 6 from 0x00D0 to 0x00EF, the desktop messages.*/
#define WM_FIRSTSESSIONMSG 0x00D0
#define WM_STARTSESSION    0x00D0
#define WM_QUERYENDSESSION 0x00D1
#define WM_ENDSESSION      0x00D2
#define WM_REINITSESSION   0x00D3
#define WM_ERASEDESKTOP    0x00E0
#define WM_PAINTDESKTOP    0x00E1
#define WM_DT_MOUSEOFF     0x00E1
#define WM_DT_LBUTTONDOWN  0x00E2
#define WM_DT_LBUTTONUP    0x00E3
#define WM_DT_LBUTTONDBLCLK    0x00E4
#define WM_DT_MOUSEMOVE    0x00E5
#define WM_DT_RBUTTONDOWN  0x00E6
#define WM_DT_RBUTTONUP    0x00E7
#define WM_DT_RBUTTONDBLCLK    0x00E8
#define WM_DT_KEYOFF       0x00DA 
#define WM_DT_KEYDOWN      0x00EA
#define WM_DT_CHAR         0x00EB
#define WM_DT_KEYUP        0x00EC
#define WM_DT_SYSKEYDOWN   0x00ED
#define WM_DT_SYSCHAR      0x00EE
#define WM_DT_SYSKEYUP     0x00EF
#define WM_LASTSESSIONMSG  0x00EF
/* Group 7 from 0x00F0 to 0x010F, the window messages.*/
#define WM_FIRSTWINDOWMSG  0x00F0
#define WM_ADDNEWMAINWIN   0x00F0
#define WM_REMOVEMAINWIN   0x00F1
#define WM_MOVETOTOPMOST   0x00F2 
#define WM_SETACTIVEMAIN   0x00F3
#define WM_GETACTIVEMAIN   0x00F4
#define WM_SHOWMAINWIN     0x00F5
#define WM_HIDEMAINWIN     0x00F6
#define WM_MOVEMAINWIN     0x00F7
#define WM_SETCAPTURE      0x00F8
#define WM_GETCAPTURE      0x00F9
#define WM_ENDTRACKMENU    0x00FA
#define WM_TRACKPOPUPMENU  0x00FB
#define WM_CLOSEMENU       0x00FC
#define WM_SCROLLMAINWIN   0x00FD
#define WM_CARET_CREATE    0x00FE
#define WM_CARET_DESTROY   0x00FF
#define WM_ENABLEMAINWIN   0x0100
#define WM_ISENABLED       0x0101
#define WM_SETWINCURSOR    0x0102
#define WM_SHOWGLOBALCTRL  0x010A
#define WM_HIDEGLOBALCTRL  0x010B
#define WM_LASTWINDOWMSG   0x010F
/* Group 8 from 0x0120 to 0x013F, the dialog and control messages.*/
#define WM_FIRSTCONTROLMSG 0x0120
#define WM_COMMAND         0x0120
#define WM_SYSCOMMAND      0x0121
#define WM_GETDLGCODE      0x0122
#define WM_INITDIALOG      0x0123
#define WM_NEXTDLGCTRL     0x0124
#define WM_ENTERIDLE       0x0125
#define WM_DLG_GETDEFID    0x0126
#define WM_DLG_SETDEFID    0x0127
#define WM_DLG_REPOSITION  0x0128
#define WM_GETFONT         0x0130
#define WM_SETFONT         0x0131
#define WM_GETTEXTLENGTH   0x0132
#define WM_GETTEXT         0x0133
#define WM_SETTEXT         0x0134
#define WM_LASTCONTROLMSG  0x013F
/* Group 9 from 0x0140 to 0x016F, the system messages.*/
#define WM_FIRSTSYSTEMMSG  0x0140
#define WM_QUIT            0x0140
#define WM_IDLE            0x0142
#define WM_TIMEOUT         0x0143
#define WM_TIMER           0x0144
#define WM_CARETBLINK      0x0145
#define WM_IME_REGISTER    0x0151
#define WM_IME_UNREGISTER  0x0152
#define WM_IME_OPEN        0x0153
#define WM_IME_CLOSE       0x0154
#define WM_IME_SETSTATUS   0x0156
#define WM_IME_GETSTATUS   0x0157
#define IS_ENABLE       1
#define IS_FULLCHAR     2
#define IS_FULLPUNC     3
#define IS_METHOD       4
#define WM_IME_SETTARGET   0x0158
#define WM_IME_GETTARGET   0x0159
#define WM_SHOWMENU        0x0160
#define WM_HIDEMENU        0x0161
#define WM_ADDTIMER        0x0162
#define WM_REMOVETIMER     0x0163
#define WM_RESETTIMER      0x0164
#define WM_WINDOWCHANGED   0x0165
#define WM_BROADCASTMSG    0x0166
#define WM_REGISTERWNDCLASS    0x0167
#define WM_UNREGISTERWNDCLASS  0x0168
#define WM_NEWCTRLINSTANCE     0x0169
#define WM_REMOVECTRLINSTANCE  0x016A
#define WM_GETCTRLCLASSINFO    0x016B
#define WM_CTRLCLASSDATAOP     0x016C
#define CCDOP_GETCCI        0x01
#define CCDOP_SETCCI        0x02
#define WM_REGISTERKEYHOOK     0x016D
#define WM_REGISTERMOUSEHOOK   0x016E
#define WM_UNREGISTERHOOK      0x016F
#define WM_LASTSYSTEMMSG   0x016F
/* Group 10 from 0x0170 to 0x018F, the menu messages*/
#define WM_FIRSTMENUMSG    0x0170
#define WM_INITMENU        0x0170
#define WM_INITMENUPOPUP   0x0171
#define WM_MENUSELECT      0x0172
#define WM_MENUCHAR        0x0173
#define WM_ENTERMENULOOP   0x0174
#define WM_EXITMENULOOP    0x0175
#define WM_CONTEXTMENU     0x0176
#define WM_NEXTMENU        0x0177
#define WM_LASTMENUMSG     0x018F
/* Group 11 is the global message*/
#define WM_FIRSTSYSMSG     0x0190
#define WM_SYS_MOUSEMOVE   0x0191
#define WM_SYS_LBUTTONDOWN 0x0192
#define WM_SYS_RBUTTONDOWN 0x0193
#define WM_SYS_LBUTTONUP   0x0194
#define WM_SYS_RBUTTONUP   0x0195
#define WM_SYS_MOUSEHOVER  0x0196
#define WM_SYS_TIMER       0x0197
#define WM_SYS_QUIT        0x0198
#define WM_SYS_KEYDOWN     0x0199
#define WM_SYS_KEYUP       0x019A
#define WM_SYS_MOUSEHOLD   0x019B
#define WM_SYS_DESTROY	   0x019C
#define WM_LASTSYSMSG      0x019D

/* self define message*/
#define WM_FIRSTSELFMSG    0x0300
#define WM_MOUSEHOVER      0x0301
#define WM_MOUSEOUT        0x0302
#define WM_SCROLL          0x0303
#define WM_MOUSEHOLD       0x0304
#define WM_WINDOWPOSCHANGED 0x0305
#define WM_WINDOWPOSCHANGE  0x0306
#define WM_LASTSELFMSG     0x0307

#define WM_FIRSTUSERMSG    0x0800
#define WM_USER            0x0800
#define WM_LASTUSERMSG     0xEFFF

/* Basic window types */
#define WS_OVERLAPPED       0x00000000L
#define WS_POPUP            0x80000000L
#define WS_CHILD            0x40000000L

/* Generic window states */
#define WS_VISIBLE          0x08000000L
#define WS_DISABLED         0x04000000L

/* Main window styles */
#define WS_CAPTION          0x20000000L
#define WS_SYSMENU          0x10000000L
#define WS_STATICEDGE       0x00800000L
#define WS_BORDER           0x00400000L
#define WS_THICKFRAME       0x00200000L
#define WS_THINFRAME        0x00100000L
#define WS_VSCROLL          0x00080000L
#define WS_HSCROLL          0x00040000L
#define WS_MINIMIZEBOX      0x00020000L
#define WS_MAXIMIZEBOX      0x00010000L

/* My window style*/
#define WS_NOFOCUS		    0x00000001L
#define WS_NOACTIVE			0x00000002L
#define WS_SYSMOUSE			0x00000004L
#define WS_CLOSEBOXCHECKED	0x00000008L
#define WS_ACTIVE			0x00000010L
#define WS_FOCUS			0x00000020L
#define WS_CAPTURE			0x00000040L
#define WS_CHECKED			0x00000080L
#define WS_MENUWND			0x00000100L
#define WS_APPWND			0x00000200L

/* Common Window Styles */
#define WS_OVERLAPPEDWINDOW (WS_OVERLAPPED  | \
                             WS_CAPTION     | \
                             WS_SYSMENU     | \
                             WS_THICKFRAME  | \
                             WS_MINIMIZEBOX | \
                             WS_MAXIMIZEBOX )
                             
#define WS_POPUPWINDOW      (WS_POPUP       | \
                             WS_BORDER      | \
                             WS_SYSMENU)
#define WS_CHILDWINDOW      (WS_CHILD)
/* Extended Window Styles */
#define WS_EX_NONE              0x00000000L     
#define WS_EX_DLGMODALFRAME     0x00000001L
#define WS_EX_USEPRIVATECDC     0x00000002L     
#define WS_EX_NOPARENTNOTIFY    0x00000004L
#define WS_EX_TOPMOST           0x00000008L
#define WS_EX_LOWMOST           0x00000010L
#define WS_EX_TRANSPARENT       0x00000020L
#define WS_EX_MDICHILD          0x00000040L
#define WS_EX_TOOLWINDOW        0x00000080L
#define WS_EX_WINDOWEDGE        0x00000100L
#define WS_EX_CLIENTEDGE        0x00000200L
#define WS_EX_CONTEXTHELP       0x00000400L
#define WS_EX_USEPARENTCURSOR   0x00000800L     
#define WS_EX_RIGHT             0x00001000L
#define WS_EX_LEFT              0x00000000L
#define WS_EX_RTLREADING        0x00002000L
#define WS_EX_LTRREADING        0x00000000L
#define WS_EX_LEFTSCROLLBAR     0x00004000L
#define WS_EX_RIGHTSCROLLBAR    0x00000000L
#define WS_EX_CONTROLPARENT     0x00010000L
#define WS_EX_STATICEDGE        0x00020000L
#define WS_EX_APPWINDOW         0x00040000L
#define WS_EX_IMECOMPOSE        0x10000000L     
#define WS_EX_NOCLOSEBOX        0x20000000L     
#define WS_EX_CTRLASMAINWIN     0x40000000L     
#define WS_EX_OVERLAPPEDWINDOW  (WS_EX_WINDOWEDGE | WS_EX_CLIENTEDGE)

/*Scroll bar styles*/
/*The first bit of the style indicate the direcation of the scrollbar*/
#define SBS_VERT				0x00000000
#define SBS_HORZ				0x00000001
#define SBS_ARROW1_CHECKED		0x00000002
#define SBS_ARROW2_CHECKED		0x00000004
#define SBS_AUTO_SCROLL_ENABLE	0x00000008
#define SBS_AUTO_SCROLL_UP		0x00000010
#define SBS_AUTO_SCROLL_DOWN	0x00000020

/*DrawTextEx formats*/
#define DT_TOP              0x00000000
#define DT_LEFT             0x00000000
#define DT_CENTER           0x00000001
#define DT_RIGHT            0x00000002
#define DT_VCENTER          0x00000004
#define DT_BOTTOM           0x00000008
#define DT_WORDBREAK        0x00000010
#define DT_SINGLELINE       0x00000020
#define DT_EXPANDTABS       0x00000040
#define DT_TABSTOP          0x00000080
#define DT_NOCLIP           0x00000100
#define DT_EXTERNALLPADING  0x00000200
#define DT_CALCRECT         0x00000400
#define DT_NOPREFIX         0x00000800
#define DT_INTERNAL         0x00001000

/* Button style*/
#define BS_PUSHBUTTON       0x00000000L
#define BS_DEFPUSHBUTTON    0x00000001L
#define BS_CHECKBOX         0x00000002L
#define BS_AUTOCHECKBOX     0x00000003L
#define BS_RADIOBUTTON      0x00000004L
#define BS_3STATE           0x00000005L
#define BS_AUTO3STATE       0x00000006L
#define BS_GROUPBOX         0x00000007L
#define BS_USERBUTTON       0x00000008L
#define BS_AUTORADIOBUTTON  0x00000009L
#define BS_OWNERDRAW        0x0000000BL
#define BS_TYPEMASK         0x0000000FL
#define BS_TEXT             0x00000000L
#define BS_LEFTTEXT         0x00000020L
#define BS_ICON             0x00000040L
#define BS_BITMAP           0x00000080L
#define BS_CONTENTMASK      0x000000F0L
#define BS_LEFT             0x00000100L
#define BS_RIGHT            0x00000200L
#define BS_CENTER           0x00000300L
#define BS_TOP              0x00000400L
#define BS_BOTTOM           0x00000800L
#define BS_VCENTER          0x00000C00L
#define BS_ALIGNMASK        0x00000F00L
#define BS_PUSHLIKE         0x00001000L
#define BS_MULTLINE         0x00002000L
#define BS_NOTIFY           0x00004000L
#define BS_FLAT             0x00008000L
#define BS_CHECKED			0x10000000L
#define BS_RIGHTBUTTON      BS_LEFTTEXT
#define BST_UNCHECKED       0x0000
#define BST_CHECKED         0x0001
#define BST_INDETERMINATE   0x0002
#define BST_PUSHED          0x0004
#define BST_FOCUS           0x0008
#define SS_LEFT             0x00000000L
#define SS_CENTER           0x00000001L
#define SS_RIGHT            0x00000002L
#define SS_ICON             0x00000003L
#define SS_3DHLINE          0x00000004L
#define SS_3DVLINE          0x00000005L
#define SS_WHITERECT        0x00000006L
#define SS_BLACKFRAME       0x00000007L
#define SS_GRAYFRAME        0x00000008L
#define SS_WHITEFRAME       0x00000009L
#define SS_GROUPBOX         0x0000000AL
#define SS_SIMPLE           0x0000000BL
#define SS_LEFTNOWORDWRAP   0x0000000CL
#define SS_OWNERDRAW        0x0000000DL
#define SS_BITMAP           0x0000000EL
#define SS_ENHMETAFILE      0x0000000FL
#define SS_TYPEMASK         0x0000000FL
#define SS_NOPREFIX         0x00000080L
#define SS_ETCHEDHORZ       0x00000010L
#define SS_ETCHEDVERT       0x00000011L
#define SS_ETCHEDFRAME      0x00000012L
#define SS_ETCTYPEMAKS      0x0000001FL
#define SS_NOTIFY           0x00000100L
#define SS_CENTERIMAGE      0x00000200L
#define SS_RIGHTJUST        0x00000400L
#define SS_REALSIZEIMAGE    0x00000800L
#define MB_OK                   0x00000001
#define MB_OKCANCEL             0x00000002
#define MB_YESNO                0x00000003
#define MB_RETRYCANCEL          0x00000004
#define MB_ABORTRETRYIGNORE     0x00000005
#define MB_YESNOCANCEL          0x00000006
#define MB_CANCELASBACK         0x00000007  /* customized style*/
#define MB_TYPEMASK             0x00000008
#define MB_CANCEL	            0x00000009
#define MB_ICONHAND             0x00000010
#define MB_ICONQUESTION         0x00000020
#define MB_ICONEXCLAMATION      0x00000030
#define MB_ICONASTERISK         0x00000040
#define MB_ICONMASK             0x000000F0
#define MB_ICONINFORMATION      MB_ICONASTERISK
#define MB_ICONSTOP             MB_ICONHAND
#define MB_DEFBUTTON1           0x00000000
#define MB_DEFBUTTON2           0x00000100
#define MB_DEFBUTTON3           0x00000200
#define MB_DEFMASK              0x00000F00

/* Listbox Notification Codes */
#define LBN_ERRSPACE            (-2)
#define LBN_SELCHANGE           1
#define LBN_DBLCLK              2
#define LBN_SELCANCEL           3
#define LBN_SETFOCUS            4
#define LBN_KILLFOCUS           5
#define LBN_CLICKCHECKMARK      6
#define LBN_CLICKED             7
#define LBN_LBUTTONUP           8
#define LBN_ABOUTSHOW			9

#endif
