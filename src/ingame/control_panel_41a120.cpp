// Decompiled by Claude Opus 5.5. Names are provisional.
// Refreshes the order buttons of the unit menu: the BUILD/ORDERS page toggle
// for the selected unit, the multi-state CLOAK, ONOFF, MOVEORD and FIREORD
// buttons from the selection's combined order state, and disables every order
// button the selection cannot use.

#pragma pack(push, 1)
struct UnitType_0041a120 {
    char unknown_0[0x22e];
    unsigned char field_22e;           // +0x22e
};

struct Unit_0041a120 {
    char unknown_0[0x92];
    UnitType_0041a120* type;           // +0x92
    char unknown_96[0x110 - 0x96];
    unsigned int bits_0 : 22;          // +0x110
    unsigned int buildPage : 1;        // +0x110 bit 22
    unsigned int bits_23 : 9;
};

struct Layer_0041a120 {
    int unknown_0;
    int value;                         // +0x4
};

struct Menu_0041a120 {
    char unknown_0[0x18];
    Layer_0041a120* layer;             // +0x18
};

struct Orders_0041a120 {
    unsigned short unknown_0 : 12;     // +0x37ebe
    unsigned short fireOrder : 3;      // +0x37ebe bits 12-14
    unsigned short unknown_15 : 1;
    unsigned short moveOrder : 3;      // +0x37ec0 bits 0-2
    unsigned short cloak : 2;          // +0x37ec0 bits 3-4
    unsigned short onOff : 2;          // +0x37ec0 bits 5-6
    unsigned short canMove : 1;        // +0x37ec0 bit 7
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

struct Game_0041a120 {
    char unknown_0[0x519];
    Menu_0041a120 menu;                // +0x519
    char unknown_535[0x37ebe - 0x535];
    Orders_0041a120 orders;            // +0x37ebe
};
#pragma pack(pop)

extern Game_0041a120* g_game;

int __stdcall FUN_0049fe60(int value, char* name);
// 0x4a11c0.cpp declares the value `short`; here it must be `int`, or the
// bitfield reads are narrowed (`shr cl, 3`) and every register changes.
void __stdcall FUN_004a11c0(Menu_0041a120* menu, int index, int value);
void __stdcall FUN_004a1200(Menu_0041a120* menu, int index, int value);
void __stdcall FUN_004a03f0(Menu_0041a120* menu, int index, char value);

// FUNCTION: 0x41a120
void __stdcall FUN_0041a120(Unit_0041a120* unit)
{
    Menu_0041a120* menu = &g_game->menu;
    int layer = g_game->menu.layer->value;
    int index;

    index = FUN_0049fe60(layer, "BUILD");
    if (index != -1) {
        if (unit && unit->type->field_22e) {
            FUN_004a11c0(menu, index, unit->buildPage);
        } else {
            FUN_004a1200(menu, index, 1);
        }
    }
    index = FUN_0049fe60(layer, "ORDERS");
    if (index != -1) {
        if (unit && unit->type->field_22e) {
            FUN_004a11c0(menu, index, !unit->buildPage);
        } else {
            FUN_004a1200(menu, index, 1);
        }
    }
    index = FUN_0049fe60(layer, "CLOAK");
    if (index != -1) {
        if (g_game->orders.cloak == 3) {
            FUN_004a1200(menu, index, 1);
        } else {
            FUN_004a11c0(menu, index, g_game->orders.cloak);
        }
    }
    index = FUN_0049fe60(layer, "ONOFF");
    if (index != -1) {
        if (g_game->orders.onOff == 3) {
            FUN_004a1200(menu, index, 1);
        } else {
            FUN_004a11c0(menu, index, g_game->orders.onOff);
        }
    }
    index = FUN_0049fe60(layer, "MOVEORD");
    if (index != -1) {
        if (g_game->orders.moveOrder == 4) {
            FUN_004a1200(menu, index, 1);
        } else {
            FUN_004a11c0(menu, index, g_game->orders.moveOrder);
        }
    }
    index = FUN_0049fe60(layer, "FIREORD");
    if (index != -1) {
        if (g_game->orders.fireOrder == 4) {
            FUN_004a1200(menu, index, 1);
        } else {
            FUN_004a11c0(menu, index, g_game->orders.fireOrder);
        }
    }
    if (!g_game->orders.canMove) {
        index = FUN_0049fe60(layer, "MOVE");
        if (index != -1) {
            FUN_004a1200(menu, index, 1);
        }
    }
    if (!g_game->orders.canStop) {
        index = FUN_0049fe60(layer, "STOP");
        if (index != -1) {
            FUN_004a1200(menu, index, 1);
        }
    }
    if (!g_game->orders.canAttack) {
        index = FUN_0049fe60(layer, "ATTACK");
        if (index != -1) {
            FUN_004a1200(menu, index, 1);
        }
    }
    if (!g_game->orders.canDefend) {
        index = FUN_0049fe60(layer, "DEFEND");
        if (index != -1) {
            FUN_004a1200(menu, index, 1);
        }
    }
    if (!g_game->orders.canPatrol) {
        index = FUN_0049fe60(layer, "PATROL");
        if (index != -1) {
            FUN_004a1200(menu, index, 1);
        }
    }
    if (!g_game->orders.canReclaim) {
        index = FUN_0049fe60(layer, "RECLAIM");
        if (index != -1) {
            FUN_004a1200(menu, index, 1);
        }
    }
    if (!g_game->orders.canRepair) {
        index = FUN_0049fe60(layer, "REPAIR");
        if (index != -1) {
            FUN_004a1200(menu, index, 1);
        }
    }
    if (!g_game->orders.canCapture) {
        index = FUN_0049fe60(layer, "CAPTURE");
        if (index != -1) {
            FUN_004a1200(menu, index, 1);
        }
    }
    if (!g_game->orders.canLoad) {
        index = FUN_0049fe60(layer, "LOAD");
        if (index != -1) {
            FUN_004a03f0(menu, index, 0);
        }
        index = FUN_0049fe60(layer, "UNLOAD");
        if (index != -1) {
            FUN_004a1200(menu, index, 1);
        }
        if (!g_game->orders.canBlast) {
            index = FUN_0049fe60(layer, "BLAST");
            if (index != -1) {
                FUN_004a1200(menu, index, 1);
            }
        }
    } else {
        index = FUN_0049fe60(layer, "BLAST");
        if (index != -1) {
            FUN_004a03f0(menu, index, 0);
        }
    }
}
