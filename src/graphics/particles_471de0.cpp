// Decompiled by space-bunny-free, finished by DeepSeek V4.1 Flash, verified by GPT-6, finished by claude-opus-5-5, finished by GPT-6, finished by Claude Opus 5.5. Names are provisional.
// Destroys the ten listener lists that 0x471d90 allocates into the game object
// (used by 0x471eb0, 0x471f40 and 0x471f90): every listener is deleted and
// erased from the front of its list, the ten vector members are then destroyed
// (that is the second, backward loop: the implicit ~vector of the array, which
// frees _First and zeroes _First, _Last and _End), the list object itself goes
// back to the global operator new and g_game->lists_38d77 is cleared. The
// virtual call is a `delete` of a listener: vtable slot 0 is its scalar
// deleting destructor, called with flag 1 and only for a non-null pointer.
//
// The list object is the header's Class_00472200 (ten vectors at +0x0), whose
// destructor the header declares. The listener class is only declared in the
// header (its views disagree), so this file gives its view: one virtual
// destructor.
#include <windows.h>
// This header set fixes the file's symbol total, which decides one store's
// operand order.
#include <shlobj.h>
#include <imagehlp.h>
#include "ta_types.h"

class ParticleSystem {
public:
    virtual ~ParticleSystem();
};

inline Class_00472200::~Class_00472200()
{
    for (int i = 0; i < 10; i++) {
        std::vector<ParticleSystem*>::iterator it = lists[i].begin();
        while (it != lists[i].end()) {
            delete *it;
            lists[i].erase(it);
        }
    }
}

extern Game* g_game;

// FUNCTION: 0x471de0
void DestroyParticleLists()
{
    if (g_game->lists_38d77) {
        delete g_game->lists_38d77;
        g_game->lists_38d77 = 0;
    }
}
