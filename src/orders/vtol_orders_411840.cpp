// Decompiled by Opus. Names are provisional.
// Returns `pad` if it is usable, otherwise the first usable pad the unit's
// script reports from QueryLandingPad, or -1.

class Class_004b0bc0 {
public:
    int FUN_004b0bc0(char* name, int* param_2, int* param_3, int* param_4, int* param_5);
};

#pragma pack(push, 1)
struct Unit_00411840 {
    char unknown_0[0x9a];
    Class_004b0bc0* script;            // +0x9a
};
#pragma pack(pop)

int __stdcall FUN_0047e570(Unit_00411840* unit, int id);

// FUNCTION: 0x411840
int __stdcall FUN_00411840(Unit_00411840* unit, int pad)
{
    if (pad != -1 && FUN_0047e570(unit, pad)) {
        return pad;
    }
    int pads[4];
    pads[0] = -1;
    pads[1] = -1;
    pads[2] = -1;
    pads[3] = -1;
    unit->script->FUN_004b0bc0("QueryLandingPad", &pads[0], &pads[1], &pads[2], &pads[3]);
    for (int i = 0; i < 4; i++) {
        if (pads[i] != -1 && FUN_0047e570(unit, pads[i])) {
            return pads[i];
        }
    }
    return -1;
}
