// Decompiled by space-bunny-free. Names are provisional.
// Builds the "Object State" block for a piece tree (the 0x45a8d0 case plus a
// player id at +0xc), then reorders the 0x36-byte entries so that the ones
// whose object name appears in the build list come first (a selection sort
// that stops at the first match, swapping only when it found a later entry),
// and relinks the whole tree with FUN_0045af90 into the same field the first
// FUN_0045aec0 result went into, because a swap can have moved that entry.
#include <string.h>

struct Class_0045ae80 {
    char unknown_0[0x1c];
    const char* name;                  // +0x1c
    char unknown_20[0x2c - 0x20];
    Class_0045ae80* unknown_2c;         // +0x2c
    Class_0045ae80* unknown_30;         // +0x30
};

struct BuildList_0045a950 {
    char unknown_0[8];
    int count;                          // +0x8
    char unknown_c[0x20 - 0xc];
    const char** names;                 // +0x20
};

#pragma pack(push, 2)

struct Entry_0045a950 {
    Class_0045ae80* object;             // +0x0
    char unknown_4[0x32];
};

struct ObjectState_0045a950 {
    int count;                          // +0x0
    char unknown_4[4];
    int field_8;                        // +0x8
    int field_c;                        // +0xc
    char unknown_10[0x1e - 0x10];
    void* field_1e;                     // +0x1e
    Entry_0045a950 entries[1];          // +0x22
};

#pragma pack(pop)

int __stdcall FUN_0045ae80(Class_0045ae80* obj);
void* __stdcall FUN_0045aec0(ObjectState_0045a950* state, Class_0045ae80* obj, int parent);
void* __stdcall FUN_0045af90(ObjectState_0045a950* state, Class_0045ae80* obj, void* parent);
void* __cdecl FUN_004d83b0(char* name, int size);

// FUNCTION: 0x45a950
ObjectState_0045a950* __stdcall FUN_0045a950(Class_0045ae80* obj, BuildList_0045a950* list, int player)
{
    int count = 1;
    if (obj->unknown_30 != 0) {
        count = FUN_0045ae80(obj->unknown_30);
        count++;
    }
    if (obj->unknown_2c != 0) {
        count += FUN_0045ae80(obj->unknown_2c);
    }
    int size = count * 0x36 + 0x22;
    ObjectState_0045a950* state = (ObjectState_0045a950*)FUN_004d83b0("Object State", size);
    memset(state, 0, size);
    state->field_1e = FUN_0045aec0(state, obj, 0);
    state->field_8 = 1;
    state->field_c = player;
    for (int i = 0; i < list->count; i++) {
        for (int j = i; j < state->count; j++) {
            if (_strcmpi(list->names[i], state->entries[j].object->name) == 0) {
                if (i != j) {
                    Entry_0045a950 temp = state->entries[i];
                    state->entries[i] = state->entries[j];
                    state->entries[j] = temp;
                }
                break;
            }
        }
    }
    state->field_1e = FUN_0045af90(state, obj, 0);
    return state;
}
