// Decompiled by GPT-6-Luna. Names are provisional.

#include <string.h>

extern char *g_game;
extern char DAT_005084ac[];
extern char DAT_0050849c[];
extern char DAT_00502a78[];
extern char DAT_00504ea8[];
extern char DAT_00504ea0[];
extern char DAT_00504e98[];

void BlankScreen();
void HandleSkirmishClick();
void *__stdcall LoadGuiLayer(void *, char *, int);
void __stdcall LoadPictureCached(char *, int, int, int);
int __stdcall FindGadgetOrNull(int, char *);
void __stdcall SetGadgetStatusByName(void *, char *, int);
void __stdcall FUN_0049fa90(void *);
void RefreshSkirmishSetup();
void __stdcall FUN_0049fb10(void *, int);
void __stdcall RenderLayer(void *, int);
void __stdcall FUN_00491c80(int);
void __stdcall HandleSkirmishCheatText(void *);

class Mission {
public:
    int LoadMissionByName(char *);
    void RefreshMapList(int);
    char *FUN_00435c30();
};
// FUNCTION: 0x47bbb0
void OpenSkirmishMenu(void)
{
    int difficulty;
    void *dialog;

    BlankScreen();
    dialog = LoadGuiLayer(g_game + 0x519, DAT_005084ac, 0);
    ((int *)dialog)[2] = (int)HandleSkirmishClick;
    ((int *)dialog)[3] = (int)g_game;
    LoadPictureCached(DAT_0050849c, 0, 0, 0);

    *(int *)(g_game + 0x37eee) = *(int *)(*(char **)(g_game + 0x29a0) + 0x228);
    difficulty = FindGadgetOrNull(*(int *)(*(char **)(g_game + 0x531) + 4), DAT_00502a78);
    if (*(int *)(g_game + 0x37eee) == 0) {
        *(char *)(difficulty + 0x137) = 0;
        SetGadgetStatusByName(g_game + 0x519, DAT_00504ea8, 1);
    }
    if (*(int *)(g_game + 0x37eee) == 1) {
        *(char *)(difficulty + 0x137) = 1;
        SetGadgetStatusByName(g_game + 0x519, DAT_00504ea0, 1);
    }
    if (*(int *)(g_game + 0x37eee) == 2) {
        *(char *)(difficulty + 0x137) = 2;
        SetGadgetStatusByName(g_game + 0x519, DAT_00504e98, 1);
    }
    FUN_0049fa90(g_game + 0x519);

    if (!((Mission *)*(int *)(g_game + 0x391e9))->LoadMissionByName(*(char **)(g_game + 0x29a0) + 0x11c)) {
        ((Mission *)*(int *)(g_game + 0x391e9))->RefreshMapList(0);
        strncpy(*(char **)(g_game + 0x29a0) + 0x11c,
                ((Mission *)*(int *)(g_game + 0x391e9))->FUN_00435c30(), 0x100);
    }

    RefreshSkirmishSetup();
    *(int *)(*(char **)(g_game + 0x531) + 0x3b) = (int)HandleSkirmishCheatText;
    FUN_0049fb10(g_game + 0x519, 1);
    RenderLayer(g_game + 0x519, 0x40);
    FUN_00491c80(0x13);
}
