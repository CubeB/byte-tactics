// Decompiled by deepseek-v4.1-flash, finished by GPT-6. Names are provisional.
// GPT-6 retry: 66.0%, not MATCH. Stack counter arrays, count helpers and
// alternate count/counter scopes do not improve the saved implementation.
// Partial: 66.0%. math.h aligns early registers; caching the current unit definition improves the final loop. Count slots and loop induction registers still differ.

#include <math.h>
#include <vector>

class Class_004c9390 {
public:
    char* data;

    void FUN_004c9390();
};

class Class_004c91a0 {
public:
    char* p;

    ~Class_004c91a0() { ((Class_004c9390*)this)->FUN_004c9390(); }
};

class Class_004c46c0 {
public:
    int FUN_004c46c0(const char* name, int def);
    int FUN_004c48c0(char* dst, const char* key, int size, char* def);
};

class Class_004c2ea0 {
public:
    int field_0;                       // +0x0
    Class_004c46c0* current;           // +0x4
    int field_8;                       // +0x8

    Class_004c2ea0();
    ~Class_004c2ea0();
};

class Class_004c2f60 {
public:
    int FUN_004c2f60(char* file);
};

class Class_004c3e10 {
public:
    void FUN_004c3e10();
};

class Class_004c3490 {
public:
    int FUN_004c3490(int index);
};

struct Flags_0042dcf0 {
    unsigned int field_0 : 5;
    unsigned int downloadable : 1;
    unsigned int field_6 : 26;
};

#pragma pack(push, 1)
struct BuildEntry_0042dcf0 {
    unsigned short typeId;             // +0x00
    unsigned char page;                // +0x02
    unsigned char slot;                // +0x03
    char name[0x21];                   // +0x04
};

struct BuildList_0042dcf0 {
    int count;                         // +0x00
    BuildEntry_0042dcf0 entries[5];    // +0x04
};

struct UnitDef_0042dcf0 {
    char unknown_0[0x20];              // +0x00
    char name[0x122];                  // +0x20
    char unknown_142[0x22e - 0x142];   // +0x142
    unsigned char field_22e;           // +0x22e
    char unknown_22f[0x241 - 0x22f];   // +0x22f
    Flags_0042dcf0 flags_241;          // +0x241
    char unknown_245[0x249 - 0x245];   // +0x245
};

struct Game_0042dcf0 {
    char unknown_0[0x1438f];
    int unitDefCount;                  // +0x1438f
    char unknown_14393[0x1439b - 0x14393];
    UnitDef_0042dcf0* unitDefs;        // +0x1439b
    char unknown_1439f[0x391c7 - 0x1439f];
    int buildListCount;                // +0x391c7
    BuildList_0042dcf0* buildLists;    // +0x391cb
};
#pragma pack(pop)

extern Game_0042dcf0* g_game;
extern char DAT_005119b8[];

void __stdcall FUN_004290f0(char* out, const char* dir, const char* name, const char* ext);
void __stdcall FUN_004bca30(const char* pattern, int flags, std::vector<Class_004c91a0>* out);
void* __cdecl FUN_004d83b0(const char* name, int size);
void __cdecl FUN_004d8780(void* p);
void __cdecl FUN_004d8710(void* p);
void __cdecl FUN_0042be30();

// FUNCTION: 0x42dcf0
void FUN_0042dcf0()
{
    char path[256];
    char unitbuf[256];
    std::vector<Class_004c91a0> files;
    FUN_004290f0(path, "download", "*", "TDF");
    FUN_004bca30(path, 0, &files);

    int n = files.size();
    g_game->buildListCount = n;
    g_game->buildLists = (BuildList_0042dcf0*)FUN_004d83b0("DOWNLOADMENU", n * 0xbd);

    for (int i = 0; i < n; i++) {
        Class_004c2ea0 parser;
        FUN_004290f0(path, "download", files[i].p, "TDF");
        if (((Class_004c2f60*)&parser)->FUN_004c2f60(path)) {
            int j = 0;
            while (1) {
                ((Class_004c3e10*)&parser)->FUN_004c3e10();
                if (!((Class_004c3490*)&parser)->FUN_004c3490(j))
                    break;
                g_game->buildLists[i].count = j + 1;
                char* buf = unitbuf;
                if (parser.current->FUN_004c48c0(unitbuf, "UNITMENU", 0x20, DAT_005119b8)) {
                    for (unsigned short u = 0; u < g_game->unitDefCount; u++) {
                        if (_strcmpi(g_game->unitDefs[u].name, buf) == 0) {
                            BuildEntry_0042dcf0* e = &g_game->buildLists[i].entries[j];
                            e->typeId = u;
                            e->page = (unsigned char)parser.current->FUN_004c46c0("MENU", 0);
                            e->slot = (unsigned char)parser.current->FUN_004c46c0("BUTTON", 0);
                            parser.current->FUN_004c48c0(e->name, "UNITNAME", 0x20, DAT_005119b8);
                            break;
                        }
                    }
                }
                j++;
            }
        }
    }

    FUN_004d8780(g_game->unitDefs);
    for (unsigned short u = 0; u < g_game->unitDefCount; u++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < g_game->buildLists[i].count; j++) {
                if (g_game->buildLists[i].entries[j].typeId == u) {
                    unsigned char page = g_game->buildLists[i].entries[j].page;
                    if (g_game->unitDefs[u].field_22e < page)
                        g_game->unitDefs[u].field_22e = page;
                }
            }
        }
    }
    FUN_004d8710(g_game->unitDefs);

    for (int k = 0; k < g_game->unitDefCount; k++) {
        UnitDef_0042dcf0* def = &g_game->unitDefs[k];
        char* name = def->name;
        for (int i = 0; i < g_game->buildListCount; i++) {
            if (_strcmpi(g_game->buildLists[i].entries[0].name, name) == 0
                && !def->flags_241.downloadable) {
                char buf[128];
                sprintf(buf, "Hey!  Somebody forgot to set downloadable=1 for %s", name);
                FUN_004d8780(g_game->unitDefs);
                def->flags_241.downloadable = 1;
                FUN_004d8710(g_game->unitDefs);
            }
        }
    }

    FUN_0042be30();
}
