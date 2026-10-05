// Decompiled by Claude Opus 5.5. Names are provisional.
#include <windows.h>
#include <string.h>

void __cdecl FUN_004d8e60(EXCEPTION_POINTERS* exception, const char* message);
void FUN_004d8390(void);
void FUN_004da0c0(void);

// The fatal error handler: appends `message` to ErrorLog.txt beside the exe,
// breaks into the debugger with a register dump and stack trace, shows the
// message and shuts down.
//
// In a function with a __try every local gets a frame slot, one scope after
// another and by name within a scope, so `length` (otherwise unused) and the
// block that declares `sep`, `file` and `written` give the original's frame:
// `written` at ebp-0x40c.
// FUNCTION: 0x4d9ab0
void __cdecl FUN_004d9ab0(const char* message)
{
    char path[1000];
    DWORD length;
    if (message && (length = GetModuleFileNameA(0, path, sizeof path)) > 0) {
        char* sep = strrchr(path, '\\');
        HANDLE file;
        DWORD written;
        if (sep)
            sep[1] = 0;
        else
            strcpy(path, "C:\\");
        strcat(path, "ErrorLog.txt");
        file = CreateFileA(path, GENERIC_WRITE, 0, 0, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
        if (file) {
            SetFilePointer(file, 0, 0, FILE_END);
            WriteFile(file, message, strlen(message), &written, 0);
            WriteFile(file, "\r\n\r\n", strlen("\r\n\r\n"), &written, 0);
            CloseHandle(file);
        }
    }
    __try {
        __asm int 3
    } __except (FUN_004d8e60(GetExceptionInformation(), "fatal error handler"), EXCEPTION_EXECUTE_HANDLER) {
    }
    if (message)
        MessageBoxA(0, message, "Cavedog", MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL | MB_TOPMOST);
    FUN_004d8390();
    FUN_004da0c0();
}
