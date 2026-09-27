// Decompiled by space-bunny-free. Names are provisional.
// PARTIAL, 76.5%. Structural shape is right (see the note on Locals_0046d2e0
// below, the one containing struct is what keeps the register allocation from
// collapsing). What still differs, all MSVC 5 scheduling:
//  1. where the hoisted `s.v.wh.w = 1` store lands: the original puts it in the
//     preheader block AFTER the loop guard (same as the matched constructor
//     0x46d040, whose `v.y = 0` / `v.w = 1` sit at 0x46d0e0/0x46d0e4, before its
//     loop head at 0x46d0e9), ours puts it before the `cmp`.
//  2. `add eax, ecx` vs our `add ecx, eax` for the def address, and the key
//     and flags loads swapped.
//  3. the `it == root` compare: the original reloads the root through eax
//     (`cmp edx, [ecx]`), we copy it into a register first.
//  4. the post-insert copy stores `v.y` from a reloaded stack slot; the
//     original forwards the constant 0.
#include <yvals.h>

struct Wh_0046d2e0 {                   // 4 bytes, the w/h pair
    short w;                            // +0x0
    short h;                            // +0x2
};

struct Value_0046d2e0 {                 // 0x10 bytes, the map's value
    int x;                              // +0x0
    int y;                              // +0x4
    Wh_0046d2e0 wh;                     // +0x8
    int flag;                           // +0xc
};

struct Pair_0046d2e0 {                  // 0x14 bytes, the value_type
    unsigned int key;                   // +0x0
    Value_0046d2e0 value;               // +0x4
};

struct Locals_0046d2e0 {
    Value_0046d2e0 v;                   // +0x00
    Pair_0046d2e0 val;                  // +0x10
};

struct Node_0046d2e0 {
    Node_0046d2e0* left;                // +0x0
    Node_0046d2e0* parent;              // +0x4
    Node_0046d2e0* right;               // +0x8
    unsigned int key;                   // +0xc
    Value_0046d2e0 value;               // +0x10
    int color;                          // +0x20
};

extern Node_0046d2e0* DAT_0051e598;     // the tree's shared _Nil node

class Class_0046ff90 {                  // the map's iterator
public:
    Node_0046d2e0* ptr;
    Class_0046ff90() {}
    Class_0046ff90(Node_0046d2e0* p) : ptr(p) {}
    bool operator==(const Class_0046ff90& other) const { return ptr == other.ptr; }
    void FUN_0046ff90();                // operator--
};

class Class_0046fad0 {                  // pair<iterator, bool>
public:
    Node_0046d2e0* first;
    bool second;
    Class_0046fad0(Node_0046d2e0** first, const bool* second);
};

class Class_0046e880 {                  // the same object, for the root pointer
public:
    char unknown_0[4];
    int* field_4;                       // +0x4
    int* FUN_0046e880(int* param_1);
};

struct Less_0046d2e0 {
    bool operator()(const unsigned int& a, const unsigned int& b) const
    {
        return a < b;
    }
};

class Map_0046d2e0 {
public:
    char allocator;                     // +0x0
    Less_0046d2e0 key_compare;          // +0x1
    Node_0046d2e0* head;                // +0x4
    char multi;                         // +0x8
    char unknown_9[3];
    int size;                           // +0xc

    Node_0046d2e0** FUN_0046fb80(Node_0046d2e0** out, Node_0046d2e0* where,
                                 Node_0046d2e0* candidate, const Pair_0046d2e0* val);
};

#pragma pack(push, 1)
struct Def_0046d2e0 {                   // 0x249 bytes
    char unknown_0[0x13e];
    unsigned int key;                   // +0x13e
    char unknown_142[0x245 - 0x142];
    unsigned int flags;                 // +0x245
};

struct Game_0046d2e0 {
    char unknown_0[0x1438f];
    int count;                          // +0x1438f
    char unknown_14393[0x1439b - 0x14393];
    Def_0046d2e0* defs;                 // +0x1439b
};
#pragma pack(pop)

extern Game_0046d2e0* g_game;

class Class_0046d040 {
public:
    Map_0046d2e0 rects;                 // +0x00
    char unknown_10[0x58 - 0x10];
    short field_58;                      // +0x58

    Class_0046fad0 Insert(const Pair_0046d2e0& val)
    {
        Node_0046d2e0* out1;
        int root;
        Node_0046d2e0* out2;
        Node_0046d2e0* out3;
        bool inserted1;
        bool inserted2;
        bool inserted3;
        bool inserted4;
        Node_0046d2e0* where = rects.head->parent;
        Node_0046d2e0* candidate = rects.head;
        bool went_left = 1;
        {
            std::_Lockit lock;
            if (where != DAT_0051e598) {
                do {
                    candidate = where;
                    went_left = rects.key_compare(val.key, where->key);
                    where = went_left ? where->left : where->right;
                } while (where != DAT_0051e598);
            }
        }
        if (rects.multi) {
            inserted1 = 1;
            return Class_0046fad0(rects.FUN_0046fb80(&out1, where, candidate, &val), &inserted1);
        }
        Class_0046ff90 it(candidate);
        if (went_left) {
            Class_0046ff90 other((Node_0046d2e0*)*((Class_0046e880*)this)->FUN_0046e880(&root));
            bool same = (it == other);

            if (same) {
                inserted2 = 1;
                return Class_0046fad0(rects.FUN_0046fb80(&out2, where, candidate, &val), &inserted2);
            }
            it.FUN_0046ff90();
        }
        if (rects.key_compare(it.ptr->key, val.key)) {
            inserted3 = 1;
            return Class_0046fad0(rects.FUN_0046fb80(&out3, where, candidate, &val), &inserted3);
        }
        inserted4 = 0;
        return Class_0046fad0(&it.ptr, &inserted4);
    }
    void FUN_0046d2e0();
};

// FUNCTION: 0x46d2e0
void Class_0046d040::FUN_0046d2e0()
{
    Locals_0046d2e0 s;
    s.v.wh.w = 1;
    for (unsigned short i = 1; i < g_game->count; i++) {
        unsigned int key = g_game->defs[i].key;
        bool flag = (g_game->defs[i].flags >> 16) & 1;
        s.v.wh.h = (short)field_58;
        s.v.x = key;
        s.v.flag = flag ? 0 : -1;
        s.val.value.flag = s.v.flag;
        s.val.value.wh = s.v.wh;
        s.val.value.x = s.v.x;
        s.val.key = key;
        s.val.value.y = s.v.y;
        s.v.y = 0;
        Class_0046fad0 p = Insert(s.val);
        p.first->value = s.v;
    }
}

