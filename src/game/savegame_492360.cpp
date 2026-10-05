// Decompiled by deepseek-v4.1-flash, finished by deepseek-v4.1-flash. Names are provisional.
// Load game screen click handler (sibling of 0x492df0, save game screen).
//
// MATCHED (1964 of 1964 bytes).
//
// The last fault was a one-byte register-allocation difference that showed up
// as jump targets one byte off: the store `g_game->p38d6b = 0;` after freeing
// the save object used ecx for the g_game load where the original used the
// 5-byte `mov eax, [g_game]`. The original source called the already-matched
// helper 0x432590 (FUN_00432590, "if (obj) { obj->CloseBank(); operator
// delete(obj); }") and /Ob2 inlined it; reproducing that call through an
// inline helper, with the field test outside it, gives the original's
// registers at both delete sites. A bare local pointer could not.
//
// The game struct needs #pragma pack(1); missing the 4-byte pad between the
// pointer at +0x391e9 and +0x391f1 shifts every later g_game field by 4.
// The flags at +0x2a44 are a bitfield union: reading one gives the original's
// `mov cl,[..]; shr cl,2; test cl,1` (a plain `(v >> 2) & 1` folds to
// `test byte ptr [..],4`), and setting one still gives `or byte ptr`.
#include <stdio.h>
#include <string.h>

#pragma pack(push, 1)
struct Entry_00492360 {                  // 0x15b bytes
    char unknown_0[0xb6];
    char text[4];                        // +0xb6
    short field_ba;                      // +0xba
    char unknown_bc[0x15b - 0xbc];
};

struct Layer_00492360 {
    int unknown_0;
    Entry_00492360* entries;             // +0x04
};

struct Gadget_00492360 {
    char unknown_0[0x18];
    Layer_00492360* layer;               // +0x18
    char unknown_1c[0x60 - 0x1c];
    int field_60;                        // +0x60
};

struct Obj_00492360 {
    char unknown_0[0x108];
    int field_108;                       // +0x108
    int field_10c;                       // +0x10c
    int field_110;                       // +0x110
    int field_114;                       // +0x114
    int field_118;                       // +0x118
    char text_11c[1];                    // +0x11c
};

struct Game {
    char unknown_0[0x519];
    char message[0x1b8a - 0x519];        // +0x519
    void* p1b8a;                         // +0x1b8a
    char unknown_1b8e[0x1cd5 - 0x1b8e];
    void* p1cd5;                         // +0x1cd5
    char unknown_1cd9[0x29a0 - 0x1cd9];
    Obj_00492360* p29a0;                 // +0x29a0
    char unknown_29a4[0x2a3c - 0x29a4];
    short field_2a3c;                    // +0x2a3c
    char unknown_2a3e[0x2a44 - 0x2a3e];
    union Flags_2a44 {
        unsigned short value;
        struct {
            unsigned short b0 : 1;
            unsigned short b1 : 1;
            unsigned short b2 : 1;
            unsigned short b3 : 1;
            unsigned short rest : 12;
        };
    } flags_2a44;                        // +0x2a44
    char unknown_2a46[0x2cbe - 0x2a46];
    unsigned char field_2cbe;            // +0x2cbe
    char unknown_2cbf[0x148cf - 0x2cbf];
    void* p148cf;                        // +0x148cf
    char unknown_148d3[0x37eee - 0x148d3];
    int field_37eee;                     // +0x37eee
    int field_37ef2;                     // +0x37ef2
    char unknown_37ef6[0x38c6b - 0x37ef6];
    char saveName[0x38d6b - 0x38c6b];    // +0x38c6b
    void* p38d6b;                        // +0x38d6b
    char unknown_38d6f[0x391cf - 0x38d6f];
    char buf391cf[0x391e9 - 0x391cf];    // +0x391cf
    void* p391e9;                        // +0x391e9
    char unknown_391ed[0x391f1 - 0x391ed];
    int field_391f1;                     // +0x391f1
    void (*field_391f5)();               // +0x391f5
    char unknown_391f9[0x3923b - 0x391f9];
    union Flags_3923b {
        unsigned short value;
        struct {
            unsigned short b0 : 1;
            unsigned short b1 : 1;
            unsigned short b2 : 1;
            unsigned short b3 : 1;
            unsigned short rest : 12;
        };
    } flags_3923b;                       // +0x3923b
};
#pragma pack(pop)

struct Mission { int FUN_00435100(); void LoadCampaign(char* name); void* LoadMissionByName(char* name); };
struct HapiBank { void OpenAccount(char* name); void CloseBank(); int GetIntegerItem(char* name, int def); char* GetStringItem(char* name, int def); int HasItem(char* name); };
extern Game* g_game;
extern char* DAT_005091c8;
extern char* DAT_0051f2e0;
extern char* DAT_0051f2e4;
extern char* DAT_0051f2e8;
extern char* DAT_0051f2ec;

void __cdecl operator delete(void* p);
void __stdcall RegisterDataArchives();
char __stdcall FindGameCdDrive(int flag);
void __stdcall FUN_0041da30();
void __stdcall BlankScreen();
void __stdcall SetFrontendState(int code, int line, char* file);
void* __stdcall FUN_00432520(char* path);
void __stdcall FUN_00432590(void* handle);
void* __stdcall FUN_004325b0(char* path);
void __stdcall FUN_00434ab0(int value);
void __stdcall PlaySoundByName(char* name, int param);
void __stdcall FUN_00491b60();
void __stdcall FUN_00491d70(int flag);
void FUN_00496bb0();
void __stdcall FUN_0049fa70(void* menu);
int __stdcall IsCurrentGadgetNamed(Gadget_00492360* gadget, char* name);
Entry_00492360* __stdcall FindGadgetChecked(Entry_00492360* entries, char* name);
void __stdcall FUN_004ab0a0(Gadget_00492360* menu);
void __stdcall FUN_004ab400(void* menu, void* data);
void __stdcall OpenMessageBox(void* menu, char* message, int a, int b, int c);
void __stdcall SetCloseHandler(void (__cdecl *callback)(int), int param);
char* __stdcall SkipTextLines(char* text, int n);
void __cdecl LeaveNetGameCallback(int param);
char* __stdcall Translate(char* text);
void __cdecl FUN_004d85a0(void* p);

__inline void DeleteSave_00492360(HapiBank* obj)
{
    if (obj) {
        obj->CloseBank();
        operator delete(obj);
    }
}

// FUNCTION: 0x492360
void __stdcall LoadGameScreenHandler(Gadget_00492360* gadget)
{
    Entry_00492360* entries = gadget->layer->entries;
    char buf[0x104];

    if (gadget->field_60 == -1)
        return;

    if (IsCurrentGadgetNamed(gadget, "CANCEL")) {
        if (g_game->flags_2a44.b2)
            FUN_0049fa70(g_game->message);
        if (DAT_0051f2e0)
            FUN_004d85a0(DAT_0051f2e0);
        if (DAT_0051f2e4)
            FUN_004d85a0(DAT_0051f2e4);
        if (DAT_0051f2e8)
            FUN_004d85a0(DAT_0051f2e8);
        DAT_0051f2e8 = 0;
        DAT_0051f2e4 = 0;
        DAT_0051f2e0 = 0;
        if (DAT_0051f2ec)
            FUN_004d85a0(DAT_0051f2ec);
        DAT_0051f2ec = 0;
        PlaySoundByName("Previous", 0);
        return;
    }
    if (!IsCurrentGadgetNamed(gadget, "LOAD") && !IsCurrentGadgetNamed(gadget, "GAMES")) {
        if (gadget->field_60 != -1)
            FUN_004ab0a0(gadget);
        return;
    }
    Entry_00492360* e = FindGadgetChecked(entries, "GAMES");
    sprintf(buf, "%s\\%s", DAT_005091c8, SkipTextLines(DAT_0051f2e0, e->field_ba));
    void* save = FUN_00432520(buf);
    if (save != 0) {
        int type = ((HapiBank*)save)->GetIntegerItem("Gametype", 0);
        FUN_00432590(save);
        switch (type) {
        case 1:
            if (!FindGameCdDrive(0)) {
                OpenMessageBox(g_game->message,
                    Translate("Please insert the Campaign CD (Disc 2) and try again"),
                    200, 1, 1);
                FUN_004ab0a0((Gadget_00492360*)g_game->message);
                return;
            }
            break;
        case 2:
            if (!FindGameCdDrive(1)) {
                OpenMessageBox(g_game->message,
                    Translate("Please insert the Multiplayer CD (Disc 1) and try again"),
                    200, 1, 1);
                FUN_004ab0a0((Gadget_00492360*)g_game->message);
                return;
            }
            break;
        default:
            goto invalid;
        }
        RegisterDataArchives();
        BlankScreen();
        if (g_game->field_2cbe != 20) {
            g_game->field_2cbe = 20;
            FUN_004ab400(g_game->message, g_game->p148cf);
        }
        if (g_game->flags_2a44.b2)
            FUN_0049fa70(g_game->message);
        PlaySoundByName("SMLBUTTON", 0);
        e = FindGadgetChecked(entries, "GAMES");
        sprintf(g_game->saveName, "%s\\%s", DAT_005091c8,
                SkipTextLines(DAT_0051f2e0, e->field_ba));
        if (g_game->flags_2a44.b2)
            FUN_00491b60();
        g_game->flags_3923b.b3 = 1;

        g_game->p38d6b = FUN_004325b0(g_game->saveName);
        if (g_game->p38d6b == 0)
            goto invalid;
        ((HapiBank*)g_game->p38d6b)->OpenAccount("summary");
        FUN_00434ab0(((HapiBank*)g_game->p38d6b)->GetIntegerItem("Gametype", 0));
        char* campaign = ((HapiBank*)g_game->p38d6b)->GetStringItem("Campaign", 0);
        if (campaign != 0)
            ((Mission*)g_game->p391e9)->LoadCampaign(campaign);
        g_game->field_37ef2 = ((HapiBank*)g_game->p38d6b)->GetIntegerItem("Side", 0);
        g_game->field_37eee = ((HapiBank*)g_game->p38d6b)->GetIntegerItem("Difficulty", 0);
        if (((Mission*)g_game->p391e9)->FUN_00435100() == 1) {
            if (g_game->field_37ef2 == 0) {
                *(unsigned char*)((char*)g_game->p1b8a + 0x95) = 0;
                *(unsigned char*)((char*)g_game->p1cd5 + 0x95) = 1;
            } else {
                *(unsigned char*)((char*)g_game->p1b8a + 0x95) = 1;
                *(unsigned char*)((char*)g_game->p1cd5 + 0x95) = 0;
            }
        }
        char* mission = ((HapiBank*)g_game->p38d6b)->GetStringItem("Mission", 0);
        if (mission == 0)
            goto invalid;
        if (strlen(mission) == 0)
            goto invalid;
        if (((Mission*)g_game->p391e9)->LoadMissionByName(mission) == 0)
            goto invalid;
        strcpy((char*)g_game->p29a0 + 0x11c, mission);
        char* thumbs = ((HapiBank*)g_game->p38d6b)->GetStringItem("Thumbs", 0);
        strncpy(g_game->buf391cf, thumbs, 0x19);
        if (strlen(g_game->buf391cf) != 0x19)
            FUN_0041da30();
        if (((Mission*)g_game->p391e9)->FUN_00435100() == 2) {
            ((HapiBank*)g_game->p38d6b)->OpenAccount("summary");
            g_game->field_2a3c =
                (short)((HapiBank*)g_game->p38d6b)->GetIntegerItem("Players", 0);
            g_game->p29a0->field_108 =
                ((HapiBank*)g_game->p38d6b)->GetIntegerItem("CommanderDeath", 1);
            g_game->p29a0->field_118 =
                ((HapiBank*)g_game->p38d6b)->GetIntegerItem("Location", 1);
            g_game->p29a0->field_10c =
                ((HapiBank*)g_game->p38d6b)->GetIntegerItem("Mapping", 1);
            g_game->p29a0->field_110 =
                ((HapiBank*)g_game->p38d6b)->GetIntegerItem("LineOfSight", 1);
            g_game->p29a0->field_114 =
                ((HapiBank*)g_game->p38d6b)->GetIntegerItem("LineOfSightType", 1);
        }
        g_game->flags_2a44.b2 = 1;
        g_game->field_391f1 = 2;
        g_game->field_391f5 = FUN_00496bb0;
        SetCloseHandler(LeaveNetGameCallback, 0);
        if (DAT_0051f2e0)
            FUN_004d85a0(DAT_0051f2e0);
        if (DAT_0051f2e4)
            FUN_004d85a0(DAT_0051f2e4);
        if (DAT_0051f2e8)
            FUN_004d85a0(DAT_0051f2e8);
        DAT_0051f2e8 = 0;
        DAT_0051f2e4 = 0;
        DAT_0051f2e0 = 0;
        if (DAT_0051f2ec)
            FUN_004d85a0(DAT_0051f2ec);
        DAT_0051f2ec = 0;
        FUN_00491d70(1);
        if (((Mission*)g_game->p391e9)->FUN_00435100() == 1 &&
            ((HapiBank*)g_game->p38d6b)->HasItem("BetweenMissions")) {
            g_game->flags_2a44.b3 = 1;
            if (g_game->p38d6b) {
                DeleteSave_00492360((HapiBank*)g_game->p38d6b);
                g_game->p38d6b = 0;
            }
            g_game->flags_2a44.b2 = 0;
            g_game->field_391f1 = 2;
            g_game->field_391f5 = FUN_00496bb0;
            SetCloseHandler(LeaveNetGameCallback, 0);
            SetFrontendState(0xe, 0x48c, "c:\\cavedog\\wargame\\wargame.cpp");
        }
        BlankScreen();
        return;
    }
invalid:
    if (g_game->p38d6b) {
        DeleteSave_00492360((HapiBank*)g_game->p38d6b);
    }
    g_game->p38d6b = 0;
    OpenMessageBox(gadget, Translate("Invalid savegame file"), 0x140, 1, 1);
    FUN_004ab0a0(gadget);
}
