// Decompiled by deepseek-v4.1-flash, finished by GPT-6.1-sol, finished by
// deepseek-v4.1-flash, finished by deepseek-v4.1, finished by deepseek-v4.1-flash,
// finished by Claude Fable 5.1. Names are provisional.
// Game start: seeds the random generators, loads the match settings for the
// current network mode (1 = skirmish defaults, 2 = multiplayer host block,
// 3 = joined game, from the host player's record), runs the between-missions
// summary, spawns each player's commander and opens the MAIN2 GUI.
//
// What took this from 82.8% to MATCH (issue 4408):
// - Cases 1 and 2 are inlined calls of FUN_00496e10 (matched on its own in
//   0x496e10.cpp, zero callers in the exe because /Ob2 inlined every call):
//   it copies the settings block's first int to +0x37ef6 and three int flags
//   into bits 2, 0 and 1 of the view-flags word at +0x14281. The inlined
//   `int -> 1-bit field` assignments are what mask with the CSE'd constant 1
//   (`mov edi,1` at the switch head, `and ebx,edi`), while the hand-written
//   `(w & ~M) | (b & M)` lanes narrow to `and bl,1; movzx si,bl`.
// - Case 3 and the post-wait block copy BITFIELDS of the player's
//   `unsigned short` flags at +0x9b: bits 8, 9 and 10 sit in the byte at
//   +0x9c, and `view->bit1 = pf->b9` (same bit position on both sides) is what
//   MSVC 5 compiles to the zero-extended byte load plus `and ebx,2` with no
//   shift; the 2-bit field at bits 11-12 and the 1-bit field at 13 give the
//   `shr ecx,0xb; and ecx,3` and `shr ecx,0xd; and ecx,1` extracts.
// - Case 2's statement order is copy 0x37eec -> 0x37ee6 first, then
//   DAT_005091cc = 1, then the settings block; the store through g_game makes
//   MSVC reload g_game for the block, as the original does.
// - The commander spawn loop is `for (int i = 0; i < 10; i++)` indexing
//   0x14b-byte records; MSVC strength-reduces it to the byte offset in ebx and
//   that puts g_game in the SIB base. A hand-written `off += 0x14b` loop swaps
//   the base and index.
// - QueryPerformanceCounter's halves: `int hi = HighPart; int lo = LowPart;
//   FUN_004b6ca0(lo + hi)` loads LowPart first; declaration order decides the
//   load order.
// Earlier passes fixed: the network flags at +0x38d75 written as volatile
// (the field the guide names); the +0x9b bit-6 test as a 1-bit bitfield; the
// mission-count loop with `i++` before `def += 6`; the ten-player walks with
// `(unsigned char)i` so the multiply is not strength-reduced; the
// std::random_shuffle call; rec+0x149 as a 1-bit bitfield (direct `or byte`);
// `pos.x.i` before `pos.y.i = 0`.
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <algorithm>
#include <time.h>

struct Settings_00496e10 {
    int value;                          // +0x0
    int flag_4;                         // +0x4
    int flag_8;                         // +0x8
    int flag_c;                         // +0xc
};

struct ViewFlags_497180 {               // g_game + 0x14281
    unsigned short bit0 : 1;
    unsigned short bit1 : 1;
    unsigned short bit2 : 1;
    unsigned short rest : 13;
};

struct PlayerFlags_497180 {             // player + 0x9b
    unsigned short low : 8;
    unsigned short b8 : 1;              // +0x9c bit 0
    unsigned short b9 : 1;              // +0x9c bit 1
    unsigned short b10 : 1;             // +0x9c bit 2
    unsigned short b11_12 : 2;
    unsigned short b13 : 1;
    unsigned short rest : 2;
};

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
    void LoadMissionByName(void* player);
};

class Class_00437320 {
public:
    int FUN_00437320(FixedPos_497180* pos, int id);
};

class PacketManager {
public:
    void SendAllQueued(int a);
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
static inline ViewFlags_497180* g_game_view() { return (ViewFlags_497180*)(g_game + 0x14281); }
extern int DAT_005091cc;
extern int g_usePacketManager;
extern PacketManager g_packetManager;

void __stdcall FUN_004b6ca0(int x);
void __stdcall FUN_004b6b50(int x);
int __stdcall FUN_004b6c30(int x);
unsigned char __stdcall FindHostSlot();
void FUN_00431740();
void HandleNetPackets();
void __stdcall LoadPlayerControllers(void* mission);
void FUN_0047a760();
void FUN_004917d0();
void FUN_00465e30();
void __stdcall FUN_004816a0(int x);
void __stdcall LoadSavedGameState(void* mission);
void CreateMissionUnits();
void FUN_0041d1f0();
void __stdcall FUN_004288d0(int a, int b, int c, int d);
void BroadcastPlayerInfo();
void UpdateNetGameInfo();
void FUN_00464f80();
void __stdcall ReportGameEvent(int x);
void FUN_004649d0();
void __stdcall SetCameraPosition(int x, int y, int z);
unsigned short __stdcall FindUnitTypeId(const char* name);
void __stdcall FUN_00496ee0(int team, int startpos);
void __stdcall CreateUnit(unsigned char team, unsigned short id, FixedPos_497180 pos, int a,
    int b, int c);
Gadget_497180* __stdcall LoadGuiLayer(Sub_497180* sub, const char* name, int flags);
void __stdcall FUN_00494890(Gadget_497180* gadget);
void __cdecl operator delete(void* p);

// Inlined into cases 1 and 2 below (matched on its own in 0x496e10.cpp).
inline void __stdcall FUN_00496e10(Settings_00496e10* s)
{
    *(int*)(g_game + 0x37ef6) = s->value;
    g_game_view()->bit2 = s->flag_c;
    g_game_view()->bit0 = s->flag_4;
    g_game_view()->bit1 = s->flag_8;
}

// FUNCTION: 0x497180
void __cdecl LoadMatch(void*)
{
    LARGE_INTEGER perfCount;
    FixedPos_497180 pos;
    FixedPos_497180 start;
    int order[10];

    QueryPerformanceCounter(&perfCount);
    int hi = perfCount.HighPart;
    int lo = perfCount.LowPart;
    FUN_004b6ca0(lo + hi);
    srand((unsigned)time(NULL));
    *(int*)(g_game + 0x38a47) = 0;

    switch (((Class_00435100*)*(void**)(g_game + 0x391e9))->FUN_00435100()) {
    case 1:
        DAT_005091cc = 0;
        FUN_00496e10((Settings_00496e10*)(g_game + 0x39219));
        FUN_00431740();
        break;
    case 2:
        *(unsigned short*)(g_game + 0x37ee6) = *(unsigned short*)(g_game + 0x37eec);
        DAT_005091cc = 1;
        FUN_00496e10((Settings_00496e10*)((char*)*(void**)(g_game + 0x29a0) + 0x108));
        break;
    case 3: {
        *(unsigned short*)(g_game + 0x37ee6) = *(unsigned short*)(g_game + 0x37eec);
        DAT_005091cc = 1;
        *(unsigned short*)(g_game + 0x38a51) &= 0xfffe;

        int sel = FindHostSlot();
        unsigned char cur = *(unsigned char*)(g_game + 0x2a42);
        if (*(unsigned char*)(g_game + 0x1b63 + 0x14b * cur + 0x21) & 2) {
            do {
                char* p = *(char**)(g_game + 0x1b63 + 0x14b * *(unsigned char*)(g_game + 0x2a42) + 0x27);
                if (g_usePacketManager)
                    g_packetManager.SendAllQueued(1);
                HandleNetPackets();
                sel = FindHostSlot();
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
            ->LoadMissionByName(*(void**)(g_game + 0x1b63 + 0x14b * sel + 0x27));
        if (FindHostSlot() == 10)
            break;

        int sel2 = FindHostSlot();
        char* p2 = *(char**)(g_game + 0x1b63 + 0x14b * sel2 + 0x27);
        PlayerFlags_497180* pf = (PlayerFlags_497180*)(p2 + 0x9b);
        DAT_005091cc = pf->b13;
        *(int*)(g_game + 0x37ef6) = pf->b11_12;
        g_game_view()->bit1 = pf->b9;
        g_game_view()->bit2 = pf->b10;
        g_game_view()->bit0 = pf->b8;
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
            LoadPlayerControllers(*(void**)(g_game + 0x38d6b));
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

            int sel = FindHostSlot();
            char* pl = *(char**)(g_game + 0x1b63 + 0x14b * sel + 0x27);
            PlayerFlags_497180* pf = (PlayerFlags_497180*)(pl + 0x9b);
            g_game_view()->bit0 = pf->b8;
            g_game_view()->bit1 = pf->b9;
            g_game_view()->bit2 = pf->b10;
            *(int*)(g_game + 0x37ef6) = pf->b11_12;

            for (int i = 0; i < 10; i++) {
                char* rec = g_game + 0x1b63 + 0x14b * i;
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
                    FindUnitTypeId(g_game + 0x37f5f + 0x232 * side);
                CreateUnit(*(unsigned char*)(rec + 0x146), id, pos, 1, 1, 0);
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
            SetCameraPosition(cx, cz, 0);
            ReportGameEvent(6);
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
            LoadSavedGameState(*(void**)(g_game + 0x38d6b));
            goto tail;
        }
    } else if (((Class_00435100*)*(void**)(g_game + 0x391e9))->FUN_00435100() != 1) {
        goto tail;
    }
    CreateMissionUnits();
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
        LoadGuiLayer((Sub_497180*)(g_game + 0x519), g_game + 0x37ea0, 0x20);
    gadget->handler = FUN_00494890;
    gadget->owner = g_game;

    char* currec = g_game + 0x1b63 + 0x14b * *(unsigned char*)(g_game + 0x2a42);
    *(unsigned char*)(*(char**)(currec + 0x27) + 0x9b) |= 0x10;
    BroadcastPlayerInfo();
    UpdateNetGameInfo();
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
