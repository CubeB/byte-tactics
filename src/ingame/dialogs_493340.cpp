// Decompiled by Opus. Names are provisional.
#include <stdio.h>

struct Holder_00493340 {
    int unknown_0;
    void* data;                      // +0x4
};

struct Object_00493340 {
    char unknown_0[0x18];
    Holder_00493340* holder;         // +0x18
};

char* __stdcall FUN_004a0200(void* data, char* key);
int __stdcall FUN_0045ba20(char* text);
void __stdcall FUN_004a0bf0(Object_00493340* obj, char* name, int param_3, int param_4);

// FUNCTION: 0x493340
void __stdcall UpdateMetalReadout(Object_00493340* obj, int unused)
{
    char buf[52];
    char* value = FUN_004a0200(obj->holder->data, "METAL");
    if (value != 0) {
        sprintf(buf, "%d", FUN_0045ba20(value));
        FUN_004a0bf0(obj, "METAL#", (int)buf, 0);
    }
}
