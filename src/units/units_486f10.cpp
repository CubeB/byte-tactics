// Decompiled by Space Bunny Free. Names are provisional.
// Walks one player slot's unit list and either damages every unit whose owner
// is a human or computer player (DamageUnit) or fires its second weapon and
// flags it (DetonateUnitWeapon / KillUnit).
// The unit range is read from a second indexing of the array rather than from
// p: that keeps MSVC's index base free of the 0x1b63 array offset, which is
// what the original does (disp 0x1bca off eax, element address in ecx).

#pragma pack(push, 1)

struct PlayerData_00486f10 {
    int field_0;                        // +0x00
    char unknown_4[0x73 - 0x4];
    unsigned char field_73;             // +0x73
};

struct Unit {
    char unknown_0[0x96];
    PlayerData_00486f10* owner;         // +0x96
    char unknown_9a[0x110 - 0x9a];
    unsigned int flags;                 // +0x110
    char unknown_114[0x118 - 0x114];
};

struct Player_00486f10 {
    PlayerData_00486f10* field_0;       // +0x00
    char unknown_4[0x67 - 0x4];
    Unit* units_begin;                  // +0x67
    Unit* units_end;                    // +0x6b
    char unknown_6f[0x144 - 0x6f];
    short field_144;                    // +0x144
    char unknown_146[0x14b - 0x146];
};

struct Game {
    char unknown_0[0x1b63];
    Player_00486f10 players[10];        // +0x1b63
};

#pragma pack(pop)

extern Game* g_game;

void __stdcall KillUnit(Unit* unit, int param_2);
void __stdcall DamageUnit(Unit* unit, Unit* unit2, int param_3,
                            int param_4, int param_5);
void __stdcall DetonateUnitWeapon(Unit* unit, int second);

// FUNCTION: 0x486f10
void __stdcall KillPlayerUnits(unsigned char player)
{
    Player_00486f10* p = &g_game->players[player];
    if (p != 0) {
        if (p->field_144 != 0) {
            Unit* u = g_game->players[player].units_begin;
            Unit* last = g_game->players[player].units_end;
            if (u != 0) {
                for (; u <= last; u++) {
                    unsigned int flags = u->flags;
                    if (flags & 0x10000000) {
                        if (!(flags & 0x4000)) {
                            PlayerData_00486f10* d = u->owner;
                            if (d->field_0 != 0 &&
                                (d->field_73 == 1 || d->field_73 == 2)) {
                                DamageUnit(u, u, 0x7530, 3, 0);
                            } else {
                                DetonateUnitWeapon(u, 1);
                                u->flags |= 0x4000;
                                KillUnit(u, 3);
                            }
                        }
                    }
                }
            }
        }
    }
}
