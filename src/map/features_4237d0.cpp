// Decompiled by Claude Opus 5.5. Names are provisional.
// Reclaims the feature at a world position: the unit gets the feature's
// resources (an AI player only half on the easy and 70% on the medium
// setting), the feature is replaced by its reclaimed state (KillFeature
// with flag 1) and the reclaim is sent to the other players.
// No header on purpose: <windows.h> reorders the width multiply operands.
#pragma pack(push, 1)
struct Feature_004237d0 {
    char unknown_0[0xec];
    float energy;                      // +0xec
    float metal;                       // +0xf0
    char unknown_f4[0xfe - 0xf4];
    unsigned char flags;               // +0xfe
    char unknown_ff[0x100 - 0xff];
};

struct Cell_004237d0 {
    char unknown_0[8];
    unsigned short feature;            // +0x8
    unsigned char offsetY;             // +0xa
    unsigned char offsetX;             // +0xb
    unsigned char flags;               // +0xc
};

struct Player_004237d0 {
    int active;                        // +0x0
    char unknown_4[0x73 - 0x4];
    unsigned char type;                // +0x73
};

struct PlayerInfo_004237d0 {
    char unknown_0[4];
    int id;                            // +0x4
};

struct Unit {
    char unknown_0[0x96];
    PlayerInfo_004237d0* info;         // +0x96
    char unknown_9a[0xbc - 0x9a];
    float energy;                      // +0xbc
    char unknown_c0[0xd4 - 0xc0];
    float metal;                       // +0xd4
    char unknown_d8[0xec - 0xd8];
    Player_004237d0* player;           // +0xec
};

struct Packet_004237d0 {
    unsigned char type;
    unsigned char sub;
    short x;
    short z;
};
#pragma pack(pop)

class Mission {
public:
    int FUN_00435100();
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x14233];
    int width;                         // +0x14233
    char unknown_14237[0x14253 - 0x14237];
    int featureCount;                  // +0x14253
    char unknown_14257[0x1426f - 0x14257];
    Feature_004237d0* features;        // +0x1426f
    char unknown_14273[0x37eee - 0x14273];
    int difficulty;                    // +0x37eee
    char unknown_37ef2[0x391e9 - 0x37ef2];
    Mission* net;                      // +0x391e9
};
#pragma pack(pop)

struct Vec3_004237d0 {
    int x, y, z;
};

extern Game* g_game;

Cell_004237d0* __stdcall GetMapCell(int x, int y);
void __stdcall KillFeature(int x, int z, int flag);
int __stdcall BroadcastPacket(int player, void* data, int size);

static inline Feature_004237d0* GetFeature(Cell_004237d0* cell)
{
    if (cell == 0)
        return 0;
    if (cell->feature < 0xfffb) {
        if (cell->feature >= g_game->featureCount)
            return 0;
        return &g_game->features[cell->feature];
    }
    if (cell->feature != 0xfffe)
        return 0;
    // Full expression twice, no locals or `cell -=`; ends with the feature return.
    if ((cell - (cell->offsetY * g_game->width + cell->offsetX))->feature >= 0xfffb)
        return 0;
    return &g_game->features[(cell - (cell->offsetY * g_game->width + cell->offsetX))->feature];
}

// Takes the field by reference and the amount as a double; the metal add stays plain code.
static inline void AddScaled(Unit* unit, float& res, double amount)
{
    if (unit->player->active && unit->player->type == 2) {
        switch (g_game->difficulty) {
        case 1:
            res += amount * 0.7;
            break;
        case 0:
            res += amount * 0.5;
            break;
        default:
            res += amount;
            break;
        }
    } else {
        res += amount;
    }
}

// FUNCTION: 0x4237d0
int __stdcall ReclaimFeature(Unit* unit, Vec3_004237d0* pos)
{
    int x = pos->x / 0x100000;
    int z = pos->z / 0x100000;
    Cell_004237d0* cell = GetMapCell(x, z);
    Feature_004237d0* f = GetFeature(GetMapCell(x, z));
    if (f == 0)
        return 0;
    if ((cell->flags & 1) && (f->flags & 1))
        return 0;
    AddScaled(unit, unit->energy, f->energy);
    float metal = f->metal;
    if (unit->player->active && unit->player->type == 2) {
        switch (g_game->difficulty) {
        case 1:
            unit->metal += metal * 0.7;
            break;
        case 0:
            unit->metal += metal * 0.5;
            break;
        default:
            unit->metal += metal;
            break;
        }
    } else {
        unit->metal += metal;
    }
    KillFeature(x, z, 1);
    if (g_game->net->FUN_00435100() == 3) {
        Packet_004237d0 packet;
        packet.type = 0xf;
        packet.sub = 0xff;
        packet.x = x;
        packet.z = z;
        BroadcastPacket(unit->info->id, &packet, 6);
    }
    return 1;
}
