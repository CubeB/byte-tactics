// Decompiled by Space Bunny Free. Names are provisional.
// Draws a kill count ("N kill(s)", plus " - Veteran" past level 4) in the
// current text colour. The singular/plural ternary sits in both arms, so the
// > 4 arm keeps a test the level can never satisfy.
#include <stdio.h>

struct Player_00467cb0 {
    char unknown_0[0xb8];
    unsigned short kills;              // +0xb8
};

struct Game {
    char unknown_0[0xdcb];
    unsigned char colors[16];          // +0xdcb
};

extern Game* g_game;

char* __stdcall FUN_004c5740(char* text);
int GetTextKeyColor();
void __stdcall SetTextColors(int color, int font);
void __stdcall DrawString(void* surface, const char* text, int x, int y, int maxWidth);

// FUNCTION: 0x467cb0
void __stdcall DrawKillCount(void* surface, Player_00467cb0* player, int x, int y)
{
    char buf[100];
    char* kills = FUN_004c5740("kills");
    char* kill = FUN_004c5740("kill");
    if (player->kills > 4) {
        sprintf(buf, "%d %s - %s", player->kills,
                player->kills == 1 ? kill : kills, FUN_004c5740("Veteran"));
    } else {
        sprintf(buf, "%d %s", player->kills,
                player->kills == 1 ? kill : kills);
    }
    SetTextColors(g_game->colors[15], GetTextKeyColor());
    DrawString(surface, buf, x, y, -1);
}
