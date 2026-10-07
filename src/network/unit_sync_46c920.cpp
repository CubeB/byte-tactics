// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash, finished by Sonnet 5.5. Names are provisional.
//
// Releases the overlay object at g_game+0x2a30 (UnitSync, built by
// 0x46c8e0 and its constructor 0x46d040): `if (obj) delete obj;` with the
// whole ~UnitSync inlined here.
#include <list>
#include <map>
#include <vector>

struct UnitSyncEntry {                 // the map's mapped type, 0x10 bytes
    int x;                             // +0x0
    int y;                             // +0x4
    short w;                           // +0x8
    short h;                           // +0xa
    int unknown_c;                     // +0xc
};

class UnitSyncPlayer {                 // the vector's element, 0x5c bytes
public:
    char unknown_0[0x5c];

    ~UnitSyncPlayer();
};

class Class_0046e610 {                 // holds a std::vector<int>
public:
    std::vector<int> vec;              // +0x0

    ~Class_0046e610();
};

class UnitSync {
public:
    // Real std::map: its erase keeps the symbols.csv name.
    std::map<unsigned int, UnitSyncEntry> map;     // +0x00
    std::vector<UnitSyncPlayer> elems;             // +0x10
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

    ~UnitSync() {}
};

struct Game {
    char unknown_0[0x2a30];
    UnitSync* field_2a30;                          // +0x2a30
};

extern Game* g_game;

// FUNCTION: 0x46c920
// __fastcall: keeps the erase loop comparing the iterator slot directly.
void __fastcall DeleteUnitSync()
{
    if (g_game->field_2a30)
        delete g_game->field_2a30;
    g_game->field_2a30 = 0;
}
