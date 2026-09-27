// Decompiled by space-bunny-free, finished by space-bunny-free. Names are provisional.
//
// 79.4 percent. Everything in the loop body matches except the register the
// reload-time maximum lives in: the original keeps it in its frame slot at
// +0x14 and reloads it into ebx for the two calls, while this version keeps
// it in ebp and puts the array induction variable in ebx. Everything the
// difference cascades into follows from that (the team byte in bl vs cl, the
// `sub edx, ebx` operand order, `and al, 0xfd` moving one instruction, and
// the `lea eax, [eax+eax*4]` chain after the loop reloading the maximum).
//
// What the register allocator is doing (worth knowing before trying more
// shapes): the callee-saved pool is taken in the order edi, esi, ebp, ebx.
// Both versions give edi to the unit pointer and esi to the walking slot
// pointer. The original then gives ebp to the strength-reduced `i * 4` and
// leaves the maximum in memory, even though ebx is then free for the whole
// body (it only holds the team byte and the reloaded maximum). So the
// original's maximum was never a register candidate at all, while here it is
// considered before the induction variable and takes ebp. The frame layout
// (+0x0 counter byte, +0x4 maximum, +0x8 and +0x14 the two Vec3s) matches.
//
// Everything tried to keep the maximum in memory, all of which MSVC 5
// promotes back into a callee-saved register: an `int *` to the member, an
// `int&` parameter on a static inline helper, `memset(&frame, 0, 8)`, a
// one-element local array of the struct, a struct returned by value from an
// inline helper, a class with a user-defined constructor (the /Ob2 inliner
// expands it and the address-taken flag is then dropped), and a helper whose
// body is too big to fold away. Two shapes do move it into memory but cost
// more elsewhere: a by-value `Acc Bump(Acc, int)` helper puts the maximum in
// edi and moves the unit pointer to ebp (62 percent), and storing the address
// in a file-scope static leaves the store in the code (36 percent). Also
// tried, all 79.4 percent: separate locals instead of the shared 8-byte
// struct, `while` and `do` loops, the update first in the body, a swapped
// comparison, a `unsigned short` temporary, a two-element Vec3 array, the
// Vec3s declared before the loop, and a `long` maximum. See the note on
// Frame_0049e070 below for what the slot order depends on.
//
// Later pass, three more attempts, all worse, so do not repeat:
// - A single temp for the two call arguments, `int m = frame.maxTime;` then
//   passing `m` to both calls. This looks right, because the original loads
//   ebx from [esp+0x14] once at 0x49e0d1 and pushes that same value for both
//   calls, but MSVC promotes the temp to ebp and drops the memory slot for
//   maxTime entirely (`xor ebx,ebx` replaces the initialising store), so the
//   maximum ends up wholly in a register, which is the opposite of the
//   original. 79.4 percent with a different cascade.
// - `volatile int maxTime` as a member of Frame_0049e070. The evidence looks
//   right (every write to the original's slot goes to memory and all three
//   reads re-load it), but making one member volatile changes how the shared
//   8-byte local is handled: the counter is then kept in bl rather than
//   round-tripped through [esp+0x10], and the Vec3 slots shift. 75.4 percent.
// - The same as a separate `volatile int` local instead of a struct member:
//   68.1 percent.
//
// So the memory residency of the maximum is not something the source can ask
// for directly, and it is not a volatile effect. It is a by-product of the
// register auction: the induction wins ebp, and once the maximum has lost that
// auction there is no callee-saved register left to hold it across the two
// calls, so it stays in its slot and ebx (free between uses) carries it. The
// lever, if there is one, has to make the induction outrank the maximum in the
// auction without adding register pressure of its own.
//
// Note: the third argument of FUN_0043e240 and FUN_0043e2e0 is the weapon
// index (see 0x43e240.cpp and 0x49d910.cpp, where it indexes a three-entry
// name array), and this function passes the reload-time maximum there. That
// looks like a mistake in the original: the maximum is a time, not a weapon
// number, and anything above 2 indexes past "Tertiary".
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
