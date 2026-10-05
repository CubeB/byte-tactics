// Decompiled by Sonnet. Names are provisional.
// Same shape as the matched sibling 0x432c00: a scalar deleting destructor
// that calls the reference-count release at 0x4c9390 (already named
// Class_004c9390::ReleaseRef in data/symbols.csv and called as a plain
// method by every other caller) then conditionally frees this.

extern void __cdecl operator delete(void*);

class Class_004c9390 {
public:
    void ReleaseRef();
};

class Class_00432c20 {
public:
    void* FUN_00432c20(unsigned char param_1);
};

// FUNCTION: 0x432c20
void* Class_00432c20::FUN_00432c20(unsigned char param_1)
{
    ((Class_004c9390*)this)->ReleaseRef();
    if ((param_1 & 1) != 0) {
        operator delete(this);
    }
    return this;
}
