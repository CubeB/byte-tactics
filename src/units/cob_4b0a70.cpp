// Decompiled by Opus. Names are provisional.
#include <string.h>

struct NameTable_004b0a70 {
    int unknown_0;
    int count;                         // +0x4
    char unknown_8[0x1c - 0x8];
    char** names;                      // +0x1c
};

class Class_004b0b00 {
public:
    int FUN_004b0b00(int index, void* param_2, int param_3, int param_4, int param_5, int param_6, int param_7, int param_8);
};

class Class_004b0a70 {
public:
    int unknown_0;
    int unknown_4;
    NameTable_004b0a70* table;         // +0x8

    int FUN_004b0a70(char* name, void* param_2, int param_3, int param_4, int param_5, int param_6, int param_7, int param_8);
};

static inline int FindName(Class_004b0a70* obj, char* name)
{
    NameTable_004b0a70* t = obj->table;
    for (int i = 0; i < t->count; i++) {
        if (strcmp(name, t->names[i]) == 0) {
            return i;
        }
    }
    return -1;
}

// FUNCTION: 0x4b0a70
int Class_004b0a70::FUN_004b0a70(char* name, void* param_2, int param_3, int param_4, int param_5, int param_6, int param_7, int param_8)
{
    return ((Class_004b0b00*)this)->FUN_004b0b00(FindName(this, name), param_2, param_3, param_4, param_5, param_6, param_7, param_8);
}
