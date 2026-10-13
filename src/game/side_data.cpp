// Decompiled by Opus, Haiku, space-bunny-free. Names are provisional.

#include <stdio.h>
#include <string.h>

class TdfFile;

#include "../util/tdf.h"
#include "../graphics/rect.h"

#pragma pack(push, 1)
struct Side_00431a60 {                  // 0x232 bytes
    char name[0x1e];                    // +0x00
    char nameprefix[4];                 // +0x1e
    char commander[0x20];               // +0x22
    Rect logo;                          // +0x42
    Rect energyBar;                     // +0x52
    Rect energyNum;                     // +0x62
    Rect metalBar;                      // +0x72
    Rect metalNum;                      // +0x82
    Rect totalUnits;                    // +0x92
    Rect totalTime;                     // +0xa2
    Rect energyMax;                     // +0xb2
    Rect metalMax;                      // +0xc2
    Rect energy0;                       // +0xd2
    Rect metal0;                        // +0xe2
    Rect energyProduced;                // +0xf2
    Rect energyConsumed;                // +0x102
    Rect metalProduced;                 // +0x112
    Rect metalConsumed;                 // +0x122
    Rect logo2;                         // +0x132
    Rect unitName;                      // +0x142
    Rect damageBar;                     // +0x152
    Rect unitEnergyMake;                // +0x162
    Rect unitEnergyUse;                 // +0x172
    Rect unitMetalMake;                 // +0x182
    Rect unitMetalUse;                  // +0x192
    Rect missionText;                   // +0x1a2
    Rect unitName2;                     // +0x1b2
    Rect damageBar2;                    // +0x1c2
    Rect nameBlock;                     // +0x1d2
    Rect description;                   // +0x1e2
    Rect reload[3];                     // +0x1f2
    int energyColor;                    // +0x222
    int metalColor;                     // +0x226
    int sideNumber;                     // +0x22a
    void* font;                         // +0x22e
};

struct Game {
    char unknown_0[0x148d7];
    union {
        void* logos;                   // +0x148d7
        int field_148d7;
    };
    void* logos32;                     // +0x148db
    char unknown_148df[0x37f39 - 0x148df];
    int sideCount;                     // +0x37f39
    Side_00431a60 sideDefs[5];         // +0x37f3d, stride 0x232
};
#pragma pack(pop)

extern Game* g_game;
extern char DAT_005119b8[];

void __stdcall BuildDataPath(char* out, const char* dir, const char* name, const char* ext);
void* __stdcall LoadGaf(char* path);
void* __stdcall FindGafEntry(void* gaf, const char* name);
void __stdcall FatalError(char* message);
void __cdecl GameFreeThunk(int* p);
void __stdcall ReadSideRect(TdfFile* obj, int* out, char* name, char* side);
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
void FreeLogos(void)
{
    int val = g_game->field_148d7;
    GameFreeThunk((int*)val);
    g_game->field_148d7 = 0;
}

// Parses the x1/y1/x2/y2 fields of the current TDF node into four ints.
// On a missing node it reports a fatal error and restores the cursor.
// FUNCTION: 0x431950
void __stdcall ReadSideRect(TdfFile* obj, int* out, char* name, char* side)
{
    int saved = obj->GetCurrentRecord();
    if (!obj->SelectRecord(name)) {
        char buf[256];
        sprintf(buf, "No [%s] in GAMEDATA/SIDEDATA.TDF for side:%s", name, side);
        FatalError(buf);
        obj->SetCurrentRecord(saved);
        return;
    }
    out[0] = obj->current->GetFieldInt("x1", 0);
    out[1] = obj->current->GetFieldInt("y1", 0);
    out[2] = obj->current->GetFieldInt("x2", 0);
    out[3] = obj->current->GetFieldInt("y2", 0);
    obj->SetCurrentRecord(saved);
}

// Frees the font of each of the five sides at g_game+0x37f3d and clears the
// pointers.
// FUNCTION: 0x431a20
void FreeSideFonts()
{
    for (int i = 0; i < 5; i++) {
        // The reference is load-bearing: it keeps FreeSideFonts at 59 bytes.
        int*& buffer = (int*&)g_game->sideDefs[i].font;
        if (buffer) {
            GameFreeThunk(buffer);
            buffer = 0;
        }
    }
}

// The x1/y1/x2/y2 block of one section; msg is the caller's buffer, which
// keeps each inlined expansion's string on its own stack slot.
static inline void LoadSideRect(TdfFile* parser, int* out, char* name, char* side, char* msg)
{
    int saved = parser->GetCurrentRecord();
    if (!parser->SelectRecord(name)) {
        sprintf(msg, "No [%s] in GAMEDATA/SIDEDATA.TDF for side:%s", name, side);
        FatalError(msg);
    } else {
        out[0] = parser->current->GetFieldInt("x1", 0);
        out[1] = parser->current->GetFieldInt("y1", 0);
        out[2] = parser->current->GetFieldInt("x2", 0);
        out[3] = parser->current->GetFieldInt("y2", 0);
    }
    parser->SetCurrentRecord(saved);
}

// Loads gamedata\sidedata.tdf. For every SIDE<n> section, n counting from 0
// until the section is missing, it reads the side's name, name prefix,
// commander name and font file (LoadFontByName's body inlined), its two colours
// and eleven x1/y1/x2/y2 rectangles, then hands sixteen more rectangles and
// the three RELOAD<n> rectangles to the shared reader ReadSideRect. The
// number of sections found is left in g_game+0x37f39.
//
// Suspected original bug, unconfirmed: the store `s->sideNumber = side` at
// +0x22a writes the loop counter, so the field is 0 for side 0 and the index
// otherwise. If that field is meant to hold something else, this is where the
// source is wrong. I could not confirm its meaning from the rest of the repo;
// 0x476830, 0x476a60 and 0x496ee0 only read the name at +0.
// __stdcall because the original file was built with /Gz.
// FUNCTION: 0x431a60
void __stdcall LoadSideData(void)
{
    TdfFile parser;
    int side;
    char name[0x100];
    char fontPath[0x100];
    // Declaration order is deliberate and differs from the frame order: do not
    // reorder.
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
    s = g_game->sideDefs;
    side = 0;
    // `while (1)`, not `for (;; side++)`: a for loop is rotated and peeled.
    while (1) {
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

        LoadSideRect(&parser, &s->logo.left, "LOGO", s->name, msgLogo);

        LoadSideRect(&parser, &s->energyBar.left, "ENERGYBAR", s->name, msgEnergybar);

        LoadSideRect(&parser, &s->energyNum.left, "ENERGYNUM", s->name, msgEnergynum);

        LoadSideRect(&parser, &s->metalBar.left, "METALBAR", s->name, msgMetalbar);

        LoadSideRect(&parser, &s->metalNum.left, "METALNUM", s->name, msgMetalnum);

        LoadSideRect(&parser, &s->totalUnits.left, "TOTALUNITS", s->name, msgTotalunits);

        LoadSideRect(&parser, &s->totalTime.left, "TOTALTIME", s->name, msgTotaltime);

        LoadSideRect(&parser, &s->energy0.left, "ENERGY0", s->name, msgEnergy0);

        LoadSideRect(&parser, &s->metal0.left, "METAL0", s->name, msgMetal0);

        LoadSideRect(&parser, &s->energyMax.left, "ENERGYMAX", s->name, msgEnergymax);

        LoadSideRect(&parser, &s->metalMax.left, "METALMAX", s->name, msgMetalmax);

        ReadSideRect(&parser, &s->energyProduced.left, "ENERGYPRODUCED", s->name);
        ReadSideRect(&parser, &s->energyConsumed.left, "ENERGYCONSUMED", s->name);
        ReadSideRect(&parser, &s->metalProduced.left, "METALPRODUCED", s->name);
        ReadSideRect(&parser, &s->metalConsumed.left, "METALCONSUMED", s->name);
        ReadSideRect(&parser, &s->logo2.left, "LOGO2", s->name);
        ReadSideRect(&parser, &s->unitName.left, "UNITNAME", s->name);
        ReadSideRect(&parser, &s->damageBar.left, "DAMAGEBAR", s->name);
        ReadSideRect(&parser, &s->unitMetalMake.left, "UNITMETALMAKE", s->name);
        ReadSideRect(&parser, &s->unitMetalUse.left, "UNITMETALUSE", s->name);
        ReadSideRect(&parser, &s->unitEnergyMake.left, "UNITENERGYMAKE", s->name);
        ReadSideRect(&parser, &s->unitEnergyUse.left, "UNITENERGYUSE", s->name);
        ReadSideRect(&parser, &s->missionText.left, "MISSIONTEXT", s->name);
        ReadSideRect(&parser, &s->unitName2.left, "UNITNAME2", s->name);
        ReadSideRect(&parser, &s->damageBar2.left, "DAMAGEBAR2", s->name);
        ReadSideRect(&parser, &s->nameBlock.left, "NAME", s->name);
        ReadSideRect(&parser, &s->description.left, "DESCRIPTION", s->name);
        {
            int n = 3;
            int k = 1;
            while (n) {
                sprintf(name, "RELOAD%d", k);
                ReadSideRect(&parser, &s->reload[k - 1].left, name, s->name);
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
