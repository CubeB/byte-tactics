// Decompiled by deepseek-v4.1-flash, finished by longcat-2.5-preview-free. Names are provisional.
// Sets up the mission briefing dialog (MSNBRIEF.GUI): builds its name from the
// local player's side, fills the gadget list, picks the planet whose name
// matches the net object's name, and wires the PANORAMA and PLANET gadgets.
// Gadget records are 0x15b bytes; Gadget has a 4-byte gap between the callback
// at +0xb6 and the gaf pointer at +0xbe.
#include <stdio.h>
#include <string.h>

extern char* g_game;                    // pointer at 0x511de8
extern int DAT_0051e654;
extern int DAT_0051e670;
extern int DAT_0051e640;

class Class_00435910 {
public:
    char unknown_0[0xd34];
    int field_d34;                      // +0xd34
    int field_d38;                      // +0xd38
    char* FUN_00435910();
};

#pragma pack(push, 1)

struct PlayerData {
    char unknown_0[0x95];
    unsigned char side;                 // +0x95
};

struct PlayerEntry {
    PlayerData* data;                   // +0x0
    char unknown_4[0x14b - 4];
};

struct Gadget {
    char unknown_0[0x1b];
    int field_1b;                       // +0x1b
    char unknown_1f[0xb6 - 0x1f];
    void* field_b6;                     // +0xb6
    char unknown_ba[4];
    void* field_be;                     // +0xbe
    char unknown_c2[0xc6 - 0xc2];
    short field_c6;                     // +0xc6
    char unknown_c8[0x15b - 0xc8];
};

struct GadgetRoot {
    char unknown_0[0xc0];
    void* surface;                      // +0xc0
};

struct Dialog {
    int unknown_0;
    Gadget* gadgets;                    // +0x4
    void* handler;                      // +0x8
    void* context;                      // +0xc
};

struct Game {
    char unknown_0[0x1b8a];
    PlayerEntry players[10];            // +0x1b8a, stride 0x14b
    char unknown_2878[0x2a42 - 0x2878];
    unsigned char localPlayer;          // +0x2a42
    char unknown_2a43[0x37ef2 - 0x2a43];
    int flag_37ef2;                     // +0x37ef2
    char unknown_37ef6[0x391e9 - 0x37ef6];
    Class_00435910* net;                // +0x391e9
};

#pragma pack(pop)

void __stdcall LoadPictureCached(char* name, int param_2, int param_3, int param_4);
Dialog* __stdcall LoadGuiLayer(char* menu, char* name, int param_3);
void __stdcall FUN_004ac7d0(char* menu, void* param_2, void* param_3);
int __stdcall FindGadgetIndex(Gadget* gadgets, char* name, int param_3);
void __stdcall FUN_004a0570(char* menu, char* name, int param_3);
void __stdcall SetButtonStageByName(char* menu, char* name, int param_3);
int __stdcall LoadScreenGaf(char* menu, char* name);
void* __stdcall FindGafEntry(void* surface, char* name);
void __stdcall InitGafSequence(void* state, void* gaf, int param_3);
void __stdcall AllocBlinkWords(char* menu, int param_2);
void __stdcall FUN_0049fb10(char* menu, int param_2);
void __stdcall RenderLayer(char* menu, int param_2);
void __stdcall FUN_00491c80(int param_1);
void FUN_00476d80();
void HandleMissionBriefingClick();
void UpdateSolarSystem();
void FUN_00478b40();
int __cdecl rand();

// FUNCTION: 0x478e80
void OpenMissionBriefing(void)
{
    char* names[16];
    char* briefs[16];
    char* pans[16];
    char* rotates[16];
    char buf[20];
    char* name;
    Dialog* dialog;
    Gadget* gadgets;
    int i;
    unsigned char side;

    side = ((PlayerEntry*)(g_game + 0x1b8a))[((Game*)g_game)->localPlayer].data->side;
    sprintf(buf, "mbrief%s", g_game + 0x37f5b + side * 0x232);

    dialog = LoadGuiLayer(g_game + 0x519, "MSNBRIEF.GUI", 0x80);
    dialog->handler = (void*)HandleMissionBriefingClick;
    dialog->context = g_game;

    LoadPictureCached(buf, 1, 1, 0);
    FUN_004ac7d0(g_game + 0x519, g_game + 0x143a7, g_game + 0x5cb);

    gadgets = (*(Dialog**)(g_game + 0x531))->gadgets;
    strcpy((char*)gadgets + 0xcc, "Start");
    strcpy((char*)gadgets + 0xdc, "PrevMenu");

    i = FindGadgetIndex(gadgets, "MOREBAR", 0xe);
    gadgets[i].field_1b &= ~0x10;
    i = FindGadgetIndex(gadgets, "TextRegion", 0xe);
    gadgets[i].field_1b &= ~0x10;

    FUN_004a0570(g_game + 0x519, "SOLARSYSTEM", 0);
    SetButtonStageByName(g_game + 0x519, "SHUTUP", 1);

    DAT_0051e654 = ((Game*)g_game)->net->field_d34 +
                   rand() % (((Game*)g_game)->net->field_d38 - ((Game*)g_game)->net->field_d34 + 1);
    DAT_0051e670 = rand() % 64;

    name = ((Game*)g_game)->net->FUN_00435910();
    if (strcmp(name, "Lunar") == 0 && ((Game*)g_game)->flag_37ef2 != 0)
        strcat(name, "2");

    names[0] = "Green planet";
    names[1] = "Archipelago";
    names[2] = "Wet Desert";
    names[3] = "Desert";
    names[4] = "Lava";
    names[5] = "Red Planet";
    names[6] = "Lunar";
    names[7] = "Metal";
    names[8] = "Lunar2";
    names[9] = "Ice";
    names[10] = "Lush";
    names[11] = "Slate";
    names[12] = "Water World";
    names[13] = "Acid";
    names[14] = "Crystal";
    names[15] = 0;

    briefs[0] = "Greenbrief";
    briefs[1] = "Archibrief";
    briefs[2] = "WDesertbrief";
    briefs[3] = "Desertbrief";
    briefs[4] = "Lavabrief";
    briefs[5] = "Marsbrief";
    briefs[6] = "Lunarbrief";
    briefs[7] = "Metalbrief";
    briefs[8] = "Lunar2brief";
    briefs[9] = "Icebrief";
    briefs[10] = "Lushbrief";
    briefs[11] = "Slatebrief";
    briefs[12] = "Waterbrief";
    briefs[13] = "Acidbrief";
    briefs[14] = "Crystalbrief";
    briefs[15] = 0;

    pans[0] = "GreenPan";
    pans[1] = "ArchiPan";
    pans[2] = "WDesPan";
    pans[3] = "DDesPan";
    pans[4] = "LavaPan";
    pans[5] = "MarsPan";
    pans[6] = "LunarPan";
    pans[7] = "MetalPan";
    pans[8] = "Lunar2Pan";
    pans[9] = "IcePan";
    pans[10] = "LushPan";
    pans[11] = "SlatePan";
    pans[12] = "WaterPan";
    pans[13] = "AcidPan";
    pans[14] = "CrystPan";
    pans[15] = 0;

    rotates[0] = "GreenRotate";
    rotates[1] = "ArchiRotate";
    rotates[2] = "WDesertRotate";
    rotates[3] = "DDesRotate";
    rotates[4] = "LavaRotate";
    rotates[5] = "MarsRotate";
    rotates[6] = "LunarRotate";
    rotates[7] = "MetalRotate";
    rotates[8] = "Lunar2Rotate";
    rotates[9] = "IceRotate";
    rotates[10] = "LushRotate";
    rotates[11] = "SlateRotate";
    rotates[12] = "WaterRotate";
    rotates[13] = "AcidRotate";
    rotates[14] = "CrystalRotate";
    rotates[15] = 0;

    i = 0;
    while (briefs[i] != 0 && strcmp(name, names[i]) != 0)
        i++;
    if (briefs[i] == 0)
        i = 0;

    if (LoadScreenGaf(g_game + 0x519, briefs[i])) {
        int idx = FindGadgetIndex(gadgets, "PANORAMA", 6);
        if (idx != -1) {
            Gadget* g = &gadgets[idx];
            g->field_c6 = 0;
            void* gaf = FindGafEntry(((GadgetRoot*)gadgets)->surface, pans[i]);
            if (gaf != 0) {
                g->field_be = gaf;
                g->field_b6 = (void*)UpdateSolarSystem;
            }
        }
        idx = FindGadgetIndex(gadgets, "PLANET", 6);
        if (idx != -1) {
            void* gaf = FindGafEntry(((GadgetRoot*)gadgets)->surface, rotates[i]);
            if (gaf != 0) {
                InitGafSequence(&DAT_0051e640, gaf, 0);
                Gadget* g = &gadgets[idx];
                g->field_c6 = 0;
                g->field_be = gaf;
                g->field_b6 = (void*)FUN_00478b40;
            }
        }
    }

    FUN_004a0570(g_game + 0x519, "SOLARSYSTEM", 0);
    AllocBlinkWords(g_game + 0x519, 0xf);
    FUN_00476d80();
    FUN_0049fb10(g_game + 0x519, 1);
    RenderLayer(g_game + 0x519, 0xc0);
    FUN_00491c80(0x13);
}
