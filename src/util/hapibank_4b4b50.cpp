// Decompiled by Opus. Names are provisional.

struct SafeDepositBox {                // 0x14 bytes
    char unknown_0[0x10];
    int value;                         // +0x10
};

struct BankAccount {                   // 0x18 bytes
    char unknown_0[0xc];
    int selected;                      // +0xc
    int unknown_10;
    SafeDepositBox* items;             // +0x14
};

struct AccountList {
    int unknown_0;
    BankAccount* entries;              // +0x4
    int current;                       // +0x8
};

class Class_004b49d0 {
public:
    int FindNumberedBox(int a, int b);
};

class Class_004b4b50 {
public:
    AccountList* table;                // +0x0
    int OpenNumberedBox(int a);
};

// FUNCTION: 0x4b4b50
int Class_004b4b50::OpenNumberedBox(int a)
{
    int r = ((Class_004b49d0*)this)->FindNumberedBox(a, 1);
    table->entries[table->current].selected = r;
    return table->entries[table->current].items[r].value != 0;
}
