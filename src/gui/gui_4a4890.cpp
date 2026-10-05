// Decompiled by Opus. Names are provisional.
// Advances GUI entry i's animated value by its step each time its interval
// elapses, until it reaches its maximum, then redraws the entry.
// Needs a header (any of <windows.h>, <stdio.h>, ...: tools/headers.py) for
// the table pointer to be loaded between the steps of the index multiply.

#include <windows.h>

#pragma pack(push, 1)
struct Entry_004a4890 {
    char unknown_0[0xba];
    int value;                         // +0xba
    int max;                           // +0xbe
    int interval;                      // +0xc2
    int next;                          // +0xc6
    float step;                        // +0xca
    int active;                        // +0xce
    char unknown_d2[0x15b - 0xd2];
};
#pragma pack(pop)

struct Table_004a4890 {
    char unknown_0[4];
    Entry_004a4890* entries;           // +0x4
};

struct Dialog {
    char unknown_0[0x18];
    Table_004a4890* table;             // +0x18
};

int __cdecl GetTicks();
void __stdcall FUN_004a4660(Dialog* obj, int i);

// FUNCTION: 0x4a4890
void __stdcall FUN_004a4890(Dialog* obj, int i)
{
    Entry_004a4890* e = &obj->table->entries[i];
    if (e->active && e->value < e->max) {
        if (GetTicks() > e->next) {
            e->value += (int)e->step;
            if (e->value > e->max) {
                e->value = e->max;
                e->active = 0;
            }
            e->next = GetTicks() + e->interval;
        }
        FUN_004a4660(obj, i);
    }
}
