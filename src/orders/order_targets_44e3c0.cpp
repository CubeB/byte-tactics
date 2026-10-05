// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Virtual method (DAT_004fd3b8 slot) of the Class_0044ce20 family: works out
// where the object should be. When the flags say it is active and not fully
// set, it aims at the reference unit's predicted position, optionally adding
// a direction offset from the fixed-point trig helpers; otherwise it just
// places itself at terrain level using the unit definition's height.
// Match notes: `pos` must be taken through a local pointer (`p = &pos`) so the
// struct-assignment destination stays in edi, and bit 22 of the unit
// definition flags must be a 1-bit bitfield so MSVC emits `shr` + `test dl,1`
// instead of folding it into `test [mem], 0x400000`.
#pragma pack(push, 1)

struct Vec3_0044e3c0 {
    int x;
    int y;
    int z;
};

struct UnitDef_0044e3c0 {
    char unknown_0[0x21c];
    short field_21c;                       // +0x21c
    char unknown_21e[0x241 - 0x21e];
    unsigned int unused : 22;              // +0x241
    unsigned int flag22 : 1;               // bit 22 of the flags
};

struct Unit {
    char unknown_0[0x66];
    short heading;                         // +0x66
    char unknown_68[0x82 - 0x68];
    int field_82;                          // +0x82
    char unknown_86[0x92 - 0x86];
    UnitDef_0044e3c0* def;                 // +0x92
};

struct Game {
    char unknown_0[0x1427f];
    unsigned char seaLevel;                // +0x1427f
    char unknown_14280[0x142b7 - 0x14280];
    int field_142b7;                       // +0x142b7
};

class Class_0044e3c0 {
public:
    char unknown_0[8];
    unsigned short flags;                  // +0x8
    short field_a;                         // +0xa
    short field_c;                         // +0xc
    short field_e;                         // +0xe
    short field_10;                        // +0x10
    Unit* unit;                            // +0x12
    char unknown_16[4];                    // +0x16
    Unit* target;                          // +0x1a
    char unknown_1e[8];                    // +0x1e
    Vec3_0044e3c0 pos;                     // +0x26
    int field_32;                          // +0x32

    int FUN_0044e3c0(Vec3_0044e3c0* out);
};
#pragma pack(pop)

extern Game* g_game;

int __cdecl FUN_004b70ef(short angle, int scale);
int __cdecl FUN_004b7123(short angle, int scale);
Vec3_0044e3c0 __stdcall FUN_0043e060(Unit* obj, int param);

static inline Vec3_0044e3c0 Direction(short angle, int scale)
{
    Vec3_0044e3c0 v;
    v.x = -FUN_004b70ef(angle, scale);
    v.y = 0;
    v.z = -FUN_004b7123(angle, scale);
    return v;
}

// FUNCTION: 0x44e3c0
int Class_0044e3c0::FUN_0044e3c0(Vec3_0044e3c0* out)
{
    unsigned short f = flags;
    if ((f & 1) && !(f & 0x80)) {
        if (target == 0 || target->field_82 == g_game->field_142b7)
            return 0;
        Vec3_0044e3c0* p = &pos;
        *p = FUN_0043e060(target, field_10);
        if (flags & 2) {
            short angle = target->heading;
            if (flags & 0x40)
                angle += field_e;
            Vec3_0044e3c0 d = Direction(angle, field_32);
            p->x -= d.x;
            p->y -= d.y;
            p->z -= d.z;
        }
        pos.y += field_c << 16;
    } else {
        if ((f & 8) == 0) {
            UnitDef_0044e3c0* def = unit->def;
            if (def->flag22)
                pos.y = (def->field_21c + g_game->seaLevel) << 16;
            else
                pos.y = (def->field_21c + *(unsigned char*)(unit->field_82 + 1)) << 16;
        }
    }
    if (pos.y > 0x1ff0000)
        pos.y = 0x1ff0000;
    *out = pos;
    return 1;
}
