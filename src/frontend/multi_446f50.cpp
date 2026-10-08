// Decompiled by Opus, deepseek-v4.1-flash, DeepSeek V4.1 Flash, deepseek-v4-flash, Space Bunny Free, space-bunny-free, LongCat 2.5 Preview Free, GPT-6, GPT-6.1-sol, GPT-5.6-Terra, claude-sonnet-5-5, Sonnet 5.5, claude-opus-5-5, Claude Opus 5.5, mimo-v2.6-pro and Haiku. Names are provisional.
// The multiplayer battle room and its dialogs (0x446f50 to 0x44ce20): the
// allies screen, the battle room's click handler and per-frame refresh, the
// end-of-multi screen, the save and load game lists and the unit
// restrictions dialog.
//
// Three functions stay in files of their own (0x449bb0, 0x44a680, 0x44c420):
// the notes where they belong in address order say why.
//
// <windows.h> and <stdio.h> shape the base/index choices and loop heads of
// several functions (0x447b10, 0x448c70, 0x44a680, 0x44c220); <string.h> and
// <stdlib.h> carry the rest.
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Dialog;
struct Class_004a1080;
struct Class_004a1450;
struct Class_0049fb10;
struct Struct_004c6ac0;

#pragma pack(push, 1)

// The per-player block g_game->players[i].info points at. Every file that
// touches it sees a different slice; this is the union of those slices.
struct PlayerInfo_00446f50 {
    char map[0x8b];                    // +0x00
    unsigned short width;              // +0x8b
    unsigned short height;             // +0x8d
    char unknown_8f[0x94 - 0x8f];
    char kind;                         // +0x94
    unsigned char side;                // +0x95
    unsigned char field_96;            // +0x96
    union {
        struct {
            union {
                unsigned char flags_97;  // +0x97 (0x44a680's byte test)
                struct {
                    unsigned char f97_0 : 1;  // +0x97
                    unsigned char f97_rest : 7;
                };
            };
            unsigned char unknown_98;  // +0x98
        };
        struct {
            unsigned short f97_0_wide : 1;  // +0x97 as the unsigned short bitfield
            unsigned short f97_rest_wide : 15;
        };
    };
    unsigned short memory;             // +0x99
    union {
        unsigned char flags_9b;        // +0x9b
        unsigned short flags;
        struct {
            unsigned short low : 4;    // +0x9b
            unsigned short started : 1;
            unsigned short ready : 1;
            unsigned short bit6 : 1;
            unsigned short watching : 1;
            unsigned short mapping : 1;
            unsigned short los : 1;
            unsigned short losType : 1;
            unsigned short commander : 2;
            unsigned short cheating : 1;
            unsigned short fixedloc : 1;
            unsigned short closed : 1;
        } b;
    };
    union {
        unsigned short flags_9d;       // +0x9d
        struct {
            unsigned short f9d_0 : 2;
            unsigned short f9d_2 : 1;
            unsigned short f9d_rest : 13;
        };
        struct {
            unsigned char f9d_0_byte : 2;
            unsigned char f9d_2_byte : 1;
            unsigned char f9d_rest_byte : 5;
        };
    };
    union {
        unsigned short pingLimit;      // +0x9f
        char unknown_9f[2];
    };
    unsigned short energy;             // +0xa1
    unsigned short metal;              // +0xa3
    unsigned short maxUnits;           // +0xa5
    unsigned char versionMajor;        // +0xa7
    unsigned char versionMinor;        // +0xa8
    unsigned int mapCrc;               // +0xa9
};

// The 0x14b-byte player record at g_game+0x1b63: the union of every view in
// this file (id and time for the battle room, ping for the rows, the ally
// byte arrays and the colour at +0x13f).
struct Player_00446f50 {
    int active;                        // +0x00
    int field_4;                       // +0x04
    unsigned int time;                 // +0x08
    char unknown_c[0x14 - 0xc];
    unsigned int ping;                 // +0x14
    char unknown_18[0x22 - 0x18];
    unsigned char field_22;            // +0x22
    char unknown_23[0x27 - 0x23];
    PlayerInfo_00446f50* info;         // +0x27
    char name[0x73 - 0x2b];            // +0x2b
    unsigned char type;                // +0x73
    char unknown_74[0x108 - 0x74];
    unsigned char field_108[0xb];      // +0x108
    unsigned char field_113[0xb];      // +0x113
    char unknown_11e[0x13f - 0x11e];
    unsigned char colour;              // +0x13f
    int field_140;                     // +0x140
    short field_144;                   // +0x144
    unsigned char field_146;           // +0x146
    char unknown_147[0x14b - 0x147];
};

// The mission object g_game->map points at (defined in the game's own files).
class Mission {
public:
    int GetTerrainLength();
    int GetTerrainSizeTier();
    int LoadMissionByName(char* map);
    void LoadMissionByName(PlayerInfo_00446f50* info);
    char* GetTranslatedName();
    char* GetMissionName();
    bool HasMissionName();
    unsigned int ComputeMapChecksum();
    void RefreshMapList(int param_1);
};

// One 0x62-byte record of the unit restrictions table at 0x5129b4.
struct Record_00446f50 {
    char name[0x52];                   // +0x00
    int field_52;                      // +0x52 unit type index
    int field_56;                      // +0x56 previous value
    int field_5a;                      // +0x5a value
    int field_5e;                      // +0x5e
};

// One 0x249-byte unit type instance of g_game->unitTypes: the name and
// description area, the costs and the bit at +0x245 that marks a type that
// cannot be restricted.
struct UnitType_00446f50 {
    char unitName[0x20];               // +0x00
    union {
        char name[0x225];              // +0x20
        struct {
            char nameShort[0x80];      // +0x20
            char description[0xe6];    // +0xa0
            float energyCost;          // +0x186
            float metalCost;           // +0x18a
            char unknown_18e[0x245 - 0x18e];
        };
        struct {
            char unknown_13e[0x13e - 0x20];
            int field_13e;             // +0x13e
        };
    };
    union {
        unsigned int raw;              // +0x245
        struct {
            unsigned int low : 15;
            unsigned int flag : 1;
            unsigned int high : 16;
        } bits;
        struct {
            unsigned int f245_low : 15;
            unsigned int f245_15 : 1;
            unsigned int f245_high : 16;
        };
    } field_245;
};

struct Event_44c220 {
    int field_0;                       // +0x00
    int field_4;                       // +0x04
    short field_8;                     // +0x08
    short field_a;                     // +0x0a
    int field_c;                       // +0x0c
};

struct Info_0044c7e0 {                  // filled by UnitSync::GetUnitEntry
    char unknown_0[0xa];
    short field_a;                      // +0x0a
    int field_c;                        // +0x0c
};

class UnitSync {
public:
    int SendAllQueued(int value);
    int IsPlayerSynced(int id);
    int AllPlayersSynced();
    void ProcessSync();
    char* GetSyncStatusText();
    void CheckUnitAvailable(unsigned int, int);
    int PopChangedEntry(Event_44c220* event);
    void SetUnitLimit(UnitType_00446f50* unit, int value);
    int DisallowUnit(UnitType_00446f50* unit);
    int AllowUnit(UnitType_00446f50* unit);
    int GetUnitEntry(UnitType_00446f50* type, Info_0044c7e0* out);
};

struct Options_00446f50 {
    char unknown_0[0x118];
    int fixedloc;                      // +0x118
};

// The 0x15b-byte GUI control record: entry 0 holds the count at +0xb6, an
// ordinary entry its text there, a bound entry its callback at +0xce, and a
// slider entry its bounds at +0x13c and its value at +0x140.
struct Entry_00446f50 {
    unsigned char field_0;             // +0x00
    char unknown_1;                    // +0x01
    char name[0x10];                   // +0x02
    char unknown_12[0x15 - 0x12];
    short field_15;                    // +0x15
    short field_17;                    // +0x17
    short height;                      // +0x19 (the OUTPUT list's visible height)
    int field_1b;                      // +0x1b
    unsigned int field_1f;             // +0x1f (0x44a680's colour)
    int colour;                        // +0x23
    char unknown_27[0x29 - 0x27];
    unsigned char visible;             // +0x29
    char unknown_2a[0xb6 - 0x2a];
    union {
        char text[0x15b - 0xb6];       // +0xb6
        struct {
            char unknown_b6[0xcc - 0xb6];
            char label[0x15b - 0xcc];  // +0xcc
        };
        struct {
            short count;               // +0xb6
            char unknown_b8[8];
            void* gaf;                 // +0xc0
        } head;
        struct {
            char unknown_b6[8];
            union {
                void* frames;          // +0xbe
                unsigned short* frameCount;
            };
        } anim;
        struct {
            char unknown_b6[8];
            union {
                int field_be;          // +0xbe
                struct {
                    short unknown_be;
                    short count;       // +0xc0
                } list;
            };
        };
        struct {
            char unknown_b6[0xba - 0xb6];
            union {
                short selected;        // +0xba
                short field_ba;
            };
            short field_bc;            // +0xbc
            char unknown_be[0xc0 - 0xbe];
            short count;               // +0xc0
            void* field_c2;            // +0xc2
            union {
                struct {
                    short frame;           // +0xc6
                    union {
                        unsigned int field_c8;  // +0xc8
                        struct {
                            unsigned int c8_0 : 1;
                            unsigned int c8_rest : 31;
                        };
                    };
                };
                struct {
                    void* field_c6;        // +0xc6
                    char unknown_ca[0xcc - 0xca];
                };
            };
            char unknown_cc[0xce - 0xcc];
            void* field_ce;            // +0xce
            Record_00446f50* records;  // +0xd2
            union {
                char* flags;           // +0xd6
                unsigned char* bits;
            };
            short field_da;            // +0xda
            char unknown_dc[0x138 - 0xdc];
            unsigned short field_138;  // +0x138
            char unknown_13a[0x13c - 0x13a];
            union {
                int max;               // +0x13c
                int field_13c;
                struct {
                    unsigned short b13c_0 : 1;
                    unsigned short b13c_rest : 15;
                };
            };
            short value;               // +0x140
            short unknown_142;         // +0x142
            union {
                void* callback;        // +0x144
                void (__stdcall* field_144)(void*, int);
            };
            char unknown_148[2];
            union {
                void* game;            // +0x14a
                int field_14a;
            };
            char unknown_14e[0x15b - 0x14e];
        };
    };
};

// The layer LoadGuiLayer returns and g_game->gui.table points at: the entry
// table at +4, the click handler at +8 and the dialog's block at +0xc.
struct Layer_00446f50 {
    Layer_00446f50* unknown_0;         // +0x00
    Entry_00446f50* entries;           // +0x04
    void* handler;                     // +0x08
    union {
        int field_c;                   // +0x0c
        void* data;
    };
    char unknown_10[0x1c - 0x10];
    void* field_1c;                    // +0x1c
    int field_20;                      // +0x20
};

// The menu object at g_game+0x519: the layer at +0x18 and the id of the
// clicked entry (-1 when the menu closes) at +0x60.
struct Gui_00446f50 {
    char unknown_0[0x18];              // +0x00
    Layer_00446f50* table;             // +0x18
    char unknown_1c[0x60 - 0x1c];      // +0x1c
    int current;                       // +0x60
};

typedef void (__stdcall* Callback_00449bb0)(Gui_00446f50* gui, int index);
typedef void (__stdcall* Callback_0044c7e0)(Gui_00446f50* gui, int index);

struct Game {
    char unknown_0[0x499];             // +0x00
    int field_499;                     // +0x499
    char unknown_49d[0x519 - 0x49d];
    Gui_00446f50 gui;                  // +0x519
    char unknown_57d[0x12ef - 0x57d];
    char messages[30][0x48];           // +0x12ef
    char unknown_1b5f[0x1b63 - 0x1b5f];
    Player_00446f50 players[10];       // +0x1b63
    char unknown_2851[0x29a0 - 0x2851];
    Options_00446f50* options;         // +0x29a0
    char unknown_29a4[0x2a30 - 0x29a4];
    UnitSync* net;                     // +0x2a30
    char unknown_2a34[0x2a3c - 0x2a34];
    unsigned short field_2a3c;         // +0x2a3c
    unsigned short scrollEnd;          // +0x2a3e
    unsigned short scrollStart;        // +0x2a40
    unsigned char localPlayer;         // +0x2a42
    char unknown_2a43;                 // +0x2a43
    unsigned char flags_2a44;          // +0x2a44
    char unknown_2a45[0x2a9b - 0x2a45];
    char* chatter;                     // +0x2a9b
    char unknown_2a9f[0x2bc0 - 0x2a9f];
    char state;                        // +0x2bc0
    char unknown_2bc1[0x2bee - 0x2bc1];
    union {                            // +0x2bee
        struct {
            unsigned short dirty : 1;
            unsigned short dirty_rest : 15;
        };
        struct {
            unsigned short bits0 : 4;
            unsigned short flag4 : 1;
            unsigned short bits5 : 11;
        };
    };
    char unknown_2bf0[0x2c28 - 0x2bf0];
    int field_2c28[11];                // +0x2c28
    char unknown_2c54[0x2c74 - 0x2c54];
    unsigned short locked : 1;         // +0x2c74
    unsigned short locked_rest : 15;
    char unknown_2c76[0x1438f - 0x2c76];
    int count;                         // +0x1438f
    char unknown_14393[0x1439b - 0x14393];
    UnitType_00446f50* unitTypes;      // +0x1439b
    char unknown_1439f[0x148db - 0x1439f];
    int field_148db;                   // +0x148db
    char unknown_148df[0x37e1b - 0x148df];
    int field_37e1b;                   // +0x37e1b
    char unknown_37e1f[0x37ebe - 0x37e1f];
    unsigned short field_37ebe;        // +0x37ebe
    char unknown_37ec0[0x37ee8 - 0x37ec0];
    unsigned short field_37ee8;        // +0x37ee8
    unsigned short maxUnits;           // +0x37eea
    unsigned short field_37eec;        // +0x37eec
    int field_37eee;                   // +0x37eee
    char unknown_37ef2[0x37ef6 - 0x37ef2];
    int field_37ef6;                   // +0x37ef6
    char unknown_37efa[0x37f1b - 0x37efa];
    unsigned short width;              // +0x37f1b
    char unknown_37f1d[2];
    unsigned short height;             // +0x37f1f
    char unknown_37f21[0x37f39 - 0x37f21];
    int sides;                         // +0x37f39
    char unknown_37f3d[0x38a47 - 0x37f3d];
    int frame;                         // +0x38a47
    char unknown_38a4b[0x38a51 - 0x38a4b];
    union {                            // +0x38a51
        unsigned short flag_38a51 : 1;
        unsigned short bits_38a51 : 15;
    };
    char unknown_38a53[0x38c6b - 0x38a53];
    char save_38c6b[0x100];            // +0x38c6b
    char unknown_38d6b[0x391e9 - 0x38d6b];
    Mission* map;                      // +0x391e9
    char unknown_391ed[0x39229 - 0x391ed];
    int commander;                     // +0x39229
    int mapping;                       // +0x3922d
    int los;                           // +0x39231
    int losType;                       // +0x39235
    char unknown_39239[0x3923b - 0x39239];
    unsigned short bits0_3923b : 2;    // +0x3923b
    unsigned short flag2_3923b : 1;
    unsigned short bit3_3923b : 1;
    unsigned short flag4_3923b : 1;
    unsigned short rest_3923b : 11;
};

// The player record seen as an object by the two slot shufflers.
class Player {
public:
    void SetType(int param_1);
};

class PacketManager {
public:
    int SendAllQueued(int value);
};

// The 0x18-byte unit portrait record 0x44c0d0 fills one at a time.
struct Record_0044c0d0 {
    unsigned short a;                  // +0x00
    unsigned short b;                  // +0x02
    unsigned short e;                  // +0x04
    unsigned short f;                  // +0x06
    unsigned char flag8;               // +0x08
    unsigned char flag9;               // +0x09
    unsigned char flaga;               // +0x0a
    unsigned char flagb;               // +0x0b
    int unknown_c;                     // +0x0c
    int d;                             // +0x10
    int unknown_14;                    // +0x14
};

class Class_0044ce20
{
public:
    void* vtable;
    int field_4;

    Class_0044ce20(int param_1);
};

#pragma pack(pop)

// GLOBAL: 0x511de8
extern Game* g_game;
extern int DAT_00512764;
extern int DAT_00512768;
extern int DAT_00512978;
extern int DAT_0051297c;
extern int DAT_00512994;
extern int DAT_005129a4;
extern unsigned int DAT_005129a8;
extern char* DAT_005129ac;
extern char* DAT_005129b0;
extern int* DAT_005129b8;
extern int DAT_005129c0;
extern int* DAT_005129c4;
// GLOBAL: 0x5129c8
extern int DAT_005129c8;
extern unsigned int DAT_0050550c;
extern char* DAT_005091c8;             // savegame directory
extern char DAT_005119b8[];
extern char DAT_0050372c[];            // "*"
extern char DAT_00505f40[];            // "LST"
extern char DAT_00505f18[];            // "GAMES"
extern char DAT_00505f30[];            // "SAVEGAME NAMES"
extern char DAT_00505f20[];            // "SAVEGAME DESCS"
extern char* DAT_00505518[];
extern char* DAT_005054b0[];
extern char DAT_00512ce8[];
extern int DAT_00512d68;
extern int DAT_00512d6c;
extern int DAT_00512d70;
extern int DAT_00512d74;
extern int DAT_00512d78;
extern int DAT_00512d7c;
extern int DAT_00512d80;
extern int DAT_00512d84;
extern int DAT_00512d88;
extern int DAT_00512d8c;
extern void* DAT_004fd2f8;
extern int g_usePacketManager;
extern PacketManager g_packetManager;
// GLOBAL: 0x5129b4
extern Record_00446f50* DAT_005129b4;

// GUI library entry points.
int __stdcall IsCurrentGadgetNamed(void* gadget, const char* name);
Entry_00446f50* __stdcall FindGadgetChecked(void* entries, const char* name);
Entry_00446f50* __stdcall FindGadgetOrNull(void* entries, const char* name);
Entry_00446f50* __stdcall FUN_004a0010(void* entries, const char* name);
Entry_00446f50* __stdcall FUN_004a0180(void* entries, const char* name);
Entry_00446f50* __stdcall FUN_004a0200(void* entries, const char* name);
Entry_00446f50* __stdcall FUN_004a0280(void* entries, const char* name);
int __stdcall FindGadgetIndex(void* entries, const char* name, int type);
int __stdcall GetButtonStage(void* gadget, int index);
int __stdcall GetButtonStageByName(void* gadget, char* name);
int __stdcall GetGadgetStatus(void* gui, int index);
int __stdcall SetButtonStageByName(void* gui, char* name, int value);
void __stdcall SetGadgetStatusByName(void* gui, char* name, int value);
void __stdcall SetGadgetName(void* gui, char* name, char* text);
void __stdcall SetGadgetText(void* gui, int index, char* text);
void __stdcall SetGadgetRows(void* table, char* name, int* pics, int count);
void __stdcall DrawButton(void* gadget, int value);
void __stdcall FUN_004a0570(void* gui, char* name, int value);
void __stdcall FUN_004a0bf0(void* gui, char* name, char* text, int size);
void __stdcall FUN_004a1250(void* gui, char* name, int value);
void __stdcall FUN_004a1450(void* gui, char* name, int value);
void __stdcall FUN_004a32a0(void* gui, char* name, char* text, int count, int flag);
void __stdcall FUN_004a7190(void* gui, int index);
void __stdcall FUN_0049fa90(void* gui);
void __stdcall FUN_0049fa50(void* gui);
void __stdcall FUN_0049fb10(void* gui, int value);
void __stdcall FUN_004a5d30(void* gui, int flag);
void __stdcall FUN_004a5d50(void* gui, int index);
void __stdcall RenderLayer(void* gui, int value);
void __stdcall CloseTopScreen(void* gui);
int __stdcall IsScreenNamed(void* gui, const char* name);
void __stdcall SetSliderFromValue(Entry_00446f50* gadget, int value);
int __stdcall ReadSliderValue(void* gadget);
void __stdcall SetAlliance(int a, int b, unsigned char allied, int d);
void __stdcall FUN_004ab0a0(void* gadget);
void __stdcall FUN_004ab190(void* gui, int flag);
void __stdcall OpenMessageBox(void* gui, const char* text, int a, int b, int c);
Layer_00446f50* __stdcall LoadGuiLayer(void* gui, const char* name, int flags);
void __stdcall HandleAlliesClick(Gui_00446f50* gadget);
void __stdcall HandleBattleRoomClick(Gui_00446f50* gadget);
void __stdcall HandleEndMultiClick(Gui_00446f50* gadget);
void __stdcall HandleLoadListClick(Gui_00446f50* menu);
void __stdcall HandleRestrictionsClick(Gui_00446f50* menu);
void __stdcall HandleSaveGameClick(Gui_00446f50* menu);
void __stdcall HandleUnitCountSlider(void* obj, char* gadget);
void __stdcall UpdateSideGadget(int side);
void __stdcall UpdateUnitSliders(Gui_00446f50* gui, int value);
void __stdcall UpdateMaxUnitsText(Gui_00446f50* gui, int index);
void __stdcall UpdateMetalText(Gui_00446f50* gui, int index);
void RefreshBattleRoomRows();
void RebuildAllyList();
void __stdcall RefreshAlliesScreen(int value);
void OpenLoadListDialog();
void OpenSaveGameDialog();
void OpenUnitRestrictions();
void FUN_0044c220();

void __stdcall SetFrontendState(int a, int line, char* file);
void __stdcall SetGameMode(int a);
void LeaveNetGame();
void BlankScreen();
void* __stdcall LoadBitmapByName(char* name, unsigned char* palette);
void __stdcall SetPaletteColors(unsigned char* palette, int first, int count);
void __stdcall SetOffscreenSurface(int param_1);
void __stdcall DrawSurface(void* dest, void* image, int x, int y);
void __stdcall FreeSurface(void* image);
void FlipScreen();
void ShowSoftwareCursor();
void __stdcall MakeDirectoryPath(char* path);
void __stdcall RemoveFile(char* path);
void __stdcall BuildDataPath(char* out, const char* dir, const char* name, const char* ext);
int __stdcall CountDirectoryEntries(const char* path, int flag);
int __stdcall ScanDirectory(char* path, void* buffer, char* p3, int p4, int p5, int p6);
void* __cdecl FUN_004d83b0(char* name, unsigned int size);
void __cdecl FUN_004d85a0(void* p);
void __stdcall SaveSettings();
void ShowSelectedMapInfo();
void __stdcall HandleViewMapClick(void* gadget);
void OpenMultiMapSelector();
void __stdcall OpenRejectDialog(int index);
void FUN_00446310();
void __stdcall FUN_004455b0();
void __stdcall RefreshTeamIcons();
void FUN_00446c70();
void __stdcall FUN_00446e90(Player_00446f50* player);
void __stdcall FUN_00452bd0(Player_00446f50* player);
void __stdcall BroadcastPlayerInfo();
void __stdcall UpdateNetGameInfo();
void __stdcall UpdateBattleRoomFlags();
unsigned char __stdcall FindHostSlot();
int IsHostLocal();
int CountHumanPlayers();
int CountComputerPlayers();
int CountLocalComputerPlayers();
unsigned int GetTicks();
void __stdcall AddMessage(char* text, int a, int b, int c);
void __stdcall SendChatMessage(void* from, char* text, int a, int b);
void __stdcall ReportGameEvent(int sound);
void __stdcall PlaySoundByName(char* sound, int b);
int __stdcall LoadPictureCached(char* name, int a, int b, int c);
int __stdcall GetSlotDpid(unsigned char player);
void __stdcall RejectPlayer(int id, unsigned char msg);
int __stdcall CreateLocalPlayer(unsigned char player, int state);
void __stdcall RequestPlayerColor(int slot);
int __stdcall GetFontLineHeight();
char* __stdcall SkipTextLines(char* text, int n);
int __stdcall GetTextPixelWidth(char* text);
void __stdcall FUN_004a15c0(Entry_00446f50* entries, int widget, RECT* rect);
void __stdcall FUN_004a50e0(int a, char* text, int x, int y, int w, int h);
int FUN_00456760();
int IsOnlineConfigLoaded();
void __stdcall CreateUnitSync(int param_1);
char __stdcall FindGameCdDrive(int side);
int __stdcall HandleNetPackets();
void __stdcall FUN_0049fad0(void* gui);
void __stdcall SendNetHeartbeat();
void* __stdcall LoadPcx(char* path, int param_2);
void __stdcall FrameFromSurface(void* dst, void* src);
void* __stdcall FindGafEntry(void* gaf, const char* name);
void FatalError(char* message);
char* __stdcall Translate(char* text);
void FUN_00428b60();

// FUNCTION: 0x446f50
void __stdcall CyclePlayerAlliance(int index)
{
    int colour = g_game->players[index].colour;
    Player_00446f50* player = &g_game->players[index];
    FUN_00446e90(player);
    player->colour = (colour + 1) % 6;
    FUN_00452bd0(player);
    FUN_00446c70();
    RefreshTeamIcons();
}

__inline int IsLiveType_00446fb0(Player_00446f50* p)
{
    if (p->type != 1 && p->type != 2 && p->type != 3)
        return 0;
    return 1;
}

// Players are passed by pointer with no local copy: keeps g_game in eax.
__inline int IsAlly_00446fb0(Player_00446f50* p)
{
    // First active test reads a local, the second reads p->active: keeps both tests.
    int act = p->active;
    if (!act)
        return 0;
    if (p->info->flags_9b & 0x40)
        return 0;
    if (!p->active)
        return 0;
    if (!IsLiveType_00446fb0(p))
        return 0;
    if (p->field_146 == 10)
        return 0;
    if (!IsLiveType_00446fb0(p))
        return 0;
    if (p->field_144 == 0 && p->field_140 != 0)
        return 0;
    return 1;
}

// Separate from IsAlly: runs the type chain once.
__inline int IsLive_00446fb0(Player_00446f50* p)
{
    if (!p->active)
        return 0;
    if (!IsLiveType_00446fb0(p))
        return 0;
    if (p->field_146 == 10)
        return 0;
    if (p->field_144 == 0 && p->field_140 != 0)
        return 0;
    if (p->info->field_96 == 0xff)
        return 0;
    return 1;
}

// FUNCTION: 0x446fb0
void RebuildAllyList()
{
    // 52 bytes, not 44: sets the frame size.
    char text[52];
    // No lp local: both pointers are built from g_game->players[...] directly.
    unsigned char* a = &g_game->players[g_game->localPlayer].field_108[0];
    unsigned char* b = &g_game->players[g_game->localPlayer].field_113[0];

    if (IsScreenNamed(&g_game->gui, "ALLIES.GUI") != 0) {
        int i;
        // Pointer induction variables, not a[i] / b[i]; i < 10, not i != 10.
        for (i = 0; i < 10; ++i, ++a, ++b) {
            if (IsAlly_00446fb0(&g_game->players[i]) && i != g_game->localPlayer
                && IsLive_00446fb0(&g_game->players[i])) {
                sprintf(text, "LIVEALLY%d", i);
                SetButtonStageByName(&g_game->gui, text, (*b << 1) | *a);
            }
        }
        FUN_0049fa90(&g_game->gui);
    }
}

// FUNCTION: 0x447150
void __stdcall HandleAlliesClick(Gui_00446f50* gadget)
{
    void* entries = gadget->table->entries;
    char buf[100];

    if (gadget->current == -1) {
        g_game->field_37ebe &= 0xffdf;
        return;
    }

    int i = 0;
    Player_00446f50* local = &g_game->players[g_game->localPlayer];

    for (; i < 10; i++) {
        sprintf(buf, "LIVEALLY%d", i);
        Player_00446f50* p = &g_game->players[i];
        if (IsCurrentGadgetNamed(gadget, buf) && p->active
            && (p->type == 1 || p->type == 2 || p->type == 3)
            && p->field_146 != 10) {
            PlaySoundByName("Options", 0);
            SetAlliance(local->field_4, p->field_4, local->field_108[i] ^= 1, 0);
            char* verb = local->field_108[i] ? "allied with" : "broke alliance with";
            sprintf(buf, " %s %s", Translate(verb),
                    (char*)g_game + 0x1b8e + i * 0x14b);
            SendChatMessage(local, buf, 4, 0);
            RebuildAllyList();
            DrawButton(&g_game->gui, gadget->current);
        }
    }

    if (IsCurrentGadgetNamed(gadget, "VICTORY")) {
        PlaySoundByName("Options", 0);
        FUN_004ab0a0(gadget);
        return;
    }
    if (IsCurrentGadgetNamed(gadget, "OK")) {
        PlaySoundByName("Options", 0);
        int old = (local->info->flags_9d >> 1) & 1;
        int index = FindGadgetIndex(entries, "VICTORY", 1);
        unsigned int value = GetButtonStage(gadget, index);
        local->info->flags_9d = (local->info->flags_9d & 0xfffd) | ((value & 1) << 1);
        if (old != ((local->info->flags_9d >> 1) & 1))
            BroadcastPlayerInfo();
    } else {
        FUN_004ab0a0(gadget);
    }
}

static inline int IsType_00447380(Player_00446f50* p)
{
    return p->type == 1 || p->type == 2 || p->type == 3;
}

static inline int IsCounted_00447380(Player_00446f50* p)
{
    if (!IsType_00447380(p))
        return 0;
    if (p->field_144 == 0 && p->field_140 != 0)
        return 0;
    return 1;
}

static inline int IsWatching_00447380(Player_00446f50* p)
{
    return p->active != 0 && (p->info->flags_9b & 0x40);
}

static inline int IsActive_00447380(Player_00446f50* p)
{
    return p->active != 0
        && (p->type == 1 || p->type == 2 || p->type == 3)
        && p->field_146 != 10;
}

// FUNCTION: 0x447380
void __stdcall RefreshAlliesScreen(int param_1)
{
    Entry_00446f50* entries = g_game->gui.table->entries;
    int i;
    int n;
    Player_00446f50* local = &g_game->players[g_game->localPlayer];
    char player[20];
    char teamicons[20];
    char ally[20];
    char live[20];
    char logo[20];
    char name[0x80];

    for (i = 0, n = 0; i < 10; i++) {
        sprintf(player, "PLAYER%d", i);
        sprintf(logo, "LOGO%d", i);
        sprintf(ally, "ALLY%d", i);
        sprintf(teamicons, "TEAMICONS%d", i);
        FUN_004a0570((char*)g_game + 0x519, player, 0);
        FUN_004a0570((char*)g_game + 0x519, logo, 0);
        FUN_004a0570((char*)g_game + 0x519, ally, 0);
        FUN_004a0570((char*)g_game + 0x519, teamicons, 0);

        Player_00446f50* p = &g_game->players[i];
        // Two helpers, not one: gives the register rotation of the second sprintf group.
        if (!IsWatching_00447380(p) && IsActive_00447380(p)
            && (i != g_game->localPlayer || param_1 == 0)
            && (!(g_game->flags_2a44 & 4) || IsCounted_00447380(p))
            && p->info->field_96 != 0xff) {
            sprintf(player, "PLAYER%d", n);
            sprintf(logo, "LOGO%d", n);
            sprintf(ally, "ALLY%d", n);
            sprintf(teamicons, "TEAMICONS%d", n);
            lstrcpynA(name, p->name, 0x80);

            int idx = FindGadgetIndex(entries, player, 0xe);
            if (entries[idx].field_0 == 1) {
                Entry_00446f50* e = FindGadgetOrNull(entries, player);
                if (e != 0 && (e->field_1b & 0x4000)) {
                    strcat(name, "|");
                    strcat(name, p->name);
                }
            }

            FUN_004a0bf0((char*)g_game + 0x519, player, name, 0x80);
            FUN_004a0570((char*)g_game + 0x519, player, 1);
            sprintf(live, "LIVEPLYR%d", i);
            SetGadgetName((char*)g_game + 0x519, player, live);

            if (p->active != 0
                && IsType_00447380(p)
                && p->field_146 != 10
                && (p->field_144 != 0 || p->field_140 == 0)
                && p->type != 1
                && p->type != 2
                && !(p->type == 3 && p->info->kind == 2)) {
                Player_00446f50* q = &g_game->players[g_game->localPlayer];
                if (q->active != 0
                    && IsType_00447380(q)
                    && q->field_146 != 10
                    && (q->field_144 != 0 || q->field_140 == 0)) {
                    FUN_004a0570((char*)g_game + 0x519, ally, 1);
                }
            }

            sprintf(live, "LIVEALLY%d", i);
            SetGadgetName((char*)g_game + 0x519, ally, live);

            if (p->colour == local->colour && p->colour != 5) {
                FUN_004a1450((char*)g_game + 0x519, live, 1);
            }

            FUN_004a0570((char*)g_game + 0x519, teamicons, 1);

            int value;
            if (p->active != 0 && (p->type == 1 || p->type == 2)
                && !(g_game->flags_2a44 & 4)) {
                value = 0;
            } else {
                value = 1;
            }
            FUN_004a1450((char*)g_game + 0x519, teamicons, value);

            Entry_00446f50* e2 = FUN_004a0280(entries, logo);
            if (e2 != 0) {
                e2->visible = 1;
                e2->field_be = g_game->field_148db;
                e2->frame = p->info->field_96;
                e2->field_c8 &= ~1;
            }

            n++;
        }
    }
}

static inline int IsPlaying_004478b0(Player_00446f50* p)
{
    return p->active != 0
        && (p->type == 1 || p->type == 2 || p->type == 3)
        && p->field_146 != 10;
}

// Repeats the type test of IsPlaying (dead): must stay.
static inline int IsCounted_004478b0(Player_00446f50* p)
{
    return (p->type == 1 || p->type == 2 || p->type == 3)
        && (p->field_144 != 0 || p->field_140 == 0);
}

static inline int CountAlliance_004478b0(int alliance)
{
    int count = 0;
    unsigned char f = (g_game->flags_2a44 >> 2) & 1;
    for (int j = 0; j < 10; j++) {
        Player_00446f50* q = &g_game->players[j];
        if (f) {
            if (q->colour == alliance && IsPlaying_004478b0(q)
                && IsCounted_004478b0(q))
                count++;
        } else {
            if (q->colour == alliance && IsPlaying_004478b0(q))
                count++;
        }
    }
    return count;
}

// FUNCTION: 0x4478b0
void OpenAlliesDialog()
{
    Layer_00446f50* gadget = LoadGuiLayer(&g_game->gui, "ALLIES.GUI", 0x800);
    gadget->handler = HandleAlliesClick;
    gadget->field_c = (int)g_game;
    g_game->field_37ebe |= 0x20;
    char* entries = (char*)g_game->gui.table->entries;
    int i, j;
    for (i = 0; (j = FindGadgetIndex(entries, "ALLYx", 0xe)) != -1; i++)
        sprintf(entries + j * 0x15b + 2, "ALLY%d", i);
    for (i = 0; (j = FindGadgetIndex(entries, "TEAMICONSx", 0xe)) != -1; i++)
        sprintf(entries + j * 0x15b + 2, "TEAMICONS%d", i);
    RefreshAlliesScreen(0);
    RebuildAllyList();
    RefreshTeamIcons();
    Player_00446f50* local = &g_game->players[g_game->localPlayer];
    int old = (local->info->flags_9d >> 1) & 1;
    unsigned char win = (local->info->flags_9b >> 6) & 1;
    SetButtonStageByName((Class_004a1080*)&g_game->gui, "VICTORY", old);
    int alliance = local->colour;
    int count;
    if (alliance == 5)
        count = 0;
    else
        count = CountAlliance_004478b0(alliance);
    FUN_004a1450((Class_004a1450*)&g_game->gui, "VICTORY",
                 (count > 1 || win) ? 1 : 0);
    FUN_0049fb10((Class_0049fb10*)&g_game->gui, 1);
    RenderLayer((Dialog*)&g_game->gui, 0x40);
}

static inline int IsPlaying_00447b10(Player_00446f50* p)
{
    return p->active != 0
        && (p->type == 1 || p->type == 2 || p->type == 3)
        && p->field_146 != 10;
}

static inline int IsCounted_00447b10(Player_00446f50* p)
{
    return (p->type == 1 || p->type == 2 || p->type == 3)
        && (p->field_144 != 0 || p->field_140 == 0);
}

static inline int IsLocalHuman_00447b10(Player_00446f50* p)
{
    return p->active != 0 && p->type == 1;
}

static inline int IsRemoteHuman_00447b10(Player_00446f50* p)
{
    return p->active != 0 && p->type == 3 && p->info->kind == 1;
}

static inline int IsLocal_00447b10(Player_00446f50* p)
{
    return p->active != 0 && (p->type == 1 || p->type == 2);
}

static inline int CountAlliance_00447b10(int alliance)
{
    if (alliance == 5)
        return 0;
    int count = 0;
    unsigned char f = (g_game->flags_2a44 >> 2) & 1;
    for (int j = 0; j < 10; j++) {
        Player_00446f50* q = &g_game->players[j];
        if (f) {
            if (q->colour == alliance && IsPlaying_00447b10(q) && IsCounted_00447b10(q))
                count++;
        } else {
            if (q->colour == alliance && IsPlaying_00447b10(q))
                count++;
        }
    }
    return count;
}

// The free slot search at 0x440c10, which has no callers. MSVC inlines it only
// when it is declared inline (it has two loops).
inline int FindUnusedLogo()
{
    int used[10];
    memset(used, 0, sizeof(used));
    for (int i = 0; i < 10; i++) {
        Player_00446f50* p = &g_game->players[i];
        if (p->active && (p->type == 1 || p->type == 2 || p->type == 3) && p->field_146 != 10)
            used[p->info->field_96 < 9 ? p->info->field_96 : 9] = 1;
    }
    int result = 0;
    for (int j = 0; j < 10; j++) {
        if (!used[j]) {
            result = j;
            break;
        }
    }
    return result;
}

// The map check at 0x440cd0, which has no callers: /Ob2 inlined it.
int CheckMapCrc_00447b10()
{
    if (!g_game->map->GetTerrainLength()) {
        return 0;
    }
    unsigned char me = FindHostSlot();
    PlayerInfo_00446f50* data = 0;
    int check = 0;
    if (me != 10) {
        data = g_game->players[me].info;
        // Version test form must stay: `>= 2`, else `== 1 && minor >= 2`.
        if (data->versionMajor >= 2)
            check = 1;
        else if (data->versionMajor == 1 && data->versionMinor >= 2)
            check = 1;
    }
    if (!check) {
        return 1;
    }
    if (g_game->map->ComputeMapChecksum() != data->mapCrc)
        return 0;
    return 1;
}

// The colour cycle at 0x446f50, which has no callers: /Ob2 inlined it.
void __stdcall CyclePlayerAlliance_00447b10(int index)
{
    // Declared before colour.
    Player_00446f50* player = &g_game->players[index];
    int colour = g_game->players[index].colour;
    FUN_00446e90(player);
    player->colour = (colour + 1) % 6;
    FUN_00452bd0(player);
    FUN_00446c70();
    RefreshTeamIcons();
}

// FUNCTION: 0x447b10
void __stdcall HandleBattleRoomClick(Gui_00446f50* gadget)
{
    // 249 or 250 bytes: puts used[] of the inlined FindUnusedLogo above the text.
    char text[250];
    Entry_00446f50* entries = gadget->table->entries;

    if (gadget->current == -1) {
        FUN_004d85a0(g_game->chatter);
        g_game->chatter = 0;
        DAT_00512994 = 0;
        FUN_00446c70();
        return;
    }

    int lp = g_game->localPlayer;
    Player_00446f50* me = &g_game->players[lp];
    int canAdd = IsHostLocal();
    int i = 0;
    // Not a for loop: that gives the tail's registers to the wrong values.
    while (1) {
        Player_00446f50* p = &g_game->players[i];

        sprintf(text, "LOGO%d", i);
        if (IsCurrentGadgetNamed(gadget, text) && IsLocal_00447b10(p)) {
            PlaySoundByName("Multi", 0);
            RequestPlayerColor(p->info->field_96 + 1);
            g_game->dirty = 1;
            BroadcastPlayerInfo();
        }

        sprintf(text, "PLAYER%d", i);
        if (IsCurrentGadgetNamed(gadget, text) && i != lp) {
            PlaySoundByName("Multi", 0);
            char type = p->type;
            if (type == 0 && canAdd) {
                // SetType is a Player method: called through a cast, as 0x445450 does.
                ((Player*)p)->SetType(4);
                p->field_4 = -1;
                g_game->field_499--;
            } else if (type != 4 && type != 0) {
                if (p->active != 0 && type == 2 && GetTicks() - p->time > 30) {
                    RejectPlayer(p->field_4, 1);
                    ((Player*)p)->SetType(0);
                } else if (canAdd && p->active != 0 && p->type == 3) {
                    OpenRejectDialog(i);
                }
            } else {
                if (type == 4) {
                    ((Player*)p)->SetType(0);
                    g_game->field_499++;
                    UpdateNetGameInfo();
                }
                if (g_game->players[FindHostSlot()].info->b.closed) {
                    OpenMessageBox(&g_game->gui, Translate("Can't add another player when game is closed."), 500, 1, 1);
                    ((Player*)p)->SetType(0);
                    g_game->dirty = 1;
                    break;
                }
                if (g_game->players[FindHostSlot()].info->b.commander != 2 && !CountLocalComputerPlayers()) {
                    CreateLocalPlayer(i, 2);
                    p->info->field_96 = FindUnusedLogo();
                }
            }
            g_game->dirty = 1;
            UpdateNetGameInfo();
            BroadcastPlayerInfo();
        }

        sprintf(text, "SIDE%d", i);
        if (IsCurrentGadgetNamed(gadget, text)) {
            PlaySoundByName("Multi", 0);
            if (p->active != 0 && p->info->b.bit6) {
                p->info->b.bit6 = 0;
                p->info->side = 0;
            } else {
                p->info->side++;
                if (p->info->side >= g_game->sides) {
                    p->info->side = 0;
                    if (g_game->players[FindHostSlot()].info->b.watching
                        && p->active != 0 && p->type == 1) {
                        p->info->b.bit6 = 1;
                    } else {
                        SetButtonStageByName(gadget, text, 0);
                        DrawButton(gadget, gadget->current);
                    }
                }
            }
            g_game->dirty = 1;
            ReportGameEvent(4);
            BroadcastPlayerInfo();
        }

        sprintf(text, "ALLY%d", i);
        if (IsCurrentGadgetNamed(gadget, text)) {
            me->field_108[i] ^= 1;
            SetAlliance(me->field_4, p->field_4, me->field_108[i], 0);
            char same;
            if (me->colour == 5)
                same = 0;
            else
                same = me->colour == p->colour;
            if (same) {
                FUN_00446e90(me);
                me->colour = 5;
                FUN_00452bd0(me);
            }
            // Original bug (docs/bugs.md): `<<` binds tighter than `==` and
            // `==` tighter than `|`, so this is ((ally2 << 1) == 3) | ally,
            // and the left side is never true.
            if (me->field_113[i] << 1 == 3 | me->field_108[i])
                PlaySoundByName("Ally", 0);
            else
                PlaySoundByName("Multi", 0);
            sprintf(text, " %s %s",
                    Translate(me->field_108[i] ? "allied with" : "broke alliance with"),
                    g_game->players[i].name);
            SendChatMessage(me, text, 4, 0);
            g_game->dirty = 1;
            BroadcastPlayerInfo();
        }

        sprintf(text, "TEAMICONS%d", i);
        if (IsCurrentGadgetNamed(gadget, text)) {
            PlaySoundByName("Ally", 0);
            CyclePlayerAlliance_00447b10(i);
            FUN_00452bd0(p);
        }

        sprintf(text, "RES%d", i);
        if (IsCurrentGadgetNamed(gadget, text) && IsLocalHuman_00447b10(p)) {
            PlaySoundByName("Multi", 0);
            FUN_00446310();
            FUN_004ab0a0(gadget);
            g_game->dirty = 1;
            return;
        }

        sprintf(text, "READY%d", i);
        if (IsCurrentGadgetNamed(gadget, text) && IsLocalHuman_00447b10(p)) {
            PlaySoundByName("Multi", 0);
            if (CheckMapCrc_00447b10()) {
                p->info->b.ready = GetGadgetStatus(&g_game->gui, FindGadgetIndex(entries, text, 1));
                if (p->info->f97_0) {
                    strcpy(entries->label, "START");
                    g_game->gui.table->field_20 = FindGadgetIndex(entries, "START", 1);
                }
                for (int j = 0; j < 10; j++) {
                    Player_00446f50* q = &g_game->players[j];
                    if (IsLocal_00447b10(q))
                        q->info->b.ready = g_game->players[g_game->localPlayer].info->b.ready;
                }
                g_game->dirty = 1;
                BroadcastPlayerInfo();
            } else {
                SetGadgetStatusByName(&g_game->gui, text, 0);
            }
        }
        i++;
        if (i >= 10)
            break;
    }
    if (i != g_game->field_2a3c)
        g_game->dirty = 1;

    if (IsCurrentGadgetNamed(gadget, "PREVMENU")) {
        PlaySoundByName("Previous", 0);
        for (int j = 0; j < 10; j++) {
            Player_00446f50* q = &g_game->players[j];
            if (IsLocal_00447b10(q))
                RejectPlayer(q->field_4, 2);
        }
        g_game->state = 3;
        return;
    }
    if (IsCurrentGadgetNamed(gadget, "MESSAGE")) {
        Entry_00446f50* box = FUN_004a0010(entries, "MESSAGE");
        char* msg = box->text;
        if (strlen(msg) != 0) {
            if (_strcmpi(msg, "+syncerr") == 0) {
                char* s = g_game->net->GetSyncStatusText();
                if (s)
                    AddMessage(s, 4, 0, 10);
            } else {
                SendChatMessage(me, msg, 4, 0);
                if (g_usePacketManager)
                    g_packetManager.SendAllQueued(1);
            }
            g_game->dirty = 1;
            strcpy(msg, "");
        }
        FUN_004a7190(&g_game->gui, FindGadgetIndex(g_game->gui.table->entries, "MESSAGE", 3));
    } else if (IsCurrentGadgetNamed(gadget, "COMMANDER")) {
        PlaySoundByName("Multi", 0);
        me->info->b.commander++;
        if (me->info->b.commander > 2)
            me->info->b.commander = 0;
        BroadcastPlayerInfo();
        UpdateNetGameInfo();
        g_game->dirty = 1;
    } else if (IsCurrentGadgetNamed(gadget, "LOSTYPE")) {
        PlaySoundByName("Multi", 0);
        if (!me->info->b.los) {
            me->info->b.los = 1;
            me->info->b.losType = 1;
        } else if (me->info->b.losType == 1) {
            me->info->b.losType = 0;
        } else {
            me->info->b.los = 0;
        }
        BroadcastPlayerInfo();
        UpdateNetGameInfo();
        g_game->dirty = 1;
    } else if (IsCurrentGadgetNamed(gadget, "WATCHING")) {
        PlaySoundByName("Multi", 0);
        me->info->b.watching = !me->info->b.watching;
        if (!me->info->b.watching && me->active != 0 && me->info->b.bit6)
            me->info->b.bit6 = 0;
        BroadcastPlayerInfo();
        UpdateNetGameInfo();
        g_game->dirty = 1;
    } else if (IsCurrentGadgetNamed(gadget, "CHEATING")) {
        PlaySoundByName("Multi", 0);
        me->info->b.cheating = !me->info->b.cheating;
        BroadcastPlayerInfo();
        g_game->dirty = 1;
    } else if (IsCurrentGadgetNamed(gadget, "FIXEDLOC")) {
        PlaySoundByName("Multi", 0);
        me->info->b.fixedloc = !me->info->b.fixedloc;
        BroadcastPlayerInfo();
        g_game->dirty = 1;
    } else if (IsCurrentGadgetNamed(gadget, "MAPPING")) {
        PlaySoundByName("Multi", 0);
        me->info->b.mapping = GetButtonStageByName(gadget, "MAPPING") == 0;
        BroadcastPlayerInfo();
        UpdateNetGameInfo();
        g_game->dirty = 1;
    } else if (IsCurrentGadgetNamed(gadget, "START")) {
        int count = 0;
        PlaySoundByName("BigButton", 0);
        for (int j = 0; j < 10; j++) {
            Player_00446f50* q = &g_game->players[j];
            if ((IsLocalHuman_00447b10(q) || IsRemoteHuman_00447b10(q)) && q->info->f9d_2_byte)
                count++;
        }
        if (count < 1 || (count < 2 && CountHumanPlayers() > 3) || (count < 3 && CountHumanPlayers() > 6)) {
            FUN_004ab0a0(&g_game->gui);
            OpenMessageBox(gadget, Translate("There are not enough game CDs present to play"), 200, 1, 1);
            return;
        }
        int total = CountComputerPlayers() + CountHumanPlayers();
        for (int t = 0; t < 5; t++) {
            if (CountAlliance_00447b10(t) == total) {
                FUN_004ab0a0(&g_game->gui);
                OpenMessageBox(gadget, Translate("Can not start game with all players on the same team."), 200, 1, 1);
                return;
            }
        }
        if (!g_game->map->HasMissionName()) {
            PlaySoundByName("Multi", 0);
            OpenMultiMapSelector();
            // Emits no code, but keeps the gadget in esi for the button tests.
            goto done;
        }
        if (!me->info->b.watching) {
            for (int j = 0; j < 10; j++) {
                Player_00446f50* q = &g_game->players[j];
                if (q->active != 0 && q->type == 3 && (q->info->flags_9b & 0x40))
                    RejectPlayer(q->field_4, 9);
            }
        }
        g_game->state = 0x11;
        me->info->b.started = 1;
        UpdateNetGameInfo();
        g_game->los = me->info->b.los;
        g_game->losType = me->info->b.losType;
        g_game->commander = me->info->b.commander;
        g_game->options->fixedloc = me->info->b.fixedloc;
        g_game->mapping = me->info->b.mapping;
        SaveSettings();
        g_game->field_37eee = 2;
        return;
    } else if (IsCurrentGadgetNamed(gadget, "GAMEOPEN")) {
        PlaySoundByName("Multi", 0);
        me->info->b.closed = GetButtonStageByName(gadget, "GAMEOPEN") == 0;
        BroadcastPlayerInfo();
        UpdateNetGameInfo();
        g_game->dirty = 1;
    } else if (IsCurrentGadgetNamed(gadget, "RESTRICTIONS")) {
        PlaySoundByName("Options", 0);
        OpenUnitRestrictions();
        FUN_004ab0a0(gadget);
    } else {
        // MAP and MAPNAME through a local, not `MAP || MAPNAME` in the
        // else-if: with the `||` the MAP body joins the region where C2 keeps
        // the constant 1 in ebp, and FUN_0049fb10 gets `push ebp` (99.2%).
        int hit = IsCurrentGadgetNamed(gadget, "MAP");
        if (!hit)
            hit = IsCurrentGadgetNamed(gadget, "MAPNAME");
        if (hit) {
            PlaySoundByName("Multi", 0);
            if (me->info->f97_0) {
                OpenMultiMapSelector();
            } else {
                Layer_00446f50* view = LoadGuiLayer(&g_game->gui, "VIEWMAP.GUI", 0x900);
                view->handler = HandleViewMapClick;
                LoadPictureCached("DVIEWMAP", 0, 0, 0);
                ShowSelectedMapInfo();
                FUN_0049fb10(&g_game->gui, 1);
                RenderLayer(&g_game->gui, 0x40);
            }
        }
    }
done:
    FUN_004ab0a0(gadget);
}

// FUNCTION: 0x448bf0
void __stdcall UpdateSideGadget(int side)
{
    char name[20];
    Player_00446f50* p = &g_game->players[side];

    sprintf(name, "SIDE%d", side);
    SetButtonStageByName((Class_004a1080*)&g_game->gui, name,
                 (p->active != 0 && (p->info->flags_9b & 0x40)) ? 2 : p->info->side);
}

static inline int IsPlaying_00448c70(Player_00446f50* p)
{
    return p->active != 0
        && (p->type == 1 || p->type == 2 || p->type == 3)
        && p->field_146 != 10;
}

static inline int IsWatching_00448c70(Player_00446f50* p)
{
    return p->active != 0 && (p->info->flags_9b & 0x40);
}

static inline int IsLocalHuman_00448c70(Player_00446f50* p)
{
    return p->active != 0 && p->type == 1;
}

static inline int IsLocalAI_00448c70(Player_00446f50* p)
{
    return p->active != 0 && p->type == 2;
}

static inline int IsRemoteHuman_00448c70(Player_00446f50* p)
{
    return p->active != 0 && p->type == 3 && p->info->kind == 1;
}

static inline int IsRemoteAI_00448c70(Player_00446f50* p)
{
    return p->active != 0 && p->type == 3 && p->info->kind == 2;
}

static inline int IsLocal_00448c70(Player_00446f50* p)
{
    return p->active != 0 && (p->type == 1 || p->type == 2);
}

// The map check at 0x440cd0, which has no callers: /Ob2 inlined it.
int CheckMapCrc_00448c70()
{
    if (!g_game->map->GetTerrainLength()) {
        return 0;
    }
    unsigned char me = FindHostSlot();
    PlayerInfo_00446f50* data = 0;
    int check = 0;
    if (me != 10) {
        data = g_game->players[me].info;
        // Version test form must stay: inlined, it gives the right registers for g_game/check.
        if (data->versionMajor >= 2)
            check = 1;
        else if (data->versionMajor == 1 && data->versionMinor >= 2)
            check = 1;
    }
    if (!check) {
        return 1;
    }
    if (g_game->map->ComputeMapChecksum() != data->mapCrc)
        return 0;
    return 1;
}

// The SIDE%d update at 0x448bf0, which has no callers: /Ob2 inlined it.
void __stdcall UpdateSideGadget_00448c70(int side)
{
    char name[20];
    Player_00446f50* p = &g_game->players[side];

    sprintf(name, "SIDE%d", side);
    SetButtonStageByName((Class_004a1080*)&g_game->gui, name,
                 (p->active != 0 && (p->info->flags_9b & 0x40)) ? 2 : p->info->side);
}

// FUNCTION: 0x448c70
void RefreshBattleRoomRows()
{
    char name[20];
    // Shared by the mapname and PING text: one frame slot.
    char* str;
    unsigned int minPing = 0xffffffff;
    int count = 0;
    Player_00446f50* me = &g_game->players[g_game->localPlayer];
    int ready = me->info->b.ready;
    Entry_00446f50* output = FindGadgetChecked(g_game->gui.table->entries, "OUTPUT");

    int end = g_game->scrollEnd;
    int start = g_game->scrollStart;
    if (end < start)
        end += 30;
    if (end - start > output->height / (GetFontLineHeight() + 2)) {
        g_game->scrollStart++;
        if (g_game->scrollStart >= 30)
            g_game->scrollStart = 0;
    }
    if (g_game->scrollEnd != g_game->scrollStart) {
        for (int i = g_game->scrollStart; g_game->scrollEnd != i; ) {
            char* line = SkipTextLines(g_game->chatter, count);
            strcpy(line, g_game->messages[i]);
            count++;
            i++;
            if (i == 30)
                i = 0;
        }
    }
    UpdateBattleRoomFlags();
    output->list.count = count;

    Entry_00446f50* mapname = FUN_004a0180(g_game->gui.table->entries, "MAPNAME");
    char* map = g_game->map->GetMissionName();
    if (!g_game->map->HasMissionName()) {
        mapname->colour = 0xc;
        FUN_004a0bf0(&g_game->gui, "MAPNAME", "NOT SELECTED", 0);
    } else {
        char* cur = g_game->map->GetTranslatedName();
        str = mapname->text;
        int differs = strcmp(str, cur);
        if (differs) {
            if (IsScreenNamed(&g_game->gui, "viewmap.gui"))
                ShowSelectedMapInfo();
            else
                g_game->map->LoadMissionByName(map);
        }
        if (!CheckMapCrc_00448c70()) {
            mapname->colour = ((int)GetTicks() / 30 & 1) ? 0xc : 0;
            if (differs) {
                SendChatMessage(me, Translate("does not have this map"), 4, 0);
                me->info->b.ready = 0;
                sprintf(name, "READY%d", g_game->localPlayer);
                SetGadgetStatusByName(&g_game->gui, name, 0);
                BroadcastPlayerInfo();
            }
            if (!g_game->players[g_game->localPlayer].info->f97_0)
                FUN_004a1450(&g_game->gui, "MAP", 1);
        } else {
            mapname->colour = 0;
            FUN_004a1450(&g_game->gui, "MAP", 0);
        }
        strcpy(str, g_game->map->GetTranslatedName());
    }

    int i;
    for (i = 0; i < 10; i++) {
        Player_00446f50* p = &g_game->players[i];
        if (IsLocal_00448c70(p))
            p->info->b.ready = g_game->players[g_game->localPlayer].info->b.ready;
    }
    for (i = 0; i < 10; i++) {
        Player_00446f50* p = &g_game->players[i];
        if (g_game->players[g_game->localPlayer].info->b.commander == 2
            && (IsLocalAI_00448c70(p) || IsRemoteAI_00448c70(p))) {
            RejectPlayer(p->field_4, 0xb);
            BroadcastPlayerInfo();
        }
        if (FindHostSlot() != 10
            && !g_game->players[FindHostSlot()].info->b.watching
            && p->active != 0) {
            unsigned short flags = p->info->flags;
            if (flags & 0x40) {
                p->info->flags = flags & ~0x40;
                p->info->side = 0;
                BroadcastPlayerInfo();
            }
        }
    }
    FUN_00446c70();
    RefreshTeamIcons();

    char* entries = (char*)g_game->gui.table->entries;
    Player_00446f50* local = &g_game->players[g_game->localPlayer];
    // Plain unsigned char counter, no int copy.
    for (unsigned char n = 0; n < 10; n++) {
        char text[32];
        char res[52];
        char blocked[52];
        Player_00446f50* p = &g_game->players[n];
        Entry_00446f50* e;
        if (!IsPlaying_00448c70(p) && !IsWatching_00448c70(p)) {
            sprintf(name, "CD%d", n);
            FUN_004a0570(&g_game->gui, name, 0);
            FUN_004a1450(&g_game->gui, name, 0);
            sprintf(name, "PLAYER%d", n);
            char* s = "UNUSED";
            if (p->type == 4) {
                sprintf(blocked, "[%s]", Translate("BLOCKED"));
                s = blocked;
            }
            strncpy(text, s, 0x1e);
            FUN_004a0bf0(&g_game->gui, name, text, 0);
            FUN_004a5d50(&g_game->gui, FindGadgetIndex(entries, name, 0xe));
            FUN_004a1450(&g_game->gui, name, ready);
            sprintf(name, "LOGO%d", n);
            e = FUN_004a0280(entries, name);
            if (e)
                e->visible = 0;
            sprintf(name, "SIDE%d", n);
            e = FindGadgetOrNull(entries, name);
            if (e)
                e->visible = 0;
            if (n != g_game->localPlayer) {
                sprintf(name, "ALLY%d", n);
                e = FindGadgetOrNull(entries, name);
                if (e)
                    e->visible = 0;
            }
            sprintf(name, "TEAMICONS%d", n);
            e = FindGadgetOrNull(entries, name);
            if (e)
                e->visible = 0;
            sprintf(name, "RES%d", n);
            e = FUN_004a0180(entries, name);
            if (e)
                e->visible = 0;
            sprintf(name, "PING%d", n);
            e = FUN_004a0180(entries, name);
            if (e)
                e->visible = 0;
            sprintf(name, "MEM%d", n);
            e = FUN_004a0180(entries, name);
            if (e)
                e->visible = 0;
            sprintf(name, "READY%d", n);
            e = FindGadgetOrNull(entries, name);
            if (e) {
                e->b13c_0 = 1;
                e->field_138 = 0;
                e->visible = 0;
            }
        } else {
            sprintf(name, "CD%d", n);
            FUN_004a0570(&g_game->gui, name,
                         ((IsLocalHuman_00448c70(p) || IsRemoteHuman_00448c70(p))
                          && p->info->f9d_2_byte) ? 1 : 0);
            FUN_004a1450(&g_game->gui, name, 0);
            sprintf(name, "LOGO%d", n);
            e = FUN_004a0280(entries, name);
            if (e) {
                e->visible = (p->info->field_96 == 0xff && !ready) ? 0 : 1;
                e->c8_0 = !ready;
                e->field_be = g_game->field_148db;
                e->frame = p->info->field_96;
            }
            sprintf(name, "PLAYER%d", n);
            strncpy(text, p->name, 0x1e);
            FUN_004a0bf0(&g_game->gui, name, text, 0);
            FUN_004a5d50(&g_game->gui, FindGadgetIndex(entries, name, 0xe));
            FUN_004a1450(&g_game->gui, name, ready);
            sprintf(name, "SIDE%d", n);
            e = FindGadgetOrNull(entries, name);
            if (e) {
                UpdateSideGadget_00448c70(n);
                e->visible = 1;
                FUN_004a1450(&g_game->gui, name, (IsLocal_00448c70(p) && !ready) ? 0 : 1);
            }
            sprintf(name, "ALLY%d", n);
            e = FindGadgetOrNull(entries, name);
            if (e) {
                SetButtonStageByName((Class_004a1080*)&g_game->gui, name,
                             me->field_113[n] << 1 | me->field_108[n]);
                e->visible = (IsLocalHuman_00448c70(p) || IsWatching_00448c70(p)
                              || IsLocalAI_00448c70(p) || IsRemoteAI_00448c70(p)
                              || IsWatching_00448c70(local)) ? 0 : 1;
                FUN_004a1450(&g_game->gui, name, ready);
            }
            sprintf(name, "TEAMICONS%d", n);
            e = FindGadgetOrNull(entries, name);
            if (e) {
                // `|| 0` emits no code but gives the 1-before-0 layout.
                e->visible = (IsWatching_00448c70(p) || 0) ? 0 : 1;  // see the top
                FUN_004a1450(&g_game->gui, name, (IsLocal_00448c70(p) && !ready) ? 0 : 1);
            }
            sprintf(name, "RES%d", n);
            if (!IsLocalHuman_00448c70(p) && !IsRemoteHuman_00448c70(p))
                sprintf(res, "%s", "n/a");
            else
                sprintf(res, "%dx%d", p->info->width, p->info->height);
            FUN_004a0bf0(&g_game->gui, name, res, 0);
            if (IsLocalHuman_00448c70(p)) {
                FUN_004a1450(&g_game->gui, name, ready);
            } else {
                e = FUN_004a0180(entries, name);
                if (e)
                    e->visible = 1;
            }
            sprintf(name, "PING%d", n);
            e = FUN_004a0180(entries, name);
            if (IsRemoteHuman_00448c70(p)) {
                if (e) {
                    str = e->text;
                    _itoa(p->ping, str, 10);
                    if (IsHostLocal())
                        strcat(str, g_game->net->IsPlayerSynced(GetSlotDpid(n)) ? ":s" : "");
                    if (minPing >= p->ping)
                        minPing = p->ping;
                    e->visible = 1;
                }
            } else {
                FUN_004a0bf0(&g_game->gui, name, "n/a", 0);
                if (e)
                    e->visible = 1;
            }
            sprintf(name, "MEM%d", n);
            e = FUN_004a0180(entries, name);
            // Full tail in each arm, not shared: gives p and n their registers.
            if (!IsLocalHuman_00448c70(p) && !IsRemoteHuman_00448c70(p)) {
                sprintf(e->text, "%s", "n/a");
                e->colour = p->info->memory < g_game->map->GetTerrainSizeTier() ? 0xc : 0;
                e->visible = 1;
            } else {
                sprintf(e->text, "%d", p->info->memory);
                e->colour = p->info->memory < g_game->map->GetTerrainSizeTier() ? 0xc : 0;
                e->visible = 1;
            }
            sprintf(name, "READY%d", n);
            e = FindGadgetOrNull(entries, name);
            if (e) {
                e->field_138 = g_game->players[n].info->b.ready;
                e->visible = 1;
                e->b13c_0 = !IsLocalHuman_00448c70(p);
            }
        }
    }
    FUN_0049fa90(&g_game->gui);
    PlayerInfo_00446f50* info = me->info;
    if (info->f97_0 && minPing < info->pingLimit) {
        info->pingLimit = minPing;
        UpdateNetGameInfo();
    }
}

// 0x449bb0 OpenBattleRoom stays in src/frontend/multi_449bb0.cpp: it matches
// only with the generated ta_types.h and ta_protos.h in front of its own
// types and externs (docs/c2-regalloc.md), so its declarations cannot be
// folded into this file.

// 0x44a680 UpdateBattleRoom stays in src/frontend/multi_44a680.cpp: in this
// file's symbol context the version text's rect sums and g_game loads land in
// different registers; the file matches alone with its own includes.

// FUNCTION: 0x44afb0
void __stdcall HandleEndMultiClick(Gui_00446f50* obj)
{
    if (obj->current == -1) {
        if (g_game->flag4)
            LeaveNetGame();
    } else if (IsCurrentGadgetNamed(obj, "OK")) {
        PlaySoundByName("BigButton", 0);
        SetFrontendState(2, 0x1412, "c:\\cavedog\\wargame\\multi.cpp");
        SetGameMode(1);
    } else {
        FUN_004ab0a0(obj);
    }
}

// FUNCTION: 0x44b020
void OpenEndMultiScreen()
{
    unsigned char palette[0x400];
    void* image;

    BlankScreen();
    image = LoadBitmapByName("Mission02WinBW", palette);
    SetPaletteColors(palette, 0, 0x100);
    SetOffscreenSurface(g_game->field_37e1b);
    DrawSurface(0, image, 0, 0);
    FreeSurface(image);
    FlipScreen();
    LoadGuiLayer(&g_game->gui, "ENDMULTI.GUI", 0x80)->handler = HandleEndMultiClick;
    FUN_004a0bf0(&g_game->gui, "RESULT",
                 Translate(g_game->flag4_3923b ? "Victory" : "Failure"), 0);
    RenderLayer(&g_game->gui, 0xc0);
    ShowSoftwareCursor();
}

// FUNCTION: 0x44b100
void FUN_0044b100()
{
    if (DAT_005129ac) {
        FUN_004d85a0(DAT_005129ac);
    }
    if (DAT_005129b0) {
        FUN_004d85a0(DAT_005129b0);
    }
    DAT_005129ac = DAT_005129b0 = 0;
}

// Reads pairs of ints from a binary file given by `name`. For each pair the
// first int is matched against the field at +0x13e of the 0x249-byte entries
// at g_game+0x1439b (entries are 1-based here); the second int is then stored
// in the field at +0x5a of the 0x62-byte entry of the table at DAT_005129b4
// whose +0x52 field equals the matched index.
// FUNCTION: 0x44b140
void __stdcall FUN_0044b140(char* name)
{
    FILE* f = fopen(name, "rb");
    int count;
    fread(&count, 4, 1, f);
    for (int i = 0; i < count; i++) {
        int a, b;
        fread(&a, 4, 1, f);
        fread(&b, 4, 1, f);
        int n = g_game->count;
        for (int idx = 1; idx < n; idx++) {
            if (g_game->unitTypes[idx].field_13e == a) {
                for (int j = 0; j < n; j++) {
                    if (DAT_005129b4[j].field_52 == idx) {
                        DAT_005129b4[j].field_5a = b;
                        break;
                    }
                }
                break;
            }
        }
    }
    fclose(f);
}

// FUNCTION: 0x44b230
void __stdcall FUN_0044b230(char* filename)
{
    FILE* f = fopen(filename, "wb+");

    int count = g_game->count - 1;
    fwrite(&count, 4, 1, f);
    count++;

    for (int i = 1; i < count; i++) {
        for (int j = 0; j < g_game->count; j++) {
            if (*(int*)((char*)DAT_005129b4 + 0x52 + j * 0x62) == i) {
                int v = g_game->unitTypes[i].field_13e;
                fwrite(&v, 4, 1, f);
                v = *(int*)((char*)DAT_005129b4 + 0x5a + j * 0x62);
                fwrite(&v, 4, 1, f);
                break;
            }
        }
    }

    fclose(f);
}

// FUNCTION: 0x44b330
void CopySelectedGameName()
{
    Gui_00446f50* menu = &g_game->gui;
    void* gadgets = g_game->gui.table->entries;
    Entry_00446f50* games = FindGadgetChecked(gadgets, "GAMES");
    int index = FindGadgetIndex(gadgets, "GAMENAME", 3);
    char* name;
    if (games->selected > -1 && (name = SkipTextLines(DAT_005129b0, games->selected)) != 0 && strlen(name) != 0)
        SetGadgetText(menu, index, name);
    else
        SetGadgetText(menu, index, DAT_005119b8);
    FUN_0049fa90(&g_game->gui);
}

// FUNCTION: 0x44b3c0
void __stdcall HandleLoadListClick(Gui_00446f50* menu)
{
    void* gadgets = menu->table->entries;
    if (menu->current == -1)
        return;
    if (IsCurrentGadgetNamed(menu, "CANCEL")) {
        PlaySoundByName("Previous", 0);
        return;
    }
    if (IsCurrentGadgetNamed(menu, "LOAD") || IsCurrentGadgetNamed(menu, "GAMES")) {
        PlaySoundByName("Options", 0);
        Entry_00446f50* games = FindGadgetChecked(gadgets, "GAMES");
        sprintf(g_game->save_38c6b, "%s\\%s", DAT_005091c8,
                SkipTextLines(DAT_005129ac, games->selected));
        FUN_0044b140(g_game->save_38c6b);
        Layer_00446f50* inner = menu->table;
        menu->table = inner->unknown_0;
        UpdateUnitSliders(menu, 0);
        menu->table = inner;
        if (DAT_005129ac)
            FUN_004d85a0(DAT_005129ac);
        if (DAT_005129b0)
            FUN_004d85a0(DAT_005129b0);
        DAT_005129b0 = 0;
        DAT_005129ac = 0;
    } else if (menu->current != -1) {
        FUN_004ab0a0(menu);
    }
}

// FUNCTION: 0x44b4e0
void* __stdcall ListSaveGameFiles(int* out)
{
    char path[0x100];
    BuildDataPath(path, DAT_005091c8, DAT_0050372c, DAT_00505f40);
    int count = CountDirectoryEntries(path, 0);
    *out = count;
    if (count == 0) {
        FUN_004a32a0((char*)g_game + 0x519, DAT_00505f18, DAT_005119b8, 0, 0);
        return 0;
    }
    DAT_005129ac = (char*)FUN_004d83b0(DAT_00505f30, count << 8);
    DAT_005129b0 = (char*)FUN_004d83b0(DAT_00505f20, *out << 8);
    memset(DAT_005129b0, 0, *out << 8);
    memset(DAT_005129ac, 0, *out << 8);
    ScanDirectory(path, DAT_005129ac, 0, 0, 0, 1);
    FUN_004a32a0((char*)g_game + 0x519, DAT_00505f18, DAT_005129ac, *out, 0);
    return *out ? DAT_005129ac : 0;
}

// FUNCTION: 0x44b600
void __stdcall FUN_0044b600(int unused1, int unused2)
{
    Gui_00446f50* menu = &g_game->gui;
    void* gadgets = g_game->gui.table->entries;
    Entry_00446f50* games = FindGadgetChecked(gadgets, "GAMES");
    int index = FindGadgetIndex(gadgets, "GAMENAME", 3);
    char* name;
    if (games->selected > -1 && (name = SkipTextLines(DAT_005129b0, games->selected)) != 0 && strlen(name) != 0)
        SetGadgetText(menu, index, name);
    else
        SetGadgetText(menu, index, DAT_005119b8);
    FUN_0049fa90(&g_game->gui);
}

// FUNCTION: 0x44b690
void __stdcall HandleSaveGameClick(Gui_00446f50* menu)
{
    Entry_00446f50* entries = menu->table->entries;
    if (menu->current == -1) {
        FUN_004ab190(menu, 1);
        if (DAT_005129ac)
            FUN_004d85a0(DAT_005129ac);
        if (DAT_005129b0)
            FUN_004d85a0(DAT_005129b0);
        DAT_005129ac = DAT_005129b0 = 0;
        g_game->flag_38a51 = 0;
        return;
    }
    if (IsCurrentGadgetNamed(menu, "CANCEL")) {
        PlaySoundByName("Previous", 0);
        return;
    }
    if (IsCurrentGadgetNamed(menu, "DELETE")) {
        PlaySoundByName("SMLBUTTON", 0);
        Entry_00446f50* games = FindGadgetChecked(entries, "GAMES");
        char buf[0x100];
        sprintf(buf, "%s\\%s", DAT_005091c8,
                SkipTextLines(DAT_005129ac, games->selected));
        RemoveFile(buf);
        int count;
        ListSaveGameFiles(&count);
        char* p = DAT_005129b0;
        for (int i = 0; i < count; i++) {
            strcpy(p, SkipTextLines(DAT_005129ac, i));
            p += strlen(SkipTextLines(DAT_005129ac, i));
            while (*p != '.')
                p--;
            *p++ = 0;
        }
        FUN_004a32a0(&g_game->gui, "GAMES", DAT_005129b0, count, 0);
        FUN_004ab0a0(menu);
        Gui_00446f50* menu2 = &g_game->gui;
        Entry_00446f50* gadgets = g_game->gui.table->entries;
        Entry_00446f50* games2 = FindGadgetChecked(gadgets, "GAMES");
        int index = FindGadgetIndex(gadgets, "GAMENAME", 3);
        char* name;
        if (games2->selected > -1 &&
            (name = SkipTextLines(DAT_005129b0, games2->selected)) != 0 &&
            strlen(name) != 0)
            SetGadgetText(menu2, index, name);
        else
            SetGadgetText(menu2, index, DAT_005119b8);
        FUN_0049fa90(&g_game->gui);
        return;
    }
    if (IsCurrentGadgetNamed(menu, "GAMES") || IsCurrentGadgetNamed(menu, "LOAD") ||
        IsCurrentGadgetNamed(menu, "GAMENAME")) {
        PlaySoundByName("Options", 0);
        int idx = FindGadgetIndex(entries, "GAMENAME", 3);
        char* name = entries[idx].text;
        if (strlen(name) != 0) {
            BuildDataPath(g_game->save_38c6b, DAT_005091c8, name, "LST");
            FUN_0044b230(g_game->save_38c6b);
        }
    } else if (menu->current != -1) {
        FUN_004ab0a0(menu);
    }
}

static char* GetSaveDescriptions()
{
    return DAT_005129b0;
}

// FUNCTION: 0x44b990
void __stdcall OpenSaveGameDialog()
{
    int count;
    Layer_00446f50* layer = LoadGuiLayer(&g_game->gui, "SAVELIST.GUI", 0x880);
    layer->handler = HandleSaveGameClick;
    layer->data = g_game;
    LoadPictureCached("DSaveList", 0, 0, 0);
    MakeDirectoryPath(DAT_005091c8);
    ListSaveGameFiles(&count);
    FUN_004a0bf0(&g_game->gui, "TITLE", "Save Game", 0);
    char* ptr = GetSaveDescriptions();
    int i = 0;
    for (; i < count; i++) {
        strcpy(ptr, SkipTextLines(DAT_005129ac, i));
        ptr += strlen(SkipTextLines(DAT_005129ac, i));
        while (*ptr != '.')
            ptr--;
        *ptr = 0;
        ptr++;
    }
    FUN_004a32a0(&g_game->gui, "GAMES", DAT_005129b0, count, 0);
    if (count == 0)
        FUN_004a0570(&g_game->gui, "DELETE", 0);
    Entry_00446f50* games = FindGadgetChecked(layer->entries, "GAMES");
    if (games != 0)
        games->field_ce = FUN_0044b600;
    int index = FindGadgetIndex(layer->entries, "GAMENAME", 3);
    layer->entries[index].field_1b |= 2;

    Gui_00446f50* menu = &g_game->gui;
    Entry_00446f50* entries = g_game->gui.table->entries;
    Entry_00446f50* games2 = FindGadgetChecked(entries, "GAMES");
    int index2 = FindGadgetIndex(entries, "GAMENAME", 3);
    char* name;
    if (games2->selected > -1 && (name = SkipTextLines(DAT_005129b0, games2->selected)) != 0 && strlen(name) != 0)
        SetGadgetText(menu, index2, name);
    else
        SetGadgetText(menu, index2, DAT_005119b8);
    FUN_0049fa90(&g_game->gui);

    FUN_004a7190(&g_game->gui, index);
    FUN_0049fb10(&g_game->gui, 1);
    FUN_00428b60();
    FUN_004a0570(&g_game->gui, "LoadGame", 0);
    FUN_0049fa50(&g_game->gui);
    RenderLayer(&g_game->gui, 0x40);
}

// FUNCTION: 0x44bc10
void OpenLoadListDialog()
{
    Layer_00446f50* gadget = LoadGuiLayer(&g_game->gui, "LOADLIST.GUI", 0x981);
    gadget->handler = HandleLoadListClick;
    gadget->data = g_game;
    LoadPictureCached("DLoadList", 0, 0, 0);
    int count;
    if (ListSaveGameFiles(&count) == 0) {
        CloseTopScreen(&g_game->gui);
        OpenMessageBox(&g_game->gui,
                     Translate("There are no saved lists to choose from"),
                     0x140, 1, 1);
        return;
    }
    char* p = DAT_005129b0;
    for (int i = 0; i < count; i++) {
        strcpy(p, SkipTextLines(DAT_005129ac, i));
        p += strlen(SkipTextLines(DAT_005129ac, i));
        char c = *p;
        while (c != '.') {
            c = *--p;
        }
        *p = 0;
        p++;
    }
    FUN_004a32a0(&g_game->gui, "GAMES", DAT_005129b0, count, 0);
    FUN_004a0570(&g_game->gui, "DELETE", 0);
    FUN_004a0570(&g_game->gui, "GAMENAME", 0);
    Entry_00446f50* entry = FindGadgetChecked(gadget->entries, "GAMES");
    if (entry != 0) {
        entry->field_ce = (void*)FUN_0044b600;
    }
    Gui_00446f50* menu = &g_game->gui;
    void* gadgets = g_game->gui.table->entries;
    Entry_00446f50* games = FindGadgetChecked(gadgets, "GAMES");
    int index = FindGadgetIndex(gadgets, "GAMENAME", 3);
    char* name;
    if (games->selected > -1 && (name = SkipTextLines(DAT_005129b0, games->selected)) != 0 && strlen(name) != 0)
        SetGadgetText(menu, index, name);
    else
        SetGadgetText(menu, index, DAT_005119b8);
    FUN_0049fa90(&g_game->gui);
    FUN_0049fb10(&g_game->gui, 1);
    FUN_00428b60();
    FUN_004a0570(&g_game->gui, "SaveGame", 0);
    FUN_0049fa50(&g_game->gui);
    RenderLayer(&g_game->gui, 0x40);
    g_game->flag_38a51 |= 1;
}

// FUNCTION: 0x44be70
void __stdcall HandleUnitCountSlider(void* obj, char* gadget)
{
    int n = atoi(gadget + 8);
    Entry_00446f50* desc = FindGadgetChecked(g_game->gui.table->entries, "DESCLIST");
    char count[20];
    sprintf(count, "COUNT%d", n);
    int value = ReadSliderValue(gadget);
    char buf[20];
    if (value > 0x64) {
        sprintf(buf, Translate("No Limit"));
        value = -1;
    } else {
        _itoa(value, buf, 10);
    }
    DAT_005129b4[n + desc->field_bc].field_5a = value;
    g_game->net->SetUnitLimit(
        &g_game->unitTypes[DAT_005129b4[n + desc->field_bc].field_52], value);
    desc->flags[n + desc->field_bc] = DAT_005129b4[n + desc->field_bc].field_5e == 0;
    desc->flags[n + desc->field_bc] |= DAT_005129b4[n + desc->field_bc].field_5a == 0 ? 2 : 0;
    FUN_004a0bf0(obj, count, (char*)buf, 0);
}

// FUNCTION: 0x44bfd0
void __stdcall UpdateUnitSliders(Gui_00446f50* param_1, int unused)
{
    // C-style locals, loop counter first: sets the operand order of the flags store.
    int i;
    Entry_00446f50* desc;
    int human;
    int base;
    Entry_00446f50* slider;
    int en;
    int value;
    char name[20];

    desc = FindGadgetChecked(param_1->table->entries, "DESCLIST");
    human = IsHostLocal();
    base = desc->field_bc;

    for (i = 0; i < 12; i++) {
        sprintf(name, "SLIDER%d", i);
        slider = FUN_004a0200(param_1->table->entries, name);
        if (slider != 0) {
            if (human == 0 || DAT_005129b4[base + i].field_5e == 0)
                en = 1;
            else
                en = 0;
            desc->flags[base + i] = en != 0;
            value = DAT_005129b4[base + i].field_5a;
            if (value == -1)
                value = slider->field_13c;
            SetSliderFromValue(slider, value);
            FUN_004a1450(param_1, name, en);
            slider->field_144(param_1, slider->field_14a);
        }
    }
}

// FUNCTION: 0x44c0d0
void LoadUnitPortrait()
{
    Record_0044c0d0 rec;
    char path[256];
    Entry_00446f50* pic = FindGadgetChecked(g_game->gui.table->entries, "PICLIST");
    if (DAT_00512768 == 0) {
        DAT_0051297c = (int)pic->field_c6;
        DAT_00512978 = (int)DAT_005129b8;
    }
    int i = DAT_00512768++;
    if (i < g_game->count) {
        int type = DAT_005129b4[i].field_52;
        UnitType_00446f50* defs = g_game->unitTypes;
        if (defs[type].name && ((unsigned char)(defs[type].field_245.raw >> 15) & 1) == 0) {
            // Indexed by the reloaded entry field, not defs[type].name.
            BuildDataPath(path, "unitpics", defs[DAT_005129b4[i].field_52].name, "PCX");
            void* img = LoadPcx(path, 0);
            *(void**)DAT_00512978 = img;
            DAT_00512978 += 4;
            if (img != 0) {
                FrameFromSurface(&rec, img);
                rec.flag8 = 9;
            } else {
                // rec.a before rec.b: field_17 is loaded before the 0x20 store.
                rec.a = pic->field_17;
                rec.b = 0x20;
                rec.d = 0;
            }
            *(Record_0044c0d0*)DAT_0051297c = rec;
            DAT_0051297c += 0x18;
            FUN_0049fa90(&g_game->gui);
        }
    }
}

// FUNCTION: 0x44c220
void FUN_0044c220()
{
    Event_44c220 event;
    int n = 0;
    Entry_00446f50* entry = (Entry_00446f50*)FindGadgetChecked(g_game->gui.table->entries, "PICLIST");

    if (DAT_005129c8 < (int)GetTicks()) {
        DAT_005129c8 = GetTicks() + 2;
        LoadUnitPortrait();
    }

    while (g_game->net->PopChangedEntry(&event) != 0) {
        n++;
        for (int i = 0; i < entry->count; i++) {
            if (event.field_0 == g_game->unitTypes[DAT_005129b4[i].field_52].field_13e) {
                entry->flags[i] = (event.field_a == 0);
                DAT_005129b4[i].field_5e = event.field_a;
                DAT_005129b4[i].field_5a = event.field_c;
                entry->flags[i] |= (event.field_c != 0) ? 0 : 2;
            }
        }
    }

    if (n != 0) {
        UpdateUnitSliders(&g_game->gui, 0);
        FUN_0049fa90(&g_game->gui);
    }
}

// FUNCTION: 0x44c370
void __stdcall FUN_0044c370(void* panel, Entry_00446f50* unit)
{
    char buf[20];
    UnitType_00446f50* def = &g_game->unitTypes[unit->records[unit->field_ba].field_52];
    sprintf(buf, "%d", (int)def->energyCost);
    FUN_004a0bf0(panel, "ENERGYTEXT", (char*)buf, 0);
    sprintf(buf, "%d", (int)def->metalCost);
    FUN_004a0bf0(panel, "METALTEXT", (char*)buf, 0);
}

// 0x44c420 HandleRestrictionsClick stays in src/frontend/multi_44c420.cpp:
// in this file's symbol context two address computations swap their operand
// order; its own file needs <stdio.h> without <windows.h>, which the rest of
// this file needs.

// FUNCTION: 0x44c7a0
int __cdecl FUN_0044c7a0(const char* a, const char* b)
{
    return strcmp(a, b);
}

// The slider set-up at 0x445e50, which has no callers: /Ob2 inlined it.
void __stdcall FUN_00445e50_0044c7e0(char* name, int max, int value, Callback_0044c7e0 callback)
{
    Gui_00446f50* gui = &g_game->gui;
    Entry_00446f50* gadgets = gui->table->entries;
    int index = FindGadgetIndex(gadgets, name, 0xe);
    if (index != -1) {
        Entry_00446f50* gadget = FUN_004a0200(gadgets, name);
        gadget->max = max;
        gadget->callback = callback;
        gadget->value = value;
        SetSliderFromValue(gadget, gadget->value);
        gadget->game = g_game;
    }
    callback(gui, index);
    FUN_0049fa90(gui);
}

// FUNCTION: 0x44c7e0
void OpenUnitRestrictions()
{
    int host = g_game->players[g_game->localPlayer].info->f97_0 & 1;
    Layer_00446f50* layer;
    Entry_00446f50* entries;
    char* flags;
    Entry_00446f50* desc;
    Entry_00446f50* pic;
    int* pics;
    char* text;
    char* dst;
    int n;
    int i;

    layer = LoadGuiLayer(&g_game->gui, "RESTRICT2.GUI", 0x880);
    layer->handler = HandleRestrictionsClick;
    layer->field_c = 0;
    layer->field_1c = FUN_0044c220;
    DAT_00512768 = 0;
    LoadPictureCached("UnitRestrict5x", 0, 0, 0);

    entries = layer->entries;
    flags = (char*)FUN_004d83b0("FLAGS", g_game->count);
    desc = FindGadgetChecked(entries, "DESCLIST");
    desc->field_ce = FUN_0044c370;
    desc->flags = flags;
    desc->field_da = 0x20;
    desc->field_1b |= 0x100;

    pic = FindGadgetChecked(layer->entries, "PICLIST");
    pic->flags = flags;
    pic->field_1b |= 0x180;
    pic->field_da = desc->field_da;

    pics = (int*)FUN_004d83b0("UNITPICARRAY", g_game->count * 0x18);
    memset(pics, 0, g_game->count * 0x18);
    text = (char*)FUN_004d83b0("UNITTEXTARRAY", g_game->count << 5);
    *(int*)text = 0;

    DAT_005129b4 = (Record_00446f50*)FUN_004d83b0("UNITSRESTRICTINFO", g_game->count * 0x62);
    desc->records = DAT_005129b4;
    for (i = 0; i < g_game->count; i++)
        DAT_005129b4[i].field_52 = 0;

    DAT_005129b8 = (int*)FUN_004d83b0("UNITSPICS", g_game->count << 2);
    memset(DAT_005129b8, 0, g_game->count << 2);
    memset(DAT_005129b4, 0, g_game->count * 0x62);
    DAT_005129c4 = (int*)FUN_004d83b0("OLDCOUNTS", g_game->count << 2);

    n = 0;
    for (i = 1; i < g_game->count; i++) {
        // continue on the bit, an int bitfield tested positively.
        if (g_game->unitTypes[i].field_245.bits.flag)
            continue;
        if (!g_game->unitTypes[i].name)
            continue;
        {
            UnitType_00446f50* type = &g_game->unitTypes[i];
            Info_0044c7e0 info;
            int count;
            sprintf(DAT_005129b4[n].name, "%s\r%s %dM  %dE",
                    g_game->unitTypes[i].unitName, Translate(type->description),
                    (int)type->metalCost, (int)type->energyCost);
            DAT_005129b4[n].field_52 = i;
            g_game->net->GetUnitEntry(&g_game->unitTypes[i], &info);
            // One ternary: the if-statement form swaps the ebx/ebp registers.
            count = info.field_c == -1 ? 0x65 : info.field_c;
            DAT_005129b4[n].field_5a = count;
            DAT_005129c4[n] = count;
            DAT_005129b4[n].field_5e = info.field_a;
            n++;
        }
    }

    qsort(DAT_005129b4, n, 0x62, (int (__cdecl*)(const void*, const void*))FUN_0044c7a0);

    dst = text;
    for (i = 0; i < g_game->count; i++) {
        strcpy(dst, DAT_005129b4[i].name);
        dst += strlen(DAT_005129b4[i].name) + 1;
    }

    for (i = 0; i < 0xc; i++) {
        char name[0x14];
        Entry_00446f50* slider;
        sprintf(name, "SLIDER%d", i);
        slider = FUN_004a0200(entries, name);
        slider->game = slider;
        slider->max = 0x65;
        slider->callback = HandleUnitCountSlider;
    }

    FUN_00445e50_0044c7e0("SCROLLSLIDER", 0xd2, 0, UpdateUnitSliders);

    FUN_004a32a0(&g_game->gui, "DESCLIST", text, n, 0);
    SetGadgetRows(g_game->gui.table, "PICLIST", pics, n);
    UpdateUnitSliders(&g_game->gui, 0);

    {
        Gui_00446f50* gui = &g_game->gui;
        UnitType_00446f50* type = &g_game->unitTypes[desc->records[desc->field_ba].field_52];
        char buf[0x14];
        sprintf(buf, "%d", (int)type->energyCost);
        FUN_004a0bf0(gui, "ENERGYTEXT", buf, 0);
        sprintf(buf, "%d", (int)type->metalCost);
        FUN_004a0bf0(gui, "METALTEXT", buf, 0);
    }

    {
        int enabled = host == 0;
        FUN_004a1250(&g_game->gui, "Load", enabled);
        FUN_004a1250(&g_game->gui, "Save", enabled);
        FUN_004a1250(&g_game->gui, "Reset", enabled);
    }
    RenderLayer(&g_game->gui, 0x40);
    FUN_0049fb10(&g_game->gui, 1);
    DAT_005129c0 = 0;
}

// FUNCTION: 0x44ce20
Class_0044ce20::Class_0044ce20(int param_1)
{
    vtable = &DAT_004fd2f8;
    field_4 = param_1;
}
