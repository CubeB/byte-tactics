// Decompiled by Opus. Names are provisional.
// Snaps a unit's height (pos.y, 16.16 fixed point) to the ground. Units
// whose type has the flag at +0x241 bit 12 (floating) stay at least at the
// sea level minus the type's draft. The ground height is fetched twice on
// the floating path: the source used a max() macro.

#pragma pack(push, 1)
struct UnitType_0048a7f0 {
    char unknown_0[0x22c];
    unsigned char draft;               // +0x22c
    char unknown_22d[0x241 - 0x22d];
    unsigned int flags_lo : 12;        // +0x241
    unsigned int floats : 1;           // +0x241 bit 12
};

struct Pos_0048a7f0 {
    int x;                             // +0x0
    int y;                             // +0x4
    int z;                             // +0x8
};

struct Unit {
    char unknown_0[0x6a];
    Pos_0048a7f0 pos;                  // +0x6a
    char unknown_76[0x92 - 0x76];
    UnitType_0048a7f0* type;           // +0x92
};

struct Game_0048a7f0 {
    char unknown_0[0x1427f];
    unsigned char seaLevel;            // +0x1427f
};
#pragma pack(pop)

extern Game_0048a7f0* g_game;

int __stdcall FUN_00485070(Pos_0048a7f0* pos);

#define max(a, b) (((a) > (b)) ? (a) : (b))

#define max(a, b) (((a) > (b)) ? (a) : (b))

// FUNCTION: 0x48a7f0
void __stdcall FUN_0048a7f0(Unit* unit)
{
    if (unit->type->floats) {
        unit->pos.y = max(FUN_00485070(&unit->pos), g_game->seaLevel - unit->type->draft) << 16;
    } else {
        unit->pos.y = FUN_00485070(&unit->pos) << 16;
    }
}
