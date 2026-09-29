// Decompiled by space-bunny-free. Names are provisional.
// This file of the original was built with /Gz, so the function is __stdcall;
// found by the orchestrator's calling-convention sweep of every partial.
// Loads gamedata\sidedata.tdf. For every SIDE<n> section, n counting from 0
// until the section is missing, it reads the side's name, name prefix,
// commander name and font file (FUN_004292e0's body inlined), its two colours
// and eleven x1/y1/x2/y2 rectangles, then hands sixteen more rectangles and
// the three RELOAD<n> rectangles to the shared reader FUN_00431950. The
// number of sections found is left in g_game+0x37f39.
//
// NOT MATCHING: 95.4 percent, and the byte count is already exact (2737 against
// 2737). Do not read that as "nearly there": the whole remaining difference is
// the order of two instructions at 34 sites, and the reconstruction below is
// otherwise complete.
//
// The remaining difference, precisely: at 34 of the calls the original emits
//   push 0
//   mov ecx, [esp+0x18]        ; reload of parser.current
// and this file emits the reload first and the `push 0` second. Same address,
// same four bytes, only the schedule of two adjacent instructions differs. 33
// of the 34 are the y1/x2/y2 calls of the eleven blocks plus the metalColor
// read; the first call of each block and the energyColor read already match.
// The byte sequence `6a 00 / 8b 4c 24 18` occurs ONLY in this function
// anywhere in the exe, so whatever produces it is local to this code and the
// trigger was not found.
//
// Ruled out for it, each compiled and scored: a redundant cast on the receiver,
// `&parser` plus a field, an explicit `*(Class_004c46c0**)&parser.current`, a
// `Class_004c46c0* cur` local (which drops the file to 89.1%, so it changes
// other things too), a reference `Class_004c46c0*&`, `*(r+n)` instead of
// `r[n]`, reading the four values into temporaries first, a `static inline`
// getter for the current node, a `static inline` helper that performs the whole
// call, and a `static inline LoadRect(parser, int* r)` helper. That last one
// scores the same 95.4% and is the tidiest source, so it is what the file uses
// even though it does not fix the bytes.
//
// Where to look next: a variant sweep in a single file, compiling every
// candidate spelling of the four calls as its own copy of the whole function
// and disassembling them together, hunting for the one that produces the
// `push 0` before the reload. The harness is in build/scratch/0x431a60/ and
// works (asm.sh, shape.sh, score.sh); the sweep file sweep1.cpp was started and
// not finished. Two candidate triggers worth trying: making the default
// argument something other than a bare literal 0 (a named constant, or a 0
// reached through a `const int` local), since the reload's position may follow
// where the argument value is materialised; and making the receiver a base-class
// subobject of a derived node class.
//
// Three findings from this pass that are worth keeping, since each was worth
// many points and none is obvious:
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
#include <stdio.h>
#include <string.h>

class Class_004c2f60 {
public:
    int FUN_004c2f60(char* file);
};

class Class_004c3240 {
public:
    void FUN_004c3240();
};

class Class_004c3410 {
public:
    int FUN_004c3410(char* name);
};

class Class_004c3e10 {
public:
    void FUN_004c3e10();
};

class Class_004c3e20 {
public:
    int FUN_004c3e20();
};

class Class_004c3e30 {
public:
    void FUN_004c3e30(int saved);
};

class Class_004c46c0 {
public:
    int FUN_004c46c0(const char* name, int def);
};

class Class_004c48c0 {
public:
    int FUN_004c48c0(char* dst, const char* key, size_t size, const char* def);
};

class Class_004c2ea0 {
public:
    int field_0;
    Class_004c46c0* current;            // +0x4
    int field_8;
    Class_004c2ea0();
    ~Class_004c2ea0();
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

struct Game_00431a60 {
    char unknown_0[0x37f39];
    int sideCount;                      // +0x37f39
    Side_00431a60 sides[2];             // +0x37f3d
};
#pragma pack(pop)

extern Game_00431a60* g_game;
extern char DAT_005119b8[];

void __stdcall FUN_004290f0(char* out, const char* dir, const char* name, const char* ext);
void __stdcall FUN_004b6290(char* message);
void* __stdcall FUN_004bbe50(char* path, int flags);
void __stdcall FUN_00431950(void* parser, int* out, const char* name, const char* side);

// FUNCTION: 0x431a60
void __stdcall FUN_00431a60(void)
{
    Class_004c2ea0 parser;
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

    FUN_004290f0(name, "gamedata", "sidedata", "TDF");
    ((Class_004c2f60*)&parser)->FUN_004c2f60(name);
    s = g_game->sides;
    side = 0;
    while (1) {
        int saved;
        s->sideNumber = side;
        sprintf(name, "SIDE%d", side);
        ((Class_004c3e10*)&parser)->FUN_004c3e10();
        if (!((Class_004c3410*)&parser)->FUN_004c3410(name))
            break;
        if (((Class_004c48c0*)parser.current)->FUN_004c48c0(name, "name", 0x1e, DAT_005119b8))
            strcpy(s->name, name);
        if (((Class_004c48c0*)parser.current)->FUN_004c48c0(name, "nameprefix", 4, DAT_005119b8))
            strcpy(s->nameprefix, name);
        if (((Class_004c48c0*)parser.current)->FUN_004c48c0(name, "commander", 0x20, DAT_005119b8))
            strcpy(s->commander, name);
        if (((Class_004c48c0*)parser.current)->FUN_004c48c0(name, "font", 0x100, DAT_005119b8)) {
            void* font;
            FUN_004290f0(fontPath, "fonts", name, "FNT");
            font = FUN_004bbe50(fontPath, 0);
            if (font == 0)
                FUN_004b6290(fontPath);
            s->font = font;
        }
        s->energyColor = ((Class_004c46c0*)parser.current)->FUN_004c46c0("energycolor", 0);
        s->metalColor = ((Class_004c46c0*)parser.current)->FUN_004c46c0("metalcolor", 0);

        saved = ((Class_004c3e20*)&parser)->FUN_004c3e20();
        if (!((Class_004c3410*)&parser)->FUN_004c3410("LOGO")) {
            sprintf(msgLogo, "No [%s] in GAMEDATA/SIDEDATA.TDF for side:%s", "LOGO", s->name);
            FUN_004b6290(msgLogo);
        } else {
            int* r = &s->logo.x1;
            r[0] = ((Class_004c46c0*)parser.current)->FUN_004c46c0("x1", 0);
            r[1] = ((Class_004c46c0*)parser.current)->FUN_004c46c0("y1", 0);
            r[2] = ((Class_004c46c0*)parser.current)->FUN_004c46c0("x2", 0);
            r[3] = ((Class_004c46c0*)parser.current)->FUN_004c46c0("y2", 0);
        }
        ((Class_004c3e30*)&parser)->FUN_004c3e30(saved);

        saved = ((Class_004c3e20*)&parser)->FUN_004c3e20();
        if (!((Class_004c3410*)&parser)->FUN_004c3410("ENERGYBAR")) {
            sprintf(msgEnergybar, "No [%s] in GAMEDATA/SIDEDATA.TDF for side:%s", "ENERGYBAR", s->name);
            FUN_004b6290(msgEnergybar);
        } else {
            int* r = &s->energyBar.x1;
            r[0] = ((Class_004c46c0*)parser.current)->FUN_004c46c0("x1", 0);
            r[1] = ((Class_004c46c0*)parser.current)->FUN_004c46c0("y1", 0);
            r[2] = ((Class_004c46c0*)parser.current)->FUN_004c46c0("x2", 0);
            r[3] = ((Class_004c46c0*)parser.current)->FUN_004c46c0("y2", 0);
        }
        ((Class_004c3e30*)&parser)->FUN_004c3e30(saved);

        saved = ((Class_004c3e20*)&parser)->FUN_004c3e20();
        if (!((Class_004c3410*)&parser)->FUN_004c3410("ENERGYNUM")) {
            sprintf(msgEnergynum, "No [%s] in GAMEDATA/SIDEDATA.TDF for side:%s", "ENERGYNUM", s->name);
            FUN_004b6290(msgEnergynum);
        } else {
            int* r = &s->energyNum.x1;
            r[0] = ((Class_004c46c0*)parser.current)->FUN_004c46c0("x1", 0);
            r[1] = ((Class_004c46c0*)parser.current)->FUN_004c46c0("y1", 0);
            r[2] = ((Class_004c46c0*)parser.current)->FUN_004c46c0("x2", 0);
            r[3] = ((Class_004c46c0*)parser.current)->FUN_004c46c0("y2", 0);
        }
        ((Class_004c3e30*)&parser)->FUN_004c3e30(saved);

        saved = ((Class_004c3e20*)&parser)->FUN_004c3e20();
        if (!((Class_004c3410*)&parser)->FUN_004c3410("METALBAR")) {
            sprintf(msgMetalbar, "No [%s] in GAMEDATA/SIDEDATA.TDF for side:%s", "METALBAR", s->name);
            FUN_004b6290(msgMetalbar);
        } else {
            int* r = &s->metalBar.x1;
            r[0] = ((Class_004c46c0*)parser.current)->FUN_004c46c0("x1", 0);
            r[1] = ((Class_004c46c0*)parser.current)->FUN_004c46c0("y1", 0);
            r[2] = ((Class_004c46c0*)parser.current)->FUN_004c46c0("x2", 0);
            r[3] = ((Class_004c46c0*)parser.current)->FUN_004c46c0("y2", 0);
        }
        ((Class_004c3e30*)&parser)->FUN_004c3e30(saved);

        saved = ((Class_004c3e20*)&parser)->FUN_004c3e20();
        if (!((Class_004c3410*)&parser)->FUN_004c3410("METALNUM")) {
            sprintf(msgMetalnum, "No [%s] in GAMEDATA/SIDEDATA.TDF for side:%s", "METALNUM", s->name);
            FUN_004b6290(msgMetalnum);
        } else {
            int* r = &s->metalNum.x1;
            r[0] = ((Class_004c46c0*)parser.current)->FUN_004c46c0("x1", 0);
            r[1] = ((Class_004c46c0*)parser.current)->FUN_004c46c0("y1", 0);
            r[2] = ((Class_004c46c0*)parser.current)->FUN_004c46c0("x2", 0);
            r[3] = ((Class_004c46c0*)parser.current)->FUN_004c46c0("y2", 0);
        }
        ((Class_004c3e30*)&parser)->FUN_004c3e30(saved);

        saved = ((Class_004c3e20*)&parser)->FUN_004c3e20();
        if (!((Class_004c3410*)&parser)->FUN_004c3410("TOTALUNITS")) {
            sprintf(msgTotalunits, "No [%s] in GAMEDATA/SIDEDATA.TDF for side:%s", "TOTALUNITS", s->name);
            FUN_004b6290(msgTotalunits);
        } else {
            int* r = &s->totalUnits.x1;
            r[0] = ((Class_004c46c0*)parser.current)->FUN_004c46c0("x1", 0);
            r[1] = ((Class_004c46c0*)parser.current)->FUN_004c46c0("y1", 0);
            r[2] = ((Class_004c46c0*)parser.current)->FUN_004c46c0("x2", 0);
            r[3] = ((Class_004c46c0*)parser.current)->FUN_004c46c0("y2", 0);
        }
        ((Class_004c3e30*)&parser)->FUN_004c3e30(saved);

        saved = ((Class_004c3e20*)&parser)->FUN_004c3e20();
        if (!((Class_004c3410*)&parser)->FUN_004c3410("TOTALTIME")) {
            sprintf(msgTotaltime, "No [%s] in GAMEDATA/SIDEDATA.TDF for side:%s", "TOTALTIME", s->name);
            FUN_004b6290(msgTotaltime);
        } else {
            int* r = &s->totalTime.x1;
            r[0] = ((Class_004c46c0*)parser.current)->FUN_004c46c0("x1", 0);
            r[1] = ((Class_004c46c0*)parser.current)->FUN_004c46c0("y1", 0);
            r[2] = ((Class_004c46c0*)parser.current)->FUN_004c46c0("x2", 0);
            r[3] = ((Class_004c46c0*)parser.current)->FUN_004c46c0("y2", 0);
        }
        ((Class_004c3e30*)&parser)->FUN_004c3e30(saved);

        saved = ((Class_004c3e20*)&parser)->FUN_004c3e20();
        if (!((Class_004c3410*)&parser)->FUN_004c3410("ENERGY0")) {
            sprintf(msgEnergy0, "No [%s] in GAMEDATA/SIDEDATA.TDF for side:%s", "ENERGY0", s->name);
            FUN_004b6290(msgEnergy0);
        } else {
            int* r = &s->energy0.x1;
            r[0] = ((Class_004c46c0*)parser.current)->FUN_004c46c0("x1", 0);
            r[1] = ((Class_004c46c0*)parser.current)->FUN_004c46c0("y1", 0);
            r[2] = ((Class_004c46c0*)parser.current)->FUN_004c46c0("x2", 0);
            r[3] = ((Class_004c46c0*)parser.current)->FUN_004c46c0("y2", 0);
        }
        ((Class_004c3e30*)&parser)->FUN_004c3e30(saved);

        saved = ((Class_004c3e20*)&parser)->FUN_004c3e20();
        if (!((Class_004c3410*)&parser)->FUN_004c3410("METAL0")) {
            sprintf(msgMetal0, "No [%s] in GAMEDATA/SIDEDATA.TDF for side:%s", "METAL0", s->name);
            FUN_004b6290(msgMetal0);
        } else {
            int* r = &s->metal0.x1;
            r[0] = ((Class_004c46c0*)parser.current)->FUN_004c46c0("x1", 0);
            r[1] = ((Class_004c46c0*)parser.current)->FUN_004c46c0("y1", 0);
            r[2] = ((Class_004c46c0*)parser.current)->FUN_004c46c0("x2", 0);
            r[3] = ((Class_004c46c0*)parser.current)->FUN_004c46c0("y2", 0);
        }
        ((Class_004c3e30*)&parser)->FUN_004c3e30(saved);

        saved = ((Class_004c3e20*)&parser)->FUN_004c3e20();
        if (!((Class_004c3410*)&parser)->FUN_004c3410("ENERGYMAX")) {
            sprintf(msgEnergymax, "No [%s] in GAMEDATA/SIDEDATA.TDF for side:%s", "ENERGYMAX", s->name);
            FUN_004b6290(msgEnergymax);
        } else {
            int* r = &s->energyMax.x1;
            r[0] = ((Class_004c46c0*)parser.current)->FUN_004c46c0("x1", 0);
            r[1] = ((Class_004c46c0*)parser.current)->FUN_004c46c0("y1", 0);
            r[2] = ((Class_004c46c0*)parser.current)->FUN_004c46c0("x2", 0);
            r[3] = ((Class_004c46c0*)parser.current)->FUN_004c46c0("y2", 0);
        }
        ((Class_004c3e30*)&parser)->FUN_004c3e30(saved);

        saved = ((Class_004c3e20*)&parser)->FUN_004c3e20();
        if (!((Class_004c3410*)&parser)->FUN_004c3410("METALMAX")) {
            sprintf(msgMetalmax, "No [%s] in GAMEDATA/SIDEDATA.TDF for side:%s", "METALMAX", s->name);
            FUN_004b6290(msgMetalmax);
        } else {
            int* r = &s->metalMax.x1;
            r[0] = ((Class_004c46c0*)parser.current)->FUN_004c46c0("x1", 0);
            r[1] = ((Class_004c46c0*)parser.current)->FUN_004c46c0("y1", 0);
            r[2] = ((Class_004c46c0*)parser.current)->FUN_004c46c0("x2", 0);
            r[3] = ((Class_004c46c0*)parser.current)->FUN_004c46c0("y2", 0);
        }
        ((Class_004c3e30*)&parser)->FUN_004c3e30(saved);

        FUN_00431950(&parser, &s->energyProduced.x1, "ENERGYPRODUCED", s->name);
        FUN_00431950(&parser, &s->energyConsumed.x1, "ENERGYCONSUMED", s->name);
        FUN_00431950(&parser, &s->metalProduced.x1, "METALPRODUCED", s->name);
        FUN_00431950(&parser, &s->metalConsumed.x1, "METALCONSUMED", s->name);
        FUN_00431950(&parser, &s->logo2.x1, "LOGO2", s->name);
        FUN_00431950(&parser, &s->unitName.x1, "UNITNAME", s->name);
        FUN_00431950(&parser, &s->damageBar.x1, "DAMAGEBAR", s->name);
        FUN_00431950(&parser, &s->unitMetalMake.x1, "UNITMETALMAKE", s->name);
        FUN_00431950(&parser, &s->unitMetalUse.x1, "UNITMETALUSE", s->name);
        FUN_00431950(&parser, &s->unitEnergyMake.x1, "UNITENERGYMAKE", s->name);
        FUN_00431950(&parser, &s->unitEnergyUse.x1, "UNITENERGYUSE", s->name);
        FUN_00431950(&parser, &s->missionText.x1, "MISSIONTEXT", s->name);
        FUN_00431950(&parser, &s->unitName2.x1, "UNITNAME2", s->name);
        FUN_00431950(&parser, &s->damageBar2.x1, "DAMAGEBAR2", s->name);
        FUN_00431950(&parser, &s->nameBlock.x1, "NAME", s->name);
        FUN_00431950(&parser, &s->description.x1, "DESCRIPTION", s->name);
        {
            int n = 3;
            int k = 1;
            while (n) {
                sprintf(name, "RELOAD%d", k);
                FUN_00431950(&parser, &s->reload[k - 1].x1, name, s->name);
                n--;
                k++;
            }
        }
        side++;
        s++;
    }
    ((Class_004c3240*)&parser)->FUN_004c3240();
    g_game->sideCount = side;
}
