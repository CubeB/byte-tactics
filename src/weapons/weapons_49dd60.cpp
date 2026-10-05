// Decompiled by GPT-5.6-Terra. Names are provisional.
#pragma pack(push, 1)
struct Vec3_0049dd60 {
    int x;
    int y;
    int z;
};

struct Type_0049dd60 {
    char unknown_0[0x10a];
    unsigned char team;
    char unknown_10b[0x111 - 0x10b];
    unsigned int flags;
};

struct Aim_0049dd60 {
    char unknown_0[0xc];
    Type_0049dd60* type;
    char unknown_10[0x16 - 0x10];
    short field_16;
    short field_18;
    char unknown_1a;
    unsigned char weapon;
};

struct Player_0049dd60 {
    char unknown_0[4];
    int id;
};

struct Owner_0049dd60 {
    char unknown_0[0x20];
    int id;
};

struct Unit {
    Owner_0049dd60* owner;
    char unknown_4[0x66 - 4];
    short heading;
    char unknown_68[0x96 - 0x68];
    Player_0049dd60* player;
    char unknown_9a[0xa8 - 0x9a];
    short field_a8;
};

struct Projectile_0049dd60 {
    char unknown_0[0x1c];
    int field_1c;
    int field_20;
    int field_24;
    char unknown_28[0x36 - 0x28];
    short field_36;
    char unknown_38[0x3a - 0x38];
    int field_3a;
    char unknown_3e[0x4e - 0x3e];
    int field_4e;
    char unknown_52[0x69 - 0x52];
    unsigned short flags;
};

struct Game {
    char unknown_0[0x2a44];
    unsigned char flags;
    char unknown_2a45[0x141f3 - 0x2a45];
    int projectile_count;
    Projectile_0049dd60* projectiles;
    char unknown_141fb[0x38a47 - 0x141fb];
    int field_38a47;
};

struct Packet_0049dd60 {
    unsigned char type;
    Vec3_0049dd60 a;
    Vec3_0049dd60 b;
    unsigned char team;
    unsigned char unknown_1a;
    short heading;
    short pitch;
    short target_a8;
    short unit_a8;
    unsigned char weapon;
};
#pragma pack(pop)

extern Game* g_game;

void __stdcall FUN_0043e240(Unit*, Vec3_0049dd60*, unsigned char, int);
void __stdcall FUN_0049c740(Projectile_0049dd60*, Type_0049dd60*, Vec3_0049dd60*, int, int, Unit*);
int __cdecl FUN_004b70ef(short, int);
int __cdecl FUN_004b7123(short, int);
int __stdcall BroadcastPacket(int, void*, int);

// FUNCTION: 0x49dd60
int __stdcall FUN_0049dd60(Unit* unit, Aim_0049dd60* aim,
                           Unit* target, Vec3_0049dd60* point)
{
    Vec3_0049dd60 p;
    FUN_0043e240(unit, &p, aim->weapon >> 2 & 3, -1);
    Projectile_0049dd60* projectile = 0;
    if (g_game->projectile_count < 300) {
        projectile = &g_game->projectiles[g_game->projectile_count++];
        projectile->flags &= ~2;
        projectile->field_4e = 0;
    }
    if (projectile != 0) {
        FUN_0049c740(projectile, aim->type, &p, 0, g_game->field_38a47, unit);
        projectile->field_36 = unit->heading;
        projectile->field_3a = 0;
        projectile->field_20 = 0;
        projectile->field_1c = -FUN_004b70ef(projectile->field_36, unit->owner->id);
        projectile->field_24 = -FUN_004b7123(projectile->field_36, unit->owner->id);
        if (g_game->flags & 1) {
            Packet_0049dd60 packet;
            packet.type = 0xd;
            packet.a = p;
            packet.b = *point;
            packet.team = aim->type->team;
            packet.weapon = aim->weapon >> 2 & 3;
            if (unit == 0)
                packet.unit_a8 = 0;
            else
                packet.unit_a8 = unit->field_a8;
            if (target == 0)
                packet.target_a8 = 0;
            else
                packet.target_a8 = target->field_a8;
            packet.heading = aim->field_16;
            packet.pitch = aim->field_18;
            packet.unknown_1a = packet.unknown_1a ^ ((aim->type->flags >> 30 ^ packet.unknown_1a) & 1);
            BroadcastPacket(unit->player->id, &packet, 0x24);
        }
        return 1;
    }
    return 0;
}
