// Decompiled by Space Bunny Free. Names are provisional.
// Grows the allocator: reserves a block of at least `size` bytes with
// VirtualAlloc (rounded up to 8k, and doubled so the block has room to grow),
// retrying with half the size while the reservation lands above 2Gb, and then
// records the block in the free-block map, merging it with the neighbours it
// touches. Same std::map idiom as 0x4db000: the block is the map's value_type,
// a base pointer and a length, and the _Ubound result plus the decremented
// iterator are compared with the tree's head (End()).
#include <windows.h>

struct Node_004db450 {
    Node_004db450* left;               // +0x0
    Node_004db450* parent;             // +0x4
    Node_004db450* right;              // +0x8
    unsigned int key;                  // +0xc
    int length;                        // +0x10
    int color;                         // +0x14
};

// The tree's iterator: one pointer. FUN_004dd2a0 is its operator--.
class Class_004dd2a0 {
public:
    Node_004db450* ptr;

    Class_004dd2a0() {}
    Class_004dd2a0(Node_004db450* q) : ptr(q) {}
    bool operator==(const Class_004dd2a0& o) const { return ptr == o.ptr; }
    void FUN_004dd2a0();
};

// The map's value_type: the block's base address and its length, 8 bytes.
struct Pair_004db450 {
    unsigned int offset;               // +0x0
    int length;                        // +0x4
};

// The map's own methods, all called on the allocator itself.
class Class_004dc620 {
public:
    void FUN_004dc620(Class_004dd2a0* out, const unsigned int& kv);
};

class Class_004dc130 {
public:
    void FUN_004dc130(Class_004dd2a0* out, Class_004dd2a0 it);
};

// begin(), out of line: it stores the tree's first node through its argument.
class Class_004dbeb0 {
public:
    int* FUN_004dbeb0(int* p);
};

class Class_004dbec0 {
public:
    void FUN_004dbec0(Class_004dd2a0* it, Pair_004db450* p);
};

class Class_004db450 {
public:
    char unknown_0[4];
    Node_004db450* head;               // +0x4
    char unknown_8[8];
    int total;                         // +0x10

    // The original tests the iterators as a value (sete; neg; sbb; inc; test),
    // which MSVC 5 only does for a `!` applied to a bool-returning member.
    bool Neq(Class_004dd2a0 a, Class_004dd2a0 b) { return !(a == b); }

    bool FUN_004db450(unsigned int size);
};

// FUNCTION: 0x4db450
bool Class_004db450::FUN_004db450(unsigned int size)
{
    Class_004dd2a0 n;
    Class_004dd2a0 it2;
    unsigned int base;
    Pair_004db450 p;
    unsigned int len = 0x10000000;
    // Once the reservation loop is done the parameter is dead, and the original
    // reuses its stack slot for the map iterator.
    Class_004dd2a0& it = *(Class_004dd2a0*)&size;

    if (2 * size > len && size < 0x40000000u)
        len = ((size + 0x1fff) & 0xffffe000) * 2;
    if (size > len)
        len = (size + 0x1fff) & 0xffffe000;
    base = (unsigned int)VirtualAlloc(0, len + 0x2000, 0x2000,
                                      PAGE_READWRITE);
    for (;;) {
        if (base != 0 && base + len <= 0x80000000u)
            break;
        if (base != 0)
            VirtualFree((void*)base, len + 0x2000, MEM_RELEASE);
        len = (len >> 1) & 0x7fffe000;
        if (len < 0x10000 || len < size)
            return false;
        base = (unsigned int)VirtualAlloc(0, len + 0x2000, 0x2000,
                                          PAGE_READWRITE);
    }
    total += len;
    p.offset = base;
    p.length = len;
    ((Class_004dc620*)this)->FUN_004dc620(&n, p.offset);
    it = n;
    ((Class_004dbeb0*)this)->FUN_004dbeb0((int*)&it2);
    if (it == it2)
        it.ptr = head;
    else
        it.FUN_004dd2a0();
    if (Neq(n, Class_004dd2a0(head))) {
        if (n.ptr->key == p.offset + p.length) {
            p.length = p.length + n.ptr->length;
            ((Class_004dc130*)this)->FUN_004dc130(&it2, n);
        }
    }
    if (Neq(it, Class_004dd2a0(head))) {
        if (it.ptr->key + it.ptr->length == p.offset) {
            p.length = p.length + it.ptr->length;
            p.offset = it.ptr->key;
            ((Class_004dc130*)this)->FUN_004dc130(&it2, it);
        }
    }
    ((Class_004dbec0*)this)->FUN_004dbec0(&it2, &p);
    return true;
}
