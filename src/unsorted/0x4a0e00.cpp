// Decompiled by deepseek-v4.1-flash. Names are provisional.
// The previous attempt (space-bunny-free) had the switch and the caller layout
// right but nested the two guards, and used a `static inline` find helper. That
// made MSVC 5 duplicate the shared `changed = 1` tail into the case 3 exit
// (318 bytes against the original 299). Two changes fix it:
//   - write the two guards as early `return`s, not nested `if`s;
//   - make `FindEntry` a real (emitted) helper instead of `static inline`.
// With the find loop inlined from a helper that also exists as a function
// before the caller, MSVC allocates the blocks as in the exe: the tail stays
// shared at 0x4a0f0f and case 3 short-jumps to it.
//
// Sets the text of the GUI entry named `name`: entries of type 1 and 5 take
// strncpy of 0x80 bytes, type 3 a plain strcpy, and for type 3, when the entry
// is the current one, the text length is stored. Then the list is marked
// changed. The entry is looked up in `holder->unknown_0->entries` but written
// through `holder->entries` for type 3, the two chains the machine code takes.
//
// Suspected original bug: the lookup uses `holder->unknown_0->entries` while
// the type 3 write goes to `holder->entries[index]`, so the text can be stored
// into a different array than the one that was searched. The function itself
// walks both chains (the two-level one at the top, the three-level one inside
// case 3), which is the evidence.
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

// Not `static inline`: the original helper is also a real function, and MSVC
// only lays the caller's blocks out as in the exe when it is emitted first.
int FindEntry(Entry_004a0e00* entries, char* name)
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
    // Loaded before the null test on purpose: the original loads the entries
    // pointer before the `je`.
    Entry_004a0e00* entries = obj->holder->unknown_0->entries;
    if (obj->holder->unknown_0 == 0)
        return;
    int index = FindEntry(entries, name);
    if (index == -1)
        return;
    switch (entries[index].type) {
    case 3:
        strcpy(obj->holder->entries[index].u.text, text);
        if (obj->current == index)
            obj->length = strlen(text);
        break;
    case 1:
        strncpy(entries[index].u.text, text, 0x80);
        break;
    case 5:
        strncpy(entries[index].u.text, text, 0x80);
        break;
    }
    obj->changed = 1;
}
