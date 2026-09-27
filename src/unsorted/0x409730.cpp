// Decompiled by Claude Opus 5.5. Names are provisional.
// Recomputes a player's per-unit-type tables (the object built by 0x409160):
// resizes the tables at +0x8d and +0x65 to the unit type count, then for each
// unit type rates it into vec_8d[i] and the three bytes of vec_65[i].
//
// Best so far 88.1%: the code is the same length and every instruction
// matches except the base/index order of two byte accesses to vec_8d:
// the original has `mov [esi + ecx], al` (store) and `movsx eax, byte ptr
// [eax + edx]` (the `(char)vec_8d[i] / 2` read), ours encodes [ecx + esi] and
// [edx + eax]. No header set (tools/headers.py plus <ddraw.h>, <string>,
// <map>, <list> and others), access form (begin()[i], *(begin() + i),
// unsigned index), full class layout, or defining 0x409160 and 0x409470
// above this function in the same file changed it. The weapon sum's
// division order also flips with the number of declarations in the file
// (it goes wrong with the full class layout), so both are probably compiler
// state from the rest of the original file.
//
// Things that were needed to get here:
// - MSVC 5's inline budget decides which STL calls stay out of line (the
//   first resize() inlines erase() and its _Destroy, the second calls insert
//   and erase out of line). It only matched with the inline Def methods
//   HasField1ce() and Bonus1ce() below, whose inlined calls use up the budget
//   the way the original's did.
// - The last byte is one windows.h min/max expression; the sum
//   `(float)(f18a * -0.02f) + (bonus ? 25 : 0)` is shared between the macro's
//   repeated evaluations (MSVC spills it to [esp+0x14]), while Bonus1ce()
//   is re-evaluated each time. Without the (float) cast MSVC folds the
//   -0.02 into a subtraction.
// - The two flag bits at +0x241 are read from one local copy of the bitfield
//   word (`mov ecx, ebx; shr ecx, 0xb; test cl, 1`).
#include <windows.h>
#include <math.h>
#include <vector>
struct Unit {
    int unknown_0;
};

struct Elem_0040cfb0 {
    char a;
    char b;
    char c;
};

struct Elem_0040d4f0 {
    char value;
};

struct Elem_0040d550 {
    int unknown_0;
};

struct Elem_0040c580 {
    int a;
    int b;
};

#pragma pack(push, 1)
struct Weapon_00409730 {
    char unknown_0[0xd4];
    unsigned short field_d4;           // +0xd4
    char unknown_d6[0xdc - 0xd6];
    int field_dc;                      // +0xdc
    char unknown_e0[0x10a - 0xe0];
    char field_10a;                    // +0x10a
};

struct Flags241_00409730 {
    unsigned int bits_0 : 6;
    unsigned int flag_6 : 1;           // bit 6
    unsigned int bits_7 : 4;
    unsigned int flag_11 : 1;          // bit 11
    unsigned int bits_12 : 12;
    unsigned int flag_24 : 1;          // bit 24
    unsigned int bits_25 : 7;
};

struct Def_00409730 {
    int HasField1ce() { return field_1ce != 0.0f; }
    int Bonus1ce() { if (field_1ce != 0.0f) return 100; return 0; }
    char unknown_0[0x186];
    float field_186;                   // +0x186
    float field_18a;                   // +0x18a
    char unknown_18e[0x1c0 - 0x18e];
    short field_1c0;                   // +0x1c0
    float field_1c2;                   // +0x1c2
    char unknown_1c6[0x1ce - 0x1c6];
    float field_1ce;                   // +0x1ce
    float field_1d2;                   // +0x1d2
    char unknown_1d6[0x1ee - 0x1d6];
    Weapon_00409730* weapons[3];       // +0x1ee
    char unknown_1fa[0x204 - 0x1fa];
    short field_204;                   // +0x204
    short field_206;                   // +0x206
    char unknown_208[0x22d - 0x208];
    char field_22d;                    // +0x22d
    char unknown_22e[0x241 - 0x22e];
    Flags241_00409730 flags_241;       // +0x241
    unsigned int bits_245_0 : 4;
    unsigned int flag_245_4 : 1;       // bit 4
    unsigned int bits_245_5 : 3;
    unsigned int flag_245_8 : 1;       // bit 8
    unsigned int bits_245_9 : 23;
};

struct Game_00409730 {
    char unknown_0[0x1425f];
    int field_1425f;                   // +0x1425f
    char unknown_14263[0x1434f - 0x14263];
    unsigned short field_1434f;        // +0x1434f
    char unknown_14351[0x1438f - 0x14351];
    int count;                         // +0x1438f
    char unknown_14393[0x1439b - 0x14393];
    Def_00409730* defs;                // +0x1439b
    char unknown_1439f[0x37ec8 - 0x1439f];
    int field_37ec8;                   // +0x37ec8
};

struct Player_00409730 {
    char unknown_0[0x144];
    unsigned short field_144;          // +0x144
};

struct UnitList_00409730 {
    std::vector<Unit*> units;
};

class Class_00409730 {
public:
    Player_00409730* player;           // +0x00
    unsigned char index;               // +0x04
    UnitList_00409730 list_5;          // +0x05
    UnitList_00409730 list_15;         // +0x15
    UnitList_00409730 list_25;         // +0x25
    int pos_35[3];                     // +0x35
    int pos_41[3];                     // +0x41
    std::vector<Elem_0040c580> vec_4d; // +0x4d
    short centerX;                     // +0x5d
    short centerY;                     // +0x5f
    int field_61;                      // +0x61
    std::vector<Elem_0040cfb0> vec_65; // +0x65
    int field_75;                      // +0x75
    int field_79;                      // +0x79
    std::vector<unsigned short> vec_7d; // +0x7d
    std::vector<unsigned char> vec_8d; // +0x8d
    std::vector<unsigned char> vec_9d; // +0x9d

    void FUN_00409730();
};
#pragma pack(pop)

extern Game_00409730* g_game;

float __stdcall FUN_00488f30(Def_00409730* def);

// FUNCTION: 0x409730
void Class_00409730::FUN_00409730()
{
    vec_8d.resize(g_game->count, 0);
    {
        Elem_0040cfb0 e;
        e.a = 0;
        e.b = 0;
        e.c = 0;
        vec_65.resize(g_game->count, e);
    }
    for (int i = 1; i < g_game->count; i++) {
        Def_00409730* def = &g_game->defs[(unsigned short)i];
        int a = 1;
        if (def->HasField1ce())
            a = 11;
        if (def->field_22d)
            a += 10;
        if (FUN_00488f30(def) < 0.0f)
            a += 10;
        int t = (int)(a - def->field_18a * -0.01f);
        a = (int)(t - def->field_186 * -0.002f);
        int b = 1;
        if (def->flag_245_4)
            b = 11;
        for (int w = 0; w < 3; w++) {
            Weapon_00409730* wp = def->weapons[w];
            if (wp->field_10a)
                b += wp->field_dc / 100 + wp->field_d4 / 40 + 5;
        }
        a += (char)max(-100, min(100, b));
        vec_8d[i] = max(-100, min(100, a));

        a = 1;
        Elem_0040cfb0* e = &vec_65[i];
        int n = (short)vec_7d[i];
        if (def->flag_245_4)
            a = 21;
        if (def->flags_241.flag_6 && n < 3)
            a += 30;
        if (FUN_00488f30(def) < 0.0f)
            a += 50;
        if (def->HasField1ce())
            a += 50;
        if (def->field_22d)
            a += 25;
        Flags241_00409730 flags = def->flags_241;
        if (flags.flag_11)
            a += 40;
        if (def->field_206)
            a += 15;
        if (def->field_204)
            a += 5;
        int x = (int)(a + min(max(def->field_1c2, 0.0f), 30.0f));
        if (n == 0)
            x *= 4;
        if (n == 1)
            x *= 2;
        if (def->field_1c0 >= 0)
            x *= 3;
        if (player->field_144 > (unsigned short)(g_game->field_1434f / 2))
            x += (char)vec_8d[i] / 2;
        if (def->flag_245_8)
            x = 0;
        if (flags.flag_24)
            x = 0;
        if (def->field_1d2 != 0.0f && g_game->field_1425f < g_game->field_37ec8 / 2)
            x = 0;
        x = min(x, 100);
        e->a = x;
        e->c = (char)max(0.0f, min(100.0f, def->field_186 * -0.0025f - FUN_00488f30(def) * 5.0f));
        e->b = (char)max(0.0f, min(100.0f, (float)(def->field_18a * -0.02f) + (def->field_22d ? 25 : 0) + def->Bonus1ce()));
    }
}
