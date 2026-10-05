// Decompiled by Opus. Names are provisional.
// Looks up an entry of the table that GetGafFrame indexes (count at +0,
// 8-byte entries from +0x28) through a handle holding an index and the table.

struct Entry_004b7ee0 {
    int value;                         // +0x0
    int unknown_4;
};

struct Table_004b7ee0 {
    unsigned short count;              // +0x0
    char unknown_2[0x26];
    Entry_004b7ee0 entries[1];         // +0x28
};

struct Handle_004b7ee0 {
    unsigned short index;              // +0x0
    char unknown_2[6];
    Table_004b7ee0* table;             // +0x8
};

// FUNCTION: 0x4b7ee0
int __stdcall GetGafSequenceFrame(Handle_004b7ee0* h)
{
    int result = 0;
    if (h->table != 0)
        result = h->table->entries[h->index].value;
    return result;
}
