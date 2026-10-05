// Decompiled by GPT-5.6-Terra, finished by Sonnet 5.5. Names are provisional.
//
// The function must be __fastcall (no arguments, so the code is otherwise
// unchanged; or the TU was built with /Gr). As a plain __cdecl function MSVC
// reloads the list iterator before the erase loop's bottom test, `mov edx,[esp+0x10];
// cmp edx,ebp`, where the original compares the slot directly. The map at +0x00 is
// the real std::map<unsigned int, UnitSyncEntry> so its erase keeps its data/symbols.csv
// name, as in 0x46d1a0.cpp.
#include <list>
#include <map>
#include <xmemory>

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
        operator delete(_First);
        _First = 0, _Last = 0, _End = 0;
    }
protected:
    void _Destroy(iterator _F, iterator _L);
};

}

#pragma pack(push, 2)
struct Elem_0046faf0 {
    int a;
    int b;
    int c;
    short d;
};
#pragma pack(pop)

typedef std::list<int> List_0046ca60;
typedef List_0046ca60::iterator (List_0046ca60::*EraseFn_0046ca60)(List_0046ca60::iterator);
typedef List_0046ca60::iterator (List_0046ca60::iterator::*PostIncFn_0046ca60)(int);

EraseFn_0046ca60 g_erase_0046ca60 = &List_0046ca60::erase;
PostIncFn_0046ca60 g_postinc_0046ca60 = &List_0046ca60::iterator::operator++;

struct UnitSyncEntry {                 // the map mapped type, 0x10 bytes
    int x;
    int y;
    short w;
    short h;
    int unknown_c;
};

class VecInt_0046ca60 {               // std::vector<int>
public:
    std::allocator<int> alloc;
    int* _First;
    int* _Last;
    int* _End;

    ~VecInt_0046ca60()
    {
        operator delete(_First);
        _First = 0, _Last = 0, _End = 0;
    }
};

class Class_0046ded0 {                 // 0x5c bytes
public:
    int field_0;                       // +0x00
    VecInt_0046ca60 list_a;            // +0x04
    VecInt_0046ca60 list_b;            // +0x14
    char unknown_24[0x18];             // +0x24
    VecInt_0046ca60 list_c;            // +0x3c
    VecInt_0046ca60 list_d;            // +0x4c
};

static inline void DestroyR0_0046ca60(Class_0046ded0* _F, Class_0046ded0* _L);
static inline void DestroyR1_0046ca60(Class_0046ded0* _F, Class_0046ded0* _L);
static inline void DestroyR2_0046ca60(Class_0046ded0* _F, Class_0046ded0* _L);
class VecElems_0046ca60 {              // std::vector<Class_0046ded0>
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

    ~VecElems_0046ca60()
    {
        DestroyR0_0046ca60(_First, _Last);

        operator delete(_First);
        _First = 0, _Last = 0, _End = 0;
    }
};

static inline void DestroyR0_0046ca60(Class_0046ded0* _F, Class_0046ded0* _L)
{
    DestroyR1_0046ca60(_F, _L);
}

static inline void DestroyR1_0046ca60(Class_0046ded0* _F, Class_0046ded0* _L)
{
    DestroyR2_0046ca60(_F, _L);
}

static inline void DestroyR2_0046ca60(Class_0046ded0* _F, Class_0046ded0* _L)
{
    std::allocator<Class_0046ded0> alloc;
    for (Class_0046ded0* p = _F; p != _L; ++p)
        alloc.destroy(p);
}

class UnitSync {
public:
    std::map<unsigned int, UnitSyncEntry> rects;
    VecElems_0046ca60 elems;
    std::list<int> ids;
    int field_2c;
    int field_30;
    int field_34;
    std::vector<Elem_0046faf0> field_38;
    std::vector<Elem_0046faf0> field_48;
    int field_58;
    int field_5c;
    int field_60;
    int field_64;

    ~UnitSync() {}
};

class Class_0046e160 {
public:
    void ApplyToUnitTypes();
};

struct Game {
    char unknown_0[0x2a30];
    UnitSync* field_2a30;
};

extern Game* g_game;

// FUNCTION: 0x46ca60
void __fastcall FinishUnitSync()
{
    ((Class_0046e160*)g_game->field_2a30)->ApplyToUnitTypes();
    delete g_game->field_2a30;
    g_game->field_2a30 = 0;
}
