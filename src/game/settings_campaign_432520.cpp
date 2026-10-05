// Decompiled by Opus. Names are provisional.
// Allocates a 4-byte object, initialises it, and opens it with the given
// name and two global strings; frees it and returns 0 on failure.

class HapiBank {
public:
    int field_0;

    HapiBank* InitBank();
    void CloseBank();
    int OpenBank(char* name, char* a, char* b);
    void OpenAccount(char* section);
};

extern char* DAT_0050331c;
extern char* DAT_00503320;

// FUNCTION: 0x432520
HapiBank* __stdcall FUN_00432520(char* name)
{
    HapiBank* mem = new HapiBank;
    HapiBank* obj;
    if (mem != 0) {
        obj = mem->InitBank();
    } else {
        obj = 0;
    }
    if (((HapiBank*)obj)->OpenBank(name, DAT_0050331c, DAT_00503320) == 0) {
        if (obj != 0) {
            ((HapiBank*)obj)->CloseBank();
            delete obj;
        }
        return 0;
    }
    ((HapiBank*)obj)->OpenAccount(DAT_00503320);
    return obj;
}
