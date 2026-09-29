// Decompiled by space-bunny-free, finished by Space Bunny Free, finished by muse-spark-1.3-free. Names are provisional.
// MATCH (454 of 454 bytes).
// Copies a file into an already open destination handle, 0x19000 bytes at a
// time, and returns the number of bytes copied (0 on failure). The size to
// copy comes from the source handle: the cached block size when it is one of
// the game's packed items, else _filelength of the real file, else 0, and an
// empty source is a failure. The destination has to be a real file: when it
// is a packed item itself the write is refused by counting -1 bytes written,
// which never matches the chunk size.
// The fix from the 81.6 percent base was the return value: the success path
// reloads the size from its home in the dead name argument slot
// (mov eax, [esp+0x20]), it does not return the destination pointer from ebp.
// Returning len keeps its live range spanning the loop (esi is busy with the
// chunk count there), so MSVC homes the size in the name slot, reloads it
// into esi before the seek, copies it to the fresh local for the loop counter
// after the allocation, and reloads it again for the return. A single
// variable cannot express those two homes, and returning dst lets the single
// variable fold to 0 at the exit, which is why that shape stalled at 81.6.
#include <stdio.h>
#include <io.h>

// The block header a packed item's handle points at.
struct Info_004bcf80 {
    char unknown_0[4];
    int size;                          // +0x4
};

// One of the game's loaded items: the file all handles onto it share.
struct Item_004bcf80 {
    FILE* fp;                          // +0x0
    int pos;                           // +0x4
    void* node;                        // +0x8
    int count;                         // +0xc, handles open on this item
    int unknown_10;                    // +0x10
    char name[0x100];                  // +0x14
};

// A file handle, the same class as 0x4bb2e0's.
struct File_004bcf80 {
    FILE* fp;                          // +0x0
    Item_004bcf80* shared;             // +0x4
    Info_004bcf80* info;               // +0x8
    int pos;                           // +0xc
    void* buffer;                      // +0x10
    void* buffer2;                     // +0x14
    char name[0x100];                  // +0x18
};

File_004bcf80* __stdcall FUN_004bb2e0(char* filename, const char* mode);
long __stdcall FUN_004bb710(File_004bcf80* file, long pos);
int __stdcall FUN_004bb7c0(File_004bcf80* file, void* buf, int size);
void* FUN_004d83b0(char* name, unsigned int size);
void FUN_004d85a0(void* p);

// Drops one reference on a handle, closing and freeing whatever it still holds.
static void Close_004bcf80(File_004bcf80* f)
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
int __stdcall FUN_004bcf80(File_004bcf80* dst, char* name)
{
    int len;
    int left;
    int got;
    int written;
    void* buf = 0;
    File_004bcf80* f = FUN_004bb2e0(name, "rb");
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
    if (FUN_004bb710(f, 0) == -1)
        goto fail;
    buf = FUN_004d83b0("COPY BUFFER", 0x19000);
    left = len;
    while (left != 0) {
        got = FUN_004bb7c0(f, buf, 0x19000);
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
    return len;
fail:
    if (buf)
        FUN_004d85a0(buf);
    Close_004bcf80(f);
    return 0;
}
