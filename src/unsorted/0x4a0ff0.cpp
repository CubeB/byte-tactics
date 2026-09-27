// Decompiled by Opus. Names are provisional.
// Returns field 0x137 of entry `index` (0x15b-byte entries) when that entry's
// state is 1, otherwise -1. The entries pointer is loaded into a local before
// the index multiply; indexing through the full chain loads it afterwards.

#pragma pack(push, 1)
struct Entry_004a0ff0 {
    char state;                        // +0x0
    char unknown_1[0x137 - 0x1];
    unsigned char field_137;           // +0x137
    char unknown_138[0x15b - 0x138];
};
#pragma pack(pop)

struct Table_004a0ff0 {
    char unknown_0[4];
    Entry_004a0ff0* entries;           // +0x4
};

struct Obj_004a0ff0 {
    char unknown_0[0x18];
    Table_004a0ff0* table;             // +0x18
};

// FUNCTION: 0x4a0ff0
int __stdcall FUN_004a0ff0(Obj_004a0ff0* obj, int index)
{
    Entry_004a0ff0* entries = obj->table->entries;
    if (entries[index].state == 1) {
        return entries[index].field_137;
    }
    return -1;
}
