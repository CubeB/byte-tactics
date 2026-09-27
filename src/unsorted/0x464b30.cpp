// Decompiled by space-bunny-free. Names are provisional.
// Moves `amount` metal from player `from` to player `to`, the metal twin of
// 0x464c60 (which moves energy). It is clamped to what `from` has stored,
// taken out of `from`'s economy object (only when `flag` is set) and added to
// `to`'s economy object. An AI player (type 2) on easy or medium only counts
// 0.5 or 0.7 of it, and the transfer is announced over the network when `flag`
// is set. Same shape as 0x464c60, which moves energy with Class_00401260.
//
// Two things were needed, both of them read off the already-matched energy
// twin 0x464c60:
// 1. The receiver is read through a ternary whose two arms are identical apart
//    from the withdrawal, so the receiver's address computation is duplicated
//    into both arms of the flag test. That is what homes `to` rather than
//    `from` in ebx. With a plain assignment after the withdrawal MSVC keeps
//    `from` in ebx and the function is 18 bytes out.
// 2. The plain (non-AI, and AI-on-hard) add is routed through a local float:
//        float m = econ->metal; m += amount; econ->metal = m;
//    Writing it directly as `econ->metal = econ->metal + amount` (or `+=`, or
//    with the operands the other way round: all three desugar to the same IR)
//    makes MSVC canonicalise the x87 operand order and emit `fld amount;
//    fadd [econ->metal]`, whereas the original loads the destination first
//    (`fld [eax]; fadd [esp+0x1c]`). Going through a local forces that.

class Class_00401220;

#pragma pack(push, 1)
struct Player_00464b30 {
    int active;                        // +0x00
    char unknown_4[0x73 - 0x4];
    unsigned char type;                // +0x73
    char unknown_74[0x8c - 0x74];
    float metal;                       // +0x8c
    char unknown_90[0xec - 0x90];
    Class_00401220* econ;              // +0xec
    char unknown_f0[0x14b - 0xf0];
};

struct Game_00464b30 {
    char unknown_0[0x1b63];
    Player_00464b30 players[10];        // +0x1b63
    char unknown_2851[0x37eee - 0x2851];
    int difficulty;                    // +0x37eee
};
#pragma pack(pop)

class Class_00401220 {
public:
    float metal;                       // +0x0
    char unknown_4[0x30 - 0x4];
    Player_00464b30* player;           // +0x30

    int FUN_00401220(float amount);
};

extern Game_00464b30* g_game;

void __stdcall FUN_00456ee0(unsigned char from, unsigned char to, int value);

// FUNCTION: 0x464b30
void __stdcall FUN_00464b30(unsigned char from, unsigned char to, float amount, int flag)
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
    player = flag ? (g_game->players[from].econ->FUN_00401220(amount),
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
            float m = g_game->players[to].econ->metal;
            m += amount;
            g_game->players[to].econ->metal = m;
            break;
        }
        }
    } else {
        float m = g_game->players[to].econ->metal;
        m += amount;
        g_game->players[to].econ->metal = m;
    }
    if (flag)
        FUN_00456ee0(from, to, *(int*)&amount);
}
