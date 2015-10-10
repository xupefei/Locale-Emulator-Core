#pragma comment(linker, "/ENTRY:main")
//#pragma comment(linker, "/SECTION:.text,ERW /MERGE:.rdata=.text /MERGE:.data=.text")
//#pragma comment(linker, "/SECTION:.Asuna,ERW /MERGE:.text=.Asuna")
//#pragma comment(lib, "../LoaderDll/LoaderDll.lib")

#define LE_LOADER_DLL 1

//#include "MyLibrary.cpp"
#include "../LoaderDll/LoaderDll.cpp"

ForceInline Void main2(Long_Ptr argc, WChar **argv)
{
    if (argc < 2)
        return;

    ULONG_PTR               Length;
    PWSTR                   CurrentDirectory, CommandLine;
    NTSTATUS                Status;
    UNICODE_STRING          CurrentDirectoryString;
    ML_PROCESS_INFORMATION  ProcessInfo;

    RtlCreateUnicodeString(&CurrentDirectoryString, argv[1]);

    *findnamew(CurrentDirectoryString.Buffer) = 0;
    CurrentDirectory = CurrentDirectoryString.Buffer[0] == 0 ? NULL : CurrentDirectoryString.Buffer;

    CommandLine = CurrentPeb()->ProcessParameters->CommandLine.Buffer;
    for (WCHAR end = CommandLine[0] == '"' ? '"' : ' '; *++CommandLine != end && CommandLine[0] != 0; );

    if (CommandLine[0] == 0)
    {
        CommandLine = NULL;
    }
    else
    {
        ++CommandLine;
        while (CommandLine[0] == ' ' || CommandLine[0] == '\t')
            ++CommandLine;
    }

    Status = LeCreateProcess(nullptr, argv[1], CommandLine, CurrentDirectory);

    RtlFreeUnicodeString(&CurrentDirectoryString);

    PrintConsoleW(L"%p\n", Status);
    Ps::Sleep(1000);

    return;
}

int __cdecl main(Long_Ptr argc, wchar_t **argv)
{
    getargsW(&argc, &argv);
    main2(argc, argv);
    ReleaseArgv(argv);
    Ps::ExitProcess(0);
}
