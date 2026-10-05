// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Stores a string value under a key of the current section of a parsed text
// file (FindItem with 1 finds or adds the key), duplicating the string and
// freeing the old value first when it was a string; returns 0 when there is no
// current section or the value is null. The string counterpart of 0x4b4630.

struct BankItem {                      // 0x10 bytes
    char unknown_0[4];
    int type;                          // +0x4, 3 = string
    char* value;                       // +0x8
    char unknown_c[4];
};

struct BankAccount {                   // 0x18 bytes
    char unknown_0[0x10];
    BankItem* values;                  // +0x10
    char unknown_14[4];
};

struct AccountList {
    char unknown_0[4];
    BankAccount* sections;             // +0x4
    int current;                       // +0x8
};

void __cdecl FUN_004d85a0(int* param_1);
char* __cdecl GameStrdup(char* s);

class HapiBank {
public:
    AccountList* file;                 // +0x0

    int SetStringItem(const char* name, char* value);
    int HasItem(const char* param_1);
    int FindItem(const char* param_1, int param_2);
};

// FUNCTION: 0x4b4750
int HapiBank::SetStringItem(const char* name, char* value)
{
    if (file && file->current >= 0 && value) {
        int i = ((HapiBank*)this)->FindItem(name, 1);
        if (file->sections[file->current].values[i].type == 3)
            FUN_004d85a0((int*)file->sections[file->current].values[i].value);
        file->sections[file->current].values[i].value = GameStrdup(value);
        file->sections[file->current].values[i].type = 3;
        return 1;
    }
    return 0;
}
