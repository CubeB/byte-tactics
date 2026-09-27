// Decompiled by space-bunny-free. Names are provisional.
// Still differs in two places (see the note above the FUNCTION line):
//  1. the copy of the unit position into the state (the original loads the two
//     halves as a dword plus a word through a `lea`ed address of state->pos,
//     mine copies a 4-byte member and a separate short), and
//  2. the tail of the inlined tree walk: the original calls FUN_0045b030 for
//     the next sibling, mine jumps back to the top of the inlined loop.
//
// On hunk 1, the obvious reading is that the original assigns a 6-byte struct
// (dword at +0, short at +4) rather than copying two fields, since that is what
// the guide's "struct copy vs field copies" note predicts for a `lea`ed
// destination. Tried: wrapping the Vec2 and the short in a packed 6-byte
// Pos3_0045ab10 member on both sides and assigning it whole. MSVC does lower it
// as a struct copy with both leas, but symmetrically and through edi, and the
// score falls to 76.7 percent because the following `state->root` load is then
// emitted twice (`mov eax, [ecx+0x1e]` before and `mov edx, [ecx+0x1e]` after)
// where the original loads it once into edx, and the flags bitfield test moves
// from dl to al. The original also interleaves `mov [ecx+8],1` between the dword
// and the word halves of the copy, which the struct assignment does not
// reproduce either. Reverted; the two separate field copies here score better.
// Snaps the object state to the unit's own position when the two differ by 8
// or more on any axis, marks the state dirty and clears the root entry's
// "modified" flag, then, when the state is dirty, restores the piece tree's
// vertices, rebuilds the root entry and clears the dirty flag.
#include <stdlib.h>
#include <string.h>

#pragma pack(push, 2)

struct Class_0045ae80 {
    char unknown_0[4];
    int point_count;                    // +0x4
    char unknown_8[0x24 - 0x8];
    void* points;                       // +0x24
    char unknown_28[0x2c - 0x28];
    Class_0045ae80* sibling;             // +0x2c
    Class_0045ae80* child;               // +0x30
};

struct Flags_0045ab10 {
    unsigned short f0 : 1;               // +0x28
    unsigned short f1 : 1;
};

struct Entry_0045ab10 {
    Class_0045ae80* object;              // +0x00
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

struct Vec2_0045ab10 {
    short x;                             // +0x0
    short y;                             // +0x2
};

struct State_0045ab10 {
    int count;                           // +0x0
    int field_4;                         // +0x4
    int field_8;                         // +0x8
    int field_c;                         // +0xc
    char unknown_10[0x18 - 0x10];
    Vec2_0045ab10 pos;                   // +0x18
    short z;                             // +0x1c
    Entry_0045ab10* root;                // +0x1e
    Entry_0045ab10 entries[1];           // +0x22
};

struct Unit_0045ab10 {
    char unknown_0[0x64];
    Vec2_0045ab10 pos;                   // +0x64
    short z;                             // +0x68
    char unknown_6a[0x9e - 0x6a];
    State_0045ab10* state;               // +0x9e
};

#pragma pack(pop)

// Restores the vertices of every modified piece in the tree (or of every
// piece, when `force` is set) from the object and clears its offset. Defined
// here so that /Ob2 inlines its first iteration into FUN_0045ab10, as the
// original does; the same body is in src/unsorted/0x45b030.cpp.
int __fastcall FUN_0045b030(Entry_0045ab10* piece, int force)
{
    int result;
    do {
        result = force;
        if (piece->unknown_26 == 0 || force) {
            memcpy(piece->points, piece->object->points, piece->object->point_count * 12);
            piece->offset_x = 0;
            piece->offset_y = 0;
            piece->offset_z = 0;
            piece->unknown_26 = 0;
            result = 1;
        }
        if (piece->child) {
            result = FUN_0045b030(piece->child, result);
        }
        piece = piece->sibling;
    } while (piece);
    return result;
}

void __fastcall FUN_0045b0a0(State_0045ab10* state, Entry_0045ab10* entry, int flag);

// FUNCTION: 0x45ab10
void __stdcall FUN_0045ab10(Unit_0045ab10* param_1)
{
    State_0045ab10* state = param_1->state;
    if (abs((short)(state->z - param_1->z)) >= 8
        || abs((short)(state->pos.y - param_1->pos.y)) >= 8
        || abs((short)(state->pos.x - param_1->pos.x)) >= 8) {
        state->pos = param_1->pos;
        state->field_8 = 1;
        state->z = param_1->z;
        state->root->unknown_26 = 0;
        if (state->root->flags.f1) {
            state->field_4 = 0;
        }
    }
    if (param_1->state->field_8 != 0) {
        FUN_0045b030(param_1->state->root, 0);
        FUN_0045b0a0(param_1->state, param_1->state->root, 0);
        param_1->state->field_8 = 0;
    }
}
