// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash. Names are provisional.
// Slot 4 of TeleportParticles (vtable 0x4fd588; see 0x472d50.cpp and 0x471430.cpp
// for the class and 0x471cc0.cpp for the family). The container at +0xc is a
// real std::vector<Class_00473590> (0x34-byte elements, the type 0x472e30.cpp
// and 0x471430.cpp already use). It first makes room for however many ten-tick
// units field_4 has fallen behind the current tick with items.reserve(...),
// then appends one element built from the three positions plus a random value,
// and finally sets field_8 ten ticks ahead of the current tick.
//
// Letting the compiler inline <vector>'s reserve is what reproduces the
// original byte for byte, including the allocator read that spills _First into
// a dead stack slot just after the delete[] argument is pushed (the extra
// `mov [esp+0x18], eax` that hand-writing the same block leaves out). The
// capacity check inside reserve is the vector's unsigned `capacity() < _N`, so
// it is a `jae`, and `_End - _First` on char pointers divides by 52 only once.
//
// push_back would inline the whole three-argument insert here (932 bytes,
// wrong); the original keeps it out of line at 0x4758c0. Naming the vector
// through the List_004737c0 layout and calling FUN_004758c0 on it keeps the
// call out of line and lands the loop in the original's registers: &items in
// esi, &pos1 in ebp and the (single iteration) counter in edi.
#include <stddef.h>
#include <stdlib.h>
#include <vector>

struct Vec3_004737c0 {
    int x;
    int y;
    int z;
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x147f3];
    void* unknown_147f3;                // +0x147f3
    char unknown_147f7[0x38a47 - 0x147f7];
    int field_38a47;                    // +0x38a47
};
#pragma pack(pop)

extern Game* g_game;

extern "C" int __stdcall GetGafFrameCount(void* ptr);

class Class_00473590 {                   // one element, 0x34 bytes
public:
    void* field_0;
    Vec3_004737c0 pos1;
    Vec3_004737c0 pos2;
    Vec3_004737c0 dir;
    int field_28;
    int field_2c;
    int field_30;
};

// Same three pointers as std::vector's _First/_Last/_End.
class List_004737c0 {
public:
    char* head;                         // +0x00
    char* first;                        // +0x04
    char* last;                         // +0x08
    char* end;                          // +0x0c

    void FUN_004758c0(char* where, int count, const Class_00473590& val);
};

class TeleportParticles {
public:
    virtual void FUN_00471430();        // slot 0
    virtual void Update();              // slot 1
    virtual void FUN_00472e30(int);     // slot 2
    virtual int FUN_00472e70();         // slot 3
    virtual void Emit();                // slot 4
    virtual int FUN_00472e00();         // slot 5
    virtual void FUN_004736e0(int, int, int); // slot 6

    int field_4;                        // +0x04
    int field_8;                        // +0x08
    std::vector<Class_00473590> items;  // +0x0c (_First +0x10)
    int field_1c;                       // +0x1c
    Vec3_004737c0 pos1;                 // +0x20
    Vec3_004737c0 pos2;                 // +0x2c
    Vec3_004737c0 dir;                  // +0x38
};

// FUNCTION: 0x4737c0
void TeleportParticles::Emit()
{
    int grow = (field_4 - g_game->field_38a47 + 10) / 10;

    if (grow > 0)
        items.reserve(grow + items.size());

    for (int i = 0; i < 1; i++) {
        Class_00473590 e;

        e.pos1 = pos1;
        e.pos2 = pos2;
        e.dir = dir;
        e.field_30 = g_game->field_38a47 + field_1c;
        e.field_0 = g_game->unknown_147f3;
        e.field_28 = GetGafFrameCount(g_game->unknown_147f3) - 1;
        e.field_2c = (int)(((__int64)rand() * e.field_28) / 0x8000);
        List_004737c0* v = (List_004737c0*)&items;
        v->FUN_004758c0(v->last, 1, e);
    }

    field_8 = g_game->field_38a47 + 10;
}
