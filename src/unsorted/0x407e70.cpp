// Decompiled by Opus. Names are provisional.
// The compiler-generated scalar deleting destructor of Class_00407d40
// (vtable 0x4fc9a0), derived from Class_00407350 (the family is listed in
// 0x407350.cpp). Its destructor is trivial, so only the inlined base
// destructor's store of 0x4fc980 is left.
//
// A trivial destructor never stores this class's vtable, so the constructor
// (0x407d40) is defined again below, unannotated, to emit the vtable and with
// it this COMDAT. 0x407d40 itself is not matched yet: the original computes
// the three vectors before storing this class's vtable (as member
// initialisers would), with the last vector's stores after it; this body
// version reaches about 78%.

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

// FUNCTION: 0x407e70 ??_GClass_00407d40@@UAEPAXI@Z
Class_00407d40::Class_00407d40(Class_00408cb0* p, void* q)
    : Class_00407350(p, q)
{
    // Half of g_game's baseX and baseY, in 16.16 fixed point.
    int x = (int)(g_game->baseX / 2 * 65536.0);
    a = Vec3_00407d40(x, 0, (int)(g_game->baseY / 2 * 65536.0));
    x = (int)(g_game->baseX / 2 * 65536.0);
    b = Vec3_00407d40(x, 0, (int)(g_game->baseY / 2 * 65536.0));
    x = (int)(g_game->baseX / 2 * 65536.0);
    c = Vec3_00407d40(x, 0, (int)(g_game->baseY / 2 * 65536.0));
    field_38 = 0;
}
