// Decompiled by mimo-v2.6-pro. Names are provisional.
// Click handler for one gadget of a dialog: dispatches on the gadget's name
// ("Start", "SHUTUP", "PrevMenu", "TextRegion", "MOREBAR") and drives the
// campaign menu from there.

class Sound {
public:
    void StopStream();
};

class Mission {
public:
    int GetNameSlot(int param_1);
};

struct Menu_00478cb0 {
    char unknown_0[0x60];
    int field_60;                      // +0x60
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x10];
    Sound* input;                      // +0x10
    char unknown_14[0x519 - 0x14];
    char menu[0x2bc0 - 0x519];         // +0x519
    unsigned char field_2bc0;          // +0x2bc0
    char unknown_2bc1[0x391e9 - 0x2bc1];
    Mission* net;                      // +0x391e9
    char unknown_391ed[0x391f1 - 0x391ed];
    int field_391f1;                   // +0x391f1
};
#pragma pack(pop)

extern Game* g_game;
extern char* DAT_0051e63c;

int __stdcall IsCurrentGadgetNamed(Menu_00478cb0* menu, char* name);
void __stdcall PlaySoundByName(char* str, int flag);
char __stdcall FindGameCdDrive(int param_1);
void RegisterDataArchives();
void __stdcall SetCursorMode(int param_1);
void BlankScreen();
char* __stdcall Translate(char* text);
void __stdcall OpenMessageBox(char* menu, char* text, int param_3, int param_4, int param_5);
void __stdcall FUN_004ab0a0(void* menu);
int __stdcall GetButtonStageByName(Menu_00478cb0* menu, char* name);
void __stdcall StreamSoundDelayed(char* text, int param_2, int param_3);
void __stdcall FUN_0049fa90(Menu_00478cb0* menu);
void DrawHelpPage();
void __stdcall FreeBlinkWords(char* menu);
void __cdecl FUN_004d85a0(void* param_1);

// FUNCTION: 0x478cb0
void __stdcall HandleMissionBriefingClick(Menu_00478cb0* menu)
{
    if (menu->field_60 == -1) {
        FreeBlinkWords(g_game->menu);
        FUN_004d85a0(DAT_0051e63c);
        DAT_0051e63c = 0;
        return;
    }
    if (IsCurrentGadgetNamed(menu, "Start")) {
        PlaySoundByName("BigButton", 0);
        if (FindGameCdDrive(0)) {
            RegisterDataArchives();
            SetCursorMode(0x14);
            g_game->input->StopStream();
            BlankScreen();
            g_game->field_2bc0 = 2;
            return;
        }
        OpenMessageBox(g_game->menu,
                     Translate("Please insert the Campaign CD (Disc 2) and try again"),
                     200, 1, 1);
        FUN_004ab0a0(g_game->menu);
        return;
    }
    if (IsCurrentGadgetNamed(menu, "SHUTUP")) {
        PlaySoundByName("Options", 0);
        if (!GetButtonStageByName(menu, "SHUTUP")) {
            g_game->input->StopStream();
        } else if (g_game->field_391f1 != 6) {
            char* text = (char*)g_game->net->GetNameSlot(3);
            if (text) {
                StreamSoundDelayed(text, 0, 0x3c);
            }
        }
        FUN_004ab0a0(menu);
        PlaySoundByName("SmallButton", 0);
        return;
    }
    if (IsCurrentGadgetNamed(menu, "PrevMenu")) {
        g_game->input->StopStream();
        PlaySoundByName("Previous", 0);
        BlankScreen();
        g_game->field_2bc0 = 3;
        SetCursorMode(0x14);
        return;
    }
    if (IsCurrentGadgetNamed(menu, "TextRegion") || IsCurrentGadgetNamed(menu, "MOREBAR")) {
        if (DAT_0051e63c) {
            PlaySoundByName("More", 0);
            DrawHelpPage();
            FUN_0049fa90(menu);
        }
    }
    FUN_004ab0a0(menu);
}
