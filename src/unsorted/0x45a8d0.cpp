// Decompiled by Opus. Names are provisional.
// Allocates and clears an "Object State" block with one 0x36-byte entry per
// node of an object's piece tree (the count from FUN_0045ae80, inlined one
// level), then fills it in with FUN_0045aec0.
#include <string.h>

struct Class_0045ae80 {
    char unknown_0[0x2c];
    Class_0045ae80* unknown_2c;        // +0x2c
    Class_0045ae80* unknown_30;        // +0x30
};

#pragma pack(push, 1)
struct ObjectState_0045a8d0 {
    char unknown_0[8];
    int field_8;                       // +0x8
    char unknown_c[0x1e - 0xc];
    int field_1e;                      // +0x1e
};
#pragma pack(pop)

int __stdcall FUN_0045ae80(Class_0045ae80* obj);
int __stdcall FUN_0045aec0(ObjectState_0045a8d0* state, Class_0045ae80* obj, int parent);
void* FUN_004d83b0(char* name, int size);

// FUNCTION: 0x45a8d0
ObjectState_0045a8d0* __stdcall FUN_0045a8d0(Class_0045ae80* obj)
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
    ObjectState_0045a8d0* state = (ObjectState_0045a8d0*)FUN_004d83b0("Object State", size);
    memset(state, 0, size);
    state->field_1e = FUN_0045aec0(state, obj, 0);
    state->field_8 = 1;
    return state;
}
