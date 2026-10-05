// Decompiled by Sonnet. Names are provisional.

struct SafeDepositBox {
    char unknown_0[8];
    int value;                          // +0x8
    char unknown_c[8];
};

struct BankAccount {
    char unknown_0[0xc];
    int subIndex;                       // +0xc
    char unknown_10[4];
    SafeDepositBox* ptr2;               // +0x14
};

struct AccountList {
    char unknown_0[4];
    BankAccount* base;                  // +0x4
    int index;                          // +0x8
};

class HapiBank {
public:
    AccountList* table;                 // +0x0

    int GetBoxSize();
};

// FUNCTION: 0x4b4bf0
int HapiBank::GetBoxSize()
{
    AccountList* p = table;
    BankAccount* a = &p->base[p->index];
    int subIdx = p->base[p->index].subIndex;
    return a->ptr2[subIdx].value;
}
