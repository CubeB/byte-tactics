// Decompiled by space-bunny-free. Names are provisional.
// Snaps the object state to the unit's own position when the two differ by 8 or
// more on any axis, marks the state dirty and clears the root entry's "modified"
// flag, then, when the state is dirty, restores the piece tree's vertices,
// rebuilds the root entry and clears the dirty flag.
//
// Still differs in two places, both inside the walk/copy above the FUNCTION
// line, so this file is a partial match (86.5 percent):
//
//  1. The copy of the unit position into the state. The original does
//         mov eax, [esi+0x64] ; lea edx, [ecx+0x18] ; mov [ecx+0x18], eax
//         mov ax, [esi+0x68]  ; mov [ecx+8], 1      ; mov [edx+4], ax
//     i.e. the source stays folded off the unit register, the destination
//     address is a register (edx) used by the last store only, and the dword
//     temp is eax. Every 6-byte struct spelling (plain assignment, nested
//     {Vec2; short}, memcpy, a by-value or by-reference setter, `*this = v`
//     in a vec method, a user copy constructor, a struct-pointer local, source
//     and destination locals) lowers to two leas instead
//     (lea edx, [esi+0x64] ; lea eax, [ecx+0x18]) with the dword temp in
//     edi/esi, which then also emits `state->root` twice and moves the flags
//     bitfield test from dl to al: 78.9 percent. The same copy in a small
//     standalone function does produce the original's shape (one lea, folded
//     source, temps in eax/ax), so this looks like register pressure: with the
//     walk's rep movsd and its live values in the same function MSVC
//     materialises both struct addresses. Copying the 4-byte member and the
//     short separately keeps every other register right and scores best.
//  2. The tail of the inlined walk. The original inlines one level of
//     FUN_0045b030 and hands the rest of the sibling walk back to it
//     (`cmp ebx, ebp ; je end ; xor edx, edx ; mov ecx, ebx ; call`), which
//     only happens if FUN_0045b030 is written recursively: its out-of-line body
//     at 0x45b030 is byte-identical either way, because MSVC turns the tail
//     recursion into the loop that 0x45b030.cpp spells out. Tried: the
//     recursive form plain, with the tail call assigned instead of returned,
//     with the child's result discarded, with the result as bool/char/short,
//     with an early return for the no-sibling path, with a local copy of the
//     original force, with a local for the root pointer, with the sibling test
//     first, and the same walk written out by hand in the caller with
//     FUN_0045b030 only declared or with the child call in both arms of the
//     if/else. All of them need one more callee-saved register than the loop
//     form: MSVC then puts the unit argument in ebp, the zero in edx and the
//     walk result in eax, so the whole function shifts (56.2 percent).
//     `__declspec(noinline)` does not exist in VC5 and taking the helper's
//     address does not stop /Ob2 from inlining it at the two call sites.
//     tools/headers.py over all 128 header sets changes nothing.
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
