// Decompiled by space-bunny-free. Names are provisional.
// Finds, in the current team's unit list, a finished unit (bit 5 of the flags
// dword at +0x110, build fraction at +0x104 down to 0, nothing queued at
// +0xfb, not attached to something that blocks it) whose type name matches
// the owning player's name, then centres the view on it and selects it.
// Layout notes for the neighbours 0x48d4d0 and 0x48d790 (same structs):
// - the 10 team records at g_game+0x1b63 are 0x14b bytes: +0x27 the player
//   (its +0x95 is the index), +0x67 / +0x6b the unit list bounds, +0x6a a
//   Pos for CenterCameraOnMapPosition, +0xa8 a word copied to g_game+0x1436f.
// - a unit is 0x118 bytes: +0x6a Pos, +0x86 a pointer to a struct with a
//   flags dword at +0x110, +0x92 the type (name at +0x20), +0xa6 a type
//   index, +0xfb an int, +0x104 the build fraction, +0x110 the flags dword.
// - the unit array at g_game+0x14357 is indexed by the same 0x118 stride.
// Notes on the two spots that cost a run each: the float test at +0x104 is
// `== 0.0f` (`fcomp`, then `je` to the loop increment, so the body needs
// C3), and the clear loop masks 0xffffff2f, so it clears bits 4, 6 and 7 of
// +0x110, not bit 5.
#include <string.h>

#pragma pack(push, 1)
struct UnitType_0048d630 {
    char unknown_0[0x20];
    char name[0x20];                   // +0x20
};

struct Player_0048d630 {
    char unknown_0[0x95];
    unsigned char index;               // +0x95
};

struct OwnerFlags_0048d630 {
    char unknown_0[0x110];
    unsigned int bits0 : 30;
    unsigned int bit30 : 1;            // bit 30
    unsigned int bit31 : 1;
};

struct Pos_0048d630 {
    short x;                           // +0
    short y;                           // +2
    short z;                           // +4
};

struct UnitFlags_0048d630 {
    unsigned int bits0 : 4;
    unsigned int selected : 1;         // bit 4
    unsigned int done : 1;             // bit 5
    unsigned int bit6 : 1;
    unsigned int bit7 : 1;
    unsigned int bits8 : 24;
};

struct Unit {
    char unknown_0[0x6a];
    Pos_0048d630 pos;                  // +0x6a
    char unknown_70[0x86 - 0x70];
    OwnerFlags_0048d630* attached;     // +0x86
    char unknown_8a[0x92 - 0x8a];
    UnitType_0048d630* type;           // +0x92
    char unknown_96[0xfb - 0x96];
    int unknown_fb;                    // +0xfb
    char unknown_ff[0x104 - 0xff];
    float remaining;                   // +0x104
    char unknown_108[0x110 - 0x108];
    UnitFlags_0048d630 flags;          // +0x110
    char unknown_114[0x118 - 0x114];
};

struct Team_0048d630 {
    char unknown_0[0x27];
    Player_0048d630* player;           // +0x27
    char unknown_2b[0x67 - 0x2b];
    Unit* unitsBegin;                  // +0x67
    Unit* unitsEnd;                    // +0x6b
    char unknown_6f[0x14b - 0x6f];
};

struct Name_0048d630 {
    char name[0x232];                  // +0x00
};

struct Orders_0048d630 {
    unsigned short bits0 : 4;
    unsigned short selected : 1;       // bit 4
    unsigned short bits5 : 11;
};

struct Game {
    char unknown_0[0x1b63];
    Team_0048d630 teams[10];           // +0x1b63
    char unknown_2851[0x2a43 - 0x2851];
    unsigned char team;                // +0x2a43
    char unknown_2a44[0x14357 - 0x2a44];
    Unit* units;                       // +0x14357
    Unit* unitsEnd;                    // +0x1435b
    char unknown_1435f[0x37ebe - 0x1435f];
    Orders_0048d630 orders;            // +0x37ebe
    char unknown_37ec0[0x37f5f - 0x37ec0];
    Name_0048d630 players[10];         // +0x37f5f
};
#pragma pack(pop)

extern Game* g_game;

void __cdecl FUN_0041c390(void);
void __stdcall CenterCameraOnMapPosition(Pos_0048d630* pos, int centre);
void SelectStopOrder(void);
int __stdcall FUN_00491d70(int force);

// FUNCTION: 0x48d630
void __stdcall FUN_0048d630(int param_1)
{
    Team_0048d630* team = &g_game->teams[g_game->team];
    char* playerName = g_game->players[team->player->index].name;
    Unit* last = team->unitsEnd;
    for (Unit* u = team->unitsBegin; u <= last; u++) {
        if (u->flags.done && u->remaining == 0.0f && u->unknown_fb == 0
            && (u->attached == 0 || u->attached->bit30)) {
            if (strcmp(u->type->name, playerName) == 0) {
                FUN_0041c390();
                CenterCameraOnMapPosition(&u->pos, 1);
                if (param_1 == 0)
                    return;
                SelectStopOrder();
                for (Unit* v = g_game->units; v <= g_game->unitsEnd; v++) {
                    v->flags.selected = 0;
                    v->flags.bit6 = 0;
                    v->flags.bit7 = 0;
                }
                FUN_00491d70(0);
                u->flags.selected = 1;
                g_game->orders.selected = 1;
                return;
            }
        }
    }
}
