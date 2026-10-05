// Decompiled by space-bunny-free. Names are provisional.
// Returns the world position of a weapon's aim piece: it first asks the
// unit's script for the "AimFrom" piece, and if the script has none
// (-1), falls back to the "Query" piece. The unit's position is then
// added to the piece offset. The two branches each build the
// GetPieceOffset call, which is how the original lays the code out.

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

class CobScript {
public:
    int QueryScript(char* name, int* param_2, int* param_3, int* param_4, int* param_5);
};

#pragma pack(push, 1)
struct Object {
    char unknown_0[0x6a];
    Vec3 pos;                          // +0x6a
    char unknown_76[0x9a - 0x76];
    CobScript* script;                 // +0x9a
};
#pragma pack(pop)

Vec3 __stdcall GetPieceOffset(Object* obj, int param);

// FUNCTION: 0x43e2e0
void __stdcall GetAimFromPosition(Object* obj, Vec3* out, unsigned char weapon)
{
    char* names[3] = { "AimFromPrimary", "AimFromSecondary", "AimFromTertiary" };
    int piece = -1;
    obj->script->QueryScript(names[weapon], &piece, 0, 0, 0);
    if (piece == -1) {
        char* qnames[3] = { "QueryPrimary", "QuerySecondary", "QueryTertiary" };
        int q = 0;
        obj->script->QueryScript(qnames[weapon], &q, 0, 0, 0);
        *out = obj->pos + GetPieceOffset(obj, q);
    } else {
        *out = obj->pos + GetPieceOffset(obj, piece);
    }
}
