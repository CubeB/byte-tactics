// Decompiled by deepseek-v4.1-flash. Names are provisional.
// PARTIAL (49.4%), best found. Sends a chat/text message (type 5, up to 64
// characters) to the players selected by the game's chat mode at +0x2bf0.
// The whole control flow, every offset and constant, and the inlined
// FUN_0044fe00 target search match. The original reloads the `text` argument
// dead into eax at the start of both mode branches, which keeps g_game in ecx;
// without that dead use MSVC keeps g_game in eax and rotates every scratch
// register by one. Tried and ruled out: unused inlined-helper parameters,
// void-cast locals, folded ternaries/commas, text[0] guards, member/__fastcall
// /__stdcall helpers, block-scoped locals, and all 128 header sets. /O2
// eliminates every construct that could produce the reload.
#include <string.h>

#pragma pack(push, 1)
struct Player_00453360 {
    int active;                        // +0x00
    int id;                            // +0x04
    char unknown_8[0x73 - 0x8];
    char state;                        // +0x73
    char unknown_74[0x108 - 0x74];
    unsigned char allied[0x3e];        // +0x108
    char unknown_146[0x14b - 0x146];
};

struct Game_00453360 {
    char unknown_0[0x1b63];
    Player_00453360 players[10];       // +0x1b63
    char unknown_2851[0x2a38 - 0x2851];
    char* buffer;                      // +0x2a38
    char unknown_2a3c[0x2a42 - 0x2a3c];
    unsigned char localPlayer;         // +0x2a42
    char unknown_2a43[0x2bf0 - 0x2a43];
    unsigned char mode;                // +0x2bf0
    unsigned char field_2bf1[10];      // +0x2bf1
};
#pragma pack(pop)

extern Game_00453360* g_game;

int __stdcall FUN_00451df0(int player, void* data, int size);
int __stdcall FUN_00451bc0(int from, int to, void* packet, int size);

// Same body as FUN_0044fe00, inlined here.
static inline int FindTarget()
{
    for (int i = 0; i < 10; i++) {
        if (g_game->players[i].state == 1)
            return g_game->players[i].id;
    }
    return -1;
}

// Best so far (49.4%): the whole control flow, every offset and every constant
// match. What still differs: the original starts each of the two mode branches
// with a dead `mov eax,[esp+0x14]` (a reload of the `text` argument that is
// never read again). That dead use makes MSVC keep g_game in ecx for the whole
// function; without it MSVC keeps g_game in eax, so every scratch register in
// the three loops is rotated by one (i/ecx vs i/eax, dl/edx vs cl/ecx, ...).
// The source construct that produces those two reloads could not be found:
// unused inlined-helper parameters, unused/void-cast locals, folded ternaries
// and comma expressions, text[0] guards, member/__fastcall/__stdcall helpers
// and block-scoped locals all get fully eliminated by /O2.
// FUNCTION: 0x453360
void __stdcall FUN_00453360(char* text)
{
    g_game->buffer[0] = 5;
    strncpy(g_game->buffer + 1, text, 0x40);

    int target = FindTarget();

    if (text[0] == '+' || g_game->mode == 0) {
        FUN_00451df0(target, g_game->buffer, 0x41);
    } else if (g_game->mode == 3) {
        for (int i = 0; i < 10; i++) {
            if (g_game->field_2bf1[i] != 0) {
                int id = g_game->players[i].id;
                if (id != 0)
                    FUN_00451bc0(target, id, g_game->buffer, 0x41);
            }
        }
    } else {
        Player_00453360* lp = &g_game->players[g_game->localPlayer];
        for (int i = 0; i < 10; i++) {
            Player_00453360* p = &g_game->players[i];
            if (p->active != 0 && p->state == 3) {
                if ((g_game->mode == 1 && lp->allied[i] != 0) ||
                    (g_game->mode == 2 && lp->allied[i] == 0))
                    FUN_00451bc0(target, p->id, g_game->buffer, 0x41);
            }
        }
    }
}
