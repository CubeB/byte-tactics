// Decompiled by space-bunny-free. Names are provisional.
// Moves `amount` metal from player `from` to player `to`, the metal twin of
// 0x464c60 (which moves energy). It is clamped to what `from` has stored,
// taken out of `from`'s economy object (only when `flag` is set) and added to
// `to`'s economy object. An AI player (type 2) on easy or medium only counts
// 0.5 or 0.7 of it, and the transfer is announced over the network when `flag`
// is set. Same shape as 0x464c60, which moves energy with UnitResources.

class UnitResources;

#pragma pack(push, 1)
struct Player_00464b30 {
    int active;                        // +0x00
    char unknown_4[0x73 - 0x4];
    unsigned char type;                // +0x73
    char unknown_74[0x8c - 0x74];
    float metal;                       // +0x8c
    char unknown_90[0xec - 0x90];
    UnitResources* econ;               // +0xec
    char unknown_f0[0x14b - 0xf0];
};

struct Game {
    char unknown_0[0x1b63];
    Player_00464b30 players[10];        // +0x1b63
    char unknown_2851[0x37eee - 0x2851];
    int difficulty;                    // +0x37eee
};
#pragma pack(pop)

class UnitResources {
public:
    float metal;                       // +0x0
    char unknown_4[0x30 - 0x4];
    Player_00464b30* player;           // +0x30

    int SpendEnergy(float amount);
};

extern Game* g_game;

void __stdcall SendShareMetal(unsigned char from, unsigned char to, int value);

// FUNCTION: 0x464b30
void __stdcall TransferMetal(unsigned char from, unsigned char to, float amount, int flag)
{
    if (from == 10)
        return;
    if (to == 10)
        return;
    if (flag) {
        float cap = g_game->players[from].metal;
        if (amount > cap)
            amount = cap;
    }
    if (amount == 0.0f)
        return;
    Player_00464b30* player;
    // Receiver read inside both arms of the flag test: homes `to` in ebx.
    player = flag ? (g_game->players[from].econ->SpendEnergy(amount),
                     g_game->players[to].econ->player)
                  : g_game->players[to].econ->player;
    if (player->active != 0 && player->type == 2) {
        switch (g_game->difficulty) {
        case 0:
            g_game->players[to].econ->metal
                = g_game->players[to].econ->metal - amount * -0.5;
            break;
        case 1:
            g_game->players[to].econ->metal
                = g_game->players[to].econ->metal - amount * -0.7;
            break;
        default: {
            // Add goes through a local float: fixes the x87 operand order.
            float m = g_game->players[to].econ->metal;
            m += amount;
            g_game->players[to].econ->metal = m;
            break;
        }
        }
    } else {
        // Add goes through a local float: fixes the x87 operand order.
        float m = g_game->players[to].econ->metal;
        m += amount;
        g_game->players[to].econ->metal = m;
    }
    if (flag)
        SendShareMetal(from, to, *(int*)&amount);
}
