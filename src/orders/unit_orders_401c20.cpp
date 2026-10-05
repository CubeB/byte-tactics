// Decompiled by Opus. Names are provisional.
// Handler of the "Stopping" entry in the order table at 0x4fc490: stops the
// unit and, for a VTOL that is flying, queues a VTOL_LANDIFCAN order.

class Class_00438760 {
public:
    unsigned char index;
    Class_00438760(const char* name);
};

struct Unit;
struct Vec_00401c20 {
    int x, y, z;
};

#pragma pack(push, 2)
class Class_0043a1f0 {
public:
    char unknown_0[0x56];
    Class_0043a1f0(Class_00438760 type, int a, Vec_00401c20* b, int c, int d, int e);
};
#pragma pack(pop)

class Class_00438880 {
public:
    void FUN_00438880(int param);
};

#pragma pack(push, 1)
struct UnitDef_00401c20 {
    char unknown_0[0x241];
    unsigned int flags;                // +0x241
};

struct Unit {
    char unknown_0[0x6a];
    Vec_00401c20 pos;                  // +0x6a
    char unknown_76[0x92 - 0x76];
    UnitDef_00401c20* def;             // +0x92
    char unknown_96[0x110 - 0x96];
    unsigned int flags;                // +0x110
};
#pragma pack(pop)

void __stdcall ClearWeaponTarget(Unit* unit, int which);
void __stdcall AppendOrder(Unit* owner, Class_0043a1f0* node);

// FUNCTION: 0x401c20
int __stdcall StopOrder(Unit* unit, Class_00438880* order, int unused)
{
    order->FUN_00438880(0);
    ClearWeaponTarget(unit, 0);
    ClearWeaponTarget(unit, 1);
    ClearWeaponTarget(unit, 2);
    if ((unit->flags & 3) == 2 && (unit->def->flags & 0x800)) {
        AppendOrder(unit, new Class_0043a1f0("VTOL_LANDIFCAN", 0, &unit->pos, 0, 0, 0));
    }
    return 5;
}
