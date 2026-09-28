// Decompiled by space-bunny-free, finished by Space Bunny Free. Names are provisional.
// Best result 81.6% (440 of 454 bytes). The whole control flow, both copies of
// the close block, the argument order and every call sequence match. What is
// left is one frame layout, and the FPO record says the original has 1 dword of
// locals, so the original has a real local dword (`push ecx` / `pop ecx`)
// that this source only half produces: the `push ecx` and the loop's home in
// the saved esi slot are right, but the three-way size chain is written here
// with the "else 0" as an initialiser, so it has two branches and one extra
// store before the open call instead of three branches:
//   original  push ebx,ebx before the pushes / mov [esp+0x18],ebx
//            the original zeroes ebx after the two argument pushes and has no
//            store before the call; it stores the 0 in the third branch
//   original  mov [esp+0x1c],edx / eax / ebx
//            the three chain stores, all in the real local at esp0-4; ours
//            stores the two branch results in the saved esi slot (esp0-16)
//   original  mov esi, [esp+0x1c]   the chain result reloaded into esi at the
//            merge and kept there across both calls, then
//   original  mov [esp+0x10], esi   copied into the loop counter's own home
//   original  mov eax, [esp+0x20]   the destination reloaded from its
//            argument slot for the return instead of taken from ebp
// So the original homes TWO variables, the chain result in the fresh local and
// the loop counter in the saved esi slot, and it keeps a register copy of the
// chain result across the two calls. The two of them are what the 78.2% shape
// (a single variable) cannot express, and every two-variable source I could
// write makes MSVC if-convert the chain into esi and drop its home (65%).
// THE CLOSEST THING FOUND, AND WHY IT IS NOT IN THIS FILE: give the source a
// second variable for the loop counter and add any use of the size after the
// loop, for example `if (len < 0) return 0;` just before
// `Close_004bcf80(f); return (int)dst;`. That keeps the size's live range
// spanning the loop (esi is busy with the chunk count there), so MSVC gives it
// the real local at esp0-4 and the counter the saved esi slot, and then the
// chain, the reload into esi, the copy into the counter's home, the loop and
// the whole frame match the original instruction for instruction: 87.1%. The
// only difference left is the 16 bytes that use itself compiles to, which the
// original does not have. I did not keep it, because a size check the original
// never performs is invented code, and every no-code way of extending that
// live range (a dead store, a bare `len;`, an empty if, an empty while, an
// empty switch, taking `&len` in an inlined helper, a post-loop test MSVC can
// fold) is removed before MSVC allocates the slot, and the file drops back to
// 65%. So the original either has such a use in a form MSVC folds late, or a
// source detail of its own that I did not find.
// Also tried without effect (~90 shapes on the free harness, build/scratch/
// 4bcf80/bodies): the size as an if/else chain, a nested ternary, a goto
// ladder, a long, a union, a one element struct, a one element array and
// through static inline helpers (with a local, with three returns, writing
// through a reference or a pointer, and a helper holding the whole copy loop),
// each of which either matches the original's chain exactly or if-converts it
// into esi; the counter as a second int, long, unsigned, short, array, inner
// block variable, a copy taken before or after the allocation or before the
// seek, a for-init copy, a reference and a struct; all 24 declaration orders;
// every loop form (while, do/while, for, for(;;) with the latch in the
// condition, while(1) with a break, an if/else nest with no gotos); a copy of
// dst into a local, a void* parameter plus a typed copy, a pointer return
// type; every 3x3 combination of int/long/unsigned for the size and the
// counter; `register`; a `&local` passed to an empty inlined helper; and
// tools/headers.py over all 128 header sets.
// NOTE FOR ANYONE REVERSING THE PARAMETERS: __stdcall puts the FIRST argument
// at [esp+4] and the second at [esp+8], so the original's `mov eax,
// [esp+0xc]` after `push ecx` is the second argument (the name) and its
// `mov ebp, [esp+0x20]` after the allocation call is [esp0+4], the first
// argument, the destination it returns. Spelling the parameters the other way
// round scores 78.8% instead of 78.2% and looks like progress, but it makes
// the compiler load the name into ebp and use it as a file handle.
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
    int len = 0;
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
