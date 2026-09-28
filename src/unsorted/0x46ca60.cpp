// Decompiled by GPT-5.6-Terra. Names are provisional.
// Partial: MSVC reloads the list iterator before the erase-loop bottom test;
// the original compares its stack slot directly.
#include <list>
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

struct Node_0046e890 {
    Node_0046e890* left;
    Node_0046e890* parent;
    Node_0046e890* right;
};

extern Node_0046e890* DAT_0051e598;
extern int DAT_0051e59c;

class Class_0046ea10 {
public:
    Node_0046e890* ptr;

    Class_0046ea10() {}
    Class_0046ea10(Node_0046e890* p) : ptr(p) {}
    bool operator==(const Class_0046ea10& o) const { return ptr == o.ptr; }
    bool operator!=(const Class_0046ea10& o) const { return !(*this == o); }
};

class Class_0046e890 {
public:
    int field_0;
    Node_0046e890* head;
    int field_8;
    int size;

    Class_0046ea10 begin() { return Class_0046ea10(head->left); }
    Class_0046ea10 end() { return Class_0046ea10(head); }
    Class_0046ea10 erase(Class_0046ea10 _F, Class_0046ea10 _L);

    void deallocate(void* _P, unsigned int) { operator delete(_P); }
    void _Freenode(Node_0046e890* _S) { deallocate(_S, 1); }

    ~Class_0046e890()
    {
        erase(begin(), end());
        _Freenode(head);
        head = 0, size = 0;
        std::_Lockit Lk;
        if (--DAT_0051e59c == 0) {
            _Freenode(DAT_0051e598);
            DAT_0051e598 = 0;
        }
    }
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

class Class_0046d040 {
public:
    Class_0046e890 rects;
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

    ~Class_0046d040() {}
};

class Class_0046e160 {
public:
    void FUN_0046e160();
};

struct Game_0046ca60 {
    char unknown_0[0x2a30];
    Class_0046d040* field_2a30;
};

extern Game_0046ca60* g_game;

// FUNCTION: 0x46ca60
void FUN_0046ca60()
{
    ((Class_0046e160*)g_game->field_2a30)->FUN_0046e160();
    delete g_game->field_2a30;
    g_game->field_2a30 = 0;
}
