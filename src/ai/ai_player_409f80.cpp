// Decompiled by deepseek-v4.1-flash. Names are provisional.
#include <string.h>

#pragma pack(push, 1)
struct Def_00409f80 {
    char unknown_0[0xbe];
    char name[0x241 - 0xbe];           // +0xbe
    unsigned int unknown_241 : 5;      // +0x241
    unsigned int field_5 : 1;          // +0x241 bit 5
    unsigned int rest : 26;            // +0x241
    char unknown_245[0x249 - 0x245];
};

struct Game {
    char unknown_0[0x1438f];
    int count;                         // +0x1438f
    char unknown_14393[8];
    Def_00409f80* defs;                // +0x1439b
};

class PlayerAI {
public:
    char unknown_0[0xc1];
    int* locked;                       // +0xc1
};
#pragma pack(pop)

class CommandArgs {
public:
    char unknown_0[0xd0];
    int field_d0;
    CommandArgs* InitArgs();
};

class Class_004b74f0 {
public:
    char* args[0x34];                  // +0x00
    int count;                         // +0xd0
};

extern Game* g_game;
extern PlayerAI* g_playerAI[];

void EnableAICommands();
int __stdcall ExecuteCommandText(char* text, int len, Class_004b74f0* vars, int param_4);

// FUNCTION: 0x409f80
void __stdcall FUN_00409f80(int player)
{
    PlayerAI* p = g_playerAI[player];
    EnableAICommands();
    for (unsigned short i = 1; i < g_game->count; i++) {
        Def_00409f80* def = &g_game->defs[i];
        if (def->field_5) {
            if (p->locked[i] != 1) {
                int len = strlen(def->name);
                if (len != 0) {
                    Class_004b74f0 vars;
                    ((CommandArgs*)&vars)->InitArgs();
                    ExecuteCommandText(def->name, len, &vars, -1);
                }
            }
        }
    }
}
