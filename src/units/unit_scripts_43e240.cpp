// Decompiled by Opus. Names are provisional.
// Returns the world position of a weapon's aim piece (the unit's position
// plus the piece offset, like 0x43e060); a negative piece is first asked
// from the unit's script (0x43e1e0, inlined).

struct Vec3 {
    int x;
    int y;
    int z;
};

static inline Vec3 operator+(const Vec3& a, const Vec3& b)
{
    Vec3 r;
    r.x = a.x + b.x;
    r.y = a.y + b.y;
    r.z = a.z + b.z;
    return r;
}

class Class_004b0bc0 {
public:
    int FUN_004b0bc0(char* name, int* param_2, int* param_3, int* param_4, int* param_5);
};

#pragma pack(push, 1)
struct Object {
    char unknown_0[0x6a];
    Vec3 pos;                          // +0x6a
    char unknown_76[0x9a - 0x76];
    Class_004b0bc0* script;            // +0x9a
};
#pragma pack(pop)

Vec3 __stdcall FUN_0043def0(Object* obj, int param);

static inline int QueryWeaponPiece(Object* obj, unsigned char weapon)
{
    char* names[3] = { "QueryPrimary", "QuerySecondary", "QueryTertiary" };
    int piece = 0;
    obj->script->FUN_004b0bc0(names[weapon], &piece, 0, 0, 0);
    return piece;
}

// FUNCTION: 0x43e240
void __stdcall FUN_0043e240(Object* obj, Vec3* out, unsigned char weapon, int piece)
{
    if (piece < 0)
        piece = QueryWeaponPiece(obj, weapon);
    *out = obj->pos + FUN_0043def0(obj, piece);
}
