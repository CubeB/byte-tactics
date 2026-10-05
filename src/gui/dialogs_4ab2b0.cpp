// Decompiled by Opus. Names are provisional.
// Appends a copy of a 0x13e-byte record to the entry list (at most 200
// entries; entry 0 holds the count) and marks the new entry as type 1.

#pragma pack(push, 1)
struct Record_004ab2b0 {
    unsigned char type;                // +0x0
    char unknown_1[0xb6 - 0x1];
    short count;                       // +0xb6 (only meaningful in entry 0)
    char unknown_b8[0x13e - 0xb8];
};

struct Entry_004ab2b0 {
    Record_004ab2b0 record;            // +0x0
    char unknown_13e[0x15b - 0x13e];
};
#pragma pack(pop)

struct Holder_004ab2b0 {
    char unknown_0[4];
    Entry_004ab2b0* entries;           // +0x4
};

struct Class_004ab2b0 {
    char unknown_0[0x18];
    Holder_004ab2b0* holder;           // +0x18
};

// FUNCTION: 0x4ab2b0
int __stdcall AddButtonGadget(Class_004ab2b0* obj, Record_004ab2b0* record)
{
    Entry_004ab2b0* entries = obj->holder->entries;
    if (entries->record.count == 200) {
        return 0;
    }
    short n = ++entries->record.count;
    entries[n].record = *record;
    entries[n].record.type = 1;
    return 1;
}
