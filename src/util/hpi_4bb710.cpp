// Decompiled by Opus. Names are provisional.
#include <stdio.h>

struct Info_004bb710 {
    int unknown_0;                     // +0x0
    int size;                          // +0x4
};

struct FileHandle {
    FILE* fp;                          // +0x0
    void* shared;                      // +0x4
    Info_004bb710* info;               // +0x8
    unsigned int pos;                  // +0xc
    int* buffer;                       // +0x10
    int* buffer2;                      // +0x14
};

void __cdecl FUN_004d85a0(int* param_1);

// FUNCTION: 0x4bb710
long __stdcall HAPI_SeekFile(FileHandle* file, long pos)
{
    if (file->shared != 0) {
        unsigned int old = file->pos;
        if (pos == -1) {
            file->pos = file->info->size;
        } else {
            file->pos = pos;
        }
        if (((old ^ file->pos) & 0xffff0000) != 0 && file->buffer2 != 0) {
            FUN_004d85a0(file->buffer2);
            file->buffer2 = 0;
        }
        return 0;
    }
    int result;
    if (pos == -1) {
        result = fseek(file->fp, 0, SEEK_END);
    } else {
        result = fseek(file->fp, pos, SEEK_SET);
    }
    if (result != 0) {
        return -1;
    }
    return ftell(file->fp);
}
