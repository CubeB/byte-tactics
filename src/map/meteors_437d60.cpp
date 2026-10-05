// Decompiled by Opus. Names are provisional.
#include <string.h>

struct Params_00437d60 {
    char name[0x20];                   // +0x00
    int field_20;                      // +0x20
    float rate;                        // +0x24
    float field_28;                    // +0x28
    float field_2c;                    // +0x2c
};

extern char DAT_005122f0[];
extern int DAT_00512310;
extern int DAT_00512314;
extern int DAT_00512324;
extern int DAT_00512338;

// FUNCTION: 0x437d60
void __stdcall SetMeteorParams(Params_00437d60* p)
{
    strcpy(DAT_005122f0, p->name);
    DAT_00512310 = p->field_20;
    DAT_00512314 = (int)(30.0f / p->rate);
    DAT_00512324 = (int)(p->field_28 * 30.0f);
    DAT_00512338 = (int)(p->field_2c * 30.0f);
}
