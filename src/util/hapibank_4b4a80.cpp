// Decompiled by Space Bunny Free. Names are provisional.
#include <string.h>

extern int __cdecl _strcmpi(const char*, const char*);

struct Entry_004b4a80 {              // 0x14 bytes
    int used;                       // +0x00
    char* name;                     // +0x04
    char unknown_8[0xc];
};

struct Slot_004b4a80 {               // 0x18 bytes
    char unknown_0[8];
    int count;                      // +0x08
    char unknown_c[8];
    Entry_004b4a80* entries;        // +0x14
};

struct Table_004b4a80 {
    char unknown_0[4];
    Slot_004b4a80* slots;           // +0x04
    int index;                      // +0x08
};

void* __cdecl FUN_004d8580(void* ptr, int size);
char* __cdecl FUN_004d8610(char* s);

class Class_004b4a80 {
public:
    Table_004b4a80* table;          // +0x00
    int FUN_004b4a80(char* name, int flag);
};

// FUNCTION: 0x4b4a80
int Class_004b4a80::FUN_004b4a80(char* name, int flag)
{
    Table_004b4a80* t = table;
    if (!t || t->index < 0)
        return -1;
    Slot_004b4a80* s = &t->slots[t->index];
    for (int i = 0; i < s->count; i++) {
        if (s->entries[i].used && _strcmpi(s->entries[i].name, name) == 0)
            return i;
    }
    if (!flag)
        return -1;
    int n = s->count;
    s->count = n + 1;
    s->entries = (Entry_004b4a80*)FUN_004d8580(s->entries, (n + 1) * sizeof(Entry_004b4a80));
    memset(&s->entries[n], 0, sizeof(Entry_004b4a80));
    s->entries[n].name = FUN_004d8610(name);
    s->entries[n].used = 1;
    return n;
}
