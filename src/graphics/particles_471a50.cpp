// Decompiled by space-bunny-free. Names are provisional.
// Creates a Class_004750b0 (vtable 0x4fd638) from the object pool, initialises
// it through virtual slot 6 (0x475150) with the first four arguments, then
// appends it to the std::vector of pointers selected by the short index in the
// last argument. When that list already holds more than 400 entries its oldest
// element is deleted and erased first. The append lives in an inlined member
// helper, and here std::vector::insert is inlined too, so both its fast path
// and its growth path (0x4b4f10 / 0x4b4f20) appear in the body; the class
// declaration is the one in 0x4750b0.cpp, so the constructor stays an
// out-of-line call. Closest match is 0x472c50.cpp, which builds the same
// class; the differences are the owner (a member here, g_game there) and that
// the insert is out of line there. Operator new (0x471d10) is inlined here as
// memset: one builtin node, which leaves /Ob2's inline budget free for insert.
// The dword loop spelling used in 0x472c50.cpp charges the budget and leaves
// the insert out of line.
// The vector's element type is the one the _Ucopy / _Ufill / _Destroy symbols
// are named after in 0x4732d0.cpp (0x473500, 0x473530 and 0x4732d0 are one
// COMDAT folded from several instantiations, and data/symbols.csv holds the
// Elem_00473500 name for all three), so the appended pointer is cast to it,
// exactly as 0x471820.cpp does.
//
// NOT MATCHING, and only because of a name in data/symbols.csv: every one of
// the 546 bytes matches, and the four references to 0x472d30 are reported as
// "0x472d30 is already named 'Class_00472d30::FUN_00472d30'". 0x472d30 is
// std::vector<T>::size for every T (its body is "_Last ? _Last - _First : 0"
// with the sar 2 of a 4-byte element, one __thiscall argument in ecx), and
// this file has to call it as the size() of a real vector, so the name it emits
// is "UElem_00473500::?$vector::size". No vector instantiation can produce
// the recorded name, so it cannot be fixed from the source. It is the same
// conflict 0x471820.cpp hits, and 0x471a50 is the only caller of 0x472d30
// that inlines the insert, so it is the only file it blocks. A one-line
// aliases.csv entry ("UElem_00473500::?$vector::size,0x472d30,folded COMDAT
// of vector<T>::size, as the _Ucopy entry above is") would settle it, the
// same way the erase overload is handled.
#include <stddef.h>
#include <string.h>
#include <vector>

class Class_00470eb0 {                 // the pool's allocation method
public:
    void* FUN_00470eb0(unsigned int size);
};

// The object pool (see 0x470ae0.cpp); its method returns the object to the
// free list. Needed by the inlined operator new.
class Class_00470ed0 {
public:
    char unknown_0[4];
    void FUN_00470ed0(void* p);
};

extern Class_00470ed0 DAT_0051e610;
extern char DAT_0051e608;

// Vtable 0x4fd5a8, constructor 0x471cc0, destructor 0x471d00, ??_G 0x471cd0.
class Class_00471cc0 {
public:
    int field_4;                                        // +0x4

    Class_00471cc0();
    virtual ~Class_00471cc0();                          // slot 0
    virtual void FUN_00472d50() = 0;                    // slot 1
    virtual void FUN_00472e30(int) = 0;                 // slot 2
    virtual int FUN_00472e70() = 0;                     // slot 3

    static void* __stdcall operator new(size_t size)
    {
        if (DAT_0051e608)
            return 0;
        void* p = ((Class_00470eb0*)&DAT_0051e610)->FUN_00470eb0(size);
        if (p)
            memset(p, 0, size);
        return p;
    }

    static void __stdcall operator delete(void* p)
    {
        DAT_0051e610.FUN_00470ed0(p);
    }
};

class Class_004750b0;

struct Elem_00473500 {                 // the vector's element (see 0x473500.cpp)
    Class_004750b0* p;
};

struct Vec3_00475150 {
    int x;
    int y;
    int z;
};

struct Record_004750b0 {
    int unknown[8];
};

// Vtable 0x4fd638, constructor 0x4750b0, ??_G 0x475110; 0x34 bytes.
class Class_004750b0 : public Class_00471cc0 {
public:
    int time;                                           // +0x8
    std::vector<Record_004750b0> records;               // +0xc (_First +0x10)
    char unknown_1c[0x34 - 0x1c];

    Class_004750b0();
    virtual void FUN_00472d50();                        // slot 1, 0x475600
    virtual void FUN_00472e30(int);                     // slot 2, 0x475700
    virtual int FUN_00472e70();                         // slot 3, 0x475330
    virtual void FUN_004751c0();                        // slot 4, 0x4751c0
    virtual int FUN_004750f0();                         // slot 5, 0x4750f0
    virtual void FUN_00475150(Vec3_00475150* p, int a, int b, int c);  // slot 6
};

// The owner of the per-index lists.
class Class_00471a50 {
public:
    std::vector<Elem_00473500> lists[1];                // 0x10 bytes each

    void Add(short index, Class_004750b0* p)
    {
        if (lists[index].size() > 400) {
            delete lists[index][0].p;
            lists[index].erase(lists[index].begin());
        }
        lists[index].push_back(*(Elem_00473500*)&p);
    }

    void FUN_00471a50(Vec3_00475150* param_1, int param_2, int param_3,
                      int param_4, short index);
};

// FUNCTION: 0x471a50
void Class_00471a50::FUN_00471a50(Vec3_00475150* param_1, int param_2,
                                  int param_3, int param_4, short index)
{
    Class_004750b0* e = new Class_004750b0;
    if (e) {
        e->FUN_00475150(param_1, param_2, param_3, param_4);
        Add(index, e);
    }
}
