// Decompiled by deepseek-v4.1, finished by deepseek-v4.1-flash. Names are provisional.
// PARTIAL: 53.0% (996 of the original's 1015 bytes). Five loops over the unit
// array (stride 0x118): A clears/sets flags 0x1000/0x700/0x300 from the player
// index and two "data" records, B ranges over pl->field_67..pl->field_6b and
// hands a Class_00467840 visitor to FUN_0047e890, C does the same with the two
// 4-byte visitors (Class_00467960 / Class_00467980), D sets flag 0x1000 and
// field_b0, E sets flag 0x100 from the per-player cell masks.
//
// Biggest lever in this session (43.6% -> 51.6% -> 53.0%): DECLARE EACH VISITOR
// LAST, after the call arguments have been materialised into locals, and call
// FUN_0047e890 through those temporaries:
//     int r = (int)u->def->field_20a << 16;
//     Vec3_00467440* pp = &u->pos.vec;
//     Class_00467960 v;
//     FUN_0047e890(pp, r, &v);
// That sinks the vptr store to just before the call, exactly where the original
// has it (after the pushes). Writing the arguments inline instead emits the
// vtable store at the top of the block; that costs ~8 points.
//
// What still differs (53.0%). The frame now MATCHES the original exactly:
// sub esp,0x28, last=+0x10, first=+0x14, visitor slots +0x18 and +0x1c, the
// 0x18-byte Class_00467840 at +0x20 (the diff shows the prologue and every
// [esp+N] offset already matching, so the older note about a 0x24 frame is
// stale). The residual gap is register choice only:
//  - Loop A tail: the original reloads `last` into esi (ebx is clobbered
//    inside the body, esi is dead scratch there) and keeps edi = first
//    untouched; ours reloads into ebx and re-materialises edi before loop B.
//  - Loop B's arithmetic: the original keeps `a` in ax, `b` in di, `t` in ecx
//    and does `imul ecx,ecx` / `imul edx,edx` in place; ours loads b into cx
//    early (`mov cx, word[ecx+0x206]`) and needs `mov edi,edx; imul edi,edx`
//    copies, plus the vptr store lands early. `def` sits in ecx here, in edx
//    there.
//  - Loop C: the two vtable constants are swapped relative to the original
//    (original: ebx = first visitor's vtable, edi = second; ours the other way
//    round), and ours re-materialises edi = first before loop C starts.
//  - Loop E: the original computes x and y INSIDE each arm of the field_14281
//    mode test (duplicated movsx/sar) and anchors the loop at esi = u+0x74
//    (flags at [esi+0x9c], x as [esi-8] = +0x6c); ours hoists x/y, anchors at
//    edi = u+0x6c and uses bh for the flags word. Duplicating x/y into both
//    arms was retried this session on the correct 0x28 frame: 1013 bytes (the
//    closest byte count of any variant) but 52.5%, and the anchor stayed at
//    u+0x6c, so it was not kept.
//  - Loop D: original materialises 0x2000 in ebx and 0x1000 in edi, ours the
//    other way round.
//
// Scratch variants scored (check.py <addr> <file>): base42 42.4%, base43 43.6%,
// loop-B reorder 43.6%, both loop-C visitors in one scope 36.6%, v516 51.6%,
// v530 (this file) 53.0%. Session 2: both loop-C visitors hoisted to the outer
// block 40.8%, per-loop `u` declarations 53.0% (tie), `b` deferred until after
// t 52.7%, loop E x/y duplicated into both arms of the field_14281 test 52.5%
// (1013 bytes, closest byte count, but it still anchors the loop at u+0x6c
// where the original uses u+0x74, so it was not kept).
//
// Session 3 (deepseek-v4.1): the loop E anchor moves from u+0x6c to u+0x74,
// matching the original's lea and [base+N] offsets, when y is computed BEFORE
// x in the source (write the f74 expression first, then the f6c one). The
// py-pointer spelling (short* py = &u->pos.half.f74; py[-4], py[-2], py[0])
// moves it identically. That y-first form is the current file, still 53.0%;
// the residual loop E diff is register names and load scheduling only.
// Combining y-first with x/y duplicated into both arms of the field_14281
// test: 1016 bytes / 52.4% (plain) and 1012 bytes / 52.8% (py-pointer), so
// neither was kept over the current file.

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

    for (u = first; u <= last; u++) {
        if (u->flags & 0x10000000) {
            u->flags &= ~0x1000;
            if (u->field_ff == player
                || (u->field_96->field_108[pl->field_146] != 0
                    && (u->field_96->data->field_97 & 0x40) != 0)
                || (*(int*)pl != 0 && (pl->data->field_9b & 0x40) != 0)) {
                u->flags |= 0x300;
            } else {
                u->flags &= ~0x700;
            }
        }
    }

    for (u = pl->field_67; u <= pl->field_6b; u++) {
        if ((u->flags & 0x10000000) && !(u->flags & 0x4000) && (u->field_10e & 1)) {
            if (u->def->field_204 != 0 || u->def->field_206 != 0) {
                short a = u->def->field_204;
                short b = u->def->field_206;
                int t = a + u->pos.half.f70 * 2;
                int r = (int)(a > b ? a : b) << 16;
                Vec3_00467440* pp = &u->pos.vec;
                Class_00467840 v;
                v.field_4 = t * t;
                v.field_8 = (int)b * (int)b;
                v.pos = u->pos.vec;
                FUN_0047e890(pp, r, &v);
            }
        }
    }

    for (u = first; u <= last; u++) {
        if ((u->flags & 0x10000000) && u->field_ff != pl->field_146 && (u->field_10e & 1)) {
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
                        u->flags |= 0x1000;
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
