// Decompiled by Haiku, class family consolidated by Opus. Names are provisional.
// Constructor of Class_00471cc0, the base of a family of objects allocated
// from the pool DAT_0051e610 through the class's own operator new (0x471d10)
// and operator delete (0x471d50). The base vtable 0x4fd5a8 holds the virtual
// destructor and three pure virtuals; every derived class overrides those
// three and adds three more of its own (slot 6 initialises the object and
// calls 0x471d70, which sets field_4).
//
//   class           vtable    constructor  ??_G      slots 1-6
//   Class_00471cc0  0x4fd5a8  0x471cc0     0x471cd0  _purecall x3
//   Class_00471430  0x4fd588  inline       0x471430  0x472d50 0x472e30 0x472e70 0x4737c0 0x472e00 0x4736e0
//   Class_00471560  0x4fd5b8  inline       0x471560  0x472eb0 0x472f90 0x472fd0 0x473d50 0x472f60 0x473b50
//   Class_004716a0  0x4fd5d8  inline       0x4716a0  0x473010 0x4730f0 0x473130 0x4743a0 0x4730c0 0x4742c0
//   Class_004717e0  0x4fd5f8  inline       0x4717e0  0x473170 0x473250 0x473290 0x474880 0x473220 0x474760
//   Class_00474cd0  0x4fd618  0x474cd0     0x474d10  0x475340 0x475470 0x474f80 0x474df0 0x475440 0x474d50
//   Class_004750b0  0x4fd638  0x4750b0     0x475110  0x475600 0x475700 0x475330 0x4751c0 0x4750f0 0x475150
//
// The base destructor is 0x471d00. An override keeps the name of the base
// slot it overrides, so slots 1-3 of every derived class carry the names of
// Class_00471430's (0x472d50, 0x472e30, 0x472e70); the matched slot methods
// are still recorded under their own placeholder classes, which the checker
// accepts for vtable slots.
//
// The first four derived classes have no out-of-line constructor: the
// functions that create them (0x471340, 0x471470, 0x4715a0, 0x4716e0 and
// others) inline it after the inlined operator new. Those functions sit in
// this class's own file, which is why their ??_G inline the base destructor
// and operator delete, while 0x474d10 and 0x475110 (compiled with
// 0x474cd0 and 0x4750b0) call them. All files of the family copy the
// declarations below verbatim.
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

// FUNCTION: 0x471cc0
Class_00471cc0::Class_00471cc0()
{
    field_4 = 0;
}
