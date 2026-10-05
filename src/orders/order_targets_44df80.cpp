// Decompiled by Sonnet. Names are provisional.
// Scalar-deleting-destructor shape: unlink the embedded list node at +0x16
// (via UnitRef::FUN_00489650, the same unlink method used elsewhere),
// restore this object's own vtable, conditionally operator delete, and
// return `this`.

extern void __cdecl operator delete(void*);
extern void* DAT_004fd2f8[];

class UnitRef {
public:
    void FUN_00489650();
};

class Class_0044df80 {
public:
    void** vtable;
    char unknown_4[0x12];

    void* FUN_0044df80(unsigned char flag);
};

// FUNCTION: 0x44df80
void* Class_0044df80::FUN_0044df80(unsigned char flag)
{
    ((UnitRef*)((char*)this + 0x16))->FUN_00489650();
    vtable = DAT_004fd2f8;
    if (flag & 1) {
        operator delete(this);
    }
    return this;
}
