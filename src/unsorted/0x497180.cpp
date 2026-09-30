// Decompiled by deepseek-v4.1-flash, finished by GPT-6.1-sol, finished by
// deepseek-v4.1-flash, edited by deepseek-v4.1. Names are provisional.
//
// 2797 bytes. Best so far: 80.7% (2822 vs 2797 bytes). No MATCH.
//
// Earlier passes (still in this file) fixed the Fixed union, the __stdcall
// declarations, the int sel/sel2 locals and the initial `==3` guard.
//
// What the last pass fixed, 78.6 -> 80.7:
// - The FUN_0041c4c0 call after the 0x9b bit-6 test was duplicated in both
//   branches here; the original computes the two ints in each arm and has ONE
//   shared call (`jmp` into a common `push 0; push eax; push esi; call`).
//   Rewritten as two ints set in the if/else plus one call. Also made the
//   bit-6 read an `unsigned char` local shifted in its own statement.
// - Still open: the g_game[0x14281] read-modify-writes. The original zero
//   extends each byte (`xor edx,edx; mov dl,[p+0x9c]`), masks 32-bit
//   (`and edx,2/4/1`) and ORs into a word load (`mov cx,[g+0x14281];
//   and ecx,0xfffd; or ecx,edx`). An `unsigned int` temp does give the 32-bit
//   AND (`and ecx,2` and `or`), but MSVC then drops the 2-byte xor (it knows
//   the mask clears the high bits) and the function comes out 2770 bytes:
//   same instruction shapes, different registers, and the checker scores it
//   LOWER (75.0), so the narrow `and bl,2; movzx si,bl` form is kept. Shapes
//   measured with tools/wcl + /Fa in build/scratch/0x497180/{t,u,u2,u3}.asm:
//   `int`/`unsigned int` temp of the whole byte then `temp & mask` in the OR
//   gives `and reg,2` and no xor; `unsigned char` temp with an `unsigned int`
//   flags temp gives the xor but then `and dl,2; movzx dx,dl`.
// - Also still open: `test byte ptr [..+0x9b],0x40` here vs the original's
//   `mov al,[..]; shr al,6; test al,1` (all of a local, a shifted local, a
//   bitfield-free expression and three bitfield shapes fold to the test in
//   tools/wcl micro-tests, so the original may read a real bitfield there);
//   `or byte ptr [g+0x38d75],4/2` here vs the original's word load/or/store
//   (the volatile network-flags signature); edi vs edx for the reused
//   constant 1; and the extra `(((long long)rand() * 2) / 0x8000)` mul/div.
//
// What this pass fixed, in order of how much it moved the number:
// - The three ten-player walks: indexing a record as
//   `g_game + 0x1b63 + 0x14b * (unsigned char)i` instead of `* i` stops MSVC
//   strength-reducing the multiply into a pointer walk. The cast reproduces
//   the original's `mov eax,ebx; and eax,0xff; ...; lea eax,[edx+ecx*2+..]`
//   and keeps the counter as a live index (`inc ebx`), not a byte offset.
//   69.8 -> 78.3, the single biggest win.
// - `std::random_shuffle(order, order + n)` from <algorithm> replaces the
//   hand-rolled shuffle loop; the header's _Rm/_Rn scaling loop compiles
//   byte-exactly. 66.8 -> 68.1.
// - The mission block after FUN_004816a0 is nested the original's way,
//   `if (mission != 0) { summary; if (BetweenMissions()==0) { FUN_00432610;
//   goto tail; } } else if (state != 1) goto tail;` then the shared
//   FUN_00488310/FUN_0041d1f0. 68.1 -> 69.7.
// - `rec+0x149` as a 1-bit `unsigned short` bitfield gives the original's
//   direct `or byte ptr [rec+0x149],1`; a plain `unsigned char |=` goes
//   through a register (this is guide item 1, and it works here too).
// - The final player-record access goes through a record local (`currec`) so
//   the pointer chain is `lea ..+0x1b63; mov eax,[rec+0x27];
//   or byte ptr [eax+0x9b],0x10`, as in the original.
//
// This pass (12 min timebox, 3 check runs, 80.7 -> 80.8):
// - Making the six 0x14281 lane updates use 32-bit temps
//   (`unsigned int b = *(unsigned char*)(p+0x9c); unsigned int w =
//   *(unsigned short*)(g_game+0x14281); w = (w & ~2) | (b & 2); store`)
//   DOES give the original's 32-bit `and reg,2` / `or reg,reg` shape with no
//   movzx (verified in tools/wcl micro-tests), but in this function it moves
//   the word into ecx, the byte into edx and g_game into esi, the lanes lose
//   the 2-byte `xor` (2764 bytes) and the checker scores 74.4. Reverted.
//   The register file is already committed at the switch, so the lane shape
//   cannot be fixed before the `mov edi,1` vs `mov edx,1` difference is.
// - `pos.x.i` before `pos.y.i = 0` (the original's order at 0x4976cd) saved
//   one instruction move: 80.7 -> 80.8.
//
// Known remaining differences:
// - The `g_game[0x14281]` flag read-modify-writes: the original loads the byte
//   into a 32-bit register and masks 32-bit (`mov bl,[p+0x9c]; and ebx,2/4/1`),
//   ours narrows the operand to 16-bit (`and bl,2; movzx si,bl`). Tried this
//   pass: an int local per statement (77.4), three separate unsigned int
//   locals (66.6), and a `static inline unsigned int Bit(b,m)` helper (78.6,
//   unchanged). None reproduces the 32-bit AND, so it looks like an allocator
//   choice, not a source shape.
// - The switch keeps its constant `1` in edx here and the original in edi
//   (`mov edi,1` once, reused as the mask in cases 1 and 2).
// - `-1` for the `order` fill is hoisted to `mov ebx,-1` at the switch here
//   (MSVC keeps the constant in a callee-saved register across the function),
//   where the original emits `or eax,0xffffffff` at the stosd site.
// - `*(unsigned short*)(g_game + 0x38d75) |= 4` folds to `or byte ptr [..],4`
//   here; the original loads the word, ORs in a register and stores it back.
//   That is the volatile-network-flags signature the guide names; left as a
//   note rather than a volatile declaration.
// - `if (pl->b9b & 0x40)` here compiles to `test byte ptr [..],0x40`; the
//   original uses `mov al,[..]; shr al,6; test al,1` for that one test.
// - the `(rand() * 2) / 0x8000` test came out of the x86 as a 64-bit
//   __allmul/__alldiv pair, kept literally.
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

    switch (((Class_00435100*)*(void**)(g_game + 0x391e9))->FUN_00435100()) {
    case 1: {
        DAT_005091cc = 0;
        char* base = g_game + 0x39219;
        *(int*)(g_game + 0x37ef6) = *(int*)base;
        *(unsigned short*)(g_game + 0x14281) =
            (unsigned short)((*(unsigned short*)(g_game + 0x14281) & 0xfffb) |
                ((*(unsigned char*)(base + 0xc) & 1) << 2));
        unsigned short v = *(unsigned short*)(g_game + 0x14281);
        unsigned char b = *(unsigned char*)(base + 4);
        *(unsigned short*)(g_game + 0x14281) = (unsigned short)(((v ^ b) & 1) ^ v);
        *(unsigned short*)(g_game + 0x14281) =
            (unsigned short)((*(unsigned short*)(g_game + 0x14281) & 0xfffd) |
                ((*(unsigned char*)(base + 8) & 1) << 1));
        FUN_00431740();
        break;
    }
    case 2: {
        char* base = (char*)*(void**)(g_game + 0x29a0) + 0x108;
        *(unsigned short*)(g_game + 0x37ee6) = *(unsigned short*)(g_game + 0x37eec);
        DAT_005091cc = 1;
        *(int*)(g_game + 0x37ef6) = *(int*)base;
        *(unsigned short*)(g_game + 0x14281) =
            (unsigned short)((*(unsigned short*)(g_game + 0x14281) & 0xfffb) |
                ((*(unsigned char*)(base + 0xc) & 1) << 2));
        unsigned short v = *(unsigned short*)(g_game + 0x14281);
        unsigned char b = *(unsigned char*)(base + 4);
        *(unsigned short*)(g_game + 0x14281) = (unsigned short)(((v ^ b) & 1) ^ v);
        *(unsigned short*)(g_game + 0x14281) =
            (unsigned short)((*(unsigned short*)(g_game + 0x14281) & 0xfffd) |
                ((*(unsigned char*)(base + 8) & 1) << 1));
        break;
    }
    case 3: {
        *(unsigned short*)(g_game + 0x37ee6) = *(unsigned short*)(g_game + 0x37eec);
        DAT_005091cc = 1;
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
                for (int i = 0; i < 10; i++) {
                    if (*def == 1 || *def == 2)
                        count = i + 1;
                    def += 6;
                }
                int cur = *(int*)(g_game + 0x38d81);
                if (cur < count)
                    cur = count;
                *(int*)(g_game + 0x38d81) = cur;
                FUN_0047a760();
            }
        }
    }

    FUN_004917d0();

    if (((Class_00435100*)*(void**)(g_game + 0x391e9))->FUN_00435100() != 1) {
        if (((Class_00435100*)*(void**)(g_game + 0x391e9))->FUN_00435100() == 3) {
            *(unsigned short*)(g_game + 0x38d75) |= 4;
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
            unsigned char lpflag = *(unsigned char*)(lp + 0x9b);
            lpflag = lpflag >> 6;
            int cx;
            int cz;
            if (lpflag & 1) {
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

    *(unsigned short*)(g_game + 0x38d75) |= 2;
}
