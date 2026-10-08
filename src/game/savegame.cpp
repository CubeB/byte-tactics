// Decompiled by deepseek-v4.1, deepseek-v4.1-flash, Sonnet, Sonnet 5.5, Haiku, space-bunny-free and GPT-6. Names are provisional.
// The saved game screens: the info panel, the load and save click handlers,
// the saved-game list and the two screen openers.

#include <stdio.h>
#include <string.h>
#include <time.h>

class Mission;

// Pack 1: the 4-byte pad after the pointer at +0x391e9 would shift later fields.
#pragma pack(push, 1)
struct Entry_00491ec0 {
    char unknown_0[0xba];
    short field_ba;                    // +0xba selected game index
    char unknown_bc[0xc2 - 0xbc];
    char* field_c2;                    // +0xc2 text/gaf pointer
    char unknown_c6[0x15b - 0xc6];
};

struct Layer_00491ec0 {
    int unknown_0;
    Entry_00491ec0* entries;           // +0x04
};

struct Menu_00491ec0 {
    char unknown_0[0x18];
    Layer_00491ec0* layer;             // +0x18
};

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
    int selected;                        // +0x60
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

struct Entry_00492df0 {                  // 0x15b bytes
    char unknown_0[0xb6];
    char text[4];                        // +0xb6
    short field_ba;                      // +0xba
    char unknown_bc[0x15b - 0xbc];
};

struct Layer_00492df0 {
    int unknown_0;
    Entry_00492df0* entries;             // +0x04
};

struct Gadget_00492df0 {
    char unknown_0[0x18];
    Layer_00492df0* layer;               // +0x18
    char unknown_1c[0x60 - 0x1c];
    int selected;                        // +0x60
};

struct Entry_00493060 {                // 0x15b bytes
    char unknown_0[0x1b];
    int attribs;                       // +0x1b
    char unknown_1f[0xce - 0x1f];
    void* handler;                     // +0xce
    char unknown_d2[0x15b - 0xd2];
};

struct Layer_00493060 {
    int unknown_0;
    Entry_00493060* entries;           // +0x04
    void (__stdcall* handler)(Gadget_00492df0*); // +0x08
    void* data;                        // +0x0c
};

struct Menu_00493060 {
    char unknown_0[0x18];
    Layer_00493060* layer;             // +0x18
};

struct Entry_004931d0 {
    char unknown_0[0xce];
    void* field_ce;                    // +0xce
};

struct Gadget_004931d0 {
    char unknown_0[4];
    Entry_004931d0* info;              // +0x4
    void (__stdcall* handler)(Gadget_00492360*); // +0x8
    void* context;                     // +0xc
};

// The DELETE block's path buffer and the count it rebuilds the "GAMES" list
// with, plus the "GAMES" entry the GAMES/LOAD/GAMENAME block looks up and
// never reads. One local, so that the buffer's address is the aggregate's.
struct Save_00492df0 {
    int games;                           // +0, stored, never read
    int count;                           // +4
    char path[0x100];                    // +8
};

// One view of the game state. The menu pointer at +0x519 is read as a Menu
// (0x491ec0, 0x493060), as a raw byte pointer (0x492b10) and as the message
// buffer (0x492360, 0x492df0); the flags at +0x2a44 are read both as the
// b0..b3 bitfield union (0x492360) and as bits0/bit2/bits3 (0x492df0).
struct Game {
    char unknown_0[0x519];
    union {
        char message[0x1b8a - 0x519];    // +0x519
        char menu[1];
        Menu_00491ec0 menu_00491ec0;
        Menu_00493060 menu_00493060;
    };
    void* p1b8a;                         // +0x1b8a
    char unknown_1b8e[0x1cd5 - 0x1b8e];
    void* p1cd5;                         // +0x1cd5
    char unknown_1cd9[0x29a0 - 0x1cd9];
    Obj_00492360* p29a0;                 // +0x29a0
    char unknown_29a4[0x2a3c - 0x29a4];
    short numPlayers;                    // +0x2a3c
    char unknown_2a3e[0x2a44 - 0x2a3e];
    // Bitfield union: reads give shr/test instead of a folded byte test.
    union Flags_2a44 {
        unsigned short value;
        struct {
            unsigned short b0 : 1;
            unsigned short b1 : 1;
            unsigned short b2 : 1;
            unsigned short b3 : 1;
            unsigned short rest : 12;
        };
        struct {
            unsigned short bits0_2a44 : 2;
            unsigned short bit2_2a44 : 1;
            unsigned short bits3_2a44 : 13;
        };
    } flags_2a44;                        // +0x2a44
    char unknown_2a46[0x2cbe - 0x2a46];
    unsigned char cursorMode;            // +0x2cbe
    char unknown_2cbf[0x148cf - 0x2cbf];
    void* p148cf;                        // +0x148cf
    char unknown_148d3[0x37eee - 0x148d3];
    int difficulty;                      // +0x37eee
    int side;                            // +0x37ef2
    char unknown_37ef6[0x38a51 - 0x37ef6];
    unsigned short flag_38a51 : 1;       // +0x38a51, bit 0
    unsigned short bits_38a51 : 15;
    char unknown_38a53[0x38c6b - 0x38a53];
    char saveName[0x38d6b - 0x38c6b];    // +0x38c6b
    void* p38d6b;                        // +0x38d6b
    char unknown_38d6f[0x391cf - 0x38d6f];
    char buf391cf[0x391e9 - 0x391cf];    // +0x391cf
    Mission* mapInfo;                    // +0x391e9
    char unknown_391ed[0x391f1 - 0x391ed];
    int mode;                            // +0x391f1
    void (*handler)();                   // +0x391f5
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

#include "../map/mission.h"

#include "../util/hapi_bank.h"

extern Game* g_game;
extern char* DAT_005091c8;
extern char DAT_005119b8[];
extern const char g_invalidSavegameText[]; // "Invalid savegame file"
extern char g_savegameRadarFrame[];
extern char* g_savegameFileNames;
extern char* g_savegameDescs;
extern char* g_savegameSideList;
extern char* g_savegameRadarPreview;

void __stdcall MarkChanged(Menu_00491ec0* menu);
Entry_00491ec0* __stdcall FindGadgetChecked(Entry_00491ec0* entries, char* name);
Entry_00492360* __stdcall FindGadgetChecked(Entry_00492360* entries, char* name);
Entry_00492df0* __stdcall FindGadgetChecked(Entry_00492df0* entries, char* name);
Entry_00493060* __stdcall FindGadgetChecked(Entry_00493060* entries, char* name);
Entry_004931d0* __stdcall FindGadgetChecked(void* entries, char* name);
int __stdcall FindGadgetIndex(Entry_00491ec0* entries, char* name, int type);
int __stdcall FindGadgetIndex(Entry_00492df0* entries, char* name, int type);
int __stdcall FindGadgetIndex(Entry_00493060* entries, char* name, int type);
void __stdcall SetGadgetText(Menu_00491ec0* menu, int index, char* text);
Entry_00491ec0* __stdcall FindGadgetChecked_E(Entry_00491ec0* entries, char* name);
void __stdcall SetGadgetActiveByName(void* menu, char* name, int value);
void __stdcall SetTranslatedTextByName(void* menu, char* name, char* text, int param_4);
char* __stdcall SkipTextLines(char* text, int n);
void __stdcall FrameFromSurface(void* dst, void* src);
void __stdcall FreeSurface(void* image);
void* __stdcall LoadSurface(HapiBank* obj);
HapiBank* __stdcall OpenSummaryBank(char* name);
void __stdcall FreeSummaryBank(void* handle);
void* __stdcall OpenHapiBank(char* path);
void __stdcall SetMissionType(int value);
void __stdcall PlaySoundByName(char* name, int param);
void __stdcall ShutdownIngameSystems();
void __stdcall PopUntilNamedLayout(int flag);
void MenuFrame();
void __stdcall DisableKeyCommands(void* menu);
void __stdcall EnableKeyCommands(void* menu);
void __stdcall SetKeyboardInput(void* menu, int value);
void __stdcall ConfigureListBoxByName(void* menu, char* name, void* text, int value, int flag);
void __stdcall BeginTextEdit(void* menu, int index);
void __stdcall RenderLayer(void* menu, int value);
void __stdcall LoadPictureCached(const char* name, int a, int b, int c);
void __stdcall MakeDirectoryPath(char* path);
void __stdcall CloseTopScreen(char* sub);
void __stdcall RemoveFile(char* path);
char* __stdcall BuildDataPath(char* buf, char* dir, char* name, char* ext);
char* __stdcall BuildSideList();
int __stdcall SaveGameFile(char* path, char* description, int param_3);
int __stdcall CountDirectoryEntries(const char* path, int flag);
void __stdcall ScanDirectory(char* path, char* list, char* sizes, int mode, int flag, int what);
void __cdecl InitMissionStatus();
void OrLabelAttribs();
void __stdcall BlankScreen();
void __stdcall RegisterDataArchives();
void __stdcall SetFrontendState(int code, int line, char* file);
char __stdcall FindGameCdDrive(int flag);
void __stdcall SetCloseHandler(void (__cdecl *callback)(int), int param);
void __cdecl LeaveNetGameCallback(int param);
void __cdecl GameFreeThunk(void* p);
void* __cdecl GameAllocIgnoreTag(const char* name, unsigned int size);
void __cdecl operator delete(void* p);
void* __cdecl operator new(unsigned int size);
Layer_00493060* __stdcall LoadGuiLayer(Menu_00493060* menu, const char* name, int flags);
Gadget_004931d0* __stdcall LoadGuiLayer(char* sub, const char* name, int flags);
int __stdcall IsCurrentGadgetNamed(Gadget_00492360* gadget, char* name);
int __stdcall IsCurrentGadgetNamed(Gadget_00492df0* gadget, char* name);
void __stdcall ClearSelectedGadget(Gadget_00492360* menu);
void __stdcall ClearSelectedGadget(Gadget_00492df0* menu);
void __stdcall SetCursorAnimation(void* menu, void* data);
void __stdcall SetDescListCleanupFlag(Gadget_00492df0* menu, int flag);
const char* __stdcall Translate(const char* key);
void __stdcall OpenMessageBox(void* menu, const char* message, int a, int b, int c);
void __stdcall LoadGameScreenHandler(Gadget_00492360* gadget);
void __stdcall SaveGameScreenHandler(Gadget_00492df0* gadget);
void __stdcall SavedGameSelectHandler(int a, int b);
char* __stdcall ListSavedGames(int* count);
void ShowSavedGameInfo();

// Stays an inline helper, with the field test outside it: gives the original's
// registers at both delete sites.
__inline void DeleteSave_00492360(HapiBank* obj)
{
    if (obj) {
        obj->CloseBank();
        operator delete(obj);
    }
}

// FUNCTION: 0x491ec0
void __stdcall ShowSavedGameInfo()
{
    Menu_00491ec0* menu = &g_game->menu_00491ec0;
    Layer_00491ec0* layer = g_game->menu_00491ec0.layer;
    Entry_00491ec0* entries = layer->entries;
    Entry_00491ec0* games = FindGadgetChecked(entries, "GAMES");
    if (games == 0)
        return;

    int index = FindGadgetIndex(entries, "GAMENAME", 3);
    char path[0x100];
    struct Buf { int gametype; char name[0x34]; char* diffs[3]; } b;
#define gametype b.gametype
#define name b.name
#define diffs b.diffs

    char* desc;
    if (games->field_ba > -1
        && (desc = SkipTextLines(g_savegameDescs, games->field_ba)) != 0
        && strlen(desc) != 0) {
        SetGadgetText(menu, index, desc);
        char* fname = SkipTextLines(g_savegameFileNames, games->field_ba);
        sprintf(path, "%s\\%s", DAT_005091c8, fname);
        HapiBank* file = OpenSummaryBank(path);
        if (file != 0) {
            file->OpenNamedBox("Radar Image");
            Entry_00491ec0* radar = FindGadgetChecked_E(menu->layer->entries, "RADAR");
            if (g_savegameRadarPreview != 0)
                FreeSurface(g_savegameRadarPreview);
            g_savegameRadarPreview = (char*)LoadSurface(file);
            if (g_savegameRadarPreview != 0) {
                FrameFromSurface(g_savegameRadarFrame, g_savegameRadarPreview);
                radar->field_c2 = g_savegameRadarFrame;
            }
            SetGadgetActiveByName(menu, "RADAR", g_savegameRadarPreview != 0);

            int players = file->GetIntegerItem("Players", 0);
            gametype = file->GetIntegerItem("Gametype", 0);
            if (players != 0) {
                if (gametype == 1)
                    strcpy(name, "Single");
                else
                    sprintf(name, "Skirmish (%d players)", players);
            } else {
                strcpy(name, "???");
            }
            SetTranslatedTextByName(menu, "GAMETYPE", name, 0);

            if (gametype == 1) {
                char* campaign = file->GetStringItem("Campaign", 0);
                if (campaign != 0) {
                    strcpy(name, campaign);
                    SetTranslatedTextByName(menu, "CAMPAIGN", name, 0);
                    SetGadgetActiveByName(menu, "CAMPTEXT", 1);
                    SetGadgetActiveByName(menu, "CAMPAIGN", 1);
                }
                char* mission = file->GetStringItem("Mission", 0);
                if (mission != 0) {
                    strcpy(name, mission);
                    SetTranslatedTextByName(menu, "MISSION", name, 0);
                }
            } else {
                SetGadgetActiveByName(menu, "CAMPTEXT", 0);
                SetGadgetActiveByName(menu, "CAMPAIGN", 0);
                char* mission = file->GetStringItem("Map", 0);
                if (mission != 0) {
                    strcpy(name, mission);
                    SetTranslatedTextByName(menu, "MISSION", name, 0);
                }
            }

            int time = file->GetIntegerItem("Game Time", 0);
            sprintf(name, "%02d:%02d:%02d", time / 108000, time / 1800 % 60,
                    time / 30 % 60);
            SetTranslatedTextByName(menu, "TIME", name, 0);

            if (g_savegameSideList != 0) {
                int side = file->GetIntegerItem("Side", 0);
                strcpy(name, SkipTextLines(g_savegameSideList, side));
            } else {
                strcpy(name, "???");
            }
            SetTranslatedTextByName(menu, "SIDE", name, 0);

            diffs[0] = "Easy";
            diffs[1] = "Medium";
            diffs[2] = "Hard";
            sprintf(name, "%s",
                    diffs[file->GetIntegerItem("Difficulty", 0)]);
            SetTranslatedTextByName(menu, "DIFF", name, 0);
            FreeSummaryBank(file);
            goto done;
        }
    }
    {
        SetGadgetText(menu, index, DAT_005119b8);
        SetTranslatedTextByName(menu, "SIDE", DAT_005119b8, 0);
        SetTranslatedTextByName(menu, "DIFF", DAT_005119b8, 0);
        SetTranslatedTextByName(menu, "MISSION", DAT_005119b8, 0);
        SetTranslatedTextByName(menu, "CAMPAIGN", DAT_005119b8, 0);
        SetTranslatedTextByName(menu, "GAMETYPE", DAT_005119b8, 0);
        SetTranslatedTextByName(menu, "TIME", DAT_005119b8, 0);
    }
done:
    MarkChanged(&g_game->menu_00491ec0);
#undef gametype
#undef name
#undef diffs
}

// FUNCTION: 0x492330
void __stdcall ShowInvalidSavegameDialog(void* param_1)
{
    OpenMessageBox(param_1, Translate(g_invalidSavegameText), 0x140, 1, 1);
    ClearSelectedGadget((Gadget_00492360*)param_1);
}

// Load game screen click handler (sibling of 0x492df0, save game screen).
// FUNCTION: 0x492360
void __stdcall LoadGameScreenHandler(Gadget_00492360* gadget)
{
    Entry_00492360* entries = gadget->layer->entries;
    char buf[0x104];

    if (gadget->selected == -1)
        return;

    if (IsCurrentGadgetNamed(gadget, "CANCEL")) {
        if (g_game->flags_2a44.b2)
            DisableKeyCommands(g_game->message);
        if (g_savegameFileNames)
            GameFreeThunk(g_savegameFileNames);
        if (g_savegameDescs)
            GameFreeThunk(g_savegameDescs);
        if (g_savegameSideList)
            GameFreeThunk(g_savegameSideList);
        g_savegameSideList = 0;
        g_savegameDescs = 0;
        g_savegameFileNames = 0;
        if (g_savegameRadarPreview)
            GameFreeThunk(g_savegameRadarPreview);
        g_savegameRadarPreview = 0;
        PlaySoundByName("Previous", 0);
        return;
    }
    if (!IsCurrentGadgetNamed(gadget, "LOAD") && !IsCurrentGadgetNamed(gadget, "GAMES")) {
        if (gadget->selected != -1)
            ClearSelectedGadget(gadget);
        return;
    }
    Entry_00492360* e = FindGadgetChecked(entries, "GAMES");
    sprintf(buf, "%s\\%s", DAT_005091c8, SkipTextLines(g_savegameFileNames, e->field_ba));
    void* save = OpenSummaryBank(buf);
    if (save != 0) {
        int type = ((HapiBank*)save)->GetIntegerItem("Gametype", 0);
        FreeSummaryBank(save);
        switch (type) {
        case 1:
            if (!FindGameCdDrive(0)) {
                OpenMessageBox(g_game->message,
                    Translate("Please insert the Campaign CD (Disc 2) and try again"),
                    200, 1, 1);
                ClearSelectedGadget((Gadget_00492360*)g_game->message);
                return;
            }
            break;
        case 2:
            if (!FindGameCdDrive(1)) {
                OpenMessageBox(g_game->message,
                    Translate("Please insert the Multiplayer CD (Disc 1) and try again"),
                    200, 1, 1);
                ClearSelectedGadget((Gadget_00492360*)g_game->message);
                return;
            }
            break;
        default:
            goto invalid;
        }
        RegisterDataArchives();
        BlankScreen();
        if (g_game->cursorMode != 20) {
            g_game->cursorMode = 20;
            SetCursorAnimation(g_game->message, g_game->p148cf);
        }
        if (g_game->flags_2a44.b2)
            DisableKeyCommands(g_game->message);
        PlaySoundByName("SMLBUTTON", 0);
        e = FindGadgetChecked(entries, "GAMES");
        sprintf(g_game->saveName, "%s\\%s", DAT_005091c8,
                SkipTextLines(g_savegameFileNames, e->field_ba));
        if (g_game->flags_2a44.b2)
            ShutdownIngameSystems();
        g_game->flags_3923b.b3 = 1;

        g_game->p38d6b = OpenHapiBank(g_game->saveName);
        if (g_game->p38d6b == 0)
            goto invalid;
        ((HapiBank*)g_game->p38d6b)->OpenAccount("summary");
        SetMissionType(((HapiBank*)g_game->p38d6b)->GetIntegerItem("Gametype", 0));
        char* campaign = ((HapiBank*)g_game->p38d6b)->GetStringItem("Campaign", 0);
        if (campaign != 0)
            g_game->mapInfo->LoadCampaign(campaign);
        g_game->side = ((HapiBank*)g_game->p38d6b)->GetIntegerItem("Side", 0);
        g_game->difficulty = ((HapiBank*)g_game->p38d6b)->GetIntegerItem("Difficulty", 0);
        if (g_game->mapInfo->GetGameType() == 1) {
            if (g_game->side == 0) {
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
        if (g_game->mapInfo->LoadMissionByName(mission) == 0)
            goto invalid;
        strcpy((char*)g_game->p29a0 + 0x11c, mission);
        char* thumbs = ((HapiBank*)g_game->p38d6b)->GetStringItem("Thumbs", 0);
        strncpy(g_game->buf391cf, thumbs, 0x19);
        if (strlen(g_game->buf391cf) != 0x19)
            InitMissionStatus();
        if (g_game->mapInfo->GetGameType() == 2) {
            ((HapiBank*)g_game->p38d6b)->OpenAccount("summary");
            g_game->numPlayers =
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
        g_game->mode = 2;
        g_game->handler = MenuFrame;
        SetCloseHandler(LeaveNetGameCallback, 0);
        if (g_savegameFileNames)
            GameFreeThunk(g_savegameFileNames);
        if (g_savegameDescs)
            GameFreeThunk(g_savegameDescs);
        if (g_savegameSideList)
            GameFreeThunk(g_savegameSideList);
        g_savegameSideList = 0;
        g_savegameDescs = 0;
        g_savegameFileNames = 0;
        if (g_savegameRadarPreview)
            GameFreeThunk(g_savegameRadarPreview);
        g_savegameRadarPreview = 0;
        PopUntilNamedLayout(1);
        if (g_game->mapInfo->GetGameType() == 1 &&
            ((HapiBank*)g_game->p38d6b)->HasItem("BetweenMissions")) {
            g_game->flags_2a44.b3 = 1;
            if (g_game->p38d6b) {
                DeleteSave_00492360((HapiBank*)g_game->p38d6b);
                g_game->p38d6b = 0;
            }
            g_game->flags_2a44.b2 = 0;
            g_game->mode = 2;
            g_game->handler = MenuFrame;
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
    ClearSelectedGadget(gadget);
}

// Lists the saved games: builds the search pattern for the "SAV" files in the
// save directory, asks how many there are, and fills two tables: the file
// names (0x100 bytes each) and, packed one after another, each save's
// "Description" line. A file without a description is dropped by shifting the
// remaining names down. Publishes the descriptions in the "GAMES" menu and
// returns the names table (0 when there are no saves); the count goes back
// through the parameter.
// FUNCTION: 0x492b10
char* __stdcall ListSavedGames(int* count)
{
    char buf[0x100];
    int found;
    char* copy;
    int i;
    char* dp;

    BuildDataPath(buf, DAT_005091c8, "*", "SAV");
    *count = CountDirectoryEntries(buf, 0);
    if (*count == 0) {
        ConfigureListBoxByName(g_game->menu, "GAMES", DAT_005119b8, 0, 0);
        return 0;
    }
    g_savegameFileNames = (char*)GameAllocIgnoreTag("SAVEGAME NAMES", *count << 8);
    g_savegameDescs = (char*)GameAllocIgnoreTag("SAVEGAME DESCS", *count << 6);
    memset(g_savegameDescs, 0, *count << 6);
    memset(g_savegameFileNames, 0, *count << 8);
    ScanDirectory(buf, g_savegameFileNames, 0, 0, 0, 1);
    dp = g_savegameDescs;
    found = 0;
    copy = (char*)GameAllocIgnoreTag("SAVEGAME2", *count << 8);
    memcpy(copy, g_savegameFileNames, *count << 8);
    for (i = 0; i < *count; i++) {
        sprintf(buf, "%s\\%s", DAT_005091c8, SkipTextLines(copy, i));
        HapiBank* file = OpenSummaryBank(buf);
        char* desc = 0;
        if (file)
            desc = file->GetStringItem("Description", 0);
        if (file && desc) {
            strcpy(buf, desc);
            strcpy(dp, buf);
            dp += strlen(buf) + 1;
            found++;
            file->CloseBank();
            delete file;
        } else {
            char* d = SkipTextLines(g_savegameFileNames, found);
            for (int j = i + 1; j < *count; j++) {
                char* next = SkipTextLines(copy, j);
                strcpy(d, next);
                d += strlen(next) + 1;
            }
        }
    }
    GameFreeThunk(copy);
    ConfigureListBoxByName(g_game->menu, "GAMES", g_savegameDescs, found, 0);
    *count = found;
    return found ? g_savegameFileNames : 0;
}

// FUNCTION: 0x492de0
void __stdcall SavedGameSelectHandler(int arg1, int arg2)
{
    ShowSavedGameInfo();
}

// Click handler of the save game screen. On close (field +0x60 == -1) it
// frees the four save lists that 0x492b10 built. CANCEL goes back; DELETE
// removes the chosen save file, rebuilds the list and redraws; GAMES, LOAD and
// GAMENAME all take the name typed in the GAMENAME box and, when it is not
// empty, write the save under that name with a timestamp.
// FUNCTION: 0x492df0
void __stdcall SaveGameScreenHandler(Gadget_00492df0* gadget)
{
    Entry_00492df0* entries = gadget->layer->entries;
    Save_00492df0 save;
    if (gadget->selected == -1) {
        SetDescListCleanupFlag(gadget, 1);
        if (g_savegameFileNames)
            GameFreeThunk(g_savegameFileNames);
        if (g_savegameDescs)
            GameFreeThunk(g_savegameDescs);
        if (g_savegameSideList)
            GameFreeThunk(g_savegameSideList);
        g_savegameSideList = 0;
        g_savegameDescs = 0;
        g_savegameFileNames = 0;
        if (g_savegameRadarPreview)
            GameFreeThunk(g_savegameRadarPreview);
        g_savegameRadarPreview = 0;
        return;
    }
    if (IsCurrentGadgetNamed(gadget, "CANCEL")) {
        if (g_game->flags_2a44.bit2_2a44)
            DisableKeyCommands(g_game->message);
        PlaySoundByName("Previous", 0);
        return;
    }
    if (IsCurrentGadgetNamed(gadget, "DELETE")) {
        PlaySoundByName("SmallButton", 0);
        Entry_00492df0* e = FindGadgetChecked(entries, "GAMES");
        sprintf(save.path, "%s\\%s", DAT_005091c8, SkipTextLines(g_savegameFileNames, e->field_ba));
        RemoveFile(save.path);
        ListSavedGames(&save.count);
        ClearSelectedGadget(gadget);
        ShowSavedGameInfo();
        return;
    }
    if (!IsCurrentGadgetNamed(gadget, "GAMES") && !IsCurrentGadgetNamed(gadget, "LOAD") &&
        !IsCurrentGadgetNamed(gadget, "GAMENAME")) {
        if (gadget->selected != -1)
            ClearSelectedGadget(gadget);
        return;
    }
    if (g_game->flags_2a44.bit2_2a44)
        DisableKeyCommands(g_game->message);
    PlaySoundByName("smlbutton", 0);
    int index = FindGadgetIndex(entries, "GAMENAME", 3);
    char* text = entries[index].text;
    if (strlen(text) != 0) {
        save.games = (int)FindGadgetChecked(entries, "GAMES");
        BuildDataPath(g_game->saveName, DAT_005091c8, text, "SAV");
        SaveGameFile(g_game->saveName, text, time(0));
    }
}

// FUNCTION: 0x493060
void ShowSaveGameScreen()
{
    int local;

    g_game->flag_38a51 |= 1;
    Layer_00493060* layer = LoadGuiLayer(&g_game->menu_00493060, "LOADGAME.GUI", 0x880);
    layer->handler = SaveGameScreenHandler;
    layer->data = g_game;
    LoadPictureCached("DSAVEGAME2", 0, 0, 0);
    MakeDirectoryPath(DAT_005091c8);
    ListSavedGames(&local);
    SetTranslatedTextByName(&g_game->menu_00493060, "TITLE", "Save Game", 0);
    if (local == 0) {
        SetGadgetActiveByName(&g_game->menu_00493060, "DELETE", 0);
    }
    Entry_00493060* games = FindGadgetChecked(layer->entries, "GAMES");
    if (games != 0) {
        games->handler = SavedGameSelectHandler;
    }
    int index = FindGadgetIndex(layer->entries, "GAMENAME", 3);
    layer->entries[index].attribs |= 2;
    g_savegameSideList = BuildSideList();
    ShowSavedGameInfo();
    BeginTextEdit(&g_game->menu_00493060, index);
    SetKeyboardInput(&g_game->menu_00493060, 1);
    OrLabelAttribs();
    SetGadgetActiveByName(&g_game->menu_00493060, "LoadGame", 0);
    EnableKeyCommands(&g_game->menu_00493060);
    RenderLayer(&g_game->menu_00493060, 0x40);
}

// FUNCTION: 0x4931d0
void ShowLoadGameScreen()
{
    Gadget_004931d0* gadget = LoadGuiLayer((char*)g_game + 0x519, "LOADGAME.GUI", 0x980);
    gadget->handler = LoadGameScreenHandler;
    gadget->context = g_game;
    LoadPictureCached("DLOADGAME2", 0, 0, 0);
    int count;
    if (ListSavedGames(&count) == 0) {
        CloseTopScreen((char*)g_game + 0x519);
        OpenMessageBox((char*)g_game + 0x519,
                     Translate("There are no saved games to choose from"),
                     0x140, 1, 1);
        return;
    }
    g_savegameSideList = BuildSideList();
    ConfigureListBoxByName((char*)g_game + 0x519, "GAMES", g_savegameDescs, count, 0);
    SetGadgetActiveByName((char*)g_game + 0x519, "DELETE", 0);
    SetGadgetActiveByName((char*)g_game + 0x519, "GAMENAME", 0);
    Entry_004931d0* entry = FindGadgetChecked(gadget->info, "GAMES");
    if (entry != 0) {
        entry->field_ce = (void*)SavedGameSelectHandler;
    }
    ShowSavedGameInfo();
    SetKeyboardInput((char*)g_game + 0x519, 1);
    OrLabelAttribs();
    SetGadgetActiveByName((char*)g_game + 0x519, "SaveGame", 0);
    EnableKeyCommands((char*)g_game + 0x519);
    RenderLayer((char*)g_game + 0x519, 0x40);
    ((char*)g_game)[0x38a51] |= 1;
}
