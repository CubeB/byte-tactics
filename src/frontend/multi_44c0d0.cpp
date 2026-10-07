// Decompiled by DeepSeek V4.1 Flash, revised by Claude Opus 5.5, finished by
// deepseek-v4-flash. Names are provisional.
// Loads the unit portrait ("unitpics/<name>.PCX") for one unit per call, the
// index counting up in DAT_00512768. On the first call it initialises the two
// parallel output arrays: DAT_00512978 (image pointers from DAT_005129b8) and
// DAT_0051297c (24-byte sprite records based at pic+0xc6). Each record is a
// 0x14-byte sprite reference built by FrameFromSurface plus 4 trailing bytes.
//
// Suspected original bug: `if (def->name)` tests the address of the name
// array inside the unit definition, which can never be null. Kept as the
// original has it.
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

Pic_0044c0d0* __stdcall FindGadgetChecked(void* gadgets, char* name);
void __stdcall BuildDataPath(char* out, const char* dir, const char* name, const char* ext);
void* __stdcall LoadPcx(char* path, int param_2);
void __stdcall FrameFromSurface(void* dst, void* src);
void __stdcall FUN_0049fa90(void* obj);

// FUNCTION: 0x44c0d0
void LoadUnitPortrait()
{
    Record_0044c0d0 rec;
    char path[256];
    Pic_0044c0d0* pic = FindGadgetChecked(g_game->menu.inner->gadgets, "PICLIST");
    if (DAT_00512768 == 0) {
        DAT_0051297c = pic->field_c6;
        DAT_00512978 = DAT_005129b8;
    }
    int i = DAT_00512768++;
    if (i < g_game->count) {
        int type = DAT_005129b4[i].unitType;
        Def_0044c0d0* defs = g_game->defs;
        if (defs[type].name && ((unsigned char)(defs[type].field_245 >> 15) & 1) == 0) {
            // Indexed by the reloaded entry field, not defs[type].name.
            BuildDataPath(path, "unitpics", defs[DAT_005129b4[i].unitType].name, "PCX");
            void* img = LoadPcx(path, 0);
            *(void**)DAT_00512978 = img;
            DAT_00512978 += 4;
            if (img != 0) {
                FrameFromSurface(&rec, img);
                rec.flag8 = 9;
            } else {
                // rec.a before rec.b: field_17 is loaded before the 0x20 store.
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
