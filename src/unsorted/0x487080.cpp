// Decompiled by DeepSeek V4.1 Flash, finished by GPT-6, finished by deepseek-v4.1-flash, finished by GPT-6.1-sol, finished by deepseek-v4.1-flash, finished by claude-sonnet-5-5, finished by Space Bunny Free, finished by claude-sonnet-5-5. Names are provisional.
// Pass 13 (Space Bunny Free): 83.7 -> 88.6 percent / 1596 bytes (original 1595). Not a MATCH.
// What moved it (free-scored variants, all kept under build/scratch/0x487080/):
//  - GetB10F_00487080(unit), a one-line static inline getter for the byte flag field, read
//    through in the four statements of the 0x10f merge. The function boundary stops MSVC
//    hoisting the load of `unit->b_10f` above the three byte stores just before it, and
//    statements 2, 3 and 4 then match byte for byte: the shift terms land in eax, ecx and
//    edx in turn and the three masks stay separate 0xfd/0xfb/0xf7 (85.6 percent).
//  - the piece copy loop's flag merge written on the plain `flags` byte instead of on the
//    bitfields: `d->flags = ((unsigned char)s->flags ^ d->flags) & 1 ^ d->flags;` then three
//    `((s->flags >> N & M) << N) | (d->flags & clear)` steps. That is what makes MSVC load
//    s->flags into bl and copy dl into cl for the first step, as the original does. The third
//    step has to move the two bits together (>> 2 & 3, clear 0xf3) and the fourth shift by 4
//    with clear 0xef, not shift by 3 with clear 0xf7 (86.0, then 86.2 percent).
//  - `unsigned int hi = rec.flags >> 4;` before the 0x110 chain, used by its first step only
//    (88.6 percent). Both operands of that step are then known disjoint, so MSVC emits
//    `xor eax, ecx` where the original has `or eax, ecx`; that one instruction is the only
//    thing this step still costs, and the shift term, the mask and the store all line up.
//    Letting `hi` feed a second step drops back to 83.7.
//  Tried and rejected here: writing either merge as one static inline helper taking the byte
//    by value or a `unsigned char*` (78.9 / 80.8 / 81.0), reading the fields directly with no
//    getter (70.1), a pointer-returning getter (81.3), a local `u`/`b` for the 0x110 chain read
//    as `unit->flags` (72.4) or seeded from a getter (85.6, no change), and swapping the
//    operands of the first `^` or of the 0x110 `|` (no change at all: MSVC normalises both).
// Pass 14 (claude-sonnet-5-5): 88.6 -> 88.8 percent. BLOCK LAYOUT fixed the epilogue: the
//  first exit is `return unit` (the already-active unit, or null) and the failure paths fall out
//  of nested `if (unit != 0) { ... return unit; } return 0;`, so the final `return 0` is the LAST
//  block of the function (xor eax,eax then the shared pops) and the early `return unit` branches
//  jump into the pops after it, exactly like the original. The old `return 0` + early `return 0`
//  form made MSVC fold the xor into the epilogue and schedule it after pop ebp.
// Still differs:
//  Pass 14 experiments that did NOT help (kept in build/scratch/0x487080/n1b.cpp, n3.cpp, f2.cpp):
//   - real bitfield unions (rec.flags as a 32-bit uint bitfield view, unit->flags and b_10f as
//     bitfield views, one plain `unit->x4 = rec.c8;` per bit): the 0x10f statements 2-4 and the
//     `or` (not xor) come out right, but MSVC puts the destination part on the LEFT of the final
//     `or` (result in the dst register, `and al,0xf3` narrowed) where the original puts the source
//     term on the left (`shr eax,4; and eax,0xc; and ecx,0xfffffff3; or eax,ecx`), so the
//     alternating eax/ecx rotation of the original is lost: 72 percent. The explicit
//     `(term) | (u & ~mask)` form in this file is therefore the right shape.
//   - a 16-bit bitfield container over b_10e/b_10f (to explain why the load of b_10f is not
//     hoisted above the b_10e store): MSVC then does 16-bit read-modify-write (70 percent).
//   - `unit->b_10f ^= (src ^ unit->b_10f) & 1;` and a local `d = GetB10F(unit)`: the dst byte is
//     still hoisted into cl above the `mov [esi+0xfa], dl` store (the original loads it into al
//     right after the b_10e store, so the allocator there put dst in al and src in cl).
//  (3) The first statement of the 0x10f chain calls the getter twice, so MSVC keeps a hoisted
//    copy in bl at 0x487275 (just after FUN_00480250) and emits `xor al, byte ptr [esp+0x4c]`
//    then `xor bl, al` where the original has `mov cl, [esp+0x4c]` / `xor cl, al` / `xor cl, al`.
//    One getter call (81.3), a local seeded from the getter (81.3), a helper doing just this
//    statement (82.7) and a pointer-returning getter (81.3) each lose more than they gain.
//  (4) The 0x110 chain is still one register off from step 3 on: the original ors into the new
//    term and ours ors into the carried value, which rotates every later step. The hoisted
//    `hi` above is what lined up steps 1 and 2; a similar hoist for a later step did not help.
//  (5) The piece copy loop writes the obj byte first: with the original statement order the
//    strength-reduced pointers anchor on obj/f8 (esi+0x10 / edi) instead of f4 (esi+0xc).
//    All five orders of the first three statements were tried and seven ways of spelling the
//    two pointers; the two orders that keep the right anchors score 83.3 and 86.2, and the
//    orders that put f0 first match the loop body byte for byte but pick the wrong anchors.
//  Leads a permuter run found that are not plausible source (its result is kept as
//    build/scratch/0x487080/weh0.cpp, 88.8 percent before this pass's other changes): it needs
//    a `do { ... } while (0)` around the 0xc0 and 0x100 steps of the 0x110 chain, plus an
//    `unsigned int` temporary for each masked value in three more steps, one for the saved
//    unit's position, and `for (; 3 > j; j++)` with j declared above the chain. Ablation: the
//    do/while(0) is worth 3.3 percent on its own (88.8 without it 85.3, 86.2 with it but no
//    temporary), each temporary 0.6, and none of the swapped `&`/`|` operands matter at all. A
//    plain `{ }` block instead of the do/while(0) scores 85.3, so it is the loop, not the
//    scope, that matters; no natural construct for it was found.
extern "C" int __cdecl sprintf(char* buf, const char* fmt, ...);
extern "C" int __cdecl sprintf(char* buf, const char* fmt, ...);
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
    unsigned char flags;                 // +0x17
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
    unsigned char flags;                 // +0x1f
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
        SrcPiece_00487080* s = (SrcPiece_00487080*)(rec.pieces + j * 0x18);
        Piece_00487080* d = &unit->pieces[j];
        d->obj[0x10a] = s->f8;
        d->f0 = s->f0;
        d->f4 = s->f4;
        d->fc = s->fc;
        d->f10 = s->f10;
        d->f12 = s->f12;
        d->f14 = s->f14;
        d->f16 = s->f16;
        d->flags = ((unsigned char)s->flags ^ d->flags) & 1 ^ d->flags;
        d->flags = (unsigned char)(((s->flags >> 1 & 1) << 1) | (d->flags & 0xfd));
        d->flags = (unsigned char)(((s->flags >> 2 & 3) << 2) | (d->flags & 0xf3));
        d->flags = (unsigned char)(((s->flags >> 4 & 1) << 4) | (d->flags & 0xef));
    }

    if (unit->b_10f & 4)
        FUN_0047db20(unit);
    return unit;
    }
    return 0;
}