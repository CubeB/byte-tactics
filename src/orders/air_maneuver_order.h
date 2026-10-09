// AirManeuverOrder: the object a VTOL order carries to fly a manoeuvre between
// two points (0x2c bytes, vtable 0x4fd3f8; Thaldren's AirManeuverOrder). The
// one declaration for the files that build it (order_queue_4384a0.cpp from a
// saved game, victory.cpp from the network, the vtol_orders files from an
// order) and set its altitude. order_targets.cpp, which defines the methods,
// keeps its own view because its OrderFx stores the vtable by hand; the
// methods it adds (GetType, FillWorldPos, IsComplete, ...) are the slots
// OrderFx already declares.
#ifndef AIR_MANEUVER_ORDER_H
#define AIR_MANEUVER_ORDER_H

// Vec3 must be complete here: the including file includes util/vec3.h or
// declares its own view first.
#include "order_fx.h"

class BitReader;
struct Owner_0044e9c0;

#pragma pack(push, 2)
class AirManeuverOrder : public OrderFx {
public:
    union {
        unsigned short flags;          // +0x8, bit 0: heading set
        struct {
            unsigned short headingSet : 1;
            unsigned short unknown_rest : 15;
        };
    };
    Vec3 target;                       // +0xa
    Vec3 other;                        // +0x16
    short saveOnly22;                  // +0x22
    unsigned short heading;            // +0x24
    unsigned short value_26;           // +0x26
    Unit* self;                        // +0x28

    AirManeuverOrder(Order* order, const Vec3& a, const Vec3& b);
    AirManeuverOrder(int owner, HapiBank* file, char* name);
    AirManeuverOrder(Owner_0044e9c0* owner, BitReader* reader);
    void* Destroy(int param_1);
    int SerializeToSave(Order* order, HapiBank* file, char* name);
    int GetDesiredHeading(unsigned short* out);
    void SetAltitude(int altitude);
};
#pragma pack(pop)

#endif
