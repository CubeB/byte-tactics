// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
// Loads one saved unit from a 0xb8-byte save record (see 0x486fd0, which reads
// the records and calls this). The record begins with the unit type name; its id
// at +0x21 is what callers pass in and compare against. g_game+0x14357 is the
// live unit array, stride 0x118.
//
// PARTIAL: flow and record->unit field map are in place, but the 0x10f byte and
// 0x110 dword bitfield merges and the piece copy at +0x4 are not yet
// instruction-exact, so this does not match.

extern "C" int sprintf(char* buf, const char* fmt, ...);

struct Vec3_00487080 {
    int x, y, z;
};

#pragma pack(push, 1)
// 0xb8-byte save record. Name at +0x0, id at +0x21 (proven by the
// `cmp word ptr [esp+0x39], bx` against the record base at esp+0x18).
struct SaveRec_00487080 {
    char name[0x21];                    // +0x0
    unsigned short id;                  // +0x21
    char gap_23[0x2b - 0x23];
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
    unsigned char flags;                // +0x17
};

struct Piece_00487080 {                 // 0x1c bytes at +0x4 + i*0x1c
    int f0;                             // +0x0
    int f4;                             // +0x4
    unsigned char* obj;                 // +0x8
    int fc;                             // +0xc
    short f10;                          // +0x10
    short f12;                          // +0x12
    short f14;                          // +0x14
    unsigned char f16;                  // +0x16
    unsigned char flags;                // +0x17
    char gap_18[4];
};

#pragma pack(pop)

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
    char gap_9e[0xe];
    int field_ac;                       // +0xac
    int field_b0;                       // +0xb0
    char gap_b4[4];
    short field_b8;                     // +0xb8
    short field_ba;                     // +0xba
    char info[0x34];                    // +0xbc
    char gap_info[0x34];
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
Unit_00487080* __stdcall FUN_0043a420(void* self, Unit_00487080* unit, Class_004b4560* file,
                                       char* name);
void __stdcall FUN_004388b0(int o);
void __stdcall FUN_0047db20(Unit_00487080* unit);
void __stdcall FUN_00401110(void* info, Unit_00487080* unit, Class_004b4560* file);
void __stdcall FUN_0043de30(void* vt, Unit_00487080* unit, Class_004b4560* file);
void __stdcall FUN_004b2040(void* o, Class_004b4560* file);

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
    int n = ((Class_004b4800*)file)->FUN_004b4800("Number of Units", 0);
    int found = 0;
    int i = 0;
    if (n > 0) {
        do {
            if (!((Class_004b4b50*)file)->FUN_004b4b50(i))
                return 0;
            ((Class_004b4c10*)file)->FUN_004b4c10(0);
            if (((Class_004b4c80*)file)->FUN_004b4c80(&rec, 0xb8) != 0xb8)
                return 0;
            if (rec.id == id) {
                found = 1;
                break;
            }
            i++;
        } while (i < n);
    }
    if (!found)
        return 0;

    unsigned char player = (rec.flags >> 4) & 3;
    unsigned short typeId = FUN_00488b10(rec.name);
    unit = FUN_00485f50(player, typeId, *(Vec3_00487080*)&rec.f2b, rec.f37, 1, rec.id);
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
    unit->field_b0 = rec.fa3;
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
    b = (unsigned char)((f >> 1 & 1) << 1) | (b & 0xfd);
    b = (unsigned char)((f >> 2 & 1) << 2) | (b & 0xfb);
    b = (unsigned char)((f >> 3 & 1) << 3) | (b & 0xf7);
    unit->b_10f = b;

    unsigned int u = unit->flags;
    u = (f >> 4 & 0xc) | (u & 0xfffffff3);
    unit->flags = u;
    u = (f >> 4 & 0x10) | (u & 0xffffffef);
    unit->flags = u;
    u = (f >> 4 & 0x20) | (u & 0xffffffdf);
    unit->flags = u;
    u = (f >> 4 & 0xc0) | (u & 0xffffff3f);
    unit->flags = u;
    u = (f >> 4 & 0x100) | (u & 0xfffffeff);
    unit->flags = u;
    u = (f >> 4 & 0x200) | (u & 0xfffffdff);
    unit->flags = u;
    u = (f >> 4 & 0x400) | (u & 0xfffffbff);
    unit->flags = u;
    u = (f >> 4 & 0x800) | (u & 0xfffff7ff);
    unit->flags = u;
    u = (f >> 3 & 0x2000) | (u & 0xffffdfff);
    unit->flags = u;
    u = (f >> 6 & 0x4000) | (u & 0xffffbfff);
    unit->flags = u;
    u = (f >> 6 & 0x8000) | (u & 0xffff7fff);
    unit->flags = u;
    u = (f >> 6 & 0x10000) | (u & 0xfffeffff);
    unit->flags = u;
    u = (f >> 6 & 0x20000) | (u & 0xfffdffff);
    unit->flags = u;
    u = (f >> 6 & 0xc0000) | (u & 0xfff3ffff);
    unit->flags = u;
    u = (f >> 6 & 0x300000) | (u & 0xffcfffff);
    unit->flags = u;
    u = (f >> 6 & 0x400000) | (u & 0xffbfffff);
    unit->flags = u;
    u = (f >> 6 & 0x3800000) | (u & 0xfc7fffff);
    unit->flags = u;

    FUN_00401110(&unit->info, unit, file);
    if (rec.f33 != 0)
        FUN_0043de30(unit->vtable, unit, file);

    Piece_00487080* dst = unit->pieces;
    SrcPiece_00487080* src = (SrcPiece_00487080*)rec.pieces;
    int k = 0;
    if (rec.f33 > 0) {
        do {
            char name[32];
            sprintf(name, "u%04xm%04x", unit->field_b8, k);
            Unit_00487080* p = (Unit_00487080*)new char[0x56];
            if (p != 0)
                p = FUN_0043a420(p, unit, file, name);
            k++;
        } while (k < rec.f33);
    }
    if (unit->listHead != 0)
        FUN_004388b0((int)unit->listHead);
    {
        char script[32];
        sprintf(script, "Script%i", rec.f3b);
        ((Class_004b4ba0*)file)->FUN_004b4ba0(script);
    }
    FUN_004b2040(unit->field_9a, file);

    for (int j = 0; j < 3; j++) {
        SrcPiece_00487080* s = (SrcPiece_00487080*)(rec.pieces + j * 0x18);
        Piece_00487080* d = &unit->pieces[j];
        d->f0 = *(int*)((char*)s - 4);
        d->f4 = s->f0;
        d->obj[0x10a] = s->f8;
        d->fc = s->fc;
        d->f10 = s->f10;
        d->f12 = s->f12;
        d->f14 = s->f14;
        d->f16 = s->f16;
        unsigned char pf = d->flags;
        unsigned char sf = s->flags;
        pf = (sf ^ pf) & 1 ^ pf;
        pf = (unsigned char)((sf & 2) << 0) | (pf & 0xfd);
        pf = (unsigned char)((sf & 4) << 0) | (pf & 0xf9);
        d->flags = (unsigned char)((sf & 0x10) << 0) | (pf & 0xf1);
    }

    if (unit->b_10f & 4)
        FUN_0047db20(unit);
    return unit;
}
