// Decompiled by GPT-6, finished by deepseek-v4.1-flash, verified by GPT-6.1-sol, finished by space-bunny-free, edited by deepseek-v4.1, finished by deepseek-v4.1-flash. Names are provisional.
// The rest of the class is in ai_player_407e70.cpp.
#pragma pack(push, 1)
struct Game {
    char unknown_0[0x14223];
    int mapWidthWorld;                 // +0x14223
    int mapHeightWorld;                // +0x14227
};
#pragma pack(pop)

extern Game* g_game;

struct Vec3_00407d40 {
    int x, y, z;

    Vec3_00407d40() {}
    // The declaration of ax, the double and the assignment are three separate
    // statements on purpose: folding them into `int ax = (int)(game->mapWidthWorld / 2
    // * 65536.0);` moves the constructor's `pop edi` back between c.y and c.z
    // and the function stops matching.
    Vec3_00407d40(Game* game) {
        int ax;
        double halfX = ((double)(((game->mapWidthWorld / 2) * 65536.0)));
        ax = (int)halfX;
        *this = Vec3_00407d40(ax, 0, (int)((game->mapHeightWorld / 2) * 65536.0));
    }
    Vec3_00407d40(int ax, int ay, int az) : x(ax), y(ay), z(az) {}
};

struct SquadManager;  // the struct key stays in the mangled name of 0x407d40
#include "squad_manager.h"

// Vtable 0x4fc980, constructor 0x407350, ??_G 0x407390.
class SquadTimer {
public:
    SquadManager* owner;               // +0x4
    void* group;                       // +0x8
    int next;                          // +0xc
    unsigned int player;               // +0x10

    SquadTimer(SquadManager* p, void* q);
    virtual void OnTimer();                         // slot 0
    virtual ~SquadTimer() {}                        // slot 1
};

extern void* DAT_004fc980[];
extern void* g_spatialTimerVtable[];

class SpatialTimer {
public:
    // Plain field, not a virtual class: both vtable stores come from source.
    void* vptr_slot;                   // +0x0
    SquadManager* owner;               // +0x4
    void* group;                       // +0x8
    int next;                          // +0xc
    unsigned int player;               // +0x10
    Vec3_00407d40 best;                // +0x14, the best position found so far
    Vec3_00407d40 probe;               // +0x20, the position being rated
    Vec3_00407d40 step;                // +0x2c, added to probe each timer tick
    int bestRating;                    // +0x38, the unit rating at best

    SpatialTimer(SquadManager* p, void* q);
};

SquadTimer::SquadTimer(SquadManager* p, void* q)
    : owner(p), group(q), next(0), player(p->index) {}

// FUNCTION: 0x407d40
SpatialTimer::SpatialTimer(SquadManager* p, void* q)
{
    owner = p;
    group = q;
    next = 0;
    player = p->index;
    vptr_slot = DAT_004fc980;
    best = Vec3_00407d40(g_game);
    probe = Vec3_00407d40(g_game);
    Vec3_00407d40 temp(g_game);
    // Order matters: bestRating, then the late vtable store, then step.
    bestRating = 0;
    vptr_slot = g_spatialTimerVtable;
    step = temp;
}
