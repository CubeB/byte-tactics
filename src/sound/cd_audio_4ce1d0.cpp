// Decompiled by Opus. Names are provisional.
// A method of the object at g_game+0x10 (its one caller, 0x4263b0, loads ecx
// from there) that never uses `this`, like 0x4ce190.

extern int g_cdPlayerWindow;

class Class_004ce1d0 {
public:
    bool HasCdPlayerWindow();
};

// FUNCTION: 0x4ce1d0
bool Class_004ce1d0::HasCdPlayerWindow()
{
    return g_cdPlayerWindow != 0;
}
