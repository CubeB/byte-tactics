// Decompiled by space-bunny-free. Names are provisional.
// Sets the text of the GUI entry named `name`: entries of type 1 and 5 take
// strncpy of 0x80 bytes, type 3 a plain strcpy, and for type 3, when the entry
// is the current one, the text length is stored. Then the list is marked
// changed. The entry is looked up in `holder->unknown_0->entries` but written
// through `holder->entries`, the two chains the machine code takes.
//
// Suspected original bug: the lookup uses `holder->unknown_0->entries` while
// the type 3 write goes to `holder->entries[index]`, so the text can be stored
// into a different array than the one that was searched. The function itself
// walks both chains (the two-level one at the top, the three-level one inside
// case 3), which is the evidence.
//
// 88.9 percent, everything matches except the shared tail. The original keeps
// `obj->changed = 1` in its own block at 0x4a0f0f and leaves the case 3 block
// ending in `mov [edx+0x74],ecx; jmp 0x4a0f0f`. Here MSVC 5 sinks that store
// into the case 3 block and clones the epilogue into it (318 bytes against
// 299), which also pushes the case 1 `je` from a short to a near jump. Every
// spelling of the tail I could find gives the same 318 bytes: the store after
// the switch, in each case, in case 3 as well, through an inline member, a
// second `obj` local, a reference, a `goto` exit, a nested `State` struct,
// sizeof() spellings, `while (1)`, all six case orders, pack(2) on the class,
// and FindEntry as a member of the list. Registers, instruction order and the
// store offsets all agree, so the blocker is that block-duplication decision,
// not a source-level difference I can still find.
#include <string.h>

#pragma pack(push, 1)
struct Entry_004a0e00 {                 // 0x15b bytes
    unsigned char type;                 // +0x00
    char unknown_1[1];
    char name[0x10];                    // +0x02
    char unknown_12[0xb6 - 0x12];
    union {
        short count;                    // +0xb6 (entry 0 holds the entry count)
        char text[0x80];
    } u;
    char unknown_136[0x15b - 0x136];
};

struct Holder_004a0e00 {
    Holder_004a0e00* unknown_0;         // +0x00
    Entry_004a0e00* entries;            // +0x04
};

struct Class_004a0e00 {
    char unknown_0[0x18];
    Holder_004a0e00* holder;            // +0x18
    char unknown_1c[0x64 - 0x1c];
    int current;                        // +0x64
    char unknown_68[0x74 - 0x68];
    int length;                         // +0x74
    char unknown_78[0xcca - 0x78];
    int changed;                        // +0xcca
};
#pragma pack(pop)

static inline int FindEntry(Entry_004a0e00* entries, char* name)
{
    for (int i = 1; i < entries[0].u.count + 1; i++) {
        if (strncmp(entries[i].name, name, 0x10) == 0)
            return i;
    }
    return -1;
}

// FUNCTION: 0x4a0e00
void __stdcall FUN_004a0e00(Class_004a0e00* obj, char* name, char* text)
{
    // Declared before the null test on purpose: the original loads the entries
    // pointer before the `je`, so the local has to be computed there.
    Entry_004a0e00* entries = obj->holder->unknown_0->entries;
    if (obj->holder->unknown_0 != 0) {
        int index = FindEntry(entries, name);
        if (index != -1) {
            switch (entries[index].type) {
            case 1:
                strncpy(entries[index].u.text, text, 0x80);
                break;
            case 3:
                strcpy(obj->holder->entries[index].u.text, text);
                if (obj->current == index) {
                    obj->length = strlen(text);
                }
                break;
            case 5:
                strncpy(entries[index].u.text, text, 0x80);
                break;
            }
            obj->changed = 1;
        }
    }
}
