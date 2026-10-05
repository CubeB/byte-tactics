// Decompiled by Opus. Names are provisional.
#include <string.h>

struct NameTable_004b0bc0 {
    int unknown_0;
    int count;                         // +0x4
    char unknown_8[0x1c - 0x8];
    char** names;                      // +0x1c
};

class Class_004b0c40 {
public:
    int FUN_004b0c40(int index, int* param_2, int* param_3, int* param_4, int* param_5);
};

class Class_004b0bc0 {
public:
    int unknown_0;
    int unknown_4;
    NameTable_004b0bc0* table;         // +0x8

    int FUN_004b0bc0(char* name, int* param_2, int* param_3, int* param_4, int* param_5);
};

static inline int FindName(Class_004b0bc0* obj, char* name)
{
    NameTable_004b0bc0* t = obj->table;
    for (int i = 0; i < t->count; i++) {
        if (strcmp(name, t->names[i]) == 0) {
            return i;
        }
    }
    return -1;
}

// FUNCTION: 0x4b0bc0
int Class_004b0bc0::FUN_004b0bc0(char* name, int* param_2, int* param_3, int* param_4, int* param_5)
{
    return ((Class_004b0c40*)this)->FUN_004b0c40(FindName(this, name), param_2, param_3, param_4, param_5);
}
