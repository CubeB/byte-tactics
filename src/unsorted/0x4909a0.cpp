// Decompiled by Sonnet. Names are provisional.

class Base4 {
public:
    virtual ~Base4();
};

// Base class, 11 virtual functions (see src/unsorted/0x44f590.cpp); its own
// destructor is empty inline, so it is folded into the derived destructor
// below. Slots not overridden by the derived class (1,3,5,6,7,8,10) keep
// their base addresses; slots overridden here (2,4,9) are named after the
// derived-class methods that occupy them so the vtable ends up right.
//
// Slots 2, 4 and 9 cannot satisfy both this class's OWN vtable
// (??_7Class_0044f590, used while unwinding to the base part) and the derived
// class's OWN vtable (??_7Class_00490880) at once: a true override must use
// one identical name in both classes, but the base's own copy is a
// different, already-established function (0x44efb0 = FUN_0044efb0,
// src/unsorted/0x44efb0.cpp; 0x44f000 = FUN_0044f000, no file yet;
// 0x44efd0 = FUN_0044efd0, src/unsorted/0x44efd0.cpp) from the derived
// override (0x490690 = Class_00490880::FUN_00490690, no file yet; 0x490650 =
// Class_00490650::FUN_00490650, src/unsorted/0x490650.cpp; 0x490a10 =
// Class_00490a10::FUN_00490a10, src/unsorted/0x490a10.cpp). Named here to
// satisfy the derived override (the more specific, already-qualified match);
// Class_0044f590's own vtable is left with a BAD reference at slots 2, 4 and 9.
class Class_0044f590 {
public:
    virtual ~Class_0044f590() {}
    virtual void FUN_0044ef90(void* param);
    virtual void FUN_00490690();
    virtual void FUN_0044ef40(int, int, int);
    virtual void FUN_00490650(int, int, int);
    virtual int FUN_0044ef80();
    virtual int FUN_0044eff0();
    virtual int FUN_0044efe0();
    virtual void FUN_0044efc0(int);
    virtual void FUN_00490a10(int);
    virtual void FUN_0044ef50(int);
};

class Class_00490880 : public Class_0044f590 {
public:
    Base4* field_4;
    virtual ~Class_00490880();
    virtual void FUN_00490690();
    virtual void FUN_00490650(int, int, int);
    virtual void FUN_00490a10(int);
};

// FUNCTION: 0x4909a0 ??_GClass_00490880@@UAEPAXI@Z
Class_00490880::~Class_00490880()
{
    delete field_4;
    field_4 = 0;
}
