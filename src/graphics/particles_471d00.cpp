// Decompiled by Haiku, class family consolidated by Opus. Names are provisional.
// The out-of-line destructor of Class_00471cc0 (the family is listed in
// 0x471cc0.cpp): an empty body, so only the vtable store is left. The
// scalar deleting destructors compiled in the same file inline it; 0x474d10
// and 0x475110, compiled elsewhere, call it.
#include <stddef.h>

// Vtable 0x4fd5a8, constructor 0x471cc0, destructor 0x471d00, ??_G 0x471cd0.
class Class_00471cc0 {
public:
    int field_4;                                        // +0x4

    Class_00471cc0();
    virtual ~Class_00471cc0();                          // slot 0
    virtual void FUN_00472d50() = 0;                    // slot 1
    virtual void FUN_00472e30(int) = 0;                 // slot 2
    virtual int FUN_00472e70() = 0;                     // slot 3
    static void* __stdcall operator new(size_t size);   // 0x471d10
    static void __stdcall operator delete(void* p);     // 0x471d50
};

// FUNCTION: 0x471d00
Class_00471cc0::~Class_00471cc0()
{
}
