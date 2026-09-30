// Decompiled by deepseek-v4.1. Names are provisional.
// Allocator address map lookup, the same tree 0x4daa30 walks (the one
// FUN_004da8d0 lazily creates; its _Lbound is 0x4dd3d0). Under the allocator
// lock it builds the 0x30-byte Class_004d8820 search record, runs the tree's
// inline find() (lower_bound plus the `_P == end() || _Kfn(_Kv, _Key(_P)) ?
// end() : _P` test) and returns the found record's second dword, or 0 when the
// lookup yields end().
//
// Shape notes that decide the bytes:
//  - `tree` must be a named local fetched BEFORE `rec` is constructed: that is
//    what lands `call FUN_004da8d0` before the Class_004d8820 ctor argument
//    pushes (writing `FUN_004da8d0()->find(rec.key)` instead evaluates the ctor
//    first and scores 95.3 percent with that single reordered block as the
//    only hunk). The final end() test calls FUN_004da8d0() a second time, so
//    the local only supplies the lower_bound `this`.
//  - the frame is 13 dwords: 12 for the record plus one for the unnamed End()
//    temporary of the ternary; the iterator itself has no slot, it reuses the
//    key parameter's home.
#include <windows.h>

class Class_004d8820 {
public:
    unsigned int key;              // +0x0
    unsigned int field_4;          // +0x4
    unsigned int field_8;          // +0x8
    char unknown_c[0x20];          // +0xc
    unsigned int field_2c;         // +0x2c

    Class_004d8820(unsigned int a, unsigned int b, unsigned int c, unsigned int d,
                   const char* e);
};

struct Node_004dbae0 {
    Node_004dbae0* left;           // +0x0
    Node_004dbae0* parent;         // +0x4
    Node_004dbae0* right;          // +0x8
    Class_004d8820 value;          // +0xc
    unsigned int color;            // +0x3c
};

struct Less_004dd3d0 {
    bool operator()(const unsigned int& a, const unsigned int& b) const { return a < b; }
};

class Iter_004dd3d0 {
public:
    Node_004dbae0* ptr;            // +0x0

    Iter_004dd3d0() {}
    Iter_004dd3d0(Node_004dbae0* p) : ptr(p) {}
    bool operator==(const Iter_004dd3d0& o) const { return ptr == o.ptr; }
};

class Class_004dd3d0 {
public:
    Less_004dd3d0 compare;         // +0x0
    Node_004dbae0* head;           // +0x4

    Iter_004dd3d0 End() { return Iter_004dd3d0(head); }
    Iter_004dd3d0 FUN_004dd3d0(const unsigned int& key);
    Iter_004dd3d0 find(const unsigned int& key)
    {
        Iter_004dd3d0 it = FUN_004dd3d0(key);
        return (it == End() || compare(key, it.ptr->value.key)) ? End() : it;
    }
};

Class_004dd3d0* FUN_004da8d0();
LPCRITICAL_SECTION FUN_004da780();

// FUNCTION: 0x4dbae0
unsigned int __cdecl FUN_004dbae0(unsigned int key)
{
    LPCRITICAL_SECTION cs = FUN_004da780();
    EnterCriticalSection(cs);
    Class_004dd3d0* tree = FUN_004da8d0();
    Class_004d8820 rec(key, 0, 0, 0, 0);
    Iter_004dd3d0 it = tree->find(rec.key);
    if (it == FUN_004da8d0()->End()) {
        LeaveCriticalSection(cs);
        return 0;
    }
    unsigned int value = it.ptr->value.field_4;
    LeaveCriticalSection(cs);
    return value;
}
