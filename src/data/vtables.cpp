// Virtual function tables that the code stores by hand. The constructors and
// destructors of these classes are matched with `this->vtable = &DAT_...;`
// rather than as classes with virtual functions (docs/consolidation.md), so
// no object emits these tables: they are defined here, one function pointer
// per slot, in the original's order. A class's own table follows its base's
// slots; `_purecall` fills a pure virtual slot.

typedef void (*VirtualFunction)();

extern "C" int __cdecl _purecall(void);

void FUN_00407e70();
void FUN_00407e90();
void GetType();
void FUN_0044ce50();
void SerializeSave();
void FUN_0044ce90();
void ApproxDist();
void KeepAfterComplete();
void FUN_0044cf00();
void ContainsCell();
void IsFxStyle();
void TryGetDesiredHeading();
void FUN_0044cf50();
void FUN_0044cfe0();
void FUN_0044cff0();
void FUN_0044d090();
void FUN_0044d0e0();
void FUN_0044d290();
void FUN_0044d2c0();
void FUN_0044d310();
void FUN_0044d350();
void FUN_0044d440();
void FUN_0044d450();
void FUN_0044d500();
void FUN_0044d560();
void FUN_0044d720();
void FUN_0044d7c0();
void FUN_0044d800();
void FUN_0044d840();
void FUN_0044d900();
void FUN_0044d910();
void FUN_0044d9a0();
void FUN_0044da00();
void FUN_0044dc60();
void FUN_0044dcb0();
void FUN_0044dd00();
void FUN_0044ddc0();
void FUN_0044df70();
void FUN_0044df80();
void FUN_0044dfb0();
void FUN_0044e3a0();
void FUN_0044e3c0();
void FUN_0044e530();
void FUN_0044e5b0();
void FUN_0044e6b0();
void FUN_0044e7a0();
void FUN_0044e7b0();
void FUN_0044e880();
void FUN_0044e930();
void FUN_0044ea50();
void FUN_0044ea60();
void FUN_0044eb40();
void FUN_0044eb60();
void FUN_0044ec00();
void FUN_0044ef50();
void FUN_0044ef90();
void FUN_0044efb0();
void FUN_0044efc0();
void FUN_0044efe0();
void FUN_0044eff0();
void FUN_0044f000();
void FUN_0044f590();
void FUN_0044f5b0();
void FUN_0044f5c0();
void FUN_0044f650();
void FUN_00481490();
void AimCobStub();

// Class_00407d40's, a class of the 0x4fc980 family: stored by its constructor 0x407d40.
// GLOBAL: 0x4fc9a0
extern VirtualFunction const DAT_004fc9a0[2] = {
    FUN_00407e90, FUN_00407e70,
};

// The base class of the 0x44ce20 family: stored by the constructors 0x44ce20 to 0x44e330 before their own, and by the destructors 0x44ce50 to 0x44e7b0 last.
// GLOBAL: 0x4fd2f8
extern VirtualFunction const DAT_004fd2f8[12] = {
    FUN_0044ce50, SerializeSave, GetType, IsFxStyle,
    FUN_0044cf00, ContainsCell, FUN_0044ce90, ApproxDist,
    (VirtualFunction)_purecall, TryGetDesiredHeading, FUN_0044cf50, KeepAfterComplete,
};

// Stored by 0x44cf60 and 0x44d010.
// GLOBAL: 0x4fd328
extern VirtualFunction const g_approachRadiusVtable[12] = {
    FUN_0044cff0, FUN_0044d090, FUN_0044cfe0, IsFxStyle,
    FUN_0044d310, FUN_0044d290, FUN_0044d0e0, FUN_0044d350,
    FUN_0044d2c0, TryGetDesiredHeading, FUN_0044cf50, KeepAfterComplete,
};

// Stored by 0x44d3b0 and 0x44d470.
// GLOBAL: 0x4fd358
extern VirtualFunction const g_ringApproachVtable[12] = {
    FUN_0044d450, FUN_0044d500, FUN_0044d440, IsFxStyle,
    FUN_0044d800, FUN_0044d7c0, FUN_0044d560, FUN_0044d840,
    FUN_0044d720, TryGetDesiredHeading, FUN_0044cf50, KeepAfterComplete,
};

// Stored by 0x44d8a0 and 0x44d930.
// GLOBAL: 0x4fd388
extern VirtualFunction const g_pointMarkerVtable[12] = {
    FUN_0044d910, FUN_0044d9a0, FUN_0044d900, IsFxStyle,
    FUN_0044cf00, FUN_0044dcb0, FUN_0044da00, FUN_0044dd00,
    FUN_0044dc60, TryGetDesiredHeading, FUN_0044cf50, KeepAfterComplete,
};

// Stored by 0x44de80, 0x44e080, 0x44e190, 0x44e250, 0x44e2d0 and 0x44e330.
// GLOBAL: 0x4fd3b8
extern VirtualFunction const g_pathOrderVtable[12] = {
    FUN_0044df80, FUN_0044dfb0, FUN_0044df70, FUN_0044e6b0,
    FUN_0044e5b0, ContainsCell, FUN_0044ce90, ApproxDist,
    FUN_0044e3c0, FUN_0044e530, FUN_0044ddc0, FUN_0044e3a0,
};

// Stored by 0x44e740, 0x44e7d0 and 0x44e9c0.
// GLOBAL: 0x4fd3f8
extern VirtualFunction const g_airManeuverOrderVtable[12] = {
    FUN_0044e7b0, FUN_0044e880, FUN_0044e7a0, FUN_0044ec00,
    FUN_0044eb60, ContainsCell, FUN_0044ce90, ApproxDist,
    FUN_0044ea60, FUN_0044eb40, FUN_0044e930, FUN_0044ea50,
};

// Class_0044f570's: stored by 0x44f570.
// GLOBAL: 0x4fd488
extern VirtualFunction const DAT_004fd488[12] = {
    FUN_0044f590, FUN_0044ef90, FUN_0044efb0, FUN_0044f650,
    FUN_0044f000, FUN_0044f5b0, FUN_0044eff0, FUN_0044efe0,
    FUN_0044efc0, FUN_0044f5c0, FUN_0044ef50, 0,
};

// Stored by 0x485e90, 0x485f50 and 0x4861d0.
// GLOBAL: 0x4fd6f0
extern VirtualFunction const g_weaponAimCobVtable[2] = {
    FUN_00481490, AimCobStub,
};
