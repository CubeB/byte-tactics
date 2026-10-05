// Decompiled by Opus. Names are provisional.
// Allocates a 4-byte object, initialises it, and opens it with the given
// name and two global strings; frees it and returns 0 on failure.

class Class_004b3620 {
public:
    int field_0;

    Class_004b3620* InitBank();
};

class Class_004b3630 {
public:
    void CloseBank();
};

class Class_004b3770 {
public:
    int OpenBank(char* name, char* a, char* b);
};

class HapiBank {
public:
    void OpenAccount(char* section);
};

extern char* DAT_0050331c;
extern char* DAT_00503320;

// FUNCTION: 0x432520
Class_004b3620* __stdcall FUN_00432520(char* name)
{
    Class_004b3620* mem = new Class_004b3620;
    Class_004b3620* obj;
    if (mem != 0) {
        obj = mem->InitBank();
    } else {
        obj = 0;
    }
    if (((Class_004b3770*)obj)->OpenBank(name, DAT_0050331c, DAT_00503320) == 0) {
        if (obj != 0) {
            ((Class_004b3630*)obj)->CloseBank();
            delete obj;
        }
        return 0;
    }
    ((HapiBank*)obj)->OpenAccount(DAT_00503320);
    return obj;
}
