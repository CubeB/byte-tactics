// Decompiled by Sonnet. Names are provisional.
#include <stdio.h>

struct FileHandle
{
    FILE* file;    // +0x0
    int error;     // +0x4
};

// FUNCTION: 0x4bbbe0
unsigned int __stdcall HAPI_WriteFile(FileHandle* param_1, void* param_2, unsigned int param_3)
{
    if (param_1->error != 0)
        return 0xffffffff;
    return fwrite(param_2, 1, param_3, param_1->file);
}
