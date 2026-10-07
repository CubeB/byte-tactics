// Decompiled by space-bunny-free, finished by Space Bunny Free, finished by muse-spark-1.3-free. Names are provisional.
// Copies a file into an already open destination handle, 0x19000 bytes at a
// time, and returns the number of bytes copied (0 on failure). The size to
// copy comes from the source handle: the cached block size when it is one of
// the game's packed items, else _filelength of the real file, else 0, and an
// empty source is a failure. The destination has to be a real file: when it
// is a packed item itself the write is refused by counting -1 bytes written,
// which never matches the chunk size.
#include <stdio.h>
#include <io.h>

// The block header a packed item's handle points at.
struct Info_004bcf80 {
    char unknown_0[4];
    int size;                          // +0x4
};

// One of the game's loaded items: the file all handles onto it share.
struct OPENHAPIFILE {
    FILE* fp;                          // +0x0
    int pos;                           // +0x4
    void* node;                        // +0x8
    int count;                         // +0xc, handles open on this item
    int unknown_10;                    // +0x10
    char name[0x100];                  // +0x14
};

// A file handle, the same class as 0x4bb2e0's.
struct FileHandle {
    FILE* fp;                          // +0x0
    OPENHAPIFILE* shared;              // +0x4
    Info_004bcf80* info;               // +0x8
    int pos;                           // +0xc
    void* buffer;                      // +0x10
    void* buffer2;                     // +0x14
    char name[0x100];                  // +0x18
};

FileHandle* __stdcall HAPI_OpenFile(char* filename, const char* mode);
long __stdcall HAPI_SeekFile(FileHandle* file, long pos);
int __stdcall HAPI_readfromfile(FileHandle* file, void* buf, int size);
void* __cdecl FUN_004d83b0(char* name, unsigned int size);
void __cdecl FUN_004d85a0(void* p);

// Drops one reference on a handle, closing and freeing whatever it still holds.
static void Close_004bcf80(FileHandle* f)
{
    if (f->shared) {
        f->shared->count--;
        if (f->shared->count == 0 && f->shared->unknown_10 == 0) {
            fclose(f->shared->fp);
            f->shared->fp = 0;
        }
    } else {
        fclose(f->fp);
    }
    if (f->buffer)
        FUN_004d85a0(f->buffer);
    if (f->buffer2)
        FUN_004d85a0(f->buffer2);
    FUN_004d85a0(f);
}

// FUNCTION: 0x4bcf80
int __stdcall HAPI_CopyIntoFile(FileHandle* dst, char* name)
{
    int len;
    int left;
    int got;
    int written;
    void* buf = 0;
    FileHandle* f = HAPI_OpenFile(name, "rb");
    if (f == 0)
        return 0;
    if (f->shared)
        len = f->info->size;
    else if (f->fp)
        len = _filelength(f->fp->_file);
    else
        len = 0;
    if (len == 0)
        goto fail;
    if (HAPI_SeekFile(f, 0) == -1)
        goto fail;
    buf = FUN_004d83b0("COPY BUFFER", 0x19000);
    left = len;
    while (left != 0) {
        got = HAPI_readfromfile(f, buf, 0x19000);
        if (got <= 0)
            goto fail;
        if (dst->shared)
            written = -1;
        else
            written = (int)fwrite(buf, 1, got, dst->fp);
        if (written != got)
            goto fail;
        left -= got;
    }
    Close_004bcf80(f);
    // Returns len, kept separate from the loop counter left; not dst.
    return len;
fail:
    if (buf)
        FUN_004d85a0(buf);
    Close_004bcf80(f);
    return 0;
}
