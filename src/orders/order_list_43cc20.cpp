// Decompiled by space-bunny-free. Names are provisional.

// Adds `amount` to the object's distance accumulator (clamped at zero), limits
// it to a range taken from the table at DAT_00505205, then writes the offset
// for the unit's heading at that distance into the position vector.

#pragma pack(push, 1)
struct Game_0043cc20 {
    char unknown_0[0x1427f];
    unsigned char seaLevel;            // +0x1427f
};

struct Flags_0043cc20 {
    char unknown_0[0x192];
    int field_192;                     // +0x192
    char unknown_196[0x241 - 0x196];
    int field_241;                     // +0x241
};

struct Unit {
    char unknown_0[0x66];
    unsigned short heading;            // +0x66
    short field_68;                    // +0x68, in 2048ths of a circle
    char unknown_6a[0x70 - 0x6a];
    short field_70;                    // +0x70
    char unknown_72[0x92 - 0x72];
    Flags_0043cc20* flags;             // +0x92
};
#pragma pack(pop)

struct Vec3_0043cc20 {
    int x;
    int y;
    int z;
};

extern Game_0043cc20* g_game;
extern signed char DAT_00505205[];

// Fixed-point trig helpers written in assembly.
int __cdecl FUN_004b70ef(short angle, int scale);
int __cdecl FUN_004b7123(short angle, int scale);

// Inlined clamp: the reload of the field is what keeps the test off the flags
// of the add, giving the copy into eax and the explicit test.
static inline void ClampToZero(int& value)
{
    if (value < 0)
        value = 0;
}

class Class_0043cc20 {
public:
    char unknown_0[8];
    Vec3_0043cc20 pos;                 // +0x8
    char unknown_14[0x20 - 0x14];
    int field_20;                      // +0x20

    void FUN_0043cc20(Unit* unit, int amount);
};

// FUNCTION: 0x43cc20
void Class_0043cc20::FUN_0043cc20(Unit* unit, int amount)
{
    field_20 = field_20 + amount;
    ClampToZero(field_20);

    // Direction index, limited to the eleven entries of the table.
    int idx = unit->field_68 >> 11;
    if (idx < -5)
        idx = -5;
    if (idx > 5)
        idx = 5;

    // 16.16 range from the table entry, halved below sea level.
    int range = (int)(((__int64)(DAT_00505205[idx] << 16) * unit->flags->field_192) >> 16);
    range = (int)(((__int64)range << 16) / 0x640000);
    if (unit->field_70 < g_game->seaLevel && !(unit->flags->field_241 & 0x81000))
        range = (int)(((__int64)range * 0x8000) >> 16);
    if (field_20 > range)
        field_20 = range;

    int dist = field_20;
    unsigned short angle = unit->heading;
    Vec3_0043cc20 v;
    v.x = -FUN_004b70ef(angle, dist);
    v.y = 0;
    v.z = -FUN_004b7123(angle, dist);
    pos = v;
}
