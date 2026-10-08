// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash, GPT-6.1-sol
// and space-bunny-free, edited by deepseek-v4.1, retried by Sonnet 5.5. Names are provisional.
// A method of UnitSync, whose other methods are in unit_sync.cpp.
// Stays in its own file: its hand-written _Tree model calls the out-of-line
// _Insert (0x46fb80), which unit_sync.cpp's _Tree model cannot share.
//
// The value handed to the map insert has an uninitialised y: the node briefly
// holds stack garbage before `= v` overwrites y with 0.
#include <map>

struct UnitSyncEntry {                // the std::map's value, 0x10 bytes
    int x;                             // +0x0
    int y;                             // +0x4
    short w;                           // +0x8
    short h;                           // +0xa
    int flag;                          // +0xc
};

struct Node_0046d2e0 {                 // the map's tree node, 0x24 bytes
    Node_0046d2e0* left;               // +0x0
    Node_0046d2e0* parent;             // +0x4
    Node_0046d2e0* right;              // +0x8
    unsigned int key;                  // +0xc
    UnitSyncEntry value;               // +0x10
    int color;                         // +0x20
};

extern Node_0046d2e0* DAT_0051e598;    // the tree's shared _Nil node

// The map's value_type, the real std::pair: its mangled name is the first half
// of _Tree's, and that is what the callee names in data/symbols.csv come from.
typedef std::pair<const unsigned int, UnitSyncEntry> Pair_0046d2e0;

// std::_Tree<...> out of MSVC 5's <xtree>, with only the two members this
// function calls out of line. The template arguments are the point of writing
// it this way: the walk itself is hand written below.
template <class Kty, class Ty, class Kfn, class Pr, class Alloc>
struct _Tree {
    struct iterator {
        Node_0046d2e0* ptr;
        iterator() {}
        iterator(Node_0046d2e0* p) : ptr(p) {}
        bool operator==(const iterator& other) const { return ptr == other.ptr; }
        void _Dec();                    // operator--
    };
    // The out-of-line insert: it takes the node pointer slot it fills and
    // returns it, and cleans its own four arguments (hence the pushes before
    // the call that stay on the stack for the pair constructor).
    Node_0046d2e0** _Insert(Node_0046d2e0** out, Node_0046d2e0* where,
                            Node_0046d2e0* candidate, const Pair_0046d2e0& val);
};

class Class_0046fad0 {                 // pair<iterator, bool>
public:
    Node_0046d2e0* first;
    bool second;
    Class_0046fad0(Node_0046d2e0** first, const bool* second);
};

class Class_0046e880 {                 // the map seen as the tree's root header
public:
    char unknown_0[4];
    int* field_4;                      // +0x4
    int* Begin(int* param_1);          // _Tree::begin
};

struct Less_0046d2e0 {
    bool operator()(const unsigned int& a, const unsigned int& b) const
    {
        return a < b;
    }
};

class Map_0046d2e0 : public _Tree<unsigned int, Pair_0046d2e0, int, int, int> {
public:
    char allocator;                    // +0x0
    Less_0046d2e0 key_compare;         // +0x1
    Node_0046d2e0* head;               // +0x4
    char multi;                        // +0x8
    char unknown_9[3];
    int size;                          // +0xc

    UnitSyncEntry& operator[](const unsigned int& k)
    {
        // std::map::operator[]: insert a default value under the key, then hand
        // back the node's value to assign to. The Rect() is never written.
        iterator p = insert(Pair_0046d2e0(k, UnitSyncEntry())).first;
        return p.ptr->value;
    }

    Class_0046fad0 insert(const Pair_0046d2e0& val)
    {
        Node_0046d2e0* out1;
        Node_0046d2e0* out2;
        Node_0046d2e0* out3;
        bool inserted1;
        bool inserted2;
        bool inserted3;
        bool inserted4;
        Node_0046d2e0* where = head->parent;
        Node_0046d2e0* candidate = head;
        bool went_left = 1;
        {
            std::_Lockit lock;
            if (where != DAT_0051e598) {
                do {
                    candidate = where;
                    went_left = key_compare(val.first, where->key);
                    where = went_left ? where->left : where->right;
                } while (where != DAT_0051e598);
            }
        }
        if (multi) {
            inserted1 = 1;
            return Class_0046fad0(_Insert(&out1, where, candidate, val), &inserted1);
        }
        iterator it(candidate);
        if (went_left) {
            int root;
            iterator other((Node_0046d2e0*)*((Class_0046e880*)this)->Begin(&root));
            bool same = (it == other);

            if (same) {
                inserted2 = 1;
                return Class_0046fad0(_Insert(&out2, where, candidate, val), &inserted2);
            }
            it._Dec();
        }
        if (key_compare(it.ptr->key, val.first)) {
            inserted3 = 1;
            return Class_0046fad0(_Insert(&out3, where, candidate, val), &inserted3);
        }
        inserted4 = 0;
        return Class_0046fad0(&it.ptr, &inserted4);
    }
};

#pragma pack(push, 1)
struct Def_0046d2e0 {                  // 0x249 bytes
    char unknown_0[0x13e];
    unsigned int key;                  // +0x13e
    char unknown_142[0x245 - 0x142];
    unsigned int flags;                // +0x245
};

struct Game {
    char unknown_0[0x1438f];
    int count;                         // +0x1438f
    char unknown_14393[0x1439b - 0x14393];
    Def_0046d2e0* defs;                // +0x1439b
};
#pragma pack(pop)

extern Game* g_game;

class UnitSync {
public:
    Map_0046d2e0 rects;                // +0x00
    char unknown_10[0x58 - 0x10];
    int field_58;                      // +0x58

    void ResetEntries();
};

// The flag expression in its own small inline helper. Written out in the loop
// MSVC keeps the tested bit in eax and adds a `mov ebx, eax`; this way it lands
// in ebx, the callee-saved register the original keeps it in.
static inline bool FlagOf_0046d2e0(Def_0046d2e0* d)
{
    return (d->flags >> 16) & 1;
}

// FUNCTION: 0x46d2e0
void UnitSync::ResetEntries()
{
    UnitSyncEntry v;
    for (unsigned short i = 1; i < g_game->count; i++) {
        v.x = g_game->defs[i].key;
        // Dead store that must stay: the uninitialised slot is what the insert copies.
        v.y = 0;
        v.w = 1;
        // field_58 is an int read into the short field.
        v.h = (short)field_58;
        v.flag = FlagOf_0046d2e0(&g_game->defs[i]) ? 0 : -1;
        rects[v.x] = v;
    }
}
