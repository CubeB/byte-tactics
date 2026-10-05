// Decompiled by space-bunny-free. Names are provisional.
// Searches the local player's unit list for a live unit whose type is in the
// "CTRL_F" category and whose field at +0xac holds the given id. Returns 1 on
// the first match. The unit's own flags at +0x110 are read as a byte here, but
// the owner object's flags at the same offset are read as a dword (0x40000000),
// so the two are separate struct types. The guard before the loop (u > end) and
// the test at the bottom are both present in the original, and the player
// address is computed into eax and left dead, which is what the &players[n]
// expression costs here.

#pragma pack(push, 1)
struct Team_0048dc30 {
    char unknown_0[0x110];
    unsigned int flags;                // +0x110
};

struct Unit_0048dc30 {
    char unknown_0[0x86];
    Team_0048dc30* owner;              // +0x86
    char unknown_8a[0xa6 - 0x8a];
    unsigned short type;               // +0xa6
    char unknown_a8[0xac - 0xa8];
    int field_ac;                      // +0xac
    char unknown_b0[0xfb - 0xb0];
    int field_fb;                      // +0xfb
    char unknown_ff[0x104 - 0xff];
    float field_104;                   // +0x104
    char unknown_108[0x110 - 0x108];
    unsigned char flags;               // +0x110
    char unknown_111[0x118 - 0x111];
};

struct Player_0048dc30 {
    char unknown_0[0x67];
    Unit_0048dc30* unitsBegin;         // +0x67
    Unit_0048dc30* unitsEnd;           // +0x6b
    char unknown_6f[0x14b - 0x6f];
};

struct Game_0048dc30 {
    char unknown_0[0x1b63];
    Player_0048dc30 players[10];       // +0x1b63
    char unknown_2851[0x2a42 - 0x2851];
    unsigned char localPlayer;         // +0x2a42
};
#pragma pack(pop)

extern Game_0048dc30* g_game;

// 0x40-byte set of unit type ids, as in 0x488d30.
class Class_00488d30 {
public:
    int bits[16];
};

Class_00488d30* __stdcall FUN_00488c50(char* name);

// The bit test was an inlined helper taking the type as unsigned short.
static inline int TestBit(Class_00488d30* set, unsigned short n)
{
    return set->bits[n >> 5] & (1 << (n & 0x1f));
}

// FUNCTION: 0x48dc30
int __stdcall FUN_0048dc30(int id)
{
    Class_00488d30* set = FUN_00488c50("CTRL_F");
    Player_0048dc30* p = &g_game->players[g_game->localPlayer];
    Unit_0048dc30* u = p->unitsBegin;
    Unit_0048dc30* end = p->unitsEnd;
    if (u > end)
        return 0;
    while (u <= end) {
        if ((u->flags & 0x20) && u->field_104 == 0.0f && u->field_fb == 0
            && (u->owner == 0 || (u->owner->flags & 0x40000000))
            && u->field_ac == id && TestBit(set, u->type))
            return 1;
        u++;
    }
    return 0;
}
