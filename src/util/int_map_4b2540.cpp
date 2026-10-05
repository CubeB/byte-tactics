// Decompiled by GPT-5.6-Terra, finished by deepseek-v4.1-flash. Names are provisional.
//
// This is the game's `g_map.erase(key)` where g_map is the file-local
// std::map<int,int> at 0x51fbc0, followed by the key object's release.
// MSVC 5 /Ob2 inlines map::erase(const _K&) and _Tree::erase(const _K&):
// equal_range calls _Ubound (0x4b3490) and _Lbound (0x4b3430) out of line,
// _Distance is the count loop, and erase(_F,_L) is inlined. Inside that,
// erase(_F++) stays out of line (0x4b2ac0) and the fast-path _Erase is
// inlined once with its self-recursive call kept out of line (0x4b2fb0).
// The count loop's prefix ++_F inlines iterator::_Inc, which keeps its own
// _Lockit and calls _Min (0x4b3370) out of line; the erase loop's _F++
// reaches _Inc out of line (0x4b3590).
//
// The out-of-line members are hand-rolled under the names data/symbols.csv
// already has for their addresses (Class_004b3490::FUN_004b3490 and so on);
// the real <map> instantiation would emit the template's mangled names
// instead, which the checker rejects. The Map wrapper is load bearing: the
// extra inlined erase layer is what makes MSVC reserve the erase iterator's
// slot before the count lock and produce the original's 0x18-byte frame.
//
// The tree is file-static, which is what lets MSVC keep the lower bound in a
// register across the bounds calls; an external object makes it spill, 435
// bytes against 423. A file-static's symbol is named `name$S<n>` with n a
// per-TU counter, and symbols.csv records this one as DAT_0051fbc0$S425, so
// the 72 unused statics below restore the original TU's counter. They emit
// no code and change nothing else.
#include <yvals.h>

struct Node_004b2540 {
    Node_004b2540* left;            // +0x00
    Node_004b2540* parent;          // +0x04
    Node_004b2540* right;           // +0x08
    int key;                        // +0x0c
    int value;                      // +0x10
    int color;                      // +0x14
};

extern Node_004b2540* DAT_0051fbbc;                              // _Nil
extern Node_004b2540* __stdcall FUN_004b3370(Node_004b2540* x);  // _Min

class Class_004b3490 {
public:
    Node_004b2540* FUN_004b3490(const int& key);                 // _Ubound
};

class Class_004b3430 {
public:
    Node_004b2540* FUN_004b3430(const int& key);                 // _Lbound
};

class Class_004b3590 {
public:
    Node_004b2540* ptr;
    void FUN_004b3590();                                         // iterator::_Inc
};

class Class_004b2fb0 {
public:
    struct iterator {
        Node_004b2540* ptr;
        iterator() {}
        iterator(Node_004b2540* p) : ptr(p) {}
        bool operator==(const iterator& x) const { return ptr == x.ptr; }
        bool operator!=(const iterator& x) const { return !(*this == x); }
        iterator& operator++()
        {
            std::_Lockit lock;
            if (ptr->right != DAT_0051fbbc)
                ptr = FUN_004b3370(ptr->right);
            else {
                Node_004b2540* p;
                while (ptr == (p = ptr->parent)->right)
                    ptr = p;
                if (ptr->right != p)
                    ptr = p;
            }
            return *this;
        }
        iterator operator++(int)
        {
            iterator tmp = *this;
            ((Class_004b3590*)&ptr)->FUN_004b3590();
            return tmp;
        }
    };

    struct _Pairii {
        iterator first;
        iterator second;
        _Pairii(iterator _F, iterator _L) : first(_F), second(_L) {}
    };

    char alloc;                     // +0x0
    char comp;                      // +0x1
    Node_004b2540* head;            // +0x4
    char multi;                     // +0x8
    int size;                       // +0xc

    iterator begin() { return head->left; }
    iterator end() { return head; }

    iterator lower_bound(const int& key) { return iterator(((Class_004b3430*)this)->FUN_004b3430(key)); }
    iterator upper_bound(const int& key) { return iterator(((Class_004b3490*)this)->FUN_004b3490(key)); }
    _Pairii equal_range(const int& key) { return _Pairii(lower_bound(key), upper_bound(key)); }

    iterator FUN_004b2ac0(iterator _P);          // erase(iterator)
    void FUN_004b2fb0(Node_004b2540* x);         // _Erase

    iterator erase(iterator _F, iterator _L)
    {
        if (size == 0 || _F != begin() || _L != end()) {
            while (_F != _L)
                FUN_004b2ac0(_F++);
            return _F;
        } else {
            std::_Lockit Lk;
            Node_004b2540* x = head->parent;
            {
                std::_Lockit Lk2;
                for (Node_004b2540* y = x; y != DAT_0051fbbc; x = y) {
                    FUN_004b2fb0(y->right);
                    y = y->left;
                    operator delete(x);
                }
            }
            head->parent = DAT_0051fbbc;
            size = 0;
            head->left = head;
            head->right = head;
            return begin();
        }
    }

    int erase(const int& _X)
    {
        _Pairii _P = equal_range(_X);
        int _N = 0;
        for (iterator _Q = _P.first; _Q != _P.second; ++_Q)
            ++_N;
        erase(_P.first, _P.second);
        return _N;
    }
};

class Map_004b2540 {
public:
    Class_004b2fb0 _Tr;             // +0x0
    int erase(const int& key) { return _Tr.erase(key); }
};

static int dummy_004b2540_00, dummy_004b2540_01, dummy_004b2540_02, dummy_004b2540_03;
static int dummy_004b2540_04, dummy_004b2540_05, dummy_004b2540_06, dummy_004b2540_07;
static int dummy_004b2540_08, dummy_004b2540_09, dummy_004b2540_10, dummy_004b2540_11;
static int dummy_004b2540_12, dummy_004b2540_13, dummy_004b2540_14, dummy_004b2540_15;
static int dummy_004b2540_16, dummy_004b2540_17, dummy_004b2540_18, dummy_004b2540_19;
static int dummy_004b2540_20, dummy_004b2540_21, dummy_004b2540_22, dummy_004b2540_23;
static int dummy_004b2540_24, dummy_004b2540_25, dummy_004b2540_26, dummy_004b2540_27;
static int dummy_004b2540_28, dummy_004b2540_29, dummy_004b2540_30, dummy_004b2540_31;
static int dummy_004b2540_32, dummy_004b2540_33, dummy_004b2540_34, dummy_004b2540_35;
static int dummy_004b2540_36, dummy_004b2540_37, dummy_004b2540_38, dummy_004b2540_39;
static int dummy_004b2540_40, dummy_004b2540_41, dummy_004b2540_42, dummy_004b2540_43;
static int dummy_004b2540_44, dummy_004b2540_45, dummy_004b2540_46, dummy_004b2540_47;
static int dummy_004b2540_48, dummy_004b2540_49, dummy_004b2540_50, dummy_004b2540_51;
static int dummy_004b2540_52, dummy_004b2540_53, dummy_004b2540_54, dummy_004b2540_55;
static int dummy_004b2540_56, dummy_004b2540_57, dummy_004b2540_58, dummy_004b2540_59;
static int dummy_004b2540_60, dummy_004b2540_61, dummy_004b2540_62, dummy_004b2540_63;
static int dummy_004b2540_64, dummy_004b2540_65, dummy_004b2540_66, dummy_004b2540_67;
static int dummy_004b2540_68, dummy_004b2540_69, dummy_004b2540_70, dummy_004b2540_71;
static Map_004b2540 DAT_0051fbc0;

extern void __cdecl FUN_004d85a0(void* x);

// FUNCTION: 0x4b2540
void __stdcall FUN_004b2540(int key)
{
    if (key != 0) {
        int local_key = key;
        DAT_0051fbc0.erase(local_key);
        FUN_004d85a0((void*)key);
    }
}
