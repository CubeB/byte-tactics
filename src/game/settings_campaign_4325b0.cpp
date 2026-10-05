// Decompiled by Opus. Names are provisional.
// Allocates a 4-byte object, initialises it, and opens it with the given
// name and a global string; frees it and returns 0 on failure.
// Sibling of 0x432520.

class HapiBank {
public:
    int field_0;

    HapiBank* InitBank();
    void CloseBank();
    int OpenBank(char* name, char* a, char* b);
};

extern char* DAT_0050331c;

// FUNCTION: 0x4325b0
HapiBank* __stdcall FUN_004325b0(char* name)
{
    HapiBank* mem = new HapiBank;
    HapiBank* obj;
    if (mem != 0) {
        obj = mem->InitBank();
    } else {
        obj = 0;
    }
    if (((HapiBank*)obj)->OpenBank(name, DAT_0050331c, 0) == 0) {
        if (obj != 0) {
            ((HapiBank*)obj)->CloseBank();
            delete obj;
        }
        return 0;
    }
    return obj;
}
