// Decompiled by DeepSeek V4.1 Flash, finished by GPT-6, finished by deepseek-v4.1-flash, finished by GPT-6.1-sol, finished by deepseek-v4.1-flash, finished by claude-sonnet-5-5, finished by Space Bunny Free, finished by claude-sonnet-5-5, finished by DeepSeek V4.1 Flash, finished by claude-opus-5-5. Names are provisional.
// Loads one unit (and, recursively, the units it carries or is built by) from the "Units"
// section of a saved game: finds its 0xb8-byte record by id, creates the unit and copies the
// record into it.
//
// MATCH (pass 18, claude-opus-5-5, from 97.1%). What closed it:
//  - The record's flags word is the save function's layout (0x4876c0): a0-a3, a 12-bit
//    block b, a bit c and a 12-bit block e. Each unit+0x110 flag is merged from those
//    blocks straight into unit->flags, `unit->flags = (unit->flags & ~m) | (rec.flags.b & m);`,
//    with no `u` local and no reload after the field_b0 store.
//  - <math.h> is included. Which step of the 0x110 chain keeps its result in the old
//    flags register (the original's edx step at 0x4873c7, the one that ors 0x200) depends
//    on how many symbols the translation unit declares before the function. Without
//    <math.h>, 286 to 797 unused `extern int` lines in front also match; 285 or fewer, or
//    798 up to at least 1830, do not. It is not the scratch rotation: removing any one
//    statement before the chains leaves their registers alone, while the chain's own
//    spelling (a `u` local, these direct merges, unit-side bitfields) moves the edx step.
// 0x43a420 runs on the result of operator new and stores vtables, so it is written as a
// constructor, `new Class_0043a420(unit, file, name)`, as the naming rule asks.
// Earlier passes (condensed):
//  - Pass 17: <stdio.h>, <string.h> and <stdlib.h> (for sprintf) and plain bitfield
//    copies for the 0x10f byte (`unit->bf.b0 = rec.flags.a0;`).
//  - Pass 16: the piece copy loop is plain array indexing in the original field order
//    (f0, f4, obj, fc, ...); MSVC anchors the strength-reduced pointer on the second
//    distinct non-zero offset. The piece flags are real bitfields.
//  - Pass 14: the failure paths fall out of `if (unit != 0) { ... return unit; } return 0;`
//    so the final `xor eax, eax` is the last block, as in the original.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>


struct FlagBits_00487080 {
    unsigned int a0 : 1;                // bits 0-3: unit+0x10f bits 0-3
    unsigned int a1 : 1;
    unsigned int a2 : 1;
    unsigned int a3 : 1;
    unsigned int b : 12;                // bits 4-15: unit+0x110 bits 0-11
    unsigned int c : 1;                 // bit 16: unit+0x110 bit 13
    unsigned int d : 3;                 // bits 17-19 (unused)
    unsigned int e : 12;                // bits 20-31: unit+0x110 bits 14-25
};

struct Bits10F_00487080 {
    unsigned char b0 : 1;
    unsigned char b1 : 1;
    unsigned char b2 : 1;
    unsigned char b3 : 1;
    unsigned char b4 : 4;
};

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
    FlagBits_00487080 flags;            // +0xb4
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
};
#pragma pack(pop)

#pragma pack(push, 1)
struct Unit {
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
    Unit* child;                        // +0xf0
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
    union { unsigned char b_10f; Bits10F_00487080 bf; };
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
Unit* __stdcall FUN_00485f50(unsigned char player, unsigned short typeId,
                                      Vec3_00487080 pos, int param_5, int mode,
                                      unsigned short id);
void __stdcall FUN_0048aac0(Unit* unit, Unit* builder, int piece, int p4);
void __stdcall FUN_00480250(Unit* unit, int id);
#pragma pack(push, 1)
// An order (0x56 bytes); 0x43a420 is its constructor from a saved record.
class Class_0043a420 {
public:
    char pad[0x42];
    unsigned int flags;                 // +0x42
    char gap_46[4];
    Class_0043a420* next;               // +0x4a
    char gap_4e[8];
    Class_0043a420(Unit* unit, Class_004b4560* file, char* name);
};
#pragma pack(pop)
class Class_004388b0 { public: void FUN_004388b0(); };
void __stdcall FUN_0047db20(Unit* unit);
class Class_00401110 { public: void FUN_00401110(Unit*, Class_004b4560*); };
class Class_0043d210 { public: void FUN_0043de30(Unit*, Class_004b4560*); };
class Class_004b0610 { public: void FUN_004b2040(Class_004b4560*); };


// FUNCTION: 0x487080
Unit* __stdcall FUN_00487080(unsigned short id, Class_004b4560* file)
{
    Unit* unit;
    if (id == 0)
        unit = 0;
    else
        unit = (Unit*)(*(char**)((char*)g_game + 0x14357) + id * 0x118);
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

    unit = FUN_00485f50(rec.player, FUN_00488b10(rec.name), *(Vec3_00487080*)&rec.f2b, 1, rec.flags.b & 3, rec.id);
    if (unit != 0) {

    unit->field_64 = *(Pair_00487080*)&rec.f37;
    unit->field_108 = rec.f3d;
    unit->field_b8 = rec.f3f;
    unit->field_6e = rec.f2f;

    if (rec.childA != 0) {
        Unit* child = FUN_00487080(rec.childA, file);
        if (child != 0)
            FUN_0048aac0(unit, child, rec.b8d, rec.flags.b & 3);
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

    unit->bf.b0 = rec.flags.a0;
    unit->bf.b1 = rec.flags.a1;
    unit->bf.b2 = rec.flags.a2;
    unit->bf.b3 = rec.flags.a3;
    unit->flags = (unit->flags & ~0xc) | (rec.flags.b & 0xc);
    unit->flags = (unit->flags & ~0x10) | (rec.flags.b & 0x10);
    unit->flags = (unit->flags & ~0x20) | (rec.flags.b & 0x20);
    unit->flags = (unit->flags & ~0xc0) | (rec.flags.b & 0xc0);
    unit->flags = (unit->flags & ~0x100) | (rec.flags.b & 0x100);
    unit->flags = (unit->flags & ~0x200) | (rec.flags.b & 0x200);
    unit->flags = (unit->flags & ~0x400) | (rec.flags.b & 0x400);
    unit->flags = (unit->flags & ~0x800) | (rec.flags.b & 0x800);
    unit->field_b0 = rec.fa3;
    unit->flags = (unit->flags & ~0x2000) | (rec.flags.c << 13);
    unit->flags = (unit->flags & ~0x4000) | ((rec.flags.e << 14) & 0x4000);
    unit->flags = (unit->flags & ~0x8000) | ((rec.flags.e << 14) & 0x8000);
    unit->flags = (unit->flags & ~0x10000) | ((rec.flags.e << 14) & 0x10000);
    unit->flags = (unit->flags & ~0x20000) | ((rec.flags.e << 14) & 0x20000);
    unit->flags = (unit->flags & ~0xc0000) | ((rec.flags.e << 14) & 0xc0000);
    unit->flags = (unit->flags & ~0x300000) | ((rec.flags.e << 14) & 0x300000);
    unit->flags = (unit->flags & ~0x400000) | ((rec.flags.e << 14) & 0x400000);
    unit->flags = (unit->flags & ~0x3800000) | ((rec.flags.e << 14) & 0x3800000);

    ((Class_00401110*)&unit->info)->FUN_00401110(unit, file);
    if (rec.f27 != 0)
        ((Class_0043d210*)unit->vtable)->FUN_0043de30(unit, file);

    Class_0043a420** normal = (Class_0043a420**)&unit->listHead;
    Class_0043a420** special = (Class_0043a420**)&unit->listTail;
    int k = 0;
    if (rec.f23 > 0) {
        do {
            sprintf(name, "u%04xm%04x", unit->id, k);
            Class_0043a420* p = new Class_0043a420(unit, file, name);
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