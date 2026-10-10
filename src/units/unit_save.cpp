// Decompiled by space-bunny-free, DeepSeek V4.1 Flash, GPT-6, GPT-6.1-sol, deepseek-v4.1, claude-sonnet-5-5, claude-opus-5-5, Space Bunny Free and Haiku. Names are provisional.

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
// Must stay, though unused: LoadUnit's flag chain registers depend on it.
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

#include "../util/vec3.h"

struct PieceBits {
    unsigned char b0 : 1;
    unsigned char b1 : 1;
    unsigned char b23 : 2;
    unsigned char b4 : 1;
    unsigned char b5 : 3;
};

#include "../util/angles.h"

#pragma pack(push, 1)

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
    Angles16 angles;                    // +0x37
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
    void* motion;                       // +0x0
    Piece weapons[3];                   // +0x4
    int extraction;                       // +0x58
    void* list;                         // +0x5c
    void* list2;                        // +0x60
    Angles16 angles;                  // +0x64
    Vec3 pos;                           // +0x6a
    int cell;                       // +0x76
    int losCacheCellX;                       // +0x7a
    int footprint;                       // +0x7e
    char gap_82[4];
    void* carrier;                      // +0x86
    char gap_8a[8];
    void* def;                     // +0x92
    char gap_96[4];
    void* script;                     // +0x9a
    char gap_9e[0xa];
    unsigned short id;                  // +0xa8
    char gap_aa[2];
    int group;                       // +0xac
    int workTime;                       // +0xb0
    char gap_b4[4];
    short killCount;                     // +0xb8
    short netDirtyFlags;                     // +0xba
    char resourceSlot[0x34];            // +0xbc
    Unit* attacker;                     // +0xf0
    unsigned char lastAttackerSlot;
    unsigned char lastDamageType;
    unsigned char healthPercent;
    unsigned char prevHealthPercent;
    unsigned char losSightFrameIdx;
    unsigned char transportPiece;
    unsigned char recentlyDamagedTimer;
    char gap_fb[4];
    unsigned char playerIndex;          // +0xff
    char gap_100[4];
    int buildLeft;                      // +0x104
    short health;                    // +0x108
    char gap_10a[4];
    unsigned char activateFlags;
    union { unsigned char b_10f; Bits10F bf; };
    unsigned int flags;                 // +0x110
    char gap_114[4];
};

struct Mission;

struct Game {
    char unknown_0[0x14357];
    Unit* units;                        // +0x14357
    char unknown_1435b[0x391e9 - 0x1435b];
    Mission* mapInfo;                   // +0x391e9
};

class MissionUnit {
public:
    char* name;                        // +0x0
    char* ident;                       // +0x4
    char unknown_8[0x24 - 8];
};

#include "../map/mission.h"

struct Struct_00487af0 {
    char unknown_0[4];
    int* field_4;                      // +0x4
};
#pragma pack(pop)

#include "../util/hapi_bank.h"

// Kept local, not the CobScript header: the header changes LoadUnit's code.
class CobScript {
public:
    void LoadScriptState(HapiBank*);
    int SaveScriptState(void* file);
};

#include "../orders/unit_motion.h"

#pragma pack(push, 1)
#include "../orders/order.h"

#pragma pack(pop)

#include "../game/unit_resources.h"

extern Game* g_game;

Unit* __stdcall LoadUnit(unsigned short id, HapiBank* file);
unsigned short __stdcall FindUnitTypeId(const char* name);
Unit* __stdcall CreateUnit(unsigned char player, unsigned short typeId,
                                      Vec3 pos, int param_5, int mode,
                                      unsigned short id);
void __stdcall AttachUnitToPiece(Unit* unit, Unit* builder, int piece, int p4);
void __stdcall SetUnitSquad(Unit* unit, int id);
void __stdcall ForceNeighborFootprintReclaim(Unit* unit);

// FUNCTION: 0x486fd0
void __stdcall LoadUnits(HapiBank* file)
{
    if (file->OpenAccount("Units")) {
        if (file->GetIntegerItem("Version", 0) == 0x11) {
            int n = file->GetIntegerItem("Number of Units", 0);
            for (int i = 0; i < n; i++) {
                if (file->OpenNumberedBox(i)) {
                    file->SeekBox(0);
                    SaveRec rec;
                    int len = file->ReadBox(&rec, 0xb8);
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
// 0x43a420 runs on the result of operator new and stores vtables, so it is written as a
// constructor, `new Order(unit, file, name)`, as the naming rule asks.
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
    int n = file->GetIntegerItem("Number of Units", 0);
    int found = 0;
    int i;
    for (i = 0; i < n; i++) {
        if (!file->OpenNumberedBox(i))
            return 0;
        file->SeekBox(0);
        if (file->ReadBox(&rec, 0xb8) != 0xb8)
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

    unit->angles = rec.angles;
    unit->health = rec.f3d;
    unit->killCount = rec.f3f;
    unit->pos.y = rec.pos.y;

    if (rec.childA != 0) {
        Unit* child = LoadUnit(rec.childA, file);
        if (child != 0)
            AttachUnitToPiece(unit, child, rec.b8d, rec.flags.b & 3);
    }
    unit->attacker = LoadUnit(rec.childB, file);
    unit->transportPiece = rec.b8d;
    unit->lastAttackerSlot = rec.b8e;
    unit->extraction = rec.f8f;
    unit->cell = rec.f93;
    unit->losCacheCellX = rec.f97;
    unit->footprint = rec.f9b;
    unit->group = rec.f9f;
    SetUnitSquad(unit, rec.f9f);
    unit->buildLeft = rec.fa7;
    unit->lastDamageType = rec.bab;
    unit->healthPercent = rec.bac;
    unit->prevHealthPercent = rec.bad;
    unit->netDirtyFlags = rec.bae;
    unit->losSightFrameIdx = rec.bb0;
    unit->recentlyDamagedTimer = rec.bb1;
    unit->activateFlags = rec.bb2;

    unit->bf.b0 = rec.flags.a0;
    unit->bf.b1 = rec.flags.a1;
    unit->bf.b2 = rec.flags.a2;
    unit->bf.b3 = rec.flags.a3;
    // Direct merges into unit->flags, no `u` local and no reload.
    unit->flags = (unit->flags & ~0xc) | (rec.flags.b & 0xc);
    unit->flags = (unit->flags & ~0x10) | (rec.flags.b & 0x10);
    unit->flags = (unit->flags & ~0x20) | (rec.flags.b & 0x20);
    unit->flags = (unit->flags & ~0xc0) | (rec.flags.b & 0xc0);
    unit->flags = (unit->flags & ~0x100) | (rec.flags.b & 0x100);
    unit->flags = (unit->flags & ~0x200) | (rec.flags.b & 0x200);
    unit->flags = (unit->flags & ~0x400) | (rec.flags.b & 0x400);
    unit->flags = (unit->flags & ~0x800) | (rec.flags.b & 0x800);
    unit->workTime = rec.fa3;
    unit->flags = (unit->flags & ~0x2000) | (rec.flags.c << 13);
    unit->flags = (unit->flags & ~0x4000) | ((rec.flags.e << 14) & 0x4000);
    unit->flags = (unit->flags & ~0x8000) | ((rec.flags.e << 14) & 0x8000);
    unit->flags = (unit->flags & ~0x10000) | ((rec.flags.e << 14) & 0x10000);
    unit->flags = (unit->flags & ~0x20000) | ((rec.flags.e << 14) & 0x20000);
    unit->flags = (unit->flags & ~0xc0000) | ((rec.flags.e << 14) & 0xc0000);
    unit->flags = (unit->flags & ~0x300000) | ((rec.flags.e << 14) & 0x300000);
    unit->flags = (unit->flags & ~0x400000) | ((rec.flags.e << 14) & 0x400000);
    unit->flags = (unit->flags & ~0x3800000) | ((rec.flags.e << 14) & 0x3800000);

    ((UnitResources*)&unit->resourceSlot)->LoadUnitAccounts((UnitInfo*)unit, file);
    if (rec.f27 != 0)
        ((UnitMotion*)unit->motion)->LoadMotion(unit, file);

    Order** normal = (Order**)&unit->list;
    Order** special = (Order**)&unit->list2;
    int k = 0;
    if (rec.f23 > 0) {
        do {
            sprintf(name, "u%04xm%04x", unit->id, k);
            Order* p = new Order(unit, file, name);
            if (p->flags_42 & 0x40000) {
                *special = p;
                special = &p->next;
            } else {
                *normal = p;
                normal = &p->next;
            }
            k++;
        } while (k < rec.f23);
    }
    if (unit->list != 0)
        ((Order*)unit->list)->ReattachFxToUnit();
    sprintf(script, "Script%i", i);
    file->OpenNamedBox(script);
    ((CobScript*)unit->script)->LoadScriptState(file);

    // Plain array indexing in this field order: it anchors the loop pointer.
    for (int j = 0; j < 3; j++) {
        unit->weapons[j].f0 = rec.pieces[j].f0;
        unit->weapons[j].f4 = rec.pieces[j].f4;
        unit->weapons[j].obj[0x10a] = rec.pieces[j].f8b;
        unit->weapons[j].fc = rec.pieces[j].fc;
        unit->weapons[j].f10 = rec.pieces[j].f10;
        unit->weapons[j].f12 = rec.pieces[j].f12;
        unit->weapons[j].f14 = rec.pieces[j].f14;
        unit->weapons[j].f16 = rec.pieces[j].f16;
        unit->weapons[j].fl.b0 = rec.pieces[j].fl.b0;
        unit->weapons[j].fl.b1 = rec.pieces[j].fl.b1;
        unit->weapons[j].fl.b23 = rec.pieces[j].fl.b23;
        unit->weapons[j].fl.b4 = rec.pieces[j].fl.b4;
    }

    if (unit->b_10f & 4)
        ForceNeighborFootprintReclaim(unit);
    return unit;
    }
    return 0;
}

// Saves every live unit (g_game+0x14357..+0x1435b, stride 0x118) as a 0xb8
// byte record; inverse of the 0x487080 loader, whose field map it shares.
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
            file->OpenNamedBox(script);
            ((CobScript*)unit->script)->SaveScriptState(file);

            int n = 0;
            Order* c = (Order*)unit->list;
            while (c != 0) {
                sprintf(bufHead, "u%04xm%04x", unit->id, n);
                c->SerializeToSave(unit, file, bufHead);
                c = c->next;
                n++;
            }
            c = (Order*)unit->list2;
            while (c != 0) {
                sprintf(bufTail, "u%04xm%04x", unit->id, n);
                c->SerializeToSave(unit, file, bufTail);
                c = c->next;
                n++;
            }

            if (unit->motion != 0)
                ((UnitMotion*)unit->motion)->SaveMotion(unit, file);
            ((UnitResources*)((char*)unit + 0xbc))->SaveUnitAccounts((UnitInfo*)unit, file);

            strcpy(rec.name, (char*)(*(char**)((char*)unit + 0x92) + 0x20));
            rec.player = unit->playerIndex;
            rec.id = unit->id;

            rec.pos = unit->pos;
            rec.angles = unit->angles;
            rec.f23 = n;
            rec.f3d = unit->health;
            rec.f3f = unit->killCount;
            rec.f27 = unit->motion != 0;

            // Plain short local assigned in the nested if, not a pointer.
            short id8b = 0;
            Unit* a = (Unit*)unit->carrier;
            if (a != 0 && (a->flags & 0x10000000)) {
                rec.childA = unit->carrier == 0 ? 0 : a->id;
                rec.b8d = unit->transportPiece;
            } else {
                rec.childA = 0;
                rec.b8d = 0xff;
            }
            Unit* a2 = (Unit*)unit->attacker;
            if (a2 != 0) {
                if (a2->flags & 0x10000000)
                    id8b = unit->attacker == 0 ? 0 : a2->id;
            }
            rec.childB = id8b;
            // Keep this statement order: it decides the scratch register chains.
            rec.f93 = unit->cell;
            rec.b8e = unit->lastAttackerSlot;
            rec.f8f = unit->extraction;
            rec.f9f = unit->group;
            rec.f97 = unit->losCacheCellX;
            rec.f9b = unit->footprint;
            rec.bac = unit->healthPercent;
            rec.fa7 = unit->buildLeft;
            rec.bab = unit->lastDamageType;
            rec.bb0 = unit->losSightFrameIdx;
            rec.bad = unit->prevHealthPercent;
            rec.bae = unit->netDirtyFlags;
            rec.fa3 = unit->workTime;
            rec.bb1 = unit->recentlyDamagedTimer;
            rec.fb2 = unit->activateFlags;
            unsigned int u = unit->flags;
            // Four 1-bit copies: a 4-bit field or `& 0xf` loses the zero extension.
            rec.flags.a0 = unit->bf.b0;
            rec.flags.a1 = unit->bf.b1;
            rec.flags.a2 = unit->bf.b2;
            rec.flags.a3 = unit->bf.b3;
            rec.flags.b = u & 0xfff;
            rec.flags.c = (u >> 13) & 1;
            rec.flags.e = (u >> 14) & 0xfff;

            // Plain array indexing in this field order: it anchors the loop pointer.
            for (int k = 0; k < 3; k++) {
                rec.pieces[k].f0 = unit->weapons[k].f0;
                rec.pieces[k].f4 = unit->weapons[k].f4;
                rec.pieces[k].f8 = ((Obj*)unit->weapons[k].obj)->f10a;
                rec.pieces[k].fc = unit->weapons[k].fc;
                rec.pieces[k].f10 = unit->weapons[k].f10;
                rec.pieces[k].f12 = unit->weapons[k].f12;
                rec.pieces[k].f14 = unit->weapons[k].f14;
                rec.pieces[k].f16 = unit->weapons[k].f16;
                rec.pieces[k].fl.b0 = unit->weapons[k].fl.b0;
                rec.pieces[k].fl.b1 = unit->weapons[k].fl.b1;
                rec.pieces[k].fl.b23 = unit->weapons[k].fl.b23;
                rec.pieces[k].fl.b4 = unit->weapons[k].fl.b4;
            }

            file->OpenNumberedBox(count);
            file->WriteBox(&rec, 0xb8);
            count++;
        }
    }
    if (count > 0) {
        file->SetIntegerItem("Number of Units", count);
        file->SetIntegerItem("Version", 0x11);
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
            if (i >= g_game->mapInfo->unitCount)
                return 0;
        }
    }
    if (i >= g_game->mapInfo->unitCount)
        return 0;
    for (; i < g_game->mapInfo->unitCount; i++) {
        MissionUnit* e = &g_game->mapInfo->units[i];
        if (e->ident && _strcmpi(e->ident, name) == 0 && param_2->field_4[i])
            return param_2->field_4[i];
        if (e->name && _strcmpi(e->name, name) == 0 && param_2->field_4[i])
            return param_2->field_4[i];
    }
    return 0;
}
