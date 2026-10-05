// Decompiled by Opus. Names are provisional.
// Returns `pad` if it is usable, otherwise the first usable pad the unit's
// script reports from QueryLandingPad, or -1.

class CobScript {
public:
    int QueryScript(char* name, int* param_2, int* param_3, int* param_4, int* param_5);
};

#pragma pack(push, 1)
struct Unit {
    char unknown_0[0x9a];
    CobScript* script;                 // +0x9a
};
#pragma pack(pop)

int __stdcall FUN_0047e570(Unit* unit, int id);

// FUNCTION: 0x411840
int __stdcall FindLandingPad(Unit* unit, int pad)
{
    if (pad != -1 && FUN_0047e570(unit, pad)) {
        return pad;
    }
    int pads[4];
    pads[0] = -1;
    pads[1] = -1;
    pads[2] = -1;
    pads[3] = -1;
    unit->script->QueryScript("QueryLandingPad", &pads[0], &pads[1], &pads[2], &pads[3]);
    for (int i = 0; i < 4; i++) {
        if (pads[i] != -1 && FUN_0047e570(unit, pads[i])) {
            return pads[i];
        }
    }
    return -1;
}
