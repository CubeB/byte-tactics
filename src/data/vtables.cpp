// Virtual function tables that the code stores by hand. The constructors and
// destructors of these classes are matched with `this->vtable = &DAT_...;`
// rather than as classes with virtual functions (docs/consolidation.md), so
// no object emits these tables: they are defined here, one function pointer
// per slot, in the original's order. A class's own table follows its base's
// slots; `_purecall` fills a pure virtual slot.

typedef void (*VirtualFunction)();

extern "C" int __cdecl _purecall(void);

// A method several classes share a name for is declared in a namespace named
// after its class, so the slot still names one address in data/symbols.csv.
void FUN_00407e70();
namespace SpatialTimer { void OnTimer(); }
void GetType();
namespace OrderFx { void Destroy(); }
void SerializeSave();
namespace Class_0044ce90 { void FillGoalCells(); }
void ApproxDist();
void KeepAfterComplete();
namespace Class_0044cf00 { void ContainsUnit(); }
void ContainsCell();
void IsFxStyle();
void TryGetDesiredHeading();
void FUN_0044cf50();
void FUN_0044cfe0();
namespace Class_0044cff0 { void Destroy(); }
namespace Class_0044d010 { void Serialize(); }
namespace ApproachRadius { void AppendGoalCell(); }
void FUN_0044d290();
namespace Class_0044d2c0 { void FillWorldPos(); }
namespace Class_0044d310 { void ContainsUnit(); }
namespace Class_0044d350 { void ApproxDistExcess(); }
void FUN_0044d440();
namespace Class_0044d450 { void Destroy(); }
namespace Class_0044d470 { void Serialize(); }
namespace Class_0044d560 { void AppendGoalCell(); }
namespace Class_0044d720 { void FillWorldPos(); }
void FUN_0044d7c0();
namespace Class_0044d800 { void ContainsUnit(); }
namespace RingApproach { void ApproxDistExcess(); }
void FUN_0044d900();
namespace Class_0044d910 { void Destroy(); }
namespace Class_0044d930 { void Serialize(); }
namespace Class_0044da00 { void AppendGoalCell(); }
namespace PointMarker { void FillWorldPos(); }
void FUN_0044dcb0();
void FUN_0044dd00();
namespace PathOrder { void SerializeToBits(); }
void FUN_0044df70();
namespace Class_0044df80 { void Destroy(); }
namespace Class_0044dfb0 { void SerializeToSave(); }
void FUN_0044e3a0();
namespace Class_0044e3c0 { void FillWorldPos(); }
namespace Class_0044e530 { void GetDesiredHeading(); }
namespace Class_0044e5b0 { void IsComplete(); }
void FUN_0044e6b0();
void FUN_0044e7a0();
namespace Class_0044e7b0 { void Destroy(); }
namespace Class_0044e740 { void SerializeToSave(); }
namespace Class_0044e740 { void SerializeToBits(); }
void FUN_0044ea50();
namespace AirManeuverOrder { void FillWorldPos(); }
namespace Class_0044eb40 { void GetDesiredHeading(); }
namespace Class_0044e740 { void IsComplete(); }
void FUN_0044ec00();
namespace PathGoal { void DrawOnSurface(); }
namespace PathGoal { void SetPathOrder(); }
namespace PathGoal { void TickTowardGoal(); }
namespace PathGoal { void SerializeNetUnitState(); }
namespace PathGoal { void HasNetUnitState(); }
namespace PathGoal { void TryClaimRepath(); }
namespace PathGoal { void ExportGoalPose(); }
void FUN_0044f590();
void FUN_0044f5b0();
void FUN_0044f5c0();
void FUN_0044f650();
void OnAimCobReturn();
void AimCobStub();

// SpatialTimer's, a class of the 0x4fc980 family: stored by its constructor 0x407d40.
// GLOBAL: 0x4fc9a0
extern VirtualFunction const g_spatialTimerVtable[2] = {
    SpatialTimer::OnTimer, FUN_00407e70,
};

// The base class of the 0x44ce20 family: stored by the constructors 0x44ce20 to 0x44e330 before their own, and by the destructors 0x44ce50 to 0x44e7b0 last.
// GLOBAL: 0x4fd2f8
extern VirtualFunction const DAT_004fd2f8[12] = {
    OrderFx::Destroy, SerializeSave, GetType, IsFxStyle,
    Class_0044cf00::ContainsUnit, ContainsCell, Class_0044ce90::FillGoalCells, ApproxDist,
    (VirtualFunction)_purecall, TryGetDesiredHeading, FUN_0044cf50, KeepAfterComplete,
};

// Stored by 0x44cf60 and 0x44d010.
// GLOBAL: 0x4fd328
extern VirtualFunction const g_approachRadiusVtable[12] = {
    Class_0044cff0::Destroy, Class_0044d010::Serialize, FUN_0044cfe0, IsFxStyle,
    Class_0044d310::ContainsUnit, FUN_0044d290, ApproachRadius::AppendGoalCell, Class_0044d350::ApproxDistExcess,
    Class_0044d2c0::FillWorldPos, TryGetDesiredHeading, FUN_0044cf50, KeepAfterComplete,
};

// Stored by 0x44d3b0 and 0x44d470.
// GLOBAL: 0x4fd358
extern VirtualFunction const g_ringApproachVtable[12] = {
    Class_0044d450::Destroy, Class_0044d470::Serialize, FUN_0044d440, IsFxStyle,
    Class_0044d800::ContainsUnit, FUN_0044d7c0, Class_0044d560::AppendGoalCell, RingApproach::ApproxDistExcess,
    Class_0044d720::FillWorldPos, TryGetDesiredHeading, FUN_0044cf50, KeepAfterComplete,
};

// Stored by 0x44d8a0 and 0x44d930.
// GLOBAL: 0x4fd388
extern VirtualFunction const g_pointMarkerVtable[12] = {
    Class_0044d910::Destroy, Class_0044d930::Serialize, FUN_0044d900, IsFxStyle,
    Class_0044cf00::ContainsUnit, FUN_0044dcb0, Class_0044da00::AppendGoalCell, FUN_0044dd00,
    PointMarker::FillWorldPos, TryGetDesiredHeading, FUN_0044cf50, KeepAfterComplete,
};

// Stored by 0x44de80, 0x44e080, 0x44e190, 0x44e250, 0x44e2d0 and 0x44e330.
// GLOBAL: 0x4fd3b8
extern VirtualFunction const g_pathOrderVtable[12] = {
    Class_0044df80::Destroy, Class_0044dfb0::SerializeToSave, FUN_0044df70, FUN_0044e6b0,
    Class_0044e5b0::IsComplete, ContainsCell, Class_0044ce90::FillGoalCells, ApproxDist,
    Class_0044e3c0::FillWorldPos, Class_0044e530::GetDesiredHeading, PathOrder::SerializeToBits, FUN_0044e3a0,
};

// Stored by 0x44e740, 0x44e7d0 and 0x44e9c0.
// GLOBAL: 0x4fd3f8
extern VirtualFunction const g_airManeuverOrderVtable[12] = {
    Class_0044e7b0::Destroy, Class_0044e740::SerializeToSave, FUN_0044e7a0, FUN_0044ec00,
    Class_0044e740::IsComplete, ContainsCell, Class_0044ce90::FillGoalCells, ApproxDist,
    AirManeuverOrder::FillWorldPos, Class_0044eb40::GetDesiredHeading, Class_0044e740::SerializeToBits, FUN_0044ea50,
};

// PatrolGoal's: stored by 0x44f570.
// GLOBAL: 0x4fd488
extern VirtualFunction const g_patrolGoalVtable[12] = {
    FUN_0044f590, PathGoal::SetPathOrder, PathGoal::TickTowardGoal, FUN_0044f650,
    PathGoal::ExportGoalPose, FUN_0044f5b0, PathGoal::TryClaimRepath, PathGoal::HasNetUnitState,
    PathGoal::SerializeNetUnitState, FUN_0044f5c0, PathGoal::DrawOnSurface, 0,
};

// Stored by 0x485e90, 0x485f50 and 0x4861d0.
// GLOBAL: 0x4fd6f0
extern VirtualFunction const g_weaponAimCobVtable[2] = {
    OnAimCobReturn, AimCobStub,
};
