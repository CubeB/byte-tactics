// Decompiled by space-bunny-free. Names are provisional.
// Adds one 0x36-byte object-state entry for obj: sets its flags bit 1, stores
// the object pointer, clears the 16-bit field, copies the object's point list
// into a fresh "Point List" block, sets flag bit 0 when the point count is at
// least 3 (else clears it) and flag bit 2, bumps the state's entry count, then
// recurses into the object's child (passing the new entry as its parent) and
// sibling (passing this call's parent). The parent store has to sit after the
// sibling if/else, with no early return, so that the null tests stay `test
// reg,reg` and the third argument gets loaded into edi before the test.

#include <string.h>

void* __cdecl FUN_004d83b0(char* name, int size);

#pragma pack(push, 2)

struct Object3do {
    char unknown_0[0x4];
    int unknown_4;                       // +0x4, point count
    char unknown_8[0x24 - 0x8];
    void* points;                        // +0x24
    char unknown_28[0x2c - 0x28];
    Object3do* unknown_2c;               // +0x2c, sibling
    Object3do* unknown_30;               // +0x30, child
};

struct Entry_0045aec0 {
    Object3do* object;                   // +0x0
    char unknown_4[0x22 - 0x4];
    void* points;                        // +0x22
    short unknown_26;                    // +0x26
    unsigned short flags;                // +0x28
    Entry_0045aec0* sibling;             // +0x2a
    Entry_0045aec0* child;               // +0x2e
    Entry_0045aec0* parent;              // +0x32
};

struct ObjectState_0045aec0 {
    int count;                           // +0x0
    char unknown_4[0x22 - 0x4];
    Entry_0045aec0 entries[1];           // +0x22
};

#pragma pack(pop)

// FUNCTION: 0x45aec0
Entry_0045aec0* __stdcall AddStateEntries(ObjectState_0045aec0* state, Object3do* obj,
                                       Entry_0045aec0* parent)
{
    Entry_0045aec0* e = &state->entries[state->count];
    e->flags |= 2;
    e->object = obj;
    e->unknown_26 = 0;
    e->points = FUN_004d83b0("Point List", obj->unknown_4 * 12);
    memcpy(e->points, obj->points, obj->unknown_4 * 12);
    if (obj->unknown_4 >= 3) {
        e->flags |= 1;
    } else {
        e->flags &= 0xfffe;
    }
    e->flags |= 4;
    state->count++;
    if (obj->unknown_30 != 0) {
        e->child = AddStateEntries(state, obj->unknown_30, e);
    } else {
        e->child = 0;
    }
    if (obj->unknown_2c != 0) {
        e->sibling = AddStateEntries(state, obj->unknown_2c, parent);
    } else {
        e->sibling = 0;
    }
    e->parent = parent;
    return e;
}
