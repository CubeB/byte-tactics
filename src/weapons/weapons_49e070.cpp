// Decompiled by space-bunny-free, finished by DeepSeek V4.1 Flash. Names are provisional.
//
// Sets up a unit's three weapon slots: clears each slot's word at +0x8, stores
// the slot's `attached` unit from the type's attached[3] array into +0x0,
// encodes the slot index into flags bits 2-3 and whether the attached unit's
// team is non-zero into bit 3, then asks for the weapon's aim from the unit's
// position (0x43e240) and from the target direction (0x43e2e0), storing the
// pitch difference between the two as the reload time at +0x4. The largest
// reload time over the three slots is handed to the script as
// "SetMaxReloadTime" in ticks (maxTime * 1000 / 30).
//
// What made this match (the previous 79.4 percent version had the maximum in
// ebp and the index in ebx, i.e. the opposite of the original):
//
// - The third argument of FUN_0043e240 / FUN_0043e2e0 is the loop index, not
//   the reload-time maximum. The parameters are `unsigned char` (see
//   0x43e240.cpp and 0x49d910.cpp), and for a char parameter MSVC 5 passes the
//   dword at the char's stack slot without widening it, so the original's
//   `mov ebx, [esp + 0x14]` at 0x49e0d1 (esp is 4 lower there because arg 4 was
//   pushed first) reads the loop counter's slot. Declaring the third parameter
//   `int` instead makes the compiler hoist the counter into a register.
// - Folding the `...->team` read directly into the second flags expression,
//   with no named team temporary, is what keeps the weapon-index load late
//   (`mov ebx, [esp + 0x14]` after `setne dl`) and the team byte in bl. With a
//   named temporary the allocator grabs ebx for the weapon value and the team
//   lands in cl, dragging the load and the push order with it.
// - Putting `s->field_e = 0` after the second flags assignment is what places
//   the byte store at 0x49e0c8, between the team load and the test. Before the
//   flags assignment it is hoisted above the `attached` store.
//
// The frame layout depends on the counter byte and the maximum sharing one
// 8-byte local (`Frame_0049e070`, initialized `{0, 0}`). Two separate locals
// put the counter in a slot of its own and shift the Vec3s.
#include <windows.h>

struct Vec3_0049e070 {
    int x;
    int y;
    int z;
};

class Class_004b0a70 {
public:
    int FUN_004b0a70(char* name, void* param_2, int param_3, int param_4,
                     int param_5, int param_6, int param_7, int param_8);
};

struct Unit_0049e070;

#pragma pack(push, 1)
struct Slot_0049e070 {
    Unit_0049e070* attached;         // +0x0
    int field_4;                     // +0x4
    short field_8;                   // +0x8
    char unknown_a[0xe - 0xa];
    unsigned char field_e;           // +0xe
    unsigned char flags;             // +0xf
    char unknown_10[0x1c - 0x10];
};

struct UnitType_0049e070 {
    char unknown_0[0x1ee];
    Unit_0049e070* attached[3];      // +0x1ee
};

struct Unit_0049e070 {
    char unknown_0[0x10];
    Slot_0049e070 slots[3];          // +0x10
    char unknown_64[0x92 - 0x64];
    UnitType_0049e070* type;         // +0x92
    char unknown_96[0x9a - 0x96];
    Class_004b0a70* script;          // +0x9a
    char unknown_9e[0xe4 - 0x9e];
    unsigned short field_e4;         // +0xe4
    char unknown_e6[0x10a - 0xe6];
    unsigned char team;              // +0x10a
};
#pragma pack(pop)

// The loop counter and the reload-time maximum share one 8-byte local; the
// counter sits at +0x0 and the maximum at +0x4.
struct Frame_0049e070 {
    unsigned char i;                  // +0x0
    int maxTime;                      // +0x4
};

void __stdcall FUN_0043e240(Unit_0049e070*, Vec3_0049e070*, unsigned char, int);
void __stdcall FUN_0043e2e0(Unit_0049e070*, Vec3_0049e070*, unsigned char);

// FUNCTION: 0x49e070
void __stdcall FUN_0049e070(Unit_0049e070* unit)
{
    Frame_0049e070 frame = {0, 0};
    for (frame.i = 0; frame.i < 3; frame.i++) {
        Slot_0049e070* s = &unit->slots[frame.i];
        s->field_8 = 0;
        s->flags = (s->flags & 0xf2) | ((frame.i & 3) << 2);
        s->attached = unit->type->attached[frame.i];
        s->flags = (s->flags & 0xfd) | (((unit->type->attached[frame.i]->team != 0) & 1 | 8) * 2);
        s->field_e = 0;
        Vec3_0049e070 a;
        FUN_0043e240(unit, &a, frame.i, -1);
        Vec3_0049e070 b;
        FUN_0043e2e0(unit, &b, frame.i);
        s->field_4 = (a.z - b.z) * 1.25;
        if (s->attached->field_e4 > frame.maxTime)
            frame.maxTime = s->attached->field_e4;
    }
    unit->script->FUN_004b0a70("SetMaxReloadTime", 0, 0, 1, frame.maxTime * 1000 / 30, 0, 0, 0);
}
