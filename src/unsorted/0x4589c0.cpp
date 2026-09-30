// Decompiled by deepseek-v4.1-flash, finished by space-bunny-free, finished by deepseek-v4.1-flash, finished by space-bunny-free, finished by deepseek-v4.1-flash, finished by LongCat 2.5 Preview Free, finished by deepseek-v4.1-flash, finished by GPT-6, finished by GPT-6.1-sol, finished by deepseek-v4.1-flash. Names are provisional.
// deepseek-v4.1-flash retry (issue #2796): 61.8% (857/861 bytes), not MATCH. Still 857 bytes.
// Two source changes from the 61.1% baseline, both about frame slot order:
// 1. Split `Fixed sz = off.z, sy = off.y;` into separate declarations
//    `Fixed sy, sz;` assigned later (`sy = off.y; sz = off.z;`). +0.3 points.
// 2. Reorder the Off struct members to `struct Off { Fixed z, y, x, c; };`
//    (x must sit third; all six permutations with x third score 61.8%, the
//    rest 61.4%). +0.4 points over the x,y,z,c order.
// Also retested on this baseline, all no better: passing Pos_4589c0() as an
// explicit temporary (61.8), short/int variants of `ya` (61.8), multi-statement
// yoff spellings that force intermediate memory accesses (61.8), all 24 Off
// field permutations, and moving the child/owner load before the first call
// (60.7 / 54.6).
// WHAT STILL DIFFERS (unchanged from the notes below): the prologue register
// split (original `mov ebp,[esp+0x8c]` then `mov esi,esp`, ours `xor eax,eax`
// then `mov edx,esp`) and the whole downstream allocation; the y-offset still
// folds to `(sz.whole - (sy.whole>>1)) << 16` (4 instructions) where the
// original emits the full `((ya<<16)-ya+yb)<<16` (6 instructions, 16-bit sar);
// and the post-loop clip spills x1/y0 to the dead bmp argument slot where ours
// keeps them in registers. Byte count stays 4 short because of the y-offset fold.
// GPT-6.1-sol retry: 61.1% (857/861 bytes), not MATCH. Kept this valid best.
// GPT-6.1-sol issue #2322 refinement: rechecked the baseline, tried a guarded do/while child traversal (no change), and tested passing the three zero Pos fields as scalar arguments (53.1% with literal zeros; 55.1% via initialized locals). Both scalar-call variants are semantically equivalent but worse; restored the 61.1% version. No MATCH. Remaining mismatch is in register/frame allocation and the fixed-point offset sequence described below.
// GPT-6.1-sol refinement after PR #2160: memberwise and initializer-list Pos
// zeroing scored 59.3%, while(1) with early break scored 56.4%; a continue
// child loop and Model* alias held 61.1% with no change. Two malformed edits
// failed to compile. Original 61.1% source is unchanged; no MATCH.
// Tested aggregate Pos initialization (56.2%) and explicit zeroing of both Pos
// locals (59.3%); explicit zeroing of only the first Pos scored 62.0% but left
// child cpos uninitialized, so it is not a valid candidate. Main remaining
// differences are the Pos/model register allocation, child-bound frame slots,
// and the y-offset multiply/shift sequence documented below.
// GPT-6 retry: 61.1% (857 of 861 bytes), not MATCH. A memset-based Pos
// constructor restores model in ebp and the rotated child loop. Position
// setup, child-bound stack slots and fixed-point arithmetic still differ.
// Constructor, layout, temporary-argument and 768 header variants tested.
// Earlier attempts and their measurements are preserved below.
//
// deepseek-v4.1-flash pass: still 59.3% (857 of 861 bytes). No source spelling
// moved the first diff, the prologue register split: the original loads the
// model argument into ebp (`mov ebp,[esp+0x8c]`) and the by-value Pos address
// into esi, ours loads model into esi and the Pos address into the caller-saved
// edx. First tried this pass: taking the address of the local Pos into a
// pointer local (`Pos* pp = &pos; pp->x = 0; ...`) to lengthen its live range,
// which compiles byte-identically, so the split is allocation-intrinsic.
// The whole tail follows from it. The `(ya*0xffff + yb) << 16` fold at
// 0x458abf is the same bar the sibling 0x48a870 hit (see build/scratch/SHARED.md).
//
// Result: 59.3% (857 of 861 bytes), from 54.6%. GAVE UP, not MATCH.
//
// THIS PASS (all screened free with check.py --sym, only the winners re-run
// for real). Four independent changes, each measured on its own:
// 1. +2.2 points. THE POST-LOOP CLIP NEGATES TWICE, and the old header never
//    spotted it. The disassembly computes `x0 = -dx` (neg eax at 0x458b51),
//    clamps it, and then negates AGAIN when storing (neg eax at 0x458b8a,
//    neg ecx at 0x458b81 just before `mov [ecx+6],cx`). So the source is
//    `this->bitmap->dx = (short)(-x0)`, NOT `(short)x0`. The old file stored
//    the clamped value directly, which is both 2 bytes short AND semantically
//    the wrong sign: with x0 = min(-dx, minX) the stored dx should come out as
//    -x0 = max(dx, -minX), i.e. positive, shifting the bitmap right when the
//    children hang off the left edge. Worth re-reading every store in a
//    "compute negated, clamp, store negated" shape; this one was hiding.
// 2. The x1/y1 spelling is `bmp->width - bmp->dx` (the original's
//    `sub ecx,edx` at 0x458b3d), not `bmp->width + x0`. The header had tried
//    both and recorded the `+ x0` form as the winner, but that verdict was
//    only true for the pre-fix-1 code; once fix 1 is in, `- bmp->dx` wins by
//    1.6 points. A lesson worth keeping: a spelling A/B result is only valid
//    for the code it was measured on, and these three changes interact.
// 3. `x0`/`y0` come from a pointer into the owner's coordinate triple,
//    `int* op = &model->owner->x; ... op[0], op[1], op[2]`, which is what
//    makes the original form `add eax,0x6a` once and index `[eax],[eax+4],
//    [eax+8]` instead of three separate displacements off model->owner.
//    Hoisting `Owner_4589c0* owner` first and then taking `&owner->x` scores
//    worse (54.3%), so the base really is `model->owner`, not a local.
// 4. The four min/max updates need their sums in named locals
//    (`int cx = cminX + xoff; ...` then `if (cx < minX) minX = cx;`), not the
//    expression repeated in the test and the assignment. Worth 0.9 points
//    even though it ADDS 4 bytes: it changes what the allocator spills.
//    Partial versions (x-only, y-only, const, `>`-flipped tests) all score
//    lower, so it is all four or none.
//
// Together these took the file from 21 bytes short to 4 bytes short.
//
// WHAT IS STILL WRONG, and note these are all REGISTER ALLOCATION now, the
// size being nearly right:
// A. The loop is rotated in the original: `je end / jmp top / latch:
//    mov ebp,[esp+0x80] / test ...`, i.e. the `model` argument is kept in ebp
//    and reloaded at the bottom of the loop, because the body clobbers ebp.
//    Ours has a plain bottom-tested loop with no latch. This is the same
//    "one more thing wants a callee-saved register" as old items 1 and 4:
//    ours puts `model` in esi and `bmp` in ebp after the loop, the original
//    does the opposite way round. do/while, for-loops and a separate head
//    pointer all still produce the unrotated loop (all 57.0%, unchanged).
// B. The old item 2 (frame slot order of the eight bbox ints) is confirmed
//    unfixable from the source and should be treated as closed: I compiled
//    ALL TWENTY-FOUR declaration orders of the four parent accumulators and
//    every one produced the identical slot order minY, minX, maxY, maxX.
//    MSVC 5 is ordering these by something internal (they are all passed by
//    address to the same call), not by declaration. Same for the child set.
//    Do not spend more runs on declaration order; it is a dead end.
// C. Old item 3, the y offset. I re-derived the arithmetic from scratch and
//    the header's reading is CORRECT: the original computes
//    ((ya<<16) - ya + yb) << 16, and because the low half is provably dead
//    the whole thing collapses to ((yb - ya) << 16), which is the same 32-bit
//    value. I confirmed no spelling recovers the six instructions: writing to
//    off.y, a separate yoff, a bitfield Fixed, `ya * 0xffff`, `ya * 65535`,
//    an int temp, and a two-step Fixed temp ALL fold to the same four
//    instructions and the same 853-byte function (56.8% each at the time).
//    The `<<16` at the end discards bits 0-15 of (ya<<16 - ya + yb), and
//    `ya<<16` has zero low half, so MSVC 5 legitimately drops it. This is
//    MSVC out-optimising the original, not a missing source construct: the
//    original exe was very likely built with a slightly different MSVC 5
//    build or optimisation setting. I do not think this one is reachable.
// D. Old item 4 (spilling x1/y0 to the dead bmp argument slot) is now FIXED:
//    the clip section spills and reloads exactly as the original does, and
//    the two sections agree instruction for instruction apart from one
//    duplicated `movsx esi, word [ebp+6]`, which I could not remove by
//    naming dx/dy in locals (all of d2-d5 scored 58.3-59.3%, none better).
// E. In the tail, the original reloads the `bmp` argument into ecx for the
//    colour byte (`mov cl, byte [ebp+8]`) where ours uses `al`, and it
//    reloads y0 from [esp+0x7c] where ours still has cx. Both follow from A.
//
// THIS PASS (all free-scored through check.py --sym):
// * Declaring the four parent bbox accumulators in the order maxX, minY, maxY,
//   minX reproduces the original's frame slot order (maxX at L+0, minY L+4,
//   maxY L+8, minX L+0xc) and is worth 0.3 points.
// * The post-loop clamps must run in the order minX, maxY, maxX, minY (not the
//   x-then-y order the disassembly's basic blocks suggest), worth 1.7 points:
//   the reordering moves the allocator so the spill/reload pattern is closer.
// * Writing the four clipped bounds as x0 = -dx, y0 = -dy, x1 = width + x0,
//   y1 = height + y0 (the x0/y0-first form) is worth another 0.1 point over
//   computing x1/y1 first; the original's `movsx edx,[bmp+4]` loads dx before
//   width, which this form lets MSVC schedule.
// Tested and no help: aliasing model/bmp into a fresh local, aggregate Pos
// initialisation, declaring Pos before the accumulators, caching model->owner
// before the loop, ddx/ddy short locals, an extra live local across the first
// call, and the twenty-three other declaration orders for both bbox sets.
//
// WHAT I CHANGED THIS PASS (both worth about a point each):
// 1. The two FUN_004b7f90 calls recompute their offsets. The original reloads
//    `this->bitmap` and does `movsx dx/dy; sub sdx/sdy` before EACH call, so the
//    source has no ddx/ddy locals: writing `img->dx - sdx, img->dy - sdy` inline
//    (with `this->bitmap` spelled out, not hoisted into an `img` local, which
//    scored worse) reproduced those eight instructions and 12 bytes of size.
// 2. `Fixed sz = off.z, sy = off.y;` (sy and sz the other way round) is worth
//    0.7 points, because it is the declaration order the original's frame slots
//    imply.
//
// THE ANCHOR FOR EVERY [esp+X] BELOW, worth knowing: the body does NOT run at
// the post-prologue esp. `sub esp,0xc` in the prologue holds the first call's
// by-value Pos, and FUN_00458310 pops it with `ret 0x20`, so every esp-relative
// operand in the rest of the function is 0xc higher than it looks. Call that
// one S (it is the esp after the first call returns). Then:
//   S+0x10 maxX 0x14 minY 0x18 maxY 0x1c minX   (the first call's arg2, arg3,
//   arg4, arg1 in that order: FUN(&minX,&maxX,&minY,&maxY, model, pos))
//   S+0x20 cminX 0x24 cminY 0x28 cmaxX 0x2c cmaxY (the child call passes
//   &cminX,&cmaxX,&cminY,&cmaxY, i.e. arg1,arg2,arg3,arg4 = 0x20,0x28,0x24,0x2c)
//   S+0x30 off.x 0x34 off.y, later reused by yoff 0x38 off.z 0x3c off.c (dead)
//   S+0x40 sy 0x44 sz
//   S+0x48 surface (0x30 bytes, its `bits` field lands at 0x54)
//   S+0x7c the bmp argument slot (reused for x1 and then y0), S+0x80 model
// The prologue's `mov ebp,[esp+0x8c]` is the model argument, read at the
// post-prologue esp, which is S-0xc.
//
// STILL WRONG, in order of how much it costs:
// 1. Register allocation, and it cascades over the whole function. The original
//    keeps `model` in ebp (reloading it from the argument slot at the loop latch
//    0x458a24 and at 0x458d00) and the address of the zeroed Pos in esi, and
//    never puts `bmp` in a register during the loop. Ours puts `bmp` in esi from
//    the first instruction, the Pos address in edx, and `model` in a frame slot.
//    Everything after the prologue is shifted by that. Tried and no help:
//    hoisting `owner` out of the loop (50.0%), a dummy live local (46.8%),
//    declaring pos before the accumulators (no change), reordering the four
//    min/max updates (50.3%), an `img` local for the blit calls (48.9%).
// 2. The frame slot ORDER of the eight bbox ints. The original's is
//    maxX,minY,maxY,minX then cminX,cminY,cmaxX,cmaxY; ours is
//    minY,maxY,minX,maxX then cminX,cmaxX,cminY,cmaxY (read out of the /Fa
//    listing, which names every slot). Declaration order does not move them:
//    declaring the accumulators maxY,minX,maxX,minY produced exactly the same
//    slot order, so the order is fixed by the front end, not by the source.
//    MSVC pairs the two Y values and the two X values; the original interleaves
//    them. Worth chasing, since it moves every operand in the loop body.
// 3. The y offset still folds. The original emits `mov dx,[sy.whole]; sar dx,1;
//    movsx eax,dx; mov ecx,eax; shl ecx,0x10; sub ecx,eax; movsx
//    edx,[sz.whole]; add ecx,edx; shl ecx,0x10; mov [off.y],ecx; movsx
//    ecx,[off.y.whole]`, i.e. it computes `((ya<<16) - ya + yb) << 16` as a full
//    32-bit int, stores all four bytes and re-reads the high word. Ours proves
//    the low half is dead and narrows the whole thing to `(yb - (ya)) << 16`,
//    which is the same value (0xffff*0x10000 == -0x10000 mod 2^32) in four
//    instructions instead of six. Writing the result into `off.y`, keeping a
//    separate `yoff`, and a bitfield Fixed all still fold, so the original's
//    stored value must have a second, low-half use that has not been found yet.
// 4. After the loop the original spills x1 to the dead `bmp` argument slot
//    (0x458b45) and reloads it at 0x458b5b, and reuses the same slot for y0 at
//    0x458b83; ours keeps all four of x0,x1,y0,y1 in registers. Same class of
//    problem as (1): one more thing wants a callee-saved register.
//
// THE FIX THAT MATCHED THE FRAME, and it is the one the earlier notes were
// missing. The three dwords dx/dy/dz are not three loose locals: they are the
// first three ints of a FOUR-int offset struct, exactly like Off_4584d0 in the
// sibling 0x4584d0 (which has `struct { int a; int b; int y; int c; } off;`,
// three used and `c` never touched, in the same source file). Declaring
// `struct Off { Fixed z, y, x, c; }; Off off;` puts the differences at frame
// 0x30/0x34/0x38 and reserves the dead 0x3c slot, so the copies sy/sz land at
// 0x40/0x44 and the surface at 0x48, giving the original's 0x68 frame. Every
// earlier spelling (loose Fixed locals, reordering, array, copy helpers) let
// MSVC pack the locals into 0x5c and scored 47.5%.
//
// A note for whoever picks this up: `tools/wcl /c /O2 /Ob2 /MT /Iinclude
// /Fa<out>.lst <file>` lists every local's frame slot by name (`_minX$ = -96`),
// which is far quicker than diffing, and it is the only way to see the slot
// ORDER problem in point 2 above.
// deepseek-v4.1-flash retry #2965: still 61.8% (857/861), not MATCH. Reconfirmed
// the memset Pos ctor is required: `x=0;y=0;z=0;` moves model from ebp to esi
// (esi already carries the child node) and drops to 60.4. Remaining diffs are
// unchanged from the notes below: the prologue register split (original loads
// model into ebp before `mov esi,esp` and expands the zeroing as three separate
// xors; ours zeroes first in compact eax form with the Pos address in edx), the
// downstream frame allocation, the y-offset fold, and the post-loop clip spills.
#include <string.h>

struct Child_4589c0;
struct Model_4589c0;

union Fixed { int value; struct { unsigned short fraction; short whole; }; };

struct Pos_4589c0 {
    int x;
    int y;
    int z;
    Pos_4589c0() { memset(this, 0, sizeof(*this)); }
};

#pragma pack(push, 2)
struct Owner_4589c0 {
    char unknown_0[0x6a];
    int x;                          // +0x6a
    int y;                          // +0x6e
    int z;                          // +0x72
    char unknown_76[0x8a - 0x76];
    Child_4589c0* firstChild;       // +0x8a
};

struct Model_4589c0 {
    int pieceCount;                 // +0x00
    char unknown_4[0xc - 0x4];
    Owner_4589c0* owner;            // +0x0c
    void* bitmap;                   // +0x10
};

struct Child_4589c0 {
    char unknown_0[0x6a];
    int x;                          // +0x6a
    int y;                          // +0x6e
    int z;                          // +0x72
    char unknown_76[0x8e - 0x76];
    Child_4589c0* next;             // +0x8e
    char unknown_92[0x9e - 0x92];
    Model_4589c0* model;            // +0x9e
    char unknown_a2[0x110 - 0xa2];
    unsigned int flags;             // +0x110
};

#pragma pack(pop)

struct Image_4589c0 {
    unsigned short width;           // +0x00
    unsigned short height;          // +0x02
    short dx;                       // +0x04
    short dy;                       // +0x06
    unsigned char colour;           // +0x08
    unsigned char flag9;            // +0x09
    unsigned char count;            // +0x0a
    unsigned char kind;             // +0x0b
    char unknown_c[4];
    unsigned char* pixels;          // +0x10
    unsigned char* shade;           // +0x14
};

struct Src_4589c0 {
    unsigned short width;           // +0x00
    unsigned short height;          // +0x02
    unsigned short x;               // +0x04
    unsigned short y;               // +0x06
    char unknown_8[8];
    int bits;                       // +0x10
};

struct Surface_4589c0 {
    int width;                      // +0x00
    int height;                     // +0x04
    int pitch;                      // +0x08
    void* bits;                     // +0x0c
    int field_10;                   // +0x10
    int field_14;                   // +0x14
    unsigned short x;               // +0x18
    unsigned short y;               // +0x1a
    char unknown_1c[0x10];
    unsigned int flag0 : 1;         // +0x2c
    unsigned int flag1 : 1;
};

class Class_00458310 {
public:
    void FUN_00458310(int* minX, int* maxX, int* minY, int* maxY, Model_4589c0* model,
                      Pos_4589c0 pos);
};

class Class_00458d30 {
public:
    int FUN_00458dd0(Image_4589c0* image, Model_4589c0* model);
};

class Class_004c6ae0;

struct Bitmap_4589c0 {
    unsigned short width;           // +0x00
    unsigned short height;          // +0x02
    short dx;                       // +0x04
    short dy;                       // +0x06
    unsigned char colour;           // +0x08
    unsigned char flag9;            // +0x09
    unsigned char count;            // +0x0a
    unsigned char kind;             // +0x0b
    int unknown_c;                  // +0x0c
    void* field_10;                 // +0x10
};

void __stdcall FUN_004b8a80(Surface_4589c0* dst, Src_4589c0* src);
void __stdcall FUN_004b7f90(Class_004c6ae0* dst, Bitmap_4589c0* bmp, int x, int y);

class Class_00459200 {
public:
    char unknown_0[0x10];
    Image_4589c0* bitmap;           // +0x10
    void FUN_004589c0(Image_4589c0* src, Model_4589c0* model);
};

// FUNCTION: 0x4589c0
void Class_00459200::FUN_004589c0(Image_4589c0* bmp, Model_4589c0* model)
{
    int maxX = 0;
    int minY = 0;
    int maxY = 0;
    int minX = 0;
    Pos_4589c0 pos;
    ((Class_00458310*)this)->FUN_00458310(&minX, &maxX, &minY, &maxY, model, pos);
    Child_4589c0* child = model->owner->firstChild;
    while (child != 0) {
        if ((child->flags & 0x20000) == 0) {
            int cminX = 0;
            int cmaxX = 0;
            int cminY = 0;
            int cmaxY = 0;
            Pos_4589c0 cpos;
            ((Class_00458310*)this)->FUN_00458310(&cminX, &cmaxX, &cminY, &cmaxY,
                                                  child->model, cpos);
            int* op = &model->owner->x;
            Fixed sy, sz;
            struct Off { Fixed z, y, x, c; };
            Off off;
            off.x.value = child->x - op[0];
            off.y.value = child->y - op[1];
            off.z.value = child->z - op[2];
            sy = off.y;
            sz = off.z;
            int xoff = off.x.whole;
            int ya = (short)(sy.whole >> 1);
            int yb = sz.whole;
            Fixed yoff;
            yoff.value = ((ya << 16) - ya + yb) << 16;
            int yo = yoff.whole;
            int cx = cminX + xoff;
            int dx2 = cmaxX + xoff;
            int cy = cminY + yo;
            int dy2 = cmaxY + yo;
            if (cx < minX) minX = cx;
            if (dx2 > maxX) maxX = dx2;
            if (cy < minY) minY = cy;
            if (dy2 > maxY) maxY = dy2;
        }
        child = child->next;
    }
    int x0 = -bmp->dx;
    int y0 = -bmp->dy;
    int x1 = bmp->width - bmp->dx;
    int y1 = bmp->height - bmp->dy;
    if (minX < x0) x0 = minX;
    if (maxX > x1) x1 = maxX;
    if (minY < y0) y0 = minY;
    if (maxY > y1) y1 = maxY;
    int newW = x1 - x0;
    int newH = y1 - y0;
    this->bitmap->width = (unsigned short)newW;
    this->bitmap->height = (unsigned short)newH;
    this->bitmap->dx = (short)(-x0);
    this->bitmap->dy = (short)(-y0);
    this->bitmap->colour = bmp->colour;
    if (newW == bmp->width && newH == bmp->height) {
        memcpy(this->bitmap->pixels, bmp->pixels, bmp->width * bmp->height);
        memcpy(this->bitmap->shade, bmp->shade, bmp->width * bmp->height);
    } else {
        short sdx = bmp->dx;
        short sdy = bmp->dy;
        bmp->dx = 0;
        bmp->dy = 0;
        Surface_4589c0 surface;
        FUN_004b8a80(&surface, (Src_4589c0*)this->bitmap);
        memset(this->bitmap->pixels, this->bitmap->colour,
               this->bitmap->height * this->bitmap->width);
        memset(this->bitmap->shade, 0, this->bitmap->height * this->bitmap->width);
        FUN_004b7f90((Class_004c6ae0*)&surface, (Bitmap_4589c0*)bmp,
                     this->bitmap->dx - sdx, this->bitmap->dy - sdy);
        unsigned char* t = bmp->pixels;
        bmp->pixels = bmp->shade;
        bmp->shade = t;
        surface.bits = this->bitmap->shade;
        FUN_004b7f90((Class_004c6ae0*)&surface, (Bitmap_4589c0*)bmp,
                     this->bitmap->dx - sdx, this->bitmap->dy - sdy);
        t = bmp->pixels;
        bmp->pixels = bmp->shade;
        bmp->shade = t;
        bmp->dx = sdx;
        bmp->dy = sdy;
    }
    ((Class_00458d30*)this)->FUN_00458dd0(this->bitmap, model);
}
