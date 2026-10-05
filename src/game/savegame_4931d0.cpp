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

Gadget_004931d0* __stdcall LoadGuiLayer(char* sub, const char* name, int flags);
void __stdcall LoadPictureCached(const char* name, int a, int b, int c);
int __stdcall ListSavedGames(int* out);
void __stdcall CloseTopScreen(char* sub);
char* __stdcall Translate(char* text);
void __stdcall OpenMessageBox(char* dest, char* text, int a, int b, int c);
char* BuildSideList();
void __stdcall FUN_004a32a0(char* menu, char* name, void* text, int count, int flag);
void __stdcall FUN_004a0570(char* menu, char* name, int value);
Entry_004931d0* __stdcall FindGadgetChecked(void* entries, char* name);
void __stdcall FUN_00492de0(int a, int b);
void __stdcall LoadGameScreenHandler(void* gadget);
void ShowSavedGameInfo();
void __stdcall FUN_0049fb10(char* sub, int value);
void FUN_00428b60();
void __stdcall FUN_0049fa50(char* sub);
void __stdcall RenderLayer(char* sub, int value);

// FUNCTION: 0x4931d0
void ShowLoadGameScreen()
{
    Gadget_004931d0* gadget = LoadGuiLayer(g_game + 0x519, "LOADGAME.GUI", 0x980);
    gadget->handler = LoadGameScreenHandler;
    gadget->context = g_game;
    LoadPictureCached("DLOADGAME2", 0, 0, 0);
    int count;
    if (ListSavedGames(&count) == 0) {
        CloseTopScreen(g_game + 0x519);
        OpenMessageBox(g_game + 0x519,
                     Translate("There are no saved games to choose from"),
                     0x140, 1, 1);
        return;
    }
    DAT_0051f2e8 = BuildSideList();
    FUN_004a32a0(g_game + 0x519, "GAMES", DAT_0051f2e4, count, 0);
    FUN_004a0570(g_game + 0x519, "DELETE", 0);
    FUN_004a0570(g_game + 0x519, "GAMENAME", 0);
    Entry_004931d0* entry = FindGadgetChecked(gadget->info, "GAMES");
    if (entry != 0) {
        entry->field_ce = (void*)FUN_00492de0;
    }
    ShowSavedGameInfo();
    FUN_0049fb10(g_game + 0x519, 1);
    FUN_00428b60();
    FUN_004a0570(g_game + 0x519, "SaveGame", 0);
    FUN_0049fa50(g_game + 0x519);
    RenderLayer(g_game + 0x519, 0x40);
    g_game[0x38a51] |= 1;
}
