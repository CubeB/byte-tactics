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

int __stdcall StopOrder(Unit* unit, Order* order, int flags);
int __stdcall MakeSelectableOrder(Unit* unit, Order* order, int flags);
int __stdcall WaitOrder(Unit* unit, Order* order, int flags);
int __stdcall AttackUTypeOrder(Unit* unit, Order* order, int flags);
int __stdcall WaitForAttackOrder(Unit* unit, Order* order, int flags);
int __stdcall SelfDestructOrder(Unit* unit, Order* order, int flags);
int __stdcall AttackNoMoveOrder(Unit* unit, Order* order, int flags);
int __stdcall GuardNoMoveOrder(Unit* unit, Order* order, int flags);
int __stdcall SelfRepairOrder(Unit* unit, Order* order, int flags);
int __stdcall BuildingBuildOrder(Unit* unit, Order* order, int flags);
int __stdcall BuildWeaponOrder(Unit* unit, Order* order, int flags);
int __stdcall ParalyzeOrder(Unit* unit, Order* order, int flags);
int __stdcall GetBuiltOrder(Unit* unit, Order* order, int flags);
int __stdcall BeCarriedOrder(Unit* unit, Order* order, int flags);
int __stdcall ActivateOrder(Unit* unit, Order* order, int flags);
int __stdcall DeactivateOrder(Unit* unit, Order* order, int flags);
int __stdcall CloakOnOrder(Unit* unit, Order* order, int flags);
int __stdcall CloakOffOrder(Unit* unit, Order* order, int flags);
int __stdcall StandingMoveOrder(Unit* unit, Order* order, int flags);
int __stdcall StandingFireOrder(Unit* unit, Order* order, int flags);
int __stdcall QMoveQPatrolOrder(Unit* unit, Order* order, int flags);
int __stdcall AttackSpecialOrder(Unit* unit, Order* order, int flags);
int __stdcall MoveGroundOrder(Unit* unit, Order* order, int flags);
int __stdcall AttackKamikazeOrder(Unit* unit, Order* order, int flags);
int __stdcall PatrolOrder(Unit* unit, Order* order, int flags);
int __stdcall AttackChaseOrder(Unit* unit, Order* order, int flags);
int __stdcall SuppressOrder(Unit* unit, Order* order, int flags);
int __stdcall MobileBuildOrder(Unit* unit, Order* order, int flags);
int __stdcall HelpBuildOrder(Unit* unit, Order* order, int flags);
int __stdcall CaptureOrder(Unit* unit, Order* order, int flags);
int __stdcall ReclaimUnitOrder(Unit* unit, Order* order, int flags);
int __stdcall ReclaimOrder(Unit* unit, Order* order, int flags);
int __stdcall ResurrectOrder(Unit* unit, Order* order, int flags);
int __stdcall RepairUnitOrder(Unit* unit, Order* order, int flags);
int __stdcall RepairUnitNoMoveOrder(Unit* unit, Order* order, int flags);
int __stdcall RepairPatrolOrder(Unit* unit, Order* order, int flags);
int __stdcall StandbyOrder(Unit* unit, Order* order, int flags);
int __stdcall StandbyMineOrder(Unit* unit, Order* order, int flags);
int __stdcall ParkOrder(Unit* unit, Order* order, int flags);
int __stdcall FollowGroundOrder(Unit* unit, Order* order, int flags);
int __stdcall GroundPickupOrder(Unit* unit, Order* order, int flags);
int __stdcall GroundUnloadOrder(Unit* unit, Order* order, int flags);
int __stdcall TeleportOrder(Unit* unit, Order* order, int flags);
int __stdcall VtolLandIfCanOrder(Unit* unit, Order* order, int flags);
int __stdcall VtolStandbyOrder(Unit* unit, Order* order, int flags);
int __stdcall VtolMoveOrder(Unit* unit, Order* order, int flags);
int __stdcall VtolFollowOrder(Unit* unit, Order* order, int flags);
int __stdcall VtolSeekAttackOrder(Unit* unit, Order* order, int flags);
int __stdcall VtolSeekGuardOrder(Unit* unit, Order* order, int flags);
int __stdcall VtolPatrolOrder(Unit* unit, Order* order, int flags);
int __stdcall VtolPickupOrder(Unit* unit, Order* order, int flags);
int __stdcall VtolUnloadOrder(Unit* unit, Order* order, int flags);
int __stdcall VtolLandingOrder(Unit* unit, Order* order, int flags);
int __stdcall AirStrikeOrder(Unit* unit, Order* order, int flags);
int __stdcall AirToGroundOrder(Unit* unit, Order* order, int flags);
int __stdcall AirToAirOrder(Unit* unit, Order* order, int flags);
int __stdcall AirToGroundHoverOrder(Unit* unit, Order* order, int flags);
int __stdcall VtolEvadeOrder(Unit* unit, Order* order, int flags);
int __stdcall VtolMobileBuildOrder(Unit* unit, Order* order, int flags);
int __stdcall VtolHelpBuildOrder(Unit* unit, Order* order, int flags);
int __stdcall VtolReclaimOrder(Unit* unit, Order* order, int flags);
int __stdcall VtolReclaimUnitOrder(Unit* unit, Order* order, int flags);
int __stdcall VtolRepairUnitOrder(Unit* unit, Order* order, int flags);
int __stdcall VtolGetRepairedOrder(Unit* unit, Order* order, int flags);
int __stdcall VtolRepairPatrolOrder(Unit* unit, Order* order, int flags);
int __stdcall ReadyOrder(Unit* unit, Order* order, int flags);
void __stdcall DrawBuildFootprint(void* surface, View* view, Order* order, Pos* out, int flag);
void __stdcall DrawPathAnim(void* surface, View* view, Order* order, Pos* out, int flag);
void __stdcall DrawWeaponCoverage(void* surface, View* view, Order* order, Pos* out, int flag);

// every unit's orders, registered by 0x403180.
// GLOBAL: 0x4fc490
extern const UnitOrderType g_unitOrders[23] = {
    {"Stopping", StopOrder, 0, 0, 0x13, 0, "Stop"},
    {"Attacking", AttackNoMoveOrder, DrawWeaponCoverage, 8, 0x28001, 0, "Attack_NoMove"},
    {"Activate", ActivateOrder, 0, 0, 0x1006013, 0, "Activate"},
    {"Deactivate", DeactivateOrder, 0, 0, 0x1006013, 0, "Deactivate"},
    {"Cloaking", CloakOnOrder, 0, 0, 0x1006013, 0, "Cloak_On"},
    {"Decloaking", CloakOffOrder, 0, 0, 0x1006013, 0, "Cloak_Off"},
    {"Acknowledged", StandingMoveOrder, 0, 0, 0x1006013, 0, "Standing_MoveOrder"},
    {"Acknowledged", StandingFireOrder, 0, 0, 0x1006013, 0, "Standing_FireOrder"},
    {"Nanolathing", BuildingBuildOrder, 0, 0, 0x10010c13, 0, "BuildingBuild"},
    {"Nanolathing", BuildWeaponOrder, 0, 0, 0xc014013, 0, "BuildWeapon"},
    {"SELF DESTRUCT ENGAGED", SelfDestructOrder, 0, 0, 0x4004013, 0, "SelfDestruct"},
    {"SELF DESTRUCT ENGAGED", SelfDestructOrder, 0, 0, 0x13, 0, "SelfDestructFG"},
    {"Paralyzed", ParalyzeOrder, 0, 0, 0x2413, 0, "Paralyze"},
    {"Under construction", GetBuiltOrder, 0, 0, 0x22413, 0, "GetBuilt"},
    {"Being transported", BeCarriedOrder, 0, 0, 0x2413, 0, "BeCarried"},
    {"Unit is available", MakeSelectableOrder, 0, 0, 0x413, 0, "MakeSelectable"},
    {"Waiting", WaitOrder, 0, 0, 0x413, 0, "Wait"},
    {"Waiting for attack", WaitForAttackOrder, 0, 0, 0x20413, 0, "WaitForAttack"},
    {"Attacking", AttackUTypeOrder, 0, 0, 0x413, 0, "AttackUType"},
    {"Ready", GuardNoMoveOrder, 0, 0, 0x2013, 0, "Guard_NoMove"},
    {"Repairing", SelfRepairOrder, 0, 0, 0x20413, 1, "SelfRepair"},
    {"Ready with orders", QMoveQPatrolOrder, DrawPathAnim, 2, 0x4000e, 0, "QMove"},
    {"Ready with orders", QMoveQPatrolOrder, DrawPathAnim, 2, 0x40007, 0, "QPatrol"},
};

// the orders of units that move on the ground, registered by 0x406bf0.
// GLOBAL: 0x4fc6e8
extern const UnitOrderType g_groundOrders[22] = {
    {"Standby", StandbyOrder, 0, 0x10, 0x200000f, 0, "Standby"},
    {"Standby", StandbyMineOrder, 0, 0x10, 0x200000f, 1, "Standby_Mine"},
    {"Moving", MoveGroundOrder, DrawPathAnim, 0x12, 0x4020e, 0, "Move_Ground"},
    {"Guarding", FollowGroundOrder, DrawPathAnim, 0x12, 0x20005, 0, "Follow_Ground"},
    {"Suppressing fire", SuppressOrder, DrawWeaponCoverage, 8, 0x41001, 0, "Suppress"},
    {"Attacking", AttackChaseOrder, DrawWeaponCoverage, 8, 0x28001, 0, "Attack_Chase"},
    {"Attacking", AttackKamikazeOrder, DrawWeaponCoverage, 8, 0x60001, 0, "Attack_Kamikaze"},
    {"Annihilating", AttackSpecialOrder, DrawWeaponCoverage, 8, 0x68001, 0, "AttackSpecial"},
    {"Parking", ParkOrder, 0, 0, 0xe, 0, "Park"},
    {"Patrolling", PatrolOrder, DrawPathAnim, 0x12, 0x41207, 0, "Patrol"},
    {"Loading", GroundPickupOrder, DrawWeaponCoverage, 8, 0x2000c, 0, "Ground_Pickup"},
    {"Unloading", GroundUnloadOrder, DrawWeaponCoverage, 8, 0x4000d, 0, "Ground_Unload"},
    {"Teleporting", TeleportOrder, DrawWeaponCoverage, 8, 0x60009, 0, "Teleport"},
    {"Nanolathing", MobileBuildOrder, DrawBuildFootprint, 0x13, 0x10050800, 0, "MobileBuild"},
    {"Nanolathing", HelpBuildOrder, DrawWeaponCoverage, 0x18, 0x10020806, 0, "HelpBuild"},
    {"Repair patrol", RepairPatrolOrder, DrawPathAnim, 0x12, 0x41207, 0, "RepairPatrol"},
    {"Repairing", RepairUnitOrder, DrawPathAnim, 0x12, 0x10020006, 0, "RepairUnit"},
    {"Capturing", CaptureOrder, DrawWeaponCoverage, 8, 0x20004, 0, "Capture"},
    {"Resurrecting", ResurrectOrder, DrawPathAnim, 0x12, 0x2000b, 0, "Resurrect"},
    {"Reclaiming", ReclaimOrder, DrawPathAnim, 0x12, 0x1008000b, 0, "Reclaim"},
    {"Reclaiming", ReclaimUnitOrder, DrawPathAnim, 0x12, 0x1002000b, 0, "ReclaimUnit"},
    {"Repairing", RepairUnitNoMoveOrder, DrawWeaponCoverage, 0x18, 0x20006, 0, "RepairUnitNoMove"},
};

// the orders of aircraft, registered by 0x415b20.
// GLOBAL: 0x4fca18
extern const UnitOrderType g_vtolOrders[22] = {
    {"Standby", VtolStandbyOrder, 0, 0, 0x200000f, 0, "VTOL_Standby"},
    {"Moving", VtolMoveOrder, DrawPathAnim, 2, 0x4020e, 0, "VTOL_Move"},
    {"Landing", VtolLandingOrder, DrawWeaponCoverage, 8, 0x6000e, 0, "VTOL_Landing"},
    {"Loading", VtolPickupOrder, DrawWeaponCoverage, 8, 0x20008, 0, "VTOL_Pickup"},
    {"Unloading", VtolUnloadOrder, DrawWeaponCoverage, 8, 0x40009, 0, "VTOL_Unload"},
    {"Guarding", VtolFollowOrder, DrawPathAnim, 2, 0x20005, 0, "VTOL_Follow"},
    {"Patrolling", VtolPatrolOrder, DrawPathAnim, 2, 0x41207, 0, "VTOL_Patrol"},
    {"Airstrike", AirStrikeOrder, DrawWeaponCoverage, 8, 0x60002, 0, "AirStrike"},
    {"Engaging target", AirToAirOrder, DrawWeaponCoverage, 8, 0x20001, 0, "AirToAir"},
    {"Engaging target", AirToGroundOrder, DrawWeaponCoverage, 8, 0x20001, 0, "AirToGround"},
    {"Engaging target", AirToGroundHoverOrder, DrawWeaponCoverage, 8, 0x20001, 0, "AirToGroundHover"},
    {"Nanolathing", VtolMobileBuildOrder, DrawBuildFootprint, 3, 0x10050800, 0, "VTOL_MobileBuild"},
    {"Nanolathing", VtolHelpBuildOrder, DrawWeaponCoverage, 8, 0x10020806, 0, "VTOL_HelpBuild"},
    {"Repair patrol", VtolRepairPatrolOrder, DrawPathAnim, 2, 0x41207, 0, "VTOL_RepairPatrol"},
    {"Repairing", VtolRepairUnitOrder, DrawPathAnim, 2, 0x10020006, 0, "VTOL_RepairUnit"},
    {"Reclaiming", VtolReclaimOrder, DrawPathAnim, 2, 0x1008000b, 0, "VTOL_Reclaim"},
    {"Reclaiming", VtolReclaimUnitOrder, DrawPathAnim, 2, 0x1002000b, 0, "VTOL_ReclaimUnit"},
    {"Evading", VtolEvadeOrder, 0, 0, 0x13, 0, "VTOL_Evade"},
    {"Seeking to attack", VtolSeekAttackOrder, 0, 0, 0x60013, 0, "VTOL_SeekAttack"},
    {"Seeking to guard", VtolSeekGuardOrder, 0, 0, 0x60013, 0, "VTOL_SeekGuard"},
    {"Under repair", VtolGetRepairedOrder, 0, 0, 0x20013, 0, "VTOL_GetRepaired"},
    {"Seeking to land", VtolLandIfCanOrder, 0, 0, 0x40013, 0, "VTOL_LandIfCan"},
};

// One order, registered on its own by 0x43bc90.
// GLOBAL: 0x4fd288
extern const UnitOrderType g_readyOrder[1] = {
    {"Ready", ReadyOrder, 0, 0, 0xf, 0, ""},
};

