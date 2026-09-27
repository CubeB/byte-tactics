// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Appends a copy of entry `param_2` to the table (count at +0xb6 of entry 0),
// initialises it through the menu (FUN_004a09c0), then sets its name, value,
// state and flags. The body is inlined at 0x4447d4 by the caller that builds a
// name with sprintf first.
#include <string.h>

#pragma pack(push, 1)
struct Entry_004444d0 {                // 0x15b bytes
    short unknown_0;                   // +0x00
    char name[0x10];                   // +0x02
    char unknown_12[0x15 - 0x12];
    short field_15;                    // +0x15
    char unknown_17[0x1f - 0x17];
    int field_1f;                      // +0x1f
    char unknown_23[0x29 - 0x23];
    char field_29;                     // +0x29
    char unknown_2a[0xb6 - 0x2a];
    union {
        short count;                   // +0xb6 (entry 0 only)
        char text[0x15b - 0xb6];       // +0xb6
    } u;
};
#pragma pack(pop)

extern char* g_game;

void __stdcall FUN_004a09c0(void* obj, int index, int param_3, int param_4);

// FUNCTION: 0x4444d0
int __stdcall FUN_004444d0(Entry_004444d0* entries, int param_2, short param_3, int param_4, char* param_5)
{
    int index = ++entries[0].u.count;
    Entry_004444d0* d = &entries[index];
    Entry_004444d0* s = &entries[param_2];
    *d = *s;
    FUN_004a09c0(g_game + 0x519, index, param_4, 0);
    strcpy(d->name, param_5);
    d->field_15 = param_3;
    d->field_29 = 1;
    d->field_1f = 0;
    return index;
}
