// Decompiled by DeepSeek V4.1 Flash, revised by Claude Opus 5.5. Names are provisional.
// Loads the unit portrait ("unitpics/<name>.PCX") for one unit per call, the
// index counting up in DAT_00512768. On the first call it initialises the two
// parallel output arrays: DAT_00512978 (image pointers from DAT_005129b8) and
// DAT_0051297c (24-byte sprite records based at pic+0xc6). Each record is a
// 0x14-byte sprite reference built by FUN_004b8ae0 plus 4 trailing bytes.
//
// Suspected original bug: `if (def->name)` tests the address of the name
// array inside the unit definition (lea eax, [esi+0x20]; test eax, eax), which
// can never be null. Kept as the original has it.
//
// STILL DIFFERS (77.0%; DeepSeek's version 76.3%). The original keeps def
// (&defs[type]) in esi and pic in edi, and at the FUN_004290f0 call it does
// not reuse def: it rebuilds
// defs[type].name from defs (edx) and type (ecx), which are still live
// (mov eax, ecx; shl eax, 6; add eax, ecx; lea ecx, [edx+eax*8];
// lea edx, [eax+ecx+0x20]). Every shape tried here lets MSVC reuse def (or
// the name address) for the call argument, which also leaves def in a scratch
// register and moves pic to esi. What was tried, all without the recompute:
// def->name or defs[type].name or g_game->defs[type].name as the argument;
// defs/type/def as locals or not; C-style declarations in all 120 orders;
// scoped def with early returns; inline getters and validity helpers (one or
// several returns); a real bitfield for the +0x245 bit; casts on the index or
// to another struct view; `register`; an inlined empty call or a store to
// path between the test and the call. Declaring `Def* defs = g_game->defs;`
// as a local gives the original's defs + t*8 + t shape for &defs[type] (edx
// for defs), but then scores lower overall (61.3%). The N-declarations test
// (0 to 400 unused externs) is flat for both this shape and DeepSeek's, so the
// source shape is still wrong, not the compiler state. The original seems to
// see the argument as a different value from def: something between the flag
// test and the call (or a different kind of expression) breaks the CSE.
//
// Second pass (deepseek-v4.1-flash worker). Root cause is one register
// tie-break: the original gives pic->edi and def->esi, ours gives pic->esi and
// def->eax (then the call argument reuses def in eax instead of rematerializing
// it). Confirmed the recompute is real: the original recomputes type*65 and
// defs[type].name from edx(defs)/ecx(type) at the call instead of using the
// esi=def it already has. Tried this pass, all at 77.0% with the same 4 hunks:
// reference Def& def = g_game->defs[type]; an extra def2 local; nested ifs; a
// forward-declared unassigned def; a def assigned from g_game->defs before the
// pic call; the else order rec.a before rec.b (no byte change, MSVC reorders).
// Worse: Def* defs = g_game->defs local (65.5%, def shape becomes right but
// everything after re-allocates), no def local at all (65.9%), arg written as
// (char*)def + 0x20 (76.3%). A static inline helper DefName(defs, type) for the
// argument changes nothing (MSVC inlines it and CSEs). So no source shape found
// that keeps def in esi and forces the argument to rematerialize.
#pragma pack(push, 1)

struct Entry_0044c0d0 {
    char unknown_0[0x52];
    int unitType;                  // +0x52
    char unknown_56[0x62 - 0x56];
};

struct Def_0044c0d0 {
    char unknown_0[0x20];
    char name[0x225];              // +0x20
    unsigned int field_245;        // +0x245
};

struct Pic_0044c0d0 {
    char unknown_0[0x17];
    unsigned short field_17;       // +0x17
    char unknown_19[0xc6 - 0x19];
    int field_c6;                  // +0xc6
};

struct Inner_0044c0d0 {
    int unknown_0;
    void* gadgets;                 // +0x4
};

struct Menu_0044c0d0 {
    char unknown_0[0x18];
    Inner_0044c0d0* inner;         // +0x18
};

struct Game_0044c0d0 {
    char unknown_0[0x519];
    Menu_0044c0d0 menu;            // +0x519
    char unknown_535[0x1438f - 0x535];
    int count;                     // +0x1438f
    char unknown_14393[0x1439b - 0x14393];
    Def_0044c0d0* defs;            // +0x1439b
};

struct Record_0044c0d0 {
    unsigned short a;              // +0x00
    unsigned short b;              // +0x02
    unsigned short e;              // +0x04
    unsigned short f;              // +0x06
    unsigned char flag8;           // +0x08
    unsigned char flag9;           // +0x09
    unsigned char flaga;           // +0x0a
    unsigned char flagb;           // +0x0b
    int unknown_c;                 // +0x0c
    int d;                         // +0x10
    int unknown_14;                // +0x14
};
#pragma pack(pop)

extern Game_0044c0d0* g_game;
extern int DAT_00512768;
extern int DAT_0051297c;
extern int DAT_00512978;
extern int DAT_005129b8;
extern Entry_0044c0d0* DAT_005129b4;

Pic_0044c0d0* __stdcall FUN_0049ff90(void* gadgets, char* name);
void __stdcall FUN_004290f0(char* out, const char* dir, const char* name, const char* ext);
void* __stdcall FUN_004caf30(char* path, int param_2);
void __stdcall FUN_004b8ae0(void* dst, void* src);
void __stdcall FUN_0049fa90(void* obj);

// FUNCTION: 0x44c0d0
void FUN_0044c0d0()
{
    Record_0044c0d0 rec;
    char path[256];
    Pic_0044c0d0* pic = FUN_0049ff90(g_game->menu.inner->gadgets, "PICLIST");
    if (DAT_00512768 == 0) {
        DAT_0051297c = pic->field_c6;
        DAT_00512978 = DAT_005129b8;
    }
    int i = DAT_00512768++;
    if (i < g_game->count) {
        int type = DAT_005129b4[i].unitType;
        Def_0044c0d0* def = &g_game->defs[type];
        if (def->name && ((unsigned char)(def->field_245 >> 15) & 1) == 0) {
            FUN_004290f0(path, "unitpics", g_game->defs[type].name, "PCX");
            void* img = FUN_004caf30(path, 0);
            *(void**)DAT_00512978 = img;
            DAT_00512978 += 4;
            if (img != 0) {
                FUN_004b8ae0(&rec, img);
                rec.flag8 = 9;
            } else {
                rec.b = 0x20;
                rec.a = pic->field_17;
                rec.d = 0;
            }
            *(Record_0044c0d0*)DAT_0051297c = rec;
            DAT_0051297c += 0x18;
            FUN_0049fa90(&g_game->menu);
        }
    }
}
