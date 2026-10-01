// Decompiled by deepseek-v4.1-flash, finished by GPT-6.1-sol, finished by
// deepseek-v4.1-flash, finished by deepseek-v4.1, finished by deepseek-v4.1-flash,
// finished by deepseek-v4.1-flash, finished by deepseek-v4.1-flash, finished by
// deepseek-v4.1-flash.
// 2026-10-01 pass 5 (deepseek-v4.1-flash, 10 min timebox, 0 scored variants kept,
// 3 scratch probes): why the case-3 lanes cannot be made 32-bit by spelling alone.
// - Dropping the `(unsigned short)` cast on a case-2 lane is byte-identical: the
//   narrowing is not driven by the assignment's type.
// - vB: with a per-lane `unsigned int m = *(unsigned char*)(p2 + 0x9c) & M;` in the
//   three case-3 post-loop lanes the AND does become 32-bit (`and ecx,2`, no movzx),
//   but the byte temp moves to ecx and the word to esi (original: bl and dx):
//   80.4% (2833 bytes).
// - vC: splitting the conversion from the AND (`unsigned int m = *(unsigned
//   char*)(p2 + 0x9c);` then `... | (m & M)`) DOES reproduce the original's
//   zero-extension idiom (`xor eax,eax; mov al,[..]`) plus the 32-bit `and eax,2`,
//   but MSVC then rewrites that lane as the xor combine
//   (`xor al,dl; and eax,2; xor eax,edx`) and puts the word in esi: 79.7% (2827).
//   So the original's AND/OR lane form only survives while the byte operand stays
//   narrowed; any 32-bit wide byte operand flips instruction selection to the xor
//   combine (vC) or rotates the register file (vB). The register file (edi=1,
//   bl byte temps, dx word) and the lane width are one allocator/IS state.
// 2026-10-01 pass 4 (deepseek-v4.1-flash, 10 min timebox, 0 scored variants): re-read the
// switch dispatch and the lane diff with the /Fa-style instruction view, no change kept.
// Confirmed by direct instruction comparison of the compiled lanes that the case-1/2
// lanes are the same length in instructions but 4 bytes wider each: ours is
// `and bl,1; movzx si,bl; shl esi,2` against the original's `and ebx,edi; shl ebx,2`,
// i.e. the byte temp's conversion is narrowed to 8 bits here and zero-extended (or
// register-masked) there. Also observed the original spends a redundant `xor ebx,ebx`
// before `mov bl,[..]` in the three case-3 lanes even though `and ebx,imm` with imm in
// {1,2,4} clears the top 24 bits anyway, so those three lanes are unambiguously
// `unsigned int m = *(unsigned char*)(p2 + 0x9c) & M;` temps, while case 1/2 must be
// `& one` in a register: the two halves of the lane region need different spellings of
// the same value, and combining them has always rotated the callee-saved file (73-81%).
// No new lever; the file is left at the 82.8% baseline.
// 2026-10-01 pass 3 (deepseek-v4.1-flash): re-confirmed 82.8 (2846 vs 2797). The whole 49-byte overage
// sits before 0x497b30; the head hunks are `mov edi,1` (ours `mov edx,1`) for the `int one = 1;` local
// plus `mov dx,[eax+0x37eec]` (ours `cx`) and the byte-wise `and bl,imm; movzx si,bl` lane masks where
// the original masks 32-bit in EBX against the EDI-held 1. All of these are the same allocator state
// already documented below; no new lever found.
// 2026-10-01 pass 2 (deepseek-v4.1-flash, 10 min timebox, 1 scored variant): re-confirmed
// 82.8 (2846 vs 2797). Tried one untried lane respelling: hoisting a per-lane
// `unsigned short v = *(unsigned short*)(g_game + 0x14281);` temp in the three case-3
// post-loop lanes so the g_game load precedes the 0x9c byte load (the original's order
// at 0x4974xx is `mov ecx,[g_game]` then `mov bl,[eax+0x9c]`, ours is the reverse).
// Result: byte-identical, 2846 bytes / 82.8, so that order is not steered by naming the
// read. No change kept. Remaining residue unchanged: the CSE'd constant 1 lands in EDX
// here and EDI in the original, and the nine 0x14281 lanes keep the movzx form.
// 2026-10-01 pass (deepseek-v4.1-flash): re-confirmed 82.8 (2846 vs 2797 bytes). Remaining
// diffs are unchanged from the notes below: the CSE'd constant 1 lands in EDX here and EDI
// in the original, and the nine 0x14281 lanes keep the movzx form.
// deepseek-v4.1-flash, edited by deepseek-v4.1, finished by deepseek-v4.1-flash,
// finished by deepseek-v4.1-flash.
// Names are provisional.
//
// 2797 bytes. Best so far: 82.8% (2846 vs 2797 bytes). No MATCH.
//
// This pass (deepseek-v4.1-flash, ~15 min, 2 scored runs): re-read the
// original lane code with fresh eyes, no gain, one new negative and one
// structural insight worth keeping.
// - Negative: a single `unsigned int mb;` reused by the three case-3
//   post-loop lanes (`mb = *(uc*)(p2 + 0x9c) & M;` then `... | mb`) scores
//   80.4% (2833 bytes). Same failure mode as the per-lane uint temps: the
//   dword AND is right, the register file rotates.
// - Insight for the next attempt: in the original, edi holds the CSE'd 1 and
//   is used as the mask ONLY in the case-1 and case-2 lanes (`and ebx,edi`,
//   `and edx,edi`, `and eax,edi` at 0x497403/0x497429/0x497444 and
//   0x49748a/0x4974b0/0x4974cb). The case-3 post-loop lanes
//   (0x497348/0x49736f/0x497396) and the later pl lanes (0x4975xx) use
//   immediates instead, because on those paths edi/edx are already holding
//   the record pointer or g_game. So the source almost certainly references
//   ONE variable everywhere; the immediate-vs-register split is MSVC
//   rematerialising where the register is busy. Our build instead folds every
//   lane to `and r8, imm; movzx`, i.e. the constant never reaches the lanes
//   as a value. That is a value-numbering/instruction-selection state of the
//   whole function, not a per-lane spelling, which is why no lane respelling
//   has ever moved it.
//
// - Two /Fa probes run this pass (build/scratch/0x497180/probe*.cpp) pin the
//   lane shape precisely: `int m = *(unsigned char*)(p + 0x9c) & 2;` followed
//   by `w = (w & 0xfffd) | m;` is the ONLY spelling that emits the original's
//   `mov al, byte [p+0x9c]; and eax, 2; or ecx, eax`. Writing the temp as a
//   byte load and masking on a later line (g1/g2/g3 in the probe) instead
//   makes MSVC emit the `xor al, cl; and eax, 2; xor cx, ax` combine, and the
//   whole-expression `(unsigned short)(...)` cast (our current form) makes it
//   narrow to `and bl, 2; movzx si, bl`. So the shape in the file is known
//   good; only the register file (ebx vs eax, dx vs cx, edi vs edx) differs.
// - A 32-bit word temp (`int w3 = *(unsigned short*)(g_game+0x14281);` then
//   `w3 = (w3 & mask) | ...;` and a truncating store) does produce the 32-bit
//   `and edx, 2` lanes and is 12 bytes shorter (2834), but scores 81.9%.
//
// This pass (deepseek-v4.1-flash, ~8 min, 0 scored check runs, all --sym):
// re-confirmed the plateaus below, no gain. Three free probes, all flat at 82.8%
// (2846 bytes) unless noted:
// - Referencing `one` in the first post-call pl lane (`& one` instead of `& 1`)
//   does not move the CSE'd 1 into edi; byte-identical to the plain immediate.
// - Hoisting `char* recbase = g_game + 0x1b63;` before the for-off spawn loop
//   (rec = recbase + off) regresses to 80.0% (2853 bytes): it strength-reduces
//   the pointer walk. Keep `rec = g_game + 0x1b63 + off` inline.
// - Flipping the OR operand order in all six 0x14281 case-3/pl lanes
//   (`m | (w & ~M)` vs `(w & ~M) | m`) is byte-identical: not a scheduling lever.
// headers.py sweeps 128 sets and every one is 82.8% with <windows.h> on top, so
// the lone SIB swap ([ecx+ebx+0x1b63] vs [ebx+ecx+0x1b63]) is compiler state.
// The remaining gaps are unchanged and all allocator-driven (see the pass notes
// below): the nine 0x14281 lanes want 32-bit zero-extend + `and ebx,imm` while
// ours narrow to `and bl,imm; movzx si,bl`, and the switch materialises the
// CSE'd 1 in edx where the original parks it in the callee-saved edi (which is
// also why case 3's copies are `mov cx` here vs `mov dx` there).
//
// This pass (deepseek-v4.1-flash, ~20 min): re-scored the baseline and the
// prior lane experiments in build/scratch/0x497180/. Two small wins, both from
// build/scratch/0x497180/vQ.cpp:
// - A shared `int one = 1;` before the switch (used in the case-1/2 mask lanes
//   and the two `DAT_005091cc = one;` case-2/3 stores) changed the xor-lane
//   allocation in cases 1/2 from edx/esi to ecx/edx and was worth 82.4 -> 82.7.
// - The mission-count loop rewritten as `int i = 0; while (i < 10) { if (...)
//   count = i + 1; i++; def += 6; }` puts `i++` before the `def += 6` pointer
//   add, matching the original's `lea esi,[eax+1]; inc eax; add edx,0x18`, for
//   82.7 -> 82.8. A plain `for` gives the pointer add first.
// - Confirmed by isolated /Fa probes that the winner lane shape is
//   `unsigned int m = *(unsigned char*)(p+off) & M; w = (w & ~M) | m;`
//   (`and ebx,2` 32-bit, no movzx). Applying it to any subset of the lanes
//   (p2 only 80.5, pl only 81.0, p2+pl 78.7, all 12 75.0) still loses to the
//   current 16-bit form, so the remaining lane gap is one allocation state,
//   not a source-shape problem. Do not re-run those.
//
// This pass (deepseek-v4.1, ~12 min, 8 check runs, 80.8 -> 82.4):
// - The 0x38d75 network flags ARE the volatile field the guide names: writing
//   `*(volatile unsigned short*)(g_game + 0x38d75) |= 4/2` reproduces the
//   original's `mov dx,[g+0x38d75]; or edx,4; mov [g+0x38d75],dx` exactly
//   (80.8 -> 81.7, then 82.0 with both sites).
// - The bit-6 test at +0x9b is a real 1-bit bitfield: `struct { unsigned short
//   : 6; unsigned short b6 : 1; ... }` over lp+0x9b gives the original's
//   `mov al,[lp+0x9b]; shr al,6; test al,1` (82.0 -> 82.3); the plain
//   `unsigned char v = ..; v >>= 6; if (v & 1)` folds to `test byte ptr,0x40`.
// - The mission-count clamp reads better with the count on the left:
//   `if (count > cur)` gives the original's `cmp esi,eax; jle` (82.3 -> 82.4).
// - Tried and reverted: `unsigned int` byte temps in the case-3 and post-loop
//   lanes (64.6, they rotate the whole register file, not just the lanes; the
//   old note had 74.4 for all six lanes), and `int one = 1;` used for the
//   DAT_005091cc stores (byte-identical to the plain constant: MSVC propagates
//   the constant, so it cannot force a register-held 1).
//
// This pass (deepseek-v4.1, second 12 min, 8 check runs): confirmed the
// 32-bit byte temp is what the original lanes use (it is the only spelling
// that makes MSVC emit the `xor r,r; mov r8,mem` zero-extension idiom at
// 0x497303/0x497353/0x49737a), but MSVC then rewrites `(w & ~2) | (b & 2)` as
// `xor b,w; and b,2; xor b,w` and moves g_game to esi and the byte to
// eax/ecx/edx: 73.3% with all nine lanes, 80.2% with case 3 only and 76.0%
// with just the two shift lanes (case 1/2), all worse than 82.4%. Read it as
// a live-range effect: here the const 1 in edx dies before the case-3 calls,
// so edx is enough; the original's `and reg,edi` inside the post-call lanes
// keeps it live across them and that is what forces edi.
//
// This pass (deepseek-v4.1, 12 min, 4 check runs): two fresh angles, both worse.
// - Case 1/2 lanes with `unsigned int` byte temps (b0/b1/b2 locals, 6 lanes):
//   77.9%, it still emits `and bl,imm; movzx si,bl` and rotates the file.
// - Case-3-tail lanes with `unsigned int m = *(uc*)(p2+0x9c) & 2;` temps:
//   79.8% (2827 bytes vs the 2846 baseline), closer in size but wrong regs.
// The const 1 cannot be kept live across a call by source means: `one` is a
// known constant, so every use folds to an immediate, and the register it is
// cached in (edx here, edi in the original) is a whole-function allocator
// decision. Fixing the case-1/2 lanes therefore needs the other 49 bytes of
// code size corrected first, not a local respelling.
//
// Still open, in order of how much they cost on the diff:
// - The nine 0x14281 lane updates: the original zero-extends the byte into a
//   32-bit register (`xor ebx,ebx; mov bl,[..]`), masks 32-bit (`and ebx,2`,
//   `and ebx,edi` for the mask 1 lanes) and ORs `or edx,ebx`; ours narrows to
//   `and bl,2; movzx si,bl`. Every 32-bit-temp spelling tried rotates the
//   register file globally and scores far lower, so the allocator state at the
//   switch has to be reproduced first.
// - `mov edi,1` at the switch (ours `mov edx,1`), which also makes case 3's
//   16-bit copies `mov dx,[..]` instead of `mov cx,[..]`. The constant is
//   CSE'd into one register in both; which one is an allocator choice.
// - One `mov eax,[ecx+ebx+0x1b63]` / `lea esi,[ecx+ebx+0x1b63]` where ours
//   encodes the base and index the other way round ([ebx+ecx+..]).
//
// This pass (deepseek-v4.1, 12 min, 3 check runs): no gain, two negatives.
// - The three post-call pl lanes with a shared 32-bit `unsigned int pb`
//   temp (xor edx,edx / mov dl,[..] / and edx,imm) still rotate the
//   function file: 75.2% at 2831 bytes. Do not retry that site either.
// - `v ^ ((v ^ b) & one)` compiles byte-identically to `((v ^ b) & one) ^ v`
//   (82.8%, 2846 bytes), so the xor-lane operand order is not the lever;
//   MSVC folds both to the (v & 0xfffe) ^ (b & 1) form because the mask is
//   the immediate 1 here, while the original masks with the edi register.
//
// Earlier passes (still in this file) fixed the Fixed union, the __stdcall
// declarations, the int sel/sel2 locals and the initial `==3` guard.
//
// What the pass before fixed, 78.6 -> 80.8:
// - The FUN_0041c4c0 call after the 0x9b bit-6 test was duplicated in both
//   branches here; the original computes the two ints in each arm and has ONE
//   shared call (`jmp` into a common `push 0; push eax; push esi; call`).
//   Rewritten as two ints set in the if/else plus one call.
// - The three ten-player walks: indexing a record as
//   `g_game + 0x1b63 + 0x14b * (unsigned char)i` instead of `* i` stops MSVC
//   strength-reducing the multiply into a pointer walk, reproducing the
//   original's `mov eax,ebx; and eax,0xff; ...; lea eax,[edx+ecx*2+..]` with a
//   live index (`inc ebx`). 69.8 -> 78.3, the single biggest win.
// - `std::random_shuffle(order, order + n)` from <algorithm> replaces the
//   hand-rolled shuffle loop; the header's _Rm/_Rn scaling loop compiles
//   byte-exactly. 66.8 -> 68.1.
// - The mission block after FUN_004816a0 is nested the original's way,
//   `if (mission != 0) { summary; if (BetweenMissions()==0) { FUN_00432610;
//   goto tail; } } else if (state != 1) goto tail;` then the shared
//   FUN_00488310/FUN_0041d1f0. 68.1 -> 69.7.
// - `rec+0x149` as a 1-bit `unsigned short` bitfield gives the original's
//   direct `or byte ptr [rec+0x149],1`; a plain `unsigned char |=` goes
//   through a register.
// - The final player-record access goes through a record local (`currec`) so
//   the pointer chain is `lea ..+0x1b63; mov eax,[rec+0x27];
//   or byte ptr [eax+0x9b],0x10`, as in the original.
// - `pos.x.i` before `pos.y.i = 0` (the original's order at 0x4976cd).
// This pass (deepseek-v4.1-flash, ~15 min, all --sym scratch scores): no gain
// over the 82.8% baseline. Confirmed the lane region is the sole real diff: the
// entire tail after the switch matches instruction-for-instruction except for
// branch targets shifted by the lanes' 49 extra bytes. Tried, all worse:
// - 32-bit byte temps (`t = *(unsigned char*)p; t &= M;`, and with `t = 0;`)
//   per lane site: post-loop only 81.0, case-3 only 80.4, case-1/2 only 81.2,
//   all twelve 77.9. The dword `and` shape is right but the register file
//   rotates and MSVC never emits the original's `xor ebx,ebx; mov bl,[..]`
//   zero-extension, so bytes shrink below 2797 and every branch target moves.
// - case-3 lanes with the mask inline and `fb = 0; fb = byte;`: 79.7; MSVC
//   folds it to the `xor al,cl; and eax,2; xor eax,ecx` combine, not the
//   original's separate `and ebx,2; and edx,0xfffd; or edx,ebx`.
// - `bool`/`short`/`char`/`unsigned int` for `one`: all byte-identical, 82.8.
// - Compiling the real preceding function (FUN_00497080, already matched in
//   src/unsorted/0x497080.cpp) above ours in this file: 82.3, allocation
//   unchanged (still `mov edx,1`), so the edi/edx choice is not file layout.
// The lane region and `mov edi,1` are one allocator state; no source respelling
// of the lanes alone moves it.
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <algorithm>
#include <time.h>

union Fixed_497180 {
    int i;                              // 16.16
    struct {
        short frac;
        short whole;
    } h;
};

struct FixedPos_497180 {
    Fixed_497180 x;
    Fixed_497180 y;
    Fixed_497180 z;
};

struct RecFlag_497180 {
    unsigned short started : 1;
    unsigned short : 15;
};

struct PlFlags_497180 {
    unsigned short : 6;
    unsigned short b6 : 1;
    unsigned short : 9;
};

struct Sub_497180 {
    char unknown_0[0x10];
};

struct Gadget_497180 {
    char unknown_0[0x8];
    void (__stdcall* handler)(Gadget_497180*);   // +0x8
    char* owner;                                 // +0xc
};

class Class_00435100 {
public:
    int FUN_00435100();
};

class Class_00435a20 {
public:
    void FUN_00435a20(void* player);
};

class Class_00437320 {
public:
    int FUN_00437320(FixedPos_497180* pos, int id);
};

class Class_004618a0 {
public:
    void FUN_004618a0(int a);
};

class Class_004b4560 {
public:
    void FUN_004b4560(const char* name);
};

class Class_004b48f0 {
public:
    int FUN_004b48f0(const char* name);
};

class Class_004b3630 {
public:
    void FUN_004b3630();
};

extern char* g_game;
extern int DAT_005091cc;
extern int DAT_00506dbc;
extern Class_004618a0 DAT_00513000;

void __stdcall FUN_004b6ca0(int x);
void __stdcall FUN_004b6b50(int x);
int __stdcall FUN_004b6c30(int x);
unsigned char __stdcall FUN_00456850();
void FUN_00431740();
void FUN_00453d40();
void __stdcall FUN_00465fb0(void* mission);
void FUN_0047a760();
void FUN_004917d0();
void FUN_00465e30();
void __stdcall FUN_004816a0(int x);
void __stdcall FUN_00432610(void* mission);
void FUN_00488310();
void FUN_0041d1f0();
void __stdcall FUN_004288d0(int a, int b, int c, int d);
void FUN_00450f90();
void FUN_00451180();
void FUN_00464f80();
void __stdcall FUN_0046c620(int x);
void FUN_004649d0();
void __stdcall FUN_0041c4c0(int x, int y, int z);
unsigned short __stdcall FUN_00488b10(const char* name);
void __stdcall FUN_00496ee0(int team, int startpos);
void __stdcall FUN_00485f50(unsigned char team, unsigned short id, FixedPos_497180 pos, int a,
    int b, int c);
Gadget_497180* __stdcall FUN_004aa8f0(Sub_497180* sub, const char* name, int flags);
void __stdcall FUN_00494890(Gadget_497180* gadget);
void __cdecl operator delete(void* p);

// FUNCTION: 0x497180
void __cdecl FUN_00497180(void)
{
    LARGE_INTEGER perfCount;
    FixedPos_497180 pos;
    FixedPos_497180 start;
    int order[10];

    QueryPerformanceCounter(&perfCount);
    FUN_004b6ca0(perfCount.LowPart + perfCount.HighPart);
    srand((unsigned)time(NULL));
    *(int*)(g_game + 0x38a47) = 0;

    int one = 1;
    switch (((Class_00435100*)*(void**)(g_game + 0x391e9))->FUN_00435100()) {
    case 1: {
        DAT_005091cc = 0;
        char* base = g_game + 0x39219;
        *(int*)(g_game + 0x37ef6) = *(int*)base;
        *(unsigned short*)(g_game + 0x14281) =
            (unsigned short)((*(unsigned short*)(g_game + 0x14281) & 0xfffb) |
                ((*(unsigned char*)(base + 0xc) & one) << 2));
        unsigned short v = *(unsigned short*)(g_game + 0x14281);
        unsigned char b = *(unsigned char*)(base + 4);
        *(unsigned short*)(g_game + 0x14281) = (unsigned short)(((v ^ b) & one) ^ v);
        *(unsigned short*)(g_game + 0x14281) =
            (unsigned short)((*(unsigned short*)(g_game + 0x14281) & 0xfffd) |
                ((*(unsigned char*)(base + 8) & one) << 1));
        FUN_00431740();
        break;
    }
    case 2: {
        char* base = (char*)*(void**)(g_game + 0x29a0) + 0x108;
        *(unsigned short*)(g_game + 0x37ee6) = *(unsigned short*)(g_game + 0x37eec);
        DAT_005091cc = one;
        *(int*)(g_game + 0x37ef6) = *(int*)base;
        *(unsigned short*)(g_game + 0x14281) =
            (unsigned short)((*(unsigned short*)(g_game + 0x14281) & 0xfffb) |
                ((*(unsigned char*)(base + 0xc) & one) << 2));
        unsigned short v = *(unsigned short*)(g_game + 0x14281);
        unsigned char b = *(unsigned char*)(base + 4);
        *(unsigned short*)(g_game + 0x14281) = (unsigned short)(((v ^ b) & one) ^ v);
        *(unsigned short*)(g_game + 0x14281) =
            (unsigned short)((*(unsigned short*)(g_game + 0x14281) & 0xfffd) |
                ((*(unsigned char*)(base + 8) & one) << 1));
        break;
    }
    case 3: {
        *(unsigned short*)(g_game + 0x37ee6) = *(unsigned short*)(g_game + 0x37eec);
        DAT_005091cc = one;
        *(unsigned short*)(g_game + 0x38a51) &= 0xfffe;

        int sel = FUN_00456850();
        unsigned char cur = *(unsigned char*)(g_game + 0x2a42);
        if (*(unsigned char*)(g_game + 0x1b63 + 0x14b * cur + 0x21) & 2) {
            do {
                char* p = *(char**)(g_game + 0x1b63 + 0x14b * *(unsigned char*)(g_game + 0x2a42) + 0x27);
                if (DAT_00506dbc)
                    DAT_00513000.FUN_004618a0(1);
                FUN_00453d40();
                sel = FUN_00456850();
                FUN_004b6b50(0x32);
                if (sel == 10)
                    continue;
                if (*(unsigned char*)(p + 0x96) == 0xff)
                    continue;
                if (*(char*)(p + 0x8f) == 0)
                    continue;
                break;
            } while (1);
            FUN_004b6b50(0x32);
        }

        ((Class_00435a20*)*(void**)(g_game + 0x391e9))
            ->FUN_00435a20(*(void**)(g_game + 0x1b63 + 0x14b * sel + 0x27));
        if (FUN_00456850() == 10)
            break;

        int sel2 = FUN_00456850();
        char* p2 = *(char**)(g_game + 0x1b63 + 0x14b * sel2 + 0x27);
        DAT_005091cc = (*(unsigned short*)(p2 + 0x9b) >> 0xd) & 1;
        *(int*)(g_game + 0x37ef6) = (*(unsigned short*)(p2 + 0x9b) >> 0xb) & 3;
        *(unsigned short*)(g_game + 0x14281) =
            (unsigned short)((*(unsigned short*)(g_game + 0x14281) & 0xfffd) |
                (*(unsigned char*)(p2 + 0x9c) & 2));
        *(unsigned short*)(g_game + 0x14281) =
            (unsigned short)((*(unsigned short*)(g_game + 0x14281) & 0xfffb) |
                (*(unsigned char*)(p2 + 0x9c) & 4));
        *(unsigned short*)(g_game + 0x14281) =
            (unsigned short)((*(unsigned short*)(g_game + 0x14281) & 0xfffe) |
                (*(unsigned char*)(p2 + 0x9c) & 1));
        *(unsigned short*)(g_game + 0x37ee6) = *(unsigned short*)(p2 + 0xa5);
        break;
    }
    default:
        break;
    }

    if (*(void**)(g_game + 0x38d6b) != 0) {
        ((Class_004b4560*)*(void**)(g_game + 0x38d6b))->FUN_004b4560("summary");
        if (((Class_004b48f0*)*(void**)(g_game + 0x38d6b))->FUN_004b48f0("BetweenMissions") ==
            0) {
            FUN_00465fb0(*(void**)(g_game + 0x38d6b));
            if (((Class_00435100*)*(void**)(g_game + 0x391e9))->FUN_00435100() == 2) {
                int count = 0;
                int* def = (int*)*(void**)(g_game + 0x29a0);
                int i = 0;
                while (i < 10) {
                    if (*def == 1 || *def == 2)
                        count = i + 1;
                    i++;
                    def += 6;
                }
                int cur = *(int*)(g_game + 0x38d81);
                if (count > cur)
                    cur = count;
                *(int*)(g_game + 0x38d81) = cur;
                FUN_0047a760();
            }
        }
    }

    FUN_004917d0();

    if (((Class_00435100*)*(void**)(g_game + 0x391e9))->FUN_00435100() != 1) {
        if (((Class_00435100*)*(void**)(g_game + 0x391e9))->FUN_00435100() == 3) {
            *(volatile unsigned short*)(g_game + 0x38d75) |= 4;
            while ((*(unsigned short*)(g_game + 0x38d75) & 8) == 0)
                FUN_004b6b50(0x32);

            int sel = FUN_00456850();
            char* pl = *(char**)(g_game + 0x1b63 + 0x14b * sel + 0x27);
            *(unsigned short*)(g_game + 0x14281) =
                (unsigned short)((*(unsigned short*)(g_game + 0x14281) & 0xfffe) |
                    (*(unsigned char*)(pl + 0x9c) & 1));
            *(unsigned short*)(g_game + 0x14281) =
                (unsigned short)((*(unsigned short*)(g_game + 0x14281) & 0xfffd) |
                    (*(unsigned char*)(pl + 0x9c) & 2));
            *(unsigned short*)(g_game + 0x14281) =
                (unsigned short)((*(unsigned short*)(g_game + 0x14281) & 0xfffb) |
                    (*(unsigned char*)(pl + 0x9c) & 4));
            *(int*)(g_game + 0x37ef6) = (*(unsigned short*)(pl + 0x9b) >> 0xb) & 3;

            for (int off = 0; off < 0xcee; off += 0x14b) {
                char* rec = g_game + 0x1b63 + off;
                if (*(int*)rec == 0)
                    continue;
                unsigned char st = *(unsigned char*)(rec + 0x73);
                if (st != 1 && st != 2)
                    continue;
                pos.x.i = (FUN_004b6c30(*(int*)(g_game + 0x14223) - 0xa0) + 0x50) << 16;
                pos.y.i = 0;
                pos.z.i = (FUN_004b6c30(*(int*)(g_game + 0x14227) - 0xa0) + 0x50) << 16;
                if (*(int*)rec != 0 &&
                    (*(unsigned char*)(*(char**)(rec + 0x27) + 0x9b) & 0x40))
                    continue;
                char* pl2 = *(char**)(rec + 0x27);
                int side = *(unsigned char*)(pl2 + 0x95);
                int which = *(unsigned char*)(rec + 0x147);
                ((Class_00437320*)*(void**)(g_game + 0x391e9))
                    ->FUN_00437320(&pos, which);
                if (*(int*)rec != 0 && *(unsigned char*)(rec + 0x73) == 1)
                    start = pos;
                unsigned short id =
                    FUN_00488b10(g_game + 0x37f5f + 0x232 * side);
                FUN_00485f50(*(unsigned char*)(rec + 0x146), id, pos, 1, 1, 0);
                int s1 = *(unsigned short*)(pl + 0xa1) * 100;
                int s2 = *(unsigned short*)(pl + 0xa3) * 100;
                ((RecFlag_497180*)(rec + 0x149))->started = 1;
                *(float*)(rec + 0xdc) = (float)(s1 >= 200 ? s1 : 200);
                *(float*)(rec + 0xe0) = (float)(s2 >= 200 ? s2 : 200);
            }

            unsigned char li = *(unsigned char*)(g_game + 0x2a42);
            char* lp = *(char**)(g_game + 0x1b63 + 0x14b * li + 0x27);
            int cx;
            int cz;
            if (((PlFlags_497180*)(lp + 0x9b))->b6) {
                *(unsigned short*)(g_game + 0x14281) &= 0xfffe;
                *(unsigned short*)(g_game + 0x14281) &= 0xfffd;
                cx = *(int*)(g_game + 0x37e37) / 2;
                cz = *(int*)(g_game + 0x37e3b) / 2;
            } else {
                cx = start.x.h.whole - *(int*)(g_game + 0x37e37) / 2;
                cz = start.z.h.whole - *(int*)(g_game + 0x37e3b) / 2;
            }
            FUN_0041c4c0(cx, cz, 0);
            FUN_0046c620(6);
        } else if (((Class_00435100*)*(void**)(g_game + 0x391e9))->FUN_00435100() == 2 &&
            *(void**)(g_game + 0x38d6b) == 0) {
            if (*(int*)((char*)*(void**)(g_game + 0x29a0) + 0x118) != 0) {
                for (int i1 = 0; i1 < 10; i1++) {
                    if ((unsigned char)i1 < 10) {
                        char* rec = g_game + 0x1b63 + 0x14b * (unsigned char)i1;
                        if (*(int*)rec != 0) {
                            unsigned char st = *(unsigned char*)(rec + 0x73);
                            if ((st == 1 || st == 2 || st == 3) &&
                                *(unsigned char*)(rec + 0x146) != 10)
                                FUN_00496ee0(i1, i1);
                        }
                    }
                }
            } else {
                for (int i2 = 0; i2 < 10; i2++)
                    order[i2] = -1;
                int n = 0;
                for (int i3 = 0; i3 < 10; i3++) {
                    if ((unsigned char)i3 < 10) {
                        char* rec = g_game + 0x1b63 + 0x14b * (unsigned char)i3;
                        if (*(int*)rec != 0) {
                            unsigned char st = *(unsigned char*)(rec + 0x73);
                            if ((st == 1 || st == 2 || st == 3) &&
                                *(unsigned char*)(rec + 0x146) != 10)
                                order[n++] = i3;
                        }
                    }
                }
                if (n > 2 || (int)(((__int64)rand() * 2) / 0x8000) != 0) {
                    std::random_shuffle(order, order + n);
                }
                int k = 0;
                for (int i4 = 0; i4 < 10; i4++) {
                    if ((unsigned char)i4 < 10) {
                        char* rec = g_game + 0x1b63 + 0x14b * (unsigned char)i4;
                        if (*(int*)rec != 0) {
                            unsigned char st = *(unsigned char*)(rec + 0x73);
                            if ((st == 1 || st == 2 || st == 3) &&
                                *(unsigned char*)(rec + 0x146) != 10)
                                FUN_00496ee0(i4, order[k++]);
                        }
                    }
                }
            }
            FUN_00465e30();
        }
    }

    FUN_004816a0(1);

    if (*(void**)(g_game + 0x38d6b) != 0) {
        ((Class_004b4560*)*(void**)(g_game + 0x38d6b))->FUN_004b4560("summary");
        if (((Class_004b48f0*)*(void**)(g_game + 0x38d6b))->FUN_004b48f0("BetweenMissions") ==
            0) {
            FUN_00432610(*(void**)(g_game + 0x38d6b));
            goto tail;
        }
    } else if (((Class_00435100*)*(void**)(g_game + 0x391e9))->FUN_00435100() != 1) {
        goto tail;
    }
    FUN_00488310();
    FUN_0041d1f0();

tail:
    {
        int pnum = *(unsigned char*)(g_game + 0x2a43);
        char* rec = g_game + 0x1b63 + 0x14b * pnum;
        FUN_004288d0(0, 0, 0, 0);
        char* pl = *(char**)(rec + 0x27);
        int side = *(unsigned char*)(pl + 0x95);
        sprintf(g_game + 0x37ea0, "%sMAIN2.GUI", g_game + 0x37f5b + 0x232 * side);
    }
    Gadget_497180* gadget =
        FUN_004aa8f0((Sub_497180*)(g_game + 0x519), g_game + 0x37ea0, 0x20);
    gadget->handler = FUN_00494890;
    gadget->owner = g_game;

    char* currec = g_game + 0x1b63 + 0x14b * *(unsigned char*)(g_game + 0x2a42);
    *(unsigned char*)(*(char**)(currec + 0x27) + 0x9b) |= 0x10;
    FUN_00450f90();
    FUN_00451180();
    FUN_00464f80();
    FUN_00465e30();

    void* mission = *(void**)(g_game + 0x38d6b);
    if (mission != 0) {
        ((Class_004b3630*)mission)->FUN_004b3630();
        operator delete(mission);
        *(void**)(g_game + 0x38d6b) = 0;
    }
    FUN_004649d0();

    *(volatile unsigned short*)(g_game + 0x38d75) |= 2;
}
