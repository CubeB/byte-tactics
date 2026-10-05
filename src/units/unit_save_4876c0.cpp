// Decompiled by deepseek-v4.1-flash, finished by GPT-6.1-sol, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by claude-sonnet-5-5, finished by DeepSeek V4.1 Flash, finished by claude-opus-5-5. Names are provisional.
// Saves every live unit (g_game+0x14357..+0x1435b, stride 0x118) as a 0xb8
// byte record; inverse of the 0x487080 loader, whose field map it shares.
//
// MATCH. What closed it from 90.1%:
//  - the low four bits of the record's flags word are four 1-bit copies,
//    `rec.flags.a0 = unit->bf.b0;` and so on, as in the loader. MSVC merges
//    them into one `& 0xf` but keeps the zero extension (`xor ecx, ecx` before
//    `mov cl, [ebp+0x10f]`), which a 4-bit field or `& 0xf` spelling folds away.
//  - the piece loop is plain array indexing in field order (f0, f4, obj, fc,
//    ...), as in the loader; MSVC anchors the pointers on the second offset.
//  - id8b is a plain `short` local assigned in the nested if (no pointer).
//  - the sixteen field copies after `rec.f8b = id8b;`: the three scratch
//    registers rotate with each statement, and the scheduler keeps each
//    register's loads in source order. The original's per-register chains
//    (ecx: f93 f9f fac fb0 fa3, edx: f8e f97 fa7 fad fb1, eax: f8f f9b fab
//    fae fb2) interleaved as ecx, edx, eax give the order below.
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
    unsigned int a0 : 1;    // bits 0-3: unit+0x10f bits 0-3
    unsigned int a1 : 1;
    unsigned int a2 : 1;
    unsigned int a3 : 1;
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

struct Bits10F_004876c0 {
    unsigned char b0 : 1;
    unsigned char b1 : 1;
    unsigned char b2 : 1;
    unsigned char b3 : 1;
    unsigned char b4 : 4;
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
    union {
        unsigned char b_10f;        // +0x10f
        Bits10F_004876c0 bf;
    };
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
                    id8b = unit->f_f0 == 0 ? 0 : a2->f_a8;
            }
            rec.f8b = id8b;
            rec.f93 = unit->f76;
            rec.f8e = unit->f_f4;
            rec.f8f = unit->f58;
            rec.f9f = unit->fac;
            rec.f97 = unit->f7a;
            rec.f9b = unit->f7e;
            rec.fac = unit->f_f6;
            rec.fa7 = unit->f104;
            rec.fab = unit->f_f5;
            rec.fb0 = unit->f_f8;
            rec.fad = unit->f_f7;
            rec.fae = unit->fba;
            rec.fa3 = unit->fb0;
            rec.fb1 = unit->f_fa;
            rec.fb2 = unit->b_10e;
            unsigned int u = unit->flags;
            rec.flags.a0 = unit->bf.b0;
            rec.flags.a1 = unit->bf.b1;
            rec.flags.a2 = unit->bf.b2;
            rec.flags.a3 = unit->bf.b3;
            rec.flags.b = u & 0xfff;
            rec.flags.c = (u >> 13) & 1;
            rec.flags.e = (u >> 14) & 0xfff;

            for (int k = 0; k < 3; k++) {
                rec.pieces[k].f0 = unit->pieces[k].f0;
                rec.pieces[k].f4 = unit->pieces[k].f8;
                rec.pieces[k].f8 = ((Obj_004876c0*)unit->pieces[k].obj)->f10a;
                rec.pieces[k].fc = unit->pieces[k].f10;
                rec.pieces[k].f10 = unit->pieces[k].f14;
                rec.pieces[k].f12 = unit->pieces[k].f16;
                rec.pieces[k].f14 = unit->pieces[k].f18;
                rec.pieces[k].f16 = unit->pieces[k].f1a;
                rec.pieces[k].flags.a = unit->pieces[k].flags.a;
                rec.pieces[k].flags.b = unit->pieces[k].flags.b;
                rec.pieces[k].flags.c = unit->pieces[k].flags.c;
                rec.pieces[k].flags.d = unit->pieces[k].flags.d;
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