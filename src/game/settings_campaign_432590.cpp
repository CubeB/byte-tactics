// Decompiled by Opus. Names are provisional.
// Deletes an object whose (out-of-line) destructor is CloseBank.

class HapiBank {
public:
    void CloseBank();
};

// FUNCTION: 0x432590
void __stdcall FUN_00432590(HapiBank* obj)
{
    if (obj) {
        obj->CloseBank();
        operator delete(obj);
    }
}
