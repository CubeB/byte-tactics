// Decompiled by space-bunny-free. Names are provisional.
// Starts a player: clears the five 11-byte per-team tables, records the side,
// the alliance and the player index, then builds the player's short name
// ("Player", "Core" or "Arm") for a campaign and for a skirmish game.
//
// Two spellings here are chosen for the register allocator, not for style:
// the local `idx` (declared after the player pointer) puts the pointer in the
// addressing-mode index slot, which is what the original's
// `mov byte [eax+ebp+0x113], 1` wants, and writing the side test as
// `side == 0` puts the "Arm" arm inline and jumps to the "Core" arm, which
// is the original's block layout. The second name block really does pass the
// raw "Core" and "Arm" strings while the first wraps them in Translate.

#include <stdio.h>
#include <string.h>

class Class_00435100 {
public:
    int FUN_00435100();
};

// The object at g_game + 0x0c. Only the field at +0x620 is read here.
struct Map_00464290 {
    char unknown_0[0x620];
    int field_620;                    // +0x620
};

#pragma pack(push, 1)
struct PlayerInfo_00464290 {
    char unknown_0[0x94];
    unsigned char field_94;           // +0x94
    unsigned char side;               // +0x95
    char unknown_96[0x99 - 0x96];
    unsigned short field_99;          // +0x99
    unsigned short bits_9b : 5;       // +0x9b, bits 0 to 4
    unsigned short bit5 : 1;
    unsigned short bit6 : 1;
    unsigned short watching : 1;
    unsigned short mapping : 1;
    unsigned short bit9 : 1;
    unsigned short bit10 : 1;
    unsigned short commander : 2;
    unsigned short cheating : 1;
    unsigned short fixedloc : 1;
    unsigned short closed : 1;
};

struct Player_00464290 {
    int active;                       // +0x00
    int index;                        // +0x04
    char unknown_8[0xc - 8];
    int field_c;                      // +0x0c
    char unknown_10[0x22 - 0x10];
    unsigned char field_22;           // +0x22
    char unknown_23[0x27 - 0x23];
    PlayerInfo_00464290* info;        // +0x27
    char name[30];                    // +0x2b
    char fullName[0x73 - 0x49];       // +0x49
    unsigned char type;               // +0x73
    int field_74;                     // +0x74
    char unknown_78[0x108 - 0x78];
    unsigned char team_108[11];       // +0x108
    unsigned char team_113[11];       // +0x113
    unsigned char team_11e[11];       // +0x11e
    unsigned char team_129[11];       // +0x129
    unsigned char team_134[11];       // +0x134
    unsigned char alliance;           // +0x13f
    char unknown_140[0x146 - 0x140];
    unsigned char field_146;          // +0x146
    unsigned char field_147;          // +0x147
    unsigned char field_148;          // +0x148
    char unknown_149[0x14b - 0x149];
};

struct Game {
    char unknown_0[0xc];
    Map_00464290* map;                // +0x0c
    char unknown_10[0x1b63 - 0x10];
    Player_00464290 players[10];      // +0x1b63
    char unknown_2851[0x391e9 - 0x2851];
    Class_00435100* campaign;         // +0x391e9
};
#pragma pack(pop)

extern Game* g_game;

char* __stdcall Translate(char* text);

// FUNCTION: 0x464290
void __stdcall FUN_00464290(int player, char type)
{
    Player_00464290* p = &g_game->players[player & 0xff];
    int idx = player & 0xff;

    memset(p->team_108, 0, 11);
    memset(p->team_113, 0, 11);
    memset(p->team_11e, 0, 11);
    memset(p->team_129, 0, 11);
    memset(p->team_134, 0, 11);
    int t = type;
    p->type = t;
    if (t != 3) {
        p->info->field_94 = t;
    }
    p->team_113[idx] = 1;
    p->field_74 = 0;
    p->field_146 = (char)player;
    p->field_148 = (char)player;
    p->team_108[idx] = 1;
    p->active = 1;
    p->field_147 = (char)player;
    p->alliance = 5;
    p->info->bit5 = 0;
    p->index = player & 0xff;
    p->field_c = 0;
    p->field_22 = 0;

    if (p->active != 0 && (p->type == 1 || p->type == 2)) {
        p->info->field_99 = g_game->map->field_620 / 0x100000 + 1;
    }

    if (g_game->campaign->FUN_00435100() == 1) {
        if (type == 1) {
            sprintf(p->name, Translate("Player"));
        } else if (type == 2) {
            if (p->info->side == 0) {
                sprintf(p->name, "Arm");
            } else {
                sprintf(p->name, "Core");
            }
        }
        strcpy(p->fullName, p->name);
    }

    if (g_game->campaign->FUN_00435100() == 2) {
        if (type == 1) {
            sprintf(p->name, Translate("Player"));
        } else if (type == 2) {
            if (p->info->side == 0) {
                sprintf(p->name, Translate("Arm"));
            } else {
                sprintf(p->name, Translate("Core"));
            }
        }
        strcpy(p->fullName, p->name);
    }
}
