// Decompiled by Sonnet. Names are provisional.

class Obj {}; // opaque, non-virtual single-inheritance class
typedef void (Obj::*ThisFn)();

// FUNCTION: 0x401000
void __stdcall FUN_00401000(Obj* base, int stride, int count, ThisFn func)
{
    if (--count < 0) return;
    int i = count + 1;
    do {
        (base->*func)();
        base = (Obj*)((char*)base + stride);
        i--;
    } while (i);
}
