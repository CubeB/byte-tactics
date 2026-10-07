// Decompiled by Sonnet. Names are provisional.
// std::vector<Unit*>::~vector() from MSVC 5's <vector>, out of line:
// _First is freed and the three pointers zeroed.
// 0x40b390 (which deletes the player AI object built by 0x409160) calls it
// on the unit lists at +0x05, +0x15 and +0x25, and 0x4103e0, 0x410850 and
// 0x4152f0 on a local that 0x40c510 (vector<Unit*>'s constructor) builds.
#include <vector>

struct Unit {
    int unknown_0;
};

typedef std::vector<Unit*> Inner_0040c530;
typedef std::vector<Inner_0040c530> Outer_0040c530;
typedef Outer_0040c530& (Outer_0040c530::*AssignFn_0040c530)(const Outer_0040c530&);

// Taking the address of the outer vector's operator= makes the compiler emit
// the inner destructor out of line.
AssignFn_0040c530 g_assign_0040c530 = &Outer_0040c530::operator=;

// FUNCTION: 0x40c530 ??1?$vector@PAUUnit@@V?$allocator@PAUUnit@@@std@@@std@@QAE@XZ
