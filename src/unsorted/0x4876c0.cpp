// Decompiled by deepseek-v4.1-flash, finished by GPT-6.1-sol, edited by deepseek-v4.1. Names are provisional.
// Saves every live unit (g_game+0x14357..+0x1435b, stride 0x118) as a 0xb8
// byte record; inverse of 0x487080/0x486fd0. Record and Piece field maps are
// complete and confirmed by the 0x487080 loader.
//
// PARTIAL 74.0%. Three of the four big register-rotation diffs came from one
// lever: what MSVC materialised as the source address of the rec+0x2b block.
//  - `rec.pos = unit->pos` (Vec3 member) reproduces the original's
//    `lea eax,[ebp+0x6a]`; a packed {int,short} copy for f64/f68 reproduces
//    `lea ecx,[ebp+0x64]`. Without them the block used [ebp+disp] directly
//    and collapsed (66.5 -> 40.6 for pointer locals, 66.5 -> 68.1 for the
//    struct copies). Do not use pointer locals here.
//  - the 3x piece copy: writing the obj deref first
//    (`dp[k].f8 = obj->f10a;` before `dp[k].f4 = sp[k].f8;`) moves the loop
//    anchor to the original's `lea esi,[ebp+0xc]` (68.1 -> 70.3).
//  - the rec+0x27 bool and the rec.f89 null guard: the original does NOT fold
//    the guard's `ptr != 0`, it emits it a third time. Writing the ternary
//    condition as the reloaded `unit->f86 != 0` (rather than the local
//    `a != 0`) stops MSVC proving non-nullness and was worth 66.5 -> 73.0 by
//    itself. The same reload on the rec.f8b guard scores 73.1 (worse) because
//    the induced allocation shift is a net loss at this state; left off.
// Still differs:
//  - the live zero for the bool lands in edx here but in ecx in the original;
//    that is the root of the remaining rotation in the rec+0x8f..+0xb3 field
//    block (ours loads the first accumulators into ecx/eax swapped).
//  - ours is 3 bytes short of the original 1062.
//
// deepseek-v4.1 (attempt 2): confirmed the diff is ONE allocator decision, not
// a statement-order problem. The original emits `xor ecx,ecx; cmp esi,ecx;
// setne al` for rec.f27 and then reuses ecx as the zero for the rec.f86 and
// rec.f_f0 guards, the rec.f89 = 0 arm and the (b & 0xf) mask; ours allocates
// that zero to edx instead, which rotates every scratch register in the
// rec+0x8f..+0xbb field block and inside the 3x piece copy.
// Tried and scored, all worse or equal to 73.7:
//  - 6 permutations of the rec.f3f / rec.f23 / rec.f27 statements (73.7 at
//    best, 70.1 at worst): the dx load and store of rec.f3f move with them but
//    the zero register does not change, so the lever is upstream of that
//    group.
//  - rec.f27 written as `unit->vtable ? 1 : 0` and as a 3-way ternary: 73.7
//    and lower, same zero register.
//  - rec.f8b moved after rec.fa7 (71.2) and rec.fa3 moved to the head of the
//    field block (71.2, and 23 bytes shorter): worse.
//  - the id8b block hoisted above the rec.f86 block: 71.3, 26 bytes shorter.
// deepseek-v4.1 (attempt 3): 73.7 -> 74.0 by putting `rec.f23 = n;` before
// rec.f3d/rec.f3f. Confirmed root of the remaining rotation: at the bool the
// original emits `xor ecx,ecx` and keeps dx = unit->fb8 live across it (its
// rec.f3f store lands after `cmp eax,ecx`), while ours stores rec.f3f at once
// and sinks the rec.f3d store below the bool, so ecx is still live holding
// unit->f108 and the zero lands in edx instead. Steered and failed:
// rec.f3f-reload-temp across the bool (70.1), f23/f3f/f27/f3d (69.4), f3f/f3d
// source swap (74.0 tie), inverted ternaries (73.3), reloaded unit->f_f0 in
// the id8b ternary (73.5). Piece loop: f0-first order (72.2) and obj-first
// with f0/f4 after (73.3) both lose the `lea esi,[ebp+0xc]` anchor, so keep
// the deref-first form. Next idea: find the statement that pins dx across the
// bool in the original's scheduler window (the f3f store is the only use).
// Next idea: give the zero its ecx identity from a statement that already
// wants a 0 in ecx before the bool, and keep dx live across the bool (in the
// original edx still holds unit->fb8 when the bool is evaluated, in ours that
// store has already retired).
// deepseek-v4.1-flash (run 4): no further gain, still 74.0. Confirmed the
// remaining diff is the single allocator decision above and that it is not
// compiler state: headers.py tried all 128 sets (best 74.0) and a sweep of 0 to
// 400 unused `extern int` declarations in front of the function was flat at
// 74.0 (so the source shape, not the compiler's state, is what is missing). All
// 24 orderings of {rec.f23, rec.f3d, rec.f3f, rec.f27} score at most 74.0 (best
// pABCD/pACBD; any ordering with rec.f3f after rec.f27 drops to 70.1). Locals
// for f108/fb8 (`short` and `int`), a `void*` vtable local, `unit->vtable ? 1
// : 0`, an explicit guarded rec.f27, and spelling the rec.f86 / rec.f_f0 guards
// as plain `a && ...` or `a ? ...` all scored 70.1 to 74.0, none higher. The
// lever is still getting the shared zero constant into ecx instead of edx,
// which needs the rec.f3f load (unit->fb8) live across the rec.f27 compare;
// source reordering of these statements alone does not produce it.
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
            char script[32];
            char bufTail[32];
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
            rec.f27 = (unit->vtable != 0);

            Unit_004876c0* a = (Unit_004876c0*)unit->f86;
            if (a != 0 && (a->flags & 0x10000000)) {
                rec.f89 = unit->f86 != 0 ? a->f_a8 : 0;
                rec.f8d = unit->f_f9;
            } else {
                rec.f89 = 0;
                rec.f8d = 0xff;
            }

            short id8b = 0;
            Unit_004876c0* a2 = (Unit_004876c0*)unit->f_f0;
            if (a2 != 0 && (a2->flags & 0x10000000))
                id8b = a2 != 0 ? a2->f_a8 : 0;

            rec.f8f = unit->f58;
            rec.f8e = unit->f_f4;
            rec.f9b = unit->f7e;
            rec.fab = unit->f_f5;
            rec.f97 = unit->f7a;
            rec.f8b = id8b;
            rec.fae = unit->fba;
            rec.fa7 = unit->f104;
            rec.f93 = unit->f76;
            rec.fad = unit->f_f7;
            rec.fb2 = unit->b_10e;
            rec.f9f = unit->fac;
            rec.fb1 = unit->f_fa;
            rec.fac = unit->f_f6;
            rec.fb0 = unit->f_f8;
            rec.fa3 = unit->fb0;

            unsigned int u = unit->flags;
            unsigned char b = unit->b_10f;
            rec.flags.a = b & 0xf;
            rec.flags.b = u & 0xfff;
            rec.flags.c = (u >> 13) & 1;
            rec.flags.e = (u >> 14) & 0xfff;

            Piece_004876c0* sp = unit->pieces;
            SavedPiece_004876c0* dp = rec.pieces;
            for (int k = 0; k < 3; k++) {
                dp[k].f8 = ((Obj_004876c0*)sp[k].obj)->f10a;
                dp[k].f4 = sp[k].f8;
                dp[k].f0 = sp[k].f0;
                dp[k].fc = sp[k].f10;
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
