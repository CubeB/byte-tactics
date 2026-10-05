// Decompiled by Opus. Names are provisional.
// Reads a string value from the current section of a parsed text file
// (FindItem finds the key's index); returns `def` when there is no
// current section, the key is missing, or its value is not a string.
// Sibling of 0x4b4800 (integer) and 0x4b4850 (float).

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

class HapiBank {
public:
    AccountList* file;                 // +0x0

    char* GetStringItem(char* name, char* def);
    int HasItem(const char* param_1);
    int FindItem(const char* param_1, int param_2);
};

// FUNCTION: 0x4b48a0
char* HapiBank::GetStringItem(char* name, char* def)
{
    if (file && file->current >= 0) {
        int i = ((HapiBank*)this)->FindItem(name, 0);
        if (i >= 0 && file->sections[file->current].values[i].type == 3)
            return file->sections[file->current].values[i].value;
    }
    return def;
}
