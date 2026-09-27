// Decompiled by Opus. Names are provisional.
// Appends a copy of a 0xd6-byte record to the entry list (at most 200
// entries; entry 0 holds the count) and marks the new entry as type 0xd.
// Same shape as 0x4ab2b0, which appends a 0x13e-byte type 1 record.

#pragma pack(push, 1)
struct Record_004ab3a0 {
    unsigned char type;                // +0x0
    char unknown_1[0xb6 - 0x1];
    short count;                       // +0xb6 (only meaningful in entry 0)
    char unknown_b8[0xd6 - 0xb8];
};

struct Entry_004ab3a0 {
    Record_004ab3a0 record;            // +0x0
    char unknown_d6[0x15b - 0xd6];
};
#pragma pack(pop)

struct Holder_004ab3a0 {
    char unknown_0[4];
    Entry_004ab3a0* entries;           // +0x4
};

struct Class_004ab3a0 {
    char unknown_0[0x18];
    Holder_004ab3a0* holder;           // +0x18
};

// FUNCTION: 0x4ab3a0
int __stdcall FUN_004ab3a0(Class_004ab3a0* obj, Record_004ab3a0* record)
{
    Entry_004ab3a0* entries = obj->holder->entries;
    if (entries->record.count == 200) {
        return 0;
    }
    short n = ++entries->record.count;
    entries[n].record = *record;
    entries[n].record.type = 0xd;
    return 1;
}
