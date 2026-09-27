// Decompiled by GPT-6. Names are provisional.
#pragma pack(push, 1)
struct Game {
    char unknown_0[0x14223];
    int baseX;                         // +0x14223
    int baseY;                         // +0x14227
};
#pragma pack(pop)

extern Game* g_game;

struct Vec3_00407d40 {
    int x, y, z;

    Vec3_00407d40() {}
    Vec3_00407d40(Game* game) {
        int ax = (int)(game->baseX / 2 * 65536.0);
        *this = Vec3_00407d40(ax, 0, (int)(game->baseY / 2 * 65536.0));
    }
    Vec3_00407d40(int ax, int ay, int az) : x(ax), y(ay), z(az) {}
};

struct Class_00408cb0 {                // the owner (constructor 0x408cb0)
    char unknown_0[4];
    unsigned char field_4;             // +0x4
};

// Vtable 0x4fc980, constructor 0x407350, ??_G 0x407390.
class Class_00407350 {
public:
    Class_00408cb0* owner;             // +0x4
    void* field_8;                     // +0x8
    int field_c;                       // +0xc
    unsigned int field_10;             // +0x10

    Class_00407350(Class_00408cb0* p, void* q);
    virtual void FUN_00407380();                    // slot 0
    virtual ~Class_00407350() {}                    // slot 1
};

// Vtable 0x4fc9a0, constructor 0x407d40, ??_G 0x407e70.
class Class_00407d40 : public Class_00407350 {
public:
    Vec3_00407d40 a;                   // +0x14
    Vec3_00407d40 b;                   // +0x20
    Vec3_00407d40 c;                   // +0x2c
    int field_38;                      // +0x38

    Class_00407d40(Class_00408cb0* p, void* q);
    virtual void FUN_00407380();                    // slot 0, 0x407e90
};

Class_00407350::Class_00407350(Class_00408cb0* p, void* q)
    : owner(p), field_8(q), field_c(0), field_10(p->field_4) {}

// Partial: the final vector stores precede field_38 and the derived vtable.
// The original places field_38 and the vtable first, then stores through ecx.
// This version emits 297 bytes instead of 296.
// FUNCTION: 0x407d40
Class_00407d40::Class_00407d40(Class_00408cb0* p, void* q)
    : Class_00407350(p, q), a(g_game), b(g_game), c(g_game), field_38(0)
{
}
