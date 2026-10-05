// Decompiled by space-bunny-free. Names are provisional.
// Scans the local player's unit list for a unit whose +0x110 flags hold 0x20 and
// 0x80000000, whose +0x104 float is 0.0f, whose +0xfb is clear, whose +0x86
// owner (when there is one) has flag 0x40000000 and whose +0xac equals the
// argument. Returns 1 for the first such unit, else 0. The predicate is the
// negation of the one in the matched FUN_0048c9b0, which clears the unit's
// 0x10 flag when exactly this state no longer holds.
// The +0x104 test is MSVC 5's x87 lowering of `== 0.0f`: it reads only the C3
// bit, so at run time the body is entered when the float is above zero, the
// opposite of what the source says. The same is true of the `!= 0.0f` test in
// FUN_0048c9b0, so both functions select the complement of the intended unit.
// Writing it `!(... != 0.0f)` compiles to the same bytes.

#pragma pack(push, 1)
struct Unit {                            // 0x118 bytes
    char unknown_0[0x86];
    Unit* owner;                         // +0x86
    char unknown_8a[0xac - 0x8a];
    int field_ac;                        // +0xac
    char unknown_b0[0xfb - 0xb0];
    int field_fb;                        // +0xfb
    char unknown_ff[0x104 - 0xff];
    float field_104;                     // +0x104
    char unknown_108[0x110 - 0x108];
    unsigned int flags;                  // +0x110
    char unknown_114[0x118 - 0x114];
};

struct Player_0048dd10 {                 // 0x14b bytes
    char unknown_0[0x67];
    Unit* units_begin;                   // +0x67
    Unit* units_end;                     // +0x6b
    char unknown_6f[0x14b - 0x6f];
};

struct Game_0048dd10 {
    char unknown_0[0x1b63];
    Player_0048dd10 players[10];         // +0x1b63
    char unknown_2851[0x2a42 - 0x2851];
    unsigned char localPlayer;           // +0x2a42
};
#pragma pack(pop)

extern Game_0048dd10* g_game;

// FUNCTION: 0x48dd10
int __stdcall FUN_0048dd10(int param_1)
{
    Player_0048dd10* player = &g_game->players[g_game->localPlayer];
    for (Unit* u = player->units_begin; u <= player->units_end; u++) {
        unsigned int flags = u->flags;
        if (flags & 0x20) {
            if (u->field_104 == 0.0f) {
                if (u->field_fb == 0) {
                    Unit* owner = u->owner;
                    if (owner == 0 || (owner->flags & 0x40000000)) {
                        if (u->field_ac == param_1) {
                            if (flags & 0x80000000)
                                return 1;
                        }
                    }
                }
            }
        }
    }
    return 0;
}
