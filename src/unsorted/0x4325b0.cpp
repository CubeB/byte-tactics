// Decompiled by Opus. Names are provisional.
// Allocates a 4-byte object, initialises it, and opens it with the given
// name and a global string; frees it and returns 0 on failure.
// Sibling of 0x432520.

class Class_004b3620 {
public:
    int field_0;

    Class_004b3620* FUN_004b3620();
};

class Class_004b3630 {
public:
    void FUN_004b3630();
};

class Class_004b3770 {
public:
    int FUN_004b3770(char* name, char* a, char* b);
};

extern char* DAT_0050331c;

// FUNCTION: 0x4325b0
Class_004b3620* __stdcall FUN_004325b0(char* name)
{
    Class_004b3620* mem = new Class_004b3620;
    Class_004b3620* obj;
    if (mem != 0) {
        obj = mem->FUN_004b3620();
    } else {
        obj = 0;
    }
    if (((Class_004b3770*)obj)->FUN_004b3770(name, DAT_0050331c, 0) == 0) {
        if (obj != 0) {
            ((Class_004b3630*)obj)->FUN_004b3630();
            delete obj;
        }
        return 0;
    }
    return obj;
}
