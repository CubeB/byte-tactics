// Decompiled by deepseek-v4.1-flash, finished by GPT-6.1-sol, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by claude-sonnet-5-5, finished by DeepSeek V4.1 Flash. Names are provisional.
// Saves every live unit (g_game+0x14357..+0x1435b, stride 0x118) as a 0xb8
// byte record; inverse of 0x487080/0x486fd0. Record and Piece field maps are
// complete and confirmed by the 0x487080 loader.
//
// PARTIAL 90.1%, size-exact at 1062 bytes. What got it here (74.0% -> 90.1%):
//  - rec.f89 is `unit->f86 == 0 ? 0 : a->f_a8` (zero arm first, the original's
//    layout) and the id8b ternary is `unit->f_f0 == 0 ? 0 : a2->f_a8`: the
//    original keeps a third, reloaded null test in both guards. The second one
//    also brings the size from 1058 to the original's 1062.
//  - the 3x piece copy is written as dp[k].fc, f4, f8 (obj deref), f0, ...:
//    that keeps the `lea esi,[ebp+0xc]` anchor (the f8 member) and puts the
//    `add esi,0x1c` / `add eax,0x18` increments mid-body like the original.
//  - tools/permute.py found the next steps: `rec.flags.a = Get10f(unit) & 0xf`
//    with Get10f an inline helper returning the unsigned char, and
//    `short id8b = 0;` declared before `Unit* a`.
//  - earlier passes: `rec.pos = unit->pos` / packed copies for the rec+0x2b
//    block, obj-deref first, `unit->f86` reload for the rec.f27 and f89 guards.
//  - 85.6% -> 88.5%: statement order in the rec+0x8f..+0xbb block. The
//    `rec.f8b = id8b;` store moves from 6th to 10th of the sixteen
//    (build/scratch/0x4876c0/search.py, mode `cheap`).
//  - 88.5% -> 88.7% and the size reaches the original's 1062: `short id8b` is
//    written through a pointer, `short* pid8b = &id8b; ... *pid8b = ...;
//    rec.f8b = *pid8b;` (build/scratch/0x4876c0/variants2.py, "id8b through
//    ptr"). The 2 bytes are a second `xor ecx, ecx`, so the size landing on 1062
//    is a real signal, but it is the wrong xor: see "Still differs" below.
//  - 88.7% -> 90.1%: with the pointer in place, `rec.f8e = unit->f_f4;` moves
//    ahead of `rec.f8f = unit->f58;` in the sixteen (both directions are the
//    same permutation). Note this single swap is worth 1.4 points WITH the
//    pointer and costs 0.8 points WITHOUT it, so the two levers only work
//    together: that is the technique-17 case, re-sweep what you rejected.
// Still differs:
//  - the original zeroes ecx a second time at 0x4879d3, immediately after the
//    rec.fa3 store `mov [esp+0xbb],ecx` at 0x4879cc and just before
//    `mov cl,[ebp+0x10f]` at 0x4879da. Our codegen fills cl of a dead ecx and
//    masks it, which is the same value 2 bytes narrower. We now emit the two
//    xors the original has, but ours is the id8b one just before
//    `mov cx,[eax+0xa8]` at 0x4878ed, where the original shares one zero
//    (emitted at 0x48785e) between the rec.f89 and rec.f8b stores. So the size
//    is right and the two zeros sit in the wrong order.
//  - the register split of the block: the original gives f104 to edx and fb0 to
//    ecx (so ecx dies right before the b_10f load and must be re-zeroed), ours
//    gives f104 to ecx and fb0 to eax. The original's first dword (f58) goes to
//    eax and the first byte (f_f4) to dl; ours sends f58 to edx and f_f4 to al,
//    which is what makes al the byte scratch for four loads where the original
//    splits dl (three) and al (one).
//  - the first piece-loop iteration loads f10 ([esi+8]) where the original loads
//    f0 ([esi-8]) and stores it at [eax-4]; the source order is the same.
// Measured and rejected on the current body:
//  - tools/headers.py: all 256 header sets are 88.5% or worse, `<string.h>` (what
//    we have) among the best, so there is no header lever here.
//  - declaration order is inert: all 6 orders of the three function-scope
//    declarations, all 6 of rec/bufTail+script/bufHead, both of n/c, both of
//    id8b/a, and id8b or u moved to each of five group tops are all 88.7% and
//    fine=1318, byte-identical. One measured sweep, recorded either way.
//  - a getter for any single other field of the block (f104, f76, f58, fac,
//    f7a, f7e, fb0, f_f6, f_f8, f_f7, f_fa) is byte-identical to no getter:
//    MSVC 5 inlines them away and the load keeps its place.
//  - all 57 single relocations and adjacent transpositions of the eight dp[k]
//    piece-copy statements are worse than our order (best 1383 fine / 87.7%
//    against 1278 / 90.1%), so the f0-first loop iteration is not reachable by
//    reordering those statements.
//  - `volatile` on unit fields (all, flags, f108, fb8, the piece fields) does
//    not help here (70.8 to 77.7), unlike 0x487bf0. See the volatile note
//    below: no field in this function meets the bar anyway.
// For `rec.flags.a = Get10f(unit) & 0xf` (measured on the 85.6% body, re-check
// the ones that changed the size): an int- or unsigned-int-returning Get10f
// gives 1065 (82.8%), a plain `unit->b_10f & 0xf` or any int/unsigned char
// temporary gives 81.7%, a signed char getter is byte-identical to the
// unsigned char one, a ushort or short getter gives the right 1062 but via
// `movzx cx,[ebp+0x10f]` rather than `xor ecx,ecx` plus `mov cl,...`, and
// moving the five flags lines before the block gives 1061 (71.0%). An 8-bit
// `flags.a` and a `flags.a : 1` + `: 3` split are both worse (73.1%, 84.9%).
//  - for `rec.flags.a = Get10f(unit) & 0xf`: an int- or unsigned-int-returning
//    Get10f gives 1065 (82.8%), a plain `unit->b_10f & 0xf` or any int/unsigned
//    char temporary gives 81.7%, a signed char getter is byte-identical to the
//    unsigned char one, and moving the five flags lines before the block gives
//    1061 (71.0%). A `flags.a : 1 + : 3` split and an 8-bit `flags.a` are worse
//    (84.9% and 73.1%).
//  - volatile on unit fields (all, flags, f108, fb8, the piece fields) does not
//    help here (70.8 to 77.7), unlike 0x487bf0.
// Pass 15 (DeepSeek V4.1 Flash): still 90.1%, no new best. Two 3-minute
//   permuter runs (seeds 11 and 777, 5168 candidates) found nothing; a third
//   run started from the 89.9% no-pointer body also found nothing. Swept all
//   15 adjacent transpositions of the sixteen rec-field stores: none beats
//   90.1 (best 89.7, s1/s2), s9 (f8b/fb2) and s14 (fb0/fa3) tie at 90.1 with
//   the same fine score 1278. `int id8b` is byte-identical to `short id8b`;
//   removing the explicit `& 0xf` on flags.a is byte-identical; `a2 == 0` or
//   a nested `if` loses the 0x4879d3 xor and drops to 1056/88.8. Everything
//   points the same way: the sixteen-store block is a stable local optimum,
//   and the remaining 9.9 points are the f7e->eax/f76->ecx/fb0->ecx register
//   split which only appears if id8b's ecx dies before the f76 load, and the
//   f0-first piece-loop iteration.
#include <string.h>

extern "C" int __cdecl sprintf(char* buf, const char* fmt, ...);

struct Vec3_004876c0 {
    int x, y, z;
};

#pragma pack(push, 1)

struct Sub_004876c0 {
    int a;
    short b;
};

struct PieceFlags_004876c0 {
    unsigned char a : 1;    // bit 0
    unsigned char b : 1;    // bit 1
    unsigned char c : 2;    // bits 2-3
    unsigned char d : 1;    // bit 4
    unsigned char : 3;
};

struct FlagBits_004876c0 {
    unsigned int a : 4;     // bits 0-3
    unsigned int b : 12;    // bits 4-15
    unsigned int c : 1;     // bit 16
    unsigned int d : 3;     // bits 17-19 (never assigned)
    unsigned int e : 12;    // bits 20-31
};

struct Piece_004876c0 {             // 0x1c bytes at unit+0x4
    int f0;                         // +0x0
    char gap_4[4];
    int f8;                         // +0x8
    void* obj;                      // +0xc
    int f10;                        // +0x10
    short f14;                      // +0x14
    short f16;                      // +0x16
    short f18;                      // +0x18
    unsigned char f1a;              // +0x1a
    PieceFlags_004876c0 flags;      // +0x1b
};

struct SavedPiece_004876c0 {        // 0x18 bytes at rec+0x41
    int f0;                         // +0x0
    int f4;                         // +0x4
    int f8;                         // +0x8
    int fc;                         // +0xc
    short f10;                      // +0x10
    short f12;                      // +0x12
    short f14;                      // +0x14
    unsigned char f16;              // +0x16
    PieceFlags_004876c0 flags;      // +0x17
};

struct UnitRecord_004876c0 {        // 0xb8 bytes
    char name[0x20];                // +0x0
    unsigned char f20;              // +0x20
    unsigned short id;              // +0x21
    int f23;                        // +0x23
    int f27;                        // +0x27
    Vec3_004876c0 pos;              // +0x2b
    Sub_004876c0 s;                 // +0x37
    short f3d;                      // +0x3d
    short f3f;                      // +0x3f
    SavedPiece_004876c0 pieces[3];  // +0x41
    short f89;                      // +0x89
    short f8b;                      // +0x8b
    unsigned char f8d;              // +0x8d
    unsigned char f8e;              // +0x8e
    int f8f;                        // +0x8f
    int f93;                        // +0x93
    int f97;                        // +0x97
    int f9b;                        // +0x9b
    int f9f;                        // +0x9f
    int fa3;                        // +0xa3
    int fa7;                        // +0xa7
    unsigned char fab;              // +0xab
    unsigned char fac;              // +0xac
    unsigned char fad;              // +0xad
    unsigned short fae;             // +0xae
    unsigned char fb0;              // +0xb0
    unsigned char fb1;              // +0xb1
    unsigned short fb2;             // +0xb2
    FlagBits_004876c0 flags;        // +0xb4
};

struct Obj_004876c0 {
    char gap_0[0x10a];
    unsigned char f10a;             // +0x10a
};

struct Unit_004876c0 {
    void* vtable;                   // +0x0
    Piece_004876c0 pieces[3];       // +0x4
    int f58;                        // +0x58
    void* listHead;                 // +0x5c
    void* listTail;                 // +0x60
    Sub_004876c0 s64;               // +0x64
    Vec3_004876c0 pos;              // +0x6a
    int f76;                        // +0x76
    int f7a;                        // +0x7a
    int f7e;                        // +0x7e
    char gap_82[4];
    void* f86;                      // +0x86
    char gap_8a[8];
    void* f92;                      // +0x92
    char gap_96[4];
    void* f9a;                      // +0x9a
    char gap_9e[0xa];
    unsigned short f_a8;            // +0xa8
    char gap_aa[2];
    int fac;                        // +0xac
    int fb0;                        // +0xb0
    char gap_b4[4];
    short fb8;                      // +0xb8
    short fba;                      // +0xba
    char info[0x34];                // +0xbc
    void* f_f0;                     // +0xf0
    unsigned char f_f4;             // +0xf4
    unsigned char f_f5;             // +0xf5
    unsigned char f_f6;             // +0xf6
    unsigned char f_f7;             // +0xf7
    unsigned char f_f8;             // +0xf8
    unsigned char f_f9;             // +0xf9
    unsigned char f_fa;             // +0xfa
    char gap_fb[4];
    unsigned char f_ff;             // +0xff
    char gap_100[4];
    int f104;                       // +0x104
    short f108;                     // +0x108
    char gap_10a[4];
    unsigned char b_10e;            // +0x10e
    unsigned char b_10f;            // +0x10f
    unsigned int flags;             // +0x110
};

#pragma pack(pop)

extern void* g_game;

#pragma pack(push, 1)
class Class_0043a1f0 {
public:
    char gap_0[0x4a];
    Class_0043a1f0* next;           // +0x4a
    void FUN_0043a970(Unit_004876c0* unit, void* file, char* name);
};
#pragma pack(pop)

class Class_004010b0 {
public:
    void FUN_004010b0(Unit_004876c0* unit, void* file);
};

class Class_0043dd70 {
public:
    void FUN_0043dd70(Unit_004876c0* unit, void* file);
};

class Class_004b4560 {
public:
    int FUN_004b4560(char* name);
};

class Class_004b4ba0 {
public:
    int FUN_004b4ba0(char* name);
};

class Class_004b4b50 {
public:
    int FUN_004b4b50(int index);
};

class Class_004b4cf0 {
public:
    int FUN_004b4cf0(void* buf, int len);
};

class Class_004b4630 {
public:
    int FUN_004b4630(char* name, int value);
};

class Class_004b0610 {
public:
    int FUN_004b1ec0(void* file);
};

static inline unsigned char Get10f(Unit_004876c0* unit) { return unit->b_10f; }

// FUNCTION: 0x4876c0
void __stdcall FUN_004876c0(Class_004b4560* file)
{
    int count = 0;
    Unit_004876c0* end = 0;
    Unit_004876c0* unit;
    file->FUN_004b4560("Units");
    end = *(Unit_004876c0**)((char*)g_game + 0x1435b);
    unit = *(Unit_004876c0**)((char*)g_game + 0x14357);
    for (; unit <= end; unit = (Unit_004876c0*)((char*)unit + 0x118)) {
        if (unit->flags & 0x10000000) {
            UnitRecord_004876c0 rec;
            char bufTail[32], script[32];
            char bufHead[32];

            sprintf(script, "Script%i", count);
            ((Class_004b4ba0*)file)->FUN_004b4ba0(script);
            ((Class_004b0610*)unit->f9a)->FUN_004b1ec0(file);

            int n = 0;
            Class_0043a1f0* c = (Class_0043a1f0*)unit->listHead;
            while (c != 0) {
                sprintf(bufHead, "u%04xm%04x", unit->f_a8, n);
                c->FUN_0043a970(unit, file, bufHead);
                c = c->next;
                n++;
            }
            c = (Class_0043a1f0*)unit->listTail;
            while (c != 0) {
                sprintf(bufTail, "u%04xm%04x", unit->f_a8, n);
                c->FUN_0043a970(unit, file, bufTail);
                c = c->next;
                n++;
            }

            if (unit->vtable != 0)
                ((Class_0043dd70*)unit->vtable)->FUN_0043dd70(unit, file);
            ((Class_004010b0*)((char*)unit + 0xbc))->FUN_004010b0(unit, file);

            strcpy(rec.name, (char*)(*(char**)((char*)unit + 0x92) + 0x20));
            rec.f20 = unit->f_ff;
            rec.id = unit->f_a8;

            rec.pos = unit->pos;
            rec.s = unit->s64;
            rec.f23 = n;
            rec.f3d = unit->f108;
            rec.f3f = unit->fb8;
            rec.f27 = unit->vtable != 0;

            short id8b = 0;
            short* pid8b = &id8b;
            Unit_004876c0* a = (Unit_004876c0*)unit->f86;

            if (a != 0 && (a->flags & 0x10000000)) {
                rec.f89 = unit->f86 == 0 ? 0 : a->f_a8;
                rec.f8d = unit->f_f9;
            } else {
                rec.f89 = 0;
                rec.f8d = 0xff;
            }
            Unit_004876c0* a2 = (Unit_004876c0*)unit->f_f0;
            if (a2 != 0) {
                if (a2->flags & 0x10000000)
                    *pid8b = unit->f_f0 == 0 ? 0 : a2->f_a8;
            }

            rec.f8e = unit->f_f4;
            rec.f8f = unit->f58;
            rec.f9b = unit->f7e;
            rec.fab = unit->f_f5;
            rec.f97 = unit->f7a;
            rec.fae = unit->fba;
            rec.fa7 = unit->f104;
            rec.f93 = unit->f76;
            rec.fad = unit->f_f7;
            rec.f8b = *pid8b;
            rec.fb2 = unit->b_10e;
            rec.f9f = unit->fac;
            rec.fb1 = unit->f_fa;
            rec.fac = unit->f_f6;
            rec.fb0 = unit->f_f8;
            rec.fa3 = unit->fb0;

            unsigned int u = unit->flags;
            rec.flags.a = Get10f(unit) & 0xf;
            rec.flags.b = u & 0xfff;
            rec.flags.c = (u >> 13) & 1;
            rec.flags.e = (u >> 14) & 0xfff;

            Piece_004876c0* sp = unit->pieces;
            SavedPiece_004876c0* dp = rec.pieces;
            for (int k = 0; k < 3; k++) {
                dp[k].fc = sp[k].f10;
                dp[k].f4 = sp[k].f8;
                dp[k].f8 = ((Obj_004876c0*)sp[k].obj)->f10a;
                dp[k].f0 = sp[k].f0;
                dp[k].f10 = sp[k].f14;
                dp[k].f12 = sp[k].f16;
                dp[k].f14 = sp[k].f18;
                dp[k].f16 = sp[k].f1a;
                dp[k].flags.a = sp[k].flags.a;
                dp[k].flags.b = sp[k].flags.b;
                dp[k].flags.c = sp[k].flags.c;
                dp[k].flags.d = sp[k].flags.d;
            }

            ((Class_004b4b50*)file)->FUN_004b4b50(count);
            ((Class_004b4cf0*)file)->FUN_004b4cf0(&rec, 0xb8);
            count++;
        }
    }
    if (count > 0) {
        ((Class_004b4630*)file)->FUN_004b4630("Number of Units", count);
        ((Class_004b4630*)file)->FUN_004b4630("Version", 0x11);
    }
}