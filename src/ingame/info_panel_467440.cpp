// Decompiled by deepseek-v4.1, finished by deepseek-v4.1-flash, GPT-6.1-sol, finished by mimo-v2.6-pro, finished by DeepSeek V4.1 Flash, checked by GPT-6. Names are provisional.
// GPT-6 retry (#4891): checkall.py confirms this file prints MATCH.
// DeepSeek V4.1 Flash 2026-10-02: MATCH (1015 bytes). The last Loop C
// mismatch is fixed by giving the field_ff load its own condition term:
//   (u->flags & 0x10000000) && (ff = u->field_ff, 1) && ff != pl->field_146
// The `, 1` keeps the assignment as a separate term, so MSVC emits
// `test dword ptr [u+0x7e],0x10000000` first and then
// `mov al,[u+0xff]; mov cl,[pl+0x146]; cmp al,cl`, and the first visitor call
// keeps the original vptr store after the pushes. Writing the compare as
// `pl->field_146 != ff` flips the cmp operands (99.6); folding the compare
// into the comma term (`(ff = u->field_ff, pl->field_146 != ff)`) moves it to
// the register form and reschedules the visitor block (97.5).
// mimo-v2.6-pro 2026-10-01 (timeboxed retry, second pass): 97.9 -> 98.6
// percent. Loop D's flags-load hoist is fixed by writing the field_b0 store
// through an `int&` to the field (the 0x41ba60 pattern from the guide): the
// reference keeps MSVC from moving the `mov eax,[esi+0x7e]` load above the
// store.
// Still differs (98.6): Loop C compare only. Original:
//   test dword ptr [esi+0x7e],0x10000000 / je
//   mov al,[esi+0x6d] / mov cl,[ebp+0x146] / cmp al,cl / je
// Ours (with the single `unsigned char ff` local that fixes the first visitor
// call block's vptr store and arg registers):
//   mov ecx,[esi+0x7e] / mov al,[esi+0x6d] / test ecx / je
//   cmp byte ptr [ebp+0x146],al / je
// The ff local fixes call 1 but makes the compiler hoist the flags load into
// ecx and compare memory-to-al. Tried and flat/worse this pass: inline
// `u->field_ff != pl->field_146` (96.5, gets `test [esi+0x7e]` back but swaps
// al/cl in the compare and breaks the visitor-call block), inline reversed
// (96.8, same), nested if with two byte locals either order (96.5/96.8, same
// al/cl swap). Remaining idea not tried: two locals `ff`/`f146` with one of
// them as `char` (signed) or the compare written through a tiny inline helper,
// to force al=field_ff / cl=field_146 while keeping the call block.
// mimo-v2.6-pro 2026-10-01 (timeboxed retry): 75.9 -> 97.9 percent (1015 of
// 1015 bytes). What fixed it: Loop E split into IsExplored/IsSeen inline
// helpers (the matched 0x4658e0 pattern, Contains/Get on a ByteMap at
// player+0x7c, tx/ty computed inside each helper) with the player pointer
// built by an inline Game::Current() member (the 0x47f300 trick) and a plain
// `unsigned int vis` result local (int vis tail-merged the two zero blocks and
// lost 4 bytes). Loop C's compare now uses an `unsigned char ff` local before
// the if, which also fixed the first visitor call block's vptr store and arg
// registers.
// deepseek-v4.1-flash 2026-10-01 (retry 7, timeboxed): 75.2 -> 75.9 percent
// (975 bytes). Moving the Loop E `int pi = g_game->playerIndex;` and the p2 lea
// chain to AFTER the y/x pos loads (`int y ...; int x ...;`) gains 0.7 percent:
// the original schedules the p2 chain and the field_14281 test ahead of the
// f70/f74/f6c loads, and this source order gets the pos loads after p2.
// Flat at 75.9: fresh loop variable for Loop E (declared outside or in the for),
// x-before-y swap, dropping the pi local (p2 from g_game->playerIndex inline).
// Regressed or flat otherwise: removing the Loop E `unsigned int f` temp 75.5,
// `vis = (a && b && c) != 0` 75.2 / 989, declaring the Loop C visitor before pp
// 75.2, `u->flags = u->flags | 0x1000` 75.2. Still open: Loop E cursor in ebp
// (original esi) with flags in ebx (original ebp), Loop C push/vptr schedule,
// Loop D flags-load order.

// deepseek-v4.1-flash 2026-10-01 (retry 6, timeboxed): no gain, stays 75.2 /
// 975 bytes. Two fresh spellings are neutral/negative: Loop C `u->field_ff !=
// pl->field_146` regresses to 74.8 / 975, and Loop D `u->flags |= 0x1000` is
// byte-neutral at 75.2 / 975.
// deepseek-v4.1-flash 2026-10-01 (retry 5, timeboxed): no gain, stays 75.2 /
// 975 bytes. Removing the Loop E `unsigned int f = u->flags;` temp (using
// u->flags directly in the two tests and the store) regresses 75.2 to 74.8 /
// 981 bytes, so the shared temp is required; the ebx-vs-ebp flags pick stands.

// Decompiled by deepseek-v4.1, finished by deepseek-v4.1-flash, GPT-6.1-sol. Names are provisional.
// deepseek-v4.1-flash 2026-10-01 (retry 4, timeboxed): no new gains, stays at
// the 75.2% / 975-byte best. Open sites unchanged: Loop E keeps the pos loads
// hoisted before the field_14281 test and allocates flags in ebx (original:
// ebp) with the cursor in ebp (original: esi); Loop C and Loop D scheduling
// differences as noted below.

// deepseek-v4.1-flash 2026-10-01 (retry 3): Loop E's `unsigned char pi` local
// forced a spill of playerIndex to [esp+0x1c] plus and 0xff reload; changing it
// to `int pi` deletes the spill and gains 74.6 -> 75.2 (975 bytes). Duplicating
// y/x declarations inside both vis arms regressed to 72.5, so the single
// pre-if y/x pair stays.
// PARTIAL STILL OPEN (75.2%): Loop E keeps the pos loads hoisted before the
// field_14281 test and allocates flags in ebx (original: ebp) with the cursor
// in ebp (original: esi); Loop C push/vptr scheduling and Loop D flags-load
// order are unchanged from the notes below.
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

struct Unit {
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

struct MapSize_00467440 {
    unsigned int width;                // +0x0
    unsigned int height;               // +0x4

    int Contains(unsigned int tx, unsigned int ty)
    {
        return tx < width && ty < height;
    }
};

struct ByteMap_00467440 {
    unsigned char* data;               // +0x0
    MapSize_00467440 size;             // +0x4

    unsigned char Get(int x, int y) { return data[size.width * y + x]; }
};

struct PlayerInfo_00467440 {
    void* field_0;                     // +0x0
    char unknown_4[0x27 - 0x4];
    PlayerData_00467440* data;         // +0x27
    char unknown_2b[0x67 - 0x2b];
    Unit* field_67;                    // +0x67
    Unit* field_6b;                    // +0x6b
    char unknown_6f[0x7c - 0x6f];
    ByteMap_00467440 explored;         // +0x7c
    char unknown_88[0x146 - 0x88];
    unsigned char field_146;           // +0x146
    char unknown_147[0x14b - 0x147];
};

struct Game_00467440 {
    char unknown_0[0x1b63];
    PlayerInfo_00467440 players[10];   // +0x1b63, stride 0x14b
    char unknown_2851[0x2a3c - 0x2851];
    unsigned short field_2a3c;         // +0x2a3c
    char unknown_2a3e[0x2a43 - 0x2a3e];
    unsigned char playerIndex;         // +0x2a43
    char unknown_2a44[0x14273 - 0x2a44];
    unsigned short* field_14273;       // +0x14273
    char unknown_14277[0x14281 - 0x14277];
    unsigned char field_14281;         // +0x14281
    char unknown_14282[0x14357 - 0x14282];
    Unit* units;                       // +0x14357
    Unit* units_end;                   // +0x1435b
    char unknown_1435f[0x38a47 - 0x1435f];
    int field_38a47;                   // +0x38a47

    PlayerInfo_00467440* Current() { return &players[playerIndex]; }
};
#pragma pack(pop)

class Class_00467840 {
public:
    virtual void FUN_00467840(Unit* unit);
    int field_4;                       // +0x4
    int field_8;                       // +0x8
    Vec3_00467440 pos;                 // +0xc
};

class Class_00467960 {
public:
    virtual void FUN_00467960(Unit* unit);
};

class Class_00467980 {
public:
    virtual void FUN_00467980(Unit* unit);
};

extern Game_00467440* g_game;

void __stdcall FUN_0047e890(Vec3_00467440* pos, int range, void* visitor);
bool __stdcall FUN_0040b0d0(int player, Vec3_00467440* p, int range);

static inline int IsExplored_00467440(PlayerInfo_00467440* p, UnitPos_00467440* pos)
{
    int tx = pos->half.f6c >> 5;
    int ty = (pos->half.f74 - (pos->half.f70 >> 1)) >> 5;
    if (p->explored.size.Contains(tx, ty) && p->explored.Get(tx, ty) != 0)
        return 1;
    return 0;
}

static inline int IsSeen_00467440(PlayerInfo_00467440* p, UnitPos_00467440* pos)
{
    int tx = pos->half.f6c >> 5;
    int ty = (pos->half.f74 - (pos->half.f70 >> 1)) >> 5;
    if (!p->explored.size.Contains(tx, ty))
        return 0;
    return (g_game->field_14273[p->explored.size.width * ty + tx] &
            (1 << g_game->playerIndex)) != 0;
}

// FUNCTION: 0x467440
void FUN_00467440(void)
{
    if (g_game->field_2a3c < 2) {
        return;
    }
    unsigned char player = g_game->playerIndex;
    Unit* first = g_game->units + 1;
    Unit* last = g_game->units_end;
    PlayerInfo_00467440* pl = (PlayerInfo_00467440*)((char*)g_game + 0x1b63
        + (unsigned int)g_game->playerIndex * 0x14b);
    Unit* u;

    Unit* a;
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
        unsigned char ff;
        if ((u->flags & 0x10000000) && (ff = u->field_ff, 1) && ff != pl->field_146 && (u->field_10e & 1)) {
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
                        int& b0 = u->field_b0;
                        b0 = g_game->field_38a47 + 0x5a;
                        u->flags |= 0x1000;
                    }
                }
            }
        }
    }

    for (u = first; u <= last; u++) {
        unsigned int f = u->flags;
        if ((f & 0x10000000) && !(f & 0x100) && !(u->field_10e & 4)) {
            int pi = g_game->playerIndex;
            PlayerInfo_00467440* p2 = g_game->Current();
            unsigned int vis;
            if ((g_game->field_14281 & 2) == 2) {
                vis = IsExplored_00467440(p2, &u->pos);
            } else {
                vis = IsSeen_00467440(p2, &u->pos);
            }
            if ((int)vis) {
                u->flags = f | 0x100;
            }
        }
    }
}
