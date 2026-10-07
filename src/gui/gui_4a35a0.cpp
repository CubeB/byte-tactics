// Decompiled by deepseek-v4.1-flash, finished by Space Bunny Free, finished by Sonnet 5.5. Names are provisional.
// Finds the GUI layout entry named `name` (0x15b-byte entries whose entry 0
// stores the entry count as a short at +0xb6) and gives it the row array
// `rows` and its count: +0xc6 gets the array, +0xc0 the count, flags +0x1b
// gets bit 7, and +0xbe the index of the topmost row that still fits in the
// entry's height (+0x19) when the row heights are summed from the bottom.
// Each row is 0x18 bytes with an unsigned short height at +0x2; when the
// entry's +0xda is non-zero it overrides the row height with its own value.
// A missing entry is reported and then treated as a null entry (the stores
// that follow then go through a null pointer, as in the original).
#include <string.h>

#pragma pack(push, 1)
struct Entry_004a35a0 {                // 0x15b-byte GUI entry
    char unknown_0[0x2];
    char name[0x10];                   // +0x02
    char unknown_12[0x19 - 0x12];
    short height;                      // +0x19
    int flags;                         // +0x1b
    char unknown_1f[0xb6 - 0x1f];
    short count;                       // +0xb6 (entry 0 holds the entry count)
    char unknown_b8[0xba - 0xb8];
    short f_ba;                        // +0xba
    short f_bc;                        // +0xbc
    short first;                       // +0xbe
    short num;                         // +0xc0
    char unknown_c2[0xc6 - 0xc2];
    int f_c6;                          // +0xc6
    char unknown_ca[0xda - 0xca];
    short f_da;                        // +0xda
    char unknown_dc[0x15b - 0xdc];
};

struct Row_004a35a0 {
    char unknown_0[2];
    unsigned short height;             // +0x2
    char unknown_4[0x18 - 4];
};

struct Table_004a35a0 {
    int unknown_0;
    Entry_004a35a0* entries;           // +0x4
};
#pragma pack(pop)

void __stdcall FatalError(const char* msg);

static inline int FindEntry(Entry_004a35a0* entries, char* name)
{
    for (int i = 1; i < entries->count + 1; i++) {
        if (strncmp(entries[i].name, name, 0x10) == 0)
            return i;
    }
    return -1;
}

// FUNCTION: 0x4a35a0
void __stdcall SetGadgetRows(Table_004a35a0* table, char* name,
                            Row_004a35a0* rows, int count)
{
    Entry_004a35a0* entries = table->entries;
    int index = FindEntry(entries, name);
    Entry_004a35a0* e;
    if (index != -1) {
        e = &entries[index];
    } else {
        FatalError("Error in GUI layout");
        e = 0;
    }
    e->f_c6 = (int)rows;
    // Computed right after the f_c6 store and before the num store.
    Row_004a35a0* row = rows + count - 1;
    e->num = (short)count;
    e->flags |= 0x80;
    e->f_bc = 0;
    e->f_ba = 0;
    e->first = (short)(count - 1);
    int remain = e->height;
    // The test is i > -1, not i >= 0.
    for (int i = count - 1; i > -1; i--, row--) {
        // An if/else with one remain -= per arm, not one remain -= cond ? a : b.
        if (e->f_da != 0) remain -= e->f_da; else remain -= row->height;
        if (remain < 0)
            break;
        e->first = (short)i;
    }
}
