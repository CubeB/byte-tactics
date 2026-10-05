// Decompiled by Space Bunny Free. Names are provisional.
#include <windows.h>

#pragma pack(push, 1)
struct Player_00479660 {
    int active;                        // +0x0
    int unknown_4;                     // +0x4
    int team;                          // +0x8
    char unknown_c[0x18 - 0xc];
};

struct Layout_00479660 {
    char unknown_0[0xc6];
    short alliance;                    // +0xc6
    char unknown_c8[0x15b - 0xc8];
};

struct Table_00479660 {
    int unknown_0;
    Layout_00479660* entries;          // +0x4
};

struct Menu_00479660 {
    char unknown_0[0x18];
};

struct Game_00479660 {
    char unknown_0[0x519];
    Menu_00479660 menu;                // +0x519
    Table_00479660* table;             // +0x531
    char unknown_535[0x29a0 - 0x535];
    Player_00479660* players;          // +0x29a0
    char unknown_29a4[0x38d81 - 0x29a4];
    int playerCount;                   // +0x38d81
};
#pragma pack(pop)

extern Game_00479660* g_game;

int __stdcall FUN_0049fdf0(Layout_00479660* entries, const char* name, int flag);
void __stdcall FUN_0049fa90(Menu_00479660* menu);

// FUNCTION: 0x479660
void FUN_00479660(void)
{
    int i = 0;
    Layout_00479660* entries = g_game->table->entries;
    for (; i < g_game->playerCount; i++) {
        char name[64];
        wsprintfA(name, "Allies%d", i);
        int index = FUN_0049fdf0(entries, name, 6);
        if (index != -1) {
            Layout_00479660* gadget = &entries[index];
            if (gadget != 0) {
                int team = g_game->players[i].team;
                int count = 0;
                int j;
                for (j = 0; j < g_game->playerCount; j++) {
                    if (g_game->players[j].team == team &&
                        g_game->players[j].active != 0) {
                        count++;
                    }
                }
                switch (count) {
                case 0:
                    gadget->alliance = 10;
                    break;
                case 1:
                    gadget->alliance = team * 2 + 1;
                    break;
                default:
                    gadget->alliance = team * 2;
                    break;
                }
            }
        }
    }
    FUN_0049fa90(&g_game->menu);
}
