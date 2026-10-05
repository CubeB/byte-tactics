// Decompiled by Space Bunny Free. Names are provisional.
//
// Point a unit's aim object at a point: work out the two aim angles from the
// world position of the aim's weapon piece, let the two "can I shoot" helpers
// have a say, and, if both agree, fire and tell the other players.
//
// Three details decide the code shape. The frame is 0x30 with the 12 dwords
// holding the aim position Vec3 and the network packet; the packet is a
// packed struct, so the 0x24 bytes go out from the byte after the type byte.
// The packet's byte at +0x1a is never assigned in this function, yet the last
// store folds the type's flag bit 30 into it, so the compiler has to keep the
// byte in memory and re-load it. The `unit == 0 ? 0 : unit->field_a8` written
// as a ternary comes out with the two branches the other way round; the if/else
// with the test on the unit gives the original's `jne` over the null store.
//
// Point a unit's aim object at a point: work out the two aim angles, then
// ask the two "can I shoot" helpers and, if both agree, fire and tell the
// other players about it.

#include <math.h>

#pragma pack(push, 1)
struct Vec3 {
    int x;
    int y;
    int z;
};

struct Type_0049d9c0 {
    char unknown_0[0x10a];
    unsigned char team;               // +0x10a
    char unknown_10b[0x111 - 0x10b];
    unsigned int flags;               // +0x111
};

struct Aim_0049d9c0 {
    char unknown_0[0xc];
    Type_0049d9c0* type;              // +0xc
    char unknown_10[0x16 - 0x10];
    short field_16;                   // +0x16
    short field_18;                   // +0x18
    char unknown_1a;
    unsigned char weapon;             // +0x1b
};

struct Player_0049d9c0 {
    char unknown_0[4];
    int id;                           // +0x4
};

struct Unit {
    char unknown_0[0x66];
    short heading;                    // +0x66
    short pitch;                      // +0x68
    Vec3 pos;                         // +0x6a
    char unknown_76[0x96 - 0x76];
    Player_0049d9c0* player;          // +0x96
    char unknown_9a[0xa8 - 0x9a];
    short field_a8;                   // +0xa8
};

struct Packet_0049d9c0 {
    unsigned char type;               // +0x00
    Vec3 a;                           // +0x01
    Vec3 b;                           // +0x0d
    unsigned char team;               // +0x19
    unsigned char unknown_1a;         // +0x1a
    short heading;                    // +0x1b
    short pitch;                      // +0x1d
    short target_a8;                  // +0x1f
    short unit_a8;                    // +0x21
    unsigned char weapon;             // +0x23
};

struct Game {
    char unknown_0[0x2a44];
    unsigned char flags;              // +0x2a44
};
#pragma pack(pop)

union Fixed {
    int value;
    struct {
        unsigned short fraction;
        short whole;
    };
};

extern Game* g_game;

void __stdcall GetWeaponPiecePosition(Unit* obj, Vec3* out, unsigned char weapon, int piece);
int __cdecl FUN_004b715a(int x, int z);
int __stdcall FUN_0049d880(Unit* unit, Aim_0049d9c0* aim, short angle1, short angle2);
int __stdcall FUN_0049c9c0(Aim_0049d9c0* aim, Unit* unit, Vec3* aimPos,
                           Vec3* point, Unit* target);
int __stdcall BroadcastPacket(int player, void* data, int size);

// FUNCTION: 0x49d9c0
int __stdcall FUN_0049d9c0(Unit* unit, Aim_0049d9c0* aim,
                           Unit* target, Vec3* point)
{
    Vec3 p;
    GetWeaponPiecePosition(unit, &p, aim->weapon >> 2 & 3, -1);
    int dx = p.x - point->x;
    Fixed dy;
    dy.value = p.y - point->y;
    int dz = p.z - point->z;
    aim->field_16 = (short)FUN_004b715a(dx, dz);
    aim->field_18 = (short)FUN_004b715a(-dy.whole,
                                       (short)((int)_hypot((double)dx, (double)dz) >> 16));
    if (FUN_0049d880(unit, aim, unit->heading, unit->pitch)) {
        if (FUN_0049c9c0(aim, unit, &p, point, target)) {
            if (g_game->flags & 1) {
                Packet_0049d9c0 msg;
                msg.type = 0xd;
                msg.a = unit->pos;
                msg.b = *point;
                msg.team = aim->type->team;
                msg.weapon = aim->weapon >> 2 & 3;
                if (unit == 0)
                    msg.unit_a8 = 0;
                else
                    msg.unit_a8 = unit->field_a8;
                msg.target_a8 = target ? target->field_a8 : 0;
                msg.heading = aim->field_16;
                msg.pitch = aim->field_18;
                msg.unknown_1a = msg.unknown_1a ^ ((aim->type->flags >> 30 ^ msg.unknown_1a) & 1);
                BroadcastPacket(unit->player->id, &msg, 0x24);
            }
            return 1;
        }
    }
    return 0;
}
