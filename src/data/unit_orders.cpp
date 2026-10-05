// The unit order types: what a unit can be told to do, by name ("Move_Ground",
// "VTOL_Patrol"), with the function that runs it and the status text the game
// shows while it does. 0x43bc90 adds each table to the sorted table of order
// types at 0x512340 (a std::vector<Elem_0043c390>), where the command and
// script code looks orders up by name.

struct Unit;
struct Order;
struct View;
struct Pos;

#pragma pack(push, 1)
struct UnitOrderType {
    const char* status;                                         // +0x00 shown while the order runs
    int (__stdcall* run)(Unit* unit, Order* order, int flags);  // +0x04 (0x43bad0 calls it)
    // +0x08 marks the order's target on screen.
    void (__stdcall* draw)(void* surface, View* view, Order* order, Pos* out, int flag);
    int target;                                                 // +0x0c
    unsigned int flags;                                         // +0x10
    unsigned char field_14;                                     // +0x14
    const char* name;                                           // +0x15 the key the table is sorted by
};
#pragma pack(pop)

int __stdcall FUN_00401c20(Unit* unit, Order* order, int flags);
int __stdcall FUN_00401cc0(Unit* unit, Order* order, int flags);
int __stdcall FUN_00401ce0(Unit* unit, Order* order, int flags);
int __stdcall FUN_00401e00(Unit* unit, Order* order, int flags);
int __stdcall FUN_00401fd0(Unit* unit, Order* order, int flags);
int __stdcall FUN_00402010(Unit* unit, Order* order, int flags);
int __stdcall FUN_00402160(Unit* unit, Order* order, int flags);
int __stdcall FUN_004021f0(Unit* unit, Order* order, int flags);
int __stdcall FUN_00402430(Unit* unit, Order* order, int flags);
int __stdcall FUN_00402640(Unit* unit, Order* order, int flags);
int __stdcall FUN_00402b70(Unit* unit, Order* order, int flags);
int __stdcall FUN_00402d10(Unit* unit, Order* order, int flags);
int __stdcall FUN_00402da0(Unit* unit, Order* order, int flags);
int __stdcall FUN_00402fc0(Unit* unit, Order* order, int flags);
int __stdcall FUN_00403010(Unit* unit, Order* order, int flags);
int __stdcall FUN_00403040(Unit* unit, Order* order, int flags);
int __stdcall FUN_00403070(Unit* unit, Order* order, int flags);
int __stdcall FUN_004030a0(Unit* unit, Order* order, int flags);
int __stdcall FUN_004030d0(Unit* unit, Order* order, int flags);
int __stdcall FUN_00403100(Unit* unit, Order* order, int flags);
int __stdcall FUN_00403160(Unit* unit, Order* order, int flags);
int __stdcall FUN_00403190(Unit* unit, Order* order, int flags);
int __stdcall FUN_004031d0(Unit* unit, Order* order, int flags);
int __stdcall FUN_00403260(Unit* unit, Order* order, int flags);
int __stdcall FUN_004033a0(Unit* unit, Order* order, int flags);
int __stdcall FUN_004034a0(Unit* unit, Order* order, int flags);
int __stdcall FUN_004038a0(Unit* unit, Order* order, int flags);
int __stdcall FUN_00403a20(Unit* unit, Order* order, int flags);
int __stdcall FUN_00403f70(Unit* unit, Order* order, int flags);
int __stdcall FUN_00404270(Unit* unit, Order* order, int flags);
int __stdcall FUN_00404730(Unit* unit, Order* order, int flags);
int __stdcall FUN_00404ad0(Unit* unit, Order* order, int flags);
int __stdcall FUN_00404db0(Unit* unit, Order* order, int flags);
int __stdcall FUN_00405300(Unit* unit, Order* order, int flags);
int __stdcall FUN_00405740(Unit* unit, Order* order, int flags);
int __stdcall FUN_00405980(Unit* unit, Order* order, int flags);
int __stdcall FUN_00405fe0(Unit* unit, Order* order, int flags);
int __stdcall FUN_00406090(Unit* unit, Order* order, int flags);
int __stdcall FUN_004061a0(Unit* unit, Order* order, int flags);
int __stdcall FUN_00406300(Unit* unit, Order* order, int flags);
int __stdcall FUN_00406780(Unit* unit, Order* order, int flags);
int __stdcall FUN_00406900(Unit* unit, Order* order, int flags);
int __stdcall FUN_00406aa0(Unit* unit, Order* order, int flags);
int __stdcall FUN_0040f2a0(Unit* unit, Order* order, int flags);
int __stdcall FUN_0040f7d0(Unit* unit, Order* order, int flags);
int __stdcall FUN_0040fa20(Unit* unit, Order* order, int flags);
int __stdcall FUN_0040fbe0(Unit* unit, Order* order, int flags);
int __stdcall FUN_004103e0(Unit* unit, Order* order, int flags);
int __stdcall FUN_00410850(Unit* unit, Order* order, int flags);
int __stdcall FUN_00410e70(Unit* unit, Order* order, int flags);
int __stdcall FUN_004111b0(Unit* unit, Order* order, int flags);
int __stdcall FUN_00411560(Unit* unit, Order* order, int flags);
int __stdcall FUN_004118e0(Unit* unit, Order* order, int flags);
int __stdcall FUN_00411f50(Unit* unit, Order* order, int flags);
int __stdcall FUN_00412710(Unit* unit, Order* order, int flags);
int __stdcall FUN_00412d40(Unit* unit, Order* order, int flags);
int __stdcall FUN_00413470(Unit* unit, Order* order, int flags);
int __stdcall FUN_00413bc0(Unit* unit, Order* order, int flags);
int __stdcall FUN_00413d80(Unit* unit, Order* order, int flags);
int __stdcall FUN_00414380(Unit* unit, Order* order, int flags);
int __stdcall FUN_00414770(Unit* unit, Order* order, int flags);
int __stdcall FUN_00414a80(Unit* unit, Order* order, int flags);
int __stdcall FUN_00414e70(Unit* unit, Order* order, int flags);
int __stdcall FUN_00415250(Unit* unit, Order* order, int flags);
int __stdcall FUN_004152f0(Unit* unit, Order* order, int flags);
int __stdcall FUN_00439ea0(Unit* unit, Order* order, int flags);
void __stdcall FUN_00438c00(void* surface, View* view, Order* order, Pos* out, int flag);
void __stdcall FUN_004394e0(void* surface, View* view, Order* order, Pos* out, int flag);
void __stdcall FUN_00439740(void* surface, View* view, Order* order, Pos* out, int flag);

// every unit's orders, registered by 0x403180.
// GLOBAL: 0x4fc490
extern const UnitOrderType g_unitOrders[23] = {
    {"Stopping", FUN_00401c20, 0, 0, 0x13, 0, "Stop"},
    {"Attacking", FUN_00402160, FUN_00439740, 8, 0x28001, 0, "Attack_NoMove"},
    {"Activate", FUN_00403010, 0, 0, 0x1006013, 0, "Activate"},
    {"Deactivate", FUN_00403040, 0, 0, 0x1006013, 0, "Deactivate"},
    {"Cloaking", FUN_00403070, 0, 0, 0x1006013, 0, "Cloak_On"},
    {"Decloaking", FUN_004030a0, 0, 0, 0x1006013, 0, "Cloak_Off"},
    {"Acknowledged", FUN_004030d0, 0, 0, 0x1006013, 0, "Standing_MoveOrder"},
    {"Acknowledged", FUN_00403100, 0, 0, 0x1006013, 0, "Standing_FireOrder"},
    {"Nanolathing", FUN_00402640, 0, 0, 0x10010c13, 0, "BuildingBuild"},
    {"Nanolathing", FUN_00402b70, 0, 0, 0xc014013, 0, "BuildWeapon"},
    {"SELF DESTRUCT ENGAGED", FUN_00402010, 0, 0, 0x4004013, 0, "SelfDestruct"},
    {"SELF DESTRUCT ENGAGED", FUN_00402010, 0, 0, 0x13, 0, "SelfDestructFG"},
    {"Paralyzed", FUN_00402d10, 0, 0, 0x2413, 0, "Paralyze"},
    {"Under construction", FUN_00402da0, 0, 0, 0x22413, 0, "GetBuilt"},
    {"Being transported", FUN_00402fc0, 0, 0, 0x2413, 0, "BeCarried"},
    {"Unit is available", FUN_00401cc0, 0, 0, 0x413, 0, "MakeSelectable"},
    {"Waiting", FUN_00401ce0, 0, 0, 0x413, 0, "Wait"},
    {"Waiting for attack", FUN_00401fd0, 0, 0, 0x20413, 0, "WaitForAttack"},
    {"Attacking", FUN_00401e00, 0, 0, 0x413, 0, "AttackUType"},
    {"Ready", FUN_004021f0, 0, 0, 0x2013, 0, "Guard_NoMove"},
    {"Repairing", FUN_00402430, 0, 0, 0x20413, 1, "SelfRepair"},
    {"Ready with orders", FUN_00403160, FUN_004394e0, 2, 0x4000e, 0, "QMove"},
    {"Ready with orders", FUN_00403160, FUN_004394e0, 2, 0x40007, 0, "QPatrol"},
};

// the orders of units that move on the ground, registered by 0x406bf0.
// GLOBAL: 0x4fc6e8
extern const UnitOrderType g_groundOrders[22] = {
    {"Standby", FUN_00405fe0, 0, 0x10, 0x200000f, 0, "Standby"},
    {"Standby", FUN_00406090, 0, 0x10, 0x200000f, 1, "Standby_Mine"},
    {"Moving", FUN_004031d0, FUN_004394e0, 0x12, 0x4020e, 0, "Move_Ground"},
    {"Guarding", FUN_00406300, FUN_004394e0, 0x12, 0x20005, 0, "Follow_Ground"},
    {"Suppressing fire", FUN_004038a0, FUN_00439740, 8, 0x41001, 0, "Suppress"},
    {"Attacking", FUN_004034a0, FUN_00439740, 8, 0x28001, 0, "Attack_Chase"},
    {"Attacking", FUN_00403260, FUN_00439740, 8, 0x60001, 0, "Attack_Kamikaze"},
    {"Annihilating", FUN_00403190, FUN_00439740, 8, 0x68001, 0, "AttackSpecial"},
    {"Parking", FUN_004061a0, 0, 0, 0xe, 0, "Park"},
    {"Patrolling", FUN_004033a0, FUN_004394e0, 0x12, 0x41207, 0, "Patrol"},
    {"Loading", FUN_00406780, FUN_00439740, 8, 0x2000c, 0, "Ground_Pickup"},
    {"Unloading", FUN_00406900, FUN_00439740, 8, 0x4000d, 0, "Ground_Unload"},
    {"Teleporting", FUN_00406aa0, FUN_00439740, 8, 0x60009, 0, "Teleport"},
    {"Nanolathing", FUN_00403a20, FUN_00438c00, 0x13, 0x10050800, 0, "MobileBuild"},
    {"Nanolathing", FUN_00403f70, FUN_00439740, 0x18, 0x10020806, 0, "HelpBuild"},
    {"Repair patrol", FUN_00405980, FUN_004394e0, 0x12, 0x41207, 0, "RepairPatrol"},
    {"Repairing", FUN_00405300, FUN_004394e0, 0x12, 0x10020006, 0, "RepairUnit"},
    {"Capturing", FUN_00404270, FUN_00439740, 8, 0x20004, 0, "Capture"},
    {"Resurrecting", FUN_00404db0, FUN_004394e0, 0x12, 0x2000b, 0, "Resurrect"},
    {"Reclaiming", FUN_00404ad0, FUN_004394e0, 0x12, 0x1008000b, 0, "Reclaim"},
    {"Reclaiming", FUN_00404730, FUN_004394e0, 0x12, 0x1002000b, 0, "ReclaimUnit"},
    {"Repairing", FUN_00405740, FUN_00439740, 0x18, 0x20006, 0, "RepairUnitNoMove"},
};

// the orders of aircraft, registered by 0x415b20.
// GLOBAL: 0x4fca18
extern const UnitOrderType g_vtolOrders[22] = {
    {"Standby", FUN_0040f7d0, 0, 0, 0x200000f, 0, "VTOL_Standby"},
    {"Moving", FUN_0040fa20, FUN_004394e0, 2, 0x4020e, 0, "VTOL_Move"},
    {"Landing", FUN_004118e0, FUN_00439740, 8, 0x6000e, 0, "VTOL_Landing"},
    {"Loading", FUN_004111b0, FUN_00439740, 8, 0x20008, 0, "VTOL_Pickup"},
    {"Unloading", FUN_00411560, FUN_00439740, 8, 0x40009, 0, "VTOL_Unload"},
    {"Guarding", FUN_0040fbe0, FUN_004394e0, 2, 0x20005, 0, "VTOL_Follow"},
    {"Patrolling", FUN_00410e70, FUN_004394e0, 2, 0x41207, 0, "VTOL_Patrol"},
    {"Airstrike", FUN_00411f50, FUN_00439740, 8, 0x60002, 0, "AirStrike"},
    {"Engaging target", FUN_00412d40, FUN_00439740, 8, 0x20001, 0, "AirToAir"},
    {"Engaging target", FUN_00412710, FUN_00439740, 8, 0x20001, 0, "AirToGround"},
    {"Engaging target", FUN_00413470, FUN_00439740, 8, 0x20001, 0, "AirToGroundHover"},
    {"Nanolathing", FUN_00413d80, FUN_00438c00, 3, 0x10050800, 0, "VTOL_MobileBuild"},
    {"Nanolathing", FUN_00414380, FUN_00439740, 8, 0x10020806, 0, "VTOL_HelpBuild"},
    {"Repair patrol", FUN_004152f0, FUN_004394e0, 2, 0x41207, 0, "VTOL_RepairPatrol"},
    {"Repairing", FUN_00414e70, FUN_004394e0, 2, 0x10020006, 0, "VTOL_RepairUnit"},
    {"Reclaiming", FUN_00414770, FUN_004394e0, 2, 0x1008000b, 0, "VTOL_Reclaim"},
    {"Reclaiming", FUN_00414a80, FUN_004394e0, 2, 0x1002000b, 0, "VTOL_ReclaimUnit"},
    {"Evading", FUN_00413bc0, 0, 0, 0x13, 0, "VTOL_Evade"},
    {"Seeking to attack", FUN_004103e0, 0, 0, 0x60013, 0, "VTOL_SeekAttack"},
    {"Seeking to guard", FUN_00410850, 0, 0, 0x60013, 0, "VTOL_SeekGuard"},
    {"Under repair", FUN_00415250, 0, 0, 0x20013, 0, "VTOL_GetRepaired"},
    {"Seeking to land", FUN_0040f2a0, 0, 0, 0x40013, 0, "VTOL_LandIfCan"},
};

// One order, registered on its own by 0x43bc90.
// GLOBAL: 0x4fd288
extern const UnitOrderType g_readyOrder[1] = {
    {"Ready", FUN_00439ea0, 0, 0, 0xf, 0, ""},
};

