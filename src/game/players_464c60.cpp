// Decompiled by space-bunny-free. Names are provisional.
// Moves `amount` energy from player `from` to player `to`. The amount is first
// clamped to what `from` has stored, then (only when `flag` is set) taken out of
// `from`'s economy object and added to `to`'s. An AI player (type 2) on easy or
// medium only counts 0.5 or 0.7 of it, and the transfer is announced over the
// network when `flag` is set (the amount travels as its bit pattern).
//
// Two phrasings here are load bearing, both verified by check.py:
// - `player` is picked inside a conditional whose two arms are the same
//   expression, so the address of `to`'s economy is computed on both arms of
//   the flag test (as in the original) and `to`, not `from`, is the parameter
//   MSVC keeps in a callee-saved register. A plain assignment after the call
//   gives the same code with `from` in ebx instead, 18 bytes out.
// - The energy is written through the full `g_game->players[to].econ`
//   expression, so MSVC does not fold the store into the load; the plain
//   `amount + energy` path is then `fld amount; fadd [field]`, the other way
//   round from a simple field destination.

class Class_00401260;

#pragma pack(push, 1)
struct Player_00464c60 {
    int active;                        // +0x00
    char unknown_4[0x73 - 4];
    unsigned char type;                // +0x73
    char unknown_74[0x98 - 0x74];
    float energy;                      // +0x98
    char unknown_9c[0xec - 0x9c];
    Class_00401260* econ;              // +0xec
    char unknown_f0[0x14b - 0xf0];
};

struct Game {
    char unknown_0[0x1b63];
    Player_00464c60 players[10];       // +0x1b63
    char unknown_2851[0x37eee - 0x2851];
    int difficulty;                    // +0x37eee
};
#pragma pack(pop)

class Class_00401260 {
public:
    char unknown_0[0x18];
    float energy;                      // +0x18
    char unknown_1c[0x30 - 0x1c];
    Player_00464c60* player;           // +0x30

    int FUN_00401260(float amount);
};

extern Game* g_game;

void __stdcall FUN_00457050(unsigned char from, unsigned char to, int value);

// FUNCTION: 0x464c60
void __stdcall FUN_00464c60(unsigned char from, unsigned char to, float amount, int flag)
{
    if (from == 10)
        return;
    if (to == 10)
        return;
    if (flag) {
        float cap = g_game->players[from].energy;
        if (amount > cap)
            amount = cap;
    }
    if (amount == 0.0f)
        return;
    Player_00464c60* player;
    player = flag ? (g_game->players[from].econ->FUN_00401260(amount), g_game->players[to].econ->player)
                  : g_game->players[to].econ->player;
    if (player->active != 0 && player->type == 2) {
        switch (g_game->difficulty) {
        case 0:
            g_game->players[to].econ->energy += amount * 0.5;
            break;
        case 1:
            g_game->players[to].econ->energy += amount * 0.7;
            break;
        default:
            g_game->players[to].econ->energy = amount + g_game->players[to].econ->energy;
            break;
        }
    } else {
        g_game->players[to].econ->energy = amount + g_game->players[to].econ->energy;
    }
    if (flag)
        FUN_00457050(from, to, *(int*)&amount);
}
