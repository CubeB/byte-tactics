// Decompiled by Opus. Names are provisional.
// Appends a string (with its terminating zero) to DEBUG.FIL, creating the
// file if it cannot be opened for appending.
#include <string.h>

struct FileHandle;

FileHandle* __stdcall HAPI_OpenFileAppend(char* path);
FileHandle* __stdcall HAPI_CreateFile(char* path);
unsigned int __stdcall HAPI_WriteFile(FileHandle* file, void* data, unsigned int size);
int __stdcall HAPI_CloseFile(FileHandle* file);

// FUNCTION: 0x49e640
void __stdcall AppendToDebugFile(char* text)
{
    FileHandle* file = HAPI_OpenFileAppend("DEBUG.FIL");
    if (!file)
        file = HAPI_CreateFile("DEBUG.FIL");
    HAPI_WriteFile(file, text, strlen(text) + 1);
    HAPI_CloseFile(file);
}
