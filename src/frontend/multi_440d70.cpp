// Decompiled by Space Bunny Free, deepseek-v4.1-flash, Haiku, GPT-6, deepseek-v4.1, space-bunny-free, claude-sonnet-5-5, Opus, DeepSeek V4.1 Flash, Claude Opus 5.5 and Sonnet. Names are provisional.
//
// The multiplayer front end: the new game, TCP, serial, modem and game list
// dialogs, the connection and service provider screens and the score
// reporting setup (0x440d70 to 0x444910).
//
// 0x441220, 0x441460, 0x443ff0 and 0x444580 stay in their own files
// (multi_441220.cpp, multi_441460.cpp, multi_443ff0.cpp and
// multi_444580.cpp): in this file's symbol context each one's registers land
// differently, the same symbol-count effect as docs/c2-regalloc.md, and each
// matches only in its own file. 0x4441a0 stays in multi_4441a0.cpp: it is a
// gap region (data/functions.csv), and place.py builds a gap region from a
// file that holds only that region's functions.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <ddraw.h>
#include <math.h>

#pragma pack(push, 1)

struct Entry_00440d70;
struct Layer_00440d70;
struct Gadget_00440d70;
struct PlayerInfo_441080;
struct Player_441080;
struct Conn_443ff0;
struct Desc_004437c0;
struct Group_00441220;
struct Msg_00441220;
struct Msg_004437c0;
struct Settings_00441460;
struct Record_00441460;
struct Serial_00441c30;
struct Elem_00441c30;
struct ConnInfo_443ff0;
struct Net_00443100;
struct Entry_004426e0;
struct LinkInfo;
class Mission;

// The 0x15b-byte menu control record. Entry 0 holds the count at +0xb6; the
// other entries hold NUL terminated text there. +0xcc is entry 0's label,
// +0xce the two-argument handler the menu calls for its own entries, and
// +0xd2 the record the entry is bound to.
struct Entry_00440d70 {                // 0x15b bytes
    unsigned char type;                // +0x00
    char unknown_1;                    // +0x01
    char name[0x13];                   // +0x02
    short field_15;                    // +0x15
    char unknown_17[0x19 - 0x17];      // +0x17
    short field_19;                    // +0x19
    char unknown_1b[0x1f - 0x1b];      // +0x1b
    int field_1f;                      // +0x1f
    char unknown_23[0x29 - 0x23];      // +0x23
    char field_29;                     // +0x29
    char unknown_2a[0xb6 - 0x2a];      // +0x2a
    union {
        short count;                   // +0xb6 (entry 0 only)
        char text[2];                  // +0xb6
    };
    char unknown_b8[0xba - 0xb8];      // +0xb8
    short index;                       // +0xba
    char unknown_bc[0xc0 - 0xbc];      // +0xbc
    unsigned short field_c0;           // +0xc0
    char unknown_c2[0xce - 0xc2];      // +0xc2
    void (__stdcall* handler)(Gadget_00440d70*, Entry_00440d70*);  // +0xce
    union {
        char* records;                 // +0xd2
        int data;                      // +0xd2
    };
    char unknown_d6[0x138 - 0xd6];     // +0xd6
    unsigned short field_138;          // +0x138
    char unknown_13a[0x15b - 0x13a];   // +0x13a
};

// The gadget LoadGuiLayer returns: the entry table at +4, the click handler
// at +8 and its owner at +0xc.
struct Layer_00440d70 {
    int unknown_0;                     // +0x00
    Entry_00440d70* entries;           // +0x04
    void (__stdcall* handler)(Gadget_00440d70*);  // +0x08
    void* owner;                       // +0x0c
    char unknown_10[0x1c - 0x10];      // +0x10
    void (__stdcall* field_1c)();      // +0x1c
};

// The menu object at g_game+0x519: its +0x18 is the gadget above, its +0x60
// the id of the entry that was clicked (-1 when the menu is closing).
struct Gadget_00440d70 {
    char unknown_0[0x18];              // +0x00
    Layer_00440d70* layer;             // +0x18
    char unknown_1c[0x60 - 0x1c];      // +0x1c
    int field_60;                      // +0x60
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

// The per-player record at g_game+0x1b63 and the block its +0x27 points at.
struct PlayerInfo_441080 {
    char unknown_0[0x80];              // +0x00
    char name[0x1b];                   // +0x80
    unsigned short flags;              // +0x9b
    char unknown_9d[0x40];             // +0x9d
};

struct Player_441080 {                 // 0x14b bytes
    char unknown_0[0x22];              // +0x00
    unsigned char status;              // +0x22
    char unknown_23[0x27 - 0x23];      // +0x23
    PlayerInfo_441080* info;           // +0x27
    char unknown_2b[0x14b - 0x2b];     // +0x2b
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
    int field_4fd;                     // +0x4e9
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

// The serial address block written to DAT_00512770.
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
    unsigned short field_0;
    unsigned short players : 4;
    unsigned short playing : 1;
    unsigned short pad5 : 3;
    unsigned short black : 1;
    unsigned short nocmd : 1;
    unsigned short pad10 : 1;
    unsigned short mode : 2;
    unsigned short pad13 : 2;
    unsigned short lock : 1;
    unsigned short field_4;
    unsigned short field_6;
    unsigned short field_8;
    unsigned short field_a;
    unsigned short field_c;
    unsigned short version;
};

struct Record_00441460 {               // 0x54 bytes
    Settings_00441460 settings;        // +0x00
    int field_10;                      // +0x10
    char name[0x20];                   // +0x14
    char name2[0x20];                  // +0x34
};

// One online service: its id and its name.
struct LinkInfo {
    int id;                            // +0x00, -1: unused
    char name[32];                     // +0x04
};

// The mission object's map list (defined in the game's own files).
class Mission {
public:
    int GetGameType();
    void RefreshMapList(int param_1);
};

struct Game {
    char unknown_0;                    // +0x00
    signed char version;               // +0x01
    char unknown_2[0x10 - 2];          // +0x02
    void* field_10;                    // +0x10
    Net_00443100 net;                  // +0x14
    Gadget_00440d70 menu;              // +0x519
    char unknown_57d[0x1b63 - 0x57d];  // +0x57d
    Player_441080 players[10];         // +0x1b63
    char unknown_2851[0x2a42 - 0x2851]; // +0x2851
    unsigned char localPlayer;         // +0x2a42
    char unknown_2a43;                 // +0x2a43
    unsigned char field_2a44;          // +0x2a44
    char unknown_2a45[0x2a47 - 0x2a45]; // +0x2a45
    // The 15 blocks DATA0 to DATA14; 441460's view starts four bytes earlier.
    union {
        struct {
            char unknown_2a47[4];      // +0x2a47
            void* data[15];            // +0x2a4b
        };
        void* data16[16];              // +0x2a47
    };
    char unknown_2a87[0x2a9f - 0x2a87]; // +0x2a87
    GUID* sessions;                    // +0x2a9f
    Conn_443ff0* conns;                // +0x2aa3
    Desc_004437c0* desc;               // +0x2aa7
    char* shared;                      // +0x2aab
    union {
        unsigned short field_2aaf;     // +0x2aaf
        struct {
            unsigned short bit0 : 1;
            unsigned short bit1 : 1;
            unsigned short bits2 : 14;
        };
    };
    char buffer[0xb9];                 // +0x2ab1
    char game_info[0x54];              // +0x2b6a
    char unknown_2bbe[0x2bbf - 0x2bbe]; // +0x2bbe
    unsigned char field_2bbf;          // +0x2bbf
    unsigned char field_2bc0;          // +0x2bc0
    char gameName[0x11];               // +0x2bc1
    char nickname[0x11];               // +0x2bd2
    char password[0xb];                // +0x2be3
    unsigned char field_2bee;          // +0x2bee
    char unknown_2bef[0x2cbe - 0x2bef]; // +0x2bef
    signed char field_2cbe;            // +0x2cbe
    char unknown_2cbf[0x37e1b - 0x2cbf]; // +0x2cbf
    int field_37e1b;                   // +0x37e1b
    char unknown_37e1f[0x391e9 - 0x37e1f]; // +0x37e1f
    Mission* field_391e9;              // +0x391e9
    char unknown_391ed[0x39201 - 0x391ed]; // +0x391ed
    ConnInfo_443ff0 info;              // +0x39201
};

#pragma pack(pop)

// GLOBAL: 0x511de8
extern Game* g_game;
// GLOBAL: 0x512c84
extern int DAT_00512c84;
// GLOBAL: 0x512d90
extern char DAT_00512d90[];

extern char DAT_00512d28;
extern char DAT_00512d48;
extern int DAT_00512c80;
extern char DAT_005119b8[];
extern char* DAT_00512980;
extern int DAT_00512984;
extern Entry_004426e0* DAT_00512988;
extern char* DAT_0051298c;
extern GUID DAT_004fcdc8;
extern GUID DAT_004fcda8;
extern GUID DAT_004fcd98;
extern GUID DAT_004fcdb8;
extern GUID DAT_004fcec8;
extern GUID DAT_004fce88;
extern GUID DAT_004fcea8;
extern GUID DAT_004fcee8;
extern GUID DAT_004fcf08;
extern char DAT_004fcfb8[];
extern Serial_00441c30 DAT_00512770;
extern int DAT_00512774;
extern int DAT_00505490[];
extern unsigned int DAT_005054a8;
extern unsigned int DAT_00512788;
extern LinkInfo DAT_005127c8[];

int __stdcall FindGadgetIndex(void* entries, const char* name, int flag);
Entry_00440d70* __stdcall FindGadgetChecked(void* entries, const char* name);
int __stdcall IsCurrentGadgetNamed(void* menu, const char* name);
void __stdcall SelectGadgetByIndex(void* menu, int index);
void __stdcall FUN_004a7190(void* menu, int index);
int __stdcall FUN_0049fc50(void* menu, int index);
void __stdcall FUN_0049fa90(void* menu);
void __stdcall FUN_0049fb10(void* menu, int value);
void __stdcall FUN_0049fad0(void* menu);
void __stdcall FUN_004ab0a0(void* menu);
void __stdcall FUN_004ab170(void* menu, int a, int b);
void __stdcall FUN_004a0570(void* menu, const char* name, int value);
void __stdcall FUN_004a0bf0(void* menu, const char* name, char* text, int param_4);
void __stdcall FUN_004a1250(void* menu, const char* name, int value);
void __stdcall FUN_004a2e40(void* menu, const char* name, int index);
void __stdcall FUN_004a32a0(void* menu, const char* name, char* text, int count, int flag);
void __stdcall FUN_004a09c0(void* menu, int index, int param_3, int param_4);
Entry_00440d70* __stdcall FUN_004a0010(Entry_00440d70* entries, const char* name);
char* __stdcall GetGadgetText(void* menu, const char* key, char* out);
int __stdcall GetGadgetStatus(void* menu, int handle);
char __stdcall FUN_004a04f0(void* menu, char* name);
void __stdcall LoadPictureCached(const char* name, int a, int b, int c);
Layer_00440d70* __stdcall LoadGuiLayer(void* menu, const char* name, int flags);
void __stdcall RenderLayer(void* menu, int value);
void __stdcall CloseTopScreen(void* menu);
void __stdcall PlaySoundByName(const char* name, int flag);
void BlankScreen();
void __stdcall OpenMessageBox(void* menu, const char* text, int a, int b, int c);
char* __stdcall Translate(const char* text);
void __stdcall SetFrontendState(int state, int line, const char* file);
void __stdcall SetFrontendErrorText(char* text);
void __stdcall SetFrontendSubState(char state, int line, char* file);
int __stdcall ReadGameRegistryValue(const char* key, void* buf, unsigned int* size);
void __stdcall WriteGameRegistryValue(void* key, void* buf, int value);
void __stdcall SetCursorMode(int value);
void __stdcall EnableReporter(int value);
void __stdcall SetOffscreenSurface(int a);
void FlipScreen();
int __stdcall HAPINET_getgames(char* net, void* desc, int a);
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
void* __cdecl FUN_004d83b0(const char* name, unsigned int size);
void __cdecl FUN_004d85a0(void* p);
int IsOnlineConfigLoaded();
void FUN_00428b60();
int __stdcall LoadReporterDll(unsigned int* a, unsigned int* b);
void __stdcall RunWhileScreenNamed(void* p, char* name);
char* GetPreferredLanguage();
unsigned char FindHostSlot();
char* __stdcall GetRejectReasonText(int value);
unsigned int __stdcall OnlineGetLinkInfo(LinkInfo* links);
int __stdcall OnlineProcessButtonCommand(int button, char* message, unsigned int size);
void OnlineUnload();
void OpenOptionsPanel();
// Defined in multi_443ff0.cpp, which keeps its own view of the game.
int __stdcall SelectConnection(int index);
void __stdcall SetPaletteColors(unsigned char* palette, int first, int count);
void __stdcall FUN_004ac7d0(void* menu, void* palette, void* param_3);
void SendNetHeartbeat(void);
void __stdcall OpenSelectGameDialog();
int InitScoreReporting();
void __stdcall HandleSerialDialogClick(Gadget_00440d70* gadget);
// Defined in multi_441220.cpp, which keeps its own views of the menu and the
// entry record.
void __stdcall FUN_00441220(Gadget_00440d70* menu, Entry_00440d70* entry);
void __stdcall FUN_00442380(Gadget_00440d70* menu, Entry_00440d70* entry);
void __stdcall FUN_004423a0(Gadget_00440d70* menu, Entry_00440d70* entry);
void __stdcall HandleNewMultiClick(Gadget_00440d70* gadget);
void __stdcall HandleTcpDialogClick(Gadget_00440d70* gadget);
void __stdcall HandleModemDialogClick(Gadget_00440d70* gadget);
void __stdcall HandleSelectGameClick(Gadget_00440d70* menu);
void __stdcall HandleReportClick(Gadget_00440d70* obj);
// Defined in multi_441460.cpp, which keeps its own view of the game.
int __stdcall ConnectToGame(Layer_00440d70* gadget);
void __stdcall ShowSelectedAccount(Gadget_00440d70* menu, Entry_00440d70* entry);
void __stdcall OpenReportDialog(unsigned int* count, char** names);
void FillAccountList(void);
int __stdcall FUN_004444d0(Entry_00440d70* entries, int param_2, short param_3,
                           int param_4, char* param_5);
void FUN_00443590(void);

// Click handler of the "create new game" dialog (NEWMULTI.GUI, opened by
// OpenNewMultiDialog). Clicking a text field gives it the focus and loads its
// contents; OK copies the password (bounded to 11 characters) into g_game and
// the game and player names into two stack buffers, complains if either is
// empty, then starts the game (InitScoreReporting) and sets the pending front-end
// state at g_game+0x2bc0 to 17. CANCEL goes back to the previous dialog.
// FUNCTION: 0x440d70
void __stdcall HandleNewMultiClick(Gadget_00440d70* gadget)
{
    Entry_00440d70* entries = gadget->layer->entries;
    if (gadget->field_60 == -1)
        return;
    // The three text fields share one tail: the first two hand the next field
    // the focus, the third one the OK button.
    if (IsCurrentGadgetNamed(gadget, "GAMENAME")) {
        SelectGadgetByIndex(gadget, FindGadgetIndex(entries, "NICKNAME", 3));
        FUN_0049fc50(gadget, FindGadgetIndex(entries, "NICKNAME", 3));
        FUN_0049fa90(gadget);
        FUN_004ab0a0(gadget);
        return;
    }
    if (IsCurrentGadgetNamed(gadget, "NICKNAME")) {
        SelectGadgetByIndex(gadget, FindGadgetIndex(entries, "PASSWORD", 3));
        FUN_0049fc50(gadget, FindGadgetIndex(entries, "PASSWORD", 3));
        FUN_0049fa90(gadget);
        FUN_004ab0a0(gadget);
        return;
    }
    if (IsCurrentGadgetNamed(gadget, "PASSWORD")) {
        SelectGadgetByIndex(gadget, FindGadgetIndex(entries, "OK", 1));
        FUN_0049fa90(gadget);
        FUN_004ab0a0(gadget);
        return;
    }
    if (FindGadgetIndex(entries, "OK", 0xe) == gadget->field_60) {
        // 100 bytes each, and the pointer local in front of them, is what
        // puts them at +0x14 and +0x78 of the 0xcc-byte frame.
        char nickbuf[100];
        char namebuf[100];
        char* dst;
        int gi;                        // GAMENAME gadget index
        int ni;                        // NICKNAME gadget index
        PlaySoundByName("BigButton", 0);
        char* pw = (char*)FUN_004a0010(entries, "PASSWORD");
        lstrcpynA(g_game->password, pw + 0xb6, 0xb);
        gi = FindGadgetIndex(entries, "GAMENAME", 3);
        dst = namebuf;                 // copied through a pointer, as in the original
        strcpy(dst, entries[gi].text);
        if (strlen(namebuf) == 0) {
            FUN_004a7190(gadget, gi);
            FUN_004ab0a0(gadget);
            OpenMessageBox((char*)gadget, Translate("You must enter a game name"), 0x140, 1, 1);
            return;
        }
        ni = FindGadgetIndex(entries, "NICKNAME", 3);
        strcpy(nickbuf, entries[ni].text);
        if (strlen(nickbuf) == 0) {
            FUN_004a7190(gadget, ni);
            FUN_004ab0a0(gadget);
            OpenMessageBox((char*)gadget, Translate("You must enter your name"), 0x140, 1, 1);
            return;
        }
        strcpy(g_game->gameName, namebuf);
        strcpy(g_game->nickname, nickbuf);
        if (!InitScoreReporting()) {
            g_game->field_2bc0 = 0x11;
            return;
        }
        FUN_004ab0a0(gadget);
        return;
    }
    if (FindGadgetIndex(entries, "CANCEL", 0xe) == gadget->field_60) {
        PlaySoundByName("Previous", 0);
        CloseTopScreen(gadget);
        OpenSelectGameDialog();
        return;
    }
    FUN_004ab0a0(gadget);
}

// Opens the new multiplayer game dialog (NEWMULTI.GUI), sets its click
// handler, fills in the game, player and password fields and shows it.
// FUNCTION: 0x441080
void OpenNewMultiDialog()
{
    DWORD size;
    Layer_00440d70* layer = LoadGuiLayer(&g_game->menu, "NEWMULTI.GUI", 0x80);
    layer->handler = HandleNewMultiClick;
    layer->owner = g_game;
    LoadPictureCached("createnew", 0, 0, 0);
    Entry_00440d70* entries = layer->entries;
    if (IsOnlineConfigLoaded() && DAT_00512d48 != 0) {
        g_game->nickname[0] = 0;
        strncat(g_game->nickname, &DAT_00512d48, 0x10);
    }
    if (strlen(g_game->nickname) == 0) {
        size = 0x11;
        GetUserNameA(g_game->nickname, &size);
    }
    Entry_00440d70* gname = FUN_004a0010(entries, "GAMENAME");
    FUN_004a0bf0(&g_game->menu, "GAMENAME", g_game->gameName, 0);
    gname->field_138 = 0x10;
    Entry_00440d70* nname = FUN_004a0010(entries, "NICKNAME");
    FUN_004a0bf0(&g_game->menu, "NICKNAME", g_game->nickname, 0);
    nname->field_138 = 0x10;
    char* pw = g_game->players[g_game->localPlayer].info->name;
    if (strlen(pw) == 0)
        pw = g_game->password;
    FUN_004a0bf0(&g_game->menu, "PASSWORD", pw, 0xa);
    FUN_00428b60();
    FUN_0049fb10(&g_game->menu, 1);
    RenderLayer(&g_game->menu, 0x40);
}

// Returns the local player's name, the byte at g_game+0x2a42 selecting the
// player record and +0x1b8a (0x1b63 + 0x27) its info block; the name is at
// +0x80 of that block.
// FUNCTION: 0x441430
int __cdecl FUN_00441430()
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
// DPSPGUID_MODEM, DPSPGUID_TCPIP, DPSPGUID_IPX and DPSPGUID_SERIAL.
// FUNCTION: 0x441bc0
int GetServiceProviderIndex()
{
    GUID* guid = &g_game->info.guid;
    if (memcmp(guid, &DAT_004fcdc8, sizeof(GUID)) == 0) {
        return 0;
    }
    if (memcmp(guid, &DAT_004fcda8, sizeof(GUID)) == 0) {
        return 1;
    }
    if (memcmp(guid, &DAT_004fcd98, sizeof(GUID)) == 0) {
        return 2;
    }
    if (memcmp(guid, &DAT_004fcdb8, sizeof(GUID)) == 0) {
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

    if (memcmp(&guid, &DAT_004fcdc8, sizeof(GUID)) == 0) {
        elements[0].guid = DAT_004fce88;
        elements[0].size = 0x10;
        elements[0].data = &DAT_004fcdc8;
        // memset, not = "": plain rep stosd.
        memset(buf1, 0, sizeof(buf1));
        char* s = DAT_00512980;
        if (s == 0) {
            s = DAT_005119b8;
        }
        lstrcpyA(buf1, s);
        elements[1].guid = DAT_004fcec8;
        elements[1].size = lstrlenA(buf1) + 1;
        elements[1].data = buf1;
        lstrcpyA(buf2, GetGadgetText(&g_game->menu, "NUMBER", 0));
        elements[2].guid = DAT_004fcea8;
        elements[2].size = lstrlenA(buf2) + 1;
        elements[2].data = buf2;
        count = 3;
    } else if (memcmp(&guid, &DAT_004fcda8, sizeof(GUID)) == 0) {
        elements[0].guid = DAT_004fce88;
        elements[0].size = 0x10;
        elements[0].data = &DAT_004fcda8;
        char* t = GetGadgetText(&g_game->menu, "ADDRESS", 0);
        if (t == 0) {
            t = DAT_005119b8;
        }
        lstrcpyA(buf3, t);
        elements[1].guid = DAT_004fcee8;
        elements[1].size = lstrlenA(buf3) + 1;
        elements[1].data = buf3;
        count = 2;
    } else if (memcmp(&guid, &DAT_004fcd98, sizeof(GUID)) == 0) {
        elements[0].guid = DAT_004fce88;
        elements[0].size = 0x10;
        elements[0].data = &DAT_004fcd98;
        count = 1;
    } else if (memcmp(&guid, &DAT_004fcdb8, sizeof(GUID)) == 0) {
        elements[0].guid = DAT_004fce88;
        elements[0].size = 0x10;
        elements[0].data = &DAT_004fcdb8;
        DAT_00512770.unknown_8 = 0;
        DAT_00512770.unknown_c = 0;
        DAT_00512770.unknown_10 = 3;
        elements[1].guid = DAT_004fcf08;
        elements[1].size = 0x14;
        elements[1].data = &DAT_00512770;
        count = 2;
    } else {
        elements[0].guid = DAT_004fce88;
        elements[0].size = 0x10;
        elements[0].data = &guid;
        count = 1;
    }

    result = HAPINET_createcompoundaddress(&g_game->net, elements, count, 0, &size);
    if (result != 0x8877001e) goto cleanup;
    block = (HGLOBAL)FUN_004d83b0("COMPOUND ADDR", size);
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
int __stdcall FUN_00442000(int unused)
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
void __stdcall HandleTcpDialogClick(Gadget_00440d70* gadget)
{
    Entry_00440d70* entries = gadget->layer->entries;
    if (DAT_00512d90[0] != 0) {
        if (DAT_00512c84 == 0)
            DAT_00512d90[0] = 0;
    }
    else if (gadget->field_60 == -1) {
        return;
    }
    else if (FindGadgetIndex(entries, "OK", 0xe) == gadget->field_60) {
        goto connect;
    }
    else if (FindGadgetIndex(entries, "JOIN", 0xe) == gadget->field_60) {
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
        FUN_004ab0a0(gadget);
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
// line (DAT_00512d90) when there is one, otherwise the "TCPADDR" value; the
// ADDRESS gadget is then selected in the dialog.
// FUNCTION: 0x4421f0
void OpenTcpDialog()
{
    Layer_00440d70* dialog = LoadGuiLayer(&g_game->menu, "TCP.GUI", 0x800);
    dialog->handler = HandleTcpDialogClick;
    dialog->owner = g_game;
    dialog->field_1c = 0;
    LoadPictureCached(0, 0, 0, 0);
    HAPINET_initlobbiedconnection(&g_game->net);
    FindGadgetIndex(dialog->entries, "ADDRESS", 3);
    char* address = GetGadgetText(&g_game->menu, "ADDRESS", 0);
    int direct = DAT_00512d90[0];
    unsigned int len = 0x80;
    if (direct) {
        address[0] = 0;
        strncat(address, DAT_00512d90, len - 1);
    } else {
        if (!ReadGameRegistryValue("TCPADDR", address, &len)) {
            address[0] = 0;
        }
    }
    FUN_004a0bf0(&g_game->menu, "ADDRESS", address, 0);
    if (direct) {
        HandleTcpDialogClick(&g_game->menu);
        g_game->field_2aaf = g_game->field_2aaf ^ ((DAT_00512c84 != 0) ^ g_game->field_2aaf) & 1;
    } else {
        SelectGadgetByIndex(&g_game->menu, FindGadgetIndex(dialog->entries, "ADDRESS", 3));
        FUN_0049fc50(&g_game->menu, FindGadgetIndex(dialog->entries, "ADDRESS", 3));
        FUN_0049fb10(&g_game->menu, 1);
        RenderLayer(&g_game->menu, 0x40);
    }
}

// The original calls these two out of line from OpenSerialDialog; in this file
// /Ob2 would inline their bodies there.
#pragma auto_inline(off)
// FUNCTION: 0x442380
void __stdcall FUN_00442380(Gadget_00440d70* menu, Entry_00440d70* entry)
{
    int value = entry->index;
    if (value >= 0) {
        DAT_00512774 = DAT_00505490[value];
    }
}

// FUNCTION: 0x4423a0
void __stdcall FUN_004423a0(Gadget_00440d70* menu, Entry_00440d70* entry)
{
    int val = entry->index;
    if (val >= 0) {
        DAT_00512770.unknown_0 = val + 1;
    }
}
#pragma auto_inline(on)

// Click handler of the serial-link dialog (SERIAL.GUI, opened by OpenSerialDialog).
// HOST and JOIN set the multiplayer connection-mode bits of g_game (+0x2aaf)
// and save the address returned by BuildCompoundAddress; PREV goes back; anything else
// resets the gadget. The chosen baud rate and COM port are then written back to
// the registry under SERBAUD and SERPORT.
// FUNCTION: 0x4423c0
void __stdcall HandleSerialDialogClick(Gadget_00440d70* gadget)
{
    Entry_00440d70* entries = gadget->layer->entries;
    int a;
    int r;
    if (gadget->field_60 == -1)
        return;
    if (FindGadgetIndex(entries, "HOST", 0xe) == gadget->field_60) {
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
    } else if (FindGadgetIndex(entries, "JOIN", 0xe) == gadget->field_60) {
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
        FUN_004ab0a0(gadget);
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
    Layer_00440d70* gadget = LoadGuiLayer(&g_game->menu, "SERIAL.GUI", 0x800);
    gadget->handler = HandleSerialDialogClick;
    gadget->owner = g_game;
    gadget->field_1c = 0;
    LoadPictureCached(0, 0, 0, 0);
    HAPINET_initlobbiedconnection((char*)g_game + 0x14);
    FUN_004a32a0(&g_game->menu, "PORTS", "COM1\0COM2\0COM3\0COM4", 4, 0);
    FUN_004a32a0(&g_game->menu, "SPEEDS", "115200\0" "57600\0" "38400\0" "19200\0" "14400\0" "9600", 6, 0);

    int value;
    unsigned int size = 4;

    if (ReadGameRegistryValue("SERBAUD", &value, &size)) {
        FUN_004a2e40(&g_game->menu, "SPEEDS", value);
    }
    if (ReadGameRegistryValue("SERPORT", &value, &size)) {
        FUN_004a2e40(&g_game->menu, "PORTS", value);
    }
    Entry_00440d70* entry = FindGadgetChecked(gadget->entries, "PORTS");
    entry->handler = FUN_004423a0;
    FUN_004423a0(&g_game->menu, entry);
    Entry_00440d70* speeds = FindGadgetChecked(gadget->entries, "SPEEDS");
    speeds->handler = FUN_00442380;
    FUN_00442380(&g_game->menu, speeds);
    FUN_0049fb10(&g_game->menu, 1);
    RenderLayer(&g_game->menu, 0x40);
}

// Fills the ACCOUNTS menu entry with the 20 account names and restores the
// selected one.
// FUNCTION: 0x4426e0
void FillAccountList(void)
{
    char* buffer = DAT_0051298c;
    *buffer = 0;
    for (int i = 0; i < 20; i++) {
        strcpy(buffer, DAT_00512988[i].name);
        buffer += strlen(DAT_00512988[i].name) + 1;
    }
    Entry_00440d70* entry = FindGadgetChecked(g_game->menu.layer->entries, "ACCOUNTS");
    int player = entry->index;
    FUN_004a32a0((char*)g_game + 0x519, "ACCOUNTS", DAT_0051298c, 20, 0);
    FUN_004a2e40((char*)g_game + 0x519, "ACCOUNTS", player);
}

// Copies the selected account's name and number into the NAME and NUMBER
// entries and refreshes the ACCOUNTS list.
// FUNCTION: 0x4427a0
void RefreshAccountList(void)
{
    Entry_00440d70* entry = FindGadgetChecked(g_game->menu.layer->entries, "ACCOUNTS");
    if (entry != 0 && DAT_00512988 != 0) {
        GetGadgetText((char*)g_game + 0x519, "NAME",
                     DAT_00512988[entry->index].name);
        GetGadgetText((char*)g_game + 0x519, "NUMBER",
                     DAT_00512988[entry->index].number);
        char* buffer = DAT_0051298c;
        *buffer = 0;
        for (int i = 0; i < 20; i++) {
            strcpy(buffer, DAT_00512988[i].name);
            buffer += strlen(DAT_00512988[i].name) + 1;
        }
        int player = FindGadgetChecked(g_game->menu.layer->entries, "ACCOUNTS")->index;
        FUN_004a32a0((char*)g_game + 0x519, "ACCOUNTS", DAT_0051298c, 20, 0);
        FUN_004a2e40((char*)g_game + 0x519, "ACCOUNTS", player);
    }
}

// The original calls this out of line from 0x443100; in this file /Ob2 would
// inline its body there.
#pragma auto_inline(off)
// Shows the account the player entry selected in the NAME and NUMBER entries.
// FUNCTION: 0x4428f0
void __stdcall ShowSelectedAccount(Gadget_00440d70* menu, Entry_00440d70* player)
{
    int entry = player->index;
    if (entry >= 0) {
        FUN_004a0bf0(menu, "NAME", DAT_00512988[entry].name, 0);
        FUN_004a0bf0(menu, "NUMBER", DAT_00512988[entry].number, 0);
        int index = FindGadgetIndex(menu->layer->entries, "NAME", 3);
        SelectGadgetByIndex(menu, index);
        FUN_0049fc50(menu, index);
        FUN_0049fa90(menu);
    }
}
#pragma auto_inline(on)

// The last used entry is moved to the front of the modem number list, which is
// then written to the registry under "MODEMNUMBERS".
// FUNCTION: 0x442970
void SaveModemNumbers(void)
{
    if (DAT_00512988 != 0) {
        short count = FindGadgetChecked(g_game->menu.layer->entries, "ACCOUNTS")->index;
        if (count > 0) {
            Entry_004426e0 temp;
            memcpy(&temp, &DAT_00512988[count], 0x102);
            for (int i = count; i > 0; i--)
                memcpy(&DAT_00512988[i], &DAT_00512988[i - 1], 0x102);
            memcpy(&DAT_00512988[0], &temp, 0x102);
        }
        WriteGameRegistryValue("MODEMNUMBERS", DAT_00512988, 0x1428);
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
    Entry_00440d70* entry = FindGadgetChecked(g_game->menu.layer->entries, "ACCOUNTS");
    if (entry != 0 && DAT_00512988 != 0) {
        GetGadgetText((char*)g_game + 0x519, "NAME",
                     DAT_00512988[entry->index].name);
        GetGadgetText((char*)g_game + 0x519, "NUMBER",
                     DAT_00512988[entry->index].number);
        FillAccountList();
    }
}

// The last used entry moves to the front of the modem number list, which is
// then written to the registry under MODEMNUMBERS.
static inline void SaveModemNumbers_00442a30()
{
    if (DAT_00512988 != 0) {
        short count = FindGadgetChecked(g_game->menu.layer->entries, "ACCOUNTS")->index;
        if (count > 0) {
            Entry_004426e0 temp;
            memcpy(&temp, &DAT_00512988[count], 0x102);
            for (int i = count; i > 0; i--)
                memcpy(&DAT_00512988[i], &DAT_00512988[i - 1], 0x102);
            memcpy(&DAT_00512988[0], &temp, 0x102);
        }
        WriteGameRegistryValue("MODEMNUMBERS", DAT_00512988, 0x1428);
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
void __stdcall HandleModemDialogClick(Gadget_00440d70* gadget)
{
    Entry_00440d70* entries = gadget->layer->entries;
    if (gadget->field_60 == -1) {
        if (DAT_00512980 != 0) {
            FUN_004d85a0(DAT_00512980);
            DAT_00512980 = 0;
        }
        if (DAT_0051298c != 0) {
            FUN_004d85a0(DAT_0051298c);
            DAT_0051298c = 0;
        }
        if (DAT_00512988 != 0) {
            FUN_004d85a0(DAT_00512988);
            DAT_00512988 = 0;
        }
        return;
    }
    if (IsCurrentGadgetNamed(gadget, "NAME")) {
        LoadAccount_00442a30();
        FUN_0049fc50(gadget, FindGadgetIndex(entries, "NUMBER", 3));
        FUN_0049fa90(gadget);
        FUN_004ab0a0(gadget);
        return;
    }
    if (IsCurrentGadgetNamed(gadget, "NUMBER")) {
        LoadAccount_00442a30();
        SelectGadgetByIndex(gadget, FindGadgetIndex(entries, "JOIN", 1));
        FUN_0049fc50(gadget, FindGadgetIndex(entries, "JOIN", 1));
        strcpy((char*)entries + 0xcc, "JOIN");
        FUN_0049fa90(gadget);
        FUN_004ab0a0(gadget);
        return;
    }
    if (FindGadgetIndex(entries, "HOST", 0xe) == gadget->field_60) {
        LoadAccount_00442a30();
        SaveModemNumbers_00442a30();
        g_game->bit0 = 1;
        g_game->bit1 = 1;
        TryConnect_00442a30();
        PlaySoundByName("SMLBUTTON", 0);
        BlankScreen();
        return;
    }
    if (FindGadgetIndex(entries, "JOIN", 0xe) != gadget->field_60) {
        if (IsCurrentGadgetNamed(gadget, "ACCOUNTS") == 0) {
            if (IsCurrentGadgetNamed(gadget, "PREV")) {
                SetFrontendState(0xf, 0x47f, "c:\\cavedog\\wargame\\multi.cpp");
                PlaySoundByName("Previous", 0);
                return;
            }
            FUN_004ab0a0(gadget);
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
// the chunk is DPAID_Modem (DAT_004fcec8, {f6dcc200-a2fe-11d0-9c4f-00a0c905425e}),
// the data is a double-null-terminated list of modem names; each one is
// copied into the buffer at DAT_00512980 and counted in DAT_00512984.
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
    if (IsEqualGUID(guidDataType, DAT_004fcec8)) {
        while (lstrlenA(name) != 0) {
            strcpy(NameSlot(DAT_00512980), name);
            DAT_00512984++;
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
    Layer_00440d70* gadget;
    // 8-byte local, not the large Mission type.
    struct { void* dp; void* dp3; } net;
    GUID iid = DAT_004fcdc8;

    gadget = LoadGuiLayer(&g_game->menu, "MODEM.GUI", 0x800);
    gadget->handler = HandleModemDialogClick;
    gadget->owner = g_game;
    LoadPictureCached(0, 0, 0, 0);
    HAPINET_initlobbiedconnection(&g_game->net);
    r = HAPINET_createdplayinterface(&iid, (Net_00443100*)&net);
    if (r >= 0) {
        r = HAPINET_getplayeraddress((Net_00443100*)&net, 0, 0, &size);
        if (r == DPERR_BUFFERTOOSMALL_00443100) {
            addr = (char*)FUN_004d83b0("MODEMADDR", size);
            if (addr != 0) {
                r = HAPINET_getplayeraddress((Net_00443100*)&net, 0, addr, &size);
                if (r >= 0) {
                    DAT_00512980 = (char*)FUN_004d83b0("MODEMINFO", 0xc8);
                    memset(DAT_00512980, 0, 0xc8);
                    DAT_00512984 = 0;
                    r = HAPINET_enumaddress(&g_game->net, (void*)EnumModemAddressCallback, addr, size, 0);
                    if (DAT_00512984 == 0) {
                        CloseTopScreen(&g_game->menu);
                        SetFrontendErrorText("Unable to find any modems");
                        SetFrontendState(0xf, 0x4e4, "c:\\cavedog\\wargame\\multi.cpp");
                        SetFrontendSubState(0, 0x4e5, "c:\\cavedog\\wargame\\multi.cpp");
                        FUN_004d85a0(DAT_00512980);
                        FUN_004d85a0(addr);
                        HAPINET_releasedplayinterface((Net_00443100*)&net);
                        return;
                    }
                    if (r >= 0) {
                        int i;
                        FUN_004a32a0(&g_game->menu, "MODEMS", DAT_00512980, DAT_00512984, 0);
                        DAT_00512988 = (Entry_004426e0*)FUN_004d83b0("MODEMACCOUNTS", 0x1428);
                        len.v = 0x1428;
                        r = ReadGameRegistryValue("MODEMNUMBERS", DAT_00512988, &len.v);
                        if (r == 0) {
                            for (i = 0; i < 20; i++) {
                                strcpy(DAT_00512988[i].name, "UNUSED");
                                DAT_00512988[i].number[0] = 0;
                            }
                        }
                        char* buffer = (char*)FUN_004d83b0("ACCOUNTNAMES", 0xa00);
                        DAT_0051298c = buffer;
                        *buffer = 0;
                        // Separate p runs the copy loop: keeps eax free until loop entry.
                        char* p = buffer;
                        for (i = 0; i < 20; i++) {
                            strcpy(p, DAT_00512988[i].name);
                            p += strlen(DAT_00512988[i].name) + 1;
                        }
                        Entry_00440d70* entry = FindGadgetChecked(g_game->menu.layer->entries, "ACCOUNTS");
                        int player = entry->index;
                        FUN_004a32a0(&g_game->menu, "ACCOUNTS", DAT_0051298c, 20, 0);
                        FUN_004a2e40(&g_game->menu, "ACCOUNTS", player);
                        entry = FindGadgetChecked(gadget->entries, "ACCOUNTS");
                        entry->handler = ShowSelectedAccount;
                        ShowSelectedAccount(&g_game->menu, entry);
                    }
                }
            }
        }
    }
    FUN_0049fb10(&g_game->menu, 1);
    RenderLayer(&g_game->menu, 0x40);
    HAPINET_releasedplayinterface((Net_00443100*)&net);
    FUN_004d85a0(addr);
}

// FUNCTION: 0x443480
void __stdcall HandleReportClick(Gadget_00440d70* obj)
{
    int i;
    Entry_00440d70* entries;
    int value;
    char buf[16];
    int handle;
    int acc;

    entries = obj->layer->entries;
    if (obj->field_60 == -1)
        return;
    if (IsCurrentGadgetNamed(obj, "OK")) {
        acc = 0;
        for (i = 0; i < 16u; i++) {
            wsprintfA(buf, "CHK%d", i);
            handle = FindGadgetIndex(entries, buf, 1);
            if (!FUN_004a04f0(obj, buf))
                break;
            value = GetGadgetStatus(obj, handle);
            acc = (int)(pow(2.0, i) * value + acc);
        }

        SetCursorMode(0x14);
        EnableReporter(acc);
        if ((g_game->field_2bee & 0x10) || g_game->field_2bbf == 0x14) {
            g_game->field_2bc0 = 0x15;
        } else {
            g_game->field_2bc0 = 0x11;
        }
    } else {
        FUN_004ab0a0(obj);
    }
}

// FUNCTION: 0x443590
void FUN_00443590(void)
{
    SendNetHeartbeat();
}

// Opens the REPORT.GUI dialog, fills in the "CHK%d" / "SERVICE%d" menu
// entries from the score report tables, and shows it.
// FUNCTION: 0x4435a0
void __stdcall OpenReportDialog(unsigned int* count, char** names)
{
    char name[16];
    Layer_00440d70* gadget = LoadGuiLayer(&g_game->menu, "REPORT.GUI", 0x800);
    gadget->handler = HandleReportClick;
    gadget->owner = g_game;
    gadget->field_1c = FUN_00443590;
    LoadPictureCached("scorebg", 0, 1, 0);
    for (unsigned int i = 0; i < *count; i++) {
        wsprintfA(name, "CHK%d", i);
        FUN_004a0570(&g_game->menu, name, 1);
        wsprintfA(name, "SERVICE%d", i);
        FUN_004a0570(&g_game->menu, name, 1);
        FUN_004a0bf0(&g_game->menu, name, names[i], 0x80);
    }
    FUN_0049fb10(&g_game->menu, 1);
    RenderLayer(&g_game->menu, 0x141);
    SetCursorMode(0x13);
    FUN_0049fa90(&g_game->menu);
    FUN_0049fad0(&g_game->menu);
}

// FUNCTION: 0x4436e0
int InitScoreReporting(void)
{
    if (g_game->field_391e9->GetGameType() != 3)
        return 0;
    int saved = g_game->field_2cbe;
    SetCursorMode(0x14);
    int r = LoadReporterDll(&DAT_005054a8, &DAT_00512788);
    if (r == 0) {
        if (DAT_005054a8 > 0) {
            SetCursorMode(0x13);
            OpenReportDialog(&DAT_005054a8, (char**)&DAT_00512788);
            SetCursorMode(saved);
            return 1;
        }
    } else if (r != 4) {
        SetOffscreenSurface(g_game->field_37e1b);
        OpenMessageBox(&g_game->menu, Translate("Unable to initialize scores reporting."), 0x190, 1, 0);
        LoadPictureCached("ReportError", 0, 1, 0);
        RunWhileScreenNamed(&g_game->menu, "MSGBOX.GUI");
    }
    SetCursorMode(saved);
    return 0;
}

// The 0xbc-byte frame is `Msg_004437c0 msg;`: the group of four dwords is
// copied out of g_game+0x2b6e at msg+0x99, so the leading pad is real and the
// trailing pad keeps the struct at 0xbc exactly.
// Handler for the SELGAME (multiplayer game list) screen. Processes the
// UPDATE / PREVMENU / WATCH / JOINGAME / STARTNEW buttons and the per-entry
// "compatible version" check. param_1 is &g_game->menu (g_game+0x519); its
// +0x18 field is the widget created by LoadGuiLayer, whose +4 is the GUI entry
// table and whose +0x60 is the id of the pressed entry.
// FUNCTION: 0x4437c0
void __stdcall HandleSelectGameClick(Gadget_00440d70* param_1)
{
    Entry_00440d70* entries = param_1->layer->entries;
    int i;
    int id;
    int cur;

    if (DAT_00512d90[0] != 0) {
        DAT_00512d90[0] = 0;
        if (DAT_00512c84 != 0)
            goto startnew;
    }

    if (param_1->field_60 == -1) {
        for (i = 0; i < 0xf; i++) {
            FUN_004d85a0(g_game->data[i]);
            g_game->data[i] = 0;
        }
        FUN_004d85a0(g_game->shared);
        FUN_004d85a0(g_game->desc);
        g_game->shared = 0;
        g_game->desc = 0;
        return;
    }

    if (FindGadgetIndex(entries, "UPDATE", 0xe) == param_1->field_60) {
        char* pass = (char*)FUN_004a0010(entries, "PASSWORD");
        if (pass != 0) {
            cur = g_game->localPlayer;
            strcpy(g_game->players[cur].info->name, pass + 0xb6);
        }
        PlaySoundByName("Multi", 0);
        ConnectToGame(param_1->layer);
        FUN_0049fa90(param_1);
        FUN_004ab0a0(param_1);
        return;
    }

    if (FindGadgetIndex(entries, "PREVMENU", 0xe) == param_1->field_60) {
        g_game->field_2bc0 = 3;
        PlaySoundByName("Previous", 0);
        return;
    }

    if (FindGadgetIndex(entries, "WATCH", 0xe) == param_1->field_60 ||
        FindGadgetIndex(entries, "JOINGAME", 0xe) == param_1->field_60 ||
        entries[param_1->field_60].type == 2) {
        Entry_00440d70* e = FindGadgetChecked(entries, "GAMENAME");
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
                FUN_004ab0a0(param_1);
                return;
            }
            GetGadgetText(param_1, "NICKNAME", g_game->nickname);
            if (strlen(g_game->nickname) == 0) {
                FUN_004a7190(param_1, FindGadgetIndex(entries, "NICKNAME", 3));
                FUN_004ab0a0(param_1);
                OpenMessageBox(param_1, Translate("You must enter your name"), 0xc8, 1, 1);
                return;
            }
            pass = (char*)FUN_004a0010(entries, "PASSWORD");
            lstrcpynA(g_game->password, pass + 0xb6, 0xb);
            cur = g_game->localPlayer;
            lstrcpynA(g_game->players[cur].info->name, pass + 0xb6, 0xb);
            b = FindHostSlot();
            if (b != 0xa &&
                (g_game->players[b].info->flags & 0x10) == 0x10)
                g_game->field_2a44 |= 4;
            if (IsCurrentGadgetNamed(param_1, "WATCH")) {
                cur = g_game->localPlayer;
                g_game->players[cur].info->flags |= 0x40;
                PlaySoundByName("Multi", 0);
                g_game->field_2bc0 = 0x13;
                return;
            }
            cur = g_game->localPlayer;
            g_game->players[cur].info->flags &= 0xffbf;
            PlaySoundByName("BigButton", 0);
            g_game->field_2bc0 = 0x12;
            return;
        }
        SetFrontendErrorText("You do not have a compatible version for this game.");
    } else if (FindGadgetIndex(entries, "STARTNEW", 0xe) == param_1->field_60) {
startnew:
        cur = g_game->localPlayer;
        g_game->players[cur].info->flags &= 0xffbf;
        PlaySoundByName("BigButton", 0);
        GetGadgetText(param_1, "NICKNAME", g_game->nickname);
        BlankScreen();
        CloseTopScreen(param_1);
        OpenNewMultiDialog();
        FUN_004ab0a0(param_1);
        return;
    }
    FUN_004ab0a0(param_1);
}

// Opens the network game selection dialog (SELGAME.GUI) and sets up the
// buffers it needs: a 0x690 byte "GAME DESCRIPTIONS" block, a 0xe74 byte
// "PLAYER SHARED" block and 15 blocks of 0xa00 bytes named "DATA0" .. "DATA14",
// then a table of (size, offset) pairs (0xb9 bytes each) in the descriptions
// block pointing into the shared block. Every GUI entry from 1 up whose type
// byte is 2 gets FUN_00441220 as its handler and the descriptions block as its
// data. ConnectToGame then connects; on failure an "Invalid TCP/IP Address"
// message box is shown, g_game->field_2bc0 is set to 3 and the function
// returns. On success the game name is put on the menu, and a connection that
// came back with an error status (neither 0 nor 2) is reported and cleared.
// FUNCTION: 0x443cb0
void OpenSelectGameDialog()
{
    char name[0x14];
    int i;
    int j;

    BlankScreen();
    Layer_00440d70* gadget = LoadGuiLayer(&g_game->menu, "SELGAME.GUI", 0x80);
    gadget->handler = HandleSelectGameClick;
    gadget->owner = g_game;
    LoadPictureCached("selectgame2x", 0, 0, 0);
    g_game->desc = (Desc_004437c0*)FUN_004d83b0("GAME DESCRIPTIONS", 0x690);
    g_game->shared = (char*)FUN_004d83b0("PLAYER SHARED", 0xe74);
    for (i = 0; i < 0xf; i++) {
        sprintf(name, "DATA%d", i);
        g_game->data[i] = FUN_004d83b0(name, 0xa00);
    }
    // Offset is j * 0xb9, no separate off counter.
    for (j = 0; j < 20; j++) {
        g_game->desc[j].size = 0xb9;
        g_game->desc[j].offset = (int)(g_game->shared + j * 0xb9);
    }
    FUN_004a0bf0(&g_game->menu, "PASSWORD", g_game->password, 10);
    FUN_004a0bf0(&g_game->menu, "NICKNAME", g_game->nickname, 10);
    FUN_004a1250(&g_game->menu, "JOIN", 1);
    FUN_004a1250(&g_game->menu, "WATCH", 1);
    for (i = 1; i < gadget->entries->count; i++) {
        if (gadget->entries[i].type == 2) {
            // Indexed inline and bound by reference: no named entries pointer local.
            Entry_00440d70& e = gadget->entries[i];
            e.handler = FUN_00441220;
            e.data = (int)g_game->desc;
        }
    }
    RenderLayer(&g_game->menu, 0x40);
    if (!ConnectToGame(gadget)) {
        CloseTopScreen(&g_game->menu);
        OpenMessageBox(&g_game->menu, Translate("Invalid TCP/IP Address"), 0xc8, 1, 1);
        g_game->field_2bc0 = 3;
        return;
    }
    SelectGadgetByIndex(&g_game->menu, FindGadgetIndex(gadget->entries, "GAMENAME", 2));
    FUN_00428b60();
    FUN_0049fb10(&g_game->menu, 1);
    RenderLayer(&g_game->menu, 0x40);
    Player_441080* conn = &g_game->players[g_game->localPlayer];
    if (conn->status != 0 && conn->status != 2) {
        RenderLayer(&g_game->menu, 0x40);
        char* msg = GetRejectReasonText(conn->status);
        OpenMessageBox(&g_game->menu, Translate(msg), 0x140, 1, 1);
        FUN_0049fa90(&g_game->menu);
        FUN_0049fad0(&g_game->menu);
        conn->status = 0;
    }
    if (DAT_00512d90[0] != 0 && DAT_00512c84 != 0) {
        if (strlen(g_game->nickname) != 0)
            HandleSelectGameClick(&g_game->menu);
    }
}

// Appends a copy of entry `param_2` to the table (count at +0xb6 of entry 0),
// initialises it through the menu (FUN_004a09c0), then sets its name, value,
// state and flags. The body is inlined at 0x4447d4 by the caller that builds a
// name with sprintf first.
// FUNCTION: 0x4444d0
int __stdcall FUN_004444d0(Entry_00440d70* entries, int param_2, short param_3, int param_4, char* param_5)
{
    int index = ++entries[0].count;
    Entry_00440d70* d = &entries[index];
    Entry_00440d70* s = &entries[param_2];
    *d = *s;
    FUN_004a09c0((char*)g_game + 0x519, index, param_4, 0);
    strcpy(d->name, param_5);
    d->field_15 = param_3;
    d->field_29 = 1;
    d->field_1f = 0;
    return index;
}

// FUNCTION: 0x444910
void __stdcall FUN_00444910(Gadget_00440d70* param1, int param2)
{
    param1->field_60 = FindGadgetIndex(param1->layer->entries, "LOGOS", 2);
}
