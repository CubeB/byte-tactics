// Decompiled by mimo-v2.6-pro. Names are provisional.
// Click handler for one gadget of a dialog: dispatches on the gadget's name
// ("Start", "SHUTUP", "PrevMenu", "TextRegion", "MOREBAR") and drives the
// campaign menu from there.

class Class_004cfb40 {
public:
    void FUN_004cfb40();
};

class Class_004356c0 {
public:
    int FUN_004356c0(int param_1);
};

struct Menu_00478cb0 {
    char unknown_0[0x60];
    int field_60;                      // +0x60
};

#pragma pack(push, 1)
struct Game_00478cb0 {
    char unknown_0[0x10];
    Class_004cfb40* input;             // +0x10
    char unknown_14[0x519 - 0x14];
    char menu[0x2bc0 - 0x519];         // +0x519
    unsigned char field_2bc0;          // +0x2bc0
    char unknown_2bc1[0x391e9 - 0x2bc1];
    Class_004356c0* net;               // +0x391e9
    char unknown_391ed[0x391f1 - 0x391ed];
    int field_391f1;                   // +0x391f1
};
#pragma pack(pop)

extern Game_00478cb0* g_game;
extern char* DAT_0051e63c;

int __stdcall FUN_0049fd60(Menu_00478cb0* menu, char* name);
void __stdcall FUN_0047f1a0(char* str, int flag);
char __stdcall FUN_0041d6a0(int param_1);
void FUN_0041d4c0();
void __stdcall FUN_00491c80(int param_1);
void FUN_004257a0();
char* __stdcall FUN_004c5740(char* text);
void __stdcall FUN_004abd90(char* menu, char* text, int param_3, int param_4, int param_5);
void __stdcall FUN_004ab0a0(void* menu);
int __stdcall FUN_004a0f60(Menu_00478cb0* menu, char* name);
void __stdcall FUN_0047f090(char* text, int param_2, int param_3);
void __stdcall FUN_0049fa90(Menu_00478cb0* menu);
void FUN_00476ef0();
void __stdcall FUN_004afcf0(char* menu);
void FUN_004d85a0(void* param_1);

// FUNCTION: 0x478cb0
void __stdcall FUN_00478cb0(Menu_00478cb0* menu)
{
    if (menu->field_60 == -1) {
        FUN_004afcf0(g_game->menu);
        FUN_004d85a0(DAT_0051e63c);
        DAT_0051e63c = 0;
        return;
    }
    if (FUN_0049fd60(menu, "Start")) {
        FUN_0047f1a0("BigButton", 0);
        if (FUN_0041d6a0(0)) {
            FUN_0041d4c0();
            FUN_00491c80(0x14);
            g_game->input->FUN_004cfb40();
            FUN_004257a0();
            g_game->field_2bc0 = 2;
            return;
        }
        FUN_004abd90(g_game->menu,
                     FUN_004c5740("Please insert the Campaign CD (Disc 2) and try again"),
                     200, 1, 1);
        FUN_004ab0a0(g_game->menu);
        return;
    }
    if (FUN_0049fd60(menu, "SHUTUP")) {
        FUN_0047f1a0("Options", 0);
        if (!FUN_004a0f60(menu, "SHUTUP")) {
            g_game->input->FUN_004cfb40();
        } else if (g_game->field_391f1 != 6) {
            char* text = (char*)g_game->net->FUN_004356c0(3);
            if (text) {
                FUN_0047f090(text, 0, 0x3c);
            }
        }
        FUN_004ab0a0(menu);
        FUN_0047f1a0("SmallButton", 0);
        return;
    }
    if (FUN_0049fd60(menu, "PrevMenu")) {
        g_game->input->FUN_004cfb40();
        FUN_0047f1a0("Previous", 0);
        FUN_004257a0();
        g_game->field_2bc0 = 3;
        FUN_00491c80(0x14);
        return;
    }
    if (FUN_0049fd60(menu, "TextRegion") || FUN_0049fd60(menu, "MOREBAR")) {
        if (DAT_0051e63c) {
            FUN_0047f1a0("More", 0);
            FUN_00476ef0();
            FUN_0049fa90(menu);
        }
    }
    FUN_004ab0a0(menu);
}
