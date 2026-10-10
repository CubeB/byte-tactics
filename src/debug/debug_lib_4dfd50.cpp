// Decompiled by deepseek-v4.1-flash. Names are provisional.
// The compiler-generated atexit term function for the function-local static
// object at 0x5292d0 (guard byte 0x5292c8, constructor 0x4df1e0, registered by
// the guard function 0x4dfd10).
//
// `_Freenode` is the pooled allocator's deallocate: it pushes the node onto
// g_nameMapFreeList.
#include <yvals.h>
#include "name_map_tree.h"

// The object at 0x5292d0 (0x4df1e0 builds it, 0x4dfd10 hands it out): only
// its map at +0x21c has anything to destroy.
class PerformanceDialog {
public:
    char unknown_0[0x21c];
    NameMapTree map;                 // +0x21c
    bool changed;                      // +0x22c
};

// FUNCTION: 0x4dfd50 _$E2
void trigger_004dfd50()
{
    static PerformanceDialog obj;
}
