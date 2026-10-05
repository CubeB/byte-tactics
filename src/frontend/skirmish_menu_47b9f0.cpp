// Decompiled by GPT-6-Luna. Names are provisional.
#include <string.h>

struct CheatText_0047b9f0 {
    char unknown_0[4];
    void* target;
};

struct Cheat_0047b9f0 {
    char unknown_0[0x18];
    CheatText_0047b9f0* text;
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x519];
    char unknown_519[0x29a0 - 0x519];
    void* field_29a0;
    char unknown_29a4[0x38d81 - 0x29a4];
    int cheat;
};
#pragma pack(pop)

extern Game* g_game;
extern const char DAT_00508498[];
extern const char DAT_00508494[];
extern const char DAT_00508490[];
extern const char DAT_00508488[];
extern const char DAT_00508480[];
extern const char DAT_00508478[];
extern const char DAT_00508474[];
extern const char DAT_00508470[];
extern const char DAT_00508460[];

void SaveSettings();
void SaveNumSkirmishPlayers();
void LoadSettings();
void FUN_0047a0e0();
void __stdcall PlaySoundByName(const char*, int);
void __stdcall FUN_004a7960(Cheat_0047b9f0*, int);
void __stdcall FUN_0049fa90(void*);

// FUNCTION: 0x47b9f0
void __stdcall FUN_0047b9f0(Cheat_0047b9f0* cheat)
{
    int zero = 0;
    int code = 0;
    if (strncmp((char*)cheat->text + 0x34, DAT_00508498, 3) == 0)
        code = 4;
    else if (strncmp((char*)cheat->text + 0x35, DAT_00508494, 2) == 0)
        code = 5;
    else if (strncmp((char*)cheat->text + 0x34, DAT_00508490, 3) == 0)
        code = 6;
    else if (strncmp((char*)cheat->text + 0x33, DAT_00508488, 4) == 0)
        code = 7;
    else if (strncmp((char*)cheat->text + 0x32, DAT_00508480, 5) == 0)
        code = 8;
    else if (strncmp((char*)cheat->text + 0x33, DAT_00508478, 4) == 0)
        code = 3;
    else if (strncmp((char*)cheat->text + 0x34, DAT_00508474, 3) == 0)
        code = 9;
    else if (strncmp((char*)cheat->text + 0x35, DAT_00508470, 2) == 0)
        code = 10;

    if (code != zero) {
        if (code == 3 || code == 4 || code == 8 || code == 9 || code == 10) {
            for (int i = 0; i < 0xf; i++)
                ((char*)cheat->text)[i + 0x28] = (char)zero;
        }
        SaveSettings();
        g_game->cheat = code;
        SaveNumSkirmishPlayers();
        LoadSettings();
        *(short*)((char*)cheat->text->target + 0xb6) =
            *(short*)((char*)g_game->field_29a0 + 0x220);
        FUN_0047a0e0();
        PlaySoundByName(DAT_00508460, 0);
        FUN_004a7960(cheat, 1);
        FUN_0049fa90((char*)g_game + 0x519);
    }
}
