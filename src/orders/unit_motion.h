// UnitMotion: a unit's movement state, the 0x2f-byte object behind the
// `motion` pointer at +0x0 of a Unit. It holds the path object at +0x0, the
// movement class, the velocity and the previous position, the speed, the turn
// and the flags at +0x2e, and the methods that steer ground units and
// aircraft, update the position and save and load it. The one declaration of
// the class for the files that call it. The types behind the pointers stay
// private to their own files.
#ifndef UNIT_MOTION_H
#define UNIT_MOTION_H

#include "../util/vec3.h"

struct Unit;
class HapiBank;
class Iface_0043dd20;
class PlayerData;

// Unused here: these forward declarations take the symbol ids that the
// anonymous vector structs took, which keep the includers matching
// (docs/c2-regalloc.md).
class PathGoal;
class OrderFx;
class BlockMap;
class BlockMapIter;
class BlockInfo;
class FreeBlockMap;
class FreeBlockIter;
class FreeBlockAllocator;
class GroundAllyVisitor;
class DetectionVisitor;
class ClaimFootprintVisitor;
class SonarJamVisitor;
class RadarJamVisitor;
class NameMapTree;
class NameMapIter;
class NameMapAllocator;
class NameTable;
class NameKey;
class MapInsertResult;
class MapCacheEntry;
class UnitTypeSet;
class UnitTable;

#pragma pack(push, 1)

class UnitMotion {
public:
    union {
        Iface_0043dd20* obj;           // +0x0, the path the mover follows
        PlayerData* player;            // +0x0, the serialisation interface
    };
    int movementClass;                 // +0x4
    Vec3 velocity;                     // +0x8
    Vec3 p2;                           // +0x14
    union {
        int speed;                     // +0x20
        struct {
            short speedLow;            // +0x20
            short field_22;            // +0x22
        };
    };
    short turn;                        // +0x24, turn this tick
    int pathLockStamp;                 // +0x26
    int lastMoveTick;                  // +0x2a
    union {
        unsigned char flags;           // +0x2e
        struct {
            unsigned char mode : 2;    // bits 0-1
            unsigned char flag : 1;    // bit 2
            unsigned char rest : 5;
        };
    };

    void UpdateVelocityFromHeading(Unit* unit, int amount);
    void SteerGroundUnit(Unit* unit);
    void ApplyBankAndPitch(Unit* unit, Vec3* v);
    void SetFlightMode(Unit* unit, int state);
    void SteerAircraft(Unit* unit);
    void UpdatePosition(Unit* unit);
    void UpdateMoveRate(Unit* unit);
    UnitMotion(Unit* unit);
    void DestroyObject();
    void UpdateMotion(Unit* u);
    void ApplyClampedTurnDelta(Unit* unit, short amount);
    void SaveMotion(Unit* unit, HapiBank* file);
    void LoadMotion(Unit* unit, HapiBank* file);
};

#pragma pack(pop)

#endif
