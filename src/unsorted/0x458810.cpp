// Decompiled by GPT-6-Luna, finished by Space Bunny Free, finished by deepseek-v4.1-flash, finished by space-bunny-free, edited by deepseek-v4.1. Names are provisional.
// PARTIAL, 87.6% (410 of 427 bytes; up from 81.9%).
//
// WHAT IS SOLVED. The piece array starts at list+0x22, not +0x44, with `info`
// at piece+0, `vertices` at +0x22 and `flags` at +0x28 on a 0x36 stride, under
// `#pragma pack(push,1)`. The `Vec3` really is a local, materialised at
// frame+0x1c and passed by pointer to 0x4584d0, while 0x459200 takes it *by
// value*, and getting that parameter order right is what produces the exact
// `sub esp,0xc / mov edx,esp / push list` interleaving. The two flag conditions
// must be written as inline `owner->flags & 0x20000000` expressions rather than
// through an `int special` local: the local lets MSVC common the two uses, and
// the inline form is what forces the spill to frame+0x18 and the reload for the
// second test, which also fixes the frame at 0x18 and puts the bitmap in ebx and
// the flags in edx (`test dh,0x20`). The prologue then falls out of two changes
// together: moving `int rebuild = 0;` above the two `g_game` reads *and* deleting
// the second `bitmap = list->bitmap;`. That reassignment alone was flipping the
// whole function's allocation, and together they give the original's
// esi=x, ebp=z, edi=list, ecx=owner exactly. `kind` is `unsigned char`, and so
// is the sixth parameter of 0x4584d0, which drops the `xor ecx,ecx` before the
// byte load. The `visible` decision needs `int t = (unsigned char)~owner->field_10e;
// if (t & 1) ... else ...`, which is the only spelling of about sixteen that emits
// the extra `and eax,0xff` in the `not al` sequence.
//
// WHERE THE 12 POINTS ARE, restated after a second pass (Space Bunny Free).
// The earlier note here claimed the 81.9% shape is a miscompilation because it
// "reloads `this` from two different stack slots, one of which holds the
// `special` value". THAT IS WRONG, and I have removed it. Recounting the
// outstanding pushes, all three `this` reloads read post+0x14, which is exactly
// where the prologue stored `this`:
//   0x1c  mov [esp+0x14],ecx            esp=post   -> post+0x14, the `this` home
//   0xe6  mov ecx,[esp+0x14]            esp=post   -> post+0x14  (before the call)
//   0x114 mov ecx,[esp+0x18]            esp=post-4 -> post+0x14  (after `push ecx`)
//   0x174 mov ecx,[esp+0x28]            esp=post-0x14 -> post+0x14 (5 pushes)
// `special` really is at post+0x18, and nothing reads it as `this`. So this
// file is semantically faithful, and 81.9% is honest progress, not a lucky
// miscompilation. Keep it.
//
// THE REAL SPLIT, and it is a register-priority tie I could not break.
// The original does two things at once that this source cannot do at once:
//   (a) it re-reads `list->bitmap` into ecx AFTER the 0x4586a0 call (0x107),
//       so the pre-call bitmap value is dead and ebx is freed for `this`;
//   (b) it still has esi=x, ebp=z, edi=list, ebx=this in the prologue.
// Spelling the tail as `if (list->bitmap != 0)` is the one-token change that
// forces (a): it frees ebx, MSVC promotes `this` to ebx, the loop counter is
// pushed out of ebx into the dead `this` slot at post+0x14, and the WHOLE tail
// then matches, including both `mov ebx,[esp+0x14]` copies, the `mov ecx,ebx`
// at each call site and the counter's `mov [esp+0x14],eax / dec / mov`. That
// variant is 423 of 427 bytes, i.e. four bytes of `lea`/`inc` reordering away.
//
// But (a) costs (b): with `this` promoted, MSVC gives `list` to esi and lets
// `coords.x` have edi, where the original has them the other way round. I
// measured about forty shapes of the reload family (statement order of
// x/z/rebuild/owner/visible, coords field order, `list->owner` vs `owner`,
// `&list->pieces[i]` vs `list->pieces + i`, a second `List*` copy, a second
// `Bitmap*` copy, a `Bitmap**` pointer-to-field, a `self` copy of `this` used
// at all three call sites, and declaring the g_game reads directly into
// `coords.x`/`coords.z`) and EVERY one of them scores exactly 69.4% with the
// same esi/edi split. It is a priority tie, not a scheduling accident: do not
// re-sweep it. The no-reload family is equally flat at exactly 81.9% across
// about twenty-five shapes. Both ceilings are recorded, with the variants, in
// build/scratch/0x458810/ (v0.cpp is this file, v1.cpp the reload family, and
// sw1..sw13 the sweeps).
//
// FRAME ARITHMETIC worth keeping. Only two locals are inside the 0x18 frame:
// `rebuild` at post+0x10 and the `this` copy at post+0x14. `special` is at
// post+0x18 and the Vec3 at post+0x1c..0x27, that is, MSVC put them ON TOP OF
// the ebx/esi/ebp/edi save area, which is legal because none of those four is
// read again before its `pop`. That is why the reload of `coords.y` reads
// post+0x20, i.e. the saved ebp slot: the value in ebp on entry. It is the
// caller's ebp, not `z`, because the `push ebp` at 0x458819 happens before
// `mov ebp,[eax+0x14323]`. So the original really does copy an uninitialised
// int into coords.y, and this file models that with the uninitialised
// `local_8`; deleting `local_8` and never assigning coords.y drops this file
// to 71.4%, so the explicit uninitialised local is required.
//
// ALSO STILL DIFFERENT. The duplicated `test eax, eax; jne` after
// `mov eax, [ebx+0x14]` (0x4588bd/0x4588bf, 3 bytes). The byte pattern
// `85 c0 75 0c 85 c0` has exactly ONE hit in the whole exe, here, so it is not
// a shared idiom; I tried seven spellings (a doubled `&&` operand, a nested
// `if (c) if (c)`, `!(c != 0) && !(c != 0)`, and a separate statement) and
// MSVC 5 folds every one of them, so this one is not reachable from a
// plausible source spelling. And `coords.y` is read from the arg2 slot at
// [esp+0x30] here rather than from post+0x20; both are "an uninitialised
// slot", 0x10 apart.
//
// THIRD PASS (Space Bunny Free). Re-measured both families and added the missing
// one. Deleting the `bitmap` local entirely and spelling every use as
// `list->bitmap` (so MSVC reloads wherever it must) is NOT a third family: it
// scores exactly 69.4%, the same as the tail-only reload, because a single
// `list->bitmap` read after the 0x4586a0 call is all it takes to promote `list`.
// The ceiling is therefore two-valued and real: 81.9% (no reload) against 69.4%
// (reload), and the gap is the esi/edi priority tie, not the loop.
// On the loop itself, four shapes that try to move the induction variable into
// the frame slot at post+0x14 the way the original has it, while leaving the
// prologue's esi=x / edi=list alone, all lose: `for (n = count; n > 0; n--)`
// over `pieces[n-1]` 78.3%, a `while (n > 0)` with `n--` 79.4%, a manual
// pointer walk with the counter used only by the test (`piece--; n--;`) 76.9%.
// A named `Class_004581e0* self = this;` used at both explicit call sites is
// byte-for-byte identical to the `this` form, 81.9%. So the this-versus-counter
// choice is not reachable from the loop shape either, which is consistent with
// the note above that the difference is a register priority tie.
// Variants live in build/scratch/0x458810/ (v0 this file, v1 tail reload,
// j reload everywhere, a/b/e/f/i loop shapes, g self copy).
//
// FOURTH PASS (space-bunny-free). Three findings, none of which moves the score,
// recorded so nobody repeats them.
//  1. The /Gz lever is ALREADY pulled. The harness compiles this file so that the
//     plain `__thiscall` definition already emits `ret 8` and mangles as
//     `?FUN_00458810@Class_004581e0@@QAEXPAUList_458810@@PAUVec3_458810@@@Z`
//     (QAE). Writing `__stdcall` on the definition mangles it QAGX instead and
//     check.py then cannot even pick the function out of the object, so there is
//     no <xutility>/<algorithm> stand-in to add here: this file calls no template
//     and no out-of-line helper besides the three real members. headers.py's 128
//     header sets are all 81.9% as well.
//  2. The tail-reload family, in its best spelling (`if (list->bitmap != 0)` after
//     the 0x4586a0 call, so ebx is freed and MSVC promotes `this` into it), is 423
//     of 427 bytes and matches the whole tail and the whole loop byte for byte.
//     The ONE thing it gets wrong is the prologue: it gives esi to `list` and edi
//     to the shifted `g_game` x, where the original has esi=x and edi=list. That
//     is the esi/edi priority tie recorded above, confirmed here to be the only
//     remaining difference and not a knock-on: a named `self` copy of `this` used
//     at both call sites, a second `List*` copy `l` used for every field access,
//     and hoisting `int visible` above the two g_game reads all still emit esi=list
//     and edi=x (66.9% each, vC/w1/w2/w3 in build/scratch/0x458810/). Note that
//     the original's `mov [esp+0x14],ecx` write of `this` in the prologue is
//     absent in that family, which is the same tie seen from the other side.
//  3. The loop is a genuine pointer walk, not `&list->pieces[i]`: the original
//     jumps back from 0x4589a8 to the flag test at 0x458972 and only does
//     `sub esi,0x36`, while this file's index form re-derives the address with the
//     `lea ecx,[eax+eax*2] / lea ecx,[ecx+ecx*8] / lea esi,[edi+ecx*2+0x44]` chain
//     every iteration. A manual `p = &list->pieces[n-1]; ... p--; n--;` walk is
//     76.9% on its own and 66.9% combined with the tail reload, i.e. it is not a
//     free win on top of the 81.9% shape either (vA/vB/vC/vD).
// So the 81.9% here is still the best known. The two open items are unchanged:
// the esi/edi priority tie, and the doubled `test eax, eax` at 0x4588bd.
//
// No suspected bug in the original. The duplicated `test eax, eax` is redundant
// and `coords.y` is an uninitialised int read out of the save area, but both look
// like ordinary MSVC artefacts of how the source was written rather than mistakes
// by Cavedog.
//
// deepseek-v4.1-flash confirmation pass. Re-measured the reload family with the
// one-line `if (list->bitmap != 0)` tail: 423 of 427 bytes, 69.4%. The ONLY
// difference left in that variant is that the compiler promotes `list` to esi
// and `x` to edi; the whole tail (ebx=this, the fresh `mov ecx,[esi+0x10]`
// reload, `mov ecx,ebx` at each call, the loop counter in the dead this slot)
// matches byte for byte. So the tail-reload and the prologue's esi=edi swap are
// one tie, exactly as recorded. The doubled `test eax,eax` was retried as
// `bitmap->field_14 == 0 && bitmap->field_14 == 0` and it still folds to the
// single test (406 bytes, 81.9%). Both ceilings stand: 81.9% no-reload against
// 69.4% reload, and 81.9% is kept here.
// UPDATE (deepseek-v4.1): 410 of 427 bytes, 87.6%. The doubled `test eax,eax`
// is SOLVED. Spelling the last conjunct twice, once through the local and once
// through `list->bitmap`, is what the original wrote:
//     && list->bitmap->field_14 == 0
//     && bitmap->field_14 == 0
// The frontend cannot fold two structurally different member accesses, the
// backend still CSEs both loads into one `mov eax,[ebx+0x14]`, and the result
// is the original's two adjacent `test eax,eax / jne` pairs. Writing the same
// expression twice (or via a `(char*)` cast) folds and stays at 406 bytes.
//
// WHAT REMAINS, exactly the four hunk groups below, all one root cause: the
// original frees ebx after the branch and lets `this` live in ebx from 0x4588fa
// to the end, while this source keeps `bitmap` (a source variable, so its value
// survives the 0x4586a0 call in ebx) live into the tail. Effects:
//   0x4588fa `mov ebx,[esp+0x14]` and its twin at 0x458913 (this -> ebx) are
//   missing; the rebuild call reloads ecx from the slot instead of `mov ecx,ebx`.
//   0x458917 the original RELOADS `list->bitmap` into ecx (0x107 mov ecx,[edi+0x10])
//   and this source tests the stale ebx instead (0x45891e `test ecx,ecx`).
//   0x45891a `mov eax,[esp+0x20]`: the uninitialised `local_8` is homed in
//   coords.y itself in the original (a self-copy), here it is homed in the
//   `result` argument slot, so the load reads [esp+0x30].
//   0x45895f onward: with `this` in ebx the original reloads `result` into ebp
//   and keeps the loop counter in memory at [esp+0x14] (`inc eax` /
//   `mov [esp+0x14],eax` / reload / `dec`), where this source has result in ebx
//   and the counter in ebp (`lea ebp,[eax+1]` / `dec ebp`), plus `mov ecx,ebx`
//   at the FUN_004584d0 call instead of a slot reload.
// Every reload spelling tried (a fresh `Bitmap*` local, `if (list->bitmap)`,
// `bitmap = list->bitmap;`, an early-declared late-assigned `bitmap2`, a `self`
// copy of `this` used at the call sites) produces the CORRECT tail (this in ebx,
// mov ecx,ebx, counter in memory, bitmap reload in ecx) at 425 bytes, but the
// allocator then recolours the whole function: `list` moves from edi to esi and
// `coords.x` from esi to edi, and the pre-branch bitmap moves from ebx to edx,
// so the score drops to 57.2%. The pre-branch plan here (esi=x, ebp=z, edi=list,
// ebx=bitmap, this spilled) is correct and must not be disturbed.
extern char* g_game;

struct Vertex_458810 { int x; int y; int z; };

struct Owner_458810 {
    void* relation;               // +0x00
    char unknown_4[0x1c];
    int field_20;                 // +0x20
    char unknown_24[0xff - 0x24];
    unsigned char kind;           // +0xff
    char unknown_100[4];
    float intensity;              // +0x104
    char unknown_108[6];
    unsigned char field_10e;      // +0x10e
    char unknown_10f;
    unsigned int flags;           // +0x110
    unsigned char field_114;      // +0x114
    char unknown_115[3];
};

struct Bitmap_458810 {
    char unknown_0[0x14];
    int field_14;                 // +0x14
};

struct PieceInfo_458810 { char unknown_0[4]; int vertexCount; };

#pragma pack(push, 1)
struct Piece_458810 {
    PieceInfo_458810* info;       // +0x00
    char unknown_4[0x1e];
    Vertex_458810* vertices;      // +0x22
    char unknown_26[2];
    unsigned char flags;          // +0x28
    char unknown_29[0xd];
};

struct List_458810 {
    int pieceCount;               // +0x00
    int frame;                    // +0x04
    char unknown_8[4];
    Owner_458810* owner;          // +0x0c
    Bitmap_458810* bitmap;        // +0x10
    int field_14;                 // +0x14
    char unknown_18[0x22 - 0x18];
    Piece_458810 pieces[1];       // +0x22
};
#pragma pack(pop)

struct Vec3_458810;

class Class_004584d0 {
public:
    void FUN_004584d0(List_458810* list, Vec3_458810* param_2, void* param_3,
        PieceInfo_458810* info, Vertex_458810* vertices, unsigned char kind, int visible);
};

class Class_00459200 {
public:
    void FUN_00459200(void* param_1, List_458810* list, Vec3_458810 coords, int visible);
};

class Class_004581e0 {
public:
    int FUN_004586a0(List_458810* list, int param_2, int param_3);
    void FUN_00458810(List_458810* list, Vec3_458810* result);
};

struct Vec3_458810 { int x; int y; int z; };

// FUNCTION: 0x458810
void Class_004581e0::FUN_00458810(List_458810* list, Vec3_458810* result)
{
    int rebuild = 0;
    int x = *(int*)(g_game + 0x1431f) << 16;
    int z = *(int*)(g_game + 0x14323) << 16;
    int visible;
    Owner_458810* owner = list->owner;
    if (owner->flags & 0x20000000) {
        int t = (unsigned char)~owner->field_10e;
        if (t & 1)
            visible = 1;
        else
            visible = 0;
    } else {
        visible = *(int*)((char*)owner->relation + 0x20) == 0;
    }
    if (list->frame == 0)
        rebuild = 1;
    Bitmap_458810* bitmap = list->bitmap;
    if (bitmap == 0)
        list->field_14 = 0;
    if ((owner->flags & 0x20000000) != 0) {
        if (bitmap == 0
            || (owner->intensity != 0.0f
                && (owner->flags & 0x2000) != 0
                && list->bitmap->field_14 == 0
                && bitmap->field_14 == 0))
            rebuild = 1;
    }
    if (bitmap == 0 && (owner->flags & 0x20000000) != 0)
        rebuild = 1;
    if ((owner->field_114 & 1) != 0 && bitmap == 0)
        rebuild = 1;
    if (rebuild) {
        list->field_14 = 0;
        FUN_004586a0(list, 0, 1);
    }
    Vec3_458810 coords;
    int local_8;                  // uninitialised, as in the original's stack slot
    coords.x = x;
    coords.y = local_8;
    coords.z = z;
    if (bitmap != 0) {
        ((Class_00459200*)this)->FUN_00459200(result, list, coords, visible);
        list->frame++;
        return;
    }
    for (int i = list->pieceCount - 1; i >= 0; i--) {
        Piece_458810* piece = &list->pieces[i];
        if (piece->flags & 1) {
            unsigned char kind = list->owner->kind;
            ((Class_004584d0*)this)->FUN_004584d0(list, result, &coords, piece->info,
                piece->vertices, kind, visible);
        }
    }
    list->frame++;
}
