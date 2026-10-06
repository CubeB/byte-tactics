// Decompiled by Haiku, Opus, Sonnet, Space Bunny Free and DeepSeek V4.1 Flash. Names are provisional.
// Class_0044f010 (vtable 0x4fd458), derived from Class_0044ef20 (see
// order_targets_44ef20.cpp for the family): a path of up to 20 map points
// that the object at +0x4 is moved along, and sent to the other players.
#include <algorithm>
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

// The bit writer of 0x415c10.
class BitWriter {
public:
    int bit;                           // +0x0 current word index
    int index;                         // +0x4 bits used in the current word
    int capacity;                      // +0x8
    unsigned int* data;                // +0xc
    unsigned int buffer[0x100];        // +0x10
    void GrowBuffer();
    void WriteBits(int value, int bits);
};
class BitReader;

#pragma pack(push, 1)
struct Target_0044f1a0 {
    char unknown_0[0x2e];
    unsigned char field_2e;            // +0x2e
};

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

struct Vec3_004907e0 {
    Coord_0044f2a0 x;
    Coord_0044f2a0 y;
    Coord_0044f2a0 z;
};

#pragma pack(push, 2)
struct Struct_004907e0 {               // the owner
    Target_0044f1a0* target;           // +0x0
    char unknown_4[0x5c - 0x4];
    Target_0044f2a0* field_5c;         // +0x5c
    char unknown_60[0x6a - 0x60];
    Vec3_004907e0 pos;                 // +0x6a
};
#pragma pack(pop)

struct Point_0044f080 {
    short x;
    short y;
};

// The object at +0x4 (see victory_490940.cpp); vtable 0x4fd2f8. Slot 8 is the
// "give me your position as a Vec3" call (an implementation is 0x44dc60); slot
// 8 of the base holds _purecall, slot 11 the stub FUN_0044cef0, which returns 0.
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
    virtual int FUN_004e6110(Vec3_004907e0* out);       // slot 8
    virtual int FUN_0044cf40();                         // slot 9
    virtual void FUN_0044cf50();                        // slot 10
    virtual int FUN_0044cef0();                         // slot 11
};

class Class_0044f010;

// Vtable 0x4fd428, constructor 0x44ef20, ??_G 0x44ef60.
class Class_0044ef20 {
public:
    Base_00490a10* field_4;            // +0x4
    Struct_004907e0* owner;            // +0x8

    Class_0044ef20(Struct_004907e0* p) { owner = p; field_4 = 0; }
    virtual ~Class_0044ef20() {}                    // slot 0
    virtual void FUN_0044ef90(void* param);         // slot 1
    virtual void FUN_0044efb0();                    // slot 2
    virtual void FUN_0044ef40(Vec3_004907e0*, int, int);  // slot 3
    virtual void FUN_0044f000(Vec3_004907e0*, Vec3_004907e0*, short*);  // slot 4
    virtual int FUN_0044ef80();                     // slot 5
    virtual Class_0044f010* FUN_0044eff0();         // slot 6
    virtual int FUN_0044efe0();                     // slot 7
    virtual void FUN_0044efc0(BitWriter*);          // slot 8
    virtual void FUN_0044efd0(BitReader*);          // slot 9
    virtual void FUN_0044ef50(void*);               // slot 10
};

// Vtable 0x4fd458, constructor 0x44f010, destructor 0x44f450, ??_G 0x44f040.
// Slots 4 and 9 are inherited.
class Class_0044f010 : public Class_0044ef20 {
public:
    Point_0044f080 points[20];         // +0xc
    int count;                         // +0x5c
    unsigned int field_60;             // +0x60
    union {
        struct {
            unsigned char active : 1;  // +0x64 bit 0
            unsigned char flag_1 : 1;  // bit 1
            unsigned char flag_2 : 1;  // bit 2
            unsigned char flag_3 : 1;  // bit 3, the path changed
        };
        unsigned char field_64;
    };

    Class_0044f010(Struct_004907e0* p);
    virtual ~Class_0044f010();                      // slot 0
    virtual void FUN_0044ef90(void* param);         // slot 1, 0x44f2a0
    virtual void FUN_0044efb0();                    // slot 2, 0x44f1a0
    virtual void FUN_0044ef40(Vec3_004907e0*, int, int);  // slot 3, 0x44f150
    virtual int FUN_0044ef80();                     // slot 5, 0x44f290
    virtual Class_0044f010* FUN_0044eff0();         // slot 6, 0x44f260
    virtual int FUN_0044efe0();                     // slot 7, 0x44f480
    virtual void FUN_0044efc0(BitWriter*);          // slot 8, 0x44f4a0
    // In console_commands_417e00.cpp: it was compiled with the console code,
    // far from the rest of the class.
    virtual void FUN_0044ef50(void*);               // slot 10, 0x417e00
    void FUN_0044f080(Point_0044f080* src, int n);
    void FUN_0044f100(int n);
};

// The constructor: the base constructor is inlined, and its vtable store is
// dead.
// FUNCTION: 0x44f010
Class_0044f010::Class_0044f010(Struct_004907e0* p)
    : Class_0044ef20(p)
{
    count = 0;
    active = 0;
    flag_1 = 0;
    flag_3 = 1;
    field_60 = 0;
}

// Sets the path points (at most 20) and marks it active, or with no
// points asks the object at +0x4 about the owner and flags it (0x40) when that
// fails.
// FUNCTION: 0x44f080
void Class_0044f010::FUN_0044f080(Point_0044f080* src, int n)
{
    if (n == 0) {
        if (field_4 && field_4->FUN_0044cf00(owner) == 0)
            ((Class_0044ced0*)field_4)->FUN_0044ced0(0x40);
        active = 0;
    } else {
        if (n >= 20)
            n = 20;
        count = n;
        std::copy(src, src + n, points);
        active = 1;
    }
    flag_1 = 0;
    flag_3 = 1;
}

// Drops the first n path points, clears the active flag when fewer
// than two are left and sets flag 3.
// FUNCTION: 0x44f100
void Class_0044f010::FUN_0044f100(int n)
{
    if (n != 0) {
        std::copy(points + n, points + count, points);
        count -= n;
        if (count < 2)
            active = 0;
        flag_3 = 1;
    }
}

// Slot 3: fills n positions from the path points, repeating the last point past the
// end of the path.
// FUNCTION: 0x44f150
void Class_0044f010::FUN_0044ef40(Vec3_004907e0* out, int unused, int n)
{
    for (int i = 0; i < n; i++) {
        int j = i < count ? i : count - 1;
        out[i].x.fixed = points[j].x << 16;
        out[i].y.fixed = 0;
        out[i].z.fixed = points[j].y << 16;
    }
}

// Slot 2: tells the object at +0x4 about the owner, then, when the unit has reached
// its second path point (within 5 units of the owner), drops that point with
// an overlapping std::copy and refreshes the flags. std::copy, not memmove:
// memmove stays a library call, std::copy is the loop the original has.
// FUNCTION: 0x44f1a0
void Class_0044f010::FUN_0044efb0()
{
    if (field_4) {
        if (field_4->FUN_0044cf00(owner)) {
            ((Class_0044ced0*)field_4)->FUN_0044ced0(0x20);
            if (!field_4->FUN_0044cef0())
                FUN_0044ef90(0);
        }
    }
    if (count >= 2) {
        int dx = owner->pos.z.half[1] - points[1].y;
        int dy = owner->pos.x.half[1] - points[1].x;
        if (dy * dy + dx * dx <= 25) {
            std::copy(points + 1, points + count, points);
            int n = --count;
            if (n < 2)
                active = 0;
            flag_3 = 1;
        }
    }
    if (field_4 && (owner->target->field_2e & 4 || count < 2))
        flag_1 = 1;
}

// Slot 6: returns this object when its flag bit 1 is set and the counter at
// g_game+0x38a47 has reached field_60 + 0x3c (storing the counter in
// field_60), or null. The path search scheduler (0x40eb70) calls it through
// slot 6 to pick the path to search for next.
// FUNCTION: 0x44f260
Class_0044f010* Class_0044f010::FUN_0044eff0()
{
    if (field_64 & 2) {
        unsigned int limit = g_game->field_38a47;
        if (limit >= field_60 + 0x3c) {
            field_60 = limit;
            return this;
        }
    }
    return 0;
}

// Slot 5: the active flag (bit 0 of +0x64).
// FUNCTION: 0x44f290
int Class_0044f010::FUN_0044ef80()
{
    return field_64 & 1;
}

// Slot 1: hands the object at +0x4 over to the
// path logic, then, unless the path is already active, steps the path
// forwards: it asks the object for its position (its slot 8) and marks the
// path active when the next path point is at most half as far from that
// object as the owner is. With no usable point left it resets the path to
// two points taken from the owner and the object's position.
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
            Vec3_004907e0 p;
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

// The out-of-line destructor: it stores its own vtable, unregisters the object, then the empty inline base destructor
// stores 0x4fd428. Its scalar deleting destructor 0x44f040 inlines it.
// FUNCTION: 0x44f040 ??_GClass_0044f010@@UAEPAXI@Z
// FUNCTION: 0x44f450
Class_0044f010::~Class_0044f010()
{
    g_game->field_14207->FUN_0040e9c0(this);
}

// Slot 7: true when there is something to send: flag 3 of +0x64 (the path changed) is
// set, or flag 2 no longer matches bit 2 of the owner's target. Slot 8
// (0x44f4a0) writes the path out and brings both flags up to date.
// FUNCTION: 0x44f480
int Class_0044f010::FUN_0044efe0()
{
    return (field_64 & 8) || ((owner->target->field_2e ^ field_64) & 4);
}

// Slot 8, the write counterpart of the reader 0x44f5c0. It sets the stream's next bit when the
// unit's mode asks for it, writes the point count in 2 bits, then up to three
// points of 16 bits each, and finally copies the unit's mode into flag 2 while
// clearing flag 3 (the "changed" flag the readers rely on).
// The tail only compiles to the original's `and 0xf3` / `xor` pair when the
// two flag bits are written as 1-bit bitfields: assigning the mode as an int
// value gives `and/or` instead.
// FUNCTION: 0x44f4a0
void Class_0044f010::FUN_0044efc0(BitWriter* stream)
{
    int n;
    if (active) {
        n = count < 3 ? count : 3;
    } else {
        n = 0;
    }
    if (owner->target->field_2e & 4) {
        stream->data[stream->bit] |= 1 << stream->index;
    }
    stream->index++;
    if (stream->index == 0x20) {
        stream->index = 0;
        stream->bit++;
        if (stream->bit == stream->capacity) {
            stream->GrowBuffer();
        }
        stream->data[stream->bit] = 0;
    }
    stream->WriteBits(n, 2);
    for (int i = 0; i < n; i++) {
        stream->WriteBits(points[i].x, 0x10);
        stream->WriteBits(points[i].y, 0x10);
    }
    flag_2 = (owner->target->field_2e & 4) != 0;
    flag_3 = 0;
}
