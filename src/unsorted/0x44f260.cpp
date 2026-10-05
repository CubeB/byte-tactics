// Decompiled by Sonnet. Names are provisional.
// Class_0044f010's override of slot 6 (vtable 0x4fd458, see 0x44f450.cpp):
// returns this object when its flag bit 1 is set and the counter at
// g_game+0x38a47 has reached field_60 + 0x3c (storing the counter in
// field_60), or null. The path search scheduler (0x40eb70) calls it through
// slot 6 to pick the path to search for next.

extern char* g_game;

class Class_0044f010 {
public:
    char unknown_4[0x60 - 0x4];
    unsigned int field_60;
    unsigned char field_64;

    virtual Class_0044f010* FUN_0044eff0();  // slot 6
};

// FUNCTION: 0x44f260
Class_0044f010* Class_0044f010::FUN_0044eff0()
{
    if (field_64 & 2) {
        unsigned int limit = *(unsigned int*)(g_game + 0x38a47);
        if (limit >= field_60 + 0x3c) {
            field_60 = limit;
            return this;
        }
    }
    return 0;
}
