// Decompiled by Opus. Names are provisional.
// Returns the 16.16 world position of the centre of a feature footprint whose
// corner is at map cell `cell`, with y set to the ground height there.

struct Pos_00421eb0 {
    int x;                             // +0x0 (16.16)
    int y;                             // +0x4
    int z;                             // +0x8
};

struct Cell_00421eb0 {
    short x;                           // +0x0
    short y;                           // +0x2
};

struct FeatureDef_00421eb0 {
    char unknown_0[0x94];
    Cell_00421eb0 footprint;           // +0x94
};

int __stdcall GetGroundHeight(Pos_00421eb0* pos);

// FUNCTION: 0x421eb0
Pos_00421eb0 __stdcall FUN_00421eb0(Cell_00421eb0* cell, FeatureDef_00421eb0* def)
{
    Cell_00421eb0 f = def->footprint;
    Cell_00421eb0 c = *cell;
    Pos_00421eb0 p;
    p.x = (f.x + c.x * 2) << 19;
    p.z = (f.y + c.y * 2) << 19;
    p.y = GetGroundHeight(&p) << 16;
    return p;
}
