// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Opens the REPORT.GUI dialog, fills in the "CHK%d" / "SERVICE%d" menu
// entries from the score report tables, and shows it.
#include <windows.h>

#pragma pack(push, 1)
struct Sub_004435a0 {
    char unknown_0[0x10];
};

struct Game {
    char unknown_0[0x519];
    Sub_004435a0 sub;                  // +0x519
};
#pragma pack(pop)

struct Gadget_004435a0 {
    char unknown_0[0x8];
    void (__stdcall* handler)(void*);  // +0x8
    Game* field_c;                     // +0xc
    char unknown_10[0xc];
    void (__stdcall* field_1c)();      // +0x1c
};

// GLOBAL: 0x511de8
extern Game* g_game;

Gadget_004435a0* __stdcall FUN_004aa8f0(Sub_004435a0* sub, const char* name, int flags);
void __stdcall FUN_004288d0(const char* name, int a, int b, int c);
void __stdcall FUN_004a0570(Sub_004435a0* sub, char* name, int value);
void __stdcall FUN_004a0bf0(Sub_004435a0* sub, char* name, char* text, int size);
void __stdcall FUN_0049fb10(Sub_004435a0* sub, int value);
void __stdcall FUN_004a81e0(Sub_004435a0* sub, int value);
void __stdcall FUN_00491c80(int n);
void __stdcall FUN_0049fa90(Sub_004435a0* sub);
void __stdcall FUN_0049fad0(Sub_004435a0* sub);
void __stdcall FUN_00443480(void* gadget);
void __stdcall FUN_00443590();

// FUNCTION: 0x4435a0
void __stdcall FUN_004435a0(unsigned int* count, char** names)
{
    char name[16];
    Gadget_004435a0* gadget = FUN_004aa8f0(&g_game->sub, "REPORT.GUI", 0x800);
    gadget->handler = FUN_00443480;
    gadget->field_c = g_game;
    gadget->field_1c = FUN_00443590;
    FUN_004288d0("scorebg", 0, 1, 0);
    for (unsigned int i = 0; i < *count; i++) {
        wsprintfA(name, "CHK%d", i);
        FUN_004a0570(&g_game->sub, name, 1);
        wsprintfA(name, "SERVICE%d", i);
        FUN_004a0570(&g_game->sub, name, 1);
        FUN_004a0bf0(&g_game->sub, name, names[i], 0x80);
    }
    FUN_0049fb10(&g_game->sub, 1);
    FUN_004a81e0(&g_game->sub, 0x141);
    FUN_00491c80(0x13);
    FUN_0049fa90(&g_game->sub);
    FUN_0049fad0(&g_game->sub);
}
