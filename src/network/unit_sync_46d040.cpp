// Decompiled by Space Bunny Free, finished by DeepSeek V4.1 Flash. Names are provisional.
// A method of UnitSync, whose other methods are in unit_sync.cpp; this one
// stays apart because it needs the real <map>, <list> or <vector>
// instantiations its own way.
//
// The map at +0x00 is the std::map<unsigned int, UnitSyncEntry> whose tree
// header lives at 0x46f720 (written out by hand there as
// Class_0046f720::FUN_0046f720).
#include <list>
#include <map>
#include <vector>

struct UnitSyncEntry {                // the std::map's value, 0x10 bytes
    int x;                             // +0x0
    int y;                             // +0x4
    short w;                           // +0x8
    short h;                           // +0xa
    int flag;                          // +0xc
};

struct PlayerSync_0046d040 {           // 0x5c bytes
    char unknown_0[0x5c];
};

#pragma pack(push, 1)
struct Def_0046d040 {                  // 0x249 bytes
    char unknown_0[0x13e];
    unsigned int key;                  // +0x13e
    char unknown_142[0x245 - 0x142];
    unsigned int flags;                // +0x245
};

struct Game {
    char unknown_0[0x1438f];
    int count;                         // +0x1438f
    char unknown_14393[0x1439b - 0x14393];
    Def_0046d040* defs;                // +0x1439b
};
#pragma pack(pop)

extern Game* g_game;

static inline Def_0046d040* Defs_0046d040()
{
    return g_game->defs;
}

class Sub_0046d040 {                   // the object at +0x2c
public:
    int a;                             // +0x0
    int b;                             // +0x4
    int c;                             // +0x8

    void SendUnsequenced(void* packet, int to);
};

class Sub2_0046d040 {                  // the object at +0x58
public:
    int flag;                          // +0x0
    int* first;                        // +0x4
    int* last;                         // +0x8
    int* end;                          // +0xc
};

struct Cmp_0046d040 {                  // the map's empty key_compare
    char x;
};

struct Alloc_0046d040 {                // the map's empty allocator
    char x;
};

class Class_0046f720 {                 // the std::map's tree header at +0x00
public:
    char field_0;                      // +0x0
    char field_1;                      // +0x1
    char unknown_2[2];
    void* head;                        // +0x4
    char multi;                        // +0x8
    char unknown_9[3];
    int size;                          // +0xc

    // Empty classes taken by value: the prologue copies them from the param slot.
    Class_0046f720(Cmp_0046d040 c, Alloc_0046d040 a)
        : field_0(c.x), field_1(a.x), multi(0)
    {
        FUN_0046f720();
    }

    void FUN_0046f720();

    UnitSyncEntry& operator[](unsigned int key)
    {
        return ((std::map<unsigned int, UnitSyncEntry>*)this)->operator[](key);
    }
};

class UnitSync {
public:
    // Hand-written header, not a real std::map: a real one emits another _Init symbol.
    Class_0046f720 rects;                            // +0x00
    std::vector<PlayerSync_0046d040> players;        // +0x10
    std::list<int> ids;                              // +0x20
    Sub_0046d040 sub;                                // +0x2c
    std::vector<int> list_a;                         // +0x38
    std::vector<int> list_b;                         // +0x48
    Sub2_0046d040 sub2;                              // +0x58

    UnitSync(int param);
};

// Plain inline helper: keeps the tested bit in ebx across the insert call.
static inline bool FlagOf_0046d040(Def_0046d040* d)
{
    return (d->flags >> 16) & 1;
}

// FUNCTION: 0x46d040
UnitSync::UnitSync(int param)
    : rects(Cmp_0046d040(), Alloc_0046d040())
{
    sub.a = 0;
    sub.b = 0;
    sub.c = 0;
    sub2.end = 0;
    sub2.flag = param;
    sub2.first = 0;
    sub2.last = 0;
    {
        UnitSyncEntry v;
        for (unsigned short i = 1; i < g_game->count; i++) {
            unsigned int key = g_game->defs[i].key;
            v.x = key;
            v.y = 0;
            v.w = 1;
            v.h = (short)sub2.flag;
            v.flag = FlagOf_0046d040(&g_game->defs[i]) ? 0 : -1;
            rects[key] = v;
        }
    }
}
