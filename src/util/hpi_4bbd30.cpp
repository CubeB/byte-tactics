// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Opens a file, seeks to an offset and reads a block into the caller's
// buffer; on success the buffer is returned, otherwise null. The file handle
// is closed (and its shared refcount dropped) on both paths. The refcount,
// fclose and buffer-free blocks are the inlined body of 0x4bb5d0.
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

FileHandle* __stdcall HAPI_OpenFile(const char* name, const char* mode);
long __stdcall HAPI_SeekFile(FileHandle* file, long pos);
int __stdcall HAPI_readfromfile(FileHandle* file, void* buffer, unsigned int size);
void __cdecl FUN_004d85a0(int* param_1);

// FUNCTION: 0x4bbd30
void* __stdcall HAPI_ReadFileAt(char* name, void* buffer, long pos, unsigned int size)
{
    FileHandle* file = HAPI_OpenFile(name, "rb");
    if (file == 0) {
        return 0;
    }
    if (HAPI_SeekFile(file, pos) == -1) {
        goto fail;
    }
    if (HAPI_readfromfile(file, buffer, size) <= 0) {
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
