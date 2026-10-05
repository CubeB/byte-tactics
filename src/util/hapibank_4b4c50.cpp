// Decompiled by Opus. Names are provisional.
// Resets field +0xc of the current item of the current entry to field +0x8
// and returns it (the returned value is what keeps it in eax).

struct Item_004b4c50 {                 // 0x14 bytes
    char unknown_0[8];
    int field_8;                       // +0x8
    int field_c;                       // +0xc
    char unknown_10[4];
};

struct Entry_004b4c50 {                // 0x18 bytes
    char unknown_0[0xc];
    int cur;                           // +0xc
    char unknown_10[4];
    Item_004b4c50* items;              // +0x14
};

struct Data_004b4c50 {
    char unknown_0[4];
    Entry_004b4c50* entries;           // +0x4
    int cur;                           // +0x8
};

class Class_004b4c50 {
public:
    Data_004b4c50* data;               // +0x0

    int FUN_004b4c50();
};

// FUNCTION: 0x4b4c50
int Class_004b4c50::FUN_004b4c50()
{
    Entry_004b4c50* e = &data->entries[data->cur];
    Item_004b4c50* it = &e->items[e->cur];
    it->field_c = it->field_8;
    return it->field_c;
}
