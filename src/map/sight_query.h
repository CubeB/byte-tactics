// SightQuery: one unit's sight query (Thaldren's LosSightQuery), the argument
// of the line-of-sight routines: the owning player, the unit's cached sight
// cell, its sight distance and eye height, the byte that holds its sight frame
// and its position. The one declaration of the struct for the files in
// map/line_of_sight*.cpp. map/line_of_sight.cpp, whose Vec3 splits y and z into
// halves, keeps its own view.
#ifndef SIGHT_QUERY_H
#define SIGHT_QUERY_H

#include "../util/vec3.h"

struct Player;

struct SightQuery {                    // 0x24 bytes
    Player* player;                    // +0x00
    short* cacheCell;                  // +0x04
    short sightDistance;               // +0x08
    unsigned char eyeHeight;           // +0x0a
    char unknown_b;                    // +0x0b
    unsigned char* frameIdx;           // +0x0c
    Vec3 pos;                          // +0x10
    int unknown_1c;                    // +0x1c
    int unknown_20;                    // +0x20
};

#endif
