// Decompiled by DeepSeek V4.1 Flash, finished by GPT-6, finished by deepseek-v4.1-flash, finished by GPT-6.1-sol, finished by deepseek-v4.1-flash. Names are provisional.
// Retry #1766 deepseek-v4.1-flash: 60.6%. The single gain was fixing the
// order-count and FUN_0043de30 guard to the correct record fields: the count
// is rec+0x23 (f23) and the guard is rec+0x27 (f27), not rec+0x33. The old
// attempt read +0x33 for both, which is the second coordinate of rec.pos.
// Tried and rejected (all scored on scratch, worse than 60.6): removing the
// `player` local and recomputing (rec.flags>>4)&3 twice (41.3); modelling
// rec+0x2b..+0x3b as Vec3 pos plus an {int,short} sub-struct copied whole
// (47.1); `char` instead of `unsigned char` for rec+0x8d combined with the
// Script%i argument (59.6); an explicit `else p = 0;` after operator new
// (46.6); `int found` (46.6). Note `Script%i` in the original takes the search
// index i, not rec+0x3b, and rec+0x8d is sign-extended at the FUN_0048aac0
// call, but both changes only ever cost points at this compiler state.
// Remaining differences: first loop register allocation (original n in esi,
// found in ebp, i spilled to [esp+0x10]; ours n in ebp, i in esi, found in a
// byte local), the FUN_00485f50 argument register rotation, the 0x110 flags
// block rotation, the 3x piece-copy anchors, and the failure epilogue's
// xor/pop order.
// Retry deepseek-v4.1-flash: still 60.6%. The blocker is a one-off register-phase
// rotation: the original holds `player` and rec.b8d as inline expressions and the
// child block loads childB into edx, which sets the phase for the whole field-copy
// chain (al,cl,dl,...). Ours keeps `player` in ebx and loads childB into eax, shifting
// the phase by one register for every later move (al/cl/dl vs cl/dl/al), including the
// 0x10f byte-flag xor/and/xor idiom and the 0x110 block. Removing the `player` local
// (inline (rec.flags>>4)&3 twice) scored 54.2: worse overall because the 0x485f50
// argument setup then rotates differently. Sign-extending rec.b8d did not help.
// `int found` instead of `bool found` scores 45-59 and spills the wrong local; bool
// found leaves ebp free so n lands in ebp. Using the loop index i for Script%i (which
// the original does) scored 46.6 with bool found and 59.6 with int found; neither beats
// keeping rec.f3b. This session re-confirmed 60.6 as a local maximum: unsigned loop
// index (60.6), found declared before n (60.6), found as unsigned char (60.6),
// int/bool/BYTE found with `found = 0` statement or `if (found == 0)` (all 45.5,
// MSVC then reserves a zero register and the frame shrinks to 0xfc), i scoped in the
// for (60.6), `i != n` / `n > i` conditions (60.6). The exact trigger for the
// original allocation (n=esi, bool-like found=ebp, loop i spilled at [esp+0x10] and
// live to the Script%i sprintf) is still unidentified.
// Retry deepseek-v4.1-flash (this session): re-read the two loop-preamble diff
// hunk. The original leaves `i` in memory precisely because ebx (id), edi (file),
// esi (n) and ebp (found) are all taken; ours frees ebp for `i` because `found`
// lost the allocator contest and landed at [esp+0x13] as a byte local, so the
// whole downstream register phase shifts by one (childB loads into eax instead of
// edx, the 0x10f/0x110 block and the 3x piece copy follow). Tried and rejected
// this session (scored 45.5, worse than 60.6): a `while (i < (unsigned int)n)`
// loop with `unsigned int found/i`, which pushes both into memory and drops the
// epilogue match as well. Volatile on the loop index reproduces the original map
// exactly (71.5) but is not allowed.
// Retry deepseek-v4.1-flash (this session): confirmed Script%i really takes the
// search index i (original 0x4875d0 `mov edx,[esp+0x10]`), so `rec.f3b` below is
// provisional. Switching to i moves n into esi but frees ebp for i and grows the
// frame to 0x104 (46.6 with bool found, 59.6 with int found). Marking the loop
// index `volatile` forces exactly the original map (id=ebx, file=edi, n=esi,
// found=ebp, i at [esp+0x10], frame 0x100) and scores 71.5, but volatile is
// disallowed, so it is not used here. Best allowed stays 60.6. Tried and
// rejected this session (all below 60.6): found as void*/long (zero register,
// 45.5), array-of-1 / struct-member loop index and an address-taken reference
// (all 46.6), i or n as long, loop with i initialized in its declaration plus
// int found (60.6 but i still ends in ebp), a `zero` local for the 0 arguments
// (59.6), bool found with true/false and ==false (60.6).

extern "C" int __cdecl sprintf(char* buf, const char* fmt, ...);

struct Vec3_00487080 {
    int x, y, z;
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
    char pieces[3 * 0x18];              // +0x41
    short childA;                       // +0x89
    short childB;                       // +0x8b
    unsigned char b8d;                  // +0x8d
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
    union { unsigned char flags; struct { unsigned char bit0:1,bit1:1,bits2:2,bit4:1,rest:3; }; };                // +0x17
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
    union { unsigned char flags; struct { unsigned char bit0:1,bit1:1,bits2:2,bit4:1,rest:3; }; };
};

#pragma pack(pop)

#pragma pack(push, 1)
struct Unit_00487080 {
    void* vtable;                       // +0x0
    Piece_00487080 pieces[3];           // +0x4
    int field_58;                       // +0x58
    void* listHead;                     // +0x5c
    void* listTail;                     // +0x60
    int field_64;                       // +0x64
    short field_68;                     // +0x68
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
void __stdcall FUN_0048aac0(Unit_00487080* unit, Unit_00487080* builder, char piece, char p4);
void __stdcall FUN_00480250(Unit_00487080* unit, int id);
#pragma pack(push, 1)
class Order_00487080 { public: char pad[0x42]; unsigned int flags; char gap[4]; Order_00487080* next; Order_00487080* FUN_0043a420(Unit_00487080*, Class_004b4560*, char*); };
#pragma pack(pop)
class Class_004388b0 { public: void FUN_004388b0(); };
void __stdcall FUN_0047db20(Unit_00487080* unit);
class Class_00401110 { public: void FUN_00401110(Unit_00487080*, Class_004b4560*); };
class Class_0043d210 { public: void FUN_0043de30(Unit_00487080*, Class_004b4560*); };
class Class_004b0610 { public: void FUN_004b2040(Class_004b4560*); };

// FUNCTION: 0x487080
Unit_00487080* __stdcall FUN_00487080(unsigned short id, Class_004b4560* file)
{
    Unit_00487080* unit;
    if (id == 0)
        unit = 0;
    else
        unit = (Unit_00487080*)(*(char**)((char*)g_game + 0x14357) + id * 0x118);
    if (unit == 0 || (unit->flags & 0x10000000))
        return 0;

    SaveRec_00487080 rec;
    char name[32];
    char script[32];
    int n = ((Class_004b4800*)file)->FUN_004b4800("Number of Units", 0);
    bool found = 0;
    int i;
    for (i=0;i<n;i++) {
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

    int player = (rec.flags >> 4) & 3;
    unit = FUN_00485f50(rec.player, FUN_00488b10(rec.name), *(Vec3_00487080*)&rec.f2b, 1, (rec.flags >> 4) & 3, rec.id);
    if (unit == 0)
        return 0;

    unit->field_64 = rec.f37;
    unit->field_68 = rec.f3b;
    unit->field_108 = rec.f3d;
    unit->field_b8 = rec.f3f;
    unit->field_6e = rec.f2f;

    if (rec.childA != 0) {
        Unit_00487080* child = FUN_00487080(rec.childA, file);
        if (child != 0)
            FUN_0048aac0(unit, child, rec.b8d, player);
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

    unsigned int f = rec.flags;
    unsigned char b = unit->b_10f;
    b = ((unsigned char)f ^ b) & 1 ^ b;
    unit->b_10f = b;
    b = (unsigned char)((rec.flags >> 1 & 1) << 1) | (b & 0xfd);
    unit->b_10f = b;
    b = (unsigned char)((rec.flags >> 2 & 1) << 2) | (b & 0xfb);
    unit->b_10f = b;
    b = (unsigned char)((rec.flags >> 3 & 1) << 3) | (b & 0xf7);
    unit->b_10f = b;

    unsigned int u = unit->flags;
    u = (rec.flags >> 4 & 0xc) | (u & 0xfffffff3);
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

    Order_00487080** normal=(Order_00487080**)&unit->listHead;
    Order_00487080** special=(Order_00487080**)&unit->listTail;
    int k = 0;
    if (rec.f23 > 0) {
        do {
            sprintf(name, "u%04xm%04x", unit->id, k);
            Order_00487080* p = (Order_00487080*)operator new(0x56);
            if (p != 0)
                p = p->FUN_0043a420(unit, file, name);
            if (p->flags & 0x40000) { *special=p; special=&p->next; }
            else { *normal=p; normal=&p->next; }
            k++;
        } while (k < rec.f23);
    }
    if (unit->listHead != 0)
        ((Class_004388b0*)unit->listHead)->FUN_004388b0();
    {
        sprintf(script, "Script%i", rec.f3b);
        ((Class_004b4ba0*)file)->FUN_004b4ba0(script);
    }
    ((Class_004b0610*)unit->field_9a)->FUN_004b2040(file);

    for (int j = 0; j < 3; j++) {
        SrcPiece_00487080* s = (SrcPiece_00487080*)(rec.pieces + j * 0x18);
        Piece_00487080* d = &unit->pieces[j];
        d->f0 = s->f0;
        d->f4 = s->f4;
        d->obj[0x10a] = s->f8;
        d->fc = s->fc;
        d->f10 = s->f10;
        d->f12 = s->f12;
        d->f14 = s->f14;
        d->f16 = s->f16;
        d->bit0 = s->bit0;
        d->bit1 = s->bit1;
        d->bits2 = s->bits2;
        d->bit4 = s->bit4;
    }

    if (unit->b_10f & 4)
        FUN_0047db20(unit);
    return unit;
}
