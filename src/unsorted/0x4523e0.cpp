// Decompiled by deepseek-v4.1-flash. Names are provisional.
//
// PARTIAL: every control-flow and data detail below matches the original
// (the free-group search, the clamp, the 2-byte message built at frame +2/+3,
// the local-player early return, the send and the id search), but MSVC 5
// keeps the `to` parameter in a register (ebp) where the original keeps it in
// its argument slot [esp+0x1c] and puts the outer loop counter `i` in ebp
// instead. As a result `i` is spilled to the packet slot and the first loop,
// the early return and the id search use different registers. Writing the
// parameter as `volatile int to` reproduces the original's register
// allocation exactly (volatile is not allowed here, and it also re-reads `to`
// in the second half, which the original does not). Something in Cavedog's
// source made `to` a non-candidate for parameter homing; tried and ruled out:
// loop forms (for/while/do/goto), condition orders, inline scan helpers, an
// inline helper for the whole first loop, a local copy of `to`, a one-int
// struct by value, a switch on `to`, const, unsigned, and unused prototypes.

#pragma pack(push, 1)
struct PlayerData_004523e0 {
    char unknown_0[0x96];
    unsigned char group;               // +0x96
};

struct Player_004523e0 {
    int active;                        // +0x0
    int id;                            // +0x4
    char unknown_8[0x27 - 8];
    PlayerData_004523e0* data;         // +0x27
    char unknown_2b[0x73 - 0x2b];
    char state;                        // +0x73
    char unknown_74[0x14b - 0x74];
};

struct Game_004523e0 {
    char unknown_0[0x1b63];
    Player_004523e0 players[11];       // +0x1b63
    char unknown_1[0x2a38 - (0x1b63 + 0x14b * 11)];
    unsigned char* buffer;             // +0x2a38
    char unknown_2[0x2a42 - 0x2a38 - 4];
    unsigned char localPlayer;         // +0x2a42
};
#pragma pack(pop)

class Class_004618a0 {
public:
    int FUN_004618a0(int param_1);
};

extern Game_004523e0* g_game;
extern int DAT_00506dbc;
extern Class_004618a0 DAT_00513000;

int __stdcall FUN_00451bc0(int from, int to, void* packet, int size);

static inline int PlayerId(unsigned char index)
{
    if (index != 10 && g_game->players[index].state != 0)
        return g_game->players[index].id;
    return -1;
}

// FUNCTION: 0x4523e0
int __stdcall FUN_004523e0(int from, int to, int group)
{
    unsigned char packet[4];
    for (int i = 0; i < 10; i++, group++) {
        if (group >= 10)
            group = 0;
        int j;
        for (j = 0; j < 10; j++) {
            Player_004523e0* p = &g_game->players[j];
            if (p->state != 0 && p->state != 4 && p->id != to && p->data->group == group)
                break;
        }
        if (j == 10) {
            packet[3] = group;
            break;
        }
    }
    if (to == g_game->players[g_game->localPlayer].id) {
        g_game->players[g_game->localPlayer].data->group = group;
        return 1;
    }
    packet[2] = 0x18;
    int result = FUN_00451bc0(from, to, packet + 2, 2);
    if (result != 0 && DAT_00506dbc != 0) {
        unsigned char idx;
        if (to == -1) {
            idx = 10;
        } else {
            for (idx = 0; idx < 10; idx++) {
                if (PlayerId(idx) == to)
                    break;
            }
        }
        g_game->players[idx].data->group = group;
        DAT_00513000.FUN_004618a0(1);
    }
    return result;
}
