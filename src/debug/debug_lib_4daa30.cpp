// Decompiled by deepseek-v4.1. Names are provisional.
// The map is the tree whose _Ubound is 0x4dd7d0 and whose _Dec is 0x4dd820
// (the same tree GetBlockMap lazily creates, _Nil = DAT_00528a50). Its
// value_type is the 0x30-byte Class_004d8820 and the key is that record's
// first dword, so the temporary built here is itself the lookup key.
// The lookup is an upper_bound plus the iterator predecessor: arg2 gets the
// record of the first key above `key` (8 zero bytes when it is end()), arg1
// gets the record of the last key at or below it (8 zero bytes when it is
// begin(), which costs the extra _Dec).
//
// MATCH: 214 of 214 bytes. The previous 82.9 percent attempt was one chained
// call away: the first lookup must go through a named tree local,
//
//     Class_004dd7d0* tree = GetBlockMap();
//     it.ptr = tree->FUN_004dd7d0(rec.key);
//
// because the chained form GetBlockMap()->FUN_004dd7d0(rec.key) evaluates
// the `lea ecx,[esp+0x10]` argument before the object call and homes the
// critical section in ebp; the split form calls GetBlockMap first and keeps
// its result in eax across the lea/push, which flips cs to ebx and every
// register choice after it (esi for the end() compare and the following
// `add esi,0xc` copy) falls into place. The iterator type must also be
// Class_004dd820 (the map's _Dec) rather than an ad hoc name, or the
// +0xb2 reference check fails.
// Negative results from the 82.9 percent attempt, still valid: explicit
// iterator temps, a `bool isEnd = ...` local and swapping the rec/it
// declaration order all scored worse or changed nothing.
#include <windows.h>

struct Node_004daa30;

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

class Class_004dd820 {
public:
    Node_004daa30* ptr;            // +0x0

    Class_004dd820() {}
    Class_004dd820(Node_004daa30* q) : ptr(q) {}
    bool operator==(const Class_004dd820& o) const { return ptr == o.ptr; }
    void FUN_004dd820();
};

struct Node_004daa30 {
    Node_004daa30* left;           // +0x0
    Node_004daa30* parent;         // +0x4
    Node_004daa30* right;          // +0x8
    Class_004d8820 value;          // +0xc
    unsigned int color;            // +0x3c
};

class Class_004dd7d0 {
public:
    char unknown_0[4];
    Node_004daa30* head;           // +0x4

    Class_004dd820 End() { return Class_004dd820(head); }
    Class_004dd820 Begin() { return Class_004dd820(head->left); }
    Node_004daa30* FUN_004dd7d0(const unsigned int& kv);
};

Class_004dd7d0* GetBlockMap();
LPCRITICAL_SECTION FUN_004da780();

// FUNCTION: 0x4daa30
void __cdecl FindBlocksAroundAddress(unsigned int key, Class_004d8820* prev, Class_004d8820* next)
{
    LPCRITICAL_SECTION cs = FUN_004da780();
    EnterCriticalSection(cs);
    Class_004d8820 rec(key, 0, 0, 0, 0);
    Class_004dd7d0* tree = GetBlockMap();
    Class_004dd820 it;
    it.ptr = tree->FUN_004dd7d0(rec.key);
    if (it == GetBlockMap()->End()) {
        next->key = 0;
        next->field_4 = 0;
    } else {
        *next = it.ptr->value;
    }
    if (it == GetBlockMap()->Begin()) {
        prev->key = 0;
        prev->field_4 = 0;
    } else {
        it.FUN_004dd820();
        *prev = it.ptr->value;
    }
    LeaveCriticalSection(cs);
}
