// Decompiled by space-bunny-free. Names are provisional.
// Resets the game for a new battle. The first loop clears the eleven player
// slots (0x14b bytes each, at g_game + 0x1b63) while keeping each slot's info
// pointer, which is read before the clear and written back to the same address
// (MSVC reaches that address as g_game + 0x1a3f + (i+1) * 0x14b, one slot
// below the players array, hence the infos[] member kept for the addressing).
// The second loop then gives every slot its default "Player %d First" /
// "Player %d Second" names and its default state.
//
// NOT MATCHING yet: 94.5% (16 diff lines, all in the tail of the second
// loop). Two scheduling differences remain, both about where MSVC puts the
// two "reload p->info" loads and the byte clear:
//   1. The original loads p->info for info->field_94 *after* `mov byte
//      [ebp-0xa0], 0` and stores info->field_94 after `add ebp, 0x14b`, so
//      its second reload uses the post-increment form [ebp-0x237]. Here the
//      first reload is hoisted to the top of the region and both the store
//      and the second reload land before the increment, so that reload uses
//      [ebp-0xec]. The statement order is confirmed right (reversing the
//      info group makes it worse), so this is MSVC 5's own scheduling of the
//      loop increment; moving the increment, the call cleanup, hoisting the
//      load through a local, an inline helper, a chained zero assignment and
//      a reversed statement order all failed to move it.
//   2. The original loads p->field_21 into `cl` just after the inlined
//      strcpy and keeps `and cl, 0xfe` next to the store; here the load is
//      scheduled after the nine zero stores and takes `al` (whose eax is
//      otherwise the zero register). The `&=` spelling does not matter:
//      `= x & 0xfe`, a cast, a temp local and an inline helper all compile
//      identically. (Note: a mask written `0xfffe >> 8` is 0xff, not 0xfe, and
//      MSVC then deletes the statement as a no-op, which fakes a higher
//      score while losing three instructions.)
// The walked player pointer, the `i++, p++` in the for clause and the order
// of the nine zero stores are all load-bearing: each change costs 3 to 6
// points, so the statement order in the loop body is believed to be right.

#include <stdio.h>
#include <string.h>

#pragma pack(push, 1)
struct PlayerInfo_004644d0 {
    char name[0x94];                   // +0x00
    unsigned char field_94;            // +0x94
    unsigned char side;                // +0x95
    unsigned char field_96;            // +0x96
    unsigned short bit_97 : 1;         // +0x97
    unsigned short rest_97 : 15;
    unsigned short field_99;           // +0x99
    unsigned short bit0 : 1;           // +0x9b
    unsigned short bit1 : 1;
    unsigned short bit2 : 1;
    unsigned short bit3 : 1;
    unsigned short bit4 : 1;
    unsigned short bit5 : 1;
    char unknown_9d[0xb9 - 0x9d];
};

struct Player_004644d0 {
    int active;                        // +0x00
    int field_4;                       // +0x04
    char unknown_8[0xc - 8];
    int field_c;                       // +0x0c
    char unknown_10[0x18 - 0x10];
    int field_18;                      // +0x18
    char unknown_1c[0x21 - 0x1c];
    unsigned char field_21;            // +0x21
    char unknown_22[0x27 - 0x22];
    PlayerInfo_004644d0* info;         // +0x27
    char name[0x49 - 0x2b];            // +0x2b
    char fullName[0x67 - 0x49];        // +0x49
    int field_67;                      // +0x67
    int field_6b;                      // +0x6b
    unsigned short field_6f;           // +0x6f
    unsigned short field_71;           // +0x71
    unsigned char type;                // +0x73
    int field_74;                      // +0x74
    char unknown_78[0x108 - 0x78];
    unsigned char team_108[11];        // +0x108
    unsigned char team_113[11];        // +0x113
    char unknown_11e[0x13f - 0x11e];
    unsigned char alliance;            // +0x13f
    int field_140;                     // +0x140
    unsigned short field_144;          // +0x144
    unsigned char field_146;           // +0x146
    char unknown_147[0x14b - 0x147];
};

struct Game_004644d0 {
    char unknown_0[0x1a3f];
    PlayerInfo_004644d0* infos[11];    // +0x1a3f (addressing only, see above)
    char unknown_1a6b[0x1b63 - 0x1a6b];
    Player_004644d0 players[11];        // +0x1b63
    char unknown_299c[0x2a3e - 0x299c];
    unsigned short field_2a3e;         // +0x2a3e
    unsigned short field_2a40;         // +0x2a40
    unsigned char field_2a42;          // +0x2a42
    unsigned char field_2a43;          // +0x2a43
    char unknown_2a44[0x2c28 - 0x2a44];
    int table_2c28[11];                // +0x2c28
};
#pragma pack(pop)

extern Game_004644d0* g_game;
extern char DAT_005119b8[];

// FUNCTION: 0x4644d0
void FUN_004644d0()
{
    Player_004644d0* p;
    int i;

    for (i = 0; i <= 10; i++) {
        PlayerInfo_004644d0* t = g_game->players[i].info;
        memset(&g_game->players[i], 0, 0x14b);
        g_game->players[i].info = t;
    }

    g_game->field_2a42 = 0;
    g_game->field_2a43 = 0;
    memset(g_game->table_2c28, 0, 0x2c);

    p = &g_game->players[0];
    for (i = 0; i <= 10; i++, p++) {
        memset(p->team_108, 0, 11);
        memset(p->team_113, 0, 11);
        memset(p->info, 0, 0xb9);
        sprintf(p->name, "Player %d First", i);
        sprintf(p->fullName, "Player %d Second", i);
        p->team_108[i] = 1;
        p->team_113[i] = 1;
        p->active = 0;
        p->type = 0;
        p->info->field_94 = 0;
        p->info->field_99 = 0;
        p->info->bit4 = 0;
        p->info->bit_97 = 0;
        p->info->field_96 = (char)i;
        p->info->side = 0;
        strcpy(p->info->name, DAT_005119b8);
        p->field_74 = 0;
        p->field_c = 0;
        p->field_18 = 0;
        p->field_67 = 0;
        p->field_6b = 0;
        p->field_6f = 0;
        p->field_71 = 0;
        p->field_144 = 0;
        p->field_140 = 0;
        p->field_21 &= 0xfe;
        p->field_4 = -1;
        p->field_146 = 10;
        p->alliance = 5;
        p->info->bit5 = 0;
    }

    g_game->field_2a3e = 0;
    g_game->field_2a40 = 0;
}
