// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash. Names are provisional.
// A method of UnitSync, whose other methods are in unit_sync_46c620.cpp.
// Stays in its own file: it needs the real <map>, <list> or <vector>
// instantiations its own way, which unit_sync_46c620.cpp's views cannot share.
//
// The destructor of the 0x68-byte object held at g_game+0x2a30
// (UnitSync, constructor 0x46d040, created by 0x46c8e0). Nothing calls
// it: 0x46ca60 tears the same object down inline and then frees it, so this is
// the out-of-line copy /Ob2 emitted for another delete.
//
// The members are destroyed last-declared first: the trivial vector at +0x48,
// the vector<Elem_0046faf0> at +0x38, the list<int> at +0x20, the
// vector<UnitSyncPlayer> at +0x10 and the std::map at +0x00.
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
    // Declared, not defined: the real one is the out-of-line 0x46e870.
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

class UnitSyncPlayer {             // 0x5c bytes, the vector at +0x10 holds these
public:
    char unknown_0[0x5c];
    ~UnitSyncPlayer();
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

    // _Destroy(_First, _Last) like the real vector: keeps _Last in a saved register.
    ~Vec_0046d1a0()
    {
        _Destroy(_First, _Last);
        alloc.deallocate(_First, _End - _First);
        _First = 0, _Last = 0, _End = 0;
    }
};

class VecElems_0046d1a0 {          // std::vector<UnitSyncPlayer> at +0x10
public:
    std::allocator<UnitSyncPlayer> alloc;
    UnitSyncPlayer* _First;
    UnitSyncPlayer* _Last;
    UnitSyncPlayer* _End;

    void _Destroy(UnitSyncPlayer* _F, UnitSyncPlayer* _L)
    {
        for (; _F != _L; ++_F)
            alloc.destroy(_F);
    }

    // _Destroy(_First, _Last) like the real vector: keeps _Last in a saved register.
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
    // Real std::map: its erase keeps the symbols.csv name.
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
