// Decompiled by deepseek-v4.1. Names are provisional.
// The map is the tree whose _Ubound is 0x4dd7d0 and whose _Dec is 0x4dd820
// (the same tree FUN_004da8d0 lazily creates, _Nil = DAT_00528a50). Its
// value_type is the 0x30-byte Class_004d8820 and the key is that record's
// first dword, so the temporary built here is itself the lookup key.
// The lookup is an upper_bound plus the iterator predecessor: arg2 gets the
// record of the first key above `key` (8 zero bytes when it is end()), arg1
// gets the record of the last key at or below it (8 zero bytes when it is
// begin(), which costs the extra _Dec).
//
// NOT MATCHING: 82.9 percent, 214 of 214 bytes, so the frame, the locals and
// the branch structure are right. Four hunks remain, all register choices:
//  - `cs` (the FUN_004da780 result) lives in ebp here, ebx in the original
//    (push ebx / mov ebx,eax / push ebx vs ebp). Rewriting cs as
//    `CRITICAL_SECTION& cs = *FUN_004da780();` compiles identically, so the
//    choice is not the declared type.
//  - the original calls FUN_004da8d0 BEFORE it evaluates the _Ubound
//    argument (`call; lea ecx,[esp+0x10]; push ecx; mov ecx,eax`); this file
//    emits `lea ecx,[esp+0x10]; push ecx; call; mov ecx,eax`.
//  - the end() compare uses it in esi and the head value in ecx in the
//    original (`mov esi,[esp+0xc]; mov ecx,[eax+4]; cmp esi,ecx`) and the
//    other way round here (ecx/esi, `cmp ecx,esi`), so the following copy
//    starts `add esi,0xc` there and `lea esi,[ecx+0xc]` here.
// The begin() compare and everything after it match byte for byte, so the
// inline bool-returning operator== shape (End() = Iter(head),
// Begin() = Iter(head->left)) is confirmed; only the end() case allocates
// differently. Tried and worse: explicit `Iter_004dd820(...->head)` temps
// (76.6 percent, 216 bytes), a `bool isEnd = ...` local (67.1 percent),
// swapping the rec/it declaration order (no change).
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

class Iter_004dd820 {
public:
    Node_004daa30* ptr;            // +0x0

    Iter_004dd820() {}
    Iter_004dd820(Node_004daa30* q) : ptr(q) {}
    bool operator==(const Iter_004dd820& o) const { return ptr == o.ptr; }
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

    Iter_004dd820 End() { return Iter_004dd820(head); }
    Iter_004dd820 Begin() { return Iter_004dd820(head->left); }
    Node_004daa30* FUN_004dd7d0(const unsigned int& kv);
};

Class_004dd7d0* FUN_004da8d0();
LPCRITICAL_SECTION FUN_004da780();

// FUNCTION: 0x4daa30
void FUN_004daa30(unsigned int key, Class_004d8820* prev, Class_004d8820* next)
{
    LPCRITICAL_SECTION cs = FUN_004da780();
    EnterCriticalSection(cs);
    Class_004d8820 rec(key, 0, 0, 0, 0);
    Iter_004dd820 it;
    it.ptr = FUN_004da8d0()->FUN_004dd7d0(rec.key);
    if (it == FUN_004da8d0()->End()) {
        next->key = 0;
        next->field_4 = 0;
    } else {
        *next = it.ptr->value;
    }
    if (it == FUN_004da8d0()->Begin()) {
        prev->key = 0;
        prev->field_4 = 0;
    } else {
        it.FUN_004dd820();
        *prev = it.ptr->value;
    }
    LeaveCriticalSection(cs);
}
