// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Slot 13 of UnitScript (vtable 0x4fd698), overriding
// CobScript::ExplodePiece; see 0x485e30.cpp and the sibling slots
// 0x480ce0 / 0x480b20 for the class and its +0x540 data. The class views
// below declare only this slot's virtual: with all 21 of the base's virtuals
// declared (as in 0x485e30.cpp) the merge block's registers come out
// differently.
//
// Two independent blocks selected by bits of the second argument `b`:
//  - !(b & 0x20): builds a 0x30-byte header on the stack (the same record
//    FUN_00421620 consumes) from the data->unit pointer at +0x0c, the first
//    argument and six FUN_004b6c30 random draws, then hands it to
//    FUN_00421620.
//  - (b & 0x3f00): computes the unit's position with GetPiecePosition and appends
//    it to up to six tables in g_game (+0x147f7, a six-pointer array) with
//    FUN_00420a30(&v, table, 2, 0).
//
// `b` must be unsigned (so CobScript declares the slot with an unsigned
// int too): the merge block's `(b >> 2)` / `(b >> 4)` are `shr`, not `sar`.
// The header's flag dword at +0x28 is a plain unsigned int, not a bitfield:
// assigning `(b & 2) << 4` to a 1-bit field would truncate it to zero, while
// the original ORs the shifted value straight in.
//
// <string.h> is load-bearing for the register allocation: without any header
// the merge block keeps the accumulator in eax (`or eax, edx`) and reads
// `(b & 2) << 4` into edx; with it the shifted value takes eax and the
// accumulator is the source, exactly as in the original. <windows.h> works
// too; <stdio.h> and <math.h> alone do not.
//
// The two arms of the if/else keep their h.f24 store in different places:
// the taken arm stores 1 after the OR (and before the trailing `or al, 4`),
// the else arm stores 0 before the OR. Swapping only the else arm's two
// statements is what reproduces the original's scheduling.
//
// Suspected original bug: h.bits is read before it is ever written (the
// first `h.bits & ~0x30` in each arm, and the three merge assignments, all
// load the uninitialised local). Only bits 6 and up survive the masks, so
// whatever the stack held leaks into the record FUN_00421620 copies.
#include <string.h>

#pragma pack(push, 1)

struct Unit {
    char unknown_0[0xba];
    unsigned short unknown_ba : 2;
    unsigned short dirty : 1;
};

struct Data_00481140 {
    int unknown_0;
    int unknown_4;
    int dirty;
    Unit* unit;                        // +0x0c
};

struct Game {
    char unknown_0[0x147f7];
    void* sources[6];                  // +0x147f7
};

struct Header_00481140 {
    void* obj;                         // +0x00
    int index;                         // +0x04
    int r1;                            // +0x08
    int r2;                            // +0x0c
    int r3;                            // +0x10
    int x;                             // +0x14
    int y;                             // +0x18
    int z;                             // +0x1c
    int f20;                           // +0x20
    int f24;                           // +0x24
    unsigned int bits;                 // +0x28
    void* field_2c;                    // +0x2c
};
#pragma pack(pop)

struct Vec3_00481140 { int x, y, z; };

extern Game* g_game;

int __stdcall FUN_004b6c30(int range);
void __stdcall FUN_00421620(Header_00481140* h);
Vec3_00481140 __stdcall GetPiecePosition(Unit* obj, int param);
void __stdcall FUN_00420a30(void* pos, void* src, int index, int flag);

struct Elem_4b0610 {
    int value;         // +0x0
    char pad[0xa0];    // pad to stride 0xa4
};

class CobScript {
public:
    int field_4;                   // +0x4
    int field_8;                   // +0x8
    char unknown_c[0x10 - 0xc];
    void* ptr10;                   // +0x10
    void* ptr14;                   // +0x14
    char unknown_18[0x1c - 0x18];
    Elem_4b0610 arr[8];            // +0x1c
    int field_53c;                 // +0x53c

    virtual void ExplodePiece(int, unsigned int);     // slot 13
};

class UnitScript : public CobScript {
public:
    Data_00481140* data;           // +0x540

    virtual void ExplodePiece(int, unsigned int);     // slot 13, 0x481140
};

// FUNCTION: 0x481140
void UnitScript::ExplodePiece(int a, unsigned int b)
{
    if (!(b & 0x20)) {
        Header_00481140 h;
        h.obj = data->unit;
        h.index = a;
        h.r1 = FUN_004b6c30(3000);
        h.r2 = FUN_004b6c30(3000);
        h.r3 = FUN_004b6c30(3000);
        h.x = (0x14 - FUN_004b6c30(0x28)) << 14;
        h.y = FUN_004b6c30(10) << 16;
        h.z = (0x14 - FUN_004b6c30(0x28)) << 14;
        h.f20 = 900;
        if (b & 1) {
            h.bits = (h.bits & ~0x30) | ((b & 2) << 4);
            h.f24 = 1;
            h.bits |= 4;
        } else {
            h.f24 = 0;
            h.bits = (h.bits & ~0x34) | ((b & 2) << 3);
        }
        h.bits = (h.bits & ~2) | ((b >> 2) & 2);
        h.bits = (h.bits & ~1) | ((b >> 4) & 1);
        h.bits = (h.bits & ~8) | ((b & 4) << 1);
        FUN_00421620(&h);
    }
    if (b & 0x3f00) {
        Vec3_00481140 v;
        v = GetPiecePosition(data->unit, a);
        if (b & 0x100)
            FUN_00420a30(&v, g_game->sources[0], 2, 0);
        if (b & 0x200)
            FUN_00420a30(&v, g_game->sources[1], 2, 0);
        if (b & 0x400)
            FUN_00420a30(&v, g_game->sources[2], 2, 0);
        if (b & 0x800)
            FUN_00420a30(&v, g_game->sources[3], 2, 0);
        if (b & 0x1000)
            FUN_00420a30(&v, g_game->sources[4], 2, 0);
        if (b & 0x2000)
            FUN_00420a30(&v, g_game->sources[5], 2, 0);
    }
}
