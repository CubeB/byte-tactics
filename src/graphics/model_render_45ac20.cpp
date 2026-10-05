// Decompiled by space-bunny-free. Names are provisional.
// This is 0x45ab10 (which is matched) applied to a unit, to every unit on
// its child list, and then one call to DrawObjectState; the whole body is inside
// `if (unit->field_86 == 0)`, which is why the early exit jumps straight to
// the three-push epilogue.
//
// What made it match (all of it inherited from src/graphics/model_render_45ab10.cpp):
//  - The distance test has to be an inline helper taking the state and a
//    POINTER to the unit's position. Written out inline, or with a helper
//    taking (state, unit), MSVC 5 gives the unit ebp, keeps no zero register
//    and loses the `lea edx, [ecx+0x18]` of the position copy.
//  - RestorePieceVertices is spelled RECURSIVELY here so /Ob2 inlines its first level
//    into BOTH copies of the body. Out of line, MSVC makes of it the sibling
//    loop, which the original does not have. The flag the first level
//    computes is passed to the child but the sibling always gets 0
//    (`xor edx,edx ; mov ecx,ebp ; call`), which is what the recursive
//    spelling gives and what the `result` spelling of 0x45b030.cpp would not.
//  - The position is one 6-byte struct copy, which MSVC splits into a dword
//    store plus a word store with `state->field_8 = 1` scheduled between them.
//  - <string.h> is needed for the inlined memcpy.
#include <stdlib.h>
#include <string.h>

#pragma pack(push, 2)

struct Object3do {
    char unknown_0[4];
    int point_count;                    // +0x4
    char unknown_8[0x24 - 0x8];
    void* points;                       // +0x24
    char unknown_28[0x2c - 0x28];
    Object3do* sibling;                 // +0x2c
    Object3do* child;                   // +0x30
};

struct Flags_0045ac20 {
    unsigned short f0 : 1;              // +0x28
    unsigned short f1 : 1;
};

struct Entry_0045ac20 {
    Object3do* object;                  // +0x00
    char unknown_4[0x16 - 0x4];
    int offset_x;                       // +0x16
    int offset_y;                       // +0x1a
    int offset_z;                       // +0x1e
    void* points;                       // +0x22
    short unknown_26;                   // +0x26
    Flags_0045ac20 flags;               // +0x28
    Entry_0045ac20* sibling;            // +0x2a
    Entry_0045ac20* child;              // +0x2e
    Entry_0045ac20* parent;             // +0x32
};

struct Vec3s_0045ac20 {
    short x;                            // +0x0
    short y;                            // +0x2
    short z;                            // +0x4
};

struct State_0045ac20 {
    int count;                          // +0x0
    int field_4;                        // +0x4
    int field_8;                        // +0x8 dirty
    int field_c;                        // +0xc
    char unknown_10[0x18 - 0x10];
    Vec3s_0045ac20 pos;                 // +0x18
    Entry_0045ac20* root;               // +0x1e
    Entry_0045ac20 entries[1];          // +0x22
};

struct Unit {
    char unknown_0[0x64];
    Vec3s_0045ac20 pos;                 // +0x64
    char unknown_6a[0x86 - 0x6a];
    int field_86;                       // +0x86
    Unit* child;                        // +0x8a
    Unit* next;                         // +0x8e
    char unknown_92[0x9e - 0x92];
    State_0045ac20* state;              // +0x9e
    char unknown_a2[0x110 - 0xa2];
    unsigned int flags;                 // +0x110
};

#pragma pack(pop)

// Defined here so that /Ob2 inlines its first level into both copies of the
// body below, as the original does; src/graphics/model_render_45b030.cpp spells the same
// function as the loop MSVC makes of it.
int __fastcall RestorePieceVertices(Entry_0045ac20* piece, int force)
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
        result = RestorePieceVertices(piece->child, result);
    if (piece->sibling)
        return RestorePieceVertices(piece->sibling, force);
    return result;
}

void __fastcall PoseModel(State_0045ac20* state, Entry_0045ac20* entry, int flag);

class CMemoryCache {
public:
    void DrawObjectState(State_0045ac20* state, void* context);
};

#pragma pack(push, 1)
struct Game {
    char pad0[0x1437b];
    CMemoryCache* obj;                  // +0x1437b
};
#pragma pack(pop)

// GLOBAL: 0x511de8
extern Game* g_game;

// Nonzero when the state's position is 8 or more away from `pos` on any axis.
static inline int FarFrom(State_0045ac20* state, const Vec3s_0045ac20* pos)
{
    return abs((short)(state->pos.z - pos->z)) >= 8
        || abs((short)(state->pos.y - pos->y)) >= 8
        || abs((short)(state->pos.x - pos->x)) >= 8;
}

// FUNCTION: 0x45ac20
void __stdcall DrawUnit(void* context, Unit* unit)
{
    if (unit->field_86 == 0) {
        State_0045ac20* state = unit->state;
        if (FarFrom(state, &unit->pos)) {
            state->pos = unit->pos;
            state->field_8 = 1;
            state->root->unknown_26 = 0;
            if (state->root->flags.f1) {
                state->field_4 = 0;
            }
        }
        if (unit->state->field_8 != 0) {
            RestorePieceVertices(unit->state->root, 0);
            PoseModel(unit->state, unit->state->root, 0);
            unit->state->field_8 = 0;
        }
        for (Unit* u = unit->child; u; u = u->next) {
            if (!(u->flags & 0x20000)) {
                State_0045ac20* child = u->state;
                if (FarFrom(child, &u->pos)) {
                    child->pos = u->pos;
                    child->field_8 = 1;
                    child->root->unknown_26 = 0;
                    if (child->root->flags.f1) {
                        child->field_4 = 0;
                    }
                }
                if (u->state->field_8 != 0) {
                    RestorePieceVertices(u->state->root, 0);
                    PoseModel(u->state, u->state->root, 0);
                    u->state->field_8 = 0;
                }
            }
        }
        g_game->obj->DrawObjectState(unit->state, context);
    }
}
