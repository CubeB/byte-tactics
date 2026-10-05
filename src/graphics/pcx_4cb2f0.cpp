// Decompiled by Opus. Names are provisional.
// Average of the +4 field of the table entries selected by a list of
// 16-bit indices.

struct Entry_004cb2f0 {
    int unknown_0;                     // +0x0
    int value;                         // +0x4
    int unknown_8;                     // +0x8
};

struct Table_004cb2f0 {
    char unknown_0[0x24];
    Entry_004cb2f0* entries;           // +0x24
};

struct List_004cb2f0 {
    int unknown_0;                     // +0x0
    int count;                         // +0x4
    int unknown_8;                     // +0x8
    unsigned short* indices;           // +0xc
};

// FUNCTION: 0x4cb2f0
int __stdcall AveragePrimitiveY(Table_004cb2f0* table, List_004cb2f0* list)
{
    int sum = 0;
    int n = list->count;
    unsigned short* p = list->indices;
    for (int i = 0; i < n; i++) {
        sum += table->entries[*p++].value;
    }
    return sum / n;
}
