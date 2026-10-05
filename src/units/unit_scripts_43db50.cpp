// Decompiled by DeepSeek V4.1 Flash. Names are provisional.

class CobScript {
public:
    int StartScriptWithArgs(char* name, void* param_2, int param_3, int param_4, int param_5, int param_6, int param_7, int param_8);
};

#pragma pack(push, 1)
struct UnitDef_0043db50 {
    char unknown_0[0x170];
    short field_170;                   // +0x170
    char unknown_172[0x22c - 0x172];
    unsigned char draft;               // +0x22c
};

struct Unit {
    char unknown_0[0x70];
    short field_70;                    // +0x70
    char unknown_72[0x92 - 0x72];
    UnitDef_0043db50* type;            // +0x92
    char unknown_96[0x9a - 0x96];
    CobScript* script;                 // +0x9a
    char unknown_9e[0x10a - 0x9e];
    int state;                         // +0x10a
    char unknown_10e[0x110 - 0x10e];
    unsigned int flags;                // +0x110
};

struct Game {
    char unknown_0[0x1427f];
    unsigned char seaLevel;            // +0x1427f
};
#pragma pack(pop)

extern Game* g_game;

class Class_0043db50 {
public:
    void UpdateSfxOccupy(Unit* unit);
};

// FUNCTION: 0x43db50
void Class_0043db50::UpdateSfxOccupy(Unit* unit)
{
    int seaLevel = g_game->seaLevel;
    int y = unit->field_70;
    int newState = unit->state;
    if ((unit->flags & 3) == 1 || (unit->flags & 3) == 2) {
        if (y > seaLevel) {
            newState = 4;
        } else {
            if (y - seaLevel > -5)
                newState = 1;
            if (unit->type->draft + y == seaLevel)
                newState = 2;
            if (unit->type->field_170 + y < seaLevel)
                newState = 3;
        }
    } else {
        newState = 0;
    }
    if (unit->state != newState) {
        unit->script->StartScriptWithArgs("setSFXoccupy", 0, 1, 1, newState, 0, 0, 0);
        unit->state = newState;
    }
}
