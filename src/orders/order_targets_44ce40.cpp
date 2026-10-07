// Decompiled by Haiku, Sonnet, Opus, deepseek-v4.1-flash and DeepSeek V4.1 Flash. Names are provisional.
// The order-target area classes, part 1: the Class_0044ce20 family (circles,
// rings and rectangles, with their hit tests, world-position writers and
// HapiBank loaders), the std::vector<Point_0044eec0> point-list builders and
// the class that writes a target to a bit stream.
// Keep the header include: without it the operand order of 0x44d560's sum flips.
#include <memory.h>
#include "../util/hapi_bank.h"
#include <stdlib.h>
#include <vector>

struct Point_0044eec0 {
    short x;
    short y;
};

// The vtables the area classes store by hand; the base class's is 0x4fd2f8.
extern void* DAT_004fd2f8[];
extern void* DAT_004fd328[];
extern void* DAT_004fd358[];
extern void* DAT_004fd388[];

extern void __cdecl operator delete(void* p);

class Class_0044ce50 {
public:
    void** vtable;

    void* FUN_0044ce50(unsigned char flag);
};

class Class_0044ce70 {
public:
    void* vtable;

    Class_0044ce70(int arg1, int arg2, int arg3);
};

struct Elem_0044ce90 {
    int unknown_0;
};

class Class_0044ce90 {
public:
    void FUN_0044ce90(std::vector<Elem_0044ce90*>* list);
};

struct Class_0044ced0 {
    char unknown_0[4];
    int field_4;

    void FUN_0044ced0(int param_1);
};

class Class_0044cf00
{
public:
    virtual void FUN_00000000();
    virtual void FUN_00000001();
    virtual void FUN_00000002();
    virtual void FUN_00000003();
    virtual void FUN_00000004();
    virtual void FUN_00000005(int param_1, int param_2);

    void FUN_0044cf00(int param_1);
};

struct Point_0044cf60 {
    short x;
    short y;
};

struct Map_0044cf60 {
    char unknown_0[0x7e];
    Point_0044cf60 origin;             // +0x7e
};

#pragma pack(push, 2)
struct Source_0044cf60 {
    char unknown_0[0xe];
    Map_0044cf60* map;                 // +0xe
};
#pragma pack(pop)

// The area classes' base: vtable 0x4fd2f8 and, at +0x4, the source object
// (or its index) the subclass was built with.
class Class_0044ce20 {
public:
    void* vtable;                      // +0x0
    int field_4;                       // +0x4

    Class_0044ce20(int param_1)
    {
        vtable = DAT_004fd2f8;
        field_4 = param_1;
    }
};

class Class_0044cf60 : public Class_0044ce20 {
public:
    Point_0044cf60 pos;                // +0x8
    int radius;                        // +0xc
    int radiusSq;                      // +0x10

    Class_0044cf60(Source_0044cf60* source, int x, int y, int r);
};

class Class_0044cff0 {
public:
    void* vtable;

    void* FUN_0044cff0(unsigned char flag);
};

struct Vec3_0044d010 {
    int x;
    int y;
    int z;
};

struct Header_0044d010 {
    int magic;                         // +0x0
    Vec3_0044d010 v;                   // +0x4
};

class Class_0044d010 : public Class_0044ce20 {
public:
    Vec3_0044d010 v;                   // +0x8

    int FUN_0044d090(int unused, HapiBank* file, char* name);
    Class_0044d010(int owner, HapiBank* file, char* name);
};

typedef std::vector<Point_0044eec0> Vec_0044d0e0;

class Class_0044d0e0 {
public:
    char unknown_0[8];
    Point_0044eec0 pos;                // +0x8

    void FUN_0044d0e0(Vec_0044d0e0* list);
};

class Class_0044d290 {
public:
    char unknown_0[0x8];
    short x;                           // +0x8
    short y;                           // +0xa
    char unknown_c[0x10 - 0xc];
    int radiusSq;                      // +0x10

    int FUN_0044d290(int px, int py);
};

struct Point_0044d2c0 {
    short x;
    short y;
};

struct Inner_0044d2c0 {
    char unknown_0[0x7e];
    Point_0044d2c0 pos;                // +0x7e
};

#pragma pack(push, 1)
struct Owner_0044d2c0 {
    char unknown_0[0xe];
    Inner_0044d2c0* inner;             // +0xe
};
#pragma pack(pop)

class Class_0044d2c0 {
public:
    char unknown_0[4];
    Owner_0044d2c0* owner;             // +0x4
    Point_0044d2c0 pos;                // +0x8

    int FUN_0044d2c0(int* out);
};

struct Obj_0044d310 {
    char unknown_0[0x76];
    short x;                           // +0x76
    short y;                           // +0x78
};

class Class_0044d310 {
public:
    char unknown_0[0x8];
    short x;                           // +0x8
    short y;                           // +0xa
    char unknown_c[0x10 - 0xc];
    int radiusSq;                      // +0x10

    int FUN_0044d310(Obj_0044d310* obj);
};

class Class_0044d350 {
public:
    char unknown_0[0x8];
    short x;                           // +0x8
    short y;                           // +0xa
    int radius;                        // +0xc

    int FUN_0044d350(int px, int py);
};

struct Point_0044d3b0 {
    short x;
    short y;
};

struct Map_0044d3b0 {
    char unknown_0[0x7e];
    Point_0044d3b0 origin;             // +0x7e
};

#pragma pack(push, 2)
struct Source_0044d3b0 {
    char unknown_0[0xe];
    Map_0044d3b0* map;                 // +0xe
};
#pragma pack(pop)

class Class_0044d3b0 : public Class_0044ce20 {
public:
    Point_0044d3b0 pos;                // +0x8
    int radius2;                       // +0xc
    int radius1;                       // +0x10
    int radius2Sq;                     // +0x14
    int radius1Sq;                     // +0x18

    Class_0044d3b0(Source_0044d3b0* source, int x, int y, int r1, int r2);
};

struct Class_0044d450
{
public:
    void** vtable;
    char unknown_4[0x49d];

    Class_0044d450* FUN_0044d450(unsigned char flag);
};

struct Data_0044d470 {
    int a;
    int b;
    int c;
    int d;
    int e;
};

struct Header_0044d470 {
    int magic;                         // +0x0
    Data_0044d470 data;                // +0x4
};

class Class_0044d470 : public Class_0044ce20 {
public:
    Data_0044d470 data;                // +0x8

    int FUN_0044d500(int unused, HapiBank* file, char* name);
    Class_0044d470(int owner, HapiBank* file, char* name);
};

typedef std::vector<Point_0044eec0> Vec_0044d560;

class Class_0044d560 {
public:
    char unknown_0[8];
    Point_0044eec0 pos;                // +0x8
    int field_c;                       // +0xc
    int field_10;                      // +0x10

    void FUN_0044d560(Vec_0044d560* list);
};

struct Point_0044d720 {
    short x;
    short y;
};

struct Vec3_0044d720 {
    int x;
    int y;
    int z;
};

#pragma pack(push, 2)
struct Inner_0044d720 {
    char unknown_0[0x6a];
    Vec3_0044d720 pos;                 // +0x6a
    char unknown_76[0x7e - 0x76];
    Point_0044d720 cell;               // +0x7e
};
#pragma pack(pop)

#pragma pack(push, 1)
struct Owner_0044d720 {
    char unknown_0[0xe];
    Inner_0044d720* inner;             // +0xe
};
#pragma pack(pop)

int __stdcall GetHeadingBetween(Vec3_0044d720* from, Vec3_0044d720* to);

// Fixed-point trig helpers written in assembly.
int __cdecl FUN_004b70ef(short angle, int scale);
int __cdecl FUN_004b7123(short angle, int scale);

class Class_0044d720 {
public:
    char unknown_0[4];
    Owner_0044d720* owner;             // +0x4
    Point_0044d720 pos;                // +0x8
    int radius1;                       // +0xc
    int radius2;                       // +0x10

    int FUN_0044d720(Vec3_0044d720* out);
};

class Class_0044d7c0 {
public:
    char unknown_0[8];
    short field_8;      // +0x8
    short field_a;      // +0xa
    char unknown_c[0x14 - 0xc];
    int field_14;       // +0x14 (min distance squared)
    int field_18;       // +0x18 (max distance squared)

    bool FUN_0044d7c0(int param_1, int param_2);
};

struct Unit {
    char unknown_0[0x76];
    short x;                           // +0x76
    short y;                           // +0x78
};

class Class_0044d800 {
public:
    char unknown_0[8];
    short field_8;                     // +0x8
    short field_a;                     // +0xa
    char unknown_c[0x14 - 0xc];
    int field_14;                      // +0x14 (min distance squared)
    int field_18;                      // +0x18 (max distance squared)

    int FUN_0044d800(Unit* unit);
};

class Class_0044d840 {
public:
    char unknown_0[8];
    short x;                           // +0x8
    short y;                           // +0xa
    int inner;                         // +0xc
    int outer;                         // +0x10

    int FUN_0044d840(int px, int py);
};

struct View_0044d8a0 {
    char unknown_0[0x7e];
    short x;                           // +0x7e
    short y;                           // +0x80
};

#pragma pack(push, 2)
struct Owner_0044d8a0 {
    char unknown_0[0xe];
    View_0044d8a0* view;               // +0xe
};
#pragma pack(pop)

struct Point_0044d8a0 {
    short x;
    short y;
};

class Class_0044d8a0 : public Class_0044ce20 {
public:
    int a;                             // +0x8
    int b;                             // +0xc
    int c;                             // +0x10
    int d;                             // +0x14

    Class_0044d8a0(Owner_0044d8a0* owner, Point_0044d8a0 pos, Point_0044d8a0 size);
};

class Class_0044d910 {
public:
    virtual ~Class_0044d910() {}

    Class_0044d910* FUN_0044d910(unsigned char should_delete);
};

struct Rect_0044d930 {
    int a;
    int b;
    int c;
    int d;
};

struct Header_0044d930 {
    int magic;                         // +0x0
    Rect_0044d930 r;                   // +0x4
};

class Class_0044d930 : public Class_0044ce20 {
public:
    Rect_0044d930 r;                   // +0x8

    int FUN_0044d9a0(int unused, HapiBank* file, char* name);
    Class_0044d930(int owner, HapiBank* file, char* name);
};

typedef std::vector<Point_0044eec0> Vec_0044da00;

class Class_0044da00 {
public:
    char unknown_0[8];
    int x1;                 // +0x8
    int x2;                 // +0xc
    int y1;                 // +0x10
    int y2;                 // +0x14

    void FUN_0044da00(Vec_0044da00* list);
};

struct Pos16_44dc60 {
    short x;
    short z;
};

struct MapInfo_44dc60 {
    char unknown_0[0x7e];
    Pos16_44dc60 pos;   // +0x7e
};

struct Class_0044dc60 {
    char unknown_0[4];
    char* mapPtr;            // +4
    int x1;                  // +8
    int x2;                  // +0xc
    int y1;                  // +0x10
    int y2;                  // +0x14

    int FUN_0044dc60(int* out);
};

class Class_0044dcb0 {
public:
    char unknown_0[8];
    int x1;                  // +8
    int x2;                  // +0xc
    int y1;                  // +0x10
    int y2;                  // +0x14

    int FUN_0044dcb0(int x, int y);
};

class Class_0044dd00 {
public:
    char unknown_0[8];
    int x1;                            // +0x8
    int x2;                            // +0xc
    int y1;                            // +0x10
    int y2;                            // +0x14

    int FUN_0044dd00(int x, int y);
};

#pragma pack(push, 2)

class BitWriter {
public:
    void WriteBits(int value, int bits);
};

struct Target_0044ddc0 {
    char unknown_0[0xa8];
    unsigned short field_a8;        // +0xa8
};

class Class_0044ddc0 {
public:
    char unknown_0[8];
    unsigned short flags;           // +0x8
    short field_a;                  // +0xa
    short field_c;                  // +0xc
    short field_e;                  // +0xe
    short field_10;                 // +0x10
    char unknown_12[0x1a - 0x12];
    Target_0044ddc0* ptr;           // +0x1a
    char unknown_1e[0x26 - 0x1e];
    int field_26;                   // +0x26
    int field_2a;                   // +0x2a
    int field_2e;                   // +0x2e

    void FUN_0044ddc0(BitWriter* stream);
};

#pragma pack(pop)

// FUNCTION: 0x44ce40
int FUN_0044ce40(void)
{
    return 1;
}

// FUNCTION: 0x44ce50
void* Class_0044ce50::FUN_0044ce50(unsigned char flag)
{
    vtable = DAT_004fd2f8;
    if (flag & 1) {
        operator delete(this);
    }
    return this;
}

// FUNCTION: 0x44ce70
Class_0044ce70::Class_0044ce70(int arg1, int arg2, int arg3)
{
    vtable = DAT_004fd2f8;
}

// FUNCTION: 0x44ce80
int __stdcall FUN_0044ce80(int, int, int)
{
    return 0;
}

// Slot 6 of the vtable at 0x4fd2f8: the default implementation ignores the
// object and empties the caller's list (an inlined vector::clear()).
// FUNCTION: 0x44ce90
void Class_0044ce90::FUN_0044ce90(std::vector<Elem_0044ce90*>* list)
{
    list->clear();
}

// FUNCTION: 0x44cec0
int __stdcall FUN_0044cec0(int, int)
{
    return 0;
}

// FUNCTION: 0x44ced0
void Class_0044ced0::FUN_0044ced0(int param_1)
{
    int val = field_4;
    if (val != 0) {
        int* target = (int*)(val + 0x4e);
        *target |= param_1;
    }
}

// FUNCTION: 0x44cef0
int FUN_0044cef0(void)
{
    return 0;
}

// FUNCTION: 0x44cf00
void Class_0044cf00::FUN_0044cf00(int param_1)
{
    short esi = *(short*)(param_1 + 0x78);
    short eax = *(short*)(param_1 + 0x76);

    FUN_00000005(eax, esi);
}

// FUNCTION: 0x44cf20
int __stdcall FUN_0044cf20(int, int)
{
    return 0;
}

// FUNCTION: 0x44cf30
int FUN_0044cf30(void)
{
    return 1;
}

// FUNCTION: 0x44cf40
int __stdcall FUN_0044cf40(int)
{
    return 0;
}

// FUNCTION: 0x44cf50
void __stdcall FUN_0044cf50(int)
{
}

// Constructor of a Class_0044ce20 subclass that stores a point converted
// from fixed-point world coordinates relative to the map origin, a radius
// and (radius / 16) squared. It stores the same vtable as Class_0044d010,
// so this is probably a second constructor of that class.
// FUNCTION: 0x44cf60
Class_0044cf60::Class_0044cf60(Source_0044cf60* source, int x, int y, int r)
    : Class_0044ce20((int)source)
{
    vtable = DAT_004fd328;
    Point_0044cf60 org = source->map->origin;
    Point_0044cf60 p;
    p.x = (x - (org.x << 19) + 0x80000) >> 20;
    p.y = (y - (org.y << 19) + 0x80000) >> 20;
    pos = p;
    radius = r;
    radiusSq = (r / 16) * (r / 16);
}

// FUNCTION: 0x44cfe0
int FUN_0044cfe0(void)
{
    return 4;
}

// FUNCTION: 0x44cff0
void* Class_0044cff0::FUN_0044cff0(unsigned char flag)
{
    vtable = DAT_004fd2f8;
    if (flag & 1) {
        operator delete(this);
    }
    return this;
}

// Constructor of a Class_0044ce20 subclass that reads a 16-byte header from
// a named entry of an open file and keeps its last three dwords.
// FUNCTION: 0x44d010
Class_0044d010::Class_0044d010(int owner, HapiBank* file, char* name)
    : Class_0044ce20(owner)
{
    vtable = DAT_004fd328;
    int bad = 0;
    if (owner == 0 || file == 0)
        bad = 1;
    if (name == 0)
        bad = 1;
    if (!bad) {
        file->OpenNamedBox(name);
        file->SeekBox(0);
        Header_0044d010 hdr;
        if (file->ReadBox(&hdr, 16) == 16) {
            v.x = hdr.v.x;
            v.y = hdr.v.y;
            v.z = hdr.v.z;
        }
    }
}

// Saving counterpart of Class_0044d010's constructor: writes a 16-byte
// header whose last three dwords are the stored values (the first dword is
// left uninitialised, as in the original).
// FUNCTION: 0x44d090
int Class_0044d010::FUN_0044d090(int unused, HapiBank* file, char* name)
{
    Header_0044d010 hdr;
    hdr.v.x = v.x;
    hdr.v.y = v.y;
    hdr.v.z = v.z;
    file->OpenNamedBox(name);
    file->SeekBox(0);
    file->WriteBox(&hdr, 16);
    return 1;
}

// An inlined std::vector insert. The vector holds 4-byte points (two shorts,
// the element type of 0x44ee60/0x44ee90/0x44eec0/0x44eef0), and the method
// resets the list to a single point taken from this->+8, that is an inlined
// vector::assign(1, pos) or clear() plus push_back(pos):
//
//   list->clear();          // erase(begin(), end())
//   list->push_back(pos);   // inserts *(Point*)(this + 8) at the end
// FUNCTION: 0x44d0e0
void Class_0044d0e0::FUN_0044d0e0(Vec_0044d0e0* list)
{
    list->clear();
    list->push_back(pos);
}

// Point version of 0x44d310: whether (px, py) lies within the circle around
// (x, y).
// FUNCTION: 0x44d290
int Class_0044d290::FUN_0044d290(int px, int py)
{
    int dy = py - y;
    int dx = px - x;
    return dx * dx + dy * dy <= radiusSq;
}

// Virtual method of the circle area class (compare 0x44d290 and 0x44d310,
// which use the same centre at +0x8): writes the centre, offset by a point
// of the owner, as 16.16 world coordinates.

static inline void ToWorld(int* out, Point_0044d2c0 a, Point_0044d2c0 b)
{
    out[0] = (a.x * 2 + b.x) << 19;
    out[2] = (a.y * 2 + b.y) << 19;
}

// FUNCTION: 0x44d2c0
int Class_0044d2c0::FUN_0044d2c0(int* out)
{
    ToWorld(out, pos, owner->inner->pos);
    return 1;
}

// Whether obj lies within the circle around (x, y).
// FUNCTION: 0x44d310
int Class_0044d310::FUN_0044d310(Obj_0044d310* obj)
{
    int dy = obj->y - y;
    int dx = obj->x - x;
    return dx * dx + dy * dy <= radiusSq;
}

// Approximate distance from (px, py) to the edge of the circle around
// (x, y), 0 when inside: 18 * major + 7 * minor axis distance.
// FUNCTION: 0x44d350
int Class_0044d350::FUN_0044d350(int px, int py)
{
    int dx = abs(px - x);
    int dy = abs(py - y);
    int d;
    if (dx > dy)
        d = dy * 7 + dx * 18;
    else
        d = dx * 7 + dy * 18;
    if (d < radius)
        return 0;
    return d - radius;
}

// Constructor of a Class_0044ce20 subclass (vtable 0x4fd358, the same class
// as Class_0044d470's file-loading constructor) that stores a point converted
// from fixed-point world coordinates relative to the map origin, two radii
// and each (radius / 16) squared. Two-radius version of 0x44cf60.
// FUNCTION: 0x44d3b0
Class_0044d3b0::Class_0044d3b0(Source_0044d3b0* source, int x, int y, int r1, int r2)
    : Class_0044ce20((int)source)
{
    vtable = DAT_004fd358;
    Point_0044d3b0 org = source->map->origin;
    Point_0044d3b0 p;
    p.x = (x - (org.x << 19) + 0x80000) >> 20;
    p.y = (y - (org.y << 19) + 0x80000) >> 20;
    pos = p;
    // Stays after the point conversion.
    radius2 = r2;
    radius1 = r1;
    radius1Sq = (r1 / 16) * (r1 / 16);
    radius2Sq = (r2 / 16) * (r2 / 16);
}

// FUNCTION: 0x44d440
int FUN_0044d440(void)
{
    return 5;
}

// FUNCTION: 0x44d450
Class_0044d450* Class_0044d450::FUN_0044d450(unsigned char flag)
{
    vtable = DAT_004fd2f8;
    if ((flag & 1) != 0) {
        operator delete(this);
    }
    return this;
}

// Constructor of a Class_0044ce20 subclass that reads a 24-byte header from
// a named entry of an open file and keeps its last five dwords.
// FUNCTION: 0x44d470
Class_0044d470::Class_0044d470(int owner, HapiBank* file, char* name)
    : Class_0044ce20(owner)
{
    vtable = DAT_004fd358;
    int bad = 0;
    if (owner == 0 || file == 0)
        bad = 1;
    if (name == 0)
        bad = 1;
    if (!bad) {
        file->OpenNamedBox(name);
        file->SeekBox(0);
        Header_0044d470 hdr;
        if (file->ReadBox(&hdr, 24) == 24) {
            data.a = hdr.data.a;
            data.b = hdr.data.b;
            data.c = hdr.data.c;
            data.d = hdr.data.d;
            data.e = hdr.data.e;
        }
    }
}

// Saving counterpart of Class_0044d470's constructor: writes a 24-byte header
// whose last five dwords are the stored values. The first dword is left
// uninitialised, exactly as in 0x44d090 and 0x44d9a0.
// FUNCTION: 0x44d500
int Class_0044d470::FUN_0044d500(int unused, HapiBank* file, char* name)
{
    Header_0044d470 hdr;
    hdr.data.a = data.a;
    hdr.data.b = data.b;
    hdr.data.c = data.c;
    hdr.data.d = data.d;
    hdr.data.e = data.e;
    file->OpenNamedBox(name);
    file->SeekBox(0);
    file->WriteBox(&hdr, 24);
    return 1;
}

// Clears a std::vector<Point_0044eec0> and then appends a copy of the point
// at +0x8, with its y moved by (field_c + field_10) / 32.
// FUNCTION: 0x44d560
void Class_0044d560::FUN_0044d560(Vec_0044d560* list)
{
    list->clear();
    Point_0044eec0 p = pos;
    // Sum written field_10 + field_c: the operand order follows the load order.
    p.y = p.y + (field_10 + field_c) / 32;
    list->push_back(p);
}

// Virtual method of the ring area class (compare 0x44d2c0, which writes the
// same centre): writes the centre as 16.16 world coordinates, then moves it
// back towards the owner by the mean of the two radii at +0xc and +0x10.
// The mean needs its own inline helper to load +0xc first.

static inline void ToWorld(Vec3_0044d720* out, Point_0044d720 a, Point_0044d720 b)
{
    out->x = (a.x * 2 + b.x) << 19;
    out->z = (a.y * 2 + b.y) << 19;
}

// Inlined copy of FUN_004103a0.
static inline Vec3_0044d720 Direction(short angle, int scale)
{
    Vec3_0044d720 v;
    v.x = -FUN_004b70ef(angle, scale);
    v.y = 0;
    // The casts stay: without them 0x44d720's radius loads come out in the other order.
    v.z = -FUN_004b7123((short)angle, (int)scale);
    return v;
}

static inline int MeanRadius(Class_0044d720* c)
{
    return (c->radius1 + c->radius2) / 2;
}

// FUNCTION: 0x44d720
int Class_0044d720::FUN_0044d720(Vec3_0044d720* out)
{
    Inner_0044d720* inner = owner->inner;
    ToWorld(out, pos, inner->cell);
    short angle = GetHeadingBetween(&inner->pos, out);
    Vec3_0044d720 d = Direction(angle, MeanRadius(this) << 16);
    out->x -= d.x;
    out->y -= d.y;
    out->z -= d.z;
    return 1;
}

// FUNCTION: 0x44d7c0
bool Class_0044d7c0::FUN_0044d7c0(int param_1, int param_2)
{
    int dy = param_2 - field_a;
    int dx = param_1 - field_8;
    int distSq = dx * dx + dy * dy;
    return distSq <= field_18 && distSq >= field_14;
}

// Same test as 0x44d7c0, on a unit's position: 1 when the squared distance
// from the centre lies within [field_14, field_18].
// FUNCTION: 0x44d800
int Class_0044d800::FUN_0044d800(Unit* unit)
{
    int dy = unit->y - field_a;
    int dx = unit->x - field_8;
    int distSq = dx * dx + dy * dy;
    return distSq <= field_18 && distSq >= field_14;
}

// Approximate distance from (px, py) to the ring around (x, y) with the
// inner radius +0xc and the outer radius +0x10, 0 between the radii.
// Same 18 * major + 7 * minor metric as 0x44d350.
// FUNCTION: 0x44d840
int Class_0044d840::FUN_0044d840(int px, int py)
{
    int dx = abs(px - x);
    int dy = abs(py - y);
    int d;
    if (dx > dy)
        d = dy * 7 + dx * 18;
    else
        d = dx * 7 + dy * 18;
    if (d > outer)
        return d - outer;
    if (d < inner)
        return inner - d;
    return 0;
}

// Second constructor of the Class_0044ce20 subclass with vtable 0x4fd388
// (compare 0x44d930): the rectangle comes from a position, made relative to
// the owner's view origin, and a size.
// FUNCTION: 0x44d8a0
Class_0044d8a0::Class_0044d8a0(Owner_0044d8a0* owner, Point_0044d8a0 pos, Point_0044d8a0 size)
    : Class_0044ce20((int)owner)
{
    vtable = DAT_004fd388;
    a = pos.x - owner->view->x;
    c = pos.y - owner->view->y;
    b = size.x + pos.x;
    d = size.y + pos.y;
}

// FUNCTION: 0x44d900
int FUN_0044d900(void)
{
    return 6;
}

// FUNCTION: 0x44d910
Class_0044d910* Class_0044d910::FUN_0044d910(unsigned char should_delete)
{
    Class_0044d910* esi = this;
    *(void**)esi = DAT_004fd2f8;
    if (should_delete & 1) {
        operator delete(esi);
    }
    return esi;
}

// Constructor of a Class_0044ce20 subclass that reads a 20-byte header from
// a named entry of an open file and keeps its last four dwords.
// FUNCTION: 0x44d930
Class_0044d930::Class_0044d930(int owner, HapiBank* file, char* name)
    : Class_0044ce20(owner)
{
    vtable = DAT_004fd388;
    file->OpenNamedBox(name);
    file->SeekBox(0);
    Header_0044d930 hdr;
    if (file->ReadBox(&hdr, 20) == 20) {
        r.a = hdr.r.a;
        r.b = hdr.r.b;
        r.c = hdr.r.c;
        r.d = hdr.r.d;
    }
}

// Saving counterpart of Class_0044d930's constructor: writes a 20-byte
// header whose last four dwords are the stored values (the first dword is
// left uninitialised, as in the original), like 0x44d090.
// FUNCTION: 0x44d9a0
int Class_0044d930::FUN_0044d9a0(int unused, HapiBank* file, char* name)
{
    Header_0044d930 hdr;
    hdr.r.a = r.a;
    hdr.r.b = r.b;
    hdr.r.c = r.c;
    hdr.r.d = r.d;
    file->OpenNamedBox(name);
    file->SeekBox(0);
    file->WriteBox(&hdr, 20);
    return 1;
}

static inline int MidX_44dc60(Class_0044dc60* w)
{
    return (w->x1 + w->x2) / 2;
}

// FUNCTION: 0x44dc60
int Class_0044dc60::FUN_0044dc60(int* out)
{
    short avg = (short)MidX_44dc60(this);
    short z = (short)y2;
    MapInfo_44dc60* info = *(MapInfo_44dc60**)(mapPtr + 0xe);
    Pos16_44dc60 pos = info->pos;
    out[0] = (pos.x + avg * 2) << 0x13;
    out[2] = (pos.z + z * 2) << 0x13;
    return 1;
}

// Same rectangle layout as 0x44dc60 (x1/x2 at +8/+0xc, y1/y2 at +0x10/+0x14):
// is the point (x, y) on the rectangle's border?
// FUNCTION: 0x44dcb0
int Class_0044dcb0::FUN_0044dcb0(int x, int y)
{
    if ((x == x1 || x == x2) && y >= y1 && y <= y2) {
        return 1;
    }
    if ((y == y1 || y == y2) && x >= x1 && x <= x2) {
        return 1;
    }
    return 0;
}

// Same rectangle layout as 0x44dc60/0x44dcb0 (x1/x2 at +8/+0xc, y1/y2 at
// +0x10/+0x14): approximate distance from (x, y) to the rectangle, 0 inside.
// Outside a corner it is 16 * major + 6 * minor axis distance (compare the
// 18/7 ring metric of 0x44d350/0x44d840); straight out from an edge it is
// 16 * distance; inside it is 16 * distance to the nearest edge.

// Stays a macro: as a function or an if chain the first argument gets another register.
#define Min_0044dd00(a, b) ((a) < (b) ? (a) : (b))

// FUNCTION: 0x44dd00
int Class_0044dd00::FUN_0044dd00(int x, int y)
{
    int dx;
    if (x < x1) {
        dx = x1 - x;
    }
    else if (x > x2) {
        dx = x - x2;
    }
    else {
        if (y < y1)
            return (y1 - y) * 16;
        if (y > y2)
            return (y - y2) * 16;
        int mx = Min_0044dd00(x - x1, x2 - x);
        int my = Min_0044dd00(y - y1, y2 - y);
        return Min_0044dd00(mx, my) * 16;
    }

    int dy;
    if (y < y1)
        dy = y1 - y;
    else if (y > y2)
        dy = y - y2;
    else
        return dx * 16;

    if (dx > dy)
        return dx * 16 + dy * 6;
    return dy * 16 + dx * 6;
}

// Slot 0 of vtable 0x4fd3e0: writes the flag word and, per flag bit, the
// short fields (or the three dwords) to a bit stream.
// FUNCTION: 0x44ddc0
void Class_0044ddc0::FUN_0044ddc0(BitWriter* stream)
{
    stream->WriteBits(flags, 8);
    if ((flags & 1) != 0) {
        stream->WriteBits(field_10, 0x10);
        stream->WriteBits((int)(unsigned short)(ptr == 0 ? 0 : ptr->field_a8), 0x10);
    }
    if ((flags & 0x10) != 0) {
        stream->WriteBits(field_a, 0x10);
    }
    if ((flags & 8) != 0) {
        stream->WriteBits(field_c, 0x10);
    }
    if ((flags & 0x40) != 0) {
        stream->WriteBits(field_e, 0x10);
    }
    if ((flags & 0x20) != 0) {
        stream->WriteBits(field_26, 0x20);
        stream->WriteBits(field_2a, 0x20);
        stream->WriteBits(field_2e, 0x20);
    }
}
