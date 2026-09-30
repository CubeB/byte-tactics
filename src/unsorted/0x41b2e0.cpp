// Decompiled by Claude Opus 5.5, finished by deepseek-v4.1-flash, finished by GPT-6. Names are provisional.
// Partial: 55.8%, 1537 bytes versus 1512. <vector>/<windows.h>, an
// early canRepair declaration and reordered first/onOff/cloak/count locals
// recover the original 0x240 frame, game in EDI and canRepair in EBP.
// Remaining first/count/onOff/cloak/cursor slots and tail register allocation
// differ. A 768-set header sweep and 80 local/type/lifetime variants were tried.

#include <vector>
#include <windows.h>

#pragma pack(push, 1)
struct UnitType_0041b2e0 {
    char unknown_0[0x22e];
    unsigned char field_22e;           // +0x22e
    char unknown_22f[0x241 - 0x22f];
    unsigned int flags_241;            // +0x241
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
    UnitType_0041b2e0* type;           // +0x92
    char unknown_96[0xa6 - 0x96];
    unsigned short typeIndex;          // +0xa6
    unsigned short id;                 // +0xa8
    char unknown_aa[0x10e - 0xaa];
    unsigned char onOff;               // +0x10e
    char unknown_10f;
    UnitFlags_0041b2e0 flags;          // +0x110
    char unknown_114[0x118 - 0x114];
};

struct Player_0041b2e0 {
    char unknown_0[0x67];
    Unit_0041b2e0* unitsBegin;         // +0x67
    Unit_0041b2e0* unitsEnd;           // +0x6b
    char unknown_6f[0x14b - 0x6f];
};

struct BuildType_0041b2e0 {
    char unknown_0[0x20];
    char name[0x229];                  // +0x20
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
    Player_0041b2e0 players[10];       // +0x1b63
    char unknown_2851[0x2a42 - 0x2851];
    unsigned char localPlayer;         // +0x2a42
    char unknown_2a43[0x14357 - 0x2a43];
    Unit_0041b2e0* units;              // +0x14357
    char unknown_1435b[0x1439b - 0x1435b];
    BuildType_0041b2e0* buildTypes;    // +0x1439b
    char unknown_1439f[0x37e9c - 0x1439f];
    unsigned short unitIndex;          // +0x37e9c
    char unknown_37e9e[0x37ebe - 0x37e9e];
    Orders_0041b2e0 orders;            // +0x37ebe
};
#pragma pack(pop)

extern Game_0041b2e0* g_game;

int __stdcall FUN_00491d70(int force);
int __stdcall FUN_004ab060(Menu_0041b2e0* menu, const char* name);
void __stdcall FUN_0041ace0(Unit_0041b2e0* unit, char* guiName, int page);
void __stdcall FUN_0041b0f0(Unit_0041b2e0* unit);

static inline Unit_0041b2e0* GetUnit(unsigned short index)
{
    Unit_0041b2e0* u = &g_game->units[index];
    if (u->typeIndex == 0)
        u = 0;
    return u;
}

// FUNCTION: 0x41b2e0
void FUN_0041b2e0()
{
    g_game->orders.refresh = 1;
    int canRepair = 0;
    int canLoad = 0;
    int canStop = 0;
    Unit_0041b2e0* first = 0;
    int fireOrder = 4;
    int moveOrder = 4;
    Player_0041b2e0* player = &g_game->players[g_game->localPlayer];
    int canMove = 0;
    int canDefend = 0;
    int onOff = 3;
    int canBlast = 0;
    int canPatrol = 0;
    int cloak = 3;
    int count = 0;
    int canReclaim = 0;
    int canAttack = 0;
    int canCapture = 0;
    if (g_game->unitIndex != 0) {
        count = 1;
        first = GetUnit(g_game->unitIndex);
        if (first == 0) {
            g_game->unitIndex = 0;
            return;
        }
    } else {
        Unit_0041b2e0* last = player->unitsEnd;
        for (Unit_0041b2e0* u = player->unitsBegin; u <= last; u++) {
            if (u->typeIndex == 0)
                continue;
            UnitFlags_0041b2e0 flags = u->flags;
            if (!flags.selected)
                continue;
            if (first == 0)
                first = u;
            UnitType_0041b2e0* type = u->type;
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
                    onOff = u->onOff & 1;
                else if (onOff != (u->onOff & 1))
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
        FUN_00491d70(0);
        g_game->orders.refresh = 0;
    } else if (count == 1 && first->type->field_22e) {
        int page = first->flags.buildPage ? first->flags.page : 0;
        if (page > 0 || (first->type->flags_241 & 0x80000000)) {
            char name[256];
            char gui[256];
            strncpy(name, g_game->buildTypes[first->typeIndex].name, 0x20);
            name[0x1f] = 0;
            sprintf(gui, "%s%d.GUI", name, page);
            if ((FUN_004ab060(&g_game->menu, gui) == 0 || g_game->unitIndex != first->id)
                && FUN_00491d70(0))
                FUN_0041ace0(first, gui, page);
            g_game->orders.refresh = 0;
        }
    }
    if (g_game->orders.refresh) {
        if (FUN_00491d70(0)) {
            if (count == 1) {
                FUN_0041b0f0(first);
                return;
            }
            FUN_0041b0f0(0);
        }
    }
}
