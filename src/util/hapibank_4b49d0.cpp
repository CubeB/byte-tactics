// Decompiled by deepseek-v4.1-flash. Names are provisional.
#include <string.h>

struct SafeDepositBox {              // 0x14 bytes
    int used;                       // +0x00
    int value;                      // +0x04
    char unknown_8[0xc];
};

struct BankAccount {                 // 0x18 bytes
    char unknown_0[8];
    int count;                      // +0x08
    char unknown_c[8];
    SafeDepositBox* entries;        // +0x14
};

struct AccountList {
    char unknown_0[4];
    BankAccount* slots;             // +0x04
    int index;                      // +0x08
};

void* __cdecl FUN_004d8580(void* ptr, int size);

class HapiBank {
public:
    AccountList* table;             // +0x00
    int FindNumberedBox(int value, int flag);
};

// FUNCTION: 0x4b49d0
int HapiBank::FindNumberedBox(int value, int flag)
{
    AccountList* t = table;
    if (!t || t->index < 0)
        return -1;
    BankAccount* s = &t->slots[t->index];
    for (int i = 0; i < s->count; i++) {
        if (s->entries[i].used == 0 && s->entries[i].value == value)
            return i;
    }
    if (!flag)
        return -1;
    int n = s->count;
    s->count = n + 1;
    s->entries = (SafeDepositBox*)FUN_004d8580(s->entries, (n + 1) * sizeof(SafeDepositBox));
    memset(&s->entries[n], 0, sizeof(SafeDepositBox));
    s->entries[n].value = value;
    s->entries[n].used = 0;
    return n;
}
