#ifndef _RUNTIME_H_c7568f49_2cb3_4dbb_8cd3_a913c77cddcc_
#define _RUNTIME_H_c7568f49_2cb3_4dbb_8cd3_a913c77cddcc_

#include "LocaleEmulator.h"

inline PWSTR AllocUnicode(ULONG_PTR Length)
{
    return (PWSTR)AllocateMemoryP(Length * sizeof(WCHAR));
}

inline PSTR AllocAnsi(ULONG_PTR Length)
{
    return (PSTR)AllocateMemoryP(Length);
}

PSTR
WCharToMByte(
    PCWSTR      Unicode,
    ULONG_PTR   Length = -1
);

PWSTR
MByteToWChar(
    PCSTR       AnsiString,
    ULONG_PTR   Length = -1
);

VOID
FreeString(
    PVOID String
);

PSTR
ClassWCharToMByte(
    PCWSTR Unicode
);

PWSTR
ClassMByteToWChar(
    PCSTR AnsiString
);

PSTR
TitleWCharToMByte(
    PCWSTR Unicode
);

PWSTR
TitleMByteToWChar(
    PCSTR AnsiString
);

BOOL
FreeClass(
    PVOID ClassName
);

VOID
InitEmptyLargeString(
    PLARGE_UNICODE_STRING String
);

PLARGE_UNICODE_STRING
LargeStringAnsiToUnicode(
    PLARGE_UNICODE_STRING LargeAnsiString,
    PLARGE_UNICODE_STRING LargeUnicodeString
);

VOID
FreeLargeString(
    PLARGE_UNICODE_STRING LargeString
);

PLARGE_UNICODE_STRING
CaptureAnsiWindowName(
    PLARGE_UNICODE_STRING WindowName,
    PLARGE_UNICODE_STRING UnicodeWindowName
);

BOOL
IsSystemCall(
    PVOID Routine
);

#endif // _RUNTIME_H_c7568f49_2cb3_4dbb_8cd3_a913c77cddcc_
