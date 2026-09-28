// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
#include <stdio.h>

#pragma pack(push, 1)

struct Player_00439740;

// The 16.16 position, the same layout the sibling helpers (0x4399f0) use.
struct Pos_00439740 {
    unsigned short x_frac;            // +0x0
    short x;                          // +0x2
    unsigned short y_frac;            // +0x4
    short y;                          // +0x6
    unsigned short z_frac;            // +0x8
    short z;                          // +0xa
};

struct Point_00439740 {
    short x;                          // +0x0
    short y;                          // +0x2
};

struct Def_00439740 {
    char unknown_0[0x216];
    unsigned short attackLength;      // +0x216
};

struct Weapon_00439740 {
    char unknown_0[0xd6];
    unsigned short field_d6;          // +0xd6
    char unknown_d8[0xe0 - 0xd8];
    int field_e0;                     // +0xe0
};

struct Slot_00439740 {
    Weapon_00439740* weapon;          // +0x0
    char unknown_4[0xf - 4];
    unsigned char flags;              // +0xf
    char unknown_10[0x1c - 0x10];
};

struct Unit_00439740 {
    char unknown_0[0x10];
    Slot_00439740 slots[3];           // +0x10
    char unknown_64[0x6a - 0x64];
    Pos_00439740 pos;                 // +0x6a
    char unknown_76[0x92 - 0x76];
    Def_00439740* def;                // +0x92
    Player_00439740* owner;           // +0x96
};

struct Node_00439740 {
    char unknown_0[4];
    unsigned char kind;               // +0x4
    char unknown_5[0xe - 5];
    Unit_00439740* unit;              // +0xe
    char unknown_12[0x16 - 0x12];
    Unit_00439740* target;            // +0x16
    char unknown_1a[0x22 - 0x1a];
    Pos_00439740 pos;                 // +0x22
    Point_00439740 field_2e;          // +0x2e
    Point_00439740 cached;            // +0x32
    char unknown_36[0x42 - 0x36];
    unsigned int flags;               // +0x42
};

struct Entry_00439740 {               // 0x19-byte entries, table at DAT_00512344
    char unknown_0[0x10];
    unsigned char field_10;           // +0x10
    char unknown_11[0x19 - 0x11];
};

struct Anim_00439740 {
    unsigned short count;             // +0x0
    char unknown_2[0x2c - 2];
    unsigned short field_2c;          // +0x2c
};

struct Game_00439740 {
    char unknown_0[0xdcf];
    unsigned char field_dcf;          // +0xdcf
    char unknown_dd0[0xdd7 - 0xdd0];
    unsigned char field_dd7;          // +0xdd7
    char unknown_dd8[0x1487f - 0xdd8];
    Anim_00439740* anims[1];          // +0x1487f
    char unknown_14883[0x38a47 - 0x14883];
    unsigned int frame;               // +0x38a47
    char unknown_38a4b[0x391bf - 0x38a4b];
    int field_391bf;                  // +0x391bf
};

struct View_00439740 {
    char unknown_0[0x2c];
    int cx;                           // +0x2c
    int cy;                           // +0x30
};

#pragma pack(pop)

extern Game_00439740* g_game;
extern Entry_00439740* DAT_00512344;

int __stdcall FUN_00465ac0(Player_00439740* owner, Unit_00439740* unit);
void __stdcall FUN_00438ea0(void* surface, View_00439740* view, Pos_00439740* pos,
                            int radius, int color, const char* text, int index);
void __stdcall FUN_004b8500(void* dest, void* bmp, int x, int y);

// FUNCTION: 0x439740
void __stdcall FUN_00439740(void* surface, View_00439740* view, Node_00439740* node,
                            Pos_00439740* out, int unused)
{
    char buf[0x40];
    Pos_00439740 pos;
    Unit_00439740* u = node->unit;
    if (node->target != 0) {
        if (FUN_00465ac0(u->owner, node->target) == 0 && (node->flags & 0x200000) != 0) {
            *(int*)&pos.x_frac = node->cached.x << 16;
            *(int*)&pos.y_frac = *(int*)&node->target->pos.y_frac;
            *(int*)&pos.z_frac = node->cached.y << 16;
        } else {
            pos = node->target->pos;
            node->cached.x = pos.x;
            node->cached.y = pos.z;
            node->flags |= 0x200000;
        }
    } else {
        pos = node->pos;
    }
    if (DAT_00512344[node->kind].field_10 == 0) {
        *out = pos;
        return;
    }
    if (g_game->field_391bf != 0 &&
        (DAT_00512344[node->kind].field_10 == 1 || DAT_00512344[node->kind].field_10 == 2)) {
        int color;
        if (g_game->frame & 1)
            color = g_game->field_dcf;
        else
            color = g_game->field_dd7;
        for (int i = 0; i < 3; i++) {
            if (u->slots[(unsigned char)i].flags & 2) {
                if (u->slots[i].weapon->field_d6 != 0) {
                    sprintf(buf, "weapon %d - area of effect", i);
                    FUN_00438ea0(surface, view, &pos, u->slots[i].weapon->field_d6, color, buf, 0);
                }
                if (u->slots[i].weapon->field_e0 != 0) {
                    sprintf(buf, "weapon %d - coverage", i);
                    FUN_00438ea0(surface, view, &pos, u->slots[i].weapon->field_e0, color, buf, 1);
                }
            }
        }
        unsigned short len = u->def->attackLength;
        if (len != 0)
            FUN_00438ea0(surface, view, &pos, len, color, "attack length", 2);
    }
    Anim_00439740* anim = g_game->anims[DAT_00512344[node->kind].field_10];
    unsigned int n = g_game->frame / ((unsigned int)anim->field_2c * 2);
    n = n % anim->count;
    void* bmp = *(void**)((char*)anim + n * 8 + 0x28);
    FUN_004b8500(surface, bmp, pos.x - view->cx + 0x80,
                 pos.z - (pos.y >> 1) - view->cy + 0x20);
    *out = pos;
}
