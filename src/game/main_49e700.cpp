// Decompiled by space-bunny-free. Names are provisional.
// Out of memory handler: appends a note to ErrorLog.txt beside the
// executable, then reports and aborts.
#include <windows.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>

void __stdcall ReportViaException(char* text);
void FUN_004d8390(void);

// FUNCTION: 0x49e700
void OutOfMemoryHandler()
{
    char path[1000];
    DWORD written;
    HANDLE file;
    char* slash;

    GetModuleFileNameA(NULL, path, 1000);
    slash = strrchr(path, '\\');
    if (slash)
        slash[1] = 0;
    else
        strcpy(path, "C:\\");
    strcat(path, "ErrorLog.txt");
    file = CreateFileA(path, GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (file) {
        SetFilePointer(file, 0, NULL, FILE_END);
        WriteFile(file, "Out of memory!\r\nYour hard disk may be full\r\n",
                  strlen("Out of memory!\r\nYour hard disk may be full\r\n"), &written, NULL);
        CloseHandle(file);
    }
    ReportViaException("Out of memory handler");
    MessageBoxA(NULL, "Out of memory!\r\nYour hard disk may be full\r\n", "Total Annihilation", 0x41010);
    FUN_004d8390();
    raise(SIGABRT);
    _exit(3);
}
