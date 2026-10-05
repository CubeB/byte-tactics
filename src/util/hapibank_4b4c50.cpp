// Decompiled by Opus. Names are provisional.
// Resets field +0xc of the current item of the current entry to field +0x8
// and returns it (the returned value is what keeps it in eax).

struct SafeDepositBox {                // 0x14 bytes
    char unknown_0[8];
    int field_8;                       // +0x8
    int field_c;                       // +0xc
    char unknown_10[4];
};

struct BankAccount {                   // 0x18 bytes
    char unknown_0[0xc];
    int cur;                           // +0xc
    char unknown_10[4];
    SafeDepositBox* items;             // +0x14
};

struct AccountList {
    char unknown_0[4];
    BankAccount* entries;              // +0x4
    int cur;                           // +0x8
};

class HapiBank {
public:
    AccountList* data;                 // +0x0

    int SeekBoxEnd();
};

// FUNCTION: 0x4b4c50
int HapiBank::SeekBoxEnd()
{
    BankAccount* e = &data->entries[data->cur];
    SafeDepositBox* it = &e->items[e->cur];
    it->field_c = it->field_8;
    return it->field_c;
}
