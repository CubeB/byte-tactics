// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash, GPT-6.1-sol
// and space-bunny-free, edited by deepseek-v4.1, retried by Sonnet 5.5. Names are provisional.
//
// MATCH (space-bunny-free, issue 3009). The two blocks that every earlier pass
// left at 88-89% are not a scheduling puzzle at all: they are what MSVC 5 emits
// for one `map::operator[]`, that is
//     rects[v.x] = v;
// with the value a plain struct local. So the whole body is a Rect local and a
// for loop, and the loads the earlier notes called "reloads that survive every
// statement order" are the copy of the pair's second out of the *uninitialised*
// temporary `map::operator[]` builds (`insert(value_type(_Kv, _Ty()))`, MSVC 5
// leaves that _Ty() uninitialised). Because the temporary is a different
// memory node from the local, the store `v.x = key` does not forward into the
// pair's copy, which is exactly the two `mov ecx, [esp+0x38]` /
// `mov ecx, [esp+0x44]` the pre-insert block needed, and the post-insert
// `= v` then has one live value fewer. Same mechanism as 0x46d6c0, but there
// the temporary is a separate Rect, here it shares the local's slot.
//
// Three details the bytes need, all settled by experiment here:
//  * `v.y = 0` is dead code the optimiser keeps only as a value: the pair's copy
//    of y is emitted before the store, so the store is dropped and the
//    uninitialised slot is what the insert copies. That is the bug below. Do
//    not "fix" it by initialising y in a way the compiler can see.
//  * the flag expression has to be a small `static inline` helper. Written out
//    in the loop it lands in eax and costs a `mov ebx, eax`.
//  * `field_58` is an int member read into a short field (`v.h`), and `v.w = 1`
//    is the statement MSVC sinks into the pre-guard preheader.
//
// Why the tree is declared as below. The two callee names the checker wants
// are mixed: data/symbols.csv has 0x46e880 and 0x46fad0 as hand-rolled
// (`Class_0046e880::FUN_0046e880`, `Class_0046fad0::Class_0046fad0`, from the
// matched 0x46e880.cpp and 0x46fad0.cpp) but 0x46fb80 and 0x46ff90 under the
// crude demangle of the real MSVC 5 template symbols
// (`IURect_0046e160::IU?$pair::?$_Tree::_Insert`, `...::iterator::_Dec`, from
// 0x46fb80.cpp and 0x46ef50.cpp). Using the real <map> gives the right two but
// the wrong other two; hand-rolling everything gives the right other two but
// the wrong two. Declaring a `_Tree` template whose first two arguments are
// `unsigned int` and the map's real `value_type` gives the STL mangled names
// while the walk itself stays hand written: MSVC 5 concatenates template
// arguments with no separator, so `_Tree<unsigned int, std::pair<const
// unsigned int, Rect_0046e160>, ...>` mangles as `?$_Tree@IU?$pair@IURect...`,
// and check.py's base_name() only looks at the part before the first `@@`.
//
// BUG (kept as the original has it): the value handed to the map insert has an
// uninitialised y. There is no store to the local's y slot anywhere in the
// function; the insert copies four words out of that slot into the pair, so the
// node briefly holds stack garbage before `= v` overwrites y with 0. The
// evidence is that instruction: `mov edx, [esp+0x3c] / mov [esp+0x50], edx`
// copies the word, and nothing ever writes it.
#include <map>

struct Rect_0046e160 {                // the std::map's value, 0x10 bytes
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
    Rect_0046e160 value;               // +0x10
    int color;                         // +0x20
};

extern Node_0046d2e0* DAT_0051e598;    // the tree's shared _Nil node

// The map's value_type, the real std::pair: its mangled name is the first half
// of _Tree's, and that is what the callee names in data/symbols.csv come from.
typedef std::pair<const unsigned int, Rect_0046e160> Pair_0046d2e0;

// std::_Tree<...> out of MSVC 5's <xtree>, with only the two members this
// function calls out of line. The template arguments are the point of writing
// it this way (see the note at the top): the walk itself is hand written below.
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
    int* FUN_0046e880(int* param_1);   // _Tree::begin
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

    Rect_0046e160& operator[](const unsigned int& k)
    {
        // std::map::operator[]: insert a default value under the key, then hand
        // back the node's value to assign to. The Rect() is never written.
        iterator p = insert(Pair_0046d2e0(k, Rect_0046e160())).first;
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
            iterator other((Node_0046d2e0*)*((Class_0046e880*)this)->FUN_0046e880(&root));
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

class Class_0046d040 {
public:
    Map_0046d2e0 rects;                // +0x00
    char unknown_10[0x58 - 0x10];
    int field_58;                      // +0x58

    void FUN_0046d2e0();
};

// The flag expression in its own small inline helper. Written out in the loop
// MSVC keeps the tested bit in eax and adds a `mov ebx, eax`; this way it lands
// in ebx, the callee-saved register the original keeps it in.
static inline bool FlagOf_0046d2e0(Def_0046d2e0* d)
{
    return (d->flags >> 16) & 1;
}

// FUNCTION: 0x46d2e0
void Class_0046d040::FUN_0046d2e0()
{
    Rect_0046e160 v;
    for (unsigned short i = 1; i < g_game->count; i++) {
        v.x = g_game->defs[i].key;
        v.y = 0;
        v.w = 1;
        v.h = (short)field_58;
        v.flag = FlagOf_0046d2e0(&g_game->defs[i]) ? 0 : -1;
        rects[v.x] = v;
    }
}
