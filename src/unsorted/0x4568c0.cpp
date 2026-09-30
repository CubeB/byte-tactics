// Decompiled by deepseek-v4.1-flash, finished by space-bunny-free, edited by
// deepseek-v4.1, finished by deepseek-v4.1-flash. Names are provisional.
// Partial, 78.9%. A short loop index restores three induction registers.
// Remaining difference, a single 4-byte stack slot. The original allocates
// 0x34 (sub esp,0x34) and keeps only two dword locals below the candidate
// array: esp+0x14 and esp+0x18, with cand[10] at esp+0x1c. esp+0x14 is
// reused three times by MSVC, first for the inlined index loop's byte
// counter, then for `int* out = &g_game->field_29fc` (0x456977 stores it
// there), then for the returning flag (0x456b7b stores the immediate 1,
// 0x456bc1 stores 0, and every epilogue reads [esp+0x14] into eax). res is
// the only other local, at esp+0x18. This file merges the index with the
// flag but gives `out` its own slot at esp+0x18, so res moves down to
// esp+0x1c and cand to esp+0x20, and the frame is 0x38: every [esp+..]
// reference in the body is off by 4. Making out share esp+0x14 needs the
// index variable to be gone from the source (the call site read at
// 0x45691b is the inlined helper's own counter), and inlining the helper
// call into the players[] expression was tried and changed nothing.
// Otherwise the body matches; the remaining misses are the readiness tests
// around 0x456b91 (field_29a4 / field_29d0) and register scheduling.
//
// deepseek-v4.1-flash re-checked the frame problem and confirmed the
// coefficient map from a /Fa listing. i/idx/ret sit at -0x34 and out at
// -0x30 here, while the original has all four at -0x30; res and cand are
// already at the original's absolute offsets. So only the i/idx/ret group
// needs to fold into out's slot and the frame drops from 0x38 to 0x34.
// Nothing tried moved it: inlining FindOccupied into the players[] index
// (vA), declaring out before res and assigning in place (vR), a function
// scope `int* out;` (vB), and a block scoping the idx local (vD) all score
// 78.9%; dropping the out local entirely scores 64.1% (vC) and assigning
// out at the top 76.6% (vS). Reordering the tail so res==0 is the
// fall-through path matches the original's `jne` but still scores 78.4%
// (the frame dominates), and changing the k4 loop counter from
// unsigned short to int (the original compares the pointer offset against
// 0x29f8, cmp bx,0xa here) drops to 77.1%.
// deepseek-v4.1-flash: PlayerId_004568c0 written with early returns
// (return id; / return -1;) instead of an `int id = -1;` assigned in an if.
// That stops MSVC spilling `id` and raises 78.9% to 79.4% (1298 to 1308
// bytes; the original is 1310).
// The early-return PlayerId was the shared upstream cause: with `id` no
// longer holding a stack slot, the k4 loop index can be a plain `int k4`
// (was `unsigned short k4`), which reproduces the original's single
// induction form and the exact 1310-byte size, 82.4%.
// deepseek-v4.1-flash then split the readiness test into two separate ifs
// (`if (res != 0) {...}` followed by `if (res == 0) {...}`) instead of
// if/else. That reproduces the original's redundant `cmp edi,ebx; jne`
// re-test at 0x456ba8 and stops MSVC hoisting `field_29a4[i]` into a
// register, 82.4% to 83.4%.
// Current state: 83.4%, exactly 1310 bytes. Remaining difference is still
// the one 4-byte stack slot (sub esp,0x38 vs 0x34): `out` does not share
// the index slot at esp+0x14. Everything at or below it (res esp+0x18,
// cand esp+0x1c) and the k4 loop's induction registers follow from that.
// headers.py changes nothing (all 128 sets 83.4%). The k4 loop still
// spills its `int` counter to memory and uses edi for the 0x29d0 offset,
// where the original keeps the counter in ebx and esi for the offset.
// deepseek-v4.1 (this pass): the shared slot at esp+0x14 comes from making
// `out` and the returning flag ONE variable of type int* (assign the pointer
// at the top, later assign (int*)1 and (int*)0, return (int)out). That
// reproduces the original's store at 0x456977, the reload at 0x456aea and
// the 0x456b7b / 0x456bc1 flag stores all at esp+0x14, raising 83.4% to
// 83.9%. Declaring out at function scope, as an array reference, as const, or
// dropping the named idx local changes nothing. What still differs is the k4
// loop: our build spills its int counter to esp+0x18 (the original's res
// slot) and keeps the field_29d0 byte offset in edi, while the original keeps
// the counter in ebx, the offset in esi and puts the PlayerId result in edi
// (ours lands it in ebx, which clobbers the counter). Computing `from` before
// `to` scores 79.3% at 1292 bytes, so the `to`-first order is kept.
// deepseek-v4.1 (this pass): the k4 loop now writes the PlayerId test out by
// hand instead of calling PlayerId_004568c0(k4):
//     int to = -1;
//     if (k4 != 10 && g_game->players[k4].state != 0) to = g_game->players[k4].id;
// That removes the 4-byte counter spill slot, so the frame is the original's
// 0x34 and res/cand sit at esp+0x18/esp+0x1c: every [esp+N] reference above
// the k4 loop now matches. 83.9% -> 86.0% (1266 bytes; the original is 1310).
// What still differs, all of it inside the res != 0 k4 loop at 0x456bd6:
//  - the original keeps a k4 byte counter in ebx (xor ebx,ebx at 0x456bcd,
//    cmp bl,0xa at 0x456c06, inc ebx at 0x456cb3), because the helper's
//    unsigned char parameter is what stops MSVC from folding `k4 != 10` into
//    the `cmp esi,0x29f8` loop test. Here k4 is folded away entirely
//    (no xor ebx,ebx / inc ebx) and the state test is merged into `test al,al`.
//  - with ebx free, `to` lands in ebx and `from` in edi; the original has
//    `to` in edi and `from` in edx (its from loop does `add edx,eax`, reusing
//    the g_game register, which is reloaded at 0x456caa).
//  - the same folding drops the original's redundant active re-test at
//    0x456c7c (mov eax,[edx+ebp+0x1b63] / test eax,eax / je).
//  - the tail's out test is `test eax,eax / jne <res!=0 path>` in the
//    original but `je <res==0 path>` here (branch order, 3 bytes).
// Tried and rejected this pass, each scored lower: declaring `from` before
// `to` 83.9; from loop as a helper 78.0; (unsigned char) casts on k4 79.3 and
// 79.9; a byte pi local 83.9; unsigned char k4 82.7; j at function scope
// 83.9; the previous helper-call form of `to` 83.9.
// deepseek-v4.1 (frame pass): the helper-call form of `to` really is the
// original shape (it reproduces xor ebx,ebx / cmp bl,0xa / inc ebx, the fresh
// `and eax,0xff` address computation and the else branch's reload), but with
// named `to`/`from` locals it makes k4 need a stack home at esp+0x18, so the
// frame goes to 0x38 and the score drops to 83.9. Writing the from scan as an
// inlined helper and passing it straight to the call
// (FUN_00451bc0(FindFrom_004568c0(), PlayerId_004568c0(k4), packet, 2))
// removes that home: the frame is the original 0x34 and the field_29d0 walk
// lands in esi as in the original, at 83.4, and its only remaining difference
// is that `res` gets ebp instead of edi (the players walk and `to` take the
// other slot of that pair). Swapping the out/res declarations does not move it.
// deepseek-v4.1-flash (this pass) confirmed the manual-expansion form at
// 86.0% (1266 bytes) is the best of everything tried. Every variant that
// restores the byte counter ebx in the k4 loop (PlayerId called as an inlined
// helper, with the from scan either an inlined helper or a local loop) gets the
// original's `xor ebx,ebx` / `cmp bl,0xa` / `inc ebx` and usually the original's
// 1310-byte IV structure, but it moves `res` from edi to ebp, which costs about
// 12 points and reshuffles the whole tail. A byte or short `pi` local instead
// forces the frame to 0x38 (79.9). Named `to` (83.9) and a FindFrom returning
// through a local `r` (83.9) also grow the frame to 0x38. FindFrom with an
// unsigned char `j` (75.1), non-inline helpers (83.4), <string.h> (83.4),
// dummy-extern sweeps N = 0..384 (flat 83.4) and headers.py (all 128 sets
// 86.0) did not help. The byte counter and `res` in edi look mutually
// exclusive in this source shape, so the next step is a construct that keeps
// the k4 counter live without demoting res.
// deepseek-v4.1-flash (last pass, out of time): left the 86.5% body as is and
// only analysed the remaining k4 loop diff. Findings for whoever continues:
// (1) the original's `to` and `from` both end in a shared `or reg,-1` tail
// block fed by two `je`s, which is the inlined early-return helper shape, not
// `int to = -1; if (...) to = ...;` (our form hoists the or before the tests);
// the from scan is an early-return helper too (jmp notfound / mov reg,id /
// jmp after / or reg,-1). (2) The original re-tests active in the else branch
// (0x456c7c) and reloads state there, so the condition must be
// `if (active && state == 3) {...} else if (active && (state==1||state==2))`
// as one combined first test, not an outer active wrapper. (3) The k4 byte
// counter in ebx with `cmp bl,0xa` and `and eax,0xff` needs the unsigned char
// narrowing of PlayerId, so the k4 site must call PlayerId_004568c0(k4) or
// index through an unsigned char. Variants vA (byte pi + early-return to +
// FindFrom helper), vB (helper-in-args with early-return FindFrom) and vC
// (restructure only) were generated under build/scratch/0x4568c0/ but the
// timebox fired before any were scored. Best remains 86.5% here.
// Remaining difference at 86.5%: the k4 loop body only (missing ebx counter
// and inc ebx, folded `k4 != 10` into `cmp esi,0x29f8`, to in ebx and from in
// edi instead of edi/edx, missing else-branch active re-test), plus two lea
// order swaps in the cand/k2 init regions.
// deepseek-v4.1 (this pass): reordering the tail so the res==0 send loop is
// the fall-through path (matching the original's `test eax,eax / jne`) lifts
// the file from 86.0% to 86.5%. The k4 loop's inline-PlayerId shape itself
// (FUN_00451bc0(FindFrom_004568c0(), PlayerId_004568c0(k4), packet, 2), with
// FindFrom_004568c0 a from-scan inline helper) reproduces the original's three
// induction variables (ebx counter, ebp player offset, esi field offset) and
// the 1310-byte size, but it makes MSVC home the FUN_00456030 result in ebp
// instead of edi, which recolours the k2 branch and the k3 loop (83.4%).
// Variants tried this pass (all lower than 86.5): helper-in-args + old tail
// 83.4, named `to` helper + named from loop 81.9, named `to` + FindFrom in the
// args 83.4, from-loop before a helper `to` in the args 79.3, manual `to` plus
// FindFrom in the args 78.0, and the three-induction k4 loop with the new tail
// 83.4. The remaining work is making the k4 loop keep a byte counter in ebx
// while `res` stays in edi.
#include <stdlib.h>
#include <algorithm>

#pragma pack(push, 1)
struct PlayerInfo_004568c0 {
    char unknown_0[0x97];
    unsigned short flag_97_0 : 1;
    unsigned short rest_97 : 15;
    char unknown_99[0x9b - 0x99];
    unsigned short pad_9b_a : 6;
    unsigned short flag_9b_6 : 1;
    unsigned short pad_9b_b : 7;
    unsigned short flag_9b_14 : 1;
};

class Class_00456030 {
  public:
    int field_0;
    char unknown_4[0x73 - 0x4];
    char field_73;
    int FUN_00456030();
};

class Player_004568c0 {
  public:
    int active;
    int id;
    char unknown_8[0x27 - 0x8];
    PlayerInfo_004568c0* info;
    char unknown_2b[0x73 - 0x2b];
    unsigned char state;
    char unknown_74[0x146 - 0x74];
    unsigned char field_146;
    unsigned char field_147;
    char unknown_148[0x14b - 0x148];
};

struct Game_004568c0 {
    char unknown_0[0x1b63];
    Player_004568c0 players[10];
    char unknown_2851[0x29a4 - 0x2851];
    int field_29a4[10];
    char unknown_29cc[0x29d0 - 0x29cc];
    int field_29d0[10];
    char unknown_29f8[0x29fc - 0x29f8];
    int field_29fc[10];
    char unknown_2a24[0x2a28 - 0x2a24];
    int field_2a28;
    char unknown_2a2c[0x2a42 - 0x2a2c];
    unsigned char localPlayer;
};
#pragma pack(pop)

extern Game_004568c0* g_game;

int __stdcall FUN_00451bc0(int from, int to, void* packet, int size);
int __stdcall FUN_00451df0(int id, void* packet, int size);

static inline unsigned char FindOccupied_004568c0() {
    for (unsigned char i = 0; i < 10; i++) {
        if (g_game->players[i].state != 0 && g_game->players[i].info->flag_97_0)
            return i;
    }
    return 10;
}

static inline int PlayerId_004568c0(unsigned char pi) {
    if (pi != 10 && g_game->players[pi].state != 0)
        return g_game->players[pi].id;
    return -1;
}

// FUNCTION: 0x4568c0
int FUN_004568c0() {
    unsigned char idx = FindOccupied_004568c0();
    int res = ((Class_00456030*)&g_game->players[idx])->FUN_00456030();
    int* out;
    if (res != 0 && g_game->field_2a28 == 0) {
        out = g_game->field_29fc;
        if (g_game->players[g_game->localPlayer].info->flag_9b_14) {
            int n = 0;
            for (int k0 = 0; k0 < 10; k0++) {
                Player_004568c0* q = &g_game->players[k0];
                if (q->active != 0 && (q->state == 1 || q->state == 2 || q->state == 3) &&
                    q->field_146 != 10 && (q->info->flag_9b_6) == 0)
                    out[k0] = n++;
                else
                    out[k0] = -1;
            }
        } else {
            int cand[10];
            for (int z = 0; z < 10; z++)
                cand[z] = -1;
            int n = 0;
            unsigned char* q = &g_game->players[0].state;
            int cnt = 10;
            do {
                Player_004568c0* p = (Player_004568c0*)(q - 0x73);
                if (p->active != 0 && (p->state == 1 || p->state == 2 || p->state == 3) &&
                    p->field_146 != 10 && (p->info->flag_9b_6) == 0) {
                    cand[n] = n;
                    n++;
                }
                q += 0x14b;
            } while (--cnt);
            if (n > 2 || (int)((__int64)rand() * 2 / 0x8000) != 0)
                std::random_shuffle(cand, cand + n);
            int* cp = cand;
            for (int k2 = 0; k2 < 10; k2++) {
                Player_004568c0* q2 = &g_game->players[k2];
                if (q2->active != 0 && (q2->state == 1 || q2->state == 2 || q2->state == 3) &&
                    q2->field_146 != 10) {
                    if (g_game->players[k2].active != 0 && (q2->info->flag_9b_6))
                        out[k2] = -1;
                    else
                        out[k2] = *cp++;
                } else {
                    out[k2] = -1;
                }
            }
        }
        g_game->field_2a28 = 1;
    }
    out = (int*)1;
    for (int k3 = 0; k3 < 10; k3++) {
        Player_004568c0* q = &g_game->players[k3];
        if (q->active != 0 && q->state == 3) {
            if (res != 0) {
                if (g_game->field_29a4[k3] == 0 || g_game->field_29d0[k3] == 0) {
                    out = (int*)0;
                    break;
                }
            }
            if (res == 0) {
                if (g_game->field_29a4[k3] == 0) {
                    out = (int*)0;
                    break;
                }
            }
        }
    }
    if (res != 0) {
        for (int k4 = 0; k4 < 10; k4++) {
            if (g_game->field_29d0[k4] == 0) {
                unsigned char packet[2];
                packet[0] = 0x1e;
                packet[1] = (unsigned char)g_game->field_29fc[k4];
                if (g_game->players[k4].active != 0) {
                    if (g_game->players[k4].state == 3) {
                        int to = -1;
                if (k4 != 10 && g_game->players[k4].state != 0)
                    to = g_game->players[k4].id;
                        int from = -1;
                        for (int j = 0; j < 10; j++) {
                            if (g_game->players[j].state == 1) {
                                from = g_game->players[j].id;
                                break;
                            }
                        }
                        FUN_00451bc0(from, to, packet, 2);
                    } else if (g_game->players[k4].active != 0 &&
                               (g_game->players[k4].state == 1 || g_game->players[k4].state == 2)) {
                        g_game->players[k4].field_147 = packet[1];
                        g_game->field_29d0[k4] = 1;
                    }
                }
            }
        }
    }
    unsigned char pkt = 0x15;
    if (res == 0) {
        for (int k6 = 0; k6 < 10; k6++) {
            Player_004568c0* q = &g_game->players[k6];
            if (q->active != 0 && (q->state == 1 || q->state == 2))
                FUN_00451df0(PlayerId_004568c0(k6), &pkt, 1);
        }
        return (int)out;
    }
    if (out != (int*)0) {
        for (int k5 = 0; k5 < 10; k5++) {
            Player_004568c0* q = &g_game->players[k5];
            if (q->active != 0 && (q->state == 1 || q->state == 2))
                FUN_00451df0(PlayerId_004568c0(k5), &pkt, 1);
        }
    }
    return (int)out;
}
