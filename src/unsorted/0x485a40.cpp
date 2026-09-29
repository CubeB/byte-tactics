// Decompiled by deepseek-v4.1-flash, finished by space-bunny-free. Names are provisional.
// Initialises a freshly placed unit from its type definition: looks the type up
// by the unit's id, mirrors the type's flags into the unit's +0x110 word, sets
// build progress / health (param_5 selects the finished or under-construction
// arm), copies the position and derives a screen-space short pair from the
// unit's +0x14a offset pair, gives the unit a random facing, marks bit 9 from
// the owning player, resets its three weapon entries, and finally resets the
// PlayerRef at +0xbc and runs FUN_00480250.
//
// PARTIAL (85.9%). Every instruction matches except the register allocator's
// choice for the first flags value:
//   - the original keeps the first flags read-modify-write in EAX: it finishes
//     the type index chain in EAX first and then reuses EAX, and it never
//     touches EBP. In my version MSVC hoists the load of unit->+0x110 above the
//     index chain, so the value is live across it, gets demoted to EBP, and
//     every [esp+X] offset is then 4 higher with a push/pop ebp at each end.
//     This is one allocator state, not several bugs: everything else in the
//     function (all the +0x110 bitfield read-modify-write masks, the +0x114
//     bit, the nibble clear at +0x10f, the position copy through the esi+0x6a
//     base pointer, the stack scratch pair, both FUN_004b6c30 calls and the
//     whole tail) is byte exact.
//   - The two spellings of the position copy trade those halves against each
//     other and neither gives both of them:
//       `unit->pos = pos;`        -> the base pointer, early block wrong
//                                    (this file, 85.9%)
//       three separate field stores -> the first 58 instructions byte exact,
//                                    but the stores use disp32 addressing and
//                                    the screen arithmetic interleaves with
//                                    them (84.1%)
//     A pointer local to &unit->pos, a nested block, a local struct copy, an
//     inline helper around the screen expression, an `|=` spelling of the
//     first flag, a separate `unsigned short tid` and a pointer-arithmetic
//     index all land on one side or the other (71% to 86%), never on both.
//   - FIXED: the random facing needs `unsigned short field_66`, not `short`.
//     The narrowing of the constant in `0x8000 - field_210 / 2` is driven by
//     the destination type: as a signed short MSVC folds 0x8000 to 0xffff8000,
//     as an unsigned short it keeps 0x8000 and still emits the original's
//     16-bit `shr dx, 1`. Every spelling around it (the cast, unsigned,
//     `>> 1`, `0x10000 / 2`, a separate int local, reassociation) gives one of
//     the two or the other, never both.
// <windows.h>/<stdio.h>/<string.h>/<math.h> and the other 124 header sets do
// not change any of this.
// Re-attempted (deepseek-v4.1-flash): an explicit
// `unsigned int flags = unit->flags.all | 0x10000000;` local with the type
// store between the load and the store (49.6%), binding `unit->type` through
// `UnitType_485a40*&` and assigning through the reference (85.6%), and putting
// the flags RMW before the type assignment (54.4%). MSVC keeps hoisting the
// flags load into a callee-saved register in all of them; 85.9% remains best.
//
// The type's +0x241 word is a bitfield union; its movOrder/fireOrder/canAttack/
// b7/b9/hi fields are the ones copied into the unit. The unit's +0x110 is a
// bitfield union too: b0/b5/b16 are set and b1-b11/b17 cleared by the
// init, b14/b29 by the type's +0x22f test, mode2/mode from movOrder/fireOrder,
// b11 from canAttack, b30 from b9, b31 from the type's high half,
// f22_23 = 3 and f24_25 = 0 when the type's +0x22e is above 1, and b8/b9 from
// the owner comparison. Writing these as explicit masks gives the same bytes;
// the bitfield form is the one that produced the original's merged clear
// masks. Note the screen pair's y is derived from the third component of the
// passed position, not the second.

#pragma pack(push, 1)

struct ShortPair_485a40 {
    short x;                           // +0x0
    short y;                           // +0x2
};

union TypeFlags_485a40 {               // the type's word at +0x241
    unsigned int all;
    struct {
        unsigned int movOrder : 2;     // bits 0-1
        unsigned int fireOrder : 2;    // bits 2-3
        unsigned int canAttack : 1;    // bit 4
        unsigned int b5 : 1;
        unsigned int b6 : 1;
        unsigned int b7 : 1;           // bit 7
        unsigned int b8 : 1;
        unsigned int b9 : 1;           // bit 9
        unsigned int b10 : 1;
        unsigned int b11 : 1;
        unsigned int b12 : 1;
        unsigned int b13 : 1;
        unsigned int b14 : 1;
        unsigned int b15 : 1;
        unsigned int hi : 16;          // bits 16-31
    } bits;
};

struct UnitType_485a40 {
    char unknown_0[0x14a];
    ShortPair_485a40 offset;           // +0x14a
    char unknown_14e[0x1fa - 0x14e];
    short field_1fa;                   // +0x1fa
    char unknown_1fc[0x210 - 0x1fc];
    unsigned short field_210;          // +0x210
    char unknown_212[0x22e - 0x212];
    unsigned char field_22e;           // +0x22e
    unsigned char field_22f;           // +0x22f
    char unknown_230[0x241 - 0x230];
    TypeFlags_485a40 field_241;        // +0x241
    char unknown_245[0x249 - 0x245];
};

struct Player_485a40 {
    char unknown_0[0x146];
    unsigned char field_146;           // +0x146
};

class PlayerRef {
public:
    int unknown[12];
    void* player;
    void Reset(unsigned char playerIndex);
};

class Class_0047cb00 {
public:
    char unknown_0[6];
    void* head;
    void FUN_0047cb00(void* node);
};

class Class_00489800 {
public:
    void FUN_00489800(unsigned char index);
};

class Class_0043dc00;

struct Pos_485a40 {
    int x, y, z;
};

union Flags_485a40 {
    unsigned int all;
    struct {
        unsigned int b0 : 1;
        unsigned int b1 : 1;
        unsigned int b2 : 1;
        unsigned int b3 : 1;
        unsigned int b4 : 1;
        unsigned int b5 : 1;
        unsigned int b6 : 1;
        unsigned int b7 : 1;
        unsigned int b8 : 1;
        unsigned int b9 : 1;
        unsigned int b10 : 1;
        unsigned int b11 : 1;
        unsigned int b12 : 1;
        unsigned int b13 : 1;
        unsigned int b14 : 1;
        unsigned int b15 : 1;
        unsigned int b16 : 1;
        unsigned int b17 : 1;
        unsigned int mode2 : 2;        // bits 18-19
        unsigned int mode : 2;         // bits 20-21
        unsigned int f22_23 : 2;       // bits 22-23
        unsigned int f24_25 : 2;       // bits 24-25
        unsigned int f26_27 : 2;       // bits 26-27
        unsigned int b28 : 1;
        unsigned int b29 : 1;
        unsigned int b30 : 1;
        unsigned int b31 : 1;
    } bits;
};

union Flags114_485a40 {
    unsigned int all;
    struct {
        unsigned int b0 : 1;
    } bits;
};

struct Unit_485a40 {
    Class_0043dc00* obj;               // +0x0
    char unknown_4[0x64 - 0x4];
    short field_64;                    // +0x64
    unsigned short field_66;           // +0x66
    short field_68;                    // +0x68
    Pos_485a40 pos;                    // +0x6a
    ShortPair_485a40 screen;           // +0x76
    short field_7a;                    // +0x7a
    short field_7c;                    // +0x7c
    ShortPair_485a40 offset;           // +0x7e
    Class_0047cb00* list;              // +0x82
    Unit_485a40* owner;                // +0x86
    Unit_485a40* first;                // +0x8a
    Unit_485a40* next;                 // +0x8e
    UnitType_485a40* type;             // +0x92
    Player_485a40* player;             // +0x96
    char unknown_9a[0xa6 - 0x9a];
    short id;                          // +0xa6
    char unknown_a8[0xaa - 0xa8];
    short field_aa;                    // +0xaa
    char unknown_ac[0xb0 - 0xac];
    int field_b0;                      // +0xb0
    char unknown_b4[0xb8 - 0xb4];
    short field_b8;                    // +0xb8
    short field_ba;                    // +0xba
    PlayerRef playerRef;               // +0xbc
    int field_f0;                      // +0xf0
    unsigned char field_f4;            // +0xf4
    char unknown_f5[1];
    unsigned char field_f6;            // +0xf6
    unsigned char field_f7;            // +0xf7
    unsigned char field_f8;            // +0xf8
    unsigned char field_f9;            // +0xf9
    unsigned char field_fa;            // +0xfa
    int field_fb;                      // +0xfb
    unsigned char field_ff;            // +0xff
    int field_100;                     // +0x100
    float field_104;                   // +0x104
    short field_108;                   // +0x108
    char unknown_10a[0x10e - 0x10a];
    unsigned char field_10e;           // +0x10e
    struct {
        unsigned char lo : 4;          // +0x10f
        unsigned char hi : 4;
    } field_10f;
    Flags_485a40 flags;                // +0x110
    Flags114_485a40 field_114;         // +0x114
};

struct Game_485a40 {
    char unknown_0[0x2a43];
    unsigned char field_2a43;          // +0x2a43
    char unknown_2a44[0x1439b - 0x2a44];
    UnitType_485a40* unitTypes;        // +0x1439b
};
#pragma pack(pop)

extern Game_485a40* g_game;

void __stdcall FUN_0048a160(Unit_485a40* unit, int index);
void __stdcall FUN_00480250(Unit_485a40* unit, int param_2);
int __stdcall FUN_004b6c30(int range);

// FUNCTION: 0x485a40
void __stdcall FUN_00485a40(Unit_485a40* unit, Pos_485a40 pos, int param_5)
{
    unit->type = &g_game->unitTypes[(unsigned short)unit->id];
    unit->flags.bits.b28 = 1;
    unit->flags.bits.b29 = (unit->type->field_22f == 0);
    unit->flags.bits.b14 = 0;
    unit->offset = unit->type->offset;
    unit->flags.bits.b31 = unit->type->field_241.bits.hi;
    unit->field_114.bits.b0 = unit->type->field_241.bits.b7;
    unit->flags.bits.b30 = unit->type->field_241.bits.b9;

    if (param_5) {
        unit->field_104 = 0;
        unit->field_108 = unit->type->field_1fa;
    } else {
        unit->field_104 = 1.0f;
        unit->field_100 = 0;
        unit->field_108 = 0;
    }

    unit->field_10f.lo = 0;
    unit->flags.bits.b0 = 1;
    unit->flags.bits.b1 = 0;
    unit->flags.bits.b2 = 0;
    unit->flags.bits.b3 = 0;
    unit->flags.bits.b4 = 0;
    unit->flags.bits.b5 = 1;
    unit->flags.bits.b10 = 0;
    unit->flags.bits.b11 = 0;
    unit->flags.bits.b16 = 1;
    unit->flags.bits.b17 = 0;
    unit->field_f6 = 0;
    unit->field_f7 = 0;
    unit->field_10e = 0;
    unit->field_b0 = 0;
    unit->field_68 = 0;
    unit->pos = pos;

    ShortPair_485a40 off = unit->offset;
    ShortPair_485a40 screen;
    screen.x = (short)((pos.x - off.x * 0x80000 + 0x80000) >> 20);
    screen.y = (short)((pos.y - off.y * 0x80000 + 0x80000) >> 20);
    unit->screen = screen;

    unit->field_66 = (short)(FUN_004b6c30(unit->type->field_210)
                             + (0x8000 - unit->type->field_210 / 2));
    unit->field_64 = 0;
    unit->field_7a = 0;
    unit->field_7c = 0;
    unit->field_fa = 0;
    unit->field_fb = 0;
    unit->flags.bits.b9 = (unit->player->field_146 == g_game->field_2a43);
    unit->flags.bits.b8 = 0;

    for (int i = 0; i < 3; i++) {
        FUN_0048a160(unit, i);
        ((Class_00489800*)unit)->FUN_00489800(i);
    }

    unit->field_ba = 0;
    unit->field_b8 = 0;
    unit->field_f0 = 0;
    unit->field_f4 = 0xa;
    unit->flags.bits.mode2 = unit->type->field_241.bits.movOrder;
    unit->flags.bits.mode = unit->type->field_241.bits.fireOrder;
    unit->flags.bits.b11 = unit->type->field_241.bits.canAttack;
    unit->flags.bits.f26_27 = 0;
    if (unit->type->field_22e > 1) {
        unit->flags.bits.f22_23 = 3;
        unit->flags.bits.f24_25 = 0;
    } else {
        unit->flags.bits.f22_23 = 0;
        unit->flags.bits.f24_25 = 0;
    }

    unit->field_f8 = 0;
    unit->playerRef.Reset(unit->field_ff);
    unit->field_f9 = 0xff;
    unit->field_aa = (short)FUN_004b6c30(0x10000);
    FUN_00480250(unit, 0);
}
