// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
// Scales the object's position vector by 0.95 (16.16 fixed point), adds the
// offset `v`, rotates the (x, z) pair by the owner's heading, then derives two
// short offsets from the rotated components and the owner type's fields at
// +0x1a2/+0x1a6.
//
// The two Vec3 helpers must stay as inlined methods: written as three separate
// statements on `pos` the compiler keeps the scaled x and y live in callee
// saved registers and emits an extra push; as methods it stores each field
// immediately, as the original does.
//
// Suspected original bug: the second FUN_004b715a call reads xz[0] (the rotated
// x component) instead of xz[1]. Both reads are of the same stack slot:
//   first  call: mov ecx,[esp+8]          (xz[0])
//   second call: push edi; mov ecx,[esp+0xc]  ([esp+0xc] is the same xz[0]
//                                               after the push)
// so field_68 (the z offset) is computed from the rotated x. Kept as-is to
// match the original.

struct Vec3 {
    int x;
    int y;
    int z;

    void Scale(int s)
    {
        x = (int)(((__int64)x * s) >> 16);
        y = (int)(((__int64)y * s) >> 16);
        z = (int)(((__int64)z * s) >> 16);
    }
    void Add(const Vec3* o)
    {
        x += o->x;
        y += o->y;
        z += o->z;
    }
};

#pragma pack(push, 1)
struct Sub_0043d0d0 {
    char unknown_0[0x1a2];
    int field_1a2;                     // +0x1a2
    int field_1a6;                     // +0x1a6
};

struct Owner_0043d0d0 {
    char unknown_0[0x64];
    short field_64;                    // +0x64
    short field_66;                    // +0x66
    short field_68;                    // +0x68
    char unknown_6a[0x92 - 0x6a];
    Sub_0043d0d0* sub;                 // +0x92
};

struct Game {
    char unknown_0[0x14263];
    int count2;                        // +0x14263
};
#pragma pack(pop)

extern Game* g_game;
void __cdecl FUN_004b7173(short angle, int* xz);
int __cdecl FUN_004b715a(int x, int z);

class Class_0043d210 {
public:
    char unknown_0[0x14];
    Vec3 pos;                          // +0x14

    void ApplyBankAndPitch(Owner_0043d0d0* owner, Vec3* v);
};

// FUNCTION: 0x43d0d0
void Class_0043d210::ApplyBankAndPitch(Owner_0043d0d0* owner, Vec3* v)
{
    pos.Scale(0xf333);
    pos.Add(v);
    int xz[2];
    xz[0] = pos.x;
    xz[1] = pos.z;
    FUN_004b7173(owner->field_66, xz);
    int n = (int)(((__int64)g_game->count2 << 16) / 0xccd);
    owner->field_64 = (short)FUN_004b715a((int)(((__int64)owner->sub->field_1a2 * -xz[0]) >> 16), n);
    owner->field_68 = (short)FUN_004b715a((int)(((__int64)owner->sub->field_1a6 * -xz[0]) >> 16), n);
}
