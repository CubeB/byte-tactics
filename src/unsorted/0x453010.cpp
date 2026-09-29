// Decompiled by DeepSeek V4.1 Flash, finished by space-bunny-free, finished by muse-spark-1.3-free. Names are provisional.
//
// 85.4%, 812 bytes against 772. What still differs: the shared exit tail.
//  The original has ONE `mov al,[esp+0x20] / mov [edi+0x22],al / mov eax,[esp+0x10]
//  / pop x4 / add esp,8 / ret 8` block at 0x4532ff and every path reaches it
//  with a jump (0x4531a2, 0x45320d, 0x4532f4, plus six `je`). Here MSVC 5
//  sinks a private copy of that block into each path whose only successor is
//  the exit, so the state==2 path and the state==3 `c` loop path each carry
//  25-27 bytes of epilogue the original does not have, and the state==1 path
//  costs a 5-byte re-test (`cmp byte ptr [edi+0x73],1 / je`) where the
//  original has a 2-byte `jmp`. Everything else, including the `mov bl,3`
//  constant, the three copies of the find-active helper and the [eax+esi]
//  base/index roles in all three players loops, is byte exact.
//
//  The `if (p->state == 1) { ... } if (p->state != 1) { ... }` pair below
//  (rather than if/else) is worth 1.3 points and 11 bytes: it gives the
//  state==1 arm a fall-through successor that jumps to the shared exit, which
//  stops one of the three sinkings. Swapping the two tests, or writing the
//  second as `== 2`, makes no difference: MSVC reorders them back.
//
// Tried and rejected (all scored with check.py --sym, none better than 85.4%):
//  - goto to a `done:` label before the tail, with 2 or with 3 goto sites:
//    MSVC turns the gotos back into the same CFG, byte-identical output to
//    the if/else form (84.1%). A label anywhere in the function also
//    collapses the frame from `sub esp, 8` to `push ecx` (result moves to
//    ebp, msg to ebx, every [esp+N] shifts by 4, 55.7%).
//  - The shared tail is NOT reachable by any source-level change to the tail
//    itself: `return p->field_22 = value, result;`, `return result;` vs
//    `return tail2;` with an extra local, a `value` local, `do{}while(0)`
//    around the chain, an `else {}` on the chain, an always-false `else if`,
//    a nested `else { if (...) }`, and `switch`/`break` forms all compile to
//    exactly the same 823 bytes / 84.1%. The decision is made on the CFG in
//    brbranch, before any of that is visible, so only the shape of the
//    if/else chain moves it.
//  - `while (1) { if (i >= 10) break; ... i++; }` in place of the state==1
//    players loop: 817 bytes, 81.1%. Same form in the state==3 `c` loop
//    (where the `for` is already correct): 819 bytes, 79.1%.
//  - `result = p->field_22 == 0;` for `result = 1`: 834 bytes, 66.4%.
//  - Splitting the state==3 nested `if` into `else if` + a second `else`:
//    836 bytes, 74.7%.
//  - `Player* q = &g_game->players[i];` in the state==1 loop: MSVC promotes
//    q to an induction variable and the whole loop changes.
//  - The `while (1)` form of the find-active helper is required; a plain
//    `for` rotates and puts the found block out of line (75.4%).
//  - Extra STL headers (<string>, <vector>, <map>, <list>, <iostream>) with
//    <windows.h>: change inlining (756/807 bytes), all worse. <string> alone
//    matches <windows.h> at 84.1% but adds nothing.
//  - headers.py picked <windows.h>; it is load bearing for the [eax+esi]
//    base/index roles in the loops and raised the score from 81.3% to 84.1%.
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
        }
        if (p->state != 1) {
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
