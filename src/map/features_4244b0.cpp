// Decompiled by Claude Opus 5.5. Names are provisional.
// Needs <windows.h> (tools/headers.py): without it the spot address is
// scaled in ecx before the table base is loaded.
#include <windows.h>

#pragma pack(push, 1)
struct PlayerData_004244b0 {
    char unknown_0[0x97];
    unsigned char flags;               // +0x97
};

struct Player_004244b0 {
    char unknown_0[0x27];
    PlayerData_004244b0* data;         // +0x27
    char unknown_2b[0x14b - 0x2b];
};

struct Feature_004244b0 {
    char unknown_0[0xea];
    unsigned short damage;             // +0xea
    char unknown_ec[0xfe - 0xec];
    unsigned short flag0 : 1;          // +0xfe
    unsigned short bits1 : 3;
    unsigned short flag4 : 1;
    unsigned short bits5 : 4;
    unsigned short flag9 : 1;
    unsigned short bits10 : 6;
};

struct Spot_004244b0 {
    char unknown_0[0x26];
    unsigned short damage;             // +0x26
    short x;                           // +0x28
    short z;                           // +0x2a
    char unknown_2c[0x30 - 0x2c];
};

struct Cell_004244b0 {
    char unknown_0[8];
    unsigned short feature;            // +0x8
    unsigned short spot;               // +0xa
    unsigned char flags;               // +0xc
};

struct Weapon_004244b0 {
    char unknown_0[0xd4];
    unsigned short damage;             // +0xd4
    char unknown_d6[0x10a - 0xd6];
    unsigned char kind;                // +0x10a
    char flag_10b;                     // +0x10b
};

struct Packet_004244b0 {
    unsigned char type;
    unsigned char sub;
    short x;
    short z;
};
#pragma pack(pop)

class Class_00435100 {
public:
    int FUN_00435100();
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x1b63];
    Player_004244b0 players[10];       // +0x1b63
    char unknown_2851[0x2a43 - 0x2851];
    unsigned char playerIndex;         // +0x2a43
    char unknown_2a44[0x1420b - 0x2a44];
    Spot_004244b0* spots;              // +0x1420b
    char unknown_1420f[0x1426f - 0x1420f];
    Feature_004244b0* features;        // +0x1426f
    char unknown_14273[0x37f2f - 0x14273];
    unsigned char flags_37f2f;         // +0x37f2f
    char unknown_37f30[0x391e9 - 0x37f30];
    Class_00435100* net;               // +0x391e9
};
#pragma pack(pop)

extern Game* g_game;

int GetLocalDpid();
int GetHostDpid();
int __stdcall SendPacketToPlayer(int from, int to, void* packet, int size);
int __stdcall BroadcastPacket(int player, void* data, int size);
void __stdcall StartFeatureBurning(int x, int z, int flag);
void __stdcall KillFeature(int x, int z, int flag);

// FUNCTION: 0x4244b0
void __stdcall DamageFeature(Cell_004244b0* cell, int x, int z, Weapon_004244b0* weapon)
{
    if (!(g_game->flags_37f2f & 8))
        return;
    if (cell->feature >= 0xfffb)
        return;
    Feature_004244b0* f = &g_game->features[cell->feature];
    if (f->flag9)
        return;
    Packet_004244b0 packet;
    int send;
    if (g_game->net->FUN_00435100() == 3) {
        if (!(g_game->players[g_game->playerIndex].data->flags & 1)) {
            packet.type = 0xf;
            packet.sub = weapon->kind;
            packet.x = x;
            packet.z = z;
            SendPacketToPlayer(GetLocalDpid(), GetHostDpid(), &packet, 6);
            return;
        }
        send = 1;
        packet.sub = 0xfc;
    } else {
        send = 0;
    }
    if (f->flag4 && weapon->flag_10b && !(cell->flags & 1)) {
        StartFeatureBurning(x, z, 0);
    } else if (cell->flags & 1) {
        if (!f->flag0) {
            Spot_004244b0* s = &g_game->spots[cell->spot];
            if (s->x != x)
                return;
            if (s->z != z)
                return;
            s->damage += weapon->damage;
            if (s->damage >= f->damage) {
                KillFeature(s->x, s->z, 0);
                packet.sub = 0xfd;
            }
        }
    } else {
        int d = weapon->damage + cell->spot;
        if (d >= f->damage) {
            KillFeature(x, z, 0);
            packet.sub = 0xfd;
        } else {
            cell->spot = d;
        }
    }
    if (send && packet.sub > 0xfc) {
        packet.type = 0xf;
        packet.x = x;
        packet.z = z;
        BroadcastPacket(GetLocalDpid(), &packet, 6);
    }
}
