// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash. Names are provisional.
// The destructor of the 0x68-byte object held at g_game+0x2a30
// (UnitSync, constructor 0x46d040, created by 0x46c8e0). Nothing calls
// it: 0x46ca60 tears the same object down inline and then frees it, so this is
// the out-of-line copy /Ob2 emitted for another delete.
//
// The members are destroyed last-declared first: the trivial vector at +0x48,
// the vector<Elem_0046faf0> at +0x38, the list<int> at +0x20, the
// vector<Class_0046ded0> at +0x10 and the std::map at +0x00.
//
// vector<Elem_0046faf0>::_Destroy is empty (its element is trivial), yet the
// original calls it out of line from the +0x38 member only. As in
// 0x46ca60.cpp, std::vector is declared here with _Destroy declared but not
// defined (the real one lives at 0x46e870), and the trivial-element vectors at
// +0x48 and +0x10 are hand-written lookalikes. Three details keep the other
// calls out of line where /Ob2 would otherwise inline them:
//  - ~Vec_0046d1a0 and ~VecElems_0046d1a0 call _Destroy(_First, _Last) the way
//    the real vector does, so the element walk keeps _Last in a callee-saved
//    register across the element destructor (otherwise it reloads it).
//  - the list member is one derived class deeper (ListWrap_0046d1a0): at the
//    extra inline depth ~list's erase(_F++) calls iterator::operator++
//    (0x46fac0) and erase (0x46eb60) instead of folding the increment.
//  - the map is the real std::map<unsigned int, UnitSyncEntry>, so its erase
//    keeps the name 0x46e890 already has in data/symbols.csv, and the shared
//    _Nil / _Nilrefs globals the tree frees come out named too.
#include <list>
#include <map>

namespace std {

template<class T, class A = allocator<T> > class vector {
public:
    typedef A::size_type size_type;
    typedef T* iterator;
    typedef T* const_iterator;
    A alloc;
    iterator _First, _Last, _End;

    ~vector()
    {
        _Destroy(_First, _Last);
        alloc.deallocate(_First, _End - _First);
        _First = 0, _Last = 0, _End = 0;
    }
protected:
    void _Destroy(iterator _F, iterator _L);
};

}

#pragma pack(push, 2)
struct Elem_0046faf0 {             // the 14-byte element type
    int a;                         // +0x0
    int b;                         // +0x4
    int c;                         // +0x8
    short d;                       // +0xc
};
#pragma pack(pop)

struct UnitSyncEntry {             // the map's mapped type, 0x10 bytes
    int x;                         // +0x0
    int y;                         // +0x4
    short w;                       // +0x8
    short h;                       // +0xa
    int unknown_c;                 // +0xc
};

class Class_0046ded0 {             // 0x5c bytes, the vector at +0x10 holds these
public:
    char unknown_0[0x5c];
    ~Class_0046ded0();
};

class Vec_0046d1a0 {               // the trivial vector at +0x48
public:
    std::allocator<int> alloc;
    int* _First;
    int* _Last;
    int* _End;

    void _Destroy(int* _F, int* _L)
    {
        for (; _F != _L; ++_F)
            alloc.destroy(_F);
    }

    ~Vec_0046d1a0()
    {
        _Destroy(_First, _Last);
        alloc.deallocate(_First, _End - _First);
        _First = 0, _Last = 0, _End = 0;
    }
};

class VecElems_0046d1a0 {          // std::vector<Class_0046ded0> at +0x10
public:
    std::allocator<Class_0046ded0> alloc;
    Class_0046ded0* _First;
    Class_0046ded0* _Last;
    Class_0046ded0* _End;

    void _Destroy(Class_0046ded0* _F, Class_0046ded0* _L)
    {
        for (; _F != _L; ++_F)
            alloc.destroy(_F);
    }

    ~VecElems_0046d1a0()
    {
        _Destroy(_First, _Last);
        alloc.deallocate(_First, _End - _First);
        _First = 0, _Last = 0, _End = 0;
    }
};

typedef std::list<int> List_0046d1a0;
typedef List_0046d1a0::iterator (List_0046d1a0::*EraseFn_0046d1a0)(List_0046d1a0::iterator);
typedef List_0046d1a0::iterator (List_0046d1a0::iterator::*PostIncFn_0046d1a0)(int);

EraseFn_0046d1a0 g_erase_0046d1a0 = &List_0046d1a0::erase;
PostIncFn_0046d1a0 g_postinc_0046d1a0 = &List_0046d1a0::iterator::operator++;

class ListWrap_0046d1a0 : public List_0046d1a0 {   // one inline level deeper, so
};                                                 // ~list's erase(_F++) is a call

class UnitSync {
public:
    std::map<unsigned int, UnitSyncEntry> rects;  // +0x00
    VecElems_0046d1a0 elems;              // +0x10
    ListWrap_0046d1a0 ids;                // +0x20
    int field_2c;                         // +0x2c
    int field_30;                         // +0x30
    int field_34;                         // +0x34
    std::vector<Elem_0046faf0> field_38;  // +0x38
    Vec_0046d1a0 field_48;                // +0x48
    int field_58;                         // +0x58
    int field_5c;                         // +0x5c
    int field_60;                         // +0x60
    int field_64;                         // +0x64

    ~UnitSync();
};

// FUNCTION: 0x46d1a0
UnitSync::~UnitSync()
{
}
