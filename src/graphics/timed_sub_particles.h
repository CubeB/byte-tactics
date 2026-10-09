// TimedSubParticles: the smoke system at vtable 0x4fd638, the same shape as
// SmokeParticles without the fog culling. The one declaration of the class, for
// particles.cpp, 0x4750f0 and 0x475700. Its element record TimedSubParticle and
// the 16.16 position it holds come with it; 0x471a50 and 0x472630 keep their
// own view, whose element placeholder and local ParticleSystem cannot share
// this one.
#ifndef TIMED_SUB_PARTICLES_H
#define TIMED_SUB_PARTICLES_H

#include <vector>

#include "particle_system.h"

struct TimedSubParticle;

struct Vec3_00475150 {
    int x;
    int y;
    int z;
};

// Vtable 0x4fd638, constructor 0x4750b0, ??_G 0x475110; 0x34 bytes.
class TimedSubParticles : public ParticleSystem {
public:
    int time;                                           // +0x8, the next emit tick
    std::vector<TimedSubParticle> records;              // +0xc (_First +0x10)
    int emitPeriod;                                     // +0x1c, the emit period
    int holdPeriod;                                     // +0x20
    int maxFrame;                                       // +0x24, the frame count - 1
    Vec3_00475150 pos;                                  // +0x28

    TimedSubParticles();
    virtual void Update();                              // slot 1, 0x475600
    virtual void Render(int);                           // slot 2, 0x475700
    virtual int IsFinished();                           // slot 3, 0x475330
    virtual void Emit();                                // slot 4, 0x4751c0
    virtual int IsEmitDue();                            // slot 5, 0x4750f0
    virtual void Init(Vec3_00475150* pos, int a, int b, int c); // slot 6, 0x475150
};

#endif
