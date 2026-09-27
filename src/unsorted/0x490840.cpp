// Decompiled by Sonnet. Names are provisional.
// A second, byte-identical copy of the scalar deleting destructor of
// Class_0044f590 (see src/unsorted/0x44f590.cpp and 0x4909a0.cpp for the
// class and its 11-slot vtable at 0x4fd428). Defining the out-of-line
// destructor here again is the key-function trigger that makes MSVC emit
// its own local copy of the vtable and this compiler-synthesised ??_G in
// this translation unit too, exactly like the duplicate std::_Lockit
// (0x4e39b0/0x4e1480).

class Class_0044f590 {
public:
    virtual ~Class_0044f590();
    virtual void FUN_0044ef90(void* param);
    virtual void FUN_0044efb0();
    virtual void FUN_0044ef40(int, int, int);
    virtual void FUN_0044f000(int, int, int);
    virtual int FUN_0044ef80();
    virtual int FUN_0044eff0();
    virtual int FUN_0044efe0();
    virtual void FUN_0044efc0(int);
    virtual void FUN_0044efd0(int);
    virtual void FUN_0044ef50(int);
};

// FUNCTION: 0x490840 ??_GClass_0044f590@@UAEPAXI@Z
Class_0044f590::~Class_0044f590()
{
}
