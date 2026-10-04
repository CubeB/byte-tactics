// Decompiled by space-bunny-free, finished by DeepSeek V4.1 Flash, verified by GPT-6, finished by claude-opus-5-5, finished by GPT-6, finished by Claude Opus 5.5. Names are provisional.
// Destroys the ten listener lists that 0x471d90 allocates into the game object
// (used by 0x471eb0, 0x471f40 and 0x471f90): every listener is deleted and
// erased from the front of its list, the ten vector members are then destroyed
// (that is the second, backward loop: the implicit ~vector of the array, which
// frees _First and zeroes _First, _Last and _End), the list object itself goes
// back to the global operator new and g_game->lists_38d77 is cleared. Both
// loops are the body of the destructor at 0x470fb0 (itself MATCH), reached
// here through the inlined `delete`. The virtual call is a `delete` of a
// listener: vtable slot 0 is its scalar deleting destructor, called with flag
// 1 and only for a non-null pointer.
//
// #5636 Claude Opus 5.5: MATCH with the game types header and the two system
// headers of the DLLs TA imports that it does not already include
// (SHELL32 and IMAGEHLP): include/ta_types.h, <shlobj.h> and <imagehlp.h>,
// the same set that matches 0x424c00.
// - The one byte left at 98.6% was the SIB order of the store in the inlined
//   erase's shift loop (the original `mov [edx + eax], ebp`, delta base). It
//   follows the file total, the front end's symbol count at the end of the
//   file (docs/c2-regalloc.md, "Symbol ids"): MATCH for totals 65257 to 65554,
//   where C2's own counter passes 65536 before this function. ta_types.h
//   alone gives 62839; with <shlobj.h> and <imagehlp.h> it is 65433. Other
//   real sets that match: <shlobj.h> with <math.h> (65359).
// - The list object is the header's Class_00472200 (ten vectors at +0x0),
//   whose destructor the header declares; it is defined inline here, as the
//   out-of-line copy at 0x470fb0 has it. The listener class is only declared
//   in the header (its views disagree), so this file gives its view: one
//   virtual destructor.
// Earlier findings (#349 to #5622): no spelling moves that byte; the
// out-of-line twin 0x470fb0 emits the walker-base form, and every source
// shape, flag and header set without the wrapped count kept the walker base.
#include <windows.h>
#include <shlobj.h>
#include <imagehlp.h>
#include "ta_types.h"

class Class_00471cc0 {
public:
    virtual ~Class_00471cc0();
};

inline Class_00472200::~Class_00472200()
{
    for (int i = 0; i < 10; i++) {
        std::vector<Class_00471cc0*>::iterator it = lists[i].begin();
        while (it != lists[i].end()) {
            delete *it;
            lists[i].erase(it);
        }
    }
}

extern Game* g_game;

// FUNCTION: 0x471de0
void FUN_00471de0()
{
    if (g_game->lists_38d77) {
        delete g_game->lists_38d77;
        g_game->lists_38d77 = 0;
    }
}
