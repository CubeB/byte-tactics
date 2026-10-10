// Decompiled by deepseek-v4.1-flash, edited by deepseek-v4.1, finished by GPT-6.1-sol, finished by mimo-v2.6-pro, finished by Claude Opus 5.5. Names are provisional.
// Sends a chat/text message (type 5, up to 64 characters) to the players
// selected by the game's chat mode at +0x2bf0, from the first player in state
// 1 (GetLocalHumanDpid inlined), and returns the last send's result.
#include <string.h>

#include "player.h"

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x1b63];
    Player players[10];                // +0x1b63
    char unknown_2851[0x2a38 - 0x2851];
    char* recvPacketPtr;               // +0x2a38
    char unknown_2a3c[0x2a42 - 0x2a3c];
    unsigned char localPlayer;         // +0x2a42
    char unknown_2a43[0x2bf0 - 0x2a43];
    unsigned char chatMode;            // +0x2bf0
    unsigned char chatRecipients[10];  // +0x2bf1
};
#pragma pack(pop)

int __stdcall BroadcastPacket(int player, void* data, int size);
int __stdcall SendPacketToPlayer(int from, int to, void* packet, int size);

// The body of GetLocalHumanDpid, inlined here.
static inline int FindTarget(Game* game)
{
    for (int i = 0; i < 10; i++) {
        if (game->players[i].type == 1)
            return game->players[i].id;
    }
    return -1;
}

// Stays in its own file: the 0x2bf1 read's SIB base/index order needs the
// block-scope g_game declaration below to be the first one; the gathered file
// declares g_game at file scope.
// FUNCTION: 0x453360
int __stdcall SendChatPacket(char* text)
{
    // Never initialised: a path with no send returns garbage, as in the
    // original (the caller ignores it).
    int result;
    int i;
    // Function-scope and declared after i: symbol order sets the SIB operand order.
    extern Game* g_game;
    g_game->recvPacketPtr[0] = 5;
    strncpy(g_game->recvPacketPtr + 1, text, 0x40);

    int target = FindTarget(g_game);

    if (text[0] == '+' || g_game->chatMode == 0) {
        result = BroadcastPacket(target, g_game->recvPacketPtr, 0x41);
    } else if (g_game->chatMode == 3) {
        for (i = 0; i < 10; i++) {
            if (g_game->chatRecipients[i] != 0) {
                int id = g_game->players[i].id;
                if (id != 0)
                    result = SendPacketToPlayer(target, id, g_game->recvPacketPtr, 0x41);
            }
        }
    } else {
        Player* lp = &g_game->players[g_game->localPlayer];
        for (int i = 0; i < 10; i++) {
            Player* p = &g_game->players[i];
            if (p->active != 0 && p->type == 3) {
                if ((g_game->chatMode == 1 && lp->allied[i] != 0) ||
                    (g_game->chatMode == 2 && lp->allied[i] == 0))
                    result = SendPacketToPlayer(target, p->id, g_game->recvPacketPtr, 0x41);
            }
        }
    }
    return result;
}
