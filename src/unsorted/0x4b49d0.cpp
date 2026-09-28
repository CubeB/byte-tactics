// Decompiled by deepseek-v4.1-flash. Names are provisional.
#include <string.h>

struct Entry_004b49d0 {              // 0x14 bytes
    int used;                       // +0x00
    int value;                      // +0x04
    char unknown_8[0xc];
};

struct Slot_004b49d0 {               // 0x18 bytes
    char unknown_0[8];
    int count;                      // +0x08
    char unknown_c[8];
    Entry_004b49d0* entries;        // +0x14
};

struct Table_004b49d0 {
    char unknown_0[4];
    Slot_004b49d0* slots;           // +0x04
    int index;                      // +0x08
};

void* __cdecl FUN_004d8580(void* ptr, int size);

class Class_004b49d0 {
public:
    Table_004b49d0* table;          // +0x00
    int FUN_004b49d0(int value, int flag);
};

// FUNCTION: 0x4b49d0
int Class_004b49d0::FUN_004b49d0(int value, int flag)
{
    Table_004b49d0* t = table;
    if (!t || t->index < 0)
        return -1;
    Slot_004b49d0* s = &t->slots[t->index];
    for (int i = 0; i < s->count; i++) {
        if (s->entries[i].used == 0 && s->entries[i].value == value)
            return i;
    }
    if (!flag)
        return -1;
    int n = s->count;
    s->count = n + 1;
    s->entries = (Entry_004b49d0*)FUN_004d8580(s->entries, (n + 1) * sizeof(Entry_004b49d0));
    memset(&s->entries[n], 0, sizeof(Entry_004b49d0));
    s->entries[n].value = value;
    s->entries[n].used = 0;
    return n;
}
