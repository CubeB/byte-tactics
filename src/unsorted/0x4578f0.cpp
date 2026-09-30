// Decompiled by space-bunny-free. Names are provisional.
// Leaves the game: if the "leaving game" flag at +0x2a44 is set, drops every
// connected human or computer player (type 1 or 2) with FUN_00452cc0 and
// calls FUN_0046c190. Then closes the network, installs a null callback, sets
// the game flag at +0x3923b and quits with the text for the local player's
// rejection reason (+0x22), the same text FUN_00452c40 returns, and no text at
// all when the reason is 0.
//
// `#include <string.h>` is load bearing here even though nothing in this file
// calls a string function. Without it the three player-loop reads of
// g_game->players[i] come out as [esi + ecx + disp] (SIB 0x0e) where the
// original has [ecx + esi + disp] (SIB 0x31, g_game as base and the byte offset
// of the induction variable as index), leaving the function 3 bytes out.
// Everything else, including the block order, the jump table and the
// `mov eax, <text>; push eax` per case, already matched.
//
// The loop below is character for character the loop of the matched sibling
// 0x451b60, and the original disassembly of the two is the same loop, which is
// what pointed at the header: 0x451b60 carries `<string.h>` and this file did
// not. The include is here only for the compiler state it sets up. 0x4573d0
// needed the same include for the same reason, see the note in that file.

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

struct Game_004578f0 {
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

extern Game_004578f0* g_game;

void __stdcall FUN_00452cc0(int dpid);
void FUN_0046c190();
int __stdcall FUN_004c9f90(Net_4c9f90* net);
void __stdcall FUN_004b4fd0(void (__cdecl *callback)(int), int param);
void __stdcall FUN_004b6230(char* message);

// FUNCTION: 0x4578f0
void __cdecl FUN_004578f0()
{
    if (g_game->leaving) {
        for (int i = 0; i < 10; i++) {
            if (g_game->players[i].active != 0
                && (g_game->players[i].type == 1 || g_game->players[i].type == 2))
                FUN_00452cc0(g_game->players[i].dpid);
        }
        FUN_0046c190();
    }
    FUN_004c9f90(&g_game->net);
    FUN_004b4fd0(0, 0);
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
    FUN_004b6230(text);
}
