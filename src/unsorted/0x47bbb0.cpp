// Decompiled by GPT-6-Luna. Names are provisional.

#include <string.h>

extern char *g_game;
extern char DAT_005084ac[];
extern char DAT_0050849c[];
extern char DAT_00502a78[];
extern char DAT_00504ea8[];
extern char DAT_00504ea0[];
extern char DAT_00504e98[];

void FUN_004257a0();
void FUN_0047ae60();
void *__stdcall FUN_004aa8f0(void *, char *, int);
void __stdcall FUN_004288d0(char *, int, int, int);
int __stdcall FUN_0049ff10(int, char *);
void __stdcall FUN_004a1110(void *, char *, int);
void __stdcall FUN_0049fa90(void *);
void FUN_0047a0e0();
void __stdcall FUN_0049fb10(void *, int);
void __stdcall FUN_004a81e0(void *, int);
void __stdcall FUN_00491c80(int);
void __stdcall FUN_0047b9f0(void *);

class Class_00435a20 {
public:
    int FUN_00435a20(char *);
};
class Class_00435d30 {
public:
    void FUN_00435d30(int);
};
class Class_00435c30 {
public:
    char *FUN_00435c30();
};

// FUNCTION: 0x47bbb0
void FUN_0047bbb0(void)
{
    int difficulty;
    void *dialog;

    FUN_004257a0();
    dialog = FUN_004aa8f0(g_game + 0x519, DAT_005084ac, 0);
    ((int *)dialog)[2] = (int)FUN_0047ae60;
    ((int *)dialog)[3] = (int)g_game;
    FUN_004288d0(DAT_0050849c, 0, 0, 0);

    *(int *)(g_game + 0x37eee) = *(int *)(*(char **)(g_game + 0x29a0) + 0x228);
    difficulty = FUN_0049ff10(*(int *)(*(char **)(g_game + 0x531) + 4), DAT_00502a78);
    if (*(int *)(g_game + 0x37eee) == 0) {
        *(char *)(difficulty + 0x137) = 0;
        FUN_004a1110(g_game + 0x519, DAT_00504ea8, 1);
    }
    if (*(int *)(g_game + 0x37eee) == 1) {
        *(char *)(difficulty + 0x137) = 1;
        FUN_004a1110(g_game + 0x519, DAT_00504ea0, 1);
    }
    if (*(int *)(g_game + 0x37eee) == 2) {
        *(char *)(difficulty + 0x137) = 2;
        FUN_004a1110(g_game + 0x519, DAT_00504e98, 1);
    }
    FUN_0049fa90(g_game + 0x519);

    if (!((Class_00435a20 *)*(int *)(g_game + 0x391e9))->FUN_00435a20(*(char **)(g_game + 0x29a0) + 0x11c)) {
        ((Class_00435d30 *)*(int *)(g_game + 0x391e9))->FUN_00435d30(0);
        strncpy(*(char **)(g_game + 0x29a0) + 0x11c,
                ((Class_00435c30 *)*(int *)(g_game + 0x391e9))->FUN_00435c30(), 0x100);
    }

    FUN_0047a0e0();
    *(int *)(*(char **)(g_game + 0x531) + 0x3b) = (int)FUN_0047b9f0;
    FUN_0049fb10(g_game + 0x519, 1);
    FUN_004a81e0(g_game + 0x519, 0x40);
    FUN_00491c80(0x13);
}
