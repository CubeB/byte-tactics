// Decompiled by Opus, deepseek-v4.1-flash, DeepSeek V4.1 Flash, space-bunny-free, deepseek-v4.1, Claude Opus 5.5, muse-spark-1.3-free, mimo-v2.6-pro, claude-opus-5-5, GPT-6 and GPT-6.1-sol. Names are provisional.
// The network game's lobby join, player colours and alliances, packet
// dispatch, rejection, timeout and chat handling: the files of the module's
// second part (0x451540 to 0x4560c0) gathered in address order.
//
// The headers carry the files' needs: <windows.h> and <process.h> the lobby
// join and display calls, <string.h> and <stdio.h> the string and printf
// patterns, and <iostream> 0x453010's shared return block.
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <process.h>
#include <string.h>
#include <stdio.h>
#include <iostream>

#pragma pack(push, 1)

typedef int (__stdcall *EntryFunc)(int);

class PacketChannel;

class PacketReceiver {
public:
    int ReceiveFrame(void* net, unsigned char* data, int* size);
};

class PacketManager {
public:
    int SendAllQueued(int param_1);
    int QueuePacket(int param_1, int param_2, void* param_3, unsigned int param_4);
    int QueueOnChannel(int param_1, PacketChannel* param_2, int param_3, int param_4);
};

struct Mission {
    int GetGameType();
};

struct CobScript {
    int StartScriptWithArgsByIndex(int, int, int, int, int, int, int, int);
};

class Class_0046d500 {
public:
    void ReceiveSyncPacket(void*, unsigned char);
};

class Class_00463be0 {
public:
    char data[0x14b];
    Class_00463be0();
};

class Class_00463c40 {
public:
    void FreeSideDataAndFogSightCounts();
};

class Class_00461620 {
public:
    void HandleIntegrityNop(int, int, int);
};

struct Class_00456030 {
    int field_0;                       // +0x0
    char unknown_4[0x73 - 0x4];
    char field_73;                     // +0x73
    int FUN_00456030();
};

struct Feature {
    char data[0x115];
};

struct Settings {
    int field_471;                     // +0x471
    int flags_475;                     // +0x475
    char unknown_479[0x48];
};

struct Guid_4517b0 {
    int data[4];
};

struct Net2_4517b0 {
    char unknown_0[8];
    char* field_8;                     // +0x8
    char* field_c;                     // +0xc
};

struct Net_4517b0 {
    char unknown_0[4];
    unsigned int field_4;              // +0x4
    char* field_8;                     // +0x8
    Net2_4517b0* field_c;              // +0xc
};

// The display object of 0x451640 and 0x4517b0: one type for both views.
struct Obj_00451640 {
    char unknown_0[0xf0];
    unsigned short bit0 : 1;
    unsigned short flag : 1;           // +0xf0, mask 2
};

// The LOBBY JOIN INFO argument block of 0x451640 and 0x451770.
struct Args_00451640 {
    char* name;                        // +0x00
    int arg_4;                         // +0x04
    int arg_8;                         // +0x08
    int arg_c;                         // +0x0c
    int arg_10;                        // +0x10
    int arg_14;                        // +0x14
    int result;                        // +0x18
};

// The packet data object of 0x451fd0 and 0x452370: one type for both views.
struct Class_00451fd0 {
    char unknown_0[0x870];
    unsigned int field_870;            // +0x870
    char unknown_874[0x1745 - 0x874];
    int field_1745;                    // +0x1745
    union {
        int* field_1749;               // +0x1749
        int* buffer;                   // +0x1749
    };
};

struct Short3_00456050 {
    short a, b, c;
};

struct Vec3_00456050 {
    int x, y, z;
};

struct Ring_00453640 {                 // 0x48 bytes, 30 of them at g_game+0x12ef
    char text[0x40];                   // +0x00
    unsigned int time;                 // +0x40
    unsigned short field_44;           // +0x44
    char field_46;                     // +0x46
    unsigned char field_47;            // +0x47
};

struct Entry_00453640 {                // gadget returned by FindGadgetChecked
    char unknown_0[0xc0];
    short count;                       // +0xc0
};

struct Holder_00453640 {
    int unknown_0;                     // +0x00
    Entry_00453640* entries;           // +0x04
};

struct Entry_004538f0 {
    char unknown_0[0xb6];
    char text[0x15b - 0xb6];           // +0xb6
};

struct Layer_004538f0 {
    int unknown_0;
    Entry_004538f0* entries;           // +0x04
};

struct Gadget_004538f0 {
    char unknown_0[0x18];
    Layer_004538f0* layer;             // +0x18
    char unknown_1c[0x60 - 0x1c];
    int field_60;                      // +0x60
};

// The 0x15b-byte entry table returned by FindGadgetChecked.
struct OutEntry_00453a50 {
    char unknown_0[0x19];
    short field_19;                    // +0x19
};

// The dialog object returned by LoadGuiLayer.
struct Gui_00453a50 {
    char unknown_0[4];
    void* entries;                     // +0x04
    void (__stdcall* callback)(void*); // +0x08
    char unknown_c[0x1c - 0xc];
    void (__stdcall* field_1c)(void*); // +0x1c
};

// The info block a player record points at (+0x27). One type for every view:
// the union members are the names the files give the same bytes.
struct PlayerInfo {
    char unknown_0[0x80];
    char name[0xb];                    // +0x80
    unsigned short field_8b;           // +0x8b
    unsigned short field_8d;           // +0x8d
    char unknown_8f;
    union {
        int field_90;                  // +0x90
        int id;                        // +0x90
    };
    unsigned char field_94;            // +0x94
    char unknown_95;
    union {
        unsigned char field_96;        // +0x96
        unsigned char group;           // +0x96
    };
    union {
        unsigned char flags_97;        // +0x97
        unsigned char flags;           // +0x97
        unsigned short flag_97_0 : 1;  // +0x97
        unsigned short ready : 1;
    };
    char unknown_99[0x9b - 0x99];
    union {
        unsigned short field_9b;       // +0x9b
        unsigned char flags_9b;        // +0x9b
        struct {
            unsigned short : 4;
            unsigned short bit4 : 1;
            unsigned short : 11;
        } b9b;
        struct {
            unsigned short : 15;
            unsigned short bit15 : 1;
        } w9b;
    };
    union {
        unsigned char flags_9d;        // +0x9d
        unsigned short word_9d;        // +0x9d
        unsigned short field_9d : 1;   // +0x9d
    };
    char unknown_9f[0xb9 - 0x9f];
};

// A player record at g_game+0x1b63 (0x14b bytes). One type for every view:
// where two views name the same bytes differently the union carries both
// names, and where a view reads them as another type the union carries that.
struct Player {
    union {
        int active;                    // +0x00
        int field_0;                   // +0x00
    };
    union {
        int id;                        // +0x04
        int field_4;                   // +0x04
    };
    int field_8;                       // +0x08
    union {
        int field_c;                   // +0x0c
        int team;                      // +0x0c
    };
    int messages;                      // +0x10
    char unknown_14[0x1c - 0x14];
    union {
        int lastHeard;                 // +0x1c
        int last_time;                 // +0x1c
    };
    unsigned char field_20;            // +0x20
    unsigned char flags_21;            // +0x21
    unsigned char field_22;            // +0x22
    char unknown_23[0x27 - 0x23];
    union {
        PlayerInfo* info;              // +0x27
        PlayerInfo* data;              // +0x27
        PlayerInfo* field_27;          // +0x27
    };
    char name[0x1e];                   // +0x2b
    char field_49[0x1e];               // +0x49
    char unknown_67[0x73 - 0x67];
    union {
        char state;                    // +0x73
        unsigned char type;            // +0x73
        char flag_73;                  // +0x73
        unsigned char valid;           // +0x73
    };
    char unknown_74[0x108 - 0x74];
    union {
        unsigned char allies[0x16];    // +0x108
        unsigned char allied[0x3e];    // +0x108
        struct {
            char unknown_108[0xb];
            unsigned char field_113[0xb]; // +0x113
        };
        struct {
            char unknown_11e[0x16];
            unsigned char field_11e[0x16]; // +0x11e
        };
        struct {
            char unknown_134[0x2c];
            unsigned char field_134[0xb];  // +0x134
        };
        struct {
            char unknown_13f[0x37];
            unsigned char field_13f;       // +0x13f
        };
    };
    union {
        unsigned char index;           // +0x146
        unsigned char field_146;       // +0x146
    };
    unsigned char field_147;           // +0x147
    char unknown_148[0x14b - 0x148];
    void SetType(int);
};

// A unit: the player it belongs to, its script, and the fields the network
// game reads. One type for the views of 0x452570, 0x453d40, 0x456050 and
// 0x4560c0.
struct Unit {
    char unknown_0[0x64];
    Short3_00456050 field_64;          // +0x64
    Vec3_00456050 field_6a;            // +0x6a
    char unknown_76[0x96 - 0x76];
    Player* player;                    // +0x96
    union {
        CobScript* field_9a;           // +0x9a
        CobScript* names;              // +0x9a
    };
    char unknown_9e[0xa6 - 0x9e];
    short field_a6;                    // +0xa6
    short field_a8;                    // +0xa8
    char unknown_aa[0x110 - 0xaa];
    unsigned int flags_110;            // +0x110
    char unknown_114[4];
    void SetStateBits(unsigned char, int);
};

struct Packet_4517b0 {
    unsigned char type;                // +0x0
    PlayerInfo data;                   // +0x1
};

struct Packet_00452960 {
    unsigned char type;                // +0x0
    int from;                          // +0x1
    int to;                            // +0x5
    unsigned char value;               // +0x9
    int extra;                         // +0xa
};

struct Packet_00452b70 {
    unsigned char type;                // +0x0
    int from;                          // +0x1
    int to;                            // +0x5
    char value;                        // +0x9
    int extra;                         // +0xa
};

struct Packet_00456050 {
    unsigned char type;                // +0x0
    short field_1;                     // +0x1
    short field_3;                     // +0x3
    Vec3_00456050 field_5;             // +0x5
    Short3_00456050 field_11;          // +0x11
};

struct Packet_004560c0 {
    unsigned char type;                // +0x0
    short field_1;                     // +0x1
    short field_3;                     // +0x3
};

// The incoming packet's header, cast onto g_game->buffer.
struct Packet {
    unsigned int type;                 // +0x00
    int field_4;                       // +0x04
    int id;                            // +0x08
    char* field_c;                     // +0x0c
    char* field_10;                    // +0x10
    int field_14;                      // +0x14
    char* field_18;                    // +0x18
};

// The 0x2a44 flag word 0x452cc0 reads as 16 bits.
union GameFlags_2a44 {
    unsigned short value;              // +0x2a44
    struct {
        unsigned short b0 : 1;
        unsigned short b1 : 1;
        unsigned short b2 : 1;
        unsigned short b3 : 1;
        unsigned short rest : 12;
    } bits;
};

struct Game {
    char unknown_0[0x14];
    char field_14[0x471 - 0x14];       // +0x14
    Settings settings;                 // +0x471
    char unknown_4c1[0x4c9 - 0x4c1];
    int from_id;                       // +0x4c9
    int local_id;                      // +0x4cd
    char unknown_4d1[0x4e5 - 0x4d1];
    Net_4517b0* field_4e5;             // +0x4e5
    char unknown_4e9[0x519 - 0x4e9];
    char message[0x531 - 0x519];       // +0x519
    union {
        Holder_00453640* holder;       // +0x531
        Layer_004538f0* layer_531;     // +0x531
    };
    char unknown_535[0x12ef - 0x535];
    Ring_00453640 ring[30];            // +0x12ef
    char unknown_1b5f[0x1b63 - 0x1b5f];
    Player players[10];                // +0x1b63
    char unknown_2851[0x299c - 0x2851];
    int field_299c;                    // +0x299c
    char unknown_29a0[0x29a4 - 0x29a0];
    int field_29a4[11];                // +0x29a4
    int field_29d0[11];                // +0x29d0
    char unknown_29fc[0x2a30 - 0x29fc];
    Class_0046d500* field_2a30;        // +0x2a30
    int field_2a34;                    // +0x2a34
    union {
        unsigned char* buffer;         // +0x2a38
        unsigned char* packet;         // +0x2a38
    };
    short field_2a3c;                  // +0x2a3c
    unsigned short tail;               // +0x2a3e
    unsigned short head;               // +0x2a40
    union {
        unsigned char localPlayer;     // +0x2a42
        unsigned char field_2a42;      // +0x2a42
        unsigned char local;           // +0x2a42
    };
    char unknown_2a43;
    union {
        unsigned char flags_2a44;      // +0x2a44, byte views
        unsigned short flags_2a44_w;   // +0x2a44, word view
        GameFlags_2a44 flags;          // +0x2a44, 0x452cc0's view
    };
    char unknown_2a46[0x2bd2 - 0x2a46];
    char field_2bd2[0x11];             // +0x2bd2
    char field_2be3[0x2bee - 0x2be3];  // +0x2be3
    union {
        unsigned short flag0 : 1;      // +0x2bee
        unsigned short dirty : 1;      // +0x2bee
    };
    unsigned char chatMode;            // +0x2bf0
    unsigned char field_2bf1[10];      // +0x2bf1
    char unknown_2bfb[0x2c28 - 0x2bfb];
    char field_2c28[40];               // +0x2c28
    char unknown_2c50[0x2cf3 - 0x2c50];
    Feature features[256];             // +0x2cf3
    char unknown_after_features[0x14357 - (0x2cf3 + 0x115 * 256)];
    Unit* units;                       // +0x14357
    char unknown_1435b[0x37f1b - 0x1435b];
    unsigned short field_37f1b;        // +0x37f1b
    char unknown_37f1d[0x37f1f - 0x37f1d];
    unsigned short field_37f1f;        // +0x37f1f
    char unknown_37f21[0x37f2f - 0x37f21];
    unsigned char field_37f2f;         // +0x37f2f
    char unknown_37f30;
    unsigned int field_37f31;          // +0x37f31
    char unknown_37f35[0x38a51 - 0x37f35];
    union {
        unsigned char field_38a51;     // +0x38a51
        unsigned short bit_38a51 : 1;  // +0x38a51
    };
    char unknown_38a53[0x38d75 - 0x38a53];
    union {
        volatile unsigned char flags_38d75; // +0x38d75, byte view
        volatile unsigned short net_flags;  // +0x38d75, word view
        struct {
            unsigned short : 2;
            unsigned short net_bit2 : 1;
            unsigned short : 13;
        } net_bits;
    };
    char unknown_38d77[0x391e9 - 0x38d77];
    Mission* net;                      // +0x391e9
    char unknown_391ed[0x391f1 - 0x391ed];
    int mode;                          // +0x391f1
    char unknown_391f5[0x3923b - 0x391f5];
    unsigned short : 2;                // +0x3923b
    unsigned short bit2_3923b : 1;
    unsigned short : 1;
    unsigned short bit4_3923b : 1;
    unsigned short : 11;
};

#pragma pack(pop)

extern Game* g_game;
extern int g_usePacketManager;
extern PacketManager g_packetManager;
extern PacketChannel DAT_00513008;
extern int DAT_00512b90[11];
extern PacketReceiver DAT_0051e300;
extern char DAT_005119b8[];
extern int DAT_00512c8c;
extern char DAT_00512d48;
extern char DAT_00512d28;
extern char* g_loungeChatter;
extern int g_loungeRefreshTime;
extern int g_timeoutPlayerDpid;
extern unsigned int g_timeoutTimerStart;
extern int g_packetModes[];
extern char DAT_00505dc4[];
extern char DAT_005065c4[];
extern char DAT_0050658c[];
extern char DAT_00506290[];
extern char s_PACKET_DATA_00506524[];
extern EntryFunc DAT_00512a28;
extern EntryFunc DAT_00512a2c;
extern EntryFunc DAT_00512a34;
extern EntryFunc DAT_00512a38;
extern EntryFunc DAT_00512a3c;
extern EntryFunc DAT_00512a40;
extern EntryFunc DAT_00512a44;
extern EntryFunc DAT_00512a48;
extern EntryFunc DAT_00512a4c;
extern EntryFunc DAT_00512a50;
extern EntryFunc DAT_00512a54;
extern EntryFunc DAT_00512a58;
extern EntryFunc DAT_00512a5c;
extern EntryFunc DAT_00512a60;
extern EntryFunc DAT_00512a64;
extern EntryFunc DAT_00512a68;
extern EntryFunc DAT_00512a6c;
extern EntryFunc DAT_00512a70;
extern EntryFunc DAT_00512a74;
extern EntryFunc DAT_00512a78;
extern EntryFunc DAT_00512a7c;
extern EntryFunc DAT_00512a80;
extern EntryFunc DAT_00512a84;
extern EntryFunc DAT_00512a88;
extern EntryFunc DAT_00512a8c;
extern EntryFunc DAT_00512a90;
extern EntryFunc DAT_00512a94;
extern EntryFunc DAT_00512a98;
extern EntryFunc DAT_00512a9c;
extern EntryFunc DAT_00512aa0;
extern EntryFunc DAT_00512aa4;
extern EntryFunc DAT_00512aa8;
extern EntryFunc DAT_00512aac;
extern EntryFunc DAT_00512ab0;
extern EntryFunc DAT_00512ab4;
extern EntryFunc DAT_00512ab8;
extern EntryFunc DAT_00512abc;
extern EntryFunc DAT_00512ac0;
extern EntryFunc DAT_00512ac4;
extern EntryFunc DAT_00512ac8;
extern EntryFunc DAT_00512ad0;
extern int DAT_00512adc;
extern int DAT_00512ae0;
extern int DAT_00512ae4;
extern int DAT_00512aec;
extern int DAT_00512af0;
extern int DAT_00512af4;
extern int DAT_00512af8;
extern int DAT_00512afc;
extern int DAT_00512b00;
extern int DAT_00512b04;
extern int DAT_00512b08;
extern int DAT_00512b0c;
extern int DAT_00512b10;
extern int DAT_00512b14;
extern int DAT_00512b18;
extern int DAT_00512b1c;
extern int DAT_00512b20;
extern int DAT_00512b24;
extern int DAT_00512b28;
extern int DAT_00512b2c;
extern int DAT_00512b30;
extern int DAT_00512b34;
extern int DAT_00512b38;
extern int DAT_00512b3c;
extern int DAT_00512b40;
extern int DAT_00512b44;
extern int DAT_00512b48;
extern int DAT_00512b4c;
extern int DAT_00512b50;
extern int DAT_00512b54;
extern int DAT_00512b58;
extern int DAT_00512b5c;
extern int DAT_00512b60;
extern int DAT_00512b64;
extern int DAT_00512b68;
extern int DAT_00512b6c;
extern int DAT_00512b70;
extern int DAT_00512b74;
extern int DAT_00512b78;
extern int DAT_00512b7c;
extern int DAT_00512b80;
extern int DAT_00512b88;
extern int DAT_00512bc8;
extern int DAT_00512bcc;
extern int DAT_00512bd4;
extern int DAT_00512bd8;
extern int DAT_00512bdc;
extern int DAT_00512be0;
extern int DAT_00512be4;
extern int DAT_00512be8;
extern int DAT_00512bec;
extern int DAT_00512bf0;
extern int DAT_00512bf4;
extern int DAT_00512bf8;
extern int DAT_00512bfc;
extern int DAT_00512c00;
extern int DAT_00512c04;
extern int DAT_00512c08;
extern int DAT_00512c0c;
extern int DAT_00512c10;
extern int DAT_00512c14;
extern int DAT_00512c18;
extern int DAT_00512c1c;
extern int DAT_00512c20;
extern int DAT_00512c24;
extern int DAT_00512c28;
extern int DAT_00512c2c;
extern int DAT_00512c30;
extern int DAT_00512c38;
extern int DAT_00512c3c;
extern int DAT_00512c40;
extern int DAT_00512c44;
extern int DAT_00512c48;
extern int DAT_00512c4c;
extern int DAT_00512c50;
extern int DAT_00512c54;
extern int DAT_00512c58;
extern int DAT_00512c5c;
extern int DAT_00512c60;
extern int DAT_00512c64;
extern int DAT_00512c68;
extern int DAT_00512c70;

int __stdcall DefaultPacketHandler(int arg1);
int __stdcall FUN_0044fd50(int arg1);
int __stdcall FUN_0044fd60(int arg1);
int __stdcall FUN_0044fd70(int arg1);
int __stdcall FUN_0044fd80(int arg1);
int __stdcall FUN_0044fd90(int arg1);
int GetTicks(void);
void* __cdecl FUN_004d83b0(char* tag, int size);
int __stdcall InitPacketTables(Class_00451fd0* param_1);
void __stdcall EnumPlayersCallback(int id, int unused1, int unused2, int unused3, int unused4);
int __stdcall JoinLobbyGame(Player* p);
void __stdcall JoinLobbyGameThread(Args_00451640* args);
unsigned __stdcall JoinLobbyGameThread(void* args);
int __stdcall SendPacketToPlayer(int from, int to, void* packet, int size);
int __stdcall BroadcastPacket(int player, void* data, int size);
unsigned char __stdcall FindSlotByDpid(int id);
int __stdcall GetSlotDpid(unsigned char index);
Player* __stdcall FindPlayerByDpid(int id);
int __stdcall AssignPlayerColor(int from, int to, int group);
int __stdcall IsColorFree(int id, int slot);
int __stdcall RequestPlayerColor(int param);
int __stdcall SetAlliance(int from, int to, unsigned char value, int extra);
int __stdcall FUN_00452bd0(Player* player);
void __stdcall RemovePlayer(int id);
int __stdcall RejectPlayer(int id, unsigned char value);
int __stdcall ReceiveNetPacket(void);
void __stdcall OpenTimeoutDialog(int id);
void CheckPlayerTimeouts(void);
void __stdcall HandleTimeoutDialog(void* gadget);
void __stdcall UpdateTimeoutDialog(void* gadget);
int __stdcall HAPINET_updategameinfo(void* obj, char* name, char* data, int d, int c, int b, int a);
void __stdcall HAPINET_uninitmultiplay(void* param_1);
bool IsReporterDllLoaded(void);
void ShutdownScoreTables(void);
char* __stdcall Translate(char* text);
void __stdcall AddMessage(void* text, int param_2, int param_3, unsigned char param_4);
void FUN_00450530(void);
void FUN_00450980(void);
void RebuildAllyList(void);
void __stdcall ReportGameEvent(int param_1);
int __stdcall HAPINET_passwordrequired(void* net);
int IsOnlineConfigLoaded(void);
void __stdcall SetCursorMode(int n);
void __cdecl FUN_004d85a0(void* p);
void* __cdecl FUN_004d84a0(void* param_1, const char* name, unsigned int param_3);
int __stdcall HAPINET_createorjoinlobbygame(void* obj, char* name, int a, int b, int c, int d, int e);
void __stdcall BuildGameInfo(char* name, int* d, int* c, int* b, int* a);
void __stdcall HAPINET_createnewgame(void* obj, char* name, char* data, int d, int c, int b, int a);
int __stdcall AddNetPlayer(int id);
void ResetPlayerSlots(void);
int __stdcall CreateLocalPlayer(int player, int param);
int __stdcall HAPINET_joingame(void* net, Guid_4517b0 guid);
int __stdcall HAPINET_enumplayers(void* net, void* session, void* callback, void* context,
                           unsigned long flags);
Obj_00451640* GetDisplay(void);
void ToggleFullScreen(void);
void __stdcall FatalError(char* msg);
int __stdcall HAPINET_sendpacket(void* net, unsigned long from, unsigned long to, void* data, unsigned long size);
void __stdcall CountMessage(unsigned char kind, int amount, int player);
void __stdcall CountPacket(int size, int overhead, int sent);
int __stdcall HAPINET_receivepacket(void* net, void* data, int* size);
void __stdcall KillPlayerUnits(unsigned char player);
int __stdcall HAPINET_removeplayer(void* net, int id);
void __stdcall HandlePing(void* msg);
void __stdcall CreateUnitFromPacket(unsigned char p, void* packet);
void __stdcall ReceiveUnitStates(Player* player, void* packet);
void __stdcall ApplyAttachUnit(void* packet);
void __stdcall ApplyUnitDamage(void* packet);
void __stdcall ApplyUnitDeath(void* packet, int param_2);
void __stdcall ApplyWeaponFirePacket(Player* player, void* packet);
void __stdcall ApplyProjectileHitPacket(Player* player, void* packet);
void __stdcall KillFeature(int x, int y, int param_3);
void __stdcall StartFeatureBurning(int x, int y, int param_3);
int __stdcall GetMapCell(int x, int y);
void __stdcall DamageFeature(int cell, int x, int y, Feature* feature);
void __stdcall FinishConstruction(Unit* unit, Unit* builder);
void __stdcall PlaySoundByIndex(int index, int param_2);
void __stdcall PlaySoundAt(int index, void* pos, int param_3);
void __stdcall GiveUnitToPlayer(Unit* unit, Player* player, void* packet);
void __stdcall TransferMetal(unsigned char from, unsigned char to, int amount, int param_4);
void __stdcall TransferEnergy(unsigned char from, unsigned char to, int amount, int param_4);
void __stdcall ShareMapInfo(unsigned char from, unsigned char to);
void __stdcall HandlePlayerEconomy(void* packet, Player* player);
void __stdcall SetGameSpeed(int speed, int param_2);
void __stdcall PlaySoundByName(char* name, int param_2);
int __stdcall IsCurrentGadgetNamed(Gadget_004538f0* gadget, char* name);
Entry_004538f0* __stdcall FUN_004a0010(Entry_004538f0* entries, char* name);
int __stdcall SendChatMessage(Player* from, char* text, int param_3, char* to);
int __stdcall FindGadgetIndex(void* gadgets, const char* name, int flag);
void __stdcall FUN_004a7190(void* menu, int index);
void __stdcall FUN_004ab0a0(void* param_1);
void __stdcall FUN_0049fa90(void* menu);
int __stdcall IsScreenNamed(void* obj, const char* name);
void* __stdcall LoadGuiLayer(void* obj, const char* name, int size);
void __stdcall CloseTopScreen(void* obj);
void* __stdcall FindGadgetChecked(void* entries, char* name);
int GetFontLineHeight(void);
void __stdcall FUN_004a32a0(void* obj, char* name, void* p, int count, int flags);
void __stdcall FUN_004a0bf0(void* obj, char* name, void* out, int flag);
void __stdcall FUN_0049fb10(void* obj, int value);
void __stdcall RenderLayer(void* obj, int value);

// The flag is a bit of an unsigned short bitfield in the packed info struct:
// that gives "or byte ptr [m], 1"; an unsigned char or a plain byte "|= 1"
// goes through a register.
// FUNCTION: 0x451540
void CreateNetGame(void)
{
    int a;
    int b;
    int c;
    int d;
    char name[32];

    g_game->players[g_game->localPlayer].info->flag_97_0 = 1;
    BuildGameInfo(name, &d, &c, &b, &a);
    g_game->field_2a3c = 0;
    HAPINET_createnewgame(g_game->field_14, name, DAT_005119b8, d, c, b, a);
}

static inline int PlayerId(unsigned char i)
{
    if (i != 10 && g_game->players[i].flag_73)
        return g_game->players[i].field_4;
    return -1;
}

// FUNCTION: 0x4515d0
void __stdcall EnumPlayersCallback(int id, int unused1, int unused2, int unused3, int unused4)
{
    if (id != -1) {
        for (unsigned char i = 0; i < 10; i++) {
            if (PlayerId(i) == id)
                break;
        }
    }
    AddNetPlayer(id);
}

// FUNCTION: 0x451640
int __stdcall JoinLobbyGame(Player* p)
{
    int result = 0;
    char* name = 0;

    if (HAPINET_passwordrequired((char*)g_game + 0x14) != 0)
        name = (char*)p->info + 0x80;

    int count = 10;
    if (IsOnlineConfigLoaded() != 0 && DAT_00512c8c > 1) {
        unsigned int n = DAT_00512c8c;
        count = n < 10 ? n : 10;
        *(int*)((char*)g_game + 0x4f1) = count;
    }

    Args_00451640* args = (Args_00451640*)FUN_004d83b0("LOBBY JOIN INFO", 0x1c);
    if (args != 0) {
        memset(args, 0, 0x1c);
        args->arg_4 = count;
        args->name = name;
        args->arg_8 = 0;
        args->arg_c = 0;
        args->arg_10 = 0;
        args->arg_14 = 0;
        args->result = 0;

        unsigned int tid = 0;
        SetCursorMode(0x14);
        unsigned long h = _beginthreadex(0, 0x8000, JoinLobbyGameThread, args, 0, &tid);
        if (h != 0) {
            if (WaitForSingleObject((HANDLE)h, 40000) == WAIT_TIMEOUT) {
                SetCursorMode(0x13);
                if (GetDisplay()->flag) {
                    ToggleFullScreen();
                    Sleep(500);
                }
                FatalError(Translate("Timed out while connecting to DirectPlay lobby!"));
            } else {
                result = args->result;
            }
        }

        SetCursorMode(0x13);
        FUN_004d85a0(args);
    }

    return result;
}

// FUNCTION: 0x451770
void __stdcall JoinLobbyGameThread(Args_00451640* args)
{
    args->result = HAPINET_createorjoinlobbygame((char*)g_game + 0x14, args->name, args->arg_4, args->arg_8,
                                args->arg_c, args->arg_10, args->arg_14);
}

// FUNCTION: 0x4517b0
int __stdcall JoinNetGame(Guid_4517b0 guid, int player)
{
    // The name buffer is never read again and must stay dead; the whole body
    // stays under the if (an early return would move the epilogues).
    char name[0x100];
    Packet_4517b0 packet;
    DWORD size;

    if (player == g_game->localPlayer) {
        Player* p = &g_game->players[player];

        Net_4517b0* net = g_game->field_4e5;
        if (net != 0) {
            int v = IsOnlineConfigLoaded();
            char* s = &DAT_00512d48;
            if (v == 0)
                s = DAT_005119b8;

            Net2_4517b0* n2 = net->field_c;
            if (n2 != 0) {
                char* t = n2->field_8;
                if (t != 0 && *t != 0) {
                    s = t;
                } else {
                    char* t2 = n2->field_c;
                    if (t2 != 0 && *t2 != 0)
                        s = t2;
                }
            }

            if (*s != 0) {
                g_game->field_2bd2[0] = 0;
                strncat(g_game->field_2bd2, s, 0x10);
            }

            if (v != 0 && DAT_00512d28 != 0) {
                lstrcpynA(p->info->name, &DAT_00512d28, 0xb);
                // field_9d must be a 16-bit 1-bit field, not a byte field.
                p->info->field_9d = 1;
                // Cast on a reloaded field_4e5, not a cached local.
                if ((*(unsigned char*)((char*)g_game->field_4e5 + 4) & 2) != 0)
                    lstrcpynA(g_game->field_2be3, &DAT_00512d28, 0xb);
            }

            ResetPlayerSlots();
        }

        if (strlen(g_game->field_2bd2) == 0) {
            size = 0x100;
            GetUserNameA(name, &size);
        } else {
            strcpy(name, g_game->field_2bd2);
        }

        // ready must be a 1-bit field.
        if (g_game->field_4e5 != 0) {
            p->info->ready = g_game->field_4e5->field_4 >> 1;
        } else {
            p->info->ready = 0;
        }

        p->info->field_96 = 0xff;
        p->info->field_9b &= 0xffdf;
        p->info->field_8b = g_game->field_37f1b;
        p->info->field_8d = g_game->field_37f1f;
        g_game->field_2a3c = 0;

        int result;
        if (g_game->field_4e5 != 0) {
            result = JoinLobbyGame(p);
            if (result == 0) {
                if (GetDisplay()->flag) {
                    ToggleFullScreen();
                    Sleep(500);
                }
                FatalError(Translate("Unable to connect to DirectPlay lobby."));
            }
        } else {
            result = HAPINET_joingame((Net_4517b0*)((char*)g_game + 0x14), guid);
        }
        if (result == 0)
            return 0;

        CreateLocalPlayer(player, 1);
        HAPINET_enumplayers((Net_4517b0*)((char*)g_game + 0x14), 0, (void*)EnumPlayersCallback, 0, 0);
        if (g_game->flags_2a44 & 1) {
            for (int i = 0; i < 10; i++) {
                Player* q = &g_game->players[i];
                if (q->active != 0 && (q->state == 1 || q->state == 2)) {
                    packet.data = *q->info;
                    packet.data.id = q->id;
                    packet.type = 0x20;
                    BroadcastPacket(q->id, &packet, sizeof(packet));
                    if (q->active != 0 && (q->state == 1 || q->state == 2)) {
                        unsigned char* msg = g_game->buffer;
                        msg[0] = 0x24;
                        *(int*)(msg + 1) = q->id;
                        msg[5] = q->field_13f;
                        BroadcastPacket(q->id, msg, 6);
                        if (g_usePacketManager != 0)
                            g_packetManager.SendAllQueued(1);
                    }
                }
            }
            FUN_00450530();
            g_packetManager.SendAllQueued(1);
        }

        RequestPlayerColor(0);
        return 1;
    }
    return 0;
}

// FUNCTION: 0x451b60
void RemoveLocalPlayers()
{
    if (!(g_game->flags_2a44_w & 1))
        return;
    for (int i = 0; i < 10; i++) {
        if (g_game->players[i].active != 0
            && (g_game->players[i].type == 1 || g_game->players[i].type == 2)) {
            RemovePlayer(g_game->players[i].field_4);
        }
    }
    ShutdownScoreTables();
}

static inline unsigned char FindIndex_00451bc0(int id)
{
    for (unsigned char i = 0; i < 10; i++) {
        if (GetSlotDpid(i) == id)
            return i;
    }
    return 10;
}

static inline int GetPlayerId_00451bc0(unsigned char i)
{
    if (i != 10 && g_game->players[i].state != 0)
        return g_game->players[i].id;
    return -1;
}

static inline unsigned char FindPlayerIndex_00451bc0(int id)
{
    if (id != -1) {
        for (unsigned char i = 0; i < 10; i++) {
            if (GetPlayerId_00451bc0(i) == id)
                return i;
        }
    }
    return 10;
}

// FUNCTION: 0x451bc0
int __stdcall SendPacketToPlayer(int from, int to, unsigned char* packet, int size)
{
    // The id != -1 guard stays in the caller, not in the lookup helper.
    unsigned char fi;
    if (from == -1)
        fi = 10;
    else
        fi = FindIndex_00451bc0(from);
    Player* fromPlayer;
    if (fi == 10)
        fromPlayer = 0;
    else
        fromPlayer = &g_game->players[FindSlotByDpid(from)];

    unsigned char ti;
    if (to == -1)
        ti = 10;
    else
        ti = FindIndex_00451bc0(to);
    Player* toPlayer;
    if (ti == 10)
        toPlayer = 0;
    else
        toPlayer = &g_game->players[FindSlotByDpid(to)];

    if ((g_game->flags_2a44 & 1) && fromPlayer != 0 && fromPlayer->active != 0 &&
        (fromPlayer->state == 1 || fromPlayer->state == 2) &&
        fromPlayer->field_22 == 0 && toPlayer != 0 && toPlayer->active != 0 &&
        toPlayer->state == 3 && toPlayer->field_22 == 0) {
        Player* target = &g_game->players[FindPlayerIndex_00451bc0(to)];
        if (target->active == 0 || (target->state != 1 && target->state != 2)) {
            if (g_usePacketManager != 0)
                return g_packetManager.QueuePacket(from, to, packet, size);
            if (HAPINET_sendpacket((char*)g_game + 0x14, from, to, packet, size) != 0)
                return 0;
            CountMessage(packet[0], size, 1);
            CountPacket(size, 0, 1);
        }
        return 1;
    }
    return 0;
}

static inline unsigned char FindPlayerIndex_00451df0(int id)
{
    if (id != -1) {
        for (unsigned char i = 0; i < 10; i++) {
            if (GetSlotDpid(i) == id)
                return i;
        }
    }
    return 10;
}

static inline Player* FindPlayer(int id)
{
    if (FindPlayerIndex_00451df0(id) == 10)
        return 0;
    // Searched twice: gives the two copies of the id loop.
    return &g_game->players[FindPlayerIndex_00451df0(id)];
}

// FUNCTION: 0x451df0
int __stdcall BroadcastPacket(int id, unsigned char* packet, int size)
{
    Player* p = FindPlayer(id);
    if (p == 0 || p->active == 0)
        return 0;
    if (p->state != 1 && p->state != 2)
        return 0;
    if (p->field_22 != 0)
        return 0;
    if ((g_game->flags_2a44 & 1) != 0) {
        if (g_game->field_299c == 0) {
            if (g_usePacketManager != 0)
                return g_packetManager.QueueOnChannel(id, &DAT_00513008, (int)packet, size);
            if (HAPINET_sendpacket((char*)g_game + 0x14, id, 0, packet, size) != 0)
                return 0;
            CountMessage(packet[0], size, 1);
            CountPacket(size, 0, 1);
            return 1;
        }
        memset(DAT_00512b90, 0, 0x2c);
        for (int i = 0; i < 10; i++) {
            if (g_game->players[i].active == 0)
                continue;
            if (g_game->players[i].state != 3)
                continue;
            if (DAT_00512b90[g_game->players[i].field_c] != 0)
                continue;
            SendPacketToPlayer(id, g_game->players[i].field_4, packet, size);
            int c = g_game->players[i].field_c;
            if (c >= 0 && c < 10)
                DAT_00512b90[c] = 1;
        }
    }
    // Single shared return: makes the flags_2a44 test a forward je.
    return 1;
}

// FUNCTION: 0x451fd0
int __stdcall InitPacketTables(Class_00451fd0* param_1)
{
    DAT_00512be4 = 4;
    DAT_00512c70 = 4;
    DAT_00512be8 = 4;
    DAT_00512bec = 4;
    DAT_00512bf0 = 4;
    DAT_00512bf4 = 4;
    DAT_00512bf8 = 4;
    DAT_00512bfc = 4;
    DAT_00512c00 = 4;
    // After nine of the stores of 4: keeps 4 live across the zeroing.
    memset(&DAT_00512adc, 0, 0xb0);
    DAT_00512b1c = 4;
    DAT_00512c04 = 4;
    DAT_00512c08 = 4;
    DAT_00512c10 = 4;
    DAT_00512ae0 = 13;
    DAT_00512a28 = FUN_0044fd50;
    DAT_00512bc8 = 7;
    DAT_00512aec = 65;
    DAT_00512a34 = FUN_0044fd60;
    DAT_00512bd4 = 7;
    DAT_00512af0 = 1;
    DAT_00512a38 = FUN_0044fd70;
    DAT_00512bd8 = 7;
    DAT_00512af4 = 1;
    DAT_00512a3c = FUN_0044fd80;
    DAT_00512bdc = 7;
    DAT_00512af8 = 1;
    DAT_00512a40 = FUN_0044fd90;
    DAT_00512be0 = 7;
    DAT_00512afc = 23;
    DAT_00512a44 = DefaultPacketHandler;
    DAT_00512b88 = 3;
    DAT_00512ad0 = DefaultPacketHandler;
    DAT_00512b00 = 7;
    DAT_00512a48 = DefaultPacketHandler;
    DAT_00512b04 = 9;
    DAT_00512a4c = DefaultPacketHandler;
    DAT_00512b08 = 11;
    DAT_00512a50 = DefaultPacketHandler;
    DAT_00512b0c = 36;
    DAT_00512a54 = DefaultPacketHandler;
    DAT_00512b10 = 14;
    DAT_00512a58 = DefaultPacketHandler;
    DAT_00512b14 = 6;
    DAT_00512a5c = DefaultPacketHandler;
    DAT_00512b18 = 22;
    DAT_00512a60 = DefaultPacketHandler;
    DAT_00512a64 = DefaultPacketHandler;
    DAT_00512b20 = 5;
    DAT_00512a68 = DefaultPacketHandler;
    DAT_00512b24 = 18;
    DAT_00512a6c = DefaultPacketHandler;
    DAT_00512c0c = 7;
    DAT_00512b28 = 24;
    DAT_00512a70 = DefaultPacketHandler;
    DAT_00512b2c = 1;
    DAT_00512a74 = DefaultPacketHandler;
    DAT_00512c14 = 6;
    DAT_00512b30 = 17;
    DAT_00512a78 = DefaultPacketHandler;
    DAT_00512c18 = 4;
    DAT_00512b34 = 2;
    DAT_00512a7c = DefaultPacketHandler;
    DAT_00512c1c = 7;
    DAT_00512b38 = 2;
    DAT_00512a80 = DefaultPacketHandler;
    DAT_00512c20 = 7;
    DAT_00512b3c = 3;
    DAT_00512a84 = DefaultPacketHandler;
    DAT_00512c24 = 7;
    DAT_00512b44 = 6;
    DAT_00512a8c = DefaultPacketHandler;
    DAT_00512c2c = 7;
    DAT_00512b48 = 5;
    DAT_00512a90 = DefaultPacketHandler;
    DAT_00512c30 = 7;
    DAT_00512b40 = 14;
    DAT_00512a88 = DefaultPacketHandler;
    DAT_00512c28 = 1;
    DAT_00512b4c = 9;
    DAT_00512a94 = DefaultPacketHandler;
    DAT_00512b6c = 5;
    DAT_00512ab4 = DefaultPacketHandler;
    DAT_00512c54 = 1;
    DAT_00512b70 = 41;
    DAT_00512ab8 = DefaultPacketHandler;
    DAT_00512c58 = 7;
    DAT_00512b64 = 14;
    DAT_00512aac = DefaultPacketHandler;
    DAT_00512c4c = 7;
    DAT_00512b68 = 6;
    DAT_00512ab0 = DefaultPacketHandler;
    DAT_00512c50 = 7;
    DAT_00512b50 = 2;
    DAT_00512a98 = DefaultPacketHandler;
    DAT_00512c38 = 6;
    DAT_00512b54 = 5;
    DAT_00512a9c = DefaultPacketHandler;
    DAT_00512c3c = 6;
    DAT_00512b58 = 186;
    DAT_00512aa0 = DefaultPacketHandler;
    DAT_00512c40 = 7;
    DAT_00512b5c = 10;
    DAT_00512aa4 = DefaultPacketHandler;
    DAT_00512c44 = 1;
    DAT_00512b60 = 6;
    DAT_00512aa8 = DefaultPacketHandler;
    DAT_00512c48 = 1;
    DAT_00512b74 = 17;
    DAT_00512abc = DefaultPacketHandler;
    DAT_00512c5c = 7;
    DAT_00512b78 = 58;
    DAT_00512ac0 = DefaultPacketHandler;
    DAT_00512c60 = 7;
    DAT_00512b7c = 3;
    DAT_00512ac4 = DefaultPacketHandler;
    DAT_00512c64 = 7;
    DAT_00512ae4 = 3;
    DAT_00512a2c = DefaultPacketHandler;
    DAT_00512bcc = 7;
    DAT_00512b80 = 2;
    DAT_00512ac8 = DefaultPacketHandler;
    DAT_00512c68 = 7;
    param_1->field_870 = GetTicks();
    param_1->field_1745 = 0x2000;
    param_1->field_1749 = (int*)FUN_004d83b0(s_PACKET_DATA_00506524, 0x2000);
    return 0 != param_1->field_1749;
}

// FUNCTION: 0x452370
void __stdcall ReleasePacketData(Class_00451fd0* obj)
{
    if (obj->buffer) {
        FUN_004d85a0(obj->buffer);
        obj->buffer = 0;
    }
    if (g_game->flags_2a44 & 1) {
        if (g_usePacketManager != 0) {
            g_packetManager.SendAllQueued(1);
        }
        if (!IsReporterDllLoaded()) {
            HAPINET_uninitmultiplay(g_game->field_14);
        }
        g_game->flags_2a44_w &= 0xfffe;
    }
}

static inline int GetPlayerId_004523e0(unsigned char i)
{
    if (i != 10 && g_game->players[i].state)
        return g_game->players[i].id;
    return -1;
}

// Early return 10 for id -1, used straight as the index: two separate stores of 10.
static inline unsigned char FindPlayerIndex_004523e0(int id)
{
    if (id == -1)
        return 10;
    for (unsigned char i = 0; i < 10; i++) {
        if (GetPlayerId_004523e0(i) == id)
            return i;
    }
    return 10;
}

// FUNCTION: 0x4523e0
int __stdcall AssignPlayerColor(int from, int to, int group)
{
    unsigned char packet[4];
    for (int i = 0; i < 10; i++, group++) {
        if (group >= 10)
            group = 0;
        int j;
        for (j = 0; j < 10; j++) {
            Player* p = &g_game->players[j];
            if (p->state != 0 && p->state != 4 && p->id != to && p->data->group == group)
                break;
        }
        if (j == 10) {
            packet[3] = group;
            break;
        }
    }
    if (to == g_game->players[g_game->localPlayer].id) {
        g_game->players[g_game->localPlayer].data->group = group;
        return 1;
    }
    packet[2] = 0x18;
    int result = SendPacketToPlayer(from, to, packet + 2, 2);
    if (result != 0 && g_usePacketManager != 0) {
        g_game->players[FindPlayerIndex_004523e0(to)].data->group = group;
        g_packetManager.SendAllQueued(1);
    }
    return result;
}

// Returns the index of the first in-use entry whose id matches, else 10.
static __inline unsigned char FindSlot_00452570(int id)
{
    unsigned char i;
    for (i = 0; i < 10; i++) {
        int v;
        // Redundant guard: kept as in the original.
        if (i == 10) {
            v = -1;
        } else {
            v = g_game->players[i].valid ? g_game->players[i].id : -1;
        }
        if (v == id) {
            return i;
        }
    }
    return 10;
}

// FUNCTION: 0x452570
int __stdcall IsColorFree(int id, int slot)
{
    if (slot == 0xff || slot < 0 || slot >= 10) {
        return 0;
    }

    unsigned char local_c[10];
    memset(local_c, -1, 10);

    unsigned char found;
    if (id == -1) {
        found = 10;
    } else {
        found = FindSlot_00452570(id);
    }
    // Second search, result discarded: kept as in the original.
    if (found != 10 && id != -1) {
        FindSlot_00452570(id);
    }
    for (int j = 0; j < 10; j++) {
        if (g_game->players[j].valid != 0 && g_game->players[j].id != id &&
            g_game->players[j].info->field_96 != 0xff) {
            local_c[g_game->players[j].info->field_96] = (unsigned char)j;
        }
    }
    return local_c[slot] == 0xff;
}

static inline unsigned char FindReadyPlayer()
{
    // Tests the fields directly, not through a per-index helper.
    for (unsigned char i = 0; i < 10; i++) {
        if (g_game->players[i].flag_73 && g_game->players[i].info->ready)
            return i;
    }
    return 10;
}

// FUNCTION: 0x4526c0
int __stdcall RequestPlayerColor(int param)
{
    Player* p = &g_game->players[g_game->localPlayer];
    // Widened into an int; localPlayer re-read from g_game, not cached.
    int i = FindReadyPlayer();
    if (i == g_game->localPlayer) {
        if (IsColorFree(p->field_4, param) == 0) {
            AssignPlayerColor(p->field_4, p->field_4, param);
            return 1;
        }
        p->info->field_96 = param;
        return 1;
    }

    unsigned char* buffer = g_game->buffer;
    buffer[0] = 0x17;
    buffer[1] = (unsigned char)param;

    int result;
    if (i == 10)
        result = BroadcastPacket(p->field_4, buffer, 2);
    else
        result = SendPacketToPlayer(p->field_4, g_game->players[i].field_4, buffer, 2);

    if (g_usePacketManager != 0)
        g_packetManager.SendAllQueued(1);
    return result;
}

static inline int PlayerId_00452800(unsigned char i)
{
    if (i != 10 && g_game->players[i].flag_73)
        return g_game->players[i].field_4;
    return -1;
}

static unsigned char LookupPlayer(int id)
{
    if (id == -1)
        return 10;
    unsigned char i;
    for (i = 0; i < 10; i++) {
        if (PlayerId_00452800(i) == id)
            return i;
    }
    return 10;
}

// FUNCTION: 0x452800
int __stdcall FUN_00452800(int id)
{
    if (LookupPlayer(id) == 10)
        return 0;
    unsigned char* msg = g_game->buffer;
    msg[0] = 0x1c;
    *(int*)(msg + 1) = id;
    unsigned char index = LookupPlayer(id);
    Player* player = &g_game->players[index];
    if ((player->field_0 != 0 && player->flag_73 == 3)
        || !(g_game->flags_38d75 & 1)
        || (g_game->flags_38d75 & 2)) {
        RemovePlayer(id);
    }
    return BroadcastPacket(g_game->players[g_game->field_2a42].field_4, msg, 5);
}

// FUNCTION: 0x452b70
int __stdcall SendAlliance(int from, int to, char value, int extra)
{
    Packet_00452b70* msg = (Packet_00452b70*)g_game->buffer;
    msg->value = value;
    msg->type = 0x23;
    msg->from = from;
    msg->to = to;
    msg->extra = extra;
    int result = SendPacketToPlayer(from, to, msg, 0xe);
    if (g_usePacketManager != 0) {
        g_packetManager.SendAllQueued(1);
    }
    return result;
}

// An inlined helper with one return per outcome: written as a plain
// condition, MSVC moves the `return 0` path after the body.
static inline int IsPlaying_00452bd0(Player* player)
{
    if (player->active == 0)
        return 0;
    if (player->state == 1 || player->state == 2)
        return 1;
    return 0;
}

// The original calls this out of line from 0x453d40.
#pragma auto_inline(off)
// FUNCTION: 0x452bd0
int __stdcall FUN_00452bd0(Player* player)
{
    if (!IsPlaying_00452bd0(player)) {
        return 0;
    }
    unsigned char* msg = g_game->buffer;
    msg[0] = 0x24;
    *(int*)(msg + 1) = player->id;
    msg[5] = player->field_13f;
    int result = BroadcastPacket(player->id, msg, 6);
    if (g_usePacketManager != 0) {
        g_packetManager.SendAllQueued(1);
    }
    return result;
}

#pragma auto_inline(on)

// FUNCTION: 0x452c40
char* __stdcall GetRejectReasonText(int reason)
{
    switch (reason) {
    case 4:
        return "You did not have the correct password";
    case 3:
        return "The game is closed";
    case 5:
        return "The game is full";
    case 6:
        return "You have lost connection with the game";
    case 7:
        return "You need a unit you don't have for this game";
    case 8:
        return "You need a newer version of the game to enter";
    case 9:
        return "No watching is allowed for this game";
    case 10:
        return "The creator has left the game";
    default:
        return "You were rejected from the game";
    }
}

static inline unsigned char FindIndex_00453010(int id)
{
    for (unsigned char i = 0; i < 10; i++) {
        if (GetSlotDpid(i) == id)
            return i;
    }
    return 10;
}

static inline int FindActiveId_00453010()
{
    int i = 0;
    while (1) {
        if (g_game->players[i].active != 0
            && (g_game->players[i].state == 1 || g_game->players[i].state == 2))
            return g_game->players[i].id;
        i++;
        if (i >= 10)
            return -1;
    }
}
// FUNCTION: 0x453010
int __stdcall RejectPlayer(int id, unsigned char value)
{
    int result = 0;

    unsigned char fi;
    if (id == -1)
        fi = 10;
    else
        fi = FindIndex_00453010(id);

    Player* p;
    if (fi == 10)
        p = 0;
    else
        p = &g_game->players[FindSlotByDpid(id)];

    if (p == 0)
        return 0;

    unsigned char* msg = g_game->buffer;
    msg[0] = 0x1b;
    *(int*)(msg + 1) = -1;
    msg[5] = value;

    if (p->active != 0
        && (p->state == 1 || p->state == 2)
        && p->field_22 == 0) {
        if (p->state == 1) {
            for (int i = 0; i < 10; i++) {
                if (g_game->players[i].active != 0
                    && (g_game->players[i].state == 1 || g_game->players[i].state == 2)) {
                    *(int*)(msg + 1) = g_game->players[i].id;
                    BroadcastPacket(FindActiveId_00453010(), msg, 6);
                    RemovePlayer(p->id);
                    g_game->players[i].field_22 = value;
                }
            }
            result = 1;
        } else {
            *(int*)(msg + 1) = p->id;
            BroadcastPacket(FindActiveId_00453010(), msg, 6);
            RemovePlayer(p->id);
            result = 1;
        }
    } else if (p->active != 0 && p->state == 3 && p->field_22 == 0) {
        *(int*)(msg + 1) = id;
        result = BroadcastPacket(FindActiveId_00453010(), msg, 6);
        if (p->active != 0 && p->state == 3 && p->field_27->field_94 == 1) {
            unsigned char c = p->field_c;
            for (int i = 0; i < 10; i++) {
                if (g_game->players[i].field_c == c) {
                    RemovePlayer(g_game->players[i].id);
                    g_game->players[i].field_22 = value;
                }
            }
        } else {
            RemovePlayer(p->id);
        }
    }

    p->field_22 = value;
    return result;
}

// FUNCTION: 0x453320
void __stdcall FUN_00453320(int from, int to)
{
    unsigned char* buf = g_game->buffer;
    *buf = 6;
    if (to == 0) {
        BroadcastPacket(from, buf, 1);
    } else {
        SendPacketToPlayer(from, to, buf, 1);
    }
}

// Declared __stdcall although it takes no arguments: places the size reload late.
// FUNCTION: 0x4534e0
int __stdcall ReceiveNetPacket(void)
{
    int size;

    if (!(g_game->flags_2a44 & 1))
        return 0;

    size = g_game->field_2a34;
    if (g_usePacketManager != 0) {
        while (1) {
            int result = DAT_0051e300.ReceiveFrame((char*)g_game + 0x14, g_game->buffer, &size);
            if (result == 0) {
                CountMessage(*g_game->buffer, size, 0);
                return 1;
            }
            if (result == 0x887700be)
                return 0;
            if (result != 0x8877001e)
                return 0;
            g_game->field_2a34 = size;
            g_game->buffer = (unsigned char*)FUN_004d84a0(g_game->buffer, "PACKET DATA AGAIN", size);
        }
    } else {
        while (1) {
            int result = HAPINET_receivepacket((char*)g_game + 0x14, g_game->buffer, &size);
            if (result == 0) {
                CountMessage(*g_game->buffer, size, 0);
                CountPacket(size, 0, 0);
                return 1;
            }
            if (result == 0x887700be)
                return 0;
            if (result != 0x8877001e)
                return 0;
            g_game->field_2a34 = size;
            g_game->buffer = (unsigned char*)FUN_004d84a0(g_game->buffer, "PACKET DATA AGAIN", size);
        }
    }
}

static inline int GetPlayerId_00453640(unsigned char i)
{
    if (i != 10 && g_game->players[i].state)
        return g_game->players[i].id;
    return -1;
}

static inline unsigned char FindPlayerIndex_00453640(int id)
{
    if (id == -1)
        return 10;
    for (unsigned char i = 0; i < 10; i++) {
        if (GetPlayerId_00453640(i) == id)
            return i;
    }
    return 10;
}

// FUNCTION: 0x453640
void UpdateTimeoutDialog()
{
    Entry_00453640* entry = (Entry_00453640*)FindGadgetChecked(g_game->holder->entries, "OUTPUT");

    if (g_loungeRefreshTime < GetTicks()) {
        g_loungeRefreshTime = GetTicks() + 2;
        FUN_0049fa90(g_game->message);
    }

    memset(g_loungeChatter, 0, 0xa00);

    int tail = g_game->tail;
    int count = entry->count;
    int idx = tail;
    for (int n = 1; n < count - 1; n++) {
        if (idx == g_game->head)
            break;
        idx--;
        if (idx < 0)
            idx = 29;
    }

    char* p = g_loungeChatter;
    if (idx != tail) {
        do {
            strcpy(p, g_game->ring[idx].text);
            p += strlen(g_game->ring[idx].text) + 1;
            idx++;
            if (idx >= 30)
                idx = 0;
        } while (idx != g_game->tail);
    }

    // The original searches for the player twice: once for the == 10 test and
    // again for the index used to fetch the record. Keep it; the bytes do this.
    unsigned char found = FindPlayerIndex_00453640(g_timeoutPlayerDpid);
    Player* player;
    if (found == 10)
        player = 0;
    else
        player = &g_game->players[FindPlayerIndex_00453640(g_timeoutPlayerDpid)];

    if (player != 0 && player->active != 0 && player->state == 3) {
        int elapsed = (GetTicks() - player->lastHeard) / 30;
        char buf[200];
        sprintf(buf, Translate("will be rejected in %d seconds"),
                g_game->field_37f31 - elapsed + 0x78);
        FUN_004a0bf0(g_game->message, "TIMETEXT", buf, 0);
        FUN_0049fa90(g_game->message);
        if (elapsed < g_game->field_37f31 + 0x78)
            return;
        RejectPlayer(g_timeoutPlayerDpid, 6);
        CloseTopScreen(g_game->message);
    }
}

// FUNCTION: 0x4538f0
void __stdcall HandleTimeoutDialog(Gadget_004538f0* gadget)
{
    Entry_004538f0* entries = gadget->layer->entries;

    if (gadget->field_60 == -1) {
        PlaySoundByName("Previous", 0);
        if (g_loungeChatter)
            FUN_004d85a0(g_loungeChatter);
        g_loungeChatter = 0;
        return;
    }
    if (IsCurrentGadgetNamed(gadget, "TALK")) {
        Entry_004538f0* entry = FUN_004a0010(entries, "TALK");
        if (strlen(entry->text)) {
            SendChatMessage(&g_game->players[g_game->localPlayer], entry->text, 4, 0);
            g_game->flag0 = 1;
            strcpy(entry->text, DAT_005119b8);
        }
        FUN_004a7190(g_game->message, FindGadgetIndex(g_game->layer_531->entries, "TALK", 3));
        FUN_004ab0a0(g_game->message);
        FUN_0049fa90(g_game->message);
        return;
    }
    if (IsCurrentGadgetNamed(gadget, "REJECT")) {
        RejectPlayer(g_timeoutPlayerDpid, 6);
        return;
    }
    FUN_004ab0a0(gadget);
}

static __inline unsigned char FindSlot_00453a50(int id)
{
    unsigned char i;
    for (i = 0; i < 10; i++) {
        int v;
        if (i == 10) {
            v = -1;
        } else {
            v = g_game->players[i].valid ? g_game->players[i].id : -1;
        }
        if (v == id) {
            return i;
        }
    }
    return 10;
}

// FUNCTION: 0x453a50
void __stdcall OpenTimeoutDialog(int id)
{
    if (IsScreenNamed((char*)g_game + 0x519, "TIMEOUT.GUI")) {
        if (id == -1) {
            CloseTopScreen((char*)g_game + 0x519);
        }
        return;
    }
    if (id == -1) {
        return;
    }

    int i = FindSlot_00453a50(id);
    if (i == 10) {
        return;
    }

    Gui_00453a50* gui = (Gui_00453a50*)LoadGuiLayer((char*)g_game + 0x519,
                                                    "TIMEOUT.GUI", 0x800);
    void* entries = gui->entries;
    gui->callback = &HandleTimeoutDialog;
    g_timeoutPlayerDpid = id;

    char* p = (char*)FUN_004d83b0("LOUNGE CHATTER", 0xa00);
    g_loungeChatter = p;
    memset(p, 0, 0x780);

    OutEntry_00453a50* out = (OutEntry_00453a50*)FindGadgetChecked(entries, "OUTPUT");
    FUN_004a32a0((char*)g_game + 0x519, "OUTPUT", g_loungeChatter,
                 (int)out->field_19 / (GetFontLineHeight() + 2), 0);

    gui->field_1c = &UpdateTimeoutDialog;
    FUN_004a7190((char*)g_game + 0x519, FindGadgetIndex(entries, "TALK", 3));

    FUN_004a0bf0((char*)g_game + 0x519, "NAME",
                 g_game->players[i].name, 0);
    FUN_0049fb10((char*)g_game + 0x519, 1);
    RenderLayer((char*)g_game + 0x519, 0x40);
}

static inline int IsTimedOut(Player* p, unsigned int now)
{
    if (p->active != 0 && p->state == 3) {
        unsigned int last = p->lastHeard;
        if (g_timeoutTimerStart > last)
            last = g_timeoutTimerStart;
        if (now - last > (unsigned int)(g_game->field_37f31 * 30))
            return 1;
    }
    return 0;
}

// FUNCTION: 0x453c20
void CheckPlayerTimeouts()
{
    if (g_game->field_37f2f & 1)
        return;
    if (g_game->field_38a51 & 1) {
        g_timeoutTimerStart = GetTicks();
        return;
    }
    unsigned int now = GetTicks();
    int mixed = 0;
    int team = -1;
    int i;
    Player* p = g_game->players;
    // Walks a pointer: indexing here would bias the loop pointer to +0xc.
    for (i = 0; i < 10; i++, p++) {
        if (IsTimedOut(p, now)) {
            if (team < 0)
                team = p->team;
            else if (team != p->team)
                mixed = 1;
        }
    }
    for (i = 0; i < 10; i++) {
        Player* q = &g_game->players[i];
        if (IsTimedOut(q, now) && !mixed) {
            OpenTimeoutDialog(q->id);
            return;
        }
    }
    OpenTimeoutDialog(-1);
}

// The original calls this out of line from 0x453d40.
#pragma auto_inline(off)
// FUNCTION: 0x456030
int Class_00456030::FUN_00456030()
{
    if (field_0 != 0 && (field_73 == 1 || field_73 == 2)) {
        return 1;
    }
    return 0;
}

#pragma auto_inline(on)

// FUNCTION: 0x456050
void __stdcall SendNewUnit(Unit* obj)
{
    Packet_00456050 packet;
    packet.type = 9;
    packet.field_1 = obj->field_a6;
    packet.field_3 = obj->field_a8;
    packet.field_5 = obj->field_6a;
    packet.field_11 = obj->field_64;
    BroadcastPacket(obj->player->field_4, &packet, 0x17);
}

// FUNCTION: 0x4560c0
void __stdcall FUN_004560c0(Unit* obj, Unit* target)
{
    Packet_004560c0 packet;
    packet.type = 0x12;
    packet.field_1 = target->field_a8;
    packet.field_3 = obj->field_a8;
    BroadcastPacket(obj->player->field_4, &packet, 5);
}
