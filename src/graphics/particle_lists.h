// ParticleLists: the ten per-index particle lists owned by g_game (see
// 0x471d90), built by 0x471d90 and walked by 0x471eb0, 0x471f40 and 0x471f90.
// The one declaration of the class, for particles.cpp and 0x472630; the inlined
// Add keeps std::vector::insert out of line the way the original calls it. The
// files 0x471160, 0x471820 and 0x471a50 keep their own view: they need the
// std::vector<Elem_00473500> element type, whose inlined insert cannot agree
// with this one.
#ifndef PARTICLE_LISTS_H
#define PARTICLE_LISTS_H

#include <vector>

class ParticleSystem;

class ParticleLists {
public:
    std::vector<ParticleSystem*> lists[10];             // 0x10 bytes each

    void Add(short index, ParticleSystem* p)
    {
        if (lists[index].size() > 400) {
            delete lists[index][0];
            lists[index].erase(lists[index].begin());
        }
        lists[index].push_back(p);
    }

    ParticleLists();
    ~ParticleLists();
    void UpdateAll();
    void DrawAll(void* param);
    void DrawList(void* param, short index);
    void AddTeleportParticles(int param_1, int param_2, int param_3, short index);
    void AddNanoParticles(int param_1, int param_2, int param_3, short index);
    void AddThrustParticles(int param_1, int param_2, int param_3, int param_4, short index);
    void AddWakeParticles(int param_1, int param_2, int param_3, int param_4,
                          short index, int param_6);
};

#endif
