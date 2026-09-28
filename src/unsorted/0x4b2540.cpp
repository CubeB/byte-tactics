// Decompiled by LongCat 2.5 Preview Free. Names are provisional.
// GAVE UP: std::_Tree::erase - could not match due to compiler differences.
// Shaped like std::_Tree<...>::erase(const _K&) from MSVC 5's <xtree> for a
// tree keyed by int (std::less<int>), under a lock object; DAT_0051fbbc is the
// tree's _Nil node and head->parent is the root.
#include <yvals.h>
#include <xtree>

struct Node_004b2540 {
    Node_004b2540* left;            // +0x0
    Node_004b2540* parent;          // +0x4
    Node_004b2540* right;           // +0x8
    int key;                        // +0xc
};

struct IntLess_004b2540 {
    bool operator()(const int& a, const int& b) const
    {
        return a < b;
    }
};

extern Node_004b2540* DAT_0051fbbc;
extern int DAT_0051fbcc;
extern Node_004b2540* DAT_0051fbc4;

class Class_004b2540 {
public:
    char allocator;                 // +0x0
    IntLess_004b2540 key_compare;   // +0x1
    Node_004b2540* head;            // +0x4
    int refcount;                   // +0x8
    Node_004b2540* FUN_004b3490(const int& key);
    Node_004b2540* FUN_004b3430(const int& key);
    void FUN_004b2fb0(Node_004b2540* x);
    Node_004b2540* FUN_004b2ac0(Node_004b2540* pos);
    void FUN_004b3590(Node_004b2540** pos);
    void FUN_004b2540(int key);
};

void FUN_004d85a0(int* param_1);

// FUNCTION: 0x4b2540
void __stdcall Class_004b2540::FUN_004b2540(int key)
{
    Node_004b2540* edi = FUN_004b3490(key);
    Node_004b2540* ebp = FUN_004b3430(key);
    Node_004b2540* esi = ebp;
    int ebx = (ebp != edi) ? 1 : 0;
    while (ebx) {
        std::_Lockit lock;
        if (esi->right != DAT_0051fbbc) {
            esi = FUN_004b3370(esi->right);
        } else {
            Node_004b2540* eax = esi->parent;
            while (esi == eax->right) {
                esi = eax;
                eax = esi->parent;
            }
            if (esi->right != eax)
                esi = eax;
        }
        ebx = (esi != edi) ? 1 : 0;
    }
    esi = ebp;
    Node_004b2540* esp_14 = esi;
    if (DAT_0051fbcc) {
        Node_004b2540* eax = DAT_0051fbc4;
        if (ebp == eax->parent && edi == eax) {
            std::_Lockit lock1;
            edi = eax->parent;
            std::_Lockit lock2;
            esi = edi;
            while (edi != DAT_0051fbbc) {
                FUN_004b2fb0(esi->right);
                esi = esi->left;
                operator delete(edi);
                edi = esi;
            }
            // ~_Lockit lock2
            DAT_0051fbc4->parent = DAT_0051fbbc;
            DAT_0051fbcc = 0;
            DAT_0051fbc4->left = DAT_0051fbc4;
            DAT_0051fbc4->right = DAT_0051fbc4;
            // ~_Lockit lock1
        }
    }
    if (ebx) {
        do {
            FUN_004b3590(&esp_14);
            FUN_004b2ac0(esi);
            esi = esp_14;
            ebx = (esi != edi) ? 1 : 0;
        } while (ebx);
    }
    FUN_004d85a0(&key);
}
