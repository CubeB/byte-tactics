// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Opens a file, seeks to an offset and reads a block into the caller's
// buffer; on success the buffer is returned, otherwise null. The file handle
// is closed (and its shared refcount dropped) on both paths. The refcount,
// fclose and buffer-free blocks are the inlined body of 0x4bb5d0.
#include <stdio.h>

struct Shared_004bbd30 {
    FILE* fp;                          // +0x0
    char unknown_4[8];
    int refCount;                      // +0xc
    int field_10;                      // +0x10
};

struct File_004bbd30 {
    FILE* fp;                          // +0x0
    Shared_004bbd30* shared;           // +0x4
    char unknown_8[8];
    int* buffer;                       // +0x10
    int* buffer2;                      // +0x14
};

File_004bbd30* __stdcall FUN_004bb2e0(const char* name, const char* mode);
long __stdcall FUN_004bb710(File_004bbd30* file, long pos);
int __stdcall FUN_004bb7c0(File_004bbd30* file, void* buffer, unsigned int size);
void __cdecl FUN_004d85a0(int* param_1);

// FUNCTION: 0x4bbd30
void* __stdcall FUN_004bbd30(char* name, void* buffer, long pos, unsigned int size)
{
    File_004bbd30* file = FUN_004bb2e0(name, "rb");
    if (file == 0) {
        return 0;
    }
    if (FUN_004bb710(file, pos) == -1) {
        goto fail;
    }
    if (FUN_004bb7c0(file, buffer, size) <= 0) {
        goto fail;
    }
    if (file->shared != 0) {
        file->shared->refCount--;
        if (file->shared->refCount == 0 && file->shared->field_10 == 0) {
            fclose(file->shared->fp);
            file->shared->fp = 0;
        }
    } else {
        fclose(file->fp);
    }
    if (file->buffer != 0) {
        FUN_004d85a0(file->buffer);
    }
    if (file->buffer2 != 0) {
        FUN_004d85a0(file->buffer2);
    }
    FUN_004d85a0((int*)file);
    return buffer;

fail:
    if (file->shared != 0) {
        file->shared->refCount--;
        if (file->shared->refCount == 0 && file->shared->field_10 == 0) {
            fclose(file->shared->fp);
            file->shared->fp = 0;
        }
    } else {
        fclose(file->fp);
    }
    if (file->buffer != 0) {
        FUN_004d85a0(file->buffer);
    }
    if (file->buffer2 != 0) {
        FUN_004d85a0(file->buffer2);
    }
    FUN_004d85a0((int*)file);
    return 0;
}
