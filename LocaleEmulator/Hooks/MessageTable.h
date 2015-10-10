#ifndef _MESSAGETABLE_H_4a84968c_ce14_49b6_99a3_353e9db99f11_
#define _MESSAGETABLE_H_4a84968c_ce14_49b6_99a3_353e9db99f11_

enum MessageFunctionIndex
{
    IMSG_DWORD                                = 0x00000000,
    IMSG_DWORD2                               = 0x00000001,
    IMSG_INLPCREATESTRUCT                     = 0x00000002,
    IMSG_INSTRINGNULL                         = 0x00000003,
    IMSG_OUTSTRING                            = 0x00000004,
    IMSG_INSTRING                             = 0x00000005,
    IMSG_INOUTLPPOINT5                        = 0x00000006,
    IMSG_INLPDRAWITEMSTRUCT                   = 0x00000007,
    IMSG_INOUTLPMEASUREITEMSTRUCT             = 0x00000008,
    IMSG_INLPDELETEITEMSTRUCT                 = 0x00000009,
    IMSG_INWPARAMCHAR                         = 0x0000000A,
    IMSG_INLPHLPSTRUCT                        = 0x0000000B,
    IMSG_INLPCOMPAREITEMSTRUCT                = 0x0000000C,
    IMSG_INOUTLPSCROLLINFO                    = 0x0000000D,
    IMSG_INLPWINDOWPOS                        = 0x0000000E,
    IMSG_COPYGLOBALDATA                       = 0x0000000F,
    IMSG_COPYDATA                             = 0x00000010,
    IMSG_INLPHELPINFOSTRUCT                   = 0x00000011,
    IMSG_INOUTSTYLECHANGE                     = 0x00000012,
    IMSG_INOUTNCCALCSIZE                      = 0x00000013,
    IMSG_DWORDOPTINLPMSG                      = 0x00000014,
    IMSG_INOUTLPRECT                          = 0x00000015,
    IMSG_OPTOUTLPDWORDOPTOUTLPDWORD           = 0x00000016,
    IMSG_OUTLPRECT                            = 0x00000017,
    IMSG_INCNTOUTSTRING                       = 0x00000018,
    IMSG_POPTINLPUINT                         = 0x00000019,
    IMSG_INOUTLPSCROLLINFO2                   = 0x0000001A,
    IMSG_INCBOXSTRING                         = 0x0000001B,
    IMSG_OUTCBOXSTRING                        = 0x0000001C,
    IMSG_INLBOXSTRING                         = 0x0000001D,
    IMSG_OUTLBOXSTRING                        = 0x0000001E,
    IMSG_POUTLPINT                            = 0x0000001F,
    IMSG_OUTDWORDINDWORD                      = 0x00000020,
    IMSG_INOUTNEXTMENU                        = 0x00000021,
    IMSG_INDEVICECHANGE                       = 0x00000022,
    IMSG_INLPMDICREATESTRUCT                  = 0x00000023,
    IMSG_INOUTLPSCROLLINFO3                   = 0x00000024,
    IMSG_NCDESTROY                            = 0x00000025,
    IMSG_INPAINTCLIPBRD                       = 0x00000026,
    IMSG_INSIZECLIPBRD                        = 0x00000027,
    IMSG_INCNTOUTSTRINGNULL                   = 0x00000028,
    IMSG_KERNELONLY                           = 0x00000029,
    IMSG_DDEINIT                              = 0x0000002A,
    IMSG_SENTDDEMSG                           = 0x0000002B,
    IMSG_GETDBCSTEXTLENGTHS                   = 0x0000002C,
    IMSG_OPTOUTLPDWORDOPTOUTLPDWORD2          = 0x0000002D,
    IMSG_NCDESTROY2                           = 0x0000002E,
    IMSG_NCDESTROY3                           = 0x0000002F,
    IMSG_OPTOUTLPDWORDOPTOUTLPDWORD3          = 0x00000030,
    IMSG_IMECONTROL                           = 0x00000031,
    IMSG_INOUTMENUGETOBJECT                   = 0x00000032,
    IMSG_POWERBROADCAST                       = 0x00000033,
    IMSG_KERNELONLY2                          = 0x00000034,
    IMSG_OUTLPCOMBOBOXINFO                    = 0x00000035,
    IMSG_OUTLPSCROLLBARINFO                   = 0x00000036,
    IMSG_INLPUAHINITMENU                      = 0x00000037,
    IMSG_INLPUAHDRAWMENUITEM                  = 0x00000038,
    IMSG_INLPUAHINITMENU2                     = 0x00000039,
    IMSG_INOUTLPUAHMEASUREMENUITEM            = 0x0000003A,
    IMSG_INLPUAHINITMENU3                     = 0x0000003B,
    IMSG_OUTLPTITLEBARINFOEX                  = 0x0000003C,
    IMSG_TOUCH                                = 0x0000003D,
    IMSG_GESTURE                              = 0x0000003E,
    IMSG_INPGESTURENOTIFYSTRUCT               = 0x0000003F,
    INPGESTURENOTIFYSTRUCT                    = 0x00000040,
    PARENTNOTIFY                              = 0x00000041,
    POINTERINPUT                              = 0x00000042,
    TOUCHHITTESTING                           = 0x00000043,
};

typedef struct
{
    BYTE    Function;
    BYTE    ThunkMessage       : 1;
    BYTE    SyncOnlyMessage    : 1;
} ML_MSG_TABLE_ENTRY;

ML_MSG_TABLE_ENTRY MessageTable[] =
{
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_NULL
    { IMSG_INLPCREATESTRUCT,                  TRUE,  TRUE,       }, // WM_CREATE
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_DESTROY
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_MOVE
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0004
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_SIZE
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_ACTIVATE
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_SETFOCUS
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_KILLFOCUS
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0009
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_ENABLE
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_SETREDRAW
    { IMSG_INSTRINGNULL,                      TRUE,  TRUE,       }, // WM_SETTEXT
    { IMSG_OUTSTRING,                         TRUE,  TRUE,       }, // WM_GETTEXT
    { IMSG_GETDBCSTEXTLENGTHS,                TRUE,  TRUE,       }, // WM_GETTEXTLENGTH
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_PAINT
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_CLOSE
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_QUERYENDSESSION
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_QUIT
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_QUERYOPEN
    { IMSG_DWORD,                             FALSE, TRUE,       }, // WM_ERASEBKGND
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_SYSCOLORCHANGE
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_ENDSESSION
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0017
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_SHOWWINDOW
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0019
    { IMSG_INSTRINGNULL,                      TRUE,  TRUE,       }, // WM_SETTINGCHANGE
    { IMSG_INSTRING,                          TRUE,  TRUE,       }, // WM_DEVMODECHANGE
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_ACTIVATEAPP
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_FONTCHANGE
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_TIMECHANGE
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_CANCELMODE
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_SETCURSOR
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_MOUSEACTIVATE
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_CHILDACTIVATE
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_QUEUESYNC
    { IMSG_INOUTLPPOINT5,                     FALSE, TRUE,       }, // WM_GETMINMAXINFO
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0025
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_PAINTICON
    { IMSG_DWORD,                             FALSE, TRUE,       }, // WM_ICONERASEBKGND
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_NEXTDLGCTL
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0029
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_SPOOLERSTATUS
    { IMSG_INLPDRAWITEMSTRUCT,                FALSE, TRUE,       }, // WM_DRAWITEM
    { IMSG_INOUTLPMEASUREITEMSTRUCT,          FALSE, TRUE,       }, // WM_MEASUREITEM
    { IMSG_INLPDELETEITEMSTRUCT,              FALSE, TRUE,       }, // WM_DELETEITEM
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_VKEYTOITEM
    { IMSG_INWPARAMCHAR,                      TRUE,  FALSE,      }, // WM_CHARTOITEM
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_SETFONT
    { IMSG_DWORD,                             FALSE, TRUE,       }, // WM_GETFONT
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_SETHOTKEY
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_GETHOTKEY
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0034
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0035
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0036
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_QUERYDRAGICON
    { IMSG_INLPHLPSTRUCT,                     FALSE, TRUE,       }, // 0x0038
    { IMSG_INLPCOMPAREITEMSTRUCT,             FALSE, TRUE,       }, // WM_COMPAREITEM
    { IMSG_KERNELONLY,                        FALSE, FALSE,      }, // 0x003A
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x003B
    { IMSG_KERNELONLY,                        FALSE, TRUE,       }, // 0x003C
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_GETOBJECT
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x003E
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x003F
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0040
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_COMPACTING
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0042
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0043
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_COMMNOTIFY
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0045
    { IMSG_INOUTLPSCROLLINFO,                 FALSE, TRUE,       }, // WM_WINDOWPOSCHANGING
    { IMSG_INLPWINDOWPOS,                     FALSE, TRUE,       }, // WM_WINDOWPOSCHANGED
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_POWER
    { IMSG_COPYGLOBALDATA,                    TRUE,  TRUE,       }, // 0x0049
    { IMSG_COPYDATA,                          FALSE, TRUE,       }, // WM_COPYDATA
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_CANCELJOURNAL
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x004C
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x004D
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_NOTIFY
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x004F
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_INPUTLANGCHANGEREQUEST
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_INPUTLANGCHANGE
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_TCARD
    { IMSG_INLPHELPINFOSTRUCT,                FALSE, TRUE,       }, // WM_HELP
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_USERCHANGED
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_NOTIFYFORMAT
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0056
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0057
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0058
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0059
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x005A
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x005B
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x005C
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x005D
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x005E
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x005F
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0060
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0061
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0062
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0063
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0064
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0065
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0066
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0067
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0068
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0069
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x006A
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x006B
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x006C
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x006D
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x006E
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x006F
    { IMSG_KERNELONLY,                        FALSE, TRUE,       }, // 0x0070
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0071
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0072
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0073
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0074
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0075
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0076
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0077
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0078
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0079
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x007A
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_CONTEXTMENU
    { IMSG_INOUTSTYLECHANGE,                  FALSE, TRUE,       }, // WM_STYLECHANGING
    { IMSG_INOUTSTYLECHANGE,                  FALSE, TRUE,       }, // WM_STYLECHANGED
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_DISPLAYCHANGE
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_GETICON
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_SETICON
    { IMSG_INLPCREATESTRUCT,                  TRUE,  TRUE,       }, // WM_NCCREATE
    { IMSG_DWORD2,                            FALSE, FALSE,      }, // WM_NCDESTROY
    { IMSG_INOUTNCCALCSIZE,                   FALSE, TRUE,       }, // WM_NCCALCSIZE
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_NCHITTEST
    { IMSG_DWORD,                             FALSE, TRUE,       }, // WM_NCPAINT
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_NCACTIVATE
    { IMSG_DWORDOPTINLPMSG,                   FALSE, TRUE,       }, // WM_GETDLGCODE
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_SYNCPAINT
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0089
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x008A
    { IMSG_INOUTLPRECT,                       FALSE, TRUE,       }, // 0x008B
    { IMSG_KERNELONLY2,                       FALSE, TRUE,       }, // 0x008C
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x008D
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x008E
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x008F
    { IMSG_DWORD,                             FALSE, TRUE,       }, // 0x0090
    { IMSG_INLPUAHINITMENU,                   FALSE, TRUE,       }, // 0x0091
    { IMSG_INLPUAHDRAWMENUITEM,               FALSE, TRUE,       }, // 0x0092
    { IMSG_INLPUAHINITMENU2,                  FALSE, TRUE,       }, // 0x0093
    { IMSG_INOUTLPUAHMEASUREMENUITEM,         FALSE, TRUE,       }, // 0x0094
    { IMSG_INLPUAHINITMENU3,                  FALSE, TRUE,       }, // 0x0095
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0096
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0097
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0098
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0099
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x009A
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x009B
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x009C
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x009D
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x009E
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x009F
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_NCMOUSEMOVE
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_NCLBUTTONDOWN
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_NCLBUTTONUP
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_NCLBUTTONDBLCLK
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_NCRBUTTONDOWN
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_NCRBUTTONUP
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_NCRBUTTONDBLCLK
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_NCMBUTTONDOWN
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_NCMBUTTONUP
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_NCMBUTTONDBLCLK
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x00AA
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_NCXBUTTONDOWN
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_NCXBUTTONUP
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_NCXBUTTONDBLCLK
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x00AE
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x00AF
    { IMSG_OPTOUTLPDWORDOPTOUTLPDWORD2,       FALSE, TRUE,       }, // EM_GETSEL
    { IMSG_NCDESTROY2,                        FALSE, FALSE,      }, // EM_SETSEL
    { IMSG_OUTLPRECT,                         FALSE, TRUE,       }, // EM_GETRECT
    { IMSG_INOUTLPRECT,                       FALSE, TRUE,       }, // EM_SETRECT
    { IMSG_INOUTLPRECT,                       FALSE, TRUE,       }, // EM_SETRECTNP
    { IMSG_DWORD,                             FALSE, FALSE,      }, // EM_SCROLL
    { IMSG_DWORD,                             FALSE, FALSE,      }, // EM_LINESCROLL
    { IMSG_DWORD,                             FALSE, FALSE,      }, // EM_SCROLLCARET
    { IMSG_DWORD,                             FALSE, FALSE,      }, // EM_GETMODIFY
    { IMSG_DWORD,                             FALSE, FALSE,      }, // EM_SETMODIFY
    { IMSG_DWORD,                             FALSE, FALSE,      }, // EM_GETLINECOUNT
    { IMSG_DWORD,                             FALSE, FALSE,      }, // EM_LINEINDEX
    { IMSG_DWORD,                             FALSE, FALSE,      }, // EM_SETHANDLE
    { IMSG_DWORD,                             FALSE, FALSE,      }, // EM_GETHANDLE
    { IMSG_DWORD,                             FALSE, FALSE,      }, // EM_GETTHUMB
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x00BF
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x00C0
    { IMSG_DWORD,                             FALSE, FALSE,      }, // EM_LINELENGTH
    { IMSG_INSTRINGNULL,                      TRUE,  TRUE,       }, // EM_REPLACESEL
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x00C3
    { IMSG_INCNTOUTSTRING,                    TRUE,  TRUE,       }, // EM_GETLINE
    { IMSG_DWORD,                             FALSE, FALSE,      }, // EM_LIMITTEXT
    { IMSG_DWORD,                             FALSE, FALSE,      }, // EM_CANUNDO
    { IMSG_DWORD,                             FALSE, FALSE,      }, // EM_UNDO
    { IMSG_DWORD,                             FALSE, FALSE,      }, // EM_FMTLINES
    { IMSG_DWORD,                             FALSE, FALSE,      }, // EM_LINEFROMCHAR
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x00CA
    { IMSG_POPTINLPUINT,                      FALSE, TRUE,       }, // EM_SETTABSTOPS
    { IMSG_NCDESTROY3,                        TRUE,  FALSE,      }, // EM_SETPASSWORDCHAR
    { IMSG_DWORD,                             FALSE, FALSE,      }, // EM_EMPTYUNDOBUFFER
    { IMSG_DWORD,                             FALSE, FALSE,      }, // EM_GETFIRSTVISIBLELINE
    { IMSG_DWORD,                             FALSE, FALSE,      }, // EM_SETREADONLY
    { IMSG_DWORD,                             FALSE, FALSE,      }, // EM_SETWORDBREAKPROC
    { IMSG_DWORD,                             FALSE, FALSE,      }, // EM_GETWORDBREAKPROC
    { IMSG_DWORD,                             FALSE, FALSE,      }, // EM_GETPASSWORDCHAR
    { IMSG_DWORD,                             FALSE, FALSE,      }, // EM_SETMARGINS
    { IMSG_DWORD,                             FALSE, FALSE,      }, // EM_GETMARGINS
    { IMSG_DWORD,                             FALSE, FALSE,      }, // EM_GETLIMITTEXT
    { IMSG_DWORD,                             FALSE, FALSE,      }, // EM_POSFROMCHAR
    { IMSG_DWORD,                             FALSE, FALSE,      }, // EM_CHARFROMPOS
    { IMSG_DWORD,                             FALSE, FALSE,      }, // EM_SETIMESTATUS
    { IMSG_DWORD,                             FALSE, FALSE,      }, // EM_GETIMESTATUS
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x00DA
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x00DB
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x00DC
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x00DD
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x00DE
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x00DF
    { IMSG_DWORD,                             FALSE, FALSE,      }, // SBM_SETPOS
    { IMSG_DWORD,                             FALSE, FALSE,      }, // SBM_GETPOS
    { IMSG_DWORD,                             FALSE, FALSE,      }, // SBM_SETRANGE
    { IMSG_OPTOUTLPDWORDOPTOUTLPDWORD,        FALSE, TRUE,       }, // SBM_GETRANGE
    { IMSG_DWORD,                             FALSE, FALSE,      }, // SBM_ENABLE_ARROWS
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x00E5
    { IMSG_DWORD,                             FALSE, FALSE,      }, // SBM_SETRANGEREDRAW
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x00E7
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x00E8
    { IMSG_INOUTLPSCROLLINFO2,                FALSE, TRUE,       }, // SBM_SETSCROLLINFO
    { IMSG_INOUTLPSCROLLINFO2,                FALSE, TRUE,       }, // SBM_GETSCROLLINFO
    { IMSG_OUTLPSCROLLBARINFO,                FALSE, TRUE,       }, // SBM_GETSCROLLBARINFO
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x00EC
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x00ED
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x00EE
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x00EF
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x00F0
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x00F1
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x00F2
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x00F3
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x00F4
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x00F5
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x00F6
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x00F7
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x00F8
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x00F9
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x00FA
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x00FB
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x00FC
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x00FD
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_INPUT_DEVICE_CHANGE
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_INPUT
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_KEYDOWN
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_KEYUP
    { IMSG_NCDESTROY3,                        TRUE,  FALSE,      }, // WM_CHAR
    { IMSG_INWPARAMCHAR,                      TRUE,  FALSE,      }, // WM_DEADCHAR
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_SYSKEYDOWN
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_SYSKEYUP
    { IMSG_INWPARAMCHAR,                      TRUE,  FALSE,      }, // WM_SYSCHAR
    { IMSG_INWPARAMCHAR,                      TRUE,  FALSE,      }, // WM_SYSDEADCHAR
    { IMSG_DWORD,                             FALSE, TRUE,       }, // WM_KEYLAST
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_UNICHAR
    { IMSG_DWORD,                             FALSE, TRUE,       }, // 0x010A
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x010B
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x010C
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_IME_STARTCOMPOSITION
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_IME_ENDCOMPOSITION
    { IMSG_INWPARAMCHAR,                      TRUE,  FALSE,      }, // WM_IME_KEYLAST
    { IMSG_DWORD,                             FALSE, TRUE,       }, // WM_INITDIALOG
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_COMMAND
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_SYSCOMMAND
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_TIMER
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_HSCROLL
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_VSCROLL
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_INITMENU
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_INITMENUPOPUP
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0118
    { IMSG_GESTURE,                           FALSE, FALSE,      }, // WM_GESTURE
    { IMSG_INPGESTURENOTIFYSTRUCT,            FALSE, TRUE,       }, // WM_GESTURENOTIFY
    { IMSG_KERNELONLY,                        FALSE, FALSE,      }, // 0x011B
    { IMSG_INPGESTURENOTIFYSTRUCT,            FALSE, TRUE,       }, // 0x011C
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x011D
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x011E
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_MENUSELECT
    { IMSG_INWPARAMCHAR,                      TRUE,  FALSE,      }, // WM_MENUCHAR
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_ENTERIDLE
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_MENURBUTTONUP
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_MENUDRAG
    { IMSG_INOUTMENUGETOBJECT,                TRUE,  TRUE,       }, // WM_MENUGETOBJECT
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_UNINITMENUPOPUP
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_MENUCOMMAND
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_CHANGEUISTATE
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_UPDATEUISTATE
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_QUERYUISTATE
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x012A
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x012B
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x012C
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x012D
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x012E
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x012F
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0130
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0131
    { IMSG_DWORD,                             FALSE, TRUE,       }, // WM_CTLCOLORMSGBOX
    { IMSG_DWORD,                             FALSE, TRUE,       }, // WM_CTLCOLOREDIT
    { IMSG_DWORD,                             FALSE, TRUE,       }, // WM_CTLCOLORLISTBOX
    { IMSG_DWORD,                             FALSE, TRUE,       }, // WM_CTLCOLORBTN
    { IMSG_DWORD,                             FALSE, TRUE,       }, // WM_CTLCOLORDLG
    { IMSG_DWORD,                             FALSE, TRUE,       }, // WM_CTLCOLORSCROLLBAR
    { IMSG_DWORD,                             FALSE, TRUE,       }, // WM_CTLCOLORSTATIC
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0139
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x013A
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x013B
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x013C
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x013D
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x013E
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x013F
    { IMSG_OPTOUTLPDWORDOPTOUTLPDWORD3,       FALSE, TRUE,       }, // CB_GETEDITSEL
    { IMSG_DWORD,                             FALSE, FALSE,      }, // CB_LIMITTEXT
    { IMSG_DWORD,                             FALSE, FALSE,      }, // CB_SETEDITSEL
    { IMSG_INCBOXSTRING,                      TRUE,  TRUE,       }, // CB_ADDSTRING
    { IMSG_DWORD,                             FALSE, FALSE,      }, // CB_DELETESTRING
    { IMSG_INSTRING,                          TRUE,  FALSE,      }, // CB_DIR
    { IMSG_DWORD,                             FALSE, FALSE,      }, // CB_GETCOUNT
    { IMSG_DWORD,                             FALSE, FALSE,      }, // CB_GETCURSEL
    { IMSG_OUTCBOXSTRING,                     TRUE,  TRUE,       }, // CB_GETLBTEXT
    { IMSG_GETDBCSTEXTLENGTHS,                TRUE,  TRUE,       }, // CB_GETLBTEXTLEN
    { IMSG_INCBOXSTRING,                      TRUE,  TRUE,       }, // CB_INSERTSTRING
    { IMSG_DWORD,                             FALSE, FALSE,      }, // CB_RESETCONTENT
    { IMSG_INCBOXSTRING,                      TRUE,  TRUE,       }, // CB_FINDSTRING
    { IMSG_INCBOXSTRING,                      TRUE,  TRUE,       }, // CB_SELECTSTRING
    { IMSG_DWORD,                             FALSE, FALSE,      }, // CB_SETCURSEL
    { IMSG_DWORD,                             FALSE, FALSE,      }, // CB_SHOWDROPDOWN
    { IMSG_DWORD,                             FALSE, FALSE,      }, // CB_GETITEMDATA
    { IMSG_DWORD,                             FALSE, FALSE,      }, // CB_SETITEMDATA
    { IMSG_OUTLPRECT,                         FALSE, TRUE,       }, // CB_GETDROPPEDCONTROLRECT
    { IMSG_DWORD,                             FALSE, FALSE,      }, // CB_SETITEMHEIGHT
    { IMSG_DWORD,                             FALSE, FALSE,      }, // CB_GETITEMHEIGHT
    { IMSG_DWORD,                             FALSE, FALSE,      }, // CB_SETEXTENDEDUI
    { IMSG_DWORD,                             FALSE, FALSE,      }, // CB_GETEXTENDEDUI
    { IMSG_DWORD,                             FALSE, FALSE,      }, // CB_GETDROPPEDSTATE
    { IMSG_INCBOXSTRING,                      TRUE,  TRUE,       }, // CB_FINDSTRINGEXACT
    { IMSG_DWORD,                             FALSE, FALSE,      }, // CB_SETLOCALE
    { IMSG_DWORD,                             FALSE, FALSE,      }, // CB_GETLOCALE
    { IMSG_DWORD,                             FALSE, FALSE,      }, // CB_GETTOPINDEX
    { IMSG_DWORD,                             FALSE, FALSE,      }, // CB_SETTOPINDEX
    { IMSG_DWORD,                             FALSE, FALSE,      }, // CB_GETHORIZONTALEXTENT
    { IMSG_DWORD,                             FALSE, FALSE,      }, // CB_SETHORIZONTALEXTENT
    { IMSG_DWORD,                             FALSE, FALSE,      }, // CB_GETDROPPEDWIDTH
    { IMSG_DWORD,                             FALSE, FALSE,      }, // CB_SETDROPPEDWIDTH
    { IMSG_DWORD,                             FALSE, FALSE,      }, // CB_INITSTORAGE
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0162
    { IMSG_DWORD,                             FALSE, FALSE,      }, // CB_MULTIPLEADDSTRING
    { IMSG_OUTLPCOMBOBOXINFO,                 FALSE, TRUE,       }, // CB_GETCOMBOBOXINFO
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0165
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0166
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0167
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0168
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0169
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x016A
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x016B
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x016C
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x016D
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x016E
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x016F
    { IMSG_DWORD,                             FALSE, FALSE,      }, // STM_SETICON
    { IMSG_DWORD,                             FALSE, FALSE,      }, // STM_GETICON
    { IMSG_DWORD,                             FALSE, FALSE,      }, // STM_SETIMAGE
    { IMSG_DWORD,                             FALSE, FALSE,      }, // STM_GETIMAGE
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0174
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0175
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0176
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0177
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0178
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0179
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x017A
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x017B
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x017C
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x017D
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x017E
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x017F
    { IMSG_INLBOXSTRING,                      TRUE,  TRUE,       }, // LB_ADDSTRING
    { IMSG_INLBOXSTRING,                      TRUE,  TRUE,       }, // LB_INSERTSTRING
    { IMSG_DWORD,                             FALSE, FALSE,      }, // LB_DELETESTRING
    { IMSG_DWORD,                             FALSE, FALSE,      }, // LB_SELITEMRANGEEX
    { IMSG_DWORD,                             FALSE, FALSE,      }, // LB_RESETCONTENT
    { IMSG_DWORD,                             FALSE, FALSE,      }, // LB_SETSEL
    { IMSG_DWORD,                             FALSE, FALSE,      }, // LB_SETCURSEL
    { IMSG_DWORD,                             FALSE, FALSE,      }, // LB_GETSEL
    { IMSG_DWORD,                             FALSE, FALSE,      }, // LB_GETCURSEL
    { IMSG_OUTLBOXSTRING,                     TRUE,  TRUE,       }, // LB_GETTEXT
    { IMSG_GETDBCSTEXTLENGTHS,                TRUE,  TRUE,       }, // LB_GETTEXTLEN
    { IMSG_DWORD,                             FALSE, FALSE,      }, // LB_GETCOUNT
    { IMSG_INLBOXSTRING,                      TRUE,  TRUE,       }, // LB_SELECTSTRING
    { IMSG_INSTRING,                          TRUE,  FALSE,      }, // LB_DIR
    { IMSG_DWORD,                             FALSE, FALSE,      }, // LB_GETTOPINDEX
    { IMSG_INLBOXSTRING,                      TRUE,  TRUE,       }, // LB_FINDSTRING
    { IMSG_DWORD,                             FALSE, FALSE,      }, // LB_GETSELCOUNT
    { IMSG_POUTLPINT,                         FALSE, TRUE,       }, // LB_GETSELITEMS
    { IMSG_POPTINLPUINT,                      FALSE, TRUE,       }, // LB_SETTABSTOPS
    { IMSG_DWORD,                             FALSE, FALSE,      }, // LB_GETHORIZONTALEXTENT
    { IMSG_DWORD,                             FALSE, FALSE,      }, // LB_SETHORIZONTALEXTENT
    { IMSG_DWORD,                             FALSE, FALSE,      }, // LB_SETCOLUMNWIDTH
    { IMSG_INSTRING,                          TRUE,  TRUE,       }, // LB_ADDFILE
    { IMSG_DWORD,                             FALSE, FALSE,      }, // LB_SETTOPINDEX
    { IMSG_INOUTLPRECT,                       FALSE, TRUE,       }, // LB_GETITEMRECT
    { IMSG_DWORD,                             FALSE, FALSE,      }, // LB_GETITEMDATA
    { IMSG_DWORD,                             FALSE, FALSE,      }, // LB_SETITEMDATA
    { IMSG_DWORD,                             FALSE, FALSE,      }, // LB_SELITEMRANGE
    { IMSG_DWORD,                             FALSE, FALSE,      }, // LB_SETANCHORINDEX
    { IMSG_DWORD,                             FALSE, FALSE,      }, // LB_GETANCHORINDEX
    { IMSG_DWORD,                             FALSE, FALSE,      }, // LB_SETCARETINDEX
    { IMSG_DWORD,                             FALSE, FALSE,      }, // LB_GETCARETINDEX
    { IMSG_DWORD,                             FALSE, FALSE,      }, // LB_SETITEMHEIGHT
    { IMSG_DWORD,                             FALSE, FALSE,      }, // LB_GETITEMHEIGHT
    { IMSG_INLBOXSTRING,                      TRUE,  TRUE,       }, // LB_FINDSTRINGEXACT
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x01A3
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x01A4
    { IMSG_DWORD,                             FALSE, FALSE,      }, // LB_SETLOCALE
    { IMSG_DWORD,                             FALSE, FALSE,      }, // LB_GETLOCALE
    { IMSG_DWORD,                             FALSE, FALSE,      }, // LB_SETCOUNT
    { IMSG_DWORD,                             FALSE, FALSE,      }, // LB_INITSTORAGE
    { IMSG_DWORD,                             FALSE, FALSE,      }, // LB_ITEMFROMPOINT
    { IMSG_INLBOXSTRING,                      TRUE,  TRUE,       }, // 0x01AA
    { IMSG_INLBOXSTRING,                      TRUE,  TRUE,       }, // 0x01AB
    { IMSG_INLBOXSTRING,                      TRUE,  TRUE,       }, // 0x01AC
    { IMSG_INLBOXSTRING,                      TRUE,  TRUE,       }, // 0x01AD
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x01AE
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x01AF
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x01B0
    { IMSG_DWORD,                             FALSE, FALSE,      }, // LB_MULTIPLEADDSTRING
    { IMSG_DWORD,                             FALSE, TRUE,       }, // LB_GETLISTBOXINFO
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x01B3
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x01B4
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x01B5
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x01B6
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x01B7
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x01B8
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x01B9
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x01BA
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x01BB
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x01BC
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x01BD
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x01BE
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x01BF
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x01C0
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x01C1
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x01C2
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x01C3
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x01C4
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x01C5
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x01C6
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x01C7
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x01C8
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x01C9
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x01CA
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x01CB
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x01CC
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x01CD
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x01CE
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x01CF
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x01D0
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x01D1
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x01D2
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x01D3
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x01D4
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x01D5
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x01D6
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x01D7
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x01D8
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x01D9
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x01DA
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x01DB
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x01DC
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x01DD
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x01DE
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x01DF
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x01E0
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x01E1
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x01E2
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x01E3
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x01E4
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x01E5
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x01E6
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x01E7
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x01E8
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x01E9
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x01EA
    { IMSG_OUTDWORDINDWORD,                   FALSE, TRUE,       }, // 0x01EB
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x01EC
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x01ED
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x01EE
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x01EF
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x01F0
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x01F1
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x01F2
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x01F3
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x01F4
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x01F5
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x01F6
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x01F7
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x01F8
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x01F9
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x01FA
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x01FB
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x01FC
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x01FD
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x01FE
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x01FF
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_MOUSEMOVE
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_LBUTTONDOWN
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_LBUTTONUP
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_LBUTTONDBLCLK
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_RBUTTONDOWN
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_RBUTTONUP
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_RBUTTONDBLCLK
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_MBUTTONDOWN
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_MBUTTONUP
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_MOUSELAST
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_MOUSEWHEEL
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_XBUTTONDOWN
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_XBUTTONUP
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_XBUTTONDBLCLK
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_MOUSEHWHEEL
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x020F
    { IMSG_DWORD,                             FALSE, TRUE,       }, // WM_PARENTNOTIFY
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_ENTERMENULOOP
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_EXITMENULOOP
    { IMSG_INOUTNEXTMENU,                     FALSE, TRUE,       }, // WM_NEXTMENU
    { IMSG_INOUTLPRECT,                       FALSE, TRUE,       }, // WM_SIZING
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_CAPTURECHANGED
    { IMSG_INOUTLPRECT,                       FALSE, TRUE,       }, // WM_MOVING
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0217
    { IMSG_POWERBROADCAST,                    FALSE, FALSE,      }, // WM_POWERBROADCAST
    { IMSG_INDEVICECHANGE,                    FALSE, FALSE,      }, // WM_DEVICECHANGE
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x021A
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x021B
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x021C
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x021D
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x021E
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x021F
    { IMSG_INLPMDICREATESTRUCT,               TRUE,  TRUE,       }, // WM_MDICREATE
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_MDIDESTROY
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_MDIACTIVATE
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_MDIRESTORE
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_MDINEXT
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_MDIMAXIMIZE
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_MDITILE
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_MDICASCADE
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_MDIICONARRANGE
    { IMSG_OPTOUTLPDWORDOPTOUTLPDWORD,        FALSE, TRUE,       }, // WM_MDIGETACTIVE
    { IMSG_INOUTLPSCROLLINFO3,                FALSE, TRUE,       }, // 0x022A
    { IMSG_INOUTLPSCROLLINFO3,                FALSE, TRUE,       }, // 0x022B
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x022C
    { IMSG_INOUTLPSCROLLINFO3,                FALSE, TRUE,       }, // 0x022D
    { IMSG_INOUTLPSCROLLINFO3,                FALSE, TRUE,       }, // 0x022E
    { IMSG_INOUTLPSCROLLINFO3,                FALSE, TRUE,       }, // 0x022F
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_MDISETMENU
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_ENTERSIZEMOVE
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_EXITSIZEMOVE
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_DROPFILES
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_MDIREFRESHMENU
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0235
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0236
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0237
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0238
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0239
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x023A
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x023B
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x023C
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x023D
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x023E
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x023F
    { IMSG_TOUCH,                             TRUE,  FALSE,      }, // WM_TOUCH
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0241
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0242
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0243
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0244
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0245
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0246
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0247
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0248
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0249
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x024A
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x024B
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x024C
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x024D
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x024E
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x024F
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0250
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0251
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0252
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0253
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0254
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0255
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0256
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0257
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0258
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0259
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x025A
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x025B
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x025C
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x025D
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x025E
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x025F
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0260
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0261
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0262
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0263
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0264
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0265
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0266
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0267
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0268
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0269
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x026A
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x026B
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x026C
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x026D
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x026E
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x026F
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0270
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0271
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0272
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0273
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0274
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0275
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0276
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0277
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0278
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0279
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x027A
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x027B
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x027C
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x027D
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x027E
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x027F
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0280
    { IMSG_DWORD,                             FALSE, TRUE,       }, // WM_IME_SETCONTEXT
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_IME_NOTIFY
    { IMSG_IMECONTROL,                        TRUE,  TRUE,       }, // WM_IME_CONTROL
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_IME_COMPOSITIONFULL
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_IME_SELECT
    { IMSG_INWPARAMCHAR,                      TRUE,  FALSE,      }, // WM_IME_CHAR
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0287
    { IMSG_DWORD,                             FALSE, TRUE,       }, // WM_IME_REQUEST
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0289
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x028A
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x028B
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x028C
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x028D
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x028E
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x028F
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_IME_KEYDOWN
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_IME_KEYUP
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0292
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0293
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0294
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0295
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0296
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0297
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0298
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0299
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x029A
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x029B
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x029C
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x029D
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x029E
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x029F
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_NCMOUSEHOVER
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_MOUSEHOVER
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_NCMOUSELEAVE
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_MOUSELEAVE
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x02A4
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x02A5
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x02A6
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x02A7
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x02A8
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x02A9
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x02AA
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x02AB
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x02AC
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x02AD
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x02AE
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x02AF
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x02B0
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_WTSSESSION_CHANGE
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x02B2
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x02B3
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x02B4
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x02B5
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x02B6
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x02B7
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x02B8
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x02B9
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x02BA
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x02BB
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x02BC
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x02BD
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x02BE
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x02BF
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_TABLET_FIRST
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x02C1
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x02C2
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x02C3
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x02C4
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x02C5
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x02C6
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x02C7
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x02C8
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x02C9
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x02CA
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x02CB
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x02CC
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x02CD
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x02CE
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x02CF
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x02D0
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x02D1
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x02D2
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x02D3
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x02D4
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x02D5
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x02D6
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x02D7
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x02D8
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x02D9
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x02DA
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x02DB
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x02DC
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x02DD
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x02DE
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_TABLET_LAST
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x02E0
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x02E1
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x02E2
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x02E3
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x02E4
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x02E5
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x02E6
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x02E7
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x02E8
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x02E9
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x02EA
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x02EB
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x02EC
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x02ED
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x02EE
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x02EF
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x02F0
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x02F1
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x02F2
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x02F3
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x02F4
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x02F5
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x02F6
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x02F7
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x02F8
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x02F9
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x02FA
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x02FB
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x02FC
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x02FD
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x02FE
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x02FF
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_CUT
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_COPY
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_PASTE
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_CLEAR
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_UNDO
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_RENDERFORMAT
    { IMSG_NCDESTROY,                         TRUE,  FALSE,      }, // WM_RENDERALLFORMATS
    { IMSG_NCDESTROY,                         TRUE,  FALSE,      }, // WM_DESTROYCLIPBOARD
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_DRAWCLIPBOARD
    { IMSG_INPAINTCLIPBRD,                    TRUE,  TRUE,       }, // WM_PAINTCLIPBOARD
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_VSCROLLCLIPBOARD
    { IMSG_INSIZECLIPBRD,                     TRUE,  TRUE,       }, // WM_SIZECLIPBOARD
    { IMSG_INCNTOUTSTRINGNULL,                TRUE,  TRUE,       }, // WM_ASKCBFORMATNAME
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_CHANGECBCHAIN
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_HSCROLLCLIPBOARD
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_QUERYNEWPALETTE
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_PALETTEISCHANGING
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_PALETTECHANGED
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_HOTKEY
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0313
    { IMSG_KERNELONLY,                        FALSE, TRUE,       }, // 0x0314
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0315
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0316
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_PRINT
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_PRINTCLIENT
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_APPCOMMAND
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_THEMECHANGED
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x031B
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x031C
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_CLIPBOARDUPDATE
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_DWMCOMPOSITIONCHANGED
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_DWMNCRENDERINGCHANGED
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_DWMCOLORIZATIONCOLORCHANGED
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_DWMWINDOWMAXIMIZEDCHANGE
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0322
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_DWMSENDICONICTHUMBNAIL
    { IMSG_KERNELONLY,                        FALSE, FALSE,      }, // 0x0324
    { IMSG_KERNELONLY,                        FALSE, FALSE,      }, // 0x0325
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_DWMSENDICONICLIVEPREVIEWBITMAP
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0327
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0328
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0329
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x032A
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x032B
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x032C
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x032D
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x032E
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x032F
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0330
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0331
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0332
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0333
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0334
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0335
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0336
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0337
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0338
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0339
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x033A
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x033B
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x033C
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x033D
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x033E
    { IMSG_OUTLPTITLEBARINFOEX,               FALSE, TRUE,       }, // WM_GETTITLEBARINFOEX
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0340
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0341
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0342
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0343
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0344
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0345
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0346
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0347
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0348
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0349
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x034A
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x034B
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x034C
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x034D
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x034E
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x034F
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0350
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0351
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0352
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0353
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0354
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0355
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0356
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0357
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_HANDHELDFIRST
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0359
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x035A
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x035B
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x035C
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x035D
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x035E
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_HANDHELDLAST
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_AFXFIRST
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0361
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0362
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0363
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0364
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0365
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0366
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0367
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0368
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0369
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x036A
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x036B
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x036C
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x036D
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x036E
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x036F
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0370
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0371
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0372
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0373
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0374
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0375
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0376
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0377
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0378
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0379
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x037A
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x037B
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x037C
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x037D
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x037E
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_AFXLAST
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_PENWINFIRST
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0381
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0382
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0383
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0384
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0385
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0386
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0387
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0388
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0389
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x038A
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x038B
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x038C
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x038D
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x038E
    { IMSG_DWORD,                             FALSE, FALSE,      }, // WM_PENWINLAST
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0390
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0391
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0392
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0393
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0394
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0395
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0396
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0397
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0398
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x0399
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x039A
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x039B
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x039C
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x039D
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x039E
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x039F
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x03A0
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x03A1
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x03A2
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x03A3
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x03A4
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x03A5
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x03A6
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x03A7
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x03A8
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x03A9
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x03AA
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x03AB
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x03AC
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x03AD
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x03AE
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x03AF
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x03B0
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x03B1
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x03B2
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x03B3
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x03B4
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x03B5
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x03B6
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x03B7
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x03B8
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x03B9
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x03BA
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x03BB
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x03BC
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x03BD
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x03BE
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x03BF
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x03C0
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x03C1
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x03C2
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x03C3
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x03C4
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x03C5
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x03C6
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x03C7
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x03C8
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x03C9
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x03CA
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x03CB
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x03CC
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x03CD
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x03CE
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x03CF
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x03D0
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x03D1
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x03D2
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x03D3
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x03D4
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x03D5
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x03D6
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x03D7
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x03D8
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x03D9
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x03DA
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x03DB
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x03DC
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x03DD
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x03DE
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x03DF
    { IMSG_DDEINIT,                           TRUE,  FALSE,      }, // 0x03E0
    { IMSG_DWORD,                             TRUE,  FALSE,      }, // 0x03E1
    { IMSG_SENTDDEMSG,                        TRUE,  FALSE,      }, // 0x03E2
    { IMSG_SENTDDEMSG,                        TRUE,  FALSE,      }, // 0x03E3
    { IMSG_DWORD,                             TRUE,  FALSE,      }, // 0x03E4
    { IMSG_SENTDDEMSG,                        TRUE,  FALSE,      }, // 0x03E5
    { IMSG_SENTDDEMSG,                        TRUE,  FALSE,      }, // 0x03E6
    { IMSG_SENTDDEMSG,                        TRUE,  FALSE,      }, // 0x03E7
    { IMSG_SENTDDEMSG,                        TRUE,  FALSE,      }, // 0x03E8
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x03E9
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x03EA
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x03EB
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x03EC
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x03ED
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x03EE
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x03EF
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x03F0
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x03F1
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x03F2
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x03F3
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x03F4
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x03F5
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x03F6
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x03F7
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x03F8
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x03F9
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x03FA
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x03FB
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x03FC
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x03FD
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x03FE
    { IMSG_DWORD,                             FALSE, FALSE,      }, // 0x03FF
};


typedef
LRESULT
(NTAPI
*FNMESSAGECALL)(
    HWND        Window,
    UINT        Message,
    WPARAM      wParam,
    LPARAM      lParam,
    ULONG_PTR   xParam,
    ULONG       xpfnProc,
    ULONG       Flags
);

typedef
LRESULT
(NTAPI
*FNUSERMESSAGECALL)(
    WNDPROC     PrevProc,
    HWND        Window,
    UINT        Message,
    WPARAM      wParam,
    LPARAM      lParam
);

typedef struct
{
    FNMESSAGECALL       KernelCall;
    FNUSERMESSAGECALL   UserCall;

} LE_MESSAGE_FUNCTION_TABLE;

#define KernelMessageCall(_Method) LRESULT NTAPI NtUserfn##_Method (HWND Window, UINT Message, WPARAM wParam, LPARAM lParam, ULONG_PTR xParam, ULONG xpfnProc, ULONG Flags)
#define UserMessageCall(_Method) LRESULT NTAPI Userfn##_Method (WNDPROC PrevProc, HWND Window, UINT Message, WPARAM wParam, LPARAM lParam)

#define HDLMSG(_msg) KernelMessageCall(_msg); UserMessageCall(_msg)

#define CALLNTMSGCALL(_msgcall) (_msgcall)(Window, Message, wParam, lParam, xParam, xpfnProc, Flags)

#define GETMSGCALL(_fn) IF_EXIST(NtUserfn##_fn) { { NtUserfn##_fn, Userfn##_fn } } IF_NOT_EXIST(NtUserfn##_fn) { { NtUserfnEMPTY, UserfnEMPTY } }


#define WS_EX_ANSI          0x80000000
#define WINDOW_FLAG_ANSI    0x00000001

#define CALLNTUSERMSGA_(_fn) (_fn)(Window, Message, wParam, lParam, xParam, xpfnProc, Flags | WINDOW_FLAG_ANSI)
#define CALLNTUSERMSGW_(_fn) (_fn)(Window, Message, wParam, lParam, xParam, xpfnProc, Flags & ~WINDOW_FLAG_ANSI)
#define CALLNTUSERMSG_(_fn) (_fn)(Window, Message, wParam, lParam, xParam, xpfnProc, Flags)

#define CallNtUserMessageCallA() CALLNTUSERMSGA_(LeGetGlobalData()->NtUserMessageCall)
#define CallNtUserMessageCallW() CALLNTUSERMSGW_(LeGetGlobalData()->NtUserMessageCall)
#define CallNtUserMessageCall() CALLNTUSERMSG_(LeGetGlobalData()->NtUserMessageCall)
#define CallNtUserLocalCall(_msgid) CALLNTUSERMSG_(NtUserfn##_msgid)

#define CallUserMessageCallA() (CallWindowProcA(PrevProc, Window, Message, wParam, lParam))
#define CallUserMessageCallW() (CallWindowProcW(PrevProc, Window, Message, wParam, lParam))
#define CallUserLocalCall(_msgid) (Userfn##_msgid)(PrevProc, Window, Message, wParam, lParam)

HDLMSG(EMPTY);
HDLMSG(INLPCREATESTRUCT);
HDLMSG(INLPMDICREATESTRUCT);
HDLMSG(INSTRINGNULL);
HDLMSG(OUTSTRING);
HDLMSG(INSTRING);
HDLMSG(INCNTOUTSTRING);
HDLMSG(INCBOXSTRING);
HDLMSG(OUTCBOXSTRING);
HDLMSG(INLBOXSTRING);
HDLMSG(OUTLBOXSTRING);
HDLMSG(INCNTOUTSTRINGNULL);
HDLMSG(GETDBCSTEXTLENGTHS);


LE_MESSAGE_FUNCTION_TABLE gapfnMessageCall[] =
{
    GETMSGCALL(NCDESTROY),
    GETMSGCALL(NCDESTROY),
    GETMSGCALL(INLPCREATESTRUCT),
    GETMSGCALL(INSTRINGNULL),
    GETMSGCALL(OUTSTRING),
    GETMSGCALL(INSTRING),
    GETMSGCALL(INOUTLPPOINT5),
    GETMSGCALL(INLPDRAWITEMSTRUCT),
    GETMSGCALL(INOUTLPMEASUREITEMSTRUCT),
    GETMSGCALL(INLPDELETEITEMSTRUCT),
    GETMSGCALL(INWPARAMCHAR),
    GETMSGCALL(INLPHLPSTRUCT),
    GETMSGCALL(INLPCOMPAREITEMSTRUCT),
    GETMSGCALL(INOUTLPSCROLLINFO),
    GETMSGCALL(INLPWINDOWPOS),
    GETMSGCALL(COPYGLOBALDATA),
    GETMSGCALL(COPYDATA),
    GETMSGCALL(INLPHELPINFOSTRUCT),
    GETMSGCALL(INOUTSTYLECHANGE),
    GETMSGCALL(INOUTNCCALCSIZE),
    GETMSGCALL(DWORDOPTINLPMSG),
    GETMSGCALL(INOUTLPRECT),
    GETMSGCALL(OPTOUTLPDWORDOPTOUTLPDWORD),
    GETMSGCALL(OUTLPRECT),
    GETMSGCALL(INCNTOUTSTRING),
    GETMSGCALL(POPTINLPUINT),
    GETMSGCALL(INOUTLPSCROLLINFO),
    GETMSGCALL(INCBOXSTRING),
    GETMSGCALL(OUTCBOXSTRING),
    GETMSGCALL(INLBOXSTRING),
    GETMSGCALL(OUTLBOXSTRING),
    GETMSGCALL(POUTLPINT),
    GETMSGCALL(OUTDWORDINDWORD),
    GETMSGCALL(INOUTNEXTMENU),
    GETMSGCALL(INDEVICECHANGE),
    GETMSGCALL(INLPMDICREATESTRUCT),
    GETMSGCALL(INOUTLPSCROLLINFO),
    GETMSGCALL(NCDESTROY),
    GETMSGCALL(INPAINTCLIPBRD),
    GETMSGCALL(INSIZECLIPBRD),
    GETMSGCALL(INCNTOUTSTRINGNULL),
    GETMSGCALL(KERNELONLY),
    GETMSGCALL(DDEINIT),
    GETMSGCALL(SENTDDEMSG),
    GETMSGCALL(GETDBCSTEXTLENGTHS),
    GETMSGCALL(OPTOUTLPDWORDOPTOUTLPDWORD),
    GETMSGCALL(NCDESTROY),
    GETMSGCALL(NCDESTROY),
    GETMSGCALL(OPTOUTLPDWORDOPTOUTLPDWORD),
    GETMSGCALL(IMECONTROL),
    GETMSGCALL(INOUTMENUGETOBJECT),
    GETMSGCALL(POWERBROADCAST),
    GETMSGCALL(KERNELONLY),
    GETMSGCALL(OUTLPCOMBOBOXINFO),
    GETMSGCALL(OUTLPSCROLLBARINFO),
    GETMSGCALL(INLPUAHINITMENU),
    GETMSGCALL(INLPUAHDRAWMENUITEM),
    GETMSGCALL(INLPUAHINITMENU),
    GETMSGCALL(INOUTLPUAHMEASUREMENUITEM),
    GETMSGCALL(INLPUAHINITMENU),
    GETMSGCALL(OUTLPTITLEBARINFOEX),
    GETMSGCALL(TOUCH),
    GETMSGCALL(GESTURE),
    GETMSGCALL(INPGESTURENOTIFYSTRUCT),
    GETMSGCALL(PARENTNOTIFY),
    GETMSGCALL(POINTERINPUT),
    GETMSGCALL(TOUCHHITTESTING),
    GETMSGCALL(EMPTY),
};

#endif // _MESSAGETABLE_H_4a84968c_ce14_49b6_99a3_353e9db99f11_
