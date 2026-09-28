// Decompiled by GPT-5.6-Terra. Names are provisional.
// Partial: 57.7%. The remaining difference is the allocation of source, target
// and aim to ebp, edi and ebx instead of the original ebx, ebp and edi.
#pragma pack(push, 1)
struct Vec3_0049db70 {
    int x;
    int y;
    int z;
};

struct Flags_0049db70 {
    unsigned int unknown_0 : 30;
    unsigned int special : 1;
    unsigned int unknown_31 : 1;
};

struct Def_0049db70 {
    char unknown_0[0x10a];
    unsigned char field_10a;
    char unknown_10b[0x111 - 0x10b];
    Flags_0049db70 flags;
};

struct Shot_0049db70 {
    char unknown_0[8];
    int piece;
    Def_0049db70* def;
    char unknown_10[6];
    short heading;
    short pitch;
    char unknown_1a;
    unsigned char weapon;
};

struct Kind_0049db70 {
    char unknown_0[4];
    int player;
};

struct Object_0049db70 {
    char unknown_0[0x96];
    Kind_0049db70* kind;
    char unknown_9a[0xa8 - 0x9a];
    short team;
};

struct Game_0049db70 {
    char unknown_0[0x2a44];
    unsigned short flags;
};

struct Packet_0049db70 {
    unsigned char type;
    Vec3_0049db70 pos;
    Vec3_0049db70 aim;
    unsigned char field_19;
    unsigned char flag : 1;
    short heading;
    short pitch;
    short target_team;
    short source_team;
    unsigned char weapon;
};
#pragma pack(pop)

extern Game_0049db70* g_game;

void __stdcall FUN_0043e240(Object_0049db70* obj, Vec3_0049db70* out, unsigned char weapon, int piece);
short __cdecl FUN_004b715a(int a, int b);
int* __stdcall FUN_0049d120(Object_0049db70* obj, unsigned int weapon);
int __stdcall FUN_0049cc20(Shot_0049db70* shot, Object_0049db70* source, Vec3_0049db70* pos,
                           Vec3_0049db70* aim, Object_0049db70* target, int* param_6);
int __stdcall FUN_00451df0(int player, void* data, int size);
double __cdecl _hypot(double x, double y);
long __cdecl _ftol();

// FUNCTION: 0x49db70
int __stdcall FUN_0049db70(Object_0049db70* source, Shot_0049db70* shot,
                           Object_0049db70* target, Vec3_0049db70* aim)
{
    Vec3_0049db70 pos;
    if (shot->piece != 0) {
        FUN_0043e240(source, &pos, (shot->weapon >> 2) & 3, -1);
        int dy = pos.y - aim->y;
        int dx = pos.x - aim->x;
        int dz = pos.z - aim->z;
        shot->heading = FUN_004b715a(dx, dz);
        short length = (short)(((int)_hypot((double)dx, (double)dz)) >> 16);
        shot->pitch = FUN_004b715a(-dy, length);
        int* p = 0;
        if (shot->def->flags.special) {
            p = FUN_0049d120(source, (shot->weapon >> 2) & 3);
            if (!p)
                return 0;
        }
        if (FUN_0049cc20(shot, source, &pos, aim, target, p)) {
            if (g_game->flags & 1) {
                Packet_0049db70 packet;
                packet.type = 0xd;
                packet.pos = pos;
                packet.aim = *aim;
                packet.field_19 = shot->def->field_10a;
                packet.weapon = (shot->weapon >> 2) & 3;
                packet.source_team = source ? source->team : 0;
                packet.target_team = target ? target->team : 0;
                packet.heading = shot->heading;
                packet.pitch = shot->pitch;
                packet.flag = shot->def->flags.special;
                FUN_00451df0(source->kind->player, &packet, 0x24);
            }
            shot->piece = 0;
            shot->weapon &= 0xfe;
            return 1;
        }
    }
    return 0;
}
