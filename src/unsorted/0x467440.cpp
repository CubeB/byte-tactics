// Decompiled by deepseek-v4.1, finished by deepseek-v4.1-flash, GPT-6.1-sol. Names are provisional.
// deepseek-v4.1-flash 2026-10-01 (retry 2): spelling the Loop C compare as
// `u->field_ff != pl->field_146` regresses 74.6 to 74.2 (986 bytes), so the
// original cl-first load plus `cmp al, cl` is not reachable by operand swap.
// deepseek-v4.1-flash retry 2026-10-01: moving `vis = 0;` to the head of the else arm (dropping the explicit else) regressed 74.6 to 74.1 (991 bytes), so the original really has the neg/sbb/neg before the bounds-failure path. Restored base.
// PARTIAL: 74.6% (986 of the original's 1015 bytes). Five loops over the unit
// array (stride 0x118). Loop B now matches after computing t = a + f70*2
// BEFORE loading b, then t = t*t, s = b*b, then if (a <= b) a = b; and calling
// with (int)a << 16. Loop C's compare matches with pl->field_146 != u->field_ff.
//
// What still differs (74.6%):
//  - Loop C first visitor call: the original stores the vptr AFTER the three
//    pushes (mov [esp+0x24], ebx) and materialises pp into edx before them;
//    ours stores the vptr before the pushes and computes pp at the third push.
//  - Loop C body: the original loads u->field_ff into al and pl->field_146
//    into cl; ours still swaps those two registers (the compare operands now
//    match, the register names do not).
//  - Loop D: the original loads u->flags AFTER the field_b0 store
//    (mov [esi+0x1e],edx; mov eax,[esi+0x7e]; or eax,edi); ours hoists the
//    flags load above the g_game reload. |= vs = x | 1000 makes no difference.
//  - Loop E: the original anchors the cursor at u+0x74 in esi with flags in
//    ebp (test ebp,0x100), ours anchors with add edi,0x74 and flags in ebx
//    (test bh,1); body register allocation (x/y/pl) differs throughout.

#pragma pack(push, 1)

struct Vec3_00467440 {
    int x;
    int y;
    int z;
};

union UnitPos_00467440 {
    Vec3_00467440 vec;
    struct {
        short f6a;
        short f6c;
        short f6e;
        short f70;
        short f72;
        short f74;
    } half;
};

struct UnitDef_00467440 {
    char unknown_0[0x204];
    short field_204;                   // +0x204
    short field_206;                   // +0x206
    short field_208;                   // +0x208
    short field_20a;                   // +0x20a
    short field_20c;                   // +0x20c
    char unknown_20e[0x245 - 0x20e];
    unsigned int field_245;            // +0x245
};

struct PlayerData_00467440 {
    char unknown_0[0x97];
    unsigned char field_97;            // +0x97
    char unknown_98[0x9b - 0x98];
    unsigned char field_9b;            // +0x9b
};

struct Owner_00467440 {
    void* field_0;                     // +0x0
    char unknown_4[0x27 - 0x4];
    PlayerData_00467440* data;         // +0x27
    char unknown_2b[0x73 - 0x2b];
    char field_73;                     // +0x73
    char unknown_74[0x108 - 0x74];
    unsigned char field_108[1];        // +0x108
};

struct Unit_00467440 {
    char unknown_0[0x6a];
    UnitPos_00467440 pos;              // +0x6a
    char unknown_76[0x92 - 0x76];
    UnitDef_00467440* def;             // +0x92
    Owner_00467440* field_96;          // +0x96
    char unknown_9a[0xb0 - 0x9a];
    int field_b0;                      // +0xb0
    char unknown_b4[0xff - 0xb4];
    unsigned char field_ff;            // +0xff
    char unknown_100[0x10e - 0x100];
    unsigned char field_10e;           // +0x10e
    char unknown_10f;
    unsigned int flags;                // +0x110
    char unknown_114[0x118 - 0x114];
};

struct PlayerInfo_00467440 {
    void* field_0;                     // +0x0
    char unknown_4[0x27 - 0x4];
    PlayerData_00467440* data;         // +0x27
    char unknown_2b[0x67 - 0x2b];
    Unit_00467440* field_67;           // +0x67
    Unit_00467440* field_6b;           // +0x6b
    char unknown_6f[0x7c - 0x6f];
    unsigned char* field_7c;           // +0x7c
    unsigned int field_80;             // +0x80
    unsigned int field_84;             // +0x84
    char unknown_88[0x146 - 0x88];
    unsigned char field_146;           // +0x146
};

struct Game_00467440 {
    char unknown_0[0x2a3c];
    unsigned short field_2a3c;         // +0x2a3c
    char unknown_2a3e[0x2a43 - 0x2a3e];
    unsigned char playerIndex;         // +0x2a43
    char unknown_2a44[0x14273 - 0x2a44];
    unsigned short* field_14273;       // +0x14273
    char unknown_14277[0x14281 - 0x14277];
    unsigned char field_14281;         // +0x14281
    char unknown_14282[0x14357 - 0x14282];
    Unit_00467440* units;              // +0x14357
    Unit_00467440* units_end;          // +0x1435b
    char unknown_1435f[0x38a47 - 0x1435f];
    int field_38a47;                   // +0x38a47
};
#pragma pack(pop)

class Class_00467840 {
public:
    virtual void FUN_00467840(Unit_00467440* unit);
    int field_4;                       // +0x4
    int field_8;                       // +0x8
    Vec3_00467440 pos;                 // +0xc
};

class Class_00467960 {
public:
    virtual void FUN_00467960(Unit_00467440* unit);
};

class Class_00467980 {
public:
    virtual void FUN_00467980(Unit_00467440* unit);
};

extern Game_00467440* g_game;

void __stdcall FUN_0047e890(Vec3_00467440* pos, int range, void* visitor);
bool __stdcall FUN_0040b0d0(int player, Vec3_00467440* p, int range);

// FUNCTION: 0x467440
void FUN_00467440(void)
{
    if (g_game->field_2a3c < 2) {
        return;
    }
    unsigned char player = g_game->playerIndex;
    Unit_00467440* first = g_game->units + 1;
    Unit_00467440* last = g_game->units_end;
    PlayerInfo_00467440* pl = (PlayerInfo_00467440*)((char*)g_game + 0x1b63
        + (unsigned int)g_game->playerIndex * 0x14b);
    Unit_00467440* u;

    Unit_00467440* a;
    for (a = first; a <= last; a++) {
        if (a->flags & 0x10000000) {
            a->flags &= ~0x1000;
            if (a->field_ff == player
                || (a->field_96->field_108[pl->field_146] != 0
                    && (a->field_96->data->field_97 & 0x40) != 0)
                || (*(int*)pl != 0 && (pl->data->field_9b & 0x40) != 0)) {
                a->flags |= 0x300;
            } else {
                a->flags &= ~0x700;
            }
        }
    }

    for (u = pl->field_67; u <= pl->field_6b; u++) {
        if ((u->flags & 0x10000000) && !(u->flags & 0x4000) && (u->field_10e & 1)) {
            if (u->def->field_204 != 0 || u->def->field_206 != 0) {
                short a = u->def->field_204;
                int t = a + u->pos.half.f70 * 2;
                short b = u->def->field_206;
                t = t * t;
                int s = (int)b * (int)b;
                if (a <= b) {
                    a = b;
                }
                Vec3_00467440* pp = &u->pos.vec;
                Class_00467840 v;
                v.field_4 = t;
                v.field_8 = s;
                v.pos = u->pos.vec;
                FUN_0047e890(pp, (int)a << 16, &v);
            }
        }
    }

    for (u = first; u <= last; u++) {
        if ((u->flags & 0x10000000) && pl->field_146 != u->field_ff && (u->field_10e & 1)) {
            if (u->def->field_20a != 0) {
                int r = (int)u->def->field_20a << 16;
                Vec3_00467440* pp = &u->pos.vec;
                Class_00467960 v;
                FUN_0047e890(pp, r, &v);
            }
            if (u->def->field_20c != 0) {
                int r2 = (int)u->def->field_20c << 16;
                Vec3_00467440* pp2 = &u->pos.vec;
                Class_00467980 v;
                FUN_0047e890(pp2, r2, &v);
            }
        }
    }

    for (u = first; u <= last; u++) {
        if ((u->flags & 0x10000000) && u->field_96->field_0 != 0) {
            char c = u->field_96->field_73;
            if (c == 1 || c == 2) {
                if (u->def->field_245 & 0x2000) {
                    if (FUN_0040b0d0(u->field_ff, &u->pos.vec, u->def->field_208)) {
                        u->field_b0 = g_game->field_38a47 + 0x5a;
                        u->flags = u->flags | 0x1000;
                    }
                }
            }
        }
    }

    for (u = first; u <= last; u++) {
        unsigned int f = u->flags;
        if ((f & 0x10000000) && !(f & 0x100) && !(u->field_10e & 4)) {
            unsigned char pi = g_game->playerIndex;
            PlayerInfo_00467440* p2 =
                (PlayerInfo_00467440*)((char*)g_game + 0x1b63 + (unsigned int)pi * 0x14b);
            int y = ((int)u->pos.half.f74 - ((int)u->pos.half.f70 >> 1)) >> 5;
            int x = (int)u->pos.half.f6c >> 5;
            int vis;
            if ((g_game->field_14281 & 2) == 2) {
                vis = 0;
                if ((unsigned int)x < p2->field_80 && (unsigned int)y < p2->field_84
                    && p2->field_7c[p2->field_80 * y + x] != 0) {
                    vis = 1;
                }
            } else {
                if ((unsigned int)x < p2->field_80 && (unsigned int)y < p2->field_84)
                    vis = (g_game->field_14273[p2->field_80 * y + x] & (1 << pi)) != 0;
                else
                    vis = 0;
            }
            if (vis) {
                u->flags = f | 0x100;
            }
        }
    }
}
