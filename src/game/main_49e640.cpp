// Decompiled by Opus. Names are provisional.
// Appends a string (with its terminating zero) to DEBUG.FIL, creating the
// file if it cannot be opened for appending.
#include <string.h>

struct File_004bb5d0;

File_004bb5d0* __stdcall FUN_004bb2c0(char* path);
File_004bb5d0* __stdcall FUN_004bb6a0(char* path);
unsigned int __stdcall FUN_004bbbe0(File_004bb5d0* file, void* data, unsigned int size);
int __stdcall FUN_004bb5d0(File_004bb5d0* file);

// FUNCTION: 0x49e640
void __stdcall AppendToDebugFile(char* text)
{
    File_004bb5d0* file = FUN_004bb2c0("DEBUG.FIL");
    if (!file)
        file = FUN_004bb6a0("DEBUG.FIL");
    FUN_004bbbe0(file, text, strlen(text) + 1);
    FUN_004bb5d0(file);
}
