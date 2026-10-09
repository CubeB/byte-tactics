// FeatureSpot: one slot of the feature animation pool (Thaldren's FeatureAnim,
// 0x30 bytes), the array at g_game+0x1420b that a cell's spot index names. A
// live slot is either a pair of GAF animation handles (a burning or dying
// feature) or the state, position and velocity of a moving feature, with the
// feature's orientation as the three angles at +0x20. The one declaration for
// the info panel and the files that read a slot; map/features.cpp, which owns
// the pool, keeps its own Spot view (its list order and loader spellings
// differ), and the orders files keep their own because their local Vec3 is a
// different type.
#ifndef FEATURE_SPOT_H
#define FEATURE_SPOT_H

#include "../graphics/handle.h"
#include "../util/angles.h"
#include "../util/vec3.h"

#pragma pack(push, 1)

struct FeatureSpot {
    short next;                        // +0x0
    short prev;                        // +0x2
    union {
        struct {
            Handle anim;               // +0x4
            Handle shadow;             // +0x10
        };
        struct {
            void* state;               // +0x4, the moving feature's model state
            Vec3 pos;                  // +0x8
            Vec3 vel;                  // +0x14
        };
    };
    Angles16 rot;                      // +0x20
    char unknown_26[0x2f - 0x26];
    unsigned char spotFlags;           // +0x2f
};

#pragma pack(pop)

#endif
