// Decompiled by Space Bunny Free. Names are provisional.
#include <list>
#include <map>
#include <vector>

struct Value_0046d040 {                // the std::map's value, 0x10 bytes
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
    char unknown_249[0x249 - 0x249];
};

struct Game_0046d040 {
    char unknown_0[0x1438f];
    int count;                         // +0x1438f
    char unknown_14393[0x1439b - 0x14393];
    Def_0046d040* defs;                // +0x1439b
};
#pragma pack(pop)

extern Game_0046d040* g_game;

static inline Def_0046d040* Defs_0046d040()
{
    return g_game->defs;
}

class Sub_0046d040 {                   // the object at +0x2c
public:
    int a;                             // +0x0
    int b;                             // +0x4
    int c;                             // +0x8

    void FUN_0046cec0(void* packet, int to);
};

class Sub2_0046d040 {                  // the object at +0x58
public:
    int flag;                          // +0x0
    int* first;                        // +0x4
    int* last;                         // +0x8
    int* end;                          // +0xc
};

class Class_0046d040 {
public:
    std::map<unsigned int, Value_0046d040> rects;    // +0x00
    std::vector<PlayerSync_0046d040> players;         // +0x10
    std::list<int> ids;                               // +0x20
    Sub_0046d040 sub;                                 // +0x2c
    std::vector<int> list_a;                          // +0x38
    std::vector<int> list_b;                          // +0x48
    Sub2_0046d040 sub2;                               // +0x58

    Class_0046d040(int param);
};

// FUNCTION: 0x46d040
Class_0046d040::Class_0046d040(int param)
{
    sub.a = 0;
    sub.b = 0;
    sub.c = 0;
    sub2.end = 0;
    sub2.flag = param;
    sub2.first = 0;
    sub2.last = 0;
    {
        Value_0046d040 v;
        for (unsigned short i = 1; i < g_game->count; i++) {
            v.y = 0;
            v.w = 1;
            bool flag = (Defs_0046d040()[i].flags >> 16) & 1;
            unsigned int key = g_game->defs[i].key;
            v.x = key;
            v.h = (short)sub2.flag;
            v.flag = flag ? 0 : -1;
            rects[key] = v;
        }
    }
}
