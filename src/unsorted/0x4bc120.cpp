// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Reads a whole file into the caller's buffer: open "rb", take its size, seek
// to the start and read that many bytes. The buffer is returned when the read
// succeeds; a read of zero bytes frees it and returns null. On any failure the
// file is closed and null is returned. The refcount/fclose/buffer-free block is
// the inlined body of FUN_004bb5d0, and the size block the inlined body of
// FUN_004bbd00, so both appear twice.
#include <stdio.h>
#include <io.h>

struct Info_004bc120 {
    int unknown_0;   // +0x0
    long size;       // +0x4
};

struct Shared_004bc120 {
    FILE* fp;        // +0x0
    char unknown_4[8];
    int refCount;    // +0xc
    int field_10;    // +0x10
};

struct File_004bc120 {
    FILE* fp;                 // +0x0
    Shared_004bc120* shared;  // +0x4
    Info_004bc120* info;      // +0x8
    unsigned int pos;         // +0xc
    int* buffer;              // +0x10
    int* buffer2;             // +0x14
};

void FUN_004d85a0(void* param_1);
File_004bc120* __stdcall FUN_004bb2e0(char* param_1, const char* param_2);
long __stdcall FUN_004bb710(File_004bc120* param_1, long param_2);
long __stdcall FUN_004bb7c0(File_004bc120* param_1, void* param_2, long param_3);

// FUNCTION: 0x4bc120
int* __stdcall FUN_004bc120(char* param_1, int* param_2)
{
    File_004bc120* file = FUN_004bb2e0(param_1, "rb");
    long size;
    if (file == 0)
        goto fail;
    if (file->shared != 0)
        size = file->info->size;
    else if (file->fp != 0)
        size = _filelength(file->fp->_file);
    else
        size = 0;
    if (size <= 0)
        goto fail;
    if (FUN_004bb710(file, 0) < 0)
        goto fail;
    size = FUN_004bb7c0(file, param_2, size);
    if (size < 0)
        goto fail;
    if (file->shared != 0) {
        file->shared->refCount--;
        if (file->shared->refCount == 0 && file->shared->field_10 == 0) {
            fclose(file->shared->fp);
            file->shared->fp = 0;
        }
    } else {
        fclose(file->fp);
    }
    if (file->buffer != 0)
        FUN_004d85a0(file->buffer);
    if (file->buffer2 != 0)
        FUN_004d85a0(file->buffer2);
    FUN_004d85a0(file);
    if (size <= 0) {
        FUN_004d85a0(param_2);
        return 0;
    }
    return param_2;
fail:
    if (file != 0) {
        if (file->shared != 0) {
            file->shared->refCount--;
            if (file->shared->refCount == 0 && file->shared->field_10 == 0) {
                fclose(file->shared->fp);
                file->shared->fp = 0;
            }
        } else {
            fclose(file->fp);
        }
        if (file->buffer != 0)
            FUN_004d85a0(file->buffer);
        if (file->buffer2 != 0)
            FUN_004d85a0(file->buffer2);
        FUN_004d85a0(file);
    }
    return 0;
}
