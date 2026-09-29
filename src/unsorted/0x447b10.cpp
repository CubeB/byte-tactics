// Decompiled by deepseek-v4.1-flash. Names are provisional.
//
// PARTIAL. This is the multiplayer setup / chat command dispatcher. It parses
// one command string against a long chain of keyword tests (LOGO%d, PLAYER%d,
// SIDE%d, ALLY%d, TEAMICONS%d, RES%d, READY%d, PREVMENU, MESSAGE, COMMANDER,
// LOSTYPE, WATCHING, CHEATING, FIXEDLOC, MAPPING, START, GAMEOPEN, ...).
// Only the prologue and the LOGO%d block have been transcribed so far; the
// frame size (sub esp,0x13c) will not match until the whole body and all
// locals (char name[252] + int local_28[10] + the pointer locals) are present.
// check.py: 6.4% (236/4306 bytes). Everything from the PLAYER%d block on is
// missing, and the few locals declared here are dead so MSVC shrinks the frame.
//
// Layout notes for the next attempt:
//   arg1 = Event*, single stack arg, ret 4 -> __stdcall free function.
//   [arg1+0x18] -> [..+4] is iVar8 (saved at esp+0x14).
//   [arg1+0x60] == -1 is the "leave game" path (frees g_game+0x2a9b, zeroes
//   DAT_00512994, calls FUN_00446c70, returns).
//   g_game (0x511de8) fields: +0x2a42 local player index (byte),
//   +0x2a3c (ushort), +0x2bee event/redraw flag byte, +0x2a9b pointer,
//   +0x37f39 int, +0x499 int, +0x519 gui/window base, +0x531 some obj,
//   +0x391e9 game object, +0x1b63 players[10] stride 0x14b,
//   +0x1b8a dword per player (player info ptr), +0x1bd6 type per player,
//   +0x1ca2 alliance colour per player, +0x2bc0 screen/menu state.
//   per-player info (Player::+0x27): +0x95 counter, +0x96 count, +0x97 flags,
//   +0x9b ushort flags (0x40 local, 0x80 ready, 0x200/0x400/0x1800 watch,
//   0x2000/0x4000 commands), +0x9c byte flags, +0x9d ushort flags.
#include <stdio.h>

#pragma pack(push, 1)
struct PlayerInfo {
    char unknown_0[0x95];
    unsigned char field_95;          // +0x95
    unsigned char field_96;          // +0x96
    char unknown_97[0x9b - 0x97];
    unsigned short flags_9b;         // +0x9b
};

struct Player {
    int active;                      // +0x00
    char unknown_4[0x27 - 0x4];
    PlayerInfo* info;                // +0x27
    char unknown_2b[0x73 - 0x2b];
    unsigned char type;              // +0x73
    char unknown_74[0x108 - 0x74];
    unsigned char field_108[0x113 - 0x108];
    unsigned char field_113[0x13f - 0x113];
    unsigned char alliance;          // +0x13f
    char unknown_140[0x14b - 0x140];
};
#pragma pack(pop)

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x1b63];
    Player players[10];              // +0x1b63
    char unknown_2851[0x2a42 - 0x2851];
    unsigned char localPlayer;       // +0x2a42
    char unknown_2a43[0x2a9b - 0x2a43];
    int ptr_2a9b;                    // +0x2a9b
    char unknown_2a9f[0x2bee - 0x2a9f];
    unsigned char flags_2bee;        // +0x2bee
};
#pragma pack(pop)

struct Event {
    char unknown_0[0x18];
    int* field_18;                   // +0x18
    char unknown_1c[0x60 - 0x1c];
    int field_60;                    // +0x60
};

extern Game* g_game;                 // 0x511de8
extern int DAT_00512994;

void FUN_00446c70();
void FUN_00450f90();
void FUN_004d85a0(void* p);
int FUN_004526c0(int n);
int FUN_0047f1a0(const char* s, int v);
int FUN_0049fd60(int a, const char* b);
int FUN_00457a50();

// FUNCTION: 0x447b10
void __stdcall FUN_00447b10(Event* ev)
{
    int iVar8 = *(int*)((char*)ev->field_18 + 4);

    if (ev->field_60 == -1) {
        FUN_004d85a0((void*)g_game->ptr_2a9b);
        g_game->ptr_2a9b = 0;
        DAT_00512994 = 0;
        FUN_00446c70();
        return;
    }

    char name[252];
    int local_28[10];
    unsigned char li = g_game->localPlayer;
    Player* localPlayer = &g_game->players[li];
    int iVar5 = FUN_00457a50();

    for (int i = 0; i < 10; i++) {
        Player* pl = &g_game->players[i];
        sprintf(name, "LOGO%d", i);
        if (FUN_0049fd60((int)ev, name)
            && pl->active != 0
            && (pl->type == 1 || pl->type == 2)) {
            FUN_0047f1a0("Multi", 0);
            FUN_004526c0(pl->info->field_96 + 1);
            g_game->flags_2bee |= 1;
            FUN_00450f90();
        }
        (void)localPlayer;
        (void)iVar5;
        (void)iVar8;
        (void)local_28;
    }
}
