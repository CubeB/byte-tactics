// Decompiled by Opus. Names are provisional.
// Clears three fields of GUI entry i (0x15b-byte entries). The stores sit in
// an inline helper taking the entry pointer; a local pointer to the entry
// loads the entries pointer before the index multiply instead of after it.

#pragma pack(push, 1)
struct Entry_004a3eb0 {
    char unknown_0[0x140];
    short field_140;                   // +0x140
    char unknown_142[2];
    int field_144;                     // +0x144
    char unknown_148[2];
    int field_14a;                     // +0x14a
    char unknown_14e[0x15b - 0x14e];
};
#pragma pack(pop)

struct Table_004a3eb0 {
    char unknown_0[4];
    Entry_004a3eb0* entries;           // +0x4
};

struct Dialog {
    char unknown_0[0x18];
    Table_004a3eb0* table;             // +0x18
};

static inline void ClearEntry(Entry_004a3eb0* e)
{
    e->field_140 = 0;
    e->field_144 = 0;
    e->field_14a = 0;
}

// FUNCTION: 0x4a3eb0
void __stdcall FUN_004a3eb0(Dialog* obj, int i)
{
    ClearEntry(&obj->table->entries[i]);
}
