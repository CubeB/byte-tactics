// Decompiled by Sonnet 5.5. Names are provisional.
// Moves a unit to another player. Does nothing when the unit already belongs
// to that player, is not live (0x10000000) or is flagged 0x4000. When the
// current owner is a real player (state 1 or 2) and the new owner is in state
// 3, the unit is instead handed over in place: it is marked (+0xfb = 0x96),
// a 0x18 byte record describing it is sent to the new owner with 0x451df0 and
// it is damaged for the full 30000 (0x489bb0). Otherwise a copy is created
// for the new owner with 0x485f50 and filled either from the record passed in
// or from the old unit, and the old unit's state bits are moved across.

class Class_00490520 {
public:
    void FUN_00490520(struct Unit* unit);
};

class Class_0048b090 {
public:
    void FUN_0048b090(unsigned char mask, int set);
};

struct Pos_00488570 {
    int x, y, z;
};

#pragma pack(push, 1)
struct Tail_00488570 {
    int a;
    unsigned short b;
};

struct Player_00488570 {
    int f0;                            // +0x0
    int f4;                            // +0x4
    char unknown_8[0x73 - 8];
    unsigned char f73;                 // +0x73
    char unknown_74[0x146 - 0x74];
    unsigned char f146;                // +0x146
    char unknown_147[0x14b - 0x147];
};

struct Packet_00488570 {               // 0x18 bytes
    unsigned char type;                // +0x0
    short team;                        // +0x1
    int who;                           // +0x3
    int x;                             // +0x7
    int y;                             // +0xb
    Tail_00488570 tail;                // +0xf
    unsigned char b15;                 // +0x15
    unsigned char b16;                 // +0x16
    unsigned char b17;                 // +0x17
};

struct Unit {                          // 0x118 bytes
    char unknown_0[0x1e];
    unsigned char b1e;                 // +0x1e
    unsigned char b1f;                 // +0x1f
    char unknown_20[0x3a - 0x20];
    unsigned char b3a;                 // +0x3a
    unsigned char b3b;                 // +0x3b
    char unknown_3c[0x56 - 0x3c];
    unsigned char b56;                 // +0x56
    unsigned char b57;                 // +0x57
    char unknown_58[0x64 - 0x58];
    Tail_00488570 tail;                // +0x64
    Pos_00488570 pos;                  // +0x6a
    char unknown_76[0x96 - 0x76];
    Player_00488570* player;           // +0x96
    char unknown_9a[0xa6 - 0x9a];
    unsigned short type;               // +0xa6
    short team;                        // +0xa8
    char unknown_aa[0xfb - 0xaa];
    int fb;                            // +0xfb
    char unknown_ff[0x104 - 0xff];
    union {
        float speed;                   // +0x104
        int speed_bits;
    };
    short s108;                        // +0x108
    char unknown_10a[0x10e - 0x10a];
    unsigned char state;               // +0x10e
    char unknown_10f[0x110 - 0x10f];
    unsigned int flags;                // +0x110
    char unknown_114[0x118 - 0x114];
};

struct Game_00488570 {
    char unknown_0[0x391ed];
    Class_00490520* list;              // +0x391ed
};
#pragma pack(pop)

extern Game_00488570* g_game;

int __stdcall FUN_00450010(Player_00488570* player);
void __stdcall FUN_00451df0(int who, Packet_00488570* packet, int size);
Unit* __stdcall FUN_00485f50(unsigned char player, unsigned short type, Pos_00488570 pos,
                                      int param_5, int mode, unsigned short id);
void __stdcall FUN_00489bb0(Unit* source, Unit* target, int amount, int type,
                            unsigned short extra);

// FUNCTION: 0x488570
void __stdcall FUN_00488570(Unit* unit, Player_00488570* other, Packet_00488570* p)
{
    if (unit->player == other)
        return;
    if (!(unit->flags & 0x10000000))
        return;
    if (unit->flags & 0x4000)
        return;
    g_game->list->FUN_00490520(unit);

    Player_00488570* cur = unit->player;
    if (cur->f0 != 0 && (cur->f73 == 1 || cur->f73 == 2)) {
        if (other->f0 == 0)
            return;
        if (other->f73 == 3) {
            Packet_00488570 pk;
            unit->flags &= ~0x10;
            unit->fb = 0x96;
            pk.type = 0x14;
            pk.who = FUN_00450010(other);
            pk.team = unit->team;
            pk.x = (int)unit->speed;
            pk.y = unit->s108;
            pk.tail = unit->tail;
            unsigned char a = unit->b1f & 2;
            pk.b15 = a ? unit->b1e : 0;
            pk.b16 = a ? unit->b3a : 0;
            pk.b17 = a ? unit->b56 : 0;
            FUN_00451df0(unit->player->f4, &pk, 0x18);
            FUN_00489bb0(0, unit, 30000, 4, 0);
            return;
        }
    }
    if (other->f0 == 0)
        return;
    if (other->f73 != 1 && other->f73 != 2)
        return;

    Unit* n = FUN_00485f50(other->f146, unit->type, unit->pos, 1, unit->flags & 3, 0);
    if (!n)
        return;
    n->flags &= 0xffc3ffff;
    if (p) {
        n->s108 = (short)p->y;
        n->speed = (float)p->x;
        n->tail = p->tail;
        if (n->b1f & 2)
            n->b1e = p->b15;
        if (n->b3b & 2)
            n->b3a = p->b16;
        if (n->b57 & 2)
            n->b56 = p->b17;
    } else {
        n->s108 = unit->s108;
        n->speed_bits = unit->speed_bits;
        n->tail = unit->tail;
        if (n->b1f & 2)
            n->b1e = unit->b1e;
        if (n->b3b & 2)
            n->b3a = unit->b3a;
        if (n->b57 & 2)
            n->b56 = unit->b56;
        FUN_00489bb0(0, unit, 30000, 4, 0);
    }
    ((Class_0048b090*)n)->FUN_0048b090(unit->state, 1);
    ((Class_0048b090*)n)->FUN_0048b090(~unit->state, 0);
}
