// Decompiled by Opus. Names are provisional.
// Deletes an object whose (out-of-line) destructor is FUN_004b3630.

class Class_004b3630 {
public:
    void FUN_004b3630();
};

// FUNCTION: 0x432590
void __stdcall FUN_00432590(Class_004b3630* obj)
{
    if (obj) {
        obj->FUN_004b3630();
        operator delete(obj);
    }
}
