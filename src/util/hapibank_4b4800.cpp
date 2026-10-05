// Decompiled by Opus. Names are provisional.
// Reads an integer value from the current section of a parsed text file
// (FindItem finds the key's index); returns `def` when there is no
// current section, the key is missing, or its value is not an integer.

struct BankItem {                      // 0x10 bytes
    char unknown_0[4];
    int type;                          // +0x4, 1 = integer
    int value;                         // +0x8
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

    int GetIntegerItem(char* name, int def);
    int HasItem(const char* param_1);
    int FindItem(const char* param_1, int param_2);
};

// FUNCTION: 0x4b4800
int HapiBank::GetIntegerItem(char* name, int def)
{
    if (file && file->current >= 0) {
        int i = ((HapiBank*)this)->FindItem(name, 0);
        if (i >= 0 && file->sections[file->current].values[i].type == 1)
            return file->sections[file->current].values[i].value;
    }
    return def;
}
