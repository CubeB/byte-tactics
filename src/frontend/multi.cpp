// Decompiled by Space Bunny Free, deepseek-v4.1-flash, Haiku, GPT-6, deepseek-v4.1, space-bunny-free, claude-sonnet-5-5, Opus, DeepSeek V4.1 Flash, Claude Opus 5.5, Sonnet, Sonnet 5.5, deepseek-v4-flash, LongCat 2.5 Preview Free, GPT-6.1-sol, GPT-5.6-Terra, claude-opus-5-5, mimo-v2.6-pro and Haiku. Names are provisional.
//
// The multiplayer front end (0x440d70 to 0x44ce20): the new game, TCP,
// serial, modem and game list dialogs, the connection and service provider
// screens and the score reporting setup, the game setup dialogs (map, team
// logo, battle room, watching, alliances, the unit limit and the display
// mode chooser), the allies screen, the battle room's click handler and
// per-frame refresh, the end-of-multi screen, the save and load game lists
// and the unit restrictions dialog, in address order.
//
// 0x441220 and 0x441460 sit after the Game type, ahead of the other types and
// out of address order: their registers follow their symbol ids
// (docs/c2-regalloc.md). 0x443ff0, 0x449bb0, 0x44a680 and 0x44c420 stay in
// files of their own (docs/split-modules.md). 0x4441a0 and 0x444580 stay in
// multi_4441a0.cpp and multi_444580.cpp: they are gap regions
// (data/functions.csv), and place.py builds a gap region from a file that holds
// only that region's functions.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>
#include <ddraw.h>
#include <math.h>

struct Gadget;
struct Layer;
struct Gui;
struct Game;
struct Record_00446f50;
struct Options_00446f50;
struct PlayerInfo;
struct Player_00444930;
struct UnitType_00446f50;
struct Logos_00445110;
class UnitSync;

#pragma pack(push, 1)

// The 0x15b-byte menu control record. Entry 0 holds the count at +0xb6; the
// other entries hold NUL terminated text there. +0xcc is entry 0's label,
// +0xce the two-argument handler the menu calls for its own entries, and
// +0xd2 the record the entry is bound to. The views of the three parts are
// one type here: +0xb6's text, count and entry_text, +0xc2's text pointer,
// +0xce's handler and +0xd6's flags keep their part's name.
struct Gadget {                        // 0x15b bytes
    union {                            // +0x00
        unsigned char type;
        unsigned char state;
    };
    char team;                         // +0x01
    char name[0x11];                   // +0x02
    short x;                           // +0x13
    short y;                           // +0x15
    short width;                       // +0x17
    short height;                      // +0x19
    int attribs;                       // +0x1b
    int colours;                       // +0x1f
    int colour;                        // +0x23
    char unknown_27[0x29 - 0x27];      // +0x27
    union {                            // +0x29
        char active;
        unsigned char visible;
    };
    char unknown_2a[0xb6 - 0x2a];      // +0x2a
    union {                            // +0xb6
        char text[0x138 - 0xb6];       // the list or label text
        short count;                   // entry 0's count
        char entry_text[0xcc - 0xb6];  // one entry's own text
        struct {                       // entry 0's label
            char unknown_b6l[0xcc - 0xb6];
            char label[0x138 - 0xcc];  // +0xcc
        };
        struct {                       // the unit restrictions' flags list
            char unknown_b6f[0xbe - 0xb6];
            union {
                int hotspotGaf;        // +0xbe
                struct {
                    short unknown_be;
                    short count;       // +0xc0, the list's length
                } list;
            };
        };
        struct {                       // the dialog's own fields
            char unknown_b6b[0xba - 0xb6];
            union {                    // +0xba
                short selected;
                short field_ba;
                unsigned char field_ba_byte;
                short index;
            };
            union {                    // +0xbc
                struct {
                    char unknown_bc[0xc0 - 0xbc];
                    unsigned short rowCount;   // +0xc0
                };
                struct {
                    short firstRow;    // +0xbc
                    char unknown_be2[0xc0 - 0xbe];
                };
            };
            union {                    // +0xc2
                char* text_c2;
                void* hotspotFrame;
            };
            union {                    // +0xc6
                char unknown_c6[0xcc - 0xc6];
                void* rows;            // +0xc6
                struct {
                    short frame;       // +0xc6
                    union {
                        unsigned int hotspotFlags; // +0xc8
                        struct {
                            unsigned int c8_0 : 1;
                            unsigned int c8_rest : 31;
                        };
                    };
                };
            };
            char unknown_cc[0xce - 0xcc];  // +0xcc
            union {                    // +0xce
                void (__stdcall* handler)(Gui* menu, Gadget* entry);
                void (__stdcall* onSelect)(Gui* menu, int index);
                void* callback;
            };
            union {                    // +0xd2
                int data;
                Record_00446f50* records;
            };
            union {                    // +0xd6
                char* flags;
                unsigned char* bits;
            };
            short scroll;              // +0xda
            char unknown_dc[0x138 - 0xdc]; // +0xdc
        };
    };
    unsigned short field_138;          // +0x138
    unsigned char quickKey;            // +0x13a
    char unknown_13b;                  // +0x13b
    union {                            // +0x13c
        int max;
        struct {
            unsigned short field_13c_bit : 1;
            unsigned short unknown_13d : 15;
        };
        struct {
            unsigned short b13c_0 : 1;
            unsigned short b13c_rest : 15;
        };
    };
    short knobPos;                     // +0x140
    short knobSize;                    // +0x142
    void (__stdcall* sliderCallback)(Gui* menu, int index); // +0x144
    char unknown_148[2];               // +0x148
    void* sliderUser;                  // +0x14a
    char unknown_14e[0x15b - 0x14e];   // +0x14e
};


// The 16-byte group copied out of g_game+0x2b6e.
struct Group_00441220 {
    unsigned int a;                    // +0x00
    unsigned int b;                    // +0x04
    unsigned int c;                    // +0x08
    unsigned int d;                    // +0x0c
};

// The 0xb9-byte message 441220 copies into g_game+0x2ab1.
struct Msg_00441220 {
    char pad_0[0x99];
    Group_00441220 group;              // +0x99
    char pad_1[0xb9 - 0xa9];
};

// The 0xbc-byte message 4437c0 copies the group into.
struct Msg_004437c0 {
    char pad_0[0x99];
    Group_00441220 group;              // +0x99
    char pad_1[0x13];
};

// A DirectPlay connection block.
struct Conn_443ff0 {
    void* data;                        // +0x0
    int size;                          // +0x4
};

// The selected connection: its GUID and its block.
struct ConnInfo_443ff0 {
    GUID guid;                         // +0x0
    Conn_443ff0 conn;                  // +0x10
};

// The network object at g_game+0x14, as the HAPINET_ wrappers lay it out.
struct Net_00443100 {
    void* dp;                          // +0x00
    void* dp3;                         // +0x04
    char unknown_8[0x431 - 8];         // +0x08
    GUID* guids;                       // +0x431
    char unknown_435[0x4b9 - 0x435];   // +0x435
    void* lobby;                       // +0x4b9
    char unknown_4bd[0x4e5 - 0x4bd];   // +0x4bd
    int count;                         // +0x4e5
    int gameCount;                     // +0x4e9
    char unknown_4ed[0x505 - 0x4ed];   // +0x4ed
};

// One 0x54-byte game description: its size and the offset of its block.
struct Desc_004437c0 {
    char unknown_0[0x4c];              // +0x00
    int size;                          // +0x4c
    int offset;                        // +0x50
};

// One 0x102-byte account record: a name and a number string.
struct Entry_004426e0 {
    char name[0x81];                   // +0x00
    char number[0x81];                 // +0x81
};

// The serial address block written to g_serialPortNumber.
struct Serial_00441c30 {               // 0x14 bytes
    int unknown_0;                     // +0x00
    char unknown_4[4];                 // +0x04
    unsigned long unknown_8;           // +0x08
    unsigned long unknown_c;           // +0x0c
    unsigned long unknown_10;          // +0x10
};

// One element of the compound address list.
struct Elem_00441c30 {                 // 0x18 bytes
    GUID guid;                         // +0x00
    unsigned long size;                // +0x10
    void* data;                        // +0x14
};

// The record header of a game description block: a flag word with bitfields
// at offset 2, then the fields up to version.
struct Settings_00441460 {
    unsigned short memory;
    unsigned short players : 4;
    unsigned short playing : 1;
    unsigned short pad5 : 3;
    unsigned short black : 1;
    unsigned short nocmd : 1;
    unsigned short pad10 : 1;
    unsigned short mode : 2;
    unsigned short pad13 : 2;
    unsigned short lock : 1;
    unsigned short allyWinFlags;
    unsigned short pingLimit;
    unsigned short energy;
    unsigned short metal;
    unsigned short maxUnits;
    unsigned short version;
};

struct Record_00441460 {               // 0x54 bytes
    Settings_00441460 settings;        // +0x00
    int maxPlayers;                    // +0x10
    char name[0x20];                   // +0x14
    char name2[0x20];                  // +0x34
};

#include "../network/link_info.h"

#include "../network/player_info.h"

// The 0x14b-byte player slot at g_game+0x1b63: the union of the three parts'
// views (the info pointer at +0x27, the type at +0x73, the alliance at
// +0x13f). Kept out of the header: its field names and alias unions disagree
// with the header's, and this module's functions match only with this view.
struct Player_00444930 {
    int active;                        // +0x00
    int id;                            // +0x04
    unsigned int time;                 // +0x08
    char unknown_c[0x14 - 0xc];        // +0x0c
    unsigned int ping;                 // +0x14
    char unknown_18[0x22 - 0x18];      // +0x18
    union {                            // +0x22
        unsigned char status;
        unsigned char rejectReason;
    };
    char unknown_23[0x27 - 0x23];      // +0x23
    union {                            // +0x27
        PlayerInfo* info;
        PlayerInfo* data;
        PlayerInfo* unit;
    };
    char name[0x73 - 0x2b];            // +0x2b
    unsigned char type;                // +0x73
    char unknown_74[0x108 - 0x74];     // +0x74
    unsigned char allied[0xb];         // +0x108
    unsigned char alliedBy[0xb];       // +0x113
    char unknown_11e[0x13f - 0x11e];   // +0x11e
    union {                            // +0x13f
        unsigned char alliance;
        unsigned char colour;
    };
    int unitsCreated;                  // +0x140
    short unitCount;                   // +0x144
    unsigned char index;               // +0x146
    char unknown_147[0x14b - 0x147];   // +0x147
};

typedef Player_00444930 Player_441080;
typedef Player_00444930 Player_00446f50;

#include "../network/player.h"

// The mission object g_game+0x391e9 points at (defined in the game's own
// files).
#include "../map/mission.h"

// The layer LoadGuiLayer returns: the entry table at +4, the click handler at
// +8 and the dialog's block or owner at +0xc.
struct Layer {
    Layer* next;                       // +0x00
    Gadget* entries;                   // +0x04
    void* handler;                     // +0x08
    union {                            // +0x0c
        void* owner;
        void* layout;
        void* data;
        int field_c;
    };
    char unknown_10[0x1c - 0x10];      // +0x10
    void* cb1c;                        // +0x1c
    int current;                       // +0x20
    char unknown_24[0x37 - 0x24];      // +0x24
    int clickMode;                     // +0x37
};

typedef Layer Holder_00444930;
typedef Layer Layer_00446f50;

#include "../gui/gui.h"

// The game state. Every view of it in this module meets here: the fields are
// at the offsets the functions use, and the differently typed views of the
// flag words share a union.
struct Game {
    char unknown_0;                    // +0x00
    signed char version;               // +0x01
    char unknown_2[0x10 - 2];          // +0x02
    void* sound;                       // +0x10
    union {                            // +0x14
        Net_00443100 net;              // the HAPINET_ wrappers' view
        struct {
            char unknown_14[0x499 - 0x14]; // +0x14
            int field_499;             // +0x499
            char unknown_49d[0x519 - 0x49d]; // +0x49d
        };
    };
    Gui gui;                           // +0x519
    char unknown_120f[0x12ef - 0x120f]; // +0x120f
    char messages[30][0x48];           // +0x12ef
    char unknown_1b5f[0x1b63 - 0x1b5f]; // +0x1b5f
    Player_00444930 players[10];       // +0x1b63
    char unknown_2851[0x29a0 - 0x2851]; // +0x2851
    Options_00446f50* options;         // +0x29a0
    char unknown_29a4[0x2a30 - 0x29a4]; // +0x29a4
    UnitSync* sync;                    // +0x2a30
    char unknown_2a34[0x2a3c - 0x2a34]; // +0x2a34
    unsigned short numPlayers;         // +0x2a3c
    unsigned short chatHudWriteIdx;    // +0x2a3e
    unsigned short chatHudReadIdx;     // +0x2a40
    unsigned char localPlayer;         // +0x2a42
    char playerIndex;                  // +0x2a43
    union {                            // +0x2a44
        unsigned char field_2a44;
        unsigned char flags_2a44_byte;
        unsigned short flags_2a44;
        struct {
            unsigned short pad_2a44 : 2;
            unsigned short bit2 : 1;
            unsigned short rest_2a44 : 13;
        };
    };
    char netOrCdSentinel;              // +0x2a46
    union {                            // +0x2a47
        struct {
            char unknown_2a47[4];      // +0x2a47
            void* data[15];            // +0x2a4b
        };
        void* data16[16];              // +0x2a47
    };
    char unknown_2a87[0x2a9b - 0x2a87]; // +0x2a87
    char* chatter;                     // +0x2a9b
    GUID* sessions;                    // +0x2a9f
    Conn_443ff0* conns;                // +0x2aa3
    Desc_004437c0* desc;               // +0x2aa7
    char* shared;                      // +0x2aab
    union {                            // +0x2aaf
        unsigned short dplayAddressDialogFlags;
        struct {
            unsigned short bit0 : 1;
            unsigned short bit1 : 1;
            unsigned short bits2 : 14;
        };
    };
    char buffer[0xb9];                 // +0x2ab1
    char game_info[0x54];              // +0x2b6a
    char unknown_2bbe[0x2bbf - 0x2bbe]; // +0x2bbe
    unsigned char frontendSubstate;    // +0x2bbf
    union {                            // +0x2bc0
        unsigned char frontendSubstateRequest;
        char state;
    };
    char gameName[0x11];               // +0x2bc1
    char nickname[0x11];               // +0x2bd2
    char password[0xb];                // +0x2be3
    union {                            // +0x2bee
        unsigned short lobbyUiDirtyFlags;
        struct {
            unsigned short flag0 : 1;
            unsigned short bits1 : 15;
        };
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
    char unknown_2bf0[0x2c28 - 0x2bf0]; // +0x2bf0
    int playerIds[11];                 // +0x2c28
    char unknown_2c54[0x2c74 - 0x2c54]; // +0x2c54
    struct {                           // +0x2c74
        unsigned short locked : 1;
        unsigned short locked_rest : 15;
    };
    char unknown_2c76[0x2cbe - 0x2c76]; // +0x2c76
    signed char cursorMode;            // +0x2cbe
    char unknown_2cbf[0x1438f - 0x2cbf]; // +0x2cbf
    int count;                         // +0x1438f
    char unknown_14393[0x1439b - 0x14393]; // +0x14393
    UnitType_00446f50* unitTypes;      // +0x1439b
    char unknown_1439f[0x148d7 - 0x1439f]; // +0x1439f
    void* logos;                       // +0x148d7
    union {                            // +0x148db
        Logos_00445110* logos32;
        int field_148db;
    };
    char unknown_148df[0x37e1b - 0x148df]; // +0x148df
    int screen;                        // +0x37e1b
    char unknown_37e1f[0x37ebe - 0x37e1f]; // +0x37e1f
    unsigned short ordersPanelFlags;   // +0x37ebe
    char unknown_37ec0[0x37ee8 - 0x37ec0]; // +0x37ec0
    unsigned short lobbyInitScratch;   // +0x37ee8
    unsigned short maxUnits;           // +0x37eea
    unsigned short unitLimit;          // +0x37eec
    int difficulty;                    // +0x37eee
    char unknown_37ef2[0x37ef6 - 0x37ef2]; // +0x37ef2
    int commanderDeath;                // +0x37ef6
    char unknown_37efa[0x37f1b - 0x37efa]; // +0x37efa
    union {                            // +0x37f1b
        int displayWidth;
        unsigned short width;
    };
    union {                            // +0x37f1f
        int displayHeight;
        unsigned short height;
    };
    char unknown_37f23[0x37f39 - 0x37f23]; // +0x37f23
    int sides;                         // +0x37f39
    char unknown_37f3d[0x38a47 - 0x37f3d]; // +0x37f3d
    int frame;                         // +0x38a47
    char unknown_38a4b[0x38a51 - 0x38a4b]; // +0x38a4b
    union {                            // +0x38a51
        unsigned short flag_38a51 : 1;
        unsigned short bits_38a51 : 15;
    };
    char unknown_38a53[0x38c6b - 0x38a53]; // +0x38a53
    char save_38c6b[0x100];            // +0x38c6b
    char unknown_38d6b[0x391e9 - 0x38d6b]; // +0x38d6b
    Mission* map;                      // +0x391e9
    char unknown_391ed[0x39201 - 0x391ed]; // +0x391ed
    ConnInfo_443ff0 info;              // +0x39201
    char unknown_39219[0x39229 - 0x39219]; // +0x39219
    int commander;                     // +0x39229
    int mapping;                       // +0x3922d
    int los;                           // +0x39231
    int losType;                       // +0x39235
    char unknown_39239[0x3923b - 0x39239]; // +0x39239
    struct {                           // +0x3923b
        unsigned short bits0_3923b : 2;
        unsigned short flag2_3923b : 1;
        unsigned short bit3_3923b : 1;
        unsigned short flag4_3923b : 1;
        unsigned short rest_3923b : 11;
    };
};

// 0x441220 and 0x441460 sit here. The declarations they use came up from the
// prototype block with them; the first two below are used only further down,
// and are here for the symbol ids they take (docs/c2-regalloc.md).
void __stdcall SetGadgetText(void* gui, int index, char* text);
void __stdcall HandleNewMultiClick(Gui* gadget);
// GLOBAL: 0x511de8
extern Game* g_game;
extern char g_onlineLobbyPassword;
extern int DAT_00512c80;
extern GUID g_dpspGuidModem;
extern GUID g_dpspGuidTcpip;
extern GUID g_dpspGuidIpx;
extern GUID g_dpspGuidSerial;
int __stdcall FindGadgetIndex(void* entries, const char* name, int flag);
void __stdcall MarkChanged(void* menu);
void __stdcall BlitMenuLayers(void* menu, int a, int b);
void __stdcall SetGadgetActiveByName(void* menu, const char* name, int value);
void __stdcall ConfigureListBoxByName(void* menu, const char* name, char* text, int count, int flag);
void __stdcall CloseTopScreen(void* menu);
void __stdcall OpenMessageBox(void* menu, const char* text, int a, int b, int c);
char* __stdcall Translate(const char* text);
void __stdcall SetOffscreenSurface(int a);
void FlipScreen();
int __stdcall HAPINET_getgames(char* net, void* desc, int a);
void* __cdecl GameAllocIgnoreTag(const char* name, unsigned int size);
int IsOnlineConfigLoaded();
char* GetPreferredLanguage();
void __stdcall UpdateGameSelection(Gui* menu, Gadget* entry);
int __stdcall ConnectToGame(Layer* gadget);

static inline void Apply_00441220(unsigned short* p, unsigned short r, int ge, Gadget* entry)
{
    unsigned short on = (unsigned short)(((entry->rowCount == 0) | (ge == 0)) & 1);
    *p = (unsigned short)((r | on) | (*p & 0xfffe));
}

// FUNCTION: 0x441220
void __stdcall UpdateGameSelection(Gui* menu, Gadget* entry)
{
    void* gadgets = menu->layer->entries;
    // Reference to gv: makes ge a real 32-bit read, so ge == 0 tests edi.
    int gv; int& ge = gv;
    unsigned short* p;
    Gadget* gd;
    unsigned short on;
    unsigned short r;
    int idx = entry->index;
    Group_00441220* rec = (Group_00441220*)((char*)entry->records + idx * 0x54 + 4);
    Msg_00441220 msg;
    msg.group = *rec;
    memcpy(g_game->buffer, &msg, 185);

    unsigned short flags = *(unsigned short*)((char*)&msg.group + 2);
    int value = *(int*)((char*)&msg.group + 0xe);

    int index = FindGadgetIndex(gadgets, "WATCH", 1);
    if (index != -1) {
        gd = (Gadget*)((char*)gadgets + index * 0x15b);
        gv = ((value & 0xff) >= (int)g_game->version) ? 1 : 0;
        p = (unsigned short*)((char*)gd + 0x13c);
        r = (unsigned short)((~flags & 0x80) | (flags >> 8));
        r >>= 3;
        r |= flags & 0x10;
        r >>= 4;
        Apply_00441220(p, r, ge, entry);
    }
    index = FindGadgetIndex(gadgets, "JOINGAME", 1);
    if (index != -1) {
        gd = (Gadget*)((char*)gadgets + index * 0x15b);
        gv = ((value & 0xff) >= (int)g_game->version) ? 1 : 0;
        p = (unsigned short*)((char*)gd + 0x13c);
        r = (unsigned short)((flags >> 11) | (flags & 0x10));
        r >>= 4;
        Apply_00441220(p, r, ge, entry);
    }

    int password = msg.group.b & 1;
    SetGadgetActiveByName((char*)g_game + 0x519, "PASSWORDTEXT", password);
    SetGadgetActiveByName((char*)g_game + 0x519, "PASSWORD", password);
    MarkChanged(menu);
}

// FUNCTION: 0x441460
int __stdcall ConnectToGame(Layer* gadget) {
    int count;
    int left;
    int i;
    char* p[21];
    char names[0x20];
    char buf[0x80];
    // Never address-taken, bigger than temp and names so it lands after them.
    struct { char pre[0x99]; Settings_00441460 s; char pad[0x13]; } sb;
    const char* msg;
    char* lang;
#define temp (buf)

#define PE(g) (memcmp(&g_game->info.guid, &(g), 0x10) == 0)
    if (!PE(g_dpspGuidModem) && !PE(g_dpspGuidTcpip)) {
        if (PE(g_dpspGuidIpx))
            goto upd;
        // Stores through the unused p[20]: keeps both dead memcmp results.
        if (PE(g_dpspGuidSerial))
            ;
        else
            *(int*)&p[20] = memcmp(&g_game->info.guid, &g_dpspGuidSerial, 0x10) != 0;
    }
    // The a8 case falls into upd:, which is followed by conn:.
    if (PE(g_dpspGuidModem))
        goto conn;
    if (!PE(g_dpspGuidTcpip)) {
        if (!PE(g_dpspGuidIpx)) {
            if (PE(g_dpspGuidSerial))
                ;
            else
                *(int*)&p[20] = memcmp(&g_game->info.guid, &g_dpspGuidSerial, 0x10) != 0;
        }
        goto conn;
    }
upd:
    msg = "Updating...";
    goto shown;
conn:
    msg = "Connecting  (ESC to abort)";
shown:
    OpenMessageBox(&g_game->gui, Translate(msg), 0x96, 0, 1);
    BlitMenuLayers(&g_game->gui, g_game->screen, 0);
    SetOffscreenSurface(g_game->screen);
    FlipScreen();
    FlipScreen();

    count = HAPINET_getgames((char*)&g_game->net, g_game->desc, 0);
    CloseTopScreen(&g_game->gui);
    if (count < 0)
        return 0;

    i = 0;
    do {
        i++;
        p[i] = (char*)g_game->data16[i];
        memset(p[i], 0, 0xa00);
    } while (i < 15);

    // dsc is loaded before the if and used inside it.
    char* dsc = (char*)g_game->desc;
    if (count > 0) {
        p[0] = dsc + 0x18;
        // Separate counter: its store lands after the jle.
        left = count;
        do {
            char* e;
            sb.s = *(Settings_00441460*)(p[0] - 0x14);
            memcpy(names, p[0], 0x20);

            strncpy(p[1], names, 0x10);
            p[1][0x10] = 0;
            p[1] += strlen(p[1]) + 1;
            sprintf(p[2], "%d/%d", sb.s.players, ((Record_00441460*)(p[0] - 0x14))->maxPlayers);
            p[2] += strlen(p[2]) + 1;

            memset(temp, 0, 0x80);
            strncpy(temp, names + 0x10, 0xf);
            e = temp + strlen(temp);
            while (e != temp) {
                e--;
                if (*e != ' ')
                    break;
                *e = 0;
            }
            if (GetPreferredLanguage() != 0) {
                if (_strcmpi(GetPreferredLanguage(), "english") != 0) {
                    _strlwr(temp);
                    lang = Translate(temp);
                    strncpy(temp, lang, 0x80);
                    temp[0x7f] = 0;
                }
            }
            strcpy(p[3], temp);
            p[3] += strlen(p[3]) + 1;

            if ((sb.s.version & 0xff) >= (int)g_game->version) {
                if (sb.s.lock)
                    msg = "Lock";
                else if (sb.s.playing)
                    msg = "Play";
                else
                    msg = "Open";
                sprintf(p[4], "%s", Translate(msg));
            } else {
                sprintf(p[4], "%s", Translate("VER!"));
            }
            p[4] += strlen(p[4]) + 1;
            sprintf(p[5], "%d", sb.s.memory);
            p[5] += strlen(p[5]) + 1;
            sprintf(p[6], "%d", sb.s.metal * 100);
            p[6] += strlen(p[6]) + 1;
            sprintf(p[7], "%d", sb.s.energy * 100);
            p[7] += strlen(p[7]) + 1;
            sprintf(p[8], "%d", sb.s.pingLimit);
            p[8] += strlen(p[8]) + 1;

            if (sb.s.mode != 0) {
                if (sb.s.mode == 1)
                    msg = "Yes";
                else
                    msg = "DM";
            } else {
                msg = "No";
            }
            sprintf(p[9], "%s", Translate(msg));
            p[9] += strlen(p[9]) + 1;

            sprintf(p[10], "%s", Translate(sb.s.black ? "Blk" : "Gray"));
            p[10] += strlen(p[10]) + 1;
            sprintf(p[11], "%s", Translate(sb.s.nocmd ? "No" : "Yes"));
            p[11] += strlen(p[11]) + 1;

            p[0] += 0x54;
        } while (--left);
    }

    ConfigureListBoxByName(&g_game->gui, "GAMENAME", (char*)g_game->data16[1], g_game->net.gameCount, 0);
    ConfigureListBoxByName(&g_game->gui, "PLAYERS", (char*)g_game->data16[2], g_game->net.gameCount, 0);
    ConfigureListBoxByName(&g_game->gui, "MAPNAME", (char*)g_game->data16[3], g_game->net.gameCount, 0);
    ConfigureListBoxByName(&g_game->gui, "STATUS", (char*)g_game->data16[4], g_game->net.gameCount, 0);
    ConfigureListBoxByName(&g_game->gui, "METAL", (char*)g_game->data16[6], g_game->net.gameCount, 0);
    ConfigureListBoxByName(&g_game->gui, "ENERGY", (char*)g_game->data16[7], g_game->net.gameCount, 0);
    ConfigureListBoxByName(&g_game->gui, "COMMANDER", (char*)g_game->data16[9], g_game->net.gameCount, 0);
    ConfigureListBoxByName(&g_game->gui, "LOS", (char*)g_game->data16[11], g_game->net.gameCount, 0);
    ConfigureListBoxByName(&g_game->gui, "PING", (char*)g_game->data16[8], g_game->net.gameCount, 0);
    ConfigureListBoxByName(&g_game->gui, "FULLMAP", (char*)g_game->data16[10], g_game->net.gameCount, 0);

    i = FindGadgetIndex(gadget->entries, "GAMENAME", 2);
    if (i != -1)
        UpdateGameSelection(&g_game->gui, gadget->entries + i);
    return 1;
}
#undef PE
#undef temp

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

// The map selector's layout block at layer+0xc (0x444ea0).
struct Data_00444ea0 {
    char unknown_0[0x14];                  // +0x00
    char* items;                           // +0x14
};

// The map dialog's layout block at holder+0xc (0x444cb0).
struct Layout_00444cb0 {
    char unknown_0[0x14];                  // +0x00
    void* items;                           // +0x14
};

// The logo dialog's layout block at holder+0xc (0x445110): the byte list of
// the free logo indices, the pointer list handed to the GUI and the copied
// animation sequences.
struct AnimSeq_00445110 {
    char unknown_0[0x28];                  // +0x00
    void* frame;                           // +0x28
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
    void* ptrList;                         // +0x18
    void* seqs;                            // +0x1c
};

// The display mode list 0x4461d0 and 0x446310 share.
struct Mode_00446310 {
    int width;                             // +0x00
    int height;                            // +0x04
    int refreshRate;                       // +0x08
};

struct ModeList {
    int count;                             // +0x00
    Mode_00446310* modes;                  // +0x04
    char unknown_8[0x14 - 0x8];            // +0x08
    char* available;                       // +0x14
    char unknown_18[0x20 - 0x18];          // +0x18
};

// The first 0x13e bytes of an entry, copied out by 0x445300 and 0x4455b0.
struct Head_00444930 {
    unsigned char state;                   // +0x00
    char team;                             // +0x01
    char name[0x13];                       // +0x02
    short y;                               // +0x15
    char unknown_17[2];                    // +0x17
    short height;                          // +0x19
    int attribs;                           // +0x1b
    char unknown_1f[0x29 - 0x1f];          // +0x1f
    unsigned char active;                  // +0x29
    char unknown_2a[0xb6 - 0x2a];          // +0x2a
    char text[0x13e - 0xb6];               // +0xb6
};

// One 0x62-byte record of the unit restrictions table at 0x5129b4.
struct Record_00446f50 {
    char name[0x52];                   // +0x00
    int unitIndex;                     // +0x52 unit type index
    int previousMax;                   // +0x56 previous value
    int max;                           // +0x5a value
    int peerEnabled;                   // +0x5e
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
            int fbiChecksum;           // +0x13e
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
    } flags2;
};

struct Event_44c220 {
    int fbiChecksum;                   // +0x00
    int field_4;                       // +0x04
    short enabled;                     // +0x08
    short peerEnabled;                 // +0x0a
    int max;                           // +0x0c
};

struct Info_0044c7e0 {                  // filled by UnitSync::GetUnitEntry
    char unknown_0[0xa];
    short peerEnabled;                 // +0x0a
    int max;                           // +0x0c
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

struct Record_0044c0d0 {
    unsigned short a;                  // +0x00
    unsigned short b;                  // +0x02
    unsigned short e;                  // +0x04
    unsigned short f;                  // +0x06
    unsigned char flag8;               // +0x08
    unsigned char flag9;               // +0x09
    unsigned char flaga;               // +0x0a
    unsigned char flagb;               // +0x0b
    int reserved;                      // +0x0c
    int d;                             // +0x10
    int scratch;                       // +0x14
};

class OrderFx
{
public:
    void* vtable;
    int source;

    OrderFx(int param_1);
};

class PacketManager {
public:
    int SendAllQueued(int value);
};

#pragma pack(pop)

class Class_004a1080;
struct Dialog;
struct Struct_004c6ac0;

// Unused here: the symbol id this declaration takes keeps the allocation
// (docs/c2-regalloc.md).
void EnableAICommands(void);

// Unused here: real functions declared to keep the file's symbol count.
void RegisterUnitOrders();
void RegisterGroundOrders();
void ResetAIPlayers();
void InitCommands();

typedef void (__stdcall* Callback_00449bb0)(Gui* gui, int index);
typedef void (__stdcall* Callback_0044c7e0)(Gui* gui, int index);

// GLOBAL: 0x512c84
extern int g_cmdlineHostMode;
// GLOBAL: 0x512d90
extern char g_cmdlineTcpJoinAddress[];
extern int g_unitRestrictPicLoadIndex;
extern int g_unitRestrictPicCursor;
extern int g_unitRestrictRecordCursor;
extern int g_startCountdownNextTick;
extern unsigned int g_heartbeatNextTick;
extern char* g_saveListFileNames;
extern char* g_saveListDisplayNames;
extern int* g_unitRestrictPics;
extern int DAT_005129c0;
extern int* g_unitRestrictOldCounts;
// GLOBAL: 0x5129c8
extern int g_unitRestrictNextPicTick;
extern unsigned int g_lastPlayerCount;
extern char* g_savegameDir;            // savegame directory
extern char DAT_005119b8[];
extern char g_star[];                  // "*"
extern char g_lstExtension[];          // "LST"
extern char g_gamesGadgetName[];       // "GAMES"
extern char g_savegameNamesName[];     // "SAVEGAME NAMES"
extern char g_savegameDescsName[];     // "SAVEGAME DESCS"
extern char* g_hostOnlyGadgets[];
extern char* g_battleRoomGadgetNames[];
extern char g_lobbyMapName[];
extern int g_lobbyLock;
extern int g_lobbyMaxUnits;
extern int g_lobbyEnergy;
extern int g_lobbyMetal;
extern int g_lobbyCommander;
extern int g_lobbyLineOfSight;
extern int g_lobbyCheating;
extern int g_lobbyFixedLoc;
extern int g_lobbyMapping;
extern int g_lobbyWatching;
extern void* g_orderFxVtable;
extern int g_usePacketManager;
extern PacketManager g_packetManager;
// GLOBAL: 0x5129b4
extern Record_00446f50* g_unitRestrictEntries;
extern char g_onlineLobbyPlayerName;
extern char* g_modemInfo;
extern int g_modemCount;
extern Entry_004426e0* g_modemAccounts;
extern char* g_modemAccountNames;
extern GUID DPAID_Modem;
extern GUID DPAID_ServiceProvider;
extern GUID DPAID_Phone;
extern GUID DPAID_INet;
extern GUID DPAID_ComPort;
extern char DAT_004fcfb8[];
extern Serial_00441c30 g_serialPortNumber;
extern int g_serialBaudRate;
extern int g_serialBaudRateTable[];
extern unsigned int g_reporterCount;
extern unsigned int g_reporterNames;
extern LinkInfo g_linkInfo[];
extern char g_okGadgetName[];              // "OK"
extern char g_multiSoundName[];            // "Multi"
extern char* g_oldMapName;
extern int DAT_00512760;
extern short g_battleRoomBaseGadgetCount;
extern int DAT_0051276c;
extern int g_battleRoomSlotsBuilt;
extern int g_rejectPlayer;

Gadget* __stdcall FindGadgetChecked(void* entries, const char* name);
int __stdcall IsCurrentGadgetNamed(void* menu, const char* name);
int __stdcall IsCurrentGadgetNamed(Gui* gadget, const char* name);
Gadget* __stdcall FindGadgetOrNull(void* entries, const char* name);
Gadget* __stdcall FindGadgetChecked_B(Gadget* entries, const char* name);
Gadget* __stdcall FindGadgetChecked_B(void* entries, const char* name);
Gadget* __stdcall FindGadgetChecked_C(void* entries, const char* name);
Gadget* __stdcall FindGadgetChecked_D(void* entries, char* name);
Gadget* __stdcall FindGadgetChecked_E(void* entries, char* name);
void __stdcall SelectGadgetByIndex(void* menu, int index);
void __stdcall BeginTextEdit(void* menu, int index);
int __stdcall TrySetFocus(void* menu, int index);
void __stdcall EnableKeyCommands(void* gui);
void __stdcall SetKeyboardInput(void* menu, int value);
void __stdcall MarkLayerChanged(void* menu);
void __stdcall ClearSelectedGadget(void* menu);
void __stdcall SetDescListCleanupFlag(void* gui, int flag);
void __stdcall SetTranslatedTextByName(void* menu, const char* name, char* text, int param_4);
void __stdcall SetGrayedOutByName(void* menu, const char* name, int value);
void __stdcall SetGrayedOutByName(void* gui, char* name, int value);
void __stdcall SetListBoxScrollByName(void* menu, const char* name, int index);
void __stdcall SetTranslatedText(void* menu, int index, int param_3, int param_4);
void __stdcall SetGadgetGrayedOutByName(void* menu, char* name, int value);
void __stdcall SetCurrentFont(void* gui, int flag);
int __stdcall TruncateGadgetText(void* menu, int index);
char* __stdcall GetGadgetText(void* menu, const char* key, char* out);
int __stdcall GetGadgetStatus(void* menu, int handle);
char __stdcall GetGadgetActiveByName(void* menu, char* name);
int __stdcall GetButtonStage(void* gadget, int index);
int __stdcall GetButtonStageByName(void* gadget, char* name);
void __stdcall SetButtonStageByName(void* gui, char* name, int value);
int __stdcall SetButtonStageByName(Class_004a1080* obj, char* name, int value);
void __stdcall SetGadgetStatusByName(void* gui, char* name, int value);
void __stdcall SetGadgetName(void* gui, char* name, char* text);
void __stdcall SetGadgetRows(void* table, char* name, int* pics, int count);
void __stdcall DrawButton(void* gadget, int value);
void __stdcall LoadPictureCached(const char* name, int a, int b, int c);
Layer* __stdcall LoadGuiLayer(void* menu, const char* name, int flags);
void __stdcall RenderLayer(void* menu, int value);
void __stdcall PlaySoundByName(const char* name, int flag);
void __stdcall PlaySoundByName(char* str, int flag);
void BlankScreen();
char* __stdcall Translate(char* text);
void __stdcall SetFrontendState(int state, int line, const char* file);
void __stdcall SetFrontendState(int a, int line, char* file);
void __stdcall SetFrontendErrorText(char* text);
void __stdcall SetFrontendSubState(char state, int line, char* file);
int __stdcall ReadGameRegistryValue(const char* key, void* buf, unsigned int* size);
void __stdcall WriteGameRegistryValue(void* key, void* buf, int value);
void __stdcall SetCursorMode(int value);
void __stdcall EnableReporter(int value);
int __stdcall BuildCompoundAddress(int* addressOut, int* sizeOut);
int __stdcall HAPINET_createcompoundaddress(void* net, void* elements, unsigned long count,
                           void* address, unsigned long* size);
int __stdcall HAPINET_initlobbiedconnection(void* net);
int __stdcall HAPINET_createdplayinterface(GUID* sp, void* net);
int __stdcall HAPINET_getplayeraddress(void* net, unsigned long player, void* data,
                           unsigned long* size);
int __stdcall HAPINET_enumaddress(void* net, void* callback, void* address, unsigned long size,
                           void* context);
int __stdcall HAPINET_releasedplayinterface(void* net);
void __stdcall HAPINET_uninitmultiplay(void* net);
void __stdcall HAPINET_getconnections(void* net, void* guids, void* conns,
                           void* descriptions, void* param_5);
void* __cdecl GameAllocIgnoreTag(char* name, unsigned int size);
void __cdecl GameFreeThunk(void* p);
void OrLabelAttribs();
int __stdcall LoadReporterDll(unsigned int* a, unsigned int* b);
void __stdcall RunWhileScreenNamed(void* p, char* name);
unsigned char FindHostSlot();
char* __stdcall GetRejectReasonText(int value);
unsigned int __stdcall OnlineGetLinkInfo(LinkInfo* links);
int __stdcall OnlineProcessButtonCommand(int button, char* message, unsigned int size);
void OnlineUnload();
void OpenOptionsPanel();
// Defined in multi_443ff0.cpp: gathered here, its pointer into the session list
// lands in ebp, and no symbol count in this file moves it back.
int __stdcall SelectConnection(int index);
void __stdcall SetPaletteColors(unsigned char* palette, int first, int count);
void __stdcall RemapPaletteToClosestIndices(void* menu, void* palette, void* param_3);
void SendNetHeartbeat(void);
void __stdcall OpenSelectGameDialog();
int InitScoreReporting();
void __stdcall HandleSerialDialogClick(Gui* gadget);
void __stdcall SetSerialBaudFromGadget(Gui* menu, Gadget* entry);
void __stdcall SetSerialPortFromGadget(Gui* menu, Gadget* entry);
void __stdcall HandleTcpDialogClick(Gui* gadget);
void __stdcall HandleModemDialogClick(Gui* gadget);
void __stdcall HandleSelectGameClick(Gui* menu);
void __stdcall HandleReportClick(Gui* obj);
void __stdcall ShowSelectedAccount(Gui* menu, Gadget* entry);
void __stdcall OpenReportDialog(unsigned int* count, char** names);
void FillAccountList(void);
int __stdcall CloneServiceSlot(Gadget* entries, int param_2, short param_3,
                           int param_4, char* param_5);
void ReportDialogFrame(void);

void __stdcall RequestPlayerColor(int value);
void* __stdcall LoadRadarPic(char* path, int* outX, int* outY);
void __stdcall ResizeRadarPicture(void* bmp, int param_2, int param_3, int param_4, int param_5);
char* __stdcall SkipTextLines(char* text, int n);
void __stdcall ReportGameEvent(int msg);
void BroadcastPlayerInfo(void);
void UpdateNetGameInfo(void);
int __stdcall LoadMapList(char** out, int param_2, int param_3);
void __stdcall SortFileList(char* items, int b, int c, int count);
void __stdcall SetGadgetItems(void* gui, const char* name, void** items, int count);
int __stdcall ReadSliderValue(void* gadget);
void __stdcall SetSliderFromValue(Gadget* gadget, int value);
int __stdcall IsGadgetNamed(int param1, int param2, char* name);
int __stdcall GetSlotDpid(unsigned char player);
void __stdcall RejectPlayer(int param_1, int param_2);
void SaveSettings(void);
int __stdcall GetDisplayModes(ModeList* obj);
void __stdcall SortDisplayModes(ModeList* obj);
void __stdcall RefreshAlliesScreen(int value);
void __stdcall OpenRejectDialog(int player);
void __stdcall BroadcastAllyTeam(Player_00444930* player);
int IsHostLocal();
void __stdcall HandleControlDialogClick(Gui* gadget);
void __stdcall SetAlliance(int, int, int, int);
void __stdcall SetAlliance(int a, int b, unsigned char allied, int d);
void ShowSelectedMapInfo(void);
void __stdcall HandleViewMapClick(Gui* gadget);
void __stdcall HandleMapSelectClick(Gui* gadget);
void __stdcall HandleLogoSelectClick(Gui* gadget);
void __stdcall HandleRejectChoice(Gui* gadget);
void __stdcall UpdateMapSelection(Gui* menu, int unused);

void __stdcall SetGameMode(int a);
void LeaveNetGame();
void* __stdcall LoadBitmapByName(char* name, unsigned char* palette);
void __stdcall DrawSurface(void* dest, void* image, int x, int y);
void __stdcall FreeSurface(void* image);
void ShowSoftwareCursor();
void __stdcall MakeDirectoryPath(char* path);
void __stdcall RemoveFile(char* path);
void __stdcall BuildDataPath(char* out, const char* dir, const char* name, const char* ext);
int __stdcall CountDirectoryEntries(const char* path, int flag);
int __stdcall ScanDirectory(char* path, void* buffer, char* p3, int p4, int p5, int p6);
void __stdcall HandleAlliesClick(Gui* gadget);
void __stdcall HandleBattleRoomClick(Gui* gadget);
void __stdcall HandleEndMultiClick(Gui* gadget);
void __stdcall HandleLoadListClick(Gui* menu);
void __stdcall HandleRestrictionsClick(Gui* menu);
void __stdcall HandleSaveGameClick(Gui* menu);
void __stdcall HandleUnitCountSlider(void* obj, char* gadget);
void __stdcall UpdateSideGadget(int side);
void __stdcall UpdateUnitSliders(Gui* gui, int value);
void __stdcall UpdateMaxUnitsText(Gui* gui, int index);
void __stdcall UpdateMetalText(Gui* gui, int index);
void RefreshBattleRoomRows();
void RebuildAllyList();
void OpenLoadListDialog();
void OpenSaveGameDialog();
void OpenUnitRestrictions();
void UnitRestrictDialogFrame();
int __stdcall IsScreenNamed(void* gui, const char* name);
void __stdcall GetGadgetRectByIndex(Gadget* entries, int widget, RECT* rect);
void __stdcall DrawTextClipped(int a, char* text, int x, int y, int w, int h);
int AreAllPlayersReady();
void __stdcall CreateUnitSync(int param_1);
char __stdcall FindGameCdDrive(int side);
int __stdcall HandleNetPackets();
void* __stdcall LoadPcx(char* path, int param_2);
void __stdcall FrameFromSurface(void* dst, void* src);
void* __stdcall FindGafEntry(void* gaf, const char* name);
void FatalError(char* message);
int CountLocalComputerPlayers();
unsigned int GetTicks();
void __stdcall AddMessage(char* text, int a, int b, int c);
void __stdcall SendChatMessage(void* from, char* text, int a, int b);
int __stdcall CreateLocalPlayer(unsigned char player, int state);
int __stdcall GetFontLineHeight();
int __stdcall GetTextPixelWidth(char* text);

// Click handler of the "create new game" dialog (NEWMULTI.GUI, opened by
// OpenNewMultiDialog). Clicking a text field gives it the focus and loads its
// contents; OK copies the password (bounded to 11 characters) into g_game and
// the game and player names into two stack buffers, complains if either is
// empty, then starts the game (InitScoreReporting) and sets the pending front-end
// state at g_game+0x2bc0 to 17. CANCEL goes back to the previous dialog.
// FUNCTION: 0x440d70
void __stdcall HandleNewMultiClick(Gui* gadget)
{
    Gadget* entries = gadget->layer->entries;
    if (gadget->hotGadgetIndex == -1)
        return;
    // The three text fields share one tail: the first two hand the next field
    // the focus, the third one the OK button.
    if (IsCurrentGadgetNamed(gadget, "GAMENAME")) {
        SelectGadgetByIndex(gadget, FindGadgetIndex(entries, "NICKNAME", 3));
        TrySetFocus(gadget, FindGadgetIndex(entries, "NICKNAME", 3));
        MarkChanged(gadget);
        ClearSelectedGadget(gadget);
        return;
    }
    if (IsCurrentGadgetNamed(gadget, "NICKNAME")) {
        SelectGadgetByIndex(gadget, FindGadgetIndex(entries, "PASSWORD", 3));
        TrySetFocus(gadget, FindGadgetIndex(entries, "PASSWORD", 3));
        MarkChanged(gadget);
        ClearSelectedGadget(gadget);
        return;
    }
    if (IsCurrentGadgetNamed(gadget, "PASSWORD")) {
        SelectGadgetByIndex(gadget, FindGadgetIndex(entries, "OK", 1));
        MarkChanged(gadget);
        ClearSelectedGadget(gadget);
        return;
    }
    if (FindGadgetIndex(entries, "OK", 0xe) == gadget->hotGadgetIndex) {
        // 100 bytes each, and the pointer local in front of them, is what
        // puts them at +0x14 and +0x78 of the 0xcc-byte frame.
        char nickbuf[100];
        char namebuf[100];
        char* dst;
        int gi;                        // GAMENAME gadget index
        int ni;                        // NICKNAME gadget index
        PlaySoundByName("BigButton", 0);
        char* pw = (char*)FindGadgetChecked_B(entries, "PASSWORD");
        lstrcpynA(g_game->password, pw + 0xb6, 0xb);
        gi = FindGadgetIndex(entries, "GAMENAME", 3);
        dst = namebuf;                 // copied through a pointer, as in the original
        strcpy(dst, entries[gi].text);
        if (strlen(namebuf) == 0) {
            BeginTextEdit(gadget, gi);
            ClearSelectedGadget(gadget);
            OpenMessageBox((char*)gadget, Translate("You must enter a game name"), 0x140, 1, 1);
            return;
        }
        ni = FindGadgetIndex(entries, "NICKNAME", 3);
        strcpy(nickbuf, entries[ni].text);
        if (strlen(nickbuf) == 0) {
            BeginTextEdit(gadget, ni);
            ClearSelectedGadget(gadget);
            OpenMessageBox((char*)gadget, Translate("You must enter your name"), 0x140, 1, 1);
            return;
        }
        strcpy(g_game->gameName, namebuf);
        strcpy(g_game->nickname, nickbuf);
        if (!InitScoreReporting()) {
            g_game->frontendSubstateRequest = 0x11;
            return;
        }
        ClearSelectedGadget(gadget);
        return;
    }
    if (FindGadgetIndex(entries, "CANCEL", 0xe) == gadget->hotGadgetIndex) {
        PlaySoundByName("Previous", 0);
        CloseTopScreen(gadget);
        OpenSelectGameDialog();
        return;
    }
    ClearSelectedGadget(gadget);
}

// Opens the new multiplayer game dialog (NEWMULTI.GUI), sets its click
// handler, fills in the game, player and password fields and shows it.
// FUNCTION: 0x441080
void OpenNewMultiDialog()
{
    DWORD size;
    Layer* layer = LoadGuiLayer(&g_game->gui, "NEWMULTI.GUI", 0x80);
    layer->handler = HandleNewMultiClick;
    layer->owner = g_game;
    LoadPictureCached("createnew", 0, 0, 0);
    Gadget* entries = layer->entries;
    if (IsOnlineConfigLoaded() && g_onlineLobbyPlayerName != 0) {
        g_game->nickname[0] = 0;
        strncat(g_game->nickname, &g_onlineLobbyPlayerName, 0x10);
    }
    if (strlen(g_game->nickname) == 0) {
        size = 0x11;
        GetUserNameA(g_game->nickname, &size);
    }
    Gadget* gname = FindGadgetChecked_B(entries, "GAMENAME");
    SetTranslatedTextByName(&g_game->gui, "GAMENAME", g_game->gameName, 0);
    gname->field_138 = 0x10;
    Gadget* nname = FindGadgetChecked_B(entries, "NICKNAME");
    SetTranslatedTextByName(&g_game->gui, "NICKNAME", g_game->nickname, 0);
    nname->field_138 = 0x10;
    char* pw = g_game->players[g_game->localPlayer].info->password;
    if (strlen(pw) == 0)
        pw = g_game->password;
    SetTranslatedTextByName(&g_game->gui, "PASSWORD", pw, 0xa);
    OrLabelAttribs();
    SetKeyboardInput(&g_game->gui, 1);
    RenderLayer(&g_game->gui, 0x40);
}

// Returns the local player's name, the byte at g_game+0x2a42 selecting the
// player record and +0x1b8a (0x1b63 + 0x27) its info block; the name is at
// +0x80 of that block.
// FUNCTION: 0x441430
int __cdecl GetLocalPlayerPassword()
{
    int ecx = (int)g_game;
    unsigned int edx = *(unsigned char*)(ecx + 0x2a42);
    int eax = edx;
    ecx = ecx + edx;
    eax = eax << 5;
    eax = eax + edx;
    eax = eax + eax * 4;
    eax = *(int*)(ecx + eax * 2 + 0x1b8a);
    return eax + 0x80;
}

// Maps the selected DirectPlay service provider GUID to an index:
// 0 modem, 1 TCP/IP, 2 IPX, 3 serial, 4 anything else. The four globals hold
// g_dpspGuidModem, g_dpspGuidTcpip, g_dpspGuidIpx and g_dpspGuidSerial.
// FUNCTION: 0x441bc0
int GetServiceProviderIndex()
{
    GUID* guid = &g_game->info.guid;
    if (memcmp(guid, &g_dpspGuidModem, sizeof(GUID)) == 0) {
        return 0;
    }
    if (memcmp(guid, &g_dpspGuidTcpip, sizeof(GUID)) == 0) {
        return 1;
    }
    if (memcmp(guid, &g_dpspGuidIpx, sizeof(GUID)) == 0) {
        return 2;
    }
    if (memcmp(guid, &g_dpspGuidSerial, sizeof(GUID)) == 0) {
        return 3;
    }
    return 4;
}

// Builds a DirectPlay compound address for the service provider selected in
// g_game + 0x39201 and hands it to the lobby's CreateCompoundAddress.
// Returns 0 on success, with the address block and its size in the two out
// parameters; otherwise the HRESULT from the lobby.
// FUNCTION: 0x441c30
int __stdcall BuildCompoundAddress(int* addressOut, int* sizeOut)
{
    // block before size, all three buffers at function top: prologue and frame layout.
    HGLOBAL block = 0;
    unsigned long size = 0;
    Elem_00441c30 elements[3];
    GUID guid;
    char buf1[200];
    char buf2[200];
    char buf3[200];
    unsigned long count;
    int result;

    guid = g_game->info.guid;

    if (memcmp(&guid, &g_dpspGuidModem, sizeof(GUID)) == 0) {
        elements[0].guid = DPAID_ServiceProvider;
        elements[0].size = 0x10;
        elements[0].data = &g_dpspGuidModem;
        // memset, not = "": plain rep stosd.
        memset(buf1, 0, sizeof(buf1));
        char* s = g_modemInfo;
        if (s == 0) {
            s = DAT_005119b8;
        }
        lstrcpyA(buf1, s);
        elements[1].guid = DPAID_Modem;
        elements[1].size = lstrlenA(buf1) + 1;
        elements[1].data = buf1;
        lstrcpyA(buf2, GetGadgetText(&g_game->gui, "NUMBER", 0));
        elements[2].guid = DPAID_Phone;
        elements[2].size = lstrlenA(buf2) + 1;
        elements[2].data = buf2;
        count = 3;
    } else if (memcmp(&guid, &g_dpspGuidTcpip, sizeof(GUID)) == 0) {
        elements[0].guid = DPAID_ServiceProvider;
        elements[0].size = 0x10;
        elements[0].data = &g_dpspGuidTcpip;
        char* t = GetGadgetText(&g_game->gui, "ADDRESS", 0);
        if (t == 0) {
            t = DAT_005119b8;
        }
        lstrcpyA(buf3, t);
        elements[1].guid = DPAID_INet;
        elements[1].size = lstrlenA(buf3) + 1;
        elements[1].data = buf3;
        count = 2;
    } else if (memcmp(&guid, &g_dpspGuidIpx, sizeof(GUID)) == 0) {
        elements[0].guid = DPAID_ServiceProvider;
        elements[0].size = 0x10;
        elements[0].data = &g_dpspGuidIpx;
        count = 1;
    } else if (memcmp(&guid, &g_dpspGuidSerial, sizeof(GUID)) == 0) {
        elements[0].guid = DPAID_ServiceProvider;
        elements[0].size = 0x10;
        elements[0].data = &g_dpspGuidSerial;
        g_serialPortNumber.unknown_8 = 0;
        g_serialPortNumber.unknown_c = 0;
        g_serialPortNumber.unknown_10 = 3;
        elements[1].guid = DPAID_ComPort;
        elements[1].size = 0x14;
        elements[1].data = &g_serialPortNumber;
        count = 2;
    } else {
        elements[0].guid = DPAID_ServiceProvider;
        elements[0].size = 0x10;
        elements[0].data = &guid;
        count = 1;
    }

    result = HAPINET_createcompoundaddress(&g_game->net, elements, count, 0, &size);
    if (result != 0x8877001e) goto cleanup;
    block = (HGLOBAL)GameAllocIgnoreTag("COMPOUND ADDR", size);
    if (block == 0) {
        result = 0x8007000e;
        goto cleanup;
    }
    result = HAPINET_createcompoundaddress(&g_game->net, elements, count, block, &size);
    if (result < 0) {
        // Label inside this body, reached by goto: the shared cleanup block is the true arm.
cleanup:
        if (block != 0) {
            GlobalUnlock(GlobalHandle(block));
            GlobalFree(GlobalHandle(block));
        }
        return result;
    }
    *addressOut = (int)block;
    *sizeOut = size;
    return 0;
}

// FUNCTION: 0x442000
int __stdcall TryConnect(int unused)
{
    int a = 0;
    int b = 0;
    int result = BuildCompoundAddress(&a, &b);
    if (result >= 0) {
        g_game->info.conn.data = (void*)a;
        result = 0;
        g_game->info.conn.size = b;
    }
    return result;
}

// Click handler of the TCP dialog (TCP.GUI, opened by OpenTcpDialog). OK and a
// direct-connect address both set the multiplayer connection-mode bits of
// g_game (+0x2aaf) and connect; JOIN sets only the second bit; ADDRESS selects
// the address gadget; PREV goes back; anything else resets the gadget. The
// address text is then written to the registry under TCPADDR.
// The original build called this helper from both connection paths and /Ob2
// inlined it at each one; 0x442000 is the out-of-line copy of the same body.
// Writing the two zero stores inside the helper, rather than inline in the
// caller, is what makes MSVC 5 schedule the first store after the argument
// push (`mov [esp+0x1c], ebx`), matching the original. Spelling the same
// statements inline in HandleTcpDialogClick hoists that store one instruction early.
static int TryConnect_00442050()
{
    int a = 0;
    int b = 0;
    int result = BuildCompoundAddress(&a, &b);
    if (result >= 0) {
        g_game->info.conn.data = (void*)a;
        g_game->info.conn.size = b;
    }
    return result;
}

// FUNCTION: 0x442050
void __stdcall HandleTcpDialogClick(Gui* gadget)
{
    Gadget* entries = gadget->layer->entries;
    if (g_cmdlineTcpJoinAddress[0] != 0) {
        if (g_cmdlineHostMode == 0)
            g_cmdlineTcpJoinAddress[0] = 0;
    }
    else if (gadget->hotGadgetIndex == -1) {
        return;
    }
    else if (FindGadgetIndex(entries, "OK", 0xe) == gadget->hotGadgetIndex) {
        goto connect;
    }
    else if (FindGadgetIndex(entries, "JOIN", 0xe) == gadget->hotGadgetIndex) {
        g_game->bit1 = 1;
        g_game->bit0 = 0;
        TryConnect_00442050();
        PlaySoundByName("Smlbutton", 0);
        goto tcpaddr;
    }
    else if (IsCurrentGadgetNamed(gadget, "ADDRESS") != 0) {
    }
    else {
        if (IsCurrentGadgetNamed(gadget, "PREV") != 0) {
            SetFrontendState(0xf, 0x305, "c:\\cavedog\\wargame\\multi.cpp");
            PlaySoundByName("Previous", 0);
            return;
        }
        ClearSelectedGadget(gadget);
        return;
    }
connect:
    g_game->bit0 = 1;
    g_game->bit1 = 1;
    TryConnect_00442050();
    PlaySoundByName("Smlbutton", 0);
    BlankScreen();
tcpaddr:
    WriteGameRegistryValue("TCPADDR", GetGadgetText(gadget, "ADDRESS", 0), 0x80);
}

// Opens the TCP settings dialog (TCP.GUI) with HandleTcpDialogClick as its handler.
// The "ADDRESS" setting gets the direct-connect address typed on the command
// line (g_cmdlineTcpJoinAddress) when there is one, otherwise the "TCPADDR" value; the
// ADDRESS gadget is then selected in the dialog.
// FUNCTION: 0x4421f0
void OpenTcpDialog()
{
    Layer* dialog = LoadGuiLayer(&g_game->gui, "TCP.GUI", 0x800);
    dialog->handler = HandleTcpDialogClick;
    dialog->owner = g_game;
    dialog->cb1c = 0;
    LoadPictureCached(0, 0, 0, 0);
    HAPINET_initlobbiedconnection(&g_game->net);
    FindGadgetIndex(dialog->entries, "ADDRESS", 3);
    char* address = GetGadgetText(&g_game->gui, "ADDRESS", 0);
    int direct = g_cmdlineTcpJoinAddress[0];
    unsigned int len = 0x80;
    if (direct) {
        address[0] = 0;
        strncat(address, g_cmdlineTcpJoinAddress, len - 1);
    } else {
        if (!ReadGameRegistryValue("TCPADDR", address, &len)) {
            address[0] = 0;
        }
    }
    SetTranslatedTextByName(&g_game->gui, "ADDRESS", address, 0);
    if (direct) {
        HandleTcpDialogClick(&g_game->gui);
        g_game->dplayAddressDialogFlags = g_game->dplayAddressDialogFlags ^ ((g_cmdlineHostMode != 0) ^ g_game->dplayAddressDialogFlags) & 1;
    } else {
        SelectGadgetByIndex(&g_game->gui, FindGadgetIndex(dialog->entries, "ADDRESS", 3));
        TrySetFocus(&g_game->gui, FindGadgetIndex(dialog->entries, "ADDRESS", 3));
        SetKeyboardInput(&g_game->gui, 1);
        RenderLayer(&g_game->gui, 0x40);
    }
}

// The original calls these two out of line from OpenSerialDialog; in this file
// /Ob2 would inline their bodies there.
#pragma auto_inline(off)
// FUNCTION: 0x442380
void __stdcall SetSerialBaudFromGadget(Gui* menu, Gadget* entry)
{
    int value = entry->index;
    if (value >= 0) {
        g_serialBaudRate = g_serialBaudRateTable[value];
    }
}

// FUNCTION: 0x4423a0
void __stdcall SetSerialPortFromGadget(Gui* menu, Gadget* entry)
{
    int val = entry->index;
    if (val >= 0) {
        g_serialPortNumber.unknown_0 = val + 1;
    }
}
#pragma auto_inline(on)

// Click handler of the serial-link dialog (SERIAL.GUI, opened by OpenSerialDialog).
// HOST and JOIN set the multiplayer connection-mode bits of g_game (+0x2aaf)
// and save the address returned by BuildCompoundAddress; PREV goes back; anything else
// resets the gadget. The chosen baud rate and COM port are then written back to
// the registry under SERBAUD and SERPORT.
// FUNCTION: 0x4423c0
void __stdcall HandleSerialDialogClick(Gui* gadget)
{
    Gadget* entries = gadget->layer->entries;
    int a;
    int r;
    if (gadget->hotGadgetIndex == -1)
        return;
    if (FindGadgetIndex(entries, "HOST", 0xe) == gadget->hotGadgetIndex) {
        g_game->bit0 = 1;
        g_game->bit1 = 1;
        a = 0;
        int b = 0;
        r = BuildCompoundAddress(&a, &b);
        if (r >= 0) {
            g_game->info.conn.data = (void*)a;
            g_game->info.conn.size = b;
        }
        PlaySoundByName("SMLBUTTON", 0);
        BlankScreen();
    } else if (FindGadgetIndex(entries, "JOIN", 0xe) == gadget->hotGadgetIndex) {
        g_game->bit1 = 1;
        g_game->bit0 = 0;
        int a = 0, b = 0;
        r = BuildCompoundAddress(&a, &b);
        if (r >= 0) {
            g_game->info.conn.data = (void*)a;
            g_game->info.conn.size = b;
        }
        PlaySoundByName("SMLBUTTON", 0);
    } else if (IsCurrentGadgetNamed(gadget, "PREV")) {
        SetFrontendState(0xf, 0x395, "c:\\cavedog\\wargame\\multi.cpp");
        PlaySoundByName("Previous", 0);
        return;
    } else {
        ClearSelectedGadget(gadget);
        return;
    }
    a = FindGadgetChecked(entries, "SPEEDS")->index;
    WriteGameRegistryValue("SERBAUD", &a, 4);
    a = FindGadgetChecked(entries, "PORTS")->index;
    WriteGameRegistryValue("SERPORT", &a, 4);
}

// Opens the serial link dialog (SERIAL.GUI), gives the "PORTS" and "SPEEDS"
// menus a single entry each, and restores the saved values from the registry.
// FUNCTION: 0x442560
void OpenSerialDialog()
{
    Layer* gadget = LoadGuiLayer(&g_game->gui, "SERIAL.GUI", 0x800);
    gadget->handler = HandleSerialDialogClick;
    gadget->owner = g_game;
    gadget->cb1c = 0;
    LoadPictureCached(0, 0, 0, 0);
    HAPINET_initlobbiedconnection((char*)g_game + 0x14);
    ConfigureListBoxByName(&g_game->gui, "PORTS", "COM1\0COM2\0COM3\0COM4", 4, 0);
    ConfigureListBoxByName(&g_game->gui, "SPEEDS", "115200\0" "57600\0" "38400\0" "19200\0" "14400\0" "9600", 6, 0);

    int value;
    unsigned int size = 4;

    if (ReadGameRegistryValue("SERBAUD", &value, &size)) {
        SetListBoxScrollByName(&g_game->gui, "SPEEDS", value);
    }
    if (ReadGameRegistryValue("SERPORT", &value, &size)) {
        SetListBoxScrollByName(&g_game->gui, "PORTS", value);
    }
    Gadget* entry = FindGadgetChecked(gadget->entries, "PORTS");
    entry->handler = SetSerialPortFromGadget;
    SetSerialPortFromGadget(&g_game->gui, entry);
    Gadget* speeds = FindGadgetChecked(gadget->entries, "SPEEDS");
    speeds->handler = SetSerialBaudFromGadget;
    SetSerialBaudFromGadget(&g_game->gui, speeds);
    SetKeyboardInput(&g_game->gui, 1);
    RenderLayer(&g_game->gui, 0x40);
}

// Fills the ACCOUNTS menu entry with the 20 account names and restores the
// selected one.
// FUNCTION: 0x4426e0
void FillAccountList(void)
{
    char* buffer = g_modemAccountNames;
    *buffer = 0;
    for (int i = 0; i < 20; i++) {
        strcpy(buffer, g_modemAccounts[i].name);
        buffer += strlen(g_modemAccounts[i].name) + 1;
    }
    Gadget* entry = FindGadgetChecked(g_game->gui.layer->entries, "ACCOUNTS");
    int player = entry->index;
    ConfigureListBoxByName((char*)g_game + 0x519, "ACCOUNTS", g_modemAccountNames, 20, 0);
    SetListBoxScrollByName((char*)g_game + 0x519, "ACCOUNTS", player);
}

// Copies the selected account's name and number into the NAME and NUMBER
// entries and refreshes the ACCOUNTS list.
// FUNCTION: 0x4427a0
void RefreshAccountList(void)
{
    Gadget* entry = FindGadgetChecked(g_game->gui.layer->entries, "ACCOUNTS");
    if (entry != 0 && g_modemAccounts != 0) {
        GetGadgetText((char*)g_game + 0x519, "NAME",
                     g_modemAccounts[entry->index].name);
        GetGadgetText((char*)g_game + 0x519, "NUMBER",
                     g_modemAccounts[entry->index].number);
        char* buffer = g_modemAccountNames;
        *buffer = 0;
        for (int i = 0; i < 20; i++) {
            strcpy(buffer, g_modemAccounts[i].name);
            buffer += strlen(g_modemAccounts[i].name) + 1;
        }
        int player = FindGadgetChecked(g_game->gui.layer->entries, "ACCOUNTS")->index;
        ConfigureListBoxByName((char*)g_game + 0x519, "ACCOUNTS", g_modemAccountNames, 20, 0);
        SetListBoxScrollByName((char*)g_game + 0x519, "ACCOUNTS", player);
    }
}

// The original calls this out of line from 0x443100; in this file /Ob2 would
// inline its body there.
#pragma auto_inline(off)
// Shows the account the player entry selected in the NAME and NUMBER entries.
// FUNCTION: 0x4428f0
void __stdcall ShowSelectedAccount(Gui* menu, Gadget* player)
{
    int entry = player->index;
    if (entry >= 0) {
        SetTranslatedTextByName(menu, "NAME", g_modemAccounts[entry].name, 0);
        SetTranslatedTextByName(menu, "NUMBER", g_modemAccounts[entry].number, 0);
        int index = FindGadgetIndex(menu->layer->entries, "NAME", 3);
        SelectGadgetByIndex(menu, index);
        TrySetFocus(menu, index);
        MarkChanged(menu);
    }
}
#pragma auto_inline(on)

// The last used entry is moved to the front of the modem number list, which is
// then written to the registry under "MODEMNUMBERS".
// FUNCTION: 0x442970
void SaveModemNumbers(void)
{
    if (g_modemAccounts != 0) {
        short count = FindGadgetChecked(g_game->gui.layer->entries, "ACCOUNTS")->index;
        if (count > 0) {
            Entry_004426e0 temp;
            memcpy(&temp, &g_modemAccounts[count], 0x102);
            for (int i = count; i > 0; i--)
                memcpy(&g_modemAccounts[i], &g_modemAccounts[i - 1], 0x102);
            memcpy(&g_modemAccounts[0], &temp, 0x102);
        }
        WriteGameRegistryValue("MODEMNUMBERS", g_modemAccounts, 0x1428);
    }
}

// Click handler of the modem/phone dialog.
// NAME/NUMBER select an account; HOST and JOIN move the selected account to the
// front of the modem number list, save it under MODEMNUMBERS and connect; PREV
// goes back; anything else resets the gadget.
// Needed: it flips the shift loop address to lea edi, [ecx+eax].
// The selected account is copied into the NAME and NUMBER gadgets and the
// account list is refreshed.
static inline void LoadAccount_00442a30()
{
    Gadget* entry = FindGadgetChecked(g_game->gui.layer->entries, "ACCOUNTS");
    if (entry != 0 && g_modemAccounts != 0) {
        GetGadgetText((char*)g_game + 0x519, "NAME",
                     g_modemAccounts[entry->index].name);
        GetGadgetText((char*)g_game + 0x519, "NUMBER",
                     g_modemAccounts[entry->index].number);
        FillAccountList();
    }
}

// The last used entry moves to the front of the modem number list, which is
// then written to the registry under MODEMNUMBERS.
static inline void SaveModemNumbers_00442a30()
{
    if (g_modemAccounts != 0) {
        short count = FindGadgetChecked(g_game->gui.layer->entries, "ACCOUNTS")->index;
        if (count > 0) {
            Entry_004426e0 temp;
            memcpy(&temp, &g_modemAccounts[count], 0x102);
            for (int i = count; i > 0; i--)
                memcpy(&g_modemAccounts[i], &g_modemAccounts[i - 1], 0x102);
            memcpy(&g_modemAccounts[0], &temp, 0x102);
        }
        WriteGameRegistryValue("MODEMNUMBERS", g_modemAccounts, 0x1428);
    }
}

static inline int TryConnect_00442a30()
{
    int a = 0;
    int b = 0;
    int result = BuildCompoundAddress(&a, &b);
    if (result >= 0) {
        g_game->info.conn.data = (void*)a;
        g_game->info.conn.size = b;
    }
    return result;
}

// FUNCTION: 0x442a30
void __stdcall HandleModemDialogClick(Gui* gadget)
{
    Gadget* entries = gadget->layer->entries;
    if (gadget->hotGadgetIndex == -1) {
        if (g_modemInfo != 0) {
            GameFreeThunk(g_modemInfo);
            g_modemInfo = 0;
        }
        if (g_modemAccountNames != 0) {
            GameFreeThunk(g_modemAccountNames);
            g_modemAccountNames = 0;
        }
        if (g_modemAccounts != 0) {
            GameFreeThunk(g_modemAccounts);
            g_modemAccounts = 0;
        }
        return;
    }
    if (IsCurrentGadgetNamed(gadget, "NAME")) {
        LoadAccount_00442a30();
        TrySetFocus(gadget, FindGadgetIndex(entries, "NUMBER", 3));
        MarkChanged(gadget);
        ClearSelectedGadget(gadget);
        return;
    }
    if (IsCurrentGadgetNamed(gadget, "NUMBER")) {
        LoadAccount_00442a30();
        SelectGadgetByIndex(gadget, FindGadgetIndex(entries, "JOIN", 1));
        TrySetFocus(gadget, FindGadgetIndex(entries, "JOIN", 1));
        strcpy((char*)entries + 0xcc, "JOIN");
        MarkChanged(gadget);
        ClearSelectedGadget(gadget);
        return;
    }
    if (FindGadgetIndex(entries, "HOST", 0xe) == gadget->hotGadgetIndex) {
        LoadAccount_00442a30();
        SaveModemNumbers_00442a30();
        g_game->bit0 = 1;
        g_game->bit1 = 1;
        TryConnect_00442a30();
        PlaySoundByName("SMLBUTTON", 0);
        BlankScreen();
        return;
    }
    if (FindGadgetIndex(entries, "JOIN", 0xe) != gadget->hotGadgetIndex) {
        if (IsCurrentGadgetNamed(gadget, "ACCOUNTS") == 0) {
            if (IsCurrentGadgetNamed(gadget, "PREV")) {
                SetFrontendState(0xf, 0x47f, "c:\\cavedog\\wargame\\multi.cpp");
                PlaySoundByName("Previous", 0);
                return;
            }
            ClearSelectedGadget(gadget);
            return;
        }
    }
    LoadAccount_00442a30();
    SaveModemNumbers_00442a30();
    g_game->bit1 = 1;
    g_game->bit0 = 0;
    TryConnect_00442a30();
    PlaySoundByName("SMLBUTTON", 0);
    OpenMessageBox(gadget, Translate("Connecting... press ESC to abort"),
                 0xfa, 1, 1);
}

// An IDirectPlayLobby::EnumAddress callback (LPDPENUMADDRESSCALLBACK). When
// the chunk is DPAID_Modem (DPAID_Modem, {f6dcc200-a2fe-11d0-9c4f-00a0c905425e}),
// the data is a double-null-terminated list of modem names; each one is
// copied into the buffer at g_modemInfo and counted in g_modemCount.
// Where the next name goes: after the first string if the buffer is not empty.
static inline char* NameSlot(char* buffer)
{
    if (strlen(buffer) != 0)
        return buffer + strlen(buffer) + 1;
    return buffer;
}

// FUNCTION: 0x443070
BOOL __stdcall EnumModemAddressCallback(REFGUID guidDataType, DWORD dataSize, LPCVOID data, LPVOID context)
{
    char* name = (char*)data;
    if (IsEqualGUID(guidDataType, DPAID_Modem)) {
        while (lstrlenA(name) != 0) {
            strcpy(NameSlot(g_modemInfo), name);
            g_modemCount++;
            name += lstrlenA(name) + 1;
        }
    }
    return TRUE;
}

// DirectX 5's DPERR_BUFFERTOOSMALL, MAKE_DPHRESULT(30).
#define DPERR_BUFFERTOOSMALL_00443100 0x8877001e

struct Len { unsigned int v; };

// Opens the modem/phone dialog (MODEM.GUI), enumerates the machine's modems,
// loads or initialises the 20 account records and shows the dialog.
// FUNCTION: 0x443100
void __stdcall OpenModemDialog()
{
    // __stdcall keeps the gadget reload after the push. size is initialised, after addr.
    char* addr = 0;
    unsigned long size = 0;
    Len len;
    // Each HRESULT goes through r before it is compared.
    int r;
    Layer* gadget;
    // 8-byte local, not the large Mission type.
    struct { void* dp; void* dp3; } net;
    GUID iid = g_dpspGuidModem;

    gadget = LoadGuiLayer(&g_game->gui, "MODEM.GUI", 0x800);
    gadget->handler = HandleModemDialogClick;
    gadget->owner = g_game;
    LoadPictureCached(0, 0, 0, 0);
    HAPINET_initlobbiedconnection(&g_game->net);
    r = HAPINET_createdplayinterface(&iid, (Net_00443100*)&net);
    if (r >= 0) {
        r = HAPINET_getplayeraddress((Net_00443100*)&net, 0, 0, &size);
        if (r == DPERR_BUFFERTOOSMALL_00443100) {
            addr = (char*)GameAllocIgnoreTag("MODEMADDR", size);
            if (addr != 0) {
                r = HAPINET_getplayeraddress((Net_00443100*)&net, 0, addr, &size);
                if (r >= 0) {
                    g_modemInfo = (char*)GameAllocIgnoreTag("MODEMINFO", 0xc8);
                    memset(g_modemInfo, 0, 0xc8);
                    g_modemCount = 0;
                    r = HAPINET_enumaddress(&g_game->net, (void*)EnumModemAddressCallback, addr, size, 0);
                    if (g_modemCount == 0) {
                        CloseTopScreen(&g_game->gui);
                        SetFrontendErrorText("Unable to find any modems");
                        SetFrontendState(0xf, 0x4e4, "c:\\cavedog\\wargame\\multi.cpp");
                        SetFrontendSubState(0, 0x4e5, "c:\\cavedog\\wargame\\multi.cpp");
                        GameFreeThunk(g_modemInfo);
                        GameFreeThunk(addr);
                        HAPINET_releasedplayinterface((Net_00443100*)&net);
                        return;
                    }
                    if (r >= 0) {
                        int i;
                        ConfigureListBoxByName(&g_game->gui, "MODEMS", g_modemInfo, g_modemCount, 0);
                        g_modemAccounts = (Entry_004426e0*)GameAllocIgnoreTag("MODEMACCOUNTS", 0x1428);
                        len.v = 0x1428;
                        r = ReadGameRegistryValue("MODEMNUMBERS", g_modemAccounts, &len.v);
                        if (r == 0) {
                            for (i = 0; i < 20; i++) {
                                strcpy(g_modemAccounts[i].name, "UNUSED");
                                g_modemAccounts[i].number[0] = 0;
                            }
                        }
                        char* buffer = (char*)GameAllocIgnoreTag("ACCOUNTNAMES", 0xa00);
                        g_modemAccountNames = buffer;
                        *buffer = 0;
                        // Separate p runs the copy loop: keeps eax free until loop entry.
                        char* p = buffer;
                        for (i = 0; i < 20; i++) {
                            strcpy(p, g_modemAccounts[i].name);
                            p += strlen(g_modemAccounts[i].name) + 1;
                        }
                        Gadget* entry = FindGadgetChecked(g_game->gui.layer->entries, "ACCOUNTS");
                        int player = entry->index;
                        ConfigureListBoxByName(&g_game->gui, "ACCOUNTS", g_modemAccountNames, 20, 0);
                        SetListBoxScrollByName(&g_game->gui, "ACCOUNTS", player);
                        entry = FindGadgetChecked(gadget->entries, "ACCOUNTS");
                        entry->handler = ShowSelectedAccount;
                        ShowSelectedAccount(&g_game->gui, entry);
                    }
                }
            }
        }
    }
    SetKeyboardInput(&g_game->gui, 1);
    RenderLayer(&g_game->gui, 0x40);
    HAPINET_releasedplayinterface((Net_00443100*)&net);
    GameFreeThunk(addr);
}

// FUNCTION: 0x443480
void __stdcall HandleReportClick(Gui* obj)
{
    int i;
    Gadget* entries;
    int value;
    char buf[16];
    int handle;
    int acc;

    entries = obj->layer->entries;
    if (obj->hotGadgetIndex == -1)
        return;
    if (IsCurrentGadgetNamed(obj, "OK")) {
        acc = 0;
        for (i = 0; i < 16u; i++) {
            wsprintfA(buf, "CHK%d", i);
            handle = FindGadgetIndex(entries, buf, 1);
            if (!GetGadgetActiveByName(obj, buf))
                break;
            value = GetGadgetStatus(obj, handle);
            acc = (int)(pow(2.0, i) * value + acc);
        }

        SetCursorMode(0x14);
        EnableReporter(acc);
        if ((g_game->lobbyUiDirtyFlags & 0x10) || g_game->frontendSubstate == 0x14) {
            g_game->frontendSubstateRequest = 0x15;
        } else {
            g_game->frontendSubstateRequest = 0x11;
        }
    } else {
        ClearSelectedGadget(obj);
    }
}

// FUNCTION: 0x443590
void ReportDialogFrame(void)
{
    SendNetHeartbeat();
}

// Opens the REPORT.GUI dialog, fills in the "CHK%d" / "SERVICE%d" menu
// entries from the score report tables, and shows it.
// FUNCTION: 0x4435a0
void __stdcall OpenReportDialog(unsigned int* count, char** names)
{
    char name[16];
    Layer* gadget = LoadGuiLayer(&g_game->gui, "REPORT.GUI", 0x800);
    gadget->handler = HandleReportClick;
    gadget->owner = g_game;
    gadget->cb1c = ReportDialogFrame;
    LoadPictureCached("scorebg", 0, 1, 0);
    for (unsigned int i = 0; i < *count; i++) {
        wsprintfA(name, "CHK%d", i);
        SetGadgetActiveByName(&g_game->gui, name, 1);
        wsprintfA(name, "SERVICE%d", i);
        SetGadgetActiveByName(&g_game->gui, name, 1);
        SetTranslatedTextByName(&g_game->gui, name, names[i], 0x80);
    }
    SetKeyboardInput(&g_game->gui, 1);
    RenderLayer(&g_game->gui, 0x141);
    SetCursorMode(0x13);
    MarkChanged(&g_game->gui);
    MarkLayerChanged(&g_game->gui);
}

// FUNCTION: 0x4436e0
int InitScoreReporting(void)
{
    if (g_game->map->GetGameType() != 3)
        return 0;
    int saved = g_game->cursorMode;
    SetCursorMode(0x14);
    int r = LoadReporterDll(&g_reporterCount, &g_reporterNames);
    if (r == 0) {
        if (g_reporterCount > 0) {
            SetCursorMode(0x13);
            OpenReportDialog(&g_reporterCount, (char**)&g_reporterNames);
            SetCursorMode(saved);
            return 1;
        }
    } else if (r != 4) {
        SetOffscreenSurface(g_game->screen);
        OpenMessageBox(&g_game->gui, Translate("Unable to initialize scores reporting."), 0x190, 1, 0);
        LoadPictureCached("ReportError", 0, 1, 0);
        RunWhileScreenNamed(&g_game->gui, "MSGBOX.GUI");
    }
    SetCursorMode(saved);
    return 0;
}

// The 0xbc-byte frame is `Msg_004437c0 msg;`: the group of four dwords is
// copied out of g_game+0x2b6e at msg+0x99, so the leading pad is real and the
// trailing pad keeps the struct at 0xbc exactly.
// Handler for the SELGAME (multiplayer game list) screen. Processes the
// UPDATE / PREVMENU / WATCH / JOINGAME / STARTNEW buttons and the per-entry
// "compatible version" check. param_1 is &g_game->gui (g_game+0x519); its
// +0x18 field is the widget created by LoadGuiLayer, whose +4 is the GUI entry
// table and whose +0x60 is the id of the pressed entry.
// FUNCTION: 0x4437c0
void __stdcall HandleSelectGameClick(Gui* param_1)
{
    Gadget* entries = param_1->layer->entries;
    int i;
    int id;
    int cur;

    if (g_cmdlineTcpJoinAddress[0] != 0) {
        g_cmdlineTcpJoinAddress[0] = 0;
        if (g_cmdlineHostMode != 0)
            goto startnew;
    }

    if (param_1->hotGadgetIndex == -1) {
        for (i = 0; i < 0xf; i++) {
            GameFreeThunk(g_game->data[i]);
            g_game->data[i] = 0;
        }
        GameFreeThunk(g_game->shared);
        GameFreeThunk(g_game->desc);
        g_game->shared = 0;
        g_game->desc = 0;
        return;
    }

    if (FindGadgetIndex(entries, "UPDATE", 0xe) == param_1->hotGadgetIndex) {
        char* pass = (char*)FindGadgetChecked_B(entries, "PASSWORD");
        if (pass != 0) {
            cur = g_game->localPlayer;
            strcpy(g_game->players[cur].info->password, pass + 0xb6);
        }
        PlaySoundByName("Multi", 0);
        ConnectToGame(param_1->layer);
        MarkChanged(param_1);
        ClearSelectedGadget(param_1);
        return;
    }

    if (FindGadgetIndex(entries, "PREVMENU", 0xe) == param_1->hotGadgetIndex) {
        g_game->frontendSubstateRequest = 3;
        PlaySoundByName("Previous", 0);
        return;
    }

    if (FindGadgetIndex(entries, "WATCH", 0xe) == param_1->hotGadgetIndex ||
        FindGadgetIndex(entries, "JOINGAME", 0xe) == param_1->hotGadgetIndex ||
        entries[param_1->hotGadgetIndex].type == 2) {
        Gadget* e = FindGadgetChecked(entries, "GAMENAME");
        Msg_004437c0 msg;
        unsigned int flags;
        // int, not unsigned: keeps the version test a signed compare (jg/jl).
        int ver;
        char* pass;
        unsigned int b;

        *(Desc_004437c0*)g_game->game_info = g_game->desc[e->index];
        msg.group = *(Group_00441220*)((char*)g_game + 0x2b6e);
        flags = *(unsigned int*)((char*)&msg.group + 2);
        ver = *(unsigned int*)((char*)&msg.group + 0xe) & 0xff;

        if (ver <= (int)g_game->version && ver >= (int)g_game->version) {
            if ((flags & 0x8000) != 0 || (flags & 0x10) != 0) {
                PlaySoundByName("Previous", 0);
                ClearSelectedGadget(param_1);
                return;
            }
            GetGadgetText(param_1, "NICKNAME", g_game->nickname);
            if (strlen(g_game->nickname) == 0) {
                BeginTextEdit(param_1, FindGadgetIndex(entries, "NICKNAME", 3));
                ClearSelectedGadget(param_1);
                OpenMessageBox(param_1, Translate("You must enter your name"), 0xc8, 1, 1);
                return;
            }
            pass = (char*)FindGadgetChecked_B(entries, "PASSWORD");
            lstrcpynA(g_game->password, pass + 0xb6, 0xb);
            cur = g_game->localPlayer;
            lstrcpynA(g_game->players[cur].info->password, pass + 0xb6, 0xb);
            b = FindHostSlot();
            if (b != 0xa &&
                (g_game->players[b].info->flags & 0x10) == 0x10)
                g_game->field_2a44 |= 4;
            if (IsCurrentGadgetNamed(param_1, "WATCH")) {
                cur = g_game->localPlayer;
                g_game->players[cur].info->flags |= 0x40;
                PlaySoundByName("Multi", 0);
                g_game->frontendSubstateRequest = 0x13;
                return;
            }
            cur = g_game->localPlayer;
            g_game->players[cur].info->flags &= 0xffbf;
            PlaySoundByName("BigButton", 0);
            g_game->frontendSubstateRequest = 0x12;
            return;
        }
        SetFrontendErrorText("You do not have a compatible version for this game.");
    } else if (FindGadgetIndex(entries, "STARTNEW", 0xe) == param_1->hotGadgetIndex) {
startnew:
        cur = g_game->localPlayer;
        g_game->players[cur].info->flags &= 0xffbf;
        PlaySoundByName("BigButton", 0);
        GetGadgetText(param_1, "NICKNAME", g_game->nickname);
        BlankScreen();
        CloseTopScreen(param_1);
        OpenNewMultiDialog();
        ClearSelectedGadget(param_1);
        return;
    }
    ClearSelectedGadget(param_1);
}

// Opens the network game selection dialog (SELGAME.GUI) and sets up the
// buffers it needs: a 0x690 byte "GAME DESCRIPTIONS" block, a 0xe74 byte
// "PLAYER SHARED" block and 15 blocks of 0xa00 bytes named "DATA0" .. "DATA14",
// then a table of (size, offset) pairs (0xb9 bytes each) in the descriptions
// block pointing into the shared block. Every GUI entry from 1 up whose type
// byte is 2 gets UpdateGameSelection as its handler and the descriptions block as its
// data. ConnectToGame then connects; on failure an "Invalid TCP/IP Address"
// message box is shown, g_game->frontendSubstateRequest is set to 3 and the function
// returns. On success the game name is put on the menu, and a connection that
// came back with an error status (neither 0 nor 2) is reported and cleared.
// FUNCTION: 0x443cb0
void OpenSelectGameDialog()
{
    char name[0x14];
    int i;
    int j;

    BlankScreen();
    Layer* gadget = LoadGuiLayer(&g_game->gui, "SELGAME.GUI", 0x80);
    gadget->handler = HandleSelectGameClick;
    gadget->owner = g_game;
    LoadPictureCached("selectgame2x", 0, 0, 0);
    g_game->desc = (Desc_004437c0*)GameAllocIgnoreTag("GAME DESCRIPTIONS", 0x690);
    g_game->shared = (char*)GameAllocIgnoreTag("PLAYER SHARED", 0xe74);
    for (i = 0; i < 0xf; i++) {
        sprintf(name, "DATA%d", i);
        g_game->data[i] = GameAllocIgnoreTag(name, 0xa00);
    }
    // Offset is j * 0xb9, no separate off counter.
    for (j = 0; j < 20; j++) {
        g_game->desc[j].size = 0xb9;
        g_game->desc[j].offset = (int)(g_game->shared + j * 0xb9);
    }
    SetTranslatedTextByName(&g_game->gui, "PASSWORD", g_game->password, 10);
    SetTranslatedTextByName(&g_game->gui, "NICKNAME", g_game->nickname, 10);
    SetGrayedOutByName(&g_game->gui, "JOIN", 1);
    SetGrayedOutByName(&g_game->gui, "WATCH", 1);
    for (i = 1; i < gadget->entries->count; i++) {
        if (gadget->entries[i].type == 2) {
            // Indexed inline and bound by reference: no named entries pointer local.
            Gadget& e = gadget->entries[i];
            e.handler = UpdateGameSelection;
            e.data = (int)g_game->desc;
        }
    }
    RenderLayer(&g_game->gui, 0x40);
    if (!ConnectToGame(gadget)) {
        CloseTopScreen(&g_game->gui);
        OpenMessageBox(&g_game->gui, Translate("Invalid TCP/IP Address"), 0xc8, 1, 1);
        g_game->frontendSubstateRequest = 3;
        return;
    }
    SelectGadgetByIndex(&g_game->gui, FindGadgetIndex(gadget->entries, "GAMENAME", 2));
    OrLabelAttribs();
    SetKeyboardInput(&g_game->gui, 1);
    RenderLayer(&g_game->gui, 0x40);
    Player_441080* conn = &g_game->players[g_game->localPlayer];
    if (conn->status != 0 && conn->status != 2) {
        RenderLayer(&g_game->gui, 0x40);
        char* msg = GetRejectReasonText(conn->status);
        OpenMessageBox(&g_game->gui, Translate(msg), 0x140, 1, 1);
        MarkChanged(&g_game->gui);
        MarkLayerChanged(&g_game->gui);
        conn->status = 0;
    }
    if (g_cmdlineTcpJoinAddress[0] != 0 && g_cmdlineHostMode != 0) {
        if (strlen(g_game->nickname) != 0)
            HandleSelectGameClick(&g_game->gui);
    }
}

// Appends a copy of entry `param_2` to the table (count at +0xb6 of entry 0),
// initialises it through the menu (SetTranslatedText), then sets its name, value,
// state and flags. The body is inlined at 0x4447d4 by the caller that builds a
// name with sprintf first.
// FUNCTION: 0x4444d0
int __stdcall CloneServiceSlot(Gadget* entries, int param_2, short param_3, int param_4, char* param_5)
{
    int index = ++entries[0].count;
    Gadget* d = &entries[index];
    Gadget* s = &entries[param_2];
    *d = *s;
    SetTranslatedText((char*)g_game + 0x519, index, param_4, 0);
    strcpy(d->name, param_5);
    d->y = param_3;
    d->active = 1;
    d->colours = 0;
    return index;
}

// FUNCTION: 0x444910
void __stdcall CacheLogosGadgetIndex(Gui* param1, int param2)
{
    param1->hotGadgetIndex = FindGadgetIndex(param1->layer->entries, "LOGOS", 2);
}
// Handler for the multiplayer side-selection dialog. When the dialog closes
// (current gadget -1) it frees the layout data; when the player picks a side
// (LOGOS/SELECT) it copies that side's byte into the local player's info.

// FUNCTION: 0x444930
void __stdcall HandleLogoSelectClick(Gui* param_1)
{
    Gadget* entries = param_1->layer->entries;
    Layout_00444930* layout = (Layout_00444930*)param_1->layer->layout;

    if (param_1->hotGadgetIndex == -1) {
        GameFreeThunk(layout->seqs);
        GameFreeThunk(layout->ptrList);
        GameFreeThunk(layout);
        PlaySoundByName("Multi", 0);
        return;
    }
    if (IsCurrentGadgetNamed(param_1, "LOGOS") || IsCurrentGadgetNamed(param_1, "SELECT")) {
        Player_00444930* player = &g_game->players[g_game->localPlayer];
        Gadget* entry = FindGadgetChecked(entries, "LOGOS");
        player->info->color = ((char*)layout)[entry->field_ba_byte];
        g_game->flag0 = 1;
        RequestPlayerColor(player->info->color);
        return;
    }
    if (!IsCurrentGadgetNamed(param_1, "Cancel"))
        ClearSelectedGadget(param_1);
}

// FUNCTION: 0x444a20
void ShowSelectedMapInfo()
{
    int outX;
    int outY;
    char buffer[100];

    if (FindGadgetIndex(g_game->gui.layer->entries, "MAPNAME", 5) != -1) {
        SetTranslatedTextByName(&g_game->gui, "MAPNAME",
                     (char*)g_game->map->GetTranslatedName(), 0);
    }

    sprintf(buffer, "%s  %s: %s",
            (char*)g_game->map + 0xdc4,
            Translate("Players"),
            (char*)g_game->map + 0xe44);
    SetTranslatedTextByName(&g_game->gui, "SIZE", (char*)buffer, 0);

    Gadget* entry = FindGadgetChecked_E(g_game->gui.layer->entries, "MAPPIC");
    if (entry->hotspotFrame != 0) {
        GameFreeThunk(entry->hotspotFrame);
        entry->hotspotFrame = 0;
    }
    void* bmp = LoadRadarPic(
        (char*)g_game->map->GetNameSlot(1), &outX, &outY);
    entry->hotspotFrame = bmp;
    if (bmp != 0) {
        ResizeRadarPicture(bmp, entry->width, entry->height, outX << 4, outY << 4);
    }

    SetTranslatedTextByName(&g_game->gui, "DESCRIPTION",
                 (char*)g_game->map->GetDescription(), 0);
    MarkChanged(&g_game->gui);
}

// FUNCTION: 0x444ba0
void __stdcall HandleViewMapClick(Gui* param_1)
{
    if (param_1->hotGadgetIndex != -1) {
        if (IsCurrentGadgetNamed(param_1, g_okGadgetName)) {
            PlaySoundByName(g_multiSoundName, 0);
        } else {
            ClearSelectedGadget(param_1);
        }
    }
}

// Opens the map view dialog (VIEWMAP.GUI) with HandleViewMapClick as its handler.
// FUNCTION: 0x444be0
void OpenViewMapDialog()
{
    LoadGuiLayer(&g_game->gui, "VIEWMAP.GUI", 0x900)->handler = HandleViewMapClick;
    LoadPictureCached("DVIEWMAP", 0, 0, 0);
    ShowSelectedMapInfo();
    SetKeyboardInput(&g_game->gui, 1);
    RenderLayer(&g_game->gui, 0x40);
}

// FUNCTION: 0x444c40
void __stdcall UpdateMapSelection(Gui* menu, int unused)
{
    Gadget* g = FindGadgetChecked(menu->layer->entries, "MAPNAMES");
    if (g_game->map->LoadMissionByName(SkipTextLines(g->text_c2, g->selected)) == 0) {
        SetGadgetActiveByName(menu, "MAPPIC", 0);
    } else {
        SetGadgetActiveByName(menu, "MAPPIC", 1);
        ShowSelectedMapInfo();
    }
}

// FUNCTION: 0x444cb0
void __stdcall HandleMapSelectClick(Gui* param_1)
{
    void* entries = param_1->layer->entries;
    Layout_00444cb0* layout = (Layout_00444cb0*)param_1->layer->layout;

    if (param_1->hotGadgetIndex == -1) {
        Gadget* entry = FindGadgetChecked_E(g_game->gui.layer->entries, "MAPPIC");
        if (entry->text_c2 != 0) {
            GameFreeThunk(entry->text_c2);
            entry->text_c2 = 0;
        }
        GameFreeThunk(layout->items);
        GameFreeThunk(layout);
        GameFreeThunk(g_oldMapName);
        g_oldMapName = 0;
        return;
    }

    if (IsCurrentGadgetNamed(param_1, "MAPNAMES") || IsCurrentGadgetNamed(param_1, "LOAD")) {
        PlaySoundByName("Multi", 0);
        Gadget* g = FindGadgetChecked(entries, "MAPNAMES");
        g_game->map->LoadMissionByName(
            SkipTextLines(g->text_c2, g->selected));

        Player_00444930* player = &g_game->players[g_game->localPlayer];
        strcpy(player->data->map,
               g_game->map->GetMissionName());
        player->data->mapCrc =
            g_game->map->ComputeMapChecksum();

        BroadcastPlayerInfo();
        ReportGameEvent(5);
        UpdateNetGameInfo();

        for (int i = 0; i < 10; i++) {
            if (g_game->players[i].active == 0 ||
                (g_game->players[i].type != 1 && g_game->players[i].type != 2)) {
                g_game->players[i].data->flags &= 0xffdf;
            }
        }
        return;
    }

    if (IsCurrentGadgetNamed(param_1, "PREVMENU")) {
        PlaySoundByName("Previous", 0);
        g_game->map->LoadMissionByName(g_oldMapName);
        BroadcastPlayerInfo();
        return;
    }

    ClearSelectedGadget(param_1);
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
    g_oldMapName = (char*)GameAllocIgnoreTag("OLDMAPNAME", 0xc8);

    if (!g_game->map->HasMissionName()) {
        OpenMessageBox(&g_game->gui,
                     Translate("There are no multiplayer maps to choose from"),
                     0x140, 1, 1);
        return;
    }

    strcpy(g_oldMapName,
           g_game->map->GetMissionName());
    g_game->map->RefreshMapList(0);

    int n = LoadMapList(0, 0, 0);
    if (n == 0) {
        OpenMessageBox(&g_game->gui,
                     Translate("There are no multiplayer maps to choose from"),
                     0x140, 1, 1);
        return;
    }

    Holder_00444930* layer = LoadGuiLayer(&g_game->gui, "SELMAP.GUI", 0x980);
    layer->handler = HandleMapSelectClick;
    Data_00444ea0* data = (Data_00444ea0*)GameAllocIgnoreTag("SELECT MAP DATA", 0x20);
    layer->data = data;
    LoadPictureCached("DSELECTMAP2", 0, 0, 0);
    LoadMapList(&data->items, 0, 0);
    SortFileList(data->items, 0, 0, n);
    ConfigureListBoxByName(&g_game->gui, "MAPNAMES", data->items, n, 0);
    FindGadgetChecked(layer->entries, "MAPNAMES")->onSelect = UpdateMapSelection;

    for (int i = 0; i < n; i++) {
        if (strcmp(g_oldMapName, SkipTextLines(data->items, i)) == 0) {
            SetListBoxScrollByName(&g_game->gui, "MAPNAMES", i);
            break;
        }
    }

    Gui* menu = &g_game->gui;
    Gadget* g = FindGadgetChecked(menu->layer->entries, "MAPNAMES");
    if (g_game->map->LoadMissionByName(
            SkipTextLines(g->text_c2, g->selected)) == 0) {
        SetGadgetActiveByName(menu, "MAPPIC", 0);
    } else {
        SetGadgetActiveByName(menu, "MAPPIC", 1);
        ShowSelectedMapInfo();
    }
    SetKeyboardInput(&g_game->gui, 1);
    RenderLayer(&g_game->gui, 0x40);
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
    Holder_00444930* gui = LoadGuiLayer(&g_game->gui, "LOGOSEL.GUI", 0x800);
    gui->handler = HandleLogoSelectClick;
    Layout_00445110* layout = (Layout_00445110*)GameAllocIgnoreTag("SELECT TEAM LOGO", 0x20);
    gui->layout = layout;
    int count = g_game->logos32->count;
    layout->ptrList = (void**)GameAllocIgnoreTag("ANIMSEQ PTR LIST", count * 4);
    layout->seqs = (AnimSeq_00445110*)GameAllocIgnoreTag("ACTUAL ANIMSEQS", count * 0x30);
    void** cursor = layout->ptrList;
    int n = 0;
    for (int j = 0; j < count; j++) {
        // Array indexing (players[k]): strength-reduced to a pointer over the type byte.
        int k;
        for (k = 0; k < 10; k++) {
            if (g_game->players[k].type != 0 && g_game->players[k].type != 4
                && g_game->players[k].data->color >= j)
                break;
        }
        layout->seqs[j] = *(AnimSeq_00445110*)g_game->logos32;
        layout->seqs[j].frame = g_game->logos32->entries[j].ptr;
        if (k == 10) {
            *cursor = &layout->seqs[j];
            ((char*)layout)[n] = (char)j;
            // n before cursor: sets the eax/ecx roles in this block.
            n++;
            cursor++;
        }
    }
    Gadget* logo = FindGadgetChecked(gui->entries, "LOGOS");
    if (logo != 0) {
        logo->callback = CacheLogosGadgetIndex;
    }
    int index = FindGadgetIndex(gui->entries, "LOGOS", 2);
    if (index != -1) {
        ((Gadget*)((char*)gui->entries + index * 0x15b))->attribs |= 0x40;
    }
    SetGadgetItems(gui, "LOGOS", layout->ptrList, n);
    SetKeyboardInput(&g_game->gui, 1);
    RenderLayer(&g_game->gui, 0x40);
}

// FUNCTION: 0x445300
void __stdcall ExpandGadgetTextToType5(Gadget* param_1)
{
    if (param_1->state == 1) {
        Head_00444930 tmp = *(Head_00444930*)param_1;
        int index = FindGadgetIndex(g_game->gui.layer->entries, param_1->name, 0xe);
        TruncateGadgetText(&g_game->gui, index);
        param_1->y += 2;
        param_1->state = 5;
        strcpy(param_1->entry_text, tmp.text);
        param_1->attribs |= 0x10;
    }
}

// Swaps two player slots, clears the first one (type 0, not active), then
// stamps index of every playing slot with its own index, or 10 for the
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
            && p->index != 10) {
            p->index = i;
        } else {
            // Re-derived: with the same pointer variable in both arms MSVC
            // keeps g_game in edx instead of ecx.
            g_game->players[i].index = 10;
        }
    }
}

// Compacts the player list: finds the first slot the renumbering pass would
// call dead, moves the next live slot into it, clears the slot it left, then
// renumbers every slot's index.
// FUNCTION: 0x445450
void CompactActivePlayerSlots()
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
                    && p->index != 10)
               || p->type == 4) {
            if (p >= end)
                break;
            p++;
        }
        q = p + 1;
        // Find the next slot that is in use.
        for (; q->active == 0
               || (q->type != 1 && q->type != 2 && q->type != 3)
               || q->index == 10;
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
            unsigned char* f = &g->players[i].index;
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

static void CloneFix_004455b0(Gadget* rec)
{
    Head_00444930 tmp = *(Head_00444930*)rec;
    int index = FindGadgetIndex(g_game->gui.layer->entries, rec->name, 0xe);
    TruncateGadgetText(&g_game->gui, index);
    rec->y += 2;
    rec->state = 5;
    strcpy(rec->entry_text, tmp.text);
    rec->attribs |= 0x10;
}

// FUNCTION: 0x4455b0
void __cdecl BuildPlayerSlotGadgets(void)
{
    char* base = (char*)g_game->gui.layer->entries;
    int p = 0;
    int t;
    char** slot;

    *(short*)(base + 0xb6) = g_battleRoomBaseGadgetCount;
    do {
        for (t = 0, slot = g_battleRoomGadgetNames; *slot != 0; slot++, t++) {
            int index = FindGadgetIndex(base, *slot, 0xe);
            Gadget* rec = (Gadget*)(base + 0x15b * index);
            Gadget* dst;
            short count;

            DAT_00512760 = rec->y;
            if (t == 0)
                DAT_0051276c = rec->height;
            count = ++*(short*)(base + 0xb6);
            dst = (Gadget*)(base + 0x15b * count);
            *dst = *rec;
            dst->name[strlen(dst->name) - 1] = (char)('0' + p);
            dst->y += p * 20;
            dst->team = 0;
            dst->active = 1;
            if (dst->state != 5) {
                switch (t) {
                case 0:
                    if (p == g_game->localPlayer) {
                        if (dst->state == 1)
                            CloneFix_004455b0(dst);
                        dst->attribs = 1;
                    } else {
                        dst->attribs |= 0x8000;
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
                            dst->active = 0;
                    }
                    break;
                case 3:
                    {
                        Player_00444930* pl = &g_game->players[p];
                        int ok = pl->active != 0 && (pl->type == 1 || pl->type == 2);
                        SetGadgetGrayedOutByName(&g_game->gui, dst->name, !ok);
                    }
                    dst->active = 0;
                    break;
                case 6:
                    if (p != g_game->localPlayer && dst->state == 1)
                        CloneFix_004455b0(dst);
                    if (*(int*)&g_game->players[p] != 0 &&
                        (&g_game->players[p])->type == 2)
                        dst->active = 0;
                    break;
                case 7:
                    if (p == g_game->localPlayer)
                        dst->active = 0;
                    break;
                case 9:
                    dst->active = 0;
                    SetButtonStageByName((Class_004a1080*)&g_game->gui, dst->name, 10);
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
            Gadget* rec = (Gadget*)(base + 0x15b * index);
            if (rec->state == 1)
                CloneFix_004455b0(rec);
        }
        sprintf(name, "READY%d", g_game->localPlayer);
        index = FindGadgetIndex(base, name, 1);
        if (index != -1) {
            Gadget* rec = (Gadget*)(base + 0x15b * index);
            rec->quickKey = (unsigned char)tolower(name[0]);
            strcpy(base + 0xcc, name);
        }
    }
    g_battleRoomSlotsBuilt = 1;
}

// GUI callback (see the entry a slider widget stores at +0x144): shows the
// unit limit of the player being watched, as text, and remembers it on the
// local player. FindHostSlot picks the watched player, or 10 for "nobody",
// in which case the value comes from the widget's own slider instead.

// FUNCTION: 0x445b70
void __stdcall UpdateMaxUnitsText(Gui* gui, int index)
{
    char text[0x14];
    int count;
    Gadget* maxunits = (Gadget*)FindGadgetChecked_D(gui->layer->entries, "MAXUNITS");
    if (maxunits != 0) {
        int player = FindHostSlot();
        if (player == g_game->localPlayer || player == 10) {
            count = ReadSliderValue(maxunits) + 0x14;
        } else {
            count = g_game->players[player].data->maxUnits;
        }
        _itoa(count, text, 10);
        SetTranslatedTextByName(gui, "MAXUNITSTEXT", text, 0);
        g_game->players[g_game->localPlayer].data->maxUnits = count;
        PlayerInfo* data = g_game->players[g_game->localPlayer].data;
        unsigned char f = data->flags_97;
        data->maxUnits = count;
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
void __stdcall UpdateMetalText(Gui* sub, int unused)
{
    char text[20];
    void* value = FindGadgetChecked_D(sub->layer->entries, "METAL");

    if (value != 0) {
        int shown = ReadSliderValue(value) / 100 * 100;
        int hundreds;
        PlayerInfo* unit;

        _itoa(shown, text, 10);
        SetTranslatedTextByName(sub, "METALTEXT", text, 0);
        hundreds = shown / 100;
        g_game->players[g_game->localPlayer].unit->metal = (unsigned short)hundreds;
        // The original writes the same value to the same field a second time,
        // through a freshly looked up unit pointer, before testing its flag.
        unit = g_game->players[g_game->localPlayer].unit;
        unit->metal = (unsigned short)hundreds;
        if (unit->flags_97 & 1) {
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
void __stdcall UpdateEnergyText(Gui* sub, int unused)
{
    char text[20];
    void* value = FindGadgetChecked_D(sub->layer->entries, "ENERGY");

    if (value != 0) {
        int shown = ReadSliderValue(value) / 100 * 100;
        PlayerInfo* unit;

        _itoa(shown, text, 10);
        SetTranslatedTextByName(sub, "ENERGYTEXT", text, 0);
        unit = g_game->players[g_game->localPlayer].unit;
        unit->energy = (unsigned short)(shown / 100);
        if (unit->flags_97 & 1) {
            BroadcastPlayerInfo();
            UpdateNetGameInfo();
        }
    }
}

// Sets the value of the named gadget of a menu (see 0x445e50).

// FUNCTION: 0x445e20
void __stdcall SetNamedSliderValue(Gui* menu, char* name, int value)
{
    Gadget* gadget = FindGadgetChecked_D(menu->layer->entries, name);
    SetSliderFromValue(gadget, value);
}
// Sets up the gadget with the given name in the game's menu (if it exists),
// then hands the gadget's index to the callback.

typedef void (__stdcall* Callback_00445e50)(Gui* menu, int index);

// FUNCTION: 0x445e50
void __stdcall BindNamedSliderWithCallback(char* name, int param_2, int param_3, Callback_00445e50 callback)
{
    Gui* menu = &g_game->gui;
    void* gadgets = menu->layer->entries;
    int index = FindGadgetIndex(gadgets, name, 0xe);
    if (index != -1) {
        Gadget* gadget = FindGadgetChecked_D(gadgets, name);
        gadget->max = param_2;
        gadget->sliderCallback = callback;
        gadget->knobPos = param_3;
        SetSliderFromValue(gadget, gadget->knobPos);
        gadget->sliderUser = g_game;
    }
    callback(menu, index);
    MarkChanged(menu);
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
    PlayerInfo* info = g_game->players[i].info;

    SetButtonStageByName((Class_004a1080*)&g_game->gui, "COMMANDER", info->commander);
    SetButtonStageByName((Class_004a1080*)&g_game->gui, "MAPPING", !info->mapping);
    int los;
    if (!info->los) {
        los = 2;
    } else {
        los = !info->losType;
    }
    SetButtonStageByName((Class_004a1080*)&g_game->gui, "LOSTYPE", los);
    SetButtonStageByName((Class_004a1080*)&g_game->gui, "WATCHING", info->watching);
    SetButtonStageByName((Class_004a1080*)&g_game->gui, "CHEATING", info->cheating);
    SetButtonStageByName((Class_004a1080*)&g_game->gui, "FIXEDLOC", info->fixedloc);
    SetButtonStageByName((Class_004a1080*)&g_game->gui, "GAMEOPEN", !info->closed);
}

// Handler for a two-choice dialog gadget: "CHOICE1" acts on the local
// player, "CHOICE2" does nothing, anything else is passed on.

// FUNCTION: 0x446020
void __stdcall HandleRejectChoice(Gui* gadget)
{
    int owner = (int)gadget->layer->entries;
    if (gadget->hotGadgetIndex == -1)
        return;
    if (IsGadgetNamed(owner, gadget->hotGadgetIndex, "CHOICE1")) {
        RejectPlayer(GetSlotDpid((unsigned char)g_rejectPlayer), 1);
    } else if (!IsGadgetNamed(owner, gadget->hotGadgetIndex, "CHOICE2")) {
        ClearSelectedGadget(gadget);
    }
}

// Opens the YESORNO.GUI dialog for player g_rejectPlayer, fills its CHOICE1 /
// CHOICE2 / TITLE fields and installs HandleRejectChoice as the handler. The title
// is "Reject <player name>?".

// FUNCTION: 0x446080
void __stdcall OpenRejectDialog(int player)
{
    char buf[100];
    g_rejectPlayer = player;
    Holder_00444930* gadget = LoadGuiLayer(&g_game->gui, "YESORNO.GUI", 0x100);
    if (gadget != 0) {
        SetKeyboardInput(&g_game->gui, 1);
        void* entries = gadget->entries;
        FindGadgetIndex(entries, "CHOICE1", 1);
        FindGadgetIndex(entries, "CHOICE2", 1);
        FindGadgetIndex(entries, "TITLE", 5);
        SetTranslatedTextByName(&g_game->gui, "CHOICE1", "Yes", 0);
        SetTranslatedTextByName(&g_game->gui, "CHOICE2", "No", 0);
        sprintf(buf, "%s %s?", Translate("Reject"),
                g_game->players[g_rejectPlayer].name);
        SetTranslatedTextByName(&g_game->gui, "TITLE", buf, 0);
        gadget->handler = HandleRejectChoice;
        gadget->owner = g_game;
        SetKeyboardInput(&g_game->gui, 1);
        RenderLayer(&g_game->gui, 0x40);
    }
}

// Handler for the "MODES" (display mode) dialog. When a mode gadget is
// selected (MODES or SELECT), it copies the selected display mode into the
// game's width/height and the local player's screen size, then applies it.
// CANCEL exits the game, OK plays the button sound. With no current gadget
// (-1) it frees the display mode list.

// FUNCTION: 0x4461d0
void __stdcall HandleDisplayModesClick(Gui* gui)
{
    Holder_00444930* holder = gui->layer;
    Gadget* gadgets = holder->entries;
    ModeList* obj = (ModeList*)holder->layout;
    if (gui->hotGadgetIndex == -1) {
        GameFreeThunk(obj->available);
        GameFreeThunk(obj->modes);
        GameFreeThunk(obj);
        return;
    }
    if (IsCurrentGadgetNamed(gui, "MODES") || IsCurrentGadgetNamed(gui, "SELECT")) {
        Player_00444930* player = &g_game->players[g_game->localPlayer];
        Gadget* entry = FindGadgetChecked(gadgets, "MODES");
        Mode_00446310* mode = &obj->modes[entry->selected];
        g_game->displayWidth = mode->width;
        g_game->displayHeight = mode->height;
        player->data->width = (unsigned short)mode->width;
        player->data->height = (unsigned short)mode->height;
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
    ClearSelectedGadget(gui);
}

// Screen resolution selection: builds the display mode list, then advances the
// local player's mode to the next (or previous) entry.

// FUNCTION: 0x446310
void CyclePlayerDisplayMode(void)
{
    ModeList* obj = (ModeList*)GameAllocIgnoreTag("SELECT VIDEO MODE", 0x20);
    obj->available = 0;
    obj->modes = (Mode_00446310*)GameAllocIgnoreTag("DISPLAY MODES", 0x4b0);

    if (GetDisplayModes(obj) != 0) {
        SortDisplayModes(obj);
        obj->available = (char*)GameAllocIgnoreTag("AVAILABLE MODES", obj->count << 8);
        obj->available[0] = 0;

        Player_00444930* player = &g_game->players[g_game->localPlayer];
        int count = obj->count;
        for (int i = 0; i < count; i++) {
            if (obj->modes[i].width == player->data->width
                && obj->modes[i].height == player->data->height) {
                if (g_game->gui.layer->clickMode == 2) {
                    i--;
                    if (i < 0)
                        i = count - 1;
                } else {
                    i++;
                    if (i >= count)
                        i = 0;
                }
                Mode_00446310& mode = obj->modes[i];
                player->data->width = (unsigned short)mode.width;
                player->data->height = (unsigned short)mode.height;
                BroadcastPlayerInfo();
                g_game->displayWidth = mode.width;
                g_game->displayHeight = mode.height;
                break;
            }
        }
    }
    GameFreeThunk(obj->modes);
    GameFreeThunk(obj);
}

// Sets the GUI's "WATCHING" and "GAMEOPEN" controls from the local player's
// flags and marks the GUI for redraw. SetButtonStageByName's value is widened as an
// int here (its own file says char; the checker compares names only).

// FUNCTION: 0x446450
void UpdateWatchingGadgets()
{
    PlayerInfo* info = g_game->players[g_game->localPlayer].info;
    SetButtonStageByName((Class_004a1080*)&g_game->gui, "WATCHING", info->watching);
    SetButtonStageByName((Class_004a1080*)&g_game->gui, "GAMEOPEN", !info->closed);
    MarkChanged((Dialog*)&g_game->gui);
}

// Handler for the CONTROL.GUI dialog: choosing a "LIVEPLYR%d" entry opens the
// reject dialog for that player, WATCHING toggles the local player's watching
// flag and republishes the GUI values, OK kicks every playing player in state
// 3 without watch permission, and any other gadget clears the current one.

// FUNCTION: 0x4464d0
void __stdcall HandleControlDialogClick(Gui* gui)
{
    PlayerInfo* info = g_game->players[g_game->localPlayer].info;
    if (gui->hotGadgetIndex != -1) {
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
            SetButtonStageByName((Class_004a1080*)&g_game->gui, "WATCHING", info->watching);
            SetButtonStageByName((Class_004a1080*)&g_game->gui, "GAMEOPEN", !info->closed);
            MarkChanged((Class_004a1080*)&g_game->gui);
            BroadcastPlayerInfo();
        } else if (IsCurrentGadgetNamed(gui, "OK")) {
            UpdateNetGameInfo();
            PlaySoundByName("Options", 0);
            if (!info->watching) {
                for (int i = 0; i < 10; i++) {
                    if (g_game->players[i].active != 0) {
                        if (g_game->players[i].type == 3) {
                            if (g_game->players[i].info->bit6) {
                                RejectPlayer(g_game->players[i].id, 9);
                            }
                        }
                    }
                }
            }
            return;
        }
        // Written once after the chain, not in each arm; the OK arm returns early.
        ClearSelectedGadget(gui);
    }
}

// Opens the CONTROL.GUI dialog (with HandleControlDialogClick as its handler) when the
// local player's info does not have bit 6 set, then sets the WATCHING and
// GAMEOPEN controls from that player's flags.

// FUNCTION: 0x4466b0
void OpenControlDialog()
{
    PlayerInfo* info = g_game->players[g_game->localPlayer].info;
    if (info->bit6) {
        return;
    }
    Holder_00444930* gadget = LoadGuiLayer(&g_game->gui, "CONTROL.GUI", 0x800);
    gadget->handler = HandleControlDialogClick;
    gadget->owner = g_game;
    RefreshAlliesScreen(1);
    info = g_game->players[g_game->localPlayer].info;
    SetButtonStageByName((Class_004a1080*)&g_game->gui, "WATCHING", info->watching);
    SetButtonStageByName((Class_004a1080*)&g_game->gui, "GAMEOPEN", !info->closed);
    MarkChanged((Dialog*)&g_game->gui);
    SetKeyboardInput((Dialog*)&g_game->gui, 1);
    RenderLayer((Dialog*)&g_game->gui, 0x40);
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
        && p->index != 10;
}

static inline int IsCounted(Player_00444930* p)
{
    return (p->type == 1 || p->type == 2 || p->type == 3)
        && (p->unitCount != 0 || p->unitsCreated == 0);
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
int __stdcall CountPlayersInAlliance(int alliance)
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
char AreAllPlayersInOneAlliance()
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
            && (!(g_game->flags_2a44 & 4) || p->info->color != 0xff)) {
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
                SetButtonStageByName((Class_004a1080*)&g_game->gui, buffer, 10);
                break;
            case 1:
                SetButtonStageByName((Class_004a1080*)&g_game->gui, buffer, alliance * 2 + 1);
                break;
            default:
                SetButtonStageByName((Class_004a1080*)&g_game->gui, buffer, alliance * 2);
                break;
            }
        }
    }
    g_game->flag0 = 1;
}

// Recomputes the per-player ally marks. For every active player it walks the
// players that share its alliance colour (FindNextAlly, inlined), sets the
// corresponding bytes of allied/alliedBy and bit 1 of the player info
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

// The original calls this out of line from 0x446f50, 0x447b10 and
// 0x448c70; in this file /Ob2 would inline it.
#pragma auto_inline(off)
// FUNCTION: 0x446c70
void SyncMutualAlliances()
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
        if (p->index == 10)
            continue;
        if (type == 4)
            continue;
        j = 0;
        while ((k = FindNextAlly_00446c70(i, j)) != -1) {
            j = k + 1;
            p->alliedBy[k] = 1;
            p->allied[k] = 1;
            Player_00444930* q = &g_game->players[k];
            q->info->flags_9d_wide |= 2;
            p->info->flags_9d_wide |= 2;
        }
        if (CountAlliance(p->alliance) < 2)
            p->info->flags_9d_wide &= 0xfffd;
    }
}

#pragma auto_inline(on)

static inline int IsSelectable(Player_00444930* p)
{
    return (p->type == 1 || p->type == 2 || p->type == 3)
        && p->index != 10;
}

// For every other player on the same side, marks the relation and clears
// bit 1 in player->info->flags_9d_wide. The extra "g_game->players[i].type != 4"
// check is dead: IsSelectable already restricts type to 1, 2 or 3, so the
// condition can never be false; kept because the compiler emitted it.

// The original calls this out of line from 0x446f50 and 0x447b10;
// in this file /Ob2 would inline it.
#pragma auto_inline(off)
// FUNCTION: 0x446e90
void __stdcall ClearAlliances(Player_00444930* player)
{
    if (player->alliance != 5) {
        for (int i = 0; i < 10; i++) {
            Player_00444930* p = &g_game->players[i];
            if (g_game->players[i].active
                && IsSelectable(p)
                && g_game->players[i].type != 4
                && g_game->players[i].alliance == player->alliance
                && i != player->index) {
                SetAlliance(player->id, p->id, 0, 1);
                player->info->flags_9d_wide &= 0xfffd;
            }
        }
    }
}
#pragma auto_inline(on)
// FUNCTION: 0x446f50
void __stdcall CyclePlayerAlliance(int index)
{
    int colour = g_game->players[index].colour;
    Player_00446f50* player = &g_game->players[index];
    ClearAlliances(player);
    player->colour = (colour + 1) % 6;
    BroadcastAllyTeam(player);
    SyncMutualAlliances();
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
    if (p->index == 10)
        return 0;
    if (!IsLiveType_00446fb0(p))
        return 0;
    if (p->unitCount == 0 && p->unitsCreated != 0)
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
    if (p->index == 10)
        return 0;
    if (p->unitCount == 0 && p->unitsCreated != 0)
        return 0;
    if (p->info->color == 0xff)
        return 0;
    return 1;
}

// FUNCTION: 0x446fb0
void RebuildAllyList()
{
    // 52 bytes, not 44: sets the frame size.
    char text[52];
    // No lp local: both pointers are built from g_game->players[...] directly.
    unsigned char* a = &g_game->players[g_game->localPlayer].allied[0];
    unsigned char* b = &g_game->players[g_game->localPlayer].alliedBy[0];

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
        MarkChanged(&g_game->gui);
    }
}

// FUNCTION: 0x447150
void __stdcall HandleAlliesClick(Gui* gadget)
{
    void* entries = gadget->layer->entries;
    char buf[100];

    if (gadget->hotGadgetIndex == -1) {
        g_game->ordersPanelFlags &= 0xffdf;
        return;
    }

    int i = 0;
    Player_00446f50* local = &g_game->players[g_game->localPlayer];

    for (; i < 10; i++) {
        sprintf(buf, "LIVEALLY%d", i);
        Player_00446f50* p = &g_game->players[i];
        if (IsCurrentGadgetNamed(gadget, buf) && p->active
            && (p->type == 1 || p->type == 2 || p->type == 3)
            && p->index != 10) {
            PlaySoundByName("Options", 0);
            SetAlliance(local->id, p->id, local->allied[i] ^= 1, 0);
            char* verb = local->allied[i] ? "allied with" : "broke alliance with";
            sprintf(buf, " %s %s", Translate(verb),
                    (char*)g_game + 0x1b8e + i * 0x14b);
            SendChatMessage(local, buf, 4, 0);
            RebuildAllyList();
            DrawButton(&g_game->gui, gadget->hotGadgetIndex);
        }
    }

    if (IsCurrentGadgetNamed(gadget, "VICTORY")) {
        PlaySoundByName("Options", 0);
        ClearSelectedGadget(gadget);
        return;
    }
    if (IsCurrentGadgetNamed(gadget, "OK")) {
        PlaySoundByName("Options", 0);
        int old = (local->info->flags_9d_wide >> 1) & 1;
        int index = FindGadgetIndex(entries, "VICTORY", 1);
        unsigned int value = GetButtonStage(gadget, index);
        local->info->flags_9d_wide = (local->info->flags_9d_wide & 0xfffd) | ((value & 1) << 1);
        if (old != ((local->info->flags_9d_wide >> 1) & 1))
            BroadcastPlayerInfo();
    } else {
        ClearSelectedGadget(gadget);
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
    if (p->unitCount == 0 && p->unitsCreated != 0)
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
        && p->index != 10;
}

// FUNCTION: 0x447380
void __stdcall RefreshAlliesScreen(int param_1)
{
    Gadget* entries = g_game->gui.layer->entries;
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
        SetGadgetActiveByName((char*)g_game + 0x519, player, 0);
        SetGadgetActiveByName((char*)g_game + 0x519, logo, 0);
        SetGadgetActiveByName((char*)g_game + 0x519, ally, 0);
        SetGadgetActiveByName((char*)g_game + 0x519, teamicons, 0);

        Player_00446f50* p = &g_game->players[i];
        // Two helpers, not one: gives the register rotation of the second sprintf group.
        if (!IsWatching_00447380(p) && IsActive_00447380(p)
            && (i != g_game->localPlayer || param_1 == 0)
            && (!(g_game->flags_2a44 & 4) || IsCounted_00447380(p))
            && p->info->color != 0xff) {
            sprintf(player, "PLAYER%d", n);
            sprintf(logo, "LOGO%d", n);
            sprintf(ally, "ALLY%d", n);
            sprintf(teamicons, "TEAMICONS%d", n);
            lstrcpynA(name, p->name, 0x80);

            int idx = FindGadgetIndex(entries, player, 0xe);
            if (entries[idx].state == 1) {
                Gadget* e = FindGadgetOrNull(entries, player);
                if (e != 0 && (e->attribs & 0x4000)) {
                    strcat(name, "|");
                    strcat(name, p->name);
                }
            }

            SetTranslatedTextByName((char*)g_game + 0x519, player, name, 0x80);
            SetGadgetActiveByName((char*)g_game + 0x519, player, 1);
            sprintf(live, "LIVEPLYR%d", i);
            SetGadgetName((char*)g_game + 0x519, player, live);

            if (p->active != 0
                && IsType_00447380(p)
                && p->index != 10
                && (p->unitCount != 0 || p->unitsCreated == 0)
                && p->type != 1
                && p->type != 2
                && !(p->type == 3 && p->info->kind == 2)) {
                Player_00446f50* q = &g_game->players[g_game->localPlayer];
                if (q->active != 0
                    && IsType_00447380(q)
                    && q->index != 10
                    && (q->unitCount != 0 || q->unitsCreated == 0)) {
                    SetGadgetActiveByName((char*)g_game + 0x519, ally, 1);
                }
            }

            sprintf(live, "LIVEALLY%d", i);
            SetGadgetName((char*)g_game + 0x519, ally, live);

            if (p->colour == local->colour && p->colour != 5) {
                SetGadgetGrayedOutByName((char*)g_game + 0x519, live, 1);
            }

            SetGadgetActiveByName((char*)g_game + 0x519, teamicons, 1);

            int value;
            if (p->active != 0 && (p->type == 1 || p->type == 2)
                && !(g_game->flags_2a44 & 4)) {
                value = 0;
            } else {
                value = 1;
            }
            SetGadgetGrayedOutByName((char*)g_game + 0x519, teamicons, value);

            Gadget* e2 = FindGadgetChecked_E(entries, logo);
            if (e2 != 0) {
                e2->visible = 1;
                e2->hotspotGaf = g_game->field_148db;
                e2->frame = p->info->color;
                e2->hotspotFlags &= ~1;
            }

            n++;
        }
    }
}

static inline int IsPlaying_004478b0(Player_00446f50* p)
{
    return p->active != 0
        && (p->type == 1 || p->type == 2 || p->type == 3)
        && p->index != 10;
}

// Repeats the type test of IsPlaying (dead): must stay.
static inline int IsCounted_004478b0(Player_00446f50* p)
{
    return (p->type == 1 || p->type == 2 || p->type == 3)
        && (p->unitCount != 0 || p->unitsCreated == 0);
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
    g_game->ordersPanelFlags |= 0x20;
    char* entries = (char*)g_game->gui.layer->entries;
    int i, j;
    for (i = 0; (j = FindGadgetIndex(entries, "ALLYx", 0xe)) != -1; i++)
        sprintf(entries + j * 0x15b + 2, "ALLY%d", i);
    for (i = 0; (j = FindGadgetIndex(entries, "TEAMICONSx", 0xe)) != -1; i++)
        sprintf(entries + j * 0x15b + 2, "TEAMICONS%d", i);
    RefreshAlliesScreen(0);
    RebuildAllyList();
    RefreshTeamIcons();
    Player_00446f50* local = &g_game->players[g_game->localPlayer];
    int old = (local->info->flags_9d_wide >> 1) & 1;
    unsigned char win = (local->info->flags_9b >> 6) & 1;
    SetButtonStageByName((Class_004a1080*)&g_game->gui, "VICTORY", old);
    int alliance = local->colour;
    int count;
    if (alliance == 5)
        count = 0;
    else
        count = CountAlliance_004478b0(alliance);
    SetGadgetGrayedOutByName(&g_game->gui, "VICTORY",
                 (count > 1 || win) ? 1 : 0);
    SetKeyboardInput(&g_game->gui, 1);
    RenderLayer((Dialog*)&g_game->gui, 0x40);
}

static inline int IsPlaying_00447b10(Player_00446f50* p)
{
    return p->active != 0
        && (p->type == 1 || p->type == 2 || p->type == 3)
        && p->index != 10;
}

static inline int IsCounted_00447b10(Player_00446f50* p)
{
    return (p->type == 1 || p->type == 2 || p->type == 3)
        && (p->unitCount != 0 || p->unitsCreated == 0);
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
        if (p->active && (p->type == 1 || p->type == 2 || p->type == 3) && p->index != 10)
            used[p->info->color < 9 ? p->info->color : 9] = 1;
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
    PlayerInfo* data = 0;
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
    ClearAlliances(player);
    player->colour = (colour + 1) % 6;
    BroadcastAllyTeam(player);
    SyncMutualAlliances();
    RefreshTeamIcons();
}

// FUNCTION: 0x447b10
void __stdcall HandleBattleRoomClick(Gui* gadget)
{
    // 249 or 250 bytes: puts used[] of the inlined FindUnusedLogo above the text.
    char text[250];
    Gadget* entries = gadget->layer->entries;

    if (gadget->hotGadgetIndex == -1) {
        GameFreeThunk(g_game->chatter);
        g_game->chatter = 0;
        g_battleRoomSlotsBuilt = 0;
        SyncMutualAlliances();
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
            RequestPlayerColor(p->info->color + 1);
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
                p->id = -1;
                g_game->field_499--;
            } else if (type != 4 && type != 0) {
                if (p->active != 0 && type == 2 && GetTicks() - p->time > 30) {
                    RejectPlayer(p->id, 1);
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
                if (g_game->players[FindHostSlot()].info->closed) {
                    OpenMessageBox(&g_game->gui, Translate("Can't add another player when game is closed."), 500, 1, 1);
                    ((Player*)p)->SetType(0);
                    g_game->dirty = 1;
                    break;
                }
                if (g_game->players[FindHostSlot()].info->commander != 2 && !CountLocalComputerPlayers()) {
                    CreateLocalPlayer(i, 2);
                    p->info->color = FindUnusedLogo();
                }
            }
            g_game->dirty = 1;
            UpdateNetGameInfo();
            BroadcastPlayerInfo();
        }

        sprintf(text, "SIDE%d", i);
        if (IsCurrentGadgetNamed(gadget, text)) {
            PlaySoundByName("Multi", 0);
            if (p->active != 0 && p->info->bit6) {
                p->info->bit6 = 0;
                p->info->side = 0;
            } else {
                p->info->side++;
                if (p->info->side >= g_game->sides) {
                    p->info->side = 0;
                    if (g_game->players[FindHostSlot()].info->watching
                        && p->active != 0 && p->type == 1) {
                        p->info->bit6 = 1;
                    } else {
                        SetButtonStageByName(gadget, text, 0);
                        DrawButton(gadget, gadget->hotGadgetIndex);
                    }
                }
            }
            g_game->dirty = 1;
            ReportGameEvent(4);
            BroadcastPlayerInfo();
        }

        sprintf(text, "ALLY%d", i);
        if (IsCurrentGadgetNamed(gadget, text)) {
            me->allied[i] ^= 1;
            SetAlliance(me->id, p->id, me->allied[i], 0);
            char same;
            if (me->colour == 5)
                same = 0;
            else
                same = me->colour == p->colour;
            if (same) {
                ClearAlliances(me);
                me->colour = 5;
                BroadcastAllyTeam(me);
            }
            // Original bug (docs/bugs.md): `<<` binds tighter than `==` and
            // `==` tighter than `|`, so this is ((ally2 << 1) == 3) | ally,
            // and the left side is never true.
            if (me->alliedBy[i] << 1 == 3 | me->allied[i])
                PlaySoundByName("Ally", 0);
            else
                PlaySoundByName("Multi", 0);
            sprintf(text, " %s %s",
                    Translate(me->allied[i] ? "allied with" : "broke alliance with"),
                    g_game->players[i].name);
            SendChatMessage(me, text, 4, 0);
            g_game->dirty = 1;
            BroadcastPlayerInfo();
        }

        sprintf(text, "TEAMICONS%d", i);
        if (IsCurrentGadgetNamed(gadget, text)) {
            PlaySoundByName("Ally", 0);
            CyclePlayerAlliance_00447b10(i);
            BroadcastAllyTeam(p);
        }

        sprintf(text, "RES%d", i);
        if (IsCurrentGadgetNamed(gadget, text) && IsLocalHuman_00447b10(p)) {
            PlaySoundByName("Multi", 0);
            CyclePlayerDisplayMode();
            ClearSelectedGadget(gadget);
            g_game->dirty = 1;
            return;
        }

        sprintf(text, "READY%d", i);
        if (IsCurrentGadgetNamed(gadget, text) && IsLocalHuman_00447b10(p)) {
            PlaySoundByName("Multi", 0);
            if (CheckMapCrc_00447b10()) {
                p->info->bit5 = GetGadgetStatus(&g_game->gui, FindGadgetIndex(entries, text, 1));
                if (p->info->host) {
                    strcpy(entries->label, "START");
                    g_game->gui.layer->current = FindGadgetIndex(entries, "START", 1);
                }
                for (int j = 0; j < 10; j++) {
                    Player_00446f50* q = &g_game->players[j];
                    if (IsLocal_00447b10(q))
                        q->info->bit5 = g_game->players[g_game->localPlayer].info->bit5;
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
    if (i != g_game->numPlayers)
        g_game->dirty = 1;

    if (IsCurrentGadgetNamed(gadget, "PREVMENU")) {
        PlaySoundByName("Previous", 0);
        for (int j = 0; j < 10; j++) {
            Player_00446f50* q = &g_game->players[j];
            if (IsLocal_00447b10(q))
                RejectPlayer(q->id, 2);
        }
        g_game->state = 3;
        return;
    }
    if (IsCurrentGadgetNamed(gadget, "MESSAGE")) {
        Gadget* box = FindGadgetChecked_B(entries, "MESSAGE");
        char* msg = box->text;
        if (strlen(msg) != 0) {
            if (_strcmpi(msg, "+syncerr") == 0) {
                char* s = g_game->sync->GetSyncStatusText();
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
        BeginTextEdit(&g_game->gui, FindGadgetIndex(g_game->gui.layer->entries, "MESSAGE", 3));
    } else if (IsCurrentGadgetNamed(gadget, "COMMANDER")) {
        PlaySoundByName("Multi", 0);
        me->info->commander++;
        if (me->info->commander > 2)
            me->info->commander = 0;
        BroadcastPlayerInfo();
        UpdateNetGameInfo();
        g_game->dirty = 1;
    } else if (IsCurrentGadgetNamed(gadget, "LOSTYPE")) {
        PlaySoundByName("Multi", 0);
        if (!me->info->los) {
            me->info->los = 1;
            me->info->losType = 1;
        } else if (me->info->losType == 1) {
            me->info->losType = 0;
        } else {
            me->info->los = 0;
        }
        BroadcastPlayerInfo();
        UpdateNetGameInfo();
        g_game->dirty = 1;
    } else if (IsCurrentGadgetNamed(gadget, "WATCHING")) {
        PlaySoundByName("Multi", 0);
        me->info->watching = !me->info->watching;
        if (!me->info->watching && me->active != 0 && me->info->bit6)
            me->info->bit6 = 0;
        BroadcastPlayerInfo();
        UpdateNetGameInfo();
        g_game->dirty = 1;
    } else if (IsCurrentGadgetNamed(gadget, "CHEATING")) {
        PlaySoundByName("Multi", 0);
        me->info->cheating = !me->info->cheating;
        BroadcastPlayerInfo();
        g_game->dirty = 1;
    } else if (IsCurrentGadgetNamed(gadget, "FIXEDLOC")) {
        PlaySoundByName("Multi", 0);
        me->info->fixedloc = !me->info->fixedloc;
        BroadcastPlayerInfo();
        g_game->dirty = 1;
    } else if (IsCurrentGadgetNamed(gadget, "MAPPING")) {
        PlaySoundByName("Multi", 0);
        me->info->mapping = GetButtonStageByName(gadget, "MAPPING") == 0;
        BroadcastPlayerInfo();
        UpdateNetGameInfo();
        g_game->dirty = 1;
    } else if (IsCurrentGadgetNamed(gadget, "START")) {
        int count = 0;
        PlaySoundByName("BigButton", 0);
        for (int j = 0; j < 10; j++) {
            Player_00446f50* q = &g_game->players[j];
            if ((IsLocalHuman_00447b10(q) || IsRemoteHuman_00447b10(q)) && q->info->f9d_2)
                count++;
        }
        if (count < 1 || (count < 2 && CountHumanPlayers() > 3) || (count < 3 && CountHumanPlayers() > 6)) {
            ClearSelectedGadget(&g_game->gui);
            OpenMessageBox(gadget, Translate("There are not enough game CDs present to play"), 200, 1, 1);
            return;
        }
        int total = CountComputerPlayers() + CountHumanPlayers();
        for (int t = 0; t < 5; t++) {
            if (CountAlliance_00447b10(t) == total) {
                ClearSelectedGadget(&g_game->gui);
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
        if (!me->info->watching) {
            for (int j = 0; j < 10; j++) {
                Player_00446f50* q = &g_game->players[j];
                if (q->active != 0 && q->type == 3 && (q->info->flags_9b & 0x40))
                    RejectPlayer(q->id, 9);
            }
        }
        g_game->state = 0x11;
        me->info->started = 1;
        UpdateNetGameInfo();
        g_game->los = me->info->los;
        g_game->losType = me->info->losType;
        g_game->commander = me->info->commander;
        g_game->options->fixedloc = me->info->fixedloc;
        g_game->mapping = me->info->mapping;
        SaveSettings();
        g_game->difficulty = 2;
        return;
    } else if (IsCurrentGadgetNamed(gadget, "GAMEOPEN")) {
        PlaySoundByName("Multi", 0);
        me->info->closed = GetButtonStageByName(gadget, "GAMEOPEN") == 0;
        BroadcastPlayerInfo();
        UpdateNetGameInfo();
        g_game->dirty = 1;
    } else if (IsCurrentGadgetNamed(gadget, "RESTRICTIONS")) {
        PlaySoundByName("Options", 0);
        OpenUnitRestrictions();
        ClearSelectedGadget(gadget);
    } else {
        // MAP and MAPNAME through a local, not `MAP || MAPNAME` in the
        // else-if: with the `||` the MAP body joins the region where C2 keeps
        // the constant 1 in ebp, and SetKeyboardInput gets `push ebp` (99.2%).
        int hit = IsCurrentGadgetNamed(gadget, "MAP");
        if (!hit)
            hit = IsCurrentGadgetNamed(gadget, "MAPNAME");
        if (hit) {
            PlaySoundByName("Multi", 0);
            if (me->info->host) {
                OpenMultiMapSelector();
            } else {
                Layer_00446f50* view = LoadGuiLayer(&g_game->gui, "VIEWMAP.GUI", 0x900);
                view->handler = HandleViewMapClick;
                LoadPictureCached("DVIEWMAP", 0, 0, 0);
                ShowSelectedMapInfo();
                SetKeyboardInput(&g_game->gui, 1);
                RenderLayer(&g_game->gui, 0x40);
            }
        }
    }
done:
    ClearSelectedGadget(gadget);
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
        && p->index != 10;
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
    PlayerInfo* data = 0;
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
    int ready = me->info->bit5;
    Gadget* output = FindGadgetChecked(g_game->gui.layer->entries, "OUTPUT");

    int end = g_game->chatHudWriteIdx;
    int start = g_game->chatHudReadIdx;
    if (end < start)
        end += 30;
    if (end - start > output->height / (GetFontLineHeight() + 2)) {
        g_game->chatHudReadIdx++;
        if (g_game->chatHudReadIdx >= 30)
            g_game->chatHudReadIdx = 0;
    }
    if (g_game->chatHudWriteIdx != g_game->chatHudReadIdx) {
        for (int i = g_game->chatHudReadIdx; g_game->chatHudWriteIdx != i; ) {
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

    Gadget* mapname = FindGadgetChecked_C(g_game->gui.layer->entries, "MAPNAME");
    char* map = g_game->map->GetMissionName();
    if (!g_game->map->HasMissionName()) {
        mapname->colour = 0xc;
        SetTranslatedTextByName(&g_game->gui, "MAPNAME", "NOT SELECTED", 0);
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
                me->info->bit5 = 0;
                sprintf(name, "READY%d", g_game->localPlayer);
                SetGadgetStatusByName(&g_game->gui, name, 0);
                BroadcastPlayerInfo();
            }
            if (!g_game->players[g_game->localPlayer].info->host)
                SetGadgetGrayedOutByName(&g_game->gui, "MAP", 1);
        } else {
            mapname->colour = 0;
            SetGadgetGrayedOutByName(&g_game->gui, "MAP", 0);
        }
        strcpy(str, g_game->map->GetTranslatedName());
    }

    int i;
    for (i = 0; i < 10; i++) {
        Player_00446f50* p = &g_game->players[i];
        if (IsLocal_00448c70(p))
            p->info->bit5 = g_game->players[g_game->localPlayer].info->bit5;
    }
    for (i = 0; i < 10; i++) {
        Player_00446f50* p = &g_game->players[i];
        if (g_game->players[g_game->localPlayer].info->commander == 2
            && (IsLocalAI_00448c70(p) || IsRemoteAI_00448c70(p))) {
            RejectPlayer(p->id, 0xb);
            BroadcastPlayerInfo();
        }
        if (FindHostSlot() != 10
            && !g_game->players[FindHostSlot()].info->watching
            && p->active != 0) {
            unsigned short flags = p->info->flags;
            if (flags & 0x40) {
                p->info->flags = flags & ~0x40;
                p->info->side = 0;
                BroadcastPlayerInfo();
            }
        }
    }
    SyncMutualAlliances();
    RefreshTeamIcons();

    char* entries = (char*)g_game->gui.layer->entries;
    Player_00446f50* local = &g_game->players[g_game->localPlayer];
    // Plain unsigned char counter, no int copy.
    for (unsigned char n = 0; n < 10; n++) {
        char text[32];
        char res[52];
        char blocked[52];
        Player_00446f50* p = &g_game->players[n];
        Gadget* e;
        if (!IsPlaying_00448c70(p) && !IsWatching_00448c70(p)) {
            sprintf(name, "CD%d", n);
            SetGadgetActiveByName(&g_game->gui, name, 0);
            SetGadgetGrayedOutByName(&g_game->gui, name, 0);
            sprintf(name, "PLAYER%d", n);
            char* s = "UNUSED";
            if (p->type == 4) {
                sprintf(blocked, "[%s]", Translate("BLOCKED"));
                s = blocked;
            }
            strncpy(text, s, 0x1e);
            SetTranslatedTextByName(&g_game->gui, name, text, 0);
            TruncateGadgetText(&g_game->gui, FindGadgetIndex(entries, name, 0xe));
            SetGadgetGrayedOutByName(&g_game->gui, name, ready);
            sprintf(name, "LOGO%d", n);
            e = FindGadgetChecked_E(entries, name);
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
            e = FindGadgetChecked_C(entries, name);
            if (e)
                e->visible = 0;
            sprintf(name, "PING%d", n);
            e = FindGadgetChecked_C(entries, name);
            if (e)
                e->visible = 0;
            sprintf(name, "MEM%d", n);
            e = FindGadgetChecked_C(entries, name);
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
            SetGadgetActiveByName(&g_game->gui, name,
                         ((IsLocalHuman_00448c70(p) || IsRemoteHuman_00448c70(p))
                          && p->info->f9d_2) ? 1 : 0);
            SetGadgetGrayedOutByName(&g_game->gui, name, 0);
            sprintf(name, "LOGO%d", n);
            e = FindGadgetChecked_E(entries, name);
            if (e) {
                e->visible = (p->info->color == 0xff && !ready) ? 0 : 1;
                e->c8_0 = !ready;
                e->hotspotGaf = g_game->field_148db;
                e->frame = p->info->color;
            }
            sprintf(name, "PLAYER%d", n);
            strncpy(text, p->name, 0x1e);
            SetTranslatedTextByName(&g_game->gui, name, text, 0);
            TruncateGadgetText(&g_game->gui, FindGadgetIndex(entries, name, 0xe));
            SetGadgetGrayedOutByName(&g_game->gui, name, ready);
            sprintf(name, "SIDE%d", n);
            e = FindGadgetOrNull(entries, name);
            if (e) {
                UpdateSideGadget_00448c70(n);
                e->visible = 1;
                SetGadgetGrayedOutByName(&g_game->gui, name, (IsLocal_00448c70(p) && !ready) ? 0 : 1);
            }
            sprintf(name, "ALLY%d", n);
            e = FindGadgetOrNull(entries, name);
            if (e) {
                SetButtonStageByName((Class_004a1080*)&g_game->gui, name,
                             me->alliedBy[n] << 1 | me->allied[n]);
                e->visible = (IsLocalHuman_00448c70(p) || IsWatching_00448c70(p)
                              || IsLocalAI_00448c70(p) || IsRemoteAI_00448c70(p)
                              || IsWatching_00448c70(local)) ? 0 : 1;
                SetGadgetGrayedOutByName(&g_game->gui, name, ready);
            }
            sprintf(name, "TEAMICONS%d", n);
            e = FindGadgetOrNull(entries, name);
            if (e) {
                // `|| 0` emits no code but gives the 1-before-0 layout.
                e->visible = (IsWatching_00448c70(p) || 0) ? 0 : 1;  // see the top
                SetGadgetGrayedOutByName(&g_game->gui, name, (IsLocal_00448c70(p) && !ready) ? 0 : 1);
            }
            sprintf(name, "RES%d", n);
            if (!IsLocalHuman_00448c70(p) && !IsRemoteHuman_00448c70(p))
                sprintf(res, "%s", "n/a");
            else
                sprintf(res, "%dx%d", p->info->width, p->info->height);
            SetTranslatedTextByName(&g_game->gui, name, res, 0);
            if (IsLocalHuman_00448c70(p)) {
                SetGadgetGrayedOutByName(&g_game->gui, name, ready);
            } else {
                e = FindGadgetChecked_C(entries, name);
                if (e)
                    e->visible = 1;
            }
            sprintf(name, "PING%d", n);
            e = FindGadgetChecked_C(entries, name);
            if (IsRemoteHuman_00448c70(p)) {
                if (e) {
                    str = e->text;
                    _itoa(p->ping, str, 10);
                    if (IsHostLocal())
                        strcat(str, g_game->sync->IsPlayerSynced(GetSlotDpid(n)) ? ":s" : "");
                    if (minPing >= p->ping)
                        minPing = p->ping;
                    e->visible = 1;
                }
            } else {
                SetTranslatedTextByName(&g_game->gui, name, "n/a", 0);
                if (e)
                    e->visible = 1;
            }
            sprintf(name, "MEM%d", n);
            e = FindGadgetChecked_C(entries, name);
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
                e->field_138 = g_game->players[n].info->bit5;
                e->visible = 1;
                e->b13c_0 = !IsLocalHuman_00448c70(p);
            }
        }
    }
    MarkChanged(&g_game->gui);
    PlayerInfo* info = me->info;
    if (info->host && minPing < info->pingLimit) {
        info->pingLimit = minPing;
        UpdateNetGameInfo();
    }
}

// 0x449bb0 OpenBattleRoom stays in src/frontend/multi_449bb0.cpp: gathered
// here, /Ob2 inlines UpdateMetalText (0x445c70) into it through
// BindNamedSliderWithCallback, where the original calls it.

// 0x44a680 UpdateBattleRoom stays in src/frontend/multi_44a680.cpp: gathered
// here, /Ob2 inlines UpdateMaxUnitsText (0x445b70) and UpdateMetalText
// (0x445c70) into it, where the original calls them.

// FUNCTION: 0x44afb0
void __stdcall HandleEndMultiClick(Gui* obj)
{
    if (obj->hotGadgetIndex == -1) {
        if (g_game->flag4)
            LeaveNetGame();
    } else if (IsCurrentGadgetNamed(obj, "OK")) {
        PlaySoundByName("BigButton", 0);
        SetFrontendState(2, 0x1412, "c:\\cavedog\\wargame\\multi.cpp");
        SetGameMode(1);
    } else {
        ClearSelectedGadget(obj);
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
    SetOffscreenSurface(g_game->screen);
    DrawSurface(0, image, 0, 0);
    FreeSurface(image);
    FlipScreen();
    LoadGuiLayer(&g_game->gui, "ENDMULTI.GUI", 0x80)->handler = HandleEndMultiClick;
    SetTranslatedTextByName(&g_game->gui, "RESULT",
                 Translate(g_game->flag4_3923b ? "Victory" : "Failure"), 0);
    RenderLayer(&g_game->gui, 0xc0);
    ShowSoftwareCursor();
}

// FUNCTION: 0x44b100
void FreeSaveListBuffers()
{
    if (g_saveListFileNames) {
        GameFreeThunk(g_saveListFileNames);
    }
    if (g_saveListDisplayNames) {
        GameFreeThunk(g_saveListDisplayNames);
    }
    g_saveListFileNames = g_saveListDisplayNames = 0;
}

// Reads pairs of ints from a binary file given by `name`. For each pair the
// first int is matched against the field at +0x13e of the 0x249-byte entries
// at g_game+0x1439b (entries are 1-based here); the second int is then stored
// in the field at +0x5a of the 0x62-byte entry of the table at g_unitRestrictEntries
// whose +0x52 field equals the matched index.
// FUNCTION: 0x44b140
void __stdcall LoadUnitRestrictListFile(char* name)
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
            if (g_game->unitTypes[idx].fbiChecksum == a) {
                for (int j = 0; j < n; j++) {
                    if (g_unitRestrictEntries[j].unitIndex == idx) {
                        g_unitRestrictEntries[j].max = b;
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
void __stdcall SaveUnitRestrictListFile(char* filename)
{
    FILE* f = fopen(filename, "wb+");

    int count = g_game->count - 1;
    fwrite(&count, 4, 1, f);
    count++;

    for (int i = 1; i < count; i++) {
        for (int j = 0; j < g_game->count; j++) {
            if (*(int*)((char*)g_unitRestrictEntries + 0x52 + j * 0x62) == i) {
                int v = g_game->unitTypes[i].fbiChecksum;
                fwrite(&v, 4, 1, f);
                v = *(int*)((char*)g_unitRestrictEntries + 0x5a + j * 0x62);
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
    Gui* menu = &g_game->gui;
    void* gadgets = g_game->gui.layer->entries;
    Gadget* games = FindGadgetChecked(gadgets, "GAMES");
    int index = FindGadgetIndex(gadgets, "GAMENAME", 3);
    char* name;
    if (games->selected > -1 && (name = SkipTextLines(g_saveListDisplayNames, games->selected)) != 0 && strlen(name) != 0)
        SetGadgetText(menu, index, name);
    else
        SetGadgetText(menu, index, DAT_005119b8);
    MarkChanged(&g_game->gui);
}

// FUNCTION: 0x44b3c0
void __stdcall HandleLoadListClick(Gui* menu)
{
    void* gadgets = menu->layer->entries;
    if (menu->hotGadgetIndex == -1)
        return;
    if (IsCurrentGadgetNamed(menu, "CANCEL")) {
        PlaySoundByName("Previous", 0);
        return;
    }
    if (IsCurrentGadgetNamed(menu, "LOAD") || IsCurrentGadgetNamed(menu, "GAMES")) {
        PlaySoundByName("Options", 0);
        Gadget* games = FindGadgetChecked(gadgets, "GAMES");
        sprintf(g_game->save_38c6b, "%s\\%s", g_savegameDir,
                SkipTextLines(g_saveListFileNames, games->selected));
        LoadUnitRestrictListFile(g_game->save_38c6b);
        Layer_00446f50* inner = menu->layer;
        menu->layer = inner->next;
        UpdateUnitSliders(menu, 0);
        menu->layer = inner;
        if (g_saveListFileNames)
            GameFreeThunk(g_saveListFileNames);
        if (g_saveListDisplayNames)
            GameFreeThunk(g_saveListDisplayNames);
        g_saveListDisplayNames = 0;
        g_saveListFileNames = 0;
    } else if (menu->hotGadgetIndex != -1) {
        ClearSelectedGadget(menu);
    }
}

// FUNCTION: 0x44b4e0
void* __stdcall ListSaveGameFiles(int* out)
{
    char path[0x100];
    BuildDataPath(path, g_savegameDir, g_star, g_lstExtension);
    int count = CountDirectoryEntries(path, 0);
    *out = count;
    if (count == 0) {
        ConfigureListBoxByName((char*)g_game + 0x519, g_gamesGadgetName, DAT_005119b8, 0, 0);
        return 0;
    }
    g_saveListFileNames = (char*)GameAllocIgnoreTag(g_savegameNamesName, count << 8);
    g_saveListDisplayNames = (char*)GameAllocIgnoreTag(g_savegameDescsName, *out << 8);
    memset(g_saveListDisplayNames, 0, *out << 8);
    memset(g_saveListFileNames, 0, *out << 8);
    ScanDirectory(path, g_saveListFileNames, 0, 0, 0, 1);
    ConfigureListBoxByName((char*)g_game + 0x519, g_gamesGadgetName, g_saveListFileNames, *out, 0);
    return *out ? g_saveListFileNames : 0;
}

// FUNCTION: 0x44b600
void __stdcall ShowSelectedSaveGame(int unused1, int unused2)
{
    Gui* menu = &g_game->gui;
    void* gadgets = g_game->gui.layer->entries;
    Gadget* games = FindGadgetChecked(gadgets, "GAMES");
    int index = FindGadgetIndex(gadgets, "GAMENAME", 3);
    char* name;
    if (games->selected > -1 && (name = SkipTextLines(g_saveListDisplayNames, games->selected)) != 0 && strlen(name) != 0)
        SetGadgetText(menu, index, name);
    else
        SetGadgetText(menu, index, DAT_005119b8);
    MarkChanged(&g_game->gui);
}

// FUNCTION: 0x44b690
void __stdcall HandleSaveGameClick(Gui* menu)
{
    Gadget* entries = menu->layer->entries;
    if (menu->hotGadgetIndex == -1) {
        SetDescListCleanupFlag(menu, 1);
        if (g_saveListFileNames)
            GameFreeThunk(g_saveListFileNames);
        if (g_saveListDisplayNames)
            GameFreeThunk(g_saveListDisplayNames);
        g_saveListFileNames = g_saveListDisplayNames = 0;
        g_game->flag_38a51 = 0;
        return;
    }
    if (IsCurrentGadgetNamed(menu, "CANCEL")) {
        PlaySoundByName("Previous", 0);
        return;
    }
    if (IsCurrentGadgetNamed(menu, "DELETE")) {
        PlaySoundByName("SMLBUTTON", 0);
        Gadget* games = FindGadgetChecked(entries, "GAMES");
        char buf[0x100];
        sprintf(buf, "%s\\%s", g_savegameDir,
                SkipTextLines(g_saveListFileNames, games->selected));
        RemoveFile(buf);
        int count;
        ListSaveGameFiles(&count);
        char* p = g_saveListDisplayNames;
        for (int i = 0; i < count; i++) {
            strcpy(p, SkipTextLines(g_saveListFileNames, i));
            p += strlen(SkipTextLines(g_saveListFileNames, i));
            while (*p != '.')
                p--;
            *p++ = 0;
        }
        ConfigureListBoxByName(&g_game->gui, "GAMES", g_saveListDisplayNames, count, 0);
        ClearSelectedGadget(menu);
        Gui* menu2 = &g_game->gui;
        Gadget* gadgets = g_game->gui.layer->entries;
        Gadget* games2 = FindGadgetChecked(gadgets, "GAMES");
        int index = FindGadgetIndex(gadgets, "GAMENAME", 3);
        char* name;
        if (games2->selected > -1 &&
            (name = SkipTextLines(g_saveListDisplayNames, games2->selected)) != 0 &&
            strlen(name) != 0)
            SetGadgetText(menu2, index, name);
        else
            SetGadgetText(menu2, index, DAT_005119b8);
        MarkChanged(&g_game->gui);
        return;
    }
    if (IsCurrentGadgetNamed(menu, "GAMES") || IsCurrentGadgetNamed(menu, "LOAD") ||
        IsCurrentGadgetNamed(menu, "GAMENAME")) {
        PlaySoundByName("Options", 0);
        int idx = FindGadgetIndex(entries, "GAMENAME", 3);
        char* name = entries[idx].text;
        if (strlen(name) != 0) {
            BuildDataPath(g_game->save_38c6b, g_savegameDir, name, "LST");
            SaveUnitRestrictListFile(g_game->save_38c6b);
        }
    } else if (menu->hotGadgetIndex != -1) {
        ClearSelectedGadget(menu);
    }
}

static char* GetSaveDescriptions()
{
    return g_saveListDisplayNames;
}

// FUNCTION: 0x44b990
void __stdcall OpenSaveGameDialog()
{
    int count;
    Layer_00446f50* layer = LoadGuiLayer(&g_game->gui, "SAVELIST.GUI", 0x880);
    layer->handler = HandleSaveGameClick;
    layer->data = g_game;
    LoadPictureCached("DSaveList", 0, 0, 0);
    MakeDirectoryPath(g_savegameDir);
    ListSaveGameFiles(&count);
    SetTranslatedTextByName(&g_game->gui, "TITLE", "Save Game", 0);
    char* ptr = GetSaveDescriptions();
    int i = 0;
    for (; i < count; i++) {
        strcpy(ptr, SkipTextLines(g_saveListFileNames, i));
        ptr += strlen(SkipTextLines(g_saveListFileNames, i));
        while (*ptr != '.')
            ptr--;
        *ptr = 0;
        ptr++;
    }
    ConfigureListBoxByName(&g_game->gui, "GAMES", g_saveListDisplayNames, count, 0);
    if (count == 0)
        SetGadgetActiveByName(&g_game->gui, "DELETE", 0);
    Gadget* games = FindGadgetChecked(layer->entries, "GAMES");
    if (games != 0)
        games->callback = ShowSelectedSaveGame;
    int index = FindGadgetIndex(layer->entries, "GAMENAME", 3);
    layer->entries[index].attribs |= 2;

    Gui* menu = &g_game->gui;
    Gadget* entries = g_game->gui.layer->entries;
    Gadget* games2 = FindGadgetChecked(entries, "GAMES");
    int index2 = FindGadgetIndex(entries, "GAMENAME", 3);
    char* name;
    if (games2->selected > -1 && (name = SkipTextLines(g_saveListDisplayNames, games2->selected)) != 0 && strlen(name) != 0)
        SetGadgetText(menu, index2, name);
    else
        SetGadgetText(menu, index2, DAT_005119b8);
    MarkChanged(&g_game->gui);

    BeginTextEdit(&g_game->gui, index);
    SetKeyboardInput(&g_game->gui, 1);
    OrLabelAttribs();
    SetGadgetActiveByName(&g_game->gui, "LoadGame", 0);
    EnableKeyCommands(&g_game->gui);
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
    char* p = g_saveListDisplayNames;
    for (int i = 0; i < count; i++) {
        strcpy(p, SkipTextLines(g_saveListFileNames, i));
        p += strlen(SkipTextLines(g_saveListFileNames, i));
        char c = *p;
        while (c != '.') {
            c = *--p;
        }
        *p = 0;
        p++;
    }
    ConfigureListBoxByName(&g_game->gui, "GAMES", g_saveListDisplayNames, count, 0);
    SetGadgetActiveByName(&g_game->gui, "DELETE", 0);
    SetGadgetActiveByName(&g_game->gui, "GAMENAME", 0);
    Gadget* entry = FindGadgetChecked(gadget->entries, "GAMES");
    if (entry != 0) {
        entry->callback = (void*)ShowSelectedSaveGame;
    }
    Gui* menu = &g_game->gui;
    void* gadgets = g_game->gui.layer->entries;
    Gadget* games = FindGadgetChecked(gadgets, "GAMES");
    int index = FindGadgetIndex(gadgets, "GAMENAME", 3);
    char* name;
    if (games->selected > -1 && (name = SkipTextLines(g_saveListDisplayNames, games->selected)) != 0 && strlen(name) != 0)
        SetGadgetText(menu, index, name);
    else
        SetGadgetText(menu, index, DAT_005119b8);
    MarkChanged(&g_game->gui);
    SetKeyboardInput(&g_game->gui, 1);
    OrLabelAttribs();
    SetGadgetActiveByName(&g_game->gui, "SaveGame", 0);
    EnableKeyCommands(&g_game->gui);
    RenderLayer(&g_game->gui, 0x40);
    g_game->flag_38a51 |= 1;
}

// FUNCTION: 0x44be70
void __stdcall HandleUnitCountSlider(void* obj, char* gadget)
{
    int n = atoi(gadget + 8);
    Gadget* desc = FindGadgetChecked(g_game->gui.layer->entries, "DESCLIST");
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
    g_unitRestrictEntries[n + desc->firstRow].max = value;
    g_game->sync->SetUnitLimit(
        &g_game->unitTypes[g_unitRestrictEntries[n + desc->firstRow].unitIndex], value);
    desc->flags[n + desc->firstRow] = g_unitRestrictEntries[n + desc->firstRow].peerEnabled == 0;
    desc->flags[n + desc->firstRow] |= g_unitRestrictEntries[n + desc->firstRow].max == 0 ? 2 : 0;
    SetTranslatedTextByName(obj, count, (char*)buf, 0);
}

// FUNCTION: 0x44bfd0
void __stdcall UpdateUnitSliders(Gui* param_1, int unused)
{
    // C-style locals, loop counter first: sets the operand order of the flags store.
    int i;
    Gadget* desc;
    int human;
    int base;
    Gadget* slider;
    int en;
    int value;
    char name[20];

    desc = FindGadgetChecked(param_1->layer->entries, "DESCLIST");
    human = IsHostLocal();
    base = desc->firstRow;

    for (i = 0; i < 12; i++) {
        sprintf(name, "SLIDER%d", i);
        slider = FindGadgetChecked_D(param_1->layer->entries, name);
        if (slider != 0) {
            if (human == 0 || g_unitRestrictEntries[base + i].peerEnabled == 0)
                en = 1;
            else
                en = 0;
            desc->flags[base + i] = en != 0;
            value = g_unitRestrictEntries[base + i].max;
            if (value == -1)
                value = slider->max;
            SetSliderFromValue(slider, value);
            SetGadgetGrayedOutByName(param_1, name, en);
            slider->sliderCallback(param_1, (int)slider->sliderUser);
        }
    }
}

// FUNCTION: 0x44c0d0
void LoadUnitPortrait()
{
    Record_0044c0d0 rec;
    char path[256];
    Gadget* pic = FindGadgetChecked(g_game->gui.layer->entries, "PICLIST");
    if (g_unitRestrictPicLoadIndex == 0) {
        g_unitRestrictRecordCursor = (int)pic->rows;
        g_unitRestrictPicCursor = (int)g_unitRestrictPics;
    }
    int i = g_unitRestrictPicLoadIndex++;
    if (i < g_game->count) {
        int type = g_unitRestrictEntries[i].unitIndex;
        UnitType_00446f50* defs = g_game->unitTypes;
        if (defs[type].name && ((unsigned char)(defs[type].flags2.raw >> 15) & 1) == 0) {
            // Indexed by the reloaded entry field, not defs[type].name.
            BuildDataPath(path, "unitpics", defs[g_unitRestrictEntries[i].unitIndex].name, "PCX");
            void* img = LoadPcx(path, 0);
            *(void**)g_unitRestrictPicCursor = img;
            g_unitRestrictPicCursor += 4;
            if (img != 0) {
                FrameFromSurface(&rec, img);
                rec.flag8 = 9;
            } else {
                // rec.a before rec.b: width is loaded before the 0x20 store.
                rec.a = pic->width;
                rec.b = 0x20;
                rec.d = 0;
            }
            *(Record_0044c0d0*)g_unitRestrictRecordCursor = rec;
            g_unitRestrictRecordCursor += 0x18;
            MarkChanged(&g_game->gui);
        }
    }
}

// FUNCTION: 0x44c220
void UnitRestrictDialogFrame()
{
    Event_44c220 event;
    int n = 0;
    Gadget* entry = (Gadget*)FindGadgetChecked(g_game->gui.layer->entries, "PICLIST");

    if (g_unitRestrictNextPicTick < (int)GetTicks()) {
        g_unitRestrictNextPicTick = GetTicks() + 2;
        LoadUnitPortrait();
    }

    while (g_game->sync->PopChangedEntry(&event) != 0) {
        n++;
        for (int i = 0; i < entry->list.count; i++) {
            if (event.fbiChecksum == g_game->unitTypes[g_unitRestrictEntries[i].unitIndex].fbiChecksum) {
                entry->flags[i] = (event.peerEnabled == 0);
                g_unitRestrictEntries[i].peerEnabled = event.peerEnabled;
                g_unitRestrictEntries[i].max = event.max;
                entry->flags[i] |= (event.max != 0) ? 0 : 2;
            }
        }
    }

    if (n != 0) {
        UpdateUnitSliders(&g_game->gui, 0);
        MarkChanged(&g_game->gui);
    }
}

// FUNCTION: 0x44c370
void __stdcall ShowSelectedUnitCosts(void* panel, Gadget* unit)
{
    char buf[20];
    UnitType_00446f50* def = &g_game->unitTypes[unit->records[unit->field_ba].unitIndex];
    sprintf(buf, "%d", (int)def->energyCost);
    SetTranslatedTextByName(panel, "ENERGYTEXT", (char*)buf, 0);
    sprintf(buf, "%d", (int)def->metalCost);
    SetTranslatedTextByName(panel, "METALTEXT", (char*)buf, 0);
}

// 0x44c420 HandleRestrictionsClick stays in src/frontend/multi_44c420.cpp:
// gathered here, two address computations swap their operand order, which
// follows symbol ids that this file cannot give the function.

// FUNCTION: 0x44c7a0
int __cdecl CompareUnitRestrictEntries(const char* a, const char* b)
{
    return strcmp(a, b);
}

// The slider set-up at 0x445e50, which has no callers: /Ob2 inlined it.
void __stdcall BindNamedSliderWithCallback_0044c7e0(char* name, int max, int value, Callback_0044c7e0 callback)
{
    Gui* gui = &g_game->gui;
    Gadget* gadgets = gui->layer->entries;
    int index = FindGadgetIndex(gadgets, name, 0xe);
    if (index != -1) {
        Gadget* gadget = FindGadgetChecked_D(gadgets, name);
        gadget->max = max;
        gadget->sliderCallback = callback;
        gadget->knobPos = value;
        SetSliderFromValue(gadget, gadget->knobPos);
        gadget->sliderUser = g_game;
    }
    callback(gui, index);
    MarkChanged(gui);
}

// FUNCTION: 0x44c7e0
void OpenUnitRestrictions()
{
    int host = g_game->players[g_game->localPlayer].info->host & 1;
    Layer_00446f50* layer;
    Gadget* entries;
    char* flags;
    Gadget* desc;
    Gadget* pic;
    int* pics;
    char* text;
    char* dst;
    int n;
    int i;

    layer = LoadGuiLayer(&g_game->gui, "RESTRICT2.GUI", 0x880);
    layer->handler = HandleRestrictionsClick;
    layer->field_c = 0;
    layer->cb1c = UnitRestrictDialogFrame;
    g_unitRestrictPicLoadIndex = 0;
    LoadPictureCached("UnitRestrict5x", 0, 0, 0);

    entries = layer->entries;
    flags = (char*)GameAllocIgnoreTag("FLAGS", g_game->count);
    desc = FindGadgetChecked(entries, "DESCLIST");
    desc->callback = ShowSelectedUnitCosts;
    desc->flags = flags;
    desc->scroll = 0x20;
    desc->attribs |= 0x100;

    pic = FindGadgetChecked(layer->entries, "PICLIST");
    pic->flags = flags;
    pic->attribs |= 0x180;
    pic->scroll = desc->scroll;

    pics = (int*)GameAllocIgnoreTag("UNITPICARRAY", g_game->count * 0x18);
    memset(pics, 0, g_game->count * 0x18);
    text = (char*)GameAllocIgnoreTag("UNITTEXTARRAY", g_game->count << 5);
    *(int*)text = 0;

    g_unitRestrictEntries = (Record_00446f50*)GameAllocIgnoreTag("UNITSRESTRICTINFO", g_game->count * 0x62);
    desc->records = g_unitRestrictEntries;
    for (i = 0; i < g_game->count; i++)
        g_unitRestrictEntries[i].unitIndex = 0;

    g_unitRestrictPics = (int*)GameAllocIgnoreTag("UNITSPICS", g_game->count << 2);
    memset(g_unitRestrictPics, 0, g_game->count << 2);
    memset(g_unitRestrictEntries, 0, g_game->count * 0x62);
    g_unitRestrictOldCounts = (int*)GameAllocIgnoreTag("OLDCOUNTS", g_game->count << 2);

    n = 0;
    for (i = 1; i < g_game->count; i++) {
        // continue on the bit, an int bitfield tested positively.
        if (g_game->unitTypes[i].flags2.bits.flag)
            continue;
        if (!g_game->unitTypes[i].name)
            continue;
        {
            UnitType_00446f50* type = &g_game->unitTypes[i];
            Info_0044c7e0 info;
            int count;
            sprintf(g_unitRestrictEntries[n].name, "%s\r%s %dM  %dE",
                    g_game->unitTypes[i].unitName, Translate(type->description),
                    (int)type->metalCost, (int)type->energyCost);
            g_unitRestrictEntries[n].unitIndex = i;
            g_game->sync->GetUnitEntry(&g_game->unitTypes[i], &info);
            // One ternary: the if-statement form swaps the ebx/ebp registers.
            count = info.max == -1 ? 0x65 : info.max;
            g_unitRestrictEntries[n].max = count;
            g_unitRestrictOldCounts[n] = count;
            g_unitRestrictEntries[n].peerEnabled = info.peerEnabled;
            n++;
        }
    }

    qsort(g_unitRestrictEntries, n, 0x62, (int (__cdecl*)(const void*, const void*))CompareUnitRestrictEntries);

    dst = text;
    for (i = 0; i < g_game->count; i++) {
        strcpy(dst, g_unitRestrictEntries[i].name);
        dst += strlen(g_unitRestrictEntries[i].name) + 1;
    }

    for (i = 0; i < 0xc; i++) {
        char name[0x14];
        Gadget* slider;
        sprintf(name, "SLIDER%d", i);
        slider = FindGadgetChecked_D(entries, name);
        slider->sliderUser = slider;
        slider->max = 0x65;
        slider->sliderCallback = (void (__stdcall*)(Gui*, int))HandleUnitCountSlider;
    }

    BindNamedSliderWithCallback_0044c7e0("SCROLLSLIDER", 0xd2, 0, UpdateUnitSliders);

    ConfigureListBoxByName(&g_game->gui, "DESCLIST", text, n, 0);
    SetGadgetRows(g_game->gui.layer, "PICLIST", pics, n);
    UpdateUnitSliders(&g_game->gui, 0);

    {
        Gui* gui = &g_game->gui;
        UnitType_00446f50* type = &g_game->unitTypes[desc->records[desc->field_ba].unitIndex];
        char buf[0x14];
        sprintf(buf, "%d", (int)type->energyCost);
        SetTranslatedTextByName(gui, "ENERGYTEXT", buf, 0);
        sprintf(buf, "%d", (int)type->metalCost);
        SetTranslatedTextByName(gui, "METALTEXT", buf, 0);
    }

    {
        int enabled = host == 0;
        SetGrayedOutByName(&g_game->gui, "Load", enabled);
        SetGrayedOutByName(&g_game->gui, "Save", enabled);
        SetGrayedOutByName(&g_game->gui, "Reset", enabled);
    }
    RenderLayer(&g_game->gui, 0x40);
    SetKeyboardInput(&g_game->gui, 1);
    DAT_005129c0 = 0;
}

// FUNCTION: 0x44ce20
OrderFx::OrderFx(int param_1)
{
    vtable = &g_orderFxVtable;
    source = param_1;
}
