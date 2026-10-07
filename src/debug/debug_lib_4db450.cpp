// Decompiled by Space Bunny Free, finished by space-bunny-free and GPT-6.1-sol, edited by deepseek-v4.1, retried by Sonnet 5.5, retried by space-bunny-free, finished by claude-sonnet-5-5. Names are provisional.
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

// insert()'s pair<iterator, bool>, 8 bytes.
struct InsertResult_004db450 { Class_004dd2a0 first; int second; };
// The helper's one frame object: the pair, then the value_type.

struct Frame_004db450 { InsertResult_004db450 r; Pair_004db450 p; };
class Class_004dbec0 {
public:
    void FUN_004dbec0(InsertResult_004db450* it, Pair_004db450* p);
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

    inline void Tail(unsigned base, unsigned len, Class_004dd2a0& it) {
    total += len;
    Class_004dd2a0 n;
    // One frame object (pair, then value_type): separate locals move the stack slots.
    Frame_004db450 f;
    f.p.offset = base;
    f.p.length = len;
    ((Class_004dc620*)this)->FUN_004dc620(&n, f.p.offset);
    it = n;
    ((Class_004dbeb0*)this)->FUN_004dbeb0((int*)&f.r);
    if (it == f.r.first)
        it.ptr = head;
    else
        it.FUN_004dd2a0();
    if (Neq(n, Class_004dd2a0(head))) {
        if (n.ptr->key == f.p.offset + f.p.length) {
            f.p.length = f.p.length + n.ptr->length;
            ((Class_004dc130*)this)->FUN_004dc130(&f.r.first, n);
        }
    }
    if (Neq(it, Class_004dd2a0(head))) {
        if (it.ptr->key + it.ptr->length == f.p.offset) {
            f.p.length = f.p.length + it.ptr->length;
            f.p.offset = it.ptr->key;
            ((Class_004dc130*)this)->FUN_004dc130(&f.r.first, it);
        }
    }
    ((Class_004dbec0*)this)->FUN_004dbec0(&f.r, &f.p);
    }
    bool GrowReservation(unsigned int size);
};

// FUNCTION: 0x4db450
bool Class_004db450::GrowReservation(unsigned int size)
{
    unsigned int len = 0x10000000;
    unsigned int base;
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
    Tail(base, len, *(Class_004dd2a0*)&size);
    return true;
}