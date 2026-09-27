// Decompiled by deepseek-v4.1-flash. Names are provisional.

#pragma pack(push, 1)
struct Entry_004931d0 {
    char unknown_0[0xce];
    void* field_ce;                    // +0xce
};

struct Gadget_004931d0 {
    char unknown_0[4];
    Entry_004931d0* info;              // +0x4
    void (__stdcall* handler)(void*);  // +0x8
    void* context;                     // +0xc
};
#pragma pack(pop)

extern char* g_game;
extern void* DAT_0051f2e4;
extern void* DAT_0051f2e8;

Gadget_004931d0* __stdcall FUN_004aa8f0(char* sub, const char* name, int flags);
void __stdcall FUN_004288d0(const char* name, int a, int b, int c);
int __stdcall FUN_00492b10(int* out);
void __stdcall FUN_004a9660(char* sub);
char* __stdcall FUN_004c5740(char* text);
void __stdcall FUN_004abd90(char* dest, char* text, int a, int b, int c);
char* FUN_00476830();
void __stdcall FUN_004a32a0(char* menu, char* name, void* text, int count, int flag);
void __stdcall FUN_004a0570(char* menu, char* name, int value);
Entry_004931d0* __stdcall FUN_0049ff90(void* entries, char* name);
void __stdcall FUN_00492de0(int a, int b);
void __stdcall FUN_00492360(void* gadget);
void FUN_00491ec0();
void __stdcall FUN_0049fb10(char* sub, int value);
void FUN_00428b60();
void __stdcall FUN_0049fa50(char* sub);
void __stdcall FUN_004a81e0(char* sub, int value);

// FUNCTION: 0x4931d0
void FUN_004931d0()
{
    Gadget_004931d0* gadget = FUN_004aa8f0(g_game + 0x519, "LOADGAME.GUI", 0x980);
    gadget->handler = FUN_00492360;
    gadget->context = g_game;
    FUN_004288d0("DLOADGAME2", 0, 0, 0);
    int count;
    if (FUN_00492b10(&count) == 0) {
        FUN_004a9660(g_game + 0x519);
        FUN_004abd90(g_game + 0x519,
                     FUN_004c5740("There are no saved games to choose from"),
                     0x140, 1, 1);
        return;
    }
    DAT_0051f2e8 = FUN_00476830();
    FUN_004a32a0(g_game + 0x519, "GAMES", DAT_0051f2e4, count, 0);
    FUN_004a0570(g_game + 0x519, "DELETE", 0);
    FUN_004a0570(g_game + 0x519, "GAMENAME", 0);
    Entry_004931d0* entry = FUN_0049ff90(gadget->info, "GAMES");
    if (entry != 0) {
        entry->field_ce = (void*)FUN_00492de0;
    }
    FUN_00491ec0();
    FUN_0049fb10(g_game + 0x519, 1);
    FUN_00428b60();
    FUN_004a0570(g_game + 0x519, "SaveGame", 0);
    FUN_0049fa50(g_game + 0x519);
    FUN_004a81e0(g_game + 0x519, 0x40);
    g_game[0x38a51] |= 1;
}
