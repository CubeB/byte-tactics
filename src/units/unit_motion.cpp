// Decompiled by DeepSeek V4.1 Flash, Opus and Haiku. Names are provisional.

class CobScript {
public:
    int StartScriptWithArgs(char* name, void* param_2, int param_3, int param_4,
                     int param_5, int param_6, int param_7, int param_8);
};

struct Cell_437840 {
    char unknown_0[7];
    unsigned char metal;               // +0x7
};

struct Point16_437840 {
    short x;
    short y;
};

#pragma pack(push, 1)
struct UnitType_437840 {
    char unknown_0[0x1ce];
    float extractsMetal;               // +0x1ce
    float field_1d2;                   // +0x1d2
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

struct Game {
    char unknown_0[0x37ed8];
    unsigned short windDirection;      // +0x37ed8
    int windSpeed;                     // +0x37eda
    char unknown_37ede[0x37ee2 - 0x37ede];
    int windEnabled;                   // +0x37ee2
};
#pragma pack(pop)

union Fixed_437840 {
    int value;
    struct {
        unsigned short fraction;
        short whole;
    } parts;
};

extern Game* g_game;

Cell_437840* __stdcall GetMapCell(int x, int y);

// Wind/metal picker for a metal extractor: when the unit type extracts metal
// (+0x1ce > 0), sums the metal byte of every map cell under the unit's
// footprint and stores the resulting rate at +0x58, then tells the script.
//
// The sum is accumulated in 16.16 fixed point: the unity's footprint is a
// Point16 at +0x7e, the cell is at +0x76, the type pointer at +0x92 and the
// script at +0x9a. The accumulator keeps the count in the high half of a
// dword so the final `(float)` conversion and the 2^-16 scale cancel out.
// FUNCTION: 0x437840
void __stdcall UpdateMetalExtraction(Unit* unit)
{
    if (unit->type->extractsMetal > 0.0f) {
        Fixed_437840 total;
        total.value = 0;
        Point16_437840 fp = unit->footprint;
        for (int y = unit->cell.y; y < unit->cell.y + fp.y; y++) {
            for (int x = unit->cell.x; x < unit->cell.x + fp.x; x++) {
                Cell_437840* c = GetMapCell(x, y);
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

// For a unit whose type has a positive value at +0x1d2 (presumably the wind
// generator rating), passes the current wind direction and speed to its
// script's SetDirection and SetSpeed functions when wind is enabled.
// FUNCTION: 0x437910
void __stdcall UpdateWindGenerator(Unit* unit)
{
    if (unit->type->field_1d2 > 0.0f && g_game->windEnabled) {
        unit->script->StartScriptWithArgs("SetDirection", 0, 0, 1, g_game->windDirection, 0, 0, 0);
        unit->script->StartScriptWithArgs("SetSpeed", 0, 0, 1, g_game->windSpeed << 4, 0, 0, 0);
    }
}

// FUNCTION: 0x437b30
int __stdcall GetHandleSize(int param_1)
{
    if (param_1 == 0) {
        return 0;
    }
    return *(int*)(param_1 - 4);
}
