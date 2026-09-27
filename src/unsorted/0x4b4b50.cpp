// Decompiled by Opus. Names are provisional.

struct Item_004b4b50 {                 // 0x14 bytes
    char unknown_0[0x10];
    int value;                         // +0x10
};

struct Entry_004b4b50 {                // 0x18 bytes
    char unknown_0[0xc];
    int selected;                      // +0xc
    int unknown_10;
    Item_004b4b50* items;              // +0x14
};

struct Table_004b4b50 {
    int unknown_0;
    Entry_004b4b50* entries;           // +0x4
    int current;                       // +0x8
};

class Class_004b49d0 {
public:
    int FUN_004b49d0(int a, int b);
};

class Class_004b4b50 {
public:
    Table_004b4b50* table;             // +0x0
    int FUN_004b4b50(int a);
};

// FUNCTION: 0x4b4b50
int Class_004b4b50::FUN_004b4b50(int a)
{
    int r = ((Class_004b49d0*)this)->FUN_004b49d0(a, 1);
    table->entries[table->current].selected = r;
    return table->entries[table->current].items[r].value != 0;
}
