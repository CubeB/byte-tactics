// Decompiled by Space Bunny Free. Names are provisional.
// Builds the "PropList" debug string for one unit type (the 0x240-byte entries
// at g_game+0x1439b): a newline separated list of four fields, then three
// speed readouts that are either a number with a unit label or the string N/A
// when the unit type's +0x22f says it has no speeds.
#include <windows.h>
#include <stdio.h>
#include <string.h>

#pragma pack(push, 1)
struct UnitType_00489280 {
    char unknown_0[0x186];
    float field_186;                   // +0x186
    float field_18a;                   // +0x18a
    char unknown_18e[0x192 - 0x18e];
    int field_192;                     // +0x192
    char unknown_196[0x19e - 0x196];
    int field_19e;                     // +0x19e
    char unknown_1a2[0x1ba - 0x1a2];
    unsigned short field_1ba;          // +0x1ba
    char unknown_1bc[0x1ea - 0x1bc];
    int field_1ea;                     // +0x1ea
    char unknown_1ee[0x22f - 0x1ee];
    unsigned char field_22f;           // +0x22f
    char unknown_230[0x240 - 0x230];
};
#pragma pack(pop)

void* __cdecl FUN_004d83b0(char* name, unsigned int size);
char* __stdcall Translate(const char* text);
int GetTickRate();

// A helper, not inline or a float local: the product is spilled before GetTickRate.
static inline float spd(int v)
{
    return v * 1.52587890625e-05f;
}

// FUNCTION: 0x489280
char* __stdcall MakePropList(UnitType_00489280* obj)
{
    char* buf = (char*)FUN_004d83b0("PropList", 0xc0);
    memset(buf, 0, 0xc0);
    char* p = buf;

    // Every append advances with `p += strlen(p) + 1`, not by wsprintfA's result.
    wsprintfA(p, "\n");
    p += strlen(p) + 1;

    wsprintfA(p, "%d", (int)obj->field_186);
    p += strlen(p) + 1;

    wsprintfA(p, "%d", (int)obj->field_18a);
    p += strlen(p) + 1;

    wsprintfA(p, "%d", obj->field_1ea);
    p += strlen(p) + 1;

    wsprintfA(p, "\n");
    p += strlen(p) + 1;

    if (obj->field_22f) {
        sprintf(p, "%.1f %s ", (double)GetTickRate() * spd(obj->field_192) * 0.4,
                Translate("m/s"));
        p += strlen(p) + 1;

        sprintf(p, "%.2f %s", (double)GetTickRate() * spd(obj->field_19e) * 0.4,
                Translate("m/s/s"));
        p += strlen(p) + 1;

        sprintf(p, "%.0f %s",
                (double)GetTickRate() * obj->field_1ba * 0.0054931640625,
                Translate("deg/s"));
    } else {
        sprintf(p, "%s", Translate("N/A"));
        p += strlen(p) + 1;

        sprintf(p, "%s", Translate("N/A"));
        p += strlen(p) + 1;

        sprintf(p, "%s", Translate("N/A"));
    }

    return buf;
}
