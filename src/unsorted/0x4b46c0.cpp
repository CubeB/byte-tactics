// Decompiled by Opus. Names are provisional.
// Stores a double value under a key of the current section of a parsed text
// file (FUN_004b4910 with 1 finds or adds the key), freeing the old value
// first when it was a string; returns 0 when there is no current section.
// The double counterpart of 0x4b4630.

struct Value_004b46c0 {                // 0x10 bytes
    char unknown_0[4];
    int type;                          // +0x4, 1 = integer, 2 = double, 3 = string
    union {
        int value;                     // +0x8
        double real;                   // +0x8
    };
};

struct Section_004b46c0 {              // 0x18 bytes
    char unknown_0[0x10];
    Value_004b46c0* values;            // +0x10
    char unknown_14[4];
};

struct File_004b46c0 {
    char unknown_0[4];
    Section_004b46c0* sections;        // +0x4
    int current;                       // +0x8
};

class Class_004b48f0 {
public:
    int FUN_004b48f0(const char* param_1);
    int FUN_004b4910(const char* param_1, int param_2);
};

void FUN_004d85a0(int* param_1);

class Class_004b46c0 {
public:
    File_004b46c0* file;               // +0x0

    int FUN_004b46c0(const char* name, double value);
};

// FUNCTION: 0x4b46c0
int Class_004b46c0::FUN_004b46c0(const char* name, double value)
{
    if (file && file->current >= 0) {
        int i = ((Class_004b48f0*)this)->FUN_004b4910(name, 1);
        if (file->sections[file->current].values[i].type == 3)
            FUN_004d85a0((int*)file->sections[file->current].values[i].value);
        file->sections[file->current].values[i].real = value;
        file->sections[file->current].values[i].type = 2;
        return 1;
    }
    return 0;
}
