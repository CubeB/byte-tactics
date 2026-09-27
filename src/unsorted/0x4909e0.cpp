// Decompiled by Opus. Names are provisional.
// Out-of-line destructor of Class_00490880 (its scalar deleting destructor,
// 0x4909a0, inlines the same body). The empty inline base destructor leaves
// only the base vtable store. Class declarations copied from 0x4909a0.cpp.

class Base4 {
public:
    virtual ~Base4();
};

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

// FUNCTION: 0x4909e0
Class_00490880::~Class_00490880()
{
    delete field_4;
    field_4 = 0;
}
