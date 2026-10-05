// Decompiled by Haiku; renamed to the real template member by the orchestrator (#406). Names are provisional.
// std::vector<Point_0044eec0>::size() for the 4-byte point (two shorts); its
// callers are the vector insert paths in 0x44d0e0 and 0x44d560.
#include <vector>
struct Point_0044eec0 {
    short x;
    short y;
};
typedef std::vector<Point_0044eec0> Vec_0044ee70;
typedef Vec_0044ee70::size_type (Vec_0044ee70::*SizeFn_0044ee70)() const;
// FUNCTION: 0x44ee70 ?size@?$vector@UPoint_0044eec0@@V?$allocator@UPoint_0044eec0@@@std@@@std@@QBEIXZ
SizeFn_0044ee70 g_size_0044ee70 = &Vec_0044ee70::size;
