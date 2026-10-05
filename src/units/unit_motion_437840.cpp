// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
// Wind/metal picker for a metal extractor: when the unit type extracts metal
// (+0x1ce > 0), sums the metal byte of every map cell under the unit's
// footprint and stores the resulting rate at +0x58, then tells the script.
//
// The sum is accumulated in 16.16 fixed point: the unity's footprint is a
// Point16 at +0x7e, the cell is at +0x76, the type pointer at +0x92 and the
// script at +0x9a. The accumulator keeps the count in the high half of a
// dword so the final `(float)` conversion and the 2^-16 scale cancel out.

struct Cell_437840 {
    char unknown_0[7];
    unsigned char metal;               // +0x7
};

struct Point16_437840 {
    short x;
    short y;
};

class CobScript {
public:
    int StartScriptWithArgs(char* name, void* param_2, int param_3, int param_4,
                     int param_5, int param_6, int param_7, int param_8);
};

#pragma pack(push, 1)
struct UnitType_437840 {
    char unknown_0[0x1ce];
    float extractsMetal;               // +0x1ce
};

struct Unit {
    char unknown_0[0x58];
    float extraction;                  // +0x58
    char unknown_5c[0x76 - 0x5c];
    Point16_437840 cell;               // +0x76
    char unknown_7a[0x7e - 0x7a];
    Point16_437840 footprint;          // +0x7e
    char unknown_82[0x92 - 0x82];
    UnitType_437840* type;             // +0x92
    char unknown_96[0x9a - 0x96];
    CobScript* script;                 // +0x9a
};
#pragma pack(pop)

union Fixed_437840 {
    int value;
    struct {
        unsigned short fraction;
        short whole;
    } parts;
};

Cell_437840* __stdcall FUN_00481550(int x, int y);

// FUNCTION: 0x437840
void __stdcall UpdateMetalExtraction(Unit* unit)
{
    if (unit->type->extractsMetal > 0.0f) {
        Fixed_437840 total;
        total.value = 0;
        Point16_437840 fp = unit->footprint;
        for (int y = unit->cell.y; y < unit->cell.y + fp.y; y++) {
            for (int x = unit->cell.x; x < unit->cell.x + fp.x; x++) {
                Cell_437840* c = FUN_00481550(x, y);
                if (c) {
                    total.parts.whole += c->metal + 1;
                }
            }
        }
        unit->extraction = unit->type->extractsMetal * 1.52587890625e-05 * (float)total.value;
        if (unit->script)
            unit->script->StartScriptWithArgs("SetSpeed", 0, 0, 1, total.parts.whole, 0, 0, 0);
    }
}
