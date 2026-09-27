// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Appends a copy of a 0xcc-byte record to the entry list (at most 200
// entries; entry 0 holds the count) and marks the new entry as type 6.
// Same shape as 0x4ab2b0 (type 1) and 0x4ab3a0 (type 0xd).

#pragma pack(push, 1)
struct Record_004ab310 {
    unsigned char type;                // +0x0
    char unknown_1[0xb6 - 0x1];
    union {
        short count;                   // +0xb6 (only meaningful in entry 0)
        int field_b6;                  // +0xb6
    };
    char unknown_ba[0xbe - 0xba];
    int field_be;                      // +0xbe
    int field_c2;                      // +0xc2
    short field_c6;                    // +0xc6
    char unknown_c8[0xcc - 0xc8];
};

struct Entry_004ab310 {
    Record_004ab310 record;            // +0x0
    char unknown_cc[0x15b - 0xcc];
};
#pragma pack(pop)

struct Holder_004ab310 {
    char unknown_0[4];
    Entry_004ab310* entries;           // +0x4
};

struct Class_004ab310 {
    char unknown_0[0x18];
    Holder_004ab310* holder;           // +0x18
};

// FUNCTION: 0x4ab310
int __stdcall FUN_004ab310(Class_004ab310* obj, Record_004ab310* record)
{
    Entry_004ab310* entries = obj->holder->entries;
    if (entries->record.count == 200) {
        return 0;
    }
    short n = ++entries->record.count;
    entries[n].record = *record;
    entries[n].record.type = 6;
    Entry_004ab310* dst = &obj->holder->entries[n];
    dst->record.field_b6 = 0;
    dst->record.field_be = 0;
    dst->record.field_c2 = 0;
    dst->record.field_c6 = 0;
    return 1;
}
