// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
// Slot 1 of Class_0044f010 (vtable 0x4fd458, the family is listed in
// 0x44f450.cpp and 0x44f080.cpp). It hands the object at +0x4 over to the
// path logic, then, unless the path is already active, steps the path
// forwards: it asks the object for its position (its slot 8) and marks the
// path active when the next path point is at most half as far from that
// object as the owner is. With no usable point left it resets the path to
// two points taken from the owner and the object's position.
#include <math.h>

class Pathfinder {
public:
    void FUN_0040e9c0(void* param);
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x14207];
    Pathfinder* field_14207;           // +0x14207
    char unknown_1420b[0x38a47 - 0x1420b];
    unsigned int field_38a47;          // +0x38a47
};
#pragma pack(pop)

extern Game* g_game;

class Class_0044ced0 {
public:
    void FUN_0044ced0(int param);
};

#pragma pack(push, 1)
struct Target_0044f2a0 {
    char unknown_0[0x42];
    unsigned int field_42;             // +0x42
};
#pragma pack(pop)

// A 16.16 fixed-point coordinate: the low word is the fraction, the high
// word the signed integer part the path points store.
union Coord_0044f2a0 {
    int fixed;
    short half[2];
};

struct Vec3_0044f2a0 {
    Coord_0044f2a0 x;
    Coord_0044f2a0 y;
    Coord_0044f2a0 z;
};

#pragma pack(push, 2)
struct Struct_004907e0 {
    char unknown_0[0x5c];
    Target_0044f2a0* field_5c;         // +0x5c
    char unknown_60[0x6a - 0x60];
    Vec3_0044f2a0 pos;                 // +0x6a
};
#pragma pack(pop)

struct Point_0044f2a0 {
    short x;
    short y;
};

// The object at +0x4 (see 0x490a10.cpp); vtable 0x4fd2f8. Slot 8 is the
// "give me your position as a Vec3" call (an implementation is 0x44dc60).
class Base_00490a10 {
public:
    virtual ~Base_00490a10();                           // slot 0
    virtual void FUN_0044ce80();                        // slot 1
    virtual void FUN_0044ce40();                        // slot 2
    virtual void FUN_0044cf30();                        // slot 3
    virtual int FUN_0044cf00(Struct_004907e0* owner);   // slot 4
    virtual int FUN_0044cf20(int x, int y);             // slot 5
    virtual void FUN_0044ce90();                        // slot 6
    virtual void FUN_0044cec0();                        // slot 7
    virtual int FUN_004e6110(Vec3_0044f2a0* out);       // slot 8
    virtual int FUN_0044cf40();                         // slot 9
    virtual void FUN_0044cf50();                        // slot 10
    virtual int FUN_0044cef0();                         // slot 11
};

// Vtable 0x4fd458, constructor 0x44f010, destructor 0x44f450, ??_G 0x44f040.
class Class_0044f010 {
public:
    Base_00490a10* field_4;            // +0x4
    Struct_004907e0* owner;            // +0x8
    Point_0044f2a0 points[20];         // +0xc
    int count;                         // +0x5c
    unsigned int field_60;             // +0x60
    unsigned char active : 1;          // +0x64 bit 0
    unsigned char flag_1 : 1;          // bit 1
    unsigned char flag_2 : 1;          // bit 2
    unsigned char flag_3 : 1;          // bit 3

    virtual void FUN_0044ef90(void* param);         // slot 1
};

// FUNCTION: 0x44f2a0
void Class_0044f010::FUN_0044ef90(void* param)
{
    g_game->field_14207->FUN_0040e9c0(this);
    if (field_4)
        ((Class_0044ced0*)field_4)->FUN_0044ced0(0x80);
    active = 0;
    field_4 = (Base_00490a10*)param;
    if (param == 0) {
        flag_1 = 0;
    } else {
        flag_1 = 1;
        if (count >= 3) {
            if (field_4->FUN_0044cf20(points[count - 1].x >> 4, points[count - 1].y >> 4)) {
                active = 1;
                flag_1 = 0;
            }
        }
        if (!active) {
            Vec3_0044f2a0 p;
            if (field_4->FUN_004e6110(&p)) {
                if (count >= 3) {
                    int sx = points[count - 1].x << 16;
                    int sz = points[count - 1].y << 16;
                    int d1 = (int)_hypot(owner->pos.x.fixed - p.x.fixed,
                                         owner->pos.z.fixed - p.z.fixed);
                    int d2 = (int)_hypot(sx - p.x.fixed,
                                         sz - p.z.fixed);
                    if (d2 * 2 < d1)
                        active = 1;
                }
                if (!active) {
                    Target_0044f2a0* t = owner->field_5c;
                    if (t && !(t->field_42 & 0x800000)) {
                        count = 2;
                        points[0].x = owner->pos.x.half[1];
                        points[0].y = owner->pos.z.half[1];
                        points[1].x = p.x.half[1];
                        points[1].y = p.z.half[1];
                        active = 1;
                    }
                }
            }
        }
    }
    if (field_60 <= g_game->field_38a47 - 10)
        field_60 = 0;
    flag_3 = 1;
}
