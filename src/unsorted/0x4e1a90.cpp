// Decompiled by Opus. Names are provisional.
// Lazily creates the global Class_004e17c0 object, allocated with
// FUN_004e1a80 (a GlobalAlloc wrapper), and returns it. It is a placement
// new into that block: the constructor is 0x4e17c0.

inline void* operator new(unsigned int, void* p) { return p; }

class Class_004e17c0 {
public:
    char unknown_0[0x14];
    Class_004e17c0();
};

void* FUN_004e1a80(unsigned int size);

extern Class_004e17c0* DAT_00529e7c;

// FUNCTION: 0x4e1a90
Class_004e17c0* FUN_004e1a90()
{
    if (DAT_00529e7c == 0)
        DAT_00529e7c = new (FUN_004e1a80(sizeof(Class_004e17c0))) Class_004e17c0;
    return DAT_00529e7c;
}
