// Decompiled by DeepSeek V4.1 Flash, revised by Claude Opus 5.5, finished by
// deepseek-v4-flash. Names are provisional.
// Loads the unit portrait ("unitpics/<name>.PCX") for one unit per call, the
// index counting up in DAT_00512768. On the first call it initialises the two
// parallel output arrays: DAT_00512978 (image pointers from DAT_005129b8) and
// DAT_0051297c (24-byte sprite records based at pic+0xc6). Each record is a
// 0x14-byte sprite reference built by FrameFromSurface plus 4 trailing bytes.
//
// Suspected original bug: `if (def->name)` tests the address of the name
// array inside the unit definition (lea eax, [esi+0x20]; test eax, eax), which
// can never be null. Kept as the original has it.
//
// MATCHED (335 bytes). The old 4-hunk wall was one compiler decision. The
// original keeps the def base in esi and rematerialises the FUN_004290f0 name
// argument from defs (edx) and type (ecx) instead of reusing the base. Neither
// a local `def` nor a local `defs` breaks that CSE: MSVC folds every spelling
// of `defs[type].name` back to the condition's temporary.
//
// What breaks it: index the body argument by the *reloaded* entry field,
//     FUN_004290f0(path, "unitpics", defs[DAT_005129b4[i].unitType].name, "PCX");
// The reloaded load is CSE'd to the same register, but the value numbering no
// longer ties this address to the condition's `defs[type].name`, so MSVC
// rebuilds the whole address from defs/type at the call, which then forces the
// condition base into esi and pic into edi. Identical to `defs[(int)DAT..]`.
//
// After that only one hunk was left, the else branch: the original loads
// pic->field_17 (`mov cx, [edi+0x17]`) before storing rec.b = 0x20. Source
// order rec.a (field_17) before rec.b produces exactly that; rec.b first makes
// MSVC emit the 0x20 store first. So `rec.a = pic->field_17;` comes first.
//
// History: earlier passes (77.0%/76.3%) tried def->name / defs[type].name /
// g_game->defs[type].name as the argument, defs/type/def as locals in many
// orders, scoped def with early returns, inline getters and validity helpers,
// a real bitfield for the +0x245 bit, casts, `register`, a store to path
// between test and call, a local `Def* defs` (65.5%), no def local (65.9%)
// and (char*)def + 0x20 (76.3%). None moved the recompute.
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

struct Game {
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

extern Game* g_game;
extern int DAT_00512768;
extern int DAT_0051297c;
extern int DAT_00512978;
extern int DAT_005129b8;
extern Entry_0044c0d0* DAT_005129b4;

Pic_0044c0d0* __stdcall FUN_0049ff90(void* gadgets, char* name);
void __stdcall FUN_004290f0(char* out, const char* dir, const char* name, const char* ext);
void* __stdcall LoadPcx(char* path, int param_2);
void __stdcall FrameFromSurface(void* dst, void* src);
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
        Def_0044c0d0* defs = g_game->defs;
        if (defs[type].name && ((unsigned char)(defs[type].field_245 >> 15) & 1) == 0) {
            FUN_004290f0(path, "unitpics", defs[DAT_005129b4[i].unitType].name, "PCX");
            void* img = LoadPcx(path, 0);
            *(void**)DAT_00512978 = img;
            DAT_00512978 += 4;
            if (img != 0) {
                FrameFromSurface(&rec, img);
                rec.flag8 = 9;
            } else {
                rec.a = pic->field_17;
                rec.b = 0x20;
                rec.d = 0;
            }
            *(Record_0044c0d0*)DAT_0051297c = rec;
            DAT_0051297c += 0x18;
            FUN_0049fa90(&g_game->menu);
        }
    }
}
