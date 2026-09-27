// Decompiled by space-bunny-free. Names are provisional.
//
// Two halves, keyed on the message's own flag at +0x05. When it is clear this
// is the first sighting: stamp the message with the current tick count, send
// it to the player id in the header (the first argument is the local lobby
// dpid from g_game + 0x4cd), and give the packets back to the network layer.
// When it is already set the message is a later update: find the sending
// player in the table, and if they are an active client (state 1 or 2) record
// the round trip into the slot of the player in g_game + 0x4c9.
//
// The two id searches are the inlined FindPlayerIndex helper, each with its
// own early `return 10` for the -1 case, which is what produces the two
// separate stores of 10. Writing the table lookups through a pointer local
// (`pl`) is what makes the compiler materialise &players[index] in a register
// while re-deriving the same address for the first field load.
#include <windows.h>

#pragma pack(push, 1)

struct Player_004565a0 {
    int active;                        // +0x00 (g_game + 0x1b63)
    int id;                            // +0x04 (g_game + 0x1b67)
    char unknown_8[0x14 - 8];
    int field_14;                      // +0x14 (g_game + 0x1b77)
    char unknown_18[0x73 - 0x18];
    char state;                        // +0x73 (g_game + 0x1bd6)
    char unknown_74[0x14b - 0x74];
};

struct Game_004565a0 {
    char unknown_0[0x4c9];
    int lobby2;                        // +0x4c9
    int lobby1;                        // +0x4cd
    char unknown_4d1[0x1b63 - 0x4d1];
    Player_004565a0 players[10];       // +0x1b63
};

struct Message_004565a0 {
    char unknown_0[1];
    int start_tick;                    // +0x01
    int sent_tick;                     // +0x05
    int id;                            // +0x09
};

#pragma pack(pop)

class Class_004618a0 {
public:
    int FUN_004618a0(int param_1);
};

extern Game_004565a0* g_game;
extern int DAT_00506dbc;
extern Class_004618a0 DAT_00513000;

int __stdcall FUN_00451bc0(int from, int to, void* packet, int size);
int __stdcall FUN_004c9790(int param_1);

static inline int GetPlayerId(unsigned char i)
{
    if (i != 10 && g_game->players[i].state)
        return g_game->players[i].id;
    return -1;
}

static inline unsigned char FindPlayerIndex(int id)
{
    if (id == -1)
        return 10;
    for (unsigned char i = 0; i < 10; i++) {
        if (GetPlayerId(i) == id)
            return i;
    }
    return 10;
}

// FUNCTION: 0x4565a0
void __stdcall FUN_004565a0(Message_004565a0* p)
{
    if (p->sent_tick == 0) {                       // not sent yet
        if (DAT_00506dbc != 0) {
            DAT_00513000.FUN_004618a0(1);
        }
        p->sent_tick = GetTickCount();
        int packets_were_guaranteed = FUN_004c9790(0);
        FUN_00451bc0(g_game->lobby1, p->id, p, 0xd);
        if (DAT_00506dbc != 0) {
            DAT_00513000.FUN_004618a0(1);
        }
        if (packets_were_guaranteed != 0) {          // hand them back
            FUN_004c9790(1);
        }
        return;
    }

    unsigned char index = FindPlayerIndex(p->id);
    Player_004565a0* pl = &g_game->players[index];
    if (pl->active == 0) {
        return;
    }
    if (pl->state != 1 && pl->state != 2) {
        return;
    }
    unsigned char other = FindPlayerIndex(g_game->lobby2);
    // No bound check on either index: an id of -1 makes FindPlayerIndex
    // return 10, so players[10] is one entry past the ten player table.
    g_game->players[other].field_14 = GetTickCount() - p->start_tick;
}
