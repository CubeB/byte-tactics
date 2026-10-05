// Decompiled by Opus. Names are provisional.

#pragma pack(push, 1)
struct Struct_00496e90 {
    char unknown_0[0xdc];
    float width;                       // +0xdc
    float height;                      // +0xe0
    char unknown_e4[0x149 - 0xe4];
    unsigned short flag_149 : 1;       // +0x149
};
#pragma pack(pop)

static inline int AtLeast200(int v)
{
    if (v < 200)
        v = 200;
    return v;
}

// FUNCTION: 0x496e90
void __stdcall FUN_00496e90(Struct_00496e90* obj, int height, int width)
{
    obj->flag_149 = 1;
    obj->width = (float)AtLeast200(width);
    obj->height = (float)AtLeast200(height);
}
