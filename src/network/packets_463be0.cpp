// Decompiled by Opus. Names are provisional.
// Constructor of the per-player record (0x14b bytes, 11 of them inside the
// game object built by 0x41d920).
#include <string.h>

void __cdecl FUN_004d83a0(int);

#pragma pack(push, 1)
struct Grid_00463be0 {
    void* cells;                       // +0x0
    int width;                         // +0x4
    int height;                        // +0x8
    int field_c;                       // +0xc
    Grid_00463be0() { width = 0; height = 0; field_c = 0; cells = 0; }
};

class Class_00463be0 {
public:
    int field_0;                       // +0x0
    char unknown_4[0x27 - 0x4];
    char* data;                        // +0x27 (0xb9 bytes)
    char unknown_2b[0x73 - 0x2b];
    char field_73;                     // +0x73
    char unknown_74[0x7c - 0x74];
    Grid_00463be0 grid;                // +0x7c
    char unknown_8c[0x146 - 0x8c];
    char field_146;                    // +0x146
    char unknown_147[0x14b - 0x147];

    Class_00463be0();
};
#pragma pack(pop)

// FUNCTION: 0x463be0
Class_00463be0::Class_00463be0() : field_0(0), field_73(0)
{
    field_146 = 10;
    data = (char*)operator new(0xb9);
    FUN_004d83a0((int)data);
    memset(data, 0, 0xb9);
}
