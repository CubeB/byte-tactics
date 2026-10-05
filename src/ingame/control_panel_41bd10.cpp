// Decompiled by Opus. Names are provisional.

#pragma pack(push, 2)
struct Stats_0041bd10 {
    char unknown_0[0x186];
    float field_186;                   // +0x186
    char unknown_18a[0x1ea - 0x18a];
    int field_1ea;                     // +0x1ea
    char unknown_1ee[0x1fa - 0x1ee];
    int field_1fa;                     // +0x1fa
};

struct Unit {
    char unknown_0[0x92];
    Stats_0041bd10* stats;             // +0x92
    char unknown_96[0x108 - 0x96];
    short field_108;                   // +0x108
};
#pragma pack(pop)

// The call site sets ecx to the resource block and also pushes it, so
// FUN_00401180 is a __thiscall method that takes the block explicitly too
// (its body never reads ecx).
class UnitResources {
public:
    char unknown_0[0x10];
    int FUN_00401180(UnitResources* r, float amount);
};

struct Obj_0041bd10 {
    char unknown_0[0xbc];
    UnitResources field_bc;            // +0xbc
};

void __stdcall DamageUnit(Obj_0041bd10* obj, Unit* unit, int n, int kind, int flag);

// Code bytes match. check.py still fails the 1.0f constant: it compares 16
// bytes from each $T float constant, so it reads our adjacent -1.0f, while the
// original pool (shared with 0x41ba60) has the -0.7 double after 1.0f.

// FUNCTION: 0x41bd10
int __stdcall FUN_0041bd10(Obj_0041bd10* obj, Unit* unit, float f)
{
    int result = 0;
    Stats_0041bd10* s = unit->stats;
    int max = s->field_1fa;
    int v = s->field_1ea;
    if (unit->field_108 >= max)
        return 0;
    int n1 = (int)((max * f - 1.0f) / v + 1.0f);
    int n2 = (int)((s->field_186 * f - 1.0f) / v + 1.0f);
    if (n1 >= 1)
        n1 = 1;
    if (n2 >= 1)
        n2 = 1;
    if (obj->field_bc.FUN_00401180(&obj->field_bc, (float)n2)) {
        DamageUnit(obj, unit, n1, 10, 0);
        result = 1;
    }
    return result;
}
