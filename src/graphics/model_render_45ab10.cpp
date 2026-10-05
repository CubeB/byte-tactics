// Decompiled by Space Bunny Free, finished by Claude Opus 5.5. Names are provisional.
// Snaps the object state to the unit's own position when the two differ by 8 or
// more on any axis, marks the state dirty and clears the root entry's "modified"
// flag, then, when the state is dirty, restores the piece tree's vertices,
// rebuilds the root entry and clears the dirty flag. 0x45ac20 inlines this
// whole function twice.
//
// What made it match:
//  - The distance test is an inline helper taking the state and a pointer to
//    the unit's position. This was the missing piece. Written out in the
//    function, or with the helper taking (state, unit) or (position, unit),
//    MSVC gives the unit ebp and keeps no zero register; with the helper
//    taking both positions by pointer, the registers are right but the
//    position copy loses its `lea edx, [ecx+0x18]`.
//  - FUN_0045b030 is written recursively (the sibling is a tail call). /Ob2
//    inlines one level of it here, which is why the original calls it for the
//    child and again for the sibling (`xor edx, edx ; mov ecx, ebx ; call`)
//    instead of looping. Out of line, MSVC turns the tail call into the loop
//    at 0x45b030, byte for byte (checked against 0x45b030 in scratch). Once
//    the helper above is in place, that level written out by hand (calling
//    a declared FUN_0045b030) matches too; the loop form never can.
//  - The position is one 6-byte struct copy.
#include <stdlib.h>
#include <string.h>

#pragma pack(push, 2)

struct Class_0045ae80 {
    char unknown_0[4];
    int point_count;                    // +0x4
    char unknown_8[0x24 - 0x8];
    void* points;                       // +0x24
    char unknown_28[0x2c - 0x28];
    Class_0045ae80* sibling;            // +0x2c
    Class_0045ae80* child;              // +0x30
};

struct Flags_0045ab10 {
    unsigned short f0 : 1;              // +0x28
    unsigned short f1 : 1;
};

struct Entry_0045ab10 {
    Class_0045ae80* object;             // +0x00
    char unknown_4[0x16 - 0x4];
    int offset_x;                       // +0x16
    int offset_y;                       // +0x1a
    int offset_z;                       // +0x1e
    void* points;                       // +0x22
    short unknown_26;                   // +0x26
    Flags_0045ab10 flags;               // +0x28
    Entry_0045ab10* sibling;            // +0x2a
    Entry_0045ab10* child;              // +0x2e
    Entry_0045ab10* parent;             // +0x32
};

struct Vec3s_0045ab10 {
    short x;                            // +0x0
    short y;                            // +0x2
    short z;                            // +0x4
};

struct State_0045ab10 {
    int count;                          // +0x0
    int field_4;                        // +0x4
    int field_8;                        // +0x8 dirty
    int field_c;                        // +0xc
    char unknown_10[0x18 - 0x10];
    Vec3s_0045ab10 pos;                 // +0x18
    Entry_0045ab10* root;               // +0x1e
    Entry_0045ab10 entries[1];          // +0x22
};

struct Unit {
    char unknown_0[0x64];
    Vec3s_0045ab10 pos;                 // +0x64
    char unknown_6a[0x9e - 0x6a];
    State_0045ab10* state;              // +0x9e
};

#pragma pack(pop)

// Restores the vertices of every modified piece in the tree (or of every
// piece, when `force` is set) from the object and clears its offset. Defined
// here so that /Ob2 inlines its first level into FUN_0045ab10, as the
// original does; src/graphics/model_render_45b030.cpp spells the same function as the
// loop MSVC makes of it.
int __fastcall FUN_0045b030(Entry_0045ab10* piece, int force)
{
    int result = force;
    if (piece->unknown_26 == 0 || force) {
        memcpy(piece->points, piece->object->points, piece->object->point_count * 12);
        piece->offset_x = 0;
        piece->offset_y = 0;
        piece->offset_z = 0;
        piece->unknown_26 = 0;
        result = 1;
    }
    if (piece->child)
        result = FUN_0045b030(piece->child, result);
    if (piece->sibling)
        return FUN_0045b030(piece->sibling, force);
    return result;
}

void __fastcall FUN_0045b0a0(State_0045ab10* state, Entry_0045ab10* entry, int flag);

// Nonzero when the state's position is 8 or more away from `pos` on any axis.
static inline int FarFrom(State_0045ab10* state, const Vec3s_0045ab10* pos)
{
    return abs((short)(state->pos.z - pos->z)) >= 8
        || abs((short)(state->pos.y - pos->y)) >= 8
        || abs((short)(state->pos.x - pos->x)) >= 8;
}

// FUNCTION: 0x45ab10
void __stdcall FUN_0045ab10(Unit* unit)
{
    State_0045ab10* state = unit->state;
    if (FarFrom(state, &unit->pos)) {
        state->pos = unit->pos;
        state->field_8 = 1;
        state->root->unknown_26 = 0;
        if (state->root->flags.f1) {
            state->field_4 = 0;
        }
    }
    if (unit->state->field_8 != 0) {
        FUN_0045b030(unit->state->root, 0);
        FUN_0045b0a0(unit->state, unit->state->root, 0);
        unit->state->field_8 = 0;
    }
}
