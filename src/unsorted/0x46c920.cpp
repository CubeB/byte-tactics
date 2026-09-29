// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash, finished by Sonnet 5.5. Names are provisional.
//
// Releases the overlay object at g_game+0x2a30 (Class_0046d040, built by
// 0x46c8e0 and its constructor 0x46d040): `if (obj) delete obj;` with the
// whole ~Class_0046d040 inlined here.
//
// The function must be __fastcall (or the TU was built with /Gr). As a plain
// __cdecl or a __thiscall method, MSVC reloads the list iterator before the
// erase loop's bottom test, `mov ecx,[esp+0x10]; cmp ecx,ebx`; the original
// compares the stack slot directly, `cmp [esp+0x10],ebx`. With __fastcall
// (no arguments, so the code is otherwise unchanged) it is byte-identical.
// The map at +0x00 is the real std::map<unsigned int, Rect_0046e160>, so its
// erase keeps the name 0x46e890 has in data/symbols.csv, as in 0x46d1a0.cpp.
#include <list>
#include <map>
#include <vector>

struct Rect_0046e160 {                 // the map's mapped type, 0x10 bytes
    int x;                             // +0x0
    int y;                             // +0x4
    short w;                           // +0x8
    short h;                           // +0xa
    int unknown_c;                     // +0xc
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
    std::map<unsigned int, Rect_0046e160> map;     // +0x00
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
void __fastcall FUN_0046c920()
{
    if (g_game->field_2a30)
        delete g_game->field_2a30;
    g_game->field_2a30 = 0;
}
