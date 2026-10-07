// Decompiled by deepseek-v4.1-flash, finished by GPT-6, edited by deepseek-v4.1, finished by space-bunny-free, finished by deepseek-v4.1-flash, finished by opus. Names are provisional.
// Click handler of the campaign screen.
#pragma pack(push, 1)
struct Entry_00477ab0 {
    char unknown_0[0xba];
    short selected;                    // +0xba
    char unknown_bc[0xc2 - 0xbc];
    char* text;                        // +0xc2
    char unknown_c6[0x15b - 0xc6];
};

struct Holder_00477ab0 {
    char unknown_0[4];
    Entry_00477ab0* entries;           // +0x04
};

struct Menu_00477ab0 {
    char unknown_0[0x18];
    Holder_00477ab0* holder;           // +0x18
    char unknown_1c[0x60 - 0x1c];
    int current;                       // +0x60
};
#pragma pack(pop)

class Mission {
public:
    void LoadCampaign(char* name);
    int BuildMissionList(int** out);
    int SelectMission(int index);
};

extern char* g_game;                   // 0x511de8
extern int* DAT_0051e65c;              // 0x51e65c
extern int* DAT_0051e660;              // 0x51e660
extern int DAT_0051e668;               // 0x51e668
extern int DAT_00507b6c;               // 0x507b6c

int __stdcall IsCurrentGadgetNamed(Menu_00477ab0* menu, char* name);
char __stdcall FindGameCdDrive(int param_1);
void RegisterDataArchives();
void FUN_0041da30();
void SaveSettings();
void __stdcall PlaySoundByName(char* name, int param_2);
void __stdcall FUN_00491c80(int value);
Entry_00477ab0* __stdcall FindGadgetChecked(Entry_00477ab0* entries, char* name);
int __stdcall FindGadgetIndex(Entry_00477ab0* entries, char* name, int type);
char* __stdcall SkipTextLines(char* text, int line);
void __stdcall FUN_004a2be0(void* menu, int index);
void __stdcall FUN_0049fa90(void* menu);
void __stdcall SetGadgetStatusByName(void* menu, char* name, int flag);
int __stdcall BuildCampaignNameList(int** out, int side);
void __stdcall FUN_004a32a0(void* menu, char* name, int* data, int count, int flag);
void __stdcall FUN_004a0570(void* menu, char* name, int flag);
void __cdecl FUN_004d85a0(void* ptr);
char* __stdcall Translate(char* text);
void __stdcall OpenMessageBox(char* dest, char* text, int param_3, int param_4, int param_5);
void __stdcall FUN_004ab0a0(void* menu);

// FillCampaignList and FillMissionList stay unannotated so they inline into the
// handler; FillMissionList takes the menu and re-reads the holder from g_game.
void __stdcall FillCampaignList(int side)
{
    Holder_00477ab0* gadget = *(Holder_00477ab0**)(g_game + 0x531);
    if (DAT_0051e65c != 0) {
        FUN_004d85a0(DAT_0051e65c);
        DAT_0051e65c = 0;
    }
    PlaySoundByName("smlbutton", 0);
    int count = BuildCampaignNameList(&DAT_0051e65c, side);
    FUN_004a32a0(g_game + 0x519, "Campaign", DAT_0051e65c, count, 0);
    int index = FindGadgetIndex(gadget->entries, "Campaign", 2);
    FUN_004a2be0(g_game + 0x519, index);
    FUN_0049fa90(g_game + 0x519);
}

void __stdcall FillMissionList(Menu_00477ab0* menu, Entry_00477ab0* unused)
{
    Holder_00477ab0* gadgets = menu->holder;
    if (DAT_0051e660 != 0) {
        FUN_004d85a0(DAT_0051e660);
        DAT_0051e660 = 0;
    }
    Entry_00477ab0* layout =
        FindGadgetChecked((*(Holder_00477ab0**)(g_game + 0x531))->entries, "Campaign");
    ((Mission*)*(void**)(g_game + 0x391e9))->LoadCampaign(
        SkipTextLines(layout->text, layout->selected));
    int count = ((Mission*)*(void**)(g_game + 0x391e9))->BuildMissionList(&DAT_0051e660);
    FUN_004a32a0(menu, "Missions", DAT_0051e660, count, 0);
    FUN_004a2be0(g_game + 0x519,
                 FindGadgetIndex(gadgets->entries, "Missions", 2));
    FUN_0049fa90(g_game + 0x519);
}

// FUNCTION: 0x477ab0
void __stdcall HandleNewGameClick(Menu_00477ab0* menu)
{
    Entry_00477ab0* entries = menu->holder->entries;
    char* playerInfo = g_game + 0x14b * *(unsigned char*)(g_game + 0x2a42);
    int index;

    if (menu->current == -1) {
        FUN_004d85a0(DAT_0051e65c);
        FUN_004d85a0(DAT_0051e660);
        DAT_0051e65c = 0;
        DAT_0051e660 = 0;
        return;
    }

    if ((DAT_0051e668 != 0 && (IsCurrentGadgetNamed(menu, "Missions") || IsCurrentGadgetNamed(menu, "Start"))) ||
        (DAT_0051e668 == 0 && (IsCurrentGadgetNamed(menu, "Campaign") || IsCurrentGadgetNamed(menu, "Start")))) {
        index = 0;
        PlaySoundByName("bigButton", 0);
        if (!FindGameCdDrive(0)) {
            OpenMessageBox(g_game + 0x519,
                         Translate("Please insert the Campaign CD (Disc 2) and try again"),
                         200, 1, 1);
            FUN_004ab0a0(g_game + 0x519);
            return;
        }
        RegisterDataArchives();
        FUN_0041da30();
        {
            char* name;
            if (DAT_00507b6c == 0) {
                Entry_00477ab0* e = FindGadgetChecked(entries, "Campaign");
                name = SkipTextLines(e->text, e->selected);
                ((Mission*)*(void**)(g_game + 0x391e9))->LoadCampaign(name);
            } else if (*(unsigned char*)(*(int*)(playerInfo + 0x1b8a) + 0x95) == 0) {
                ((Mission*)*(void**)(g_game + 0x391e9))->LoadCampaign("Arm Campaign");
            } else {
                ((Mission*)*(void**)(g_game + 0x391e9))->LoadCampaign("Core Campaign");
            }
        }
        if (DAT_0051e668 != 0) {
            Entry_00477ab0* e = FindGadgetChecked(entries, "Missions");
            index = e->selected;
        }
        if (((Mission*)*(void**)(g_game + 0x391e9))->SelectMission(index) != 0) {
            FUN_00491c80(0x14);
            *(unsigned char*)(*(int*)(g_game + 0x1b8a) + 0x96) = 0;
            *(unsigned char*)(*(int*)(g_game + 0x1cd5) + 0x96) = 1;
            SaveSettings();
            if (DAT_0051e668 != 0) {
                *(unsigned char*)(g_game + 0x2bc0) = 0x10;
                return;
            }
            *(unsigned char*)(g_game + 0x2bc0) = 0x0f;
            return;
        }
        goto End;
    } else {
        if (IsCurrentGadgetNamed(menu, "PrevMenu")) {
            PlaySoundByName("Previous", 0);
            *(unsigned char*)(g_game + 0x2bc0) = 3;
            FUN_00491c80(0x14);
            return;
        }
        if (IsCurrentGadgetNamed(menu, "Difficulty")) {
            PlaySoundByName("SmlButton", 0);
            // If/else-if chain into one shared exit: a switch or per-arm return changes registers.
            int diff = *(int*)(g_game + 0x37eee);
            if (diff == 0) {
                *(int*)(g_game + 0x37eee) = 1;
            } else if (diff == 1) {
                *(int*)(g_game + 0x37eee) = 2;
            } else if (diff == 2) {
                *(int*)(g_game + 0x37eee) = 0;
            } else {
                goto End;
            }
            FUN_004ab0a0(menu);
            return;
        }
        if (IsCurrentGadgetNamed(menu, "Side0") || IsCurrentGadgetNamed(menu, "Arm"))
            goto ArmSide;
        if (!IsCurrentGadgetNamed(menu, "Side1") && !IsCurrentGadgetNamed(menu, "Core"))
            goto End;

        SetGadgetStatusByName(g_game + 0x519, "Core", 1);
        SetGadgetStatusByName(g_game + 0x519, "Side1", 1);
        PlaySoundByName("SideSelect2", 0);
        index = FindGadgetIndex(entries, "Side1", 1);
        *(int*)((char*)entries + index * 0x15b + 0x1f) = 0x1f;
        *(int*)(g_game + 0x37ef2) = 1;
        *(unsigned char*)(*(int*)(playerInfo + 0x1b8a) + 0x95) = 1;
        *(unsigned char*)(*(int*)(playerInfo + 0x1cd5) + 0x95) = 0;
        if (DAT_00507b6c == 0)
            FillCampaignList(*(unsigned char*)(*(int*)(g_game + 0x14b * *(unsigned char*)(g_game + 0x2a42) + 0x1b8a) + 0x95));
        if (DAT_0051e668 != 0) {
            FillMissionList((Menu_00477ab0*)(g_game + 0x519), FindGadgetChecked(entries, "Campaign"));
            FUN_004ab0a0(menu);
            return;
        }
        goto End;

ArmSide:
        SetGadgetStatusByName(g_game + 0x519, "Arm", 1);
        SetGadgetStatusByName(g_game + 0x519, "Side0", 1);
        PlaySoundByName("SideSelect", 0);
        index = FindGadgetIndex(entries, "Side0", 1);
        *(int*)((char*)entries + index * 0x15b + 0x1f) = 0x1f;
        *(int*)(g_game + 0x37ef2) = 0;
        *(unsigned char*)(*(int*)(playerInfo + 0x1b8a) + 0x95) = 0;
        *(unsigned char*)(*(int*)(playerInfo + 0x1cd5) + 0x95) = 1;
        FillCampaignList(*(unsigned char*)(*(int*)(g_game + 0x14b * *(unsigned char*)(g_game + 0x2a42) + 0x1b8a) + 0x95));
        FUN_004a0570(menu, "Campaign", DAT_00507b6c == 0);
        if (DAT_0051e668 != 0)
            FillMissionList((Menu_00477ab0*)(g_game + 0x519), FindGadgetChecked(entries, "Campaign"));

    }
End:
    FUN_004ab0a0(menu);
}
