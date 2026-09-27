// Decompiled by Opus. Names are provisional.
// Reads a floating-point value from the current section of a parsed text
// file (FUN_004b4910 finds the key's index); returns `def` when there is no
// current section, the key is missing, or its value is not a float.
// Sibling of 0x4b4800 (the integer version).

struct Value_004b4850 {                // 0x10 bytes
    char unknown_0[4];
    int type;                          // +0x4, 2 = float
    double value;                      // +0x8
};

struct Section_004b4850 {              // 0x18 bytes
    char unknown_0[0x10];
    Value_004b4850* values;            // +0x10
    char unknown_14[4];
};

struct File_004b4850 {
    char unknown_0[4];
    Section_004b4850* sections;        // +0x4
    int current;                       // +0x8
};

class Class_004b48f0 {
public:
    int FUN_004b48f0(const char* param_1);
    int FUN_004b4910(const char* param_1, int param_2);
};

class Class_004b4850 {
public:
    File_004b4850* file;               // +0x0

    double FUN_004b4850(char* name, double def);
};

// FUNCTION: 0x4b4850
double Class_004b4850::FUN_004b4850(char* name, double def)
{
    if (file && file->current >= 0) {
        int i = ((Class_004b48f0*)this)->FUN_004b4910(name, 0);
        if (i >= 0 && file->sections[file->current].values[i].type == 2)
            return file->sections[file->current].values[i].value;
    }
    return def;
}
