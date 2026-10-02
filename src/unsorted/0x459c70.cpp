// Decompiled by deepseek-v4.1-flash, finished by space-bunny-free, finished by GPT-6, finished by GPT-6.1-sol, finished by deepseek-v4.1-flash, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by deepseek-v4.1-flash, finished by deepseek-v4.1-flash, edited by claude-opus-5-5, deepseek-v4.1-flash retry, second deepseek-v4.1-flash pass, finished by Space Bunny Free. Names are provisional.
//
// WHAT THIS PASS ADDED (Space Bunny Free). Baseline 66.6% (2047 original bytes,
// 2038 ours) and the file is UNCHANGED, because nothing measured beat it. About
// 270 scored compiles this pass, 151 of them one declaration-order sweep, run
// through a new harness that compiles and scores N candidates in one process at
// about 0.4 s each instead of check.py's ~60 s (the recipe is at the end of
// this header, so the next pass can afford the same kind of sweep). Every
// claim below is a measurement, not a reading of the disassembly.
//
// Second pass: 63.7 -> 66.6%, and the file is now semantically faithful
// (item 3 below fixes a real bug the first pass had).
//
//  1. The two face loops' first-face prologue must be a FULL if/else that
//     assigns both the face pointer and the index:
//        Face_459c70* face; int fi;
//        if (info->firstFace != -1) { face = info->faces + 1; fi = 1; }
//        else { face = info->faces; fi = 0; }
//     That is what emits the original's `je` + `jmp` diamond at 0x459f4b and
//     0x45a13d (+1.5). Writing it as `face = info->faces; fi = 0; if (...)`
//     drops the `jmp`; doing the same in the THIRD (poly) loop costs 2.5, and
//     that loop wants a named `int skipFirst` test instead.
//  2. tools/permute.py took the file from 64.4 to 66.6 and all of that
//     survived cleaning. Free to delete: the self-assignments it added
//     (`mode = mode;`, `offX = offX;`, `x = x;`, `tmp5 = tmp5;`), both getter
//     helpers, its `int tmp2 = X, n = tmp2` copy chain, the empty `else` after
//     a `continue`, the `(int)` casts, the `(Bitmap_459c70*)bmp` cast, the
//     `if (f->count != 4) {} else {}` inversion, the `do {} while (0)` wrapper
//     (-0.3 if kept, so it is gone), the `if (x == 0) {} else {}` in the tail
//     and its `for (;;) + break` row loop. What is left of its 66.6 is the
//     tail's statement order: `y++;` BEFORE `d += bmp->width;` (+0.2), and
//     `Face_459c70* f = info->faces;` as one declaration. Load-bearing, and kept here under
//     real names because every alternative spelling costs more:
//       * a FUNCTION-SCOPE `float w;` for the division loop (-1.7 if it moves
//         back inside the loop);
//       * `int xs = x >> 16;` before `x = (short)xs << 1;` in the doubled
//         projection arm, and `int y2 = y / 2;` in the z arm (-11 if the x
//         one is inlined);
//       * `int vy = verts[k].y;` in the undoubled arm, and `float t0`/`float
//         t2` around the two `accum[k][c] = t + normal[i].?` updates: these
//         decide which operand MSVC loads first, and the temps give the
//         original's `fld accum / fadd normal` order (each is worth 0.2-0.7);
//       * the second face loop as `while (i < info->faceCount) { ... i++, f++; }`
//         rather than a `for` header (-5.8);
//       * the poly loop's `for (; i < info->faceCount; )` with `i++, f++;` at
//         the bottom, and `int x, y, z;` written as three declarations (-0.3).
//     Two spellings that are NOT free and are deliberately not taken:
//     `int x, y, z;` on one line and merging the second face loop's
//     `unsigned short* idx = f->indices;` back into one declaration (0.3 each).
//  3. SEMANTIC FIX: the two vertex.z arms are NOT interchangeable. In the
//     original the mode!=0 arm doubles y and then halves it again, so both
//     arms compute y16 + bias; the first pass had `y + bias` in the mode!=0
//     arm (2*y16 + bias) and `y/2 + bias` in the mode==0 arm. The correct
//     `if (mode != 0) { z = bias + y/2 } else { z = y + bias }` is in the
//     file; the wrong one scores about 0.3 higher and the permuter keeps
//     proposing it, so do not "fix" it back. The arms differ in C only because
//     a truncating /2 of an odd value is not exact, but with the doubling
//     above, the two paths are.
//  4. `offX`/`offY` and the draw target come from the `bmp` local, which holds
//     exactly what the original's reassigned `bitmap` parameter holds (the
//     shadow in mode 1, the front bitmap in mode 0), so the values are right;
//     only the spelling differs. The original really does overwrite its
//     parameter slot 0x159e8 with the shadow at 0x459d28 and then read the
//     vertex offsets back out of it (0x459df0/0x459dfc); every spelling of
//     that reassignment costs 1.7-2.0 points, so the separate local is kept.
//
// Still differing, all of it register and slot placement with no semantic
// content left:
//  * Frame slots. The original's map is 0x10 shade / 0x14 verts-walk / 0x18
//    verts (reused for firstFace) / 0x1c piece / 0x20 mode / 0x24 info / 0x28
//    src / 0x2c offY / 0x30 n / 0x34 x / 0x38 p / 0x3c offX. MSVC 5.0 does not
//    assign these in declaration order (moving every declaration was inert
//    here), so the map can only be moved by changing the dataflow.
//  * The vertex loop (0x459e08-0x459f38, the largest single loss left). The
//    original has three induction registers: esi = &vertex[k], ebx =
//    &accum[k][1] (stored at +4/0/-4 off that one base) and a third, the
//    verts walk, which it SPILLS to [esp+0x14]; it also spills x to
//    [esp+0x34] and materialises the mode-test zero with `xor ecx,ecx; cmp
//    edx,ecx` so the same ecx can store the three accum zeros. Here MSVC
//    shares one induction between `verts[k]` and `accum[k][2]` (an
//    `accum - verts` delta in a register instead of a third induction), keeps
//    x in a register, and uses `test`/`movl $0`. Giving the accum its own
//    pointer induction (`float* a` walked by `a += 3`) or the vertex walk its
//    own pointer each costs 7-8 points, so the sharing is a net win for MSVC
//    5 and the original's shape is not reachable from this source shape.
//  * The bias helper materialises in ecx where the original uses edx (six
//    instructions at 0x459eb8 and 0x459ee5). Passing the loaded flag word, or
//    a `char*`, to `shade_bias` instead of the list does not move it.
//  * 0x45a320: the original's scratch for the `usePic` bit test is ecx
//    (`mov ecx,eax; shr ecx,1; test cl,1`), ours picks edx. This is MSVC 5's
//    [eax,ecx,edx] temp rotation and no spelling of the three bit tests
//    moves it.
//  * 0x45a29b: the piece bit-2 test is `mov dl,[ecx+0x28]; shr dl,2; test
//    dl,1` in the original and `test byte ptr [ecx+0x28],4` here; `(f >> 2)
//    & 1` folds to the mask test in every spelling tried.
//  * 0x45a1e2: the original re-derives the division loop's bound from
//    `[piece]->info->vertexCount` and keeps firstFace in ebp across the face
//    loops; using the re-read bound costs 7-9 points, so the loop keeps the
//    `n` slot here.
//  * 0x45a419 tail: the original's inner copy loop is entered through
//    `mov edi,eax; dec eax; test edi,edi; je; lea edi,[eax+1]` and reloads
//    `src->height` at the row end; no spelling of `while (x--)`, `for (x = w;
//    x; --x)`, `do {} while (--x)` or a pre-decrement reproduces it.
//  * 0x45a2fd: the original bumps the poly induction before the index one.
//  * 0x459c70-0x459d6a: the original keeps the memset product in ebx and the
//    front bitmap in ebp, and re-reads `useColor` from its argument slot three
//    times instead of holding it in a register.
//
//  5. Two bits of permuter noise in `shade_bias` are load-bearing and are left
//     in on purpose: the `int ret0; ret0 = c ? 125 : 50; return ret0;` chain
//     (-0.4 if written as `return c ? 125 : 50;`). The split `bool c; c = ...;`
//     declaration beside it is free to merge and has been.
//
// SPACE BUNNY FREE PASS, 66.6% held. Residual by hunk (build/scratch/0x459c70/
// hunks.py: one line per unified-diff hunk with the original address it
// covers), so the next pass knows where the 214 removed / 205 added lines are:
//   0x459ec7 -52/+49  the vertex loop's tail          <- the largest single loss
//   0x45a442 -40/+36  the tail's inner copy loop
//   0x45a133 -26/+28  the second face loop's prologue
//   0x459dd2 -21/+26  the vertex loop's prologue
//   0x45a223 -16/+14  the division loop
//   0x459d57 -15/+11  the prologue's else arm
//   0x45a1e2 -11/+11  the division loop's bound
//   0x45a363  -9/+9   the FUN_004c8bb0 argument (see C below)
//   0x45a30b  -9/+8   the piece bit-2 test and the two bit tests after it
//   0x45a2f3  -5/+3   the poly loop's two inductions
//  A. THE FRAME SLOTS ARE WORTH 4.8 POINTS, NOT MORE, AND DECLARATION ORDER
//     DOES NOT MOVE THEM. A differ that rewrites every local
//     `dword ptr [esp + 0xNN]` with NN < 0x100 to one token scores 71.4%
//     against the real 66.6% (shape 74.0%), so no amount of moving variables
//     between frame slots can buy more than 4.8; the other 24.6 is register
//     choice and instruction shape. And the order is not the source order:
//     151 random permutations of the entire function-scope scalar block
//     (shadow, src, bmp, mode, offX, w, s, d, piece, verts, pflags) all
//     compile BYTE-IDENTICALLY, 2038 bytes and 66.6% every one. That confirms
//     and hardens the "moving every declaration was inert" note above: on this
//     file it is inert over 151 orders, not over the three or four an earlier
//     pass tried. MSVC 5 ranks slots by how often the variable's slot is
//     referenced (the rule 0x4c8bb0's header records), so the only way to
//     move one is to change its reference count, not its declaration.
//  B. `bmp` REALLY IS THE REASSIGNED `bitmap` PARAMETER, AND SAYING SO FIXES
//     FOUR THINGS AT ONCE. Written as `bitmap = shadow;` in the antiAlias arm
//     with `src = bitmap;` hoisted to the top of the function and no `bmp`
//     local at all, MSVC 5 emits `mov [esp+0x159e8], edi` (the original's
//     `mov [esp+0x159e8], esi` at 0x459d28, writing the shadow over the
//     parameter), the `mode` slot lands on [esp+0x20] as in the original
//     (ours is 0x24), `offX` lands on [esp+0x3c] as in the original (ours is
//     0x34), and the tail's inner loop gets the original's `mov edi, eax;
//     dec eax; test edi, edi; je; lea edi, [eax + 1]` entry with the `je` and
//     `jne` on the original's addresses. The whole function also comes out at
//     exactly 2047 bytes, the original's size. It still does not score better:
//     65.9% with `src = bitmap` left after `haveMode`, 66.6% (shape 75.0%,
//     the best shape measured, 2028 bytes) with it hoisted to the top and the
//     division loop as a `for`, and 65.6% with the other `src` placement. What
//     it costs is the vertex loop, which grows from -52/+49 to -75/+75 because
//     `bitmap->field_4` and `field_6` now have to come from the parameter slot
//     instead of a register. So: right shape, wrong trade, kept out. The one
//     thing worth stealing from it is the `mode` slot at 0x20.
//  C. THE TWO DRAW CALLS DO NOT TAKE THE BITMAP AS THEIR FIRST ARGUMENT, and
//     the file is wrong about both. The encodings are unambiguous:
//       0x45a394  8b 84 24 f4 59 01 00   mov eax, [esp + 0x159f4]
//       0x45a3a9  8b 8c 24 ec 59 01 00   mov ecx, [esp + 0x159ec]
//     0x159f4 is the fourth parameter (`useColor`) and 0x159ec the second
//     (`list`), and stdcall pushes right to left, so the original really calls
//     FUN_004c8bb0(useColor, pic, poly, 0) and FUN_004c0c70(list, poly,
//     f->count, f->unknown_0). `bmp` is the bitmap argument at 0x159e8 and is
//     only ever passed to FUN_004b95a0 (0x45a419 `push ebp; push esi` with
//     esi reloaded from 0x159e8 at 0x459d57). Writing the real arguments, and
//     loosening both prototypes' first parameter to int, matches both call
//     sites exactly and scores 66.2% (shape 74.9%), 0.4 BELOW the file as it
//     stands, with or without B, with or without the `!= 0` spelling of the
//     usePic test (66.3%). Not applied, because a smaller score is a
//     regression, but it is a two-line change and it is the one place left
//     where this file is known not to be semantically faithful.
//  D. The vertex loop's `shade` reload (the original re-reads [esp+0x10] at
//     0x459f18 for `shade += 3`, ours keeps the value in a register) cannot be
//     bought: `int* sp = &shade; *sp` , a one-field local struct, and
//     `shade = shade + 3` all compile byte-identically (2038, 66.6%), and
//     moving `shade += 3` to the end of the body costs 0.9. The original's
//     reload is a consequence of its register allocation (edx carries the
//     vertices walk and is clobbered by the vertex loads), not of anything the
//     source asks for.
//  E. The tail's inner loop idiom is not reachable from any spelling tried.
//     Eight loop forms (`for (x = w; x != 0; x--)`, guarded and unguarded,
//     `while (x != 0) { ...; --x; }`, `do {} while (--x != 0)`, `x-- > 1`,
//     `x > 0`, `do {} while (x-- != 0)`, and a goto form) all score 66.6% or
//     64.0% and none emits the entry sequence. A sixteen-function micro probe
//     (int, unsigned, short, unsigned short, pre-decrement and post-decrement
//     counters, with and without a surrounding row loop and with `d += width`
//     at the row end) produces `mov si, [eax]; test si, si` or
//     `mov esi, ...; and esi, 0xffff` and never `mov edi, eax; dec eax; test
//     edi, edi; lea edi, [eax + 1]`. That pattern is not a C spelling MSVC 5
//     will emit; it looks like an artefact of the register pressure in the
//     original, the same conclusion item B reaches from the other side.
//  F. Neutral or worse, all measured on this file, none applied: adding
//     <windows.h> (item 1 of the shared brief) 66.6%, 2038 bytes, no SIB
//     operand-order difference in this function to fix; dropping <math.h>
//     62.9%; dropping <stdio.h> 62.6%; `src = bitmap` hoisted above the
//     antiAlias `if` with `bmp` kept 66.6%; `(pflags >> 2) & 1` for the piece
//     bit 2 54.3% and `pflags != 0` 59.0% (the reload of `piece->flags` is
//     what the original has, and `piece->flags & 4` is right); `shade` as a
//     pointer or a struct field, neutral; `float* a = accum[k]` written out,
//     neutral; the accum zero stores in the order 2,1,0 65.4% and chained
//     `accum[k][2] = accum[k][1] = accum[k][0] = 0.0f` 66.6% (neutral); the
//     vertex fields read into vx, vy, vz before the mode test 64.3%, which is
//     surprising because that IS the original's load pattern; reading them
//     through `piece->vertices[k]` 64.6%; an explicit `int*` vertices walk
//     43.5%; the division loop's bound re-read from
//     `piece->info->vertexCount` 54.2% (confirms the existing note); the
//     division loop as a `for` or as `while (q1 < n)`, both neutral; the poly
//     loop walked with explicit `pp`/`ip` pointers 55.0%, `poly-temp` 50.1%;
//     `(f->flags.usePic) != 0` 65.5%, `f->flags.shaded != 0` 65.3%,
//     `f->flags.textured != 1` 65.6%; and `src = bitmap` kept only inside the
//     antiAlias arm 62.2%.
//
// THE HARNESS THIS PASS BUILT (reusable; all under build/scratch/0x459c70/).
// check.py re-parses the whole exe and data/symbols.csv on every run, which is
// most of its ~60 s. fastcheck.py parses both once and compiles and compares
// any number of candidates in the same process, 0.4 s each: it is
// check.py's Original, compile_source and compare, driven from a list.
// sweep.py applies text substitutions listed in variants.py to v0.cpp, writes
// each result to gen/<name>.cpp and ranks them. permdecl.py regenerates
// variants.py with N random orders of a declaration block. hunks.py prints one
// line per diff hunk with its original address, dump.py prints the original and
// our disassembly aligned instruction by instruction over a byte range, and
// slotscore.py reports the slot-blind score described in A. Run them as
//   wsl -d Ubuntu-24.04 -e bash .../build/scratch/0x459c70/sw.sh   (sweep)
//   wsl -d Ubuntu-24.04 -e bash .../build/scratch/0x459c70/pd.sh 150 (orders)
//   wsl -d Ubuntu-24.04 -e bash .../build/scratch/0x459c70/hk.sh  (hunks)
//   wsl -d Ubuntu-24.04 -e bash .../build/scratch/0x459c70/dm.sh 0x459c70 <src> <lo> <hi>
//   wsl -d Ubuntu-24.04 -e bash .../build/scratch/0x459c70/ss.sh <src> (slots)
// A first version of the loop-counters list in the original above (0x18 verts,
// 0x24 info) cannot be right as written: the original stores `ebx = [ecx]`,
// which is `piece->info`, to [esp+0x24] at 0x459de6, and 0x459fd1 loads
// [esp+0x24] into ebp to index the vertices at 0x459fd8. One of those two
// readings is wrong and it is worth settling, since the whole frame map
// argument rests on it.
//
// Counted check.py runs against this file: 163 in the previous pass (63.7 ->
// 66.6) and about 270 this pass, none of which beat 66.6%, of which 2 were
// full `bt.cmd check` runs on this file and the rest the same compile through
// the harness above.
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ddraw.h>

extern char* g_game;
extern float DAT_005065f8;
extern float DAT_005065fc;
extern float DAT_00506600;
extern const float DAT_004fd4cc;
struct Bitmap_459c70;

struct Vec3 { int x; int y; int z; };

struct Flags_37f06 {
    unsigned short damagebars : 1;
    unsigned short antiAlias : 1;
    unsigned short shadows : 1;
    unsigned short vehicleShadows : 1;
    unsigned short featureShadows : 1;
    unsigned short shading : 1;
    unsigned short ditheredFog : 1;
    unsigned short unused7 : 1;
    unsigned short switchAlt : 1;
};

struct Vec3f { float x; float y; float z; };

Vec3f __stdcall FUN_004b6f00(Vec3 a, Vec3 b);
Vec3f __stdcall FUN_004b6f70(Vec3f a, Vec3f b);
Vec3f __stdcall FUN_004b6ff0(Vec3f v);
void* __stdcall FUN_004b7ee0(void* pic);
void* __stdcall FUN_004b7f30(unsigned short* table, int index);
void __stdcall FUN_004b95a0(Bitmap_459c70* dst, Bitmap_459c70* src);
void __stdcall FUN_004c0c70(Bitmap_459c70* surface, void* poly, int count, int flag);
void __stdcall FUN_004c8bb0(Bitmap_459c70* surface, void* pic, void* poly, int flag);

struct Bitmap_459c70 {
    unsigned short width;            // +0x00
    unsigned short height;           // +0x02
    short field_4;                   // +0x04
    short field_6;                   // +0x06
    char colorKey;                   // +0x08
    char unknown_9[7];
    char* data;                      // +0x10
    char* data2;                     // +0x14
};

#pragma pack(push, 1)
struct Owner_459c70 {
    char unknown_0[0x92];
    char* field_92;                  // +0x92
    char unknown_96[0x104 - 0x96];
    float field_104;                 // +0x104
    char unknown_108[0x110 - 0x108];
    unsigned int field_110;          // +0x110
    unsigned char field_114;         // +0x114
};

struct FaceFlags_459c70 {
    unsigned int textured : 1;
    unsigned int usePic : 1;
    unsigned int shaded : 1;
    unsigned int rest : 29;
};

struct Face_459c70 {
    int unknown_0;                   // +0x00
    int count;                       // +0x04
    int unknown_8;                   // +0x08
    unsigned short* indices;         // +0x0c
    void* pic;                       // +0x10
    int unknown_14;                  // +0x14
    unsigned short* color;           // +0x18
    FaceFlags_459c70 flags;         // +0x1c
};

struct PieceInfo_459c70 {
    char unknown_0[4];
    int vertexCount;                 // +0x04
    int faceCount;                   // +0x08
    int firstFace;                   // +0x0c
    char unknown_10[8];
    unsigned short* color;           // +0x18
    char unknown_1c[0xc];
    Face_459c70* faces;              // +0x28
};

struct Piece_459c70 {
    PieceInfo_459c70* info;          // +0x00
    char unknown_4[0x22 - 0x04];
    Vec3* vertices;         // +0x22
    char unknown_26[0x28 - 0x26];
    unsigned char flags;             // +0x28
    char unknown_29[0x36 - 0x29];
};

struct List_459c70 {
    int count;                       // +0x00
    char unknown_4[8];
    Owner_459c70* owner;             // +0x0c
    Bitmap_459c70* bitmap;           // +0x10
    char unknown_14[0x22 - 0x14];
    Piece_459c70 pieces[1];          // +0x22
};
#pragma pack(pop)

struct Poly_459c70 { int x; int y; int z; int shade; };

struct Class_004581e0 {
    char unknown_0[0x10];
    Bitmap_459c70* shadow;           // +0x10
    void FUN_00459c70(Bitmap_459c70* bitmap, List_459c70* list, int kind, int useColor);
};

// The 50 or 125 bias the original materialises separately in each arm of the
// doubled-bitmap test, once per projected vertex.
static __inline int shade_bias(List_459c70* list)
{
    bool c = 0 != ((*(unsigned int*)((char*)list->owner->field_92 + 0x241) >> 30) & 1);
    int ret0;
    ret0 = c ? 125 : 50;
    return ret0;
}



// FUNCTION: 0x459c70
void Class_004581e0::FUN_00459c70(Bitmap_459c70* bitmap, List_459c70* list,
    int kind, int useColor)
{
    PieceInfo_459c70* info;
    Poly_459c70 poly[25];
    float accum[2000][3];
    Vec3f normal[2000];
    int weight[2000];
    Poly_459c70 vertex[2000];

    Bitmap_459c70* shadow;
    Bitmap_459c70* src;
    Bitmap_459c70* bmp;
    int mode;
    int offX;
    float w;
    char* s;
    char* d;
    Piece_459c70* piece;
    Vec3* verts;
    unsigned char pflags;

    if (((Flags_37f06*)(g_game + 0x37f06))->antiAlias) {
        if ((list->owner->field_110 & 0x20000000) != 0) {
            if (useColor != 0) {
                shadow = this->shadow;
                mode = 1;
                src = bitmap;
                shadow->width = (unsigned short)(bitmap->width << 1);
                shadow->height = (unsigned short)(bitmap->height << 1);
                shadow->unknown_9[0] = 0;
                shadow->colorKey = 1;
                shadow->field_4 = (short)(bitmap->field_4 << 1);
                shadow->field_6 = (short)(bitmap->field_6 << 1);
                memset(shadow->data2, 0, shadow->height * shadow->width);
                memset(shadow->data, 1, shadow->width * shadow->height);
                bmp = shadow;
                goto haveMode;
            }
        }
    }
    mode = 0;
    bmp = bitmap;
    src = bitmap;
haveMode:
    for (int p = list->count - 1; p >= 0; p--) {
        pflags = list->pieces[p].flags;
        piece = &list->pieces[p];
        if (!(list->pieces[p].flags & 1))
            continue;
        if (!(useColor == -1 || useColor == ((pflags >> 1) & 1)
                || list->owner->field_104 != 0.0f))
            continue;
        verts = piece->vertices;
        info = piece->info;
        int n = info->vertexCount;
        if (0 < n) {
            int k;
            int offY = (short)bmp->field_6;
            int shade = 0;
            int offX = (short)bmp->field_4;
            memset(weight, 0, n * sizeof(int));
            for (k = 0; k < n; k++) {
                int y;
                int z;
                int x;

                x = verts[k].x;
                if (mode != 0) {
                    int xs = x >> 16;
                    y = (short)(verts[k].y >> 16) << 1;
                    x = (short)xs << 1;
                    z = (short)(-verts[k].z >> 16) << 1;
                } else {
                    int vy = verts[k].y;
                    y = (short)(vy >> 16);
                    x = (short)(x >> 16);
                    z = (short)(-verts[k].z >> 16);
                }
                vertex[k].x = x;
                vertex[k].y = z - (y >> 1);
                if (mode != 0) {
                    int y2 = y / 2;
                    vertex[k].z = shade_bias(list) + y2;
                } else {
                    vertex[k].z = y + shade_bias(list);
                }
                vertex[k].shade = 0x1f & shade;
                vertex[k].x += offX;
                vertex[k].y = vertex[k].y + offY;
                shade += 3;
                accum[k][0] = 0.0f;
                accum[k][1] = 0.0f;
                accum[k][2] = 0.0f;
            }
        }

        {
            int fi;
            Face_459c70* face;
            if (info->firstFace != -1) {
                face = info->faces + 1;
                fi = 1;
            } else {
                face = info->faces;
                fi = 0;
            }
            for (; fi < info->faceCount; fi++, face++) {
                unsigned short* idx = face->indices;
                if (!(idx[0] == idx[1] || idx[1] == idx[2] || idx[2] == idx[0])) {
                    Vec3 p0 = verts[idx[1]];
                    Vec3 p1 = verts[idx[0]];
                    Vec3 p2 = verts[idx[2]];
                    Vec3f a = FUN_004b6f00(p0, p1);
                    Vec3f b = FUN_004b6f00(p0, p2);
                    Vec3f c = FUN_004b6f70(a, b);
                    normal[fi] = c;
                    c = FUN_004b6ff0(c);
                    normal[fi] = c;
                } else {
                    normal[fi].x = 0.0f;
                    normal[fi].y = 1.0f;
                    normal[fi].z = 0.0f;
                }
            }
        }

        {
            int i;
            Face_459c70* f;
            if (info->firstFace != -1) {
                i = 1;
                f = info->faces + 1;
            } else {
                f = info->faces;
                i = 0;
            }
            while (i < info->faceCount) {
                    unsigned short* idx;
                    int j;
                    idx = f->indices;
                    for (j = 0; j < f->count; j++) {
                        int k = idx[j];
                        float t0 = accum[k][0];
                        accum[k][0] = t0 + normal[i].x;
                        accum[k][1] += normal[i].y;
                        float t2 = accum[k][2];
                        accum[k][2] = t2 + normal[i].z;
                        weight[k]++;
                    }
                    i++, f++;
                }
        }

        int q1 = 0;
        while (n > q1) {
            if (weight[q1] != 0) {
                w = (float)weight[q1];
                    accum[q1][0] /= w;
            accum[q1][1] /= w;
            accum[q1][2] /= w;
            }
            q1++;
        }

        {
            int i;
            Face_459c70* f = info->faces;
            i = 0;
            if (info->firstFace != -1) {
                f = f + 1;
                i = 1;
            }
            for (; i < info->faceCount; ) {
                int j;
                unsigned short* idx;
                idx = f->indices;
                for (j = 0; j < f->count; j = j + 1) {
                    poly[j] = vertex[idx[j]];
                    if ((piece->flags & 4) != 0) {
                        float v = accum[idx[j]][0] * DAT_005065f8 + accum[idx[j]][1] * DAT_005065fc;
                        v += accum[idx[j]][2] * DAT_00506600;
                        poly[j].shade = 0x1f & ((int)(DAT_004fd4cc * v));
                    } else {
                        poly[j].shade = 0xf;
                    }
                }
                if (f->flags.textured == 0) {
                    if (f->count == 4) {
                        void* pic;
                        if (f->flags.usePic) {
                            if (f->flags.shaded) {
                                int unit = *(int*)((0x1b8a + g_game) + (kind * 0x14b));
                                pic = FUN_004b7f30(f->color,
                                    *(unsigned char*)(unit + 0x96));
                            } else if (useColor) {
                                pic = FUN_004b7f30(f->color, 0);
                            } else {
                                pic = FUN_004b7ee0(&f->pic);
                            }
                        } else {
                            pic = f->pic;
                        }
                        FUN_004c8bb0(bmp, pic, poly, 0);
                    }
                } else {
                    FUN_004c0c70(bmp, poly, f->count, f->unknown_0);
                }
                i++, f++;
            }
        }
    }

    if (((Flags_37f06*)(0x37f06 + g_game))->antiAlias) {
        if (mode != 0) {
            FUN_004b95a0(bmp, src);
            s = src->data2;
            if (s != 0) {
                int y = 0;
                d = bmp->data2;
                while (y < src->height) {
                    int x = src->width;
                    if (x != 0) {
                        do {
                            *s++ = *d;
                            d = d + 2;
                        } while (--x);
                    }
                    y++;
                    d += bmp->width;
                }
            }
        }
    }
}