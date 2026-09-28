// Decompiled by space-bunny-free. Names are provisional.
// Best result 78.2% (438 of 454 bytes). Everything matches instruction for
// instruction except five instructions, all in the copy loop's frame layout:
//   original  push ecx / pop ecx          a second local home ([esp+0x10]) we do
//                                            not get, so every esp displacement
//                                            below is 4 lower than the original
//   original  mov esi, [esp+0x1c]         the size loaded into esi, kept across
//                                            the two calls, then copied
//   original  mov [esp+0x10], esi         into the loop counter's own slot
//   original  mov eax, [esp+0x20]         the dst parameter re-read from its
//                                            argument slot instead of kept in ebp
// The original therefore homes TWO variables, the size in the dead second
// argument's slot and the loop counter in a freshly reserved dword, while MSVC 5
// always enregisters the two-use size and homes only the counter. The five
// instructions are one symptom: with one home the size is born in esi, the
// counter takes the argument slot and the parameter keeps ebp to the return.
// Tried without effect (~30 shapes on the free harness, see
// build/scratch/0x4bcf80/bodies): the size as an if/else chain, as a nested
// ternary, as a long, as a one element array, as a struct member and through a
// reference returning helper (each forces a memory home on its own and matches
// the original's chain exactly); the counter as a second int, long, array,
// inner block variable, a function-scope variable initialised after the alloc,
// a for-init copy, a reference and a struct; while, do/while and for with the
// latch in the increment clause (all three identical); every declaration order;
// a redundant second size test; a copy of dst into a local; the size as a
// reference aliasing the name parameter's slot (same code), plus a separate
// loop counter on top of it (65.3%, the size loses its memory home). The one thing that
// would explain it is a source detail that makes the size's live range reach
// into the loop, where esi holds the chunk count, so it could not be promoted.
// Copies a file into an already open destination handle, 0x19000 bytes at a
// time. The size to copy comes from the source handle: the cached block size
// when it is one of the game's packed items, else _filelength of the real
// file, else 0, and an empty source is a failure. The destination has to be a
// real file: when it is a packed item itself the write is refused by counting
// -1 bytes written, which never matches the chunk size.
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
    while (len != 0) {
        got = FUN_004bb7c0(f, buf, 0x19000);
        if (got <= 0)
            goto fail;
        if (dst->shared)
            written = -1;
        else
            written = (int)fwrite(buf, 1, got, dst->fp);
        if (written != got)
            goto fail;
        len -= got;
    }
    Close_004bcf80(f);
    return (int)dst;
fail:
    if (buf)
        FUN_004d85a0(buf);
    Close_004bcf80(f);
    return 0;
}
