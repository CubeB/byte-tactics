// Decompiled by deepseek-v4.1-flash. Names are provisional.
//
// 2797 bytes, transcribed from the disassembly and Ghidra's pseudo-C inside a
// short timebox. Not matched. Known remaining differences:
// - the three ten-player walks each carry a redundant `cmp idx,10; jae` guard
//   before the body (an inlined bounds-checked accessor in the original);
//   written here as `if ((unsigned char)i < 10)` wrappers.
// - case 3 (skirmish/game start) is the bulk of the function and its register
//   allocation (g_game in esi/ebp vs edx, the zero in ebx) is unverified.
// - the `(rand() * 2) / 0x8000` test came out of the x86 as a 64-bit
//   __allmul/__alldiv pair, kept literally.
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

struct Fixed_497180 {
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

void FUN_004b6ca0(int x);
void FUN_004b6b50(int x);
int FUN_004b6c30(int x);
int FUN_00456850();
void FUN_00431740();
void FUN_00453d40();
void FUN_00465fb0(void* mission);
void FUN_0047a760();
void FUN_004917d0();
void FUN_00465e30();
void FUN_004816a0(int x);
void FUN_00432610(void* mission);
void FUN_00488310();
void FUN_0041d1f0();
void FUN_004288d0(int a, int b, int c, int d);
void FUN_00450f90();
void FUN_00451180();
void FUN_00464f80();
void FUN_0046c620(int x);
void FUN_004649d0();
void FUN_0041c4c0(int x, int y, int z);
unsigned short FUN_00488b10(const char* name);
void FUN_00496ee0(int team, int startpos);
void FUN_00485f50(unsigned char team, unsigned short id, FixedPos_497180 pos, int a, int b,
    int c);
Gadget_497180* FUN_004aa8f0(Sub_497180* sub, const char* name, int flags);
void __stdcall FUN_00494890(Gadget_497180* gadget);
void __cdecl operator delete(void* p);

// FUNCTION: 0x497180
void FUN_00497180(void)
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

        unsigned char sel = (unsigned char)FUN_00456850();
        if (*(unsigned char*)(g_game + 0x1b63 + 0x14b * sel + 0x21) & 2) {
            do {
                char* p = *(char**)(g_game + 0x1b63 + 0x14b * *(unsigned char*)(g_game + 0x2a42) + 0x27);
                if (DAT_00506dbc)
                    DAT_00513000.FUN_004618a0(1);
                FUN_00453d40();
                sel = (unsigned char)FUN_00456850();
                FUN_004b6b50(0x32);
                if (sel == 10)
                    continue;
                if (*(char*)(p + 0x96) == -1)
                    continue;
                if (*(char*)(p + 0x8f) == 0)
                    continue;
                break;
            } while (1);
            FUN_004b6b50(0x32);
        }

        ((Class_00435a20*)*(void**)(g_game + 0x391e9))
            ->FUN_00435a20(*(void**)(g_game + 0x1b63 + 0x14b * sel + 0x27));
        if ((unsigned char)FUN_00456850() == 10)
            break;

        unsigned char sel2 = (unsigned char)FUN_00456850();
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

            unsigned char sel = (unsigned char)FUN_00456850();
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
                pos.y.i = 0;
                pos.x.i = (FUN_004b6c30(*(int*)(g_game + 0x14223) - 0xa0) + 0x50) << 16;
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
                *(unsigned char*)(rec + 0x149) |= 1;
                *(float*)(rec + 0xdc) = (float)(s1 >= 200 ? s1 : 200);
                *(float*)(rec + 0xe0) = (float)(s2 >= 200 ? s2 : 200);
            }

            unsigned char li = *(unsigned char*)(g_game + 0x2a42);
            char* lp = *(char**)(g_game + 0x1b63 + 0x14b * li + 0x27);
            if ((*(unsigned char*)(lp + 0x9b) >> 6) & 1) {
                *(unsigned short*)(g_game + 0x14281) &= 0xfffe;
                *(unsigned short*)(g_game + 0x14281) &= 0xfffd;
                FUN_0041c4c0(*(int*)(g_game + 0x37e37) / 2,
                    *(int*)(g_game + 0x37e3b) / 2, 0);
            } else {
                FUN_0041c4c0(start.x.h.whole - *(int*)(g_game + 0x37e37) / 2,
                    start.z.h.whole - *(int*)(g_game + 0x37e3b) / 2, 0);
            }
            FUN_0046c620(6);
        } else if (((Class_00435100*)*(void**)(g_game + 0x391e9))->FUN_00435100() == 2 &&
            *(void**)(g_game + 0x38d6b) == 0) {
            if (*(int*)((char*)*(void**)(g_game + 0x29a0) + 0x118) != 0) {
                for (int i1 = 0; i1 < 10; i1++) {
                    if ((unsigned char)i1 < 10) {
                        char* rec = g_game + 0x1b63 + 0x14b * i1;
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
                        char* rec = g_game + 0x1b63 + 0x14b * i3;
                        if (*(int*)rec != 0) {
                            unsigned char st = *(unsigned char*)(rec + 0x73);
                            if ((st == 1 || st == 2 || st == 3) &&
                                *(unsigned char*)(rec + 0x146) != 10)
                                order[n++] = i3;
                        }
                    }
                }
                if (n > 2 || (int)(((__int64)rand() * 2) / 0x8000) != 0) {
                    for (int i5 = 1; i5 < n; i5++) {
                        int j = rand() % i5;
                        int t = order[i5];
                        order[i5] = order[j];
                        order[j] = t;
                    }
                }
                int k = 0;
                for (int i4 = 0; i4 < 10; i4++) {
                    if ((unsigned char)i4 < 10) {
                        char* rec = g_game + 0x1b63 + 0x14b * i4;
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

    if (*(void**)(g_game + 0x38d6b) == 0) {
        if (((Class_00435100*)*(void**)(g_game + 0x391e9))->FUN_00435100() != 1)
            goto tail;
    } else {
        ((Class_004b4560*)*(void**)(g_game + 0x38d6b))->FUN_004b4560("summary");
        if (((Class_004b48f0*)*(void**)(g_game + 0x38d6b))->FUN_004b48f0("BetweenMissions") ==
            0) {
            FUN_00432610(*(void**)(g_game + 0x38d6b));
            goto tail;
        }
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

    *(unsigned char*)(*(char**)(g_game + 0x1b63 +
                          0x14b * *(unsigned char*)(g_game + 0x2a42) + 0x27) +
        0x9b) |= 0x10;
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
