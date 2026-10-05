// Decompiled by space-bunny-free. Names are provisional.
// Sets a team up for one start position, for each of the (up to ten) teams the
// caller walks: the two bytes of the team's definition are copied into the
// player object, the two sizes in the definition are clamped to a minimum of
// 200 and stored as floats, the start position is looked up in the campaign's
// entry table (16.16 fixed point), and the team number, the player's own unit
// type and the position go to CreateUnit. When the team is the local one the
// view is centred on the position afterwards.
//
// Layout notes, for whoever reads the neighbours of this code:
// - the ten team records at g_game+0x1b63 are 0x14b bytes: +0x27 the player
//   (its +0x95 is the index into the name table, its +0x96 the second copied
//   byte), +0xdc and +0xe0 the two clamped sizes as floats, +0x149 a 1-bit
//   unsigned short bitfield (setting it gives the straight-to-memory
//   `or byte ptr [ecx+0x149], 1`; a plain unsigned char field goes through a
//   register, which moves the whole allocation).
// - the team definitions are 0x18-byte records behind the pointer at
//   g_game+0x29a0: +0x4 and +0x14 the two copied bytes, +0xc and +0x10 the two
//   sizes, +0x10 first stored to +0xdc.
// - the name table at g_game+0x37f5f is 0x232 bytes per entry and the player
//   index selects the entry, whose name is looked up by FindUnitTypeId. MSVC 5
//   gives `Entry names[8]` a size of 0x1190, not 8 * 0x232, so the field after
//   it starts at +0x390ef.
// - the position is a Vec3 of 16.16 values; the view is centred on the whole
//   part of x and z, read as the high half of each fixed-point int through a
//   `union Fixed`. x goes with the view width and z with the view height.
#include <stdio.h>

#pragma pack(push, 1)
struct TeamDef_00496ee0 {            // 0x18 bytes, array at g_game+0x29a0
    char unknown_0[0x4];
    unsigned char nameIndex;           // +0x4
    char unknown_5[0xc - 0x5];
    int size1;                         // +0xc
    int size2;                         // +0x10
    unsigned char index2;              // +0x14
    char unknown_15[0x18 - 0x15];
};

struct Player_00496ee0 {
    char unknown_0[0x95];
    unsigned char nameIndex;           // +0x95
    unsigned char index2;              // +0x96
};

struct PlayerRec_00496ee0 {            // 0x14b bytes, array at g_game+0x1b63
    char unknown_0[0x27];
    Player_00496ee0* player;            // +0x27
    char unknown_2b[0xdc - 0x2b];
    float size1;                       // +0xdc
    float size2;                       // +0xe0
    char unknown_e4[0x148 - 0xe4];
    unsigned short bits0 : 8;          // +0x148
    unsigned short started : 1;        // +0x149
    char unknown_14a[0x14b - 0x14a];
};

struct PlayerName_00496ee0 {            // 0x232 bytes, array at g_game+0x37f5f
    char name[0x232];
};

struct Vec3_00437320 {
    int x;
    int y;
    int z;
};

class Class_00437320 {
public:
    int FUN_00437320(Vec3_00437320* out, int id);
};

struct Game {
    char unknown_0[0x1b63];
    PlayerRec_00496ee0 players[10];    // +0x1b63
    char unknown_2851[0x29a0 - 0x2851];
    TeamDef_00496ee0* teams;            // +0x29a0
    char unknown_29a4[0x2a42 - 0x29a4];
    unsigned char localPlayer;          // +0x2a42
    char unknown_2a43[0x37e37 - 0x2a43];
    int viewWidth;                      // +0x37e37
    int viewHeight;                     // +0x37e3b
    char unknown_37e3f[0x37f5f - 0x37e3f];
    PlayerName_00496ee0 names[8];        // +0x37f5f
    char unknown_390ef[0x391e9 - 0x390ef];
    Class_00437320* net;                // +0x391e9
};
#pragma pack(pop)

extern Game* g_game;

unsigned short __stdcall FindUnitTypeId(const char* name);
void __stdcall FatalError(char* message);
void __stdcall FUN_0041c4c0(int x, int y, int instant);

union Fixed_00496ee0 {
    int i;                              // 16.16
    struct {
        short frac;
        short whole;
    } h;
};

struct FixedPos_00496ee0 {
    Fixed_00496ee0 x;
    Fixed_00496ee0 y;
    Fixed_00496ee0 z;
};

void __stdcall CreateUnit(int team, unsigned short id, FixedPos_00496ee0 pos, int a, int b,
    int c);

// FUNCTION: 0x496ee0
void __stdcall FUN_00496ee0(int team, int startpos)
{
    g_game->players[team].player->nameIndex = g_game->teams[team].nameIndex;
    g_game->players[team].player->index2 = g_game->teams[team].index2;
    PlayerRec_00496ee0* p = &g_game->players[team];
    int w = g_game->teams[team].size2;
    int v = g_game->teams[team].size1;
    p->started = 1;
    p->size1 = (float)(w >= 200 ? w : 200);
    p->size2 = (float)(v >= 200 ? v : 200);

    FixedPos_00496ee0 pos;
    if (g_game->net->FUN_00437320((Vec3_00437320*)&pos, startpos)) {
        unsigned short id = FindUnitTypeId(
            g_game->names[g_game->players[team].player->nameIndex].name);
        CreateUnit(team, id, pos, 1, 1, 0);
    } else {
        char buf[128];
        sprintf(buf,
            "Error: Could not find start position number %i on the map!",
            startpos);
        FatalError(buf);
    }

    if (team == g_game->localPlayer)
        FUN_0041c4c0(pos.x.h.whole - g_game->viewWidth / 2,
            pos.z.h.whole - g_game->viewHeight / 2, 0);
}
