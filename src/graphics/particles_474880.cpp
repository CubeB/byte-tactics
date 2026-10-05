// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Slot 4 of Class_004717e0 (vtable 0x4fd5f8; the family is listed in
// 0x471cc0.cpp). It makes room in the std::vector at +0xc for one 68-byte
// element per tick field_4 has fallen behind (the period is 1), then appends
// one element built from the object's three points, with the first point
// jittered by up to 3 units on each axis, and finally sets field_8 one tick
// ahead of the current tick.
// The append runs once inside a countdown loop: MSVC 5 keeps the
// `do { ... } while (--i)` as a counter (i = 1) because the body is large,
// exactly as in the sibling 0x4751c0. The element is a 12-byte point made of
// three 16.16 fixed-point pairs of shorts, so the jitter is three `+=` on the
// high half, and the vector insert stays out of line at 0x475ef0.
#include <stddef.h>
#include <stdlib.h>
#include <vector>

// 16.16 fixed point seen as a pair of shorts; the jitter lands in the high one.
struct Pair_00474880 {
    short lo;
    short hi;
};
struct Vec3_00474880 {
    Pair_00474880 x;
    Pair_00474880 y;
    Pair_00474880 z;
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x147cf];
    void* unknown_147cf;                 // +0x147cf
    char unknown_147d3[0x38a47 - 0x147d3];
    int ticks;                           // +0x38a47
};
#pragma pack(pop)

extern Game* g_game;

// Vtable 0x4fd5a8, constructor 0x471cc0, destructor 0x471d00, ??_G 0x471cd0.
class Class_00471cc0 {
public:
    int field_4;                                        // +0x4

    Class_00471cc0();
    virtual ~Class_00471cc0();                          // slot 0
    virtual void FUN_00472d50() = 0;                    // slot 1
    virtual void FUN_00472e30(int) = 0;                 // slot 2
    virtual int FUN_00472e70() = 0;                     // slot 3
};

class Class_004745e0 {                 // one element, 0x44 bytes
public:
    void* data;                        // +0x00
    Vec3_00474880 pos;                 // +0x04
    Vec3_00474880 pos2;                // +0x10
    Vec3_00474880 vel;                 // +0x1c
    int field_28;                      // +0x28
    int field_2c;                      // +0x2c
    int field_30;                      // +0x30
    int field_34;                      // +0x34
    int field_38;                      // +0x38
    int field_3c;                      // +0x3c
    int field_40;                      // +0x40
};

// Same three pointers as std::vector's _First/_Last/_End.
class List_00474880 {
public:
    char* head;                        // +0x00
    char* first;                       // +0x04
    char* last;                        // +0x08
    char* end;                         // +0x0c

    void FUN_00475ef0(char* where, int count, const Class_004745e0& val);
};

// Vtable 0x4fd5f8, ??_G 0x4717e0; 0x48 bytes.
class Class_004717e0 : public Class_00471cc0 {
public:
    int field_8;                                        // +0x8
    std::vector<Class_004745e0> items;                  // +0xc (_First +0x10)
    int field_1c;                                       // +0x1c
    Vec3_00474880 pos_a;                                // +0x20
    Vec3_00474880 pos_b;                                // +0x2c
    Vec3_00474880 dir;                                  // +0x38
    int field_44;                                       // +0x44

    Class_004717e0() {}
    virtual void FUN_00472d50();                        // slot 1, 0x473170
    virtual void FUN_00472e30(int);                     // slot 2, 0x473250
    virtual int FUN_00472e70();                         // slot 3, 0x473290
    virtual void FUN_00474880();                        // slot 4, 0x474880
    virtual int FUN_00473220();                         // slot 5, 0x473220
    virtual void FUN_00474760(int, int, int, int, int); // slot 6, 0x474760
};

// FUNCTION: 0x474880
void Class_004717e0::FUN_00474880()
{
    int grow = field_4 - g_game->ticks + 1;

    if (grow > 0)
        items.reserve(grow + items.size());

    int i = 1;
    do {
        Class_004745e0 e;

        e.field_38 = 0;
        e.field_3c = field_1c;
        e.field_40 = g_game->ticks + field_1c * 6;
        e.pos = pos_a;
        e.pos.x.hi += (int)((__int64)rand() * 7 / 0x8000) - 3;
        e.pos.y.hi += (int)((__int64)rand() * 7 / 0x8000) - 3;
        e.pos.z.hi += (int)((__int64)rand() * 7 / 0x8000) - 3;
        e.pos2 = pos_b;
        e.vel = dir;
        e.data = g_game->unknown_147cf;
        e.field_28 = 0x61;
        e.field_2c = 0x67;
        if (field_44) {
            e.field_30 = 0x61;
            e.field_34 = 1;
        } else {
            e.field_30 = 0x67;
            e.field_34 = -1;
        }
        List_00474880* v = (List_00474880*)&items;
        v->FUN_00475ef0(v->last, 1, e);
    } while (--i);

    field_8 = g_game->ticks + 1;
}
