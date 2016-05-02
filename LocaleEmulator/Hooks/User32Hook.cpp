#include "stdafx.h"
#include "MessageTable.h"

VOID ResetDCCharset(PLeGlobalData GlobalData, HWND hWnd);

ForceInline VOID CheckDC(HDC hDC)
{
    // if (GdiGetCodePage(hDC) == 0x3A4) _asm ud2;
}

ForceInline BOOL IsCallProcHandle(PVOID WndProc)
{
    return ((ULONG)WndProc & 0xFFFF0000) == 0xFFFF0000;
}

ForceInline VOID InitUnicodeProc(PLeGlobalData GlobalData, HWND hWnd, PVOID UnicodeProc, PVOID OriginalProcA)
{
    SetWindowLongPtrW(hWnd, GWLP_WNDPROC, (LONG_PTR)UnicodeProc);
    GlobalData->SetWindowDataA(hWnd, OriginalProcA);
    ResetDCCharset(GlobalData, hWnd);
}

/************************************************************************
  ansi to unicode
************************************************************************/

UserMessageCall(EMPTY)
{
    return CallUserMessageCallA();
}

UserMessageCall(INLPCREATESTRUCT)
{
    LRESULT         Result;
    LPCREATESTRUCTW CreateStructW;
    CREATESTRUCTA   CreateStructA;

    CreateStructW = (LPCREATESTRUCTW)lParam;

    SEH_TRY
    {
        PCBT_CREATE_PARAM CbtCreateParam = (PCBT_CREATE_PARAM)CreateStructW->lpCreateParams;
        if (CbtCreateParam->Magic == CBT_PROC_PARAM_CONTEXT)
            CreateStructW->lpCreateParams = CbtCreateParam->CreateParams;
    }
    SEH_EXCEPT(EXCEPTION_EXECUTE_HANDLER)
    {
        ;
    }

    CreateStructA.lpszClass = nullptr;
    CreateStructA.lpszName = nullptr;

    LOOP_ONCE
    {
        if (CreateStructW == nullptr)
            break;

        CreateStructA = *(LPCREATESTRUCTA)CreateStructW;

        CreateStructA.lpszClass = nullptr;
        CreateStructA.lpszName = nullptr;

        CreateStructA.lpszClass = ClassWCharToMByte(CreateStructW->lpszClass);
        if (CreateStructA.lpszClass == nullptr)
            break;

        if (CreateStructW->lpszName != nullptr)
        {
            CreateStructA.lpszName = TitleWCharToMByte(CreateStructW->lpszName);
            if (CreateStructA.lpszName == nullptr)
                break;
        }

        lParam = (LPARAM)&CreateStructA;
    }

    Result = CallUserMessageCallA();

    FreeClass((PVOID)CreateStructA.lpszClass);
    FreeString((PVOID)CreateStructA.lpszName);

    return Result;
}

UserMessageCall(INLPMDICREATESTRUCT)
{
    LRESULT             Result;
    LPMDICREATESTRUCTW  MdiCreateStructW;
    MDICREATESTRUCTA    MdiCreateStructA;

    MdiCreateStructW = (LPMDICREATESTRUCTW)lParam;

    MdiCreateStructA.szClass = nullptr;
    MdiCreateStructA.szTitle = nullptr;

    LOOP_ONCE
    {
        if (MdiCreateStructW == nullptr)
            break;

        MdiCreateStructA = *(LPMDICREATESTRUCTA)MdiCreateStructW;

        MdiCreateStructA.szClass = nullptr;
        MdiCreateStructA.szTitle = nullptr;

        MdiCreateStructA.szClass = ClassWCharToMByte(MdiCreateStructW->szClass);
        if (MdiCreateStructA.szClass == nullptr)
            break;

        if (MdiCreateStructW->szTitle != nullptr)
        {
            MdiCreateStructA.szTitle = TitleWCharToMByte(MdiCreateStructW->szTitle);
            if (MdiCreateStructA.szTitle == nullptr)
                break;
        }

        lParam = (LPARAM)&MdiCreateStructA;
    }

    Result = CallUserMessageCallA();

    FreeClass((PVOID)MdiCreateStructA.szClass);
    FreeString((PVOID)MdiCreateStructA.szTitle);

    return Result;
}

UserMessageCall(INSTRINGNULL)
{
    PSTR    Ansi;
    LRESULT Result, Length;

    Ansi = nullptr;

    LOOP_ONCE
    {
        if (lParam == 0)
            break;

        Length = StrLengthW((PWSTR)lParam);
        if (Length == 0)
            break;

        Ansi = WCharToMByte((PWSTR)lParam, Length);
        if (Ansi == nullptr)
            break;

        lParam = (LPARAM)Ansi;
    }

    Result = CallUserMessageCallA();

    FreeString(Ansi);

    return Result;
}

UserMessageCall(OUTSTRING)
{
    LRESULT OutputSize, Length;
    PWSTR   OutputBuffer;
    PSTR    Ansi;

    OutputSize = wParam;
    OutputBuffer = (PWSTR)lParam;

    Length = OutputSize * sizeof(WCHAR);
    Ansi = AllocAnsi(Length);
    if (Ansi == nullptr)
        return 0;

    wParam = Length;
    lParam = (LPARAM)Ansi;

    Length = CallUserMessageCallA();

    if (Length != 0)
        AnsiToUnicode(OutputBuffer, OutputSize, Ansi, Length, (PULONG_PTR)&Length);

    FreeString(Ansi);

    return Length / sizeof(WCHAR);
}

UserMessageCall(INSTRING)
{
    return CallUserLocalCall(INSTRINGNULL);
}

UserMessageCall(INCNTOUTSTRING)
{
    LRESULT OutputSize, Length;
    PWSTR   OutputBuffer;
    PSTR    Ansi;

    PMSG_INPUT_COUNT_OUPUT_STRING ParamA, ParamW;

    ParamW = (PMSG_INPUT_COUNT_OUPUT_STRING)lParam;

    OutputSize = ParamW->BufferSize;
    OutputBuffer = ParamW->UnicodeBuffer;

    Length = OutputSize * sizeof(WCHAR);
    Ansi = AllocAnsi(Length);
    if (Ansi == nullptr)
        return 0;

    ParamA = (PMSG_INPUT_COUNT_OUPUT_STRING)Ansi;
    ParamA->BufferSize = Length;
    lParam = (LPARAM)ParamA;

    Length = CallUserMessageCallA();

    if (Length != 0)
        AnsiToUnicode(OutputBuffer, OutputSize, Ansi, Length, (PULONG_PTR)&Length);

    FreeString(Ansi);

    return Length / sizeof(WCHAR);
}

UserMessageCall(INCBOXSTRING)
{
    return CallUserLocalCall(INSTRINGNULL);
}

UserMessageCall(OUTCBOXSTRING)
{
    LRESULT OutputSize, Length;
    PWSTR   OutputBuffer;
    PSTR    Ansi;

    OutputSize = CallWindowProcA(PrevProc, Window, Message + 1, wParam, 0);
    if (OutputSize == 0 || OutputSize == LB_ERR)
        return OutputSize;

    OutputBuffer = (PWSTR)lParam;
    ++OutputSize;
    Ansi = AllocAnsi(OutputSize);
    if (Ansi == nullptr)
        return 0;

    Length = CallWindowProcA(PrevProc, Window, Message, wParam, (LPARAM)Ansi);

    if (Length != 0 && Length != LB_ERR)
        AnsiToUnicode(OutputBuffer, -1, Ansi, Length, (PULONG_PTR)&Length);

    FreeString(Ansi);

    return Length / sizeof(WCHAR);
}

UserMessageCall(INLBOXSTRING)
{
    return CallUserLocalCall(INSTRINGNULL);
}

UserMessageCall(OUTLBOXSTRING)
{
    return CallUserLocalCall(OUTCBOXSTRING);
}

UserMessageCall(INCNTOUTSTRINGNULL)
{
    return CallUserLocalCall(OUTSTRING);
}

UserMessageCall(GETDBCSTEXTLENGTHS)
{
    PSTR    Ansi;
    LRESULT Length;

    Length = CallUserMessageCallA();
    if (Length == 0 || Length == LB_ERR)
        return 0;

    ++Length;
    Ansi = AllocAnsi(Length);
    if (Ansi == nullptr)
        return 0;

    wParam = Message == WM_GETTEXTLENGTH ? Length : wParam;
    lParam = (LPARAM)Ansi;
    --Message;
    Length = CallUserMessageCallA();

    if (Length != 0 && Length != LB_ERR)
    {
        ULONG UnicodeBytes;
        RtlMultiByteToUnicodeSize(&UnicodeBytes, Ansi, Length);
        Length = UnicodeBytes;
    }

    FreeString(Ansi);

    return Length / sizeof(WCHAR);
}

LRESULT NTAPI WindowProcW(HWND Window, UINT Message, WPARAM wParam, LPARAM lParam)
{
    WNDPROC             PrevProc;
    FNUSERMESSAGECALL   MessageCall;
    PLeGlobalData       GlobalData = LeGetGlobalData();

    PrevProc = (WNDPROC)GlobalData->GetWindowDataA(Window);

    if (Message < countof(MessageTable))
    {
        MessageCall = gapfnMessageCall[MessageTable[Message].Function].UserCall;
        return MessageCall(PrevProc, Window, Message, wParam, lParam);
    }

    return CallUserMessageCallA();
}

/************************************************************************
  unicode to ansi
************************************************************************/

KernelMessageCall(EMPTY)
{
    return CallNtUserMessageCall();
}

KernelMessageCall(INLPCREATESTRUCT)
{
    LRESULT         Result;
    LPCREATESTRUCTA CreateStructA;
    CREATESTRUCTW   CreateStructW;

    CreateStructA = (LPCREATESTRUCTA)lParam;
    CreateStructW.lpszClass = nullptr;
    CreateStructW.lpszName = nullptr;

    LOOP_ONCE
    {
        if (CreateStructA == nullptr)
            break;

        CreateStructW = *(LPCREATESTRUCTW)CreateStructA;

        CreateStructW.lpszClass = nullptr;
        CreateStructW.lpszName = nullptr;

        CreateStructW.lpszClass = ClassMByteToWChar(CreateStructA->lpszClass);
        if (CreateStructW.lpszClass == nullptr)
            break;

        if (CreateStructA->lpszName != nullptr)
        {
            CreateStructW.lpszName = TitleMByteToWChar(CreateStructA->lpszName);
            if (CreateStructW.lpszName == nullptr)
                break;
        }

        lParam = (LPARAM)&CreateStructW;
        CLEAR_FLAG(Flags, WINDOW_FLAG_ANSI);
    }

    Result = CallNtUserMessageCall();

    FreeClass((PVOID)CreateStructW.lpszClass);
    FreeString((PVOID)CreateStructW.lpszName);

    return Result;
}

KernelMessageCall(INLPMDICREATESTRUCT)
{
    LRESULT             Result;
    LPMDICREATESTRUCTA  MdiCreateStructA;
    MDICREATESTRUCTW    MdiCreateStructW;

    MdiCreateStructA = (LPMDICREATESTRUCTA)lParam;
    MdiCreateStructW.szClass = nullptr;
    MdiCreateStructW.szTitle = nullptr;

    LOOP_ONCE
    {
        if (MdiCreateStructA == nullptr)
            break;

        MdiCreateStructW = *(LPMDICREATESTRUCTW)MdiCreateStructA;

        MdiCreateStructW.szClass = nullptr;
        MdiCreateStructW.szTitle = nullptr;

        MdiCreateStructW.szClass = ClassMByteToWChar(MdiCreateStructA->szClass);
        if (MdiCreateStructW.szClass == nullptr)
            break;

        if (MdiCreateStructA->szTitle != nullptr)
        {
            MdiCreateStructW.szTitle = TitleMByteToWChar(MdiCreateStructA->szTitle);
            if (MdiCreateStructW.szTitle == nullptr)
                break;
        }

        lParam = (LPARAM)&MdiCreateStructW;
        CLEAR_FLAG(Flags, WINDOW_FLAG_ANSI);
    }

    Result = CallNtUserMessageCall();

    FreeClass((PVOID)MdiCreateStructW.szClass);
    FreeString((PVOID)MdiCreateStructW.szTitle);

    return Result;
}

KernelMessageCall(INSTRINGNULL)
{
    PWSTR   Unicode;
    LRESULT Result, Length;

    Unicode = nullptr;

    LOOP_ONCE
    {
        if (lParam == 0)
            break;

        Length = StrLengthA((PSTR)lParam);
        if (Length == 0)
            break;

        Unicode = MByteToWChar((PSTR)lParam, Length);
        if (Unicode == nullptr)
            break;

        lParam = (LPARAM)Unicode;
        CLEAR_FLAG(Flags, WINDOW_FLAG_ANSI);
    }

    Result = CallNtUserMessageCall();

    FreeString(Unicode);

    return Result;
}

KernelMessageCall(OUTSTRING)
{
    LRESULT Length, OutputSize;
    PWSTR   Unicode;
    PSTR    OutputBuffer;

    PLeGlobalData GlobalData = LeGetGlobalData();

    CLEAR_FLAG(Flags, WINDOW_FLAG_ANSI);

    OutputBuffer = (PSTR)lParam;
    OutputSize = wParam;

    Length = GlobalData->NtUserMessageCall(Window, Message + 1, wParam, lParam, xParam, xpfnProc, Flags);
    if (Length == 0)
        return 0;

    ++Length;
    Unicode = AllocUnicode(Length);
    if (Unicode == nullptr)
        return 0;

    Length = GlobalData->NtUserMessageCall(Window, Message, Length, (LPARAM)Unicode, xParam, xpfnProc, Flags);

    UnicodeToAnsi(OutputBuffer, OutputSize, Unicode, Length, (PULONG_PTR)&Length);

    FreeString(Unicode);

    return Length;
}

KernelMessageCall(INSTRING)
{
    return CallNtUserLocalCall(INSTRINGNULL);
}

KernelMessageCall(INCNTOUTSTRING)
{
    LRESULT OutputSize, Length;
    PSTR    OutputBuffer;
    PWSTR   Unicode;

    PMSG_INPUT_COUNT_OUPUT_STRING ParamA, ParamW;

    ParamA = (PMSG_INPUT_COUNT_OUPUT_STRING)lParam;

    OutputSize = ParamA->BufferSize;
    OutputBuffer = ParamA->AnsiBuffer;

    Unicode = AllocUnicode(OutputSize);
    if (Unicode == nullptr)
        return 0;

    ParamW = (PMSG_INPUT_COUNT_OUPUT_STRING)Unicode;
    ParamW->BufferSize = OutputSize;
    lParam = (LPARAM)ParamW;

    Length = CallNtUserMessageCallW();

    if (Length != 0)
        UnicodeToAnsi(OutputBuffer, OutputSize, Unicode, Length, (PULONG_PTR)&Length);

    FreeString(Unicode);

    return Length;
}

KernelMessageCall(INCBOXSTRING)
{
    return CallNtUserLocalCall(INSTRINGNULL);
}

KernelMessageCall(OUTCBOXSTRING)
{
    LRESULT Length, OutputSize;
    PWSTR   Unicode;
    PSTR    OutputBuffer;

    PLeGlobalData GlobalData = LeGetGlobalData();

    CLEAR_FLAG(Flags, WINDOW_FLAG_ANSI);

    OutputBuffer = (PSTR)lParam;

    Length = GlobalData->NtUserMessageCall(Window, Message + 1, wParam, lParam, xParam, xpfnProc, Flags);
    if (Length == 0 || Length == LB_ERR)
        return 0;

    ++Length;
    Unicode = AllocUnicode(Length);
    if (Unicode == nullptr)
        return 0;

    Length = GlobalData->NtUserMessageCall(Window, Message, wParam, (LPARAM)Unicode, xParam, xpfnProc, Flags);

    if (Length != 0 && Length != LB_ERR)
        UnicodeToAnsi(OutputBuffer, -1, Unicode, Length, (PULONG_PTR)&Length);

    FreeString(Unicode);

    return Length;
}

KernelMessageCall(INLBOXSTRING)
{
    return CallNtUserLocalCall(INSTRINGNULL);
}

KernelMessageCall(OUTLBOXSTRING)
{
    return CallNtUserLocalCall(OUTCBOXSTRING);
}

KernelMessageCall(INCNTOUTSTRINGNULL)
{
    return CallNtUserLocalCall(OUTSTRING);
}

KernelMessageCall(GETDBCSTEXTLENGTHS)
{
    LRESULT Length;
    PWSTR   Unicode;

    Length = CallNtUserMessageCallW();
    if (Length == 0 || Length == LB_ERR)
        return 0;

    ++Length;
    Unicode = AllocUnicode(Length);
    if (Unicode == nullptr)
        return 0;

    wParam = Message == WM_GETTEXTLENGTH ? Length : wParam;
    lParam = (LPARAM)Unicode;
    --Message;
    Length = CallNtUserMessageCallW();

    if (Length != 0 && Length != LB_ERR)
    {
        ULONG AnsiBytes;
        RtlUnicodeToMultiByteSize(&AnsiBytes, Unicode, Length * sizeof(WCHAR));
        Length = AnsiBytes;
    }

    FreeString(Unicode);

    return Length;
}

LRESULT
NTAPI
LeNtUserMessageCall(
    HWND         Window,
    UINT         Message,
    WPARAM       wParam,
    LPARAM       lParam,
    ULONG_PTR    xParam,
    ULONG        xpfnProc,
    ULONG        Flags
)
{
    FNMESSAGECALL MessageCall;

    LOOP_ONCE
    {
        if (Message >= countof(MessageTable))
            continue;

        if (!FLAG_ON(Flags, WINDOW_FLAG_ANSI))
            break;

        MessageCall = gapfnMessageCall[MessageTable[Message].Function].KernelCall;
        return CALLNTMSGCALL(MessageCall);
    }

    return CallNtUserMessageCall();
}

BOOL
NTAPI
LeNtUserDefSetText(
    HWND                    hWnd,
    PLARGE_UNICODE_STRING   Text
)
{
    BOOL                    Success;
    LARGE_UNICODE_STRING    UnicodeText;
    PLeGlobalData           GlobalData = LeGetGlobalData();

    InitEmptyLargeString(&UnicodeText);

    LOOP_ONCE
    {
        if (Text == nullptr)
            break;

        if (GlobalData->GetWindowDataA(hWnd) == nullptr)
            break;

        if (LargeStringAnsiToUnicode(Text, &UnicodeText) == nullptr)
            break;

        Text = &UnicodeText;
    }

    Success = GlobalData->NtUserSetDefText(hWnd, Text);

    FreeLargeString(&UnicodeText);

    return Success;
}

BOOL VerifyWindowParam(PCBT_CREATE_PARAM CbtCreateParam, PCBT_PROC_PARAM CbtParam)
{
    SEH_TRY
    {
        return CbtCreateParam->StackPointer == CbtParam->StackPointer;
    }
    SEH_EXCEPT(EXCEPTION_EXECUTE_HANDLER)
    {
        return FALSE;
    }
}

VOID ResetDCCharset(PLeGlobalData GlobalData, HWND hWnd)
{
    HDC hDC;
    HFONT Font;

    Font = (HFONT)GetStockObject(SYSTEM_FONT);

    hDC = GetDC(hWnd);
    //Font = GetFontFromDC(GlobalData, hDC);
    SelectObject(hDC, Font);
    //DeleteObject(Font);

    CheckDC(hDC);

    ReleaseDC(hWnd, hDC);

    hDC = GetWindowDC(hWnd);
    //Font = GetFontFromDC(GlobalData, hDC);
    SelectObject(hDC, Font);

    CheckDC(hDC);

    //DeleteObject(Font);
    ReleaseDC(hWnd, hDC);
}

LRESULT CALLBACK CBTProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    PCBT_PROC_PARAM     CbtParam = (PCBT_PROC_PARAM)FindThreadFrame(CBT_PROC_PARAM_CONTEXT);
    LPCBT_CREATEWND     CreateWnd;
    PCBT_CREATE_PARAM   CreateParam;

    if (nCode == HCBT_CREATEWND) LOOP_ONCE
    {
        BOOL            UnicodeWindow;
        HWND            hWnd;
        WNDPROC         OriginalProcA, OriginalProcW;
        PLeGlobalData   GlobalData;

        hWnd        = (HWND)wParam;
        CreateWnd   = (LPCBT_CREATEWND)lParam;
        CreateParam = (PCBT_CREATE_PARAM)CreateWnd->lpcs->lpCreateParams;

        if (VerifyWindowParam(CreateParam, CbtParam) == FALSE)
            break;

        CreateWnd->lpcs->lpCreateParams = CreateParam->CreateParams;
        GlobalData = CbtParam->GlobalData;

        if (GlobalData->GetWindowDataA(hWnd) != nullptr)
            break;

        OriginalProcA = (WNDPROC)GlobalData->GetWindowLongA(hWnd, GWLP_WNDPROC);
        //if (IsCallProcHandle(OriginalProcA) == FALSE)
        {
            InitUnicodeProc(GlobalData, hWnd, WindowProcW, OriginalProcA);
        }
    }

    return CallNextHookEx(CbtParam->Hook, nCode, wParam, lParam);
}

LRESULT CALLBACK CBTProcU(int nCode, WPARAM wParam, LPARAM lParam)
{
    LRESULT             Result;
    PCBT_PROC_PARAM     CbtParam = (PCBT_PROC_PARAM)FindThreadFrame(CBT_PROC_PARAM_CONTEXT);
    LPCBT_CREATEWND     CreateWnd;
    PCBT_CREATE_PARAM   CreateParam;

    Result = CallNextHookEx(CbtParam->Hook, nCode, wParam, lParam);

    if (nCode == HCBT_CREATEWND) LOOP_ONCE
    {
        ResetDCCharset(CbtParam->GlobalData, (HWND)wParam);
    }

    return Result;
}

BOOL
InstallCbtHook(
    PLeGlobalData       GlobalData,
    PCBT_PROC_PARAM     CbtParam,
    PCBT_CREATE_PARAM   CreateParam,
    PVOID               StackPointer,
    PVOID&              Param
)
{
    CbtParam->Hook = SetWindowsHookExA(WH_CBT, CBTProc, nullptr, CurrentTid());
    if (CbtParam->Hook == nullptr)
        return FALSE;

    CbtParam->GlobalData = GlobalData;
    CbtParam->StackPointer = StackPointer;
    CbtParam->Push();

    CreateParam->StackPointer = StackPointer;
    CreateParam->CreateParams = Param;
    Param = CreateParam;

    return TRUE;
}

BOOL
InstallUnicodeCbtHook(
    PLeGlobalData       GlobalData,
    PCBT_PROC_PARAM     CbtParam
)
{
    CbtParam->Hook = SetWindowsHookExA(WH_CBT, CBTProcU, nullptr, CurrentTid());
    if (CbtParam->Hook == nullptr)
        return FALSE;

    CbtParam->GlobalData = GlobalData;
    CbtParam->Push();

    return TRUE;
}

VOID UninstallCbtHook(PCBT_PROC_PARAM CbtParam)
{
    if (CbtParam->Hook == nullptr)
        return;

    UnhookWindowsHookEx(CbtParam->Hook);
    CbtParam->Pop();
}

typedef struct
{
    ULONG                   ExStyle;
    PLARGE_UNICODE_STRING   ClassName;
    PLARGE_UNICODE_STRING   ClassVersion;
    PLARGE_UNICODE_STRING   WindowName;
    ULONG                   Style;
    LONG                    X;
    LONG                    Y;
    LONG                    Width;
    LONG                    Height;
    HWND                    ParentWnd;
    HMENU                   Menu;
    PVOID                   Instance;
    LPVOID                  Param;
    ULONG                   ShowMode;
    ULONG                   Unknown1;
    ULONG                   Unknown2;
    ULONG                   Unknown3;

} *PUSER_CREATE_WINDOW;

typedef struct { PVOID _[15]; } USER_CREATE_WINDOW_WIN7;
typedef struct { PVOID _[16]; } USER_CREATE_WINDOW_WIN8;
typedef struct { PVOID _[17]; } USER_CREATE_WINDOW_WIN10;

template<class PARAM_TYPE>
HWND NtUserCreateWindowExInvoker(PVOID Routine, PUSER_CREATE_WINDOW Parameters)
{
    HWND (NTAPI *NtUserCreateWindowEx)(PARAM_TYPE);
    *(PVOID *)&NtUserCreateWindowEx = Routine;
    return NtUserCreateWindowEx(*(PARAM_TYPE *)Parameters);
}

HWND LeNtUserCreateWindowExWorker(PUSER_CREATE_WINDOW Parameters, HWND (*Invoker)(PVOID, PUSER_CREATE_WINDOW))
{
    HWND                    hWnd;
    NTSTATUS                LastError;
    LARGE_UNICODE_STRING    UnicodeWindowName;
    PLeGlobalData           GlobalData;
    CBT_PROC_PARAM          CbtParam;
    CBT_CREATE_PARAM        CreateParam;

    GlobalData = LeGetGlobalData();

    InitEmptyLargeString(&UnicodeWindowName);

    LOOP_ONCE
    {
        if (!FLAG_ON(Parameters->ExStyle, WS_EX_ANSI))
        {
            InstallUnicodeCbtHook(GlobalData, &CbtParam);
            break;
        }

        if (Parameters->WindowName != nullptr)
        {
            if (CaptureAnsiWindowName(Parameters->WindowName, &UnicodeWindowName) == nullptr)
                break;
        }

        Parameters->WindowName = &UnicodeWindowName;

        InstallCbtHook(GlobalData, &CbtParam, &CreateParam, _AddressOfReturnAddress(), Parameters->Param);
    }

    hWnd = Invoker(GlobalData->HookStub.StubNtUserCreateWindowEx, Parameters);

    LastError = RtlGetLastWin32Error();
/*
    if (hWnd != nullptr)
    {
        HDC hDC;

        hDC= GetDC(hWnd);

        CheckDC(hDC);

        ReleaseDC(hWnd, hDC);

        hDC= GetWindowDC(hWnd);

        CheckDC(hDC);

        ReleaseDC(hWnd, hDC);
    }
*/
    UninstallCbtHook(&CbtParam);
    FreeLargeString(&UnicodeWindowName);

    RtlSetLastWin32Error(LastError);

    return hWnd;
}

HWND NTAPI LeNtUserCreateWindowEx_Win7(USER_CREATE_WINDOW_WIN7 Parameters)
{
    return LeNtUserCreateWindowExWorker((PUSER_CREATE_WINDOW)&Parameters, NtUserCreateWindowExInvoker<USER_CREATE_WINDOW_WIN7>);
}

HWND NTAPI LeNtUserCreateWindowEx_Win8(USER_CREATE_WINDOW_WIN8 Parameters)
{
    return LeNtUserCreateWindowExWorker((PUSER_CREATE_WINDOW)&Parameters, NtUserCreateWindowExInvoker<USER_CREATE_WINDOW_WIN8>);
}

HWND NTAPI LeNtUserCreateWindowEx_Win10(USER_CREATE_WINDOW_WIN10 Parameters)
{
    return LeNtUserCreateWindowExWorker((PUSER_CREATE_WINDOW)&Parameters, NtUserCreateWindowExInvoker<USER_CREATE_WINDOW_WIN10>);
}

LONG_PTR NTAPI LeGetWindowLongA(HWND hWnd, int Index)
{
    PVOID           Proc;
    PLeGlobalData   GlobalData = LeGetGlobalData();

    switch (Index)
    {
        case GWL_WNDPROC:
            Proc = GlobalData->GetWindowDataA(hWnd);
            if (Proc == nullptr)
                break;

            return (LONG_PTR)Proc;
    }

    return GlobalData->GetWindowLongA(hWnd, Index);
}

LONG_PTR NTAPI LeSetWindowLongA(HWND hWnd, int Index, LONG_PTR NewLong)
{
    PVOID           OriginalProcA;
    PLeGlobalData   GlobalData = LeGetGlobalData();

    switch (Index)
    {
        case GWL_WNDPROC:
            OriginalProcA = GlobalData->GetWindowDataA(hWnd);
            if (OriginalProcA != nullptr)
            {
                GlobalData->SetWindowDataA(hWnd, (PVOID)NewLong);
            }
            else
            {
                OriginalProcA = (PVOID)GlobalData->SetWindowLongA(hWnd, Index, NewLong);
                InitUnicodeProc(GlobalData, hWnd, WindowProcW, (PVOID)NewLong);
            }

            return (LONG_PTR)OriginalProcA;
    }

    return GlobalData->SetWindowLongA(hWnd, Index, NewLong);
}

BOOL NTAPI LeIsWindowUnicode(HWND hWnd)
{
    PLeGlobalData GlobalData = LeGetGlobalData();

    return GlobalData->GetWindowDataA(hWnd) == nullptr ? GlobalData->IsWindowUnicode(hWnd) : FALSE;
}

HANDLE NTAPI LeSetClipboardData(UINT Format, HANDLE Memory)
{
    HGLOBAL             Data;
    PWSTR               Unicode;
    PSTR                Ansi;
    ULONG_PTR           Length;
    PLeGlobalData       GlobalData = LeGetGlobalData();

    Ansi = nullptr;
    switch (Format)
    {
        case CF_TEXT:

            Ansi = (PSTR)GlobalLock(Memory);
            if (Ansi == nullptr)
                break;

            Length = StrLengthA(Ansi);
            if (Length == 0)
                break;

            ++Length;
            Data = GlobalAlloc(GHND, Length * sizeof(WCHAR));
            if (Data == nullptr)
                break;

            Unicode = (PWSTR)GlobalLock(Data);
            AnsiToUnicode(Unicode, Length, Ansi, Length - 1);
            GlobalUnlock(Data);

            if (GlobalData->SetClipboardData(CF_UNICODETEXT, Data) == nullptr)
            {
                GlobalFree(Data);
            }

            break;
    }

    if (Ansi != nullptr)
        GlobalUnlock(Memory);

    return GlobalData->SetClipboardData(Format, Memory);
}

HANDLE NTAPI LeGetClipboardData(UINT Format)
{
    HGLOBAL             Data, AnsiData;
    ULONG_PTR           Length, Flags;
    PWSTR               Unicode;
    PSTR                Ansi;
    PLeGlobalData       GlobalData = LeGetGlobalData();

    switch (Format)
    {
        case CF_TEXT:
            Data = GlobalData->GetClipboardData(CF_UNICODETEXT);
            if (Data == nullptr)
                break;

            Flags = GlobalFlags(Data);
            if (FLAG_ON(Flags, GMEM_INVALID_HANDLE))
                break;

            Unicode = (PWSTR)GlobalLock(Data);
            if (Unicode == nullptr)
                break;

            Length = StrLengthW(Unicode);
            AnsiData = GlobalAlloc(GHND, Length * sizeof(WCHAR) + 1);
            if (AnsiData == nullptr)
            {
                GlobalUnlock(Data);
                break;
            }

            Ansi = (PSTR)GlobalLock(AnsiData);

            UnicodeToAnsi(Ansi, Length * sizeof(WCHAR) + 1, Unicode, Length);

            GlobalUnlock(AnsiData);
            GlobalUnlock(Data);

            Data = SetClipboardData(CF_TEXT, AnsiData);
            if (Data == nullptr)
            {
                GlobalFree(AnsiData);
            }

            break;
    }

    return GlobalData->GetClipboardData(Format);
}

#define GetSystemFont(...) (HFONT)GetStockObject(SYSTEM_FONT)

HDC NTAPI LeGetDCEx(HWND hWnd, HRGN hrgnClip, DWORD flags)
{
    HDC             DC;
    HFONT           Font;
    PLeGlobalData   GlobalData = LeGetGlobalData();

    DC = GlobalData->GetDCEx(hWnd, hrgnClip, flags);
    //if (hWnd == nullptr)
    {
        Font = GetSystemFont(GlobalData, DC);
        if (Font != nullptr)
        {
            SelectObject(DC, Font);
            //DeleteObject(Font);
        }
    }

    return DC;
}

HDC NTAPI LeGetDC(HWND hWnd)
{
    HDC             DC;
    HFONT           Font;
    PLeGlobalData   GlobalData = LeGetGlobalData();

    DC = GlobalData->GetDC(hWnd);
    //if (hWnd == nullptr)
    {
        Font = GetSystemFont(GlobalData, DC);
        if (Font != nullptr)
        {
            SelectObject(DC, Font);
            //DeleteObject(Font);
        }
    }

    return DC;
}

HDC NTAPI LeGetWindowDC(HWND hWnd)
{
    HDC             DC;
    HFONT           Font;
    PLeGlobalData   GlobalData = LeGetGlobalData();

    DC = GlobalData->GetWindowDC(hWnd);
    //if (hWnd == nullptr)
    {
        Font = GetSystemFont(GlobalData, DC);
        if (Font != nullptr)
        {
            SelectObject(DC, Font);
            //DeleteObject(Font);
        }
    }

    return DC;
}

HDC NTAPI LeBeginPaint(HWND hWnd, LPPAINTSTRUCT lpPaint)
{
    HDC             DC;
    HFONT           Font;
    PLeGlobalData   GlobalData = LeGetGlobalData();

    DC = GlobalData->BeginPaint(hWnd, lpPaint);
    Font = GetFontFromDC(GlobalData, DC);
    if (Font != nullptr)
    {
        SelectObject(DC, Font);
        DeleteObject(Font);
    }

    return DC;
}

/************************************************************************
  init
************************************************************************/

PVOID FindNtUserMessageCall(PVOID User32)
{
    PVOID NtUserMessageCall = nullptr;
    PVOID SendNotifyMessageW = LookupExportTable(User32, USER32_SendNotifyMessageW);

    NtUserMessageCall = WalkOpCodeT(SendNotifyMessageW, 0x40,
        WalkOpCodeM(Buffer, OpLength, Ret)
        {
            switch (Buffer[0])
            {
                case CALL:
                    Buffer = GetCallDestination(Buffer);
                    if (IsSystemCall(Buffer) == FALSE)
                        break;

                    Ret = Buffer;
                    return STATUS_SUCCESS;

                case 0xC2:
                    return STATUS_TIMEOUT;
            }

            return STATUS_NOT_FOUND;
        }
    );

    return NtUserMessageCall;
}

PVOID FindNtUserMessageCall2(PVOID User32)
{
    ULONG Phase;
    PVOID NtUserMessageCall;
    PVOID GlobalRoutines[3];
    PVOID fnCOPYGLOBALDATA[3];

    GlobalRoutines[0] = IATLookupRoutineByHash(User32, KERNEL32_GlobalLock);
    GlobalRoutines[1] = IATLookupRoutineByHash(User32, KERNEL32_GlobalUnlock);
    GlobalRoutines[2] = IATLookupRoutineByHash(User32, KERNEL32_GlobalFree);

    NtUserMessageCall = nullptr;

    Phase = 0;

    static const ULONG FinalPhase = countof(GlobalRoutines);

    WalkRelocTableT(User32,
        WalkRelocCallbackM(ImageBase, RelocationEntry, Offset, Context)
        {
            PVOID *VA = (PVOID *)PtrAdd(ImageBase, RelocationEntry->VirtualAddress + Offset->Offset);

            if (*VA != GlobalRoutines[Phase])
            {
                Phase = 0;
                return 0;
            }

            fnCOPYGLOBALDATA[Phase++] = VA;

            if (Phase != FinalPhase)
                return 0;

            PBYTE Begin, End;

            Begin = (PBYTE)fnCOPYGLOBALDATA[0] + sizeof(*VA);
            End   = (PBYTE)fnCOPYGLOBALDATA[1] + sizeof(*VA);

            WalkOpCodeT(Begin, End - Begin,
                WalkOpCodeM(Buffer, OpLength, Ret)
                {
                    switch (Buffer[0])
                    {
                        case CALL:
                            SEH_TRY
                            {
                                Buffer = GetCallDestination(Buffer);
                                if (IsSystemCall(Buffer) == FALSE)
                                    break;
                            }
                            SEH_EXCEPT(EXCEPTION_EXECUTE_HANDLER)
                            {
                                break;
                            }

                            NtUserMessageCall = Buffer;
                            return STATUS_SUCCESS;
                    }

                    return STATUS_NOT_FOUND;
                }
            );

            if (NtUserMessageCall == nullptr)
            {
                Phase = 0;
                return STATUS_SUCCESS;
            }

            return STATUS_UNSUCCESSFUL;
        },
        0
    );

    return NtUserMessageCall;
}

PVOID FindNtUserCreateWindowEx(PVOID User32)
{
    PVOID NtUserCreateWindowEx, IATRtlQueryInformationActiveActivationContext;

    IATRtlQueryInformationActiveActivationContext = IATLookupRoutineByHash(User32, NTDLL_RtlQueryInformationActiveActivationContext);
    if (IATRtlQueryInformationActiveActivationContext == nullptr)
        return nullptr;

    NtUserCreateWindowEx = nullptr;

    WalkRelocTableT(User32,
        WalkRelocCallbackM(ImageBase, RelocationEntry, Offset, Context)
        {
            PVOID *VA = (PVOID *)PtrAdd(ImageBase, RelocationEntry->VirtualAddress + Offset->Offset);

            if (*VA != IATRtlQueryInformationActiveActivationContext)
                return 0;

            VA = PtrAdd(VA, -2);
            if (*(PUSHORT)VA != 0x15FF) // call dword ptr [RtlQueryInformationActiveActivationContext]
                return 0;

            WalkOpCodeT(VA, 0x150,
                WalkOpCodeM(Buffer, OpLength, Ret)
                {
                    switch (Buffer[0])
                    {
                        case 0xE8:
                            Buffer = GetCallDestination(Buffer);
                            if (!IsSystemCall(Buffer))
                                break;

                            NtUserCreateWindowEx = Buffer;
                            return STATUS_SUCCESS;

                        case 0xC2:
                        case 0xC3:
                            return STATUS_NOTHING_TO_TERMINATE;
                    }

                    return STATUS_NOT_FOUND;
                }
            );

            return NtUserCreateWindowEx == nullptr ? STATUS_SUCCESS : STATUS_UNSUCCESSFUL;
        },
        0
    );

    return NtUserCreateWindowEx;
}

PVOID FindNtUserDefSetText(PVOID User32)
{
    PLDR_MODULE Module;

    PVOID Start, End;

    PVOID NotifyWinEvent;
    PVOID push_EVENT_OBJECT_NAMECHANGE;
    PVOID CallNotifyWinEvent;
    PVOID DefSetText;
    PVOID NtUserDefSetText;

    BYTE Stubpush_EVENT_OBJECT_NAMECHANGE[] =
    {
        0x68, 0x0C, 0x80, 0x00, 0x00,
    };

    SEARCH_PATTERN_DATA PushEVENT_OBJECT_NAMECHANGE[] =
    {
        ADD_PATTERN(Stubpush_EVENT_OBJECT_NAMECHANGE),
    };

    push_EVENT_OBJECT_NAMECHANGE    = nullptr;
    CallNotifyWinEvent              = nullptr;
    DefSetText                      = nullptr;
    NtUserDefSetText                = nullptr;

    *(PVOID *)&NotifyWinEvent = LookupExportTable(User32, USER32_NotifyWinEvent);

    Module = FindLdrModuleByHandle(User32);

    Start = PtrAdd(User32, IMAGE_FIRST_SECTION((PIMAGE_NT_HEADERS)PtrAdd(User32, ((PIMAGE_DOS_HEADER)User32)->e_lfanew))->VirtualAddress);
    End = PtrAdd(Start, Module->SizeOfImage - PtrOffset(Start, User32));

    while (Start < End)
    {
        push_EVENT_OBJECT_NAMECHANGE = SearchPattern(
                                            PushEVENT_OBJECT_NAMECHANGE,
                                            countof(PushEVENT_OBJECT_NAMECHANGE),
                                            Start,
                                            PtrOffset(End, Start)
                                        );

        if (push_EVENT_OBJECT_NAMECHANGE == nullptr)
            return nullptr;

        WalkOpCodeT(push_EVENT_OBJECT_NAMECHANGE, 0x10,
            WalkOpCodeM(Buffer, OpLength, Ret)
            {
                if (Buffer[0] != CALL)
                    return STATUS_NOT_FOUND;

                if (GetCallDestination(Buffer) != NotifyWinEvent)
                    return STATUS_NOT_FOUND;

                CallNotifyWinEvent = Buffer;

                return STATUS_SUCCESS;
            }
        );

        if (CallNotifyWinEvent != nullptr)
            break;

        Start = PtrAdd(push_EVENT_OBJECT_NAMECHANGE, 1);
    }

    if (CallNotifyWinEvent == nullptr)
        return nullptr;

    Start = User32;

    for (ULONG_PTR Length = 0x10; Length; --Length)
    {
        PBYTE Buffer = (PBYTE)PtrSub(push_EVENT_OBJECT_NAMECHANGE, Length);
        if (Buffer[0] != CALL)
            continue;

        DefSetText = GetCallDestination(Buffer);
        if (!IN_RANGEEX(Start, DefSetText, End))
        {
            DefSetText = nullptr;
            continue;
        }

        if (*(PULONG)DefSetText != 0x8B55FF8B)
        {
            DefSetText = nullptr;
            continue;
        }

        break;
    }

    if (DefSetText == nullptr)
        return nullptr;

    WalkOpCodeT(DefSetText, 0x50,
        WalkOpCodeM(Buffer, OpLength, Ret)
        {
            switch (Buffer[0])
            {
                case CALL:
                    Buffer = GetCallDestination(Buffer);
                    if (!IsSystemCall(Buffer))
                        break;

                    NtUserDefSetText = Buffer;
                    return STATUS_SUCCESS;

                case 0xC9:  // leave
                case 0xC2:
                case 0xC3:
                    return STATUS_SUCCESS;
            }

            return STATUS_NOT_FOUND;
        }
    );

    return NtUserDefSetText;
}

LRESULT CALLBACK SendMessageWorkerProbe(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
    PVOID *StackFrame, *PrevFrame;

    if (Message != WM_SETTEXT)
        return 0;

    AllocStack(1);

    StackFrame = (PVOID *)_AddressOfReturnAddress() - 1;
    PrevFrame = nullptr;
    while (*StackFrame < (PVOID)lParam)
    {
        PrevFrame = StackFrame;
        StackFrame = (PVOID *)*StackFrame;
    }

    if (PrevFrame != nullptr)
    {
        PBYTE Buffer = (PBYTE)PrevFrame[1] - 5;

        if (Buffer[0] == CALL)
            return (LRESULT)GetCallDestination(Buffer);
    }

    return 0;
}

PVOID FindSendMessageWorker(PVOID User32)
{
    API_POINTER(CreateWindowExA)    CreateWindowExA;
    API_POINTER(SetWindowLongPtrA)  SetWindowLongPtrA;
    API_POINTER(SendMessageA)       SendMessageA;
    API_POINTER(DestroyWindow)      DestroyWindow;

    HWND        ProbeButton;
    LONG_PTR    WndProc;
    PVOID       SendMessageWorker;

    *(PVOID *)&CreateWindowExA      = LookupExportTable(User32, USER32_CreateWindowExA);
    *(PVOID *)&SetWindowLongPtrA    = LookupExportTable(User32, USER32_SetWindowLongA);
    *(PVOID *)&SendMessageA         = LookupExportTable(User32, USER32_SendMessageA);
    *(PVOID *)&DestroyWindow        = LookupExportTable(User32, USER32_DestroyWindow);

    ProbeButton         = CreateWindowExA(0, WC_BUTTONA, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    WndProc             = SetWindowLongPtrA(ProbeButton, GWLP_WNDPROC, (LONG_PTR)SendMessageWorkerProbe);
    SendMessageWorker   = (PVOID)SendMessageA(ProbeButton, WM_SETTEXT, 0, (LPARAM)AllocStack(1));
    SetWindowLongPtrA(ProbeButton, GWLP_WNDPROC, WndProc);
    DestroyWindow(ProbeButton);

    return SendMessageWorker;
}

NTSTATUS LeGlobalData::HookUser32Routines(PVOID User32)
{
    PVOID       NtUserMessageCall;
    PVOID       NtUserCreateWindowEx;
    PVOID       NtUserDefSetText;
    PVOID       SendMessageWorker;
    PVOID       LeNtUserCreateWindowEx;
    NTSTATUS    Status;

    Status = NtAddAtom(PROP_WINDOW_ANSI_PROC, CONST_STRLEN(PROP_WINDOW_ANSI_PROC) * sizeof(WCHAR), &this->AtomAnsiProc);
    FAIL_RETURN(Status);
/*
    Status = NtAddAtom(PROP_WINDOW_UNICODE_PROC, CONST_STRLEN(PROP_WINDOW_UNICODE_PROC) * sizeof(WCHAR), &this->AtomUnicodeProc);
    FAIL_RETURN(Status);
*/
    if (this->HasWin32U)
    {
        HMODULE Win32uMod = (HMODULE)Nt_GetModuleHandle(L"win32u.dll");
        if (Win32uMod == nullptr)
            return STATUS_NOT_FOUND;
        NtUserMessageCall = Nt_GetProcAddress(Win32uMod, "NtUserMessageCall");
        NtUserCreateWindowEx = Nt_GetProcAddress(Win32uMod, "NtUserCreateWindowEx");
        NtUserDefSetText = Nt_GetProcAddress(Win32uMod, "NtUserDefSetText");
        if (!NtUserMessageCall || !NtUserCreateWindowEx || !NtUserDefSetText)
            return STATUS_NOT_FOUND;
    }
    else
    {
        NtUserMessageCall = FindNtUserMessageCall2(User32);
        if (NtUserMessageCall == nullptr)
            return STATUS_NOT_FOUND;

        NtUserCreateWindowEx = FindNtUserCreateWindowEx(User32);
        if (NtUserCreateWindowEx == nullptr)
            return STATUS_NOT_FOUND;

        NtUserDefSetText = FindNtUserDefSetText(User32);
        if (NtUserDefSetText == nullptr)
            return STATUS_NOT_FOUND;
    }

    if (HookStub.StubEnumFontFamiliesExW != nullptr)
    {
        InitFontCharsetInfo();
    }

    switch (CurrentPeb()->OSMajorVersion)
    {
        case 6:
            switch (CurrentPeb()->OSMinorVersion)
            {
                case 2: // win 8
                case 3: // win 8.1
                    LeNtUserCreateWindowEx = LeNtUserCreateWindowEx_Win8;
                    break;

                case 4: // win 10
                    LeNtUserCreateWindowEx = LeNtUserCreateWindowEx_Win10;
                    break;

                default:
                    LeNtUserCreateWindowEx = LeNtUserCreateWindowEx_Win7;
                    break;
            }
            break;

        case 10: // win 10
            LeNtUserCreateWindowEx = LeNtUserCreateWindowEx_Win10;
            break;

        default:
            return STATUS_UNKNOWN_REVISION;
    }

    Mp::PATCH_MEMORY_DATA p[] =
    {
        LeFunctionJump(NtUserCreateWindowEx),
        LeFunctionJump(NtUserMessageCall),
        LeFunctionJump(NtUserDefSetText),

        LeHookFromEAT(User32, USER32, SetWindowLongA),
        LeHookFromEAT(User32, USER32, GetWindowLongA),
        LeHookFromEAT(User32, USER32, IsWindowUnicode),

        LeHookFromEAT(User32, USER32, GetClipboardData),
        LeHookFromEAT(User32, USER32, SetClipboardData),

        LeHookFromEAT(User32, USER32, GetDC),
        LeHookFromEAT(User32, USER32, GetDCEx),
        LeHookFromEAT(User32, USER32, GetWindowDC),

        LeHookFromEAT(User32, USER32, BeginPaint),
    };

    return Mp::PatchMemory(p, countof(p));
}

NTSTATUS LeGlobalData::UnHookUser32Routines()
{
    Mp::RestoreMemory(HookStub.StubNtUserCreateWindowEx);
    Mp::RestoreMemory(HookStub.StubNtUserMessageCall);
    Mp::RestoreMemory(HookStub.StubNtUserDefSetText);

    Mp::RestoreMemory(HookStub.StubSetWindowLongA);
    Mp::RestoreMemory(HookStub.StubGetWindowLongA);
    Mp::RestoreMemory(HookStub.StubIsWindowUnicode);

    Mp::RestoreMemory(HookStub.StubGetClipboardData);
    Mp::RestoreMemory(HookStub.StubSetClipboardData);

    Mp::RestoreMemory(HookStub.StubGetDC);
    Mp::RestoreMemory(HookStub.StubGetDCEx);
    Mp::RestoreMemory(HookStub.StubGetWindowDC);

    Mp::RestoreMemory(HookStub.StubBeginPaint);

    if (AtomAnsiProc != 0)
    {
        NtDeleteAtom(AtomAnsiProc);
        AtomAnsiProc = 0;
    }
/*
    if (AtomUnicodeProc != NULL)
    {
        NtDeleteAtom(AtomUnicodeProc);
        AtomUnicodeProc = NULL;
    }
*/
    return 0;
}
