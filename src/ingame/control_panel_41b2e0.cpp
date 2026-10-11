// Decompiled by Claude Opus 5.5, finished by deepseek-v4.1-flash, finished by GPT-6, finished by deepseek-v4.1-flash, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by Claude Opus 5.5, finished by GPT-6, finished by Claude Opus 5.5. Names are provisional.
// Refreshes the order buttons for the current selection: a single unit
// (g_game->unitIndex) or every selected unit of the local player, merging
// their fire and move orders, cloak and on/off states and the can* bits into
// g_game->orders, then opens the unit's build menu page or redraws the bar.
// This include set decides the symbol ids of the locals: do not change it.
#include <windows.h>
#include <shlobj.h>
#include <memory.h>
#include "ta_types.h"


// Own views of the unit, UnitDef and game, with the names of units/unit_def.h: the shared header's types have plain words where bitfields are needed.
#pragma pack(push, 1)
struct UnitType_0041b2e0 {
    char unknown_0[0x22e];
    unsigned char buildMenuPageCount;  // +0x22e
    char unknown_22f[0x241 - 0x22f];
    unsigned int flags1;               // +0x241
    unsigned int canMoveOrder : 1;     // +0x245 bit 0
    unsigned int canFireOrder : 1;     // bit 1
    unsigned int canOnOff : 1;         // bit 2
    unsigned int canStop : 1;          // bit 3
    unsigned int canAttack : 1;        // bit 4
    unsigned int canDefend : 1;        // bit 5
    unsigned int canPatrol : 1;        // bit 6
    unsigned int canMove : 1;          // bit 7
    unsigned int canLoad : 1;          // bit 8
    unsigned int canRepair : 1;        // bit 9
    unsigned int canReclaim : 1;       // bit 10
    unsigned int bit11 : 1;
    unsigned int canCapture : 1;       // bit 12
    unsigned int canCloak : 1;         // bit 13
    unsigned int canBlast : 1;         // bit 14
    unsigned int bits15 : 17;
};

struct UnitFlags_0041b2e0 {
    unsigned int bits0 : 4;
    unsigned int selected : 1;         // bit 4
    unsigned int bits5 : 6;
    unsigned int cloak : 1;            // bit 11
    unsigned int bits12 : 6;
    unsigned int moveOrder : 2;        // bits 18-19
    unsigned int fireOrder : 2;        // bits 20-21
    unsigned int buildPage : 1;        // bit 22
    unsigned int page : 3;             // bits 23-25
    unsigned int bits26 : 6;
};

struct Unit_0041b2e0 {
    char unknown_0[0x92];
    UnitType_0041b2e0* def;            // +0x92
    char unknown_96[0xa6 - 0x96];
    unsigned short unitDefIndex;       // +0xa6
    unsigned short id;                 // +0xa8
    char unknown_aa[0x10e - 0xaa];
    unsigned char activateFlags;       // +0x10e
    char cobStateFlags;
    UnitFlags_0041b2e0 flags;          // +0x110
    char unknown_114[0x118 - 0x114];
};

struct BuildType_0041b2e0 {
    char unknown_0[0x20];
    char unitname[0x229];              // +0x20
};

struct Orders_0041b2e0 {
    unsigned short unknown_0 : 1;      // +0x37ebe
    unsigned short refresh : 1;        // bit 1
    unsigned short unknown_2 : 10;
    unsigned short fireOrder : 3;      // bits 12-14
    unsigned short unknown_15 : 1;
    unsigned short moveOrder : 3;      // +0x37ec0 bits 0-2
    unsigned short cloak : 2;          // bits 3-4
    unsigned short onOff : 2;          // bits 5-6
    unsigned short canMove : 1;        // bit 7
    unsigned short canStop : 1;        // bit 8
    unsigned short canAttack : 1;      // bit 9
    unsigned short canDefend : 1;      // bit 10
    unsigned short canPatrol : 1;      // bit 11
    unsigned short canLoad : 1;        // bit 12
    unsigned short canReclaim : 1;     // bit 13
    unsigned short canCapture : 1;     // bit 14
    unsigned short canRepair : 1;      // bit 15
    unsigned short canBlast : 1;       // +0x37ec2 bit 0
    unsigned short unknown_1 : 15;
};

struct Menu_0041b2e0 {
    char unknown_0[0x10];
};

struct Game_0041b2e0 {
    char unknown_0[0x519];
    Menu_0041b2e0 menu;                // +0x519
    char unknown_529[0x1b63 - 0x529];
    Player players[10];                 // +0x1b63
    char unknown_2851[0x2a42 - 0x2851];
    unsigned char localPlayer;         // +0x2a42
    char unknown_2a43[0x14357 - 0x2a43];
    Unit_0041b2e0* units;              // +0x14357
    char unknown_1435b[0x1439b - 0x1435b];
    BuildType_0041b2e0* unitDefs;      // +0x1439b
    char unknown_1439f[0x37e9c - 0x1439f];
    unsigned short unitIndex;          // +0x37e9c
    char unknown_37e9e[0x37ebe - 0x37e9e];
    Orders_0041b2e0 orders;            // +0x37ebe
};
#pragma pack(pop)

extern Game_0041b2e0* g_game;

int __stdcall PopUntilNamedLayout(int force);
int __stdcall IsScreenNamed(Menu_0041b2e0* menu, const char* name);
void __stdcall OpenBuildMenuGui(Unit_0041b2e0* unit, char* guiName, int page);
void __stdcall OpenGeneratorDialog(Unit_0041b2e0* unit);

static inline Unit_0041b2e0* GetUnit(unsigned short index)
{
    Unit_0041b2e0* u = &g_game->units[index];
    if (u->unitDefIndex == 0)
        u = 0;
    return u;
}

// Stays in its own file: its locals' symbol ids decide the match, and only
// this include set puts them in the window (docs/c2-regalloc.md).
// FUNCTION: 0x41b2e0
void RefreshSelectionOrders()
{
    g_game->orders.refresh = 1;
    // Declaration order matters: cloak before onOff, first and count left
    // uninitialised and set at the top of each branch.
    int fireOrder = 4;
    int moveOrder = 4;
    int cloak = 3;
    Unit_0041b2e0* u;
    int onOff = 3;
    Unit_0041b2e0* first;
    int count;
    int canMove = 0;
    int canAttack = 0;
    int canDefend = 0;
    int canPatrol = 0;
    int canLoad = 0;
    int canCapture = 0;
    int canReclaim = 0;
    int canBlast = 0;
    int canStop = 0;
    int canRepair = 0;
    Player* player = &g_game->players[g_game->localPlayer];
    if (g_game->unitIndex != 0) {
        count = 1;
        first = GetUnit(g_game->unitIndex);
        if (first == 0) {
            g_game->unitIndex = 0;
            return;
        }
    } else {
        first = 0;
        count = 0;
        Unit_0041b2e0* last = (Unit_0041b2e0*)player->unitsEnd;
        for (u = (Unit_0041b2e0*)player->unitsBegin; u <= last; u++) {
            if (u->unitDefIndex == 0)
                continue;
            UnitFlags_0041b2e0 flags = u->flags;
            if (!flags.selected)
                continue;
            if (first == 0)
                first = u;
            UnitType_0041b2e0* type = u->def;
            if (type->canFireOrder) {
                if (fireOrder == 4)
                    fireOrder = flags.fireOrder;
                else if (fireOrder != flags.fireOrder)
                    fireOrder = 3;
            }
            if (type->canMoveOrder) {
                if (moveOrder == 4)
                    moveOrder = flags.moveOrder;
                else if (moveOrder != flags.moveOrder)
                    moveOrder = 3;
            }
            if (type->canOnOff) {
                if (onOff == 3)
                    onOff = u->activateFlags & 1;
                else if (onOff != (u->activateFlags & 1))
                    onOff = 2;
            }
            if (type->canCloak) {
                if (cloak == 3)
                    cloak = flags.cloak;
                else
                    cloak = 2;
            }
            if (type->canMove)
                canMove = 1;
            if (type->canStop)
                canStop = 1;
            if (type->canAttack)
                canAttack = 1;
            if (type->canDefend)
                canDefend = 1;
            if (type->canPatrol)
                canPatrol = 1;
            if (type->canLoad)
                canLoad = 1;
            if (type->canRepair)
                canRepair = 1;
            if (type->canCapture)
                canCapture = 1;
            if (type->canReclaim)
                canReclaim = 1;
            if (type->canBlast)
                canBlast = 1;
            count++;
        }
        // Stores in the original's order.
        g_game->orders.fireOrder = fireOrder;
        g_game->orders.moveOrder = moveOrder;
        g_game->orders.cloak = cloak;
        g_game->orders.onOff = onOff;
        g_game->orders.canStop = canStop;
        g_game->orders.canAttack = canAttack;
        g_game->orders.canMove = canMove;
        g_game->orders.canDefend = canDefend;
        g_game->orders.canPatrol = canPatrol;
        g_game->orders.canLoad = canLoad;
        g_game->orders.canRepair = canRepair;
        g_game->orders.canReclaim = canReclaim;
        g_game->orders.canCapture = canCapture;
        g_game->orders.canBlast = canBlast;
    }
    if (count == 0) {
        PopUntilNamedLayout(0);
        g_game->orders.refresh = 0;
    } else if (count == 1 && first->def->buildMenuPageCount) {
        int page = first->flags.buildPage ? first->flags.page : 0;
        if (page > 0 || (first->def->flags1 & 0x80000000)) {
            char name[256];
            char gui[256];
            strncpy(name, g_game->unitDefs[first->unitDefIndex].unitname, 0x20);
            name[0x1f] = 0;
            sprintf(gui, "%s%d.GUI", name, page);
            if ((IsScreenNamed(&g_game->menu, gui) == 0 || g_game->unitIndex != first->id)
                && PopUntilNamedLayout(0))
                OpenBuildMenuGui(first, gui, page);
            g_game->orders.refresh = 0;
        }
    }
    if (g_game->orders.refresh) {
        if (PopUntilNamedLayout(0)) {
            if (count == 1) {
                OpenGeneratorDialog(first);
                return;
            }
            OpenGeneratorDialog(0);
        }
    }
}
