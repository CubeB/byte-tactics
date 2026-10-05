// Decompiled by deepseek-v4.1-flash. Names are provisional.
#include <string.h>

extern int __cdecl _strcmpi(const char*, const char*);

struct BankItem {                      // 0x10 bytes
    char* name;                        // +0x00
    int type;                          // +0x04
    int unknown_8;                     // +0x08
    int unknown_c;                     // +0x0c
};

struct BankAccount {                   // 0x18 bytes
    char unknown_0[4];
    int count;                         // +0x04
    char unknown_8[8];
    BankItem* values;                  // +0x10
    char unknown_14[4];
};

struct AccountList {
    char unknown_0[4];
    BankAccount* sections;             // +0x04
    int current;                       // +0x08
};

void* __cdecl FUN_004d8580(void* ptr, int size);
char* __cdecl GameStrdup(char* s);

class HapiBank {
public:
    AccountList* file;                 // +0x00
    int FindItem(char* name, int flag);
};

// FUNCTION: 0x4b4910
int HapiBank::FindItem(char* name, int flag)
{
    AccountList* f = file;
    if (!f || f->current < 0)
        return -1;
    BankAccount* s = &f->sections[f->current];
    for (int i = 0; i < s->count; i++) {
        if (_strcmpi(s->values[i].name, name) == 0)
            return i;
    }
    if (!flag)
        return -1;
    int n = s->count;
    s->count = n + 1;
    s->values = (BankItem*)FUN_004d8580(s->values, (n + 1) * sizeof(BankItem));
    memset(&s->values[n], 0, sizeof(BankItem));
    s->values[n].name = GameStrdup(name);
    return n;
}
