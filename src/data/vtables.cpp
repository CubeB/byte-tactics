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
namespace OrderFx { void GetType(); }
namespace OrderFx { void Destroy(); }
void SerializeSave();
namespace OrderFx { void FillGoalCells(); }
namespace OrderFx { void ApproxDist(); }
namespace OrderFx { void KeepAfterComplete(); }
namespace Class_0044cf00 { void ContainsUnit(); }
namespace OrderFx { void ContainsCell(); }
namespace OrderFx { void IsFxStyle(); }
void TryGetDesiredHeading();
namespace OrderFx { void WriteBits(); }
namespace ApproachRadius { void GetType(); }
namespace ApproachRadius { void Destroy(); }
namespace ApproachRadius { void Serialize(); }
namespace ApproachRadius { void AppendGoalCell(); }
namespace ApproachRadius { void ContainsCell(); }
namespace ApproachRadius { void FillWorldPos(); }
namespace ApproachRadius { void ContainsUnit(); }
namespace ApproachRadius { void ApproxDistExcess(); }
namespace RingApproach { void GetType(); }
namespace RingApproach { void Destroy(); }
namespace RingApproach { void Serialize(); }
namespace RingApproach { void AppendGoalCell(); }
namespace RingApproach { void ContainsCell(); }
namespace RingApproach { void FillWorldPos(); }
namespace RingApproach { void ContainsUnit(); }
namespace RingApproach { void ApproxDistExcess(); }
namespace PointMarker { void GetType(); }
namespace PointMarker { void Destroy(); }
namespace PointMarker { void Serialize(); }
namespace PointMarker { void AppendGoalCell(); }
namespace PointMarker { void FillWorldPos(); }
namespace PointMarker { void ContainsCell(); }
namespace PointMarker { void ApproxDist(); }
namespace PathOrder { void SerializeToBits(); }
namespace PathOrder { void GetType(); }
namespace PathOrder { void Destroy(); }
namespace PathOrder { void SerializeToSave(); }
namespace PathOrder { void KeepAfterComplete(); }
namespace PathOrder { void FillWorldPos(); }
namespace PathOrder { void GetDesiredHeading(); }
namespace Class_0044e5b0 { void IsComplete(); }
namespace PathOrder { void IsFxStyle(); }
namespace AirManeuverOrder { void GetType(); }
namespace AirManeuverOrder { void Destroy(); }
namespace AirManeuverOrder { void SerializeToSave(); }
namespace AirManeuverOrder { void SerializeToBits(); }
namespace AirManeuverOrder { void KeepAfterComplete(); }
namespace AirManeuverOrder { void FillWorldPos(); }
namespace AirManeuverOrder { void GetDesiredHeading(); }
namespace AirManeuverOrder { void IsComplete(); }
namespace AirManeuverOrder { void IsFxStyle(); }
namespace PathGoal { void DrawOnSurface(); }
namespace PathGoal { void SetPathOrder(); }
namespace PathGoal { void TickTowardGoal(); }
namespace PathGoal { void SerializeNetUnitState(); }
namespace PathGoal { void HasNetUnitState(); }
namespace PathGoal { void TryClaimRepath(); }
namespace PathGoal { void ExportGoalPose(); }
void FUN_0044f590();
namespace PatrolGoal { void HasReadyWaypoints(); }
namespace PatrolGoal { void DeserializeNetUnitState(); }
namespace PatrolGoal { void FillWaypointWorldPos(); }
void OnAimCobReturn();
void AimCobStub();

// SpatialTimer's, a class of the 0x4fc980 family: stored by its constructor 0x407d40.
// GLOBAL: 0x4fc9a0
extern VirtualFunction const g_spatialTimerVtable[2] = {
    SpatialTimer::OnTimer, FUN_00407e70,
};

// The base class of the 0x44ce20 family: stored by the constructors 0x44ce20 to 0x44e330 before their own, and by the destructors 0x44ce50 to 0x44e7b0 last.
// GLOBAL: 0x4fd2f8
extern VirtualFunction const g_orderFxVtable[12] = {
    OrderFx::Destroy, SerializeSave, OrderFx::GetType, OrderFx::IsFxStyle,
    Class_0044cf00::ContainsUnit, OrderFx::ContainsCell, OrderFx::FillGoalCells, OrderFx::ApproxDist,
    (VirtualFunction)_purecall, TryGetDesiredHeading, OrderFx::WriteBits, OrderFx::KeepAfterComplete,
};

// Stored by 0x44cf60 and 0x44d010.
// GLOBAL: 0x4fd328
extern VirtualFunction const g_approachRadiusVtable[12] = {
    ApproachRadius::Destroy, ApproachRadius::Serialize, ApproachRadius::GetType, OrderFx::IsFxStyle,
    ApproachRadius::ContainsUnit, ApproachRadius::ContainsCell, ApproachRadius::AppendGoalCell, ApproachRadius::ApproxDistExcess,
    ApproachRadius::FillWorldPos, TryGetDesiredHeading, OrderFx::WriteBits, OrderFx::KeepAfterComplete,
};

// Stored by 0x44d3b0 and 0x44d470.
// GLOBAL: 0x4fd358
extern VirtualFunction const g_ringApproachVtable[12] = {
    RingApproach::Destroy, RingApproach::Serialize, RingApproach::GetType, OrderFx::IsFxStyle,
    RingApproach::ContainsUnit, RingApproach::ContainsCell, RingApproach::AppendGoalCell, RingApproach::ApproxDistExcess,
    RingApproach::FillWorldPos, TryGetDesiredHeading, OrderFx::WriteBits, OrderFx::KeepAfterComplete,
};

// Stored by 0x44d8a0 and 0x44d930.
// GLOBAL: 0x4fd388
extern VirtualFunction const g_pointMarkerVtable[12] = {
    PointMarker::Destroy, PointMarker::Serialize, PointMarker::GetType, OrderFx::IsFxStyle,
    Class_0044cf00::ContainsUnit, PointMarker::ContainsCell, PointMarker::AppendGoalCell, PointMarker::ApproxDist,
    PointMarker::FillWorldPos, TryGetDesiredHeading, OrderFx::WriteBits, OrderFx::KeepAfterComplete,
};

// Stored by 0x44de80, 0x44e080, 0x44e190, 0x44e250, 0x44e2d0 and 0x44e330.
// GLOBAL: 0x4fd3b8
extern VirtualFunction const g_pathOrderVtable[12] = {
    PathOrder::Destroy, PathOrder::SerializeToSave, PathOrder::GetType, PathOrder::IsFxStyle,
    Class_0044e5b0::IsComplete, OrderFx::ContainsCell, OrderFx::FillGoalCells, OrderFx::ApproxDist,
    PathOrder::FillWorldPos, PathOrder::GetDesiredHeading, PathOrder::SerializeToBits, PathOrder::KeepAfterComplete,
};

// Stored by 0x44e740, 0x44e7d0 and 0x44e9c0.
// GLOBAL: 0x4fd3f8
extern VirtualFunction const g_airManeuverOrderVtable[12] = {
    AirManeuverOrder::Destroy, AirManeuverOrder::SerializeToSave, AirManeuverOrder::GetType, AirManeuverOrder::IsFxStyle,
    AirManeuverOrder::IsComplete, OrderFx::ContainsCell, OrderFx::FillGoalCells, OrderFx::ApproxDist,
    AirManeuverOrder::FillWorldPos, AirManeuverOrder::GetDesiredHeading, AirManeuverOrder::SerializeToBits, AirManeuverOrder::KeepAfterComplete,
};

// PatrolGoal's: stored by 0x44f570.
// GLOBAL: 0x4fd488
extern VirtualFunction const g_patrolGoalVtable[12] = {
    FUN_0044f590, PathGoal::SetPathOrder, PathGoal::TickTowardGoal, PatrolGoal::FillWaypointWorldPos,
    PathGoal::ExportGoalPose, PatrolGoal::HasReadyWaypoints, PathGoal::TryClaimRepath, PathGoal::HasNetUnitState,
    PathGoal::SerializeNetUnitState, PatrolGoal::DeserializeNetUnitState, PathGoal::DrawOnSurface, 0,
};

// Stored by 0x485e90, 0x485f50 and 0x4861d0.
// GLOBAL: 0x4fd6f0
extern VirtualFunction const g_weaponAimCobVtable[2] = {
    OnAimCobReturn, AimCobStub,
};
