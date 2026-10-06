// Decompiled by Opus, Haiku, space-bunny-free. Names are provisional.

#include <stdio.h>
#include <string.h>

class TdfFile;

class TdfRecord {
public:
    int GetFieldString(char* dst, const char* key, size_t size, const char* def);
    int GetFieldInt(const char* name, int def);
};

class TdfFile {
public:
    int field_0;
    TdfRecord* current;            // +0x4
    int field_8;
    TdfFile();
    ~TdfFile();
    int LoadFile(char* file);
    void Unload();
    int SelectRecord(char* name);
    void ResetCurrentRecord();
    int GetCurrentRecord();
    void SetCurrentRecord(int saved);
};

struct Obj_00431950 {
    char unknown_0[4];
    TdfRecord* table;                   // +0x4
};

// One x1/y1/x2/y2 block of the TDF section.
struct Rect_00431a60 {
    int x1;                             // +0x0
    int y1;                             // +0x4
    int x2;                             // +0x8
    int y2;                             // +0xc
};

#pragma pack(push, 1)
struct Side_00431a60 {                  // 0x232 bytes
    char name[0x1e];                    // +0x00
    char nameprefix[4];                 // +0x1e
    char commander[0x20];               // +0x22
    Rect_00431a60 logo;                 // +0x42
    Rect_00431a60 energyBar;            // +0x52
    Rect_00431a60 energyNum;            // +0x62
    Rect_00431a60 metalBar;             // +0x72
    Rect_00431a60 metalNum;             // +0x82
    Rect_00431a60 totalUnits;           // +0x92
    Rect_00431a60 totalTime;            // +0xa2
    Rect_00431a60 energyMax;            // +0xb2
    Rect_00431a60 metalMax;             // +0xc2
    Rect_00431a60 energy0;              // +0xd2
    Rect_00431a60 metal0;               // +0xe2
    Rect_00431a60 energyProduced;       // +0xf2
    Rect_00431a60 energyConsumed;       // +0x102
    Rect_00431a60 metalProduced;        // +0x112
    Rect_00431a60 metalConsumed;        // +0x122
    Rect_00431a60 logo2;                // +0x132
    Rect_00431a60 unitName;             // +0x142
    Rect_00431a60 damageBar;            // +0x152
    Rect_00431a60 unitEnergyMake;       // +0x162
    Rect_00431a60 unitEnergyUse;        // +0x172
    Rect_00431a60 unitMetalMake;        // +0x182
    Rect_00431a60 unitMetalUse;         // +0x192
    Rect_00431a60 missionText;          // +0x1a2
    Rect_00431a60 unitName2;            // +0x1b2
    Rect_00431a60 damageBar2;           // +0x1c2
    Rect_00431a60 nameBlock;            // +0x1d2
    Rect_00431a60 description;          // +0x1e2
    Rect_00431a60 reload[3];            // +0x1f2
    int energyColor;                    // +0x222
    int metalColor;                     // +0x226
    int sideNumber;                     // +0x22a
    void* font;                         // +0x22e
};

struct Entry_00431a20 {
    int* buffer;                       // +0x0
    char unknown_4[0x22e];
};

struct Game {
    char unknown_0[0x148d7];
    union {
        void* logos;                   // +0x148d7
        int field_148d7;
    };
    void* logos32;                     // +0x148db
    char unknown_148df[0x37f39 - 0x148df];
    union {
        struct {
            int sideCount;             // +0x37f39
            Side_00431a60 sides[2];    // +0x37f3d
        };
        struct {
            char pad_00431a20[0x3816b - 0x37f39];
            Entry_00431a20 entries[5]; // +0x3816b
        };
    };
};
#pragma pack(pop)

extern Game* g_game;
extern char DAT_005119b8[];

void __stdcall BuildDataPath(char* out, const char* dir, const char* name, const char* ext);
void* __stdcall LoadGaf(char* path);
void* __stdcall FindGafEntry(void* gaf, const char* name);
void __stdcall FatalError(char* message);
void __cdecl FUN_004d85a0(int* param_1);
void __stdcall ReadSideRect(Obj_00431950* obj, int* out, char* name, char* side);
void* __stdcall HAPI_LoadFile(char* path, int flags);

// FUNCTION: 0x4318c0
void LoadLogos()
{
    char path[256];
    BuildDataPath(path, "textures", "logos", "GAF");
    g_game->logos = LoadGaf(path);
    g_game->logos32 = FindGafEntry(g_game->logos, "32xlogos");
}

// FUNCTION: 0x431920
void FUN_00431920(void)
{
    int val = g_game->field_148d7;
    FUN_004d85a0((int*)val);
    g_game->field_148d7 = 0;
}

// Parses the x1/y1/x2/y2 fields of the current TDF node into four ints.
// On a missing node it reports a fatal error and restores the cursor.
// FUNCTION: 0x431950
void __stdcall ReadSideRect(Obj_00431950* obj, int* out, char* name, char* side)
{
    int saved = ((TdfFile*)obj)->GetCurrentRecord();
    if (!((TdfFile*)obj)->SelectRecord(name)) {
        char buf[256];
        sprintf(buf, "No [%s] in GAMEDATA/SIDEDATA.TDF for side:%s", name, side);
        FatalError(buf);
        ((TdfFile*)obj)->SetCurrentRecord(saved);
        return;
    }
    out[0] = obj->table->GetFieldInt("x1", 0);
    out[1] = obj->table->GetFieldInt("y1", 0);
    out[2] = obj->table->GetFieldInt("x2", 0);
    out[3] = obj->table->GetFieldInt("y2", 0);
    ((TdfFile*)obj)->SetCurrentRecord(saved);
}

// Frees the buffer of each of the five entries at g_game+0x3816b and clears
// the pointers.
// FUNCTION: 0x431a20
void FUN_00431a20()
{
    for (int i = 0; i < 5; i++) {
        int*& buffer = g_game->entries[i].buffer;
        if (buffer) {
            FUN_004d85a0(buffer);
            buffer = 0;
        }
    }
}

// This file of the original was built with /Gz, so the function is __stdcall.
// Declared __cdecl it scores 95.4%: 34 calls schedule the reload of
// parser.current before their `push 0` instead of after it.
// Loads gamedata\sidedata.tdf. For every SIDE<n> section, n counting from 0
// until the section is missing, it reads the side's name, name prefix,
// commander name and font file (LoadFontByName's body inlined), its two colours
// and eleven x1/y1/x2/y2 rectangles, then hands sixteen more rectangles and
// the three RELOAD<n> rectangles to the shared reader ReadSideRect. The
// number of sections found is left in g_game+0x37f39.
//
// Three source shapes the match depends on, none of them obvious:
//  - `int* r = &s->field; r[0]..r[3]` rather than named fields is what selects
//    the biased side pointer. With `s->logo.x1 = ...` MSVC 5 chose
//    `ebx = side+0x22`; with the `int*` form it chose `side+0x4a`, the original's
//    bias. Worth 22 points. The `&s->field` address expression being present is
//    what makes the back end pick that middle-field bias.
//  - `while (1)` with a `break` inside is NOT rotated, while `for (init;; incr)`
//    is. `for (side = 0;; side++)` made MSVC rotate into test-at-bottom form
//    and peel the first iteration, which pushed edi/ebx inside the loop instead
//    of into the prologue and shifted the whole frame. `while (1)` reproduced
//    the unrotated loop, the four-register prologue and the exact
//    `sub esp,0xd14` with the parser at `[esp+0x10]`.
//  - Frame slack tells you the loop shape: a peeled loop leaves 8 unused bytes
//    at the bottom of the frame, the unpeeled one leaves none, and the original
//    has none.
//
// The eleven per-block error buffers are declared in the source in a different
// order from the one the frame uses. The frame order is ENERGYNUM, TOTALUNITS,
// TOTALTIME, LOGO, ENERGY0, METALNUM, METAL0, METALBAR, ENERGYMAX, ENERGYBAR,
// METALMAX. That is deliberate and load-bearing; do not "tidy" it.
//
// Suspected original bug, unconfirmed: the store `s->sideNumber = side` at
// +0x22a writes the loop counter, so the field is 0 for side 0 and the index
// otherwise. If that field is meant to hold something else, this is where the
// source is wrong. I could not confirm its meaning from the rest of the repo;
// 0x476830, 0x476a60 and 0x496ee0 only read the name at +0.
// FUNCTION: 0x431a60
void __stdcall LoadSideData(void)
{
    TdfFile parser;
    int side;
    char name[0x100];
    char fontPath[0x100];
    char msgEnergynum[0x100];
    char msgTotalunits[0x100];
    char msgTotaltime[0x100];
    char msgLogo[0x100];
    char msgEnergy0[0x100];
    char msgMetalnum[0x100];
    char msgMetal0[0x100];
    char msgMetalbar[0x100];
    char msgEnergymax[0x100];
    char msgEnergybar[0x100];
    char msgMetalmax[0x100];
    Side_00431a60* s;

    BuildDataPath(name, "gamedata", "sidedata", "TDF");
    parser.LoadFile(name);
    s = g_game->sides;
    side = 0;
    while (1) {
        int saved;
        s->sideNumber = side;
        sprintf(name, "SIDE%d", side);
        parser.ResetCurrentRecord();
        if (!parser.SelectRecord(name))
            break;
        if (parser.current->GetFieldString(name, "name", 0x1e, DAT_005119b8))
            strcpy(s->name, name);
        if (parser.current->GetFieldString(name, "nameprefix", 4, DAT_005119b8))
            strcpy(s->nameprefix, name);
        if (parser.current->GetFieldString(name, "commander", 0x20, DAT_005119b8))
            strcpy(s->commander, name);
        if (parser.current->GetFieldString(name, "font", 0x100, DAT_005119b8)) {
            void* font;
            BuildDataPath(fontPath, "fonts", name, "FNT");
            font = HAPI_LoadFile(fontPath, 0);
            if (font == 0)
                FatalError(fontPath);
            s->font = font;
        }
        s->energyColor = parser.current->GetFieldInt("energycolor", 0);
        s->metalColor = parser.current->GetFieldInt("metalcolor", 0);

        saved = parser.GetCurrentRecord();
        if (!parser.SelectRecord("LOGO")) {
            sprintf(msgLogo, "No [%s] in GAMEDATA/SIDEDATA.TDF for side:%s", "LOGO", s->name);
            FatalError(msgLogo);
        } else {
            int* r = &s->logo.x1;
            r[0] = parser.current->GetFieldInt("x1", 0);
            r[1] = parser.current->GetFieldInt("y1", 0);
            r[2] = parser.current->GetFieldInt("x2", 0);
            r[3] = parser.current->GetFieldInt("y2", 0);
        }
        parser.SetCurrentRecord(saved);

        saved = parser.GetCurrentRecord();
        if (!parser.SelectRecord("ENERGYBAR")) {
            sprintf(msgEnergybar, "No [%s] in GAMEDATA/SIDEDATA.TDF for side:%s", "ENERGYBAR", s->name);
            FatalError(msgEnergybar);
        } else {
            int* r = &s->energyBar.x1;
            r[0] = parser.current->GetFieldInt("x1", 0);
            r[1] = parser.current->GetFieldInt("y1", 0);
            r[2] = parser.current->GetFieldInt("x2", 0);
            r[3] = parser.current->GetFieldInt("y2", 0);
        }
        parser.SetCurrentRecord(saved);

        saved = parser.GetCurrentRecord();
        if (!parser.SelectRecord("ENERGYNUM")) {
            sprintf(msgEnergynum, "No [%s] in GAMEDATA/SIDEDATA.TDF for side:%s", "ENERGYNUM", s->name);
            FatalError(msgEnergynum);
        } else {
            int* r = &s->energyNum.x1;
            r[0] = parser.current->GetFieldInt("x1", 0);
            r[1] = parser.current->GetFieldInt("y1", 0);
            r[2] = parser.current->GetFieldInt("x2", 0);
            r[3] = parser.current->GetFieldInt("y2", 0);
        }
        parser.SetCurrentRecord(saved);

        saved = parser.GetCurrentRecord();
        if (!parser.SelectRecord("METALBAR")) {
            sprintf(msgMetalbar, "No [%s] in GAMEDATA/SIDEDATA.TDF for side:%s", "METALBAR", s->name);
            FatalError(msgMetalbar);
        } else {
            int* r = &s->metalBar.x1;
            r[0] = parser.current->GetFieldInt("x1", 0);
            r[1] = parser.current->GetFieldInt("y1", 0);
            r[2] = parser.current->GetFieldInt("x2", 0);
            r[3] = parser.current->GetFieldInt("y2", 0);
        }
        parser.SetCurrentRecord(saved);

        saved = parser.GetCurrentRecord();
        if (!parser.SelectRecord("METALNUM")) {
            sprintf(msgMetalnum, "No [%s] in GAMEDATA/SIDEDATA.TDF for side:%s", "METALNUM", s->name);
            FatalError(msgMetalnum);
        } else {
            int* r = &s->metalNum.x1;
            r[0] = parser.current->GetFieldInt("x1", 0);
            r[1] = parser.current->GetFieldInt("y1", 0);
            r[2] = parser.current->GetFieldInt("x2", 0);
            r[3] = parser.current->GetFieldInt("y2", 0);
        }
        parser.SetCurrentRecord(saved);

        saved = parser.GetCurrentRecord();
        if (!parser.SelectRecord("TOTALUNITS")) {
            sprintf(msgTotalunits, "No [%s] in GAMEDATA/SIDEDATA.TDF for side:%s", "TOTALUNITS", s->name);
            FatalError(msgTotalunits);
        } else {
            int* r = &s->totalUnits.x1;
            r[0] = parser.current->GetFieldInt("x1", 0);
            r[1] = parser.current->GetFieldInt("y1", 0);
            r[2] = parser.current->GetFieldInt("x2", 0);
            r[3] = parser.current->GetFieldInt("y2", 0);
        }
        parser.SetCurrentRecord(saved);

        saved = parser.GetCurrentRecord();
        if (!parser.SelectRecord("TOTALTIME")) {
            sprintf(msgTotaltime, "No [%s] in GAMEDATA/SIDEDATA.TDF for side:%s", "TOTALTIME", s->name);
            FatalError(msgTotaltime);
        } else {
            int* r = &s->totalTime.x1;
            r[0] = parser.current->GetFieldInt("x1", 0);
            r[1] = parser.current->GetFieldInt("y1", 0);
            r[2] = parser.current->GetFieldInt("x2", 0);
            r[3] = parser.current->GetFieldInt("y2", 0);
        }
        parser.SetCurrentRecord(saved);

        saved = parser.GetCurrentRecord();
        if (!parser.SelectRecord("ENERGY0")) {
            sprintf(msgEnergy0, "No [%s] in GAMEDATA/SIDEDATA.TDF for side:%s", "ENERGY0", s->name);
            FatalError(msgEnergy0);
        } else {
            int* r = &s->energy0.x1;
            r[0] = parser.current->GetFieldInt("x1", 0);
            r[1] = parser.current->GetFieldInt("y1", 0);
            r[2] = parser.current->GetFieldInt("x2", 0);
            r[3] = parser.current->GetFieldInt("y2", 0);
        }
        parser.SetCurrentRecord(saved);

        saved = parser.GetCurrentRecord();
        if (!parser.SelectRecord("METAL0")) {
            sprintf(msgMetal0, "No [%s] in GAMEDATA/SIDEDATA.TDF for side:%s", "METAL0", s->name);
            FatalError(msgMetal0);
        } else {
            int* r = &s->metal0.x1;
            r[0] = parser.current->GetFieldInt("x1", 0);
            r[1] = parser.current->GetFieldInt("y1", 0);
            r[2] = parser.current->GetFieldInt("x2", 0);
            r[3] = parser.current->GetFieldInt("y2", 0);
        }
        parser.SetCurrentRecord(saved);

        saved = parser.GetCurrentRecord();
        if (!parser.SelectRecord("ENERGYMAX")) {
            sprintf(msgEnergymax, "No [%s] in GAMEDATA/SIDEDATA.TDF for side:%s", "ENERGYMAX", s->name);
            FatalError(msgEnergymax);
        } else {
            int* r = &s->energyMax.x1;
            r[0] = parser.current->GetFieldInt("x1", 0);
            r[1] = parser.current->GetFieldInt("y1", 0);
            r[2] = parser.current->GetFieldInt("x2", 0);
            r[3] = parser.current->GetFieldInt("y2", 0);
        }
        parser.SetCurrentRecord(saved);

        saved = parser.GetCurrentRecord();
        if (!parser.SelectRecord("METALMAX")) {
            sprintf(msgMetalmax, "No [%s] in GAMEDATA/SIDEDATA.TDF for side:%s", "METALMAX", s->name);
            FatalError(msgMetalmax);
        } else {
            int* r = &s->metalMax.x1;
            r[0] = parser.current->GetFieldInt("x1", 0);
            r[1] = parser.current->GetFieldInt("y1", 0);
            r[2] = parser.current->GetFieldInt("x2", 0);
            r[3] = parser.current->GetFieldInt("y2", 0);
        }
        parser.SetCurrentRecord(saved);

        ReadSideRect((Obj_00431950*)&parser, &s->energyProduced.x1, "ENERGYPRODUCED", s->name);
        ReadSideRect((Obj_00431950*)&parser, &s->energyConsumed.x1, "ENERGYCONSUMED", s->name);
        ReadSideRect((Obj_00431950*)&parser, &s->metalProduced.x1, "METALPRODUCED", s->name);
        ReadSideRect((Obj_00431950*)&parser, &s->metalConsumed.x1, "METALCONSUMED", s->name);
        ReadSideRect((Obj_00431950*)&parser, &s->logo2.x1, "LOGO2", s->name);
        ReadSideRect((Obj_00431950*)&parser, &s->unitName.x1, "UNITNAME", s->name);
        ReadSideRect((Obj_00431950*)&parser, &s->damageBar.x1, "DAMAGEBAR", s->name);
        ReadSideRect((Obj_00431950*)&parser, &s->unitMetalMake.x1, "UNITMETALMAKE", s->name);
        ReadSideRect((Obj_00431950*)&parser, &s->unitMetalUse.x1, "UNITMETALUSE", s->name);
        ReadSideRect((Obj_00431950*)&parser, &s->unitEnergyMake.x1, "UNITENERGYMAKE", s->name);
        ReadSideRect((Obj_00431950*)&parser, &s->unitEnergyUse.x1, "UNITENERGYUSE", s->name);
        ReadSideRect((Obj_00431950*)&parser, &s->missionText.x1, "MISSIONTEXT", s->name);
        ReadSideRect((Obj_00431950*)&parser, &s->unitName2.x1, "UNITNAME2", s->name);
        ReadSideRect((Obj_00431950*)&parser, &s->damageBar2.x1, "DAMAGEBAR2", s->name);
        ReadSideRect((Obj_00431950*)&parser, &s->nameBlock.x1, "NAME", s->name);
        ReadSideRect((Obj_00431950*)&parser, &s->description.x1, "DESCRIPTION", s->name);
        {
            int n = 3;
            int k = 1;
            while (n) {
                sprintf(name, "RELOAD%d", k);
                ReadSideRect((Obj_00431950*)&parser, &s->reload[k - 1].x1, name, s->name);
                n--;
                k++;
            }
        }
        side++;
        s++;
    }
    parser.Unload();
    g_game->sideCount = side;
}
