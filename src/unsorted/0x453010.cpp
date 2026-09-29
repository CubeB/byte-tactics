// Decompiled by DeepSeek V4.1 Flash, finished by space-bunny-free, finished by muse-spark-1.3-free. Names are provisional.
//
// 84.1%, 823 bytes against 772. What still differs:
//  Three sites emit their own copy of the final
//  `p->field_22 = value; return result;` tail plus epilogue
//  (after the state==1 loop, after the state==2 block, after the
//  state==3 c-loop). The original has a single shared tail at 0x4532ff
//  that every path reaches with a jump; each branch only stores 1 into
//  the result local first. Everything else matches, including the
//  `mov bl, 3` constant and the [eax + esi] base/index roles in both
//  players loops (the roles need `#include <windows.h>`, found via
//  headers.py, which also raised the score from 81.3% to 84.1%).
//
// Tried and rejected:
//  - goto done (2-3 sites) plus a `done:` label before the tail: MSVC
//    duplicates the return block at each goto site instead of jumping,
//    and any label, do-while(0), or nested outer `if (p->active)` in
//    the function collapses the frame from `sub esp, 8` to `push ecx`
//    (result goes to ebp, msg to ebx, every [esp+N] shifts by 4,
//    55.7%). So the shared tail is not reachable that way.
//  - switch on state with breaks, do-while(0) with breaks: same or
//    worse (65.3%, 38.9%); breaks do not jump to a shared tail either.
//  - Extra STL headers (<string>, <vector>, <map>, <list>, <iostream>)
//    with <windows.h>: change inlining (756/807 bytes), all worse.
//    <string> alone matches <windows.h> at 84.1% but adds nothing.
//  - `Player* q = &g_game->players[i];` in the state==1 loop: MSVC
//    promotes q to an induction variable, whole loop changes (57%).
//  - `while (1)` form of the find-active helper is required; plain
//    `for` rotates and puts the found block out of line (75.4%).
#include <windows.h>

#pragma pack(push, 1)
struct PlayerInfo_00453010 {
    char unknown_0[0x94];
    unsigned char field_94;            // +0x94
};

struct Player_00453010 {
    int active;                        // +0x00
    int id;                            // +0x04
    char unknown_8[0xc - 8];
    int field_c;                       // +0x0c
    char unknown_10[0x22 - 0x10];
    unsigned char field_22;            // +0x22
    char unknown_23[0x27 - 0x23];
    PlayerInfo_00453010* field_27;     // +0x27
    char unknown_2b[0x73 - 0x2b];
    unsigned char state;               // +0x73
    char unknown_74[0x14b - 0x74];
};

struct Game_00453010 {
    char unknown_0[0x1b63];
    Player_00453010 players[10];       // +0x1b63
    char unknown_2851[0x2a38 - (0x1b63 + 0x14b * 10)];
    unsigned char* buffer;             // +0x2a38
};
#pragma pack(pop)

extern Game_00453010* g_game;

int __stdcall FUN_0044ffd0(unsigned char index);
unsigned char __stdcall FUN_0044fe40(int id);
int __stdcall FUN_00451df0(int player, unsigned char* data, int size);
void __stdcall FUN_00452cc0(int id);

static inline unsigned char FindIndex_00453010(int id)
{
    for (unsigned char i = 0; i < 10; i++) {
        if (FUN_0044ffd0(i) == id)
            return i;
    }
    return 10;
}

static inline int FindActiveId_00453010()
{
    int i = 0;
    while (1) {
        if (g_game->players[i].active != 0
            && (g_game->players[i].state == 1 || g_game->players[i].state == 2))
            return g_game->players[i].id;
        i++;
        if (i >= 10)
            return -1;
    }
}
// FUNCTION: 0x453010
int __stdcall FUN_00453010(int id, unsigned char value)
{
    int result = 0;

    unsigned char fi;
    if (id == -1)
        fi = 10;
    else
        fi = FindIndex_00453010(id);

    Player_00453010* p;
    if (fi == 10)
        p = 0;
    else
        p = &g_game->players[FUN_0044fe40(id)];

    if (p == 0)
        return 0;

    unsigned char* msg = g_game->buffer;
    msg[0] = 0x1b;
    *(int*)(msg + 1) = -1;
    msg[5] = value;

    if (p->active != 0
        && (p->state == 1 || p->state == 2)
        && p->field_22 == 0) {
        if (p->state == 1) {
            for (int i = 0; i < 10; i++) {
                if (g_game->players[i].active != 0
                    && (g_game->players[i].state == 1 || g_game->players[i].state == 2)) {
                    *(int*)(msg + 1) = g_game->players[i].id;
                    FUN_00451df0(FindActiveId_00453010(), msg, 6);
                    FUN_00452cc0(p->id);
                    g_game->players[i].field_22 = value;
                }
            }
            result = 1;
        } else {
            *(int*)(msg + 1) = p->id;
            FUN_00451df0(FindActiveId_00453010(), msg, 6);
            FUN_00452cc0(p->id);
            result = 1;
        }
    } else if (p->active != 0 && p->state == 3 && p->field_22 == 0) {
        *(int*)(msg + 1) = id;
        result = FUN_00451df0(FindActiveId_00453010(), msg, 6);
        if (p->active != 0 && p->state == 3 && p->field_27->field_94 == 1) {
            unsigned char c = p->field_c;
            for (int i = 0; i < 10; i++) {
                if (g_game->players[i].field_c == c) {
                    FUN_00452cc0(g_game->players[i].id);
                    g_game->players[i].field_22 = value;
                }
            }
        } else {
            FUN_00452cc0(p->id);
        }
    }

    p->field_22 = value;
    return result;
}
