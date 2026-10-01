// Decompiled by deepseek-v4.1-flash, verified by GPT-6.1-sol, finished by
// Parent trial: split the index load from the j increment (same 98.7%); residual add/load order is unchanged.
// space-bunny-free, retried by Sonnet 5.5, finished by deepseek-v4.1-flash. Names are provisional.
// GPT-6.1-sol retry (#3174): changing the face-index copy into a guarded do/while
// and incrementing j before the copy raises the best from 96.1% to 98.7%. The
// only remaining bytes are the order of add eax, 8 vs mov di, [edx] and
// inc ecx in the copy loop: target loads the index and increments j before
// advancing the destination cursor. Pointer-cursor and materialized-index
// spellings regressed to 91.9% and 73.0%; keep the guarded do/while.
// deepseek-v4.1-flash retry (#2977): 90.6 -> 96.1%. The first-face setup now
// matches instruction for instruction. The lever is to assign `face` in BOTH
// arms instead of incrementing it in one arm:
//     if (arr->firstFace != -1) { face = arr->faces + 1; i = 1; }
//     else                       { face = arr->faces;     i = 0; }
// A ternary (setne/shl/add) scored 90.6%, `i = 0; if (...) { i = 1; face++; }`
// scored 92.5% but spilled `i` twice (once per definition). The both-arms form
// keeps arr in ebp and stores `i` exactly once at the join, as the original
// does (0x4212a3..0x4212c1). Everything before and after this block matches.
// The ONLY remaining difference is the copy loop's ecx/edx tie: original gives
// idx=edx / j=ecx (`mov edx,[esi+0xc]; xor ecx,ecx; inc ecx` at the loop top),
// this file gives idx=ecx / j=edx (`mov ecx,[esi+0xc]; xor edx,edx`), which
// also flips the increment scheduling (ours `add eax,8` before the load, `inc`
// at the bottom; original `inc` at the top). Declaration order (idx/j at
// function or block scope, either order), unsigned counters, do/while, while,
// indexed source (`projected[idx[j]]`), dest walk and pointer-range loops are
// all flat at 96.1% with the same swap, so it is a compiler register tie-break,
// not a source lever.
// Sonnet 5.5 retry (#2451), still 90.6%. The diff of this version is now only
// the first-face setup (original: branches, `add esi,0x20; mov edi,1` or
// `xor edi,edi`; ours: setne/shl/add) and the ecx/edx swap in the copy loop,
// which follows from it (the setne sequence leaves ecx in use, so the copy
// loop's first temp lands in ecx instead of edx). Note that this 90.6% version
// already has the original's `i` spill at [esp+0x10] and `arr` in ebp; the
// branchy spelling (`if (arr->firstFace != -1) { face++; i = 1; } else i = 0;`)
// has the original's instructions but loses it (82.0%, 459 bytes: arr evicted
// instead of i). Not tried before, all no change (90.6% or 82.0% as before):
// `register` on arr, i and j; idx and j declared at function scope in either
// order, `for (idx = face->indices, j = 0; ...)`, a while-form copy loop; a
// sweep of 0..500 unused declarations on the branchy spelling; headers.py on
// the branchy spelling. Siblings 0x4584d0, 0x459830 and 0x459c70 use the
// branchy spelling and stop at the same tie, so they share this problem.
// PARTIAL, 90.6% (refined by GPT-6.1-sol). Changing the first-face setup to
// `i = arr->firstFace == -1 ? 0 : 1; face += i;` moves the score from 82.0% to
// 90.6%. The remaining main difference starts at the outer face-loop setup:
// original branches to increment `face` and stores the counter in the frame;
// this version forms `face + i` with a shift/add. In the polygon copy loop,
// original uses edx for indices and ecx for j, while this version uses ecx for
// indices and edx for j. Tried `face++` under `if (i)`, which scored 82.5%.
// PARTIAL, 82.0% (fourth pass: unchanged). What still differs is ONE eviction:
// the original spills the face counter `i` to frame+0x10 (`mov [esp+0x10],edi`
// at 0x4212bd, reloaded by `mov edi,[esp+0x10]` at 0x421307, stored again at
// 0x42138e) and keeps `arr` in ebp, while this file keeps `i` in edi and leaves
// `arr` in the frame+0x10 slot (it reloaded that slot at 0x42129f for the
// vertex loop and reloads it again after the copy loop). Everything downstream
// is a consequence: with `i` in edi the copy loop has to borrow ebx for its
// index and ebp for the loaded point, so it runs ecx = indices / edx = j and
// reloads `surface` then `arr`, where the original runs ecx = j / edx = indices
// and reloads `i` then `surface`. Everything before 0x4212ba and after the
// dispatch already matches instruction for instruction.
// Two more negative results on that tie (both measured with check.py --sym):
// 1. headers.py over all 128 sets is flat at 82.0%, so no header side effect.
// 2. Neither declaration order nor loop weighting decides it. Declaring `arr`
// before `int i;` (so C1 sees the variable the original keeps first) is exactly
// 82.0%, and giving the vertex loop its own counter so that `i` is referenced in
// only one loop instead of two (which would change MSVC's loop-weighted register
// priority, the lever that worked at 0x40e160) is also exactly 82.0% with the
// same diff hunk, and the frame does not move. The tie is not decided by
// declaration order, use count or loop depth; it needs a construct that changes
// how many callee-saved registers the loop body wants at once.
// PARTIAL, 82.0%. Lead probes: taking the face index address and using it
// through a pointer kept 82.0%; hoisting faceCount scored 75.8%, reversing the
// loop comparison scored 81.3%, and reversing the firstFace branch scored 80.0%.
// Restored this best version.
// PARTIAL, 82.0%. Frame size (0x3f58), the prologue, the vertex loop and the
// flag dispatch now match the original instruction for instruction. What
// still differs is one register-priority tie, the same one 0x4584d0 hit:
// - the face counter `i` keeps edi here, so the pre-loop test is
//   `cmp edi,[ebp+8]` and there is no `mov [esp+0x10],edi`; the original
//   spills `i` to [esp+0x10] (0x4212bd and 0x42138e) and reloads it at
//   0x421307, which frees edi for the copy-loop index.
// - consequently the copy loop runs `edx = j, ecx = indices, ebx = index`
//   here vs the original's `ecx = j, edx = indices, edi = index`, and after
//   it the original reloads `i` then `surface` where this file reloads
//   `surface` then `arr`.
// What fixed the prologue: take the address of the offset aggregate once
// (`short* hp = (short*)&off;`) and read the high words as hp[1]/hp[3]/hp[5].
// That forces MSVC to spill `off` and emit the original's
// `movsx r, word ptr [esp+0x16/0x1a/0x1e]` sequence (54.4 -> 64.2).
// What fixed the vertex loop: make BOTH accesses indexed (`projected[i].x`
// with `v[i].x`) so MSVC biases the destination by +4, then switch the
// SOURCE to a walked pointer `Vec3* u = v; for (; ; i++, u++)` with
// `u->x/u->z/u->y`. Indexed destination plus walked source is byte-exact
// (73.4 -> 82.0); walked destination plus indexed source and both-walked are
// both much worse.
// The flag tests come from the bitfield union Flags_004211d0, copied from the
// already-partial near-copy 0x4584d0.cpp: it emits the original's
// `shr ecx,1 / test cl,1` and `shr eax,2 / test al,1` chain.
struct Point_004211d0 { int x; int y; };
struct Vec3_004b6cc0 { int x; int y; int z; };

struct Pic_004211d0 { void* pic; int unknown_4; };

struct Flags_004211d0 {
    union {
        unsigned int raw;
        struct {
            unsigned int a : 1;
            unsigned int b : 1;
            unsigned int c : 1;
            unsigned int rest : 29;
        } bits;
    };
};

struct Face_004211d0 {
    int unknown_0;                   // +0x00
    int count;                       // +0x04
    int unknown_8;                   // +0x08
    unsigned short* indices;         // +0x0c
    Pic_004211d0 pic;                // +0x10
    unsigned short* color;           // +0x18
    Flags_004211d0 flags;            // +0x1c
};

struct Arr_00421550 {
    char unknown_0[4];
    int count;                       // +0x04
    int faceCount;                   // +0x08
    int firstFace;                   // +0x0c
    char unknown_10[0x14];
    Vec3_004b6cc0* items;            // +0x24
    Face_004211d0* faces;            // +0x28
};

#pragma pack(push, 1)
struct Inner_00421550 {
    Arr_00421550* f0;                // +0x00
    char unknown_4[0xc];
    short f10;                       // +0x10
    short f12;                       // +0x12
    short f14;                       // +0x14
    int f16;                         // +0x16
    int f1a;                         // +0x1a
    int f1e;                         // +0x1e
    Vec3_004b6cc0* f22;              // +0x22
};
#pragma pack(pop)

struct Obj_00421170 {
    int f0;                          // +0x00
};

#pragma pack(push, 1)
struct Game_004211d0 {
    char unknown_0[0x1431f];
    int cameraX;                     // +0x1431f
    int cameraZ;                     // +0x14323
    char unknown_14327[0x37e27 - 0x14327];
    int viewport[4];                 // +0x37e27
};
#pragma pack(pop)

extern Game_004211d0* g_game;

int __stdcall FUN_004b6720(void* rect, int x, int y);
void* __stdcall FUN_004b7ee0(Pic_004211d0* pic);
void* __stdcall FUN_004b7f30(unsigned short* table, int index);
void __stdcall FUN_004c0310(void* surface, Point_004211d0* points, int count, int flags);
void __stdcall FUN_004c7580(void* surface, void* pic, Point_004211d0* points, void* src);

// FUNCTION: 0x4211d0
void __stdcall FUN_004211d0(void* surface, Obj_00421170* obj, Inner_00421550* inner)
{
    Point_004211d0 projected[2000];
    Point_004211d0 poly[25];
    int i;

    Arr_00421550* arr = inner->f0;
    struct Off_004211d0 { int a; int y; int b; };
    Off_004211d0 off;
    off.a = inner->f16 - (g_game->cameraX << 16);
    off.y = inner->f1a;
    off.b = inner->f1e - (g_game->cameraZ << 16);

    short* hp = (short*)&off;
    int sy = hp[5] - (hp[3] >> 1) + 0x20;
    int sx = hp[1] + 0x80;
    if (!FUN_004b6720(&g_game->viewport[0], sx, sy)) {
        return;
    }

    Vec3_004b6cc0* v = inner->f22;
    Vec3_004b6cc0* u = v;
    for (i = 0; i < arr->count; i++, u++) {
        projected[i].x = (short)((u->x + off.a) >> 16) + 0x80;
        projected[i].y = (short)((off.b - u->z) >> 16)
            - ((short)((u->y + off.y) >> 16) >> 1) + 0x20;
    }

    int j;
    Face_004211d0* face;
    if (arr->firstFace != -1) {
        face = arr->faces + 1;
        i = 1;
    } else {
        face = arr->faces;
        i = 0;
    }
    for (; i < arr->faceCount; i++, face++) {
        unsigned short* idx = face->indices;
        j = 0;
        if (face->count > 0) {
            do {
                ++j;
                poly[j - 1] = projected[*idx++];
            } while (j < face->count);
        }
        Flags_004211d0 flags = face->flags;
        if (!flags.bits.a) {
            if (face->count == 4) {
                void* pic;
                if (flags.bits.b) {
                    if (flags.bits.c) {
                        int player = *(int*)(*(int*)(obj->f0 + 0x96) + 0x27);
                        pic = FUN_004b7f30(face->color,
                            *(unsigned char*)(player + 0x96));
                    } else {
                        pic = FUN_004b7ee0(&face->pic);
                    }
                } else {
                    pic = face->pic.pic;
                }
                FUN_004c7580(surface, pic, poly, 0);
            }
        } else {
            FUN_004c0310(surface, poly, face->count, face->unknown_0);
        }
    }
}