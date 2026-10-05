// Decompiled by Opus. Names are provisional.

struct SafeDepositBox {             // 0x14 bytes
    char unknown_0[0x10];
    int field_10;                   // +0x10
};

struct BankAccount {                // 0x18 bytes
    char unknown_0[0xc];
    int current;                    // +0x0c
    char unknown_10[4];
    SafeDepositBox* entries;        // +0x14
};

struct AccountList {
    char unknown_0[4];
    BankAccount* slots;             // +0x04
    int index;                      // +0x08
};

class Class_004b4a80 {
public:
    int FindNamedBox(char* name, int flag);
};

class Class_004b4ba0 {
public:
    AccountList* table;             // +0x00
    int OpenNamedBox(char* name);
};

// FUNCTION: 0x4b4ba0
int Class_004b4ba0::OpenNamedBox(char* name)
{
    int i = ((Class_004b4a80*)this)->FindNamedBox(name, 1);
    table->slots[table->index].current = i;
    return table->slots[table->index].entries[i].field_10 != 0;
}
