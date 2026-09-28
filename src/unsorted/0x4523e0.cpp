// Decompiled by DeepSeek V4.1 Flash, finished by Claude Opus 5.5. Names are provisional.
//
// Gives player `to` the first colour/team group, starting from `group` and
// wrapping at 10, that no other in-use player (state not 0 or 4) holds. The
// local player is updated directly; anyone else is sent packet 0x18 with the
// group, and the group is recorded locally once the send succeeds.
//
// What made it match: the id search in the second half is an inlined
// FindPlayerIndex with its own early `return 10` for id -1 (the two
// `return 10`s give the two separate stores of 10), used straight as the
// index. Comparing `to` inside the search helper instead of in this function
// is what lets `to` stay in its argument slot during the first loop and
// frees ebp for the outer counter.
//
// Suspected original bug: when `to` is -1, FindPlayerIndex returns 10 and the
// last store goes through players[10].data, the spare eleventh slot of the
// table (a pointer read from g_game+0x2878). Nothing guards the index here,
// unlike the callers of the same search in 0x44fed0 and 0x452800.
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
    Player_004523e0 players[10];       // +0x1b63
    char unknown_1[0x2a38 - (0x1b63 + 0x14b * 10)];
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
        g_game->players[FindPlayerIndex(to)].data->group = group;
        DAT_00513000.FUN_004618a0(1);
    }
    return result;
}
