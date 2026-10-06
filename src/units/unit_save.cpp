// Decompiled by space-bunny-free, DeepSeek V4.1 Flash, GPT-6, GPT-6.1-sol, deepseek-v4.1, claude-sonnet-5-5, claude-opus-5-5, Space Bunny Free and Haiku. Names are provisional.

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

struct FlagBits {
    unsigned int a0 : 1;                // bits 0-3: unit+0x10f bits 0-3
    unsigned int a1 : 1;
    unsigned int a2 : 1;
    unsigned int a3 : 1;
    unsigned int b : 12;                // bits 4-15: unit+0x110 bits 0-11
    unsigned int c : 1;                 // bit 16: unit+0x110 bit 13
    unsigned int d : 3;                 // bits 17-19 (unused)
    unsigned int e : 12;                // bits 20-31: unit+0x110 bits 14-25
};

struct Bits10F {
    unsigned char b0 : 1;
    unsigned char b1 : 1;
    unsigned char b2 : 1;
    unsigned char b3 : 1;
    unsigned char b4 : 4;
};

struct Vec3 {
    int x, y, z;
};

struct PieceBits {
    unsigned char b0 : 1;
    unsigned char b1 : 1;
    unsigned char b23 : 2;
    unsigned char b4 : 1;
    unsigned char b5 : 3;
};

#pragma pack(push, 1)
struct Pair {
    int a;
    short b;
};

struct SrcPiece {                       // 0x18 bytes at +0x41 + i*0x18
    int f0;                             // +0x0
    int f4;                             // +0x4
    union {
        int f8;                         // +0x8
        unsigned char f8b;
    };
    int fc;                             // +0xc
    short f10;                          // +0x10
    short f12;                          // +0x12
    short f14;                          // +0x14
    unsigned char f16;                  // +0x16
    PieceBits fl;                       // +0x17
};

struct SaveRec {                        // 0xb8 bytes, one saved unit
    char name[0x20];
    unsigned char player;               // +0x20
    unsigned short id;                  // +0x21
    int f23;                            // +0x23
    int f27;                            // +0x27
    Vec3 pos;                           // +0x2b
    Pair s;                             // +0x37
    short f3d;                          // +0x3d
    short f3f;                          // +0x3f
    SrcPiece pieces[3];                 // +0x41
    short childA;                       // +0x89
    short childB;                       // +0x8b
    char b8d;                           // +0x8d
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
    union {
        unsigned short fb2;             // +0xb2
        struct {
            unsigned char bb2;
            unsigned char b3;           // (unused padding)
        };
    };
    FlagBits flags;                     // +0xb4
};

struct Piece {                          // 0x1c bytes at +0x4 + i*0x1c
    int f0;
    int unused;
    int f4;
    unsigned char* obj;
    int fc;
    short f10;
    short f12;
    short f14;
    unsigned char f16;
    PieceBits fl;                       // +0x1f
};

struct Obj {
    char gap_0[0x10a];
    unsigned char f10a;                 // +0x10a
};

struct Unit {
    void* vtable;                       // +0x0
    Piece pieces[3];                    // +0x4
    int field_58;                       // +0x58
    void* listHead;                     // +0x5c
    void* listTail;                     // +0x60
    Pair field_64;                      // +0x64 (int + short)
    Vec3 pos;                           // +0x6a
    int field_76;                       // +0x76
    int field_7a;                       // +0x7a
    int field_7e;                       // +0x7e
    char gap_82[4];
    void* f86;                          // +0x86
    char gap_8a[8];
    void* field_92;                     // +0x92
    char gap_96[4];
    void* field_9a;                     // +0x9a
    char gap_9e[0xa];
    unsigned short id;                  // +0xa8
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
    char gap_fb[4];
    unsigned char b_ff;                 // +0xff
    char gap_100[4];
    int field_104;                      // +0x104
    short field_108;                    // +0x108
    char gap_10a[4];
    unsigned char b_10e;
    union { unsigned char b_10f; Bits10F bf; };
    unsigned int flags;                 // +0x110
    char gap_114[4];
};

struct Mission;

struct Game {
    char unknown_0[0x14357];
    Unit* unitsBegin;                   // +0x14357
    char unknown_1435b[0x391e9 - 0x1435b];
    Mission* field_391e9;               // +0x391e9
};

class Entry_00487af0 {
public:
    char* field_0;                     // +0x0
    char* field_4;                     // +0x4
    char unknown_8[0x24 - 8];
};

struct Mission {
    char unknown_0[0xdac];
    Entry_00487af0* list;              // +0xdac
    int count;                         // +0xdb0
};

struct Struct_00487af0 {
    char unknown_0[4];
    int* field_4;                      // +0x4
};
#pragma pack(pop)

class HapiBank {
public:
    int OpenAccount(char* name);
    int GetIntegerItem(char* name, int def);
    int SetIntegerItem(char* name, int value);
    int OpenNumberedBox(int a);
    int OpenNamedBox(char* name);
    void SeekBox(int pos);
    int ReadBox(void* buf, int len);
    int WriteBox(void* buf, int len);
};

class CobScript {
public:
    void LoadScriptState(HapiBank*);
    int SaveScriptState(void* file);
};

class UnitMotion {
public:
    void LoadMotion(Unit*, HapiBank*);
    void SaveMotion(Unit* unit, void* file);
};

#pragma pack(push, 1)
// An order (0x56 bytes); 0x43a420 is its constructor from a saved record.
class Class_0043a420 {
public:
    char pad[0x42];
    unsigned int flags;                 // +0x42
    char gap_46[4];
    Class_0043a420* next;               // +0x4a
    char gap_4e[8];
    Class_0043a420(Unit* unit, HapiBank* file, char* name);
};

class Class_0043a1f0 {
public:
    char gap_0[0x4a];
    Class_0043a1f0* next;               // +0x4a
    void FUN_0043a970(Unit* unit, void* file, char* name);
};
#pragma pack(pop)

class Class_004388b0 { public: void FUN_004388b0(); };
class Class_00401110 { public: void LoadUnitAccounts(Unit*, HapiBank*); };
class Class_004010b0 {
public:
    void SaveUnitAccounts(Unit* unit, void* file);
};

extern Game* g_game;

Unit* __stdcall LoadUnit(unsigned short id, HapiBank* file);
unsigned short __stdcall FindUnitTypeId(const char* name);
Unit* __stdcall CreateUnit(unsigned char player, unsigned short typeId,
                                      Vec3 pos, int param_5, int mode,
                                      unsigned short id);
void __stdcall AttachUnitToPiece(Unit* unit, Unit* builder, int piece, int p4);
void __stdcall SetUnitSquad(Unit* unit, int id);
void __stdcall FUN_0047db20(Unit* unit);

// FUNCTION: 0x486fd0
void __stdcall LoadUnits(HapiBank* file)
{
    if (file->OpenAccount("Units")) {
        if (((HapiBank*)file)->GetIntegerItem("Version", 0) == 0x11) {
            int n = ((HapiBank*)file)->GetIntegerItem("Number of Units", 0);
            for (int i = 0; i < n; i++) {
                if (((HapiBank*)file)->OpenNumberedBox(i)) {
                    ((HapiBank*)file)->SeekBox(0);
                    SaveRec rec;
                    int len = ((HapiBank*)file)->ReadBox(&rec, 0xb8);
                    if (len != 0xb8) {
                        if (len + 2 != 0xb8)
                            continue;
                        rec.id = 0;
                    }
                    LoadUnit(rec.id, file);
                }
            }
        }
    }
}

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
// FUNCTION: 0x487080
Unit* __stdcall LoadUnit(unsigned short id, HapiBank* file)
{
    Unit* unit;
    if (id == 0)
        unit = 0;
    else
        unit = (Unit*)(*(char**)((char*)g_game + 0x14357) + id * 0x118);
    if (unit == 0 || (unit->flags & 0x10000000))
        return unit;

    SaveRec rec;
    char name[32];
    char script[32];
    int n = ((HapiBank*)file)->GetIntegerItem("Number of Units", 0);
    int found = 0;
    int i;
    for (i = 0; i < n; i++) {
        if (!((HapiBank*)file)->OpenNumberedBox(i))
            return 0;
        ((HapiBank*)file)->SeekBox(0);
        if (((HapiBank*)file)->ReadBox(&rec, 0xb8) != 0xb8)
            return 0;
        if (rec.id == id) {
            found = 1;
            break;
        }
    }
    if (!found)
        return 0;

    unit = CreateUnit(rec.player, FindUnitTypeId(rec.name), rec.pos, 1, rec.flags.b & 3, rec.id);
    if (unit != 0) {

    unit->field_64 = rec.s;
    unit->field_108 = rec.f3d;
    unit->field_b8 = rec.f3f;
    unit->pos.y = rec.pos.y;

    if (rec.childA != 0) {
        Unit* child = LoadUnit(rec.childA, file);
        if (child != 0)
            AttachUnitToPiece(unit, child, rec.b8d, rec.flags.b & 3);
    }
    unit->child = LoadUnit(rec.childB, file);
    unit->b_f9 = rec.b8d;
    unit->b_f4 = rec.b8e;
    unit->field_58 = rec.f8f;
    unit->field_76 = rec.f93;
    unit->field_7a = rec.f97;
    unit->field_7e = rec.f9b;
    unit->field_ac = rec.f9f;
    SetUnitSquad(unit, rec.f9f);
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

    ((Class_00401110*)&unit->info)->LoadUnitAccounts(unit, file);
    if (rec.f27 != 0)
        ((UnitMotion*)unit->vtable)->LoadMotion(unit, file);

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
    ((HapiBank*)file)->OpenNamedBox(script);
    ((CobScript*)unit->field_9a)->LoadScriptState(file);

    for (int j = 0; j < 3; j++) {
        unit->pieces[j].f0 = rec.pieces[j].f0;
        unit->pieces[j].f4 = rec.pieces[j].f4;
        unit->pieces[j].obj[0x10a] = rec.pieces[j].f8b;
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
// FUNCTION: 0x4876c0
void __stdcall SaveUnits(HapiBank* file)
{
    int count = 0;
    Unit* end = 0;
    Unit* unit;
    file->OpenAccount("Units");
    end = *(Unit**)((char*)g_game + 0x1435b);
    unit = *(Unit**)((char*)g_game + 0x14357);
    for (; unit <= end; unit = (Unit*)((char*)unit + 0x118)) {
        if (unit->flags & 0x10000000) {
            SaveRec rec;
            char bufTail[32], script[32];
            char bufHead[32];

            sprintf(script, "Script%i", count);
            ((HapiBank*)file)->OpenNamedBox(script);
            ((CobScript*)unit->field_9a)->SaveScriptState(file);

            int n = 0;
            Class_0043a1f0* c = (Class_0043a1f0*)unit->listHead;
            while (c != 0) {
                sprintf(bufHead, "u%04xm%04x", unit->id, n);
                c->FUN_0043a970(unit, file, bufHead);
                c = c->next;
                n++;
            }
            c = (Class_0043a1f0*)unit->listTail;
            while (c != 0) {
                sprintf(bufTail, "u%04xm%04x", unit->id, n);
                c->FUN_0043a970(unit, file, bufTail);
                c = c->next;
                n++;
            }

            if (unit->vtable != 0)
                ((UnitMotion*)unit->vtable)->SaveMotion(unit, file);
            ((Class_004010b0*)((char*)unit + 0xbc))->SaveUnitAccounts(unit, file);

            strcpy(rec.name, (char*)(*(char**)((char*)unit + 0x92) + 0x20));
            rec.player = unit->b_ff;
            rec.id = unit->id;

            rec.pos = unit->pos;
            rec.s = unit->field_64;
            rec.f23 = n;
            rec.f3d = unit->field_108;
            rec.f3f = unit->field_b8;
            rec.f27 = unit->vtable != 0;

            short id8b = 0;
            Unit* a = (Unit*)unit->f86;
            if (a != 0 && (a->flags & 0x10000000)) {
                rec.childA = unit->f86 == 0 ? 0 : a->id;
                rec.b8d = unit->b_f9;
            } else {
                rec.childA = 0;
                rec.b8d = 0xff;
            }
            Unit* a2 = (Unit*)unit->child;
            if (a2 != 0) {
                if (a2->flags & 0x10000000)
                    id8b = unit->child == 0 ? 0 : a2->id;
            }
            rec.childB = id8b;
            rec.f93 = unit->field_76;
            rec.b8e = unit->b_f4;
            rec.f8f = unit->field_58;
            rec.f9f = unit->field_ac;
            rec.f97 = unit->field_7a;
            rec.f9b = unit->field_7e;
            rec.bac = unit->b_f6;
            rec.fa7 = unit->field_104;
            rec.bab = unit->b_f5;
            rec.bb0 = unit->b_f8;
            rec.bad = unit->b_f7;
            rec.bae = unit->field_ba;
            rec.fa3 = unit->field_b0;
            rec.bb1 = unit->b_fa;
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
                rec.pieces[k].f4 = unit->pieces[k].f4;
                rec.pieces[k].f8 = ((Obj*)unit->pieces[k].obj)->f10a;
                rec.pieces[k].fc = unit->pieces[k].fc;
                rec.pieces[k].f10 = unit->pieces[k].f10;
                rec.pieces[k].f12 = unit->pieces[k].f12;
                rec.pieces[k].f14 = unit->pieces[k].f14;
                rec.pieces[k].f16 = unit->pieces[k].f16;
                rec.pieces[k].fl.b0 = unit->pieces[k].fl.b0;
                rec.pieces[k].fl.b1 = unit->pieces[k].fl.b1;
                rec.pieces[k].fl.b23 = unit->pieces[k].fl.b23;
                rec.pieces[k].fl.b4 = unit->pieces[k].fl.b4;
            }

            ((HapiBank*)file)->OpenNumberedBox(count);
            ((HapiBank*)file)->WriteBox(&rec, 0xb8);
            count++;
        }
    }
    if (count > 0) {
        ((HapiBank*)file)->SetIntegerItem("Number of Units", count);
        ((HapiBank*)file)->SetIntegerItem("Version", 0x11);
    }
}

// The element address has to go through this helper: written inline as
// param_2->field_4[i++] the strength reducer turns the loop into a pointer
// walk (add eax, 4 plus the loaded value in a register), while the original
// recomputes esi*4 with a lea and compares straight from memory.
static inline int* Elem_00487af0(int* arr, int i)
{
    return arr + i;
}

// Looks a name up in the class-name list at Mission+0xdac (one entry
// per team, each entry holding two names at +0 and +4) and returns the caller's
// parallel int array (param_2+4) at the same index, skipping entries whose int
// is 0. When the caller passes a nonzero value the search for the entry
// carrying that value starts at index 1 and the result is the match index
// plus one, so index 0 of the array is never returned: the original's
// `i++` sits inside the subscript, so the first test is array[0] with i
// already 1, and a match on array[0] leaves i = 1.
// FUNCTION: 0x487af0
int __stdcall FindMissionUnit(char* name, Struct_00487af0* param_2, int value)
{
    int i = 0;

    if (value) {
        while (*Elem_00487af0(param_2->field_4, i++) != value) {
            if (i >= g_game->field_391e9->count)
                return 0;
        }
    }
    if (i >= g_game->field_391e9->count)
        return 0;
    for (; i < g_game->field_391e9->count; i++) {
        Entry_00487af0* e = &g_game->field_391e9->list[i];
        if (e->field_4 && _strcmpi(e->field_4, name) == 0 && param_2->field_4[i])
            return param_2->field_4[i];
        if (e->field_0 && _strcmpi(e->field_0, name) == 0 && param_2->field_4[i])
            return param_2->field_4[i];
    }
    return 0;
}
