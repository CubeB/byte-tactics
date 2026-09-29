// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash. Names are provisional.
// PARTIAL, 88.4%, and the whole function now compiles to the original's exact
// 475 bytes, so only the in-block scheduling inside two blocks is left.
// What is settled (do not undo):
//  * ONE local struct `Locals_0046d2e0` holding both the value and the pair,
//    with `Insert(s.val)` letting &s.val escape. Two separate locals collapse
//    the register allocation (the frame grows to 0x50, `this` spills).
//  * The loop is an `if (i < count) { ...; do { ...; i++; } while (i < count); }`.
//    That, not `s.v.wh.w = 1` before a `for`, is what puts the
//    `mov word [esp+0x40], cx` in the post-guard preheader like the original,
//    and it is also the only form that keeps the bottom-tested latch.
//  * The post-insert copy is a CONSTRUCTED value,
//    `p.first->value = Value_0046d2e0(s.v.x, 0, s.v.wh, s.v.flag)`.
//    A plain `p.first->value = s.v;` reloads s.v.y and is 2 bytes over; four
//    separate field assignments re-read `p.first` four times (the stores go
//    through a pointer, so MSVC 5 cannot CSE the four `p.first` loads) and give
//    479 bytes. Only the constructor materialises the constant 0 into a
//    register (`xor eax,eax / mov [edx+4],eax`) the way the original does; a
//    literal 0 in a field assignment always becomes `mov dword [m], 0`.
//  * `s.v.y = 0;` must be the last statement before `Insert`, and the y copy
//    `s.val.value.y = s.v.y;` must come before it.
// What still differs, all MSVC 5 scheduling inside two blocks:
//  1. pre-insert block: the original hoists the `s.v.wh.h` store and the
//     `s.val.value.y` load+store pair to the top of the block (before the key
//     and flags loads) and reloads s.v.x and s.v.flag through the stack instead
//     of forwarding the register. No permutation of the eight statements
//     changes this: all 5040 legal orders compile identically, so it is not
//     statement order.
//  2. post-insert block: same instruction sequence, wrong registers. Ours puts
//     the destination base in edi and hoists all three loads (edi/eax/edx/esi),
//     the original uses edx for the base and reuses eax for the 0 and the flag,
//     i.e. it has one live value fewer, which is a consequence of 1.
// Claude Sonnet 5.5 pass (#601): compiler state is ruled out for the two blocks
// below. N unused `extern int dummyK;` lines after the include, K = 8 to 400 step
// 8 (50 builds, check.py --sym, not committed): 88.4 percent and 475 bytes for
// every K; headers.py, all 128 sets: best is 88.4, the empty set. So it is the
// source shape, as the notes above conclude.
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
    // The constructor is what makes the post-insert copy a constructed
    // temporary rather than a struct assignment; see the note above.
    Value_0046d2e0() {}
    Value_0046d2e0(int a, int b, Wh_0046d2e0 c, int d) : x(a), y(b), wh(c), flag(d) {}
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
    unsigned short i = 1;
    if (i < g_game->count) {
        s.v.wh.w = 1;
        do {
            unsigned int key = g_game->defs[i].key;
            bool flag = (g_game->defs[i].flags >> 16) & 1;
            s.v.wh.h = field_58;
            s.v.x = key;
            s.v.flag = flag ? 0 : -1;
            s.val.value.flag = s.v.flag;
            s.val.value.wh = s.v.wh;
            s.val.value.x = s.v.x;
            s.val.key = key;
            s.val.value.y = s.v.y;
            s.v.y = 0;
            Class_0046fad0 p = Insert(s.val);
            p.first->value = Value_0046d2e0(s.v.x, 0, s.v.wh, s.v.flag);
            i++;
        } while (i < g_game->count);
    }
}
