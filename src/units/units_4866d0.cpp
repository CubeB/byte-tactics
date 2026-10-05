// Decompiled by Claude Sonnet 5.5, finished by GPT-6, finished by deepseek-v4.1-flash, finished by GPT-6.1-sol, finished by space-bunny-free, finished by deepseek-v4.1-flash, finished by claude-sonnet-5-5, finished by Space Bunny Free, finished by DeepSeek V4.1 Flash, finished by claude-opus-5-5. Names are provisional.
// Handles the "unit died" record that 0x4864b0 builds: credits the kill, updates the
// kill leaderboard ("%s has taken the lead with %d kills"), then tears the unit down.
//
// Pass 16 (claude-opus-5-5): 92.5 -> MATCH. The last differences were all register choices
// in the kind==5 block, the tail calls and the script's virtual delete, and they moved
// together; four things were needed at once:
//  - <stdio.h> for sprintf instead of a hand declaration, with the kind==5 block storing
//    xd4 in each arm: together they put cmd in edi for the tail and keep the original's
//    x87 shape (93.0). With only <string.h> the per-arm stores break the x87 shape, and a
//    single `par->xd4 = f;` after the arms never gets edi.
//  - `Unit** par = &unit->parent;` read through `(*par)->`, a field pointer (found by
//    tools/permute.py). A `Unit* par` copy or plain `unit->parent->` both leave info in
//    the wrong register.
//  - `float health = 1.0f - x104; float f = health; f *= info->x18a;`: the copy into f
//    matters. `(1.0f - x104) * info->x18a`, `info->x18a * health` and inline helpers taking
//    the health all put info in eax (97.7 at best).
//  - `int flag = cmd->kind != 7;` inside the count block, passed to CreateUnitCorpse: without
//    it everything else matched but the virtual delete's vtable went to edx (97.7).
// The register choices of the three spots were coupled: every single change above that
// fixed one of them broke another, so they were found by scoring all combinations of the
// candidate spellings in one batch, not one change at a time.
//
// Pass 15 (claude-opus-5-5): 87.0 -> 92.5 percent. Rewritten on real types (Game/Player/Unit/
// Cmd structs, the record's count and kind as 4-bit bitfields, a virtual destructor for the
// script object) instead of at<> offsets. What moved it, in order:
//  - the typed rewrite itself put the tail's `cmd->count > 0` test back to `jbe` and the
//    script/head teardown on the original registers; but the leaderboard loop must stay a
//    do/while over a Player pointer with `mine` read through a ternary, or MSVC keeps the
//    loop's g_game->mode in a register across the loop and grows the frame to 0x6c.
//  - `flags &= ~0x10000000; info = g_game->x1439b; flags &= ~0x30;` in that order: with
//    the two clears adjacent MSVC folds them into one `and` (+1.2).
//  - the leaderboard compare as `int ahead; if (mode == 2) ahead = mine > p->kills2; else
//    ahead = mine > p->kills;`, with rank and best set before mine. MSVC tail-merges the two
//    setg arms, which is exactly the original's per-arm movsx, `setg bl` and `mov ecx, ebx`
//    (+3.5; the whole leaderboard now matches).
// Passes 1 to 14 (several models) reached 87.0 on an at<>-offset version of this function.
#include <stdio.h>
#include <string.h>


extern char DAT_00508be8[];
extern char DAT_00508bf0[];

class Class_004904c0 {
public:
    void FUN_004904c0(void* unit);
};

class Class_00435100 {
public:
    int FUN_00435100();
};

class UnitMotion {
public:
    void FUN_0043dd10();
};

class CobScript {
public:
    int StartScriptWithArgs(char* name, void* a, int b, int c, int d, int e, int f, int g);
};

// The unit's script object; deleting it calls the virtual destructor in slot 0x50.
class Script_004866d0 {
public:
    virtual void v00();
    virtual void v04();
    virtual void v08();
    virtual void v0c();
    virtual void v10();
    virtual void v14();
    virtual void v18();
    virtual void v1c();
    virtual void v20();
    virtual void v24();
    virtual void v28();
    virtual void v2c();
    virtual void v30();
    virtual void v34();
    virtual void v38();
    virtual void v3c();
    virtual void v40();
    virtual void v44();
    virtual void v48();
    virtual void v4c();
    virtual ~Script_004866d0();         // +0x50
};

#pragma pack(push, 1)
// The 0xb-byte "unit died" network record built by 0x4864b0.
struct Cmd_004866d0 {
    unsigned char type;                 // +0x0
    unsigned short unitId;              // +0x1
    int killerId;                       // +0x3
    unsigned short parentId;            // +0x7
    signed char amount;                 // +0x9
    unsigned char count : 4;            // +0xa
    unsigned char kind : 4;
};

struct Owner_004866d0 {
    char unknown_0[0x95];
    unsigned char nameIndex;            // +0x95
    char unknown_96[5];
    unsigned short b0 : 1, b1 : 1, b2 : 1, b3 : 1, b4 : 1, b5 : 1, b6 : 1, b7 : 1,
        b8 : 1, b9 : 1, b10 : 1, b11 : 1, b12 : 1, b13 : 1, b14 : 1, b15 : 1;   // +0x9b
};

struct Player_004866d0 {
    int active;                         // +0x0
    int dpid;                           // +0x4
    char unknown_8[0x1f];
    Owner_004866d0* owner;              // +0x27
    char name[0x48];                    // +0x2b
    char state;                         // +0x73
    char unknown_74[0x88];
    short kills;                        // +0xfc
    short losses;                       // +0xfe
    char unknown_100[4];
    short kills2;                       // +0x104
    short losses2;                      // +0x106
    char unknown_108[0x21];
    char allied[10];                    // +0x129
    char unknown_133[0x11];
    short unitCount;                    // +0x144
    unsigned char index;                // +0x146
    char unknown_147;
    unsigned char rank;                 // +0x148
    char unknown_149[2];
};

struct UnitInfo_004866d0 {
    char unknown_0[0x20];
    char name[0x150];                   // +0x20
    short x170;                         // +0x170
    char unknown_172[0x18];
    float x18a;                         // +0x18a
    char unknown_18e[0x74];
    short x202;                         // +0x202
};

struct Unit {
    UnitMotion* head;                   // +0x0
    char unknown_4[0x66];
    char pos[0x1c];                     // +0x6a
    int x86;                            // +0x86
    Unit* x8a;                          // +0x8a
    char unknown_8e[4];
    UnitInfo_004866d0* info;            // +0x92
    Player_004866d0* player;            // +0x96
    Script_004866d0* script;            // +0x9a
    void* x9e;                          // +0x9e
    char unknown_a2[4];
    short xa6;                          // +0xa6
    char unknown_a8[0x10];
    short kills;                        // +0xb8
    char unknown_ba[0x1a];
    float xd4;                          // +0xd4
    char unknown_d8[0x14];
    Player_004866d0* xec;               // +0xec
    Unit* parent;                       // +0xf0
    unsigned char killer;               // +0xf4
    char unknown_f5[0xa];
    unsigned char owner;                // +0xff
    char unknown_100[4];
    float x104;                         // +0x104
    char unknown_108[8];
    unsigned int flags;                 // +0x110
    char unknown_114[4];
};

struct Name_004866d0 {
    char name[0x232];
};

struct Game {
    char unknown_0[0x1b63];
    Player_004866d0 players[10];        // +0x1b63
    char unknown_2851[0x2a42 - 0x2851];
    unsigned char localPlayer;          // +0x2a42
    unsigned char x2a43;                // +0x2a43
    char unknown_2a44[0x14281 - 0x2a44];
    unsigned char x14281;               // +0x14281
    char unknown_14282[0x14357 - 0x14282];
    Unit* units;                        // +0x14357
    char unknown_1435b[0x1439b - 0x1435b];
    UnitInfo_004866d0* x1439b;          // +0x1439b
    char unknown_1439f[0x37eee - 0x1439f];
    int x37eee;                         // +0x37eee
    char unknown_37ef2[4];
    int mode;                           // +0x37ef6
    char unknown_37efa[0x37f06 - 0x37efa];
    unsigned short b0 : 1, b1 : 1, b2 : 1, b3 : 1, b4 : 1, b5 : 1, b6 : 1, b7 : 1,
        b8 : 1, b9 : 1, b10 : 1, b11 : 1, b12 : 1, b13 : 1, b14 : 1, b15 : 1;   // +0x37f06
    char unknown_37f08[0x37f5f - 0x37f08];
    Name_004866d0 names[8];             // +0x37f5f
    char unknown_390ef[0x391e9 - 0x390ef];
    Class_00435100* x391e9;             // +0x391e9
    Class_004904c0* x391ed;             // +0x391ed
};
#pragma pack(pop)

extern Game* g_game;

void __stdcall FUN_00482910(void* pos, int a, int b, int c);
unsigned char __stdcall FindSlotByDpid(int id);
void __stdcall DeleteOrders(void* unit, int flag);
void __stdcall FUN_0047f8c0(void* unit);
void __stdcall SetUnitSquad(void* unit, int flag);
void __stdcall FUN_0049c880(void* unit);
void __stdcall AttachUnitToPiece(void* unit, void* builder, int a, int c);
void __stdcall DamageUnit(void* a, void* b, int c, int d, int e);
void __stdcall FUN_0047cbd0(void* unit);
void __stdcall FUN_00482090(void* unit);
void __stdcall FUN_00494ff0(int flag);
char* __stdcall FUN_004c5740(char* text);
void __stdcall AddMessage(char* text, int a, int b, int c);
void __stdcall FUN_004948b0(int a, int b);
void __stdcall FUN_0049b000(void* unit, int flag);
void __stdcall CreateUnitCorpse(void* unit, int a, int b);
void __stdcall ClearUnitRefs(void* unit);
void __stdcall FreeObjectState(void* state);
void __cdecl operator delete(void* p);
void __stdcall AnnouncePlayerLeft(int id);
void __stdcall FUN_0047bd70(void* player);

// FUNCTION: 0x4866d0
void __stdcall ApplyUnitDeath(Cmd_004866d0* cmd, int local)
{
    Unit* unit;
    if (cmd->unitId == 0)
        unit = 0;
    else
        unit = &g_game->units[cmd->unitId];
    if ((unit->flags & 0x10000000) == 0)
        return;

    if (unit->player->index == g_game->x2a43)
        FUN_00482910(unit->pos, unit->info->x202, unit->info->x170, 60);
    Unit* parent;
    if (cmd->parentId == 0)
        parent = 0;
    else
        parent = &g_game->units[cmd->parentId];
    unit->parent = parent;
    unit->killer = FindSlotByDpid(cmd->killerId);
    g_game->x391ed->FUN_004904c0(unit);
    DeleteOrders(unit, 1);
    FUN_0047f8c0(unit);
    SetUnitSquad(unit, -1);
    FUN_0049c880(unit);
    if (unit->x86 != 0)
        AttachUnitToPiece(unit, 0, -1, 1);
    while (unit->x8a != 0) {
        unsigned char depth = cmd->kind != 3 ? 6 : 3;
        DamageUnit(unit->parent, unit->x8a, 30000, depth, 0);
        AttachUnitToPiece(unit->x8a, 0, -1, 1);
    }
    FUN_0047cbd0(unit);
    if ((g_game->x14281 & 2) == 2)
        FUN_00482090(unit);
    if (local == 0 && cmd->amount > 0)
        ((CobScript*)unit->script)->StartScriptWithArgs(DAT_00508be8, 0, 1, 1, cmd->amount, 0, 0, 0);

    int credited = 0;
    switch (cmd->kind) {
    case 5:
        if (unit->killer == 10 || unit->killer == unit->owner)
            break;
    case 1:
    case 6:
        if (unit->player != 0) {
            unit->player->losses++;
            if (unit->killer != 10 && unit->x104 == 0.0f && unit->owner != unit->killer)
                g_game->players[unit->killer].kills++;
            int same = _strcmpi(g_game->names[unit->player->owner->nameIndex].name,
                                unit->info->name) == 0;
            if (same) {
                if (unit->killer != 10)
                    g_game->players[unit->killer].kills2++;
                unit->player->losses2++;
            }
            if (unit->parent != 0 && unit->x104 == 0.0f && unit->owner != unit->killer)
                unit->parent->kills++;
            if (unit->killer == g_game->localPlayer)
                FUN_00494ff0(5);
            credited = 1;
        }
        break;
    case 3:
        if (unit->player != 0 && g_game->players[g_game->localPlayer].allied[unit->player->index] == 0) {
            unit->player->losses++;
            int same = _strcmpi(g_game->names[unit->player->owner->nameIndex].name,
                                unit->info->name) == 0;
            if (same)
                unit->player->losses2++;
            credited = 1;
        }
        break;
    }

    if (credited && unit->killer != 10) {
        Player_004866d0* rec = &g_game->players[unit->killer];
        if (rec->active != 0 && (rec->state == 1 || rec->state == 2 || rec->state == 3)
            && rec->index != 10
            && (g_game->x391e9->FUN_00435100() == 3 || g_game->x391e9->FUN_00435100() == 2)
            && rec->rank > 0) {
            int rank = rec->rank;
            int best = rank;
            int mine = g_game->mode == 2 ? rec->kills2 : rec->kills;
            int i = 10;
            Player_004866d0* p = g_game->players;
            do {
                if (p->state != 0) {
                    bool hid = p->owner->b6;
                    if (!hid) {
                        int ahead;
                        if (g_game->mode == 2)
                            ahead = mine > p->kills2;
                        else
                            ahead = mine > p->kills;
                        if (ahead) {
                            if (p->rank < best)
                                best = p->rank;
                        }
                    }
                }
                p++;
                i--;
            } while (i != 0);
            if (best < rank) {
                i = 10;
                p = g_game->players;
                do {
                    if (p->rank >= best && p->rank < rec->rank)
                        p->rank = p->rank + 1;
                    p++;
                    i--;
                } while (i != 0);
                rec->rank = best;
                if (best == 0) {
                    char text[100];
                    sprintf(text, FUN_004c5740(DAT_00508bf0), rec->name,
                            g_game->mode == 2 ? rec->kills2 : rec->kills);
                    AddMessage(text, 2, 0, 10);
                }
            }
        }
        if (g_game->b7)
            FUN_004948b0(unit->killer, unit->player->index);
    }

    if (cmd->kind == 5 && unit->parent != 0) {
        Unit** par = &unit->parent;
        float health = 1.0f - unit->x104;
        float f = health;
        f *= unit->info->x18a;
        if ((*par)->xec->active != 0 && (*par)->xec->state == 2) {
            switch (g_game->x37eee) {
            case 0:
                (*par)->xd4 = (*par)->xd4 - f * -0.5;
                break;
            case 1:
                (*par)->xd4 = (*par)->xd4 - f * -0.7;
                break;
            default:
                (*par)->xd4 += f;
            }
        } else {
            (*par)->xd4 += f;
        }
    }
    if (cmd->amount > 0 && unit->x104 == 0.0f)
        FUN_0049b000(unit, cmd->kind == 3);
    if (cmd->count > 0) {
        int flag = cmd->kind != 7;
        CreateUnitCorpse(unit, cmd->count, flag);
    }

    ClearUnitRefs(unit);
    if (unit->script != 0) {
        delete unit->script;
        unit->script = 0;
    }
    if (unit->x9e != 0) {
        FreeObjectState(unit->x9e);
        unit->x9e = 0;
    }
    UnitMotion* head = unit->head;
    if (head != 0) {
        head->FUN_0043dd10();
        operator delete(head);
        unit->head = 0;
    }
    unit->xa6 = 0;
    unit->flags &= ~0x10000000;
    unit->info = g_game->x1439b;
    unit->flags &= ~0x30;
    unit->player->unitCount--;
    if (unit->player->unitCount == 0) {
        if (g_game->x391e9->FUN_00435100() == 3)
            AnnouncePlayerLeft(unit->player->dpid);
        if (g_game->x391e9->FUN_00435100() == 2)
            FUN_0047bd70(unit->player);
    }
}
