// Decompiled by Opus. Names are provisional.
// A global std::vector: the compiler generates its initialiser and the
// destructor it registers with atexit.
#include <vector>


// FUNCTION: 0x438450 _$E5
// The destructor registered with atexit is at 0x438480; it does not match yet
// because the element type is unknown.
std::vector<int> DAT_00512340;
