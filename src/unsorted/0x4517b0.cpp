// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
//
// Joins the game as the local player: picks the player/session name (from the
// network object or the two default strings), fills in the player's info block
// and, depending on whether a DirectPlay interface is present, either joins the
// lobby in a worker thread (FUN_00451640) or connects through the local net
// object (FUN_004c9fd0). On success it starts the game (FUN_00451220,
// FUN_004ca840), broadcasts every playing player's 0xb9-byte data block and
// 6-byte message (the same code as FUN_00450f90), and finishes with
// FUN_004526c0(0).
//
// What made this match:
// - The user name read into the 0x100-byte stack buffer is never read again:
//   the original computes it with GetUserNameA/strcpy and drops it, so the
//   buffer has to stay dead here too (an early `return 0` would move the
//   epilogues, so the whole body sits under `if (player == localPlayer)`).
// - info+0x9d is a 16-bit 1-bit field, not a byte field. With a byte field
//   MSVC emits a load/or/store (9 bytes) for `field_9d = 1`; with the 16-bit
//   bitfield it emits the single `or byte [eax+0x9d],1` the original has.
// - info+0x97 is likewise a 1-bit field: `ready = net->field_4 >> 1` gives the
//   (old ^ value) & 1 ^ old word sequence, and `ready = 0` the `and word,
//   0xfffe`.
// - The flag test reloads g_game->field_4e5 and reads a byte of its +4 dword,
//   which is why it is written as a cast rather than through a cached local.
#include <windows.h>
#include <string.h>

struct Guid_4517b0 {
    int data[4];
};

#pragma pack(push, 1)
struct PlayerData_4517b0 {
    char unknown_0[0x80];
    char name[0xb];                    // +0x80
    unsigned short field_8b;           // +0x8b
    unsigned short field_8d;           // +0x8d
    char unknown_8f[1];                // +0x8f
    int id;                            // +0x90
    char unknown_94[2];                // +0x94
    unsigned char field_96;            // +0x96
    unsigned short ready : 1;          // +0x97
    unsigned short rest_97 : 15;       // +0x97
    char unknown_99[2];                // +0x99
    unsigned short field_9b;           // +0x9b
    unsigned short field_9d : 1;       // +0x9d
    char unknown_9f[0xb9 - 0x9f];
};

struct Player_4517b0 {
    int active;                        // +0x0
    int id;                            // +0x4
    char unknown_8[0x27 - 8];
    PlayerData_4517b0* info;           // +0x27
    char unknown_2b[0x73 - 0x2b];
    char state;                        // +0x73
    char unknown_74[0x13f - 0x74];
    char field_13f;                    // +0x13f
    char unknown_140[0x14b - 0x140];
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

struct Game_4517b0 {
    char unknown_0[0x14];
    char net[0x4e5 - 0x14];            // +0x14
    Net_4517b0* field_4e5;             // +0x4e5
    char unknown_4e9[0x1b63 - 0x4e9];
    Player_4517b0 players[10];         // +0x1b63
    char unknown_2851[0x2a38 - 0x2851];
    unsigned char* buffer;             // +0x2a38
    unsigned short field_2a3c;         // +0x2a3c
    char unknown_2a3e[0x2a42 - 0x2a3e];
    unsigned char localPlayer;         // +0x2a42
    char unknown_2a43[1];
    unsigned char flags_2a44;          // +0x2a44
    char unknown_2a45[0x2bd2 - 0x2a45];
    char field_2bd2[0x11];             // +0x2bd2
    char field_2be3[0xb];              // +0x2be3
    char unknown_2bee[0x37f1b - 0x2bee];
    unsigned short field_37f1b;        // +0x37f1b
    char unknown_37f1d[0x37f1f - 0x37f1d];
    unsigned short field_37f1f;        // +0x37f1f
};

struct Packet_4517b0 {
    unsigned char type;                // +0x0
    PlayerData_4517b0 data;            // +0x1
};
#pragma pack(pop)

struct Obj_4517b0 {
    char unknown_0[0xf0];
    unsigned short bit0 : 1;
    unsigned short flag : 1;           // +0xf0, mask 2
};

class Class_004618a0 {
public:
    int FUN_004618a0(int param_1);
};

extern Game_4517b0* g_game;
extern char DAT_00512d48;
extern char DAT_005119b8;
extern char DAT_00512d28;
extern int DAT_00506dbc;
extern Class_004618a0 DAT_00513000;

int FUN_0045b660(void);
void FUN_004644d0(void);
void FUN_00450530(void);
int __stdcall FUN_00451640(Player_4517b0* p);
int __stdcall FUN_00451df0(int player, void* data, int size);
int __stdcall FUN_004526c0(int param);
int __stdcall FUN_00451220(int player, int param);
int __stdcall FUN_004c9fd0(void* net, Guid_4517b0 guid);
int __stdcall FUN_004ca840(void* net, void* session, void* callback, void* context,
                           unsigned long flags);
void __stdcall FUN_004515d0(int, int, int, int, int);
Obj_4517b0* FUN_004b6220(void);
void FUN_004b5910(void);
char* __stdcall FUN_004c5740(char* s);
void __stdcall FUN_004b6290(char* msg);

// FUNCTION: 0x4517b0
int __stdcall FUN_004517b0(Guid_4517b0 guid, int player)
{
    char name[0x100];
    Packet_4517b0 packet;
    DWORD size;

    if (player == g_game->localPlayer) {
        Player_4517b0* p = &g_game->players[player];

        Net_4517b0* net = g_game->field_4e5;
        if (net != 0) {
            int v = FUN_0045b660();
            char* s = &DAT_00512d48;
            if (v == 0)
                s = &DAT_005119b8;

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
                p->info->field_9d = 1;
                if ((*(unsigned char*)((char*)g_game->field_4e5 + 4) & 2) != 0)
                    lstrcpynA(g_game->field_2be3, &DAT_00512d28, 0xb);
            }

            FUN_004644d0();
        }

        if (strlen(g_game->field_2bd2) == 0) {
            size = 0x100;
            GetUserNameA(name, &size);
        } else {
            strcpy(name, g_game->field_2bd2);
        }

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
            result = FUN_00451640(p);
            if (result == 0) {
                if (FUN_004b6220()->flag) {
                    FUN_004b5910();
                    Sleep(500);
                }
                FUN_004b6290(FUN_004c5740("Unable to connect to DirectPlay lobby."));
            }
        } else {
            result = FUN_004c9fd0((Net_4517b0*)((char*)g_game + 0x14), guid);
        }
        if (result == 0)
            return 0;

        FUN_00451220(player, 1);
        FUN_004ca840((Net_4517b0*)((char*)g_game + 0x14), 0, (void*)FUN_004515d0, 0, 0);
        if (g_game->flags_2a44 & 1) {
            for (int i = 0; i < 10; i++) {
                Player_4517b0* q = &g_game->players[i];
                if (q->active != 0 && (q->state == 1 || q->state == 2)) {
                    packet.data = *q->info;
                    packet.data.id = q->id;
                    packet.type = 0x20;
                    FUN_00451df0(q->id, &packet, sizeof(packet));
                    if (q->active != 0 && (q->state == 1 || q->state == 2)) {
                        unsigned char* msg = g_game->buffer;
                        msg[0] = 0x24;
                        *(int*)(msg + 1) = q->id;
                        msg[5] = q->field_13f;
                        FUN_00451df0(q->id, msg, 6);
                        if (DAT_00506dbc != 0)
                            DAT_00513000.FUN_004618a0(1);
                    }
                }
            }
            FUN_00450530();
            DAT_00513000.FUN_004618a0(1);
        }

        FUN_004526c0(0);
        return 1;
    }
    return 0;
}
