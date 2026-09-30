// Decompiled by deepseek-v4.1-flash, finished by GPT-6. Names are provisional.
// Partial, 48.7%. Restored player and campaign-holder reloads around callbacks.
// Remaining differences include duplicated mission blocks and branch/register scheduling.
// Click handler for the single-player Campaign screen. Dispatches on the gadget
// name (Start/Campaign/Missions/bigButton, PrevMenu, Difficulty, Side0/Arm,
// Side1/Core) and rebuilds the campaign or missions list, checking the campaign
// CD. Status: partial. See note at the bottom for what still differs.

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

class Class_00435110 {
public:
    void FUN_00435110(char* name);
};

class Class_00435760 {
public:
    int FUN_00435760(int** out);
};

class Class_00435c00 {
public:
    int FUN_00435c00(int index);
};

extern char* g_game;                   // 0x511de8
extern int* DAT_0051e65c;              // 0x51e65c
extern int* DAT_0051e660;              // 0x51e660
extern int DAT_0051e668;               // 0x51e668
extern int DAT_00507b6c;               // 0x507b6c

int __stdcall FUN_0049fd60(Menu_00477ab0* menu, char* name);
char __stdcall FUN_0041d6a0(int param_1);
void FUN_0041d4c0();
void FUN_0041da30();
void FUN_00430f00();
void __stdcall FUN_0047f1a0(char* name, int param_2);
void __stdcall FUN_00491c80(int value);
Entry_00477ab0* __stdcall FUN_0049ff90(Entry_00477ab0* entries, char* name);
int __stdcall FUN_0049fdf0(Entry_00477ab0* entries, char* name, int type);
char* __stdcall FUN_004b6af0(char* text, int line);
void __stdcall FUN_004a2be0(void* menu, int index);
void __stdcall FUN_0049fa90(void* menu);
void __stdcall FUN_004a1110(void* menu, char* name, int flag);
int __stdcall FUN_00476a60(int** out, int side);
void __stdcall FUN_004a32a0(void* menu, char* name, int* data, int count, int flag);
void __stdcall FUN_004a0570(void* menu, char* name, int flag);
void __cdecl FUN_004d85a0(void* ptr);
char* __stdcall FUN_004c5740(char* text);
void __stdcall FUN_004abd90(char* dest, char* text, int param_3, int param_4, int param_5);
void __stdcall FUN_004ab0a0(void* menu);

// FUNCTION: 0x477ab0
void __stdcall FUN_00477ab0(Menu_00477ab0* menu)
{
    unsigned char idx0 = *(unsigned char*)(g_game + 0x2a42);
    Entry_00477ab0* entries = menu->holder->entries;
    char* playerInfo = g_game + 0x14b * idx0;
    int index = 0;

    if (menu->current == -1) {
        FUN_004d85a0(DAT_0051e65c);
        FUN_004d85a0(DAT_0051e660);
        DAT_0051e65c = 0;
        DAT_0051e660 = 0;
        return;
    }

    if (DAT_0051e668 != 0) {
        if (FUN_0049fd60(menu, "Missions"))
            goto BigButton;
        if (FUN_0049fd60(menu, "Start"))
            goto BigButton;
    }
    if (DAT_0051e668 != 0)
        goto PrevMenu;
    if (FUN_0049fd60(menu, "Campaign"))
        goto BigButton;
    if (FUN_0049fd60(menu, "Start"))
        goto BigButton;

PrevMenu:
    if (FUN_0049fd60(menu, "PrevMenu")) {
        FUN_0047f1a0("Previous", 0);
        *(unsigned char*)(g_game + 0x2bc0) = 3;
        FUN_00491c80(0x14);
        return;
    }
    if (FUN_0049fd60(menu, "Difficulty")) {
        FUN_0047f1a0("SmlButton", 0);
        int diff = *(int*)(g_game + 0x37eee);
        if (diff == 0) {
            *(int*)(g_game + 0x37eee) = 1;
            FUN_004ab0a0(menu);
            return;
        }
        if (diff == 1) {
            *(int*)(g_game + 0x37eee) = 2;
            FUN_004ab0a0(menu);
            return;
        }
        if (diff == 2) {
            *(int*)(g_game + 0x37eee) = 0;
            FUN_004ab0a0(menu);
            return;
        }
        goto End;
    }
    if (FUN_0049fd60(menu, "Side0") || FUN_0049fd60(menu, "Arm"))
        goto ArmSide;
    if (!FUN_0049fd60(menu, "Side1") && !FUN_0049fd60(menu, "Core"))
        goto End;

CoreSide:
    FUN_004a1110(g_game + 0x519, "Core", 1);
    FUN_004a1110(g_game + 0x519, "Side1", 1);
    FUN_0047f1a0("SideSelect2", 0);
    index = FUN_0049fdf0(entries, "Side1", 1);
    *(int*)((char*)entries + index * 0x15b + 0x1f) = 0x1f;
    *(int*)(g_game + 0x37ef2) = 1;
    *(unsigned char*)(*(int*)(playerInfo + 0x1b8a) + 0x95) = 1;
    *(unsigned char*)(*(int*)(playerInfo + 0x1cd5) + 0x95) = 0;
    if (DAT_00507b6c == 0) {
        playerInfo = g_game + 0x14b * *(unsigned char*)(g_game + 0x2a42);
        Holder_00477ab0* campaignHolder = *(Holder_00477ab0**)(g_game + 0x531);
        int side = *(unsigned char*)(*(int*)(playerInfo + 0x1b8a) + 0x95);
        if (DAT_0051e65c) {
            FUN_004d85a0(DAT_0051e65c);
            DAT_0051e65c = 0;
        }
        FUN_0047f1a0("smlbutton", 0);
        int count = FUN_00476a60(&DAT_0051e65c, side);
        FUN_004a32a0(g_game + 0x519, "Campaign", DAT_0051e65c, count, 0);
        index = FUN_0049fdf0(campaignHolder->entries, "Campaign", 2);
        FUN_004a2be0(g_game + 0x519, index);
        FUN_0049fa90(g_game + 0x519);
    }
    if (DAT_0051e668 != 0)
        goto MissionsCore;
    goto End;

ArmSide:
    FUN_004a1110(g_game + 0x519, "Arm", 1);
    FUN_004a1110(g_game + 0x519, "Side0", 1);
    FUN_0047f1a0("SideSelect", 0);
    index = FUN_0049fdf0(entries, "Side0", 1);
    *(int*)((char*)entries + index * 0x15b + 0x1f) = 0x1f;
    *(int*)(g_game + 0x37ef2) = 0;
    *(unsigned char*)(*(int*)(playerInfo + 0x1b8a) + 0x95) = 0;
    *(unsigned char*)(*(int*)(playerInfo + 0x1cd5) + 0x95) = 1;
    {
        playerInfo = g_game + 0x14b * *(unsigned char*)(g_game + 0x2a42);
        Holder_00477ab0* campaignHolder = *(Holder_00477ab0**)(g_game + 0x531);
        int side = *(unsigned char*)(*(int*)(playerInfo + 0x1b8a) + 0x95);
        if (DAT_0051e65c) {
            FUN_004d85a0(DAT_0051e65c);
            DAT_0051e65c = 0;
        }
        FUN_0047f1a0("smlbutton", 0);
        int count = FUN_00476a60(&DAT_0051e65c, side);
        FUN_004a32a0(g_game + 0x519, "Campaign", DAT_0051e65c, count, 0);
        index = FUN_0049fdf0(campaignHolder->entries, "Campaign", 2);
        FUN_004a2be0(g_game + 0x519, index);
        FUN_0049fa90(g_game + 0x519);
    }
    FUN_004a0570(menu, "Campaign", DAT_00507b6c == 0);
    if (DAT_0051e668 != 0)
        goto MissionsArm;
    goto End;

BigButton:
    index = 0;
    FUN_0047f1a0("bigButton", 0);
    if (!FUN_0041d6a0(0)) {
        FUN_004abd90(g_game + 0x519,
                     FUN_004c5740("Please insert the Campaign CD (Disc 2) and try again"),
                     200, 1, 1);
        FUN_004ab0a0(g_game + 0x519);
        return;
    }
    FUN_0041d4c0();
    FUN_0041da30();
    {
        char* name;
        if (DAT_00507b6c == 0) {
            Entry_00477ab0* e = FUN_0049ff90(entries, "Campaign");
            name = FUN_004b6af0(e->text, e->selected);
        } else if (*(unsigned char*)(*(int*)(playerInfo + 0x1b8a) + 0x95) != 0) {
            name = "Core Campaign";
        } else {
            name = "Arm Campaign";
        }
        ((Class_00435110*)*(void**)(g_game + 0x391e9))->FUN_00435110(name);
    }
    if (DAT_0051e668 != 0) {
        Entry_00477ab0* e = FUN_0049ff90(entries, "Missions");
        index = e->selected;
    }
    if (((Class_00435c00*)*(void**)(g_game + 0x391e9))->FUN_00435c00(index) != 0) {
        FUN_00491c80(0x14);
        *(unsigned char*)(*(int*)(g_game + 0x1b8a) + 0x96) = 0;
        *(unsigned char*)(*(int*)(g_game + 0x1cd5) + 0x96) = 1;
        FUN_00430f00();
        if (DAT_0051e668 != 0) {
            *(unsigned char*)(g_game + 0x2bc0) = 0x10;
            return;
        }
        *(unsigned char*)(g_game + 0x2bc0) = 0x0f;
        return;
    }
    goto End;

MissionsCore:
    FUN_0049ff90(entries, "Campaign");
    {
        int holder = *(int*)(g_game + 0x531);
        void* menuSub = g_game + 0x519;
        if (DAT_0051e660) {
            FUN_004d85a0(DAT_0051e660);
            DAT_0051e660 = 0;
        }
        Entry_00477ab0* e = FUN_0049ff90(*(Entry_00477ab0**)(holder + 4), "Campaign");
        char* text = FUN_004b6af0(e->text, e->selected);
        ((Class_00435110*)*(void**)(g_game + 0x391e9))->FUN_00435110(text);
        index = ((Class_00435760*)*(void**)(g_game + 0x391e9))->FUN_00435760(&DAT_0051e660);
        FUN_004a32a0(menuSub, "Missions", DAT_0051e660, index, 0);
        index = FUN_0049fdf0(*(Entry_00477ab0**)(holder + 4), "Missions", 2);
        FUN_004a2be0(g_game + 0x519, index);
        FUN_0049fa90(g_game + 0x519);
    }
    FUN_004ab0a0(menu);
    return;

MissionsArm:
    FUN_0049ff90(entries, "Campaign");
    {
        int holder = *(int*)(g_game + 0x531);
        void* menuSub = g_game + 0x519;
        if (DAT_0051e660) {
            FUN_004d85a0(DAT_0051e660);
            DAT_0051e660 = 0;
        }
        Entry_00477ab0* e = FUN_0049ff90(*(Entry_00477ab0**)(holder + 4), "Campaign");
        char* text = FUN_004b6af0(e->text, e->selected);
        ((Class_00435110*)*(void**)(g_game + 0x391e9))->FUN_00435110(text);
        index = ((Class_00435760*)*(void**)(g_game + 0x391e9))->FUN_00435760(&DAT_0051e660);
        FUN_004a32a0(menuSub, "Missions", DAT_0051e660, index, 0);
        index = FUN_0049fdf0(*(Entry_00477ab0**)(holder + 4), "Missions", 2);
        FUN_004a2be0(g_game + 0x519, index);
        FUN_0049fa90(g_game + 0x519);
    }
    FUN_004ab0a0(menu);
    return;

End:
    FUN_004ab0a0(menu);
}

