// Decompiled by space-bunny-free. Names are provisional.
// The destructor of the 0x68-byte object held at g_game+0x2a30
// (Class_0046d040, constructor 0x46d040, created by 0x46c8e0). Nothing calls
// it: 0x46ca60 tears the same object down inline and then frees it, so this is
// the out-of-line copy /Ob2 emitted for another delete.
//
// The body is nothing but the members' destructors, in reverse declaration
// order: the vector<int> at +0x48, the vector<Elem_0046faf0> at +0x38, the
// list<int> at +0x20, the vector<Class_0046ded0> at +0x10 and the
// std::map<unsigned int, Rect> at +0x00. Every one of them is at the offset
// its first pointer field uses, so the map is written out by hand as
// Class_0046e890: that keeps the erase call going to the name already
// recorded for 0x46e890, and its destructor below is the stock ~_Tree from
// MSVC 5's <xtree> (erase(begin(), end()), free the head node, then the
// _Lockit and --_Nilrefs block), which is why DAT_0051e598 (the tree's shared
// _Nil node) and DAT_0051e59c (its reference count) appear here.
//
// Best: 85%. What still differs: the original calls
// vector<Elem_0046faf0>::_Destroy (0x46e870) OUT OF LINE to destroy the
// elements of the vector at +0x38, and this file inlines it away (the element
// destructor is trivial, so the body is empty). Everything else, including the
// out-of-line list<int>::erase (0x46eb60), iterator::operator++ (0x46fac0),
// ~Class_0046ded0 (0x46ded0), the map's erase (0x46e890) and both
// std::_Lockit calls, matches. MSVC 5 always inlines a call whose body it
// can see, and 0x46e870 is `ret 8`, so no amount of /Ob2 budget pressure
// (extra empty inline calls before the vector, extra members in the element
// class, extra destructor nesting) leaves the call in place: those all make
// _Destroy inline as a loop instead. The element type may have been
// non-trivial in the original, which would make _Destroy a real function, but
// the out-of-line copy at 0x46e870 is empty, so the element type was
// trivial and MSVC 5 was called not to inline it.
#include <list>
#include <vector>
#include <yvals.h>

extern void* DAT_0051e598;          // the red-black tree's shared _Nil node
extern int DAT_0051e59c;            // its reference count (_Nilrefs)

struct Node_0046e890 {              // a map node, only its first field is read
    void* left;                     // +0x0
    void* parent;                   // +0x4
    void* right;                    // +0x8
};

struct IterBase_0046e890 { void* ptr; };
class Iter_0046e890 : public IterBase_0046e890 {   // 4 bytes, returned through
public:                                            // a hidden pointer
    Iter_0046e890() {}
    Iter_0046e890(void* p) { ptr = p; }
};

class Class_0046e890 {             // the std::map<unsigned int, Rect> at +0x00
public:
    int field_0;                   // +0x00
    Node_0046e890* head;            // +0x04  the tree's _Head node
    int field_8;                   // +0x08  the _Multi flag
    int size;                      // +0x0c  the tree's _Size

    Iter_0046e890 begin() { return Iter_0046e890(head->left); }
    Iter_0046e890 end() { return Iter_0046e890(head); }
    Iter_0046e890 erase(Iter_0046e890 f, Iter_0046e890 l);

    ~Class_0046e890()
    {
        erase(begin(), end());
        operator delete(head);
        head = 0;
        size = 0;
        {
            std::_Lockit lock;
            if (--DAT_0051e59c == 0) {
                operator delete(DAT_0051e598);
                DAT_0051e598 = 0;
            }
        }
    }
};

#pragma pack(push, 2)
struct Elem_0046faf0 {             // the vector at +0x38 holds these
    int a;                         // +0x0
    int b;                         // +0x4
    int c;                         // +0x8
    short d;                       // +0xc
};
#pragma pack(pop)

class Class_0046ded0 {             // 0x5c bytes, the vector at +0x10 holds these
public:
    char unknown_0[0x5c];
    ~Class_0046ded0();
};

class Class_0046d040 {
public:
    Class_0046e890 rects;                       // +0x00
    std::vector<Class_0046ded0> elems;          // +0x10
    std::list<int> ids;                         // +0x20
    int field_2c;                               // +0x2c
    int field_30;                               // +0x30
    int field_34;                               // +0x34
    std::vector<Elem_0046faf0> field_38;        // +0x38
    std::vector<int> field_48;                  // +0x48
    int field_58;                               // +0x58
    int field_5c;                               // +0x5c
    int field_60;                               // +0x60
    int field_64;                               // +0x64

    ~Class_0046d040();
};

// FUNCTION: 0x46d1a0
Class_0046d040::~Class_0046d040()
{
}
