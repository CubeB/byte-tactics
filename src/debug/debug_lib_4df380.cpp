// Decompiled by space-bunny-free. Names are provisional.
// Pushes the global name table (GetNameTable) entry whose key equals this
// object's name into the edit control at id 0x3f0, but only when the control
// does not already hold that text. The whole scan runs under the global
// critical section (FUN_004e1ac0) and stops at the first key that matches, so
// a duplicate key leaves the later entry unused.
#include <string.h>
#include <windows.h>
#include <yvals.h>

class CritSec_004e1ac0 {
public:
    CRITICAL_SECTION cs;
};

CritSec_004e1ac0* FUN_004e1ac0();

// The map's value: 500 bytes, the size of the edit control's buffer.
struct Value_004df380 {
    char text[500];                     // +0x00
};

struct Node_004df380 {
    Node_004df380* left;               // +0x0
    Node_004df380* parent;             // +0x4
    Node_004df380* right;              // +0x8
    const char* first;                 // +0xc
    Value_004df380 second;             // +0x10
    int color;                         // +0x204
};

extern Node_004df380* DAT_005292c4;

Node_004df380* __cdecl FUN_004e04e0(Node_004df380* p);

class Iter_004df380 {
public:
    Node_004df380* ptr;

    Iter_004df380() {}
    Iter_004df380(Node_004df380* p) : ptr(p) {}
    Iter_004df380& operator++() { _Inc(); return *this; }
    bool operator==(const Iter_004df380& x) const { return ptr == x.ptr; }
    bool operator!=(const Iter_004df380& x) const { return !(*this == x); }
    // The tree's iterator increment, out of <xtree>. The while loop is
    // unrolled twice and the trailing "if" is the tree's own test, which
    // keeps the parent from being stored when the node is a left child.
    void _Inc()
    {
        std::_Lockit lk;
        if (ptr->right != DAT_005292c4)
            ptr = FUN_004e04e0(ptr->right);
        else {
            Node_004df380* p;
            while (ptr == (p = ptr->parent)->right)
                ptr = p;
            if (ptr->right != p)
                ptr = p;
        }
    }
};

class Map_004df380 {
public:
    char compare;                      // +0x0
    char allocator;                    // +0x1
    Node_004df380* head;               // +0x4
    char multi;                        // +0x8
    int size;                          // +0xc
    char changed;                      // +0x10

    Iter_004df380 begin() { return Iter_004df380(head->left); }
    Iter_004df380 end() { return Iter_004df380(head); }
};

// The global name table singleton; the tree is its first member (0x4e1a90).
class NameTable {
public:
    Map_004df380 names;                // +0x00
};

NameTable* GetNameTable();

class Class_004df380 {
public:
    HWND hwnd;                          // +0x00
    char unknown_4[0x20];
    const char* name;                   // +0x24

    // The key test, with the pointer compare the original does first: the
    // key is loaded before this->name, which is the order the code needs.
    bool Same(const char* key)
    {
        return key == name || strcmp(key, name) == 0;
    }
    void FUN_004df380();
};

// FUNCTION: 0x4df380
void Class_004df380::FUN_004df380()
{
    CritSec_004e1ac0* lock = FUN_004e1ac0();
    EnterCriticalSection(&lock->cs);
    Map_004df380* map = &GetNameTable()->names;
    for (Iter_004df380 it = map->begin(); it != map->end(); ++it) {
        if (Same(it.ptr->first)) {
            char buf[500];
            if (GetDlgItemTextA(hwnd, 0x3f0, buf, 500) == 0
                || strcmp(buf, it.ptr->second.text) != 0)
                SetDlgItemTextA(hwnd, 0x3f0, it.ptr->second.text);
            break;
        }
    }
    LeaveCriticalSection(&lock->cs);
}
