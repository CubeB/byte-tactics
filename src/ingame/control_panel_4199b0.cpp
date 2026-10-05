// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
// Refreshes the text of the menu entries owned by the unit's owner. The menu
// keeps a table of 0x15b-byte entries whose first entry stores the entry count
// as a short at +0xb6 (the same offset entry 1..n use for their text). Entries
// whose byte at +0 is 1 have either a name at +2 looked up with FindUnitTypeId
// (printing "+<amount>") or show the unit's own count at +0x1e. Entry 0 holds
// only the count, so the loop starts at entry 1. The "count" field is read as
// an int so MSVC sign-extends it once and keeps the decrementing loop counter
// in eax; the entry pointer must be incremented after the read so the chain
// stays in ecx.
#include <stdio.h>
#include <string.h>

#pragma pack(push, 1)
struct Entry_004199b0 {                // 0x15b bytes
    unsigned char type;                // +0x00
    char unknown_1;
    char name[0x10];                   // +0x02
    char unknown_12[0x2a - 0x12];
    unsigned char flags;               // +0x2a
    char unknown_2b[0xb6 - 0x2b];
    union {
        short count;                   // +0xb6 (entry 0 only)
        char text[0x15b - 0xb6];       // +0xb6
    } u;
};
#pragma pack(pop)

struct Data_004199b0 {
    int unknown_0;
    Entry_004199b0* entries;           // +0x04
};

struct Menu_004199b0 {
    char unknown_0[0x18];
    Data_004199b0* data;               // +0x18
};

struct Unit {
    char unknown_0[0x1e];
    unsigned char field_1e;            // +0x1e
};

extern char* g_game;

unsigned short __stdcall FindUnitTypeId(void* name);
int __stdcall FUN_00439d80(void* owner, int index);
void __stdcall FUN_0049fa90(void* obj);

// FUNCTION: 0x4199b0
void __stdcall FUN_004199b0(Menu_004199b0* menu, Unit* unit)
{
    Entry_004199b0* e = menu->data->entries;
    int count = e->u.count;
    e++;
    for (int i = 1; i < count + 1; i++) {
        char* text = e->u.text;
        if (e->type == 1) {
            if (e->flags & 4) {
                unsigned short v = FindUnitTypeId(e->name);
                if (v != 0) {
                    int r = FUN_00439d80(unit, v);
                    if (r != 0)
                        sprintf(text, "+%d", r);
                    else
                        text[0] = 0;
                }
            } else if (e->flags & 8) {
                int n = unit->field_1e;
                int r = FUN_00439d80(unit, 0);
                text[0] = 0;
                if (n != 0)
                    sprintf(text, "%d", n);
                if (r != 0)
                    sprintf(text + strlen(text), " +%d", r);
            }
        }
        e++;
    }
    FUN_0049fa90(g_game + 0x519);
}
