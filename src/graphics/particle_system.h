// ParticleSystem: the base of the particle systems (the 0x4fd5a8 family), the
// pool's operator new and delete, the three pure virtual slots and the lifetime
// field at +0x4. The one declaration of the class for the files that create or
// walk a particle system; particles.cpp defines the methods. It declares no
// other type, and includes <stddef.h> for size_t alone.
// particles_471820.cpp, particles_471a50.cpp and particles_472630.cpp keep
// their own view: they define operator new (and 471820 the constructor) inside
// the class, and /Ob2 inlines them there, where this header declares them out
// of line.
#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#include <stddef.h>

class ParticleSystem {
public:
    int deadline;                                       // +0x4

    ParticleSystem();
    virtual ~ParticleSystem();                          // slot 0
    virtual void Update() = 0;                          // slot 1
    virtual void Render(int) = 0;                       // slot 2
    virtual int IsFinished() = 0;                       // slot 3
    static void* __stdcall operator new(size_t size);   // 0x471d10
    static void __stdcall operator delete(void* p);     // 0x471d50
    void SetLifetime(int ticks);
};

#endif
