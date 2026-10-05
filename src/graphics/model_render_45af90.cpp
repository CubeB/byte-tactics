// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
// Recursively looks up the 0x36-byte object-state entry whose object pointer
// equals obj, then links and returns that entry: it recurses into the object's
// sibling (object+0x2c) passing the caller's parent and into the child
// (object+0x30) passing the entry itself, storing the results at entry+0x2a
// and entry+0x2e, and the entry's parent at entry+0x32. Returns 0 when no
// entry matches.

#pragma pack(push, 2)

struct Class_0045ae80 {
    char unknown_0[0x2c];
    Class_0045ae80* unknown_2c;      // +0x2c
    Class_0045ae80* unknown_30;      // +0x30
};

struct Entry_0045af90 {
    Class_0045ae80* object;          // +0x0
    char unknown_4[0x1e];
    void* points;                    // +0x22
    short unknown_26;                // +0x26
    unsigned short flags;            // +0x28
    Entry_0045af90* sibling;         // +0x2a
    Entry_0045af90* child;           // +0x2e
    Entry_0045af90* parent;          // +0x32
};

struct ObjectState_0045af90 {
    int count;                       // +0x0
    char unknown_4[0x1e];
    Entry_0045af90 entries[1];       // +0x22
};

#pragma pack(pop)

// FUNCTION: 0x45af90
Entry_0045af90* __stdcall FUN_0045af90(ObjectState_0045af90* state, Class_0045ae80* obj, Entry_0045af90* parent)
{
    int i = state->count - 1;
    if (i >= 0) {
        while (1) {
            if (state->entries[i].object == obj)
                break;
            i--;
            if (i < 0)
                return 0;
        }
        Entry_0045af90* e = &state->entries[i];
        if (e->object->unknown_2c != 0) {
            e->sibling = FUN_0045af90(state, e->object->unknown_2c, parent);
        } else {
            e->sibling = 0;
        }
        if (e->object->unknown_30 != 0) {
            e->child = FUN_0045af90(state, e->object->unknown_30, e);
        } else {
            e->child = 0;
        }
        e->parent = parent;
        return e;
    }
    return 0;
}
