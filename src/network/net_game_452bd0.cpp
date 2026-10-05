// Decompiled by Opus. Names are provisional.
// Sends a 6-byte message (type 0x24) to a player that is active in state 1
// or 2.

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x2a38];
    unsigned char* buffer;             // +0x2a38
};

struct Player_00452bd0 {
    int active;                        // +0x0
    int id;                            // +0x4
    char unknown_8[0x73 - 8];
    char state;                        // +0x73
    char unknown_74[0x13f - 0x74];
    char field_13f;                    // +0x13f
};
#pragma pack(pop)

class Class_004618a0 {
public:
    int FUN_004618a0(int param_1);
};

extern Game* g_game;
extern int DAT_00506dbc;
extern Class_004618a0 DAT_00513000;

int __stdcall FUN_00451df0(int player, void* data, int size);

// An inlined helper with one return per outcome: written as a plain
// condition, MSVC moves the `return 0` path after the body.
static inline int IsPlaying(Player_00452bd0* player)
{
    if (player->active == 0)
        return 0;
    if (player->state == 1 || player->state == 2)
        return 1;
    return 0;
}

// FUNCTION: 0x452bd0
int __stdcall FUN_00452bd0(Player_00452bd0* player)
{
    if (!IsPlaying(player)) {
        return 0;
    }
    unsigned char* msg = g_game->buffer;
    msg[0] = 0x24;
    *(int*)(msg + 1) = player->id;
    msg[5] = player->field_13f;
    int result = FUN_00451df0(player->id, msg, 6);
    if (DAT_00506dbc != 0) {
        DAT_00513000.FUN_004618a0(1);
    }
    return result;
}
