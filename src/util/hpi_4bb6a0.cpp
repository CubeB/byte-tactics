// Decompiled by Opus. Names are provisional.
// Creates a file for writing ("w+b") and wraps it in a zeroed "File Handle"
// that remembers the file name.
#include <stdio.h>
#include <string.h>

struct File_004bb6a0 {
    FILE* fp;                          // +0x0
    void* shared;                      // +0x4
    void* info;                        // +0x8
    unsigned int pos;                  // +0xc
    int* buffer;                       // +0x10
    int* buffer2;                      // +0x14
    char name[0x100];                  // +0x18
};

void* __cdecl FUN_004d83b0(char* name, unsigned int size);

// FUNCTION: 0x4bb6a0
File_004bb6a0* __stdcall FUN_004bb6a0(char* path)
{
    FILE* fp = fopen(path, "w+b");
    if (fp) {
        File_004bb6a0* file = (File_004bb6a0*)FUN_004d83b0("File Handle", sizeof(File_004bb6a0));
        memset(file, 0, sizeof(File_004bb6a0));
        strncpy(file->name, path, sizeof(file->name));
        file->name[sizeof(file->name) - 1] = 0;
        file->fp = fp;
        file->shared = 0;
        return file;
    }
    return 0;
}
