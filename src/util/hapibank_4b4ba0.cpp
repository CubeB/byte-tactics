// Decompiled by Opus. Names are provisional.

struct Entry_004b4ba0 {             // 0x14 bytes
    char unknown_0[0x10];
    int field_10;                   // +0x10
};

struct Slot_004b4ba0 {              // 0x18 bytes
    char unknown_0[0xc];
    int current;                    // +0x0c
    char unknown_10[4];
    Entry_004b4ba0* entries;        // +0x14
};

struct Table_004b4ba0 {
    char unknown_0[4];
    Slot_004b4ba0* slots;           // +0x04
    int index;                      // +0x08
};

class Class_004b4a80 {
public:
    int FUN_004b4a80(char* name, int flag);
};

class Class_004b4ba0 {
public:
    Table_004b4ba0* table;          // +0x00
    int FUN_004b4ba0(char* name);
};

// FUNCTION: 0x4b4ba0
int Class_004b4ba0::FUN_004b4ba0(char* name)
{
    int i = ((Class_004b4a80*)this)->FUN_004b4a80(name, 1);
    table->slots[table->index].current = i;
    return table->slots[table->index].entries[i].field_10 != 0;
}
