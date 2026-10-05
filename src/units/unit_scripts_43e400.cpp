// Decompiled by Opus. Names are provisional.
// Asks the unit's script for its nano piece and returns that piece's world
// position (the unit's position plus the piece offset), like 0x43e060.

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
    int QueryScript(char* name, int* param_2, int* param_3, int* param_4, int* param_5);
};

#pragma pack(push, 1)
struct Object {
    char unknown_0[0x6a];
    Vec3 pos;                          // +0x6a
    char unknown_76[0x9a - 0x76];
    Class_004b0bc0* script;            // +0x9a
};
#pragma pack(pop)

Vec3 __stdcall GetPieceOffset(Object* obj, int param);

// FUNCTION: 0x43e400
void __stdcall GetNanoPiecePosition(Object* obj, Vec3* out)
{
    int piece = 0;
    obj->script->QueryScript("QueryNanoPiece", &piece, 0, 0, 0);
    *out = obj->pos + GetPieceOffset(obj, piece);
}
