// Decompiled by space-bunny-free. Names are provisional.
//
// 79.4 percent, and every remaining difference is one thing: the original
// keeps the reload-time maximum in its frame slot at +0x14 and loads it into
// ebx for the two calls, while this version keeps it in ebp and the array
// induction variable in ebx. That swaps the registers the rest of the loop
// body uses (the team byte, the subtraction, the comparison) and adds the
// missing `mov [esp+0x14], ebp` in the preheader. Nothing I tried makes MSVC
// 5 spill it: a 5-byte/4-byte local struct, an address-taken reference, an
// extra use, a windows.h min/max, a loop-carried pointer, and an escaping
// local struct all leave it in a callee-saved register. The escaping struct
// that does put it in memory (+0x14, and the induction variable in ebp, as
// the original has it) also promotes the loop counter into al, which changes
// the loop shape and drops the score to 62 percent. See the note on
// Frame_0049e070 below for what the slot order depends on.
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

// The loop counter and the reload-time maximum share one 8-byte local. Two
// separate locals leave the counter a slot of its own at the top of the
// frame, which shifts every other slot down by four.
struct Frame_0049e070 {
    unsigned char i;                  // +0x0
    int maxTime;                      // +0x4
};

void __stdcall FUN_0043e240(Unit_0049e070*, Vec3_0049e070*, int, int);
void __stdcall FUN_0043e2e0(Unit_0049e070*, Vec3_0049e070*, int);

// FUNCTION: 0x49e070
void __stdcall FUN_0049e070(Unit_0049e070* unit)
{
    Frame_0049e070 frame = {0, 0};
    for (frame.i = 0; frame.i < 3; frame.i++) {
        Slot_0049e070* s = &unit->slots[frame.i];
        s->field_8 = 0;
        s->flags = (s->flags & 0xf2) | ((frame.i & 3) << 2);
        s->attached = unit->type->attached[frame.i];
        // Read into a local: that is what keeps the byte load in a register
        // between the +0xe store and the test, instead of folding it into a
        // compare against memory.
        unsigned char team = unit->type->attached[frame.i]->team;
        s->field_e = 0;
        s->flags = (s->flags & 0xfd) | (((team != 0) & 1 | 8) * 2);
        Vec3_0049e070 a;
        FUN_0043e240(unit, &a, frame.maxTime, -1);
        Vec3_0049e070 b;
        FUN_0043e2e0(unit, &b, frame.maxTime);
        s->field_4 = (a.z - b.z) * 1.25;
        if (s->attached->field_e4 > frame.maxTime)
            frame.maxTime = s->attached->field_e4;
    }
    unit->script->FUN_004b0a70("SetMaxReloadTime", 0, 0, 1, frame.maxTime * 1000 / 30, 0, 0, 0);
}
