// Decompiled by deepseek-v4.1-flash, edited by deepseek-v4.1, finished by GPT-6.1-sol, finished by mimo-v2.6-pro, finished by Claude Opus 5.5. Names are provisional.
// Sends a chat/text message (type 5, up to 64 characters) to the players
// selected by the game's chat mode at +0x2bf0, from the first player in state
// 1 (GetLocalHumanDpid inlined), and returns the last send's result.
//
// What earlier passes were missing (54.4%):
// - The function returns an int: `result` takes each send's return value and
//   is never initialised, so a path with no send returns whatever is in its
//   home. MSVC homes it in the dead `text` argument slot, which is the bare
//   `mov eax, [esp+0x14]` at the top of the mode-3 and default arms that no
//   dead-code spelling could produce (an original bug: the caller 0x463e50
//   ignores the value, so it is harmless). That alone gives 99.2%.
// - The last byte is the SIB order of `g_game->field_2bf1[i]`
//   (`[ecx + edi + 0x2bf1]`): the operand with the larger symbol id takes the
//   base slot, so g_game must be numbered after the mode-3 loop's `i`. That is
//   the function-scope `extern` declared after `i` (as in 0x493bf0), with no
//   file-scope declaration before it, so the inlined target search takes the
//   game pointer as a parameter. Sharing `i` with the default arm's loop too
//   gives 98.3%.
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

struct Game {
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

int __stdcall BroadcastPacket(int player, void* data, int size);
int __stdcall SendPacketToPlayer(int from, int to, void* packet, int size);

// The body of GetLocalHumanDpid, inlined here.
static inline int FindTarget(Game* game)
{
    for (int i = 0; i < 10; i++) {
        if (game->players[i].state == 1)
            return game->players[i].id;
    }
    return -1;
}

// FUNCTION: 0x453360
int __stdcall SendChatPacket(char* text)
{
    int result;
    int i;
    extern Game* g_game;
    g_game->buffer[0] = 5;
    strncpy(g_game->buffer + 1, text, 0x40);

    int target = FindTarget(g_game);

    if (text[0] == '+' || g_game->mode == 0) {
        result = BroadcastPacket(target, g_game->buffer, 0x41);
    } else if (g_game->mode == 3) {
        for (i = 0; i < 10; i++) {
            if (g_game->field_2bf1[i] != 0) {
                int id = g_game->players[i].id;
                if (id != 0)
                    result = SendPacketToPlayer(target, id, g_game->buffer, 0x41);
            }
        }
    } else {
        Player_00453360* lp = &g_game->players[g_game->localPlayer];
        for (int i = 0; i < 10; i++) {
            Player_00453360* p = &g_game->players[i];
            if (p->active != 0 && p->state == 3) {
                if ((g_game->mode == 1 && lp->allied[i] != 0) ||
                    (g_game->mode == 2 && lp->allied[i] == 0))
                    result = SendPacketToPlayer(target, p->id, g_game->buffer, 0x41);
            }
        }
    }
    return result;
}
