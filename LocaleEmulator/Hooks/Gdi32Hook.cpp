#include "stdafx.h"

ULONG (NTAPI *GdiGetCodePage)(HDC NewDC);

HFONT GetFontFromFont(PLeGlobalData GlobalData, HFONT Font)
{
    LOGFONTW LogFont;

    if (GetObjectW(Font, sizeof(LogFont), &LogFont) == 0)
        return nullptr;

    LogFont.lfCharSet = GlobalData->GetLeb()->DefaultCharset;
    Font = CreateFontIndirectW(&LogFont);

    return Font;
}

HFONT GetFontFromDC(PLeGlobalData GlobalData, HDC hDC)
{
    HFONT       Font;
    LOGFONTW    LogFont;

    Font = (HFONT)GetCurrentObject(hDC, OBJ_FONT);
    if (Font == nullptr)
        return nullptr;

    return GetFontFromFont(GlobalData, Font);
}

BOOL IsGdiHookBypassed()
{
    return FindThreadFrame(GDI_HOOK_BYPASS) != nullptr;
}

HFONT CreateFontIndirectBypassA(CONST LOGFONTA *lplf)
{
    TEB_ACTIVE_FRAME Bypass(GDI_HOOK_BYPASS);
    Bypass.Push();

    return CreateFontIndirectA(lplf);
}

HFONT CreateFontIndirectBypassW(CONST LOGFONTW *lplf)
{
    TEB_ACTIVE_FRAME Bypass(GDI_HOOK_BYPASS);
    Bypass.Push();

    return CreateFontIndirectW(lplf);
}

VOID LeGlobalData::GetTextMetricsAFromLogFont(PTEXTMETRICA TextMetricA, CONST LOGFONTW *LogFont)
{
    HDC     hDC;
    HFONT   Font, OldFont;
    ULONG   GraphicsMode;

    hDC = this->CreateCompatibleDC(nullptr);
    if (hDC == nullptr)
        return;

    GraphicsMode = SetGraphicsMode(hDC, GM_ADVANCED);

    LOOP_ONCE
    {
        Font = CreateFontIndirectBypassW(LogFont);
        if (Font == nullptr)
            break;

        OldFont = (HFONT)SelectObject(hDC, Font);
        if (OldFont != nullptr)
        {
            TextMetricA->tmCharSet = 0x80;
            GetTextMetricsA(hDC, TextMetricA);
        }

        SelectObject(hDC, OldFont);
        DeleteObject(Font);
    }

    DeleteDC(hDC);
}

VOID LeGlobalData::GetTextMetricsWFromLogFont(PTEXTMETRICW TextMetricW, CONST LOGFONTW *LogFont)
{
    HDC     hDC;
    HFONT   Font, OldFont;
    ULONG   GraphicsMode;

    hDC = this->CreateCompatibleDC(nullptr);
    if (hDC == nullptr)
        return;

    GraphicsMode = SetGraphicsMode(hDC, GM_ADVANCED);

    LOOP_ONCE
    {
        Font = CreateFontIndirectBypassW(LogFont);
        if (Font == nullptr)
            break;

        OldFont = (HFONT)SelectObject(hDC, Font);
        if (OldFont != nullptr)
            GetTextMetricsW(hDC, TextMetricW);

        SelectObject(hDC, OldFont);
        DeleteObject(Font);
    }

    DeleteDC(hDC);
}

PTEXT_METRIC_INTERNAL LeGlobalData::GetTextMetricFromCache(LPENUMLOGFONTEXW LogFont)
{
    WCHAR buf[LF_FULLFACESIZE * 2];

    StringUpperW(buf, swprintf(buf, L"%s@%d", LogFont->elfFullName, LogFont->elfLogFont.lfCharSet));

    return this->TextMetricCache.Get((PWSTR)buf);
}

VOID LeGlobalData::AddTextMetricToCache(LPENUMLOGFONTEXW LogFont, PTEXT_METRIC_INTERNAL TextMetric)
{
    WCHAR buf[LF_FULLFACESIZE * 2];

    StringUpperW(buf, swprintf(buf, L"%s@%d", LogFont->elfFullName, LogFont->elfLogFont.lfCharSet));

    this->TextMetricCache.Add((PWSTR)buf, *TextMetric);
}

HGDIOBJ NTAPI LeGetStockObject(LONG Object)
{
    HGDIOBJ         StockObject;
    PULONG_PTR      Index;
    PLeGlobalData   GlobalData = LeGetGlobalData();

    StockObject = nullptr;

    static ULONG_PTR StockObjectIndex[] =
    {
        ANSI_FIXED_FONT,
        ANSI_VAR_FONT,
        DEVICE_DEFAULT_FONT,
        DEFAULT_GUI_FONT,
        OEM_FIXED_FONT,
        SYSTEM_FONT,
        SYSTEM_FIXED_FONT,
    };

    if (!GlobalData->HookRoutineData.Gdi32.StockObjectInitialized)
    {
        PROTECT_SECTION(&GlobalData->HookRoutineData.Gdi32.GdiLock)
        {
            if (GlobalData->HookRoutineData.Gdi32.StockObjectInitialized)
                break;

            FOR_EACH(Index, StockObjectIndex, countof(StockObjectIndex))
            {
                GlobalData->HookRoutineData.Gdi32.StockObject[*Index] = GetFontFromFont(GlobalData, (HFONT)GlobalData->GetStockObject(*Index));
            }

            GlobalData->HookRoutineData.Gdi32.StockObjectInitialized = TRUE;
        }
    }

    LOOP_ONCE
    {
        if (Object > countof(GlobalData->HookRoutineData.Gdi32.StockObject))
            break;

        StockObject = GlobalData->HookRoutineData.Gdi32.StockObject[Object];

        if (StockObject != nullptr)
            return StockObject;
    }

    return GlobalData->GetStockObject(Object);
}

BOOL NTAPI LeDeleteObject(HGDIOBJ GdiObject)
{
    HGDIOBJ*        StockObject;
    PLeGlobalData   GlobalData = LeGetGlobalData();

    if (GdiObject == nullptr || GlobalData->HookRoutineData.Gdi32.StockObjectInitialized == FALSE)
        return TRUE;

    FOR_EACH_ARRAY(StockObject, GlobalData->HookRoutineData.Gdi32.StockObject)
    {
        if (GdiObject == *StockObject)
            return TRUE;
    }

    return GlobalData->DeleteObject(GdiObject);
}

HDC NTAPI LeCreateCompatibleDC(HDC hDC)
{
    HDC             NewDC;
    HFONT           Font;
    LOGFONTW        LogFont;
    PLeGlobalData   GlobalData = LeGetGlobalData();

    NewDC = GlobalData->CreateCompatibleDC(hDC);

    if (NewDC == nullptr)
        return NewDC;

    Font = (HFONT)GetCurrentObject(NewDC, OBJ_FONT);
    if (Font == nullptr)
        return NewDC;

    SelectObject(NewDC, GetStockObject(SYSTEM_FONT));

    return NewDC;
}

NTSTATUS
LeGlobalData::
GetNameRecordFromNameTable(
    PVOID           TableBuffer,
    ULONG_PTR       TableSize,
    ULONG_PTR       NameID,
    ULONG_PTR       LanguageID,
    PUNICODE_STRING Name
)
{
    using namespace Gdi;

    ULONG_PTR               StorageOffset, NameRecordCount;
    PTT_NAME_TABLE_HEADER   NameHeader;
    PTT_NAME_RECORD         NameRecord, NameRecordUser, NameRecordEn;

    NameHeader = (PTT_NAME_TABLE_HEADER)TableBuffer;

    NameRecordCount = Bswap(NameHeader->NameRecordCount);
    StorageOffset = Bswap(NameHeader->StorageOffset);

    if (StorageOffset >= TableSize)
        return STATUS_NOT_SUPPORTED;

    if (StorageOffset < NameRecordCount * sizeof(*NameRecord))
        return STATUS_NOT_SUPPORTED;

    LanguageID      = Bswap((USHORT)LanguageID);
    NameRecordUser  = nullptr;
    NameRecordEn    = nullptr;
    NameRecord      = (PTT_NAME_RECORD)(NameHeader + 1);

    FOR_EACH(NameRecord, NameRecord, NameRecordCount)
    {
        if (NameRecord->PlatformID != TT_PLATFORM_ID_WINDOWS)
            continue;

        if (NameRecord->EncodingID != TT_ENCODEING_ID_UTF16_BE)
            continue;

        if (NameRecord->NameID != NameID)
            continue;

        if (NameRecord->LanguageID == Bswap((USHORT)0x0409))
            NameRecordEn = NameRecord;

        if (NameRecord->LanguageID != LanguageID)
            continue;

        NameRecordUser = NameRecord;
        break;
    }

    NameRecordUser = NameRecordUser == nullptr ? NameRecordEn : NameRecordUser;
    if (NameRecordUser == nullptr)
        return STATUS_NOT_FOUND;

    PWSTR       FaceName, Buffer;
    ULONG_PTR   Offset, Length;

    Offset = StorageOffset + Bswap(NameRecordUser->StringOffset);
    Length = Bswap(NameRecordUser->StringLength);
    FaceName = (PWSTR)PtrAdd(TableBuffer, Offset);

    Buffer = Name->Buffer;
    Length = (USHORT)ML_MIN(Length, Name->MaximumLength);
    Name->Length = Length;

    for (ULONG_PTR Index = 0; Index != Length / sizeof(WCHAR); ++Index)
    {
        Buffer[Index] = Bswap(FaceName[Index]);
    }

    if (Length < Name->MaximumLength)
        *PtrAdd(Buffer, Length) = 0;

    return STATUS_SUCCESS;
}

NTSTATUS LeGlobalData::AdjustFontDataInternal(PADJUST_FONT_DATA AdjustData)
{
    NTSTATUS        Status;
    PVOID           Table;
    ULONG_PTR       TableSize, TableName;
    WCHAR           FaceNameBuffer[LF_FACESIZE];
    WCHAR           FullNameBuffer[LF_FULLFACESIZE];
    UNICODE_STRING  FaceName, FullName;

    if (FLAG_ON(AdjustData->FontType, RASTER_FONTTYPE))
        return STATUS_NOT_SUPPORTED;

    TableName = Gdi::TT_TABLE_TAG_NAME;
    TableSize = GetFontData(AdjustData->DC, TableName, 0, 0, 0);
    if (TableSize == GDI_ERROR)
        return STATUS_OBJECT_NAME_NOT_FOUND;

    Table = AllocStack(TableSize);
    TableSize = GetFontData(AdjustData->DC, TableName, 0, Table, TableSize);
    if (TableSize == GDI_ERROR)
        return STATUS_OBJECT_NAME_NOT_FOUND;

    RtlInitEmptyString(&FaceName, FaceNameBuffer, sizeof(FaceNameBuffer));
    RtlInitEmptyString(&FullName, FullNameBuffer, sizeof(FullNameBuffer));

    Status = this->GetNameRecordFromNameTable(
                Table,
                TableSize,
                Gdi::TT_NAME_ID_FACENAME,
                this->GetLePeb()->OriginalLocaleID,
                &FaceName
            );

    PCWSTR lfFaceName = AdjustData->EnumLogFontEx->elfLogFont.lfFaceName;
    BOOL Vertical = lfFaceName[0] == '@';

    if (NT_FAILED(Status) || wcsicmp(FaceName.Buffer, lfFaceName + Vertical) != 0)
        return STATUS_CONTEXT_MISMATCH;

    Status = this->GetNameRecordFromNameTable(
                Table,
                TableSize,
                Gdi::TT_NAME_ID_FACENAME,
                this->GetLeb()->LocaleID,
                &FaceName
            );

    Status = NT_SUCCESS(Status) ?
                this->GetNameRecordFromNameTable(
                    Table,
                    TableSize,
                    Gdi::TT_NAME_ID_FULLNAME,
                    this->GetLeb()->LocaleID,
                    &FullName)
                : Status;

    if (NT_SUCCESS(Status))
    {
        BOOL        Vertical;
        PWSTR       Buffer;
        ULONG_PTR   Length;

        Vertical = AdjustData->EnumLogFontEx->elfLogFont.lfFaceName[0] == '@';

        Buffer = AdjustData->EnumLogFontEx->elfLogFont.lfFaceName + Vertical;
        Length = ML_MIN(sizeof(AdjustData->EnumLogFontEx->elfLogFont.lfFaceName) - Vertical, FaceName.Length);
        CopyMemory(Buffer, FaceName.Buffer, Length);
        *PtrAdd(Buffer, Length) = 0;

        Buffer = AdjustData->EnumLogFontEx->elfFullName + Vertical;
        Length = ML_MIN(sizeof(AdjustData->EnumLogFontEx->elfFullName) - Vertical, FullName.Length);
        CopyMemory(Buffer, FullName.Buffer, Length);
        *PtrAdd(Buffer, Length) = 0;
    }

    return Status;
}

NTSTATUS LeGlobalData::AdjustFontData(HDC DC, LPENUMLOGFONTEXW EnumLogFontEx, PTEXT_METRIC_INTERNAL TextMetric, ULONG_PTR FontType)
{
    NTSTATUS Status;
    ADJUST_FONT_DATA AdjustData;

    ZeroMemory(&AdjustData, sizeof(AdjustData));
    AdjustData.EnumLogFontEx = EnumLogFontEx;
    AdjustData.DC = DC;

    Status = STATUS_UNSUCCESSFUL;

    LOOP_ONCE
    {
        AdjustData.Font = CreateFontIndirectBypassW(&EnumLogFontEx->elfLogFont);
        if (AdjustData.Font == nullptr)
            break;

        AdjustData.OldFont = (HFONT)SelectObject(DC, AdjustData.Font);
        if (AdjustData.OldFont == nullptr)
            break;

        Status = this->AdjustFontDataInternal(&AdjustData);
        if (Status == STATUS_CONTEXT_MISMATCH)
            break;

        if (TextMetric != nullptr)
        {
            if (GetTextMetricsA(DC, &TextMetric->TextMetricA) &&
                GetTextMetricsW(DC, &TextMetric->TextMetricW))
            {
                TextMetric->Filled = TRUE;
            }
        }
    }

    if (AdjustData.OldFont != nullptr)
        SelectObject(DC, AdjustData.OldFont);

    if (AdjustData.Font != nullptr)
        this->DeleteObject(AdjustData.Font);

    return Status;
}

VOID ConvertAnsiLogfontToUnicode(PLOGFONTW LogFontW, PLOGFONTA LogFontA)
{
    CopyMemory(LogFontW, LogFontA, PtrOffset(&LogFontA->lfFaceName, LogFontA));
    RtlMultiByteToUnicodeN(LogFontW->lfFaceName, sizeof(LogFontW->lfFaceName), nullptr, LogFontA->lfFaceName, StrLengthA(LogFontA->lfFaceName) + 1);
}

VOID ConvertUnicodeLogfontToAnsi(PLOGFONTA LogFontA, PLOGFONTW LogFontW)
{
    CopyMemory(LogFontA, LogFontW, PtrOffset(&LogFontW->lfFaceName, LogFontW));
    RtlUnicodeToMultiByteN(LogFontA->lfFaceName, sizeof(LogFontA->lfFaceName), nullptr, LogFontW->lfFaceName, (StrLengthW(LogFontW->lfFaceName) + 1) * sizeof(WCHAR));
}

VOID ConvertUnicodeTextMetricToAnsi(PTEXTMETRICA TextMetricA, CONST TEXTMETRICW *TextMetricW)
{
    TextMetricA->tmHeight           = TextMetricW->tmHeight;
    TextMetricA->tmAscent           = TextMetricW->tmAscent;
    TextMetricA->tmDescent          = TextMetricW->tmDescent;
    TextMetricA->tmInternalLeading  = TextMetricW->tmInternalLeading;
    TextMetricA->tmExternalLeading  = TextMetricW->tmExternalLeading;
    TextMetricA->tmAveCharWidth     = TextMetricW->tmAveCharWidth;
    TextMetricA->tmMaxCharWidth     = TextMetricW->tmMaxCharWidth;
    TextMetricA->tmWeight           = TextMetricW->tmWeight;
    TextMetricA->tmOverhang         = TextMetricW->tmOverhang;
    TextMetricA->tmDigitizedAspectX = TextMetricW->tmDigitizedAspectX;
    TextMetricA->tmDigitizedAspectY = TextMetricW->tmDigitizedAspectY;

    TextMetricA->tmFirstChar        = TextMetricW->tmStruckOut;
    TextMetricA->tmLastChar         = ML_MIN(0xFF, TextMetricW->tmLastChar);
    TextMetricA->tmDefaultChar      = TextMetricW->tmDefaultChar;
    TextMetricA->tmBreakChar        = TextMetricW->tmBreakChar;

    TextMetricA->tmItalic           = TextMetricW->tmItalic;
    TextMetricA->tmUnderlined       = TextMetricW->tmUnderlined;
    TextMetricA->tmStruckOut        = TextMetricW->tmStruckOut;
    TextMetricA->tmPitchAndFamily   = TextMetricW->tmPitchAndFamily;
    TextMetricA->tmCharSet          = TextMetricW->tmCharSet;
}

INT NTAPI LeEnumFontCallbackAFromW(CONST LOGFONTW *lf, CONST TEXTMETRICW *TextMetricW, DWORD FontType, LPARAM param)
{
    ENUMLOGFONTEXA          EnumLogFontA;
    LPENUMLOGFONTEXW        EnumLogFontW;
    PGDI_ENUM_FONT_PARAM    EnumParam;
    PTEXT_METRIC_INTERNAL   TextMetric;
    TEXTMETRICA             TextMetricA;
    PTEXTMETRICA            tma;

    TextMetric = FIELD_BASE(TextMetricW, TEXT_METRIC_INTERNAL, TextMetricW);
    EnumParam = (PGDI_ENUM_FONT_PARAM)param;

    EnumLogFontW = (LPENUMLOGFONTEXW)lf;
    ConvertUnicodeLogfontToAnsi(&EnumLogFontA.elfLogFont, &EnumLogFontW->elfLogFont);

    RtlUnicodeToMultiByteN((PSTR)EnumLogFontA.elfFullName, sizeof(EnumLogFontA.elfFullName), nullptr, EnumLogFontW->elfFullName, (StrLengthW(EnumLogFontW->elfFullName) + 1) * sizeof(WCHAR));
    RtlUnicodeToMultiByteN((PSTR)EnumLogFontA.elfScript, sizeof(EnumLogFontA.elfScript), nullptr, EnumLogFontW->elfScript, (StrLengthW(EnumLogFontW->elfScript) + 1) * sizeof(WCHAR));
    RtlUnicodeToMultiByteN((PSTR)EnumLogFontA.elfStyle, sizeof(EnumLogFontA.elfStyle), nullptr, EnumLogFontW->elfStyle, (StrLengthW(EnumLogFontW->elfStyle) + 1) * sizeof(WCHAR));

    if (TextMetric->VerifyMagic() == FALSE)
    {
        ConvertUnicodeTextMetricToAnsi(&TextMetricA, TextMetricW);
        tma = &TextMetricA;
    }
    else
    {
        tma = &TextMetric->TextMetricA;
    }

    return ((FONTENUMPROCA)(EnumParam->Callback))(&EnumLogFontA.elfLogFont, tma, FontType, EnumParam->lParam);
}

INT NTAPI LeEnumFontCallbackW(CONST LOGFONTW *lf, CONST TEXTMETRICW *TextMetricW, DWORD FontType, LPARAM param)
{
    NTSTATUS                Status;
    PGDI_ENUM_FONT_PARAM    EnumParam;
    TEXT_METRIC_INTERNAL    TextMetric;
    LPENUMLOGFONTEXW        EnumLogFontEx;
    ULONG_PTR               LogFontCharset;

    EnumParam = (PGDI_ENUM_FONT_PARAM)param;
    EnumLogFontEx = (LPENUMLOGFONTEXW)lf;
    LogFontCharset = EnumLogFontEx->elfLogFont.lfCharSet;

    LOOP_ONCE
    {
        PTEXT_METRIC_INTERNAL CachedTextMetric;

        CachedTextMetric = EnumParam->GlobalData->GetTextMetricFromCache(EnumLogFontEx);
        if (CachedTextMetric != nullptr)
        {
            if (CachedTextMetric->VerifyMagic() == FALSE)
                return TRUE;

            if (CachedTextMetric->Filled == FALSE)
                break;

            TextMetric.TextMetricA = CachedTextMetric->TextMetricA;
            TextMetric.TextMetricW = CachedTextMetric->TextMetricW;
            TextMetric.Filled = TRUE;
            break;
        }

        LOOP_ONCE
        {
            if (EnumParam->Charset != DEFAULT_CHARSET)
                break;

            if (lf->lfCharSet != ANSI_CHARSET &&
                lf->lfCharSet != DEFAULT_CHARSET &&
                lf->lfCharSet != EnumParam->GlobalData->GetLePeb()->OriginalCharset)
            {
                break;
            }

            EnumLogFontEx->elfLogFont.lfCharSet = EnumParam->GlobalData->GetLeb()->DefaultCharset;
        }

        //if (wcscmp(lf->lfFaceName, L"MS Gothic") == 0) _asm int 4;
        //PrintConsole(L"Original:        %s\n", lf->lfFaceName);

        Status = EnumParam->GlobalData->AdjustFontData(EnumParam->DC, EnumLogFontEx, &TextMetric, FontType);

        ENUMLOGFONTEXW Captured = *EnumLogFontEx;

        Captured.elfLogFont.lfCharSet = LogFontCharset;

        if (Status == STATUS_OBJECT_NAME_NOT_FOUND || Status == STATUS_CONTEXT_MISMATCH)
        {
            TextMetric.Magic = 0;
            EnumParam->GlobalData->AddTextMetricToCache(&Captured, &TextMetric);
            return TRUE;
        }

        EnumParam->GlobalData->AddTextMetricToCache(&Captured, &TextMetric);
    }

    if (TextMetric.Filled == FALSE)
    {
        TextMetric.TextMetricW = *TextMetricW;
        TextMetric.Magic = 0;
        EnumLogFontEx->elfLogFont.lfCharSet = LogFontCharset;
    }

    TextMetricW = &TextMetric.TextMetricW;

    return ((FONTENUMPROCW)(EnumParam->Callback))(lf, TextMetricW, FontType, EnumParam->lParam);
}

int NTAPI LeEnumFontFamiliesExW(HDC hdc, LPLOGFONTW lpLogfont, FONTENUMPROCW lpProc, LPARAM lParam, DWORD dwFlags)
{
    INT                 Result;
    GDI_ENUM_FONT_PARAM Param;
    LOGFONTW            LocalLogFont;
    PLeGlobalData       GlobalData = LeGetGlobalData();

    if (NT_FAILED(Param.Prepare(GlobalData)))
        return FALSE;

    Param.Callback      = lpProc;
    Param.GlobalData    = GlobalData;
    Param.lParam        = lParam;
    Param.Charset       = lpLogfont->lfCharSet;

    LocalLogFont = *lpLogfont;

    return GlobalData->EnumFontFamiliesExW(hdc, &LocalLogFont, LeEnumFontCallbackW, (LPARAM)&Param, dwFlags);
}

int NTAPI LeEnumFontFamiliesExA(HDC hdc, LPLOGFONTA lpLogfont, FONTENUMPROCA lpProc, LPARAM lParam, DWORD dwFlags)
{
    LOGFONTW            lf;
    PLeGlobalData       GlobalData = LeGetGlobalData();
    GDI_ENUM_FONT_PARAM Param;

    Param.Callback      = lpProc;
    Param.GlobalData    = GlobalData;
    Param.lParam        = lParam;
    Param.Charset       = lpLogfont->lfCharSet;

    ConvertAnsiLogfontToUnicode(&lf, lpLogfont);

    return LeEnumFontFamiliesExW(hdc, &lf, LeEnumFontCallbackAFromW, (LPARAM)&Param, dwFlags);
}

int NTAPI LeEnumFontFamiliesW(HDC hdc, PCWSTR lpszFamily, FONTENUMPROCW lpProc, LPARAM lParam)
{
    GDI_ENUM_FONT_PARAM Param;
    LOGFONTW            LocalLogFont;
    PLeGlobalData       GlobalData = LeGetGlobalData();

    if (NT_FAILED(Param.Prepare(GlobalData)))
        return FALSE;

    Param.Callback      = lpProc;
    Param.GlobalData    = GlobalData;
    Param.lParam        = lParam;
    Param.Charset       = DEFAULT_CHARSET;

    return GlobalData->EnumFontFamiliesW(hdc, lpszFamily, LeEnumFontCallbackW, (LPARAM)&Param);
}

int NTAPI LeEnumFontFamiliesA(HDC hdc, PCSTR lpszFamily, FONTENUMPROCA lpProc, LPARAM lParam)
{
    INT                 Result;
    PWSTR               Family;
    GDI_ENUM_FONT_PARAM Param;
    LOGFONTW            LocalLogFont;
    PLeGlobalData       GlobalData = LeGetGlobalData();

    Param.Callback      = lpProc;
    Param.GlobalData    = GlobalData;
    Param.lParam        = lParam;
    Param.Charset       = DEFAULT_CHARSET;

    if (lpszFamily != nullptr)
    {
        Family = MByteToWChar(lpszFamily);
        if (Family == nullptr)
            return FALSE;
    }
    else
    {
        Family = nullptr;
    }

    Result = LeEnumFontFamiliesW(hdc, Family, LeEnumFontCallbackAFromW, (LPARAM)&Param);
    FreeString(Family);

    return Result;
}

int NTAPI LeEnumFontsW(HDC hdc, PCWSTR lpFaceName, FONTENUMPROCW lpProc, LPARAM lParam)
{
    GDI_ENUM_FONT_PARAM Param;
    LOGFONTW            LocalLogFont;
    PLeGlobalData       GlobalData = LeGetGlobalData();

    if (NT_FAILED(Param.Prepare(GlobalData)))
        return FALSE;

    Param.Callback      = lpProc;
    Param.GlobalData    = GlobalData;
    Param.lParam        = lParam;
    Param.Charset       = DEFAULT_CHARSET;

    return GlobalData->EnumFontsW(hdc, lpFaceName, LeEnumFontCallbackW, (LPARAM)&Param);
}

int NTAPI LeEnumFontsA(HDC hdc, PCSTR lpFaceName, FONTENUMPROCA lpProc, LPARAM lParam)
{
    INT                 Result;
    PWSTR               FaceName;
    GDI_ENUM_FONT_PARAM Param;
    LOGFONTW            LocalLogFont;
    PLeGlobalData       GlobalData = LeGetGlobalData();

    Param.Callback      = lpProc;
    Param.GlobalData    = GlobalData;
    Param.lParam        = lParam;
    Param.Charset       = DEFAULT_CHARSET;

    if (lpFaceName != nullptr)
    {
        FaceName = MByteToWChar(lpFaceName);
        if (FaceName == nullptr)
            return FALSE;
    }
    else
    {
        FaceName = nullptr;
    }

    Result = LeEnumFontsW(hdc, FaceName, LeEnumFontCallbackAFromW, (LPARAM)&Param);
    FreeString(FaceName);

    return Result;
}

HFONT
NTAPI
LeNtGdiHfontCreate(
    PENUMLOGFONTEXDVW   EnumLogFont,
    ULONG               SizeOfEnumLogFont,
    LONG                LogFontType,
    LONG                Unknown,
    PVOID               FreeListLocalFont
)
{
    PENUMLOGFONTEXDVW   enumlfex;
    PLeGlobalData       GlobalData = LeGetGlobalData();

    if (EnumLogFont != nullptr && IsGdiHookBypassed() == FALSE) LOOP_ONCE
    {
        ULONG_PTR Charset;

        Charset = EnumLogFont->elfEnumLogfontEx.elfLogFont.lfCharSet;

        if (Charset != ANSI_CHARSET && Charset != DEFAULT_CHARSET)
            break;

        enumlfex = (PENUMLOGFONTEXDVW)AllocStack(SizeOfEnumLogFont);

        CopyMemory(enumlfex, EnumLogFont, SizeOfEnumLogFont);

        enumlfex->elfEnumLogfontEx.elfLogFont.lfCharSet = GlobalData->GetLeb()->DefaultCharset;

        //if (GdiGetCodePage == NULL)
        //CopyStruct(enumlfex.elfEnumLogfontEx.elfLogFont.lfFaceName, GlobalData->GetLeb()->DefaultFaceName, LF_FACESIZE);
        //AllocConsole();
        //PrintConsoleW(L"%s\n", enumlfex.elfEnumLogfontEx.elfLogFont.lfFaceName);

        EnumLogFont = enumlfex;
    }

    return GlobalData->HookStub.StubNtGdiHfontCreate(EnumLogFont, SizeOfEnumLogFont, LogFontType, Unknown, FreeListLocalFont);
}

API_POINTER(SelectObject)  StubSelectObject;

HGDIOBJ NTAPI LeSelectObject(HDC hdc, HGDIOBJ h)
{
    HGDIOBJ obj;

    obj = StubSelectObject(hdc, h);

    switch (GdiGetCodePage(hdc))
    {
        //case 0x3A4:
        case 0x3A8:
        {
            ULONG objtype = GetObjectType(h);

            union
            {
                LOGFONTW lf;
            };

            switch (objtype)
            {
                case OBJ_FONT:
                    GetObjectW(h, sizeof(lf), &lf);
                    ExceptionBox(lf.lfFaceName, L"FUCK FACE");
                    break;

                default:
                    return obj;
            }

            break;
        }
    }

    return obj;
}

/************************************************************************
  init
************************************************************************/

PVOID FindNtGdiHfontCreate(PVOID Gdi32)
{
    PVOID CreateFontIndirectExW, NtGdiHfontCreate;

    CreateFontIndirectExW = LookupExportTable(Gdi32, GDI32_CreateFontIndirectExW);

    NtGdiHfontCreate = WalkOpCodeT(CreateFontIndirectExW, 0xA0,
                            WalkOpCodeM(Buffer, OpLength, Ret)
                            {
                                switch (Buffer[0])
                                {
                                    case CALL:
                                        Buffer = GetCallDestination(Buffer);
                                        if (IsSystemCall(Buffer) == FALSE)
                                            break;

                                        Ret = Buffer;
                                        //return STATUS_SUCCESS;
                                        break;
                                }

                                return STATUS_NOT_FOUND;
                            }
                        );

    return NtGdiHfontCreate;
}


/************************************************************************
  init end
************************************************************************/

NTSTATUS LeGlobalData::HookGdi32Routines(PVOID Gdi32)
{
    PVOID NtGdiHfontCreate, Fms;

    *(PVOID *)&GdiGetCodePage = GetRoutineAddress(Gdi32, "GdiGetCodePage");

    if (this->HasWin32U)
    {
        HMODULE Win32uMod = (HMODULE)Nt_GetModuleHandle(L"win32u.dll");
        if (Win32uMod == nullptr)
            return STATUS_NOT_FOUND;
        NtGdiHfontCreate = Nt_GetProcAddress(Win32uMod, "NtGdiHfontCreate");
        if (!NtGdiGetGlyphOutline)
            return STATUS_NOT_FOUND;
    }
    else
    {
        NtGdiHfontCreate = FindNtGdiHfontCreate(Gdi32);
        if (NtGdiHfontCreate == nullptr)
            return STATUS_NOT_FOUND;
    }

    RtlInitializeCriticalSectionAndSpinCount(&HookRoutineData.Gdi32.GdiLock, 4000);

    if (HookStub.StubNtUserCreateWindowEx != nullptr)
    {
        InitFontCharsetInfo();
    }

    //Fms = Ldr::LoadDll(L"fms.dll");
    //if (Fms == nullptr)
    //{
    //    return STATUS_DLL_NOT_FOUND;
    //}
    //LdrAddRefDll(LDR_ADDREF_DLL_PIN, Fms);

    Mp::PATCH_MEMORY_DATA p[] =
    {
        LeHookFromEAT(Gdi32, GDI32, GetStockObject),
        LeHookFromEAT(Gdi32, GDI32, DeleteObject),
        //LeHookFromEAT(Gdi32, GDI32, CreateFontIndirectExW),
        LeHookFromEAT(Gdi32, GDI32, CreateCompatibleDC),
        LeHookFromEAT(Gdi32, GDI32, EnumFontsW),
        LeHookFromEAT(Gdi32, GDI32, EnumFontsA),
        LeHookFromEAT(Gdi32, GDI32, EnumFontFamiliesA),
        LeHookFromEAT(Gdi32, GDI32, EnumFontFamiliesW),
        LeHookFromEAT(Gdi32, GDI32, EnumFontFamiliesExA),
        LeHookFromEAT(Gdi32, GDI32, EnumFontFamiliesExW),

        LeFunctionJump(NtGdiHfontCreate),

        //Mp::MemoryPatchVa((ULONG_PTR)LeFmsEnumFontFamiliesExW, sizeof(ULONG_PTR), LookupImportTable(Fms, "GDI32.dll", GDI32_EnumFontFamiliesExW)),

        //Mp::FunctionJumpVa(LookupExportTable(Gdi32, GDI32_SelectObject), LeSelectObject, &StubSelectObject),
    };

    return Mp::PatchMemory(p, countof(p));
}

NTSTATUS LeGlobalData::UnHookGdi32Routines()
{
    Mp::RestoreMemory(HookStub.StubGetStockObject);
    Mp::RestoreMemory(HookStub.StubDeleteObject);
    Mp::RestoreMemory(HookStub.StubCreateCompatibleDC);
    Mp::RestoreMemory(HookStub.StubEnumFontFamiliesExA);
    Mp::RestoreMemory(HookStub.StubEnumFontFamiliesExW);
    Mp::RestoreMemory(HookStub.StubEnumFontsA);
    Mp::RestoreMemory(HookStub.StubEnumFontsW);
    Mp::RestoreMemory(HookStub.StubNtGdiHfontCreate);

    return 0;
}
