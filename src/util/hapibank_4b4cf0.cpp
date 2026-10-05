// Decompiled by Opus. Names are provisional.
#include <string.h>

struct SafeDepositBox {             // 0x14 bytes
    char unknown_0[8];
    int capacity;                   // +0x08
    int size;                       // +0x0c
    char* data;                     // +0x10
};

struct BankAccount {                // 0x18 bytes
    char unknown_0[0xc];
    int current;                    // +0x0c
    char unknown_10[4];
    SafeDepositBox* chunks;         // +0x14
};

struct AccountList {
    char unknown_0[4];
    BankAccount* slots;             // +0x04
    int index;                      // +0x08
};

int* __cdecl FUN_004d8580(int* param_1, int param_2);

class HapiBank {
public:
    AccountList* table;             // +0x00
    int WriteBox(void* src, int len);
};

// FUNCTION: 0x4b4cf0
int HapiBank::WriteBox(void* src, int len)
{
    SafeDepositBox* c = &table->slots[table->index].chunks[table->slots[table->index].current];
    int need = len + c->size;
    if (need > c->capacity) {
        c->data = (char*)FUN_004d8580((int*)c->data, need);
        c->capacity = need;
    }
    memcpy(c->data + c->size, src, len);
    c->size += len;
    return len;
}
