// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash. Names are provisional.
//
// Still 94.2%: the inlined ~list<int> erase(first,last) loop is byte-identical
// except that MSVC here reloads the iterator before the bottom test,
// `mov ecx,[esp+0x10]; cmp ecx,ebx`, where the original compares the memory
// operand directly, `cmp [esp+0x10],ebx`. The register-allocator choice is
// compiler state: it survives every class/type/flag/header variation tried
// (plain list vs a derived ListWrap, containment wrappers, real std::map vs
// the hand-written tree, std::vector vs custom vector members, defining the
// member destructors, all tools/headers.py sets), and 0x46ca60 has the same
// one-instruction difference. Everything else is byte-identical.
//
// Releases the overlay object at g_game+0x2a30 (Class_0046d040, built by
// 0x46c8e0 and its constructor 0x46d040): `if (obj) delete obj;` with the
// whole ~Class_0046d040 inlined here. The tree at +0x00 is written out by
// hand to 0x46e890.cpp's shape so the call keeps the name it has in
// data/symbols.csv; the shared _Nil node is released the way 0x46f720 (its
// _Init) takes it, with a std::_Lockit.
#include <list>
#include <vector>
#include <yvals.h>

struct Node_0046c920 {                 // the tree's node, 0xc bytes
    Node_0046c920* left;               // +0x0
    Node_0046c920* parent;             // +0x4
    Node_0046c920* right;              // +0x8
};

extern Node_0046c920* DAT_0051e598;    // the tree's shared _Nil node
extern int DAT_0051e59c;                // and its reference count

class Iter_0046c920 {
public:
    Node_0046c920* node;

    Iter_0046c920() {}
    Iter_0046c920(Node_0046c920* p) : node(p) {}
    bool operator==(const Iter_0046c920& other) const { return node == other.node; }
    bool operator!=(const Iter_0046c920& other) const { return !(*this == other); }
};

class Class_0046e890 {                 // the std::map<unsigned int, Rect> tree
public:
    int unknown_0;                     // +0x0
    Node_0046c920* head;               // +0x4
    int field_8;                       // +0x8
    int size;                          // +0xc

    Iter_0046c920 begin() { return Iter_0046c920(head->left); }
    Iter_0046c920 end() { return Iter_0046c920(head); }
    Iter_0046c920 erase(Iter_0046c920 first, Iter_0046c920 last);

    ~Class_0046e890()
    {
        erase(begin(), end());
        delete head;
        head = 0;
        size = 0;
        std::_Lockit lock;
        if (--DAT_0051e59c == 0) {
            delete DAT_0051e598;
            DAT_0051e598 = 0;
        }
    }
};

class Class_0046ded0 {                 // the vector's element, 0x5c bytes
public:
    char unknown_0[0x5c];

    ~Class_0046ded0();
};

class Class_0046e610 {                 // holds a std::vector<int>
public:
    std::vector<int> vec;              // +0x0

    ~Class_0046e610();
};

class Class_0046d040 {
public:
    Class_0046e890 map;                            // +0x00
    std::vector<Class_0046ded0> elems;             // +0x10
    std::list<int> ids;                            // +0x20
    int field_2c;                                  // +0x2c
    int field_30;                                  // +0x30
    int field_34;                                  // +0x34
    Class_0046e610 field_38;                       // +0x38
    std::vector<int> field_48;                     // +0x48
    short field_58;                                // +0x58
    int field_5c;                                  // +0x5c
    int field_60;                                  // +0x60
    int field_64;                                  // +0x64

    ~Class_0046d040() {}
};

struct Game_0046c920 {
    char unknown_0[0x2a30];
    Class_0046d040* field_2a30;                    // +0x2a30
};

extern Game_0046c920* g_game;

// FUNCTION: 0x46c920
void FUN_0046c920()
{
    if (g_game->field_2a30)
        delete g_game->field_2a30;
    g_game->field_2a30 = 0;
}
