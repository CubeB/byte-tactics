// Decompiled by deepseek-v4.1-flash, finished by GPT-6, finished by space-bunny-free, edited by deepseek-v4.1. Names are provisional.
// Edited by deepseek-v4.1: MATCH. Fixes this round:
// 1. `int c; int i;` are declared at the top of the body, before `int n = files.size();`,
//    and the last two loops reuse that one counter `c`. A group of scalar locals gets
//    its esp slots in reverse declaration order, so with the counters declared after n
//    the file count n landed in [esp+0x18]/EBP instead of the original [esp+0x10]/EBX;
//    declaring them first moved n to 0x10/EBX and cascaded the whole group (the max-page
//    scan and the downloadable scan, g_game into EDI, the lea esi,[ebx+eax] addressing),
//    lifting 77.6 -> 97.3.
// 2. The downloadable scan (0x42e037-0x42e0bf) kept coming out with the build-list
//    counter in EBX and the i*0xbd byte offset in EBP, the exact inverse of the
//    original (counter EBP, offset EBX, `inc ebp`; `add ebx,0xbd`; `cmp ebp,[eax+..]`).
//    The register assignment follows the order the loop variables are created, mapped
//    onto ESI, EDI, EBX, EBP (the four callee-saved registers live across the _strcmpi
//    call here), so the offset has to be a real source variable created before the
//    counter. Declaring `int off = 0;` and walking with it (`i++, off += 0xbd`,
//    `((BuildList_0042dcf0*)((char*)g_game->buildLists + off))->entries[0].name`)
//    makes the strength-reduction temp disappear and gives offset EBX / counter EBP.
// 3. `defs[c].name` is used directly at both call sites instead of a `char* name`
//    local: that drops one live-across-call variable, so the hoisted name temp lands
//    in EDI (the `lea edi,[esi-0x221]` in the preheader) instead of competing for EBX.
// Note 0x4c48c0 and 0x4c46c0 are two different classes in data/symbols.csv
// (Class_004c48c0::FUN_004c48c0 and Class_004c46c0::FUN_004c46c0), so `current`
// is a Class_004c48c0* and the int-arg calls cast it to Class_004c46c0*.

#include <math.h>
#include <vector>

class Class_004c9390 {
public:
    char* data;

    void ReleaseRef();
};

class Class_004c91a0 {
public:
    char* p;

    ~Class_004c91a0() { ((Class_004c9390*)this)->ReleaseRef(); }
};

class Class_004c48c0 {
public:
    int FUN_004c48c0(char* dst, const char* key, int size, char* def);
};

class Class_004c46c0 {
public:
    int FUN_004c46c0(const char* name, int def);
};

class Class_004c2ea0 {
public:
    int field_0;                       // +0x0
    Class_004c48c0* current;           // +0x4
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

struct Game {
    char unknown_0[0x1438f];
    int unitDefCount;                  // +0x1438f
    char unknown_14393[0x1439b - 0x14393];
    UnitDef_0042dcf0* unitDefs;        // +0x1439b
    char unknown_1439f[0x391c7 - 0x1439f];
    int buildListCount;                // +0x391c7
    BuildList_0042dcf0* buildLists;    // +0x391cb
};
#pragma pack(pop)

extern Game* g_game;
extern char DAT_005119b8[];

void __stdcall FUN_004290f0(char* out, const char* dir, const char* name, const char* ext);
void __stdcall FUN_004bca30(const char* pattern, int flags, std::vector<Class_004c91a0>* out);
void* __cdecl FUN_004d83b0(const char* name, int size);
void __cdecl FUN_004d8780(void* p);
void __cdecl FUN_004d8710(void* p);
void __cdecl AddDownloadBuildOptions();

// FUNCTION: 0x42dcf0
void LoadDownloadMenus()
{
    int c;
    int i;
    char path[256];
    char unitbuf[256];
    std::vector<Class_004c91a0> files;
    FUN_004290f0(path, "download", "*", "TDF");
    FUN_004bca30(path, 0, &files);

    int n = files.size();
    g_game->buildListCount = n;
    g_game->buildLists = (BuildList_0042dcf0*)FUN_004d83b0("DOWNLOADMENU", n * 0xbd);

    for (i = 0; i < n; i++) {
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
                            g_game->buildLists[i].entries[j].typeId = u;
                            g_game->buildLists[i].entries[j].page = (unsigned char)((Class_004c46c0*)parser.current)->FUN_004c46c0("MENU", 0);
                            g_game->buildLists[i].entries[j].slot = (unsigned char)((Class_004c46c0*)parser.current)->FUN_004c46c0("BUTTON", 0);
                            parser.current->FUN_004c48c0(g_game->buildLists[i].entries[j].name, "UNITNAME", 0x20, DAT_005119b8);
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
        for (c = 0; c < n; c++) {
            for (int d = 0; d < g_game->buildLists[c].count; d++) {
                if (g_game->buildLists[c].entries[d].typeId == u) {
                    if (g_game->unitDefs[u].field_22e < g_game->buildLists[c].entries[d].page)
                        g_game->unitDefs[u].field_22e = g_game->buildLists[c].entries[d].page;
                }
            }
        }
    }
    FUN_004d8710(g_game->unitDefs);

    UnitDef_0042dcf0* defs = g_game->unitDefs;
    for (c = 0; c < g_game->unitDefCount; c++) {
        for (int i = 0; i < g_game->buildListCount; i++) {
            if (_strcmpi(g_game->buildLists[i].entries[0].name, defs[c].name) == 0
                && !defs[c].flags_241.downloadable) {
                char buf[128];
                sprintf(buf, "Hey!  Somebody forgot to set downloadable=1 for %s", defs[c].name);
                FUN_004d8780(g_game->unitDefs);
                defs[c].flags_241.downloadable = 1;
                FUN_004d8710(g_game->unitDefs);
            }
        }
    }

    AddDownloadBuildOptions();
}
