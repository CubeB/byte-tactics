// Decompiled by Opus. Names are provisional.
#include <stdio.h>

struct Holder_00493390 {
    int unknown_0;
    void* data;                      // +0x4
};

struct Object_00493390 {
    char unknown_0[0x18];
    Holder_00493390* holder;         // +0x18
};

char* __stdcall FUN_004a0200(void* data, char* key);
int __stdcall FUN_0045ba20(char* text);
void __stdcall FUN_004a0bf0(Object_00493390* obj, char* name, int param_3, int param_4);

// FUNCTION: 0x493390
void __stdcall FUN_00493390(Object_00493390* obj, int unused)
{
    char buf[52];
    char* value = FUN_004a0200(obj->holder->data, "ENERGY");
    if (value != 0) {
        sprintf(buf, "%d", FUN_0045ba20(value));
        FUN_004a0bf0(obj, "ENERGY#", (int)buf, 0);
    }
}
