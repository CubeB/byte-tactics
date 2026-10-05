// Decompiled by Space Bunny Free. Names are provisional.
// Slot 8 of Class_0044e740 (vtable 0x4fd3f8, constructor 0x44e740), the slot
// before 0x44eb40: copies the first point out, advances it by the second point
// and, when flag bit 0 is set, turns the second point (a step per frame) towards
// the stored heading, by at most an eighth of the unit type's turn rate.
// The rotation helper takes a pair of ints, so the (x, z) step is built in the
// local's x and y fields and read back from y into other.z.

struct Vec3_0044ea60 {
    int x;
    int y;
    int z;

    Vec3_0044ea60() {}
    Vec3_0044ea60(int ax, int ay, int az)
    {
        x = ax;
        y = ay;
        z = az;
    }
};

#pragma pack(push, 1)
struct UnitType_0044ea60 {
    char unknown_0[0x1ba];
    unsigned short max_turn;           // +0x1ba
};

struct Object_0044ea60 {
    char unknown_0[0x92];
    UnitType_0044ea60* type;           // +0x92
};
#pragma pack(pop)

#pragma pack(push, 2)
class Class_0044ea60 {
public:
    char unknown_0[8];
    short field_8;                     // +0x8
    Vec3_0044ea60 target;              // +0xa
    Vec3_0044ea60 other;               // +0x16
    short field_22;                    // +0x22
    unsigned short heading;            // +0x24
    char unknown_26[2];
    Object_0044ea60* self;             // +0x28

    int FUN_0044ea60(Vec3_0044ea60* out);
};
#pragma pack(pop)

unsigned short __stdcall FUN_0048a980(Vec3_0044ea60* from, Vec3_0044ea60* to);
void __cdecl FUN_004b7173(short angle, int* xy);

// FUNCTION: 0x44ea60
int Class_0044ea60::FUN_0044ea60(Vec3_0044ea60* out)
{
    *out = target;
    target.x += other.x;
    target.z += other.z;
    Vec3_0044ea60 v;
    v = Vec3_0044ea60(0, 0, 0);
    unsigned short h = FUN_0048a980(&v, &other);
    if ((field_8 & 1) && h != heading) {
        short diff = heading - h;
        v.x = other.x;
        v.y = other.z;
        short step = diff;
        unsigned short max = self->type->max_turn;
        if (step >= (max >> 3))
            step = max >> 3;
        else if (step <= -(max >> 3))
            step = -(max >> 3);
        FUN_004b7173(-step, (int*)&v);
        other.x = v.x;
        other.z = v.y;
        other.y = 0;
    }
    return 1;
}
