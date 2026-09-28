// Decompiled by space-bunny-free. Names are provisional.
// Decodes the body of a PCX file: reads the 0x80 byte header, checks it is the
// 0x0a version 5 variant, allocates the width x height body and the 0x300 byte
// colour map, seeks back over the colour map to the body, and run-length
// expands the body into the freshly allocated pixels. Returns 1 when the file
// was a PCX it could decode, 0 when the header was not the expected variant.
//
// 78.7%, 495 of 506 bytes. The prologue, the header test and the whole inner
// RLE loop (both the run-len==1 store, the memset expansion with its reloads
// out of the stack slots, the literal path and the loop tail) are now
// instruction for instruction identical to the original.
//
// What still differs, in order of size:
//
// 1. The frame is 0x94, the original's is 0x98: the original has six dword
//    locals, ours has five, because MSVC 5 gives our `p` no stack slot at all
//    (it lives in ebp and is never reloaded), while the original's `p` has a
//    home at [esp+0x1c] that is stored before the loop and reloaded and stored
//    again at the outer latch (`mov ebp,[esp+0x1c] / add ebp,eax /
//    mov [esp+0x1c],ebp`). That one missing slot shifts every stack
//    displacement in the function, which is most of the remaining score.
//    Everything that could give a pointer local a home was tried and none of
//    it works: unsigned char / char / void pointer types, by-reference static
//    inline helpers for the store and for the run, a pointer returned from the
//    memset, block scope for the loop variables, a for / while / do-while /
//    goto / comma-expression row loop, the row advance in the for header or in
//    the loop condition, and p declared in every position of the declaration
//    list. C1 allocates a slot for every local and C2 shrinks the frame for the
//    ones it never touches, so whatever the original wrote, its p is one MSVC
//    5 here keeps in memory. Register pressure is not the reason: the inner
//    loop leaves esi free in both.
//
// 2. The height guard. The original computes it in the register and puts the
//    load of h back afterwards: `dec esi / js / inc esi / mov [esp+0x20],esi`,
//    i.e. (h - 1) < 0 tested on a register whose value is then restored. Every
//    spelling that keeps h unmodified gives either `cmp esi,1 / jl` (ours) or
//    `dec esi / test esi,esi / jl`; only `if (--h < 0) { h++; ... }` drops the
//    test, and that form stores h-1 and decodes one row too few, so it was not
//    used. The guard shape also decides where the store of h lands, so it costs
//    three bytes of shift as well as the two instructions.
//
// 3. The row-pointer reload at the outer latch described in (1), and with it
//    the `mov [esp+0x1c],ebp` store.
//
// Advice for docs/agent-guide.md:
// - A `memset` of a 4-field, 16-byte struct is expanded by MSVC 5 as
//   `xor eax,eax` plus four `mov [base+N],eax` stores, base register copied
//   with `mov ecx,esi`, not as `rep stosd`. Writing the four assignments out
//   instead gives four `mov [esi+N],0` immediate stores, so the source really
//   was a `memset` (or a struct clear the front end expanded the same way).
// - The `unsigned char` types of a byte read buffer and of a 0..0x3f run length
//   are pinned by one instruction each: the mask test wants a byte load
//   (`mov al,[c] / mov cl,al / and cl,0xc0`) and the memset fill and count want
//   `mov eax,[c] / and eax,0xff` and `mov esi,[len] / and esi,0xff`, which is
//   the dword-load-and-mask an `unsigned char` local gives once its value is
//   known only in memory. `int` gives a byte load at the fill, `char` gives
//   `movsx`, and a `unsigned short` source for the header's 16-bit fields
//   narrows the mask to `and reg,0xff` and byte loads, so the 0xffff masks need
//   explicit `*(int*)` reads of the header.

#include <string.h>

struct PCX_004caa40 {
    unsigned char* data;     // +0x0
    unsigned char* palette;  // +0x4
    int width;               // +0x8
    int height;              // +0xc
};

int __stdcall FUN_004bb7c0(void* file, void* buf, int size);
void __stdcall FUN_004bb710(void* file, int pos);
int __stdcall FUN_004bbd00(void* file);
void* FUN_004d83b0(char* name, unsigned int size);

// FUNCTION: 0x4caa40
int __stdcall FUN_004caa40(void* file, PCX_004caa40* pcx)
{
    int x;
    unsigned char c;
    unsigned char len;
    unsigned char* p;
    int h;
    int w;
    unsigned char header[0x80];
    memset(pcx, 0, sizeof(PCX_004caa40));
    if (FUN_004bb7c0(file, header, 0x80) != 0x80 || header[0] != 0x0a || header[1] != 5)
        return 0;
    pcx->width = 1 + ((*(int*)(header + 0x18) & 0xffff) - (*(int*)(header + 0x14) & 0xffff));
    pcx->height = 1 + ((*(int*)(header + 0x1a) & 0xffff) - (*(int*)(header + 0x16) & 0xffff));
    pcx->data = (unsigned char*)FUN_004d83b0("PCX BODY", pcx->height * pcx->width);
    pcx->palette = (unsigned char*)FUN_004d83b0("COLOR MAP", 0x300);
    FUN_004bb710(file, FUN_004bbd00(file) - 0x300);
    FUN_004bb7c0(file, pcx->palette, 0x300);
    FUN_004bb710(file, 0x80);
    p = pcx->data;
    w = pcx->width;
    if (pcx->height < 1)
        goto done;
    h = pcx->height;
    do {
        x = w;
        while (x > 0) {
            FUN_004bb7c0(file, &c, 1);
            if ((c & 0xc0) == 0xc0) {
                len = c & 0x3f;
                x -= len;
                if (x < 0)
                    len += x;
                FUN_004bb7c0(file, &c, 1);
                if (len == 1) {
                    *p = c;
                    p++;
                } else {
                    memset(p, c, len);
                    p += len;
                }
            } else {
                *p = c;
                p++;
                x--;
            }
        }
        p += w;
    } while (--h);
done:
    return 1;
}
