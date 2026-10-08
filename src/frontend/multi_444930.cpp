// Decompiled by deepseek-v4.1-flash, Sonnet, Opus, DeepSeek V4.1 Flash and Space Bunny Free. Names are provisional.
//
// The multiplayer game setup dialogs: map selection and preview, team logo
// selection, the battle room (player control, watching, team icons and
// alliances), the unit limit and resource readouts and the display mode
// chooser (0x444930 to 0x446e90).
//
// <windows.h> is kept for its declarations: 0x445450's renumbering loop and
// 0x446c70's player addresses pick their SIB base/index order from it.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>

#pragma pack(push, 1)

// The per-player block g_game->players[i].info points at: the name, the
// connection flags, the screen size, the resource readouts and the battle
// room's flag word.
struct PlayerInfo_00444930 {
    union {
        char name[0x9b];                   // +0x00
        struct {
            char unknown_0[0x8b];          // +0x00
            unsigned short field_8b;       // +0x8b
            unsigned short field_8d;       // +0x8d
            char unknown_8f[0x96 - 0x8f];  // +0x8f
            unsigned char field_96;        // +0x96
            unsigned char flags;           // +0x97
            char unknown_98[0x9b - 0x98];  // +0x98
        };
    };
    union {
        unsigned short flags_9b;           // +0x9b
        struct {
            unsigned short bits_9b_0 : 6;  // +0x9b
            unsigned short bit6 : 1;       // bit 6
            unsigned short watching : 1;   // bit 7
            unsigned short mapping : 1;    // bit 8
            unsigned short bit9 : 1;       // bit 9
            unsigned short bit10 : 1;      // bit 10
            unsigned short commander : 2;  // bits 11 and 12
            unsigned short cheating : 1;   // bit 13
            unsigned short fixedloc : 1;   // bit 14
            unsigned short closed : 1;     // bit 15
        };
    };
    unsigned short flags_9d;               // +0x9d
    char unknown_9f[0xa1 - 0x9f];          // +0x9f
    unsigned short field_a1;               // +0xa1
    unsigned short field_a3;               // +0xa3
    unsigned short maxunits;               // +0xa5
    char unknown_a7[0xa9 - 0xa7];          // +0xa7
    unsigned int field_a9;                 // +0xa9
    char unknown_ad[0xb9 - 0xad];          // +0xad
};

// The 0x14b-byte player slot at g_game+0x1b63: +0x27 points at the block
// above (the dialog code also calls it data or unit), +0x73 is the slot type
// and +0x13f the alliance.
struct Player_00444930 {
    int active;                            // +0x00
    int field_4;                           // +0x04
    char unknown_8[0x27 - 0x8];            // +0x08
    union {
        PlayerInfo_00444930* info;         // +0x27
        PlayerInfo_00444930* data;
        PlayerInfo_00444930* unit;
        int field_27;
    };
    char name[0x73 - 0x2b];                // +0x2b
    unsigned char type;                    // +0x73
    char unknown_74[0x108 - 0x74];         // +0x74
    unsigned char field_108[0x113 - 0x108]; // +0x108
    unsigned char field_113[0x13f - 0x113]; // +0x113
    unsigned char alliance;                // +0x13f
    int field_140;                         // +0x140
    short field_144;                       // +0x144
    unsigned char field_146;               // +0x146
    char unknown_147[0x14b - 0x147];       // +0x147
};

// The player record seen as an object by the two slot shufflers.
class Player {
public:
    void SetType(int param_1);
};

// The mission object g_game+0x391e9 points at (defined in the game's own
// files).
class Mission {
public:
    int GetNameSlot(int param_1);
    int GetTranslatedName();
    int GetDescription();
    int LoadMissionByName(char* name);
    char* GetMissionName();
    unsigned int ComputeMapChecksum();
    bool HasMissionName();
    void RefreshMapList(int param_1);
};

// One element of the team logo table at g_game+0x148db.
struct LogoEntry_00445110 {
    void* ptr;                             // +0x00
    int unknown_4;                         // +0x04
};

struct Logos_00445110 {
    unsigned short count;                  // +0x00
    char unknown_2[0x28 - 0x2];            // +0x02
    LogoEntry_00445110 entries[1];         // +0x28
};

// The menu object at g_game+0x519: +0x18 is the layer LoadGuiLayer returns
// and +0x60 the id of the entry that was clicked (-1 when the menu closes).
struct Gadget_00444930 {
    char unknown_0[0x18];                  // +0x00
    struct Holder_00444930* holder;        // +0x18
    char unknown_1c[0x60 - 0x1c];          // +0x1c
    int field_60;                          // +0x60
};

// The layer at g_game+0x531: the entry table at +4, the click handler at +8
// and the dialog's block or owner at +0xc. +0x37 selects the direction the
// display mode chooser walks its list.
struct Holder_00444930 {
    int unknown_0;                         // +0x00
    struct Entry_00444930* entries;        // +0x04
    void (__stdcall* handler)(Gadget_00444930* menu); // +0x08
    union {
        void* layout;                      // +0x0c
        void* owner;
        void* data;
    };
    char unknown_10[0x37 - 0x10];          // +0x10
    int field_37;                          // +0x37
};

struct Game;

// The 0x15b-byte GUI control record. Entry 0 holds the entry count at +0xb6;
// an ordinary entry holds its NUL terminated text there and its ready area at
// +0xcc; an entry bound to a callback holds the callback at +0xce. The
// slider entries 0x445e50 sets up hold an int at +0x13c where 0x4455b0's
// ready entries hold a single bit.
struct Entry_00444930 {
    unsigned char state;                   // +0x00
    char unknown_1;                        // +0x01
    char name[0x13];                       // +0x02
    short field_15;                        // +0x15
    short field_17;                        // +0x17
    short field_19;                        // +0x19
    int flags;                             // +0x1b
    int field_1f;                          // +0x1f
    char unknown_23[0x29 - 0x23];          // +0x23
    unsigned char field_29;                // +0x29
    char unknown_2a[0xb6 - 0x2a];          // +0x2a
    union {
        struct {
            union {
                short count;               // +0xb6 (entry 0 only)
                char entry_text[0xcc - 0xb6]; // +0xb6
            };
            char ready[0x138 - 0xcc];      // +0xcc
        };
        struct {
            char unknown_b6[0xba - 0xb6];  // +0xb6
            union {
                short selected;            // +0xba
                unsigned char field_ba;    // +0xba
            };
            char unknown_bc[0xc2 - 0xbc];  // +0xbc
            union {
                char* text;                // +0xc2
                void* field_c2;            // +0xc2
            };
            char unknown_c6[0xce - 0xc6];  // +0xc6
            union {
                void (__stdcall* onSelect)(Gadget_00444930* menu, int index); // +0xce
                void (__stdcall* field_ce)(void* gadget, int param_2);
            };
            char unknown_d2[0x138 - 0xd2]; // +0xd2
        };
    };
    short field_138;                       // +0x138
    unsigned char field_13a;               // +0x13a
    unsigned char unknown_13b;             // +0x13b
    union {
        int field_13c;                     // +0x13c (0x445e50's slider value)
        struct {
            unsigned short field_13c_bit : 1; // +0x13c (0x4455b0's ready flag)
            unsigned short unknown_13d : 15;
        };
    };
    short field_140;                       // +0x140
    short unknown_142;                     // +0x142
    void (__stdcall* callback)(Gadget_00444930* menu, int index); // +0x144
    char unknown_148[2];                   // +0x148
    Game* game;                            // +0x14a
    char unknown_14e[0x15b - 0x14e];       // +0x14e
};

// The first 0x13e bytes of an entry, copied out by 0x445300 and 0x4455b0.
struct Head_00444930 {
    unsigned char state;                   // +0x00
    char unknown_1;                        // +0x01
    char name[0x13];                       // +0x02
    short field_15;                        // +0x15
    char unknown_17[2];                    // +0x17
    short field_19;                        // +0x19
    int flags;                             // +0x1b
    char unknown_1f[0x29 - 0x1f];          // +0x1f
    unsigned char field_29;                // +0x29
    char unknown_2a[0xb6 - 0x2a];          // +0x2a
    char text[0x13e - 0xb6];               // +0xb6
};

struct Game {
    char unknown_0[0x519];                 // +0x00
    Gadget_00444930 menu;                  // +0x519
    char unknown_57d[0x1b63 - 0x57d];      // +0x57d
    Player_00444930 players[10];           // +0x1b63
    char unknown_2851[0x2a42 - 0x2851];    // +0x2851
    unsigned char localPlayer;             // +0x2a42
    char unknown_2a43;                     // +0x2a43
    union {
        unsigned short flags_2a44;         // +0x2a44
        struct {
            unsigned short bit0 : 1;
            unsigned short bit1 : 1;
            unsigned short bit2 : 1;
            unsigned short rest : 13;
        };
    };
    char unknown_2a46[0x2bee - 0x2a46];    // +0x2a46
    unsigned short flag0 : 1;              // +0x2bee
    unsigned short bits1 : 15;
    char unknown_2bf0[0x148d7 - 0x2bf0];   // +0x2bf0
    void* logos;                           // +0x148d7
    Logos_00445110* logos32;               // +0x148db
    char unknown_148df[0x37f1b - 0x148df]; // +0x148df
    int field_37f1b;                       // +0x37f1b
    int field_37f1f;                       // +0x37f1f
    char unknown_37f23[0x391e9 - 0x37f23]; // +0x37f23
    Mission* field_391e9;                  // +0x391e9
};

// The map selector's layout block at layer+0xc (0x444ea0).
struct Data_00444ea0 {
    char unknown_0[0x14];                  // +0x00
    char* items;                           // +0x14
};

// The map dialog's layout block at holder+0xc (0x444cb0).
struct Layout_00444cb0 {
    char unknown_0[0x14];                  // +0x00
    void* field_14;                        // +0x14
};

// The logo dialog's layout block at holder+0xc (0x445110): the byte list of
// the free logo indices, the pointer list handed to the GUI and the copied
// animation sequences.
struct AnimSeq_00445110 {
    char unknown_0[0x28];                  // +0x00
    void* field_28;                        // +0x28
    char unknown_2c[0x30 - 0x2c];          // +0x2c
};

struct Layout_00445110 {
    char selected[0x18];                   // +0x00
    void** ptrList;                        // +0x18
    AnimSeq_00445110* seqs;                // +0x1c
};

// The map preview dialog's layout block at holder+0xc (0x444930).
struct Layout_00444930 {
    char unknown_0[0x18];                  // +0x00
    void* field_18;                        // +0x18
    void* field_1c;                        // +0x1c
};

// The display mode list 0x4461d0 and 0x446310 share.
struct Mode_00446310 {
    int width;                             // +0x00
    int height;                            // +0x04
    int field_8;                           // +0x08
};

struct Class_00446310 {
    int count;                             // +0x00
    Mode_00446310* modes;                  // +0x04
    char unknown_8[0x14 - 0x8];            // +0x08
    char* available;                       // +0x14
    char unknown_18[0x20 - 0x18];          // +0x18
};

#pragma pack(pop)

class Class_004a1080;
struct Dialog;

// GLOBAL: 0x511de8
extern Game* g_game;
extern char DAT_00502ae8[];                // "OK"
extern char DAT_00505974[];                // "Multi"
extern char* DAT_00512990;
extern char* DAT_005054b0[];
extern int DAT_00512760;
extern short DAT_00512764;
extern int DAT_0051276c;
extern int DAT_00512994;
extern int DAT_00505510;

int __stdcall IsCurrentGadgetNamed(Gadget_00444930* gadget, const char* name);
Entry_00444930* __stdcall FindGadgetChecked(void* entries, const char* name);
int __stdcall FindGadgetIndex(void* entries, const char* name, int type);
void __stdcall PlaySoundByName(char* str, int flag);
void __stdcall FUN_004ab0a0(void* gadget);
void __stdcall RequestPlayerColor(int value);
void __cdecl FUN_004d85a0(void* p);
void* __cdecl FUN_004d83b0(const char* name, unsigned int size);
void __stdcall FUN_004a0bf0(void* menu, const char* name, char* text, int param_4);
Entry_00444930* __stdcall FUN_004a0280(void* entries, char* name);
void* __stdcall FUN_004295b0(char* path, int* outX, int* outY);
void __stdcall ResizeRadarPicture(void* bmp, int param_2, int param_3, int param_4, int param_5);
void __stdcall FUN_0049fa90(void* menu);
char* __stdcall Translate(char* text);
Holder_00444930* __stdcall LoadGuiLayer(void* menu, const char* name, int flags);
void __stdcall LoadPictureCached(const char* name, int a, int b, int c);
void __stdcall FUN_0049fb10(void* menu, int value);
void __stdcall RenderLayer(void* menu, int value);
char* __stdcall SkipTextLines(char* text, int n);
void __stdcall FUN_004a0570(void* menu, const char* name, int value);
void __stdcall ReportGameEvent(int msg);
void BroadcastPlayerInfo(void);
void UpdateNetGameInfo(void);
int __stdcall LoadMapList(char** out, int param_2, int param_3);
void __stdcall OpenMessageBox(void* menu, const char* text, int width, int a, int b);
void __stdcall SortFileList(char* items, int b, int c, int count);
void __stdcall FUN_004a32a0(void* menu, const char* name, char* items, int count, int flag);
void __stdcall FUN_004a2e40(void* menu, const char* name, int index);
void __stdcall SetGadgetItems(void* gui, const char* name, void** items, int count);
void __stdcall FUN_00444910(void* gadget, int param_2);
int __stdcall FUN_004a5d50(void* menu, int index);
void __stdcall FUN_004a1450(void* menu, char* name, int value);
int __stdcall SetButtonStageByName(Class_004a1080* obj, char* name, int value);
Entry_00444930* __stdcall FUN_004a0200(void* entries, char* name);
unsigned char FindHostSlot(void);
int __stdcall ReadSliderValue(void* gadget);
void __stdcall SetSliderFromValue(Entry_00444930* gadget, int value);
int __stdcall IsGadgetNamed(int param1, int param2, char* name);
int __stdcall GetSlotDpid(unsigned char player);
void __stdcall RejectPlayer(int param_1, int param_2);
void SaveSettings(void);
int __stdcall GetDisplayModes(Class_00446310* obj);
void __stdcall SortDisplayModes(Class_00446310* obj);
void __stdcall RefreshAlliesScreen(int value);
void __stdcall OpenRejectDialog(int player);
void __stdcall HandleControlDialogClick(Gadget_00444930* gadget);
void __stdcall SetAlliance(int, int, int, int);
void ShowSelectedMapInfo(void);
void __stdcall HandleViewMapClick(Gadget_00444930* gadget);
void __stdcall HandleMapSelectClick(Gadget_00444930* gadget);
void __stdcall HandleLogoSelectClick(Gadget_00444930* gadget);
void __stdcall HandleRejectChoice(Gadget_00444930* gadget);
void __stdcall UpdateMapSelection(Gadget_00444930* menu, int unused);
// Handler for the multiplayer side-selection dialog. When the dialog closes
// (current gadget -1) it frees the layout data; when the player picks a side
// (LOGOS/SELECT) it copies that side's byte into the local player's info.

// FUNCTION: 0x444930
void __stdcall HandleLogoSelectClick(Gadget_00444930* param_1)
{
    Entry_00444930* entries = param_1->holder->entries;
    Layout_00444930* layout = (Layout_00444930*)param_1->holder->layout;

    if (param_1->field_60 == -1) {
        FUN_004d85a0(layout->field_1c);
        FUN_004d85a0(layout->field_18);
        FUN_004d85a0(layout);
        PlaySoundByName("Multi", 0);
        return;
    }
    if (IsCurrentGadgetNamed(param_1, "LOGOS") || IsCurrentGadgetNamed(param_1, "SELECT")) {
        Player_00444930* player = &g_game->players[g_game->localPlayer];
        Entry_00444930* entry = FindGadgetChecked(entries, "LOGOS");
        player->info->field_96 = ((char*)layout)[entry->field_ba];
        g_game->flag0 = 1;
        RequestPlayerColor(player->info->field_96);
        return;
    }
    if (!IsCurrentGadgetNamed(param_1, "Cancel"))
        FUN_004ab0a0(param_1);
}

// FUNCTION: 0x444a20
void ShowSelectedMapInfo()
{
    int outX;
    int outY;
    char buffer[100];

    if (FindGadgetIndex(g_game->menu.holder->entries, "MAPNAME", 5) != -1) {
        FUN_004a0bf0(&g_game->menu, "MAPNAME",
                     (char*)((Mission*)g_game->field_391e9)->GetTranslatedName(), 0);
    }

    sprintf(buffer, "%s  %s: %s",
            (char*)g_game->field_391e9 + 0xdc4,
            Translate("Players"),
            (char*)g_game->field_391e9 + 0xe44);
    FUN_004a0bf0(&g_game->menu, "SIZE", (char*)buffer, 0);

    Entry_00444930* entry = FUN_004a0280(g_game->menu.holder->entries, "MAPPIC");
    if (entry->field_c2 != 0) {
        FUN_004d85a0(entry->field_c2);
        entry->field_c2 = 0;
    }
    void* bmp = FUN_004295b0(
        (char*)((Mission*)g_game->field_391e9)->GetNameSlot(1), &outX, &outY);
    entry->field_c2 = bmp;
    if (bmp != 0) {
        ResizeRadarPicture(bmp, entry->field_17, entry->field_19, outX << 4, outY << 4);
    }

    FUN_004a0bf0(&g_game->menu, "DESCRIPTION",
                 (char*)((Mission*)g_game->field_391e9)->GetDescription(), 0);
    FUN_0049fa90(&g_game->menu);
}

// FUNCTION: 0x444ba0
void __stdcall HandleViewMapClick(Gadget_00444930* param_1)
{
    if (param_1->field_60 != -1) {
        if (IsCurrentGadgetNamed(param_1, DAT_00502ae8)) {
            PlaySoundByName(DAT_00505974, 0);
        } else {
            FUN_004ab0a0(param_1);
        }
    }
}

// Opens the map view dialog (VIEWMAP.GUI) with HandleViewMapClick as its handler.
// FUNCTION: 0x444be0
void OpenViewMapDialog()
{
    LoadGuiLayer(&g_game->menu, "VIEWMAP.GUI", 0x900)->handler = HandleViewMapClick;
    LoadPictureCached("DVIEWMAP", 0, 0, 0);
    ShowSelectedMapInfo();
    FUN_0049fb10(&g_game->menu, 1);
    RenderLayer(&g_game->menu, 0x40);
}

// FUNCTION: 0x444c40
void __stdcall UpdateMapSelection(Gadget_00444930* menu, int unused)
{
    Entry_00444930* g = FindGadgetChecked(menu->holder->entries, "MAPNAMES");
    if (g_game->field_391e9->LoadMissionByName(SkipTextLines(g->text, g->selected)) == 0) {
        FUN_004a0570(menu, "MAPPIC", 0);
    } else {
        FUN_004a0570(menu, "MAPPIC", 1);
        ShowSelectedMapInfo();
    }
}

// FUNCTION: 0x444cb0
void __stdcall HandleMapSelectClick(Gadget_00444930* param_1)
{
    void* entries = param_1->holder->entries;
    Layout_00444cb0* layout = (Layout_00444cb0*)param_1->holder->layout;

    if (param_1->field_60 == -1) {
        Entry_00444930* entry = FUN_004a0280(g_game->menu.holder->entries, "MAPPIC");
        if (entry->text != 0) {
            FUN_004d85a0(entry->text);
            entry->text = 0;
        }
        FUN_004d85a0(layout->field_14);
        FUN_004d85a0(layout);
        FUN_004d85a0(DAT_00512990);
        DAT_00512990 = 0;
        return;
    }

    if (IsCurrentGadgetNamed(param_1, "MAPNAMES") || IsCurrentGadgetNamed(param_1, "LOAD")) {
        PlaySoundByName("Multi", 0);
        Entry_00444930* g = FindGadgetChecked(entries, "MAPNAMES");
        ((Mission*)g_game->field_391e9)->LoadMissionByName(
            SkipTextLines(g->text, g->selected));

        Player_00444930* player = &g_game->players[g_game->localPlayer];
        strcpy(player->data->name,
               ((Mission*)g_game->field_391e9)->GetMissionName());
        player->data->field_a9 =
            ((Mission*)g_game->field_391e9)->ComputeMapChecksum();

        BroadcastPlayerInfo();
        ReportGameEvent(5);
        UpdateNetGameInfo();

        for (int i = 0; i < 10; i++) {
            if (g_game->players[i].active == 0 ||
                (g_game->players[i].type != 1 && g_game->players[i].type != 2)) {
                g_game->players[i].data->flags_9b &= 0xffdf;
            }
        }
        return;
    }

    if (IsCurrentGadgetNamed(param_1, "PREVMENU")) {
        PlaySoundByName("Previous", 0);
        ((Mission*)g_game->field_391e9)->LoadMissionByName(DAT_00512990);
        BroadcastPlayerInfo();
        return;
    }

    FUN_004ab0a0(param_1);
}

// Opens the multiplayer map selector (SELMAP.GUI): saves the map the local
// player was last on in a global buffer, counts the multiplayer maps, fills
// the MAPNAMES list with them, selects the saved one, then applies the
// selection the way the MAPNAMES callback does.

// The call to Mission::RefreshMapList(0) is compiled without its
// argument push, although the callee ends in "ret 4" (see 0x435d30) and every
// other call site of it does push (0x430b98, 0x4446d7, 0x44a49e). That leaves
// the stack 4 bytes short, so this call is kept exactly as the original has
// it. It is never reached: the only caller (0x4488ea) calls this function
// precisely when Mission::HasMissionName() is false, and the test at the
// top of this function then returns early.

// FUNCTION: 0x444ea0
void OpenMultiMapSelector()
{
    DAT_00512990 = (char*)FUN_004d83b0("OLDMAPNAME", 0xc8);

    if (!((Mission*)g_game->field_391e9)->HasMissionName()) {
        OpenMessageBox(&g_game->menu,
                     Translate("There are no multiplayer maps to choose from"),
                     0x140, 1, 1);
        return;
    }

    strcpy(DAT_00512990,
           ((Mission*)g_game->field_391e9)->GetMissionName());
    ((Mission*)g_game->field_391e9)->RefreshMapList(0);

    int n = LoadMapList(0, 0, 0);
    if (n == 0) {
        OpenMessageBox(&g_game->menu,
                     Translate("There are no multiplayer maps to choose from"),
                     0x140, 1, 1);
        return;
    }

    Holder_00444930* layer = LoadGuiLayer(&g_game->menu, "SELMAP.GUI", 0x980);
    layer->handler = HandleMapSelectClick;
    Data_00444ea0* data = (Data_00444ea0*)FUN_004d83b0("SELECT MAP DATA", 0x20);
    layer->data = data;
    LoadPictureCached("DSELECTMAP2", 0, 0, 0);
    LoadMapList(&data->items, 0, 0);
    SortFileList(data->items, 0, 0, n);
    FUN_004a32a0(&g_game->menu, "MAPNAMES", data->items, n, 0);
    FindGadgetChecked(layer->entries, "MAPNAMES")->onSelect = UpdateMapSelection;

    for (int i = 0; i < n; i++) {
        if (strcmp(DAT_00512990, SkipTextLines(data->items, i)) == 0) {
            FUN_004a2e40(&g_game->menu, "MAPNAMES", i);
            break;
        }
    }

    Gadget_00444930* menu = &g_game->menu;
    Entry_00444930* g = FindGadgetChecked(menu->holder->entries, "MAPNAMES");
    if (((Mission*)g_game->field_391e9)->LoadMissionByName(
            SkipTextLines(g->text, g->selected)) == 0) {
        FUN_004a0570(menu, "MAPPIC", 0);
    } else {
        FUN_004a0570(menu, "MAPPIC", 1);
        ShowSelectedMapInfo();
    }
    FUN_0049fb10(&g_game->menu, 1);
    RenderLayer(&g_game->menu, 0x40);
}

// Sets up the multiplayer "select team logo" dialog (LOGOSEL.GUI). It opens
// the dialog, allocates a small layout object holding the list of logo
// pointers and a copy of the logo sequence records, then walks the ten player
// slots: a slot takes a logo index j when its type byte is neither 0 nor 4
// and its player data's logo byte is at least j. The pointer list and the
// byte array are filled in from the slots that no player claimed.

// FUNCTION: 0x445110
void OpenLogoSelectDialog()
{
    Holder_00444930* gui = LoadGuiLayer(&g_game->menu, "LOGOSEL.GUI", 0x800);
    gui->handler = HandleLogoSelectClick;
    Layout_00445110* layout = (Layout_00445110*)FUN_004d83b0("SELECT TEAM LOGO", 0x20);
    gui->layout = layout;
    int count = g_game->logos32->count;
    layout->ptrList = (void**)FUN_004d83b0("ANIMSEQ PTR LIST", count * 4);
    layout->seqs = (AnimSeq_00445110*)FUN_004d83b0("ACTUAL ANIMSEQS", count * 0x30);
    void** cursor = layout->ptrList;
    int n = 0;
    for (int j = 0; j < count; j++) {
        // Array indexing (players[k]): strength-reduced to a pointer over the type byte.
        int k;
        for (k = 0; k < 10; k++) {
            if (g_game->players[k].type != 0 && g_game->players[k].type != 4
                && g_game->players[k].data->field_96 >= j)
                break;
        }
        layout->seqs[j] = *(AnimSeq_00445110*)g_game->logos32;
        layout->seqs[j].field_28 = g_game->logos32->entries[j].ptr;
        if (k == 10) {
            *cursor = &layout->seqs[j];
            ((char*)layout)[n] = (char)j;
            // n before cursor: sets the eax/ecx roles in this block.
            n++;
            cursor++;
        }
    }
    Entry_00444930* logo = FindGadgetChecked(gui->entries, "LOGOS");
    if (logo != 0) {
        logo->field_ce = FUN_00444910;
    }
    int index = FindGadgetIndex(gui->entries, "LOGOS", 2);
    if (index != -1) {
        ((Entry_00444930*)((char*)gui->entries + index * 0x15b))->flags |= 0x40;
    }
    SetGadgetItems(gui, "LOGOS", layout->ptrList, n);
    FUN_0049fb10(&g_game->menu, 1);
    RenderLayer(&g_game->menu, 0x40);
}

// FUNCTION: 0x445300
void __stdcall FUN_00445300(Entry_00444930* param_1)
{
    if (param_1->state == 1) {
        Head_00444930 tmp = *(Head_00444930*)param_1;
        int index = FindGadgetIndex(g_game->menu.holder->entries, param_1->name, 0xe);
        FUN_004a5d50(&g_game->menu, index);
        param_1->field_15 += 2;
        param_1->state = 5;
        strcpy(param_1->entry_text, tmp.text);
        param_1->flags |= 0x10;
    }
}

// Swaps two player slots, clears the first one (type 0, not active), then
// stamps field_146 of every playing slot with its own index, or 10 for the
// others.

// FUNCTION: 0x4453a0
void __stdcall SwapPlayerSlots(Player_00444930* param_1, Player_00444930* param_2)
{
    Player_00444930 tmp = *param_2;
    *param_2 = *param_1;
    *param_1 = tmp;
    ((Player*)param_1)->SetType(0);
    param_1->active = 0;
    // The original bound is i <= 10, not i < 10: the offset test is
    // "cmp eax, 0xcee; jle" (0xcee is 10 * 0x14b, the size of players), so
    // the last pass reads and writes players[10]. The table really has 11
    // slots (+0x1b63 to +0x299c), so this is the spare last slot, not an
    // overrun; this file declares only the first ten.
    for (int i = 0; i <= 10; i++) {
        Player_00444930* p = &g_game->players[i];
        if (p->active != 0
            && (p->type == 1 || p->type == 2 || p->type == 3)
            && p->field_146 != 10) {
            p->field_146 = i;
        } else {
            // Re-derived: with the same pointer variable in both arms MSVC
            // keeps g_game in edx instead of ecx.
            g_game->players[i].field_146 = 10;
        }
    }
}

// Compacts the player list: finds the first slot the renumbering pass would
// call dead, moves the next live slot into it, clears the slot it left, then
// renumbers every slot's field_146.
// FUNCTION: 0x445450
void FUN_00445450()
{
    Player_00444930* p = g_game->players;
    Player_00444930* q = g_game->players + 1;
    Player_00444930* end = g_game->players + 10;
    while (1) {
        if (q >= end && p >= end)
            break;
        // Step over slots that are in use, and over type 4 slots.
        while ((p->active != 0
                    && (p->type == 1 || p->type == 2 || p->type == 3)
                    && p->field_146 != 10)
               || p->type == 4) {
            if (p >= end)
                break;
            p++;
        }
        q = p + 1;
        // Find the next slot that is in use.
        for (; q->active == 0
               || (q->type != 1 && q->type != 2 && q->type != 3)
               || q->field_146 == 10;
             q++) {
            if (q >= end)
                break;
        }
        if (q >= end)
            break;
        if (p >= end)
            break;
        Player_00444930 tmp = *p;
        *p = *q;
        *q = tmp;
        ((Player*)q)->SetType(0);
        q->active = 0;
        for (int i = 0; i <= 10; i++) {
            // Global read into a local first: moves the reload into ecx.
            Game* g = g_game;
            // Through a byte pointer: avoids a reload via an extra lea.
            unsigned char* f = &g->players[i].field_146;
            if (g->players[i].active != 0
                && (g->players[i].type == 1 || g->players[i].type == 2
                    || g->players[i].type == 3) && *f != 10) {
                *f = (unsigned char)i;
            } else {
                *f = 10;
            }
        }
    }
}

static void CloneFix_004455b0(Entry_00444930* rec)
{
    Head_00444930 tmp = *(Head_00444930*)rec;
    int index = FindGadgetIndex(g_game->menu.holder->entries, rec->name, 0xe);
    FUN_004a5d50(&g_game->menu, index);
    rec->field_15 += 2;
    rec->state = 5;
    strcpy(rec->entry_text, tmp.text);
    rec->flags |= 0x10;
}

// FUNCTION: 0x4455b0
void __cdecl FUN_004455b0(void)
{
    char* base = (char*)g_game->menu.holder->entries;
    int p = 0;
    int t;
    char** slot;

    *(short*)(base + 0xb6) = DAT_00512764;
    do {
        for (t = 0, slot = DAT_005054b0; *slot != 0; slot++, t++) {
            int index = FindGadgetIndex(base, *slot, 0xe);
            Entry_00444930* rec = (Entry_00444930*)(base + 0x15b * index);
            Entry_00444930* dst;
            short count;

            DAT_00512760 = rec->field_15;
            if (t == 0)
                DAT_0051276c = rec->field_19;
            count = ++*(short*)(base + 0xb6);
            dst = (Entry_00444930*)(base + 0x15b * count);
            *dst = *rec;
            dst->name[strlen(dst->name) - 1] = (char)('0' + p);
            dst->field_15 += p * 20;
            dst->unknown_1 = 0;
            dst->field_29 = 1;
            if (dst->state != 5) {
                switch (t) {
                case 0:
                    if (p == g_game->localPlayer) {
                        if (dst->state == 1)
                            CloneFix_004455b0(dst);
                        dst->flags = 1;
                    } else {
                        dst->flags |= 0x8000;
                    }
                    break;
                case 1:
                    if (p != g_game->localPlayer) {
                        dst->field_13c_bit = 1;
                        dst->field_138 = 0;
                    }
                    break;
                case 2:
                    {
                        // Indexed through g_game->players[p], not a byte offset: g_game is the SIB base.
                        Player_00444930* pl = &g_game->players[p];
                        if (pl->active == 0 || (pl->type != 1 && pl->type != 2))
                            dst->field_29 = 0;
                    }
                    break;
                case 3:
                    {
                        Player_00444930* pl = &g_game->players[p];
                        int ok = pl->active != 0 && (pl->type == 1 || pl->type == 2);
                        FUN_004a1450(&g_game->menu, dst->name, !ok);
                    }
                    dst->field_29 = 0;
                    break;
                case 6:
                    if (p != g_game->localPlayer && dst->state == 1)
                        CloneFix_004455b0(dst);
                    if (*(int*)&g_game->players[p] != 0 &&
                        (&g_game->players[p])->type == 2)
                        dst->field_29 = 0;
                    break;
                case 7:
                    if (p == g_game->localPlayer)
                        dst->field_29 = 0;
                    break;
                case 9:
                    dst->field_29 = 0;
                    SetButtonStageByName((Class_004a1080*)&g_game->menu, dst->name, 10);
                    break;
                default:
                    if (dst->state == 1)
                        CloneFix_004455b0(dst);
                    break;
                }
            }
        }
        p++;
    } while (p < 10);

    {
        char name[52];
        int index;
        sprintf(name, "PLAYER%d", g_game->localPlayer);
        index = FindGadgetIndex(base, name, 0xe);
        if (index != -1) {
            Entry_00444930* rec = (Entry_00444930*)(base + 0x15b * index);
            if (rec->state == 1)
                CloneFix_004455b0(rec);
        }
        sprintf(name, "READY%d", g_game->localPlayer);
        index = FindGadgetIndex(base, name, 1);
        if (index != -1) {
            Entry_00444930* rec = (Entry_00444930*)(base + 0x15b * index);
            rec->field_13a = (unsigned char)tolower(name[0]);
            strcpy(base + 0xcc, name);
        }
    }
    DAT_00512994 = 1;
}

// GUI callback (see the entry a slider widget stores at +0x144): shows the
// unit limit of the player being watched, as text, and remembers it on the
// local player. FindHostSlot picks the watched player, or 10 for "nobody",
// in which case the value comes from the widget's own slider instead.

// FUNCTION: 0x445b70
void __stdcall UpdateMaxUnitsText(Gadget_00444930* gui, int index)
{
    char text[0x14];
    int count;
    Entry_00444930* maxunits = (Entry_00444930*)FUN_004a0200(gui->holder->entries, "MAXUNITS");
    if (maxunits != 0) {
        int player = FindHostSlot();
        if (player == g_game->localPlayer || player == 10) {
            count = ReadSliderValue(maxunits) + 0x14;
        } else {
            count = g_game->players[player].data->maxunits;
        }
        _itoa(count, text, 10);
        FUN_004a0bf0(gui, "MAXUNITSTEXT", text, 0);
        g_game->players[g_game->localPlayer].data->maxunits = count;
        PlayerInfo_00444930* data = g_game->players[g_game->localPlayer].data;
        unsigned char f = data->flags;
        data->maxunits = count;
        if (f & 1) {
            BroadcastPlayerInfo();
        }
    }
}

// Writes the local player's stored metal (gadget "METAL") rounded down to
// hundreds into the "METALTEXT" label, mirrors it into the unit at +0xa3 and,
// if the unit's flag byte at +0x97 has bit 0, re-sends the player block and the
// name call. Near-copy of 0x445d60 (the "ENERGY" one) and 0x445b70.

// FUNCTION: 0x445c70
void __stdcall UpdateMetalText(Gadget_00444930* sub, int unused)
{
    char text[20];
    void* value = FUN_004a0200(sub->holder->entries, "METAL");

    if (value != 0) {
        int shown = ReadSliderValue(value) / 100 * 100;
        int hundreds;
        PlayerInfo_00444930* unit;

        _itoa(shown, text, 10);
        FUN_004a0bf0(sub, "METALTEXT", text, 0);
        hundreds = shown / 100;
        g_game->players[g_game->localPlayer].unit->field_a3 = (unsigned short)hundreds;
        // The original writes the same value to the same field a second time,
        // through a freshly looked up unit pointer, before testing its flag.
        unit = g_game->players[g_game->localPlayer].unit;
        unit->field_a3 = (unsigned short)hundreds;
        if (unit->flags & 1) {
            BroadcastPlayerInfo();
            UpdateNetGameInfo();
        }
    }
}

// Shows the local player's stored energy (gadget "ENERGY") rounded down to
// hundreds in the "ENERGYTEXT" label, mirrors it into the unit at +0xa1 and,
// if the unit's flag byte at +0x97 has bit 0, re-sends the player block and
// the name call. Near-copy of 0x445c70 (the "METAL" one).

// FUNCTION: 0x445d60
void __stdcall UpdateEnergyText(Gadget_00444930* sub, int unused)
{
    char text[20];
    void* value = FUN_004a0200(sub->holder->entries, "ENERGY");

    if (value != 0) {
        int shown = ReadSliderValue(value) / 100 * 100;
        PlayerInfo_00444930* unit;

        _itoa(shown, text, 10);
        FUN_004a0bf0(sub, "ENERGYTEXT", text, 0);
        unit = g_game->players[g_game->localPlayer].unit;
        unit->field_a1 = (unsigned short)(shown / 100);
        if (unit->flags & 1) {
            BroadcastPlayerInfo();
            UpdateNetGameInfo();
        }
    }
}

// Sets the value of the named gadget of a menu (see 0x445e50).

// FUNCTION: 0x445e20
void __stdcall FUN_00445e20(Gadget_00444930* menu, char* name, int value)
{
    Entry_00444930* gadget = FUN_004a0200(menu->holder->entries, name);
    SetSliderFromValue(gadget, value);
}
// Sets up the gadget with the given name in the game's menu (if it exists),
// then hands the gadget's index to the callback.

typedef void (__stdcall* Callback_00445e50)(Gadget_00444930* menu, int index);

// FUNCTION: 0x445e50
void __stdcall FUN_00445e50(char* name, int param_2, int param_3, Callback_00445e50 callback)
{
    Gadget_00444930* menu = &g_game->menu;
    void* gadgets = menu->holder->entries;
    int index = FindGadgetIndex(gadgets, name, 0xe);
    if (index != -1) {
        Entry_00444930* gadget = FUN_004a0200(gadgets, name);
        gadget->field_13c = param_2;
        gadget->callback = callback;
        gadget->field_140 = param_3;
        SetSliderFromValue(gadget, gadget->field_140);
        gadget->game = g_game;
    }
    callback(menu, index);
    FUN_0049fa90(menu);
}

// Pushes the local player's status flags (commander, mapping, los type,
// watching, cheating, fixed position, game open) into the GUI by name.
// SetButtonStageByName's value parameter is widened to an int here, as in 0x446450.

// FUNCTION: 0x445ed0
void UpdateBattleRoomFlags()
{
    int i = FindHostSlot();
    if (i == 10) {
        i = g_game->localPlayer;
    }
    PlayerInfo_00444930* info = g_game->players[i].info;

    SetButtonStageByName((Class_004a1080*)&g_game->menu, "COMMANDER", info->commander);
    SetButtonStageByName((Class_004a1080*)&g_game->menu, "MAPPING", !info->mapping);
    int los;
    if (!info->bit9) {
        los = 2;
    } else {
        los = !info->bit10;
    }
    SetButtonStageByName((Class_004a1080*)&g_game->menu, "LOSTYPE", los);
    SetButtonStageByName((Class_004a1080*)&g_game->menu, "WATCHING", info->watching);
    SetButtonStageByName((Class_004a1080*)&g_game->menu, "CHEATING", info->cheating);
    SetButtonStageByName((Class_004a1080*)&g_game->menu, "FIXEDLOC", info->fixedloc);
    SetButtonStageByName((Class_004a1080*)&g_game->menu, "GAMEOPEN", !info->closed);
}

// Handler for a two-choice dialog gadget: "CHOICE1" acts on the local
// player, "CHOICE2" does nothing, anything else is passed on.

// FUNCTION: 0x446020
void __stdcall HandleRejectChoice(Gadget_00444930* gadget)
{
    int owner = (int)gadget->holder->entries;
    if (gadget->field_60 == -1)
        return;
    if (IsGadgetNamed(owner, gadget->field_60, "CHOICE1")) {
        RejectPlayer(GetSlotDpid((unsigned char)DAT_00505510), 1);
    } else if (!IsGadgetNamed(owner, gadget->field_60, "CHOICE2")) {
        FUN_004ab0a0(gadget);
    }
}

// Opens the YESORNO.GUI dialog for player DAT_00505510, fills its CHOICE1 /
// CHOICE2 / TITLE fields and installs HandleRejectChoice as the handler. The title
// is "Reject <player name>?".

// FUNCTION: 0x446080
void __stdcall OpenRejectDialog(int player)
{
    char buf[100];
    DAT_00505510 = player;
    Holder_00444930* gadget = LoadGuiLayer(&g_game->menu, "YESORNO.GUI", 0x100);
    if (gadget != 0) {
        FUN_0049fb10(&g_game->menu, 1);
        void* entries = gadget->entries;
        FindGadgetIndex(entries, "CHOICE1", 1);
        FindGadgetIndex(entries, "CHOICE2", 1);
        FindGadgetIndex(entries, "TITLE", 5);
        FUN_004a0bf0(&g_game->menu, "CHOICE1", "Yes", 0);
        FUN_004a0bf0(&g_game->menu, "CHOICE2", "No", 0);
        sprintf(buf, "%s %s?", Translate("Reject"),
                g_game->players[DAT_00505510].name);
        FUN_004a0bf0(&g_game->menu, "TITLE", buf, 0);
        gadget->handler = HandleRejectChoice;
        gadget->owner = g_game;
        FUN_0049fb10(&g_game->menu, 1);
        RenderLayer(&g_game->menu, 0x40);
    }
}

// Handler for the "MODES" (display mode) dialog. When a mode gadget is
// selected (MODES or SELECT), it copies the selected display mode into the
// game's width/height and the local player's screen size, then applies it.
// CANCEL exits the game, OK plays the button sound. With no current gadget
// (-1) it frees the display mode list.

// FUNCTION: 0x4461d0
void __stdcall HandleDisplayModesClick(Gadget_00444930* gui)
{
    Holder_00444930* holder = gui->holder;
    Entry_00444930* gadgets = holder->entries;
    Class_00446310* obj = (Class_00446310*)holder->layout;
    if (gui->field_60 == -1) {
        FUN_004d85a0(obj->available);
        FUN_004d85a0(obj->modes);
        FUN_004d85a0(obj);
        return;
    }
    if (IsCurrentGadgetNamed(gui, "MODES") || IsCurrentGadgetNamed(gui, "SELECT")) {
        Player_00444930* player = &g_game->players[g_game->localPlayer];
        Entry_00444930* entry = FindGadgetChecked(gadgets, "MODES");
        Mode_00446310* mode = &obj->modes[entry->selected];
        g_game->field_37f1b = mode->width;
        g_game->field_37f1f = mode->height;
        player->data->field_8b = (unsigned short)mode->width;
        player->data->field_8d = (unsigned short)mode->height;
        BroadcastPlayerInfo();
        SaveSettings();
        return;
    }
    if (IsCurrentGadgetNamed(gui, "CANCEL")) {
        PlaySoundByName("Exit", 0);
        SaveSettings();
        return;
    }
    if (IsCurrentGadgetNamed(gui, "OK")) {
        PlaySoundByName("SMLBUTTON", 0);
        SaveSettings();
        return;
    }
    FUN_004ab0a0(gui);
}

// Screen resolution selection: builds the display mode list, then advances the
// local player's mode to the next (or previous) entry.

// FUNCTION: 0x446310
void FUN_00446310(void)
{
    Class_00446310* obj = (Class_00446310*)FUN_004d83b0("SELECT VIDEO MODE", 0x20);
    obj->available = 0;
    obj->modes = (Mode_00446310*)FUN_004d83b0("DISPLAY MODES", 0x4b0);

    if (GetDisplayModes(obj) != 0) {
        SortDisplayModes(obj);
        obj->available = (char*)FUN_004d83b0("AVAILABLE MODES", obj->count << 8);
        obj->available[0] = 0;

        Player_00444930* player = &g_game->players[g_game->localPlayer];
        int count = obj->count;
        for (int i = 0; i < count; i++) {
            if (obj->modes[i].width == player->data->field_8b
                && obj->modes[i].height == player->data->field_8d) {
                if (g_game->menu.holder->field_37 == 2) {
                    i--;
                    if (i < 0)
                        i = count - 1;
                } else {
                    i++;
                    if (i >= count)
                        i = 0;
                }
                Mode_00446310& mode = obj->modes[i];
                player->data->field_8b = (unsigned short)mode.width;
                player->data->field_8d = (unsigned short)mode.height;
                BroadcastPlayerInfo();
                g_game->field_37f1b = mode.width;
                g_game->field_37f1f = mode.height;
                break;
            }
        }
    }
    FUN_004d85a0(obj->modes);
    FUN_004d85a0(obj);
}

// Sets the GUI's "WATCHING" and "GAMEOPEN" controls from the local player's
// flags and marks the GUI for redraw. SetButtonStageByName's value is widened as an
// int here (its own file says char; the checker compares names only).

// FUNCTION: 0x446450
void UpdateWatchingGadgets()
{
    PlayerInfo_00444930* info = g_game->players[g_game->localPlayer].info;
    SetButtonStageByName((Class_004a1080*)&g_game->menu, "WATCHING", info->watching);
    SetButtonStageByName((Class_004a1080*)&g_game->menu, "GAMEOPEN", !info->closed);
    FUN_0049fa90((Dialog*)&g_game->menu);
}

// Handler for the CONTROL.GUI dialog: choosing a "LIVEPLYR%d" entry opens the
// reject dialog for that player, WATCHING toggles the local player's watching
// flag and republishes the GUI values, OK kicks every playing player in state
// 3 without watch permission, and any other gadget clears the current one.

// FUNCTION: 0x4464d0
void __stdcall HandleControlDialogClick(Gadget_00444930* gui)
{
    PlayerInfo_00444930* info = g_game->players[g_game->localPlayer].info;
    if (gui->field_60 != -1) {
        char buf[100];
        for (int i = 0; i < 10; i++) {
            sprintf(buf, "LIVEPLYR%d", i);
            if (IsCurrentGadgetNamed(gui, buf)) {
                OpenRejectDialog(i);
                return;
            }
        }
        if (IsCurrentGadgetNamed(gui, "WATCHING")) {
            info->watching = !info->watching;
            PlaySoundByName("Options", 0);
            info = g_game->players[g_game->localPlayer].info;
            SetButtonStageByName((Class_004a1080*)&g_game->menu, "WATCHING", info->watching);
            SetButtonStageByName((Class_004a1080*)&g_game->menu, "GAMEOPEN", !info->closed);
            FUN_0049fa90((Class_004a1080*)&g_game->menu);
            BroadcastPlayerInfo();
        } else if (IsCurrentGadgetNamed(gui, "OK")) {
            UpdateNetGameInfo();
            PlaySoundByName("Options", 0);
            if (!info->watching) {
                for (int i = 0; i < 10; i++) {
                    if (g_game->players[i].active != 0) {
                        if (g_game->players[i].type == 3) {
                            if (g_game->players[i].info->bit6) {
                                RejectPlayer(g_game->players[i].field_4, 9);
                            }
                        }
                    }
                }
            }
            return;
        }
        // Written once after the chain, not in each arm; the OK arm returns early.
        FUN_004ab0a0(gui);
    }
}

// Opens the CONTROL.GUI dialog (with HandleControlDialogClick as its handler) when the
// local player's info does not have bit 6 set, then sets the WATCHING and
// GAMEOPEN controls from that player's flags.

// FUNCTION: 0x4466b0
void OpenControlDialog()
{
    PlayerInfo_00444930* info = g_game->players[g_game->localPlayer].info;
    if (info->bit6) {
        return;
    }
    Holder_00444930* gadget = LoadGuiLayer(&g_game->menu, "CONTROL.GUI", 0x800);
    gadget->handler = HandleControlDialogClick;
    gadget->owner = g_game;
    RefreshAlliesScreen(1);
    info = g_game->players[g_game->localPlayer].info;
    SetButtonStageByName((Class_004a1080*)&g_game->menu, "WATCHING", info->watching);
    SetButtonStageByName((Class_004a1080*)&g_game->menu, "GAMEOPEN", !info->closed);
    FUN_0049fa90((Dialog*)&g_game->menu);
    FUN_0049fb10((Dialog*)&g_game->menu, 1);
    RenderLayer((Dialog*)&g_game->menu, 0x40);
}

// Returns whether two players are allied: alliance 5 means "no alliance".

// FUNCTION: 0x4467c0
bool __stdcall ArePlayersAllied(Player_00444930* a, Player_00444930* b)
{
    if (a->alliance == 5)
        return false;
    return a->alliance == b->alliance;
}

static inline int IsPlaying(Player_00444930* p)
{
    return p->active != 0 && (p->type == 1 || p->type == 2 || p->type == 3)
        && p->field_146 != 10;
}

static inline int IsCounted(Player_00444930* p)
{
    return (p->type == 1 || p->type == 2 || p->type == 3)
        && (p->field_144 != 0 || p->field_140 == 0);
}

static inline int CountAlliance(int alliance)
{
    if (alliance == 5)
        return 0;
    int count = 0;
    for (int i = 0; i < 10; i++) {
        Player_00444930* p = &g_game->players[i];
        if (g_game->bit2) {
            if (p->alliance == alliance && IsPlaying(p) && IsCounted(p))
                count++;
        } else {
            if (p->alliance == alliance && IsPlaying(p))
                count++;
        }
    }
    return count;
}

// Counts the players (0..9) that are still in the game and belong to `alliance`
// when the extra "counted" test applies. Same helper that 0x4468c0 inlines.

// FUNCTION: 0x4467f0
int __stdcall FUN_004467f0(int alliance)
{
    if (alliance == 5)
        return 0;
    int count = 0;
    for (int i = 0; i < 10; i++) {
        Player_00444930* p = &g_game->players[i];
        if (g_game->bit2) {
            if (p->alliance == alliance && IsPlaying(p) && IsCounted(p))
                count++;
        } else {
            if (p->alliance == alliance && IsPlaying(p))
                count++;
        }
    }
    return count;
}

// Returns true if some alliance (0..4) contains every counted player, i.e.
// the players still in the game are all on one side.

// With both calls in one expression MSVC calls the later-declared one first.
int CountHumanPlayers();
int CountComputerPlayers();

// FUNCTION: 0x4468c0
char FUN_004468c0()
{
    int total = CountComputerPlayers() + CountHumanPlayers();
    for (int alliance = 0; alliance < 5; alliance++) {
        if (CountAlliance(alliance) == total)
            return 1;
    }
    return 0;
}

// FUNCTION: 0x4469c0
int __stdcall FindNextAlly(int player, int start)
{
    Player_00444930* players = g_game->players;
    if (start == 10)
        return -1;
    for (int i = start; i < 10; i++) {
        if ((players[i].alliance == players[player].alliance && players[i].type != 0
             && players[i].alliance != 5) || i == player)
            return i;
    }
    return -1;
}

// Refreshes the "TEAMICONS%d" gadget for every active player: the name uses
// either the player index or a running icon counter, and the value comes from
// the player's alliance and how many players are still counted on that side.

// FUNCTION: 0x446a50
void RefreshTeamIcons()
{
    int i = 0;
    int teamIcon = 0;
    char buffer[0x40];

    for (; i < 10; i++) {
        Player_00444930* p = &g_game->players[i];
        if (IsPlaying(p) && p->type != 4
            && (!(g_game->flags_2a44 & 4) || IsCounted(p))
            && (!(g_game->flags_2a44 & 4) || p->info->field_96 != 0xff)) {
            if (g_game->bit2) {
                wsprintfA(buffer, "TEAMICONS%d", teamIcon);
                teamIcon++;
            } else {
                wsprintfA(buffer, "TEAMICONS%d", i);
            }

            int alliance = p->alliance;
            int count = CountAlliance(alliance);

            // Three separate calls: they tail-merge in the original.
            switch (count) {
            case 0:
                SetButtonStageByName((Class_004a1080*)&g_game->menu, buffer, 10);
                break;
            case 1:
                SetButtonStageByName((Class_004a1080*)&g_game->menu, buffer, alliance * 2 + 1);
                break;
            default:
                SetButtonStageByName((Class_004a1080*)&g_game->menu, buffer, alliance * 2);
                break;
            }
        }
    }
    g_game->flag0 = 1;
}

// Recomputes the per-player ally marks. For every active player it walks the
// players that share its alliance colour (FindNextAlly, inlined), sets the
// corresponding bytes of field_108/field_113 and bit 1 of the player info
// flags, then keeps that bit only when at least two players share the
// alliance (CountAlliance, inlined from 0x4468c0).

// The original's out-of-line FindNextAlly, inlined at its only call site.
static inline int FindNextAlly_00446c70(int player, int start)
{
    Player_00444930* players = g_game->players;
    if (start == 10)
        return -1;
    for (int i = start; i < 10; i++) {
        if ((players[i].alliance == players[player].alliance && players[i].type != 0
             && players[i].alliance != 5) || i == player)
            return i;
    }
    return -1;
}

// FUNCTION: 0x446c70
void FUN_00446c70()
{
    for (int i = 0; i < 10; i++) {
        // j and k before p: puts p in the base slot of the field stores.
        int j;
        int k;
        Player_00444930* p = &g_game->players[i];
        if (p->active == 0)
            continue;
        unsigned char type = p->type;
        if (type != 1 && type != 2 && type != 3)
            continue;
        if (p->field_146 == 10)
            continue;
        if (type == 4)
            continue;
        j = 0;
        while ((k = FindNextAlly_00446c70(i, j)) != -1) {
            j = k + 1;
            p->field_113[k] = 1;
            p->field_108[k] = 1;
            Player_00444930* q = &g_game->players[k];
            q->info->flags_9d |= 2;
            p->info->flags_9d |= 2;
        }
        if (CountAlliance(p->alliance) < 2)
            p->info->flags_9d &= 0xfffd;
    }
}

static inline int IsSelectable(Player_00444930* p)
{
    return (p->type == 1 || p->type == 2 || p->type == 3)
        && p->field_146 != 10;
}

// For every other player on the same side, marks the relation and clears
// bit 1 in player->info->flags_9d. The extra "g_game->players[i].type != 4"
// check is dead: IsSelectable already restricts type to 1, 2 or 3, so the
// condition can never be false; kept because the compiler emitted it.

// FUNCTION: 0x446e90
void __stdcall FUN_00446e90(Player_00444930* player)
{
    if (player->alliance != 5) {
        for (int i = 0; i < 10; i++) {
            Player_00444930* p = &g_game->players[i];
            if (g_game->players[i].active
                && IsSelectable(p)
                && g_game->players[i].type != 4
                && g_game->players[i].alliance == player->alliance
                && i != player->field_146) {
                SetAlliance(player->field_4, p->field_4, 0, 1);
                player->info->flags_9d &= 0xfffd;
            }
        }
    }
}
