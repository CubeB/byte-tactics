// Decompiled by Opus. Names are provisional.
// Clamps a 16.16 fixed-point position's x and z to the map
// ([0, baseX) and [0, baseY) in whole units).
//
// `add reg, 0xffff; shl reg, 16` for (size - 1) << 16 only comes from
// copying a bitfield struct {frac : 16, whole : 16} built in a local; plain
// integer arithmetic always folds to `shl; sub reg, 0x10000`.

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x14223];
    int baseX;                         // +0x14223
    int baseY;                         // +0x14227
};
#pragma pack(pop)

extern Game* g_game;

struct FixedParts_004853b0 {
    unsigned int frac : 16;
    int whole : 16;
};

union Fixed_004853b0 {
    int value;
    FixedParts_004853b0 parts;
};

struct Vec3_004853b0 {
    Fixed_004853b0 x;
    Fixed_004853b0 y;
    Fixed_004853b0 z;
};

static inline Fixed_004853b0 MakeFixed(int i)
{
    Fixed_004853b0 f;
    f.parts.frac = 0;
    f.parts.whole = i;
    return f;
}

// FUNCTION: 0x4853b0
void __stdcall ClampPositionToMap(Vec3_004853b0* p)
{
    if (p->x.value < 0)
        p->x.value = 0;
    else if (p->x.value >= MakeFixed(g_game->baseX).value)
        p->x = MakeFixed(g_game->baseX - 1);
    if (p->z.value < 0)
        p->z.value = 0;
    else if (p->z.value >= MakeFixed(g_game->baseY).value)
        p->z = MakeFixed(g_game->baseY - 1);
}
