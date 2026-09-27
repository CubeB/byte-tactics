// Decompiled by Opus. Names are provisional.
// Reads a string value from the current section of a parsed text file
// (FUN_004b4910 finds the key's index); returns `def` when there is no
// current section, the key is missing, or its value is not a string.
// Sibling of 0x4b4800 (integer) and 0x4b4850 (float).

struct Value_004b48a0 {                // 0x10 bytes
    char unknown_0[4];
    int type;                          // +0x4, 3 = string
    char* value;                       // +0x8
    char unknown_c[4];
};

struct Section_004b48a0 {              // 0x18 bytes
    char unknown_0[0x10];
    Value_004b48a0* values;            // +0x10
    char unknown_14[4];
};

struct File_004b48a0 {
    char unknown_0[4];
    Section_004b48a0* sections;        // +0x4
    int current;                       // +0x8
};

class Class_004b48f0 {
public:
    int FUN_004b48f0(const char* param_1);
    int FUN_004b4910(const char* param_1, int param_2);
};

class Class_004b48a0 {
public:
    File_004b48a0* file;               // +0x0

    char* FUN_004b48a0(char* name, char* def);
};

// FUNCTION: 0x4b48a0
char* Class_004b48a0::FUN_004b48a0(char* name, char* def)
{
    if (file && file->current >= 0) {
        int i = ((Class_004b48f0*)this)->FUN_004b4910(name, 0);
        if (i >= 0 && file->sections[file->current].values[i].type == 3)
            return file->sections[file->current].values[i].value;
    }
    return def;
}
