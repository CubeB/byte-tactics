// Decompiled by Sonnet. Names are provisional.
// The object at 0x5292d0, built on first use. Its layout is 0x4df1e0's: the
// name map at +0x21c is what 0x4dfd50 destroys at exit.

class Class_004e17c0 {                 // the name map (see 0x4e17c0.cpp)
public:
    char unknown_0[0x14];
};

class Class_004df1e0 {
public:
    int unknown_0;                     // +0x0
    int unknown_4;                     // +0x4
    int unknown_8;                     // +0x8
    int unknown_c;                     // +0xc
    double time;                       // +0x10
    int count;                         // +0x18
    void* table;                       // +0x1c
    char flag_20;                      // +0x20
    const char* id;                    // +0x24
    char text[0x1f4];                  // +0x28
    Class_004e17c0 map;                // +0x21c

    Class_004df1e0();
    ~Class_004df1e0() {}
};

// FUNCTION: 0x4dfd10
Class_004df1e0* GetPerformanceWindow(void)
{
    static Class_004df1e0 obj;
    return &obj;
}
