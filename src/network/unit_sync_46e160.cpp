// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
// Refreshes the map entry of every unit type: the 0x249-byte defs at
// g_game+0x1439b are looked up in the same std::map<unsigned int, Rect> that
// 0x46e330 uses (its find() is inlined here), the entry's bit 23 becomes "the
// rect has a non-empty size", and def+0x15a takes the rect's last field.

#pragma pack(push, 1)
struct Rect_0046e160 {
    int x;                             // +0x0
    int y;                             // +0x4
    short w;                           // +0x8
    short h;                           // +0xa
    int unknown_c;                     // +0xc
};

struct Node_0046e160 {
    Node_0046e160* left;               // +0x0
    Node_0046e160* parent;             // +0x4
    Node_0046e160* right;              // +0x8
    unsigned int key;                  // +0xc
    Rect_0046e160 value;               // +0x10
};

struct Flags_0046e160 {
    unsigned int bits_0 : 23;
    unsigned int flag_23 : 1;          // bit 23
    unsigned int bits_24 : 8;
};

struct Def_0046e160 {
    char unknown_0[0x13e];
    unsigned int key;                  // +0x13e
    char unknown_142[0x15a - 0x142];
    int field_15a;                     // +0x15a
    char unknown_15e[0x241 - 0x15e];
    Flags_0046e160 flags_241;          // +0x241
    char unknown_245[0x249 - 0x245];
};

struct Game_0046e160 {
    char unknown_0[0x1438f];
    int count;                         // +0x1438f
    char unknown_14393[0x1439b - 0x14393];
    Def_0046e160* defs;                // +0x1439b
};
#pragma pack(pop)

class Iter_0046e160 {
public:
    Node_0046e160* ptr;

    Iter_0046e160() {}
    Iter_0046e160(Node_0046e160* p) : ptr(p) {}
    bool operator==(const Iter_0046e160& other) const { return ptr == other.ptr; }
};

struct Less_0046e160 {
    bool operator()(const unsigned int& a, const unsigned int& b) const { return a < b; }
};

class Class_0046fe60 {
public:
    Node_0046e160* FUN_0046fe60(const unsigned int* key);
};

class Class_0046e160 {
public:
    char allocator;                    // +0x0
    Less_0046e160 compare;             // +0x1
    char unknown_2[2];
    Node_0046e160* head;               // +0x4
    char unknown_8[0x64 - 0x8];
    int field_64;                      // +0x64

    Iter_0046e160 End() { return Iter_0046e160(head); }
    Iter_0046e160 Find(const unsigned int* key)
    {
        Iter_0046e160 p = Iter_0046e160(((Class_0046fe60*)this)->FUN_0046fe60(key));
        return (p == End() || compare(*key, p.ptr->key)) ? End() : p;
    }
    void FUN_0046e160();
};

extern Game_0046e160* g_game;

void FUN_00428fe0();
void FUN_00428fc0();

// FUNCTION: 0x46e160
void Class_0046e160::FUN_0046e160()
{
    if (field_64 != 0)
        return;
    for (unsigned short i = 1; i < g_game->count; i++) {
        Def_0046e160* def = &g_game->defs[i];
        Iter_0046e160 it = Find(&def->key);
        if (it == End()) {
            FUN_00428fe0();
            def->field_15a = 0;
            def->flags_241.flag_23 = 0;
        } else {
            FUN_00428fe0();
            def->flags_241.flag_23 = (it.ptr->value.w != 0 && it.ptr->value.h != 0);
            def->field_15a = it.ptr->value.unknown_c;
        }
        FUN_00428fc0();
    }
}
