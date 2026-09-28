// Decompiled by space-bunny-free. Names are provisional.
// Per-tick update of the smoke layer: a hundred live smoke puffs walking the
// 640x480 cell map, one cell per tick. A puff that is not running is dropped
// at a random cell that still holds a burnt feature (low nibble > 12); from
// then on it steps one cell per tick, is killed by leaving the map, by running
// out of life, or by meeting a cell that is not burnt, and on every hit it
// paints 0xaa and burns one unit of fuel. When the fuel runs out it turns:
// a puff drifting vertically starts drifting horizontally, and the other way
// round, three cells a tick in the direction its (parity) cell suggests, and
// refills its fuel.
// Three source details decide the code, all found by reading the listing:
//   - the heading is a conditional expression with char-typed arms
//     (`(s->x & 1) ? (char)-3 : (char)3`), which is what keeps the whole
//     computation in 8-bit registers; with int arms MSVC uses edx and eax.
//   - the same heading written as an if/else in the other turn branch, which
//     is what stops MSVC from hoisting the `s->dx = 0` store above it.
//   - the map cell test on the spawning side is `> 12`, not `<= 12` with a
//     continue, because every path has to reach the `s++` at the loop bottom.
// Suspected original bug: a running puff writes the smoke template byte over
// the feature nibble of the cell it stands on (`dest[s->pos] = src[s->pos]`,
// and 0xaa, whose low nibble is 10) instead of only setting a smoke bit, so
// burnt features under a puff stop looking burnt and later puffs die there.
#include <stdlib.h>

// 13-byte smoke puff record: the loop steps the pointer by 13.
#pragma pack(push, 1)
struct Smoke_00425b80 {
    short x;                          // +0x0
    short y;                          // +0x2
    unsigned char active;             // +0x4
    char dx;                          // +0x5
    char dy;                          // +0x6
    unsigned char life;               // +0x7
    unsigned char fuel;               // +0x8
    int pos;                          // +0x9
};
#pragma pack(pop)

// The cell map, a byte per cell whose low nibble is the feature there.
struct Map_00425b80 {
    char unknown_0[0xc];
    unsigned char* cells;             // +0xc
};

struct World_00425b80 {
    char unknown_0[0xbc];
    Map_00425b80* map;                // +0xbc
};

// The smoke map is its own object, the map the smoke is drawn into is reached
// through the world at +0x4.
struct Terrain_00425b80 {
    char unknown_0[4];
    World_00425b80* world;            // +0x4
    char unknown_8[0x24 - 8];
    Map_00425b80* smoke;              // +0x24
};

#pragma pack(push, 1)
struct Game_00425b80 {
    char unknown_0[0x519];
    void* menu;                       // +0x519
    char unknown_51d[0x531 - 0x51d];
    Terrain_00425b80* terrain;        // +0x531
};
#pragma pack(pop)

extern Game_00425b80* g_game;
extern Smoke_00425b80* DAT_00512298;

void __stdcall FUN_0049fad0(void* menu);

// FUNCTION: 0x425b80
void FUN_00425b80()
{
    Terrain_00425b80* terrain = g_game->terrain;
    if (terrain->smoke != 0) {
        unsigned char* src = terrain->smoke->cells;
        Smoke_00425b80* s = DAT_00512298;
        unsigned char* dest = terrain->world->map->cells;
        int count = 100;
        do {
            if (s->active) {
                dest[s->pos] = src[s->pos];
                s->y += s->dy;
                s->x += s->dx;
                if (s->x < 0 || s->x >= 640 || s->y >= 480 || s->y < 0 || s->life == 0) {
                    s->active = 0;
                } else {
                    s->life--;
                    s->pos = s->pos + s->dx;
                    if (s->dy != 0) {
                        s->pos = s->pos + s->dy * 640;
                    }
                    if ((dest[s->pos] & 0xf) < 13) {
                        s->active = 0;
                    } else {
                        dest[s->pos] = 0xaa;
                        if (s->fuel == 0) {
                            if (s->dy != 0) {
                                s->dy = 0;
                                s->dx = (s->x & 1) ? (char)-3 : (char)3;
                                s->fuel = (rand() & 0xf) + 1;
                            } else {
                                if (s->y & 1) {
                                    s->dy = 3;
                                } else {
                                    s->dy = -3;
                                }
                                s->dx = 0;
                                s->fuel = (rand() & 0xf) + 1;
                            }
                        } else {
                            s->fuel--;
                        }
                    }
                }
            } else {
                s->x = rand() % 640;
                s->y = rand() % 220;
                s->pos = s->x + s->y * 640;
                if ((dest[s->pos] & 0xf) > 12) {
                    s->active = 1;
                    s->life = rand() + 1;
                    s->fuel = (rand() & 0x1f) + 1;
                    if (s->life & 1) {
                        if (s->y & 1) {
                            s->dy = -3;
                            s->dx = 0;
                        } else {
                            s->dy = 3;
                            s->dx = 0;
                        }
                    } else {
                        if (s->x & 1) {
                            s->dx = -3;
                        } else {
                            s->dx = 3;
                        }
                        s->dy = 0;
                    }
                }
            }
            s++;
        } while (--count);
        FUN_0049fad0((char*)g_game + 0x519);
    }
}
