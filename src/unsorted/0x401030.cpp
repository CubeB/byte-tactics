// Decompiled by Opus. Names are provisional.
// Calls a destructor-like member function on each of `count` objects of
// `stride` bytes, last to first (the counterpart of 0x401000).

class Obj {}; // opaque, non-virtual single-inheritance class
typedef void (Obj::*ThisFn)();

// FUNCTION: 0x401030
void __stdcall FUN_00401030(Obj* base, int stride, int count, ThisFn func)
{
    base = (Obj*)((char*)base + count * stride);
    while (--count >= 0) {
        base = (Obj*)((char*)base - stride);
        (base->*func)();
    }
}
