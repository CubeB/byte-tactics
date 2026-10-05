// Decompiled by Opus. Names are provisional.
// Deletes an object whose (out-of-line) destructor is CloseBank.

class Class_004b3630 {
public:
    void CloseBank();
};

// FUNCTION: 0x432590
void __stdcall FUN_00432590(Class_004b3630* obj)
{
    if (obj) {
        obj->CloseBank();
        operator delete(obj);
    }
}
