import os, sys

MessageTable = \
[
    0x00, 0xc2, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc3, 0xc4, 0xec, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc3, 0xc5, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x86, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x87, 0x88, 0x89, 0x00, 0x4a,
    0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x8b, 0x8c, 0x29, 0x00, 0xa9, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x8d, 0x8e, 0x00, 0xcf, 0x90, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x91, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xa9, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x92, 0x92, 0x00, 0x00,
    0x00, 0xc2, 0x01, 0x93, 0x00, 0x80, 0x00, 0x94, 0x00, 0x00, 0x00, 0x95, 0xb4, 0x00, 0x00, 0x00,
    0x80, 0xb7, 0xb8, 0xb9, 0xba, 0xbb, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xad, 0x2e, 0x97, 0x95, 0x95, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0xc3, 0x00, 0xd8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x99, 0x6f, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x96, 0x00, 0x00, 0x00, 0x00, 0x00, 0x9a, 0x9a, 0xb6, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x6f, 0x4a, 0x00, 0x00, 0x4a, 0x4a, 0x80, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x4a,
    0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3e, 0xbf, 0x29, 0xbf, 0x00, 0x00, 0x00,
    0x4a, 0x00, 0x00, 0x00, 0xf2, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xb0, 0x00, 0x00, 0xdb, 0x00, 0x45, 0x00, 0x00, 0xdc, 0xec, 0xdb, 0x00, 0xdb, 0xdb, 0x00, 0x00,
    0x00, 0x00, 0x97, 0x00, 0x00, 0x00, 0x00, 0x00, 0xdb, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0xb5, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xdd, 0xdd, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xde, 0xec, 0x00, 0xdd, 0x45, 0x00, 0xdd,
    0x00, 0x9f, 0x99, 0x00, 0x00, 0x00, 0xc5, 0x00, 0x95, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0xdd, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xdd, 0xdd, 0xdd, 0xdd, 0x00, 0x00,
    0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa0, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x80, 0x00, 0x00, 0xa1, 0x95, 0x00, 0x95, 0x00, 0x33, 0x22, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xe3, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x96, 0xa4, 0xa4, 0x00, 0xa4, 0xa4, 0xa4,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x7d, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x80, 0x00, 0xf1, 0x00, 0x00, 0x4a, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x65, 0x65, 0x00, 0xe6, 0x00, 0xe7, 0xe8, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0xa9, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x29, 0x29, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xbc,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x6a, 0x40, 0x6b, 0x6b, 0x40, 0x6b, 0x6b, 0x6b, 0x6b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
]

MessageFunctionIndex = []

MessageFunction = \
[
    'DWORD',
    'DWORD2',
    'INLPCREATESTRUCT',
    'INSTRINGNULL',
    'OUTSTRING',
    'INSTRING',
    'INOUTLPPOINT5',
    'INLPDRAWITEMSTRUCT',
    'INOUTLPMEASUREITEMSTRUCT',
    'INLPDELETEITEMSTRUCT',
    'INWPARAMCHAR',
    'INLPHLPSTRUCT',
    'INLPCOMPAREITEMSTRUCT',
    'INOUTLPSCROLLINFO',
    'INLPWINDOWPOS',
    'COPYGLOBALDATA',
    'COPYDATA',
    'INLPHELPINFOSTRUCT',
    'INOUTSTYLECHANGE',
    'INOUTNCCALCSIZE',
    'DWORDOPTINLPMSG',
    'INOUTLPRECT',
    'OPTOUTLPDWORDOPTOUTLPDWORD',
    'OUTLPRECT',
    'INCNTOUTSTRING',
    'POPTINLPUINT',
    'INOUTLPSCROLLINFO2',
    'INCBOXSTRING',
    'OUTCBOXSTRING',
    'INLBOXSTRING',
    'OUTLBOXSTRING',
    'POUTLPINT',
    'OUTDWORDINDWORD',
    'INOUTNEXTMENU',
    'INDEVICECHANGE',
    'INLPMDICREATESTRUCT',
    'INOUTLPSCROLLINFO3',
    'NCDESTROY',
    'INPAINTCLIPBRD',
    'INSIZECLIPBRD',
    'INCNTOUTSTRINGNULL',
    'KERNELONLY',
    'DDEINIT',
    'SENTDDEMSG',
    'GETDBCSTEXTLENGTHS',
    'OPTOUTLPDWORDOPTOUTLPDWORD2',
    'NCDESTROY2',
    'NCDESTROY3',
    'OPTOUTLPDWORDOPTOUTLPDWORD3',
    'IMECONTROL',
    'INOUTMENUGETOBJECT',
    'POWERBROADCAST',
    'KERNELONLY2',
    'OUTLPCOMBOBOXINFO',
    'OUTLPSCROLLBARINFO',
    'INLPUAHINITMENU',
    'INLPUAHDRAWMENUITEM',
    'INLPUAHINITMENU2',
    'INOUTLPUAHMEASUREMENUITEM',
    'INLPUAHINITMENU3',
    'OUTLPTITLEBARINFOEX',
    'TOUCH',
    'GESTURE',
    'INPGESTURENOTIFYSTRUCT',
]

WM_NULL                                 = 0x0000
WM_CREATE                               = 0x0001
WM_DESTROY                              = 0x0002
WM_MOVE                                 = 0x0003
WM_SIZE                                 = 0x0005
WM_ACTIVATE                             = 0x0006
WM_SETFOCUS                             = 0x0007
WM_KILLFOCUS                            = 0x0008
WM_ENABLE                               = 0x000A
WM_SETREDRAW                            = 0x000B
WM_SETTEXT                              = 0x000C
WM_GETTEXT                              = 0x000D
WM_GETTEXTLENGTH                        = 0x000E
WM_PAINT                                = 0x000F
WM_CLOSE                                = 0x0010
WM_QUERYENDSESSION                      = 0x0011
WM_QUERYOPEN                            = 0x0013
WM_ENDSESSION                           = 0x0016
WM_QUIT                                 = 0x0012
WM_ERASEBKGND                           = 0x0014
WM_SYSCOLORCHANGE                       = 0x0015
WM_SHOWWINDOW                           = 0x0018
WM_WININICHANGE                         = 0x001A
WM_SETTINGCHANGE                        = WM_WININICHANGE
WM_DEVMODECHANGE                        = 0x001B
WM_ACTIVATEAPP                          = 0x001C
WM_FONTCHANGE                           = 0x001D
WM_TIMECHANGE                           = 0x001E
WM_CANCELMODE                           = 0x001F
WM_SETCURSOR                            = 0x0020
WM_MOUSEACTIVATE                        = 0x0021
WM_CHILDACTIVATE                        = 0x0022
WM_QUEUESYNC                            = 0x0023
WM_GETMINMAXINFO                        = 0x0024
WM_PAINTICON                            = 0x0026
WM_ICONERASEBKGND                       = 0x0027
WM_NEXTDLGCTL                           = 0x0028
WM_SPOOLERSTATUS                        = 0x002A
WM_DRAWITEM                             = 0x002B
WM_MEASUREITEM                          = 0x002C
WM_DELETEITEM                           = 0x002D
WM_VKEYTOITEM                           = 0x002E
WM_CHARTOITEM                           = 0x002F
WM_SETFONT                              = 0x0030
WM_GETFONT                              = 0x0031
WM_SETHOTKEY                            = 0x0032
WM_GETHOTKEY                            = 0x0033
WM_QUERYDRAGICON                        = 0x0037
WM_COMPAREITEM                          = 0x0039
WM_GETOBJECT                            = 0x003D
WM_COMPACTING                           = 0x0041
WM_COMMNOTIFY                           = 0x0044
WM_WINDOWPOSCHANGING                    = 0x0046
WM_WINDOWPOSCHANGED                     = 0x0047
WM_POWER                                = 0x0048
WM_COPYDATA                             = 0x004A
WM_CANCELJOURNAL                        = 0x004B
WM_NOTIFY                               = 0x004E
WM_INPUTLANGCHANGEREQUEST               = 0x0050
WM_INPUTLANGCHANGE                      = 0x0051
WM_TCARD                                = 0x0052
WM_HELP                                 = 0x0053
WM_USERCHANGED                          = 0x0054
WM_NOTIFYFORMAT                         = 0x0055
WM_CONTEXTMENU                          = 0x007B
WM_STYLECHANGING                        = 0x007C
WM_STYLECHANGED                         = 0x007D
WM_DISPLAYCHANGE                        = 0x007E
WM_GETICON                              = 0x007F
WM_SETICON                              = 0x0080
WM_NCCREATE                             = 0x0081
WM_NCDESTROY                            = 0x0082
WM_NCCALCSIZE                           = 0x0083
WM_NCHITTEST                            = 0x0084
WM_NCPAINT                              = 0x0085
WM_NCACTIVATE                           = 0x0086
WM_GETDLGCODE                           = 0x0087
WM_SYNCPAINT                            = 0x0088
WM_NCMOUSEMOVE                          = 0x00A0
WM_NCLBUTTONDOWN                        = 0x00A1
WM_NCLBUTTONUP                          = 0x00A2
WM_NCLBUTTONDBLCLK                      = 0x00A3
WM_NCRBUTTONDOWN                        = 0x00A4
WM_NCRBUTTONUP                          = 0x00A5
WM_NCRBUTTONDBLCLK                      = 0x00A6
WM_NCMBUTTONDOWN                        = 0x00A7
WM_NCMBUTTONUP                          = 0x00A8
WM_NCMBUTTONDBLCLK                      = 0x00A9
WM_NCXBUTTONDOWN                        = 0x00AB
WM_NCXBUTTONUP                          = 0x00AC
WM_NCXBUTTONDBLCLK                      = 0x00AD
WM_INPUT_DEVICE_CHANGE                  = 0x00FE
WM_INPUT                                = 0x00FF
WM_KEYFIRST                             = 0x0100
WM_KEYDOWN                              = 0x0100
WM_KEYUP                                = 0x0101
WM_CHAR                                 = 0x0102
WM_DEADCHAR                             = 0x0103
WM_SYSKEYDOWN                           = 0x0104
WM_SYSKEYUP                             = 0x0105
WM_SYSCHAR                              = 0x0106
WM_SYSDEADCHAR                          = 0x0107
WM_UNICHAR                              = 0x0109
WM_KEYLAST                              = 0x0109
WM_KEYLAST                              = 0x0108
WM_IME_STARTCOMPOSITION                 = 0x010D
WM_IME_ENDCOMPOSITION                   = 0x010E
WM_IME_COMPOSITION                      = 0x010F
WM_IME_KEYLAST                          = 0x010F
WM_INITDIALOG                           = 0x0110
WM_COMMAND                              = 0x0111
WM_SYSCOMMAND                           = 0x0112
WM_TIMER                                = 0x0113
WM_HSCROLL                              = 0x0114
WM_VSCROLL                              = 0x0115
WM_INITMENU                             = 0x0116
WM_INITMENUPOPUP                        = 0x0117
WM_GESTURE                              = 0x0119
WM_GESTURENOTIFY                        = 0x011A
WM_MENUSELECT                           = 0x011F
WM_MENUCHAR                             = 0x0120
WM_ENTERIDLE                            = 0x0121
WM_MENURBUTTONUP                        = 0x0122
WM_MENUDRAG                             = 0x0123
WM_MENUGETOBJECT                        = 0x0124
WM_UNINITMENUPOPUP                      = 0x0125
WM_MENUCOMMAND                          = 0x0126
WM_CHANGEUISTATE                        = 0x0127
WM_UPDATEUISTATE                        = 0x0128
WM_QUERYUISTATE                         = 0x0129
WM_CTLCOLORMSGBOX                       = 0x0132
WM_CTLCOLOREDIT                         = 0x0133
WM_CTLCOLORLISTBOX                      = 0x0134
WM_CTLCOLORBTN                          = 0x0135
WM_CTLCOLORDLG                          = 0x0136
WM_CTLCOLORSCROLLBAR                    = 0x0137
WM_CTLCOLORSTATIC                       = 0x0138
WM_MOUSEFIRST                           = 0x0200
WM_MOUSEMOVE                            = 0x0200
WM_LBUTTONDOWN                          = 0x0201
WM_LBUTTONUP                            = 0x0202
WM_LBUTTONDBLCLK                        = 0x0203
WM_RBUTTONDOWN                          = 0x0204
WM_RBUTTONUP                            = 0x0205
WM_RBUTTONDBLCLK                        = 0x0206
WM_MBUTTONDOWN                          = 0x0207
WM_MBUTTONUP                            = 0x0208
WM_MBUTTONDBLCLK                        = 0x0209
WM_MOUSEWHEEL                           = 0x020A
WM_XBUTTONDOWN                          = 0x020B
WM_XBUTTONUP                            = 0x020C
WM_XBUTTONDBLCLK                        = 0x020D
WM_MOUSEHWHEEL                          = 0x020E
WM_MOUSELAST                            = 0x020E
WM_MOUSELAST                            = 0x020D
WM_MOUSELAST                            = 0x020A
WM_MOUSELAST                            = 0x0209
WM_PARENTNOTIFY                         = 0x0210
WM_ENTERMENULOOP                        = 0x0211
WM_EXITMENULOOP                         = 0x0212
WM_NEXTMENU                             = 0x0213
WM_SIZING                               = 0x0214
WM_CAPTURECHANGED                       = 0x0215
WM_MOVING                               = 0x0216
WM_POWERBROADCAST                       = 0x0218
WM_DEVICECHANGE                         = 0x0219
WM_MDICREATE                            = 0x0220
WM_MDIDESTROY                           = 0x0221
WM_MDIACTIVATE                          = 0x0222
WM_MDIRESTORE                           = 0x0223
WM_MDINEXT                              = 0x0224
WM_MDIMAXIMIZE                          = 0x0225
WM_MDITILE                              = 0x0226
WM_MDICASCADE                           = 0x0227
WM_MDIICONARRANGE                       = 0x0228
WM_MDIGETACTIVE                         = 0x0229
WM_MDISETMENU                           = 0x0230
WM_ENTERSIZEMOVE                        = 0x0231
WM_EXITSIZEMOVE                         = 0x0232
WM_DROPFILES                            = 0x0233
WM_MDIREFRESHMENU                       = 0x0234
WM_TOUCH                                = 0x0240
WM_IME_SETCONTEXT                       = 0x0281
WM_IME_NOTIFY                           = 0x0282
WM_IME_CONTROL                          = 0x0283
WM_IME_COMPOSITIONFULL                  = 0x0284
WM_IME_SELECT                           = 0x0285
WM_IME_CHAR                             = 0x0286
WM_IME_REQUEST                          = 0x0288
WM_IME_KEYDOWN                          = 0x0290
WM_IME_KEYUP                            = 0x0291
WM_MOUSEHOVER                           = 0x02A1
WM_MOUSELEAVE                           = 0x02A3
WM_NCMOUSEHOVER                         = 0x02A0
WM_NCMOUSELEAVE                         = 0x02A2
WM_WTSSESSION_CHANGE                    = 0x02B1
WM_TABLET_FIRST                         = 0x02c0
WM_TABLET_LAST                          = 0x02df
WM_CUT                                  = 0x0300
WM_COPY                                 = 0x0301
WM_PASTE                                = 0x0302
WM_CLEAR                                = 0x0303
WM_UNDO                                 = 0x0304
WM_RENDERFORMAT                         = 0x0305
WM_RENDERALLFORMATS                     = 0x0306
WM_DESTROYCLIPBOARD                     = 0x0307
WM_DRAWCLIPBOARD                        = 0x0308
WM_PAINTCLIPBOARD                       = 0x0309
WM_VSCROLLCLIPBOARD                     = 0x030A
WM_SIZECLIPBOARD                        = 0x030B
WM_ASKCBFORMATNAME                      = 0x030C
WM_CHANGECBCHAIN                        = 0x030D
WM_HSCROLLCLIPBOARD                     = 0x030E
WM_QUERYNEWPALETTE                      = 0x030F
WM_PALETTEISCHANGING                    = 0x0310
WM_PALETTECHANGED                       = 0x0311
WM_HOTKEY                               = 0x0312
WM_PRINT                                = 0x0317
WM_PRINTCLIENT                          = 0x0318
WM_APPCOMMAND                           = 0x0319
WM_THEMECHANGED                         = 0x031A
WM_CLIPBOARDUPDATE                      = 0x031D
WM_DWMCOMPOSITIONCHANGED                = 0x031E
WM_DWMNCRENDERINGCHANGED                = 0x031F
WM_DWMCOLORIZATIONCOLORCHANGED          = 0x0320
WM_DWMWINDOWMAXIMIZEDCHANGE             = 0x0321
WM_DWMSENDICONICTHUMBNAIL               = 0x0323
WM_DWMSENDICONICLIVEPREVIEWBITMAP       = 0x0326
WM_GETTITLEBARINFOEX                    = 0x033F
WM_HANDHELDFIRST                        = 0x0358
WM_HANDHELDLAST                         = 0x035F
WM_AFXFIRST                             = 0x0360
WM_AFXLAST                              = 0x037F
WM_PENWINFIRST                          = 0x0380
WM_PENWINLAST                           = 0x038F
WM_USER                                 = 0x0400

EM_GETSEL                               = 0x00B0
EM_SETSEL                               = 0x00B1
EM_GETRECT                              = 0x00B2
EM_SETRECT                              = 0x00B3
EM_SETRECTNP                            = 0x00B4
EM_SCROLL                               = 0x00B5
EM_LINESCROLL                           = 0x00B6
EM_SCROLLCARET                          = 0x00B7
EM_GETMODIFY                            = 0x00B8
EM_SETMODIFY                            = 0x00B9
EM_GETLINECOUNT                         = 0x00BA
EM_LINEINDEX                            = 0x00BB
EM_SETHANDLE                            = 0x00BC
EM_GETHANDLE                            = 0x00BD
EM_GETTHUMB                             = 0x00BE
EM_LINELENGTH                           = 0x00C1
EM_REPLACESEL                           = 0x00C2
EM_GETLINE                              = 0x00C4
EM_LIMITTEXT                            = 0x00C5
EM_CANUNDO                              = 0x00C6
EM_UNDO                                 = 0x00C7
EM_FMTLINES                             = 0x00C8
EM_LINEFROMCHAR                         = 0x00C9
EM_SETTABSTOPS                          = 0x00CB
EM_SETPASSWORDCHAR                      = 0x00CC
EM_EMPTYUNDOBUFFER                      = 0x00CD
EM_GETFIRSTVISIBLELINE                  = 0x00CE
EM_SETREADONLY                          = 0x00CF
EM_SETWORDBREAKPROC                     = 0x00D0
EM_GETWORDBREAKPROC                     = 0x00D1
EM_GETPASSWORDCHAR                      = 0x00D2
EM_SETMARGINS                           = 0x00D3
EM_GETMARGINS                           = 0x00D4
EM_GETLIMITTEXT                         = 0x00D5
EM_POSFROMCHAR                          = 0x00D6
EM_CHARFROMPOS                          = 0x00D7
EM_SETIMESTATUS                         = 0x00D8
EM_GETIMESTATUS                         = 0x00D9

SBM_SETPOS                              = 0x00E0
SBM_GETPOS                              = 0x00E1
SBM_SETRANGE                            = 0x00E2
SBM_SETRANGEREDRAW                      = 0x00E6
SBM_GETRANGE                            = 0x00E3
SBM_ENABLE_ARROWS                       = 0x00E4
SBM_SETSCROLLINFO                       = 0x00E9
SBM_GETSCROLLINFO                       = 0x00EA
SBM_GETSCROLLBARINFO                    = 0x00EB

CB_GETEDITSEL                           = 0x0140
CB_LIMITTEXT                            = 0x0141
CB_SETEDITSEL                           = 0x0142
CB_ADDSTRING                            = 0x0143
CB_DELETESTRING                         = 0x0144
CB_DIR                                  = 0x0145
CB_GETCOUNT                             = 0x0146
CB_GETCURSEL                            = 0x0147
CB_GETLBTEXT                            = 0x0148
CB_GETLBTEXTLEN                         = 0x0149
CB_INSERTSTRING                         = 0x014A
CB_RESETCONTENT                         = 0x014B
CB_FINDSTRING                           = 0x014C
CB_SELECTSTRING                         = 0x014D
CB_SETCURSEL                            = 0x014E
CB_SHOWDROPDOWN                         = 0x014F
CB_GETITEMDATA                          = 0x0150
CB_SETITEMDATA                          = 0x0151
CB_GETDROPPEDCONTROLRECT                = 0x0152
CB_SETITEMHEIGHT                        = 0x0153
CB_GETITEMHEIGHT                        = 0x0154
CB_SETEXTENDEDUI                        = 0x0155
CB_GETEXTENDEDUI                        = 0x0156
CB_GETDROPPEDSTATE                      = 0x0157
CB_FINDSTRINGEXACT                      = 0x0158
CB_SETLOCALE                            = 0x0159
CB_GETLOCALE                            = 0x015A
CB_GETTOPINDEX                          = 0x015b
CB_SETTOPINDEX                          = 0x015c
CB_GETHORIZONTALEXTENT                  = 0x015d
CB_SETHORIZONTALEXTENT                  = 0x015e
CB_GETDROPPEDWIDTH                      = 0x015f
CB_SETDROPPEDWIDTH                      = 0x0160
CB_INITSTORAGE                          = 0x0161
CB_MULTIPLEADDSTRING                    = 0x0163
CB_GETCOMBOBOXINFO                      = 0x0164

STM_SETICON                             = 0x0170
STM_GETICON                             = 0x0171
STM_SETIMAGE                            = 0x0172
STM_GETIMAGE                            = 0x0173

LB_ADDSTRING                            = 0x0180
LB_INSERTSTRING                         = 0x0181
LB_DELETESTRING                         = 0x0182
LB_SELITEMRANGEEX                       = 0x0183
LB_RESETCONTENT                         = 0x0184
LB_SETSEL                               = 0x0185
LB_SETCURSEL                            = 0x0186
LB_GETSEL                               = 0x0187
LB_GETCURSEL                            = 0x0188
LB_GETTEXT                              = 0x0189
LB_GETTEXTLEN                           = 0x018A
LB_GETCOUNT                             = 0x018B
LB_SELECTSTRING                         = 0x018C
LB_DIR                                  = 0x018D
LB_GETTOPINDEX                          = 0x018E
LB_FINDSTRING                           = 0x018F
LB_GETSELCOUNT                          = 0x0190
LB_GETSELITEMS                          = 0x0191
LB_SETTABSTOPS                          = 0x0192
LB_GETHORIZONTALEXTENT                  = 0x0193
LB_SETHORIZONTALEXTENT                  = 0x0194
LB_SETCOLUMNWIDTH                       = 0x0195
LB_ADDFILE                              = 0x0196
LB_SETTOPINDEX                          = 0x0197
LB_GETITEMRECT                          = 0x0198
LB_GETITEMDATA                          = 0x0199
LB_SETITEMDATA                          = 0x019A
LB_SELITEMRANGE                         = 0x019B
LB_SETANCHORINDEX                       = 0x019C
LB_GETANCHORINDEX                       = 0x019D
LB_SETCARETINDEX                        = 0x019E
LB_GETCARETINDEX                        = 0x019F
LB_SETITEMHEIGHT                        = 0x01A0
LB_GETITEMHEIGHT                        = 0x01A1
LB_FINDSTRINGEXACT                      = 0x01A2
LB_SETLOCALE                            = 0x01A5
LB_GETLOCALE                            = 0x01A6
LB_SETCOUNT                             = 0x01A7
LB_INITSTORAGE                          = 0x01A8
LB_ITEMFROMPOINT                        = 0x01A9
LB_MULTIPLEADDSTRING                    = 0x01B1
LB_GETLISTBOXINFO                       = 0x01B2

MessageText = []

i = 0
while i <= WM_USER:
    MessageText.append('0x%04X' % i)
    i = i + 1

MessageText[WM_NULL] = 'WM_NULL'
MessageText[WM_CREATE] = 'WM_CREATE'
MessageText[WM_DESTROY] = 'WM_DESTROY'
MessageText[WM_MOVE] = 'WM_MOVE'
MessageText[WM_SIZE] = 'WM_SIZE'
MessageText[WM_ACTIVATE] = 'WM_ACTIVATE'
MessageText[WM_SETFOCUS] = 'WM_SETFOCUS'
MessageText[WM_KILLFOCUS] = 'WM_KILLFOCUS'
MessageText[WM_ENABLE] = 'WM_ENABLE'
MessageText[WM_SETREDRAW] = 'WM_SETREDRAW'
MessageText[WM_SETTEXT] = 'WM_SETTEXT'
MessageText[WM_GETTEXT] = 'WM_GETTEXT'
MessageText[WM_GETTEXTLENGTH] = 'WM_GETTEXTLENGTH'
MessageText[WM_PAINT] = 'WM_PAINT'
MessageText[WM_CLOSE] = 'WM_CLOSE'
MessageText[WM_QUERYENDSESSION] = 'WM_QUERYENDSESSION'
MessageText[WM_QUERYOPEN] = 'WM_QUERYOPEN'
MessageText[WM_ENDSESSION] = 'WM_ENDSESSION'
MessageText[WM_QUIT] = 'WM_QUIT'
MessageText[WM_ERASEBKGND] = 'WM_ERASEBKGND'
MessageText[WM_SYSCOLORCHANGE] = 'WM_SYSCOLORCHANGE'
MessageText[WM_SHOWWINDOW] = 'WM_SHOWWINDOW'
MessageText[WM_WININICHANGE] = 'WM_WININICHANGE'
MessageText[WM_SETTINGCHANGE] = 'WM_SETTINGCHANGE'
MessageText[WM_DEVMODECHANGE] = 'WM_DEVMODECHANGE'
MessageText[WM_ACTIVATEAPP] = 'WM_ACTIVATEAPP'
MessageText[WM_FONTCHANGE] = 'WM_FONTCHANGE'
MessageText[WM_TIMECHANGE] = 'WM_TIMECHANGE'
MessageText[WM_CANCELMODE] = 'WM_CANCELMODE'
MessageText[WM_SETCURSOR] = 'WM_SETCURSOR'
MessageText[WM_MOUSEACTIVATE] = 'WM_MOUSEACTIVATE'
MessageText[WM_CHILDACTIVATE] = 'WM_CHILDACTIVATE'
MessageText[WM_QUEUESYNC] = 'WM_QUEUESYNC'
MessageText[WM_GETMINMAXINFO] = 'WM_GETMINMAXINFO'
MessageText[WM_PAINTICON] = 'WM_PAINTICON'
MessageText[WM_ICONERASEBKGND] = 'WM_ICONERASEBKGND'
MessageText[WM_NEXTDLGCTL] = 'WM_NEXTDLGCTL'
MessageText[WM_SPOOLERSTATUS] = 'WM_SPOOLERSTATUS'
MessageText[WM_DRAWITEM] = 'WM_DRAWITEM'
MessageText[WM_MEASUREITEM] = 'WM_MEASUREITEM'
MessageText[WM_DELETEITEM] = 'WM_DELETEITEM'
MessageText[WM_VKEYTOITEM] = 'WM_VKEYTOITEM'
MessageText[WM_CHARTOITEM] = 'WM_CHARTOITEM'
MessageText[WM_SETFONT] = 'WM_SETFONT'
MessageText[WM_GETFONT] = 'WM_GETFONT'
MessageText[WM_SETHOTKEY] = 'WM_SETHOTKEY'
MessageText[WM_GETHOTKEY] = 'WM_GETHOTKEY'
MessageText[WM_QUERYDRAGICON] = 'WM_QUERYDRAGICON'
MessageText[WM_COMPAREITEM] = 'WM_COMPAREITEM'
MessageText[WM_GETOBJECT] = 'WM_GETOBJECT'
MessageText[WM_COMPACTING] = 'WM_COMPACTING'
MessageText[WM_COMMNOTIFY] = 'WM_COMMNOTIFY'
MessageText[WM_WINDOWPOSCHANGING] = 'WM_WINDOWPOSCHANGING'
MessageText[WM_WINDOWPOSCHANGED] = 'WM_WINDOWPOSCHANGED'
MessageText[WM_POWER] = 'WM_POWER'
MessageText[WM_COPYDATA] = 'WM_COPYDATA'
MessageText[WM_CANCELJOURNAL] = 'WM_CANCELJOURNAL'
MessageText[WM_NOTIFY] = 'WM_NOTIFY'
MessageText[WM_INPUTLANGCHANGEREQUEST] = 'WM_INPUTLANGCHANGEREQUEST'
MessageText[WM_INPUTLANGCHANGE] = 'WM_INPUTLANGCHANGE'
MessageText[WM_TCARD] = 'WM_TCARD'
MessageText[WM_HELP] = 'WM_HELP'
MessageText[WM_USERCHANGED] = 'WM_USERCHANGED'
MessageText[WM_NOTIFYFORMAT] = 'WM_NOTIFYFORMAT'
MessageText[WM_CONTEXTMENU] = 'WM_CONTEXTMENU'
MessageText[WM_STYLECHANGING] = 'WM_STYLECHANGING'
MessageText[WM_STYLECHANGED] = 'WM_STYLECHANGED'
MessageText[WM_DISPLAYCHANGE] = 'WM_DISPLAYCHANGE'
MessageText[WM_GETICON] = 'WM_GETICON'
MessageText[WM_SETICON] = 'WM_SETICON'
MessageText[WM_NCCREATE] = 'WM_NCCREATE'
MessageText[WM_NCDESTROY] = 'WM_NCDESTROY'
MessageText[WM_NCCALCSIZE] = 'WM_NCCALCSIZE'
MessageText[WM_NCHITTEST] = 'WM_NCHITTEST'
MessageText[WM_NCPAINT] = 'WM_NCPAINT'
MessageText[WM_NCACTIVATE] = 'WM_NCACTIVATE'
MessageText[WM_GETDLGCODE] = 'WM_GETDLGCODE'
MessageText[WM_SYNCPAINT] = 'WM_SYNCPAINT'
MessageText[WM_NCMOUSEMOVE] = 'WM_NCMOUSEMOVE'
MessageText[WM_NCLBUTTONDOWN] = 'WM_NCLBUTTONDOWN'
MessageText[WM_NCLBUTTONUP] = 'WM_NCLBUTTONUP'
MessageText[WM_NCLBUTTONDBLCLK] = 'WM_NCLBUTTONDBLCLK'
MessageText[WM_NCRBUTTONDOWN] = 'WM_NCRBUTTONDOWN'
MessageText[WM_NCRBUTTONUP] = 'WM_NCRBUTTONUP'
MessageText[WM_NCRBUTTONDBLCLK] = 'WM_NCRBUTTONDBLCLK'
MessageText[WM_NCMBUTTONDOWN] = 'WM_NCMBUTTONDOWN'
MessageText[WM_NCMBUTTONUP] = 'WM_NCMBUTTONUP'
MessageText[WM_NCMBUTTONDBLCLK] = 'WM_NCMBUTTONDBLCLK'
MessageText[WM_NCXBUTTONDOWN] = 'WM_NCXBUTTONDOWN'
MessageText[WM_NCXBUTTONUP] = 'WM_NCXBUTTONUP'
MessageText[WM_NCXBUTTONDBLCLK] = 'WM_NCXBUTTONDBLCLK'
MessageText[WM_INPUT_DEVICE_CHANGE] = 'WM_INPUT_DEVICE_CHANGE'
MessageText[WM_INPUT] = 'WM_INPUT'
MessageText[WM_KEYFIRST] = 'WM_KEYFIRST'
MessageText[WM_KEYDOWN] = 'WM_KEYDOWN'
MessageText[WM_KEYUP] = 'WM_KEYUP'
MessageText[WM_CHAR] = 'WM_CHAR'
MessageText[WM_DEADCHAR] = 'WM_DEADCHAR'
MessageText[WM_SYSKEYDOWN] = 'WM_SYSKEYDOWN'
MessageText[WM_SYSKEYUP] = 'WM_SYSKEYUP'
MessageText[WM_SYSCHAR] = 'WM_SYSCHAR'
MessageText[WM_SYSDEADCHAR] = 'WM_SYSDEADCHAR'
MessageText[WM_UNICHAR] = 'WM_UNICHAR'
MessageText[WM_KEYLAST] = 'WM_KEYLAST'
MessageText[WM_KEYLAST] = 'WM_KEYLAST'
MessageText[WM_IME_STARTCOMPOSITION] = 'WM_IME_STARTCOMPOSITION'
MessageText[WM_IME_ENDCOMPOSITION] = 'WM_IME_ENDCOMPOSITION'
MessageText[WM_IME_COMPOSITION] = 'WM_IME_COMPOSITION'
MessageText[WM_IME_KEYLAST] = 'WM_IME_KEYLAST'
MessageText[WM_INITDIALOG] = 'WM_INITDIALOG'
MessageText[WM_COMMAND] = 'WM_COMMAND'
MessageText[WM_SYSCOMMAND] = 'WM_SYSCOMMAND'
MessageText[WM_TIMER] = 'WM_TIMER'
MessageText[WM_HSCROLL] = 'WM_HSCROLL'
MessageText[WM_VSCROLL] = 'WM_VSCROLL'
MessageText[WM_INITMENU] = 'WM_INITMENU'
MessageText[WM_INITMENUPOPUP] = 'WM_INITMENUPOPUP'
MessageText[WM_GESTURE] = 'WM_GESTURE'
MessageText[WM_GESTURENOTIFY] = 'WM_GESTURENOTIFY'
MessageText[WM_MENUSELECT] = 'WM_MENUSELECT'
MessageText[WM_MENUCHAR] = 'WM_MENUCHAR'
MessageText[WM_ENTERIDLE] = 'WM_ENTERIDLE'
MessageText[WM_MENURBUTTONUP] = 'WM_MENURBUTTONUP'
MessageText[WM_MENUDRAG] = 'WM_MENUDRAG'
MessageText[WM_MENUGETOBJECT] = 'WM_MENUGETOBJECT'
MessageText[WM_UNINITMENUPOPUP] = 'WM_UNINITMENUPOPUP'
MessageText[WM_MENUCOMMAND] = 'WM_MENUCOMMAND'
MessageText[WM_CHANGEUISTATE] = 'WM_CHANGEUISTATE'
MessageText[WM_UPDATEUISTATE] = 'WM_UPDATEUISTATE'
MessageText[WM_QUERYUISTATE] = 'WM_QUERYUISTATE'
MessageText[WM_CTLCOLORMSGBOX] = 'WM_CTLCOLORMSGBOX'
MessageText[WM_CTLCOLOREDIT] = 'WM_CTLCOLOREDIT'
MessageText[WM_CTLCOLORLISTBOX] = 'WM_CTLCOLORLISTBOX'
MessageText[WM_CTLCOLORBTN] = 'WM_CTLCOLORBTN'
MessageText[WM_CTLCOLORDLG] = 'WM_CTLCOLORDLG'
MessageText[WM_CTLCOLORSCROLLBAR] = 'WM_CTLCOLORSCROLLBAR'
MessageText[WM_CTLCOLORSTATIC] = 'WM_CTLCOLORSTATIC'
MessageText[WM_MOUSEFIRST] = 'WM_MOUSEFIRST'
MessageText[WM_MOUSEMOVE] = 'WM_MOUSEMOVE'
MessageText[WM_LBUTTONDOWN] = 'WM_LBUTTONDOWN'
MessageText[WM_LBUTTONUP] = 'WM_LBUTTONUP'
MessageText[WM_LBUTTONDBLCLK] = 'WM_LBUTTONDBLCLK'
MessageText[WM_RBUTTONDOWN] = 'WM_RBUTTONDOWN'
MessageText[WM_RBUTTONUP] = 'WM_RBUTTONUP'
MessageText[WM_RBUTTONDBLCLK] = 'WM_RBUTTONDBLCLK'
MessageText[WM_MBUTTONDOWN] = 'WM_MBUTTONDOWN'
MessageText[WM_MBUTTONUP] = 'WM_MBUTTONUP'
MessageText[WM_MBUTTONDBLCLK] = 'WM_MBUTTONDBLCLK'
MessageText[WM_MOUSEWHEEL] = 'WM_MOUSEWHEEL'
MessageText[WM_XBUTTONDOWN] = 'WM_XBUTTONDOWN'
MessageText[WM_XBUTTONUP] = 'WM_XBUTTONUP'
MessageText[WM_XBUTTONDBLCLK] = 'WM_XBUTTONDBLCLK'
MessageText[WM_MOUSEHWHEEL] = 'WM_MOUSEHWHEEL'
MessageText[WM_MOUSELAST] = 'WM_MOUSELAST'
MessageText[WM_MOUSELAST] = 'WM_MOUSELAST'
MessageText[WM_MOUSELAST] = 'WM_MOUSELAST'
MessageText[WM_MOUSELAST] = 'WM_MOUSELAST'
MessageText[WM_PARENTNOTIFY] = 'WM_PARENTNOTIFY'
MessageText[WM_ENTERMENULOOP] = 'WM_ENTERMENULOOP'
MessageText[WM_EXITMENULOOP] = 'WM_EXITMENULOOP'
MessageText[WM_NEXTMENU] = 'WM_NEXTMENU'
MessageText[WM_SIZING] = 'WM_SIZING'
MessageText[WM_CAPTURECHANGED] = 'WM_CAPTURECHANGED'
MessageText[WM_MOVING] = 'WM_MOVING'
MessageText[WM_POWERBROADCAST] = 'WM_POWERBROADCAST'
MessageText[WM_DEVICECHANGE] = 'WM_DEVICECHANGE'
MessageText[WM_MDICREATE] = 'WM_MDICREATE'
MessageText[WM_MDIDESTROY] = 'WM_MDIDESTROY'
MessageText[WM_MDIACTIVATE] = 'WM_MDIACTIVATE'
MessageText[WM_MDIRESTORE] = 'WM_MDIRESTORE'
MessageText[WM_MDINEXT] = 'WM_MDINEXT'
MessageText[WM_MDIMAXIMIZE] = 'WM_MDIMAXIMIZE'
MessageText[WM_MDITILE] = 'WM_MDITILE'
MessageText[WM_MDICASCADE] = 'WM_MDICASCADE'
MessageText[WM_MDIICONARRANGE] = 'WM_MDIICONARRANGE'
MessageText[WM_MDIGETACTIVE] = 'WM_MDIGETACTIVE'
MessageText[WM_MDISETMENU] = 'WM_MDISETMENU'
MessageText[WM_ENTERSIZEMOVE] = 'WM_ENTERSIZEMOVE'
MessageText[WM_EXITSIZEMOVE] = 'WM_EXITSIZEMOVE'
MessageText[WM_DROPFILES] = 'WM_DROPFILES'
MessageText[WM_MDIREFRESHMENU] = 'WM_MDIREFRESHMENU'
MessageText[WM_TOUCH] = 'WM_TOUCH'
MessageText[WM_IME_SETCONTEXT] = 'WM_IME_SETCONTEXT'
MessageText[WM_IME_NOTIFY] = 'WM_IME_NOTIFY'
MessageText[WM_IME_CONTROL] = 'WM_IME_CONTROL'
MessageText[WM_IME_COMPOSITIONFULL] = 'WM_IME_COMPOSITIONFULL'
MessageText[WM_IME_SELECT] = 'WM_IME_SELECT'
MessageText[WM_IME_CHAR] = 'WM_IME_CHAR'
MessageText[WM_IME_REQUEST] = 'WM_IME_REQUEST'
MessageText[WM_IME_KEYDOWN] = 'WM_IME_KEYDOWN'
MessageText[WM_IME_KEYUP] = 'WM_IME_KEYUP'
MessageText[WM_MOUSEHOVER] = 'WM_MOUSEHOVER'
MessageText[WM_MOUSELEAVE] = 'WM_MOUSELEAVE'
MessageText[WM_NCMOUSEHOVER] = 'WM_NCMOUSEHOVER'
MessageText[WM_NCMOUSELEAVE] = 'WM_NCMOUSELEAVE'
MessageText[WM_WTSSESSION_CHANGE] = 'WM_WTSSESSION_CHANGE'
MessageText[WM_TABLET_FIRST] = 'WM_TABLET_FIRST'
MessageText[WM_TABLET_LAST] = 'WM_TABLET_LAST'
MessageText[WM_CUT] = 'WM_CUT'
MessageText[WM_COPY] = 'WM_COPY'
MessageText[WM_PASTE] = 'WM_PASTE'
MessageText[WM_CLEAR] = 'WM_CLEAR'
MessageText[WM_UNDO] = 'WM_UNDO'
MessageText[WM_RENDERFORMAT] = 'WM_RENDERFORMAT'
MessageText[WM_RENDERALLFORMATS] = 'WM_RENDERALLFORMATS'
MessageText[WM_DESTROYCLIPBOARD] = 'WM_DESTROYCLIPBOARD'
MessageText[WM_DRAWCLIPBOARD] = 'WM_DRAWCLIPBOARD'
MessageText[WM_PAINTCLIPBOARD] = 'WM_PAINTCLIPBOARD'
MessageText[WM_VSCROLLCLIPBOARD] = 'WM_VSCROLLCLIPBOARD'
MessageText[WM_SIZECLIPBOARD] = 'WM_SIZECLIPBOARD'
MessageText[WM_ASKCBFORMATNAME] = 'WM_ASKCBFORMATNAME'
MessageText[WM_CHANGECBCHAIN] = 'WM_CHANGECBCHAIN'
MessageText[WM_HSCROLLCLIPBOARD] = 'WM_HSCROLLCLIPBOARD'
MessageText[WM_QUERYNEWPALETTE] = 'WM_QUERYNEWPALETTE'
MessageText[WM_PALETTEISCHANGING] = 'WM_PALETTEISCHANGING'
MessageText[WM_PALETTECHANGED] = 'WM_PALETTECHANGED'
MessageText[WM_HOTKEY] = 'WM_HOTKEY'
MessageText[WM_PRINT] = 'WM_PRINT'
MessageText[WM_PRINTCLIENT] = 'WM_PRINTCLIENT'
MessageText[WM_APPCOMMAND] = 'WM_APPCOMMAND'
MessageText[WM_THEMECHANGED] = 'WM_THEMECHANGED'
MessageText[WM_CLIPBOARDUPDATE] = 'WM_CLIPBOARDUPDATE'
MessageText[WM_DWMCOMPOSITIONCHANGED] = 'WM_DWMCOMPOSITIONCHANGED'
MessageText[WM_DWMNCRENDERINGCHANGED] = 'WM_DWMNCRENDERINGCHANGED'
MessageText[WM_DWMCOLORIZATIONCOLORCHANGED] = 'WM_DWMCOLORIZATIONCOLORCHANGED'
MessageText[WM_DWMWINDOWMAXIMIZEDCHANGE] = 'WM_DWMWINDOWMAXIMIZEDCHANGE'
MessageText[WM_DWMSENDICONICTHUMBNAIL] = 'WM_DWMSENDICONICTHUMBNAIL'
MessageText[WM_DWMSENDICONICLIVEPREVIEWBITMAP] = 'WM_DWMSENDICONICLIVEPREVIEWBITMAP'
MessageText[WM_GETTITLEBARINFOEX] = 'WM_GETTITLEBARINFOEX'
MessageText[WM_HANDHELDFIRST] = 'WM_HANDHELDFIRST'
MessageText[WM_HANDHELDLAST] = 'WM_HANDHELDLAST'
MessageText[WM_AFXFIRST] = 'WM_AFXFIRST'
MessageText[WM_AFXLAST] = 'WM_AFXLAST'
MessageText[WM_PENWINFIRST] = 'WM_PENWINFIRST'
MessageText[WM_PENWINLAST] = 'WM_PENWINLAST'
MessageText[WM_USER] = 'WM_USER'

MessageText[EM_GETSEL] = 'EM_GETSEL'
MessageText[EM_SETSEL] = 'EM_SETSEL'
MessageText[EM_GETRECT] = 'EM_GETRECT'
MessageText[EM_SETRECT] = 'EM_SETRECT'
MessageText[EM_SETRECTNP] = 'EM_SETRECTNP'
MessageText[EM_SCROLL] = 'EM_SCROLL'
MessageText[EM_LINESCROLL] = 'EM_LINESCROLL'
MessageText[EM_SCROLLCARET] = 'EM_SCROLLCARET'
MessageText[EM_GETMODIFY] = 'EM_GETMODIFY'
MessageText[EM_SETMODIFY] = 'EM_SETMODIFY'
MessageText[EM_GETLINECOUNT] = 'EM_GETLINECOUNT'
MessageText[EM_LINEINDEX] = 'EM_LINEINDEX'
MessageText[EM_SETHANDLE] = 'EM_SETHANDLE'
MessageText[EM_GETHANDLE] = 'EM_GETHANDLE'
MessageText[EM_GETTHUMB] = 'EM_GETTHUMB'
MessageText[EM_LINELENGTH] = 'EM_LINELENGTH'
MessageText[EM_REPLACESEL] = 'EM_REPLACESEL'
MessageText[EM_GETLINE] = 'EM_GETLINE'
MessageText[EM_LIMITTEXT] = 'EM_LIMITTEXT'
MessageText[EM_CANUNDO] = 'EM_CANUNDO'
MessageText[EM_UNDO] = 'EM_UNDO'
MessageText[EM_FMTLINES] = 'EM_FMTLINES'
MessageText[EM_LINEFROMCHAR] = 'EM_LINEFROMCHAR'
MessageText[EM_SETTABSTOPS] = 'EM_SETTABSTOPS'
MessageText[EM_SETPASSWORDCHAR] = 'EM_SETPASSWORDCHAR'
MessageText[EM_EMPTYUNDOBUFFER] = 'EM_EMPTYUNDOBUFFER'
MessageText[EM_GETFIRSTVISIBLELINE] = 'EM_GETFIRSTVISIBLELINE'
MessageText[EM_SETREADONLY] = 'EM_SETREADONLY'
MessageText[EM_SETWORDBREAKPROC] = 'EM_SETWORDBREAKPROC'
MessageText[EM_GETWORDBREAKPROC] = 'EM_GETWORDBREAKPROC'
MessageText[EM_GETPASSWORDCHAR] = 'EM_GETPASSWORDCHAR'
MessageText[EM_SETMARGINS] = 'EM_SETMARGINS'
MessageText[EM_GETMARGINS] = 'EM_GETMARGINS'
MessageText[EM_GETLIMITTEXT] = 'EM_GETLIMITTEXT'
MessageText[EM_POSFROMCHAR] = 'EM_POSFROMCHAR'
MessageText[EM_CHARFROMPOS] = 'EM_CHARFROMPOS'
MessageText[EM_SETIMESTATUS] = 'EM_SETIMESTATUS'
MessageText[EM_GETIMESTATUS] = 'EM_GETIMESTATUS'

MessageText[SBM_SETPOS] = 'SBM_SETPOS'
MessageText[SBM_GETPOS] = 'SBM_GETPOS'
MessageText[SBM_SETRANGE] = 'SBM_SETRANGE'
MessageText[SBM_SETRANGEREDRAW] = 'SBM_SETRANGEREDRAW'
MessageText[SBM_GETRANGE] = 'SBM_GETRANGE'
MessageText[SBM_ENABLE_ARROWS] = 'SBM_ENABLE_ARROWS'
MessageText[SBM_SETSCROLLINFO] = 'SBM_SETSCROLLINFO'
MessageText[SBM_GETSCROLLINFO] = 'SBM_GETSCROLLINFO'
MessageText[SBM_GETSCROLLBARINFO] = 'SBM_GETSCROLLBARINFO'

MessageText[CB_GETEDITSEL] = 'CB_GETEDITSEL'
MessageText[CB_LIMITTEXT] = 'CB_LIMITTEXT'
MessageText[CB_SETEDITSEL] = 'CB_SETEDITSEL'
MessageText[CB_ADDSTRING] = 'CB_ADDSTRING'
MessageText[CB_DELETESTRING] = 'CB_DELETESTRING'
MessageText[CB_DIR] = 'CB_DIR'
MessageText[CB_GETCOUNT] = 'CB_GETCOUNT'
MessageText[CB_GETCURSEL] = 'CB_GETCURSEL'
MessageText[CB_GETLBTEXT] = 'CB_GETLBTEXT'
MessageText[CB_GETLBTEXTLEN] = 'CB_GETLBTEXTLEN'
MessageText[CB_INSERTSTRING] = 'CB_INSERTSTRING'
MessageText[CB_RESETCONTENT] = 'CB_RESETCONTENT'
MessageText[CB_FINDSTRING] = 'CB_FINDSTRING'
MessageText[CB_SELECTSTRING] = 'CB_SELECTSTRING'
MessageText[CB_SETCURSEL] = 'CB_SETCURSEL'
MessageText[CB_SHOWDROPDOWN] = 'CB_SHOWDROPDOWN'
MessageText[CB_GETITEMDATA] = 'CB_GETITEMDATA'
MessageText[CB_SETITEMDATA] = 'CB_SETITEMDATA'
MessageText[CB_GETDROPPEDCONTROLRECT] = 'CB_GETDROPPEDCONTROLRECT'
MessageText[CB_SETITEMHEIGHT] = 'CB_SETITEMHEIGHT'
MessageText[CB_GETITEMHEIGHT] = 'CB_GETITEMHEIGHT'
MessageText[CB_SETEXTENDEDUI] = 'CB_SETEXTENDEDUI'
MessageText[CB_GETEXTENDEDUI] = 'CB_GETEXTENDEDUI'
MessageText[CB_GETDROPPEDSTATE] = 'CB_GETDROPPEDSTATE'
MessageText[CB_FINDSTRINGEXACT] = 'CB_FINDSTRINGEXACT'
MessageText[CB_SETLOCALE] = 'CB_SETLOCALE'
MessageText[CB_GETLOCALE] = 'CB_GETLOCALE'
MessageText[CB_GETTOPINDEX] = 'CB_GETTOPINDEX'
MessageText[CB_SETTOPINDEX] = 'CB_SETTOPINDEX'
MessageText[CB_GETHORIZONTALEXTENT] = 'CB_GETHORIZONTALEXTENT'
MessageText[CB_SETHORIZONTALEXTENT] = 'CB_SETHORIZONTALEXTENT'
MessageText[CB_GETDROPPEDWIDTH] = 'CB_GETDROPPEDWIDTH'
MessageText[CB_SETDROPPEDWIDTH] = 'CB_SETDROPPEDWIDTH'
MessageText[CB_INITSTORAGE] = 'CB_INITSTORAGE'
MessageText[CB_MULTIPLEADDSTRING] = 'CB_MULTIPLEADDSTRING'
MessageText[CB_GETCOMBOBOXINFO] = 'CB_GETCOMBOBOXINFO'

MessageText[STM_SETICON] = 'STM_SETICON'
MessageText[STM_GETICON] = 'STM_GETICON'
MessageText[STM_SETIMAGE] = 'STM_SETIMAGE'
MessageText[STM_GETIMAGE] = 'STM_GETIMAGE'

MessageText[LB_ADDSTRING] = 'LB_ADDSTRING'
MessageText[LB_INSERTSTRING] = 'LB_INSERTSTRING'
MessageText[LB_DELETESTRING] = 'LB_DELETESTRING'
MessageText[LB_SELITEMRANGEEX] = 'LB_SELITEMRANGEEX'
MessageText[LB_RESETCONTENT] = 'LB_RESETCONTENT'
MessageText[LB_SETSEL] = 'LB_SETSEL'
MessageText[LB_SETCURSEL] = 'LB_SETCURSEL'
MessageText[LB_GETSEL] = 'LB_GETSEL'
MessageText[LB_GETCURSEL] = 'LB_GETCURSEL'
MessageText[LB_GETTEXT] = 'LB_GETTEXT'
MessageText[LB_GETTEXTLEN] = 'LB_GETTEXTLEN'
MessageText[LB_GETCOUNT] = 'LB_GETCOUNT'
MessageText[LB_SELECTSTRING] = 'LB_SELECTSTRING'
MessageText[LB_DIR] = 'LB_DIR'
MessageText[LB_GETTOPINDEX] = 'LB_GETTOPINDEX'
MessageText[LB_FINDSTRING] = 'LB_FINDSTRING'
MessageText[LB_GETSELCOUNT] = 'LB_GETSELCOUNT'
MessageText[LB_GETSELITEMS] = 'LB_GETSELITEMS'
MessageText[LB_SETTABSTOPS] = 'LB_SETTABSTOPS'
MessageText[LB_GETHORIZONTALEXTENT] = 'LB_GETHORIZONTALEXTENT'
MessageText[LB_SETHORIZONTALEXTENT] = 'LB_SETHORIZONTALEXTENT'
MessageText[LB_SETCOLUMNWIDTH] = 'LB_SETCOLUMNWIDTH'
MessageText[LB_ADDFILE] = 'LB_ADDFILE'
MessageText[LB_SETTOPINDEX] = 'LB_SETTOPINDEX'
MessageText[LB_GETITEMRECT] = 'LB_GETITEMRECT'
MessageText[LB_GETITEMDATA] = 'LB_GETITEMDATA'
MessageText[LB_SETITEMDATA] = 'LB_SETITEMDATA'
MessageText[LB_SELITEMRANGE] = 'LB_SELITEMRANGE'
MessageText[LB_SETANCHORINDEX] = 'LB_SETANCHORINDEX'
MessageText[LB_GETANCHORINDEX] = 'LB_GETANCHORINDEX'
MessageText[LB_SETCARETINDEX] = 'LB_SETCARETINDEX'
MessageText[LB_GETCARETINDEX] = 'LB_GETCARETINDEX'
MessageText[LB_SETITEMHEIGHT] = 'LB_SETITEMHEIGHT'
MessageText[LB_GETITEMHEIGHT] = 'LB_GETITEMHEIGHT'
MessageText[LB_FINDSTRINGEXACT] = 'LB_FINDSTRINGEXACT'
MessageText[LB_SETLOCALE] = 'LB_SETLOCALE'
MessageText[LB_GETLOCALE] = 'LB_GETLOCALE'
MessageText[LB_SETCOUNT] = 'LB_SETCOUNT'
MessageText[LB_INITSTORAGE] = 'LB_INITSTORAGE'
MessageText[LB_ITEMFROMPOINT] = 'LB_ITEMFROMPOINT'
MessageText[LB_MULTIPLEADDSTRING] = 'LB_MULTIPLEADDSTRING'
MessageText[LB_GETLISTBOXINFO] = 'LB_GETLISTBOXINFO'

tbl = []

Message = 0

tbl.append('enum MessageFunctionIndex')
tbl.append('{')

for MsgIndex in MessageFunction:
    idx = '    IMSG_%s = 0x%08X,' % (MsgIndex.ljust(36), Message)
    tbl.append(idx)
    MessageFunctionIndex.append('IMSG_%s' % MsgIndex)

    Message = Message + 1

tbl.append('};')
tbl.append('')

MsgTableEntryDef = '''\
typedef struct
{
    BYTE    Function;
    BYTE    ThunkMessage       : 1;
    BYTE    SyncOnlyMessage    : 1;
} ML_MSG_TABLE_ENTRY;
\
'''

tbl.append(MsgTableEntryDef)
tbl.append('ML_MSG_TABLE_ENTRY MessageTable[] =')
tbl.append('{')

Message = 0

for msg in MessageTable:
    Function = msg & 0x3F
    ThunkMessage = (msg & 0x40) != 0
    SyncOnlyMessage = (msg & 0x80) != 0

    Name = MessageFunction[Function]

    msgentry = '%s%s%s' % ((MessageFunctionIndex[Function] + ', ').ljust(40), (str(ThunkMessage).upper() + ', ').ljust(7), str(SyncOnlyMessage).upper())
    msgentry = '%s%s' % (('    { ' + msgentry + ', ').ljust(64) + ' }, ', '// ' + MessageText[Message])
    tbl.append(msgentry)

    Message = Message + 1

tbl.append('};')
tbl.append('')

fs = open('fuck.h','wb')
fs.write('\r\n'.join(tbl).encode('936'))
fs.close()

# input()
