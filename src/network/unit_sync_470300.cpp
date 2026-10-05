// Decompiled by Opus. Names are provisional.
// The compiler-generated scalar deleting destructor of UnitSyncPlayer (four
// std::vector members; its out-of-line destructor is 0x46ded0, inlined here).
// The class has no virtual destructor, so MSVC only emits this ??_G where an
// inlined vector<UnitSyncPlayer> destroy loop runs out of inline depth and
// calls it with flag 0: its one caller, 0x46ca60, which deletes the object
// held at g_game+0x2a30 (UnitSync, constructor 0x46d040).
//
// That caller is rebuilt below, unannotated and only approximately (about
// 63%), to emit this COMDAT. Its sibling 0x46c920 deletes the same object
// one inline level shallower and calls ~UnitSyncPlayer instead.
#include <map>
#include <list>
#include <vector>

struct Rect_0046e330 {                 // 0x10 bytes
    int x;                             // +0x0
    int y;                             // +0x4
    short w;                           // +0x8
    short h;                           // +0xa
    int unknown_c;                     // +0xc
};

class UnitSyncPlayer {
public:
    int field_0;                       // +0x00
    std::vector<int> list_a;           // +0x04
    std::vector<int> list_b;           // +0x14
    char unknown_24[0x18];
    std::vector<int> list_c;           // +0x3c
    std::vector<int> list_d;           // +0x4c

    ~UnitSyncPlayer();
};

UnitSyncPlayer::~UnitSyncPlayer()
{
}

class Class_0046e610 {
public:
    std::vector<int> vec;
};

class UnitSync {
public:
    std::map<unsigned int, Rect_0046e330> rects;   // +0x00
    std::vector<UnitSyncPlayer> elems;             // +0x10
    std::list<int> ids;                            // +0x20
    int field_2c;                                  // +0x2c
    int field_30;                                  // +0x30
    int field_34;                                  // +0x34
    Class_0046e610 field_38;                       // +0x38
    std::vector<int> field_48;                     // +0x48
    int field_58;                                  // +0x58
    int field_5c;                                  // +0x5c
    int field_60;                                  // +0x60
    int field_64;                                  // +0x64
};

class Class_0046e160 {
public:
    void ApplyToUnitTypes();
};

struct Game {
    char unknown_0[0x2a30];
    UnitSync* field_2a30;                          // +0x2a30
};

extern Game* g_game;

// FUNCTION: 0x470300 ??_GUnitSyncPlayer@@QAEPAXI@Z
void FinishUnitSync()
{
    ((Class_0046e160*)g_game->field_2a30)->ApplyToUnitTypes();
    if (g_game->field_2a30)
        delete g_game->field_2a30;
    g_game->field_2a30 = 0;
}
