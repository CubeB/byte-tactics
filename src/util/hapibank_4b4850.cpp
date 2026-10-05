// Decompiled by Opus. Names are provisional.
// Reads a floating-point value from the current section of a parsed text
// file (FindItem finds the key's index); returns `def` when there is no
// current section, the key is missing, or its value is not a float.
// Sibling of 0x4b4800 (the integer version).

struct BankItem {                      // 0x10 bytes
    char unknown_0[4];
    int type;                          // +0x4, 2 = float
    double value;                      // +0x8
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

class Class_004b48f0 {
public:
    int HasItem(const char* param_1);
    int FindItem(const char* param_1, int param_2);
};

class Class_004b4850 {
public:
    AccountList* file;                 // +0x0

    double GetDoubleItem(char* name, double def);
};

// FUNCTION: 0x4b4850
double Class_004b4850::GetDoubleItem(char* name, double def)
{
    if (file && file->current >= 0) {
        int i = ((Class_004b48f0*)this)->FindItem(name, 0);
        if (i >= 0 && file->sections[file->current].values[i].type == 2)
            return file->sections[file->current].values[i].value;
    }
    return def;
}
