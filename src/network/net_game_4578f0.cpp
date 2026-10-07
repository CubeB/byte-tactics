// Decompiled by space-bunny-free. Names are provisional.
// Leaves the game: if the "leaving game" flag at +0x2a44 is set, drops every
// connected human or computer player (type 1 or 2) with RemovePlayer and
// calls ShutdownScoreTables. Then closes the network, installs a null callback, sets
// the game flag at +0x3923b and quits with the text for the local player's
// rejection reason (+0x22), the same text GetRejectReasonText returns, and no text at
// all when the reason is 0.

// <string.h> must stay although unused: it sets the index order of the
// players[i] reads.
#include <string.h>

#pragma pack(push, 1)
struct Player_004578f0 {
    int active;                        // +0x00
    int dpid;                          // +0x04
    char unknown_8[0x22 - 0x8];
    unsigned char rejectReason;        // +0x22
    char unknown_23[0x73 - 0x23];
    unsigned char type;                // +0x73
    char unknown_74[0x14b - 0x74];
};

struct Net_4c9f90 {
    char unknown_0[0x4c9];
};

struct Game {
    char unknown_0[0x14];
    Net_4c9f90 net;                    // +0x14
    char unknown_4dd[0x1b63 - 0x4dd];
    Player_004578f0 players[10];       // +0x1b63
    char unknown_2851[0x2a42 - 0x2851];
    unsigned char localPlayer;         // +0x2a42
    char unknown_2a43;
    unsigned char leaving : 1;         // +0x2a44
    unsigned char unknown_2a44b : 7;
    char unknown_2a45[0x3923b - 0x2a45];
    unsigned short bits_3923b : 2;     // +0x3923b
    unsigned short flag2 : 1;
    unsigned short bit3 : 1;
    unsigned short flag4 : 1;
    unsigned short flag5 : 1;
    unsigned short flag6 : 1;
    unsigned short rest : 9;
};
#pragma pack(pop)

extern Game* g_game;

void __stdcall RemovePlayer(int dpid);
void ShutdownScoreTables();
int __stdcall HAPINET_quitgame(Net_4c9f90* net);
void __stdcall SetCloseHandler(void (__cdecl *callback)(int), int param);
void __stdcall QuitApp(char* message);

// FUNCTION: 0x4578f0
void __cdecl LeaveNetGameCallback(int)
{
    if (g_game->leaving) {
        for (int i = 0; i < 10; i++) {
            if (g_game->players[i].active != 0
                && (g_game->players[i].type == 1 || g_game->players[i].type == 2))
                RemovePlayer(g_game->players[i].dpid);
        }
        ShutdownScoreTables();
    }
    HAPINET_quitgame(&g_game->net);
    SetCloseHandler(0, 0);
    g_game->flag2 = 1;
    int reason = g_game->players[g_game->localPlayer].rejectReason;
    char* text;
    if (reason) {
        switch (reason) {
        case 4:
            text = "You did not have the correct password";
            break;
        case 3:
            text = "The game is closed";
            break;
        case 5:
            text = "The game is full";
            break;
        case 6:
            text = "You have lost connection with the game";
            break;
        case 7:
            text = "You need a unit you don't have for this game";
            break;
        case 8:
            text = "You need a newer version of the game to enter";
            break;
        case 9:
            text = "No watching is allowed for this game";
            break;
        case 10:
            text = "The creator has left the game";
            break;
        default:
            text = "You were rejected from the game";
            break;
        }
    }
    else {
        text = 0;
    }
    QuitApp(text);
}
