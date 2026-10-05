// Decompiled by Opus. Names are provisional.
#include <stdio.h>

struct OPENHAPIFILE {
    FILE* fp;                          // +0x0
    char unknown_4[8];
    int refCount;                      // +0xc
    int field_10;                      // +0x10
};

struct FileHandle {
    FILE* fp;                          // +0x0
    OPENHAPIFILE* shared;              // +0x4
    char unknown_8[8];
    int* buffer;                       // +0x10
    int* buffer2;                      // +0x14
};

void __cdecl FUN_004d85a0(int* param_1);

// FUNCTION: 0x4bb5d0
int __stdcall HAPI_CloseFile(FileHandle* file)
{
    int result;
    if (file->shared != 0) {
        result = 0;
        file->shared->refCount--;
        if (file->shared->refCount == 0 && file->shared->field_10 == 0) {
            result = fclose(file->shared->fp);
            file->shared->fp = 0;
        }
    } else {
        result = fclose(file->fp);
    }
    if (file->buffer != 0) {
        FUN_004d85a0(file->buffer);
    }
    if (file->buffer2 != 0) {
        FUN_004d85a0(file->buffer2);
    }
    FUN_004d85a0((int*)file);
    return result;
}
