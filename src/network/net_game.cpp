// Decompiled by Haiku, Sonnet, Opus, deepseek-v4.1-flash, DeepSeek V4.1 Flash,
// deepseek-v4.1, gpt-6-luna, GPT-6.1-sol, mimo-v2.6-pro, Claude Opus 5.5,
// space-bunny-free, claude-sonnet-5-5, Space Bunny Free, muse-spark-1.3-free,
// claude-opus-5-5, GPT-6 and GPT-6-Luna. Names are provisional.
// The network game module (0x44fd40 to 0x457d30): the player lookups and slot
// handling, player names and info packets, the session setup, the lobby join,
// player colours and alliances, packet dispatch, rejection, timeout and chat
// handling, the script-call packets, heartbeat, ping, load progress, resource
// sharing and the lobby connection. The module's three parts joined in
// address order. The functions that only match with their own file's symbol
// ids stay apart (net_game_450530.cpp, net_game_452960.cpp,
// net_game_452cc0.cpp, net_game_453360.cpp and net_game_453d40.cpp).
//
// <stdio.h> and <stdlib.h> carry 0x450380's sprintf and rand, and <string.h>
// the string copies of 0x450090, 0x450140, 0x450980, 0x451090 and 0x451220.
// <windows.h> is deliberately absent here: 0x450f90's original file needed it
// for its loop's base/index order, but in this file's declaration context it
// matches without it. It arrives with the module's second part, whose files
// included it.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// DirectPlay 3 DPNAME (the toolchain's DirectX 3 <dplay.h> lacks it).
struct DPNAME {
    unsigned long dwSize;
    unsigned long dwFlags;
    char* lpszShortNameA;
    char* lpszLongNameA;
};

typedef void (__stdcall *FuncPtr)(void*);
extern FuncPtr g_packetHandlers[];

class Class_0044fda0 {
public:
    void DispatchPacket();
};

class PacketChannel;

class Mission {
public:
    int GetGameType();
    int GetMissionName();
};

class PacketManager {
public:
    int SendAllQueued(int param_1);
    int QueuePacket(int param_1, int param_2, void* param_3, unsigned int param_4);
    int QueueOnChannel(int param_1, PacketChannel* param_2, int param_3, int param_4);
    void SetDefaultSendPacing(int param);
};

class CobScript {
public:
    int StartScriptWithArgsByIndex(int, int, int, int, int, int, int, int);
    int FindScript(char* name);
};

class MissionConditions {
public:
    int CheckVictory();
};

struct Class_00456030 {
    int field_0;                       // +0x0
    char unknown_4[0x73 - 0x4];
    char field_73;                     // +0x73
    int IsPlayableSlot();
};

#pragma pack(push, 1)

struct Settings {
    int field_471;                     // +0x471
    union {
        int flags_475;                 // +0x475
        struct {
            unsigned int unknown_475_0 : 5;
            unsigned int flag_475_5 : 1;
            unsigned int unknown_475_6 : 26;
        } bits_475;
    };
    char unknown_479[0x48];
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

struct Feature {
    char data[0x115];
};

struct Holder_00453640;
struct Layer_004538f0;
struct Net_4517b0;
class Class_0046d500;

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

// The info block a player record points at (+0x27). One type for every view:
// where two views name the same bytes differently the union carries both
// names, and the bitfields carry every name the views give a bit.
struct PlayerInfo {
    char unknown_0[0x80];
    char name[0xb];                    // +0x80
    unsigned short field_8b;           // +0x8b
    unsigned short field_8d;           // +0x8d
    char unknown_8f;
    union {
        int field_90;                  // +0x90
        int id;
    };
    unsigned char field_94;            // +0x94
    char unknown_95;
    union {
        unsigned char field_96;        // +0x96
        unsigned char group;
    };
    union {
        unsigned char flags_97;        // +0x97
        unsigned char flags;
        unsigned short flag_97_0 : 1;
        unsigned short ready : 1;
        struct {
            unsigned short b0 : 1;
            unsigned short b1 : 1;
            unsigned short b2 : 1;
            unsigned short b3 : 1;
            unsigned short b4 : 1;
            unsigned short b5 : 1;
            unsigned short b6 : 1;
            unsigned short b7 : 1;
            unsigned short b8 : 1;
            unsigned short b9 : 1;
            unsigned short b10 : 1;
            unsigned short b11 : 1;
            unsigned short b12 : 1;
            unsigned short b13 : 1;
            unsigned short b14 : 1;
            unsigned short b15 : 1;
        } bits_97;
    };
    char unknown_99[0x9b - 0x99];
    union {
        unsigned short field_9b;       // +0x9b
        unsigned char flags_9b;
        struct {
            unsigned short : 4;
            unsigned short flag_9b_4 : 1;
            unsigned short : 1;
            unsigned short flag6 : 1;
            unsigned short : 7;
            unsigned short flag14 : 1;
        } bits_9b;
    };
    union {
        unsigned char flags_9d;        // +0x9d
        unsigned short word_9d;
        unsigned short field_9d : 1;
        unsigned short flag_9d_0 : 1;
    };
    char unknown_9f[0xa5 - 0x9f];
    unsigned short field_a5;           // +0xa5
    unsigned char field_a7;            // +0xa7
    unsigned char field_a8;            // +0xa8
    char unknown_a9[0xb9 - 0xa9];
};

// A player record at g_game+0x1b63 (0x14b bytes). One type for every view:
// where two views name the same bytes differently the union carries both
// names.
struct Player {
    union {
        int active;                    // +0x00
        int field_0;
    };
    union {
        int id;                        // +0x04
        int dpid;
        int field_4;
    };
    int field_8;                       // +0x08
    union {
        int field_c;                   // +0x0c
        int team;
    };
    int messages;                      // +0x10
    int field_14;                      // +0x14
    char unknown_18[0x1c - 0x18];
    union {
        int field_1c;                  // +0x1c
        int lastHeard;
    };
    union {
        unsigned char field_20;        // +0x20
        unsigned char progress;
    };
    union {
        unsigned char field_21;        // +0x21
        unsigned char flags_21;
    };
    union {
        unsigned char field_22;        // +0x22
        unsigned char reason;
        unsigned char rejectReason;
    };
    char unknown_23[0x27 - 0x23];
    union {
        PlayerInfo* info;              // +0x27
        PlayerInfo* data;
        PlayerInfo* field_27;
    };
    char name[0x1e];                   // +0x2b
    char fullName[0x2a];               // +0x49
    union {
        char state;                    // +0x73
        unsigned char type;
        char flag_73;
        unsigned char valid;
    };
    char unknown_74[0x8c - 0x74];
    union {
        int field_8c;                  // +0x8c
        float metal;
    };
    char unknown_90[0x98 - 0x90];
    union {
        int field_98;                  // +0x98
        float energy;
    };
    char unknown_9c[0xa4 - 0x9c];
    union {
        int field_a4;                  // +0xa4
        float field_a4f;
    };
    union {
        int field_a8;                  // +0xa8
        float field_a8f;
    };
    double field_ac;                   // +0xac
    double field_b4;                   // +0xb4
    double field_bc;                   // +0xbc
    double field_c4;                   // +0xc4
    double field_cc;                   // +0xcc
    double field_d4;                   // +0xd4
    char unknown_dc[0xe4 - 0xdc];
    float field_e4;                    // +0xe4
    float field_e8;                    // +0xe8
    char unknown_ec[0xfc - 0xec];
    short field_fc;                    // +0xfc
    short field_fe;                    // +0xfe
    char unknown_100[0x104 - 0x100];
    short field_104;                   // +0x104
    short field_106;                   // +0x106
    unsigned char field_108[0x16];     // +0x108
    unsigned char t0[11];              // +0x11e
    unsigned char t1[11];              // +0x129
    unsigned char t2[11];              // +0x134
    union {
        unsigned char field_13f;       // +0x13f
        char alliance;
    };
    int field_140;                     // +0x140
    short field_144;                   // +0x144
    union {
        unsigned char field_146;       // +0x146
        unsigned char index;
    };
    unsigned char field_147;           // +0x147
    char unknown_148[0x14b - 0x148];
    void SetType(int value);
};

// A unit: the player it belongs to, its script and the fields the network
// game reads. One type for the views of the part files.
struct Unit {
    char unknown_0[0x64];
    Short3_00456050 rot;          // +0x64
    Vec3_00456050 pos;            // +0x6a
    char unknown_76[0x96 - 0x76];
    Player* player;                    // +0x96
    CobScript* names;                  // +0x9a
    char unknown_9e[0xa6 - 0x9e];
    short unitDefIndex;                    // +0xa6
    short id;                    // +0xa8
    char unknown_aa[0x110 - 0xaa];
    unsigned int flags_110;            // +0x110
    char unknown_114[4];
    void SetStateBits(unsigned char, int);
};

// The game state, one type for every view. Where the parts read the same
// bytes differently the union carries both names: the session block at +0x14,
// the 0x2a44 flags, the menu at +0x519, the name strings at +0x2bd2, the
// pending lists at +0x2c28, the tick and stage words at +0x38a47 and +0x38d6f
// and the game flags at +0x3923b.
struct Game {
    char unknown_0[1];
    unsigned char field_1;             // +0x01
    unsigned char field_2;             // +0x02
    char unknown_3[0x14 - 3];
    union {
        char net[0x4c9];               // +0x14
        char field_14[0x471 - 0x14];
        struct {
            char unknown_14[0x471 - 0x14];
            Settings settings;         // +0x471
            char unknown_4c1[0x4c9 - 0x4c1];
            union {
                int from_id;           // +0x4c9
                int lobby2;
            };
            union {
                int local_id;          // +0x4cd
                int lobby1;
            };
            char unknown_4d1[0xc];
        };
    };
    char unknown_4dd[0x4e5 - 0x4dd];
    Net_4517b0* field_4e5;             // +0x4e5
    char unknown_4e9[0x4f1 - 0x4e9];
    int netMode;                       // +0x4f1
    char unknown_4f5[0x519 - 0x4f5];
    union {
        char menu[0x1b63 - 0x519];     // +0x519
        struct {
            char message[0x531 - 0x519];
            union {
                Holder_00453640* holder;   // +0x531
                Layer_004538f0* layer_531;
            };
            char unknown_535[0x12ef - 0x535];
            Ring_00453640 ring[30];    // +0x12ef
            int field_1b5f;            // +0x1b5f
        };
    };
    Player players[10];                // +0x1b63
    char unknown_2851[0x299c - 0x2851];
    union {
        int duplicateIds;              // +0x299c
        int field_299c;
    };
    char unknown_29a0[0x29a4 - 0x29a0];
    int field_29a4[11];                // +0x29a4
    int field_29d0[11];                // +0x29d0
    int field_29fc[10];                // +0x29fc
    char unknown_2a24[0x2a28 - 0x2a24];
    int field_2a28;                    // +0x2a28
    char unknown_2a2c[0x2a30 - 0x2a2c];
    Class_0046d500* field_2a30;        // +0x2a30
    int field_2a34;                    // +0x2a34
    union {
        unsigned char* buffer;         // +0x2a38
        unsigned char* packet;
    };
    union {
        unsigned short field_2a3c;     // +0x2a3c
        short field_2a3c_signed;
    };
    unsigned short tail;               // +0x2a3e
    unsigned short head;               // +0x2a40
    union {
        unsigned char localPlayer;     // +0x2a42
        unsigned char field_2a42;
        unsigned char local;
    };
    char unknown_2a43;
    union {
        unsigned char flags_2a44;      // +0x2a44
        unsigned short flags_2a44_w;
        GameFlags_2a44 flags;
        struct {
            unsigned char leaving : 1;
            unsigned char rest_2a44 : 7;
        } bits_2a44;
    };
    char unknown_2a46[0x2bc1 - 0x2a46];
    char gameName[0x10];               // +0x2bc1
    char unknown_2bd1;
    union {
        char nickName[0x11];           // +0x2bd2
        char field_2bd2[0x11];
    };
    union {
        char passWord[0x11];           // +0x2be3
        struct {
            char field_2be3[0x2bee - 0x2be3];
            union {
                unsigned short flag0 : 1;      // +0x2bee
                unsigned short dirty : 1;
                unsigned short bit0_2bee : 1;
            };
            unsigned char chatMode;    // +0x2bf0
            unsigned char field_2bf1[10];  // +0x2bf1
        };
    };
    char unknown_2bfb[0x2c28 - 0x2bfb];
    union {
        char field_2c28[40];           // +0x2c28
        int table_2c28[10];
    };
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
    char unknown_37f35[0x38a47 - 0x37f35];
    union {
        unsigned int ticks;            // +0x38a47
        struct {
            char unknown_38a47[0x38a51 - 0x38a47];
            union {
                unsigned char field_38a51;   // +0x38a51
                unsigned short bit_38a51 : 1;
            };
        };
    };
    char unknown_38a53[0x38d6f - 0x38a53];
    union {
        volatile unsigned char stages[6];    // +0x38d6f
        struct {
            char unknown_38d6f[0x38d75 - 0x38d6f];
            union {
                volatile unsigned char flags_38d75;   // +0x38d75
                volatile unsigned short net_flags;
                struct {
                    unsigned short : 2;
                    unsigned short net_bit2 : 1;
                    unsigned short : 13;
                } net_bits;
            };
        };
    };
    char unknown_38d77[0x391e9 - 0x38d77];
    Mission* campaign;                 // +0x391e9
    MissionConditions* conditions;     // +0x391ed
    int mode;                          // +0x391f1
    char unknown_391f5[0x39211 - 0x391f5];
    char connection[4];                // +0x39211
    char unknown_39215[0x3923b - 0x39215];
    union {
        unsigned short field_3923b;    // +0x3923b
        struct {
            unsigned short bits_3923b : 2;
            unsigned short flag2 : 1;
            unsigned short bit3 : 1;
            unsigned short flag4 : 1;
            unsigned short flag5 : 1;
            unsigned short flag6 : 1;
            unsigned short rest_3923b : 9;
        } bits;
    };
};

struct Packet_00450a10 {
    unsigned char type;                // +0x0
    PlayerInfo data;                   // +0x1
};

struct Packet_00450f90 {
    unsigned char type;                // +0x0
    PlayerInfo data;                   // +0x1
};

#pragma pack(pop)

// GLOBAL: 0x511de8
extern Game* g_game;
extern char* g_leftGameTexts[8];
extern int g_usePacketManager;
extern PacketManager g_packetManager;
extern char DAT_005119b8[];

int __stdcall HAPINET_setplayername(void* net, int dpid, DPNAME* name, int flags);
int __stdcall HAPINET_getplayername(void* net, unsigned long id, void* data, unsigned long* size);
int __stdcall HAPINET_addplayer(void* net, unsigned long* id, char* shortName, char* longName,
                                char* name, short field_11, short field_13);
void __stdcall HAPINET_updategameinfo(void* obj, char* name, char* data, int d, int c, int b, int a);
void __stdcall HAPINET_initmultiplaydefaults(void* net);
void __stdcall HAPINET_initconnection(void* net, void* connection);
int __stdcall HAPINET_quitgame(void* net);
void __stdcall HAPINET_uninitmultiplay(void* param_1);
int __stdcall InitPacketManager(int a, int b);
bool IsReporterDllLoaded();
void __stdcall RemovePlayer(int id);
void ShutdownScoreTables();
void __stdcall SetCloseHandler(void (__cdecl *callback)(int), int param);
void __stdcall QuitApp(char* message);
char* __stdcall Translate(char* text);
void __stdcall AddMessage(char* text, int param_2, int param_3, char param_4);
void SendLobbySyncRequests();
int __stdcall BroadcastPacket(int player, void* data, int size);
void __stdcall SetupPlayerSlot(unsigned char player, char type);
int GetTicks();
void __stdcall ReportGameEvent(int param_1);
void __stdcall OpenMessageBox(void* menu, const char* text, int a, int b, int c);

// FUNCTION: 0x44fd40
int __stdcall DefaultPacketHandler(int arg1)
{
    return 0xffffffff;
}

// FUNCTION: 0x44fd50
int __stdcall FUN_0044fd50(int arg1)
{
    return 0xffffffff;
}

// FUNCTION: 0x44fd60
int __stdcall FUN_0044fd60(int)
{
    return 0xffffffff;
}

// FUNCTION: 0x44fd70
int __stdcall FUN_0044fd70(int arg1)
{
    return 0xffffffff;
}

// FUNCTION: 0x44fd80
int __stdcall FUN_0044fd80(int arg1)
{
    return 0xffffffff;
}

// FUNCTION: 0x44fd90
int __stdcall FUN_0044fd90(int arg1)
{
    return 0xffffffff;
}

// FUNCTION: 0x44fda0
void Class_0044fda0::DispatchPacket()
{
    unsigned int idx = 0;
    idx = *(unsigned char*)this;
    g_packetHandlers[idx](this);
}

// Returns the id of the first active player whose type is 1 or 2, or -1.
// FUNCTION: 0x44fdb0
int GetLocalDpid()
{
    int i = 0;
    while (!(g_game->players[i].active != 0
             && (g_game->players[i].type == 1 || g_game->players[i].type == 2))) {
        if (++i >= 10)
            return -1;
    }
    return g_game->players[i].id;
}

// FUNCTION: 0x44fe00
int GetLocalHumanDpid()
{
    for (int i = 0; i < 10; i++) {
        if (g_game->players[i].type == 1) {
            return g_game->players[i].id;
        }
    }
    return -1;
}

static inline int PlayerId(unsigned char i)
{
    if (i == 10 || !g_game->players[i].type)
        return -1;
    return g_game->players[i].id;
}

// The second part's lookups (0x451bc0, 0x453010) call this out of line;
// the merged file would otherwise inline it into them.
#pragma auto_inline(off)
// FUNCTION: 0x44fe40
unsigned char __stdcall FindSlotByDpid(int id)
{
    if (id != -1) {
        for (unsigned char i = 0; i < 10; i++) {
            if (PlayerId(i) == id)
                return i;
        }
    }
    return 10;
}
#pragma auto_inline(on)

// FUNCTION: 0x44feb0
unsigned char __stdcall FUN_0044feb0(Player* player)
{
    if (player == 0) {
        return 10;
    }
    return player->field_146;
}

static inline int GetPlayerField_0044fed0(unsigned char i)
{
    // Redundant i != 10 test must stay: 10 is the not-found index.
    if (i != 10 && g_game->players[i].type)
        return g_game->players[i].id;
    return -1;
}

static inline unsigned char FindPlayerIndex_0044fed0(int id)
{
    if (id != -1) {
        for (unsigned char i = 0; i < 10; i++) {
            if (GetPlayerField_0044fed0(i) == id)
                return i;
        }
    }
    return 10;
}

// Finds the player whose field_4 equals the given id and returns a pointer to
// it, or null when there is none.
// FUNCTION: 0x44fed0
Player* __stdcall FindPlayerByDpid(int id)
{
    if (FindPlayerIndex_0044fed0(id) == 10)
        return 0;
    return &g_game->players[FindPlayerIndex_0044fed0(id)];
}

// Returns the id at +4 of a player slot (index 10 means none), or -1 when
// the slot is unused; the pointer version is 0x450010.
// The second part's lookups (0x451bc0, 0x453010) call this out of line;
// the merged file would otherwise inline it into them.
#pragma auto_inline(off)
// FUNCTION: 0x44ffd0
int __stdcall GetSlotDpid(unsigned char index)
{
    if (index != 10 && g_game->players[index].type != 0)
        return g_game->players[index].id;
    return -1;
}
#pragma auto_inline(on)

// FUNCTION: 0x450010
int __stdcall GetPlayerDpid(Player* obj)
{
    if (obj != 0 && obj->type != 0) {
        return (int)obj->field_4;
    }
    return -1;
}

static inline int PlayerField(unsigned char index)
{
    if (index != 10 && g_game->players[index].type != 0)
        return g_game->players[index].id;
    return -1;
}

// FUNCTION: 0x450030
int GetHostDpid()
{
    int i;
    for (i = 0; i < 10; i++) {
        if (g_game->players[i].info->flags_97 & 1)
            return PlayerField(i);
    }
    return -1;
}

static inline int PlayerDpid(Player* p)
{
    if (p != 0 && p->type != 0)
        return p->dpid;
    return -1;
}

// FUNCTION: 0x450090
int __stdcall SetPlayerName(unsigned char index, char* shortName, char* longName)
{
    Player* p = &g_game->players[index];
    strncpy(p->name, longName, 30);
    strncpy(p->fullName, shortName, 30);
    DPNAME name;
    name.dwSize = sizeof(DPNAME);
    name.dwFlags = 0;
    name.lpszShortNameA = shortName;
    name.lpszLongNameA = longName;
    int r = HAPINET_setplayername(g_game->net, PlayerDpid(p), &name, 2);
    return r == 0 ? 1 : 0;
}

// FUNCTION: 0x450140
int __stdcall GetPlayerName(int dpid, char* shortName, char* longName)
{
    int r;
    if (dpid != -1) {
        char buf[0x400];
        unsigned long size = 0x400;
        r = HAPINET_getplayername(g_game->net, dpid, buf, &size);
        if (r == 0) {
            strcpy(shortName, ((DPNAME*)buf)->lpszShortNameA);
            strcpy(longName, ((DPNAME*)buf)->lpszLongNameA);
        }
    } else {
        strcpy(shortName, "COMPUTER");
        strcpy(longName, "COMPUTER");
        r = 0;
    }
    return r == 0;
}

static inline int GetPlayerField(unsigned char i)
{
    if (i != 10 && g_game->players[i].type)
        return g_game->players[i].field_4;
    return -1;
}

static inline unsigned char FindPlayerIndex_00450240(int id)
{
    // Early return, not a loop wrapped in if (id != -1).
    if (id == -1)
        return 10;
    for (unsigned char i = 0; i < 10; i++) {
        if (GetPlayerField(i) == id)
            return i;
    }
    return 10;
}

static inline Player* FindPlayer(int id)
{
    if (FindPlayerIndex_00450240(id) == 10)
        return 0;
    return &g_game->players[FindPlayerIndex_00450240(id)];
}

// Finds the highest field_4 among the active players of type 1 or 3, looks
// that player up by field_4 and sets bit 0 of its info flags. Nothing in the
// exe calls it. It returns the constant 10, the "no player" index.
// FUNCTION: 0x450240
unsigned char PickNewHost()
{
    unsigned int max = 0;
    Player* p = g_game->players;
    int n = 10;
    do {
        if ((p->active != 0 && p->type == 3)
            || (p->active != 0 && p->type == 1)) {
            if (p->field_4 > max)
                max = p->field_4;
        }
        p++;
    } while (--n);

    Player* q = FindPlayer(max);
    if (q)
        q->info->flags_97 |= 1;
    // Must return 10, not void.
    return 10;
}

static inline int GetPlayerField_00450380(unsigned char i)
{
    if (i != 10 && g_game->players[i].type)
        return g_game->players[i].id;
    return -1;
}

static inline unsigned char FindPlayerIndex_00450380(int id)
{
    if (id != -1) {
        for (unsigned char i = 0; i < 10; i++) {
            if (GetPlayerField_00450380(i) == id)
                return i;
        }
    }
    return 10;
}

// Announces that the player with the given id has left, unless the local
// player is flagged. The index search was an inlined helper and appears twice
// in the original (see 0x44fed0, which has the same shape).
// FUNCTION: 0x450380
void __stdcall AnnouncePlayerLeft(int id)
{
    char buf[200];
    if (g_game->players[g_game->localPlayer].field_22 == 1)
        return;
    Player* p;
    if (FindPlayerIndex_00450380(id) == 10)
        p = 0;
    else
        p = &g_game->players[FindPlayerIndex_00450380(id)];
    if (p == 0)
        return;
    sprintf(buf, "%s %s", p->name, Translate(g_leftGameTexts[rand() & 7]));
    AddMessage(buf, 4, 0, p->field_146);
}

// Returns the first player slot that is inactive and not of type 4, or 10
// when there is none.
// FUNCTION: 0x4504f0
int FindFreeSlot()
{
    int found = 0;
    int i;
    for (i = 0; i < 10; i++) {
        Player* p = &g_game->players[i];
        if (p->active == 0 && p->type != 4) {
            found = 1;
            break;
        }
    }
    if (!found)
        i = 10;
    return i;
}

// SendLobbySyncRequests (0x450530) stays in net_game_450530.cpp. Its original
// translation unit saw only a prototype of GetSlotDpid (0x44ffd0), so the
// compiler could not inline it; here the definition is in the same file and
// /Ob2 expands it into the FindTo helper, which then misses the inline budget
// at one IsPlaying site and spills the loop counter, 989 bytes against 977.
// Putting the definition after the caller does not help (MSVC inlines across
// the whole file); #pragma auto_inline(off) does, but the guide allows that
// only in a class's file, so 0x450530 keeps a file of its own.

// FUNCTION: 0x450910
char FindFreePlayerId(void)
{
    Game* game = g_game;
    for (int id = 1; id <= 10; id++) {
        int used = 0;
        for (int i = 0; i < 10; i++) {
            Player* p = &game->players[i];
            if (p->active != 0
                && (p->type == 1 || p->type == 2 || p->type == 3)
                && p->field_146 != 10
                && p->field_c == id) {
                used = 1;
            }
        }
        if (!used) {
            return id;
        }
    }
    return 0;
}

// Sets the game's flag at +0x299c when two active players (of type 1, 2 or
// 3, and not in state 10) share an id from 1 to 10 (see 0x450910, which
// finds the first unused id).
// Indexing g_game->players[i] in every test (rather than a player pointer
// local) is what makes MSVC walk the array from the type field.
// FUNCTION: 0x450980
void CheckDuplicatePlayerIds(void)
{
    int counts[11];
    memset(counts, 0, sizeof(counts));
    int dup = 0;
    for (int i = 0; i < 10; i++) {
        if (g_game->players[i].active != 0
            && (g_game->players[i].type == 1 || g_game->players[i].type == 2 || g_game->players[i].type == 3)
            && g_game->players[i].field_146 != 10
            && g_game->players[i].field_c > 0 && g_game->players[i].field_c <= 10) {
            if (++counts[g_game->players[i].field_c] > 1) {
                dup = 1;
                break;
            }
        }
    }
    g_game->duplicateIds = dup;
}

static inline unsigned char FindSlot_00450a10(int id)
{
    unsigned char i;
    for (i = 0; i < 10; i++) {
        int v;
        if (i == 10) {
            v = -1;
        } else {
            v = g_game->players[i].type ? g_game->players[i].id : -1;
        }
        if (v == id) {
            return i;
        }
    }
    return 10;
}

// FUNCTION: 0x450a10
int __stdcall AddNetPlayer(int param_1)
{
    unsigned char slot;
    if (param_1 == -1) {
        slot = 10;
    } else {
        slot = FindSlot_00450a10(param_1);
    }
    int flag = 0;
    if (slot != 10) {
        if (g_game->players[slot].type != 1 && g_game->players[slot].type != 2) {
            flag = 1;
        }
        if (g_game->players[slot].active != 0) {
            flag |= 1;
        }
    } else {
        int found = 0;
        int i = 0;
        while (i < 10) {
            Player* q = &g_game->players[i];
            if (q->active == 0 && q->type != 4) {
                found = 1;
                break;
            }
            i++;
        }
        int s = i;
        if (!found) {
            s = 10;
        }
        slot = s;
        if (slot == 10) {
            flag = 1;
        } else {
            g_game->players[slot].type = 3;
        }
    }
    Player* p = &g_game->players[slot];
    if (flag) {
        return 1;
    }
    unsigned long size;
    Packet_00450a10 packet;
    char buf[0x400];
    int result;
    // Destinations precomputed before the branch: keeps the strcpy source scan canonical.
    char* d_full = p->fullName;
    char* d_name = p->name;
    if (param_1 != -1) {
        size = 0x400;
        result = HAPINET_getplayername(g_game->net, param_1, buf, &size);
        if (result == 0) {
            strcpy(d_full, ((DPNAME*)buf)->lpszShortNameA);
            strcpy(d_name, ((DPNAME*)buf)->lpszLongNameA);
        }
    } else {
        strcpy(d_full, "COMPUTER");
        strcpy(d_name, "COMPUTER");
        result = 0;
    }
    if (result) {
        return 1;
    }
    SetupPlayerSlot(slot, g_game->players[slot].type);
    p->field_22 = 0;
    p->id = param_1;
    p->field_1c = GetTicks();
    g_game->field_2a3c++;
    if (g_game->flags_2a44_w & 1) {
        for (int i = 0; i < 10; i++) {
            Player* q = &g_game->players[i];
            if (q->active != 0 && (q->type == 1 || q->type == 2)) {
                packet.data = *q->data;
                packet.data.field_90 = q->id;
                packet.type = 0x20;
                BroadcastPacket(q->id, &packet, sizeof(packet));
                if (q->active != 0 && (q->type == 1 || q->type == 2)) {
                    unsigned char* msg = g_game->buffer;
                    msg[0] = 0x24;
                    *(int*)(msg + 1) = q->id;
                    msg[5] = q->alliance;
                    BroadcastPacket(q->id, msg, 6);
                    if (g_usePacketManager != 0) {
                        g_packetManager.SendAllQueued(1);
                    }
                }
            }
        }
        SendLobbySyncRequests();
        g_packetManager.SendAllQueued(1);
    }
    if (g_game->campaign->GetGameType() == 3 && g_game->field_2a3c > 1) {
        ReportGameEvent(2);
    }
    return 1;
}

// FUNCTION: 0x450d80
void InitNetConnection()
{
    HAPINET_initmultiplaydefaults(g_game->net);
    g_game->netMode = 10;
    if (InitPacketManager(2, 100)) {
        HAPINET_initconnection(g_game->net, g_game->connection);
    }
}

// FUNCTION: 0x450dd0
void CloseNetSession()
{
    if (g_game->flags_2a44_w & 1) {
        if (g_usePacketManager != 0) {
            g_packetManager.SendAllQueued(1);
        }
        if (!IsReporterDllLoaded()) {
            HAPINET_uninitmultiplay(g_game->net);
        }
        g_game->flags_2a44_w &= 0xfffe;
    }
}

static inline int IsPlaying_00450e20(Player* player)
{
    if (player->active == 0)
        return 0;
    if (player->state == 1 || player->state == 2)
        return 1;
    return 0;
}

// FUNCTION: 0x450e20
void LeaveNetGame()
{
    if (g_usePacketManager != 0) {
        g_packetManager.SendAllQueued(1);
    }
    if (g_game->flags_2a44_w & 1) {
        for (int i = 0; i < 10; i++) {
            if (IsPlaying_00450e20(&g_game->players[i])) {
                RemovePlayer(g_game->players[i].id);
            }
        }
        ShutdownScoreTables();
    }
    HAPINET_quitgame(g_game->net);
    SetCloseHandler(0, 0);
    g_game->field_3923b |= 4;
    int reason = g_game->players[g_game->localPlayer].reason;
    char* text;
    if (reason != 0) {
        switch (reason) {
        case 4:
            text = "You did not have the correct password";
            break;
        case 3:
            text = "The game is closed";
            break;
        case 5:
            text = "The game is full";
            break;
        case 6:
            text = "You have lost connection with the game";
            break;
        case 7:
            text = "You need a unit you don't have for this game";
            break;
        case 8:
            text = "You need a newer version of the game to enter";
            break;
        case 9:
            text = "No watching is allowed for this game";
            break;
        case 10:
            text = "The creator has left the game";
            break;
        default:
            text = "You were rejected from the game";
            break;
        }
    } else {
        text = 0;
    }
    QuitApp(text);
}

static inline int IsPlaying_00450f90(Player* player)
{
    if (player->active == 0)
        return 0;
    if (player->state == 1 || player->state == 2)
        return 1;
    return 0;
}

// When the game is networked (flag 1 at +0x2a44), sends every playing
// player's 0xb9-byte data block (type 0x20) and then its 6-byte message
// (type 0x24, the same code as BroadcastAllyTeam).
// The packet is declared at function scope: its address escapes to
// BroadcastPacket in one iteration, so MSVC re-reads player->id around the
// stores into it in the next, as the original does.
// FUNCTION: 0x450f90
void BroadcastPlayerInfo()
{
    Packet_00450f90 packet;
    if (g_game->flags_2a44_w & 1) {
        for (int i = 0; i < 10; i++) {
            Player* player = &g_game->players[i];
            if (IsPlaying_00450f90(player)) {
                packet.data = *player->data;
                packet.data.field_90 = player->id;
                packet.type = 0x20;
                BroadcastPacket(player->id, &packet, sizeof(packet));
                if (IsPlaying_00450f90(player)) {
                    unsigned char* msg = g_game->buffer;
                    msg[0] = 0x24;
                    *(int*)(msg + 1) = player->id;
                    msg[5] = player->field_13f;
                    BroadcastPacket(player->id, msg, 6);
                    if (g_usePacketManager != 0) {
                        g_packetManager.SendAllQueued(1);
                    }
                }
            }
        }
        SendLobbySyncRequests();
        g_packetManager.SendAllQueued(1);
    }
}

// FUNCTION: 0x451090
void __stdcall BuildGameInfo(char* name, int* d, int* c, int* b, int* a)
{
    PlayerInfo* info = g_game->players[g_game->localPlayer].info;
    char* p = (char*)info;
    p[0xa7] = g_game->field_1;
    p += 0x99;
    p[0xf] = g_game->field_2;
    unsigned short w = *(unsigned short*)(p + 2);
    p += 4;
    *(unsigned short*)(p - 2) = w ^ ((g_game->field_2a3c ^ w) & 0xf);
    p += 4;
    *d = *(int*)(p - 8);
    *c = *(int*)(p - 4);
    *b = *(int*)(p);
    *a = *(int*)(p + 4);
    memset(name, ' ', 0x20);
    name[0x1f] = 0;
    strncpy(name, g_game->gameName, 0x10);
    int src = g_game->campaign->GetMissionName();
    strncpy(name + 0x10, (char*)src, 0xf);
    char* q = name;
    int n = 0x20;
    do {
        if (*q == 0)
            *q = ' ';
        q++;
    } while (--n);
    name[0x1f] = 0;
}

// The flag is a bit of an unsigned short bitfield whose storage starts at the
// odd offset 0x9b (packed struct): that gives the byte load and "shr al, 4;
// test al, 1". An unsigned char bitfield folds to "test byte ptr".
// FUNCTION: 0x451180
void UpdateNetGameInfo(void)
{
    int a;
    int b;
    int c;
    int d;
    char name[32];

    BuildGameInfo(name, &d, &c, &b, &a);
    if (g_game->players[g_game->localPlayer].info->bits_9b.flag_9b_4) {
        g_game->settings.bits_475.flag_475_5 = 1;
    }
    HAPINET_updategameinfo(g_game->net, name, DAT_005119b8, d, c, b, a);
}

// The index of the first connected player, or 10 when there is none.
// Must stay a static inline helper: return i in the loop, return 10 after it.
static inline unsigned char FindPlayerInUse()
{
    for (unsigned char i = 0; i < 10; i++) {
        if (g_game->players[i].type != 0 && g_game->players[i].info->flag_97_0)
            return i;
    }
    return 10;
}

// Sends the "AI:" or nickname name to DirectPlay for one player slot and
// fills in that slot's PlayerInfo. The two "is any player connected" searches
// use the FindPlayerInUse helper above, called once before the name is
// built and once on the failure path; see 0x4515d0, 0x451bc0 and 0x451df0 for
// the same helper.
// FUNCTION: 0x451220
int __stdcall CreateLocalPlayer(unsigned char playerIndex, int flag)
{
    char buf[256];

    Player* player = &g_game->players[playerIndex];
    player->SetType(flag);

    // Parameter on the left of the compare.
    int same = (playerIndex == FindPlayerInUse());

    if (flag == 1) {
        strcpy(buf, g_game->nickName);
    } else {
        sprintf(buf, "AI:%s", g_game->players[g_game->localPlayer].name);
        buf[16] = 0;
    }

    player->info->flag_97_0 = same;
    player->info->field_96 = 0xff;
    player->field_21 &= 0xfd;
    player->field_8 = GetTicks();
    PlayerInfo* info = player->info;
    info->field_9b = (info->field_9b ^ ((g_game->field_2a3c_signed ^ info->field_9b) & 0xf)) & 0x7fff;
    info->flag_9d_0 = (strlen(g_game->passWord) != 0);
    info->field_a5 = 0x64;
    info->field_8b = g_game->field_37f1b;
    info->field_8d = g_game->field_37f1f;
    info->field_a7 = g_game->field_1;
    info->field_a8 = g_game->field_2;

    int r = HAPINET_addplayer(g_game->net, (unsigned long*)&player->field_4,
                         buf, buf, g_game->passWord, 0, 0x50);
    if (r == 0) {
        g_game->players[playerIndex].SetType(0);
        unsigned char i = FindPlayerInUse();
        Player* slot = &g_game->players[i];
        if (slot->active != 0 && (slot->type == 1 || slot->type == 2)) {
            OpenMessageBox(g_game->menu,
                Translate("Direct Play failed to add new player.\n\nRecommended you go to previous screen and re-create the game session.\n"),
                500, 1, 1);
        }
        else {
            OpenMessageBox(g_game->menu,
                Translate("Direct Play failed to add new player.\n\nRecommended you go to previous screen and re-join the game session.\n"),
                500, 1, 1);
        }
    }
    return r;
}

// The module's second part (0x451540 to 0x4560c0): the lobby join,
// player colours and alliances, packet dispatch, rejection, timeout and
// chat handling. <windows.h> and <process.h> carry the lobby join and
// display calls, <string.h> and <stdio.h> the string and printf
// patterns, and <iostream> 0x453010's shared return block.
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <process.h>
#include <string.h>
#include <stdio.h>
#include <iostream>

#pragma pack(push, 1)

typedef int (__stdcall *EntryFunc)(int);

class PacketReceiver {
public:
    int ReceiveFrame(void* net, unsigned char* data, int* size);
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

#pragma pack(pop)

extern PacketChannel DAT_00513008;
extern int DAT_00512b90[11];
extern PacketReceiver DAT_0051e300;
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

void* __cdecl FUN_004d83b0(char* tag, int size);
int __stdcall InitPacketTables(Class_00451fd0* param_1);
void __stdcall EnumPlayersCallback(int id, int unused1, int unused2, int unused3, int unused4);
int __stdcall JoinLobbyGame(Player* p);
void __stdcall JoinLobbyGameThread(Args_00451640* args);
unsigned __stdcall JoinLobbyGameThread(void* args);
int __stdcall SendPacketToPlayer(int from, int to, void* packet, int size);
int __stdcall AssignPlayerColor(int from, int to, int group);
int __stdcall IsColorFree(int id, int slot);
int __stdcall RequestPlayerColor(int param);
int __stdcall SetAlliance(int from, int to, unsigned char value, int extra);
int __stdcall BroadcastAllyTeam(Player* player);
int __stdcall RejectPlayer(int id, unsigned char value);
int __stdcall ReceiveNetPacket(void);
void __stdcall OpenTimeoutDialog(int id);
void CheckPlayerTimeouts(void);
void __stdcall HandleTimeoutDialog(void* gadget);
void __stdcall UpdateTimeoutDialog(void* gadget);
void RebuildAllyList(void);
int __stdcall HAPINET_passwordrequired(void* net);
int IsOnlineConfigLoaded(void);
void __stdcall SetCursorMode(int n);
void __cdecl FUN_004d85a0(void* p);
void* __cdecl FUN_004d84a0(void* param_1, const char* name, unsigned int param_3);
int __stdcall HAPINET_createorjoinlobbygame(void* obj, char* name, int a, int b, int c, int d, int e);
void __stdcall HAPINET_createnewgame(void* obj, char* name, char* data, int d, int c, int b, int a);
void ResetPlayerSlots(void);
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
void __stdcall ShareMapInfo(unsigned char from, unsigned char to);
void __stdcall HandlePlayerEconomy(void* packet, Player* player);
void __stdcall SetGameSpeed(int speed, int param_2);
void __stdcall PlaySoundByName(char* name, int param_2);
int __stdcall IsCurrentGadgetNamed(Gadget_004538f0* gadget, char* name);
Entry_004538f0* __stdcall FUN_004a0010(Entry_004538f0* entries, char* name);
int __stdcall SendChatMessage(Player* from, char* text, int param_3, char* to);
int __stdcall FindGadgetIndex(void* gadgets, const char* name, int flag);
void __stdcall FUN_004a7190(void* menu, int index);
void __stdcall ClearSelectedGadget(void* param_1);
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

static inline int PlayerId_004515d0(unsigned char i)
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
            if (PlayerId_004515d0(i) == id)
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
            SendLobbySyncRequests();
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

static inline Player* FindPlayer_00451df0(int id)
{
    if (FindPlayerIndex_00451df0(id) == 10)
        return 0;
    // Searched twice: gives the two copies of the id loop.
    return &g_game->players[FindPlayerIndex_00451df0(id)];
}

// FUNCTION: 0x451df0
int __stdcall BroadcastPacket(int id, unsigned char* packet, int size)
{
    Player* p = FindPlayer_00451df0(id);
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
int __stdcall BroadcastPlayerLeft(int id)
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
int __stdcall BroadcastAllyTeam(Player* player)
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
void __stdcall SendProbe(int from, int to)
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
        ClearSelectedGadget(g_game->message);
        FUN_0049fa90(g_game->message);
        return;
    }
    if (IsCurrentGadgetNamed(gadget, "REJECT")) {
        RejectPlayer(g_timeoutPlayerDpid, 6);
        return;
    }
    ClearSelectedGadget(gadget);
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
int Class_00456030::IsPlayableSlot()
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
    packet.field_1 = obj->unitDefIndex;
    packet.field_3 = obj->id;
    packet.field_5 = obj->pos;
    packet.field_11 = obj->rot;
    BroadcastPacket(obj->player->field_4, &packet, 0x17);
}

// FUNCTION: 0x4560c0
void __stdcall BroadcastBuilderLink(Unit* obj, Unit* target)
{
    Packet_004560c0 packet;
    packet.type = 0x12;
    packet.field_1 = target->id;
    packet.field_3 = obj->id;
    BroadcastPacket(obj->player->field_4, &packet, 5);
}

// The module's third part (0x456110 to 0x457d30): the script-call packets,
// heartbeat, ping, load progress, player counts, resource sharing and the
// lobby connection. <stdio.h> gives 0x456de0's [ecx + esi] order, <string.h>
// the loop index orders of 0x4572a0, 0x4573d0 and 0x4578f0, <stdlib.h> the
// first loop's address sums in 0x457540 and <algorithm> 0x4568c0's
// random_shuffle; <time.h> 0x457710's time and localtime.
#include <algorithm>
#include <time.h>
#pragma pack(push, 1)

struct Message_004565a0 {
    char unknown_0[1];
    int start_tick;                    // +0x01
    int sent_tick;                     // +0x05
    int id;                            // +0x09
};

struct Packet_00456110 {
    unsigned char type;                // +0x0
    short id;                          // +0x1
    short index;                       // +0x3
    char field_5;                      // +0x5
    int field_6;                       // +0x6
    int field_a;                       // +0xa
    int field_e;                       // +0xe
    int field_12;                      // +0x12
};

struct Msg13_00456310 {
    unsigned char type;                // +0x00
    int start_tick;                    // +0x01
    int sent_tick;                     // +0x05
    int id;                            // +0x09
};

struct Msg26_00456310 {
    unsigned char type;                // +0x00
    int table[10];                     // +0x01
};

struct Msg20_00456310 {
    unsigned char type;                // +0x00
    PlayerInfo info;                   // +0x01
};

struct Packet_00456de0 {
    unsigned char type;                 // +0x0
    unsigned char progress;             // +0x1
};

struct Packet_00456ee0 {
    unsigned char type;                // +0x0
    int subtype;                       // +0x1
    int from;                          // +0x5
    int to;                            // +0x9
    int extra;                         // +0xd
};

struct Packet_00457050 {
    unsigned char type;                // +0x0
    int subtype;                       // +0x1
    int from;                          // +0x5
    int to;                            // +0x9
    int value;                         // +0xd
};

struct Packet_004571c0 {
    unsigned char type;                // +0x0
    int subtype;                       // +0x1
    int from;                          // +0x5
    int to;                            // +0x9
    int zero;                          // +0xd
};

struct Packet_004573d0 {              // 0x3a bytes
    unsigned char type;                // +0x0
    unsigned char flag;                // +0x1
    int field_2;                       // +0x2
    int field_6;                       // +0x6
    int field_a;                       // +0xa
    int field_e;                       // +0xe
    int field_12;                      // +0x12
    int field_16;                      // +0x16
    int field_1a;                      // +0x1a
    int field_1e;                      // +0x1e
    float field_22;                    // +0x22
    float field_26;                    // +0x26
    float field_2a;                    // +0x2a
    float field_2e;                    // +0x2e
    float field_32;                    // +0x32
    float field_36;                    // +0x36
};

struct Packet_00457540 {              // 0x3a bytes
    unsigned char type;                // +0x0
    unsigned char flag;                // +0x1
    short field_2;                     // +0x2
    char unknown_4[0x6 - 0x4];
    short field_6;                     // +0x6
    char unknown_8[0xa - 0x8];
    short field_a;                     // +0xa
    char unknown_c[0xe - 0xc];
    short field_e;                     // +0xe
    char unknown_10[0x12 - 0x10];
    int field_12;                      // +0x12
    int field_16;                      // +0x16
    int field_1a;                      // +0x1a
    int field_1e;                      // +0x1e
    float field_22;                    // +0x22
    float field_26;                    // +0x26
    float field_2a;                    // +0x2a
    float field_2e;                    // +0x2e
    float field_32;                    // +0x32
    float field_36;                    // +0x36
};

struct TeamPacket_00457540 {           // 3 bytes
    unsigned char type;                // +0x0
    unsigned char flag;                // +0x1
    unsigned char flag2;               // +0x2
};

struct Packet_00457d30 {
    unsigned char type;                // +0x0
    int subtype;                       // +0x1
    int from;                          // +0x5
    int to;                            // +0x9
    int zero;                          // +0xd
};

#pragma pack(pop)

extern char DAT_00512ca8[];
extern char DAT_00512c98[];

int __stdcall HAPINET_guaranteepackets(int param_1);
int __stdcall RequestPlayerColor(int param_1);
void __stdcall SleepMilliseconds(unsigned int param_1);
void __stdcall SendPlayerEconomy(Player* from, Player* to, unsigned char param_3);
void __stdcall TransferEnergy(unsigned char from, unsigned char to, float amount, int flag);
void __stdcall TransferMetal(unsigned char from, unsigned char to, float amount, int flag);
int __stdcall HAPINET_initlobbiedconnection(void* p);
void __stdcall SetMissionType(int a);

// FUNCTION: 0x456110
int __stdcall SendScriptCallNoArgsByName(Unit* obj, char* name)
{
    Packet_00456110 packet;
    int index = obj->names->FindScript(name);
    if (!(g_game->flags_2a44 & 1)) {
        return 0;
    }
    packet.type = 0x10;
    packet.id = obj->id;
    packet.index = index;
    packet.field_5 = 0;
    packet.field_6 = 0;
    packet.field_a = 0;
    packet.field_e = 0;
    packet.field_12 = 0;
    return BroadcastPacket(obj->player->id, &packet, 0x16);
}

// FUNCTION: 0x456190
int __stdcall SendScriptCallNoArgs(Unit* obj, short index)
{
    Packet_00456110 packet;
    if (!(g_game->flags_2a44 & 1)) {
        return 0;
    }
    packet.type = 0x10;
    packet.id = obj->id;
    packet.index = index;
    packet.field_5 = 0;
    packet.field_6 = 0;
    packet.field_a = 0;
    packet.field_e = 0;
    packet.field_12 = 0;
    return BroadcastPacket(obj->player->id, &packet, 0x16);
}

// Sends a 0x16-byte type 0x10 packet for the object, like 0x456290, with the
// index looked up by name in the object's name table (+0x9a) first.
// FUNCTION: 0x456200
int __stdcall SendScriptCallByName(Unit* obj, char* name, char field_5,
                           int field_6, int field_a, int field_e, int field_12)
{
    Packet_00456110 packet;
    short index = obj->names->FindScript(name);
    if (!(g_game->flags_2a44 & 1)) {
        return 0;
    }
    packet.type = 0x10;
    packet.id = obj->id;
    packet.index = index;
    packet.field_5 = field_5;
    packet.field_6 = field_6;
    packet.field_a = field_a;
    packet.field_e = field_e;
    packet.field_12 = field_12;
    return BroadcastPacket(obj->player->id, &packet, 0x16);
}

// Sends a 0x16-byte type 0x10 packet for the object (the sibling 0x456110
// sends the same packet with only the index filled in).
// FUNCTION: 0x456290
int __stdcall SendScriptCall(Unit* obj, short index, char field_5,
                           int field_6, int field_a, int field_e, int field_12)
{
    Packet_00456110 packet;
    if (!(g_game->flags_2a44 & 1)) {
        return 0;
    }
    packet.type = 0x10;
    packet.id = obj->id;
    packet.index = index;
    packet.field_5 = field_5;
    packet.field_6 = field_6;
    packet.field_a = field_a;
    packet.field_e = field_e;
    packet.field_12 = field_12;
    return BroadcastPacket(obj->player->id, &packet, 0x16);
}

// Declaring the Msg20 message at function scope (not inside the loop) is what
// puts its address in eax and fixes the whole send block; the trailing
// g_game+0x2bee flag is a 1-bit unsigned short bitfield, which is what makes
// the original emit `or byte ptr [eax+0x2bee], 1` in one instruction.
// FUNCTION: 0x456310
void SendNetHeartbeat()
{
    unsigned int now = GetTicks();
    if ((int)(now - g_game->field_1b5f) <= 0x3c)
        return;
    g_game->field_1b5f += 0x3c;

    for (int i = 0; i < 10; i++) {
        Player* p = &g_game->players[i];
        if (p->active != 0 && (p->type == 1 || p->type == 2)) {
            if (g_usePacketManager != 0)
                g_packetManager.SendAllQueued(1);

            Msg13_00456310 msg;
            msg.type = 2;
            msg.start_tick = GetTickCount();
            msg.sent_tick = 0;
            msg.id = p->id;

            int was = HAPINET_guaranteepackets(0);
            BroadcastPacket(p->id, (unsigned char*)&msg, 0xd);
            if (g_usePacketManager != 0)
                g_packetManager.SendAllQueued(1);
            if (was != 0)
                HAPINET_guaranteepackets(1);

            int id = p->id;
            unsigned char* buf = g_game->buffer;
            buf[0] = 6;
            BroadcastPacket(id, buf, 1);

            if (p->info->field_96 == 0xff)
                RequestPlayerColor(0);

            if (p->info->bits_97.b0 & 1) {
                for (int k = 0; k < 10; k++) {
                    unsigned char st = g_game->players[k].type;
                    if (st == 4)
                        g_game->table_2c28[k] = -1;
                    else if (st == 0)
                        g_game->table_2c28[k] = 0;
                    else
                        g_game->table_2c28[k] = g_game->players[k].id;
                }

                Msg26_00456310 msg26;
                memcpy(msg26.table, g_game->table_2c28, 0x28);
                msg26.type = 0x26;
                BroadcastPacket(p->id, (unsigned char*)&msg26, 0x29);
            }
        }
    }

    Msg20_00456310 msg;
    if (g_game->flags_2a44 & 1) {
        for (int i = 0; i < 10; i++) {
            Player* p = &g_game->players[i];
            if (p->active != 0 && (p->type == 1 || p->type == 2)) {
                msg.info = *p->info;
                msg.info.field_90 = p->id;
                msg.type = 0x20;
                BroadcastPacket(p->id, (unsigned char*)&msg, 0xba);

                if (p->active != 0 && (p->type == 1 || p->type == 2)) {
                    unsigned char* buf2 = g_game->buffer;
                    buf2[0] = 0x24;
                    *(int*)(buf2 + 1) = p->id;
                    buf2[5] = p->field_13f;
                    BroadcastPacket(p->id, buf2, 6);
                    if (g_usePacketManager != 0)
                        g_packetManager.SendAllQueued(1);
                }
            }
        }
        SendLobbySyncRequests();
        g_packetManager.SendAllQueued(1);
    }

    g_game->bit0_2bee = 1;
}

// Two halves, keyed on the message's own flag at +0x05. When it is clear this
// is the first sighting: stamp the message with the current tick count, send
// it to the player id in the header (the first argument is the local lobby
// dpid from g_game + 0x4cd), and give the packets back to the network layer.
// When it is already set the message is a later update: find the sending
// player in the table, and if they are an active client (state 1 or 2) record
// the round trip into the slot of the player in g_game + 0x4c9.
static inline int GetPlayerId_00456de0(unsigned char i)
{
    if (i != 10 && g_game->players[i].type)
        return g_game->players[i].id;
    return -1;
}

static inline unsigned char FindPlayerIndex(int id)
{
    // Early return 10 for -1, not a guarded loop.
    if (id == -1)
        return 10;
    for (unsigned char i = 0; i < 10; i++) {
        if (GetPlayerId_00456de0(i) == id)
            return i;
    }
    return 10;
}

// FUNCTION: 0x4565a0
void __stdcall HandlePing(Message_004565a0* p)
{
    if (p->sent_tick == 0) {                       // not sent yet
        if (g_usePacketManager != 0) {
            g_packetManager.SendAllQueued(1);
        }
        p->sent_tick = GetTickCount();
        int packets_were_guaranteed = HAPINET_guaranteepackets(0);
        SendPacketToPlayer(g_game->lobby1, p->id, p, 0xd);
        if (g_usePacketManager != 0) {
            g_packetManager.SendAllQueued(1);
        }
        if (packets_were_guaranteed != 0) {          // hand them back
            HAPINET_guaranteepackets(1);
        }
        return;
    }

    unsigned char index = FindPlayerIndex(p->id);
    // Table lookup goes through the pl pointer local.
    Player* pl = &g_game->players[index];
    if (pl->active == 0) {
        return;
    }
    if (pl->state != 1 && pl->state != 2) {
        return;
    }
    unsigned char other = FindPlayerIndex(g_game->lobby2);
    // No bound check on either index: an id of -1 makes FindPlayerIndex
    // return 10, the spare eleventh slot (the table has 11, see docs/bugs.md).
    g_game->players[other].field_14 = GetTickCount() - p->start_tick;
}

// Returns 0 when no player is in state 3. Otherwise every active player of
// type 1 or 2, or in state 3, must have bit 0x20 set in its data flags
// (+0x9b), else it returns 0. Returns 1 when some slot is inactive or lacks
// bit 0x40, unless field_2a3c is 1 (then 0).
static inline int IsPlaying(unsigned char i)
{
    return g_game->players[i].active != 0 && g_game->players[i].type == 3;
}

// The mix of `p->` and `g_game->players[j].` in the second loop is needed:
// it keeps the later reads of active and data from reusing the first loads,
// which frees dl for j and bl for the 0x40 mask as in the original.
// FUNCTION: 0x456760
int AreAllPlayersReady()
{
    int count = 0;
    for (int i = 0; i < 10; i++) {
        Player* p = &g_game->players[i];
        if (p->active != 0 && p->type == 3)
            count++;
    }
    if (count == 0)
        return 0;

    int all = 1;
    for (unsigned char j = 0; j < 10; j++) {
        Player* p = &g_game->players[j];
        if (p->active != 0 && (p->type == 1 || p->type == 2 || IsPlaying(j))) {
            if (!(g_game->players[j].data->flags_9b & 0x20))
                return 0;
        }
        if (g_game->players[j].active == 0 || !(g_game->players[j].data->flags_9b & 0x40))
            all = 0;
    }
    if (g_game->field_2a3c == 1)
        return 0;
    return all == 0;
}

// FUNCTION: 0x456850
unsigned char FindHostSlot()
{
    unsigned char i;
    for (i = 0; i < 10; i++) {
        if (g_game->players[i].type != 0 && (g_game->players[i].data->flags_97 & 1))
            return i;
    }
    return 10;
}

// FUNCTION: 0x4568b0
void __stdcall ReportPacketGap(int, int, int)
{
}

static inline unsigned char FindOccupied_004568c0() {
    for (unsigned char i = 0; i < 10; i++) {
        if (g_game->players[i].type != 0 && g_game->players[i].info->bits_97.b0)
            return i;
    }
    return 10;
}

static inline int FirstJoinedId_004568c0() {
    for (int j = 0; j < 10; j++) {
        if (g_game->players[j].type == 1)
            return g_game->players[j].id;
    }
    return -1;
}

static inline int IsConnected_004568c0(Player* p) {
    return p->active != 0 && (p->type == 1 || p->type == 2);
}

static inline int PlayerId_004568c0(unsigned char pi) {
    // Separate pi == 10 test, then a widened copy; no Player* local, no combined test.
    if (pi == 10)
        return -1;
    int i = pi;
    if (g_game->players[i].type != 0)
        return g_game->players[i].id;
    return -1;
}

// FUNCTION: 0x4568c0
int AssignStartPositions() {
    unsigned char idx = FindOccupied_004568c0();
    int res = ((Class_00456030*)&g_game->players[idx])->IsPlayableSlot();
    if (res != 0 && g_game->field_2a28 == 0) {
        int* out = g_game->field_29fc;
        if (g_game->players[g_game->localPlayer].info->bits_9b.flag14) {
            int n = 0;
            for (int k0 = 0; k0 < 10; k0++) {
                Player* q = &g_game->players[k0];
                if (q->active != 0 && (q->type == 1 || q->type == 2 || q->type == 3) &&
                    q->field_146 != 10 && (q->info->bits_9b.flag6) == 0)
                    out[k0] = n++;
                else
                    out[k0] = -1;
            }
        } else {
            int cand[10];
            for (int z = 0; z < 10; z++)
                cand[z] = -1;
            int n = 0;
            for (int k1 = 0; k1 < 10; k1++) {
                Player* p = &g_game->players[k1];
                if (p->active != 0 && (p->type == 1 || p->type == 2 || p->type == 3) &&
                    p->field_146 != 10 && (p->info->bits_9b.flag6) == 0) {
                    cand[n] = n;
                    n++;
                }
            }
            if (n > 2 || (int)((__int64)rand() * 2 / 0x8000) != 0)
                std::random_shuffle(cand, cand + n);
            int j = 0;
            for (int k2 = 0; k2 < 10; k2++) {
                Player* q2 = &g_game->players[k2];
                if (q2->active != 0 && (q2->type == 1 || q2->type == 2 || q2->type == 3) &&
                    q2->field_146 != 10) {
                    if (g_game->players[k2].active != 0 && (q2->info->bits_9b.flag6))
                        out[k2] = -1;
                    else
                        // Candidates read by index, cand[j++].
                        out[k2] = cand[j++];
                } else {
                    out[k2] = -1;
                }
            }
        }
        g_game->field_2a28 = 1;
    }
    int ok = 1;
    for (int k3 = 0; k3 < 10; k3++) {
        Player* q = &g_game->players[k3];
        if (q->active != 0 && q->type == 3) {
            if (res != 0) {
                if (g_game->field_29a4[k3] == 0 || g_game->field_29d0[k3] == 0) {
                    ok = 0;
                    break;
                }
            }
            if (res == 0) {
                if (g_game->field_29a4[k3] == 0) {
                    ok = 0;
                    break;
                }
            }
        }
    }
    if (res != 0) {
        for (int k4 = 0; k4 < 10; k4++) {
            // Skip with continue; the send stays a plain call.
            if (g_game->field_29d0[k4] != 0)
                continue;
            unsigned char packet[2];
            packet[0] = 0x1e;
            packet[1] = (unsigned char)g_game->field_29fc[k4];
            if (g_game->players[k4].active != 0) {
                if (g_game->players[k4].type == 3) {
                    SendPacketToPlayer(FirstJoinedId_004568c0(), PlayerId_004568c0(k4), packet, 2);
                } else if (IsConnected_004568c0(&g_game->players[k4])) {
                    g_game->players[k4].field_147 = packet[1];
                    g_game->field_29d0[k4] = 1;
                }
            }
        }
    }
    unsigned char pkt = 0x15;
    // Tail shape: if (res == 0) {...} else if (ok) {...}, then return ok.
    if (res == 0) {
        for (int k6 = 0; k6 < 10; k6++) {
            if (IsConnected_004568c0(&g_game->players[k6]))
                BroadcastPacket(PlayerId_004568c0(k6), &pkt, 1);
        }
    } else if (ok) {
        for (int k5 = 0; k5 < 10; k5++) {
            if (IsConnected_004568c0(&g_game->players[k5]))
                BroadcastPacket(PlayerId_004568c0(k5), &pkt, 1);
        }
    }
    return ok;
}

static inline int PlayerId_00456de0(unsigned char i)
{
    if (i != 10 && g_game->players[i].type)
        return g_game->players[i].id;
    return -1;
}

// Sends the average of the six load-stage percentages (g_game+0x38d6f..74)
// as a two-byte packet (type 0x2a) to every active player of type 1 or 2,
// and stores it in that player's +0x20.
// FUNCTION: 0x456de0
void __stdcall SendLoadProgress()
{
    Packet_00456de0 packet;
    // The type store must be the first statement, ahead of the prologue stores.
    packet.type = 0x2a;
    // Own int total, summed as a += chain in this order: sets the load order.
    int total = g_game->stages[0];
    total += g_game->stages[5];
    total += g_game->stages[4];
    total += g_game->stages[3];
    total += g_game->stages[2];
    total += g_game->stages[1];

    packet.progress = total / 6;
    for (int i = 0; i < 10; i++) {
        if (g_game->players[i].active != 0
            && (g_game->players[i].type == 1 || g_game->players[i].type == 2)) {
            g_game->players[i].progress = packet.progress;
            BroadcastPacket(PlayerId_00456de0(i), &packet, 2);
        }
    }
}

// Sends a 17-byte packet (type 0x16, subtype 1) from player `from` to player
// `to`, carrying both players' DirectPlay ids plus a dword argument. Slot 10
// means "no player".
//
// Same player layout and packet as 0x4571c0 (type 0x16, subtype 3); see
// 0x4515d0 for the PlayerDpid helper spelling.
static inline int PlayerDpid(unsigned char i)
{
    if (i != 10 && g_game->players[i].type != 0)
        return g_game->players[i].dpid;
    return -1;
}

static inline int PlayerReady(Player* p)
{
    return p->active != 0
        && (p->type == 1 || p->type == 2 || p->type == 3)
        && p->field_146 != 10
        && (p->field_144 != 0 || p->field_140 == 0);
}

// FUNCTION: 0x456ee0
void __stdcall SendShareMetal(unsigned char from, unsigned char to, int param_3)
{
    Player* a = &g_game->players[from];
    Player* b = &g_game->players[to];

    if (PlayerReady(a) && PlayerReady(b)) {
        Packet_00456ee0 packet;
        packet.type = 0x16;
        packet.subtype = 1;
        packet.from = PlayerDpid(from);
        packet.to = PlayerDpid(to);
        packet.extra = param_3;
        SendPacketToPlayer(PlayerDpid(from), PlayerDpid(to), &packet, sizeof(packet));
    }
}

// Sends a 17-byte packet (type 0x16, subtype 2) from player `from` to player
// `to` carrying both players' DirectPlay ids plus a dword, but only when both
// slots hold a real player of type 1, 2 or 3 (compare 0x4571c0, which is the
// same packet with subtype 3 and a trailing zero).
// FUNCTION: 0x457050
void __stdcall SendShareEnergy(unsigned char from, unsigned char to, int value)
{
    Player* first = &g_game->players[from];
    Player* second = &g_game->players[to];
    if (first->active != 0
        && (first->type == 1 || first->type == 2 || first->type == 3)
        && first->field_146 != 10
        && (first->field_144 != 0 || first->field_140 == 0)
        && second->active != 0
        && (second->type == 1 || second->type == 2 || second->type == 3)
        && second->field_146 != 10
        && (second->field_144 != 0 || second->field_140 == 0)) {
        Packet_00457050 packet;
        packet.type = 0x16;
        packet.subtype = 2;
        packet.from = PlayerDpid(from);
        packet.to = PlayerDpid(to);
        packet.value = value;
        SendPacketToPlayer(PlayerDpid(from), PlayerDpid(to), &packet, sizeof(packet));
    }
}

// Sends a 17-byte packet (type 0x16, subtype 3) from player `from` to
// player `to`, carrying both players' DirectPlay ids. Slot 10 means "no
// player".
static inline int PlayerDpid_00457d30(unsigned char i)
{
    if (g_game->players[i].type != 0)
        return g_game->players[i].dpid;
    return -1;
}

// FUNCTION: 0x4571c0
void __stdcall SendShareMapInfo(unsigned char from, unsigned char to)
{
    if (from != 10 && to != 10) {
        Packet_004571c0 packet;
        packet.type = 0x16;
        packet.subtype = 3;
        packet.from = PlayerDpid_00457d30(from);
        packet.to = PlayerDpid_00457d30(to);
        packet.zero = 0;
        SendPacketToPlayer(PlayerDpid_00457d30(from), PlayerDpid_00457d30(to), &packet, sizeof(packet));
    }
}

// Scans the ten player slots twice. The outer pass picks every slot that looks
// like a local player (active, type 1 or 2, field_140 set, field_22 clear); the
// inner pass then looks for a network slot (active, type 3) whose data->field_94
// is 1 and whose team (field_146) is still clear in the target's three per-team
// byte tables, and hands the pair to SendPlayerEconomy, returning 0 in that case.
// The three tables live at +0x11e, +0x129 and +0x134, eleven bytes each.
// FUNCTION: 0x4572a0
int FUN_004572a0()
{
    int result = 1;
    for (int i = 0; i < 10; i++) {
        Player* pi = &g_game->players[i];
        if (pi->active == 0)
            continue;
        if (pi->type != 1 && pi->type != 2)
            continue;
        if (pi->field_140 == 0)
            continue;
        if (pi->field_22 != 0)
            continue;
        for (int j = 0; j < 10; j++) {
            Player* pj = &g_game->players[j];
            if (pj->active != 0 && pj->type == 3 || pj->field_140 == 0
                || pj->field_22 != 0) {
                // Two sibling ifs, each with its own call: the jump layout and
                // registers follow the original.
                if (pj->active != 0 && pj->type == 3) {
                    if (pj->data->field_94 == 1
                        && (pi->t0[pj->field_146] == 0
                            || pi->t2[pj->field_146] == 0
                            || pi->t1[pj->field_146] == 0))
                        goto send;
                }
                if (pj->active != 0 && pj->type == 3) {
                    if (pi->t1[pj->field_146] == 0) {
                        SendPlayerEconomy(pi, pj, 1);
                        result = 0;
                    }
                }
                continue;
            send:
                SendPlayerEconomy(pi, pj, 1);
                result = 0;
            }
        }
    }
    SleepMilliseconds(0xfa);
    return result;
}

// Sends a 0x3a-byte packet (type 0x28) carrying a player's four short angles
// (+0xfc, +0xfe, +0x104, +0x106), four dwords (+0x98, +0x8c, +0xa8, +0xa4) and
// six doubles (+0xac, +0xbc, +0xcc, +0xb4, +0xc4, +0xd4) as floats, plus the
// caller's flag byte. It goes to `target` when there is one, otherwise to
// every occupied player slot of type 3 whose data->field_94 is 1.
// FUNCTION: 0x4573d0
void __stdcall SendPlayerEconomy(Player* player, Player* target,
                            unsigned char flag)
{
    if (player->active == 0)
        return;
    if (player->type != 1 && player->type != 2)
        return;
    if (player->field_22 != 0)
        return;

    Packet_004573d0 packet;
    packet.type = 0x28;
    packet.flag = flag;
    packet.field_2 = player->field_fc;
    packet.field_6 = player->field_fe;
    packet.field_a = player->field_104;
    packet.field_e = player->field_106;
    packet.field_12 = player->field_98;
    packet.field_16 = player->field_8c;
    packet.field_1a = player->field_a8;
    packet.field_1e = player->field_a4;
    packet.field_22 = (float)player->field_ac;
    packet.field_26 = (float)player->field_bc;
    packet.field_2a = (float)player->field_cc;
    packet.field_2e = (float)player->field_b4;
    packet.field_32 = (float)player->field_c4;
    packet.field_36 = (float)player->field_d4;

    if (target != 0) {
        if (target->field_22 == 0)
            SendPacketToPlayer(player->dpid, target->dpid, &packet, 0x3a);
        return;
    }

    for (int i = 0; i < 10; i++) {
        if (g_game->players[i].active == 0)
            continue;
        if (g_game->players[i].type != 3)
            continue;
        if (g_game->players[i].data->field_94 != 1)
            continue;
        if (g_game->players[i].field_22 != 0)
            continue;
        SendPacketToPlayer(player->dpid, g_game->players[i].dpid, &packet, 0x3a);
    }
}

// A received 0x3a-byte team packet (the same shape 0x4573d0 sends, the four
// angles as shorts in the packet dwords, the six doubles as floats) updates one
// player record, and then, when the packet's flag byte is set, tells the other
// local players about that player's team.
//
// The first pass looks for a local player (active, type 1 or 2) that already
// has the target player's team in its +0x129 byte table. If there is none, the
// packet's fields are copied into the player; the +0x146 byte is that player's
// team number, the two tables at +0x11e and +0x129 are eleven bytes each, one
// entry per team.
// FUNCTION: 0x457540
void __stdcall HandlePlayerEconomy(Packet_00457540* packet, Player* player)
{
    if (player == 0)
        return;
    if (player->field_22 != 0)
        return;

    int found = 0;
    for (int i = 0; i < 10; i++) {
        if (g_game->players[i].active != 0
            && (g_game->players[i].type == 1 || g_game->players[i].type == 2)
            && g_game->players[i].t1[player->field_146] != 0)
            found = 1;
    }

    if (found == 0) {
        player->field_fc = packet->field_2;
        player->field_fe = packet->field_6;
        player->field_104 = packet->field_a;
        player->field_106 = packet->field_e;
        player->field_98 = packet->field_12;
        player->field_8c = packet->field_16;
        player->field_a8 = packet->field_1a;
        player->field_a4 = packet->field_1e;
        player->field_ac = packet->field_22;
        player->field_bc = packet->field_26;
        player->field_cc = packet->field_2a;
        player->field_b4 = packet->field_2e;
        player->field_c4 = packet->field_32;
        player->field_d4 = packet->field_36;
    }

    if (packet->flag == 0)
        return;

    TeamPacket_00457540 team;
    team.type = 0x29;
    team.flag = 1;

    {
    for (int i = 0; i < 10; i++) {
        Player* p = &g_game->players[i];
        if (p->active == 0)
            continue;
        if (p->type != 1 && p->type != 2)
            continue;
        if (p->field_22 != 0)
            continue;
        p->t1[player->field_146] = 1;
        // Must be an if/else of constant stores, not a bool expression, which
        // adds a 32 bit temporary.
        if (p->t0[player->field_146] != 0)
            team.flag2 = 1;
        else
            team.flag2 = 0;
        SendPacketToPlayer(p->dpid, player->dpid, &team, 3);
        if (g_game->conditions->CheckVictory() != 0)
            continue;
        if (p->t0[player->field_146] != 0)
            continue;
        SendPlayerEconomy(p, player, 1);
    }
    }
}

// FUNCTION: 0x457710
int InitLobbiedConnection()
{
    if (*(int*)((char*)g_game + 0x4e5) != 0)
        return 1;

    if (HAPINET_initlobbiedconnection((char*)g_game + 0x14)) {
        HAPINET_initmultiplaydefaults((char*)g_game + 0x14);
        *(int*)((char*)g_game + 0x4f1) = 10;
        if (InitPacketManager(2, 100)) {
            SetMissionType(3);
            char* name = *(char**)(*(char**)(*(char**)((char*)g_game + 0x4e5) + 8) + 0x30);
            if (name != 0) {
                strncpy((char*)g_game + 0x14, name, 0x10);
                return 1;
            }

            char username[16];
            username[0] = 0;
            if (DAT_00512ca8[0] != 0) {
                strncat(username, DAT_00512ca8, 0x10);
            } else if (DAT_00512c98[0] != 0) {
                strncat(username, DAT_00512c98, 0x10);
            } else {
                DWORD size = 0xf;
                if (GetUserNameA(username, &size))
                    username[size] = 0;
                else
                    strcpy(username, "TotalA");
            }

            time_t now = time(0);
            struct tm* local = localtime(&now);
            int len = strlen(username);
            if (15 - len >= 7)
                sprintf(username + len, "_%02d%02d%02d", local->tm_hour, local->tm_min, local->tm_sec);
            strcpy((char*)g_game + 0x14, username);
            return 1;
        }
    }
    return 0;
}

// FUNCTION: 0x4578d0
void __stdcall SetPacketRate(int param_1)
{
    g_packetManager.SetDefaultSendPacing(param_1);
}

// Leaves the game: if the "leaving game" flag at +0x2a44 is set, drops every
// connected human or computer player (type 1 or 2) with RemovePlayer and
// calls ShutdownScoreTables. Then closes the network, installs a null callback, sets
// the game flag at +0x3923b and quits with the text for the local player's
// rejection reason (+0x22), the same text GetRejectReasonText returns, and no text at
// all when the reason is 0.
// FUNCTION: 0x4578f0
void __cdecl LeaveNetGameCallback(int)
{
    if (g_game->bits_2a44.leaving) {
        for (int i = 0; i < 10; i++) {
            if (g_game->players[i].active != 0
                && (g_game->players[i].type == 1 || g_game->players[i].type == 2))
                RemovePlayer(g_game->players[i].dpid);
        }
        ShutdownScoreTables();
    }
    HAPINET_quitgame(g_game->net);
    SetCloseHandler(0, 0);
    g_game->bits.flag2 = 1;
    int reason = g_game->players[g_game->localPlayer].rejectReason;
    char* text;
    if (reason) {
        switch (reason) {
        case 4:
            text = "You did not have the correct password";
            break;
        case 3:
            text = "The game is closed";
            break;
        case 5:
            text = "The game is full";
            break;
        case 6:
            text = "You have lost connection with the game";
            break;
        case 7:
            text = "You need a unit you don't have for this game";
            break;
        case 8:
            text = "You need a newer version of the game to enter";
            break;
        case 9:
            text = "No watching is allowed for this game";
            break;
        case 10:
            text = "The creator has left the game";
            break;
        default:
            text = "You were rejected from the game";
            break;
        }
    }
    else {
        text = 0;
    }
    QuitApp(text);
}

// Finds the first player (of 10) with a type set and bit 0 of its info flags
// (index 10 if none), then returns 1 if that player slot is active with type
// 1 or 2. Types and layout as in 0x457af0.
static inline unsigned char FindPlayer_00457a50()
{
    for (unsigned char i = 0; i < 10; i++) {
        if (g_game->players[i].type != 0 && (g_game->players[i].info->flags_97 & 1))
            return i;
    }
    return 10;
}

static inline int IsHuman(Player* p)
{
    if (p->active != 0 && (p->type == 1 || p->type == 2))
        return 1;
    return 0;
}

// FUNCTION: 0x457a50
int IsHostLocal()
{
    return IsHuman(&g_game->players[FindPlayer_00457a50()]);
}

// FUNCTION: 0x457af0
int CountHumanPlayers()
{
    int count = 0;
    for (int i = 0; i < 10; i++) {
        if ((g_game->players[i].active != 0 && g_game->players[i].type == 1)
            || (g_game->players[i].active != 0 && g_game->players[i].type == 3
                && g_game->players[i].data->field_94 == 1))
            count++;
    }
    return count;
}

// FUNCTION: 0x457b40
int CountComputerPlayers()
{
    int count = 0;
    for (int i = 0; i < 10; i++) {
        if ((g_game->players[i].active != 0 && g_game->players[i].type == 2)
            || (g_game->players[i].active != 0 && g_game->players[i].type == 3
                && g_game->players[i].data->field_94 == 2))
            count++;
    }
    return count;
}

// Counts the active players of type 2 (compare 0x457b40). The pointer local
// per iteration keeps the walk at the start of each entry; indexing twice
// makes MSVC walk the type field instead.
// FUNCTION: 0x457b90
int CountLocalComputerPlayers()
{
    int count = 0;
    for (int i = 0; i < 10; i++) {
        Player* p = &g_game->players[i];
        if (p->active != 0 && p->type == 2)
            count++;
    }
    return count;
}

// Counts active players of type 2 whose field_146 is not 10 (compare
// 0x457c10).
// FUNCTION: 0x457bc0
int CountActiveAIPlayers()
{
    int count = 0;
    for (int i = 0; i < 10; i++) {
        if (g_game->players[i].active != 0
            && g_game->players[i].type == 2
            && g_game->players[i].field_146 != 10
            && (g_game->players[i].field_144 != 0 || g_game->players[i].field_140 == 0))
            count++;
    }
    return count;
}

// FUNCTION: 0x457c10
int FUN_00457c10()
{
    int count = 0;
    for (int i = 0; i < 10; i++) {
        if (g_game->players[i].active != 0
            && (g_game->players[i].type == 1 || g_game->players[i].type == 2)
            && (g_game->players[i].type == 1 || g_game->players[i].type == 2 || g_game->players[i].type == 3)
            && g_game->players[i].field_146 != 10
            && (g_game->players[i].field_144 != 0 || g_game->players[i].field_140 == 0))
            count++;
    }
    return count;
}

// Counts the active players of type 3 (compare 0x457b90, type 2).
// FUNCTION: 0x457c80
int CountRemotePlayers()
{
    int count = 0;
    for (int i = 0; i < 10; i++) {
        Player* p = &g_game->players[i];
        if (p->active != 0 && p->type == 3)
            count++;
    }
    return count;
}

// FUNCTION: 0x457cb0
int CountCombatPlayers()
{
    int count = 0;
    for (int i = 0; i < 10; i++) {
        if (g_game->players[i].active != 0
            && (g_game->players[i].type == 1 || g_game->players[i].type == 2 || g_game->players[i].type == 3)
            && g_game->players[i].field_146 != 10
            && (g_game->players[i].field_144 != 0 || g_game->players[i].field_140 == 0)
            && (g_game->players[i].type == 1
                || (g_game->players[i].type == 3 && g_game->players[i].data->field_94 == 1))
            && !(g_game->players[i].data->flags_9b & 0x40))
            count++;
    }
    return count;
}

// Shared-resource tick: every 60 game ticks, if the local player is over its
// energy (or metal) reserve and has the matching flag, give up to a third (a
// half for metal) of the surplus to the neediest ally that shows up in the
// per-player array. Every 450 ticks it also sends the subtype-3 share packet
// (the same one 0x4571c0 sends) to every allied player.
// FUNCTION: 0x457d30
void __stdcall UpdateResourceSharing(Player* player)
{
    if ((g_game->flags_2a44 & 1) == 0)
        return;

    if (g_game->ticks % 60 == 0) {
        Player* found = player;
        if (player->info->bits_97.b1 && player->energy > player->field_e4) {
            for (int i = 0; i < 10; i++) {
                Player* p = &g_game->players[i];
                if (p->active != 0
                    && (p->type == 1 || p->type == 2 || p->type == 3)
                    && p->field_146 != 10
                    && (p->type == 1 || p->type == 2 || p->type == 3)
                    && (p->field_144 != 0 || p->field_140 == 0)
                    && p->type == 3
                    && p->info->field_94 == 1
                    && player->field_108[i] != 0
                    && p->energy < player->energy)
                    found = p;
            }
        }
        if (found != player && player->energy > player->field_e4) {
            float amount = (player->energy - player->field_e4) * 0.33333334f;
            float limit = found->field_a8f - found->energy;
            // The min() must be a ternary into a fresh local, not an if.
            float result = amount < limit ? amount : limit;
            amount = result;
            TransferEnergy(player->field_146, found->field_146, amount, 1);
        }

        found = player;
        if (player->info->bits_97.b2 && player->metal > player->field_e8) {
            for (int i = 0; i < 10; i++) {
                Player* p = &g_game->players[i];
                if (p->active != 0
                    && (p->type == 1 || p->type == 2 || p->type == 3)
                    && p->field_146 != 10
                    && (p->type == 1 || p->type == 2 || p->type == 3)
                    && (p->field_144 != 0 || p->field_140 == 0)
                    && p->type == 3
                    && p->info->field_94 == 1
                    && player->field_108[i] != 0
                    && p->metal < player->metal)
                    found = p;
            }
        }
        if (found != player && player->metal > player->field_e8) {
            float amount = (player->metal - player->field_e8) * 0.5f;
            float limit = found->field_a4f - found->metal;
            // The min() must be a ternary into a fresh local, not an if.
            float result = amount < limit ? amount : limit;
            amount = result;
            TransferMetal(player->field_146, found->field_146, amount, 1);
        }
    }

    if (g_game->ticks % 450 == 0) {
        // Must stay a standalone bitfield test, not a mask or shift.
        if (player->info->bits_97.b5) {
        for (int i = 0; i < 10; i++) {
            Player* p = &g_game->players[i];
            if (p->active != 0
                && (p->type == 1 || p->type == 2 || p->type == 3)
                && p->field_146 != 10
                && (p->type == 1 || p->type == 2 || p->type == 3)
                && (p->field_144 != 0 || p->field_140 == 0)
                && p->type == 3
                && p->info->field_94 == 1
                && player->field_108[i] != 0) {
                unsigned char a = player->field_146;
                unsigned char b = p->field_146;
                if (a != 10 && b != 10) {
                    Packet_00457d30 packet;
                    packet.type = 0x16;
                    packet.subtype = 3;
                    packet.from = PlayerDpid_00457d30(a);
                    packet.to = PlayerDpid_00457d30(b);
                    packet.zero = 0;
                    SendPacketToPlayer(PlayerDpid_00457d30(a), PlayerDpid_00457d30(b),
                                 &packet, sizeof(packet));
                }
            }
        }
    }
    }
}
