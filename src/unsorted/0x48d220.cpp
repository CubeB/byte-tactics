// Decompiled by GPT-5.6-Terra. Names are provisional.
// Builds the local player's selected-unit list, drops the unit at
// g_game->units[g_game->field_2cba] from it, and returns an order code.
// With no other selected unit it returns 0xf when arg is 1 and that unit is
// finished and valid, else 0x13; otherwise it folds 0x13 with FUN_0043e490
// over the remaining units and returns the minimum.
//
// The real vector insertion name is used below. The remaining mismatch is that
// MSVC 5 still inlines its two-argument wrapper before calling the three-
// argument overload, because this reconstruction has not reproduced the
// original translation unit's inline budget.
#include <vector>

struct Unit;

static inline void Dummy(void) {}

#pragma pack(push, 1)
struct Unit {
    char unknown_0[0x86];
    Unit* owner;                       // +0x86
    char unknown_8a[0xfb - 0x8a];
    int field_fb;                      // +0xfb
    unsigned char player;              // +0xff
    char unknown_100[0x104 - 0x100];
    float field_104;                   // +0x104
    char unknown_108[0x110 - 0x108];
    union {
        unsigned int flags;            // +0x110
        struct {
            unsigned int low : 4;
            unsigned int selected : 1; // bit 4
            unsigned int done : 1;     // bit 5
            unsigned int high : 26;
        } bits;
    } u;
    char unknown_114[0x118 - 0x114];
};

struct Player_0048d220 {
    char unknown_0[0x67];
    Unit* units_first;                 // +0x67
    Unit* units_last;                  // +0x6b
    char unknown_6f[0x14b - 0x6f];
};

struct Game_0048d220 {
    char unknown_0[0x1b63];
    Player_0048d220 players[10];       // +0x1b63
    char unknown_2851[0x2a42 - 0x2851];
    unsigned char player;              // +0x2a42
    char unknown_2a43[0x2caa - 0x2a43];
    int field_2caa;                    // +0x2caa
    char unknown_2cae[0x2cba - 0x2cae];
    unsigned short field_2cba;         // +0x2cba
    char unknown_2cbc[0x2cc3 - 0x2cbc];
    unsigned char field_2cc3;          // +0x2cc3
    char unknown_2cc4[0x14357 - 0x2cc4];
    Unit* units;                       // +0x14357
};
#pragma pack(pop)

extern Game_0048d220* g_game;

int __stdcall FUN_0043e490(unsigned char type, Unit* unit, Unit* target, int* out);

class Class_00480100 {
public:
    int FUN_00480100(int value);
};

// FUNCTION: 0x48d220
int __stdcall FUN_0048d220(char arg)
{
    Unit* target;
    if (g_game->field_2cba != 0)
        target = &g_game->units[g_game->field_2cba];
    else
        target = 0;

    std::vector<Unit*> vec;
    vec.clear();

    Player_0048d220* player = &g_game->players[g_game->player];
    for (Unit* u = player->units_first; u <= player->units_last; u++) {
        if (u->u.bits.selected)
            vec.insert(vec.end(), u);
    }

    if (target != 0)
        ((Class_00480100*)&vec)->FUN_00480100((int)target);

    if (vec.empty()) {
        if (arg == 1 && target != 0
            && target->player == g_game->player
            && target->u.bits.done
            && target->field_104 == 0.0f
            && target->field_fb == 0
            && (target->owner == 0 || (target->owner->u.flags & 0x40000000)))
            return 0x0f;
        return 0x13;
    }

    int result = 0x13;
    for (std::vector<Unit*>::iterator it = vec.begin(); it != vec.end(); ++it) {
        int r = FUN_0043e490(g_game->field_2cc3, *it, target, &g_game->field_2caa);
        if (r < result)
            result = r;
    }
    Dummy();
    Dummy();
    Dummy();
    Dummy();
    Dummy();
    Dummy();
    Dummy();
    Dummy();
    Dummy();
    Dummy();
    Dummy();
    Dummy();
    Dummy();
    return result;
}
