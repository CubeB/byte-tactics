// Decompiled by DeepSeek V4.1 Flash, finished by GPT-6, finished by deepseek-v4.1-flash, finished by GPT-6.1-sol, finished by deepseek-v4.1-flash, finished by claude-sonnet-5-5, finished by Space Bunny Free, finished by claude-sonnet-5-5, finished by DeepSeek V4.1 Flash, finished by claude-opus-5-5. Names are provisional.
// Loads one unit (and, recursively, the units it carries or is built by) from the "Units"
// section of a saved game: finds its 0xb8-byte record by id, creates the unit and copies the
// record into it.
//
// Pass 16 (claude-opus-5-5): 88.8 -> 91.9 percent, 1595 bytes (the original's size). Not a MATCH.
//  - The piece copy loop is now plain array indexing, `unit->pieces[j].f0 = rec.pieces[j].f0;`
//    and so on in the original order (f0, f4, obj, fc, ...), with SaveRec.pieces typed as an
//    array of SrcPiece. That fixes the whole loop, anchors included (+3.1). The rule behind
//    it: MSVC anchors the strength-reduced pointer on the second distinct non-zero offset the
//    loop body uses. With `d = &unit->pieces[j]` pointers, f0 sits at offset 0 and does not
//    count, so the anchor moves to obj; indexed off `unit`, f0 is +4 and the anchor lands on
//    f4 as in the original.
//  - The piece flags are now real bitfields (`unit->pieces[j].fl.b0 = rec.pieces[j].fl.b0;`).
//    They compile to the same xor/and/xor code as the old explicit masks.
//  Still differs (register allocation only, the code shape is identical):
//  (1) the 0x10f merge: the getter's two loads leave a hoisted `mov bl,[esi+0x10f]` and a
//    d-left `xor al,[esp+0xcc]` where the original has `mov cl,[esp+0xcc]; xor cl,al`.
//    Tried this pass, all lower or identical: bitfield copies (from rec bitfields, from
//    `rec.flags & 1`, from `rec.flags >> n`) 81.8 to 83.3; a `b` local for the four steps
//    86.9; a `(unsigned char)(rec.flags & 1)`, int or 32-bit source term, `|`/`& 0xfe` forms
//    and `^=` (identical or 83.3). In a small test file the bitfield copies give exactly the
//    original's s-left xor and src-left `or` rotation, so the original was most likely
//    bitfields, and the residual is the allocator's context.
//  (2) the 0x110 chain: `xor` for `or` at step 1 (the `hi` hoist), then edx at step 3 and a
//    dst-left `or` at step 5 where the original has them at step 6, and no reload of
//    unit->flags after the field_b0 store. Bitfields (75.7), reading unit->flags in every
//    statement (75.5 to 76.2), a reference or pointer to unit->flags (77.7), re-reading u
//    after field_b0 (81.3, MSVC forwards it without a load) and other spellings of the first
//    two steps were all lower.
//  - The real preceding function, 0x486fd0, defined above this one changes nothing.
//  - A 15-minute permuter run on this file (15k candidates) only found declaration moves
//    (rec, i and k) that lower its fine score but leave the percentage at 91.9.
// Earlier passes (condensed):
//  - Pass 13: GetB10F_00487080 (a one-line inline getter) keeps statements 2 to 4 of the 0x10f
//    merge byte for byte; `unsigned int hi = rec.flags >> 4;` used by the first 0x110 step only
//    lines up steps 1 and 2 (removing it loses 2.4).
//  - Pass 14: the failure paths fall out of `if (unit != 0) { ... return unit; } return 0;`
//    so the final `xor eax, eax` is the last block, as in the original.
//  - A permuter lead from pass 14 (not plausible source): a `do { } while (0)` around the 0xc0
//    and 0x100 steps of the 0x110 chain was worth 3.3 points on the old file.
extern "C" int __cdecl sprintf(char* buf, const char* fmt, ...);


struct Vec3_00487080 {
    int x, y, z;
};

struct PieceBits_00487080 {
    unsigned char b0 : 1;
    unsigned char b1 : 1;
    unsigned char b23 : 2;
    unsigned char b4 : 1;
    unsigned char b5 : 3;
};

struct SrcPiece_00487080 {              // 0x18 bytes at +0x41 + i*0x18
    int f0;                             // +0x0
    int f4;                             // +0x4
    unsigned char f8;                   // +0x8
    char gap_9[3];
    int fc;                             // +0xc
    short f10;                          // +0x10
    short f12;                          // +0x12
    short f14;                          // +0x14
    unsigned char f16;                  // +0x16
    PieceBits_00487080 fl;              // +0x17
};
#pragma pack(push, 1)
// 0xb8-byte save record. Name at +0x0, id at +0x21 (proven by the
// `cmp word ptr [esp+0x39], bx` against the record base at esp+0x18).
struct SaveRec_00487080 {
    char name[0x20];
    unsigned char player;                    // +0x0
    unsigned short id;                  // +0x21
    int f23;                            // +0x23
    int f27;                            // +0x27
    int f2b;                            // +0x2b
    int f2f;                            // +0x2f
    int f33;                            // +0x33
    int f37;                            // +0x37
    short f3b;                          // +0x3b
    short f3d;                          // +0x3d
    short f3f;                          // +0x3f
    SrcPiece_00487080 pieces[3];        // +0x41
    short childA;                       // +0x89
    short childB;                       // +0x8b
    char b8d;                  // +0x8d
    unsigned char b8e;                  // +0x8e
    int f8f;                            // +0x8f
    int f93;                            // +0x93
    int f97;                            // +0x97
    int f9b;                            // +0x9b
    int f9f;                            // +0x9f
    int fa3;                            // +0xa3
    int fa7;                            // +0xa7
    unsigned char bab;                  // +0xab
    unsigned char bac;                  // +0xac
    unsigned char bad;                  // +0xad
    short bae;                          // +0xae
    unsigned char bb0;                  // +0xb0
    unsigned char bb1;                  // +0xb1
    unsigned char bb2;                  // +0xb2
    unsigned char b3;                   // +0xb3 (unused padding)
    unsigned int flags;                 // +0xb4
};


struct Piece_00487080 {                 // 0x1c bytes at +0x4 + i*0x1c
    int f0;
    int unused;
    int f4;
    unsigned char* obj;
    int fc;
    short f10;
    short f12;
    short f14;
    unsigned char f16;
    PieceBits_00487080 fl;              // +0x1f
};

#pragma pack(pop)

#pragma pack(push, 1)
struct Pair_00487080 {
    int a;
    short b;
    void Set(int x, short y)
    {
        a = x;
        b = y;
    }
};
#pragma pack(pop)

#pragma pack(push, 1)
struct Unit_00487080 {
    void* vtable;                       // +0x0
    Piece_00487080 pieces[3];           // +0x4
    int field_58;                       // +0x58
    void* listHead;                     // +0x5c
    void* listTail;                     // +0x60
    Pair_00487080 field_64;             // +0x64 (int + short)
    char gap_6a[4];
    int field_6e;                       // +0x6e
    char gap_72[4];
    int field_76;                       // +0x76
    int field_7a;                       // +0x7a
    int field_7e;                       // +0x7e
    char gap_82[0x18];
    void* field_9a;                     // +0x9a
    char gap_9e[0xa];
    unsigned short id;
    char gap_aa[2];
    int field_ac;                       // +0xac
    int field_b0;                       // +0xb0
    char gap_b4[4];
    short field_b8;                     // +0xb8
    short field_ba;                     // +0xba
    char info[0x34];                    // +0xbc
    Unit_00487080* child;               // +0xf0
    unsigned char b_f4;
    unsigned char b_f5;
    unsigned char b_f6;
    unsigned char b_f7;
    unsigned char b_f8;
    unsigned char b_f9;
    unsigned char b_fa;
    char gap_fb[9];
    int field_104;                      // +0x104
    short field_108;                    // +0x108
    char gap_10a[4];
    unsigned char b_10e;
    unsigned char b_10f;
    unsigned int flags;                 // +0x110
    char gap_114[4];
};

#pragma pack(pop)
class Class_004b4560 {
public:
    int FUN_004b4560(char* name);
};
class Class_004b4800 {
public:
    int FUN_004b4800(char* name, int def);
};
class Class_004b4b50 {
public:
    int FUN_004b4b50(int a);
};
class Class_004b4c10 {
public:
    void FUN_004b4c10(int pos);
};
class Class_004b4c80 {
public:
    int FUN_004b4c80(void* buf, int len);
};
class Class_004b4ba0 {
public:
    int FUN_004b4ba0(char* name);
};

extern void* g_game;


unsigned short __stdcall FUN_00488b10(const char* name);
Unit_00487080* __stdcall FUN_00485f50(unsigned char player, unsigned short typeId,
                                      Vec3_00487080 pos, int param_5, int mode,
                                      unsigned short id);
void __stdcall FUN_0048aac0(Unit_00487080* unit, Unit_00487080* builder, int piece, int p4);
void __stdcall FUN_00480250(Unit_00487080* unit, int id);
#pragma pack(push, 1)
class Order_00487080 { public: char pad[0x42]; unsigned int flags; char gap[4]; Order_00487080* next; Order_00487080* FUN_0043a420(Unit_00487080*, Class_004b4560*, char*); };
#pragma pack(pop)
class Class_004388b0 { public: void FUN_004388b0(); };
void __stdcall FUN_0047db20(Unit_00487080* unit);
class Class_00401110 { public: void FUN_00401110(Unit_00487080*, Class_004b4560*); };
class Class_0043d210 { public: void FUN_0043de30(Unit_00487080*, Class_004b4560*); };
class Class_004b0610 { public: void FUN_004b2040(Class_004b4560*); };

// Reads the unit's saved-byte flag field. Called only so that the inlined load
// stays where the original has it, just after the three byte stores above it.
static inline unsigned char GetB10F_00487080(Unit_00487080* u) { return u->b_10f; }

// FUNCTION: 0x487080
Unit_00487080* __stdcall FUN_00487080(unsigned short id, Class_004b4560* file)
{
    Unit_00487080* unit;
    if (id == 0)
        unit = 0;
    else
        unit = (Unit_00487080*)(*(char**)((char*)g_game + 0x14357) + id * 0x118);
    if (unit == 0 || (unit->flags & 0x10000000))
        return unit;

    SaveRec_00487080 rec;
    char name[32];
    char script[32];
    int n = ((Class_004b4800*)file)->FUN_004b4800("Number of Units", 0);
    int found = 0;
    int i;
    for (i = 0; i < n; i++) {
        if (!((Class_004b4b50*)file)->FUN_004b4b50(i))
            return 0;
        ((Class_004b4c10*)file)->FUN_004b4c10(0);
        if (((Class_004b4c80*)file)->FUN_004b4c80(&rec, 0xb8) != 0xb8)
            return 0;
        if (rec.id == id) {
            found = 1;
            break;
        }
    }
    if (!found)
        return 0;

    unit = FUN_00485f50(rec.player, FUN_00488b10(rec.name), *(Vec3_00487080*)&rec.f2b, 1, (rec.flags >> 4) & 3, rec.id);
    if (unit != 0) {

    unit->field_64 = *(Pair_00487080*)&rec.f37;
    unit->field_108 = rec.f3d;
    unit->field_b8 = rec.f3f;
    unit->field_6e = rec.f2f;

    if (rec.childA != 0) {
        Unit_00487080* child = FUN_00487080(rec.childA, file);
        if (child != 0)
            FUN_0048aac0(unit, child, rec.b8d, (rec.flags >> 4) & 3);
    }
    unit->child = FUN_00487080(rec.childB, file);
    unit->b_f9 = rec.b8d;
    unit->b_f4 = rec.b8e;
    unit->field_58 = rec.f8f;
    unit->field_76 = rec.f93;
    unit->field_7a = rec.f97;
    unit->field_7e = rec.f9b;
    unit->field_ac = rec.f9f;
    FUN_00480250(unit, rec.f9f);
    unit->field_104 = rec.fa7;
    unit->b_f5 = rec.bab;
    unit->b_f6 = rec.bac;
    unit->b_f7 = rec.bad;
    unit->field_ba = rec.bae;
    unit->b_f8 = rec.bb0;
    unit->b_fa = rec.bb1;
    unit->b_10e = rec.bb2;

    unit->b_10f = ((unsigned char)rec.flags ^ GetB10F_00487080(unit)) & 1
        ^ GetB10F_00487080(unit);
    unit->b_10f = ((rec.flags >> 1 & 1) << 1) | (GetB10F_00487080(unit) & 0xfd);
    unit->b_10f = ((rec.flags >> 2 & 1) << 2) | (GetB10F_00487080(unit) & 0xfb);
    unit->b_10f = ((rec.flags >> 3 & 1) << 3) | (GetB10F_00487080(unit) & 0xf7);

    unsigned int hi = rec.flags >> 4;
    unsigned int u = unit->flags;
    u = (hi & 0xc) | (u & 0xfffffff3);
    unit->flags = u;
    u = (rec.flags >> 4 & 0x10) | (u & 0xffffffef);
    unit->flags = u;
    u = (rec.flags >> 4 & 0x20) | (u & 0xffffffdf);
    unit->flags = u;
    u = (rec.flags >> 4 & 0xc0) | (u & 0xffffff3f);
    unit->flags = u;
    u = (rec.flags >> 4 & 0x100) | (u & 0xfffffeff);
    unit->flags = u;
    u = (rec.flags >> 4 & 0x200) | (u & 0xfffffdff);
    unit->flags = u;
    u = (rec.flags >> 4 & 0x400) | (u & 0xfffffbff);
    unit->flags = u;
    u = (rec.flags >> 4 & 0x800) | (u & 0xfffff7ff);
    unit->flags = u;
    unit->field_b0 = rec.fa3;
    u = (rec.flags >> 3 & 0x2000) | (u & 0xffffdfff);
    unit->flags = u;
    u = (rec.flags >> 6 & 0x4000) | (u & 0xffffbfff);
    unit->flags = u;
    u = (rec.flags >> 6 & 0x8000) | (u & 0xffff7fff);
    unit->flags = u;
    u = (rec.flags >> 6 & 0x10000) | (u & 0xfffeffff);
    unit->flags = u;
    u = (rec.flags >> 6 & 0x20000) | (u & 0xfffdffff);
    unit->flags = u;
    u = (rec.flags >> 6 & 0xc0000) | (u & 0xfff3ffff);
    unit->flags = u;
    u = (rec.flags >> 6 & 0x300000) | (u & 0xffcfffff);
    unit->flags = u;
    u = (rec.flags >> 6 & 0x400000) | (u & 0xffbfffff);
    unit->flags = u;
    u = (rec.flags >> 6 & 0x3800000) | (u & 0xfc7fffff);
    unit->flags = u;

    ((Class_00401110*)&unit->info)->FUN_00401110(unit, file);
    if (rec.f27 != 0)
        ((Class_0043d210*)unit->vtable)->FUN_0043de30(unit, file);

    Order_00487080** normal = (Order_00487080**)&unit->listHead;
    Order_00487080** special = (Order_00487080**)&unit->listTail;
    int k = 0;
    if (rec.f23 > 0) {
        do {
            sprintf(name, "u%04xm%04x", unit->id, k);
            Order_00487080* p = (Order_00487080*)operator new(0x56);
            p = p ? p->FUN_0043a420(unit, file, name) : 0;
            if (p->flags & 0x40000) {
                *special = p;
                special = &p->next;
            } else {
                *normal = p;
                normal = &p->next;
            }
            k++;
        } while (k < rec.f23);
    }
    if (unit->listHead != 0)
        ((Class_004388b0*)unit->listHead)->FUN_004388b0();
    sprintf(script, "Script%i", i);
    ((Class_004b4ba0*)file)->FUN_004b4ba0(script);
    ((Class_004b0610*)unit->field_9a)->FUN_004b2040(file);

    for (int j = 0; j < 3; j++) {
        unit->pieces[j].f0 = rec.pieces[j].f0;
        unit->pieces[j].f4 = rec.pieces[j].f4;
        unit->pieces[j].obj[0x10a] = rec.pieces[j].f8;
        unit->pieces[j].fc = rec.pieces[j].fc;
        unit->pieces[j].f10 = rec.pieces[j].f10;
        unit->pieces[j].f12 = rec.pieces[j].f12;
        unit->pieces[j].f14 = rec.pieces[j].f14;
        unit->pieces[j].f16 = rec.pieces[j].f16;
        unit->pieces[j].fl.b0 = rec.pieces[j].fl.b0;
        unit->pieces[j].fl.b1 = rec.pieces[j].fl.b1;
        unit->pieces[j].fl.b23 = rec.pieces[j].fl.b23;
        unit->pieces[j].fl.b4 = rec.pieces[j].fl.b4;
    }


    if (unit->b_10f & 4)
        FUN_0047db20(unit);
    return unit;
    }
    return 0;
}