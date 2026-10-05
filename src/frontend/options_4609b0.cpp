// Decompiled by space-bunny-free. Names are provisional.
// Handler of the options menu (ARMOPT.GUI, opened by 0x460cc0). A gadget whose
// parent is gone (field_60 == -1) takes the teardown path: hide the menu, drop
// the two flip surfaces, clear the settings-changed words, re-assert the mouse
// capture. Otherwise the pressed gadget name selects the action:
// LOADGAME / SAVEGAME / PREFS / HELP / MISSION / EXIT / OK, and anything else
// falls through to the default handler FUN_004ab0a0.

class Mission {
public:
    int FUN_00435100();
};

class Class_004ce910 {
public:
    void PauseCdAudio(int value);
};

class Class_004c6a60 {
public:
    char unknown_0[4];
};

#pragma pack(push, 1)
struct Sub_004609b0 {
    char unknown_0[0x10];
};

// One gadget inside a .GUI file. The two writes in the MISSION arm reach the
// entry as gadgets + i + i * 0x15a, that is, one gadget stride too far.
struct Entry_004609b0 {
    char unknown_0[0x1b];
    int flags;                         // +0x1b
    char unknown_1f[0xb6 - 0x1f];
    short count;                       // +0xb6
    char unknown_b8[0x15a - 0xb8];
};

struct Game {
    char unknown_0[0x10];
    Class_004ce910* field_10;          // +0x10
    char unknown_14[0x519 - 0x14];
    Sub_004609b0 sub;                  // +0x519
    char unknown_529[0x2a44 - 0x529];
    unsigned char flags_2a44;          // +0x2a44
    char unknown_2a45[0x37ebe - 0x2a45];
    unsigned short orders;             // +0x37ebe
    char unknown_37ec0[0x38a51 - 0x37ec0];
    unsigned short flags_38a51;        // +0x38a51
    char unknown_38a53[0x391e9 - 0x38a53];
    Mission* mode;                     // +0x391e9
};
#pragma pack(pop)

struct Gadget_004609b0 {
    char unknown_0[0x60];
    int field_60;                      // +0x60
};

struct Info_004609b0 {
    char unknown_0[0x4];
    Entry_004609b0* info;              // +0x4
    int (__stdcall* handler)(void*);   // +0x8
};

extern Game* g_game;
extern Class_004c6a60* DAT_00512fe8;
extern Class_004c6a60* DAT_00512ff4;
extern int DAT_00512fe4;
extern int DAT_00512ef0;

int __stdcall IsCurrentGadgetNamed(Gadget_004609b0* gadget, char* name);
void __stdcall PlaySoundByName(char* str, int flag);
void __stdcall FUN_0049fa70(Sub_004609b0* sub);
void __stdcall FreeSurface(Class_004c6a60* surface);
Info_004609b0* __stdcall LoadGuiLayer(Sub_004609b0* sub, const char* name, int flags);
void __stdcall LoadPictureCached(const char* name, int a, int b, int c);
int __stdcall FillHelpPage(Sub_004609b0* sub, int a, int b);
int __stdcall FindGadgetIndex(Entry_004609b0* info, const char* name, int type);
void __stdcall AllocBlinkWords(Sub_004609b0* sub, int value);
void __stdcall FUN_0049fb10(Sub_004609b0* sub, int value);
void __stdcall RenderLayer(Sub_004609b0* sub, int value);
void __stdcall FUN_004ab0a0(Gadget_004609b0* gadget);
void ShowLoadGameScreen();
void ShowSaveGameScreen();
void OpenOptionsPanel();
void FUN_0045f1d0();
void FUN_00476d80();
void OpenExitMenu();
int __stdcall HandleHelpClick(void* gadget);
int __stdcall HandleBriefingClick(void* gadget);

// FUNCTION: 0x4609b0
void __stdcall HandleInGameOptionsClick(Gadget_004609b0* gadget)
{
    if (gadget->field_60 == -1) {
        FUN_0049fa70(&g_game->sub);
        DAT_00512fe4 = 0;
        if (DAT_00512fe8) {
            FreeSurface(DAT_00512fe8);
            DAT_00512fe8 = 0;
        }
        if (DAT_00512ff4) {
            FreeSurface(DAT_00512ff4);
            DAT_00512ff4 = 0;
        }
        if (g_game->flags_2a44 & 4) {
            if (g_game->mode->FUN_00435100() != 3)
                g_game->flags_38a51 &= 0xfffe;
        }
        g_game->orders &= 0xfffe;
        g_game->field_10->PauseCdAudio(0);
        return;
    }
    if (IsCurrentGadgetNamed(gadget, "LOADGAME")) {
        PlaySoundByName("Options", 0);
        ShowLoadGameScreen();
        return;
    }
    if (IsCurrentGadgetNamed(gadget, "SAVEGAME")) {
        PlaySoundByName("Options", 0);
        ShowSaveGameScreen();
        return;
    }
    if (IsCurrentGadgetNamed(gadget, "PREFS")) {
        PlaySoundByName("Options", 0);
        OpenOptionsPanel();
        return;
    }
    if (IsCurrentGadgetNamed(gadget, "HELP")) {
        PlaySoundByName("Options", 0);
        Info_004609b0* g = LoadGuiLayer(&g_game->sub, "HELP.GUI", 0x1881);
        g->handler = HandleHelpClick;
        LoadPictureCached("dhelp", 0, 0, 0);
        DAT_00512ef0 = g->info->count;
        FillHelpPage(&g_game->sub, 0, 0x11);
        FUN_0049fb10(&g_game->sub, 1);
        RenderLayer(&g_game->sub, 0x40);
        return;
    }
    if (IsCurrentGadgetNamed(gadget, "MISSION")) {
        PlaySoundByName("Options", 0);
        if (g_game->mode->FUN_00435100() == 1) {
            Info_004609b0* g = LoadGuiLayer(&g_game->sub, "BRIEFING.GUI", 0);
            Entry_004609b0* gadgets = g->info;
            g->handler = HandleBriefingClick;
            int i = FindGadgetIndex(gadgets, "MOREBAR", 0xe);
            // The entry is reached as gadgets + i + i * 0x15a, not gadgets +
            // i * 0x15a, so this clears the flag of a gadget one stride past
            // the one just looked up.
            ((Entry_004609b0*)((char*)gadgets + i))[i].flags &= ~0x10;
            i = FindGadgetIndex(gadgets, "TextRegion", 0xe);
            ((Entry_004609b0*)((char*)gadgets + i))[i].flags &= ~0x10;
            LoadPictureCached("igmbrief", 0, 0, 0);
            AllocBlinkWords(&g_game->sub, 0xf);
            FUN_00476d80();
            RenderLayer(&g_game->sub, 0x40);
            return;
        }
        FUN_0045f1d0();
        return;
    }
    if (IsCurrentGadgetNamed(gadget, "EXIT")) {
        PlaySoundByName("Options", 0);
        OpenExitMenu();
        return;
    }
    if (IsCurrentGadgetNamed(gadget, "OK")) {
        PlaySoundByName("Options", 0);
        return;
    }
    if (gadget->field_60 != -1)
        FUN_004ab0a0(gadget);
}
